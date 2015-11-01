

/* 
     FLipMouseGUI - Graphical user Interface for FlipMouse / FlipWare firmware
       built upon Assistive Button Interface (FABI) Version 2.0  - AsTeRICS Academy 2015 - http://www.asterics-academy.net
   
     for a list of supported AT commands, see commands.cs
   
*/



using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;



namespace MouseApp2
{
    public partial class FLipMouseGUI : Form
    {
        const string VERSION_STRING = "2.0";

        const int SPEED_CHANGE_STEP = 2;
        const int DEADZONE_CHANGE_STEP = 2;
        const int SPECIALMODE_CHANGE_STEP = 5;
        const int PRESSURE_CHANGE_STEP = 1;
        const int GAIN_CHANGE_STEP = 1;



        const String TOKEN_SEPERATOR = "-,-";

        String[] keyOptions = {    "Clear Keycodes!", "KEY_A","KEY_B","KEY_C","KEY_D","KEY_E","KEY_F","KEY_G","KEY_H","KEY_I","KEY_J","KEY_K","KEY_L",
                                   "KEY_M","KEY_N","KEY_O","KEY_P","KEY_Q","KEY_R","KEY_S","KEY_T","KEY_U","KEY_V","KEY_W","KEY_X",
                                   "KEY_Y","KEY_Z","KEY_1","KEY_2","KEY_3","KEY_4","KEY_5","KEY_6","KEY_7","KEY_8","KEY_9","KEY_0",
                                   "KEY_F1","KEY_F2","KEY_F3","KEY_F4","KEY_F5","KEY_F6","KEY_F7","KEY_F8","KEY_F9","KEY_F10","KEY_F11","KEY_F12",	
                                   "KEY_UP","KEY_DOWN","KEY_LEFT","KEY_RIGHT","KEY_SPACE","KEY_ENTER",
                                   "KEY_ALT","KEY_BACKSPACE","KEY_CAPS_LOCK","KEY_CTRL","KEY_DELETE","KEY_END","KEY_ESC","KEY_GUI",
                                   "KEY_HOME","KEY_INSERT","KEY_NUM_LOCK","KEY_PAGE_DOWN","KEY_PAGE_UP","KEY_PAUSE","KEY_RIGHT_ALT",
                                   "KEY_RIGHT_GUI","KEY_SCROLL_LOCK","KEY_SHIFT","KEY_TAB"
                              };

        
        Boolean useAlternativeFunctions = false;
        Boolean readDone = false;
        static int slotCounter = 0;
        static int actSlot = 0;


        public delegate void StringReceivedDelegate(string newLine);
        public StringReceivedDelegate stringReceivedDelegate;

        public delegate void ClickDelegate(object sender, EventArgs e);
        ClickDelegate functionPointer;

        System.Windows.Forms.Timer clickTimer = new System.Windows.Forms.Timer();
        System.Windows.Forms.Timer IdTimer = new System.Windows.Forms.Timer();


        const int MAX_SLOTS = 5;
        public List<Slot> slots = new List<Slot>();

        public void initSlots()
        {
            slots.Clear();
            slots.Add(new Slot());
            slotNames.Items.Clear();
            slotNames.Items.Add("default");
        }

        public void storeSlot(int slotNumber)
        {
            slots[slotNumber].settingStrings.Clear();
            slots[slotNumber].slotName = slotNames.Text;
            slotNames.Items.Clear();
            foreach (Slot s in slots) slotNames.Items.Add(s.slotName); 


            foreach (CommandGuiLink guiLink in commandGuiLinks.guiLinks)
            {
                String actSettingString =guiLink.cmd;
                if (actSettingString.StartsWith("AT BM "))
                {
                 //   guiLink.cb.SelectedIndex
                    slots[slotNumber].settingStrings.Add(actSettingString);
                    actSettingString=allCommands.getCommand(guiLink.cb.Text);
                }
                switch (allCommands.getGuiTypeFromCommand(actSettingString))
                {
                    case GUITYPE_INTFIELD: actSettingString+=(" "+guiLink.nud.Value); break;
                    case GUITYPE_KEYSELECT:
                    case GUITYPE_TEXTFIELD: actSettingString+=(" "+guiLink.tb.Text); break;
                    case GUITYPE_SLIDER: actSettingString += (" " + guiLink.tr.Value); break;
                    case GUITYPE_BOOLEAN: if (guiLink.rb1.Checked) actSettingString += (" 1"); 
                                          else actSettingString += (" 0");
                                          break;
                    case GUITYPE_STANDARD: break;
                }
                slots[slotNumber].settingStrings.Add(actSettingString);
            }
        }

