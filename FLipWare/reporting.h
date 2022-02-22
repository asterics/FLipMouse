/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: reporting.h - implementation of serial reports

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _REPORTING_H_
#define _REPORTING_H_


#define REPORT_NONE  0
#define REPORT_ALL_SLOTS 1

extern uint8_t reportSlotParameters;
extern uint8_t reportRawValues;


void printCurrentSlot();
void reportValues();

#endif
