using System;
using System.Windows.Forms;
using System.Drawing;
using System.IO;


namespace MouseApp2
{
    public partial class FLipMouseGUI
    {
        // draw live values on panels 

        public void drawRawValues(String newValues)
        {
            if (newValues.Length == 0)
                return;

            //  Console.WriteLine(newValues);

            String[] values = newValues.Split(',');
            if (values.Length == 7)
            {
                Int32 value = Convert.ToInt32(values[0]); //1023-Convert.ToInt32(values[0]);
                pressureLabel.Text = value.ToString();
                Graphics g = pressureDrawPanel.CreateGraphics();
                Brush brush = new SolidBrush(Color.Green);
                Brush brush2 = new SolidBrush(Color.White);
                value = value * pressureDrawPanel.Height / 1024;
                g.FillRectangle(brush, 0, pressureDrawPanel.Height - value, pressureDrawPanel.Width-1, value);
                g.FillRectangle(brush2, 0, 0, pressureDrawPanel.Width-1, pressureDrawPanel.Height - value);


                brush = new SolidBrush(Color.Red);
                g = puffLevelIndicatorPanel.CreateGraphics();
                if (Convert.ToInt32(values[0]) > puffThresholdBar.Value)
                    g.FillRectangle(brush, 0, 0, puffLevelIndicatorPanel.Width - 1, puffLevelIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, puffLevelIndicatorPanel.Width - 1, puffLevelIndicatorPanel.Height - 1);
                
                g = specialLevelIndicatorPanel.CreateGraphics();
                if (Convert.ToInt32(values[0]) > specialThresholdBar.Value)
                    g.FillRectangle(brush, 0, 0, specialLevelIndicatorPanel.Width - 1, specialLevelIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, specialLevelIndicatorPanel.Width - 1, specialLevelIndicatorPanel.Height - 1);

                g = sipLevelIndicatorPanel.CreateGraphics();
                if (Convert.ToInt32(values[0]) < sipThresholdBar.Value)
                    g.FillRectangle(brush, 0, 0, sipLevelIndicatorPanel.Width - 1, sipLevelIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, sipLevelIndicatorPanel.Width - 1, sipLevelIndicatorPanel.Height - 1);

                g = holdLevelIndicatorPanel.CreateGraphics(); 
                if (Convert.ToInt32(values[0]) < holdThresholdBar.Value)
                    g.FillRectangle(brush, 0, 0, holdLevelIndicatorPanel.Width - 1, holdLevelIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, holdLevelIndicatorPanel.Width - 1, holdLevelIndicatorPanel.Height - 1);

                
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
                g.FillRectangle(brush, leftPanel.Width - value, 0, value, leftPanel.Height);
                g.FillRectangle(brush2, 0, 0, leftPanel.Width - value, leftPanel.Height);

                value = Convert.ToInt32(values[3]);
                rightSensorLabel.Text = value.ToString();
                g = rightPanel.CreateGraphics();
                value = value * rightPanel.Width / 1024;
                g.FillRectangle(brush, 0, 0, value, rightPanel.Height);
                g.FillRectangle(brush2, value, 0, rightPanel.Width - value, rightPanel.Height);

                brush = new SolidBrush(Color.DeepSkyBlue);
                value = Convert.ToInt32(values[6]);
                g = upIndicatorPanel.CreateGraphics();
                if (value < -deadzoneYBar.Value)
                    g.FillRectangle(brush, 0, 0, upIndicatorPanel.Width - 1, upIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, upIndicatorPanel.Width - 1, upIndicatorPanel.Height - 1);

                g = downIndicatorPanel.CreateGraphics();
                if (value > deadzoneYBar.Value)
                    g.FillRectangle(brush, 0, 0, downIndicatorPanel.Width - 1, downIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, upIndicatorPanel.Width - 1, upIndicatorPanel.Height - 1);

                value = Convert.ToInt32(values[5]);
                g = rightIndicatorPanel.CreateGraphics();
                if (value > deadzoneXBar.Value)
                    g.FillRectangle(brush, 0, 0, rightIndicatorPanel.Width - 1, rightIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, rightIndicatorPanel.Width - 1, rightIndicatorPanel.Height - 1);

                g = leftIndicatorPanel.CreateGraphics();
                if (value < -deadzoneXBar.Value)
                    g.FillRectangle(brush, 0, 0, leftIndicatorPanel.Width - 1, leftIndicatorPanel.Height - 1);
                else
                    g.FillRectangle(brush2, 0, 0, leftIndicatorPanel.Width - 1, leftIndicatorPanel.Height - 1);
            }
        }
    }
}
