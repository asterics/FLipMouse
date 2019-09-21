

/* 
     FLipMouseGUI - Graphical user Interface for FlipMouse / FlipWare firmware
     http://www.asterics-academy.net
   
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
        const string VERSION_STRING = "2.81";
        const int MAX_KEYSTRING_LEN = 65;

        const int SENS_CHANGE_STEP = 1;
        const int DEADZONE_CHANGE_STEP = 1;
        const int STRONGMODE_CHANGE_STEP = 1;
        const int PRESSURE_CHANGE_STEP = 1;
        const int GAIN_CHANGE_STEP = 1;

        Boolean readDone = false;
        static int slotCounter = 0;
        static int actSlot = 0;
        static int checkVersion = 1;

        TabPage saveAlternativeTab = new TabPage();

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
            slotNames.Items.Add("mouse");
        }

        public void copyToAllSlots(int slotNumber, string cmd)
        {
            string replaceWith="AT NC";

            if (slotNumber < 0) return;

            for (int i = 0; i < slots[slotNumber].settingStrings.Count(); i++)
            {
                string c = slots[slotNumber].settingStrings[i];
                if (c.StartsWith(cmd))
                {
                    if (cmd.StartsWith("AT BM "))
                        replaceWith = slots[slotNumber].settingStrings[i + 1];
                    else replaceWith = c;
                    // Console.WriteLine("replace with:" + replaceWith);
                    break;
                }
            }


            foreach (Slot s in slots)
            {
                // Console.WriteLine("SLOT " + s.slotName);

                for (int x=0; x < s.settingStrings.Count();x++)
                {
                    string str =s.settingStrings[x];
                    // Console.WriteLine(str);
                    if (str.StartsWith(cmd)) {
                        // Console.WriteLine("-->> replace!");
                        if (cmd.StartsWith("AT BM ")) 
                            s.settingStrings[x+1]=replaceWith;
                        else 
                            s.settingStrings[x]=replaceWith;
                        break;
                    }
                }
            }
        }

        public void storeSlot(int slotNumber)
        {
            if (slotNumber < 0) return;
            slots[slotNumber].settingStrings.Clear();
            slots[slotNumber].slotName = slotNames.Text;
            slotNames.Items.Clear();
            foreach (Slot s in slots) slotNames.Items.Add(s.slotName); 


            foreach (CommandGuiLink guiLink in commandGuiLinks)
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
                    case GUITYPE_IRSELECT:
                    case GUITYPE_TEXTFIELD: actSettingString += (" " + guiLink.tb.Text); break;
                    case GUITYPE_COMBO_ONLY: actSettingString += (" " + guiLink.cb.Text); break;
                    case GUITYPE_COMBO_INDEX: actSettingString += (" " + guiLink.cb.SelectedIndex); break;
                    case GUITYPE_SLIDER: actSettingString += (" " + guiLink.tr.Value); break;
                    case GUITYPE_3RADIOBUTTONS: 
                        if (guiLink.rb1.Checked) actSettingString += (" 1"); 
                        else if (guiLink.rb3.Checked) actSettingString += (" 0"); 
                        else if (guiLink.rb2.Checked) actSettingString += (" "+(joyModeBox.SelectedIndex +2));
                        break;
                    case GUITYPE_STANDARD: break;
                }
                slots[slotNumber].settingStrings.Add(actSettingString);
            }
            if (irIdleSequenceBox.Text != "")
            {
                slots[slotNumber].settingStrings.Add("AT IO " + irIdleSequenceBox.Text);
            }
        }

        public void displaySlot(int slotNumber)
        {
            CommandGuiLink actButtonLink = null;
            String strValue="";

            slotNames.Text = slots[slotNumber].slotName;
            foreach (String settingString in slots[slotNumber].settingStrings)
            {
                if (settingString == null) { Console.WriteLine("null value detected **** SlotNr."+slotNumber); break; }
                if (actButtonLink != null)
                {
                    String cmd = settingString.Substring(0, 5);
                    switch (allCommands.getGuiTypeFromCommand(cmd))
                    {
                        case GUITYPE_INTFIELD: actButtonLink.cb.SelectedIndex = allCommands.getSelectionIndex(cmd);
                            strValue = settingString.Substring(6);
                            actButtonLink.nud.Value = Int32.Parse(strValue); break;
                        case GUITYPE_KEYSELECT:
                        case GUITYPE_IRSELECT:
                        case GUITYPE_COMBO_ONLY:
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

                else foreach (CommandGuiLink guiLink in commandGuiLinks)
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
                                case GUITYPE_COMBO_ONLY: guiLink.cb.Text = settingString.Substring(6);
                                    break;
                                case GUITYPE_COMBO_INDEX: 
                                    strValue = settingString.Substring(guiLink.cmd.Length + 1);
                                    guiLink.cb.SelectedIndex = Int32.Parse(strValue);
                                    break;
                                case GUITYPE_3RADIOBUTTONS: 
                                    strValue = settingString.Substring(guiLink.cmd.Length + 1);
                                    int value= Int32.Parse(strValue);
                                    switch (value) {
                                        case 0: guiLink.rb1.Checked = false; 
                                                guiLink.rb2.Checked = false;
                                                guiLink.rb3.Checked = true;
                                                break;
                                        case 1: guiLink.rb1.Checked = true; 
                                                guiLink.rb2.Checked = false;
                                                guiLink.rb3.Checked = false;
                                                break;
                                        case 2: case 3: case 4 :
                                                guiLink.rb1.Checked = false; 
                                                guiLink.rb2.Checked = true;
                                                guiLink.rb3.Checked = false;
                                                joyModeBox.SelectedIndex = value - 2;
                                                break;
                                    }
                                    break;

                            }
                        }
                    }
                }
            }
            sensBar.Value=sensXBar.Value;
            sensLabel.Text = sensBar.Value.ToString();
            deadzoneBar.Value = deadzoneXBar.Value;
            deadzoneLabel.Text = deadzoneBar.Value.ToString();

            if ((sensXBar.Value != sensYBar.Value) ||
                (deadzoneXBar.Value != deadzoneYBar.Value))
                splitXYBox.Checked = true;
            else splitXYBox.Checked = false;

            splitXYBox_CheckedChanged(this, null);
        }


        public FLipMouseGUI(string[] args)
        {
            if (args.Length == 1)
            {
                if (args[0].Equals("-noversioncheck")) checkVersion=0;
            }
                
            InitializeComponent();

            initCommands();
            initCommandGuiLinks();
            initSlots();

            clickTimer.Interval = 500; // specify interval time as you want
            clickTimer.Tick += new EventHandler(timer_Tick);

            IdTimer.Tick += new EventHandler(IdTimer_Tick);

            Text += VERSION_STRING;

            saveAlternativeTab = tabControl.TabPages[1];
            tabControl.TabPages.Remove(tabControl.TabPages[1]);  // default: no alternative actions

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
                    StrongSipFunctionMenu.Items.Add(str);
                    PuffFunctionMenu.Items.Add(str);
                    StrongPuffFunctionMenu.Items.Add(str);
                    StrongSipUpFunctionBox.Items.Add(str);
                    StrongSipDownFunctionBox.Items.Add(str);
                    StrongSipLeftFunctionBox.Items.Add(str);
                    StrongSipRightFunctionBox.Items.Add(str);
                    StrongPuffUpFunctionBox.Items.Add(str);
                    StrongPuffDownFunctionBox.Items.Add(str);
                    StrongPuffLeftFunctionBox.Items.Add(str);
                    StrongPuffRightFunctionBox.Items.Add(str);
                }
            }
         
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
                StrongSipComboBox.Items.Add(str);
                PuffComboBox.Items.Add(str);
                StrongPuffComboBox.Items.Add(str);
                StrongSipUpComboBox.Items.Add(str);
                StrongSipDownComboBox.Items.Add(str);
                StrongSipLeftComboBox.Items.Add(str);
                StrongSipRightComboBox.Items.Add(str);
                StrongPuffUpComboBox.Items.Add(str);
                StrongPuffDownComboBox.Items.Add(str);
                StrongPuffLeftComboBox.Items.Add(str);
                StrongPuffRightComboBox.Items.Add(str);

            }

            joyModeBox.Items.Add("X/Y-Axis");
            joyModeBox.Items.Add("Z/Rotation-Axis");
            joyModeBox.Items.Add("Slider1/Slider2");
            joyModeBox.SelectedIndex = 0;

            orientationBox.Items.Add("0");
            orientationBox.Items.Add("90");
            orientationBox.Items.Add("180");
            orientationBox.Items.Add("270");
            orientationBox.SelectedIndex = 0;

            HIDComboBox.Items.Add("disable");
            HIDComboBox.Items.Add("USB HID only");
            HIDComboBox.Items.Add("Bluetooth HID only");
            HIDComboBox.Items.Add("both (USB + BT)");
            HIDComboBox.SelectedIndex = 2;

            displaySlot(0);

            updateComPorts();

            addToLog("FLipMouse GUI ready!");
            this.Load += LipmouseGUI_Load;
            this.FormClosed += MyWindow_Closing;
        }


        private void connectComButton_click(object sender, EventArgs e) //select button
        {
            storeSlot(actSlot);
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
                        StoreButton.Enabled = true;
                        playIRButton.Enabled = true;
                        recordIRButton.Enabled = true;
                        deleteIRButton.Enabled = true;
                        deleteAllIRButton.Enabled = true;
                        connectComButton.Enabled = false;

                        IdTimer.Interval = 1500;
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
            StoreButton.Enabled = false;
            playIRButton.Enabled = false;
            recordIRButton.Enabled = false;
            deleteIRButton.Enabled = false;
            deleteAllIRButton.Enabled = false;
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

        private void selectStick_Checked(object sender, EventArgs e)
        {
           // ((Control)tabControl.TabPages[1]).Enabled = false;
            if (tabControl.TabPages[1].Name == "AlternativeTab")
                tabControl.TabPages.Remove(tabControl.TabPages[1]);
            joyModeBox.Visible = false;
            joyModeLabel.Visible = false;

        }

        private void selectJoystick_CheckedChanged(object sender, EventArgs e)
        {
            //((Control)tabControl.TabPages[1]).Enabled = false;
            if (tabControl.TabPages[1].Name == "AlternativeTab") 
              tabControl.TabPages.Remove(tabControl.TabPages[1]);
            joyModeBox.Visible = true;
            joyModeLabel.Visible = true;
        }

        private void selectAlternative_Checked(object sender, EventArgs e)
        {
         //   ((Control)tabControl.TabPages[1]).Enabled = true;
            if (tabControl.TabPages[1].Name != "AlternativeTab")
                tabControl.TabPages.Insert(1, saveAlternativeTab);
            joyModeBox.Visible = false;
            joyModeLabel.Visible = false;
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
                int cnt=1;
                Boolean exists;
                String newSlotName;

                storeSlot(actSlot);
                do
                {
                    newSlotName = "Slot" + cnt++;
                    exists = false;
                    foreach (Slot s in slots)
                        if (s.slotName.Equals(newSlotName)) exists = true;
                } while (exists == true);
                
                slotNames.Items.Add(newSlotName);
                slots.Add(new Slot(newSlotName));

                slots[slots.Count - 1].settingStrings.Clear();
                foreach (string s in slots[actSlot].settingStrings)
                    slots[slots.Count - 1].settingStrings.Add(s);
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
            storeSlot(actSlot);   
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
                case GUITYPE_TEXTFIELD: la.Visible = true; la.Text = "    Text:"; nud.Visible = false; tb.Enabled = true; tb.ReadOnly = false; tb.Visible = true; cb.Visible = false; bu.Visible = true; bu.Enabled = true; tb.Text = ""; break; 
                case GUITYPE_IRSELECT: // combo box used for IR commands
                                        la.Visible = true; la.Text = " IR-Code:"; nud.Visible = false; tb.Text = "";  tb.Visible = true; tb.ReadOnly = true; cb.Visible = true; bu.Visible = true; bu.Enabled = true; 
                                        cb.Items.Clear(); foreach (Object itm in irCommandBox.Items) cb.Items.Add(itm.ToString()); cb.Text="";break;
                case GUITYPE_KEYSELECT: // combo box used for keycodes 
                                        la.Visible = true; la.Text = "KeyCodes:"; nud.Visible = false;  tb.Visible = true; tb.ReadOnly = true; cb.Visible = true; bu.Visible = true; bu.Enabled = true; if (!tb.Text.StartsWith("KEY_")) tb.Text = ""; 
                                        cb.Items.Clear(); foreach (string str in keyOptions) cb.Items.Add(str); cb.Text="";break;
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

        private void StrongSipFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongSipFunctionMenu.Text, StrongSipParameterText, StrongSipNumericParameter, StrongSipComboBox, SpecialSipParameterLabel, clearButtonStrongSip);
        }

        private void PuffFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(PuffFunctionMenu.Text, PuffParameterText, PuffNumericParameter, PuffComboBox, PuffParameterLabel, clearButtonPuff);
        }

        private void StrongPuffFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongPuffFunctionMenu.Text, StrongPuffParameterText, StrongPuffNumericParameter, StrongPuffComboBox, SpecialPuffParameterLabel, clearButtonStrongPuff);
        }

        private void StrongSipUpFunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongSipUpFunctionBox.Text, StrongSipUpParameterText, StrongSipUpNumericParameter, StrongSipUpComboBox, StrongSipUpLabel, clearButtonStrongSipUp);
        }

        private void StrongSipDownFunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongSipDownFunctionBox.Text, StrongSipDownParameterText, StrongSipDownNumericParameter, StrongSipDownComboBox, StrongSipDownLabel, clearButtonStrongSipDown);
        }

        private void StrongSipLeftFunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongSipLeftFunctionBox.Text, StrongSipLeftParameterText, StrongSipLeftNumericParameter, StrongSipLeftComboBox, StrongSipLeftLabel, clearButtonStrongSipLeft);
        }

        private void StrongSipRightFunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongSipRightFunctionBox.Text, StrongSipRightParameterText, StrongSipRightNumericParameter, StrongSipRightComboBox, StrongSipRightLabel, clearButtonStrongSipRight);
        }
        
        private void StrongPuffUpFunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongPuffUpFunctionBox.Text, StrongPuffUpParameterText, StrongPuffUpNumericParameter, StrongPuffUpComboBox, StrongPuffUpLabel, clearButtonStrongPuffUp);
        }

        private void StrongPuffDownFunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongPuffDownFunctionBox.Text, StrongPuffDownParameterText, StrongPuffDownNumericParameter, StrongPuffDownComboBox, StrongPuffDownLabel, clearButtonStrongPuffDown);
        }

        private void StrongPuffLeftFunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongPuffLeftFunctionBox.Text, StrongPuffLeftParameterText, StrongPuffLeftNumericParameter, StrongPuffLeftComboBox, StrongPuffLeftLabel, clearButtonStrongPuffLeft);
        }

        private void StrongPuffRightFunctionBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(StrongPuffRightFunctionBox.Text, StrongPuffRightParameterText, StrongPuffRightNumericParameter, StrongPuffRightComboBox, StrongPuffRightLabel, clearButtonStrongPuffRight);
        }


        // update the keycode parameters:

        private void updateKeyCodeParameter(ComboBox cb, TextBox tb)
        {
            if (cb.Text.ToString().StartsWith("KEY_"))   // combo box used for Keycodes
            {
                String add = cb.Text.ToString() + " ";
                if (!tb.Text.Contains(add))
                    tb.Text += add;
            }
            else tb.Text = cb.Text.ToString();    // combo box used for ir command names
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

        private void StrongSipComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongSipComboBox, StrongSipParameterText);
        }
        private void clearButtonStrongSip_Click(object sender, EventArgs e)
        {
            StrongSipParameterText.Text = "";
        }

        private void PuffComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(PuffComboBox, PuffParameterText);
        }
        private void clearButtonPuff_Click(object sender, EventArgs e)
        {
            PuffParameterText.Text = "";
        }

        private void StrongPuffComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongPuffComboBox, StrongPuffParameterText);
        }
        private void clearButtonStrongPuff_Click(object sender, EventArgs e)
        {
            StrongPuffParameterText.Text = "";
        }

        private void StrongSipUpComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongSipUpComboBox, StrongSipUpParameterText);
        }
        private void clearStrongSipUp_Click(object sender, EventArgs e)
        {
            StrongSipUpParameterText.Text = "";
        }

        private void StrongSipDownComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongSipDownComboBox, StrongSipDownParameterText);
        }
        private void clearStrongSipDown_Click(object sender, EventArgs e)
        {
            StrongSipDownParameterText.Text = "";
        }

        private void StrongSipLeftComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongSipLeftComboBox, StrongSipLeftParameterText);
        }
        private void clearStrongSipLeft_Click(object sender, EventArgs e)
        {
            StrongSipLeftParameterText.Text = "";
        }

        private void StrongSipRightComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongSipRightComboBox, StrongSipRightParameterText);
        }
        private void clearStrongSipRight_Click(object sender, EventArgs e)
        {
            StrongSipRightParameterText.Text = "";
        }

        private void StrongPuffUpComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongPuffUpComboBox, StrongPuffUpParameterText);
        }
        private void clearStrongPuffUp_Click(object sender, EventArgs e)
        {
            StrongPuffUpParameterText.Text = "";
        }

        private void StrongPuffDownComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongPuffDownComboBox, StrongPuffDownParameterText);
        }
        private void clearStrongPuffDown_Click(object sender, EventArgs e)
        {
            StrongPuffDownParameterText.Text = "";
        }

        private void StrongPuffLeftComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongPuffLeftComboBox, StrongPuffLeftParameterText);
        }
        private void clearStrongPuffLeft_Click(object sender, EventArgs e)
        {
            StrongPuffLeftParameterText.Text = "";
        }

        private void StrongPuffRightComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(StrongPuffRightComboBox, StrongPuffRightParameterText);
        }
        private void clearStrongPuffRight_Click(object sender, EventArgs e)
        {
            StrongPuffRightParameterText.Text = "";
        }



        private void splitXYBox_CheckedChanged(object sender, EventArgs e)
        {
            if (splitXYBox.Checked)
            {
                splitPanel.Visible = true;
                splitPanel.Enabled = true;
                singlePanel.Visible = false;
                singlePanel.Enabled = false;
            }
            else
            {
                splitPanel.Visible = false;
                splitPanel.Enabled = false;
                singlePanel.Visible = true;
                singlePanel.Enabled = true;
            }

        }


        // update scroll bars and dedicated +/- buttons

        private void updateGangBars()
        {
            sensXBar.Value = sensBar.Value;
            sensXLabel.Text = sensBar.Value.ToString();
            sensYBar.Value = sensBar.Value;
            sensYLabel.Text = sensBar.Value.ToString();
            
            deadzoneXBar.Value = deadzoneBar.Value;
            deadzoneXLabel.Text = deadzoneBar.Value.ToString();
            deadzoneYBar.Value = deadzoneBar.Value;
            deadzoneYLabel.Text = deadzoneBar.Value.ToString();
        }

        private void sensBar_Scroll(object sender, EventArgs e)
        {
            sensLabel.Text  = sensBar.Value.ToString();
            updateGangBars();
        }
        private void decSens_Click(object sender, EventArgs e)
        {
            if (sensBar.Value >= sensBar.Minimum + SENS_CHANGE_STEP)
                sensBar.Value -= SENS_CHANGE_STEP;
            sensLabel.Text = sensBar.Value.ToString();
            updateGangBars();
        }
        private void decSens_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decSens_Click);
            clickTimer.Start();
        }
        private void incSens_Click(object sender, EventArgs e)
        {
            if (sensBar.Value <= sensBar.Maximum - SENS_CHANGE_STEP)
                sensBar.Value += SENS_CHANGE_STEP;
            sensLabel.Text = sensBar.Value.ToString();
            updateGangBars();
        }
        private void incSens_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incSens_Click);
            clickTimer.Start();
        }

        private void deadzoneBar_Scroll(object sender, EventArgs e)
        {
            deadzoneLabel.Text  = deadzoneBar.Value.ToString();
            updateGangBars();
        }
        private void decDeadzone_Click(object sender, EventArgs e)
        {
            if (deadzoneBar.Value >= deadzoneBar.Minimum + DEADZONE_CHANGE_STEP)
                deadzoneBar.Value -= DEADZONE_CHANGE_STEP;
            deadzoneLabel.Text = deadzoneBar.Value.ToString();
            updateGangBars();
        }
        private void decDeadzone_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decDeadzone_Click);
            clickTimer.Start();
        }
        private void incDeadzone_Click(object sender, EventArgs e)
        {
            if (deadzoneBar.Value <= deadzoneBar.Maximum - DEADZONE_CHANGE_STEP)
                deadzoneBar.Value += DEADZONE_CHANGE_STEP;
            deadzoneLabel.Text = deadzoneBar.Value.ToString();
            updateGangBars();
        }
        private void incDeadzone_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incDeadzone_Click);
            clickTimer.Start();
        }

        private void sensXBar_Scroll(object sender, EventArgs e)
        {
            sensXLabel.Text = sensXBar.Value.ToString();
        }
        private void decSensX_Click(object sender, EventArgs e)
        {
            if (sensXBar.Value >= sensXBar.Minimum + SENS_CHANGE_STEP)
                sensXBar.Value -= SENS_CHANGE_STEP;
            sensXLabel.Text = sensXBar.Value.ToString();
        }
        private void decSensX_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decSensX_Click);
            clickTimer.Start();
        }
        private void incSensX_Click(object sender, EventArgs e)
        {
            if (sensXBar.Value <= sensXBar.Maximum - SENS_CHANGE_STEP)
                sensXBar.Value += SENS_CHANGE_STEP;
            sensXLabel.Text = sensXBar.Value.ToString();
        }
        private void incSensX_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incSensX_Click);
            clickTimer.Start();
        }


        private void sensYBar_Scroll(object sender, EventArgs e)
        {
            sensYLabel.Text = sensYBar.Value.ToString();
        }
        private void decSensY_Click(object sender, EventArgs e)
        {
            if (sensYBar.Value >= sensYBar.Minimum + SENS_CHANGE_STEP)
                sensYBar.Value -= SENS_CHANGE_STEP;
            sensYLabel.Text = sensYBar.Value.ToString();
        }
        private void decSensY_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decSensY_Click);
            clickTimer.Start();
        }
        private void incSensY_Click(object sender, EventArgs e)
        {
            if (sensYBar.Value <= sensYBar.Maximum - SENS_CHANGE_STEP)
                sensYBar.Value += SENS_CHANGE_STEP;
            sensYLabel.Text = sensYBar.Value.ToString();
        }
        private void incSensY_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incSensY_Click);
            clickTimer.Start();
        }

        private void deadzoneX_Scroll(object sender, EventArgs e)
        {
            deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
        }
        private void decDeadzoneX_Click(object sender, EventArgs e)
        {
            if (deadzoneXBar.Value >= deadzoneXBar.Minimum + DEADZONE_CHANGE_STEP)
                deadzoneXBar.Value -= DEADZONE_CHANGE_STEP;
            deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
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
        }
        private void incDeadzoneX_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incDeadzoneX_Click);
            clickTimer.Start();
        }

        private void deadzoneYBar_Scroll(object sender, EventArgs e)
        {
            deadzoneYLabel.Text = deadzoneYBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                deadzoneXBar.Value = deadzoneYBar.Value;
                deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
            }
        }
        private void decDeadzoneY_Click(object sender, EventArgs e)
        {
            if (deadzoneYBar.Value >= deadzoneYBar.Minimum + DEADZONE_CHANGE_STEP)
                deadzoneYBar.Value -= DEADZONE_CHANGE_STEP;
            deadzoneYLabel.Text = deadzoneYBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                deadzoneXBar.Value = deadzoneYBar.Value;
                deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
            }
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
            if (splitXYBox.Checked == false)
            {
                deadzoneXBar.Value = deadzoneYBar.Value;
                deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
            }
        }
        private void incDeadzoneY_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incDeadzoneY_Click);
            clickTimer.Start();
        }



        private void maxspeedBar_Scroll(object sender, EventArgs e)
        {
            maxspeedLabel.Text = maxspeedBar.Value.ToString();
        }

        private void decMaxspeed_Click(object sender, EventArgs e)
        {
            if (maxspeedBar.Value >= maxspeedBar.Minimum + SENS_CHANGE_STEP)
                maxspeedBar.Value -= SENS_CHANGE_STEP;
            maxspeedLabel.Text = maxspeedBar.Value.ToString();
        }
        private void decMaxspeed_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decMaxspeed_Click);
            clickTimer.Start();
        }
        private void incMaxspeed_Click(object sender, EventArgs e)
        {
            if (maxspeedBar.Value <= maxspeedBar.Maximum - SENS_CHANGE_STEP)
                maxspeedBar.Value += SENS_CHANGE_STEP;
            maxspeedLabel.Text = maxspeedBar.Value.ToString();
        }

        private void incMaxspeed_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incMaxspeed_Click);
            clickTimer.Start();
        }
 



        private void accelerationBar_Scroll(object sender, EventArgs e)
        {
            accelerationLabel.Text = accelerationBar.Value.ToString();
        }

        private void decAcceleration_Click(object sender, EventArgs e)
        {
            if (accelerationBar.Value >= accelerationBar.Minimum + SENS_CHANGE_STEP)
                accelerationBar.Value -= SENS_CHANGE_STEP;
            accelerationLabel.Text = accelerationBar.Value.ToString();
        }
        private void decAcceleration_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decAcceleration_Click);
            clickTimer.Start();
        }
        private void incAcceleration_Click(object sender, EventArgs e)
        {
            if (accelerationBar.Value <= accelerationBar.Maximum - SENS_CHANGE_STEP)
                accelerationBar.Value += SENS_CHANGE_STEP;
            accelerationLabel.Text = accelerationBar.Value.ToString();
        }
        private void incAcceleration_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incAcceleration_Click);
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

        private void strongPuffThresholdBar_Scroll(object sender, EventArgs e)
        {
            strongPuffThresholdLabel.Text =  strongPuffThresholdBar.Value.ToString();
        }
        private void decStrongPuffThreshold_Click(object sender, EventArgs e)
        {
            if (strongPuffThresholdBar.Value >= strongPuffThresholdBar.Minimum + STRONGMODE_CHANGE_STEP)
                strongPuffThresholdBar.Value -= STRONGMODE_CHANGE_STEP;
            strongPuffThresholdLabel.Text = strongPuffThresholdBar.Value.ToString();
        }
        private void decStrongPuffThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decStrongPuffThreshold_Click);
            clickTimer.Start();
        }
        private void incStrongPuffThreshold_Click(object sender, EventArgs e)
        {
            if (strongPuffThresholdBar.Value <= strongPuffThresholdBar.Maximum - STRONGMODE_CHANGE_STEP)
                strongPuffThresholdBar.Value += STRONGMODE_CHANGE_STEP;
            strongPuffThresholdLabel.Text = strongPuffThresholdBar.Value.ToString();
        }
        private void incSpecialThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incStrongPuffThreshold_Click);
            clickTimer.Start();
        }

        private void strongSipThresholdBar_Scroll(object sender, EventArgs e)
        {
            stongSipThresholdLabel.Text = strongSipThresholdBar.Value.ToString();
        }
        private void incStrongSipThreshold_Click(object sender, EventArgs e)
        {
            if (strongSipThresholdBar.Value <= strongSipThresholdBar.Maximum - STRONGMODE_CHANGE_STEP)
                strongSipThresholdBar.Value += STRONGMODE_CHANGE_STEP;
            stongSipThresholdLabel.Text = strongSipThresholdBar.Value.ToString();
        }
        private void incStrongSipThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incStrongSipThreshold_Click);
            clickTimer.Start();
        }
        private void decStrongSipThreshold_Click(object sender, EventArgs e)
        {
            if (strongSipThresholdBar.Value >= strongSipThresholdBar.Minimum + STRONGMODE_CHANGE_STEP)
                strongSipThresholdBar.Value -= STRONGMODE_CHANGE_STEP;
            stongSipThresholdLabel.Text = strongSipThresholdBar.Value.ToString();

        }
        private void decStrongSipThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decStrongSipThreshold_Click);
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

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show( "FLipMouse V"+VERSION_STRING +" - AsTeRICS Academy\nFor more information see: http://www.asterics-academy.net");
        }


        private void recordIRButton_Click(object sender, EventArgs e)
        {
            if (irCommandBox.Text.Length == 0) return;

            int timeout = Int32.Parse(irTimeoutBox.Text);
            if ((timeout < 1) || (timeout > 10000)) return;

            addToLog("Recording IR Command " + irCommandBox.Text + ", timeout=" + irTimeoutBox.Text+"ms.");
            sendCmd("AT IT " + irTimeoutBox.Text);
            sendRecordIRCommand(irCommandBox.Text);
        }

        private void playIRButton_Click(object sender, EventArgs e)
        {
            if (irCommandBox.Text.Length == 0) return;
            addToLog("Playing IR Command " + irCommandBox.Text);
            sendPlayIRCommand(irCommandBox.Text);
            
        }


        private void deleteIRButton_Click(object sender, EventArgs e)
        {
            if (irCommandBox.Text.Length == 0) return;
            addToLog("Deleting IR Command " + irCommandBox.Text);
            sendClearIRCommand(irCommandBox.Text);

            irCommandBox.Items.Clear();
            irIdleSequenceBox.Items.Clear();
            sendListIRCommand();
        }

        private void deleteAllIRButton_Click(object sender, EventArgs e)
        {
            addToLog("Deleting IR Commands ");
            sendClearIRCommands();

            irCommandBox.Items.Clear();
            irIdleSequenceBox.Items.Clear();
            sendListIRCommand();
        }

        private void StoreButton_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            storeSettingsToFLipmouse();
        }

        private void clearIROffButton_Click(object sender, EventArgs e)
        {
            irIdleSequenceBox.Text = "";
        }

        private void copy_orientation_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            copyToAllSlots(actSlot, "AT RO");
        }

        private void copy_btmode_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            copyToAllSlots(actSlot, "AT BT");
        }

        private void copy_buttons_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            copyToAllSlots(actSlot, "AT BM 01");
            copyToAllSlots(actSlot, "AT BM 02");
            copyToAllSlots(actSlot, "AT BM 03");

        }

        private void copy_levels_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            copyToAllSlots(actSlot, "AT TS");
            copyToAllSlots(actSlot, "AT TP");
            copyToAllSlots(actSlot, "AT SS");
            copyToAllSlots(actSlot, "AT SP");

        }

        private void copy_stickactions_Click(object sender, EventArgs e)
        {
            storeSlot(actSlot);
            copyToAllSlots(actSlot, "AT AX");
            copyToAllSlots(actSlot, "AT AY");
            copyToAllSlots(actSlot, "AT DX");
            copyToAllSlots(actSlot, "AT DY");
            copyToAllSlots(actSlot, "AT MS");
            copyToAllSlots(actSlot, "AT AC");

        }

    }
}
