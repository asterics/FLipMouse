/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: infrared.h - implementation of the infrared record/replay functions 

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#ifndef _INFRARED_H_
#define _INFRARED_H_


#define IR_LED_PIN     6    //  IR-Led output pin
#define IR_SENSOR_PIN  4    //  input pin of the TSOP IR receiver

//Time until the record command will be canceled (in milliseconds)
#define IR_USER_TIMEOUT_MS 10000

// ir code repeat gap (in microseconds)
#define IR_REPEAT_GAP 10000

// Maximum count of uint16_t edges for one IR-command
#define IR_EDGE_REC_MAX 250

// minimum count of signal edges which are necessary to accept a command
#define IR_EDGE_REC_MIN 5

// name of the idle code command (played after other ir commands if it exists)
#define IDLESEQUENCE_NAME "idle"
#define IDLESEQUENCE_REPEAT 1

// maximum time interval which can be stored in high precision (microseconds) format
#define MAX_HIGHPRECISION_DURATION 64000

// IR function declarations
void initIR();
void record_IR_command(char * name);
void play_IR_command(char * name);
void hold_IR_command(char * name);
void stop_IR_command();
void list_IR_commands();
uint8_t delete_IR_command(char * name);
void set_IR_timeout(uint16_t ms);
void wipe_IR_commands();

#endif
