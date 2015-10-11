
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
using System.IO.Ports;
using System.IO;
using System.Text.RegularExpressions;




namespace MouseApp2
{
    public partial class FLipMouseGUI : Form
    {
        const string VERSION_STRING = "1.0";

        const int SPEED_CHANGE_STEP = 2;
        const int DEADZONE_CHANGE_STEP = 10;
        const int TIME_CHANGE_STEP = 50;
        const int PRESSURE_CHANGE_STEP = 1;
        const int GAIN_CHANGE_STEP = 2;

        const int CMD_NOACTION     = 0;
        const int CMD_NEXT         = 1;
        const int CMD_CLICK_LEFT   = 2;
        const int CMD_CLICK_RIGHT  = 3;
        const int CMD_CLICK_MIDDLE = 4;
        const int CMD_CLICK_DOUBLE = 5;
        const int CMD_PRESS_LEFT   = 6;
        const int CMD_PRESS_RIGHT  = 7;
        const int CMD_PRESS_MIDDLE = 8;
        const int CMD_WHEEL_UP     = 9;
        const int CMD_WHEEL_DOWN   = 10;
        const int CMD_MOVE_X       = 11;
        const int CMD_MOVE_Y       = 12;
        const int CMD_WRITE_TEXT   = 13;
        const int CMD_PRESS_KEYS   = 14;
        const int CMD_CALIBRATE    = 15;
        const int CMD_SWITCH_ALTER = 16;

        const String TOKEN_SEPERATOR = "-,-";


        String[] commands = {  "No Action", "Switch to next configuration", 
                               "Click Left Mouse Button", "Click Right Mouse Button", "Click Middle Mouse Button" , "Double Click Left Mouse Button",
                               "Hold Left Mouse Button", "Hold Right Mouse Button", "Hold Middle Mouse Button", 
                               "Wheel Up", "Wheel down",  "Move Mouse X", "Move Mouse Y",
                               "Write Text", "Press Keys", "Calibrate Middle Position", "Switch Cursor/Alternative"
                             };
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
        String receivedString = "";
        Boolean readDone = false;
        Boolean storingToFile = false;
        static int slotCounter = 0;
        String firstSlotname = null;

        System.IO.StreamWriter settingsFile = null;

        public delegate void RawValuesDelegate(string newValues);
        public RawValuesDelegate rawValuesDelegate;

        public delegate void SlotValuesDelegate(string newValues);
        public SlotValuesDelegate slotValuesDelegate;

        public delegate void LoadValuesDelegate(string newValues);
        public LoadValuesDelegate loadValuesDelegate;

        public delegate void StoreValuesDelegate(string newValues);
        public StoreValuesDelegate storeValuesDelegate;

        public delegate void ClickDelegate(object sender, EventArgs e);
        ClickDelegate functionPointer;

        System.Windows.Forms.Timer clickTimer = new System.Windows.Forms.Timer();

        public FLipMouseGUI()
        {

            InitializeComponent();

            clickTimer.Interval = 500; // specify interval time as you want
            clickTimer.Tick += new EventHandler(timer_Tick);

            Text += " "+ VERSION_STRING;
            foreach (string str in commands)
            {
                Button1FunctionBox.Items.Add(str);
                Button2FunctionBox.Items.Add(str);
                Button3FunctionBox.Items.Add(str);
                UpFunctionMenu.Items.Add(str);
                DownFunctionMenu.Items.Add(str);
                LeftFunctionMenu.Items.Add(str);
                RightFunctionMenu.Items.Add(str);
                SipFunctionMenu.Items.Add(str);
                LongSipFunctionMenu.Items.Add(str);
                PuffFunctionMenu.Items.Add(str);
                LongPuffFunctionMenu.Items.Add(str);
            }

            Button1FunctionBox.SelectedIndex = CMD_NEXT;
            Button2FunctionBox.SelectedIndex = CMD_SWITCH_ALTER;
            Button3FunctionBox.SelectedIndex = CMD_WHEEL_DOWN;
            UpFunctionMenu.SelectedIndex = CMD_PRESS_KEYS; UpParameterText.Text = "KEY_UP ";
            DownFunctionMenu.SelectedIndex = CMD_PRESS_KEYS; DownParameterText.Text = "KEY_DOWN ";
            LeftFunctionMenu.SelectedIndex = CMD_PRESS_KEYS; LeftParameterText.Text = "KEY_LEFT ";
            RightFunctionMenu.SelectedIndex = CMD_PRESS_KEYS; RightParameterText.Text = "KEY_RIGHT ";
            SipFunctionMenu.SelectedIndex = CMD_PRESS_LEFT;
            LongSipFunctionMenu.SelectedIndex = CMD_NOACTION;
            PuffFunctionMenu.SelectedIndex = CMD_CLICK_RIGHT;
            LongPuffFunctionMenu.SelectedIndex = CMD_CALIBRATE;

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
                LongSipComboBox.Items.Add(str);
                PuffComboBox.Items.Add(str);
                LongPuffComboBox.Items.Add(str);
            }

