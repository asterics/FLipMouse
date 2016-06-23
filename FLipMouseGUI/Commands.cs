
/* 
 
  Supported AT-commands:  
  (sent via serial interface, 115200 baud, using spaces between parameters.  Enter (<cr>, ASCII-code 0x0d) finishes a command)
   
         AT                returns "OK"
         AT ID             returns identification string (e.g. "FLipMouse V2.0")
         AT BM <uint>      puts button into programming mode (e.g. "AT BM 2" -> next AT-command defines the new function for button 2)
                           for the FLipmouse, there are 11 buttons available (3 physical buttons, 8 virtual functions): 

                             1: internal button1 / StrongPuff + UP
                             2: external button2 / StrongPuff + LEFT 
                             3: external button3 / StrongPuff + RIGHT 
                             4: alternative UP 
                             5: alternative DOWN 
                             6: alternative LEFT 
                             7: alternative RIGHT
                             8: Sip (pressure lower than sip threshold)
                             9: Strong Sip
                             10: Puff (pressure bigger than puff threshold)
                             11: StrongSip + Up
                             12: StrongSip + Down
                             13: StrongSip + Left
                             14: StrongSip + Right
                             15: StrongPuff + Up
                             16: StrongPuff + Down
                             17: StrongPuff + Left
                             18: StrongPuff + Right


   USB HID commands:
      
         AT CL             click left mouse button  
         AT CR             click right mouse button  
         AT CM             click middle mouse button  
         AT CD             click double with left mouse button

         AT PL             press/hold the left mouse button  
         AT PR             press/hold the right mouse button
         AT PM             press/hold the middle mouse button 
  
         AT RL             release the left mouse button  
         AT RR             release the right mouse button
         AT RM             release the middle mouse button 
          
         AT WU             move mouse wheel up  
         AT WD             move mouse wheel down  
         AT WS <uint>      set mouse wheel stepsize (e.g. "AT WS 3" sets the wheel stepsize to 3 rows)
   
         AT MX <int>       move mouse in x direction (e.g. "AT MX 4" moves cursor 4 pixels to the right)  
         AT MY <int>       move mouse in y direction (e.g. "AT MY -10" moves cursor 10 pixels up)  

         AT KW <string>    keyboard write string (e.g." AT KW Hello!" writes "Hello!")    
         AT KP <string>    key press: press/hold keys identifier 
                           (e.g. "AT KP KEY_UP" presses the "Cursor-Up" key, "AT KP KEY_CTRL KEY_ALT KEY_DELETE" presses all three keys)
         AT KH <string>    key hold: sticky hold keys (key will be pressed until next button press or "AT KH" command) 
                           (e.g. "AT KH KEY_A" presses the "A" key until  "AT KH KEY_A" is called again)
                           for a list of supported key idientifier strings see below ! 
                            
         AT KR <string>    key release: releases all keys identified in the string    
         AT RA             release all: releases all currently pressed keys and buttons    
          
   Housekeeping commands:

         AT SA <string>  save settings and current button modes to next free eeprom slot under given name (e.g. AT SAVE mouse1)
         AT LO <string>  load button modes from eeprom slot (e.g. AT LOAD mouse1 -> loads profile named "mouse1")
         AT LA           load all slots (displays names and settings of all stored slots) 
         AT LI           list all saved mode names 
         AT NE           next mode will be loaded (wrap around after last slot)
         AT DE           delete EEPROM content (delete all stored slots)
         AT NC           no command (idle operation)
          
   FLipMouse-specific settings and commands:

         AT MM <uint>    mouse mode: cursor on (uint==1) or alternative functions on (uint==0)
         AT SW           switch between mouse cursor and alternative functions
         AT SR           start reporting raw values (5 sensor values, starting with "VALUES:") 
         AT ER           end reporting raw values
         AT CA           calibration of zeropoint
         AT AX <uint>    acceleration x-axis  (0-100)
         AT AY <uint>    acceleration y-axis  (0-100)
         AT DX <uint>    deadzone x-axis  (0-1000)
         AT DY <uint>    deadzone y-axis  (0-1000)
         AT TS <uint>    treshold for sip action  (0-512)
         AT TP <uint>    treshold for puff action (512-1023)
         AT SP <uint>    treshold for strong puff (512-1023)
         AT SS <uint>    treshold for strong sip (0-512)
         AT GU <uint>    gain for up sensor (0-100)
         AT GD <uint>    gain for down sensor (0-100)
         AT GL <uint>    gain for left sensor (0-100)
         AT GR <uint>    gain for right sensor (0-100)
  
   Infrared-specific commands:

         AT IR <string>  record new infrared code and store it under given name (e.g. "AT IR vol_up")
         AT IP <string>  play  infrared code with given name (e.g. "AT IP vol_up")
         AT IC <string>  clear infrared code with given name (e.g. "AT ID vol_up")
         AT IL           lists all stored infrared command names

  supported key identifiers for key press command (AT KP):
 
   KEY_A   KEY_B   KEY_C   KEY_D    KEY_E   KEY_F   KEY_G   KEY_H   KEY_I   KEY_J    KEY_K    KEY_L
   KEY_M   KEY_N   KEY_O   KEY_P    KEY_Q   KEY_R   KEY_S   KEY_T   KEY_U   KEY_V    KEY_W    KEY_X 
   KEY_Y   KEY_Z   KEY_1   KEY_2    KEY_3   KEY_4   KEY_5   KEY_6   KEY_7   KEY_8    KEY_9    KEY_0
   KEY_F1  KEY_F2  KEY_F3  KEY_F4   KEY_F5  KEY_F6  KEY_F7  KEY_F8  KEY_F9  KEY_F10  KEY_F11  KEY_F12	
    
   KEY_RIGHT   KEY_LEFT       KEY_DOWN        KEY_UP      KEY_ENTER    KEY_ESC   KEY_BACKSPACE   KEY_TAB	
   KEY_HOME    KEY_PAGE_UP    KEY_PAGE_DOWN   KEY_DELETE  KEY_INSERT   KEY_END	  KEY_NUM_LOCK    KEY_SCROLL_LOCK
   KEY_SPACE   KEY_CAPS_LOCK  KEY_PAUSE       KEY_SHIFT   KEY_CTRL     KEY_ALT   KEY_RIGHT_ALT   KEY_GUI 
   KEY_RIGHT_GUI
    
*/



