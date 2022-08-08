/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: tone.h - functions for tone/audio feedback 

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _TONE_H_
#define _TONE_H_


/**
   constant definitions for tone generation 
*/ 
#define TONE_PIN  25    // physical pin for piezo tone generation

#define TONE_CALIB            1
#define TONE_CHANGESLOT       2
#define TONE_ENTER_STRONGSIP  3
#define TONE_EXIT_STRONGSIP   4
#define TONE_ENTER_STRONGPUFF 5
#define TONE_EXIT_STRONGPUFF  6
#define TONE_INDICATE_SIP     7
#define TONE_INDICATE_PUFF    8
#define TONE_IR               9
#define TONE_BT_PAIRING      10
#define TONE_IR_REC          11

/**
   @name UpdateTones
   @brief updates state/timing of running tone signals
   @return none
*/
void UpdateTones();

/**
   @name makeTone
   @brief initiates a new tone signal
   @return none
*/
void makeTone(uint8_t kind, uint8_t param);

#endif
