namespace MouseApp2
{
    partial class FLipMouseGUI
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FLipMouseGUI));
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.portComboBox = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.connectComButton = new System.Windows.Forms.Button();
            this.disconnectComButton = new System.Windows.Forms.Button();
            this.portStatus = new System.Windows.Forms.Label();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.LipmouseTab = new System.Windows.Forms.TabPage();
            this.joyModeLabel = new System.Windows.Forms.Label();
            this.joyModeBox = new System.Windows.Forms.ComboBox();
            this.label27 = new System.Windows.Forms.Label();
            this.selectJoystick = new System.Windows.Forms.RadioButton();
            this.singlePanel = new System.Windows.Forms.Panel();
            this.incSpeed = new System.Windows.Forms.Button();
            this.decSpeed = new System.Windows.Forms.Button();
            this.incDeadzone = new System.Windows.Forms.Button();
            this.decDeadzone = new System.Windows.Forms.Button();
            this.deadzoneLabel = new System.Windows.Forms.Label();
            this.label34 = new System.Windows.Forms.Label();
            this.deadzoneBar = new System.Windows.Forms.TrackBar();
            this.speedLabel = new System.Windows.Forms.Label();
            this.speedBar = new System.Windows.Forms.TrackBar();
            this.label36 = new System.Windows.Forms.Label();
            this.splitPanel = new System.Windows.Forms.Panel();
            this.incDeadzoneY = new System.Windows.Forms.Button();
            this.decDeadzoneY = new System.Windows.Forms.Button();
            this.incSpeedY = new System.Windows.Forms.Button();
            this.decSpeedY = new System.Windows.Forms.Button();
            this.incSpeedX = new System.Windows.Forms.Button();
            this.decSpeedX = new System.Windows.Forms.Button();
            this.incDeadzoneX = new System.Windows.Forms.Button();
            this.decDeadzoneX = new System.Windows.Forms.Button();
            this.DeadzoneYNameLabel = new System.Windows.Forms.Label();
            this.SpeedYNameLabel = new System.Windows.Forms.Label();
            this.speedYLabel = new System.Windows.Forms.Label();
            this.deadzoneYLabel = new System.Windows.Forms.Label();
            this.deadzoneYBar = new System.Windows.Forms.TrackBar();
            this.speedYBar = new System.Windows.Forms.TrackBar();
            this.deadzoneXLabel = new System.Windows.Forms.Label();
            this.DeadzoneXNameLabel = new System.Windows.Forms.Label();
            this.deadzoneXBar = new System.Windows.Forms.TrackBar();
            this.speedXLabel = new System.Windows.Forms.Label();
            this.speedXBar = new System.Windows.Forms.TrackBar();
            this.SpeedXNameLabel = new System.Windows.Forms.Label();
            this.splitXYBox = new System.Windows.Forms.CheckBox();
            this.selectAlternative = new System.Windows.Forms.RadioButton();
            this.selectStick = new System.Windows.Forms.RadioButton();
            this.calButton = new System.Windows.Forms.Button();
            this.AlternativeTab = new System.Windows.Forms.TabPage();
            this.clearButtonRight = new System.Windows.Forms.Button();
            this.clearButtonLeft = new System.Windows.Forms.Button();
            this.clearButtonDown = new System.Windows.Forms.Button();
            this.clearButtonUp = new System.Windows.Forms.Button();
            this.label18 = new System.Windows.Forms.Label();
            this.RightComboBox = new System.Windows.Forms.ComboBox();
            this.RightNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.RightLabel = new System.Windows.Forms.Label();
            this.RightParameterText = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.RightFunctionMenu = new System.Windows.Forms.ComboBox();
            this.LeftComboBox = new System.Windows.Forms.ComboBox();
            this.DownComboBox = new System.Windows.Forms.ComboBox();
            this.UpComboBox = new System.Windows.Forms.ComboBox();
            this.LeftNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.DownNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.UpNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.LeftLabel = new System.Windows.Forms.Label();
            this.LeftParameterText = new System.Windows.Forms.TextBox();
            this.DownLabel = new System.Windows.Forms.Label();
            this.DownParameterText = new System.Windows.Forms.TextBox();
            this.UpLabel = new System.Windows.Forms.Label();
            this.UpParameterText = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.LeftFunctionMenu = new System.Windows.Forms.ComboBox();
            this.label12 = new System.Windows.Forms.Label();
            this.DownFunctionMenu = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.UpFunctionMenu = new System.Windows.Forms.ComboBox();
            this.SipPuffLevelsTab = new System.Windows.Forms.TabPage();
            this.holdLevelIndicatorPanel = new System.Windows.Forms.Panel();
            this.sipLevelIndicatorPanel = new System.Windows.Forms.Panel();
            this.specialLevelIndicatorPanel = new System.Windows.Forms.Panel();
            this.puffLevelIndicatorPanel = new System.Windows.Forms.Panel();
            this.label30 = new System.Windows.Forms.Label();
            this.label29 = new System.Windows.Forms.Label();
            this.decHoldThreshold = new System.Windows.Forms.Button();
            this.incHoldThreshold = new System.Windows.Forms.Button();
            this.stongSipThresholdLabel = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.strongSipThresholdBar = new System.Windows.Forms.TrackBar();
            this.incTimeThreshld = new System.Windows.Forms.Button();
            this.decTimeThreshold = new System.Windows.Forms.Button();
            this.incSipThreshold = new System.Windows.Forms.Button();
            this.decSipThreshold = new System.Windows.Forms.Button();
            this.incPuffThreshold = new System.Windows.Forms.Button();
            this.decPuffThreshold = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.strongPuffThresholdLabel = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.strongPuffThresholdBar = new System.Windows.Forms.TrackBar();
            this.pressureDrawPanel = new System.Windows.Forms.Panel();
            this.pressureLabel = new System.Windows.Forms.Label();
            this.puffThresholdLabel = new System.Windows.Forms.Label();
            this.puffThresholdBar = new System.Windows.Forms.TrackBar();
            this.thresholdLabelForPuff = new System.Windows.Forms.Label();
            this.sipThresholdLabel = new System.Windows.Forms.Label();
            this.sipThresholdBar = new System.Windows.Forms.TrackBar();
            this.thresholdLabelForSip = new System.Windows.Forms.Label();
            this.SipPuffTab = new System.Windows.Forms.TabPage();
            this.sipPuffSelections = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.clearButtonStrongPuff = new System.Windows.Forms.Button();
            this.clearButtonPuff = new System.Windows.Forms.Button();
            this.clearButtonStrongSip = new System.Windows.Forms.Button();
            this.clearButtonSip = new System.Windows.Forms.Button();
            this.StrongPuffComboBox = new System.Windows.Forms.ComboBox();
            this.SpecialPuffParameterLabel = new System.Windows.Forms.Label();
            this.StrongPuffNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.label14 = new System.Windows.Forms.Label();
            this.StrongPuffFunctionMenu = new System.Windows.Forms.ComboBox();
            this.StrongSipComboBox = new System.Windows.Forms.ComboBox();
            this.SpecialSipParameterLabel = new System.Windows.Forms.Label();
            this.StrongSipNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.label9 = new System.Windows.Forms.Label();
            this.StrongSipFunctionMenu = new System.Windows.Forms.ComboBox();
            this.PuffComboBox = new System.Windows.Forms.ComboBox();
            this.SipComboBox = new System.Windows.Forms.ComboBox();
            this.PuffParameterLabel = new System.Windows.Forms.Label();
            this.SipParameterLabel = new System.Windows.Forms.Label();
            this.PuffNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.SipNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.label20 = new System.Windows.Forms.Label();
            this.PuffFunctionMenu = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.SipFunctionMenu = new System.Windows.Forms.ComboBox();
            this.SipParameterText = new System.Windows.Forms.TextBox();
            this.StrongSipParameterText = new System.Windows.Forms.TextBox();
            this.PuffParameterText = new System.Windows.Forms.TextBox();
            this.StrongPuffParameterText = new System.Windows.Forms.TextBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.clearButtonStrongSipRight = new System.Windows.Forms.Button();
            this.StrongSipRightComboBox = new System.Windows.Forms.ComboBox();
            this.StrongSipRightNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.StrongSipRightLabel = new System.Windows.Forms.Label();
            this.StrongSipRightParameterText = new System.Windows.Forms.TextBox();
            this.label48 = new System.Windows.Forms.Label();
            this.StrongSipRightFunctionBox = new System.Windows.Forms.ComboBox();
            this.clearButtonStrongSipLeft = new System.Windows.Forms.Button();
            this.clearButtonStrongSipDown = new System.Windows.Forms.Button();
            this.clearButtonStrongSipUp = new System.Windows.Forms.Button();
            this.StrongSipLeftComboBox = new System.Windows.Forms.ComboBox();
            this.StrongSipDownComboBox = new System.Windows.Forms.ComboBox();
            this.StrongSipUpComboBox = new System.Windows.Forms.ComboBox();
            this.StrongSipLeftNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.StrongSipDownNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.StrongSipUpNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.StrongSipLeftLabel = new System.Windows.Forms.Label();
            this.StrongSipLeftParameterText = new System.Windows.Forms.TextBox();
            this.StrongSipDownLabel = new System.Windows.Forms.Label();
            this.StrongSipDownParameterText = new System.Windows.Forms.TextBox();
            this.StrongSipUpLabel = new System.Windows.Forms.Label();
            this.StrongSipUpParameterText = new System.Windows.Forms.TextBox();
            this.label52 = new System.Windows.Forms.Label();
            this.StrongSipLeftFunctionBox = new System.Windows.Forms.ComboBox();
            this.label53 = new System.Windows.Forms.Label();
            this.StrongSipDownFunctionBox = new System.Windows.Forms.ComboBox();
            this.label54 = new System.Windows.Forms.Label();
            this.StrongSipUpFunctionBox = new System.Windows.Forms.ComboBox();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.clearButtonStrongPuffRight = new System.Windows.Forms.Button();
            this.StrongPuffRightComboBox = new System.Windows.Forms.ComboBox();
            this.StrongPuffRightNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.StrongPuffRightLabel = new System.Windows.Forms.Label();
            this.StrongPuffRightParameterText = new System.Windows.Forms.TextBox();
            this.label40 = new System.Windows.Forms.Label();
            this.StrongPuffRightFunctionBox = new System.Windows.Forms.ComboBox();
            this.clearButtonStrongPuffLeft = new System.Windows.Forms.Button();
            this.clearButtonStrongPuffDown = new System.Windows.Forms.Button();
            this.clearButtonStrongPuffUp = new System.Windows.Forms.Button();
            this.StrongPuffLeftComboBox = new System.Windows.Forms.ComboBox();
            this.StrongPuffDownComboBox = new System.Windows.Forms.ComboBox();
            this.StrongPuffUpComboBox = new System.Windows.Forms.ComboBox();
            this.StrongPuffLeftNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.StrongPuffDownNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.StrongPuffUpNumericParameter = new System.Windows.Forms.NumericUpDown();
            this.StrongPuffLeftLabel = new System.Windows.Forms.Label();
            this.StrongPuffLeftParameterText = new System.Windows.Forms.TextBox();
            this.StrongPuffDownLabel = new System.Windows.Forms.Label();
            this.StrongPuffDownParameterText = new System.Windows.Forms.TextBox();
            this.StrongPuffUpLabel = new System.Windows.Forms.Label();
            this.StrongPuffUpParameterText = new System.Windows.Forms.TextBox();
            this.label44 = new System.Windows.Forms.Label();
            this.StrongPuffLeftFunctionBox = new System.Windows.Forms.ComboBox();
            this.label45 = new System.Windows.Forms.Label();
            this.StrongPuffDownFunctionBox = new System.Windows.Forms.ComboBox();
            this.label46 = new System.Windows.Forms.Label();
            this.StrongPuffUpFunctionBox = new System.Windows.Forms.ComboBox();
            this.ButtonsTab = new System.Windows.Forms.TabPage();
            this.clearButton3 = new System.Windows.Forms.Button();
            this.clearButton2 = new System.Windows.Forms.Button();
            this.clearButton1 = new System.Windows.Forms.Button();
            this.label21 = new System.Windows.Forms.Label();
            this.Button3ComboBox = new System.Windows.Forms.ComboBox();
            this.Button2ComboBox = new System.Windows.Forms.ComboBox();
            this.Button1ComboBox = new System.Windows.Forms.ComboBox();
            this.Button3NumericParameter = new System.Windows.Forms.NumericUpDown();
            this.Button2NumericParameter = new System.Windows.Forms.NumericUpDown();
            this.Button1NumericParameter = new System.Windows.Forms.NumericUpDown();
            this.Button3Label = new System.Windows.Forms.Label();
            this.Button3ParameterText = new System.Windows.Forms.TextBox();
            this.Button2Label = new System.Windows.Forms.Label();
            this.Button2ParameterText = new System.Windows.Forms.TextBox();
            this.Button1Label = new System.Windows.Forms.Label();
            this.Button1ParameterText = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.Button3FunctionBox = new System.Windows.Forms.ComboBox();
            this.label7 = new System.Windows.Forms.Label();
            this.Button2FunctionBox = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.Button1FunctionBox = new System.Windows.Forms.ComboBox();
            this.RawValueTab = new System.Windows.Forms.TabPage();
            this.incRightGain = new System.Windows.Forms.Button();
            this.decRightGain = new System.Windows.Forms.Button();
            this.rightGainLabel = new System.Windows.Forms.Label();
            this.label28 = new System.Windows.Forms.Label();
            this.rightGainBar = new System.Windows.Forms.TrackBar();
            this.incDownGain = new System.Windows.Forms.Button();
            this.decDownGain = new System.Windows.Forms.Button();
            this.downGainLabel = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.incLeftGain = new System.Windows.Forms.Button();
            this.decLeftGain = new System.Windows.Forms.Button();
            this.leftGainLabel = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.incUpGain = new System.Windows.Forms.Button();
            this.decUpGain = new System.Windows.Forms.Button();
            this.upGainLabel = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.downSensorLabel = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.leftSensorLabel = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.rightSensorLabel = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.upSensorLabel = new System.Windows.Forms.Label();
            this.rightPanel = new System.Windows.Forms.Panel();
            this.leftPanel = new System.Windows.Forms.Panel();
            this.downPanel = new System.Windows.Forms.Panel();
            this.upPanel = new System.Windows.Forms.Panel();
            this.leftGainBar = new System.Windows.Forms.TrackBar();
            this.downGainBar = new System.Windows.Forms.TrackBar();
            this.upGainBar = new System.Windows.Forms.TrackBar();
            this.IRTab = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.irTimeoutBox = new System.Windows.Forms.TextBox();
            this.irCommandBox = new System.Windows.Forms.ComboBox();
            this.deleteIRButton = new System.Windows.Forms.Button();
            this.label31 = new System.Windows.Forms.Label();
            this.playIRButton = new System.Windows.Forms.Button();
            this.recordIRButton = new System.Windows.Forms.Button();
            this.label24 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.activityLogTextbox = new System.Windows.Forms.RichTextBox();
            this.slotNames = new System.Windows.Forms.ComboBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToFileMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadFromFileMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fLipMouseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.storeSlotSettingsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadSlotSettingsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.prevSlotButton = new System.Windows.Forms.Button();
            this.nextSlotButton = new System.Windows.Forms.Button();
            this.ApplyButton = new System.Windows.Forms.Button();
            this.newSlotButton = new System.Windows.Forms.Button();
            this.deleteSlotButton = new System.Windows.Forms.Button();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.upIndicatorPanel = new System.Windows.Forms.Panel();
            this.downIndicatorPanel = new System.Windows.Forms.Panel();
            this.rightIndicatorPanel = new System.Windows.Forms.Panel();
            this.leftIndicatorPanel = new System.Windows.Forms.Panel();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label39 = new System.Windows.Forms.Label();
            this.label38 = new System.Windows.Forms.Label();
            this.label37 = new System.Windows.Forms.Label();
            this.label33 = new System.Windows.Forms.Label();
            this.StoreButton = new System.Windows.Forms.Button();
            this.label32 = new System.Windows.Forms.Label();
            this.label35 = new System.Windows.Forms.Label();
            this.label41 = new System.Windows.Forms.Label();
            this.tabControl.SuspendLayout();
            this.LipmouseTab.SuspendLayout();
            this.singlePanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.deadzoneBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.speedBar)).BeginInit();
            this.splitPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.deadzoneYBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.speedYBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.deadzoneXBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.speedXBar)).BeginInit();
            this.AlternativeTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RightNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.LeftNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.DownNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpNumericParameter)).BeginInit();
            this.SipPuffLevelsTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.strongSipThresholdBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.strongPuffThresholdBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.puffThresholdBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.sipThresholdBar)).BeginInit();
            this.SipPuffTab.SuspendLayout();
            this.sipPuffSelections.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PuffNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SipNumericParameter)).BeginInit();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipRightNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipLeftNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipDownNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipUpNumericParameter)).BeginInit();
            this.tabPage3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffRightNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffLeftNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffDownNumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffUpNumericParameter)).BeginInit();
            this.ButtonsTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Button3NumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Button2NumericParameter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Button1NumericParameter)).BeginInit();
            this.RawValueTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.rightGainBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.leftGainBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.downGainBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.upGainBar)).BeginInit();
            this.IRTab.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 115200;
            // 
            // portComboBox
            // 
            this.portComboBox.Location = new System.Drawing.Point(166, 54);
            this.portComboBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.portComboBox.Name = "portComboBox";
            this.portComboBox.Size = new System.Drawing.Size(119, 24);
            this.portComboBox.TabIndex = 60;
            this.portComboBox.Click += new System.EventHandler(this.portComboBox_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(51, 57);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(112, 17);
            this.label3.TabIndex = 7;
            this.label3.Text = "FLipMouse-Port:";
            // 
            // connectComButton
            // 
            this.connectComButton.Location = new System.Drawing.Point(394, 51);
            this.connectComButton.Margin = new System.Windows.Forms.Padding(4);
            this.connectComButton.Name = "connectComButton";
            this.connectComButton.Size = new System.Drawing.Size(83, 28);
            this.connectComButton.TabIndex = 8;
            this.connectComButton.Text = "Connect";
            this.connectComButton.UseVisualStyleBackColor = true;
            this.connectComButton.Click += new System.EventHandler(this.connectComButton_click);
            // 
            // disconnectComButton
            // 
            this.disconnectComButton.Enabled = false;
            this.disconnectComButton.ForeColor = System.Drawing.Color.Black;
            this.disconnectComButton.Location = new System.Drawing.Point(485, 51);
            this.disconnectComButton.Margin = new System.Windows.Forms.Padding(4);
            this.disconnectComButton.Name = "disconnectComButton";
            this.disconnectComButton.Size = new System.Drawing.Size(93, 28);
            this.disconnectComButton.TabIndex = 10;
            this.disconnectComButton.Text = "Disconnect";
            this.disconnectComButton.UseVisualStyleBackColor = true;
            this.disconnectComButton.Click += new System.EventHandler(this.disconnnectComButton_Click);
            // 
            // portStatus
            // 
            this.portStatus.AutoSize = true;
            this.portStatus.ForeColor = System.Drawing.Color.SlateGray;
            this.portStatus.Location = new System.Drawing.Point(292, 57);
            this.portStatus.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.portStatus.Name = "portStatus";
            this.portStatus.Size = new System.Drawing.Size(94, 17);
            this.portStatus.TabIndex = 12;
            this.portStatus.Text = "Disconnected";
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.LipmouseTab);
            this.tabControl.Controls.Add(this.AlternativeTab);
            this.tabControl.Controls.Add(this.SipPuffLevelsTab);
            this.tabControl.Controls.Add(this.SipPuffTab);
            this.tabControl.Controls.Add(this.ButtonsTab);
            this.tabControl.Controls.Add(this.RawValueTab);
            this.tabControl.Controls.Add(this.IRTab);
            this.tabControl.Location = new System.Drawing.Point(50, 103);
            this.tabControl.Margin = new System.Windows.Forms.Padding(4);
            this.tabControl.Multiline = true;
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(788, 390);
            this.tabControl.TabIndex = 61;
            this.tabControl.SelectedIndexChanged += new System.EventHandler(this.tabControl_SelectedIndexChanged);
            // 
            // LipmouseTab
            // 
            this.LipmouseTab.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.LipmouseTab.Controls.Add(this.joyModeLabel);
            this.LipmouseTab.Controls.Add(this.joyModeBox);
            this.LipmouseTab.Controls.Add(this.label27);
            this.LipmouseTab.Controls.Add(this.selectJoystick);
            this.LipmouseTab.Controls.Add(this.singlePanel);
            this.LipmouseTab.Controls.Add(this.splitPanel);
            this.LipmouseTab.Controls.Add(this.splitXYBox);
            this.LipmouseTab.Controls.Add(this.selectAlternative);
            this.LipmouseTab.Controls.Add(this.selectStick);
            this.LipmouseTab.Controls.Add(this.calButton);
            this.LipmouseTab.Location = new System.Drawing.Point(4, 25);
            this.LipmouseTab.Margin = new System.Windows.Forms.Padding(4);
            this.LipmouseTab.Name = "LipmouseTab";
            this.LipmouseTab.Padding = new System.Windows.Forms.Padding(4);
            this.LipmouseTab.Size = new System.Drawing.Size(780, 361);
            this.LipmouseTab.TabIndex = 0;
            this.LipmouseTab.Text = "Stick Actions";
            // 
            // joyModeLabel
            // 
            this.joyModeLabel.AutoSize = true;
            this.joyModeLabel.Location = new System.Drawing.Point(55, 283);
            this.joyModeLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.joyModeLabel.Name = "joyModeLabel";
            this.joyModeLabel.Size = new System.Drawing.Size(134, 17);
            this.joyModeLabel.TabIndex = 132;
            this.joyModeLabel.Text = "Select Joystick Axis:";
            this.joyModeLabel.Visible = false;
            // 
            // joyModeBox
            // 
            this.joyModeBox.FormattingEnabled = true;
            this.joyModeBox.Location = new System.Drawing.Point(56, 305);
            this.joyModeBox.Margin = new System.Windows.Forms.Padding(4);
            this.joyModeBox.Name = "joyModeBox";
            this.joyModeBox.Size = new System.Drawing.Size(144, 24);
            this.joyModeBox.TabIndex = 131;
            this.joyModeBox.Visible = false;
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(62, 30);
            this.label27.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(92, 17);
            this.label27.TabIndex = 83;
            this.label27.Text = "Use Stick for:";
            // 
            // selectJoystick
            // 
            this.selectJoystick.AutoSize = true;
            this.selectJoystick.Location = new System.Drawing.Point(390, 31);
            this.selectJoystick.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.selectJoystick.Name = "selectJoystick";
            this.selectJoystick.Size = new System.Drawing.Size(148, 21);
            this.selectJoystick.TabIndex = 23;
            this.selectJoystick.Text = "Joystick Movement";
            this.selectJoystick.UseVisualStyleBackColor = true;
            this.selectJoystick.CheckedChanged += new System.EventHandler(this.selectJoystick_CheckedChanged);
            // 
            // singlePanel
            // 
            this.singlePanel.Controls.Add(this.incSpeed);
            this.singlePanel.Controls.Add(this.decSpeed);
            this.singlePanel.Controls.Add(this.incDeadzone);
            this.singlePanel.Controls.Add(this.decDeadzone);
            this.singlePanel.Controls.Add(this.deadzoneLabel);
            this.singlePanel.Controls.Add(this.label34);
            this.singlePanel.Controls.Add(this.deadzoneBar);
            this.singlePanel.Controls.Add(this.speedLabel);
            this.singlePanel.Controls.Add(this.speedBar);
            this.singlePanel.Controls.Add(this.label36);
            this.singlePanel.Location = new System.Drawing.Point(25, 64);
            this.singlePanel.Name = "singlePanel";
            this.singlePanel.Size = new System.Drawing.Size(724, 203);
            this.singlePanel.TabIndex = 65;
            // 
            // incSpeed
            // 
            this.incSpeed.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incSpeed.ForeColor = System.Drawing.Color.Black;
            this.incSpeed.Location = new System.Drawing.Point(643, 37);
            this.incSpeed.Margin = new System.Windows.Forms.Padding(4);
            this.incSpeed.Name = "incSpeed";
            this.incSpeed.Size = new System.Drawing.Size(41, 37);
            this.incSpeed.TabIndex = 74;
            this.incSpeed.Text = "+";
            this.incSpeed.UseVisualStyleBackColor = true;
            this.incSpeed.Click += new System.EventHandler(this.incSpeed_Click);
            this.incSpeed.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incSpeed.MouseHover += new System.EventHandler(this.incSpeed_MouseHover);
            // 
            // decSpeed
            // 
            this.decSpeed.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decSpeed.ForeColor = System.Drawing.Color.Black;
            this.decSpeed.Location = new System.Drawing.Point(42, 37);
            this.decSpeed.Margin = new System.Windows.Forms.Padding(4);
            this.decSpeed.Name = "decSpeed";
            this.decSpeed.Size = new System.Drawing.Size(41, 37);
            this.decSpeed.TabIndex = 73;
            this.decSpeed.Text = "-";
            this.decSpeed.UseVisualStyleBackColor = true;
            this.decSpeed.Click += new System.EventHandler(this.decSpeed_Click);
            this.decSpeed.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decSpeed.MouseHover += new System.EventHandler(this.decSpeed_MouseHover);
            // 
            // incDeadzone
            // 
            this.incDeadzone.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incDeadzone.ForeColor = System.Drawing.Color.Black;
            this.incDeadzone.Location = new System.Drawing.Point(641, 137);
            this.incDeadzone.Margin = new System.Windows.Forms.Padding(4);
            this.incDeadzone.Name = "incDeadzone";
            this.incDeadzone.Size = new System.Drawing.Size(41, 37);
            this.incDeadzone.TabIndex = 72;
            this.incDeadzone.Text = "+";
            this.incDeadzone.UseVisualStyleBackColor = true;
            this.incDeadzone.Click += new System.EventHandler(this.incDeadzone_Click);
            this.incDeadzone.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incDeadzone.MouseHover += new System.EventHandler(this.incDeadzone_MouseHover);
            // 
            // decDeadzone
            // 
            this.decDeadzone.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decDeadzone.ForeColor = System.Drawing.Color.Black;
            this.decDeadzone.Location = new System.Drawing.Point(42, 137);
            this.decDeadzone.Margin = new System.Windows.Forms.Padding(4);
            this.decDeadzone.Name = "decDeadzone";
            this.decDeadzone.Size = new System.Drawing.Size(41, 37);
            this.decDeadzone.TabIndex = 71;
            this.decDeadzone.Text = "-";
            this.decDeadzone.UseVisualStyleBackColor = true;
            this.decDeadzone.Click += new System.EventHandler(this.decDeadzone_Click);
            this.decDeadzone.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decDeadzone.MouseHover += new System.EventHandler(this.decDeadzone_MouseHover);
            // 
            // deadzoneLabel
            // 
            this.deadzoneLabel.AutoSize = true;
            this.deadzoneLabel.Location = new System.Drawing.Point(592, 122);
            this.deadzoneLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.deadzoneLabel.Name = "deadzoneLabel";
            this.deadzoneLabel.Size = new System.Drawing.Size(32, 17);
            this.deadzoneLabel.TabIndex = 70;
            this.deadzoneLabel.Text = "100";
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.Location = new System.Drawing.Point(109, 122);
            this.label34.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(73, 17);
            this.label34.TabIndex = 69;
            this.label34.Text = "Deadzone";
            // 
            // deadzoneBar
            // 
            this.deadzoneBar.LargeChange = 10;
            this.deadzoneBar.Location = new System.Drawing.Point(97, 145);
            this.deadzoneBar.Margin = new System.Windows.Forms.Padding(4);
            this.deadzoneBar.Maximum = 650;
            this.deadzoneBar.Minimum = 1;
            this.deadzoneBar.Name = "deadzoneBar";
            this.deadzoneBar.Size = new System.Drawing.Size(534, 56);
            this.deadzoneBar.TabIndex = 68;
            this.deadzoneBar.TickFrequency = 20;
            this.deadzoneBar.Value = 100;
            this.deadzoneBar.Scroll += new System.EventHandler(this.deadzoneBar_Scroll);
            // 
            // speedLabel
            // 
            this.speedLabel.AutoSize = true;
            this.speedLabel.Location = new System.Drawing.Point(600, 23);
            this.speedLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.speedLabel.Name = "speedLabel";
            this.speedLabel.Size = new System.Drawing.Size(24, 17);
            this.speedLabel.TabIndex = 67;
            this.speedLabel.Text = "40";
            // 
            // speedBar
            // 
            this.speedBar.AutoSize = false;
            this.speedBar.LargeChange = 10;
            this.speedBar.Location = new System.Drawing.Point(97, 44);
            this.speedBar.Margin = new System.Windows.Forms.Padding(4);
            this.speedBar.Maximum = 255;
            this.speedBar.Minimum = 1;
            this.speedBar.Name = "speedBar";
            this.speedBar.Size = new System.Drawing.Size(534, 55);
            this.speedBar.TabIndex = 66;
            this.speedBar.TickFrequency = 5;
            this.speedBar.Value = 40;
            this.speedBar.Scroll += new System.EventHandler(this.speedBar_Scroll);
            // 
            // label36
            // 
            this.label36.AutoSize = true;
            this.label36.Location = new System.Drawing.Point(109, 25);
            this.label36.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(49, 17);
            this.label36.TabIndex = 65;
            this.label36.Text = "Speed";
            // 
            // splitPanel
            // 
            this.splitPanel.Controls.Add(this.incDeadzoneY);
            this.splitPanel.Controls.Add(this.decDeadzoneY);
            this.splitPanel.Controls.Add(this.incSpeedY);
            this.splitPanel.Controls.Add(this.decSpeedY);
            this.splitPanel.Controls.Add(this.incSpeedX);
            this.splitPanel.Controls.Add(this.decSpeedX);
            this.splitPanel.Controls.Add(this.incDeadzoneX);
            this.splitPanel.Controls.Add(this.decDeadzoneX);
            this.splitPanel.Controls.Add(this.DeadzoneYNameLabel);
            this.splitPanel.Controls.Add(this.SpeedYNameLabel);
            this.splitPanel.Controls.Add(this.speedYLabel);
            this.splitPanel.Controls.Add(this.deadzoneYLabel);
            this.splitPanel.Controls.Add(this.deadzoneYBar);
            this.splitPanel.Controls.Add(this.speedYBar);
            this.splitPanel.Controls.Add(this.deadzoneXLabel);
            this.splitPanel.Controls.Add(this.DeadzoneXNameLabel);
            this.splitPanel.Controls.Add(this.deadzoneXBar);
            this.splitPanel.Controls.Add(this.speedXLabel);
            this.splitPanel.Controls.Add(this.speedXBar);
            this.splitPanel.Controls.Add(this.SpeedXNameLabel);
            this.splitPanel.Enabled = false;
            this.splitPanel.Location = new System.Drawing.Point(23, 67);
            this.splitPanel.Name = "splitPanel";
            this.splitPanel.Size = new System.Drawing.Size(729, 202);
            this.splitPanel.TabIndex = 39;
            this.splitPanel.Visible = false;
            // 
            // incDeadzoneY
            // 
            this.incDeadzoneY.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incDeadzoneY.ForeColor = System.Drawing.Color.Black;
            this.incDeadzoneY.Location = new System.Drawing.Point(678, 136);
            this.incDeadzoneY.Margin = new System.Windows.Forms.Padding(4);
            this.incDeadzoneY.Name = "incDeadzoneY";
            this.incDeadzoneY.Size = new System.Drawing.Size(41, 37);
            this.incDeadzoneY.TabIndex = 58;
            this.incDeadzoneY.Text = "+";
            this.incDeadzoneY.UseVisualStyleBackColor = true;
            this.incDeadzoneY.Click += new System.EventHandler(this.incDeadzoneY_Click);
            this.incDeadzoneY.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incDeadzoneY.MouseHover += new System.EventHandler(this.incDeadzoneY_MouseHover);
            // 
            // decDeadzoneY
            // 
            this.decDeadzoneY.AccessibleDescription = " ";
            this.decDeadzoneY.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decDeadzoneY.ForeColor = System.Drawing.Color.Black;
            this.decDeadzoneY.Location = new System.Drawing.Point(379, 138);
            this.decDeadzoneY.Margin = new System.Windows.Forms.Padding(4);
            this.decDeadzoneY.Name = "decDeadzoneY";
            this.decDeadzoneY.Size = new System.Drawing.Size(41, 37);
            this.decDeadzoneY.TabIndex = 57;
            this.decDeadzoneY.Text = "-";
            this.decDeadzoneY.UseVisualStyleBackColor = true;
            this.decDeadzoneY.Click += new System.EventHandler(this.decDeadzoneY_Click);
            this.decDeadzoneY.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decDeadzoneY.MouseHover += new System.EventHandler(this.decDeadzoneY_MouseHover);
            // 
            // incSpeedY
            // 
            this.incSpeedY.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incSpeedY.ForeColor = System.Drawing.Color.Black;
            this.incSpeedY.Location = new System.Drawing.Point(678, 36);
            this.incSpeedY.Margin = new System.Windows.Forms.Padding(4);
            this.incSpeedY.Name = "incSpeedY";
            this.incSpeedY.Size = new System.Drawing.Size(41, 37);
            this.incSpeedY.TabIndex = 56;
            this.incSpeedY.Text = "+";
            this.incSpeedY.UseVisualStyleBackColor = true;
            this.incSpeedY.Click += new System.EventHandler(this.incSpeedY_Click);
            this.incSpeedY.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incSpeedY.MouseHover += new System.EventHandler(this.incSpeedY_MouseHover);
            // 
            // decSpeedY
            // 
            this.decSpeedY.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decSpeedY.ForeColor = System.Drawing.Color.Black;
            this.decSpeedY.Location = new System.Drawing.Point(379, 36);
            this.decSpeedY.Margin = new System.Windows.Forms.Padding(4);
            this.decSpeedY.Name = "decSpeedY";
            this.decSpeedY.Size = new System.Drawing.Size(41, 37);
            this.decSpeedY.TabIndex = 55;
            this.decSpeedY.Text = "-";
            this.decSpeedY.UseVisualStyleBackColor = true;
            this.decSpeedY.Click += new System.EventHandler(this.decSpeedY_Click);
            this.decSpeedY.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decSpeedY.MouseHover += new System.EventHandler(this.decSpeedY_MouseHover);
            // 
            // incSpeedX
            // 
            this.incSpeedX.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incSpeedX.ForeColor = System.Drawing.Color.Black;
            this.incSpeedX.Location = new System.Drawing.Point(313, 36);
            this.incSpeedX.Margin = new System.Windows.Forms.Padding(4);
            this.incSpeedX.Name = "incSpeedX";
            this.incSpeedX.Size = new System.Drawing.Size(41, 37);
            this.incSpeedX.TabIndex = 54;
            this.incSpeedX.Text = "+";
            this.incSpeedX.UseVisualStyleBackColor = true;
            this.incSpeedX.Click += new System.EventHandler(this.incSpeedX_Click);
            this.incSpeedX.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incSpeedX.MouseHover += new System.EventHandler(this.incSpeedX_MouseHover);
            // 
            // decSpeedX
            // 
            this.decSpeedX.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decSpeedX.ForeColor = System.Drawing.Color.Black;
            this.decSpeedX.Location = new System.Drawing.Point(14, 36);
            this.decSpeedX.Margin = new System.Windows.Forms.Padding(4);
            this.decSpeedX.Name = "decSpeedX";
            this.decSpeedX.Size = new System.Drawing.Size(41, 37);
            this.decSpeedX.TabIndex = 53;
            this.decSpeedX.Text = "-";
            this.decSpeedX.UseVisualStyleBackColor = true;
            this.decSpeedX.Click += new System.EventHandler(this.decSpeedX_Click);
            this.decSpeedX.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decSpeedX.MouseHover += new System.EventHandler(this.decSpeedX_MouseHover);
            // 
            // incDeadzoneX
            // 
            this.incDeadzoneX.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incDeadzoneX.ForeColor = System.Drawing.Color.Black;
            this.incDeadzoneX.Location = new System.Drawing.Point(312, 138);
            this.incDeadzoneX.Margin = new System.Windows.Forms.Padding(4);
            this.incDeadzoneX.Name = "incDeadzoneX";
            this.incDeadzoneX.Size = new System.Drawing.Size(41, 37);
            this.incDeadzoneX.TabIndex = 52;
            this.incDeadzoneX.Text = "+";
            this.incDeadzoneX.UseVisualStyleBackColor = true;
            this.incDeadzoneX.Click += new System.EventHandler(this.incDeadzoneX_Click);
            this.incDeadzoneX.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incDeadzoneX.MouseHover += new System.EventHandler(this.incDeadzoneX_MouseHover);
            // 
            // decDeadzoneX
            // 
            this.decDeadzoneX.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decDeadzoneX.ForeColor = System.Drawing.Color.Black;
            this.decDeadzoneX.Location = new System.Drawing.Point(14, 136);
            this.decDeadzoneX.Margin = new System.Windows.Forms.Padding(4);
            this.decDeadzoneX.Name = "decDeadzoneX";
            this.decDeadzoneX.Size = new System.Drawing.Size(41, 37);
            this.decDeadzoneX.TabIndex = 51;
            this.decDeadzoneX.Text = "-";
            this.decDeadzoneX.UseVisualStyleBackColor = true;
            this.decDeadzoneX.Click += new System.EventHandler(this.decDeadzoneX_Click);
            this.decDeadzoneX.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decDeadzoneX.MouseHover += new System.EventHandler(this.decDeadzoneX_MouseHover);
            // 
            // DeadzoneYNameLabel
            // 
            this.DeadzoneYNameLabel.AutoSize = true;
            this.DeadzoneYNameLabel.Location = new System.Drawing.Point(442, 121);
            this.DeadzoneYNameLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.DeadzoneYNameLabel.Name = "DeadzoneYNameLabel";
            this.DeadzoneYNameLabel.Size = new System.Drawing.Size(87, 17);
            this.DeadzoneYNameLabel.TabIndex = 50;
            this.DeadzoneYNameLabel.Text = "Deadzone-Y";
            // 
            // SpeedYNameLabel
            // 
            this.SpeedYNameLabel.AutoSize = true;
            this.SpeedYNameLabel.Location = new System.Drawing.Point(442, 23);
            this.SpeedYNameLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.SpeedYNameLabel.Name = "SpeedYNameLabel";
            this.SpeedYNameLabel.Size = new System.Drawing.Size(63, 17);
            this.SpeedYNameLabel.TabIndex = 49;
            this.SpeedYNameLabel.Text = "Speed-Y";
            // 
            // speedYLabel
            // 
            this.speedYLabel.AutoSize = true;
            this.speedYLabel.Location = new System.Drawing.Point(640, 24);
            this.speedYLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.speedYLabel.Name = "speedYLabel";
            this.speedYLabel.Size = new System.Drawing.Size(24, 17);
            this.speedYLabel.TabIndex = 48;
            this.speedYLabel.Text = "40";
            // 
            // deadzoneYLabel
            // 
            this.deadzoneYLabel.AutoSize = true;
            this.deadzoneYLabel.Location = new System.Drawing.Point(631, 121);
            this.deadzoneYLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.deadzoneYLabel.Name = "deadzoneYLabel";
            this.deadzoneYLabel.Size = new System.Drawing.Size(32, 17);
            this.deadzoneYLabel.TabIndex = 47;
            this.deadzoneYLabel.Text = "100";
            // 
            // deadzoneYBar
            // 
            this.deadzoneYBar.AutoSize = false;
            this.deadzoneYBar.LargeChange = 10;
            this.deadzoneYBar.Location = new System.Drawing.Point(419, 143);
            this.deadzoneYBar.Margin = new System.Windows.Forms.Padding(4);
            this.deadzoneYBar.Maximum = 650;
            this.deadzoneYBar.Minimum = 1;
            this.deadzoneYBar.Name = "deadzoneYBar";
            this.deadzoneYBar.Size = new System.Drawing.Size(258, 55);
            this.deadzoneYBar.TabIndex = 46;
            this.deadzoneYBar.TickFrequency = 25;
            this.deadzoneYBar.Value = 100;
            this.deadzoneYBar.Scroll += new System.EventHandler(this.deadzoneYBar_Scroll);
            // 
            // speedYBar
            // 
            this.speedYBar.AutoSize = false;
            this.speedYBar.LargeChange = 1;
            this.speedYBar.Location = new System.Drawing.Point(419, 43);
            this.speedYBar.Margin = new System.Windows.Forms.Padding(4);
            this.speedYBar.Maximum = 255;
            this.speedYBar.Minimum = 1;
            this.speedYBar.Name = "speedYBar";
            this.speedYBar.Size = new System.Drawing.Size(258, 55);
            this.speedYBar.TabIndex = 45;
            this.speedYBar.TickFrequency = 5;
            this.speedYBar.Value = 40;
            this.speedYBar.Scroll += new System.EventHandler(this.speedYBar_Scroll);
            // 
            // deadzoneXLabel
            // 
            this.deadzoneXLabel.AutoSize = true;
            this.deadzoneXLabel.Location = new System.Drawing.Point(267, 121);
            this.deadzoneXLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.deadzoneXLabel.Name = "deadzoneXLabel";
            this.deadzoneXLabel.Size = new System.Drawing.Size(32, 17);
            this.deadzoneXLabel.TabIndex = 44;
            this.deadzoneXLabel.Text = "100";
            // 
            // DeadzoneXNameLabel
            // 
            this.DeadzoneXNameLabel.AutoSize = true;
            this.DeadzoneXNameLabel.Location = new System.Drawing.Point(69, 121);
            this.DeadzoneXNameLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.DeadzoneXNameLabel.Name = "DeadzoneXNameLabel";
            this.DeadzoneXNameLabel.Size = new System.Drawing.Size(87, 17);
            this.DeadzoneXNameLabel.TabIndex = 43;
            this.DeadzoneXNameLabel.Text = "Deadzone-X";
            // 
            // deadzoneXBar
            // 
            this.deadzoneXBar.LargeChange = 10;
            this.deadzoneXBar.Location = new System.Drawing.Point(56, 144);
            this.deadzoneXBar.Margin = new System.Windows.Forms.Padding(4);
            this.deadzoneXBar.Maximum = 650;
            this.deadzoneXBar.Minimum = 1;
            this.deadzoneXBar.Name = "deadzoneXBar";
            this.deadzoneXBar.Size = new System.Drawing.Size(257, 56);
            this.deadzoneXBar.TabIndex = 42;
            this.deadzoneXBar.TickFrequency = 20;
            this.deadzoneXBar.Value = 100;
            this.deadzoneXBar.Scroll += new System.EventHandler(this.deadzoneX_Scroll);
            // 
            // speedXLabel
            // 
            this.speedXLabel.AutoSize = true;
            this.speedXLabel.Location = new System.Drawing.Point(275, 24);
            this.speedXLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.speedXLabel.Name = "speedXLabel";
            this.speedXLabel.Size = new System.Drawing.Size(24, 17);
            this.speedXLabel.TabIndex = 41;
            this.speedXLabel.Text = "40";
            // 
            // speedXBar
            // 
            this.speedXBar.AutoSize = false;
            this.speedXBar.LargeChange = 1;
            this.speedXBar.Location = new System.Drawing.Point(56, 43);
            this.speedXBar.Margin = new System.Windows.Forms.Padding(4);
            this.speedXBar.Maximum = 255;
            this.speedXBar.Minimum = 1;
            this.speedXBar.Name = "speedXBar";
            this.speedXBar.Size = new System.Drawing.Size(257, 55);
            this.speedXBar.TabIndex = 40;
            this.speedXBar.TickFrequency = 5;
            this.speedXBar.Value = 40;
            this.speedXBar.Scroll += new System.EventHandler(this.speedXBar_Scroll);
            // 
            // SpeedXNameLabel
            // 
            this.SpeedXNameLabel.AutoSize = true;
            this.SpeedXNameLabel.Location = new System.Drawing.Point(66, 24);
            this.SpeedXNameLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.SpeedXNameLabel.Name = "SpeedXNameLabel";
            this.SpeedXNameLabel.Size = new System.Drawing.Size(63, 17);
            this.SpeedXNameLabel.TabIndex = 39;
            this.SpeedXNameLabel.Text = "Speed-X";
            // 
            // splitXYBox
            // 
            this.splitXYBox.AutoSize = true;
            this.splitXYBox.Location = new System.Drawing.Point(605, 304);
            this.splitXYBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.splitXYBox.Name = "splitXYBox";
            this.splitXYBox.Size = new System.Drawing.Size(81, 21);
            this.splitXYBox.TabIndex = 24;
            this.splitXYBox.Text = "split X/Y";
            this.splitXYBox.UseVisualStyleBackColor = true;
            this.splitXYBox.CheckedChanged += new System.EventHandler(this.splitXYBox_CheckedChanged);
            // 
            // selectAlternative
            // 
            this.selectAlternative.AutoSize = true;
            this.selectAlternative.Location = new System.Drawing.Point(559, 32);
            this.selectAlternative.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.selectAlternative.Name = "selectAlternative";
            this.selectAlternative.Size = new System.Drawing.Size(146, 21);
            this.selectAlternative.TabIndex = 24;
            this.selectAlternative.Text = "Alternative Actions";
            this.selectAlternative.UseVisualStyleBackColor = true;
            this.selectAlternative.CheckedChanged += new System.EventHandler(this.selectAlternative_Checked);
            // 
            // selectStick
            // 
            this.selectStick.AutoSize = true;
            this.selectStick.Checked = true;
            this.selectStick.Location = new System.Drawing.Point(188, 30);
            this.selectStick.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.selectStick.Name = "selectStick";
            this.selectStick.Size = new System.Drawing.Size(182, 21);
            this.selectStick.TabIndex = 22;
            this.selectStick.TabStop = true;
            this.selectStick.Text = "MouseCursor Movement";
            this.selectStick.UseVisualStyleBackColor = true;
            this.selectStick.CheckedChanged += new System.EventHandler(this.selectStick_Checked);
            // 
            // calButton
            // 
            this.calButton.Enabled = false;
            this.calButton.ForeColor = System.Drawing.Color.Black;
            this.calButton.Location = new System.Drawing.Point(248, 285);
            this.calButton.Margin = new System.Windows.Forms.Padding(4);
            this.calButton.Name = "calButton";
            this.calButton.Size = new System.Drawing.Size(291, 59);
            this.calButton.TabIndex = 18;
            this.calButton.Text = "Calibrate Middle Position";
            this.calButton.UseVisualStyleBackColor = true;
            this.calButton.Click += new System.EventHandler(this.calibration_Click);
            // 
            // AlternativeTab
            // 
            this.AlternativeTab.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.AlternativeTab.Controls.Add(this.clearButtonRight);
            this.AlternativeTab.Controls.Add(this.clearButtonLeft);
            this.AlternativeTab.Controls.Add(this.clearButtonDown);
            this.AlternativeTab.Controls.Add(this.clearButtonUp);
            this.AlternativeTab.Controls.Add(this.label18);
            this.AlternativeTab.Controls.Add(this.RightComboBox);
            this.AlternativeTab.Controls.Add(this.RightNumericParameter);
            this.AlternativeTab.Controls.Add(this.RightLabel);
            this.AlternativeTab.Controls.Add(this.RightParameterText);
            this.AlternativeTab.Controls.Add(this.label15);
            this.AlternativeTab.Controls.Add(this.RightFunctionMenu);
            this.AlternativeTab.Controls.Add(this.LeftComboBox);
            this.AlternativeTab.Controls.Add(this.DownComboBox);
            this.AlternativeTab.Controls.Add(this.UpComboBox);
            this.AlternativeTab.Controls.Add(this.LeftNumericParameter);
            this.AlternativeTab.Controls.Add(this.DownNumericParameter);
            this.AlternativeTab.Controls.Add(this.UpNumericParameter);
            this.AlternativeTab.Controls.Add(this.LeftLabel);
            this.AlternativeTab.Controls.Add(this.LeftParameterText);
            this.AlternativeTab.Controls.Add(this.DownLabel);
            this.AlternativeTab.Controls.Add(this.DownParameterText);
            this.AlternativeTab.Controls.Add(this.UpLabel);
            this.AlternativeTab.Controls.Add(this.UpParameterText);
            this.AlternativeTab.Controls.Add(this.label11);
            this.AlternativeTab.Controls.Add(this.LeftFunctionMenu);
            this.AlternativeTab.Controls.Add(this.label12);
            this.AlternativeTab.Controls.Add(this.DownFunctionMenu);
            this.AlternativeTab.Controls.Add(this.label13);
            this.AlternativeTab.Controls.Add(this.UpFunctionMenu);
            this.AlternativeTab.Location = new System.Drawing.Point(4, 25);
            this.AlternativeTab.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.AlternativeTab.Name = "AlternativeTab";
            this.AlternativeTab.Size = new System.Drawing.Size(780, 361);
            this.AlternativeTab.TabIndex = 3;
            this.AlternativeTab.Text = "Alternative Actions";
            // 
            // clearButtonRight
            // 
            this.clearButtonRight.Enabled = false;
            this.clearButtonRight.ForeColor = System.Drawing.Color.Black;
            this.clearButtonRight.Location = new System.Drawing.Point(533, 298);
            this.clearButtonRight.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonRight.Name = "clearButtonRight";
            this.clearButtonRight.Size = new System.Drawing.Size(36, 28);
            this.clearButtonRight.TabIndex = 143;
            this.clearButtonRight.Text = "x";
            this.clearButtonRight.UseVisualStyleBackColor = true;
            this.clearButtonRight.Visible = false;
            this.clearButtonRight.Click += new System.EventHandler(this.clearButtonRight_Click);
            // 
            // clearButtonLeft
            // 
            this.clearButtonLeft.Enabled = false;
            this.clearButtonLeft.ForeColor = System.Drawing.Color.Black;
            this.clearButtonLeft.Location = new System.Drawing.Point(533, 231);
            this.clearButtonLeft.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonLeft.Name = "clearButtonLeft";
            this.clearButtonLeft.Size = new System.Drawing.Size(36, 28);
            this.clearButtonLeft.TabIndex = 142;
            this.clearButtonLeft.Text = "x";
            this.clearButtonLeft.UseVisualStyleBackColor = true;
            this.clearButtonLeft.Visible = false;
            this.clearButtonLeft.Click += new System.EventHandler(this.clearButtonLeft_Click);
            // 
            // clearButtonDown
            // 
            this.clearButtonDown.Enabled = false;
            this.clearButtonDown.ForeColor = System.Drawing.Color.Black;
            this.clearButtonDown.Location = new System.Drawing.Point(533, 162);
            this.clearButtonDown.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonDown.Name = "clearButtonDown";
            this.clearButtonDown.Size = new System.Drawing.Size(36, 28);
            this.clearButtonDown.TabIndex = 141;
            this.clearButtonDown.Text = "x";
            this.clearButtonDown.UseVisualStyleBackColor = true;
            this.clearButtonDown.Visible = false;
            this.clearButtonDown.Click += new System.EventHandler(this.clearButtonDown_Click);
            // 
            // clearButtonUp
            // 
            this.clearButtonUp.Enabled = false;
            this.clearButtonUp.ForeColor = System.Drawing.Color.Black;
            this.clearButtonUp.Location = new System.Drawing.Point(533, 94);
            this.clearButtonUp.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonUp.Name = "clearButtonUp";
            this.clearButtonUp.Size = new System.Drawing.Size(36, 28);
            this.clearButtonUp.TabIndex = 140;
            this.clearButtonUp.Text = "x";
            this.clearButtonUp.UseVisualStyleBackColor = true;
            this.clearButtonUp.Visible = false;
            this.clearButtonUp.Click += new System.EventHandler(this.clearButtonUp_Click);
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(215, 29);
            this.label18.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(300, 17);
            this.label18.TabIndex = 139;
            this.label18.Text = "Select alternative actions for stick-movements:";
            // 
            // RightComboBox
            // 
            this.RightComboBox.FormattingEnabled = true;
            this.RightComboBox.Location = new System.Drawing.Point(533, 270);
            this.RightComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.RightComboBox.Name = "RightComboBox";
            this.RightComboBox.Size = new System.Drawing.Size(161, 24);
            this.RightComboBox.TabIndex = 138;
            this.RightComboBox.Visible = false;
            this.RightComboBox.SelectedIndexChanged += new System.EventHandler(this.RightComboBox_SelectedIndexChanged);
            // 
            // RightNumericParameter
            // 
            this.RightNumericParameter.Location = new System.Drawing.Point(216, 300);
            this.RightNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.RightNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.RightNumericParameter.Name = "RightNumericParameter";
            this.RightNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.RightNumericParameter.TabIndex = 137;
            this.RightNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.RightNumericParameter.Visible = false;
            // 
            // RightLabel
            // 
            this.RightLabel.AutoSize = true;
            this.RightLabel.Location = new System.Drawing.Point(121, 302);
            this.RightLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.RightLabel.Name = "RightLabel";
            this.RightLabel.Size = new System.Drawing.Size(78, 17);
            this.RightLabel.TabIndex = 136;
            this.RightLabel.Text = "Parameter:";
            // 
            // RightParameterText
            // 
            this.RightParameterText.Location = new System.Drawing.Point(216, 300);
            this.RightParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.RightParameterText.Name = "RightParameterText";
            this.RightParameterText.ReadOnly = true;
            this.RightParameterText.Size = new System.Drawing.Size(311, 22);
            this.RightParameterText.TabIndex = 135;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(88, 274);
            this.label15.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(109, 17);
            this.label15.TabIndex = 134;
            this.label15.Text = "Action for Right:";
            // 
            // RightFunctionMenu
            // 
            this.RightFunctionMenu.FormattingEnabled = true;
            this.RightFunctionMenu.Location = new System.Drawing.Point(216, 270);
            this.RightFunctionMenu.Margin = new System.Windows.Forms.Padding(4);
            this.RightFunctionMenu.Name = "RightFunctionMenu";
            this.RightFunctionMenu.Size = new System.Drawing.Size(311, 24);
            this.RightFunctionMenu.TabIndex = 133;
            this.RightFunctionMenu.SelectedIndexChanged += new System.EventHandler(this.RightFunctionMenu_SelectedIndexChanged);
            // 
            // LeftComboBox
            // 
            this.LeftComboBox.FormattingEnabled = true;
            this.LeftComboBox.Location = new System.Drawing.Point(533, 204);
            this.LeftComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.LeftComboBox.Name = "LeftComboBox";
            this.LeftComboBox.Size = new System.Drawing.Size(161, 24);
            this.LeftComboBox.TabIndex = 132;
            this.LeftComboBox.Visible = false;
            this.LeftComboBox.SelectedIndexChanged += new System.EventHandler(this.LeftComboBox_SelectedIndexChanged);
            // 
            // DownComboBox
            // 
            this.DownComboBox.FormattingEnabled = true;
            this.DownComboBox.Location = new System.Drawing.Point(533, 134);
            this.DownComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.DownComboBox.Name = "DownComboBox";
            this.DownComboBox.Size = new System.Drawing.Size(161, 24);
            this.DownComboBox.TabIndex = 131;
            this.DownComboBox.Visible = false;
            this.DownComboBox.SelectedIndexChanged += new System.EventHandler(this.DownComboBox_SelectedIndexChanged);
            // 
            // UpComboBox
            // 
            this.UpComboBox.FormattingEnabled = true;
            this.UpComboBox.Location = new System.Drawing.Point(533, 66);
            this.UpComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.UpComboBox.Name = "UpComboBox";
            this.UpComboBox.Size = new System.Drawing.Size(161, 24);
            this.UpComboBox.TabIndex = 130;
            this.UpComboBox.Visible = false;
            this.UpComboBox.SelectedIndexChanged += new System.EventHandler(this.UpComboBox_SelectedIndexChanged);
            // 
            // LeftNumericParameter
            // 
            this.LeftNumericParameter.Location = new System.Drawing.Point(216, 234);
            this.LeftNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.LeftNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.LeftNumericParameter.Name = "LeftNumericParameter";
            this.LeftNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.LeftNumericParameter.TabIndex = 129;
            this.LeftNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.LeftNumericParameter.Visible = false;
            // 
            // DownNumericParameter
            // 
            this.DownNumericParameter.Location = new System.Drawing.Point(216, 164);
            this.DownNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.DownNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.DownNumericParameter.Name = "DownNumericParameter";
            this.DownNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.DownNumericParameter.TabIndex = 128;
            this.DownNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.DownNumericParameter.Visible = false;
            // 
            // UpNumericParameter
            // 
            this.UpNumericParameter.Location = new System.Drawing.Point(216, 94);
            this.UpNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.UpNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.UpNumericParameter.Name = "UpNumericParameter";
            this.UpNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.UpNumericParameter.TabIndex = 127;
            this.UpNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.UpNumericParameter.Visible = false;
            // 
            // LeftLabel
            // 
            this.LeftLabel.AutoSize = true;
            this.LeftLabel.Location = new System.Drawing.Point(121, 235);
            this.LeftLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.LeftLabel.Name = "LeftLabel";
            this.LeftLabel.Size = new System.Drawing.Size(78, 17);
            this.LeftLabel.TabIndex = 126;
            this.LeftLabel.Text = "Parameter:";
            // 
            // LeftParameterText
            // 
            this.LeftParameterText.Location = new System.Drawing.Point(216, 234);
            this.LeftParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.LeftParameterText.Name = "LeftParameterText";
            this.LeftParameterText.ReadOnly = true;
            this.LeftParameterText.Size = new System.Drawing.Size(311, 22);
            this.LeftParameterText.TabIndex = 125;
            // 
            // DownLabel
            // 
            this.DownLabel.AutoSize = true;
            this.DownLabel.Location = new System.Drawing.Point(121, 165);
            this.DownLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.DownLabel.Name = "DownLabel";
            this.DownLabel.Size = new System.Drawing.Size(78, 17);
            this.DownLabel.TabIndex = 124;
            this.DownLabel.Text = "Parameter:";
            // 
            // DownParameterText
            // 
            this.DownParameterText.Location = new System.Drawing.Point(216, 164);
            this.DownParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.DownParameterText.Name = "DownParameterText";
            this.DownParameterText.ReadOnly = true;
            this.DownParameterText.Size = new System.Drawing.Size(311, 22);
            this.DownParameterText.TabIndex = 123;
            // 
            // UpLabel
            // 
            this.UpLabel.AutoSize = true;
            this.UpLabel.Location = new System.Drawing.Point(121, 98);
            this.UpLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.UpLabel.Name = "UpLabel";
            this.UpLabel.Size = new System.Drawing.Size(78, 17);
            this.UpLabel.TabIndex = 122;
            this.UpLabel.Text = "Parameter:";
            this.UpLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // UpParameterText
            // 
            this.UpParameterText.Location = new System.Drawing.Point(216, 94);
            this.UpParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.UpParameterText.Name = "UpParameterText";
            this.UpParameterText.ReadOnly = true;
            this.UpParameterText.Size = new System.Drawing.Size(311, 22);
            this.UpParameterText.TabIndex = 121;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(98, 206);
            this.label11.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(100, 17);
            this.label11.TabIndex = 120;
            this.label11.Text = "Action for Left:";
            // 
            // LeftFunctionMenu
            // 
            this.LeftFunctionMenu.FormattingEnabled = true;
            this.LeftFunctionMenu.Location = new System.Drawing.Point(216, 204);
            this.LeftFunctionMenu.Margin = new System.Windows.Forms.Padding(4);
            this.LeftFunctionMenu.Name = "LeftFunctionMenu";
            this.LeftFunctionMenu.Size = new System.Drawing.Size(311, 24);
            this.LeftFunctionMenu.TabIndex = 119;
            this.LeftFunctionMenu.SelectedIndexChanged += new System.EventHandler(this.LeftFunctionMenu_SelectedIndexChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(85, 137);
            this.label12.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(111, 17);
            this.label12.TabIndex = 118;
            this.label12.Text = "Action for Down:";
            // 
            // DownFunctionMenu
            // 
            this.DownFunctionMenu.FormattingEnabled = true;
            this.DownFunctionMenu.Location = new System.Drawing.Point(216, 134);
            this.DownFunctionMenu.Margin = new System.Windows.Forms.Padding(4);
            this.DownFunctionMenu.Name = "DownFunctionMenu";
            this.DownFunctionMenu.Size = new System.Drawing.Size(311, 24);
            this.DownFunctionMenu.TabIndex = 117;
            this.DownFunctionMenu.SelectedIndexChanged += new System.EventHandler(this.DownFunctionMenu_SelectedIndexChanged);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(104, 69);
            this.label13.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(94, 17);
            this.label13.TabIndex = 116;
            this.label13.Text = "Action for Up:";
            // 
            // UpFunctionMenu
            // 
            this.UpFunctionMenu.FormattingEnabled = true;
            this.UpFunctionMenu.Location = new System.Drawing.Point(216, 66);
            this.UpFunctionMenu.Margin = new System.Windows.Forms.Padding(4);
            this.UpFunctionMenu.Name = "UpFunctionMenu";
            this.UpFunctionMenu.Size = new System.Drawing.Size(311, 24);
            this.UpFunctionMenu.TabIndex = 115;
            this.UpFunctionMenu.SelectedIndexChanged += new System.EventHandler(this.UpFunctionMenu_SelectedIndexChanged);
            // 
            // SipPuffLevelsTab
            // 
            this.SipPuffLevelsTab.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.SipPuffLevelsTab.Controls.Add(this.holdLevelIndicatorPanel);
            this.SipPuffLevelsTab.Controls.Add(this.sipLevelIndicatorPanel);
            this.SipPuffLevelsTab.Controls.Add(this.specialLevelIndicatorPanel);
            this.SipPuffLevelsTab.Controls.Add(this.puffLevelIndicatorPanel);
            this.SipPuffLevelsTab.Controls.Add(this.label30);
            this.SipPuffLevelsTab.Controls.Add(this.label29);
            this.SipPuffLevelsTab.Controls.Add(this.decHoldThreshold);
            this.SipPuffLevelsTab.Controls.Add(this.incHoldThreshold);
            this.SipPuffLevelsTab.Controls.Add(this.stongSipThresholdLabel);
            this.SipPuffLevelsTab.Controls.Add(this.label2);
            this.SipPuffLevelsTab.Controls.Add(this.strongSipThresholdBar);
            this.SipPuffLevelsTab.Controls.Add(this.incTimeThreshld);
            this.SipPuffLevelsTab.Controls.Add(this.decTimeThreshold);
            this.SipPuffLevelsTab.Controls.Add(this.incSipThreshold);
            this.SipPuffLevelsTab.Controls.Add(this.decSipThreshold);
            this.SipPuffLevelsTab.Controls.Add(this.incPuffThreshold);
            this.SipPuffLevelsTab.Controls.Add(this.decPuffThreshold);
            this.SipPuffLevelsTab.Controls.Add(this.label4);
            this.SipPuffLevelsTab.Controls.Add(this.strongPuffThresholdLabel);
            this.SipPuffLevelsTab.Controls.Add(this.label16);
            this.SipPuffLevelsTab.Controls.Add(this.strongPuffThresholdBar);
            this.SipPuffLevelsTab.Controls.Add(this.pressureDrawPanel);
            this.SipPuffLevelsTab.Controls.Add(this.pressureLabel);
            this.SipPuffLevelsTab.Controls.Add(this.puffThresholdLabel);
            this.SipPuffLevelsTab.Controls.Add(this.puffThresholdBar);
            this.SipPuffLevelsTab.Controls.Add(this.thresholdLabelForPuff);
            this.SipPuffLevelsTab.Controls.Add(this.sipThresholdLabel);
            this.SipPuffLevelsTab.Controls.Add(this.sipThresholdBar);
            this.SipPuffLevelsTab.Controls.Add(this.thresholdLabelForSip);
            this.SipPuffLevelsTab.Location = new System.Drawing.Point(4, 25);
            this.SipPuffLevelsTab.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.SipPuffLevelsTab.Name = "SipPuffLevelsTab";
            this.SipPuffLevelsTab.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.SipPuffLevelsTab.Size = new System.Drawing.Size(780, 361);
            this.SipPuffLevelsTab.TabIndex = 5;
            this.SipPuffLevelsTab.Text = "Sip/Puff Levels";
            // 
            // holdLevelIndicatorPanel
            // 
            this.holdLevelIndicatorPanel.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.holdLevelIndicatorPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.holdLevelIndicatorPanel.Location = new System.Drawing.Point(86, 204);
            this.holdLevelIndicatorPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.holdLevelIndicatorPanel.Name = "holdLevelIndicatorPanel";
            this.holdLevelIndicatorPanel.Size = new System.Drawing.Size(34, 24);
            this.holdLevelIndicatorPanel.TabIndex = 122;
            // 
            // sipLevelIndicatorPanel
            // 
            this.sipLevelIndicatorPanel.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.sipLevelIndicatorPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.sipLevelIndicatorPanel.Location = new System.Drawing.Point(230, 204);
            this.sipLevelIndicatorPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.sipLevelIndicatorPanel.Name = "sipLevelIndicatorPanel";
            this.sipLevelIndicatorPanel.Size = new System.Drawing.Size(34, 24);
            this.sipLevelIndicatorPanel.TabIndex = 122;
            // 
            // specialLevelIndicatorPanel
            // 
            this.specialLevelIndicatorPanel.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.specialLevelIndicatorPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.specialLevelIndicatorPanel.Location = new System.Drawing.Point(667, 82);
            this.specialLevelIndicatorPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.specialLevelIndicatorPanel.Name = "specialLevelIndicatorPanel";
            this.specialLevelIndicatorPanel.Size = new System.Drawing.Size(34, 24);
            this.specialLevelIndicatorPanel.TabIndex = 122;
            // 
            // puffLevelIndicatorPanel
            // 
            this.puffLevelIndicatorPanel.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.puffLevelIndicatorPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.puffLevelIndicatorPanel.Location = new System.Drawing.Point(516, 82);
            this.puffLevelIndicatorPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.puffLevelIndicatorPanel.Name = "puffLevelIndicatorPanel";
            this.puffLevelIndicatorPanel.Size = new System.Drawing.Size(34, 24);
            this.puffLevelIndicatorPanel.TabIndex = 121;
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(56, 59);
            this.label30.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(164, 17);
            this.label30.TabIndex = 141;
            this.label30.Text = "  for sip and puff actions:";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(56, 40);
            this.label29.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(150, 17);
            this.label29.TabIndex = 140;
            this.label29.Text = "Select threshold levels";
            // 
            // decHoldThreshold
            // 
            this.decHoldThreshold.Cursor = System.Windows.Forms.Cursors.Hand;
            this.decHoldThreshold.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decHoldThreshold.ForeColor = System.Drawing.Color.Black;
            this.decHoldThreshold.Location = new System.Drawing.Point(83, 285);
            this.decHoldThreshold.Margin = new System.Windows.Forms.Padding(4);
            this.decHoldThreshold.Name = "decHoldThreshold";
            this.decHoldThreshold.Size = new System.Drawing.Size(41, 37);
            this.decHoldThreshold.TabIndex = 135;
            this.decHoldThreshold.Text = "-";
            this.decHoldThreshold.UseVisualStyleBackColor = true;
            this.decHoldThreshold.Click += new System.EventHandler(this.decStrongSipThreshold_Click);
            this.decHoldThreshold.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decHoldThreshold.MouseHover += new System.EventHandler(this.decStrongSipThreshold_MouseHover);
            // 
            // incHoldThreshold
            // 
            this.incHoldThreshold.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incHoldThreshold.ForeColor = System.Drawing.Color.Black;
            this.incHoldThreshold.Location = new System.Drawing.Point(83, 240);
            this.incHoldThreshold.Margin = new System.Windows.Forms.Padding(4);
            this.incHoldThreshold.Name = "incHoldThreshold";
            this.incHoldThreshold.Size = new System.Drawing.Size(41, 37);
            this.incHoldThreshold.TabIndex = 134;
            this.incHoldThreshold.Text = "+";
            this.incHoldThreshold.UseVisualStyleBackColor = true;
            this.incHoldThreshold.Click += new System.EventHandler(this.incStrongSipThreshold_Click);
            this.incHoldThreshold.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incHoldThreshold.MouseHover += new System.EventHandler(this.incStrongSipThreshold_MouseHover);
            // 
            // stongSipThresholdLabel
            // 
            this.stongSipThresholdLabel.AutoSize = true;
            this.stongSipThresholdLabel.Location = new System.Drawing.Point(121, 170);
            this.stongSipThresholdLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.stongSipThresholdLabel.Name = "stongSipThresholdLabel";
            this.stongSipThresholdLabel.Size = new System.Drawing.Size(32, 17);
            this.stongSipThresholdLabel.TabIndex = 133;
            this.stongSipThresholdLabel.Text = "300";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(82, 151);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(108, 17);
            this.label2.TabIndex = 132;
            this.label2.Text = "StrongSip Level";
            // 
            // strongSipThresholdBar
            // 
            this.strongSipThresholdBar.LargeChange = 20;
            this.strongSipThresholdBar.Location = new System.Drawing.Point(140, 191);
            this.strongSipThresholdBar.Margin = new System.Windows.Forms.Padding(4);
            this.strongSipThresholdBar.Maximum = 512;
            this.strongSipThresholdBar.Name = "strongSipThresholdBar";
            this.strongSipThresholdBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.strongSipThresholdBar.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.strongSipThresholdBar.Size = new System.Drawing.Size(56, 141);
            this.strongSipThresholdBar.TabIndex = 131;
            this.strongSipThresholdBar.TickFrequency = 40;
            this.strongSipThresholdBar.Value = 300;
            this.strongSipThresholdBar.Scroll += new System.EventHandler(this.strongSipThresholdBar_Scroll);
            // 
            // incTimeThreshld
            // 
            this.incTimeThreshld.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incTimeThreshld.ForeColor = System.Drawing.Color.Black;
            this.incTimeThreshld.Location = new System.Drawing.Point(663, 115);
            this.incTimeThreshld.Margin = new System.Windows.Forms.Padding(4);
            this.incTimeThreshld.Name = "incTimeThreshld";
            this.incTimeThreshld.Size = new System.Drawing.Size(41, 37);
            this.incTimeThreshld.TabIndex = 130;
            this.incTimeThreshld.Text = "+";
            this.incTimeThreshld.UseVisualStyleBackColor = true;
            this.incTimeThreshld.Click += new System.EventHandler(this.incStrongPuffThreshold_Click);
            this.incTimeThreshld.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incTimeThreshld.MouseHover += new System.EventHandler(this.incSpecialThreshold_MouseHover);
            // 
            // decTimeThreshold
            // 
            this.decTimeThreshold.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decTimeThreshold.ForeColor = System.Drawing.Color.Black;
            this.decTimeThreshold.Location = new System.Drawing.Point(663, 161);
            this.decTimeThreshold.Margin = new System.Windows.Forms.Padding(4);
            this.decTimeThreshold.Name = "decTimeThreshold";
            this.decTimeThreshold.Size = new System.Drawing.Size(41, 37);
            this.decTimeThreshold.TabIndex = 129;
            this.decTimeThreshold.Text = "-";
            this.decTimeThreshold.UseVisualStyleBackColor = true;
            this.decTimeThreshold.Click += new System.EventHandler(this.decStrongPuffThreshold_Click);
            this.decTimeThreshold.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decTimeThreshold.MouseHover += new System.EventHandler(this.decStrongPuffThreshold_MouseHover);
            // 
            // incSipThreshold
            // 
            this.incSipThreshold.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incSipThreshold.ForeColor = System.Drawing.Color.Black;
            this.incSipThreshold.Location = new System.Drawing.Point(227, 238);
            this.incSipThreshold.Margin = new System.Windows.Forms.Padding(4);
            this.incSipThreshold.Name = "incSipThreshold";
            this.incSipThreshold.Size = new System.Drawing.Size(41, 37);
            this.incSipThreshold.TabIndex = 128;
            this.incSipThreshold.Text = "+";
            this.incSipThreshold.UseVisualStyleBackColor = true;
            this.incSipThreshold.Click += new System.EventHandler(this.incSipThreshold_Click);
            this.incSipThreshold.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incSipThreshold.MouseHover += new System.EventHandler(this.incSipThreshold_MouseHover);
            // 
            // decSipThreshold
            // 
            this.decSipThreshold.Cursor = System.Windows.Forms.Cursors.Hand;
            this.decSipThreshold.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decSipThreshold.ForeColor = System.Drawing.Color.Black;
            this.decSipThreshold.Location = new System.Drawing.Point(227, 283);
            this.decSipThreshold.Margin = new System.Windows.Forms.Padding(4);
            this.decSipThreshold.Name = "decSipThreshold";
            this.decSipThreshold.Size = new System.Drawing.Size(41, 37);
            this.decSipThreshold.TabIndex = 127;
            this.decSipThreshold.Text = "-";
            this.decSipThreshold.UseVisualStyleBackColor = true;
            this.decSipThreshold.Click += new System.EventHandler(this.decSipThreshold_Click);
            this.decSipThreshold.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decSipThreshold.MouseHover += new System.EventHandler(this.decSipThreshold_MouseHover);
            // 
            // incPuffThreshold
            // 
            this.incPuffThreshold.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incPuffThreshold.ForeColor = System.Drawing.Color.Black;
            this.incPuffThreshold.Location = new System.Drawing.Point(512, 115);
            this.incPuffThreshold.Margin = new System.Windows.Forms.Padding(4);
            this.incPuffThreshold.Name = "incPuffThreshold";
            this.incPuffThreshold.Size = new System.Drawing.Size(41, 37);
            this.incPuffThreshold.TabIndex = 126;
            this.incPuffThreshold.Text = "+";
            this.incPuffThreshold.UseVisualStyleBackColor = true;
            this.incPuffThreshold.Click += new System.EventHandler(this.incPuffThreshold_Click);
            this.incPuffThreshold.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incPuffThreshold.MouseHover += new System.EventHandler(this.incPuffThreshold_MouseHover);
            // 
            // decPuffThreshold
            // 
            this.decPuffThreshold.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decPuffThreshold.ForeColor = System.Drawing.Color.Black;
            this.decPuffThreshold.Location = new System.Drawing.Point(512, 161);
            this.decPuffThreshold.Margin = new System.Windows.Forms.Padding(4);
            this.decPuffThreshold.Name = "decPuffThreshold";
            this.decPuffThreshold.Size = new System.Drawing.Size(41, 37);
            this.decPuffThreshold.TabIndex = 125;
            this.decPuffThreshold.Text = "-";
            this.decPuffThreshold.UseVisualStyleBackColor = true;
            this.decPuffThreshold.Click += new System.EventHandler(this.decPuffThreshold_Click);
            this.decPuffThreshold.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decPuffThreshold.MouseHover += new System.EventHandler(this.decPuffThreshold_MouseHover);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(357, 32);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 17);
            this.label4.TabIndex = 124;
            this.label4.Text = "Pressure";
            // 
            // strongPuffThresholdLabel
            // 
            this.strongPuffThresholdLabel.AutoSize = true;
            this.strongPuffThresholdLabel.Location = new System.Drawing.Point(632, 240);
            this.strongPuffThresholdLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.strongPuffThresholdLabel.Name = "strongPuffThresholdLabel";
            this.strongPuffThresholdLabel.Size = new System.Drawing.Size(32, 17);
            this.strongPuffThresholdLabel.TabIndex = 123;
            this.strongPuffThresholdLabel.Text = "700";
            this.strongPuffThresholdLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(600, 219);
            this.label16.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(113, 17);
            this.label16.TabIndex = 122;
            this.label16.Text = "StrongPuff Level";
            // 
            // strongPuffThresholdBar
            // 
            this.strongPuffThresholdBar.LargeChange = 10;
            this.strongPuffThresholdBar.Location = new System.Drawing.Point(580, 72);
            this.strongPuffThresholdBar.Margin = new System.Windows.Forms.Padding(4);
            this.strongPuffThresholdBar.Maximum = 1023;
            this.strongPuffThresholdBar.Minimum = 512;
            this.strongPuffThresholdBar.Name = "strongPuffThresholdBar";
            this.strongPuffThresholdBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.strongPuffThresholdBar.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.strongPuffThresholdBar.RightToLeftLayout = true;
            this.strongPuffThresholdBar.Size = new System.Drawing.Size(56, 141);
            this.strongPuffThresholdBar.TabIndex = 121;
            this.strongPuffThresholdBar.TickFrequency = 40;
            this.strongPuffThresholdBar.Value = 700;
            this.strongPuffThresholdBar.Scroll += new System.EventHandler(this.strongPuffThresholdBar_Scroll);
            // 
            // pressureDrawPanel
            // 
            this.pressureDrawPanel.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.pressureDrawPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pressureDrawPanel.Location = new System.Drawing.Point(348, 72);
            this.pressureDrawPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.pressureDrawPanel.Name = "pressureDrawPanel";
            this.pressureDrawPanel.Size = new System.Drawing.Size(86, 260);
            this.pressureDrawPanel.TabIndex = 120;
            // 
            // pressureLabel
            // 
            this.pressureLabel.AutoSize = true;
            this.pressureLabel.Location = new System.Drawing.Point(368, 50);
            this.pressureLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.pressureLabel.Name = "pressureLabel";
            this.pressureLabel.Size = new System.Drawing.Size(16, 17);
            this.pressureLabel.TabIndex = 119;
            this.pressureLabel.Text = "0";
            this.pressureLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // puffThresholdLabel
            // 
            this.puffThresholdLabel.AutoSize = true;
            this.puffThresholdLabel.Location = new System.Drawing.Point(493, 242);
            this.puffThresholdLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.puffThresholdLabel.Name = "puffThresholdLabel";
            this.puffThresholdLabel.Size = new System.Drawing.Size(32, 17);
            this.puffThresholdLabel.TabIndex = 118;
            this.puffThresholdLabel.Text = "525";
            // 
            // puffThresholdBar
            // 
            this.puffThresholdBar.AllowDrop = true;
            this.puffThresholdBar.LargeChange = 20;
            this.puffThresholdBar.Location = new System.Drawing.Point(435, 70);
            this.puffThresholdBar.Margin = new System.Windows.Forms.Padding(4);
            this.puffThresholdBar.Maximum = 1023;
            this.puffThresholdBar.Minimum = 512;
            this.puffThresholdBar.Name = "puffThresholdBar";
            this.puffThresholdBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.puffThresholdBar.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.puffThresholdBar.RightToLeftLayout = true;
            this.puffThresholdBar.Size = new System.Drawing.Size(56, 143);
            this.puffThresholdBar.TabIndex = 117;
            this.puffThresholdBar.TickFrequency = 40;
            this.puffThresholdBar.Value = 525;
            this.puffThresholdBar.Scroll += new System.EventHandler(this.puffThresholdBar_Scroll);
            // 
            // thresholdLabelForPuff
            // 
            this.thresholdLabelForPuff.AutoSize = true;
            this.thresholdLabelForPuff.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.thresholdLabelForPuff.Location = new System.Drawing.Point(480, 221);
            this.thresholdLabelForPuff.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.thresholdLabelForPuff.Name = "thresholdLabelForPuff";
            this.thresholdLabelForPuff.Size = new System.Drawing.Size(71, 17);
            this.thresholdLabelForPuff.TabIndex = 116;
            this.thresholdLabelForPuff.Text = "Puff Level";
            this.thresholdLabelForPuff.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // sipThresholdLabel
            // 
            this.sipThresholdLabel.AutoSize = true;
            this.sipThresholdLabel.Location = new System.Drawing.Point(261, 170);
            this.sipThresholdLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.sipThresholdLabel.Name = "sipThresholdLabel";
            this.sipThresholdLabel.Size = new System.Drawing.Size(32, 17);
            this.sipThresholdLabel.TabIndex = 115;
            this.sipThresholdLabel.Text = "500";
            // 
            // sipThresholdBar
            // 
            this.sipThresholdBar.LargeChange = 20;
            this.sipThresholdBar.Location = new System.Drawing.Point(284, 191);
            this.sipThresholdBar.Margin = new System.Windows.Forms.Padding(4);
            this.sipThresholdBar.Maximum = 512;
            this.sipThresholdBar.Name = "sipThresholdBar";
            this.sipThresholdBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.sipThresholdBar.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.sipThresholdBar.Size = new System.Drawing.Size(56, 141);
            this.sipThresholdBar.TabIndex = 114;
            this.sipThresholdBar.TickFrequency = 40;
            this.sipThresholdBar.Value = 500;
            this.sipThresholdBar.Scroll += new System.EventHandler(this.sipThresholdBar_Scroll);
            // 
            // thresholdLabelForSip
            // 
            this.thresholdLabelForSip.AutoSize = true;
            this.thresholdLabelForSip.Location = new System.Drawing.Point(243, 151);
            this.thresholdLabelForSip.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.thresholdLabelForSip.Name = "thresholdLabelForSip";
            this.thresholdLabelForSip.Size = new System.Drawing.Size(66, 17);
            this.thresholdLabelForSip.TabIndex = 113;
            this.thresholdLabelForSip.Text = "Sip Level";
            // 
            // SipPuffTab
            // 
            this.SipPuffTab.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.SipPuffTab.Controls.Add(this.sipPuffSelections);
            this.SipPuffTab.Location = new System.Drawing.Point(4, 25);
            this.SipPuffTab.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.SipPuffTab.Name = "SipPuffTab";
            this.SipPuffTab.Size = new System.Drawing.Size(780, 361);
            this.SipPuffTab.TabIndex = 2;
            this.SipPuffTab.Text = "Sip/Puff Actions";
            // 
            // sipPuffSelections
            // 
            this.sipPuffSelections.Alignment = System.Windows.Forms.TabAlignment.Right;
            this.sipPuffSelections.Controls.Add(this.tabPage1);
            this.sipPuffSelections.Controls.Add(this.tabPage2);
            this.sipPuffSelections.Controls.Add(this.tabPage3);
            this.sipPuffSelections.Location = new System.Drawing.Point(25, 14);
            this.sipPuffSelections.Multiline = true;
            this.sipPuffSelections.Name = "sipPuffSelections";
            this.sipPuffSelections.SelectedIndex = 0;
            this.sipPuffSelections.Size = new System.Drawing.Size(731, 331);
            this.sipPuffSelections.TabIndex = 142;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tabPage1.Controls.Add(this.label41);
            this.tabPage1.Controls.Add(this.clearButtonStrongPuff);
            this.tabPage1.Controls.Add(this.clearButtonPuff);
            this.tabPage1.Controls.Add(this.clearButtonStrongSip);
            this.tabPage1.Controls.Add(this.clearButtonSip);
            this.tabPage1.Controls.Add(this.StrongPuffComboBox);
            this.tabPage1.Controls.Add(this.SpecialPuffParameterLabel);
            this.tabPage1.Controls.Add(this.StrongPuffNumericParameter);
            this.tabPage1.Controls.Add(this.label14);
            this.tabPage1.Controls.Add(this.StrongPuffFunctionMenu);
            this.tabPage1.Controls.Add(this.StrongSipComboBox);
            this.tabPage1.Controls.Add(this.SpecialSipParameterLabel);
            this.tabPage1.Controls.Add(this.StrongSipNumericParameter);
            this.tabPage1.Controls.Add(this.label9);
            this.tabPage1.Controls.Add(this.StrongSipFunctionMenu);
            this.tabPage1.Controls.Add(this.PuffComboBox);
            this.tabPage1.Controls.Add(this.SipComboBox);
            this.tabPage1.Controls.Add(this.PuffParameterLabel);
            this.tabPage1.Controls.Add(this.SipParameterLabel);
            this.tabPage1.Controls.Add(this.PuffNumericParameter);
            this.tabPage1.Controls.Add(this.SipNumericParameter);
            this.tabPage1.Controls.Add(this.label20);
            this.tabPage1.Controls.Add(this.PuffFunctionMenu);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Controls.Add(this.SipFunctionMenu);
            this.tabPage1.Controls.Add(this.SipParameterText);
            this.tabPage1.Controls.Add(this.StrongSipParameterText);
            this.tabPage1.Controls.Add(this.PuffParameterText);
            this.tabPage1.Controls.Add(this.StrongPuffParameterText);
            this.tabPage1.Location = new System.Drawing.Point(4, 4);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(702, 323);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Single";
            // 
            // clearButtonStrongPuff
            // 
            this.clearButtonStrongPuff.Enabled = false;
            this.clearButtonStrongPuff.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongPuff.Location = new System.Drawing.Point(501, 281);
            this.clearButtonStrongPuff.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongPuff.Name = "clearButtonStrongPuff";
            this.clearButtonStrongPuff.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongPuff.TabIndex = 144;
            this.clearButtonStrongPuff.Text = "x";
            this.clearButtonStrongPuff.UseVisualStyleBackColor = true;
            this.clearButtonStrongPuff.Visible = false;
            this.clearButtonStrongPuff.Click += new System.EventHandler(this.clearButtonStrongPuff_Click);
            // 
            // clearButtonPuff
            // 
            this.clearButtonPuff.Enabled = false;
            this.clearButtonPuff.ForeColor = System.Drawing.Color.Black;
            this.clearButtonPuff.Location = new System.Drawing.Point(501, 214);
            this.clearButtonPuff.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonPuff.Name = "clearButtonPuff";
            this.clearButtonPuff.Size = new System.Drawing.Size(36, 28);
            this.clearButtonPuff.TabIndex = 143;
            this.clearButtonPuff.Text = "x";
            this.clearButtonPuff.UseVisualStyleBackColor = true;
            this.clearButtonPuff.Visible = false;
            this.clearButtonPuff.Click += new System.EventHandler(this.clearButtonPuff_Click);
            // 
            // clearButtonStrongSip
            // 
            this.clearButtonStrongSip.Enabled = false;
            this.clearButtonStrongSip.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongSip.Location = new System.Drawing.Point(501, 140);
            this.clearButtonStrongSip.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongSip.Name = "clearButtonStrongSip";
            this.clearButtonStrongSip.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongSip.TabIndex = 142;
            this.clearButtonStrongSip.Text = "x";
            this.clearButtonStrongSip.UseVisualStyleBackColor = true;
            this.clearButtonStrongSip.Visible = false;
            this.clearButtonStrongSip.Click += new System.EventHandler(this.clearButtonStrongSip_Click);
            // 
            // clearButtonSip
            // 
            this.clearButtonSip.Enabled = false;
            this.clearButtonSip.ForeColor = System.Drawing.Color.Black;
            this.clearButtonSip.Location = new System.Drawing.Point(501, 71);
            this.clearButtonSip.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonSip.Name = "clearButtonSip";
            this.clearButtonSip.Size = new System.Drawing.Size(36, 28);
            this.clearButtonSip.TabIndex = 141;
            this.clearButtonSip.Text = "x";
            this.clearButtonSip.UseVisualStyleBackColor = true;
            this.clearButtonSip.Visible = false;
            this.clearButtonSip.Click += new System.EventHandler(this.clearButtonSip_Click);
            // 
            // StrongPuffComboBox
            // 
            this.StrongPuffComboBox.FormattingEnabled = true;
            this.StrongPuffComboBox.Location = new System.Drawing.Point(501, 253);
            this.StrongPuffComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffComboBox.Name = "StrongPuffComboBox";
            this.StrongPuffComboBox.Size = new System.Drawing.Size(136, 24);
            this.StrongPuffComboBox.TabIndex = 140;
            this.StrongPuffComboBox.Visible = false;
            this.StrongPuffComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongPuffComboBox_SelectedIndexChanged);
            // 
            // SpecialPuffParameterLabel
            // 
            this.SpecialPuffParameterLabel.AutoSize = true;
            this.SpecialPuffParameterLabel.Location = new System.Drawing.Point(79, 288);
            this.SpecialPuffParameterLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.SpecialPuffParameterLabel.Name = "SpecialPuffParameterLabel";
            this.SpecialPuffParameterLabel.Size = new System.Drawing.Size(74, 17);
            this.SpecialPuffParameterLabel.TabIndex = 138;
            this.SpecialPuffParameterLabel.Text = "Parameter";
            // 
            // StrongPuffNumericParameter
            // 
            this.StrongPuffNumericParameter.Location = new System.Drawing.Point(170, 285);
            this.StrongPuffNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongPuffNumericParameter.Name = "StrongPuffNumericParameter";
            this.StrongPuffNumericParameter.Size = new System.Drawing.Size(57, 22);
            this.StrongPuffNumericParameter.TabIndex = 137;
            this.StrongPuffNumericParameter.Visible = false;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(78, 257);
            this.label14.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(75, 17);
            this.label14.TabIndex = 136;
            this.label14.Text = "StrongPuff";
            // 
            // StrongPuffFunctionMenu
            // 
            this.StrongPuffFunctionMenu.FormattingEnabled = true;
            this.StrongPuffFunctionMenu.Location = new System.Drawing.Point(168, 253);
            this.StrongPuffFunctionMenu.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffFunctionMenu.Name = "StrongPuffFunctionMenu";
            this.StrongPuffFunctionMenu.Size = new System.Drawing.Size(312, 24);
            this.StrongPuffFunctionMenu.TabIndex = 135;
            this.StrongPuffFunctionMenu.SelectedIndexChanged += new System.EventHandler(this.StrongPuffFunctionMenu_SelectedIndexChanged);
            // 
            // StrongSipComboBox
            // 
            this.StrongSipComboBox.FormattingEnabled = true;
            this.StrongSipComboBox.Location = new System.Drawing.Point(501, 111);
            this.StrongSipComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipComboBox.Name = "StrongSipComboBox";
            this.StrongSipComboBox.Size = new System.Drawing.Size(136, 24);
            this.StrongSipComboBox.TabIndex = 134;
            this.StrongSipComboBox.Visible = false;
            this.StrongSipComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongSipComboBox_SelectedIndexChanged);
            // 
            // SpecialSipParameterLabel
            // 
            this.SpecialSipParameterLabel.AutoSize = true;
            this.SpecialSipParameterLabel.Location = new System.Drawing.Point(79, 146);
            this.SpecialSipParameterLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.SpecialSipParameterLabel.Name = "SpecialSipParameterLabel";
            this.SpecialSipParameterLabel.Size = new System.Drawing.Size(74, 17);
            this.SpecialSipParameterLabel.TabIndex = 132;
            this.SpecialSipParameterLabel.Text = "Parameter";
            // 
            // StrongSipNumericParameter
            // 
            this.StrongSipNumericParameter.Location = new System.Drawing.Point(172, 144);
            this.StrongSipNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongSipNumericParameter.Name = "StrongSipNumericParameter";
            this.StrongSipNumericParameter.Size = new System.Drawing.Size(57, 22);
            this.StrongSipNumericParameter.TabIndex = 131;
            this.StrongSipNumericParameter.Visible = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(84, 113);
            this.label9.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(70, 17);
            this.label9.TabIndex = 130;
            this.label9.Text = "StrongSip";
            // 
            // StrongSipFunctionMenu
            // 
            this.StrongSipFunctionMenu.FormattingEnabled = true;
            this.StrongSipFunctionMenu.Location = new System.Drawing.Point(170, 111);
            this.StrongSipFunctionMenu.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipFunctionMenu.Name = "StrongSipFunctionMenu";
            this.StrongSipFunctionMenu.Size = new System.Drawing.Size(310, 24);
            this.StrongSipFunctionMenu.TabIndex = 129;
            this.StrongSipFunctionMenu.SelectedIndexChanged += new System.EventHandler(this.StrongSipFunctionMenu_SelectedIndexChanged);
            // 
            // PuffComboBox
            // 
            this.PuffComboBox.FormattingEnabled = true;
            this.PuffComboBox.Location = new System.Drawing.Point(501, 185);
            this.PuffComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.PuffComboBox.Name = "PuffComboBox";
            this.PuffComboBox.Size = new System.Drawing.Size(136, 24);
            this.PuffComboBox.TabIndex = 128;
            this.PuffComboBox.Visible = false;
            this.PuffComboBox.SelectedIndexChanged += new System.EventHandler(this.PuffComboBox_SelectedIndexChanged);
            // 
            // SipComboBox
            // 
            this.SipComboBox.FormattingEnabled = true;
            this.SipComboBox.Location = new System.Drawing.Point(501, 42);
            this.SipComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.SipComboBox.Name = "SipComboBox";
            this.SipComboBox.Size = new System.Drawing.Size(136, 24);
            this.SipComboBox.TabIndex = 126;
            this.SipComboBox.Visible = false;
            this.SipComboBox.SelectedIndexChanged += new System.EventHandler(this.SipComboBox_SelectedIndexChanged);
            // 
            // PuffParameterLabel
            // 
            this.PuffParameterLabel.AutoSize = true;
            this.PuffParameterLabel.Location = new System.Drawing.Point(79, 220);
            this.PuffParameterLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.PuffParameterLabel.Name = "PuffParameterLabel";
            this.PuffParameterLabel.Size = new System.Drawing.Size(74, 17);
            this.PuffParameterLabel.TabIndex = 124;
            this.PuffParameterLabel.Text = "Parameter";
            // 
            // SipParameterLabel
            // 
            this.SipParameterLabel.AutoSize = true;
            this.SipParameterLabel.Location = new System.Drawing.Point(79, 78);
            this.SipParameterLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.SipParameterLabel.Name = "SipParameterLabel";
            this.SipParameterLabel.Size = new System.Drawing.Size(74, 17);
            this.SipParameterLabel.TabIndex = 123;
            this.SipParameterLabel.Text = "Parameter";
            // 
            // PuffNumericParameter
            // 
            this.PuffNumericParameter.Location = new System.Drawing.Point(170, 217);
            this.PuffNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.PuffNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.PuffNumericParameter.Name = "PuffNumericParameter";
            this.PuffNumericParameter.Size = new System.Drawing.Size(57, 22);
            this.PuffNumericParameter.TabIndex = 122;
            this.PuffNumericParameter.Visible = false;
            // 
            // SipNumericParameter
            // 
            this.SipNumericParameter.Location = new System.Drawing.Point(172, 75);
            this.SipNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.SipNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.SipNumericParameter.Name = "SipNumericParameter";
            this.SipNumericParameter.Size = new System.Drawing.Size(57, 22);
            this.SipNumericParameter.TabIndex = 121;
            this.SipNumericParameter.Visible = false;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(61, 191);
            this.label20.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(91, 17);
            this.label20.TabIndex = 120;
            this.label20.Text = "Puff Function";
            // 
            // PuffFunctionMenu
            // 
            this.PuffFunctionMenu.FormattingEnabled = true;
            this.PuffFunctionMenu.Location = new System.Drawing.Point(168, 185);
            this.PuffFunctionMenu.Margin = new System.Windows.Forms.Padding(4);
            this.PuffFunctionMenu.Name = "PuffFunctionMenu";
            this.PuffFunctionMenu.Size = new System.Drawing.Size(312, 24);
            this.PuffFunctionMenu.TabIndex = 119;
            this.PuffFunctionMenu.SelectedIndexChanged += new System.EventHandler(this.PuffFunctionMenu_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(67, 45);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(86, 17);
            this.label5.TabIndex = 118;
            this.label5.Text = "Sip Function";
            // 
            // SipFunctionMenu
            // 
            this.SipFunctionMenu.FormattingEnabled = true;
            this.SipFunctionMenu.Location = new System.Drawing.Point(168, 43);
            this.SipFunctionMenu.Margin = new System.Windows.Forms.Padding(4);
            this.SipFunctionMenu.Name = "SipFunctionMenu";
            this.SipFunctionMenu.Size = new System.Drawing.Size(313, 24);
            this.SipFunctionMenu.TabIndex = 117;
            this.SipFunctionMenu.SelectedIndexChanged += new System.EventHandler(this.SipFunctionMenu_SelectedIndexChanged);
            // 
            // SipParameterText
            // 
            this.SipParameterText.Location = new System.Drawing.Point(170, 75);
            this.SipParameterText.Margin = new System.Windows.Forms.Padding(4);
            this.SipParameterText.Name = "SipParameterText";
            this.SipParameterText.ReadOnly = true;
            this.SipParameterText.Size = new System.Drawing.Size(312, 22);
            this.SipParameterText.TabIndex = 125;
            this.SipParameterText.Visible = false;
            // 
            // StrongSipParameterText
            // 
            this.StrongSipParameterText.Location = new System.Drawing.Point(170, 143);
            this.StrongSipParameterText.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipParameterText.Name = "StrongSipParameterText";
            this.StrongSipParameterText.ReadOnly = true;
            this.StrongSipParameterText.Size = new System.Drawing.Size(312, 22);
            this.StrongSipParameterText.TabIndex = 133;
            this.StrongSipParameterText.Visible = false;
            // 
            // PuffParameterText
            // 
            this.PuffParameterText.Location = new System.Drawing.Point(168, 217);
            this.PuffParameterText.Margin = new System.Windows.Forms.Padding(4);
            this.PuffParameterText.Name = "PuffParameterText";
            this.PuffParameterText.ReadOnly = true;
            this.PuffParameterText.Size = new System.Drawing.Size(313, 22);
            this.PuffParameterText.TabIndex = 127;
            this.PuffParameterText.Visible = false;
            // 
            // StrongPuffParameterText
            // 
            this.StrongPuffParameterText.Location = new System.Drawing.Point(168, 285);
            this.StrongPuffParameterText.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffParameterText.Name = "StrongPuffParameterText";
            this.StrongPuffParameterText.ReadOnly = true;
            this.StrongPuffParameterText.Size = new System.Drawing.Size(313, 22);
            this.StrongPuffParameterText.TabIndex = 139;
            this.StrongPuffParameterText.Visible = false;
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tabPage2.Controls.Add(this.label35);
            this.tabPage2.Controls.Add(this.clearButtonStrongSipRight);
            this.tabPage2.Controls.Add(this.StrongSipRightComboBox);
            this.tabPage2.Controls.Add(this.StrongSipRightNumericParameter);
            this.tabPage2.Controls.Add(this.StrongSipRightLabel);
            this.tabPage2.Controls.Add(this.StrongSipRightParameterText);
            this.tabPage2.Controls.Add(this.label48);
            this.tabPage2.Controls.Add(this.StrongSipRightFunctionBox);
            this.tabPage2.Controls.Add(this.clearButtonStrongSipLeft);
            this.tabPage2.Controls.Add(this.clearButtonStrongSipDown);
            this.tabPage2.Controls.Add(this.clearButtonStrongSipUp);
            this.tabPage2.Controls.Add(this.StrongSipLeftComboBox);
            this.tabPage2.Controls.Add(this.StrongSipDownComboBox);
            this.tabPage2.Controls.Add(this.StrongSipUpComboBox);
            this.tabPage2.Controls.Add(this.StrongSipLeftNumericParameter);
            this.tabPage2.Controls.Add(this.StrongSipDownNumericParameter);
            this.tabPage2.Controls.Add(this.StrongSipUpNumericParameter);
            this.tabPage2.Controls.Add(this.StrongSipLeftLabel);
            this.tabPage2.Controls.Add(this.StrongSipLeftParameterText);
            this.tabPage2.Controls.Add(this.StrongSipDownLabel);
            this.tabPage2.Controls.Add(this.StrongSipDownParameterText);
            this.tabPage2.Controls.Add(this.StrongSipUpLabel);
            this.tabPage2.Controls.Add(this.StrongSipUpParameterText);
            this.tabPage2.Controls.Add(this.label52);
            this.tabPage2.Controls.Add(this.StrongSipLeftFunctionBox);
            this.tabPage2.Controls.Add(this.label53);
            this.tabPage2.Controls.Add(this.StrongSipDownFunctionBox);
            this.tabPage2.Controls.Add(this.label54);
            this.tabPage2.Controls.Add(this.StrongSipUpFunctionBox);
            this.tabPage2.Location = new System.Drawing.Point(4, 4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(702, 323);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "StrongSip+";
            // 
            // clearButtonStrongSipRight
            // 
            this.clearButtonStrongSipRight.Enabled = false;
            this.clearButtonStrongSipRight.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongSipRight.Location = new System.Drawing.Point(513, 281);
            this.clearButtonStrongSipRight.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongSipRight.Name = "clearButtonStrongSipRight";
            this.clearButtonStrongSipRight.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongSipRight.TabIndex = 205;
            this.clearButtonStrongSipRight.Text = "x";
            this.clearButtonStrongSipRight.UseVisualStyleBackColor = true;
            this.clearButtonStrongSipRight.Visible = false;
            this.clearButtonStrongSipRight.Click += new System.EventHandler(this.clearStrongSipRight_Click);
            // 
            // StrongSipRightComboBox
            // 
            this.StrongSipRightComboBox.FormattingEnabled = true;
            this.StrongSipRightComboBox.Location = new System.Drawing.Point(513, 254);
            this.StrongSipRightComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipRightComboBox.Name = "StrongSipRightComboBox";
            this.StrongSipRightComboBox.Size = new System.Drawing.Size(143, 24);
            this.StrongSipRightComboBox.TabIndex = 204;
            this.StrongSipRightComboBox.Visible = false;
            this.StrongSipRightComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongSipRightComboBox_SelectedIndexChanged);
            // 
            // StrongSipRightNumericParameter
            // 
            this.StrongSipRightNumericParameter.Location = new System.Drawing.Point(177, 285);
            this.StrongSipRightNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipRightNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongSipRightNumericParameter.Name = "StrongSipRightNumericParameter";
            this.StrongSipRightNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.StrongSipRightNumericParameter.TabIndex = 203;
            this.StrongSipRightNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.StrongSipRightNumericParameter.Visible = false;
            // 
            // StrongSipRightLabel
            // 
            this.StrongSipRightLabel.AutoSize = true;
            this.StrongSipRightLabel.Location = new System.Drawing.Point(93, 286);
            this.StrongSipRightLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StrongSipRightLabel.Name = "StrongSipRightLabel";
            this.StrongSipRightLabel.Size = new System.Drawing.Size(78, 17);
            this.StrongSipRightLabel.TabIndex = 202;
            this.StrongSipRightLabel.Text = "Parameter:";
            // 
            // StrongSipRightParameterText
            // 
            this.StrongSipRightParameterText.Location = new System.Drawing.Point(177, 284);
            this.StrongSipRightParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StrongSipRightParameterText.Name = "StrongSipRightParameterText";
            this.StrongSipRightParameterText.ReadOnly = true;
            this.StrongSipRightParameterText.Size = new System.Drawing.Size(329, 22);
            this.StrongSipRightParameterText.TabIndex = 201;
            // 
            // label48
            // 
            this.label48.AutoSize = true;
            this.label48.Location = new System.Drawing.Point(44, 115);
            this.label48.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label48.Name = "label48";
            this.label48.Size = new System.Drawing.Size(125, 17);
            this.label48.TabIndex = 200;
            this.label48.Text = "StrongSip + Down:";
            // 
            // StrongSipRightFunctionBox
            // 
            this.StrongSipRightFunctionBox.FormattingEnabled = true;
            this.StrongSipRightFunctionBox.Location = new System.Drawing.Point(177, 254);
            this.StrongSipRightFunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipRightFunctionBox.Name = "StrongSipRightFunctionBox";
            this.StrongSipRightFunctionBox.Size = new System.Drawing.Size(329, 24);
            this.StrongSipRightFunctionBox.TabIndex = 199;
            this.StrongSipRightFunctionBox.SelectedIndexChanged += new System.EventHandler(this.StrongSipRightFunctionBox_SelectedIndexChanged);
            // 
            // clearButtonStrongSipLeft
            // 
            this.clearButtonStrongSipLeft.Enabled = false;
            this.clearButtonStrongSipLeft.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongSipLeft.Location = new System.Drawing.Point(513, 210);
            this.clearButtonStrongSipLeft.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongSipLeft.Name = "clearButtonStrongSipLeft";
            this.clearButtonStrongSipLeft.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongSipLeft.TabIndex = 198;
            this.clearButtonStrongSipLeft.Text = "x";
            this.clearButtonStrongSipLeft.UseVisualStyleBackColor = true;
            this.clearButtonStrongSipLeft.Visible = false;
            this.clearButtonStrongSipLeft.Click += new System.EventHandler(this.clearStrongSipLeft_Click);
            // 
            // clearButtonStrongSipDown
            // 
            this.clearButtonStrongSipDown.Enabled = false;
            this.clearButtonStrongSipDown.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongSipDown.Location = new System.Drawing.Point(513, 140);
            this.clearButtonStrongSipDown.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongSipDown.Name = "clearButtonStrongSipDown";
            this.clearButtonStrongSipDown.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongSipDown.TabIndex = 197;
            this.clearButtonStrongSipDown.Text = "x";
            this.clearButtonStrongSipDown.UseVisualStyleBackColor = true;
            this.clearButtonStrongSipDown.Visible = false;
            this.clearButtonStrongSipDown.Click += new System.EventHandler(this.clearStrongSipDown_Click);
            // 
            // clearButtonStrongSipUp
            // 
            this.clearButtonStrongSipUp.Enabled = false;
            this.clearButtonStrongSipUp.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongSipUp.Location = new System.Drawing.Point(513, 70);
            this.clearButtonStrongSipUp.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongSipUp.Name = "clearButtonStrongSipUp";
            this.clearButtonStrongSipUp.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongSipUp.TabIndex = 178;
            this.clearButtonStrongSipUp.Text = "x";
            this.clearButtonStrongSipUp.UseVisualStyleBackColor = true;
            this.clearButtonStrongSipUp.Visible = false;
            this.clearButtonStrongSipUp.Click += new System.EventHandler(this.clearStrongSipUp_Click);
            // 
            // StrongSipLeftComboBox
            // 
            this.StrongSipLeftComboBox.FormattingEnabled = true;
            this.StrongSipLeftComboBox.Location = new System.Drawing.Point(513, 183);
            this.StrongSipLeftComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipLeftComboBox.Name = "StrongSipLeftComboBox";
            this.StrongSipLeftComboBox.Size = new System.Drawing.Size(143, 24);
            this.StrongSipLeftComboBox.TabIndex = 196;
            this.StrongSipLeftComboBox.Visible = false;
            this.StrongSipLeftComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongSipLeftComboBox_SelectedIndexChanged);
            // 
            // StrongSipDownComboBox
            // 
            this.StrongSipDownComboBox.FormattingEnabled = true;
            this.StrongSipDownComboBox.Location = new System.Drawing.Point(513, 112);
            this.StrongSipDownComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipDownComboBox.Name = "StrongSipDownComboBox";
            this.StrongSipDownComboBox.Size = new System.Drawing.Size(143, 24);
            this.StrongSipDownComboBox.TabIndex = 195;
            this.StrongSipDownComboBox.Visible = false;
            this.StrongSipDownComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongSipDownComboBox_SelectedIndexChanged);
            // 
            // StrongSipUpComboBox
            // 
            this.StrongSipUpComboBox.FormattingEnabled = true;
            this.StrongSipUpComboBox.Location = new System.Drawing.Point(513, 42);
            this.StrongSipUpComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipUpComboBox.Name = "StrongSipUpComboBox";
            this.StrongSipUpComboBox.Size = new System.Drawing.Size(143, 24);
            this.StrongSipUpComboBox.TabIndex = 194;
            this.StrongSipUpComboBox.Visible = false;
            this.StrongSipUpComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongSipUpComboBox_SelectedIndexChanged);
            // 
            // StrongSipLeftNumericParameter
            // 
            this.StrongSipLeftNumericParameter.Location = new System.Drawing.Point(177, 214);
            this.StrongSipLeftNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipLeftNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongSipLeftNumericParameter.Name = "StrongSipLeftNumericParameter";
            this.StrongSipLeftNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.StrongSipLeftNumericParameter.TabIndex = 193;
            this.StrongSipLeftNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.StrongSipLeftNumericParameter.Visible = false;
            // 
            // StrongSipDownNumericParameter
            // 
            this.StrongSipDownNumericParameter.Location = new System.Drawing.Point(177, 143);
            this.StrongSipDownNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipDownNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongSipDownNumericParameter.Name = "StrongSipDownNumericParameter";
            this.StrongSipDownNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.StrongSipDownNumericParameter.TabIndex = 192;
            this.StrongSipDownNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.StrongSipDownNumericParameter.Visible = false;
            // 
            // StrongSipUpNumericParameter
            // 
            this.StrongSipUpNumericParameter.Location = new System.Drawing.Point(177, 74);
            this.StrongSipUpNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipUpNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongSipUpNumericParameter.Name = "StrongSipUpNumericParameter";
            this.StrongSipUpNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.StrongSipUpNumericParameter.TabIndex = 191;
            this.StrongSipUpNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.StrongSipUpNumericParameter.Visible = false;
            // 
            // StrongSipLeftLabel
            // 
            this.StrongSipLeftLabel.AutoSize = true;
            this.StrongSipLeftLabel.Location = new System.Drawing.Point(93, 215);
            this.StrongSipLeftLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StrongSipLeftLabel.Name = "StrongSipLeftLabel";
            this.StrongSipLeftLabel.Size = new System.Drawing.Size(78, 17);
            this.StrongSipLeftLabel.TabIndex = 190;
            this.StrongSipLeftLabel.Text = "Parameter:";
            // 
            // StrongSipLeftParameterText
            // 
            this.StrongSipLeftParameterText.Location = new System.Drawing.Point(177, 213);
            this.StrongSipLeftParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StrongSipLeftParameterText.Name = "StrongSipLeftParameterText";
            this.StrongSipLeftParameterText.ReadOnly = true;
            this.StrongSipLeftParameterText.Size = new System.Drawing.Size(329, 22);
            this.StrongSipLeftParameterText.TabIndex = 189;
            // 
            // StrongSipDownLabel
            // 
            this.StrongSipDownLabel.AutoSize = true;
            this.StrongSipDownLabel.Location = new System.Drawing.Point(93, 145);
            this.StrongSipDownLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StrongSipDownLabel.Name = "StrongSipDownLabel";
            this.StrongSipDownLabel.Size = new System.Drawing.Size(78, 17);
            this.StrongSipDownLabel.TabIndex = 188;
            this.StrongSipDownLabel.Text = "Parameter:";
            // 
            // StrongSipDownParameterText
            // 
            this.StrongSipDownParameterText.Location = new System.Drawing.Point(177, 143);
            this.StrongSipDownParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StrongSipDownParameterText.Name = "StrongSipDownParameterText";
            this.StrongSipDownParameterText.ReadOnly = true;
            this.StrongSipDownParameterText.Size = new System.Drawing.Size(329, 22);
            this.StrongSipDownParameterText.TabIndex = 187;
            // 
            // StrongSipUpLabel
            // 
            this.StrongSipUpLabel.AutoSize = true;
            this.StrongSipUpLabel.Location = new System.Drawing.Point(93, 77);
            this.StrongSipUpLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StrongSipUpLabel.Name = "StrongSipUpLabel";
            this.StrongSipUpLabel.Size = new System.Drawing.Size(78, 17);
            this.StrongSipUpLabel.TabIndex = 186;
            this.StrongSipUpLabel.Text = "Parameter:";
            this.StrongSipUpLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // StrongSipUpParameterText
            // 
            this.StrongSipUpParameterText.Location = new System.Drawing.Point(177, 74);
            this.StrongSipUpParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StrongSipUpParameterText.Name = "StrongSipUpParameterText";
            this.StrongSipUpParameterText.ReadOnly = true;
            this.StrongSipUpParameterText.Size = new System.Drawing.Size(329, 22);
            this.StrongSipUpParameterText.TabIndex = 185;
            // 
            // label52
            // 
            this.label52.AutoSize = true;
            this.label52.Location = new System.Drawing.Point(46, 256);
            this.label52.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label52.Name = "label52";
            this.label52.Size = new System.Drawing.Size(123, 17);
            this.label52.TabIndex = 184;
            this.label52.Text = "StrongSip + Right:";
            // 
            // StrongSipLeftFunctionBox
            // 
            this.StrongSipLeftFunctionBox.FormattingEnabled = true;
            this.StrongSipLeftFunctionBox.Location = new System.Drawing.Point(177, 183);
            this.StrongSipLeftFunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipLeftFunctionBox.Name = "StrongSipLeftFunctionBox";
            this.StrongSipLeftFunctionBox.Size = new System.Drawing.Size(329, 24);
            this.StrongSipLeftFunctionBox.TabIndex = 183;
            this.StrongSipLeftFunctionBox.SelectedIndexChanged += new System.EventHandler(this.StrongSipLeftFunctionBox_SelectedIndexChanged);
            // 
            // label53
            // 
            this.label53.AutoSize = true;
            this.label53.Location = new System.Drawing.Point(55, 185);
            this.label53.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label53.Name = "label53";
            this.label53.Size = new System.Drawing.Size(114, 17);
            this.label53.TabIndex = 182;
            this.label53.Text = "StrongSip + Left:";
            // 
            // StrongSipDownFunctionBox
            // 
            this.StrongSipDownFunctionBox.FormattingEnabled = true;
            this.StrongSipDownFunctionBox.Location = new System.Drawing.Point(177, 112);
            this.StrongSipDownFunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipDownFunctionBox.Name = "StrongSipDownFunctionBox";
            this.StrongSipDownFunctionBox.Size = new System.Drawing.Size(329, 24);
            this.StrongSipDownFunctionBox.TabIndex = 181;
            this.StrongSipDownFunctionBox.SelectedIndexChanged += new System.EventHandler(this.StrongSipDownFunctionBox_SelectedIndexChanged);
            // 
            // label54
            // 
            this.label54.AutoSize = true;
            this.label54.Location = new System.Drawing.Point(62, 46);
            this.label54.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label54.Name = "label54";
            this.label54.Size = new System.Drawing.Size(108, 17);
            this.label54.TabIndex = 180;
            this.label54.Text = "StrongSip + Up:";
            // 
            // StrongSipUpFunctionBox
            // 
            this.StrongSipUpFunctionBox.FormattingEnabled = true;
            this.StrongSipUpFunctionBox.Location = new System.Drawing.Point(177, 42);
            this.StrongSipUpFunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongSipUpFunctionBox.Name = "StrongSipUpFunctionBox";
            this.StrongSipUpFunctionBox.Size = new System.Drawing.Size(329, 24);
            this.StrongSipUpFunctionBox.TabIndex = 179;
            this.StrongSipUpFunctionBox.SelectedIndexChanged += new System.EventHandler(this.StrongSipUpFunctionBox_SelectedIndexChanged);
            // 
            // tabPage3
            // 
            this.tabPage3.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tabPage3.Controls.Add(this.label32);
            this.tabPage3.Controls.Add(this.clearButtonStrongPuffRight);
            this.tabPage3.Controls.Add(this.StrongPuffRightComboBox);
            this.tabPage3.Controls.Add(this.StrongPuffRightNumericParameter);
            this.tabPage3.Controls.Add(this.StrongPuffRightLabel);
            this.tabPage3.Controls.Add(this.StrongPuffRightParameterText);
            this.tabPage3.Controls.Add(this.label40);
            this.tabPage3.Controls.Add(this.StrongPuffRightFunctionBox);
            this.tabPage3.Controls.Add(this.clearButtonStrongPuffLeft);
            this.tabPage3.Controls.Add(this.clearButtonStrongPuffDown);
            this.tabPage3.Controls.Add(this.clearButtonStrongPuffUp);
            this.tabPage3.Controls.Add(this.StrongPuffLeftComboBox);
            this.tabPage3.Controls.Add(this.StrongPuffDownComboBox);
            this.tabPage3.Controls.Add(this.StrongPuffUpComboBox);
            this.tabPage3.Controls.Add(this.StrongPuffLeftNumericParameter);
            this.tabPage3.Controls.Add(this.StrongPuffDownNumericParameter);
            this.tabPage3.Controls.Add(this.StrongPuffUpNumericParameter);
            this.tabPage3.Controls.Add(this.StrongPuffLeftLabel);
            this.tabPage3.Controls.Add(this.StrongPuffLeftParameterText);
            this.tabPage3.Controls.Add(this.StrongPuffDownLabel);
            this.tabPage3.Controls.Add(this.StrongPuffDownParameterText);
            this.tabPage3.Controls.Add(this.StrongPuffUpLabel);
            this.tabPage3.Controls.Add(this.StrongPuffUpParameterText);
            this.tabPage3.Controls.Add(this.label44);
            this.tabPage3.Controls.Add(this.StrongPuffLeftFunctionBox);
            this.tabPage3.Controls.Add(this.label45);
            this.tabPage3.Controls.Add(this.StrongPuffDownFunctionBox);
            this.tabPage3.Controls.Add(this.label46);
            this.tabPage3.Controls.Add(this.StrongPuffUpFunctionBox);
            this.tabPage3.Location = new System.Drawing.Point(4, 4);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(702, 323);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "StrongPuff+";
            // 
            // clearButtonStrongPuffRight
            // 
            this.clearButtonStrongPuffRight.Enabled = false;
            this.clearButtonStrongPuffRight.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongPuffRight.Location = new System.Drawing.Point(511, 281);
            this.clearButtonStrongPuffRight.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongPuffRight.Name = "clearButtonStrongPuffRight";
            this.clearButtonStrongPuffRight.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongPuffRight.TabIndex = 177;
            this.clearButtonStrongPuffRight.Text = "x";
            this.clearButtonStrongPuffRight.UseVisualStyleBackColor = true;
            this.clearButtonStrongPuffRight.Visible = false;
            this.clearButtonStrongPuffRight.Click += new System.EventHandler(this.clearStrongPuffRight_Click);
            // 
            // StrongPuffRightComboBox
            // 
            this.StrongPuffRightComboBox.FormattingEnabled = true;
            this.StrongPuffRightComboBox.Location = new System.Drawing.Point(511, 254);
            this.StrongPuffRightComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffRightComboBox.Name = "StrongPuffRightComboBox";
            this.StrongPuffRightComboBox.Size = new System.Drawing.Size(143, 24);
            this.StrongPuffRightComboBox.TabIndex = 176;
            this.StrongPuffRightComboBox.Visible = false;
            this.StrongPuffRightComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongPuffRightComboBox_SelectedIndexChanged);
            // 
            // StrongPuffRightNumericParameter
            // 
            this.StrongPuffRightNumericParameter.Location = new System.Drawing.Point(175, 285);
            this.StrongPuffRightNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffRightNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongPuffRightNumericParameter.Name = "StrongPuffRightNumericParameter";
            this.StrongPuffRightNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.StrongPuffRightNumericParameter.TabIndex = 175;
            this.StrongPuffRightNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.StrongPuffRightNumericParameter.Visible = false;
            // 
            // StrongPuffRightLabel
            // 
            this.StrongPuffRightLabel.AutoSize = true;
            this.StrongPuffRightLabel.Location = new System.Drawing.Point(91, 286);
            this.StrongPuffRightLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StrongPuffRightLabel.Name = "StrongPuffRightLabel";
            this.StrongPuffRightLabel.Size = new System.Drawing.Size(78, 17);
            this.StrongPuffRightLabel.TabIndex = 174;
            this.StrongPuffRightLabel.Text = "Parameter:";
            // 
            // StrongPuffRightParameterText
            // 
            this.StrongPuffRightParameterText.Location = new System.Drawing.Point(175, 284);
            this.StrongPuffRightParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StrongPuffRightParameterText.Name = "StrongPuffRightParameterText";
            this.StrongPuffRightParameterText.ReadOnly = true;
            this.StrongPuffRightParameterText.Size = new System.Drawing.Size(329, 22);
            this.StrongPuffRightParameterText.TabIndex = 173;
            // 
            // label40
            // 
            this.label40.AutoSize = true;
            this.label40.Location = new System.Drawing.Point(36, 115);
            this.label40.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label40.Name = "label40";
            this.label40.Size = new System.Drawing.Size(130, 17);
            this.label40.TabIndex = 172;
            this.label40.Text = "StrongPuff + Down:";
            // 
            // StrongPuffRightFunctionBox
            // 
            this.StrongPuffRightFunctionBox.FormattingEnabled = true;
            this.StrongPuffRightFunctionBox.Location = new System.Drawing.Point(175, 254);
            this.StrongPuffRightFunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffRightFunctionBox.Name = "StrongPuffRightFunctionBox";
            this.StrongPuffRightFunctionBox.Size = new System.Drawing.Size(329, 24);
            this.StrongPuffRightFunctionBox.TabIndex = 171;
            this.StrongPuffRightFunctionBox.SelectedIndexChanged += new System.EventHandler(this.StrongPuffRightFunctionBox_SelectedIndexChanged);
            // 
            // clearButtonStrongPuffLeft
            // 
            this.clearButtonStrongPuffLeft.Enabled = false;
            this.clearButtonStrongPuffLeft.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongPuffLeft.Location = new System.Drawing.Point(511, 210);
            this.clearButtonStrongPuffLeft.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongPuffLeft.Name = "clearButtonStrongPuffLeft";
            this.clearButtonStrongPuffLeft.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongPuffLeft.TabIndex = 170;
            this.clearButtonStrongPuffLeft.Text = "x";
            this.clearButtonStrongPuffLeft.UseVisualStyleBackColor = true;
            this.clearButtonStrongPuffLeft.Visible = false;
            this.clearButtonStrongPuffLeft.Click += new System.EventHandler(this.clearStrongPuffLeft_Click);
            // 
            // clearButtonStrongPuffDown
            // 
            this.clearButtonStrongPuffDown.Enabled = false;
            this.clearButtonStrongPuffDown.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongPuffDown.Location = new System.Drawing.Point(511, 140);
            this.clearButtonStrongPuffDown.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongPuffDown.Name = "clearButtonStrongPuffDown";
            this.clearButtonStrongPuffDown.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongPuffDown.TabIndex = 169;
            this.clearButtonStrongPuffDown.Text = "x";
            this.clearButtonStrongPuffDown.UseVisualStyleBackColor = true;
            this.clearButtonStrongPuffDown.Visible = false;
            this.clearButtonStrongPuffDown.Click += new System.EventHandler(this.clearStrongPuffDown_Click);
            // 
            // clearButtonStrongPuffUp
            // 
            this.clearButtonStrongPuffUp.Enabled = false;
            this.clearButtonStrongPuffUp.ForeColor = System.Drawing.Color.Black;
            this.clearButtonStrongPuffUp.Location = new System.Drawing.Point(511, 70);
            this.clearButtonStrongPuffUp.Margin = new System.Windows.Forms.Padding(4);
            this.clearButtonStrongPuffUp.Name = "clearButtonStrongPuffUp";
            this.clearButtonStrongPuffUp.Size = new System.Drawing.Size(36, 28);
            this.clearButtonStrongPuffUp.TabIndex = 150;
            this.clearButtonStrongPuffUp.Text = "x";
            this.clearButtonStrongPuffUp.UseVisualStyleBackColor = true;
            this.clearButtonStrongPuffUp.Visible = false;
            this.clearButtonStrongPuffUp.Click += new System.EventHandler(this.clearStrongPuffUp_Click);
            // 
            // StrongPuffLeftComboBox
            // 
            this.StrongPuffLeftComboBox.FormattingEnabled = true;
            this.StrongPuffLeftComboBox.Location = new System.Drawing.Point(511, 183);
            this.StrongPuffLeftComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffLeftComboBox.Name = "StrongPuffLeftComboBox";
            this.StrongPuffLeftComboBox.Size = new System.Drawing.Size(143, 24);
            this.StrongPuffLeftComboBox.TabIndex = 168;
            this.StrongPuffLeftComboBox.Visible = false;
            this.StrongPuffLeftComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongPuffLeftComboBox_SelectedIndexChanged);
            // 
            // StrongPuffDownComboBox
            // 
            this.StrongPuffDownComboBox.FormattingEnabled = true;
            this.StrongPuffDownComboBox.Location = new System.Drawing.Point(511, 112);
            this.StrongPuffDownComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffDownComboBox.Name = "StrongPuffDownComboBox";
            this.StrongPuffDownComboBox.Size = new System.Drawing.Size(143, 24);
            this.StrongPuffDownComboBox.TabIndex = 167;
            this.StrongPuffDownComboBox.Visible = false;
            this.StrongPuffDownComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongPuffDownComboBox_SelectedIndexChanged);
            // 
            // StrongPuffUpComboBox
            // 
            this.StrongPuffUpComboBox.FormattingEnabled = true;
            this.StrongPuffUpComboBox.Location = new System.Drawing.Point(511, 42);
            this.StrongPuffUpComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffUpComboBox.Name = "StrongPuffUpComboBox";
            this.StrongPuffUpComboBox.Size = new System.Drawing.Size(143, 24);
            this.StrongPuffUpComboBox.TabIndex = 166;
            this.StrongPuffUpComboBox.Visible = false;
            this.StrongPuffUpComboBox.SelectedIndexChanged += new System.EventHandler(this.StrongPuffUpComboBox_SelectedIndexChanged);
            // 
            // StrongPuffLeftNumericParameter
            // 
            this.StrongPuffLeftNumericParameter.Location = new System.Drawing.Point(175, 214);
            this.StrongPuffLeftNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffLeftNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongPuffLeftNumericParameter.Name = "StrongPuffLeftNumericParameter";
            this.StrongPuffLeftNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.StrongPuffLeftNumericParameter.TabIndex = 165;
            this.StrongPuffLeftNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.StrongPuffLeftNumericParameter.Visible = false;
            // 
            // StrongPuffDownNumericParameter
            // 
            this.StrongPuffDownNumericParameter.Location = new System.Drawing.Point(175, 143);
            this.StrongPuffDownNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffDownNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongPuffDownNumericParameter.Name = "StrongPuffDownNumericParameter";
            this.StrongPuffDownNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.StrongPuffDownNumericParameter.TabIndex = 164;
            this.StrongPuffDownNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.StrongPuffDownNumericParameter.Visible = false;
            // 
            // StrongPuffUpNumericParameter
            // 
            this.StrongPuffUpNumericParameter.Location = new System.Drawing.Point(175, 74);
            this.StrongPuffUpNumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffUpNumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.StrongPuffUpNumericParameter.Name = "StrongPuffUpNumericParameter";
            this.StrongPuffUpNumericParameter.Size = new System.Drawing.Size(60, 22);
            this.StrongPuffUpNumericParameter.TabIndex = 163;
            this.StrongPuffUpNumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.StrongPuffUpNumericParameter.Visible = false;
            // 
            // StrongPuffLeftLabel
            // 
            this.StrongPuffLeftLabel.AutoSize = true;
            this.StrongPuffLeftLabel.Location = new System.Drawing.Point(91, 215);
            this.StrongPuffLeftLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StrongPuffLeftLabel.Name = "StrongPuffLeftLabel";
            this.StrongPuffLeftLabel.Size = new System.Drawing.Size(78, 17);
            this.StrongPuffLeftLabel.TabIndex = 162;
            this.StrongPuffLeftLabel.Text = "Parameter:";
            // 
            // StrongPuffLeftParameterText
            // 
            this.StrongPuffLeftParameterText.Location = new System.Drawing.Point(175, 213);
            this.StrongPuffLeftParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StrongPuffLeftParameterText.Name = "StrongPuffLeftParameterText";
            this.StrongPuffLeftParameterText.ReadOnly = true;
            this.StrongPuffLeftParameterText.Size = new System.Drawing.Size(329, 22);
            this.StrongPuffLeftParameterText.TabIndex = 161;
            // 
            // StrongPuffDownLabel
            // 
            this.StrongPuffDownLabel.AutoSize = true;
            this.StrongPuffDownLabel.Location = new System.Drawing.Point(91, 145);
            this.StrongPuffDownLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StrongPuffDownLabel.Name = "StrongPuffDownLabel";
            this.StrongPuffDownLabel.Size = new System.Drawing.Size(78, 17);
            this.StrongPuffDownLabel.TabIndex = 160;
            this.StrongPuffDownLabel.Text = "Parameter:";
            // 
            // StrongPuffDownParameterText
            // 
            this.StrongPuffDownParameterText.Location = new System.Drawing.Point(175, 143);
            this.StrongPuffDownParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StrongPuffDownParameterText.Name = "StrongPuffDownParameterText";
            this.StrongPuffDownParameterText.ReadOnly = true;
            this.StrongPuffDownParameterText.Size = new System.Drawing.Size(329, 22);
            this.StrongPuffDownParameterText.TabIndex = 159;
            // 
            // StrongPuffUpLabel
            // 
            this.StrongPuffUpLabel.AutoSize = true;
            this.StrongPuffUpLabel.Location = new System.Drawing.Point(91, 77);
            this.StrongPuffUpLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StrongPuffUpLabel.Name = "StrongPuffUpLabel";
            this.StrongPuffUpLabel.Size = new System.Drawing.Size(78, 17);
            this.StrongPuffUpLabel.TabIndex = 158;
            this.StrongPuffUpLabel.Text = "Parameter:";
            this.StrongPuffUpLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // StrongPuffUpParameterText
            // 
            this.StrongPuffUpParameterText.Location = new System.Drawing.Point(175, 74);
            this.StrongPuffUpParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.StrongPuffUpParameterText.Name = "StrongPuffUpParameterText";
            this.StrongPuffUpParameterText.ReadOnly = true;
            this.StrongPuffUpParameterText.Size = new System.Drawing.Size(329, 22);
            this.StrongPuffUpParameterText.TabIndex = 157;
            // 
            // label44
            // 
            this.label44.AutoSize = true;
            this.label44.Location = new System.Drawing.Point(38, 257);
            this.label44.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label44.Name = "label44";
            this.label44.Size = new System.Drawing.Size(128, 17);
            this.label44.TabIndex = 156;
            this.label44.Text = "StrongPuff + Right:";
            // 
            // StrongPuffLeftFunctionBox
            // 
            this.StrongPuffLeftFunctionBox.FormattingEnabled = true;
            this.StrongPuffLeftFunctionBox.Location = new System.Drawing.Point(175, 183);
            this.StrongPuffLeftFunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffLeftFunctionBox.Name = "StrongPuffLeftFunctionBox";
            this.StrongPuffLeftFunctionBox.Size = new System.Drawing.Size(329, 24);
            this.StrongPuffLeftFunctionBox.TabIndex = 155;
            this.StrongPuffLeftFunctionBox.SelectedIndexChanged += new System.EventHandler(this.StrongPuffLeftFunctionBox_SelectedIndexChanged);
            // 
            // label45
            // 
            this.label45.AutoSize = true;
            this.label45.Location = new System.Drawing.Point(47, 186);
            this.label45.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label45.Name = "label45";
            this.label45.Size = new System.Drawing.Size(119, 17);
            this.label45.TabIndex = 154;
            this.label45.Text = "StrongPuff + Left:";
            // 
            // StrongPuffDownFunctionBox
            // 
            this.StrongPuffDownFunctionBox.FormattingEnabled = true;
            this.StrongPuffDownFunctionBox.Location = new System.Drawing.Point(175, 112);
            this.StrongPuffDownFunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffDownFunctionBox.Name = "StrongPuffDownFunctionBox";
            this.StrongPuffDownFunctionBox.Size = new System.Drawing.Size(329, 24);
            this.StrongPuffDownFunctionBox.TabIndex = 153;
            this.StrongPuffDownFunctionBox.SelectedIndexChanged += new System.EventHandler(this.StrongPuffDownFunctionBox_SelectedIndexChanged);
            // 
            // label46
            // 
            this.label46.AutoSize = true;
            this.label46.Location = new System.Drawing.Point(55, 46);
            this.label46.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label46.Name = "label46";
            this.label46.Size = new System.Drawing.Size(113, 17);
            this.label46.TabIndex = 152;
            this.label46.Text = "StrongPuff + Up:";
            // 
            // StrongPuffUpFunctionBox
            // 
            this.StrongPuffUpFunctionBox.FormattingEnabled = true;
            this.StrongPuffUpFunctionBox.Location = new System.Drawing.Point(175, 42);
            this.StrongPuffUpFunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.StrongPuffUpFunctionBox.Name = "StrongPuffUpFunctionBox";
            this.StrongPuffUpFunctionBox.Size = new System.Drawing.Size(329, 24);
            this.StrongPuffUpFunctionBox.TabIndex = 151;
            this.StrongPuffUpFunctionBox.SelectedIndexChanged += new System.EventHandler(this.StrongPuffUpFunctionBox_SelectedIndexChanged);
            // 
            // ButtonsTab
            // 
            this.ButtonsTab.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.ButtonsTab.Controls.Add(this.clearButton3);
            this.ButtonsTab.Controls.Add(this.clearButton2);
            this.ButtonsTab.Controls.Add(this.clearButton1);
            this.ButtonsTab.Controls.Add(this.label21);
            this.ButtonsTab.Controls.Add(this.Button3ComboBox);
            this.ButtonsTab.Controls.Add(this.Button2ComboBox);
            this.ButtonsTab.Controls.Add(this.Button1ComboBox);
            this.ButtonsTab.Controls.Add(this.Button3NumericParameter);
            this.ButtonsTab.Controls.Add(this.Button2NumericParameter);
            this.ButtonsTab.Controls.Add(this.Button1NumericParameter);
            this.ButtonsTab.Controls.Add(this.Button3Label);
            this.ButtonsTab.Controls.Add(this.Button3ParameterText);
            this.ButtonsTab.Controls.Add(this.Button2Label);
            this.ButtonsTab.Controls.Add(this.Button2ParameterText);
            this.ButtonsTab.Controls.Add(this.Button1Label);
            this.ButtonsTab.Controls.Add(this.Button1ParameterText);
            this.ButtonsTab.Controls.Add(this.label8);
            this.ButtonsTab.Controls.Add(this.Button3FunctionBox);
            this.ButtonsTab.Controls.Add(this.label7);
            this.ButtonsTab.Controls.Add(this.Button2FunctionBox);
            this.ButtonsTab.Controls.Add(this.label6);
            this.ButtonsTab.Controls.Add(this.Button1FunctionBox);
            this.ButtonsTab.Location = new System.Drawing.Point(4, 25);
            this.ButtonsTab.Margin = new System.Windows.Forms.Padding(4);
            this.ButtonsTab.Name = "ButtonsTab";
            this.ButtonsTab.Padding = new System.Windows.Forms.Padding(4);
            this.ButtonsTab.Size = new System.Drawing.Size(780, 361);
            this.ButtonsTab.TabIndex = 1;
            this.ButtonsTab.Text = "Button Actions";
            // 
            // clearButton3
            // 
            this.clearButton3.Enabled = false;
            this.clearButton3.ForeColor = System.Drawing.Color.Black;
            this.clearButton3.Location = new System.Drawing.Point(566, 278);
            this.clearButton3.Margin = new System.Windows.Forms.Padding(4);
            this.clearButton3.Name = "clearButton3";
            this.clearButton3.Size = new System.Drawing.Size(36, 28);
            this.clearButton3.TabIndex = 142;
            this.clearButton3.Text = "x";
            this.clearButton3.UseVisualStyleBackColor = true;
            this.clearButton3.Visible = false;
            this.clearButton3.Click += new System.EventHandler(this.clearButton3_Click);
            // 
            // clearButton2
            // 
            this.clearButton2.Enabled = false;
            this.clearButton2.ForeColor = System.Drawing.Color.Black;
            this.clearButton2.Location = new System.Drawing.Point(566, 201);
            this.clearButton2.Margin = new System.Windows.Forms.Padding(4);
            this.clearButton2.Name = "clearButton2";
            this.clearButton2.Size = new System.Drawing.Size(36, 28);
            this.clearButton2.TabIndex = 141;
            this.clearButton2.Text = "x";
            this.clearButton2.UseVisualStyleBackColor = true;
            this.clearButton2.Visible = false;
            this.clearButton2.Click += new System.EventHandler(this.clearButton2_Click);
            // 
            // clearButton1
            // 
            this.clearButton1.Enabled = false;
            this.clearButton1.ForeColor = System.Drawing.Color.Black;
            this.clearButton1.Location = new System.Drawing.Point(566, 124);
            this.clearButton1.Margin = new System.Windows.Forms.Padding(4);
            this.clearButton1.Name = "clearButton1";
            this.clearButton1.Size = new System.Drawing.Size(36, 28);
            this.clearButton1.TabIndex = 78;
            this.clearButton1.Text = "x";
            this.clearButton1.UseVisualStyleBackColor = true;
            this.clearButton1.Visible = false;
            this.clearButton1.Click += new System.EventHandler(this.clearButton1_Click);
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(228, 47);
            this.label21.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(307, 17);
            this.label21.TabIndex = 140;
            this.label21.Text = "Select Actions for internal and external Buttons:";
            // 
            // Button3ComboBox
            // 
            this.Button3ComboBox.FormattingEnabled = true;
            this.Button3ComboBox.Location = new System.Drawing.Point(566, 251);
            this.Button3ComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.Button3ComboBox.Name = "Button3ComboBox";
            this.Button3ComboBox.Size = new System.Drawing.Size(143, 24);
            this.Button3ComboBox.TabIndex = 114;
            this.Button3ComboBox.Visible = false;
            this.Button3ComboBox.SelectedIndexChanged += new System.EventHandler(this.Button3ComboBox_SelectedIndexChanged);
            // 
            // Button2ComboBox
            // 
            this.Button2ComboBox.FormattingEnabled = true;
            this.Button2ComboBox.Location = new System.Drawing.Point(566, 173);
            this.Button2ComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.Button2ComboBox.Name = "Button2ComboBox";
            this.Button2ComboBox.Size = new System.Drawing.Size(143, 24);
            this.Button2ComboBox.TabIndex = 113;
            this.Button2ComboBox.Visible = false;
            this.Button2ComboBox.SelectedIndexChanged += new System.EventHandler(this.Button2ComboBox_SelectedIndexChanged);
            // 
            // Button1ComboBox
            // 
            this.Button1ComboBox.FormattingEnabled = true;
            this.Button1ComboBox.Location = new System.Drawing.Point(566, 96);
            this.Button1ComboBox.Margin = new System.Windows.Forms.Padding(4);
            this.Button1ComboBox.Name = "Button1ComboBox";
            this.Button1ComboBox.Size = new System.Drawing.Size(143, 24);
            this.Button1ComboBox.TabIndex = 112;
            this.Button1ComboBox.Visible = false;
            this.Button1ComboBox.SelectedIndexChanged += new System.EventHandler(this.Button1ComboBox_SelectedIndexChanged);
            // 
            // Button3NumericParameter
            // 
            this.Button3NumericParameter.Location = new System.Drawing.Point(230, 282);
            this.Button3NumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.Button3NumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.Button3NumericParameter.Name = "Button3NumericParameter";
            this.Button3NumericParameter.Size = new System.Drawing.Size(60, 22);
            this.Button3NumericParameter.TabIndex = 109;
            this.Button3NumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.Button3NumericParameter.Visible = false;
            // 
            // Button2NumericParameter
            // 
            this.Button2NumericParameter.Location = new System.Drawing.Point(230, 204);
            this.Button2NumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.Button2NumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.Button2NumericParameter.Name = "Button2NumericParameter";
            this.Button2NumericParameter.Size = new System.Drawing.Size(60, 22);
            this.Button2NumericParameter.TabIndex = 108;
            this.Button2NumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.Button2NumericParameter.Visible = false;
            // 
            // Button1NumericParameter
            // 
            this.Button1NumericParameter.Location = new System.Drawing.Point(230, 128);
            this.Button1NumericParameter.Margin = new System.Windows.Forms.Padding(4);
            this.Button1NumericParameter.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.Button1NumericParameter.Name = "Button1NumericParameter";
            this.Button1NumericParameter.Size = new System.Drawing.Size(60, 22);
            this.Button1NumericParameter.TabIndex = 107;
            this.Button1NumericParameter.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.Button1NumericParameter.Visible = false;
            // 
            // Button3Label
            // 
            this.Button3Label.AutoSize = true;
            this.Button3Label.Location = new System.Drawing.Point(146, 283);
            this.Button3Label.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Button3Label.Name = "Button3Label";
            this.Button3Label.Size = new System.Drawing.Size(78, 17);
            this.Button3Label.TabIndex = 102;
            this.Button3Label.Text = "Parameter:";
            // 
            // Button3ParameterText
            // 
            this.Button3ParameterText.Location = new System.Drawing.Point(230, 281);
            this.Button3ParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Button3ParameterText.Name = "Button3ParameterText";
            this.Button3ParameterText.ReadOnly = true;
            this.Button3ParameterText.Size = new System.Drawing.Size(329, 22);
            this.Button3ParameterText.TabIndex = 101;
            // 
            // Button2Label
            // 
            this.Button2Label.AutoSize = true;
            this.Button2Label.Location = new System.Drawing.Point(146, 206);
            this.Button2Label.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Button2Label.Name = "Button2Label";
            this.Button2Label.Size = new System.Drawing.Size(78, 17);
            this.Button2Label.TabIndex = 100;
            this.Button2Label.Text = "Parameter:";
            // 
            // Button2ParameterText
            // 
            this.Button2ParameterText.Location = new System.Drawing.Point(230, 204);
            this.Button2ParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Button2ParameterText.Name = "Button2ParameterText";
            this.Button2ParameterText.ReadOnly = true;
            this.Button2ParameterText.Size = new System.Drawing.Size(329, 22);
            this.Button2ParameterText.TabIndex = 99;
            // 
            // Button1Label
            // 
            this.Button1Label.AutoSize = true;
            this.Button1Label.Location = new System.Drawing.Point(146, 131);
            this.Button1Label.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Button1Label.Name = "Button1Label";
            this.Button1Label.Size = new System.Drawing.Size(78, 17);
            this.Button1Label.TabIndex = 98;
            this.Button1Label.Text = "Parameter:";
            this.Button1Label.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // Button1ParameterText
            // 
            this.Button1ParameterText.Location = new System.Drawing.Point(230, 128);
            this.Button1ParameterText.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Button1ParameterText.Name = "Button1ParameterText";
            this.Button1ParameterText.ReadOnly = true;
            this.Button1ParameterText.Size = new System.Drawing.Size(329, 22);
            this.Button1ParameterText.TabIndex = 97;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(93, 254);
            this.label8.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(129, 17);
            this.label8.TabIndex = 92;
            this.label8.Text = "Button3  (external):";
            // 
            // Button3FunctionBox
            // 
            this.Button3FunctionBox.FormattingEnabled = true;
            this.Button3FunctionBox.Location = new System.Drawing.Point(230, 251);
            this.Button3FunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.Button3FunctionBox.Name = "Button3FunctionBox";
            this.Button3FunctionBox.Size = new System.Drawing.Size(329, 24);
            this.Button3FunctionBox.TabIndex = 91;
            this.Button3FunctionBox.SelectedIndexChanged += new System.EventHandler(this.Button3FunctionBox_SelectedIndexChanged_1);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(97, 177);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(125, 17);
            this.label7.TabIndex = 90;
            this.label7.Text = "Button2 (external):";
            // 
            // Button2FunctionBox
            // 
            this.Button2FunctionBox.FormattingEnabled = true;
            this.Button2FunctionBox.Location = new System.Drawing.Point(230, 173);
            this.Button2FunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.Button2FunctionBox.Name = "Button2FunctionBox";
            this.Button2FunctionBox.Size = new System.Drawing.Size(329, 24);
            this.Button2FunctionBox.TabIndex = 89;
            this.Button2FunctionBox.SelectedIndexChanged += new System.EventHandler(this.Button2FunctionBox_SelectedIndexChanged_1);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(97, 100);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(126, 17);
            this.label6.TabIndex = 88;
            this.label6.Text = " Button1 (internal):";
            // 
            // Button1FunctionBox
            // 
            this.Button1FunctionBox.FormattingEnabled = true;
            this.Button1FunctionBox.Location = new System.Drawing.Point(230, 96);
            this.Button1FunctionBox.Margin = new System.Windows.Forms.Padding(4);
            this.Button1FunctionBox.Name = "Button1FunctionBox";
            this.Button1FunctionBox.Size = new System.Drawing.Size(329, 24);
            this.Button1FunctionBox.TabIndex = 87;
            this.Button1FunctionBox.SelectedIndexChanged += new System.EventHandler(this.Button1FunctionBox_SelectedIndexChanged_1);
            // 
            // RawValueTab
            // 
            this.RawValueTab.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.RawValueTab.Controls.Add(this.incRightGain);
            this.RawValueTab.Controls.Add(this.decRightGain);
            this.RawValueTab.Controls.Add(this.rightGainLabel);
            this.RawValueTab.Controls.Add(this.label28);
            this.RawValueTab.Controls.Add(this.rightGainBar);
            this.RawValueTab.Controls.Add(this.incDownGain);
            this.RawValueTab.Controls.Add(this.decDownGain);
            this.RawValueTab.Controls.Add(this.downGainLabel);
            this.RawValueTab.Controls.Add(this.label25);
            this.RawValueTab.Controls.Add(this.incLeftGain);
            this.RawValueTab.Controls.Add(this.decLeftGain);
            this.RawValueTab.Controls.Add(this.leftGainLabel);
            this.RawValueTab.Controls.Add(this.label26);
            this.RawValueTab.Controls.Add(this.incUpGain);
            this.RawValueTab.Controls.Add(this.decUpGain);
            this.RawValueTab.Controls.Add(this.upGainLabel);
            this.RawValueTab.Controls.Add(this.label23);
            this.RawValueTab.Controls.Add(this.label22);
            this.RawValueTab.Controls.Add(this.downSensorLabel);
            this.RawValueTab.Controls.Add(this.label19);
            this.RawValueTab.Controls.Add(this.leftSensorLabel);
            this.RawValueTab.Controls.Add(this.label17);
            this.RawValueTab.Controls.Add(this.rightSensorLabel);
            this.RawValueTab.Controls.Add(this.label10);
            this.RawValueTab.Controls.Add(this.upSensorLabel);
            this.RawValueTab.Controls.Add(this.rightPanel);
            this.RawValueTab.Controls.Add(this.leftPanel);
            this.RawValueTab.Controls.Add(this.downPanel);
            this.RawValueTab.Controls.Add(this.upPanel);
            this.RawValueTab.Controls.Add(this.leftGainBar);
            this.RawValueTab.Controls.Add(this.downGainBar);
            this.RawValueTab.Controls.Add(this.upGainBar);
            this.RawValueTab.Location = new System.Drawing.Point(4, 25);
            this.RawValueTab.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.RawValueTab.Name = "RawValueTab";
            this.RawValueTab.Size = new System.Drawing.Size(780, 361);
            this.RawValueTab.TabIndex = 4;
            this.RawValueTab.Text = "Force Levels";
            // 
            // incRightGain
            // 
            this.incRightGain.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incRightGain.ForeColor = System.Drawing.Color.Black;
            this.incRightGain.Location = new System.Drawing.Point(477, 252);
            this.incRightGain.Margin = new System.Windows.Forms.Padding(4);
            this.incRightGain.Name = "incRightGain";
            this.incRightGain.Size = new System.Drawing.Size(41, 37);
            this.incRightGain.TabIndex = 136;
            this.incRightGain.Text = "+";
            this.incRightGain.UseVisualStyleBackColor = true;
            this.incRightGain.Click += new System.EventHandler(this.incRightGain_Click);
            this.incRightGain.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incRightGain.MouseHover += new System.EventHandler(this.incRightGain_MouseHover);
            // 
            // decRightGain
            // 
            this.decRightGain.Cursor = System.Windows.Forms.Cursors.Hand;
            this.decRightGain.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decRightGain.ForeColor = System.Drawing.Color.Black;
            this.decRightGain.Location = new System.Drawing.Point(429, 252);
            this.decRightGain.Margin = new System.Windows.Forms.Padding(4);
            this.decRightGain.Name = "decRightGain";
            this.decRightGain.Size = new System.Drawing.Size(41, 37);
            this.decRightGain.TabIndex = 135;
            this.decRightGain.Text = "-";
            this.decRightGain.UseVisualStyleBackColor = true;
            this.decRightGain.Click += new System.EventHandler(this.decRightGain_Click);
            this.decRightGain.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decRightGain.MouseHover += new System.EventHandler(this.decRightGain_MouseHover);
            // 
            // rightGainLabel
            // 
            this.rightGainLabel.AutoSize = true;
            this.rightGainLabel.Location = new System.Drawing.Point(535, 274);
            this.rightGainLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.rightGainLabel.Name = "rightGainLabel";
            this.rightGainLabel.Size = new System.Drawing.Size(24, 17);
            this.rightGainLabel.TabIndex = 134;
            this.rightGainLabel.Text = "50";
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(532, 254);
            this.label28.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(67, 17);
            this.label28.TabIndex = 132;
            this.label28.Text = "right gain";
            // 
            // rightGainBar
            // 
            this.rightGainBar.LargeChange = 10;
            this.rightGainBar.Location = new System.Drawing.Point(418, 210);
            this.rightGainBar.Margin = new System.Windows.Forms.Padding(4);
            this.rightGainBar.Maximum = 100;
            this.rightGainBar.Name = "rightGainBar";
            this.rightGainBar.Size = new System.Drawing.Size(117, 56);
            this.rightGainBar.TabIndex = 133;
            this.rightGainBar.TickFrequency = 10;
            this.rightGainBar.Value = 50;
            this.rightGainBar.Scroll += new System.EventHandler(this.rightGainBar_Scroll);
            // 
            // incDownGain
            // 
            this.incDownGain.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incDownGain.ForeColor = System.Drawing.Color.Black;
            this.incDownGain.Location = new System.Drawing.Point(258, 272);
            this.incDownGain.Margin = new System.Windows.Forms.Padding(4);
            this.incDownGain.Name = "incDownGain";
            this.incDownGain.Size = new System.Drawing.Size(41, 37);
            this.incDownGain.TabIndex = 131;
            this.incDownGain.Text = "+";
            this.incDownGain.UseVisualStyleBackColor = true;
            this.incDownGain.Click += new System.EventHandler(this.incDownGain_Click);
            this.incDownGain.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incDownGain.MouseHover += new System.EventHandler(this.incDownGain_MouseHover);
            // 
            // decDownGain
            // 
            this.decDownGain.Cursor = System.Windows.Forms.Cursors.Hand;
            this.decDownGain.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decDownGain.ForeColor = System.Drawing.Color.Black;
            this.decDownGain.Location = new System.Drawing.Point(258, 228);
            this.decDownGain.Margin = new System.Windows.Forms.Padding(4);
            this.decDownGain.Name = "decDownGain";
            this.decDownGain.Size = new System.Drawing.Size(41, 37);
            this.decDownGain.TabIndex = 130;
            this.decDownGain.Text = "-";
            this.decDownGain.UseVisualStyleBackColor = true;
            this.decDownGain.Click += new System.EventHandler(this.decDownGain_Click);
            this.decDownGain.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decDownGain.MouseHover += new System.EventHandler(this.decDownGain_MouseHover);
            // 
            // downGainLabel
            // 
            this.downGainLabel.AutoSize = true;
            this.downGainLabel.Location = new System.Drawing.Point(210, 268);
            this.downGainLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.downGainLabel.Name = "downGainLabel";
            this.downGainLabel.Size = new System.Drawing.Size(24, 17);
            this.downGainLabel.TabIndex = 129;
            this.downGainLabel.Text = "50";
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(172, 250);
            this.label25.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(72, 17);
            this.label25.TabIndex = 127;
            this.label25.Text = "down gain";
            // 
            // incLeftGain
            // 
            this.incLeftGain.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incLeftGain.ForeColor = System.Drawing.Color.Black;
            this.incLeftGain.Location = new System.Drawing.Point(236, 82);
            this.incLeftGain.Margin = new System.Windows.Forms.Padding(4);
            this.incLeftGain.Name = "incLeftGain";
            this.incLeftGain.Size = new System.Drawing.Size(41, 37);
            this.incLeftGain.TabIndex = 126;
            this.incLeftGain.Text = "+";
            this.incLeftGain.UseVisualStyleBackColor = true;
            this.incLeftGain.Click += new System.EventHandler(this.incLeftGain_Click);
            this.incLeftGain.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incLeftGain.MouseHover += new System.EventHandler(this.incLeftGain_MouseHover);
            // 
            // decLeftGain
            // 
            this.decLeftGain.Cursor = System.Windows.Forms.Cursors.Hand;
            this.decLeftGain.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decLeftGain.ForeColor = System.Drawing.Color.Black;
            this.decLeftGain.Location = new System.Drawing.Point(288, 82);
            this.decLeftGain.Margin = new System.Windows.Forms.Padding(4);
            this.decLeftGain.Name = "decLeftGain";
            this.decLeftGain.Size = new System.Drawing.Size(41, 37);
            this.decLeftGain.TabIndex = 125;
            this.decLeftGain.Text = "-";
            this.decLeftGain.UseVisualStyleBackColor = true;
            this.decLeftGain.Click += new System.EventHandler(this.decLeftGain_Click);
            this.decLeftGain.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decLeftGain.MouseHover += new System.EventHandler(this.decLeftGain_MouseHover);
            // 
            // leftGainLabel
            // 
            this.leftGainLabel.AutoSize = true;
            this.leftGainLabel.Location = new System.Drawing.Point(197, 100);
            this.leftGainLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.leftGainLabel.Name = "leftGainLabel";
            this.leftGainLabel.Size = new System.Drawing.Size(24, 17);
            this.leftGainLabel.TabIndex = 124;
            this.leftGainLabel.Text = "50";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(172, 82);
            this.label26.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(58, 17);
            this.label26.TabIndex = 122;
            this.label26.Text = "left gain";
            // 
            // incUpGain
            // 
            this.incUpGain.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.incUpGain.ForeColor = System.Drawing.Color.Black;
            this.incUpGain.Location = new System.Drawing.Point(460, 55);
            this.incUpGain.Margin = new System.Windows.Forms.Padding(4);
            this.incUpGain.Name = "incUpGain";
            this.incUpGain.Size = new System.Drawing.Size(41, 37);
            this.incUpGain.TabIndex = 121;
            this.incUpGain.Text = "+";
            this.incUpGain.UseVisualStyleBackColor = true;
            this.incUpGain.Click += new System.EventHandler(this.incUpGain_Click);
            this.incUpGain.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.incUpGain.MouseHover += new System.EventHandler(this.incUpGain_MouseHover);
            // 
            // decUpGain
            // 
            this.decUpGain.Cursor = System.Windows.Forms.Cursors.Hand;
            this.decUpGain.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decUpGain.ForeColor = System.Drawing.Color.Black;
            this.decUpGain.Location = new System.Drawing.Point(460, 100);
            this.decUpGain.Margin = new System.Windows.Forms.Padding(4);
            this.decUpGain.Name = "decUpGain";
            this.decUpGain.Size = new System.Drawing.Size(41, 37);
            this.decUpGain.TabIndex = 120;
            this.decUpGain.Text = "-";
            this.decUpGain.UseVisualStyleBackColor = true;
            this.decUpGain.Click += new System.EventHandler(this.decUpGain_Click);
            this.decUpGain.MouseLeave += new System.EventHandler(this.stop_ClickTimer);
            this.decUpGain.MouseHover += new System.EventHandler(this.decUpGain_MouseHover);
            // 
            // upGainLabel
            // 
            this.upGainLabel.AutoSize = true;
            this.upGainLabel.Location = new System.Drawing.Point(509, 103);
            this.upGainLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.upGainLabel.Name = "upGainLabel";
            this.upGainLabel.Size = new System.Drawing.Size(24, 17);
            this.upGainLabel.TabIndex = 119;
            this.upGainLabel.Text = "50";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(508, 86);
            this.label23.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(55, 17);
            this.label23.TabIndex = 117;
            this.label23.Text = "up gain";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(261, 334);
            this.label22.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(92, 17);
            this.label22.TabIndex = 114;
            this.label22.Text = "Down Sensor";
            // 
            // downSensorLabel
            // 
            this.downSensorLabel.AutoSize = true;
            this.downSensorLabel.Location = new System.Drawing.Point(359, 334);
            this.downSensorLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.downSensorLabel.Name = "downSensorLabel";
            this.downSensorLabel.Size = new System.Drawing.Size(16, 17);
            this.downSensorLabel.TabIndex = 113;
            this.downSensorLabel.Text = "0";
            this.downSensorLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(120, 162);
            this.label19.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(81, 17);
            this.label19.TabIndex = 112;
            this.label19.Text = "Left Sensor";
            // 
            // leftSensorLabel
            // 
            this.leftSensorLabel.AutoSize = true;
            this.leftSensorLabel.Location = new System.Drawing.Point(162, 183);
            this.leftSensorLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.leftSensorLabel.Name = "leftSensorLabel";
            this.leftSensorLabel.Size = new System.Drawing.Size(16, 17);
            this.leftSensorLabel.TabIndex = 111;
            this.leftSensorLabel.Text = "0";
            this.leftSensorLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(562, 162);
            this.label17.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(90, 17);
            this.label17.TabIndex = 110;
            this.label17.Text = "Right Sensor";
            // 
            // rightSensorLabel
            // 
            this.rightSensorLabel.AutoSize = true;
            this.rightSensorLabel.Location = new System.Drawing.Point(567, 183);
            this.rightSensorLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.rightSensorLabel.Name = "rightSensorLabel";
            this.rightSensorLabel.Size = new System.Drawing.Size(16, 17);
            this.rightSensorLabel.TabIndex = 109;
            this.rightSensorLabel.Text = "0";
            this.rightSensorLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(280, 16);
            this.label10.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(75, 17);
            this.label10.TabIndex = 108;
            this.label10.Text = "Up Sensor";
            // 
            // upSensorLabel
            // 
            this.upSensorLabel.AutoSize = true;
            this.upSensorLabel.Location = new System.Drawing.Point(359, 16);
            this.upSensorLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.upSensorLabel.Name = "upSensorLabel";
            this.upSensorLabel.Size = new System.Drawing.Size(16, 17);
            this.upSensorLabel.TabIndex = 107;
            this.upSensorLabel.Text = "0";
            this.upSensorLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // rightPanel
            // 
            this.rightPanel.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.rightPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.rightPanel.Location = new System.Drawing.Point(404, 164);
            this.rightPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.rightPanel.Name = "rightPanel";
            this.rightPanel.Size = new System.Drawing.Size(142, 40);
            this.rightPanel.TabIndex = 94;
            // 
            // leftPanel
            // 
            this.leftPanel.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.leftPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.leftPanel.Location = new System.Drawing.Point(213, 164);
            this.leftPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.leftPanel.Name = "leftPanel";
            this.leftPanel.Size = new System.Drawing.Size(142, 40);
            this.leftPanel.TabIndex = 93;
            // 
            // downPanel
            // 
            this.downPanel.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.downPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.downPanel.Location = new System.Drawing.Point(357, 205);
            this.downPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.downPanel.Name = "downPanel";
            this.downPanel.Size = new System.Drawing.Size(45, 128);
            this.downPanel.TabIndex = 92;
            // 
            // upPanel
            // 
            this.upPanel.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.upPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.upPanel.Location = new System.Drawing.Point(357, 34);
            this.upPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.upPanel.Name = "upPanel";
            this.upPanel.Size = new System.Drawing.Size(45, 128);
            this.upPanel.TabIndex = 91;
            // 
            // leftGainBar
            // 
            this.leftGainBar.LargeChange = 10;
            this.leftGainBar.Location = new System.Drawing.Point(226, 126);
            this.leftGainBar.Margin = new System.Windows.Forms.Padding(4);
            this.leftGainBar.Maximum = 100;
            this.leftGainBar.Name = "leftGainBar";
            this.leftGainBar.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.leftGainBar.Size = new System.Drawing.Size(117, 56);
            this.leftGainBar.TabIndex = 123;
            this.leftGainBar.TickFrequency = 10;
            this.leftGainBar.Value = 50;
            this.leftGainBar.Scroll += new System.EventHandler(this.leftGainBar_Scroll);
            // 
            // downGainBar
            // 
            this.downGainBar.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.downGainBar.LargeChange = 10;
            this.downGainBar.Location = new System.Drawing.Point(311, 214);
            this.downGainBar.Margin = new System.Windows.Forms.Padding(4);
            this.downGainBar.Maximum = 100;
            this.downGainBar.Name = "downGainBar";
            this.downGainBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.downGainBar.Size = new System.Drawing.Size(56, 106);
            this.downGainBar.TabIndex = 128;
            this.downGainBar.TickFrequency = 10;
            this.downGainBar.Value = 50;
            this.downGainBar.Scroll += new System.EventHandler(this.downGainBar_Scroll);
            // 
            // upGainBar
            // 
            this.upGainBar.LargeChange = 10;
            this.upGainBar.Location = new System.Drawing.Point(387, 46);
            this.upGainBar.Margin = new System.Windows.Forms.Padding(4);
            this.upGainBar.Maximum = 100;
            this.upGainBar.Name = "upGainBar";
            this.upGainBar.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.upGainBar.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.upGainBar.RightToLeftLayout = true;
            this.upGainBar.Size = new System.Drawing.Size(56, 106);
            this.upGainBar.TabIndex = 118;
            this.upGainBar.TickFrequency = 10;
            this.upGainBar.Value = 50;
            this.upGainBar.Scroll += new System.EventHandler(this.upGainBar_Scroll);
            // 
            // IRTab
            // 
            this.IRTab.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.IRTab.Controls.Add(this.label1);
            this.IRTab.Controls.Add(this.irTimeoutBox);
            this.IRTab.Controls.Add(this.irCommandBox);
            this.IRTab.Controls.Add(this.deleteIRButton);
            this.IRTab.Controls.Add(this.label31);
            this.IRTab.Controls.Add(this.playIRButton);
            this.IRTab.Controls.Add(this.recordIRButton);
            this.IRTab.Controls.Add(this.label24);
            this.IRTab.Location = new System.Drawing.Point(4, 25);
            this.IRTab.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.IRTab.Name = "IRTab";
            this.IRTab.Size = new System.Drawing.Size(780, 361);
            this.IRTab.TabIndex = 6;
            this.IRTab.Text = "IR-Remote";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(193, 247);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(288, 17);
            this.label1.TabIndex = 146;
            this.label1.Text = "IR-Code timeout for recording (milliseconds):";
            // 
            // irTimeoutBox
            // 
            this.irTimeoutBox.Location = new System.Drawing.Point(497, 245);
            this.irTimeoutBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.irTimeoutBox.Name = "irTimeoutBox";
            this.irTimeoutBox.Size = new System.Drawing.Size(110, 22);
            this.irTimeoutBox.TabIndex = 145;
            this.irTimeoutBox.Text = "10";
            // 
            // irCommandBox
            // 
            this.irCommandBox.FormattingEnabled = true;
            this.irCommandBox.Location = new System.Drawing.Point(196, 117);
            this.irCommandBox.Margin = new System.Windows.Forms.Padding(4);
            this.irCommandBox.Name = "irCommandBox";
            this.irCommandBox.Size = new System.Drawing.Size(411, 24);
            this.irCommandBox.TabIndex = 144;
            // 
            // deleteIRButton
            // 
            this.deleteIRButton.Enabled = false;
            this.deleteIRButton.ForeColor = System.Drawing.Color.Black;
            this.deleteIRButton.Location = new System.Drawing.Point(420, 167);
            this.deleteIRButton.Margin = new System.Windows.Forms.Padding(4);
            this.deleteIRButton.Name = "deleteIRButton";
            this.deleteIRButton.Size = new System.Drawing.Size(187, 40);
            this.deleteIRButton.TabIndex = 143;
            this.deleteIRButton.Text = "Delete this command";
            this.deleteIRButton.UseVisualStyleBackColor = true;
            this.deleteIRButton.Click += new System.EventHandler(this.deleteIRButton_Click);
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Location = new System.Drawing.Point(36, 36);
            this.label31.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(236, 17);
            this.label31.TabIndex = 141;
            this.label31.Text = "Manage infrared remote commands:";
            // 
            // playIRButton
            // 
            this.playIRButton.Enabled = false;
            this.playIRButton.ForeColor = System.Drawing.Color.Black;
            this.playIRButton.Location = new System.Drawing.Point(196, 167);
            this.playIRButton.Margin = new System.Windows.Forms.Padding(4);
            this.playIRButton.Name = "playIRButton";
            this.playIRButton.Size = new System.Drawing.Size(189, 40);
            this.playIRButton.TabIndex = 119;
            this.playIRButton.Text = "Play this command ";
            this.playIRButton.UseVisualStyleBackColor = true;
            this.playIRButton.Click += new System.EventHandler(this.playIRButton_Click);
            // 
            // recordIRButton
            // 
            this.recordIRButton.Enabled = false;
            this.recordIRButton.ForeColor = System.Drawing.Color.Black;
            this.recordIRButton.Location = new System.Drawing.Point(196, 280);
            this.recordIRButton.Margin = new System.Windows.Forms.Padding(4);
            this.recordIRButton.Name = "recordIRButton";
            this.recordIRButton.Size = new System.Drawing.Size(411, 40);
            this.recordIRButton.TabIndex = 116;
            this.recordIRButton.Text = "Record new command now !";
            this.recordIRButton.UseVisualStyleBackColor = true;
            this.recordIRButton.Click += new System.EventHandler(this.recordIRButton_Click);
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(193, 87);
            this.label24.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(365, 17);
            this.label24.TabIndex = 114;
            this.label24.Text = "Infrared command name (enter new name for recording):";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.activityLogTextbox);
            this.groupBox1.Location = new System.Drawing.Point(53, 562);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox1.Size = new System.Drawing.Size(553, 153);
            this.groupBox1.TabIndex = 74;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Activity-Log";
            // 
            // activityLogTextbox
            // 
            this.activityLogTextbox.ForeColor = System.Drawing.SystemColors.MenuText;
            this.activityLogTextbox.HideSelection = false;
            this.activityLogTextbox.Location = new System.Drawing.Point(28, 25);
            this.activityLogTextbox.Margin = new System.Windows.Forms.Padding(4);
            this.activityLogTextbox.Name = "activityLogTextbox";
            this.activityLogTextbox.ReadOnly = true;
            this.activityLogTextbox.Size = new System.Drawing.Size(501, 110);
            this.activityLogTextbox.TabIndex = 63;
            this.activityLogTextbox.Text = "";
            // 
            // slotNames
            // 
            this.slotNames.CausesValidation = false;
            this.slotNames.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.slotNames.ForeColor = System.Drawing.SystemColors.MenuHighlight;
            this.slotNames.Location = new System.Drawing.Point(339, 512);
            this.slotNames.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.slotNames.Name = "slotNames";
            this.slotNames.Size = new System.Drawing.Size(206, 28);
            this.slotNames.TabIndex = 76;
            this.slotNames.Text = "<choose>";
            this.slotNames.SelectedIndexChanged += new System.EventHandler(this.slotNames_SelectedIndexChanged);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileMenu,
            this.fLipMouseToolStripMenuItem,
            this.aboutToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(5, 2, 0, 2);
            this.menuStrip1.Size = new System.Drawing.Size(885, 28);
            this.menuStrip1.TabIndex = 77;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileMenu
            // 
            this.fileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToFileMenuItem,
            this.loadFromFileMenuItem,
            this.exitMenuItem});
            this.fileMenu.Name = "fileMenu";
            this.fileMenu.Size = new System.Drawing.Size(44, 24);
            this.fileMenu.Text = "File";
            // 
            // saveToFileMenuItem
            // 
            this.saveToFileMenuItem.Name = "saveToFileMenuItem";
            this.saveToFileMenuItem.Size = new System.Drawing.Size(168, 24);
            this.saveToFileMenuItem.Text = "Save Settings";
            this.saveToFileMenuItem.Click += new System.EventHandler(this.saveToFileMenuItem_Click);
            // 
            // loadFromFileMenuItem
            // 
            this.loadFromFileMenuItem.Name = "loadFromFileMenuItem";
            this.loadFromFileMenuItem.Size = new System.Drawing.Size(168, 24);
            this.loadFromFileMenuItem.Text = "Load Settings";
            this.loadFromFileMenuItem.Click += new System.EventHandler(this.loadFromFileMenuItem_Click);
            // 
            // exitMenuItem
            // 
            this.exitMenuItem.Name = "exitMenuItem";
            this.exitMenuItem.Size = new System.Drawing.Size(168, 24);
            this.exitMenuItem.Text = "Exit";
            this.exitMenuItem.Click += new System.EventHandler(this.exitMenuItem_Click);
            // 
            // fLipMouseToolStripMenuItem
            // 
            this.fLipMouseToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.storeSlotSettingsMenuItem,
            this.loadSlotSettingsMenuItem});
            this.fLipMouseToolStripMenuItem.Name = "fLipMouseToolStripMenuItem";
            this.fLipMouseToolStripMenuItem.Size = new System.Drawing.Size(92, 24);
            this.fLipMouseToolStripMenuItem.Text = "FLipMouse";
            // 
            // storeSlotSettingsMenuItem
            // 
            this.storeSlotSettingsMenuItem.Enabled = false;
            this.storeSlotSettingsMenuItem.Name = "storeSlotSettingsMenuItem";
            this.storeSlotSettingsMenuItem.Size = new System.Drawing.Size(276, 24);
            this.storeSlotSettingsMenuItem.Text = "Store Settings to FLipMouse";
            this.storeSlotSettingsMenuItem.Click += new System.EventHandler(this.storeSlotSettingsMenuItem_Click);
            // 
            // loadSlotSettingsMenuItem
            // 
            this.loadSlotSettingsMenuItem.Enabled = false;
            this.loadSlotSettingsMenuItem.Name = "loadSlotSettingsMenuItem";
            this.loadSlotSettingsMenuItem.Size = new System.Drawing.Size(276, 24);
            this.loadSlotSettingsMenuItem.Text = "Load Settings from FlipMouse";
            this.loadSlotSettingsMenuItem.Click += new System.EventHandler(this.loadSlotSettingsMenuItem_Click);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(62, 24);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // prevSlotButton
            // 
            this.prevSlotButton.Location = new System.Drawing.Point(186, 505);
            this.prevSlotButton.Margin = new System.Windows.Forms.Padding(4);
            this.prevSlotButton.Name = "prevSlotButton";
            this.prevSlotButton.Size = new System.Drawing.Size(143, 40);
            this.prevSlotButton.TabIndex = 79;
            this.prevSlotButton.Text = " < Previous Slot  ";
            this.prevSlotButton.UseVisualStyleBackColor = true;
            this.prevSlotButton.Click += new System.EventHandler(this.prevSlotButton_Click);
            // 
            // nextSlotButton
            // 
            this.nextSlotButton.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.nextSlotButton.Location = new System.Drawing.Point(553, 505);
            this.nextSlotButton.Margin = new System.Windows.Forms.Padding(4);
            this.nextSlotButton.Name = "nextSlotButton";
            this.nextSlotButton.Size = new System.Drawing.Size(145, 40);
            this.nextSlotButton.TabIndex = 80;
            this.nextSlotButton.Text = "Next  Slot >";
            this.nextSlotButton.UseVisualStyleBackColor = true;
            this.nextSlotButton.Click += new System.EventHandler(this.nextSlotButton_Click);
            // 
            // ApplyButton
            // 
            this.ApplyButton.Enabled = false;
            this.ApplyButton.ForeColor = System.Drawing.Color.Black;
            this.ApplyButton.Location = new System.Drawing.Point(601, 51);
            this.ApplyButton.Margin = new System.Windows.Forms.Padding(4);
            this.ApplyButton.Name = "ApplyButton";
            this.ApplyButton.Size = new System.Drawing.Size(107, 28);
            this.ApplyButton.TabIndex = 36;
            this.ApplyButton.Text = "Test settings";
            this.ApplyButton.UseVisualStyleBackColor = true;
            this.ApplyButton.Click += new System.EventHandler(this.ApplyButton_Click);
            // 
            // newSlotButton
            // 
            this.newSlotButton.ForeColor = System.Drawing.Color.Black;
            this.newSlotButton.Location = new System.Drawing.Point(51, 505);
            this.newSlotButton.Margin = new System.Windows.Forms.Padding(4);
            this.newSlotButton.Name = "newSlotButton";
            this.newSlotButton.Size = new System.Drawing.Size(126, 40);
            this.newSlotButton.TabIndex = 81;
            this.newSlotButton.Text = "New Slot";
            this.newSlotButton.UseVisualStyleBackColor = true;
            this.newSlotButton.Click += new System.EventHandler(this.newSlotButton_Click);
            // 
            // deleteSlotButton
            // 
            this.deleteSlotButton.ForeColor = System.Drawing.Color.Black;
            this.deleteSlotButton.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.deleteSlotButton.Location = new System.Drawing.Point(708, 505);
            this.deleteSlotButton.Margin = new System.Windows.Forms.Padding(4);
            this.deleteSlotButton.Name = "deleteSlotButton";
            this.deleteSlotButton.Size = new System.Drawing.Size(131, 40);
            this.deleteSlotButton.TabIndex = 82;
            this.deleteSlotButton.Text = "Delete Slot";
            this.deleteSlotButton.UseVisualStyleBackColor = true;
            this.deleteSlotButton.Click += new System.EventHandler(this.deleteSlotButton_Click);
            // 
            // upIndicatorPanel
            // 
            this.upIndicatorPanel.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.upIndicatorPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.upIndicatorPanel.Location = new System.Drawing.Point(90, 46);
            this.upIndicatorPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.upIndicatorPanel.Name = "upIndicatorPanel";
            this.upIndicatorPanel.Size = new System.Drawing.Size(34, 24);
            this.upIndicatorPanel.TabIndex = 123;
            // 
            // downIndicatorPanel
            // 
            this.downIndicatorPanel.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.downIndicatorPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.downIndicatorPanel.Location = new System.Drawing.Point(90, 99);
            this.downIndicatorPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.downIndicatorPanel.Name = "downIndicatorPanel";
            this.downIndicatorPanel.Size = new System.Drawing.Size(34, 24);
            this.downIndicatorPanel.TabIndex = 124;
            // 
            // rightIndicatorPanel
            // 
            this.rightIndicatorPanel.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.rightIndicatorPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.rightIndicatorPanel.Location = new System.Drawing.Point(126, 73);
            this.rightIndicatorPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.rightIndicatorPanel.Name = "rightIndicatorPanel";
            this.rightIndicatorPanel.Size = new System.Drawing.Size(34, 24);
            this.rightIndicatorPanel.TabIndex = 125;
            // 
            // leftIndicatorPanel
            // 
            this.leftIndicatorPanel.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.leftIndicatorPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.leftIndicatorPanel.Location = new System.Drawing.Point(53, 73);
            this.leftIndicatorPanel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.leftIndicatorPanel.Name = "leftIndicatorPanel";
            this.leftIndicatorPanel.Size = new System.Drawing.Size(34, 24);
            this.leftIndicatorPanel.TabIndex = 126;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label39);
            this.groupBox2.Controls.Add(this.label38);
            this.groupBox2.Controls.Add(this.label37);
            this.groupBox2.Controls.Add(this.label33);
            this.groupBox2.Controls.Add(this.leftIndicatorPanel);
            this.groupBox2.Controls.Add(this.upIndicatorPanel);
            this.groupBox2.Controls.Add(this.rightIndicatorPanel);
            this.groupBox2.Controls.Add(this.downIndicatorPanel);
            this.groupBox2.Location = new System.Drawing.Point(612, 561);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox2.Size = new System.Drawing.Size(222, 154);
            this.groupBox2.TabIndex = 75;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Stick-Direction";
            // 
            // label39
            // 
            this.label39.AutoSize = true;
            this.label39.Location = new System.Drawing.Point(23, 77);
            this.label39.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(27, 17);
            this.label39.TabIndex = 129;
            this.label39.Text = "left";
            // 
            // label38
            // 
            this.label38.AutoSize = true;
            this.label38.Location = new System.Drawing.Point(164, 77);
            this.label38.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(36, 17);
            this.label38.TabIndex = 128;
            this.label38.Text = "right";
            // 
            // label37
            // 
            this.label37.AutoSize = true;
            this.label37.Location = new System.Drawing.Point(87, 125);
            this.label37.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(41, 17);
            this.label37.TabIndex = 127;
            this.label37.Text = "down";
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(95, 29);
            this.label33.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(24, 17);
            this.label33.TabIndex = 75;
            this.label33.Text = "up";
            // 
            // StoreButton
            // 
            this.StoreButton.Enabled = false;
            this.StoreButton.ForeColor = System.Drawing.Color.Black;
            this.StoreButton.Location = new System.Drawing.Point(716, 51);
            this.StoreButton.Margin = new System.Windows.Forms.Padding(4);
            this.StoreButton.Name = "StoreButton";
            this.StoreButton.Size = new System.Drawing.Size(118, 28);
            this.StoreButton.TabIndex = 83;
            this.StoreButton.Text = "Store settings";
            this.StoreButton.UseVisualStyleBackColor = true;
            this.StoreButton.Click += new System.EventHandler(this.StoreButton_Click);
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(172, 13);
            this.label32.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(251, 17);
            this.label32.TabIndex = 178;
            this.label32.Text = "Select StrongPuff combination actions:";
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.Location = new System.Drawing.Point(175, 13);
            this.label35.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(246, 17);
            this.label35.TabIndex = 206;
            this.label35.Text = "Select StrongSip combination actions:";
            // 
            // label41
            // 
            this.label41.AutoSize = true;
            this.label41.Location = new System.Drawing.Point(167, 13);
            this.label41.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label41.Name = "label41";
            this.label41.Size = new System.Drawing.Size(181, 17);
            this.label41.TabIndex = 179;
            this.label41.Text = "Select Sip and Puff actions:";
            // 
            // FLipMouseGUI
            // 
            this.AccessibleDescription = "FlipMouseGUI";
            this.AccessibleName = "FlipMouseGUI";
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(885, 732);
            this.Controls.Add(this.StoreButton);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.deleteSlotButton);
            this.Controls.Add(this.newSlotButton);
            this.Controls.Add(this.nextSlotButton);
            this.Controls.Add(this.prevSlotButton);
            this.Controls.Add(this.slotNames);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.tabControl);
            this.Controls.Add(this.portStatus);
            this.Controls.Add(this.ApplyButton);
            this.Controls.Add(this.disconnectComButton);
            this.Controls.Add(this.connectComButton);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.portComboBox);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "FLipMouseGUI";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FLipMouse Settings Manager V";
            this.Load += new System.EventHandler(this.LipmouseGUI_Load);
            this.tabControl.ResumeLayout(false);
            this.LipmouseTab.ResumeLayout(false);
            this.LipmouseTab.PerformLayout();
            this.singlePanel.ResumeLayout(false);
            this.singlePanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.deadzoneBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.speedBar)).EndInit();
            this.splitPanel.ResumeLayout(false);
            this.splitPanel.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.deadzoneYBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.speedYBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.deadzoneXBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.speedXBar)).EndInit();
            this.AlternativeTab.ResumeLayout(false);
            this.AlternativeTab.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RightNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.LeftNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.DownNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpNumericParameter)).EndInit();
            this.SipPuffLevelsTab.ResumeLayout(false);
            this.SipPuffLevelsTab.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.strongSipThresholdBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.strongPuffThresholdBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.puffThresholdBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.sipThresholdBar)).EndInit();
            this.SipPuffTab.ResumeLayout(false);
            this.sipPuffSelections.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PuffNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SipNumericParameter)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipRightNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipLeftNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipDownNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongSipUpNumericParameter)).EndInit();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffRightNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffLeftNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffDownNumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.StrongPuffUpNumericParameter)).EndInit();
            this.ButtonsTab.ResumeLayout(false);
            this.ButtonsTab.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Button3NumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Button2NumericParameter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Button1NumericParameter)).EndInit();
            this.RawValueTab.ResumeLayout(false);
            this.RawValueTab.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.rightGainBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.leftGainBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.downGainBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.upGainBar)).EndInit();
            this.IRTab.ResumeLayout(false);
            this.IRTab.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.ComboBox portComboBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button connectComButton;
        private System.Windows.Forms.Button disconnectComButton;
        private System.Windows.Forms.Label portStatus;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage LipmouseTab;
        private System.Windows.Forms.TabPage ButtonsTab;
        private System.Windows.Forms.Button calButton;
        private System.Windows.Forms.TabPage SipPuffTab;
        private System.Windows.Forms.ComboBox Button3ComboBox;
        private System.Windows.Forms.ComboBox Button2ComboBox;
        private System.Windows.Forms.ComboBox Button1ComboBox;
        private System.Windows.Forms.NumericUpDown Button3NumericParameter;
        private System.Windows.Forms.NumericUpDown Button2NumericParameter;
        private System.Windows.Forms.NumericUpDown Button1NumericParameter;
        private System.Windows.Forms.Label Button3Label;
        private System.Windows.Forms.TextBox Button3ParameterText;
        private System.Windows.Forms.Label Button2Label;
        private System.Windows.Forms.TextBox Button2ParameterText;
        private System.Windows.Forms.Label Button1Label;
        private System.Windows.Forms.TextBox Button1ParameterText;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox Button3FunctionBox;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox Button2FunctionBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox Button1FunctionBox;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RichTextBox activityLogTextbox;
        private System.Windows.Forms.TabPage AlternativeTab;
        private System.Windows.Forms.ComboBox RightComboBox;
        private System.Windows.Forms.NumericUpDown RightNumericParameter;
        private System.Windows.Forms.Label RightLabel;
        private System.Windows.Forms.TextBox RightParameterText;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.ComboBox RightFunctionMenu;
        private System.Windows.Forms.ComboBox LeftComboBox;
        private System.Windows.Forms.ComboBox DownComboBox;
        private System.Windows.Forms.ComboBox UpComboBox;
        private System.Windows.Forms.NumericUpDown LeftNumericParameter;
        private System.Windows.Forms.NumericUpDown DownNumericParameter;
        private System.Windows.Forms.NumericUpDown UpNumericParameter;
        private System.Windows.Forms.Label LeftLabel;
        private System.Windows.Forms.TextBox LeftParameterText;
        private System.Windows.Forms.Label DownLabel;
        private System.Windows.Forms.TextBox DownParameterText;
        private System.Windows.Forms.Label UpLabel;
        private System.Windows.Forms.TextBox UpParameterText;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.ComboBox LeftFunctionMenu;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.ComboBox DownFunctionMenu;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ComboBox UpFunctionMenu;
        private System.Windows.Forms.TabPage RawValueTab;
        private System.Windows.Forms.Panel rightPanel;
        private System.Windows.Forms.Panel leftPanel;
        private System.Windows.Forms.Panel downPanel;
        private System.Windows.Forms.Panel upPanel;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label downSensorLabel;
        private System.Windows.Forms.Label leftSensorLabel;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label rightSensorLabel;
        private System.Windows.Forms.Label upSensorLabel;
        private System.Windows.Forms.ComboBox slotNames;
        private System.Windows.Forms.CheckBox splitXYBox;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileMenu;
        private System.Windows.Forms.ToolStripMenuItem saveToFileMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadFromFileMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitMenuItem;
        private System.Windows.Forms.Button incUpGain;
        private System.Windows.Forms.Button decUpGain;
        private System.Windows.Forms.Label upGainLabel;
        private System.Windows.Forms.TrackBar upGainBar;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Button incLeftGain;
        private System.Windows.Forms.Button decLeftGain;
        private System.Windows.Forms.Label leftGainLabel;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TrackBar leftGainBar;
        private System.Windows.Forms.Button incRightGain;
        private System.Windows.Forms.Button decRightGain;
        private System.Windows.Forms.Label rightGainLabel;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.TrackBar rightGainBar;
        private System.Windows.Forms.Button incDownGain;
        private System.Windows.Forms.Button decDownGain;
        private System.Windows.Forms.Label downGainLabel;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.TrackBar downGainBar;
        private System.Windows.Forms.Button clearButton1;
        private System.Windows.Forms.Button clearButtonUp;
        private System.Windows.Forms.Button clearButton3;
        private System.Windows.Forms.Button clearButton2;
        private System.Windows.Forms.Button clearButtonRight;
        private System.Windows.Forms.Button clearButtonLeft;
        private System.Windows.Forms.Button clearButtonDown;
        private System.Windows.Forms.Button prevSlotButton;
        private System.Windows.Forms.Button nextSlotButton;
        private System.Windows.Forms.Button ApplyButton;
        private System.Windows.Forms.Button newSlotButton;
        private System.Windows.Forms.Button deleteSlotButton;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.ToolStripMenuItem fLipMouseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem storeSlotSettingsMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadSlotSettingsMenuItem;
        private System.Windows.Forms.TabPage SipPuffLevelsTab;
        private System.Windows.Forms.Button incTimeThreshld;
        private System.Windows.Forms.Button decTimeThreshold;
        private System.Windows.Forms.Button incSipThreshold;
        private System.Windows.Forms.Button decSipThreshold;
        private System.Windows.Forms.Button incPuffThreshold;
        private System.Windows.Forms.Button decPuffThreshold;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label strongPuffThresholdLabel;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TrackBar strongPuffThresholdBar;
        private System.Windows.Forms.Panel pressureDrawPanel;
        private System.Windows.Forms.Label pressureLabel;
        private System.Windows.Forms.Label puffThresholdLabel;
        private System.Windows.Forms.TrackBar puffThresholdBar;
        private System.Windows.Forms.Label thresholdLabelForPuff;
        private System.Windows.Forms.Label sipThresholdLabel;
        private System.Windows.Forms.TrackBar sipThresholdBar;
        private System.Windows.Forms.Label thresholdLabelForSip;
        private System.Windows.Forms.Button decHoldThreshold;
        private System.Windows.Forms.Button incHoldThreshold;
        private System.Windows.Forms.Label stongSipThresholdLabel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar strongSipThresholdBar;
        private System.Windows.Forms.TabPage IRTab;
        private System.Windows.Forms.Button playIRButton;
        private System.Windows.Forms.Button recordIRButton;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Panel splitPanel;
        private System.Windows.Forms.Button incDeadzoneY;
        private System.Windows.Forms.Button decDeadzoneY;
        private System.Windows.Forms.Button incSpeedY;
        private System.Windows.Forms.Button decSpeedY;
        private System.Windows.Forms.Button incSpeedX;
        private System.Windows.Forms.Button decSpeedX;
        private System.Windows.Forms.Button incDeadzoneX;
        private System.Windows.Forms.Button decDeadzoneX;
        private System.Windows.Forms.Label DeadzoneYNameLabel;
        private System.Windows.Forms.Label SpeedYNameLabel;
        private System.Windows.Forms.Label speedYLabel;
        private System.Windows.Forms.Label deadzoneYLabel;
        private System.Windows.Forms.TrackBar deadzoneYBar;
        private System.Windows.Forms.TrackBar speedYBar;
        private System.Windows.Forms.Label deadzoneXLabel;
        private System.Windows.Forms.Label DeadzoneXNameLabel;
        private System.Windows.Forms.TrackBar deadzoneXBar;
        private System.Windows.Forms.Label speedXLabel;
        private System.Windows.Forms.TrackBar speedXBar;
        private System.Windows.Forms.Label SpeedXNameLabel;
        private System.Windows.Forms.Panel singlePanel;
        private System.Windows.Forms.Button incSpeed;
        private System.Windows.Forms.Button decSpeed;
        private System.Windows.Forms.Button incDeadzone;
        private System.Windows.Forms.Button decDeadzone;
        private System.Windows.Forms.Label deadzoneLabel;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.TrackBar deadzoneBar;
        private System.Windows.Forms.Label speedLabel;
        private System.Windows.Forms.TrackBar speedBar;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.Panel puffLevelIndicatorPanel;
        private System.Windows.Forms.Panel holdLevelIndicatorPanel;
        private System.Windows.Forms.Panel sipLevelIndicatorPanel;
        private System.Windows.Forms.Panel specialLevelIndicatorPanel;
        private System.Windows.Forms.Panel leftIndicatorPanel;
        private System.Windows.Forms.Panel rightIndicatorPanel;
        private System.Windows.Forms.Panel downIndicatorPanel;
        private System.Windows.Forms.Panel upIndicatorPanel;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label39;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.Button deleteIRButton;
        private System.Windows.Forms.ComboBox irCommandBox;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.ComboBox joyModeBox;
        private System.Windows.Forms.RadioButton selectJoystick;
        private System.Windows.Forms.RadioButton selectAlternative;
        private System.Windows.Forms.RadioButton selectStick;
        private System.Windows.Forms.Label joyModeLabel;
        private System.Windows.Forms.Button StoreButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox irTimeoutBox;
        private System.Windows.Forms.TabControl sipPuffSelections;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Button clearButtonStrongPuff;
        private System.Windows.Forms.Button clearButtonPuff;
        private System.Windows.Forms.Button clearButtonStrongSip;
        private System.Windows.Forms.Button clearButtonSip;
        private System.Windows.Forms.ComboBox StrongPuffComboBox;
        private System.Windows.Forms.Label SpecialPuffParameterLabel;
        private System.Windows.Forms.NumericUpDown StrongPuffNumericParameter;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.ComboBox StrongPuffFunctionMenu;
        private System.Windows.Forms.ComboBox StrongSipComboBox;
        private System.Windows.Forms.Label SpecialSipParameterLabel;
        private System.Windows.Forms.NumericUpDown StrongSipNumericParameter;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox StrongSipFunctionMenu;
        private System.Windows.Forms.ComboBox PuffComboBox;
        private System.Windows.Forms.ComboBox SipComboBox;
        private System.Windows.Forms.Label PuffParameterLabel;
        private System.Windows.Forms.Label SipParameterLabel;
        private System.Windows.Forms.NumericUpDown PuffNumericParameter;
        private System.Windows.Forms.NumericUpDown SipNumericParameter;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.ComboBox PuffFunctionMenu;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox SipFunctionMenu;
        private System.Windows.Forms.TextBox SipParameterText;
        private System.Windows.Forms.TextBox StrongSipParameterText;
        private System.Windows.Forms.TextBox PuffParameterText;
        private System.Windows.Forms.TextBox StrongPuffParameterText;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Button clearButtonStrongSipRight;
        private System.Windows.Forms.ComboBox StrongSipRightComboBox;
        private System.Windows.Forms.NumericUpDown StrongSipRightNumericParameter;
        private System.Windows.Forms.Label StrongSipRightLabel;
        private System.Windows.Forms.TextBox StrongSipRightParameterText;
        private System.Windows.Forms.Label label48;
        private System.Windows.Forms.ComboBox StrongSipRightFunctionBox;
        private System.Windows.Forms.Button clearButtonStrongSipLeft;
        private System.Windows.Forms.Button clearButtonStrongSipDown;
        private System.Windows.Forms.Button clearButtonStrongSipUp;
        private System.Windows.Forms.ComboBox StrongSipLeftComboBox;
        private System.Windows.Forms.ComboBox StrongSipDownComboBox;
        private System.Windows.Forms.ComboBox StrongSipUpComboBox;
        private System.Windows.Forms.NumericUpDown StrongSipLeftNumericParameter;
        private System.Windows.Forms.NumericUpDown StrongSipDownNumericParameter;
        private System.Windows.Forms.NumericUpDown StrongSipUpNumericParameter;
        private System.Windows.Forms.Label StrongSipLeftLabel;
        private System.Windows.Forms.TextBox StrongSipLeftParameterText;
        private System.Windows.Forms.Label StrongSipDownLabel;
        private System.Windows.Forms.TextBox StrongSipDownParameterText;
        private System.Windows.Forms.Label StrongSipUpLabel;
        private System.Windows.Forms.TextBox StrongSipUpParameterText;
        private System.Windows.Forms.Label label52;
        private System.Windows.Forms.ComboBox StrongSipLeftFunctionBox;
        private System.Windows.Forms.Label label53;
        private System.Windows.Forms.ComboBox StrongSipDownFunctionBox;
        private System.Windows.Forms.Label label54;
        private System.Windows.Forms.ComboBox StrongSipUpFunctionBox;
        private System.Windows.Forms.Button clearButtonStrongPuffRight;
        private System.Windows.Forms.ComboBox StrongPuffRightComboBox;
        private System.Windows.Forms.NumericUpDown StrongPuffRightNumericParameter;
        private System.Windows.Forms.Label StrongPuffRightLabel;
        private System.Windows.Forms.TextBox StrongPuffRightParameterText;
        private System.Windows.Forms.Label label40;
        private System.Windows.Forms.ComboBox StrongPuffRightFunctionBox;
        private System.Windows.Forms.Button clearButtonStrongPuffLeft;
        private System.Windows.Forms.Button clearButtonStrongPuffDown;
        private System.Windows.Forms.Button clearButtonStrongPuffUp;
        private System.Windows.Forms.ComboBox StrongPuffLeftComboBox;
        private System.Windows.Forms.ComboBox StrongPuffDownComboBox;
        private System.Windows.Forms.ComboBox StrongPuffUpComboBox;
        private System.Windows.Forms.NumericUpDown StrongPuffLeftNumericParameter;
        private System.Windows.Forms.NumericUpDown StrongPuffDownNumericParameter;
        private System.Windows.Forms.NumericUpDown StrongPuffUpNumericParameter;
        private System.Windows.Forms.Label StrongPuffLeftLabel;
        private System.Windows.Forms.TextBox StrongPuffLeftParameterText;
        private System.Windows.Forms.Label StrongPuffDownLabel;
        private System.Windows.Forms.TextBox StrongPuffDownParameterText;
        private System.Windows.Forms.Label StrongPuffUpLabel;
        private System.Windows.Forms.TextBox StrongPuffUpParameterText;
        private System.Windows.Forms.Label label44;
        private System.Windows.Forms.ComboBox StrongPuffLeftFunctionBox;
        private System.Windows.Forms.Label label45;
        private System.Windows.Forms.ComboBox StrongPuffDownFunctionBox;
        private System.Windows.Forms.Label label46;
        private System.Windows.Forms.ComboBox StrongPuffUpFunctionBox;
        private System.Windows.Forms.Label label41;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.Label label32;
    }
}