        public void displaySlot(int slotNumber)
        {
            CommandGuiLink actButtonLink = null;
            String strValue="";

            slotNames.Text = slots[slotNumber].slotName;
            foreach (String settingString in slots[slotNumber].settingStrings)
            {
                if (actButtonLink != null)
                {
                    String cmd = settingString.Substring(0, 5);
                    switch (allCommands.getGuiTypeFromCommand(cmd))
                    {
                        case GUITYPE_INTFIELD: actButtonLink.cb.SelectedIndex = allCommands.getSelectionIndex(cmd);
                            strValue = settingString.Substring(6);
                            actButtonLink.nud.Value = Int32.Parse(strValue); break;
                        case GUITYPE_KEYSELECT:
                        case GUITYPE_TEXTFIELD: actButtonLink.cb.SelectedIndex = allCommands.getSelectionIndex(cmd);
                            strValue = settingString.Substring(6);
                            actButtonLink.tb.Text = strValue; break;
                        case GUITYPE_SLIDER: actButtonLink.tr.Value = Int32.Parse(strValue);
                            actButtonLink.tl.Text = strValue; break;
                        case GUITYPE_STANDARD: actButtonLink.cb.SelectedIndex = allCommands.getSelectionIndex(cmd);
                            break;
                    }
                    actButtonLink = null;
                }

                else foreach (CommandGuiLink guiLink in commandGuiLinks.guiLinks)
                {
                    if (settingString.StartsWith(guiLink.cmd))  // improve this !
                    {
                        if (settingString.StartsWith("AT BM")) 
                        {
                            actButtonLink = guiLink;
                            break;
                        }
                        else
                        {
                            switch (allCommands.getGuiTypeFromCommand(guiLink.cmd))
                            {
                                case GUITYPE_INTFIELD: guiLink.cb.SelectedIndex = allCommands.getSelectionIndex(guiLink.cmd);
                                    strValue = settingString.Substring(guiLink.cmd.Length+1);
                                    guiLink.nud.Value = Int32.Parse(strValue); break;
                                case GUITYPE_TEXTFIELD: guiLink.cb.SelectedIndex = allCommands.getSelectionIndex(guiLink.cmd);
                                    strValue = settingString.Substring(guiLink.cmd.Length+1);
                                    guiLink.tb.Text = strValue; break;
                                case GUITYPE_SLIDER:
                                    strValue = settingString.Substring(guiLink.cmd.Length + 1);
                                    guiLink.tr.Value = Int32.Parse(strValue);
                                    guiLink.tl.Text = strValue; break;
                                case GUITYPE_BOOLEAN: 
                                    strValue = settingString.Substring(guiLink.cmd.Length + 1);
                                    int value= Int32.Parse(strValue);
                                    if (value == 1)  {
                                        guiLink.rb1.Checked = true; guiLink.rb2.Checked = false;
                                    }
                                    else {
                                        guiLink.rb1.Checked = false; guiLink.rb2.Checked = true;
                                    }
                                    break;

                            }
                        }
                    }
                }
            }
        }


        public FLipMouseGUI()
        {

            InitializeComponent();

            initCommands();
            initCommandGuiLinks();
            initSlots();

            clickTimer.Interval = 500; // specify interval time as you want
            clickTimer.Tick += new EventHandler(timer_Tick);

            Text += " "+ VERSION_STRING;


            for (int i = 0; i < allCommands.length(); i++)
            {
                if (allCommands.getComboEntry(i) == COMBOENTRY_YES)
                {
                    string str = allCommands.getCommandDescription(i);
                    Button1FunctionBox.Items.Add(str);
                    Button2FunctionBox.Items.Add(str);
                    Button3FunctionBox.Items.Add(str);
                    UpFunctionMenu.Items.Add(str);
                    DownFunctionMenu.Items.Add(str);
                    LeftFunctionMenu.Items.Add(str);
                    RightFunctionMenu.Items.Add(str);
                    SipFunctionMenu.Items.Add(str);
                    SpecialSipFunctionMenu.Items.Add(str);
                    PuffFunctionMenu.Items.Add(str);
                    SpecialPuffFunctionMenu.Items.Add(str);
                }
            }
         
            Button1FunctionBox.SelectedIndex = 0;
            Button2FunctionBox.SelectedIndex = 0;
            Button3FunctionBox.SelectedIndex = 0;
            UpFunctionMenu.SelectedIndex = 0;
            DownFunctionMenu.SelectedIndex = 0;
            LeftFunctionMenu.SelectedIndex = 0;
            RightFunctionMenu.SelectedIndex = 0;
            SipFunctionMenu.SelectedIndex = 0;
            SpecialSipFunctionMenu.SelectedIndex = 0;
            PuffFunctionMenu.SelectedIndex = 0;
            SpecialPuffFunctionMenu.SelectedIndex = 0;

            foreach (string str in keyOptions)
            {
                Button1ComboBox.Items.Add(str);
                Button2ComboBox.Items.Add(str);
                Button3ComboBox.Items.Add(str);
                UpComboBox.Items.Add(str);
                DownComboBox.Items.Add(str);
                LeftComboBox.Items.Add(str);
                RightComboBox.Items.Add(str);
                SipComboBox.Items.Add(str);
                SpecialSipComboBox.Items.Add(str);
                PuffComboBox.Items.Add(str);
                SpecialPuffComboBox.Items.Add(str);
            }

            displaySlot(0);

            updateComPorts();

            addToLog("FLipMouse GUI ready!");
            this.Load += LipmouseGUI_Load;
            this.FormClosed += MyWindow_Closing;
        }


