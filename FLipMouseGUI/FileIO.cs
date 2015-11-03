using System;
using System.Windows.Forms;
using System.IO;


namespace MouseApp2
{
    public partial class FLipMouseGUI
    {
            Stream myStream;
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            System.IO.StreamWriter settingsFile = null;


            public void saveSettingsToFile()
            {
                saveFileDialog1.Filter = "settings files (*.set)|*.set|All files (*.*)|*.*";
                saveFileDialog1.RestoreDirectory = true;

                if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    if ((myStream = saveFileDialog1.OpenFile()) != null)
                    {
                        settingsFile = new System.IO.StreamWriter(myStream);
                        slotCounter = 0;
                        for (slotCounter = 0; slotCounter < slots.Count; slotCounter++)
                        {
                            settingsFile.Write("Slot "+(slotCounter+1)+":"+slots[slotCounter].slotName+"\n");
                            foreach (String settingString in slots[slotCounter].settingStrings)
                            {
                                settingsFile.Write(settingString + "\n");
                            }
                        }
                        settingsFile.Close();
                        settingsFile = null;
                        addToLog("The settings were saved!");
                    }
                    else addToLog("The settings could not be saved!");

                }
            }
            public void loadSettingsFromFile()
            {
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
                            int actSlot = -1;
                            slotNames.Items.Clear();
                            slots.Clear();

                            string line;
                            System.IO.StreamReader file = new System.IO.StreamReader(myStream);
                            while ((line = file.ReadLine()) != null)
                            {
                                if ((line.StartsWith("Slot ")) && (line.IndexOf(':') > 0))
                                {
                                    actSlot++;
                                    slots.Add(new Slot());
                                    Console.WriteLine("\n"+line);
                                    String actSlotName = line.Substring(line.IndexOf(':')+1);
                                    slotNames.Items.Add(actSlotName);
                                    slots[actSlot].slotName = actSlotName;
                                }
                                else if (line.StartsWith("AT "))
                                {
                                    Console.Write(line+", ");
                                    slots[actSlot].settingStrings.Add(line);
                                }
                            }
                            file.Close();

                            if (slots.Count < 1) 
                                slots.Add(new Slot());

                            actSlot = 0; 
                            displaySlot(actSlot);

                        }
                    }
                    catch (Exception ex)
                    {
                        addToLog("Error: Could not read file from disk. Original error: " + ex.Message);
                    }
                }

            }

    }
}