using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;


namespace MouseApp2
{
    public partial class FLipMouseGUI
    {
        const int PARTYPE_NONE = 0;
        const int PARTYPE_UINT = 1;
        const int PARTYPE_INT =  2;
        const int PARTYPE_STRING = 3;

        const int COMBOENTRY_YES = 1;
        const int COMBOENTRY_NO = 0;

        const int GUITYPE_STANDARD  = 0;
        const int GUITYPE_INTFIELD  = 1;
        const int GUITYPE_TEXTFIELD = 2;
        const int GUITYPE_KEYSELECT = 3;
        const int GUITYPE_SLIDER    = 4;
        const int GUITYPE_3RADIOBUTTONS   = 5;
        const int GUITYPE_GENERIC   = 6;

        const string PREFIX_FLIPMOUSE_VERSION = "FLIPMOUSE ";
        const string PREFIX_REPORT_VALUES = "VALUES:";
        const string PREFIX_SLOT_NAME = "SLOT:";
        const string PREFIX_AT_COMMAND = "AT ";
        const string PREFIX_END_OF_SLOTS = "END";
        const string PREFIX_IRCOMMAND_NAME = "IRCOMMAND";
        const string PREFIX_IRCOMMAND_RECORDED = "IR: RECORDED ";
        const string PREFIX_IRCOMMAND_TIMEOUT = "IR_TIMEOUT:";


