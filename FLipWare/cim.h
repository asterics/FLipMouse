/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: cim.h - header file for AsTeRICS LipMouse CIM Firmware using Teensy controller

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html


   AsTeRICS CIM Protocol Packet Frame:
   ===================================

   Packet ID       2 bytes "@T" (0x4054 )
  ARE ID / CIM ID     2 bytes
  Data Size       2 bytes 0x0000-0x0800
  Packet serial number  1 byte  0x00-0x7f (0x80-0xff for event-replies from CIM )
  CIM-Feature address   2 bytes
  Request / Reply code  2 bytes
  -------------------------------------------> 11 bytes = minimum frame length
  Optional data     0-2048 bytes
  Optional CRC checksum 0 or 4 bytes CRC32
  -------------------------------------------> 2063 bytes = maximum frame length

*/

#ifndef _CIMPROTOCOL_H_
#define _CIMPROTOCOL_H_

#include <inttypes.h>

#define CIM_HEADER_LEN      11
#define CIM_FRAME_START     0x5440  // little endian
#define CIM_ID_LIPMOUSE      0xa401

#define CMD_REQUEST_FEATURELIST   0x00
#define CMD_REPLY_FEATURELIST     0x00
#define CMD_REQUEST_WRITE_FEATURE 0x10
#define CMD_REPLY_WRITE_FEATURE   0x10
#define CMD_REQUEST_READ_FEATURE  0x11
#define CMD_REPLY_READ_FEATURE    0x11
#define CMD_EVENT_REPLY			  0x20
#define CMD_REQUEST_RESET_CIM 	  0x80
#define CMD_REPLY_RESET_CIM       0x80
#define CMD_REQUEST_START_CIM 	  0x81
#define CMD_REPLY_START_CIM       0x81
#define CMD_REQUEST_STOP_CIM 	  0x82
#define CMD_REPLY_STOP_CIM 	      0x82

#define CIM_STATUS_CRC      		  (1<<0)
#define CIM_ERROR_LOST_PACKETS        (1<<1)
#define CIM_ERROR_CRC_MISMATCH        (1<<2)
#define CIM_ERROR_INVALID_FEATURE     (1<<3)
#define CIM_ERROR_INVALID_ARE_VERSION (1<<4)
#define CIM_ERROR_CIM_NOT_READY       (1<<5)
#define CIM_ERROR_CIM_OTHER_ERROR     (1<<7)

#define LIPMOUSE_CIM_FEATURE_UNIQUENUMBER    0x00
#define LIPMOUSE_CIM_FEATURE_SET_ADCPERIOD 	 0x01
#define LIPMOUSE_CIM_FEATURE_ADCREPORT 	     0x02
#define LIPMOUSE_CIM_FEATURE_BUTTONREPORT 	 0x03
#define LIPMOUSE_CIM_FEATURE_SET_LEDS 		 0x04
#define LIPMOUSE_CIM_FEATURE_ATCMD      0x05
#define LIPMOUSE_CIM_FEATURE_SET_MODE      0x06


#define DATABUF_SIZE 15       // max. 15 bytes needed for data field


struct ARE_frame_t {
  uint16_t packet_id;
  uint16_t are_id;
  uint16_t data_size;
  uint8_t  serial_number;
  uint16_t cim_feature;
  uint16_t request_code;
  uint8_t  data[DATABUF_SIZE];
  uint32_t crc;
};

struct CIM_frame_t {
  uint16_t packet_id;
  uint16_t cim_id;
  uint16_t data_size;
  uint8_t  serial_number;
  uint16_t cim_feature;
  uint16_t reply_code;
  uint8_t  data[DATABUF_SIZE];
  uint32_t crc;
};



void init_CIM_frame (void);
void parse_CIM_protocol(int);

uint16_t adcval;
uint16_t  ADC_Read(uint16_t);
uint16_t sendCont;

uint8_t update_Buttonval();

void reply_FeatureList(void);
void reply_UniqueNumber(void);
void reply_Acknowledge(void);
void reply_DataFrame(void);

void generate_ADCFrame(void);
void generate_ButtonFrame(void);

void ADC_Init(void);
uint16_t ADC_Read(uint16_t channel );

#endif
