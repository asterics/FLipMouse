
#include "fabi.h"

uint8_t get_uint(char * str, int16_t * result)
{
    int num=0;
    if ((str==0)||(*str==0)) return (0);
    while (*str)
    {
      if ((*str >= '0') && (*str<='9'))
         num=num*10+(*str - '0'); 
      else return(0);
      str ++;
    }
    *result=num;
    return(1);    
}

uint8_t get_int(char * str, int16_t * result)
{
    int16_t num,fact;
    if (str==0) return(0);
    if (*str =='-') {fact=-1; str++;} else fact=1;
    if (!get_uint(str,&num)) return(0);
    *result=num*fact;
    return(1);    
}

void strup (char * str)   // convert to upper case letters
{
  if (!str) return;
  while (*str)
  {
    if ((*str>='a') && (*str<='z')) *str=*str-'a'+'A';
    str++;
  }
}

void parseCommand (char * cmdstr)
{
    int8_t cmd=-1;
    int16_t num=0;
    
     // Serial.print("parseCommand:"); Serial.println(cmdstr);
    char * actpos = strtok(cmdstr," ");   // see a nice explaination of strtok here:  http://www.reddit.com/r/arduino/comments/2h9l1l/using_the_strtok_function/
    if (actpos) 
    {
        strup(actpos);

        // housekeeping commands
        if (!strcmp(actpos,"ID"))   cmd=CMD_PRINT_ID;
        if (!strcmp(actpos,"SAVE"))  { actpos=strtok(NULL," "); cmd=CMD_SAVE_SLOT; }
        if (!strcmp(actpos,"LOAD"))  { actpos=strtok(NULL," "); cmd=CMD_LOAD_SLOT; }
        if (!strcmp(actpos,"NEXT"))  cmd=CMD_NEXT_SLOT;
        if (!strcmp(actpos,"CLEAR")) cmd=CMD_DELETE_SLOTS;
        if (!strcmp(actpos,"LIST"))  cmd=CMD_LIST_SLOTS;
        if (!strcmp(actpos,"IDLE"))  cmd=CMD_IDLE;
        
        //  button feature commands
        if (!strcmp(actpos,"BM")) { actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=CMD_BUTTON_MODE; }
        if (!strcmp(actpos,"CL")) cmd=CMD_MOUSE_CLICK_LEFT;
        if (!strcmp(actpos,"CR")) cmd=CMD_MOUSE_CLICK_RIGHT; 
        if (!strcmp(actpos,"CM")) cmd=CMD_MOUSE_CLICK_MIDDLE; 
        if (!strcmp(actpos,"CD")) cmd=CMD_MOUSE_CLICK_DOUBLE; 
        if (!strcmp(actpos,"PL")) cmd=CMD_MOUSE_PRESS_LEFT; 
        if (!strcmp(actpos,"PR")) cmd=CMD_MOUSE_PRESS_RIGHT; 
        if (!strcmp(actpos,"PM")) cmd=CMD_MOUSE_PRESS_MIDDLE; 
        if (!strcmp(actpos,"RL")) cmd=CMD_MOUSE_RELEASE_LEFT; 
        if (!strcmp(actpos,"RR")) cmd=CMD_MOUSE_RELEASE_RIGHT; 
        if (!strcmp(actpos,"RM")) cmd=CMD_MOUSE_RELEASE_MIDDLE; 
        if (!strcmp(actpos,"WU")) cmd=CMD_MOUSE_WHEEL_UP; 
        if (!strcmp(actpos,"WD")) cmd=CMD_MOUSE_WHEEL_DOWN; 
        if (!strcmp(actpos,"WS")) { actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=CMD_MOUSE_WHEEL_STEP; }
        if (!strcmp(actpos,"MX")) { actpos=strtok(NULL," "); if (get_int(actpos, &num)) cmd=CMD_MOUSE_MOVEX; }
        if (!strcmp(actpos,"MY")) { actpos=strtok(NULL," "); if (get_int(actpos, &num)) cmd=CMD_MOUSE_MOVEY; }
        if (!strcmp(actpos,"KW")) { actpos+=3; cmd=CMD_KEY_WRITE; }
        if (!strcmp(actpos,"KP")) { actpos+=3; cmd=CMD_KEY_PRESS; }
        if (!strcmp(actpos,"KR")) { actpos+=3; cmd=CMD_KEY_RELEASE; }
        if (!strcmp(actpos,"RA")) cmd=CMD_RELEASE_ALL;
    
    
        // FLipMouse specific commands
        
        if (!strcmp(actpos,"MM"))  cmd=CMD_MM;
        if (!strcmp(actpos,"AF"))  cmd=CMD_AF;
        if (!strcmp(actpos,"SW"))  cmd=CMD_SW;
        if (!strcmp(actpos,"SR"))  cmd=CMD_SR;
        if (!strcmp(actpos,"ER"))  cmd=CMD_ER;
        if (!strcmp(actpos,"CA"))  cmd=CMD_CA;
        if (!strcmp(actpos,"AX"))  { actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=CMD_AX;}
        if (!strcmp(actpos,"AY"))  { actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=CMD_AY;}
        if (!strcmp(actpos,"DX"))  { actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=CMD_DX;}
        if (!strcmp(actpos,"DY"))  { actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=CMD_DY;}
        if (!strcmp(actpos,"TS"))  { actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=CMD_TS;}
        if (!strcmp(actpos,"TP"))  { actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=CMD_TP;}
        if (!strcmp(actpos,"TT"))  { actpos=strtok(NULL," "); if (get_uint(actpos, &num)) cmd=CMD_TT;}
    }
    if (cmd>-1)
    {
        // Serial.print("cmd parser found:");Serial.print(cmd); Serial.print(", "); Serial.print(num); 
        // if (actpos) {Serial.print(", "); Serial.println(actpos);} else Serial.println();   
        performCommand(cmd,num,actpos,0);        
    }
    else   Serial.println("cmd parser: ?");              
}


extern void init_CIM_frame (void);
extern void parse_CIM_protocol(int actbyte);
extern uint8_t CimMode;

static char cmdstring[MAX_CMDLEN];

void parseByte (int newByte)  // parse an incoming commandbyte from serial interface, perform command if valid
{
   static uint8_t state=0;
   static uint8_t cmdlen=0;
  
   if (CimMode==1)
          parse_CIM_protocol(newByte);   // handle AsTeRICS CIM protocal messages !
   else
   {
      switch (state) {
        case 0: 
                if ((newByte=='A') || (newByte=='a')) state++;
                else if (newByte=='@') { 
                    init_CIM_frame ();
                }
             break;
        case 1: 
                if ((newByte=='T') || (newByte=='t')) state++; else state=0;
            break;
        case 2: 
                if ((newByte==13) || (newByte==10))
                {  Serial.println("OK");  state=0; }
                else if (newByte==' ') { cmdlen=0; state++; } 
                else goto err;
            break;
        case 3: 
                if ((newByte==13) || (newByte==10) || (cmdlen>=MAX_CMDLEN-1))
                {  cmdstring[cmdlen]=0;  parseCommand(cmdstring); 
                  state=0; }
                else cmdstring[cmdlen++]=newByte;
            break;   
        default: err: Serial.println("?");state=0;
      }
   }
}