            updateComPorts();

            addToLog("FLipMouse GUI ready!");
            this.Load += LipmouseGUI_Load;
            this.FormClosed += MyWindow_Closing;
        }

        private void saveToFileMenuItem_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                addToLog("Please connect FlipMouse device to save settings !");
                return;
            }

            if (slotNames.Items.Count < 1)
            {
                addToLog("No slots are stored in the FLipMouse device !");
                return;
            }
            Stream myStream;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.Filter = "settings files (*.set)|*.set|All files (*.*)|*.*";
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                if ((myStream = saveFileDialog1.OpenFile()) != null)
                {
                    settingsFile = new System.IO.StreamWriter(myStream);
                    slotCounter = 0;
                    storingToFile = true;
                    sendCmd("AT ER");
                    sendCmd("AT LOAD " + slotNames.Items[0].ToString());   // get first slot
                    // incoming data will now be handeled in gotStoreValues()
                    // Code to write the stream goes here.
                    //myStream.Close();
                }
            }
        }



        private void loadFromFileMenuItem_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                addToLog("Please connect FlipMouse device to transfer settings !");
                return;
            }
            Stream myStream = null;
            OpenFileDialog fd = new OpenFileDialog();
            fd.DefaultExt = "*.set";
            fd.Filter = "settings files (*.set)|*.set|All files (*.*)|*.*";
            if (fd.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    if ((myStream = fd.OpenFile()) != null)
                    {

                        sendCmd("AT ER");
                        sendCmd("AT CLEAR");
                        slotNames.Items.Clear();

                        string line;
                        System.IO.StreamReader file = new System.IO.StreamReader(myStream);
                        while ((line = file.ReadLine()) != null)
                        {
                            Console.WriteLine("load line:" + line);

                            gotLoadValues(line);

                            slotNames.Text = slotNames.Text.Replace(" ", "");
                            slotNames.Text = slotNames.Text.Replace("\n", "");
                            slotNames.Text = slotNames.Text.Replace("\r", "");
                            addToLog("Saving Slot: " + slotNames.Text);

                            ApplyButton_Click(this, null);
                            sendCmd("AT SAVE " + slotNames.Text);
                        }
                        file.Close();
                        addToLog("The settings were transferred to the FLipMouse device!");
                        slotNames.Items.Clear();
                        sendCmd("AT LIST");
                        sendCmd("AT SR");

                    }
                }
                catch (Exception ex)
                {
                    addToLog("Error: Could not read file from disk. Original error: " + ex.Message);
                }
            }

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

        private void updateComPorts()
        {
            var ports = SerialPort.GetPortNames();
            portComboBox.DataSource = ports;
        }

        private void portComboBox_Click(object sender, EventArgs e)
        {
            updateComPorts();
        }

        private void LipmouseGUI_Load(object sender, EventArgs e)
        {
            this.slotValuesDelegate = new SlotValuesDelegate(gotSlotValues);
            this.rawValuesDelegate = new RawValuesDelegate(gotValues);
            this.loadValuesDelegate = new LoadValuesDelegate(gotLoadValues);
            this.storeValuesDelegate = new StoreValuesDelegate(gotStoreValues);

            BeginInvoke(this.rawValuesDelegate, new Object[] { "512,512,512,512,512" });
        }


        public void gotStoreValues(String newValues)
        {
            Console.WriteLine("got store values ("+slotCounter+"):"+newValues);
            String actSlotname = newValues.Substring(0, newValues.IndexOf("-,-"));
            if (settingsFile != null)
            {
                if (slotCounter == 0)
                {
                    firstSlotname = actSlotname;
                    Console.WriteLine("first slotname=" + firstSlotname);
                }
                
                if ((slotCounter > 0) && (firstSlotname.Equals(actSlotname)))
                {
                    Console.WriteLine("Now closing settings file!");
                    sendCmd("AT SR");
                    storingToFile = false;
                    settingsFile.Close();
                    settingsFile = null;
                }
                else
                {
                    settingsFile.Write(newValues);
                    sendCmd("AT NEXT");
                    slotCounter++;
                }
            }
        }

        public void gotLoadValues(String newValues)
        {
            String actToken;
            int i = 0;
            bool done = false;
            while (!done)
            {
                actToken = newValues.Substring(0, newValues.IndexOf("-,-"));
                // Console.WriteLine("Found Token " + i + " " + actToken);
                switch (i)
                {
                    case 0: slotNames.Text = actToken; break;  // slotname
                    case 1: int d = Int32.Parse(actToken);
                        if (d == 1) { selectStick.Checked = true; selectAlternative.Checked = false; useAlternativeFunctions = false; }
                        else { selectStick.Checked = false; selectAlternative.Checked = true; useAlternativeFunctions = true; }
                        break;  // cursor / alternative function

                    case 2: speedXBar.Value = Int32.Parse(actToken); speedXLabel.Text = speedXBar.Value.ToString(); break; // acceleration x
                    case 3: speedYBar.Value = Int32.Parse(actToken); speedYLabel.Text = speedYBar.Value.ToString(); break;  // acceleration y
                    case 4: deadzoneXBar.Value = Int32.Parse(actToken); deadzoneXLabel.Text = deadzoneXBar.Value.ToString(); break;  // deadzone x
                    case 5: deadzoneYBar.Value = Int32.Parse(actToken); deadzoneYLabel.Text = deadzoneYBar.Value.ToString();  break;  // deadzone y
                    case 6: sipThresholdBar.Value = Int32.Parse(actToken); break;  // threshold sip
                    case 7: puffThresholdBar.Value = Int32.Parse(actToken); break;  // threshold puff
                    case 8: break;  // mouse wheel stepsize, currently not used
                    case 9: timeThresholdBar.Value = Int32.Parse(actToken); break;  // time threshold for longpress, currently not used
                    
                    case 10: Button1FunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 11: Button1NumericParameter.Value = Int32.Parse(actToken); break;
                    case 12: Button1ParameterText.Text = actToken; break;
                    case 13: Button2FunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 14: Button2NumericParameter.Value = Int32.Parse(actToken); break;
                    case 15: Button2ParameterText.Text = actToken; break;
                    case 16: Button3FunctionBox.SelectedIndex = Int32.Parse(actToken); break;
                    case 17: Button3NumericParameter.Value = Int32.Parse(actToken); break;
                    case 18: Button3ParameterText.Text = actToken; break;
                    case 19: UpFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 20: UpNumericParameter.Value = Int32.Parse(actToken); break;
                    case 21: UpParameterText.Text = actToken; break;
                    case 22: DownFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 23: DownNumericParameter.Value = Int32.Parse(actToken); break;
                    case 24: DownParameterText.Text = actToken; break;
                    case 25: LeftFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 26: LeftNumericParameter.Value = Int32.Parse(actToken); break;
                    case 27: LeftParameterText.Text = actToken; break;
                    case 28: RightFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 29: RightNumericParameter.Value = Int32.Parse(actToken); break;
                    case 30: RightParameterText.Text = actToken; break;
                    case 31: SipFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 32: SipNumericParameter.Value = Int32.Parse(actToken); break;
                    case 33: SipParameterText.Text = actToken; break;
                    case 34: LongSipFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 35: LongSipNumericParameter.Value = Int32.Parse(actToken); break;
                    case 36: LongSipParameterText.Text = actToken; break;
                    case 37: PuffFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 38: PuffNumericParameter.Value = Int32.Parse(actToken); break;
                    case 39: PuffParameterText.Text = actToken; break;
                    case 40: LongPuffFunctionMenu.SelectedIndex = Int32.Parse(actToken); break;
                    case 41: LongPuffNumericParameter.Value = Int32.Parse(actToken); break;
                    case 42: LongPuffParameterText.Text = actToken; break;

                    default: done = true; break;
                }
                newValues = newValues.Substring(actToken.Length + 3);
                if (newValues.ToUpper().StartsWith("END")) done = true;
                else i++;
            }
        }

        // update paint areas if tabs are changed
        private void tabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            BeginInvoke(this.rawValuesDelegate, new Object[] { "512,512,512,512,512" });
        }

        // update activity log
        private void addToLog(String text)
        {
            activityLogTextbox.SelectedText = DateTime.Now.ToString() + ": ";
            activityLogTextbox.AppendText(text); 
            activityLogTextbox.AppendText("\n");
        }

        // serial port / communication handling
        private bool Connect(string portName)
        {
            if (!serialPort1.IsOpen)
            {
                serialPort1.PortName = portName;
                serialPort1.BaudRate = 9600;
                serialPort1.DataBits = 8;
                serialPort1.Parity = Parity.None;
                serialPort1.Handshake = Handshake.None;
                serialPort1.DtrEnable = true;
                serialPort1.ReadTimeout =2500;
                serialPort1.WriteTimeout =2500;
                serialPort1.NewLine = "\n";

                try{
                  serialPort1.Open();
                  return (true);
                }
                catch (Exception ex)
                {
                    addToLog("Could not open COM port");
                }
            }
            return (false);

        }

        private void sendCmd(string command)
        {
            if (serialPort1.IsOpen)
            {
                Console.WriteLine("Send:" + command);
                try {
                    serialPort1.Write(command + "\r");
                }
                catch (Exception ex)    {
                    addToLog("Could not write to COM port");
                }
            }
        }

        private void select_Click(object sender, EventArgs e) //select button
        {
            addToLog("Connecting to COM port...");
            if (portComboBox.SelectedIndex > -1)
            {
                if (serialPort1.IsOpen)
                {
                    addToLog(String.Format("Port '{0}' is already connected.", portComboBox.SelectedItem));
                    dcButton.Enabled = true;
                }
                else
                {

                    if (Connect(portComboBox.SelectedItem.ToString()))
                    {
                        addToLog(String.Format("Port '{0}' is now connected", portComboBox.SelectedItem));
                        portStatus.Text = "Connected";
                        portStatus.ForeColor = Color.Green;
                        saveSettings.Enabled = true;
                        calButton.Enabled = true;
                        dcButton.Enabled = true;
                        ClearButton.Enabled = true;
                        LoadButton.Enabled = true;
                        ApplyButton.Enabled = true;
                        SelectButton.Enabled = false;

                        readDone = false;
                        Thread thread = new Thread(new ThreadStart(WorkThreadFunction));
                        thread.Start();

                        slotNames.Items.Clear();
                        sendCmd("AT LIST");
                        sendCmd("AT LOAD");
                        sendCmd("AT SR");   // start reporting raw values !
                    }
                }
            }
            else addToLog("No port has been selected");
        }

        public void WorkThreadFunction()
        {
            try
            {
                while (serialPort1.IsOpen && !readDone)
                {
                    try  {
                        receivedString = serialPort1.ReadLine();
                        //Console.Write("received:" + receivedString);
                        if (receivedString.ToUpper().StartsWith("AT RR "))  // raw report found ?
                        {
                            BeginInvoke(this.rawValuesDelegate, new Object[] { receivedString.Substring(6) });
                        }
                        else if (receivedString.ToUpper().StartsWith("SLOT"))  // raw report found ?
                        {
                            BeginInvoke(this.slotValuesDelegate, new Object[] { receivedString.Substring(6) });
                        }
                        else if (receivedString.ToUpper().StartsWith("LOADING:"))  // slot name found ?
                        {
                            if (storingToFile)
                                BeginInvoke(this.storeValuesDelegate, new Object[] { receivedString.Substring(8) });
                            else
                                BeginInvoke(this.loadValuesDelegate, new Object[] { receivedString.Substring(8) });
                        }
                         
                    }
                    catch (Exception ex)
                    {
                        // addToLog("Could not read from COM port...");
                    }
                }
            }
            catch (Exception ex)
            { }
        }

        public void gotSlotValues(String newValues)
        {
            slotNames.Items.Add(newValues);
            if (slotNames.Items.Count == 1)
            {
                firstSlotname = newValues;
            }
        }

        private void disconnect()
        {
            readDone = true;
            if (serialPort1.IsOpen)
            {
                sendCmd("AT ER");  // end reporting raw values !

                portStatus.Text = "Disconnected";
                addToLog("Port " + portComboBox.Text + " is now disconnected");

                portStatus.ForeColor = Color.SlateGray;
                saveSettings.Enabled = false;
                calButton.Enabled = false;
                LoadButton.Enabled = false;
                ClearButton.Enabled = false;
                ApplyButton.Enabled = false;

                try
                {
                    receivedString = "";
                    slotNames.Items.Clear();
                    serialPort1.Close();
                }
                catch (Exception ex)
                {
                    addToLog("Error disconnecting COM Port");
                }
            }
        }

        private void dcButton_Click(object sender, EventArgs e) //disconnect button
        {
            addToLog("Disconnecting from COM Port...");
            disconnect();
            dcButton.Enabled = false;
            SelectButton.Enabled = true;
        }

        private void selectStick_CheckedChanged(object sender, EventArgs e)
        {
            // Console.WriteLine("stick selected");
            useAlternativeFunctions = false;
        }

        private void selectAlternative_CheckedChanged(object sender, EventArgs e)
        {
            // Console.WriteLine("alternative selected");
            useAlternativeFunctions = true;

        }

        // update assigned actions
        private void updateOneButton(int button, int cmdIndex, String parameter, String numParameter)
        {
            sendCmd("AT BM " + button);  // store command to this button function !
            switch (cmdIndex)
            {
                case CMD_NOACTION:     sendCmd("AT IDLE"); break;
                case CMD_NEXT:         sendCmd("AT NEXT"); break;
                case CMD_CLICK_LEFT:   sendCmd("AT CL"); break;
                case CMD_CLICK_RIGHT:  sendCmd("AT CR"); break;
                case CMD_CLICK_MIDDLE: sendCmd("AT CM"); break;
                case CMD_CLICK_DOUBLE: sendCmd("AT CD"); break;
                case CMD_PRESS_LEFT:   sendCmd("AT PL"); break;
                case CMD_PRESS_RIGHT:  sendCmd("AT PR"); break;
                case CMD_PRESS_MIDDLE: sendCmd("AT PM"); break;
                case CMD_WHEEL_UP:     sendCmd("AT WU"); break;
                case CMD_WHEEL_DOWN:   sendCmd("AT WD"); break;
                case CMD_CALIBRATE:    sendCmd("AT CA"); break;
                case CMD_SWITCH_ALTER: sendCmd("AT SW " + parameter); break;
                case CMD_MOVE_X:       sendCmd("AT MX " + numParameter); break;
                case CMD_MOVE_Y:       sendCmd("AT MY " + numParameter); break;
                case CMD_WRITE_TEXT:   sendCmd("AT KW " + parameter); break;
                case CMD_PRESS_KEYS:   sendCmd("AT KP " + parameter); break;
            }
        }

        // handle settings- and slot-management buttons

        private void calibration_Click(object sender, EventArgs e) //calibration button
        {
            addToLog("Starting Calibration...");
            if (serialPort1.IsOpen)
            {
                sendCmd("AT CA");
                addToLog("Your device has been calibrated. ");
            }
            else addToLog("Could not send to device - please connect COM port!");
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            addToLog("Applying Settings...");
            if (serialPort1.IsOpen)
            {
                sendCmd("AT ER");

                sendCmd("AT AX " + speedXLabel.Text);
                sendCmd("AT AY " + speedYLabel.Text);
                sendCmd("AT DX " + deadzoneXLabel.Text);
                sendCmd("AT DY " + deadzoneYLabel.Text);
                sendCmd("AT TS " + sipThresholdLabel.Text);
                sendCmd("AT TP " + puffThresholdLabel.Text);
                sendCmd("AT TT " + timeThresholdLabel.Text);
                if (useAlternativeFunctions) sendCmd("AT AF"); 
                else sendCmd("AT MM");

                // update the 11 button functions (starting with the 3 physical buttons)
                updateOneButton(1, Button1FunctionBox.SelectedIndex, Button1ParameterText.Text, Button1NumericParameter.Value.ToString());
                updateOneButton(2, Button2FunctionBox.SelectedIndex, Button2ParameterText.Text, Button2NumericParameter.Value.ToString());
                updateOneButton(3, Button3FunctionBox.SelectedIndex, Button3ParameterText.Text, Button3NumericParameter.Value.ToString());
                updateOneButton(4, UpFunctionMenu.SelectedIndex, UpParameterText.Text, UpNumericParameter.Value.ToString());
                updateOneButton(5, DownFunctionMenu.SelectedIndex, DownParameterText.Text, DownNumericParameter.Value.ToString());
                updateOneButton(6, LeftFunctionMenu.SelectedIndex, LeftParameterText.Text, LeftNumericParameter.Value.ToString());
                updateOneButton(7, RightFunctionMenu.SelectedIndex, RightParameterText.Text, RightNumericParameter.Value.ToString());
                updateOneButton(8, SipFunctionMenu.SelectedIndex, SipParameterText.Text, SipNumericParameter.Value.ToString());
                updateOneButton(9, LongSipFunctionMenu.SelectedIndex, LongSipParameterText.Text, LongSipNumericParameter.Value.ToString());
                updateOneButton(10, PuffFunctionMenu.SelectedIndex, PuffParameterText.Text, PuffNumericParameter.Value.ToString());
                updateOneButton(11, LongPuffFunctionMenu.SelectedIndex, LongPuffParameterText.Text, LongPuffNumericParameter.Value.ToString());

                addToLog("The selected settings have been applied.");
                sendCmd("AT SR");
            }
            else addToLog("Please connect a device before applying configuration changes.");
        }

        private void saveSettings_Click(object sender, EventArgs e) //button to save options to EEPROM
        {
            slotNames.Text=slotNames.Text.Replace(" ","");
            slotNames.Text = slotNames.Text.Replace("\n", "");
            slotNames.Text = slotNames.Text.Replace("\r", "");
            addToLog("Saving Slot: " + slotNames.Text);
            if (serialPort1.IsOpen)
            {
                sendCmd("AT ER");
                ApplyButton_Click(this, null);
                sendCmd("AT SAVE " + slotNames.Text);
                addToLog("The settings were saved");
                slotNames.Items.Clear();
                sendCmd("AT LIST");
                sendCmd("AT SR");
            }
            else addToLog("Could not send to device - please connect COM port!");
        }

        private void ClearButton_Click(object sender, EventArgs e)
        {
            addToLog("Clearing slots from memory...");
            if (serialPort1.IsOpen)
            {
                sendCmd("AT CLEAR\n");
                addToLog("All memory slots have been cleared.");
                slotNames.Items.Clear();
            }
            else addToLog("Could not send to device - please connect COM port!");
        }

        private void load_Click(object sender, EventArgs e)
        {
            slotNames.Text = slotNames.Text.Replace(" ", "");
            slotNames.Text = slotNames.Text.Replace("\n", ""); 
            slotNames.Text = slotNames.Text.Replace("\r", "");

            addToLog("Load Slot: " + slotNames.Text);
            if (serialPort1.IsOpen)
            {
                sendCmd("AT ER");
                sendCmd("AT LOAD " + slotNames.Text);
                sendCmd("AT SR");
            }

        }

        // update scroll bars :

        private void speedBar_Scroll(object sender, EventArgs e)
        {
            speedXLabel.Text = speedXBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                speedYBar.Value = speedXBar.Value;
                speedYLabel.Text = speedYBar.Value.ToString();
            }
        }

        private void deadzone_Scroll(object sender, EventArgs e)
        {
            deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
            if (splitXYBox.Checked == false)
            {
                deadzoneYBar.Value = deadzoneXBar.Value;
                deadzoneYLabel.Text = deadzoneYBar.Value.ToString();
            }
        }

        private void speedYBar_Scroll(object sender, EventArgs e)
        {
            speedYLabel.Text = speedYBar.Value.ToString();
        }

        private void deadzoneYBar_Scroll(object sender, EventArgs e)
        {
            deadzoneYLabel.Text = deadzoneYBar.Value.ToString();

        }

        
        
        private void sipThresholdBar_Scroll(object sender, EventArgs e)
        {
            sipThresholdLabel.Text = sipThresholdBar.Value.ToString();
        }

        private void puffThresholdBar_Scroll(object sender, EventArgs e)
        {
            puffThresholdLabel.Text = puffThresholdBar.Value.ToString();
        }

        private void timeThresholdBar_Scroll(object sender, EventArgs e)
        {
            timeThresholdLabel.Text = timeThresholdBar.Value.ToString();
        }


        // update visibility of parameter fields:

        private void updateVisibility(int selectedFunction, TextBox tb, NumericUpDown nud, ComboBox cb, Label la, Button bu)
        {
            switch (selectedFunction)
            {
                case CMD_MOVE_X:
                case CMD_MOVE_Y: la.Visible = true; la.Text = "   Speed:"; nud.Visible = true; tb.Visible = false; cb.Visible = false; bu.Visible = false; bu.Enabled = false; break;
                case CMD_WRITE_TEXT: la.Visible = true; la.Text = "    Text:"; nud.Visible = false; tb.Enabled = true; tb.ReadOnly = false; tb.Visible = true; tb.Text = ""; cb.Visible = false; bu.Visible = true; bu.Enabled = true; break;
                case CMD_PRESS_KEYS: la.Visible = true; la.Text = "KeyCodes:"; nud.Visible = false; tb.Visible = true; tb.Text = ""; tb.ReadOnly = true; cb.Visible = true; bu.Visible = true; bu.Enabled = true; break;
                default: la.Visible = false; nud.Visible = false; tb.Visible = false; cb.Visible = false; bu.Visible = false; bu.Enabled = false; break;
            }
        }

        private void Button1FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button1FunctionBox.SelectedIndex, Button1ParameterText, Button1NumericParameter, Button1ComboBox, Button1Label, clearButton1);
        }

        private void Button2FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button2FunctionBox.SelectedIndex, Button2ParameterText, Button2NumericParameter, Button2ComboBox, Button2Label, clearButton2);
        }

        private void Button3FunctionBox_SelectedIndexChanged_1(object sender, EventArgs e)
        {
            updateVisibility(Button3FunctionBox.SelectedIndex, Button3ParameterText, Button3NumericParameter, Button3ComboBox, Button3Label, clearButton3);
        }

        private void UpFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(UpFunctionMenu.SelectedIndex, UpParameterText, UpNumericParameter, UpComboBox, UpLabel, clearButtonUp);
        }

        private void DownFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(DownFunctionMenu.SelectedIndex, DownParameterText, DownNumericParameter, DownComboBox, DownLabel, clearButtonDown);
        }

        private void LeftFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(LeftFunctionMenu.SelectedIndex, LeftParameterText, LeftNumericParameter, LeftComboBox, LeftLabel, clearButtonLeft);
        }

        private void RightFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(RightFunctionMenu.SelectedIndex, RightParameterText, RightNumericParameter, RightComboBox, RightLabel, clearButtonRight);
        }

        private void SipFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(SipFunctionMenu.SelectedIndex, SipParameterText, SipNumericParameter, SipComboBox, SipParameterLabel, clearButtonSip);
        }

        private void LongSipFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(LongSipFunctionMenu.SelectedIndex, LongSipParameterText, LongSipNumericParameter, LongSipComboBox, LongSipParameterLabel, clearButtonLongSip);
        }

        private void PuffFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(PuffFunctionMenu.SelectedIndex, PuffParameterText, PuffNumericParameter, PuffComboBox, PuffParameterLabel, clearButtonPuff);
        }

        private void LongPuffFunctionMenu_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateVisibility(LongPuffFunctionMenu.SelectedIndex, LongPuffParameterText, LongPuffNumericParameter, LongPuffComboBox, LongPuffParameterLabel, clearButtonLongPuff);
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

        private void LongSipComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(LongSipComboBox, LongSipParameterText);
        }
        private void clearButtonLongSip_Click(object sender, EventArgs e)
        {
            LongSipParameterText.Text = "";
        }

        private void PuffComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(PuffComboBox, PuffParameterText);
        }
        private void clearButtonPuff_Click(object sender, EventArgs e)
        {
            PuffParameterText.Text = "";
        }

        private void LongPuffComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateKeyCodeParameter(LongPuffComboBox, LongPuffParameterText);
        }
        private void clearButtonLongPuff_Click(object sender, EventArgs e)
        {
            LongPuffParameterText.Text = "";
        }

        // draw live values on panels 

        public void gotValues(String newValues)
        {
            if (newValues.Length == 0)
                return;

            //  Console.WriteLine(newValues);

            String[] values = newValues.Split(',');
            if (values.Length == 5)
            {
                Int32 value = Convert.ToInt32(values[0]); //1023-Convert.ToInt32(values[0]);
                pressureLabel.Text = value.ToString();
                Graphics g = panel1.CreateGraphics();
                Brush brush = new SolidBrush(Color.Green);
                Brush brush2 = new SolidBrush(Color.White);
                value = value * panel1.Height / 1024;
                g.FillRectangle(brush, 0, panel1.Height - value, 30, value);
                g.FillRectangle(brush2, 0, 0, 30, panel1.Height - value);

                brush = new SolidBrush(Color.Orange);

                value = Convert.ToInt32(values[2]);
                upSensorLabel.Text = value.ToString();
                g = upPanel.CreateGraphics();
                value = value * upPanel.Height / 1024;
                g.FillRectangle(brush, 0, upPanel.Height - value, upPanel.Width, value);
                g.FillRectangle(brush2, 0, 0, upPanel.Width, upPanel.Height - value);

                value = Convert.ToInt32(values[1]);
                downSensorLabel.Text = value.ToString();
                g = downPanel.CreateGraphics();
                value = value * downPanel.Height / 1024;
                g.FillRectangle(brush, 0, 0, downPanel.Width, value);
                g.FillRectangle(brush2, 0, value, downPanel.Width, downPanel.Height - value);

                value = Convert.ToInt32(values[4]);
                leftSensorLabel.Text = value.ToString();
                g = leftPanel.CreateGraphics();
                value = value * leftPanel.Width / 1024;
                g.FillRectangle(brush, leftPanel.Width - value,0, value, leftPanel.Height);
                g.FillRectangle(brush2, 0, 0, leftPanel.Width - value, leftPanel.Height);

                value = Convert.ToInt32(values[3]);
                rightSensorLabel.Text = value.ToString();
                g = rightPanel.CreateGraphics();
                value = value * rightPanel.Width/ 1024;
                g.FillRectangle(brush, 0, 0, value, rightPanel.Height);
                g.FillRectangle(brush2, value, 0, rightPanel.Width-value, rightPanel.Height);
            }
        }

        private void splitXYBox_CheckedChanged(object sender, EventArgs e)
        {
            if (splitXYBox.Checked)
            {
                speedXBar.Width = 200; speedXLabel.Left = 270;
                speedYBar.Enabled = true; speedYBar.Visible = true;
                speedYLabel.Enabled = true; speedYLabel.Visible = true;
                deadzoneXBar.Width = 200; deadzoneXLabel.Left = 270;
                deadzoneYBar.Enabled = true; deadzoneYBar.Visible = true;
                deadzoneYLabel.Enabled = true; deadzoneYLabel.Visible = true;
                DeadzoneXNameLabel.Text = "Deadzone-X"; DeadzoneYNameLabel.Visible = true;
                SpeedXNameLabel.Text = "Speed-X"; SpeedYNameLabel.Visible = true;
            }
            else
            {
                speedXBar.Width = 420; speedXLabel.Left = 470;
                speedYBar.Enabled = false; speedYBar.Visible = false;
                speedYLabel.Enabled = false; speedYLabel.Visible = false;
                deadzoneXBar.Width = 420; deadzoneXLabel.Left = 470;
                deadzoneYBar.Enabled = false; deadzoneYBar.Visible = false;
                deadzoneYLabel.Enabled = false; deadzoneYLabel.Visible = false;
                DeadzoneXNameLabel.Text = "Deadzone"; DeadzoneYNameLabel.Visible = false;
                SpeedXNameLabel.Text = "Speed"; SpeedYNameLabel.Visible = false;
            }
            
        }

        private void decSpeedX_Click(object sender, EventArgs e)
        {
            if (speedXBar.Value >= speedXBar.Minimum + SPEED_CHANGE_STEP)
                speedXBar.Value -= SPEED_CHANGE_STEP;
            speedXLabel.Text = speedXBar.Value.ToString();
        }
        private void decSpeedX_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decSpeedX_Click);
            clickTimer.Start();
        }

        private void incSpeedY_Click(object sender, EventArgs e)
        {
            if (speedXBar.Value <= speedXBar.Maximum - SPEED_CHANGE_STEP)
                speedXBar.Value += SPEED_CHANGE_STEP;
            speedXLabel.Text = speedXBar.Value.ToString();

        }
        private void incSpeedY_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incSpeedY_Click);
            clickTimer.Start();
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

        private void incDeadzoneY_Click(object sender, EventArgs e)
        {
            if (deadzoneXBar.Value <= deadzoneXBar.Maximum - DEADZONE_CHANGE_STEP)
                deadzoneXBar.Value += DEADZONE_CHANGE_STEP;
            deadzoneXLabel.Text = deadzoneXBar.Value.ToString();
        }
        private void incDeadzoneY_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incDeadzoneY_Click);
            clickTimer.Start();
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

        private void decTimeThreshold_Click(object sender, EventArgs e)
        {
            if (timeThresholdBar.Value >= timeThresholdBar.Minimum + TIME_CHANGE_STEP)
                timeThresholdBar.Value -= TIME_CHANGE_STEP;
            timeThresholdLabel.Text = timeThresholdBar.Value.ToString();

        }
        private void decTimeThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(decTimeThreshold_Click);
            clickTimer.Start();
        }

        private void incTimeThreshold_Click(object sender, EventArgs e)
        {
            if (timeThresholdBar.Value <= timeThresholdBar.Maximum - TIME_CHANGE_STEP)
                timeThresholdBar.Value += TIME_CHANGE_STEP;
            timeThresholdLabel.Text = timeThresholdBar.Value.ToString();

        }
        private void incTimeThreshold_MouseHover(object sender, EventArgs e)
        {
            functionPointer = new ClickDelegate(incTimeThreshold_Click);
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

    }
}
