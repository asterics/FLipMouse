#include "FlipWare.h"

//Time until the record command will be canceled
#define IR_USER_TIMEOUT_MS 10000
//Timeout for the infrared command itself
#define IR_EDGE_TIMEOUT_US 10000
//Maximum count of edges for one command
//Note: this value may be increased if your recorded command exceeds this value
#define IR_EDGE_REC_MAX 70

extern uint8_t IR_SENSOR_PIN;
extern uint8_t IR_LED_PIN;
extern uint8_t DebugOutput;

uint8_t edges;

uint8_t timings[IR_EDGE_REC_MAX];

/**
 * Record an infrared remote command with a given name.
 * 
 * This method records one infrared command to the EEPROM
 * memory. It blocks either until a command is received or
 * the user defined timeout (IR_USER_TIMEOUT_MS) occurs.
 * */
void record_IR_command(char * name)
{
	unsigned long now = 0;
	unsigned long prev = 0;
	
	int duration;
	int i;
	int toggle = 1;
	int wait = 1;
	
	prev = millis();
	while(wait)	//wait for start bit or user timeout of 10s
	{	
		now = millis();
		duration = now - prev;
		if(duration >= IR_USER_TIMEOUT_MS) 
		{
			Serial.println(F("IR_TIMEOUT: User timeout"));
			return;
		}
		else if(!digitalRead(IR_SENSOR_PIN))
		{
			if(DebugOutput == DEBUG_FULLOUTPUT) { Serial.println(F("IR: Start condition")); }
			wait = 0;
		}
	}
	
	for(i=0; i<IR_EDGE_REC_MAX; i++)
	{
		prev = micros();
		wait = 1;
		while(wait)	
		{
			now = micros();
			duration = now - prev;
			if(duration >= IR_EDGE_TIMEOUT_US) 
			{
				if(DebugOutput == DEBUG_FULLOUTPUT) { Serial.println(F("IR_TIMEOUT: Edge timeout")); }
				wait = 0;
				edges = i;
				i = IR_EDGE_REC_MAX;
			}
			else if(digitalRead(IR_SENSOR_PIN) == toggle)
			{
				wait = 0;
			}
		}	
		timings[i] = (now - prev) / 37;
		toggle = !toggle;
	}
	
	//save the recorded command to the EEPROM storage
	saveIRToEEPROM(name,timings,edges);
	
	//return the recorded command name and the edge count
	Serial.print(F("IR: recorded "));
	Serial.print(name);
	Serial.print("/");
	Serial.println(edges);
	
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

/**
 * Play a previously recorded infrared remote command with a given name.
 *
 * */
void play_IR_command(char * name)
{
	uint32_t edge_now = 0;
	uint32_t edge_prev = 0;	
	uint32_t duration = 0;
	uint8_t i;
	uint8_t j;
	uint32_t state_time;
	boolean output_state = HIGH;
	boolean output = HIGH;
	uint32_t time1, time2;
	
	time1 = millis();
	
	edges = readIRFromEEPROM(name,timings,IR_EDGE_REC_MAX);
	
	if(edges == 0)
	{
		if(DebugOutput == DEBUG_FULLOUTPUT) Serial.println(F("No IR command found"));
		return;
	}
	
	for(i=0; i<edges; i++)
	{
		state_time = timings[i] * 37;
		edge_prev = micros();
		if(output_state == HIGH)
		{
			analogWrite(IR_LED_PIN, 128);	//activate burst (PWM with 50% duty cycle)
			do
			{
				edge_now = micros();
				duration = edge_now - edge_prev;
			}
			while(duration <= state_time);
			analogWrite(IR_LED_PIN, 0);		//deactivate PWM
			output_state = LOW;
		}
		else
		{
			digitalWrite(IR_LED_PIN,output_state);
			do
			{
				edge_now = micros();
				duration = edge_now - edge_prev;
			}
			while(duration <= state_time);
			output_state = HIGH;
		}

	}
	time2 = millis();
	time1 = time2 - time1;	
	
	if(DebugOutput == DEBUG_FULLOUTPUT) 
	{
		Serial.print(F("IR: play "));
		Serial.print(name);
		Serial.print("/");
		Serial.println(IRName);
	}
	digitalWrite(IR_LED_PIN,LOW);	//infrared LED must be turned of after this function
}
