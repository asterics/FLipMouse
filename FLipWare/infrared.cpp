/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: infrared.cpp - implementation of the IR command support

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "FlipWare.h"
#include "infrared.h"
#include "tone.h"

/**
   static variables for infrared code generation and timekeeping
 * */
uint32_t edge_timeout = 10000UL;  // timeout for IR code edge length in microseconds
int repeatCounter;        // number of desired code repetitions (-1 for endless)
int idlesequenceCounter;  // bumber of gaps which should be inserted before code repetition
uint16_t edges;           // number of edges for current code
uint16_t act_edge;        // current edge
uint8_t output_state;     // PWM duty cycle

// array for edge times (time difference between the IR code edges)
uint16_t timings [IR_EDGE_REC_MAX];

// Timer for edge time management
IntervalTimer playTimer;

/**
   forward declarations of module-internal functions
*/
void start_IR_command_playback(char * name);

/**
   Initialize GPIO pin for IR receive/transmit,
   Set Carrier Frequency for PWM
 * */
void initIR() {
  pinMode(IR_SENSOR_PIN, INPUT);
  analogWriteFrequency(IR_LED_PIN, 38000);  // TBD: flexible carrier frequency for IR, not only 38kHz !
  pinMode(IR_LED_PIN, OUTPUT);
  digitalWrite(IR_LED_PIN, LOW);
}

/**
   Record an infrared remote command with a given name.

   This method records one infrared command to the EEPROM
   memory. It blocks either until a command is received or
   the user defined timeout (IR_USER_TIMEOUT_MS) occurs.
 * */
void record_IR_command(char * name)
{
  uint32_t prev;
  uint32_t duration;
  uint8_t toggle = 0;

  // fetch current time as previous value the first time
  prev = millis();
  while (digitalRead(IR_SENSOR_PIN))	//wait for start bit or user timeout of 10s
  {
    duration = millis() - prev; //calculate the difference

    //cancel recording if takes longer than the user timeout
    if (duration >= IR_USER_TIMEOUT_MS)
    {
      Serial.println("IR_TIMEOUT: User timeout");
      return;
    }
  }

  //record for a maximum of IR_EDGE_REC_MAX edges
  for (edges = 0; edges < IR_EDGE_REC_MAX; edges++)
  {
    prev = micros(); // current timestamp in microseconds
    do
    {
      duration = micros() - prev;
    } while ((duration < edge_timeout) && (digitalRead(IR_SENSOR_PIN) == toggle));

    if (duration >= edge_timeout) break;

    if (duration > MAX_HIGHPRECISION_DURATION) {   // switch to milliseconds
      duration /= 1000;
      duration += MAX_HIGHPRECISION_DURATION;
    }
    timings[edges] = (uint16_t)duration;
    toggle = !toggle;  // for next edge detection
  }

  if (edges == IR_EDGE_REC_MAX)
    Serial.println("IR-Code sequence full.");
  else Serial.println("IR-Code timeout reached.");

  //play a feedback tone
  makeTone(TONE_IR_REC, 0);

  //full edge feedback, if full debug is enabled
#ifdef DEBUG_OUTPUT_FULL
  Serial.println("START IR ----------");
  for (uint8_t i = 0; i < edges; i++)
    Serial.println(timings[i]);
  Serial.println("END ----------");
#endif

  //return the recorded command name and the edge count
  Serial.print("IR: recorded command ");
  Serial.print(name);
  Serial.print(" with ");
  Serial.print(edges);
  Serial.println(" edge times.");

  //save the recorded command to the EEPROM storage
  saveIRToEEPROM(name, timings, edges);

}

/**
   list all available (stored) IR commands, also parsed by the GUI
 * */
void list_IR_commands()
{
  listIRCommands();
}

/**
   Delete one IR command identified by a given name.
   If you want to clear all IR commands, use '\0' as name
   returns 1 is successful, 0 otherwise
 * */
uint8_t delete_IR_command(char * name)
{
  return deleteIRCommand(name);
}

/**
   @name generate_next_IR_phase
   @brief generates the current timings for PWM or NON-PWM phases, using a timer 
          updates the stored edge time for the next edge timing
          repeats a whole code playback (repeatCounter times)
   @return none
*/
void generate_next_IR_phase(void)
{
  if (act_edge > edges) {          // one code repetition finished
    analogWrite(IR_LED_PIN, 0);
    digitalWrite(IR_LED_PIN, LOW);
    output_state = 0;
    act_edge = 0;
    if (repeatCounter > 0) repeatCounter--;
    if (repeatCounter == 0) {
      playTimer.end();  // stop time if last repetition done
      // note: repeatCounter is -1 for hold mode
      if (idlesequenceCounter > 0) {
        idlesequenceCounter--;
        delayMicroseconds(IR_REPEAT_GAP);  // pause before next idlesequence (TBD: make that non-blocking)
        start_IR_command_playback(IDLESEQUENCE_NAME);  // in case the idlesequence command exists: play it!
      }
    }
  }
  else {
    if (act_edge == edges)
      playTimer.update(IR_REPEAT_GAP);  // gap between code repetitions
    else {
      uint32_t duration = timings[act_edge];
      if (duration > MAX_HIGHPRECISION_DURATION)  // timing in milliseconds
        duration = (duration - MAX_HIGHPRECISION_DURATION) * 1000; // switch to microseconds
      playTimer.update(duration);  // set interval for current on/off phase
    }

    analogWrite(IR_LED_PIN, output_state);
    if (output_state == 0) output_state = 128;
    else output_state = 0;

    act_edge++;   // increase edge index for next interrupt
  }
}


/**
   @name start_IR_command_playback
   @brief copies the edge timing information of a stored IR command of given name
          from EEPROM to timings array in RAM and starts playback
   @param name name of the IR command to be played
   @return none
*/
void start_IR_command_playback(char * name)
{
  uint32_t edge_now = 0;
  uint32_t edge_prev = 0;
  uint32_t duration = 0;
  uint8_t i;

  //fetch the IR command from the eeprom
  edges = readIRFromEEPROM(name, timings, IR_EDGE_REC_MAX);

  //no edges, no command -> cancel
  if (edges == 0)
  {
#ifdef DEBUG_OUTPUT_FULL
    Serial.println("No IR command found");
#endif
    return;
  }

  //full edge feedback, if full debug is enabled
#ifdef DEBUG_OUTPUT_FULL
  Serial.println("START IR ----------");
  for (uint16_t i = 0; i < edges; i++)
  {
    Serial.println(timings[i]);
  }
  Serial.println("END ----------");
#endif

  makeTone(TONE_IR, 0);
  act_edge = 0;
  output_state = 0;
  playTimer.begin(generate_next_IR_phase, 10); // the first timing is just a dummy value
  playTimer.priority(20);                      // quite high priority for our timer
}

void play_IR_command(char * name)
{
  repeatCounter = 1; idlesequenceCounter = IDLESEQUENCE_REPEAT;
  start_IR_command_playback(name);
}

void hold_IR_command(char * name)
{
  repeatCounter = -1; idlesequenceCounter = IDLESEQUENCE_REPEAT;
  start_IR_command_playback(name);
}

void stop_IR_command()
{
  repeatCounter = 0;
}

void wipe_IR_commands()
{
  deleteIRCommand(0);
}

void set_IR_timeout(uint16_t tout_ms)
{
  if (tout_ms < 1) return;
  edge_timeout = (uint32_t)tout_ms * 1000;
}