        public AllCommands allCommands = new AllCommands();
        public void initCommands()
        {
            allCommands.add(new Command("AT ID", PARTYPE_NONE, "Get Id String", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT BM", PARTYPE_UINT, "Update Button mode", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT CL", PARTYPE_NONE, "Click Left Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT CR", PARTYPE_NONE, "Click Right Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT CM", PARTYPE_NONE, "Click Middle Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT CD", PARTYPE_NONE, "Double Click Left Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT PL", PARTYPE_NONE, "Hold Left Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT PR", PARTYPE_NONE, "Hold Right Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT PM", PARTYPE_NONE, "Hold Middle Mouse Button", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT RL", PARTYPE_NONE, "Release Left Mouse Button", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT RR", PARTYPE_NONE, "Release Right Mouse Button", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT RM", PARTYPE_NONE, "Release Middle Mouse Button", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT WU", PARTYPE_NONE, "Wheel Up", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT WD", PARTYPE_NONE, "Wheel Down", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT WS", PARTYPE_UINT, "Mouse Wheel Step Size", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT MX", PARTYPE_INT, "Move Mouse X", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT MY", PARTYPE_INT, "Move Mouse Y", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT JX", PARTYPE_INT, "Joystick X-Axis", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT JY", PARTYPE_INT, "Joystick Y-Axis", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT JZ", PARTYPE_INT, "Joystick Z-Axis", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT JT", PARTYPE_INT, "Joystick Z Turn", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT JS", PARTYPE_INT, "Joystick Slider", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT JP", PARTYPE_INT, "Press Joystick Button", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT JR", PARTYPE_INT, "Release Joystick Button", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT JH", PARTYPE_INT, "Joystick Hat position", COMBOENTRY_YES, GUITYPE_INTFIELD));
            allCommands.add(new Command("AT KW", PARTYPE_STRING, "Write Text", COMBOENTRY_YES, GUITYPE_TEXTFIELD));
            allCommands.add(new Command("AT KP", PARTYPE_STRING, "Press Keys", COMBOENTRY_YES, GUITYPE_KEYSELECT));
            allCommands.add(new Command("AT KH", PARTYPE_STRING, "Hold Keys", COMBOENTRY_YES, GUITYPE_KEYSELECT));
            allCommands.add(new Command("AT KR", PARTYPE_STRING, "Release Keys", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT RA", PARTYPE_NONE, "Release All", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT SA", PARTYPE_STRING, "Save Slot", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT LO", PARTYPE_STRING, "Load Slot by Name", COMBOENTRY_YES, GUITYPE_TEXTFIELD));
            allCommands.add(new Command("AT LA", PARTYPE_NONE, "Load All", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT LI", PARTYPE_NONE, "List Slots", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT NE", PARTYPE_NONE, "Load Next Slot", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT DE", PARTYPE_NONE, "Delete all configurations", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT NC", PARTYPE_NONE, "No Command", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT MM", PARTYPE_UINT, "Mouse Mode (1) or Joystick (2) or Alternative (0)", COMBOENTRY_NO, GUITYPE_3RADIOBUTTONS));
            allCommands.add(new Command("AT SW", PARTYPE_NONE, "Switch Mouse/Joystick/Alternative", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT SR", PARTYPE_NONE, "Start Rawvalue reports", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT ER", PARTYPE_NONE, "End Rawvalue reports", COMBOENTRY_NO, GUITYPE_STANDARD));
            allCommands.add(new Command("AT CA", PARTYPE_NONE, "Calibrate Middle Position", COMBOENTRY_YES, GUITYPE_STANDARD));
            allCommands.add(new Command("AT AX", PARTYPE_UINT, "Acceleration X", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT AY", PARTYPE_UINT, "Acceleration Y", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT DX", PARTYPE_UINT, "Deadzone X", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT DY", PARTYPE_UINT, "Deadzone Y", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT TS", PARTYPE_UINT, "Theshold Sip", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT TP", PARTYPE_UINT, "Theshold Puff", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT SP", PARTYPE_UINT, "Threshold StrongPuff", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT SS", PARTYPE_UINT, "Threshold StrongSip", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT GU", PARTYPE_UINT, "Gain for Up Sensor", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT GD", PARTYPE_UINT, "Gain for Down Sensor", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT GL", PARTYPE_UINT, "Gain for Left Sensor", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT GR", PARTYPE_UINT, "Gain for Right Sensor", COMBOENTRY_NO, GUITYPE_SLIDER));
            allCommands.add(new Command("AT IR", PARTYPE_STRING, "Record Infrared Command", COMBOENTRY_YES, GUITYPE_TEXTFIELD));
            allCommands.add(new Command("AT IP", PARTYPE_STRING, "Play Infrared Command", COMBOENTRY_YES, GUITYPE_TEXTFIELD));
            allCommands.add(new Command("AT IC", PARTYPE_STRING, "Clear Infrared Command", COMBOENTRY_YES, GUITYPE_TEXTFIELD));
            allCommands.add(new Command("AT IL", PARTYPE_NONE,   "List Infrared Commands", COMBOENTRY_YES, GUITYPE_STANDARD));

        }


        static public List<CommandGuiLink> commandGuiLinks = new List<CommandGuiLink>();

        public void initCommandGuiLinks()
        {
            commandGuiLinks.Add(new CommandGuiLink("AT AX", speedXBar, speedXLabel, "60"));
            commandGuiLinks.Add(new CommandGuiLink("AT AY", speedYBar, speedYLabel, "60"));
            commandGuiLinks.Add(new CommandGuiLink("AT DX", deadzoneXBar, deadzoneXLabel, "20"));
            commandGuiLinks.Add(new CommandGuiLink("AT DY", deadzoneYBar, deadzoneYLabel, "20"));
            commandGuiLinks.Add(new CommandGuiLink("AT TS", sipThresholdBar, sipThresholdLabel, "500"));
            commandGuiLinks.Add(new CommandGuiLink("AT TP", puffThresholdBar, puffThresholdLabel, "525"));
            commandGuiLinks.Add(new CommandGuiLink("AT SP", strongPuffThresholdBar, strongPuffThresholdLabel, "700"));
            commandGuiLinks.Add(new CommandGuiLink("AT SS", strongSipThresholdBar, stongSipThresholdLabel , "300"));
            commandGuiLinks.Add(new CommandGuiLink("AT GU", upGainBar, upGainLabel, "50"));
            commandGuiLinks.Add(new CommandGuiLink("AT GD", downGainBar, downGainLabel, "50"));
            commandGuiLinks.Add(new CommandGuiLink("AT GL", leftGainBar, leftGainLabel, "50"));
            commandGuiLinks.Add(new CommandGuiLink("AT GR", rightGainBar, rightGainLabel, "50"));
            commandGuiLinks.Add(new CommandGuiLink("AT MM", selectStick, selectJoystick, selectAlternative, "1"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 01", Button1FunctionBox, Button1ParameterText, Button1NumericParameter, "AT NE"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 02", Button2FunctionBox, Button2ParameterText, Button2NumericParameter, "AT KP KEY_ESC "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 03", Button3FunctionBox, Button3ParameterText, Button3NumericParameter, "AT NC" ));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 04", UpFunctionMenu, UpParameterText, UpNumericParameter, "AT KP KEY_UP "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 05", DownFunctionMenu, DownParameterText, DownNumericParameter, "AT KP KEY_DOWN "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 06", LeftFunctionMenu, LeftParameterText, LeftNumericParameter, "AT KP KEY_LEFT "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 07", RightFunctionMenu, RightParameterText, RightNumericParameter, "AT KP KEY_RIGHT "));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 08", SipFunctionMenu, SipParameterText, SipNumericParameter, "AT PL"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 09", StrongSipFunctionMenu, StrongSipParameterText, StrongSipNumericParameter, "AT NC"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 10", PuffFunctionMenu, PuffParameterText, PuffNumericParameter, "AT CR"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 11", StrongPuffFunctionMenu, StrongPuffParameterText, StrongPuffNumericParameter, "AT CA"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 12", StrongSipUpFunctionBox, StrongSipUpParameterText, StrongSipUpNumericParameter, "AT NC"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 13", StrongSipDownFunctionBox, StrongSipDownParameterText, StrongSipDownNumericParameter, "AT NC"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 14", StrongSipLeftFunctionBox, StrongSipLeftParameterText, StrongSipLeftNumericParameter, "AT NC"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 15", StrongSipRightFunctionBox, StrongSipRightParameterText, StrongSipRightNumericParameter, "AT NC"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 16", StrongPuffUpFunctionBox, StrongPuffUpParameterText, StrongPuffUpNumericParameter, "AT NC"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 17", StrongPuffDownFunctionBox, StrongPuffDownParameterText, StrongPuffDownNumericParameter, "AT NC"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 18", StrongPuffLeftFunctionBox, StrongPuffLeftParameterText, StrongPuffLeftNumericParameter, "AT NC"));
            commandGuiLinks.Add(new CommandGuiLink("AT BM 19", StrongPuffRightFunctionBox, StrongPuffRightParameterText, StrongPuffRightNumericParameter, "AT NC"));
        }

        String[] keyOptions = {    "Clear Keycodes!", "KEY_A","KEY_B","KEY_C","KEY_D","KEY_E","KEY_F","KEY_G","KEY_H","KEY_I","KEY_J","KEY_K","KEY_L",
                                   "KEY_M","KEY_N","KEY_O","KEY_P","KEY_Q","KEY_R","KEY_S","KEY_T","KEY_U","KEY_V","KEY_W","KEY_X",
                                   "KEY_Y","KEY_Z","KEY_1","KEY_2","KEY_3","KEY_4","KEY_5","KEY_6","KEY_7","KEY_8","KEY_9","KEY_0",
                                   "KEY_F1","KEY_F2","KEY_F3","KEY_F4","KEY_F5","KEY_F6","KEY_F7","KEY_F8","KEY_F9","KEY_F10","KEY_F11","KEY_F12",	
                                   "KEY_UP","KEY_DOWN","KEY_LEFT","KEY_RIGHT","KEY_SPACE","KEY_ENTER",
                                   "KEY_ALT","KEY_BACKSPACE","KEY_CAPS_LOCK","KEY_CTRL","KEY_DELETE","KEY_END","KEY_ESC","KEY_GUI",
                                   "KEY_HOME","KEY_INSERT","KEY_NUM_LOCK","KEY_PAGE_DOWN","KEY_PAGE_UP","KEY_PAUSE","KEY_RIGHT_ALT",
                                   "KEY_RIGHT_GUI","KEY_SCROLL_LOCK","KEY_SHIFT","KEY_TAB"
                              };

        public void sendIDCommand()
        {
            sendCmd("AT ID");
        }

        public void sendNextCommand()
        {
            sendCmd("AT NE");
        }

        public void sendLoadSlotCommand(String slotname)
        {
            sendCmd("AT LO "+slotname);
        }

        public void sendLoadAllCommand()
        {
            sendCmd("AT LA");
        }

        public void sendEndReportingCommand()
        {
            sendCmd("AT ER");
        }

        public void sendStartReportingCommand()
        {
            sendCmd("AT SR");
        }

        public void sendCalibrationCommand()
        {
            sendCmd("AT CA");
        }

        public void sendSaveSlotCommands(String slotname)
        {
            sendCmd("AT SA " + slotname);
        }

        public void sendClearCommand()
        {
            sendCmd("AT DE");
        }

        public void sendListIRCommand()
        {
            sendCmd("AT IL ");
        }

        public void sendRecordIRCommand(String commandName)
        {
            sendCmd("AT IR " + commandName);
        }

        public void sendPlayIRCommand(String commandName)
        {
            sendCmd("AT IP " + commandName);
        }

        public void sendClearIRCommand(String commandName)
        {
            sendCmd("AT IC " + commandName);
        }


        public class Slot
        {
            public String slotName;
            public List<String> settingStrings = new List<String>();
            public Slot()
            {
                this.slotName = "mouse";
                init();
            }
            public Slot(String slotName)
            {
                this.slotName = slotName;
                init();
            }
            void init() {
                foreach (CommandGuiLink cgl in commandGuiLinks)
                {
                    switch (cgl.type)
                    {
                        case GUITYPE_GENERIC:
                            settingStrings.Add(cgl.cmd); settingStrings.Add(cgl.def);
                            break;
                        case GUITYPE_3RADIOBUTTONS:
                        case GUITYPE_SLIDER:
                            settingStrings.Add(cgl.cmd + " " + cgl.def);
                            break;
                    }
                }
            }
        };

        private String buildCommandString(String cmdDescription, String stringParam, int intParam)
        {
            String str = allCommands.getCommand(cmdDescription);
            switch (allCommands.getParameterType(cmdDescription))
            {
                case PARTYPE_UINT:
                case PARTYPE_INT: str += " " + intParam; break;
                case PARTYPE_STRING: str += " " + stringParam; break;
            }
            return (str);
        }


        public void sendApplyCommands()
        {

            foreach (CommandGuiLink cgl in commandGuiLinks)
            {
                switch (cgl.type)
                {
                    case GUITYPE_GENERIC:
                        sendCmd(cgl.cmd);
                        sendCmd(buildCommandString(cgl.cb.Text, cgl.tb.Text, (int)cgl.nud.Value));
                        break;
                    case GUITYPE_SLIDER:
                        sendCmd(cgl.cmd + " " + cgl.tl.Text);
                        break;
                    case GUITYPE_3RADIOBUTTONS:
                        if (cgl.rb1.Checked)
                            sendCmd(cgl.cmd + " 1");
                        else if (cgl.rb2.Checked) sendCmd(cgl.cmd + " " + (joyModeBox.SelectedIndex+2));  // sorry for this hack ;)    TBD: improve ! 
                        else if (cgl.rb3.Checked) sendCmd(cgl.cmd + " 0");
                        break;
                }
            }
        }


        public class Command
        {
            public String cmd;
            public int parameterType; // 0=no params, 1=uint paramter, 3=int parameter, 4=string parameter
            public String commandDescription;
            public int comboEntry;
            public int guiType;

            public Command(String cmd, int parameterType, String commandDescription, int comboEntry, int guiType)
            {
                this.cmd = cmd;
                this.parameterType = parameterType;
                this.commandDescription = commandDescription;
                this.comboEntry = comboEntry;
                this.guiType = guiType;
            }
        };

        public class AllCommands
        {
            List<Command> commandList = new List<Command>();

            public void add(Command command)
            {
                commandList.Add(command);
            }
            public String getCommandDescription(int index)
            {
                if ((index>=0)&&(index<commandList.Count))
                    return(commandList[index].commandDescription);
                return (null);
            }
            public int getParameterType(String commandDescription)
            {
                foreach (Command c in commandList)
                    if (c.commandDescription.Equals(commandDescription)) return (c.parameterType);
                return (-1);
            }
            public int getGuiTypeFromDescription(String commandDescription)
            {
                foreach (Command c in commandList)
                    if (c.commandDescription.Equals(commandDescription)) return (c.guiType);
                return (-1);
            }
            public int getGuiTypeFromCommand(String command)
            {
                foreach (Command c in commandList)
                    if (c.cmd.Equals(command)) return (c.guiType);
                return (-1);
            }
            public String getCommand(String commandDescription)
            {
                foreach (Command c in commandList)
                    if (c.commandDescription.Equals(commandDescription)) return (c.cmd);
                return (null);
            }

            public int getComboEntry(String commandDescription)
            {
                foreach (Command c in commandList)
                    if (c.commandDescription.Equals(commandDescription)) return (c.comboEntry);
                return (-1);
            }
            public int getComboEntry(int index)
            {
                if ((index >= 0) && (index < commandList.Count))
                    return (commandList[index].comboEntry);
                return (-1);
            }
            public int getSelectionIndex(int index)
            {
                int selectionIndex = 0;
                if ((index >= 0) && (index < commandList.Count))
                {
                    for (int i = 0; i < index; i++)
                        if (getComboEntry(i) == COMBOENTRY_YES)
                            selectionIndex++;
                }
                return (selectionIndex);
            }
            public int getSelectionIndex(string cmd)
            {
                int selectionIndex = -1;

                for (int i = 0;i < commandList.Count;i++)
                {
                    if (getComboEntry(i) == COMBOENTRY_YES)
                        selectionIndex++;
                    if (cmd.Equals(commandList[i].cmd)) return (selectionIndex);
                }
                return (selectionIndex);
            }
            public int length()
            {
                return commandList.Count;
            }

        };


        public class CommandGuiLink
        {
            public int type;
            public String cmd;
            public String def;

            public ComboBox cb;
            public TextBox tb;
            public NumericUpDown nud;
            public TrackBar tr;
            public Label tl;
            public RadioButton rb1;
            public RadioButton rb2;
            public RadioButton rb3;

            public CommandGuiLink(String cmd, ComboBox cb, TextBox tb, NumericUpDown nud, String def)
            {
                this.type = GUITYPE_GENERIC;
                this.cmd = cmd;
                this.cb = cb;
                this.tb = tb;
                this.nud = nud;
                this.def = def;
            }

            public CommandGuiLink(String cmd, TrackBar tr, Label tl, String def)
            {
                this.type = GUITYPE_SLIDER;
                this.cmd = cmd;
                this.tr = tr;
                this.tl = tl;
                this.def = def;
            }
            public CommandGuiLink(String cmd, RadioButton rb1, RadioButton rb2, RadioButton rb3, String def)
            {
                this.type = GUITYPE_3RADIOBUTTONS;
                this.cmd = cmd;
                this.rb1 = rb1;
                this.rb2 = rb2;
                this.rb3 = rb3;
                this.def = def;
            }
        };

    }
}
