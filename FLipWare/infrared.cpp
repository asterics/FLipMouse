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
#define RAW_BUFFER_LENGTH IR_EDGE_REC_MAX
#include <IRremote.hpp>
/**
   static variables for infrared code generation and timekeeping
 * */
uint32_t edge_timeout = 10000UL;  // timeout for IR code edge length in microseconds
int repeatCounter;        // number of desired code repetitions (-1 for endless)
int idlesequenceCounter;  // bumber of gaps which should be inserted before code repetition
uint16_t edges;           // number of edges for current code
uint16_t act_edge;        // current edge

//uint8_t output_state;     // PWM duty cycle

// array for edge times (time difference between the IR code edges)
//uint16_t timings [IR_EDGE_REC_MAX];

//IRrecv irrecv(IR_SENSOR_PIN);

// Timer for edge time management
//IntervalTimer playTimer;
//TODO: some PIO based library?

/**
   forward declarations of module-internal functions
*/
void start_IR_command_playback(char * name);

/**
   Initialize GPIO pin for IR receive/transmit,
   Set Carrier Frequency for PWM
 * */
void initIR() {
	//init library
	IrSender.begin(IR_LED_PIN); 
	IrReceiver.begin(IR_SENSOR_PIN, DISABLE_LED_FEEDBACK);
  /*pinMode(IR_SENSOR_PIN, INPUT);
  analogWriteFrequency(IR_LED_PIN, 38000);  // TBD: flexible carrier frequency for IR, not only 38kHz !
  pinMode(IR_LED_PIN, OUTPUT);
  digitalWrite(IR_LED_PIN, LOW);*/
}

/**
   Record an infrared remote command with a given name.

   This method records one infrared command to the EEPROM
   memory. It blocks either until a command is received or
   the user defined timeout (IR_USER_TIMEOUT_MS) occurs.
 * */
void record_IR_command(char * name)
{
	uint32_t startTime = millis();
	IrReceiver.start();
	while(abs((long int)(millis()-startTime)) < (edge_timeout / 1000))
	{
		if(IrReceiver.decode())
		{
			if(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW)
			{
				Serial.println("IR-Code sequence full.");
				IrReceiver.stop();
				return;
			}
			
			//save the recorded command to the EEPROM storage
			saveIRToEEPROM(name, IrReceiver.decodedIRData.rawDataPtr->rawbuf, IrReceiver.decodedIRData.rawDataPtr->rawlen);
			
			#ifdef DEBUG_OUTPUT_FULL
			  Serial.println("START IR ----------");
			  IrReceiver.printIRResultRawFormatted(&Serial, false); 
			  Serial.println("END ----------");
			#endif
			
			IrReceiver.stop();
			return;
		} else {
			delay(10);
		}
	}
	Serial.println("IR_TIMEOUT: User timeout");
	IrReceiver.stop();
	return;
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
/*
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
* */


/**
   @name start_IR_command_playback
   @brief copies the edge timing information of a stored IR command of given name
          from EEPROM to timings array in RAM and starts playback
   @param name name of the IR command to be played
   @return none
*/
void start_IR_command_playback(char * name)
{
	size_t edges;
	IRData raw;

	//fetch the IR command from the eeprom
	edges = readIRFromEEPROM(name, raw.rawDataPtr->rawbuf, IR_EDGE_REC_MAX);
	raw.rawDataPtr->rawlen = edges;
	
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
  IrSender.write(&raw);
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
  //not available in library
  return;
}
