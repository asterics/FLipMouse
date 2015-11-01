using System;
using System.Windows.Forms;
using System.Drawing;
using System.Threading.Tasks;
using System.Threading;
using System.IO.Ports;
using System.IO;


namespace MouseApp2
{
    public partial class FLipMouseGUI
    {
        int flipMouseOnline=0;


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
                serialPort1.ReadTimeout = 2500;
                serialPort1.WriteTimeout = 2500;
                serialPort1.NewLine = "\n";

                try
                {
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

        private void updateComPorts()
        {
            var ports = SerialPort.GetPortNames();
            portComboBox.DataSource = ports;
        }

        private void sendCmd(string command)
        {
            if (serialPort1.IsOpen)
            {
                Console.WriteLine("Send:" + command);
                try
                {
                    serialPort1.Write(command + "\r");
                }
                catch (Exception ex)
                {
                    addToLog("Could not write to COM port");
                }
            }
        }


        public void WorkThreadFunction()
        {
            String receivedString = "";

            sendGetID();   // start (after connect): request ID String from Lipmouse; receive ID before timeout ! (else close port)
            
            try
            {
                while (serialPort1.IsOpen && !readDone)
                {
                    try
                    {
                        receivedString = serialPort1.ReadLine();
                        // Console.Write("received:" + receivedString);
                        BeginInvoke(this.stringReceivedDelegate, new Object[] { receivedString });
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


        void IdTimer_Tick(object sender, EventArgs e)
        {
            IdTimer.Stop();
            if (flipMouseOnline == 0)
            {
                addToLog("ID-Timeout ! No Flipmouse module found !");
                disconnnectComButton_Click(this, null);
            }
        }


        public void stringReceived(String newLine)
        {
            if (newLine.ToUpper().StartsWith(PREFIX_FLIPMOUSE_VERSION))  // read flipmouse ID 
            {
                gotID(newLine);
            } 
            else if (newLine.ToUpper().StartsWith(PREFIX_REPORT_VALUES))  // read raw report (ADC values)
            {
                drawRawValues(newLine.Substring(PREFIX_REPORT_VALUES.Length));
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_SLOT_NAME))  // read next slot name 
            {
                gotSlotNames(newLine.Substring(PREFIX_SLOT_NAME.Length));
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_AT_COMMAND))  // read setting for a slot 
            {
                gotAT(newLine);
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_END_OF_SLOTS))  // end of slot 
            {
                gotEnd();
            }

        }

        public void gotEnd()
        {
            if (slots.Count < 1)
              slots.Add(new Slot());

              actSlot = 0;
              displaySlot(actSlot);

              addToLog("The settings were loaded from FLipMouse device!");
              sendStartReporting();          
        }

        public void gotID(String newLine)
        {
            addToLog("Flipmouse detected:" + newLine);
            flipMouseOnline = 1;
            slotNames.Items.Clear();
            sendStartReporting();   // start reporting raw values !
        }

        public void gotSlotNames(String newSlotName)
        {
            slots.Add(new Slot());
            newSlotName = newSlotName.Replace("\r",""); newSlotName = newSlotName.Replace("\n", "");
            Console.WriteLine("Slotname:" + newSlotName);
            slotNames.Items.Add(newSlotName);
            actSlot = slots.Count - 1;
            slots[actSlot].settingStrings.Clear();
            slots[actSlot].slotName=newSlotName;
        }

        public void gotAT(String newATCommand)
        {
            newATCommand = newATCommand.Replace("\r", ""); 
            newATCommand = newATCommand.Replace("\n", "");
            Console.WriteLine("adding:" + newATCommand);
            slots[actSlot].settingStrings.Add(newATCommand);
        }


        private void disconnect()
        {
            readDone = true;
            if (serialPort1.IsOpen)
            {
                sendEndReporting();  // end reporting raw values !

                portStatus.Text = "Disconnected";
                addToLog("Port " + portComboBox.Text + " is now disconnected");

                portStatus.ForeColor = Color.SlateGray;
                calButton.Enabled = false;

                try
                {
                    slotNames.Items.Clear();
                    serialPort1.Close();
                }
                catch (Exception ex)
                {
                    addToLog("Error disconnecting COM Port");
                }
            }
            flipMouseOnline = 0;
        }

        public void storeSettingsToFLipmouse()
        {
            if (serialPort1.IsOpen)
            {
                sendEndReporting();
                sendClearCommand();  // delete all slots on FlipMouse

                slotCounter = 0;
                for (slotCounter = 0; slotCounter < slots.Count; slotCounter++)
                {
                    displaySlot(slotCounter);
                    sendApplyCommands();
                    sendCmd("AT SA " + slots[slotCounter].slotName);   // save slotname
                    addToLog("Slot "+slots[slotCounter].slotName+" was stored into FLipmouse.");

                }
                addToLog("The settings were stored!");
                sendStartReporting();
            }

        }

        public void loadSettingsFromFLipmouse()
        {
            if (serialPort1.IsOpen)
            {
                sendEndReporting();
                slotNames.Items.Clear();
                slots.Clear();
                actSlot = -1;
                sendLoadAll();
            }
        }
    }
}



/*            else if (newLine.ToUpper().StartsWith("LOADING:"))  // read all settings  
            {
                if (storingToFile)
                    gotStoreValues(newLine.Substring(8));
                else
                    gotLoadValues(newLine.Substring(8));
            } */




