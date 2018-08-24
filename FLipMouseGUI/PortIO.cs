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
                serialPort1.ReadTimeout = 10000;
                serialPort1.WriteTimeout = 10000;
                serialPort1.NewLine = "\n";
                serialPort1.Encoding = System.Text.Encoding.Default;


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
                    Console.WriteLine("Could not write to COM port:" + ex.ToString());
                    addToLog("Could not write to COM port:" + ex.ToString());
                }
            }
        }


        public void WorkThreadFunction()
        {
            String receivedString = "";

            Console.WriteLine("sending ID request ..");
            sendIDCommand();   // start (after connect): request ID String from Lipmouse; receive ID before timeout ! (else close port)
            
            try
            {
                while (serialPort1.IsOpen && !readDone)
                {
                    try
                    {
                        receivedString = serialPort1.ReadLine();
                        if (!receivedString.Contains("VALUES"))
                           Console.Write("received:" + receivedString);
                        BeginInvoke(this.stringReceivedDelegate, new Object[] { receivedString });
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Could not read from COM port: "+ex.ToString());
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
            if (newLine.ToUpper().StartsWith(PREFIX_REPORT_VALUES))  // read raw report (ADC values)
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
            else if (newLine.ToUpper().StartsWith(PREFIX_IRCOMMAND_NAME))  // ir command name received 
            {
                gotIRCommandName(newLine);
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_IRCOMMAND_RECORDED))  // ir recording sucessful 
            {
                gotIRCommandRecorded(newLine);
            }
            else if (newLine.ToUpper().StartsWith(PREFIX_IRCOMMAND_TIMEOUT))  // ir recording sucessful 
            {
                gotIRCommandTimeout(newLine);
            }
            else if (newLine.ToUpper().Contains(PREFIX_FLIPMOUSE_VERSION))  // read flipmouse ID 
            {
                gotID(newLine);
            } 
        }

        public void gotEnd()
        {
            if (slots.Count < 1)
            {
                //slots.Add(new Slot());
                initSlots();
            }

            actSlot = 0;
            displaySlot(actSlot);

            sendStartReportingCommand();          
        }

        public void gotID(String newLine)
        {
            DialogResult dialogResult;
            addToLog("Connected device: " + newLine);
            if ((!(newLine.Contains(VERSION_STRING))) && (checkVersion==1))
                {
                dialogResult = MessageBox.Show(newLine + "The firmware is not compatible to GUI version " + VERSION_STRING + " !\nCurrent releases can be found at: https://github.com/asterics/FLipMouse/releases \nShould the Firmware be updated automatically ? ", "FlipMouse update necessary", MessageBoxButtons.YesNo);
                disconnnectComButton_Click(this, null);
                if (dialogResult == DialogResult.Yes)
                {
                    Thread.Sleep(500);
                    string strCmdText;
                    strCmdText = "/C tycmd.exe upload FlipWare.ino.hex";
                    System.Diagnostics.Process.Start("CMD.exe", strCmdText);
                }
                return;
            }
            flipMouseOnline = 1;

            dialogResult = MessageBox.Show("Connected device: " + newLine + "Do you want to load the slots and settings which are stored in the FLipMouse device ?", "Load Settings ?", MessageBoxButtons.YesNo);
            if (dialogResult == DialogResult.Yes)
            {
                loadSettingsFromFLipmouse();
                slotNames.Items.Clear();
            }

            irCommandBox.Items.Clear();
            sendListIRCommand();
            sendStartReportingCommand();   // start reporting raw values !


        }

        public void gotSlotNames(String newSlotName)
        {
            slots.Add(new Slot());
            newSlotName = newSlotName.Replace("\r",""); newSlotName = newSlotName.Replace("\n", "");
            Console.WriteLine("Slotname:" + newSlotName);
            addToLog("Loading slot " + newSlotName +" from FLipMouse device.");

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

        public void gotIRCommandName(String newIRCommandName)
        {
            newIRCommandName = newIRCommandName.Replace("\r", ""); newIRCommandName = newIRCommandName.Replace("\n", "");
            newIRCommandName = newIRCommandName.Substring(newIRCommandName.IndexOf(":")+1);
            Console.WriteLine("found IRCommand:" + newIRCommandName);
            irCommandBox.Items.Add(newIRCommandName);
            //irCommandBox.SelectedIndex=0;
            irCommandBox.Text = newIRCommandName;


            // update all infrared code selection boxes
            // TDB: improve this ... (should be performed only once, after the last received ir-code)
            foreach (CommandGuiLink cgl in commandGuiLinks)
            {
                if (cgl.cb != null)
                {
                    if (cgl.cb.Text.Equals("Play Infrared Command") || cgl.cb.Text.Equals("Hold Infrared Command"))
                    {
                        cgl.cb2.Items.Clear();
                        foreach (Object itm in irCommandBox.Items)
                            cgl.cb2.Items.Add(itm.ToString());
                    }
                }
            }
        }

        public void gotIRCommandRecorded(String newIRCommandName)
        {
            addToLog("IR Command recorded!" + newIRCommandName);
            irCommandBox.Items.Clear();
            sendListIRCommand();
        }

        public void gotIRCommandTimeout(String newIRCommandName)
        {
            addToLog("Timeout ! No IR Command recorded ..." + newIRCommandName);
        }


        private void disconnect()
        {
            readDone = true;
            if (serialPort1.IsOpen)
            {
                sendEndReportingCommand();  // end reporting raw values !

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
                Cursor.Current = Cursors.WaitCursor;
                sendEndReportingCommand();
                sendClearCommand();  // delete all slots on FlipMouse

                slotCounter = 0;
                for (slotCounter = 0; slotCounter < slots.Count; slotCounter++)
                {
                    displaySlot(slotCounter);
                    sendApplyCommands();
                    sendSaveSlotCommands(slots[slotCounter].slotName);
                    addToLog("Slot " + slots[slotCounter].slotName + " is stored into FLipmouse.");
                    Thread.Sleep(1000);  // time to store slot in flipmouse
                }
                addToLog("The settings were stored!");
                displaySlot(0);
                sendLoadSlotCommand(slots[0].slotName);
                sendStartReportingCommand();
                Thread.Sleep(1000);  // time to activate config in flipmouse
                Cursor.Current = Cursors.Default;
            }

        }

        public void loadSettingsFromFLipmouse()
        {
            if (serialPort1.IsOpen)
            {
                sendEndReportingCommand();
                slotNames.Items.Clear();
                slots.Clear();
                actSlot = -1;
                sendLoadAllCommand();
            }
        }
    }
}


