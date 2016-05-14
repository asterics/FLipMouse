#include "FlipWare.h"

#define IR_USER_TIMEOUT_MS 10000
#define IR_EDGE_TIMEOUT_US 10000
#define IR_EDGE_REC_MAX 70

extern uint8_t IR_SENSOR_PIN;
extern uint8_t IR_LED_PIN;

uint8_t edges;
uint8_t timings[70];

void record_IR_command()
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
			Serial.print("User timeout\n");
			wait = 0;
		}
		else if(!digitalRead(IR_SENSOR_PIN))
		{
			Serial.print("Start condition\n");
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
				Serial.print("Edge timeout\n");
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
		Serial.print(timings[i]);
		Serial.print("\n");
		toggle = !toggle;
	}
	Serial.print(edges);
	Serial.print("..........\n");
}

void play_IR_command()
{
	uint32_t edge_now = 0;
	uint32_t edge_prev = 0;	
	uint32_t burst_now = 0;
	uint32_t burst_prev = 0;
	uint32_t duration = 0;
	int test = 0;
	uint8_t i;
	uint8_t j;
	uint32_t state_time;
	boolean output_state = HIGH;
	boolean output = HIGH;
	uint32_t time1, time2;
	
	time1 = millis();
	for(i=0; i<edges; i++)
	{
		state_time = timings[i] * 37;
		edge_prev = micros();
		if(output_state == HIGH)
		{
			do
			{
				digitalWrite(IR_LED_PIN,output);
				output = !output;
				burst_prev = micros();
				
				for(j=0; j<= 74; j++)
				{
					__asm__("nop\n\t");
				}
				
				edge_now = micros();
				duration = edge_now - edge_prev;
			}
			while(duration <= state_time);
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
	
	Serial.print(time1);
	Serial.print("\n");
	digitalWrite(IR_LED_PIN,LOW);
}