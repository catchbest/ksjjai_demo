namespace SimpleImageDisplaySample
{
    partial class Form1
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
            this.CameraIDTextBox = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.SearchButton = new System.Windows.Forms.Button();
            this.StartButton = new System.Windows.Forms.Button();
            this.StopButton = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.WidthNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.HeightNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.GaintextBox = new System.Windows.Forms.TextBox();
            this.ExposureNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.CheckBox_SAVE = new System.Windows.Forms.CheckBox();
            this.TextBox_ELAPSE_TIME = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.WidthNumericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.HeightNumericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ExposureNumericUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // CameraIDTextBox
            // 
            this.CameraIDTextBox.Enabled = false;
            this.CameraIDTextBox.Location = new System.Drawing.Point(6, 18);
            this.CameraIDTextBox.Multiline = true;
            this.CameraIDTextBox.Name = "CameraIDTextBox";
            this.CameraIDTextBox.Size = new System.Drawing.Size(441, 42);
            this.CameraIDTextBox.TabIndex = 0;
            this.toolTip1.SetToolTip(this.CameraIDTextBox, "ID text string for the first camera detected during Device Discovery");
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.SearchButton);
            this.groupBox1.Controls.Add(this.CameraIDTextBox);
            this.groupBox1.Location = new System.Drawing.Point(12, 11);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(488, 66);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "ID of the first camera found";
            // 
            // SearchButton
            // 
            this.SearchButton.Location = new System.Drawing.Point(453, 18);
            this.SearchButton.Name = "SearchButton";
            this.SearchButton.Size = new System.Drawing.Size(29, 21);
            this.SearchButton.TabIndex = 6;
            this.SearchButton.Text = "...";
            this.toolTip1.SetToolTip(this.SearchButton, "Search for cameras");
            this.SearchButton.UseVisualStyleBackColor = true;
            this.SearchButton.Click += new System.EventHandler(this.SearchButton_Click);
            // 
            // StartButton
            // 
            this.StartButton.Enabled = false;
            this.StartButton.Location = new System.Drawing.Point(272, 87);
            this.StartButton.Name = "StartButton";
            this.StartButton.Size = new System.Drawing.Size(75, 21);
            this.StartButton.TabIndex = 2;
            this.StartButton.Text = "Start";
            this.toolTip1.SetToolTip(this.StartButton, "Start Image Acquisition");
            this.StartButton.UseVisualStyleBackColor = true;
            this.StartButton.Click += new System.EventHandler(this.StartButton_Click);
            // 
            // StopButton
            // 
            this.StopButton.Enabled = false;
            this.StopButton.Location = new System.Drawing.Point(384, 87);
            this.StopButton.Name = "StopButton";
            this.StopButton.Size = new System.Drawing.Size(75, 21);
            this.StopButton.TabIndex = 3;
            this.StopButton.Text = "Stop";
            this.toolTip1.SetToolTip(this.StopButton, "Stop Image Acquisition");
            this.StopButton.UseVisualStyleBackColor = true;
            this.StopButton.Click += new System.EventHandler(this.StopButton_Click);
            // 
            // WidthNumericUpDown
            // 
            this.WidthNumericUpDown.Enabled = false;
            this.WidthNumericUpDown.Location = new System.Drawing.Point(92, 85);
            this.WidthNumericUpDown.Name = "WidthNumericUpDown";
            this.WidthNumericUpDown.Size = new System.Drawing.Size(120, 21);
            this.WidthNumericUpDown.TabIndex = 0;
            this.toolTip1.SetToolTip(this.WidthNumericUpDown, "Width");
            this.WidthNumericUpDown.ValueChanged += new System.EventHandler(this.WidthNumericUpDown_ValueChanged);
            // 
            // HeightNumericUpDown
            // 
            this.HeightNumericUpDown.Enabled = false;
            this.HeightNumericUpDown.Location = new System.Drawing.Point(92, 111);
            this.HeightNumericUpDown.Name = "HeightNumericUpDown";
            this.HeightNumericUpDown.Size = new System.Drawing.Size(120, 21);
            this.HeightNumericUpDown.TabIndex = 1;
            this.toolTip1.SetToolTip(this.HeightNumericUpDown, "Height");
            this.HeightNumericUpDown.ValueChanged += new System.EventHandler(this.HeightNumericUpDown_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(25, 87);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(17, 12);
            this.label1.TabIndex = 2;
            this.label1.Text = "宽";
            this.toolTip1.SetToolTip(this.label1, "Width");
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(25, 113);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "高";
            this.toolTip1.SetToolTip(this.label2, "Height");
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 168);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 8;
            this.label3.Text = "增益";
            // 
            // GaintextBox
            // 
            this.GaintextBox.Location = new System.Drawing.Point(92, 165);
            this.GaintextBox.Name = "GaintextBox";
            this.GaintextBox.Size = new System.Drawing.Size(120, 21);
            this.GaintextBox.TabIndex = 9;
            this.GaintextBox.TextChanged += new System.EventHandler(this.GaintextBox_TextChanged);
            // 
            // ExposureNumericUpDown
            // 
            this.ExposureNumericUpDown.Location = new System.Drawing.Point(92, 138);
            this.ExposureNumericUpDown.Name = "ExposureNumericUpDown";
            this.ExposureNumericUpDown.Size = new System.Drawing.Size(120, 21);
            this.ExposureNumericUpDown.TabIndex = 10;
            this.ExposureNumericUpDown.ValueChanged += new System.EventHandler(this.ExposureNumericUpDown_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(27, 146);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 12);
            this.label4.TabIndex = 11;
            this.label4.Text = "曝光";
            // 
            // CheckBox_SAVE
            // 
            this.CheckBox_SAVE.AutoSize = true;
            this.CheckBox_SAVE.Location = new System.Drawing.Point(291, 149);
            this.CheckBox_SAVE.Name = "CheckBox_SAVE";
            this.CheckBox_SAVE.Size = new System.Drawing.Size(48, 16);
            this.CheckBox_SAVE.TabIndex = 12;
            this.CheckBox_SAVE.Text = "Save";
            this.CheckBox_SAVE.UseVisualStyleBackColor = true;
            // 
            // TextBox_ELAPSE_TIME
            // 
            this.TextBox_ELAPSE_TIME.Location = new System.Drawing.Point(372, 147);
            this.TextBox_ELAPSE_TIME.Name = "TextBox_ELAPSE_TIME";
            this.TextBox_ELAPSE_TIME.Size = new System.Drawing.Size(100, 21);
            this.TextBox_ELAPSE_TIME.TabIndex = 13;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(512, 303);
            this.Controls.Add(this.TextBox_ELAPSE_TIME);
            this.Controls.Add(this.CheckBox_SAVE);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.ExposureNumericUpDown);
            this.Controls.Add(this.GaintextBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.StopButton);
            this.Controls.Add(this.HeightNumericUpDown);
            this.Controls.Add(this.WidthNumericUpDown);
            this.Controls.Add(this.StartButton);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "Form1";
            this.Text = "Asynchronous Image Recording Sample";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.WidthNumericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.HeightNumericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ExposureNumericUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox CameraIDTextBox;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button StartButton;
        private System.Windows.Forms.Button StopButton;
        private System.Windows.Forms.Button SearchButton;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.NumericUpDown WidthNumericUpDown;
        private System.Windows.Forms.NumericUpDown HeightNumericUpDown;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox GaintextBox;
        private System.Windows.Forms.NumericUpDown ExposureNumericUpDown;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox CheckBox_SAVE;
        private System.Windows.Forms.TextBox TextBox_ELAPSE_TIME;
    }
}

