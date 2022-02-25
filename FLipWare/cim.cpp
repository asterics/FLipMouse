/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: cim.cpp - AsTeRICS LipMouse CIM Firmware using Teensy controller

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html


   AsTeRICS CIM Protocol Packet Frame:
   ===================================

 	Packet ID				2 bytes	"@T" (0x4054 )
 	ARE ID / CIM ID			2 bytes
	Data Size				2 bytes	0x0000-0x0800
	Packet serial number	1 byte	0x00-0x7f (0x80-0xff for event-replies from CIM )
	CIM-Feature address		2 bytes
	Request / Reply code	2 bytes
	-------------------------------------------> 11 bytes = minimum frame length
	Optional data			0-2048 bytes
	Optional CRC checksum	0 or 4 bytes CRC32
	-------------------------------------------> 2063 bytes = maximum frame length

*/


#include "FlipWare.h"
#include "gpio.h"
#include "cim.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

#define ARE_MINIMAL_VERSION 1


/**
   unique number for Lipmouse CIM in ARE
*/
const uint32_t LIPMOUSE_CIM_UNIQUE_NUMBER = 7;

/**
   static variables and structures for this module
*/
volatile uint16_t ADC_updatetime = 0;
unsigned long updateReportsTimestamp;

const char LIPMOUSE_CIM_FEATURELIST[] =
{
  0x00, 0x00, // unique number, data: 4 bytes
  0x01, 0x00, // Activate periodic ADC Reports, data: 2 bytes: period in milliseconds (0=off)
  0x02, 0x00, // ADC value report, data: 10 bytes (chn low / high byte for chn1-5)

} ;


extern  int8_t led_map[];              //  maps leds pins
extern uint8_t readstate;

volatile uint8_t CimParserActive = 0;
volatile uint8_t StandAloneMode = 1;
volatile uint8_t CimMode = 0; // generate CIM pakets from main loop ?
volatile uint8_t actMode = 0; // 0: standalone functions disabled, 1: standalond functions enabled

struct ARE_frame_t ARE_frame;
struct CIM_frame_t CIM_frame;

uint8_t buttonval;
unsigned int  datapos = 0;
uint8_t first_packet = 1;
uint8_t reports_running = 0;

void parseCommand (char * cmdstr);

void init_CIM_frame (void)
{
  CIM_frame.packet_id = CIM_FRAME_START; // '@','T': Packet-ID/sync bytes
  CIM_frame.cim_id = CIM_ID_LIPMOUSE;
}


uint8_t process_ARE_frame(uint8_t status_code)
{
  uint8_t ack_needed;
  uint8_t data_size = 0;
  uint8_t command;

  command = (uint8_t)ARE_frame.request_code;
  CIM_frame.cim_feature = ARE_frame.cim_feature;
  CIM_frame.serial_number = ARE_frame.serial_number;
  CIM_frame.reply_code = (((uint16_t)status_code) << 8) + command;
  data_size = (uint8_t)ARE_frame.data_size;

  ack_needed = 1;


  //	if ((status_code & (CIM_ERROR_INVALID_ARE_VERSION | CIM_ERROR_CRC_MISMATCH)) == 0)
  if ((status_code & CIM_ERROR_INVALID_ARE_VERSION) == 0)
  {
    // UART_Print(" feature ");  UART_Putchar(command);
    // no serious packet error
    switch (command)   {  // process requested command

      case CMD_REQUEST_FEATURELIST:
        if (data_size == 0) {
          reply_FeatureList();  // reply requested feature list
          ack_needed = 0;
        } else status_code |= CIM_ERROR_INVALID_FEATURE;
        break;

      case CMD_REQUEST_RESET_CIM:
        if (data_size != 0) status_code |= CIM_ERROR_INVALID_FEATURE;
        break;
      case CMD_REQUEST_START_CIM:
        if (data_size == 0) {
          updateReportsTimestamp = millis();
          StandAloneMode = actMode;
          CimMode = 1;
          // enable_timer_ISR();
        } else status_code |= CIM_ERROR_INVALID_FEATURE;
        break;
      case CMD_REQUEST_STOP_CIM:
        if (data_size == 0) {
          first_packet = 1; // reset first frame indicator etc.
          //disable_timer_ISR();
          StandAloneMode = 1;
          CimMode = 0;
        } else status_code |= CIM_ERROR_INVALID_FEATURE;
        break;

      case CMD_REQUEST_READ_FEATURE:  //  read feature from CIM

        switch (ARE_frame.cim_feature) {
          case LIPMOUSE_CIM_FEATURE_UNIQUENUMBER:   // read unique serial number
            if (data_size == 0) {
              reply_UniqueNumber();
              ack_needed = 0;
            } else status_code |= CIM_ERROR_INVALID_FEATURE;
            break;

          case LIPMOUSE_CIM_FEATURE_ADCREPORT:
            generate_ADCFrame();
            reply_DataFrame();
            ack_needed = 0;
            break;

          case LIPMOUSE_CIM_FEATURE_BUTTONREPORT:
            generate_ButtonFrame();
            reply_DataFrame();
            ack_needed = 0;
            break;
          default: 				// not a valid read  feature;
            status_code |= CIM_ERROR_INVALID_FEATURE;
        }
        break;


      case CMD_REQUEST_WRITE_FEATURE:  //  write feature to CIM

        switch (ARE_frame.cim_feature) {  // which feature address ?

          case LIPMOUSE_CIM_FEATURE_SET_ADCPERIOD:
            if (data_size == 2) {
              ADC_updatetime =  (uint16_t)ARE_frame.data[0];
              ADC_updatetime += ((uint16_t)ARE_frame.data[1]) << 8;
            }
            break;
          case LIPMOUSE_CIM_FEATURE_SET_LEDS:
            if (data_size == 1) {
              uint8_t actLeds = ARE_frame.data[0];
              if (actLeds & 1) digitalWrite (led_map[0], LOW); else digitalWrite (led_map[0], HIGH);
              if (actLeds & 2) digitalWrite (led_map[1], LOW); else digitalWrite (led_map[1], HIGH);
              if (actLeds & 4) digitalWrite (led_map[2], LOW); else digitalWrite (led_map[2], HIGH);
            }
            break;
          case LIPMOUSE_CIM_FEATURE_ATCMD:
            parseCommand((char *)workingmem);
            break;
          case LIPMOUSE_CIM_FEATURE_SET_MODE:
            if (data_size == 1) {
              actMode = ARE_frame.data[0];
              StandAloneMode = actMode;
            }
            break;
          default:         // not a valid write  feature;
            status_code |= CIM_ERROR_INVALID_FEATURE;
        }
    }
  }

  if (status_code & CIM_ERROR_INVALID_FEATURE)   {  // invalid data size or feature
    //	LEDs_ToggleLEDs(LED5);  // indicate wrong feature
    //	UART_Print(" invalid data size or no feature ");
  }


  if (ack_needed) {
    reply_Acknowledge();
  }
  return (1);
}