        private void connectComButton_click(object sender, EventArgs e) //select button
        {
            addToLog("Trying to open COM port...");
            if (portComboBox.SelectedIndex > -1)
            {
                if (serialPort1.IsOpen)
                {
                    addToLog(String.Format("Port '{0}' is already connected.", portComboBox.SelectedItem));
                    disconnectComButton.Enabled = true;
                    loadSlotSettingsMenuItem.Enabled = true;
                    storeSlotSettingsMenuItem.Enabled = true;
                }
                else
                {

                    if (Connect(portComboBox.SelectedItem.ToString()))
                    {
                        addToLog(String.Format("Port '{0}' is now connected", portComboBox.SelectedItem));
                        portStatus.Text = "Connected";
                        portStatus.ForeColor = Color.Green;
                        calButton.Enabled = true;
                        disconnectComButton.Enabled = true;
                        loadSlotSettingsMenuItem.Enabled = true;
                        storeSlotSettingsMenuItem.Enabled = true;
                        ApplyButton.Enabled = true;
                        connectComButton.Enabled = false;

                        IdTimer.Interval = 1500;
                        IdTimer.Tick += new EventHandler(IdTimer_Tick);
                        IdTimer.Start();
                        Console.WriteLine("IdTimer started!");

                        readDone = false;
                        Thread thread = new Thread(new ThreadStart(WorkThreadFunction));
                        thread.Start();
                    }
                }
            }
            else addToLog("No port has been selected");
        }

        private void disconnnectComButton_Click(object sender, EventArgs e) //disconnect button
        {
            addToLog("Disconnecting from COM Port...");
            disconnect();
            disconnectComButton.Enabled = false;
            loadSlotSettingsMenuItem.Enabled = false;
            storeSlotSettingsMenuItem.Enabled = false;
            ApplyButton.Enabled = false;
            connectComButton.Enabled = true;
        }


        private void LipmouseGUI_Load(object sender, EventArgs e)
        {
            this.stringReceivedDelegate = new StringReceivedDelegate(stringReceived);
            BeginInvoke(this.stringReceivedDelegate, new Object[] { "VALUES:512,512,512,512,512" });
        }

        // update paint areas if tabs are changed
        private void tabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            BeginInvoke(this.stringReceivedDelegate, new Object[] { "VALUES:512,512,512,512,512" });
        }


        private void loadFromFileMenuItem_Click(object sender, EventArgs e)
        {
            loadSettingsFromFile();
        }

