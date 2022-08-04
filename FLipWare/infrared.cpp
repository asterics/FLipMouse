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
uint32_t edge_timeout = 10000UL;  // timeout for IR receiving, [ms]
volatile int repeatCounter;        // number of desired code repetitions (-1 for endless)
volatile int idlesequenceCounter;  // bumber of gaps which should be inserted before code repetition
volatile uint16_t edges;           // number of edges for current code
volatile uint16_t act_edge;        // current edge

//RP2040:
alarm_id_t ir_alarm_id = -1; //alarm id for replaying
alarm_pool_t *ir_alarm_pool = nullptr; //alarm pool for replaying
// Keep std::map safe for multicore use
auto_init_mutex(_irMutex);

volatile uint8_t output_state;     // PWM duty cycle

// array for edge times (time difference between the IR code edges)
volatile uint16_t timings [IR_EDGE_REC_MAX];

//Teensy:
// Timer for edge time management
//IntervalTimer playTimer;

/**
   forward declarations of module-internal functions
*/
void start_IR_command_playback(char * name);

/**
   Initialize GPIO pin for IR receive/transmit,
   Set Carrier Frequency for PWM
 * */
void initIR() {
  //RP2040: enable default alarm pool, used for IR edge playback.
  //alarm_pool_init_default();
  //RP2040: create a new alarm pool, the default one is already crowded (tone & analogWrite?)
  //Note/Todo: figure out why the alarm pool fills up and needs to be this big,
  //even if we just use one alarm.
  ir_alarm_pool = alarm_pool_create(2, 64); 

  //GPIO & PWM setup
  pinMode(IR_SENSOR_PIN, INPUT);
  //Teensy:
  //analogWriteFrequency(IR_LED_PIN, 38000);  // TBD: flexible carrier frequency for IR, not only 38kHz !
  //RP2040:
  analogWriteFreq(38000);
  analogWriteRange(255); //set to 8 bit
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
  //RP2040:
  // Ensure only 1 core can start or stop at a time
  //source: Tone.cpp
  CoreMutex m(&_irMutex);
  if (!m) {
    return;    // Weird deadlock case
  }
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
#ifdef DEBUG_OUTPUT_IR
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
  saveIRToEEPROM(name, (uint16_t *)timings, (uint16_t)edges);
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
//RP2040:
int64_t generate_next_IR_phase(alarm_id_t id, void *user_data)
//Teensy:
//void generate_next_IR_phase(void)
{
  //RP2040: return value determines alarm behaviour
  int64_t ret = 0; //return 0 -> no reschedule; <0 [us] from last timestamp; >0 [us] from this return.
  /* Disabled, IRQ context here!
   * #ifdef DEBUG_OUTPUT_IR
    Serial.print("id:");
    Serial.println(id);
  #endif*/
  if (act_edge > edges) {          // one code repetition finished
    analogWrite(IR_LED_PIN, 0);
    digitalWrite(IR_LED_PIN, LOW);
    output_state = 0;
    act_edge = 0;
    if (repeatCounter > 0) repeatCounter--;
    if (repeatCounter == 0) {
      ret = 0;  // stop time if last repetition done
      // note: repeatCounter is -1 for hold mode
      //TODO: rework this for RP2040!
      /*if (idlesequenceCounter > 0) {
        idlesequenceCounter--;
        delayMicroseconds(IR_REPEAT_GAP);  // pause before next idlesequence (TBD: make that non-blocking)
        start_IR_command_playback(IDLESEQUENCE_NAME);  // in case the idlesequence command exists: play it!
      }*/
    } else {
      //RP2040:
      //we need this alarm again
      ret = IR_REPEAT_GAP;
    }
  }
  else {
    if (act_edge == edges)
      //RP2040:
      ret = IR_REPEAT_GAP; // gap between code repetitions
      //teensy:
      //playTimer.update(IR_REPEAT_GAP);  // gap between code repetitions
    else {
      uint32_t duration = timings[act_edge];
      if (duration > MAX_HIGHPRECISION_DURATION)  // timing in milliseconds
        duration = (duration - MAX_HIGHPRECISION_DURATION) * 1000; // switch to microseconds
      //Teensy:
      //playTimer.update(duration);  // set interval for current on/off phase
      //RP2040:
      ret = duration;
    }

    analogWrite(IR_LED_PIN, output_state);
    if (output_state == 0) output_state = 128;
    else output_state = 0;

    act_edge++;   // increase edge index for next interrupt
  }
  
  //RP2040:
  /*if(ret != 0)
  {
    ir_alarm_id = alarm_pool_add_alarm_in_us(ir_alarm_pool, ret, generate_next_IR_phase, nullptr, true);
    if(ir_alarm_id == -1) Serial.println("IR: no alarm available (in generate)");
  }*/
  //alarm_pool_cancel_alarm(ir_alarm_pool,id); //we cancel ourself
  return ret;
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
  //RP2040:
  // Ensure only 1 core can start or stop at a time
  //source: Tone.cpp
  CoreMutex m(&_irMutex);
  if (!m) {
    return;    // Weird deadlock case
  }
    
  //fetch the IR command from the eeprom
  edges = readIRFromEEPROM(name, (uint16_t*)timings, IR_EDGE_REC_MAX);

  //no edges, no command -> cancel
  if (edges == 0)
  {
#ifdef DEBUG_OUTPUT_IR
    Serial.print("No IR command found: ");
    Serial.println(name);
#endif
    return;
  }

  //full edge feedback, if full debug is enabled
#ifdef DEBUG_OUTPUT_IR
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
  //RP2040:
  //add CB function as alarm, store alarm_id for reuse
  //ir_alarm_id = add_alarm_in_us(10, generate_next_IR_phase, nullptr, true);
  #ifdef DEBUG_OUTPUT_IR
    Serial.println("Starting alarm");
  #endif
  ir_alarm_id = alarm_pool_add_alarm_in_us(ir_alarm_pool, 25, generate_next_IR_phase, nullptr, true);
  if(ir_alarm_id == -1) Serial.println("IR: no alarm available!");
  //busy wait for finished IR
  while(act_edge < edges);
  
  //Teensy:
  //playTimer.begin(generate_next_IR_phase, 10); // the first timing is just a dummy value
  //playTimer.priority(20);                      // quite high priority for our timer
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
  edge_timeout = tout_ms;
  return;
}
