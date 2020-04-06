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

//Time until the record command will be canceled (in milliseconds)
#define IR_USER_TIMEOUT_MS 10000

// ir code repeat gap (in microseconds)
#define IR_REPEAT_GAP 10000

// Maximum count of uint16_t edges for one IR-command
#define IR_EDGE_REC_MAX 250

// minimum count of signal edges which are necessary to accept a command
#define IR_EDGE_REC_MIN 5

// name of the stop code command
#define IDLESEQUENCE_REPEAT 1

// maximum time interval which can be stored in high precision (microseconds) format
#define MAX_HIGHPRECISION_DURATION 64000

extern uint8_t IR_SENSOR_PIN;
extern uint8_t IR_LED_PIN;
extern uint8_t DebugOutput;

uint32_t edge_timeout = 10000UL;  // timeout for IR code edge length in microseconds
uint32_t state_time;
int repeatCounter;
int idlesequenceCounter;

//current edge count
uint16_t edges;
uint16_t act_edge;
uint8_t output_state;

//array of the time difference between the edges
uint16_t timings [IR_EDGE_REC_MAX];

IntervalTimer playTimer;

void start_IR_command_playback(char * name);


/**
 * Record an infrared remote command with a given name.
 * 
 * This method records one infrared command to the EEPROM
 * memory. It blocks either until a command is received or
 * the user defined timeout (IR_USER_TIMEOUT_MS) occurs.
 * */
void record_IR_command(char * name)
{
  uint32_t prev;
	uint32_t duration;
	uint8_t toggle = 0;
  
	// fetch current time as previous value the first time
	prev = millis(); 
	while(digitalRead(IR_SENSOR_PIN))	//wait for start bit or user timeout of 10s
	{	
		duration = millis() - prev; //calculate the difference
			
		//cancel recording if takes longer than the user timeout
		if(duration >= IR_USER_TIMEOUT_MS)
		{
			Serial.println("IR_TIMEOUT: User timeout");
			return;
		} 
	}
		
	//record for a maximum of IR_EDGE_REC_MAX edges
	for(edges=0; edges<IR_EDGE_REC_MAX; edges++)
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

  if (edges==IR_EDGE_REC_MAX)
    Serial.println("IR-Code sequence full.");
  else Serial.println("IR-Code timeout reached.");

	//play a feedback tone
	makeTone(TONE_IR_REC,0);
	
	//full edge feedback, if full debug is enabled
	if(DebugOutput == DEBUG_FULLOUTPUT)
	{
		Serial.println("START IR ----------");
		for(uint8_t i = 0; i<edges; i++)
			Serial.println(timings[i]);
		Serial.println("END ----------");
	}
	
	//return the recorded command name and the edge count
	Serial.print("IR: recorded command ");
	Serial.print(name);
	Serial.print(" with ");
	Serial.print(edges);
  Serial.println(" edge times.");

  //save the recorded command to the EEPROM storage
  saveIRToEEPROM(name,timings,edges);

}

/**
 * list all available (stored) IR commands, also parsed by the GUI
 * */
void list_IR_commands() 
{ 
	listIRCommands();
}

/**
 * Delete one IR command identified by a given name.
 * If you want to clear all IR commands, use '\0' as name
 * */
void delete_IR_command(char * name)
{
	deleteIRCommand(name);
}

void generate_next_IR_phase(void)
{

//  if (state_time)  Serial.println(micros()-state_time);  // show last timing
//  state_time=micros();

  if (act_edge>edges) {            // one code repetition finished
    analogWrite(IR_LED_PIN, 0);   
    digitalWrite(IR_LED_PIN, LOW);  
    output_state=0;
    act_edge=0;
    if (repeatCounter > 0) repeatCounter--;  
    if (repeatCounter==0) { 
      playTimer.end();  // stop time if last repetition done 
                        // note: repeatCounter is -1 for hold mode
      if (idlesequenceCounter > 0) {
        idlesequenceCounter--;
        delayMicroseconds(IR_REPEAT_GAP);  // pause before next idlesequence (TBD: make that non-blocking)
        start_IR_command_playback(settings.ii);  // in case the idlesequence command exists: play it!
      }
    }
  }
  else {
    if (act_edge == edges)
       playTimer.update(IR_REPEAT_GAP);  // gap between code repetitions 
    else {
      uint32_t duration=timings[act_edge];
      if (duration > MAX_HIGHPRECISION_DURATION)  // timing in milliseconds
         duration= (duration-MAX_HIGHPRECISION_DURATION) * 1000;  // switch to microseconds
      playTimer.update(duration);  // set interval for current on/off phase 
    }

    analogWrite(IR_LED_PIN, output_state);
    if(output_state == 0) output_state=128;
    else output_state=0;

    act_edge++;   // increase edge index for next interrupt 
  }
}


/**
 * Play a previously recorded infrared remote command with a given name.
 *
 * */
void start_IR_command_playback(char * name)
{
	uint32_t edge_now = 0;
	uint32_t edge_prev = 0;	
	uint32_t duration = 0;
	uint8_t i;

	//fetch the IR command from the eeprom
	edges = readIRFromEEPROM(name,timings,IR_EDGE_REC_MAX);
	
	//no edges, no command -> cancel
	if(edges == 0)
	{
		if(DebugOutput == DEBUG_FULLOUTPUT) Serial.println("No IR command found");
		return;
	}
	
	//full edge feedback, if full debug is enabled
	if(DebugOutput == DEBUG_FULLOUTPUT)
	{
		Serial.println("START IR ----------");
		for(uint16_t i = 0; i<edges; i++)
		{
			Serial.println(timings[i]);
		}
		Serial.println("END ----------");
	}

  makeTone(TONE_IR,0);
  act_edge=0;
  output_state = 0;
  state_time = 0;
  playTimer.begin(generate_next_IR_phase,10);  // the first timing is just a dummy value  
  playTimer.priority(20);                      // quite high priority for our timer
}

void play_IR_command(char * name)
{
  repeatCounter=1; idlesequenceCounter=IDLESEQUENCE_REPEAT;
  start_IR_command_playback(name);
}

void hold_IR_command(char * name)
{
  repeatCounter=-1; idlesequenceCounter=IDLESEQUENCE_REPEAT;
  start_IR_command_playback(name);
}

void stop_IR_command()
{
   repeatCounter=0; 
}

void wipe_IR_commands()
{
   deleteIRCommand(0);
}

void set_IR_timeout(uint16_t tout_ms)
{
	if(tout_ms < 1) return;
   edge_timeout = (uint32_t)tout_ms * 1000;
}