        /*
        public void gotStoreValues(String newValues)
        {
            Console.WriteLine("got store values (" + slotCounter + "):" + newValues);
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
                    sendCmd("AT NE");
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
                Console.WriteLine("act string = " + newValues);
                actToken = newValues.Substring(0, newValues.IndexOf("-,-"));
                Console.WriteLine("Found Token " + i + " " + actToken);
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
                    case 5: deadzoneYBar.Value = Int32.Parse(actToken); deadzoneYLabel.Text = deadzoneYBar.Value.ToString(); break;  // deadzone y
                    case 6: sipThresholdBar.Value = Int32.Parse(actToken); break;  // threshold sip
                    case 7: puffThresholdBar.Value = Int32.Parse(actToken); break;  // threshold puff
                    case 8: break;  // mouse wheel stepsize, currently not used
                        
                    case 9: specialThresholdBar.Value = Int32.Parse(actToken); specialThresholdLabel.Text = specialThresholdBar.Value.ToString() + " / " + (1024 - specialThresholdBar.Value).ToString(); break;  // threshold special/hold functions
                    case 10: upGainBar.Value = Int32.Parse(actToken); upGainLabel.Text = upGainBar.Value.ToString(); break;    // up gain 
                    case 11: downGainBar.Value = Int32.Parse(actToken); downGainLabel.Text = downGainBar.Value.ToString(); break;  // down gain
                    case 12: leftGainBar.Value = Int32.Parse(actToken); leftGainLabel.Text = leftGainBar.Value.ToString(); break;  // left gain
                    case 13: rightGainBar.Value = Int32.Parse(actToken); rightGainLabel.Text = rightGainBar.Value.ToString(); break;  // right gain
                    case 14: Button1FunctionBox.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 15: Button1NumericParameter.Value = Int32.Parse(actToken); break;
                    case 16: Button1ParameterText.Text = actToken; break;
                    case 17: Button2FunctionBox.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 18: Button2NumericParameter.Value = Int32.Parse(actToken); break;
                    case 19: Button2ParameterText.Text = actToken; break;
                    case 20: Button3FunctionBox.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 21: Button3NumericParameter.Value = Int32.Parse(actToken); break;
                    case 22: Button3ParameterText.Text = actToken; break;
                    case 23: UpFunctionMenu.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 24: UpNumericParameter.Value = Int32.Parse(actToken); break;
                    case 25: UpParameterText.Text = actToken; break;
                    case 26: DownFunctionMenu.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 27: DownNumericParameter.Value = Int32.Parse(actToken); break;
                    case 28: DownParameterText.Text = actToken; break;
                    case 29: LeftFunctionMenu.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 30: LeftNumericParameter.Value = Int32.Parse(actToken); break;
                    case 31: LeftParameterText.Text = actToken; break;
                    case 32: RightFunctionMenu.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 33: RightNumericParameter.Value = Int32.Parse(actToken); break;
                    case 34: RightParameterText.Text = actToken; break;
                    case 35: SipFunctionMenu.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 36: SipNumericParameter.Value = Int32.Parse(actToken); break;
                    case 37: SipParameterText.Text = actToken; break;
                    case 38: SpecialSipFunctionMenu.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 39: SpecialSipNumericParameter.Value = Int32.Parse(actToken); break;
                    case 40: SpecialSipParameterText.Text = actToken; break;
                    case 41: PuffFunctionMenu.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 42: PuffNumericParameter.Value = Int32.Parse(actToken); break;
                    case 43: PuffParameterText.Text = actToken; break;
                    case 44: SpecialPuffFunctionMenu.SelectedIndex = allCommands.getSelectionIndex(Int32.Parse(actToken)); break;
                    case 45: SpecialPuffNumericParameter.Value = Int32.Parse(actToken); break;
                    case 46: SpecialPuffParameterText.Text = actToken; break;
                        
                    default: done = true; break;
                }
                newValues = newValues.Substring(actToken.Length + 3);
                if (newValues.ToUpper().StartsWith("END"))
                {
                    done = true;
                }
                else i++;
            }
        }
*/