void reply_FeatureList(void)
{
  Serial.write ((uint8_t *) &CIM_frame, 7);    // attention : byte alignment in struct not possible for Cortex M0 !
  Serial.write ((uint8_t *) & (CIM_frame.cim_feature), 4);
  Serial.write  ( (uint8_t *)&LIPMOUSE_CIM_FEATURELIST, CIM_frame.data_size);
}

void reply_UniqueNumber(void)
{
  CIM_frame.data_size = 4;  // lenght of unique number
  Serial.write ((uint8_t *) &CIM_frame, 7);    // attention : byte alignment in struct not possible for Cortex M0 !
  Serial.write ((uint8_t *) & (CIM_frame.cim_feature), 4);
  Serial.write ((uint8_t *) &LIPMOUSE_CIM_UNIQUE_NUMBER, CIM_frame.data_size);
}

void reply_Acknowledge(void)
{
  CIM_frame.data_size = 0;   // no data in ack frame
  Serial.write ((uint8_t *) &CIM_frame, 7);    // attention : byte alignment in struct not possible for Cortex M0 !
  Serial.write ((uint8_t *) & (CIM_frame.cim_feature), 4);
}

void reply_DataFrame(void)
{
  Serial.write ((uint8_t *) &CIM_frame, 7);    // attention : byte alignment in struct not possible for Cortex M0 !
  Serial.write ((uint8_t *) & (CIM_frame.cim_feature), 4);
  Serial.write ((uint8_t *) CIM_frame.data, CIM_frame.data_size);
}


void generate_ADCFrame()
{
  uint16_t adcval;

  adcval = sensorData.left - sensorData.right;
  CIM_frame.data[0] = adcval & 0xff;
  CIM_frame.data[1] = adcval >> 8;

  adcval = sensorData.up - sensorData.down;
  CIM_frame.data[2] = adcval & 0xff;
  CIM_frame.data[3] = adcval >> 8;

  adcval = sensorData.pressure;
  CIM_frame.data[4] = adcval & 0xff;
  CIM_frame.data[5] = adcval >> 8;

  CIM_frame.data_size = 6;
}



uint8_t update_Buttonval()
{
  uint8_t actval = 0;

  if (digitalRead(input_map[0]) == LOW) actval |= 1;
  if (digitalRead(input_map[1]) == LOW) actval |= 2;
  if (digitalRead(input_map[2]) == LOW) actval |= 4;

  if (actval != buttonval)
  {
    buttonval = actval;
    return (1);
  }
  else return (0);
}

void generate_ButtonFrame()
{
  update_Buttonval();

  CIM_frame.data[0] = buttonval;
  CIM_frame.data_size = 1;
}

#define FRAME_DONE 99


