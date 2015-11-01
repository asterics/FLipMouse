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
                g.FillRectangle(brush, leftPanel.Width - value, 0, value, leftPanel.Height);
                g.FillRectangle(brush2, 0, 0, leftPanel.Width - value, leftPanel.Height);

                value = Convert.ToInt32(values[3]);
                rightSensorLabel.Text = value.ToString();
                g = rightPanel.CreateGraphics();
                value = value * rightPanel.Width / 1024;
                g.FillRectangle(brush, 0, 0, value, rightPanel.Height);
                g.FillRectangle(brush2, value, 0, rightPanel.Width - value, rightPanel.Height);
            }
        }
    }
}
