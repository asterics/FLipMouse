/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: https://www.asterics-foundation.org

     Module: tone.cpp - functions for tone/audio feedback

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include <Arduino.h>
#include "tone.h"

/**
   static variables for tone signal generation
*/
uint16_t toneHeight;
uint16_t toneOnTime;
uint16_t toneOffTime;
uint16_t toneCount = 0;


void updateTones()
{
  static uint16_t toneState = 0;
  static uint16_t cnt = 0;

  if (!toneCount) return;

  uint8_t tonePin = TONE_PIN;

  switch (toneState) {
    case 0:
      tone(tonePin, toneHeight, toneOnTime);
      toneState++;
      break;
    case 1:
      if (++cnt > (toneOnTime + toneOffTime) / UPDATE_INTERVAL )  {
        toneCount--;
        toneState = 0;
        cnt = 0;
      }
      break;
  }
}


void makeTone(uint8_t kind, uint8_t param)
{
  uint8_t tonePin = TONE_PIN;

  switch (kind) {
    case TONE_ENTER_STRONGPUFF:
      tone(tonePin, 400, 200);
      break;
    case TONE_EXIT_STRONGPUFF:
      tone(tonePin, 400, 100);
      break;
    case TONE_CALIB:
      tone(tonePin, 200, 400);
      break;
    case TONE_CHANGESLOT:
      if (!toneCount) {
        toneHeight = 2000 + 200 * param;
        toneOnTime = 150;
        toneOffTime = 50;
        toneCount = param + 1;
      }
      break;
    case TONE_ENTER_STRONGSIP:
      tone(tonePin, 300, 200);
      break;
    case TONE_EXIT_STRONGSIP:
      tone(tonePin, 300, 100);
      break;
    case TONE_IR:
      tone(tonePin, 2500, 30);
      break;
    case TONE_IR_REC:
      tone(tonePin, 350, 500);
      break;
    case TONE_INDICATE_SIP:
      tone(tonePin, 5000, 5);
      break;
    case TONE_INDICATE_PUFF:
      tone(tonePin, 4000, 5);
      break;
    case TONE_BT_PAIRING:
      tone(tonePin, 230, 4000);
      break;
  }
}