void parse_CIM_protocol(int actbyte)
{
  uint32_t checksum = 0;
  static uint8_t transmission_mode;
  static uint8_t reply_status_code;
  static uint8_t last_serial;

  switch (readstate)
  {
    case 0: // first sync byte
      if (actbyte == '@') {
        readstate++;
      }
      if ((actbyte == 'A') || (actbyte == 'a'))
      {
        readstate++;  // switch to normal AT-command mode
        CimParserActive = 0;
        StandAloneMode = 1;
      }
      break;
    case 1: // second sync byte
      if (actbyte == 'T')  readstate++;
      else readstate = 0;
      break;

    // packet in sync !

    case 2: // ARE-ID: SW-version low byte
      reply_status_code = 0;
      ARE_frame.are_id = actbyte;
      readstate++;
      break;
    case 3: // ARE-ID: SW-version high byte
      ARE_frame.are_id += ((uint16_t)actbyte) << 8;
      if (ARE_frame.are_id < ARE_MINIMAL_VERSION)    // outdated ARE ?
        reply_status_code |= CIM_ERROR_INVALID_ARE_VERSION;
      readstate++;
      break;
    case 4: // data length low byte
      ARE_frame.data_size = actbyte;
      readstate++;
      break;
    case 5: // data length high byte
      ARE_frame.data_size += ((uint16_t)actbyte) << 8;
      if (ARE_frame.data_size > DATABUF_SIZE)  { // dismiss packets of excessive length
        readstate = 0;
        //ARE_frame.data_size=0;
        //reply_status_code |= CIM_ERROR_INVALID_FEATURE;
      }
      else readstate++;
      break;
    case 6: // serial_number
      ARE_frame.serial_number = actbyte;
      if (first_packet)    // don't check first serial
        first_packet = 0;
      else if (actbyte != (last_serial + 1) % 0x80) // check current serial number
        reply_status_code |= CIM_ERROR_LOST_PACKETS;

      last_serial = actbyte;
      readstate++;
      break;
    case 7: // CIM-feature low byte
      ARE_frame.cim_feature = actbyte;
      readstate++;
      break;
    case 8: // CIM-feature high byte
      ARE_frame.cim_feature += ((int)actbyte) << 8;
      readstate++;
      break;
    case 9: // Request code low byte ( command )
      ARE_frame.request_code = actbyte;
      readstate++;
      break;
    case 10:// Request code high byte (transmission mode)
      transmission_mode = actbyte; // bit 0: CRC enable
      // reply_status_code|=(actbyte & CIM_STATUS_CRC);
      // remember CRC state for reply

      if (ARE_frame.data_size > 0) {
        readstate++;
        datapos = 0;
      }
      else {  // no data in packet
        if (transmission_mode & CIM_STATUS_CRC)
          readstate += 2;    // proceed with CRC
        else readstate = FRAME_DONE; // frame is finished here !
      }
      break;

    case 11: // read out data

      if ( (ARE_frame.cim_feature == LIPMOUSE_CIM_FEATURE_ATCMD )
           && (datapos < WORKINGMEM_SIZE - 4)) { // receive AT-Command
        workingmem[datapos] = actbyte;
        workingmem[datapos + 1] = 0;
      }
      else ARE_frame.data[datapos] = actbyte; // receive normal CIM packet

      datapos++;
      if (datapos == ARE_frame.data_size)  {
        if (transmission_mode & CIM_STATUS_CRC)  // with CRC: get checksum
          readstate++;
        else readstate = FRAME_DONE; // no CRC:  frame is finished here !
      }
      break;
    case 12: // checksum byte 1
      checksum = actbyte;
      readstate++;
      break;
    case 13: // checksum byte 2
      checksum += ((long)actbyte) << 8;
      readstate++;
      break;
    case 14: // checksum byte 3
      checksum += ((long)actbyte) << 16;
      readstate++;
      break;
    case 15: // checksum byte 4
      checksum += ((long)actbyte) << 24;

      // check CRC now (currently not used):
      // if (checksum != crc32(ARE_frame.data, ARE_frame.data_size))
      reply_status_code |= CIM_ERROR_CRC_MISMATCH;

      // frame finished here !
      readstate = FRAME_DONE;
      break;
    default: readstate = 0; break;
  }

  if (readstate == FRAME_DONE)  { // frame finished: store command in ringbuffer
    process_ARE_frame(reply_status_code);
    readstate = 0;
  }
}

extern struct CIM_frame_t CIM_frame;
extern uint8_t buttonval;     // actual button states (defined in CimProtocol.c>
uint8_t autoreply_num = 0x80; // sequential number for automatic replies, 0x80-0xff


void handleCimMode(void)
{
  if ( (ADC_updatetime > 0) && (millis() >= updateReportsTimestamp + ADC_updatetime))
  {
    updateReportsTimestamp = millis();
    autoreply_num++;
    if (autoreply_num == 0) autoreply_num = 0x80;

    CIM_frame.cim_feature = LIPMOUSE_CIM_FEATURE_ADCREPORT;
    CIM_frame.serial_number = autoreply_num;
    CIM_frame.reply_code = CMD_EVENT_REPLY;
    generate_ADCFrame();
    reply_DataFrame();

    if (update_Buttonval())  // if buttonstate has changed
    {
      autoreply_num++;
      if (autoreply_num == 0) autoreply_num = 0x80;

      CIM_frame.cim_feature = LIPMOUSE_CIM_FEATURE_BUTTONREPORT;
      CIM_frame.serial_number = autoreply_num;
      CIM_frame.reply_code = CMD_EVENT_REPLY;
      generate_ButtonFrame();  // send new buttonframe
      reply_DataFrame();
    }
  }
}