        private void saveToFileMenuItem_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            saveSettingsToFile();
        }

        private void exitMenuItem_Click(object sender, EventArgs e)
        {
            disconnect();
            System.Windows.Forms.Application.Exit();
        }

        void MyWindow_Closing(object sender, FormClosedEventArgs e)
        {
            disconnect();
        }


        private void portComboBox_Click(object sender, EventArgs e)
        {
            updateComPorts();
        }

        // update activity log
        private void addToLog(String text)
        {
            activityLogTextbox.SelectedText = DateTime.Now.ToString() + ": ";
            activityLogTextbox.AppendText(text); 
            activityLogTextbox.AppendText("\n");
        }

        private void selectMouse_CheckedChanged(object sender, EventArgs e)
        {
            // Console.WriteLine("stick selected");
            useAlternativeFunctions = false;
        }

        private void selectAlternative_CheckedChanged(object sender, EventArgs e)
        {
            // Console.WriteLine("alternative selected");
            useAlternativeFunctions = true;

        }


        private void prevSlotButton_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            if (actSlot > 0) actSlot--;
            else actSlot = slots.Count - 1;
            displaySlot(actSlot);
        }

        private void nextSlotButton_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            if (slots.Count - 1 > actSlot) actSlot++;
            else actSlot = 0;
            displaySlot(actSlot);
        }

        private void newSlotButton_Click(object sender, EventArgs e)
        {
            if (slots.Count < MAX_SLOTS) 
            {
                storeSlot(actSlot);
                slots.Add(new Slot());
                slotNames.Items.Add("default");
                actSlot = slots.Count - 1;
                displaySlot(actSlot);
            }
            else MessageBox.Show("Maximum number of slots reached !");
        }

        private void deleteSlotButton_Click(object sender, EventArgs e)
        {
            if (slots.Count > 1)
            {
                slots.RemoveAt(actSlot);
                slotNames.Items.Clear();
                foreach (Slot s in slots) slotNames.Items.Add(s.slotName);
                if (actSlot > 0) actSlot--;
                displaySlot(actSlot);
            }
            else MessageBox.Show("One slot must stay active !");
        }



        // send all current settings for active slot
        private void ApplyButton_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);            
            if (serialPort1.IsOpen)
            {
                sendEndReportingCommand();
                sendApplyCommands();
                sendCalibrationCommand();
                sendStartReportingCommand();
                addToLog("The selected settings have been applied to the FLipmouse");

            }
            else addToLog("Please connect a device before applying configuration changes.");            
        }


        // handle settings- and slot-management buttons
        private void calibration_Click(object sender, EventArgs e) //calibration button
        {
            addToLog("Starting Calibration...");
            if (serialPort1.IsOpen)
            {
                sendCalibrationCommand();
                addToLog("Calibration started. ");
            }
            else addToLog("Could not send to device - please connect COM port!");
        }

        private void storeSlotSettingsMenuItem_Click(object sender, EventArgs e)
        {
            storeSettingsToFLipmouse();

        }

        private void loadSlotSettingsMenuItem_Click(object sender, EventArgs e)
        {
            loadSettingsFromFLipmouse();

        }


        // update visibility of parameter fields:

        private void updateVisibility(string selectedFunction, TextBox tb, NumericUpDown nud, ComboBox cb, Label la, Button bu)
        {
            switch (allCommands.getGuiTypeFromDescription(selectedFunction))
            {
                case GUITYPE_INTFIELD: la.Visible = true; la.Text = "   Value:"; nud.Visible = true; tb.Visible = false; cb.Visible = false; bu.Visible = false; bu.Enabled = false; break;
                case GUITYPE_TEXTFIELD: la.Visible = true; la.Text = "    Text:"; nud.Visible = false; tb.Enabled = true; tb.ReadOnly = false; tb.Visible = true; tb.Text = ""; cb.Visible = false; bu.Visible = true; bu.Enabled = true; break;
                case GUITYPE_KEYSELECT: la.Visible = true; la.Text = "KeyCodes:"; nud.Visible = false; tb.Visible = true; tb.Text = ""; tb.ReadOnly = true; cb.Visible = true; bu.Visible = true; bu.Enabled = true; break;
                default: la.Visible = false; nud.Visible = false; tb.Visible = false; cb.Visible = false; bu.Visible = false; bu.Enabled = false; break;
            }
        }

        private void Button1FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button1FunctionBox.Text, Button1ParameterText, Button1NumericParameter, Button1ComboBox, Button1Label, clearButton1);
        }

        private void Button2FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button2FunctionBox.Text, Button2ParameterText, Button2NumericParameter, Button2ComboBox, Button2Label, clearButton2);
        }

        private void Button3FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button3FunctionBox.Text, Button3ParameterText, Button3NumericParameter, Button3ComboBox, Button3Label, clearButton3);
        }

        private void UpFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(UpFunctionMenu.Text, UpParameterText, UpNumericParameter, UpComboBox, UpLabel, clearButtonUp);
        }

        private void DownFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(DownFunctionMenu.Text, DownParameterText, DownNumericParameter, DownComboBox, DownLabel, clearButtonDown);
        }

        private void LeftFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(LeftFunctionMenu.Text, LeftParameterText, LeftNumericParameter, LeftComboBox, LeftLabel, clearButtonLeft);
        }

        private void RightFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(RightFunctionMenu.Text, RightParameterText, RightNumericParameter, RightComboBox, RightLabel, clearButtonRight);
        }

        private void SipFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(SipFunctionMenu.Text, SipParameterText, SipNumericParameter, SipComboBox, SipParameterLabel, clearButtonSip);
        }

        private void SpecialSipFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(SpecialSipFunctionMenu.Text, SpecialSipParameterText, SpecialSipNumericParameter, SpecialSipComboBox, SpecialSipParameterLabel, clearButtonSpecialSip);
        }

        private void PuffFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(PuffFunctionMenu.Text, PuffParameterText, PuffNumericParameter, PuffComboBox, PuffParameterLabel, clearButtonPuff);
        }

        private void SpecialPuffFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(SpecialPuffFunctionMenu.Text, SpecialPuffParameterText, SpecialPuffNumericParameter, SpecialPuffComboBox, SpecialPuffParameterLabel, clearButtonSpecialPuff);
        }

        // update the keycode parameters:

        private void updateKeyCodeParameter(ComboBox cb, TextBox tb)
        {
            if (cb.SelectedIndex == 0)
                tb.Text = "";
            else
            {
                String add = cb.Text.ToString() + " ";
                if (!tb.Text.Contains(add))
                    tb.Text += add;
            }
        }

        private void Button1ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button1ComboBox,Button1ParameterText);
        }
        private void clearButton1_Click(object sender, EventArgs e)
        {
            Button1ParameterText.Text = "";
        }

        private void Button2ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button2ComboBox, Button2ParameterText);
        }
        private void clearButton2_Click(object sender, EventArgs e)
        {
            Button2ParameterText.Text = "";
        }

        private void Button3ComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(Button3ComboBox, Button3ParameterText);
        }
        private void clearButton3_Click(object sender, EventArgs e)
        {
            Button3ParameterText.Text = "";
        }

        private void UpComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(UpComboBox, UpParameterText);
        }
        private void clearButtonUp_Click(object sender, EventArgs e)
        {
            UpParameterText.Text = "";
        }

        private void DownComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(DownComboBox, DownParameterText);
        }
        private void clearButtonDown_Click(object sender, EventArgs e)
        {
            DownParameterText.Text = "";
        }

        private void LeftComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(LeftComboBox, LeftParameterText);
        }
        private void clearButtonLeft_Click(object sender, EventArgs e)
        {
            LeftParameterText.Text = "";
        }

        private void RightComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(RightComboBox, RightParameterText);
        }
        private void clearButtonRight_Click(object sender, EventArgs e)
        {
            RightParameterText.Text = "";
        }


        private void SipComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(SipComboBox, SipParameterText);
        }
        private void clearButtonSip_Click(object sender, EventArgs e)
        {
            SipParameterText.Text = "";
        }

        private void SpecialSipComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(SpecialSipComboBox, SpecialSipParameterText);
        }
        private void clearButtonSpecialSip_Click(object sender, EventArgs e)
        {
            SpecialSipParameterText.Text = "";
        }

        private void PuffComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(PuffComboBox, PuffParameterText);
        }
        private void clearButtonPuff_Click(object sender, EventArgs e)
        {
            PuffParameterText.Text = "";
        }

        private void SpecialPuffComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(SpecialPuffComboBox, SpecialPuffParameterText);
        }
        private void clearButtonSpecialPuff_Click(object sender, EventArgs e)
        {
            SpecialPuffParameterText.Text = "";
        }


        private void splitXYBox_CheckedChanged(object sender, EventArgs e)
        {
            if (splitXYBox.Checked)
            {
                SpeedXNameLabel.Text = "Speed-X"; SpeedXNameLabel.Left-= 30; 
                SpeedYNameLabel.Visible = true;
                speedXBar.Width = 200; speedXLabel.Left -= 240;
                speedXBar.Left-=30; decSpeedX.Left -= 30;
                incSpeedX.Left -= 255;

                speedYBar.Enabled = true; speedYBar.Visible = true;
                speedYLabel.Enabled = true; speedYLabel.Visible = true;
                incSpeedY.Visible = true; incSpeedY.Enabled = true;
                decSpeedY.Visible = true; decSpeedY.Enabled = true;

                DeadzoneXNameLabel.Text = "Deadzone-X";  DeadzoneXNameLabel.Left -= 30;
                DeadzoneYNameLabel.Visible = true;
                deadzoneXBar.Width = 200; deadzoneXLabel.Left -= 240;
                deadzoneXBar.Left -= 30; decDeadzoneX.Left -= 30;
                incDeadzoneX.Left -= 255;

                deadzoneYBar.Enabled = true; deadzoneYBar.Visible = true;
                deadzoneYLabel.Enabled = true; deadzoneYLabel.Visible = true;
                incDeadzoneY.Visible = true; incDeadzoneY.Enabled = true;
                decDeadzoneY.Visible = true; decDeadzoneY.Enabled = true;

            }
            else
            {
                SpeedXNameLabel.Text = "Speed"; SpeedXNameLabel.Left += 30;
                SpeedYNameLabel.Visible = false;
                speedXBar.Width = 420; speedXLabel.Left += 240;
                speedXBar.Left += 30; decSpeedX.Left += 30;
                incSpeedX.Left += 255;

                speedYBar.Enabled = false; speedYBar.Visible = false;
                speedYLabel.Enabled = false; speedYLabel.Visible = false;
                incSpeedY.Visible = false; incSpeedY.Enabled = false;
                decSpeedY.Visible = false; decSpeedY.Enabled = false;

                DeadzoneXNameLabel.Text = "Deadzone"; DeadzoneXNameLabel.Left += 30;
                DeadzoneYNameLabel.Visible = false;
                deadzoneXBar.Width = 420; deadzoneXLabel.Left += 240;
                deadzoneXBar.Left += 30; decDeadzoneX.Left += 30;
                incDeadzoneX.Left += 255;

                deadzoneYBar.Enabled = false; deadzoneYBar.Visible = false;
                deadzoneYLabel.Enabled = false; deadzoneYLabel.Visible = false;
                incDeadzoneY.Visible = false; incDeadzoneY.Enabled = false;
                decDeadzoneY.Visible = false; decDeadzoneY.Enabled = false;

            }
            
        }


        // update scroll bars and dedicated +/- buttons

        private void speedXBar_Scroll(object sender, EventArgs e)
        {
            speedXLabel.Text = speedXBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                speedYBar.Value = speedXBar.Value;
                speedYLabel.Text = speedYBar.Value.ToString();
            }
        }

        private void decSpeedX_Click(object sender, EventArgs e)
        {
            if (speedXBar.Value >= speedXBar.Minimum + SPEED_CHANGE_STEP)
                speedXBar.Value -= SPEED_CHANGE_STEP;
            speedXLabel.Text = speedXBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                speedYBar.Value = speedXBar.Value;
                speedYLabel.Text = speedYBar.Value.ToString();
            }

        }
        private void decSpeedX_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decSpeedX_Click);
            clickTimer.Start();
        }
        
        private void incSpeedX_Click(object sender, EventArgs e)
        {
            if (speedXBar.Value <= speedXBar.Maximum - SPEED_CHANGE_STEP)
                speedXBar.Value += SPEED_CHANGE_STEP;
            speedXLabel.Text = speedXBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                speedYBar.Value = speedXBar.Value;
                speedYLabel.Text = speedYBar.Value.ToString();
            }
        }
        private void incSpeedX_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incSpeedX_Click);
            clickTimer.Start();
        }


        private void speedYBar_Scroll(object sender, EventArgs e)
        {
            speedYLabel.Text = speedYBar.Value.ToString();
        }

        private void decSpeedY_Click(object sender, EventArgs e)
        {
            if (speedYBar.Value >= speedYBar.Minimum + SPEED_CHANGE_STEP)
                speedYBar.Value -= SPEED_CHANGE_STEP;
            speedYLabel.Text = speedYBar.Value.ToString();
        }
        private void decSpeedY_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decSpeedY_Click);
            clickTimer.Start();
        }

        private void incSpeedY_Click(object sender, EventArgs e)
        {
            if (speedYBar.Value <= speedYBar.Maximum - SPEED_CHANGE_STEP)
                speedYBar.Value += SPEED_CHANGE_STEP;
            speedYLabel.Text = speedYBar.Value.ToString();
        }
        private void incSpeedY_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incSpeedY_Click);
            clickTimer.Start();
        }


        private void deadzoneX_Scroll(object sender, EventArgs e)
        {
            deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                deadzoneYBar.Value = deadzoneXBar.Value;
                deadzoneYLabel.Text = deadzoneYBar.Value.ToString();
            }
        }
        private void decDeadzoneX_Click(object sender, EventArgs e)
        {
            if (deadzoneXBar.Value >= deadzoneXBar.Minimum + DEADZONE_CHANGE_STEP)
                deadzoneXBar.Value -= DEADZONE_CHANGE_STEP;
            deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                deadzoneYBar.Value = deadzoneXBar.Value;
                deadzoneYLabel.Text = deadzoneXBar.Value.ToString();
            }
        }
        private void decDeadzoneX_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decDeadzoneX_Click);
            clickTimer.Start();
        }

        private void incDeadzoneX_Click(object sender, EventArgs e)
        {
            if (deadzoneXBar.Value <= deadzoneXBar.Maximum - DEADZONE_CHANGE_STEP)
                deadzoneXBar.Value += DEADZONE_CHANGE_STEP;
            deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                deadzoneYBar.Value = deadzoneXBar.Value;
                deadzoneYLabel.Text = deadzoneXBar.Value.ToString();
            }
        }
        private void incDeadzoneX_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incDeadzoneX_Click);
            clickTimer.Start();
        }

        private void deadzoneYBar_Scroll(object sender, EventArgs e)
        {
            deadzoneYLabel.Text = deadzoneYBar.Value.ToString();

        }

        private void decDeadzoneY_Click(object sender, EventArgs e)
        {
            if (deadzoneYBar.Value >= deadzoneYBar.Minimum + DEADZONE_CHANGE_STEP)
                deadzoneYBar.Value -= DEADZONE_CHANGE_STEP;
            deadzoneYLabel.Text = deadzoneYBar.Value.ToString();
        }
        private void decDeadzoneY_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decDeadzoneY_Click);
            clickTimer.Start();
        }

        private void incDeadzoneY_Click(object sender, EventArgs e)
        {
            if (deadzoneYBar.Value <= deadzoneYBar.Maximum - DEADZONE_CHANGE_STEP)
                deadzoneYBar.Value += DEADZONE_CHANGE_STEP;
            deadzoneYLabel.Text = deadzoneYBar.Value.ToString();
        }
        private void incDeadzoneY_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incDeadzoneY_Click);
            clickTimer.Start();
        }


        private void puffThresholdBar_Scroll(object sender, EventArgs e)
        {
            puffThresholdLabel.Text = puffThresholdBar.Value.ToString();
        }

        private void decPuffThreshold_Click(object sender, EventArgs e)
        {
            if (puffThresholdBar.Value >= puffThresholdBar.Minimum + PRESSURE_CHANGE_STEP)
                puffThresholdBar.Value -= PRESSURE_CHANGE_STEP;
            puffThresholdLabel.Text = puffThresholdBar.Value.ToString();

        }
        private void decPuffThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decPuffThreshold_Click);
            clickTimer.Start();
        }

        private void incPuffThreshold_Click(object sender, EventArgs e)
        {
            if (puffThresholdBar.Value <= puffThresholdBar.Maximum - PRESSURE_CHANGE_STEP)
                puffThresholdBar.Value += PRESSURE_CHANGE_STEP;
            puffThresholdLabel.Text = puffThresholdBar.Value.ToString();
        }
        private void incPuffThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incPuffThreshold_Click);
            clickTimer.Start();
        }


        private void sipThresholdBar_Scroll(object sender, EventArgs e)
        {
            sipThresholdLabel.Text = sipThresholdBar.Value.ToString();
        }

        private void decSipThreshold_Click(object sender, EventArgs e)
        {
            if (sipThresholdBar.Value >= sipThresholdBar.Minimum + PRESSURE_CHANGE_STEP)
                sipThresholdBar.Value -= PRESSURE_CHANGE_STEP;
            sipThresholdLabel.Text = sipThresholdBar.Value.ToString();

        }
        private void decSipThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decSipThreshold_Click);
            clickTimer.Start();
        }

        private void incSipThreshold_Click(object sender, EventArgs e)
        {
            if (sipThresholdBar.Value <= sipThresholdBar.Maximum - PRESSURE_CHANGE_STEP)
                sipThresholdBar.Value += PRESSURE_CHANGE_STEP;
            sipThresholdLabel.Text = sipThresholdBar.Value.ToString();

        }
        private void incSipThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incSipThreshold_Click);
            clickTimer.Start();
        }

        private void specialThresholdBar_Scroll(object sender, EventArgs e)
        {
            specialThresholdLabel.Text =  specialThresholdBar.Value.ToString();
        }
        private void decSpecialThreshold_Click(object sender, EventArgs e)
        {
            if (specialThresholdBar.Value >= specialThresholdBar.Minimum + SPECIALMODE_CHANGE_STEP)
                specialThresholdBar.Value -= SPECIALMODE_CHANGE_STEP;
            specialThresholdLabel.Text = specialThresholdBar.Value.ToString();
        }
        private void decSpecialThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decSpecialThreshold_Click);
            clickTimer.Start();
        }

        private void incSpecialThreshold_Click(object sender, EventArgs e)
        {
            if (specialThresholdBar.Value <= specialThresholdBar.Maximum - SPECIALMODE_CHANGE_STEP)
                specialThresholdBar.Value += SPECIALMODE_CHANGE_STEP;
            specialThresholdLabel.Text = specialThresholdBar.Value.ToString();
        }
        private void incSpecialThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incSpecialThreshold_Click);
            clickTimer.Start();
        }

        private void holdThresholdBar_Scroll(object sender, EventArgs e)
        {
            holdThresholdLabel.Text = holdThresholdBar.Value.ToString();
        }


        private void incHoldThreshold_Click(object sender, EventArgs e)
        {
            if (holdThresholdBar.Value <= holdThresholdBar.Maximum - SPECIALMODE_CHANGE_STEP)
                holdThresholdBar.Value += SPECIALMODE_CHANGE_STEP;
            holdThresholdLabel.Text = holdThresholdBar.Value.ToString();
        }

        private void incHoldThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incHoldThreshold_Click);
            clickTimer.Start();
        }

        private void decHoldThreshold_Click(object sender, EventArgs e)
        {
            if (holdThresholdBar.Value >= holdThresholdBar.Minimum + SPECIALMODE_CHANGE_STEP)
                holdThresholdBar.Value -= SPECIALMODE_CHANGE_STEP;
            holdThresholdLabel.Text = holdThresholdBar.Value.ToString();

        }

        private void decHoldThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decHoldThreshold_Click);
            clickTimer.Start();
        }


        private void upGainBar_Scroll(object sender, EventArgs e)
        {
            upGainLabel.Text = upGainBar.Value.ToString();
        }

        private void incUpGain_Click(object sender, EventArgs e)
        {
            if (upGainBar.Value <= upGainBar.Maximum - GAIN_CHANGE_STEP)
                upGainBar.Value += GAIN_CHANGE_STEP;
            upGainLabel.Text = upGainBar.Value.ToString();

        }
        private void incUpGain_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incUpGain_Click);
            clickTimer.Start();
        }

        private void decUpGain_Click(object sender, EventArgs e)
        {
            if (upGainBar.Value >= upGainBar.Minimum + GAIN_CHANGE_STEP)
                upGainBar.Value -= GAIN_CHANGE_STEP;
            upGainLabel.Text = upGainBar.Value.ToString();
        }
        private void decUpGain_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decUpGain_Click);
            clickTimer.Start();
        }

        private void downGainBar_Scroll(object sender, EventArgs e)
        {
            downGainLabel.Text = (100-downGainBar.Value).ToString();

        }

        private void incDownGain_Click(object sender, EventArgs e)
        {
            if (downGainBar.Value >= downGainBar.Minimum + GAIN_CHANGE_STEP)
                downGainBar.Value -= GAIN_CHANGE_STEP;
            downGainLabel.Text = (100-downGainBar.Value).ToString();

        }
        private void incDownGain_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incDownGain_Click);
            clickTimer.Start();
        }

        private void decDownGain_Click(object sender, EventArgs e)
        {
            if (downGainBar.Value <= downGainBar.Maximum - GAIN_CHANGE_STEP)
                downGainBar.Value += GAIN_CHANGE_STEP;
            downGainLabel.Text = (100-downGainBar.Value).ToString();
        }
        private void decDownGain_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decDownGain_Click);
            clickTimer.Start();
        }

        private void leftGainBar_Scroll(object sender, EventArgs e)
        {
            leftGainLabel.Text = leftGainBar.Value.ToString();
        }

        private void incLeftGain_Click(object sender, EventArgs e)
        {
            if (leftGainBar.Value <= leftGainBar.Maximum - GAIN_CHANGE_STEP)
                leftGainBar.Value += GAIN_CHANGE_STEP;
            leftGainLabel.Text = leftGainBar.Value.ToString();
        }
        private void incLeftGain_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incLeftGain_Click);
            clickTimer.Start();
        }

        private void decLeftGain_Click(object sender, EventArgs e)
        {
            if (leftGainBar.Value >= leftGainBar.Minimum + GAIN_CHANGE_STEP)
                leftGainBar.Value -= GAIN_CHANGE_STEP;
            leftGainLabel.Text = leftGainBar.Value.ToString();

        }
        private void decLeftGain_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decLeftGain_Click);
            clickTimer.Start();
        }

        
        private void rightGainBar_Scroll(object sender, EventArgs e)
        {
            rightGainLabel.Text = rightGainBar.Value.ToString();
        }

        private void incRightGain_Click(object sender, EventArgs e)
        {
            if (rightGainBar.Value <= rightGainBar.Maximum - GAIN_CHANGE_STEP)
                rightGainBar.Value += GAIN_CHANGE_STEP;
            rightGainLabel.Text = rightGainBar.Value.ToString();
        }
        private void incRightGain_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incRightGain_Click);
            clickTimer.Start();
        }

        private void decRightGain_Click(object sender, EventArgs e)
        {
            if (rightGainBar.Value >= rightGainBar.Minimum + GAIN_CHANGE_STEP)
                rightGainBar.Value -= GAIN_CHANGE_STEP;
            rightGainLabel.Text = rightGainBar.Value.ToString();
        }
        private void decRightGain_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decRightGain_Click);
            clickTimer.Start();
        }

        void timer_Tick(object sender, EventArgs e)
        {
             Console.Write("*");
             functionPointer(this, null);
        }

        private void stop_ClickTimer(object sender, EventArgs e)
        {
            clickTimer.Stop();
        }

        private void slotNames_SelectedIndexChanged(object sender, EventArgs e)
        {
            actSlot = slotNames.SelectedIndex;
            displaySlot(actSlot);

        }

    }
}
