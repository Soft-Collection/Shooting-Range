namespace ShootingRange
{
    partial class SettingsFrm
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
            this.tbAverageBufferSizeInFrames = new System.Windows.Forms.TrackBar();
            this.tbResizeSquare = new System.Windows.Forms.TrackBar();
            this.tbTolerance = new System.Windows.Forms.TrackBar();
            this.tbMedianSquare = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.tbMinimalDistanceBetweenTwoObjects = new System.Windows.Forms.TrackBar();
            this.label6 = new System.Windows.Forms.Label();
            this.tbMaximalAmountOfObjectsToFind = new System.Windows.Forms.TrackBar();
            this.cbWhatToShowType = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.txtAverageBufferSizeInFrames = new System.Windows.Forms.TextBox();
            this.txtResizeSquare = new System.Windows.Forms.TextBox();
            this.txtTolerance = new System.Windows.Forms.TextBox();
            this.txtMedianSquare = new System.Windows.Forms.TextBox();
            this.txtMinimalDistanceBetweenTwoObjects = new System.Windows.Forms.TextBox();
            this.txtMaximalAmountOfObjectsToFind = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.cbCameraPosition = new System.Windows.Forms.ComboBox();
            this.bDefaults = new System.Windows.Forms.Button();
            this.caCalibration = new ShootingRange.ucCalibration();
            this.timer = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.tbAverageBufferSizeInFrames)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbResizeSquare)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbTolerance)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbMedianSquare)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbMinimalDistanceBetweenTwoObjects)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbMaximalAmountOfObjectsToFind)).BeginInit();
            this.SuspendLayout();
            // 
            // tbAverageBufferSizeInFrames
            // 
            this.tbAverageBufferSizeInFrames.AutoSize = false;
            this.tbAverageBufferSizeInFrames.Location = new System.Drawing.Point(2, 25);
            this.tbAverageBufferSizeInFrames.Maximum = 100;
            this.tbAverageBufferSizeInFrames.Minimum = 1;
            this.tbAverageBufferSizeInFrames.Name = "tbAverageBufferSizeInFrames";
            this.tbAverageBufferSizeInFrames.Size = new System.Drawing.Size(323, 25);
            this.tbAverageBufferSizeInFrames.TabIndex = 0;
            this.tbAverageBufferSizeInFrames.Value = 1;
            this.tbAverageBufferSizeInFrames.Scroll += new System.EventHandler(this.tbAverageBufferSizeInFrames_Scroll);
            // 
            // tbResizeSquare
            // 
            this.tbResizeSquare.AutoSize = false;
            this.tbResizeSquare.Location = new System.Drawing.Point(2, 69);
            this.tbResizeSquare.Maximum = 20;
            this.tbResizeSquare.Minimum = 1;
            this.tbResizeSquare.Name = "tbResizeSquare";
            this.tbResizeSquare.Size = new System.Drawing.Size(323, 25);
            this.tbResizeSquare.TabIndex = 1;
            this.tbResizeSquare.Value = 1;
            this.tbResizeSquare.Scroll += new System.EventHandler(this.tbResizeSquare_Scroll);
            // 
            // tbTolerance
            // 
            this.tbTolerance.AutoSize = false;
            this.tbTolerance.Location = new System.Drawing.Point(2, 113);
            this.tbTolerance.Maximum = 255;
            this.tbTolerance.Minimum = 1;
            this.tbTolerance.Name = "tbTolerance";
            this.tbTolerance.Size = new System.Drawing.Size(323, 25);
            this.tbTolerance.TabIndex = 2;
            this.tbTolerance.TickFrequency = 10;
            this.tbTolerance.Value = 1;
            this.tbTolerance.Scroll += new System.EventHandler(this.tbTolerance_Scroll);
            // 
            // tbMedianSquare
            // 
            this.tbMedianSquare.AutoSize = false;
            this.tbMedianSquare.Location = new System.Drawing.Point(2, 157);
            this.tbMedianSquare.Maximum = 20;
            this.tbMedianSquare.Minimum = 1;
            this.tbMedianSquare.Name = "tbMedianSquare";
            this.tbMedianSquare.Size = new System.Drawing.Size(323, 25);
            this.tbMedianSquare.TabIndex = 3;
            this.tbMedianSquare.Value = 1;
            this.tbMedianSquare.Scroll += new System.EventHandler(this.tbMedianSquare_Scroll);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(150, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Average Buffer Size In Frames";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 53);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(76, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Resize Square";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 97);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(55, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Tolerance";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 141);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(79, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Median Square";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(9, 185);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(195, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Minimal Distance Between Two Objects";
            // 
            // tbMinimalDistanceBetweenTwoObjects
            // 
            this.tbMinimalDistanceBetweenTwoObjects.AutoSize = false;
            this.tbMinimalDistanceBetweenTwoObjects.Location = new System.Drawing.Point(2, 201);
            this.tbMinimalDistanceBetweenTwoObjects.Maximum = 200;
            this.tbMinimalDistanceBetweenTwoObjects.Minimum = 1;
            this.tbMinimalDistanceBetweenTwoObjects.Name = "tbMinimalDistanceBetweenTwoObjects";
            this.tbMinimalDistanceBetweenTwoObjects.Size = new System.Drawing.Size(323, 25);
            this.tbMinimalDistanceBetweenTwoObjects.TabIndex = 8;
            this.tbMinimalDistanceBetweenTwoObjects.TickFrequency = 10;
            this.tbMinimalDistanceBetweenTwoObjects.Value = 1;
            this.tbMinimalDistanceBetweenTwoObjects.Scroll += new System.EventHandler(this.tbMinimalDistanceBetweenTwoObjects_Scroll);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(9, 229);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(176, 13);
            this.label6.TabIndex = 11;
            this.label6.Text = "Maximal Amount Of Objects To Find";
            // 
            // tbMaximalAmountOfObjectsToFind
            // 
            this.tbMaximalAmountOfObjectsToFind.AutoSize = false;
            this.tbMaximalAmountOfObjectsToFind.Location = new System.Drawing.Point(2, 245);
            this.tbMaximalAmountOfObjectsToFind.Maximum = 200;
            this.tbMaximalAmountOfObjectsToFind.Minimum = 1;
            this.tbMaximalAmountOfObjectsToFind.Name = "tbMaximalAmountOfObjectsToFind";
            this.tbMaximalAmountOfObjectsToFind.Size = new System.Drawing.Size(323, 25);
            this.tbMaximalAmountOfObjectsToFind.TabIndex = 10;
            this.tbMaximalAmountOfObjectsToFind.TickFrequency = 10;
            this.tbMaximalAmountOfObjectsToFind.Value = 1;
            this.tbMaximalAmountOfObjectsToFind.Scroll += new System.EventHandler(this.tbMaximalAmountOfObjectsToFind_Scroll);
            // 
            // cbWhatToShowType
            // 
            this.cbWhatToShowType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbWhatToShowType.FormattingEnabled = true;
            this.cbWhatToShowType.Items.AddRange(new object[] {
            "RGB Frame",
            "VMD Frame",
            "Average Frame",
            "Difference Frame",
            "Tolerance Frame",
            "Median Frame"});
            this.cbWhatToShowType.Location = new System.Drawing.Point(11, 292);
            this.cbWhatToShowType.Name = "cbWhatToShowType";
            this.cbWhatToShowType.Size = new System.Drawing.Size(119, 21);
            this.cbWhatToShowType.TabIndex = 24;
            this.cbWhatToShowType.SelectedIndexChanged += new System.EventHandler(this.cbWhatToShowType_SelectedIndexChanged);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(9, 276);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(63, 13);
            this.label13.TabIndex = 25;
            this.label13.Text = "Frame Type";
            // 
            // txtAverageBufferSizeInFrames
            // 
            this.txtAverageBufferSizeInFrames.BackColor = System.Drawing.Color.White;
            this.txtAverageBufferSizeInFrames.Location = new System.Drawing.Point(331, 25);
            this.txtAverageBufferSizeInFrames.Name = "txtAverageBufferSizeInFrames";
            this.txtAverageBufferSizeInFrames.ReadOnly = true;
            this.txtAverageBufferSizeInFrames.Size = new System.Drawing.Size(45, 20);
            this.txtAverageBufferSizeInFrames.TabIndex = 28;
            // 
            // txtResizeSquare
            // 
            this.txtResizeSquare.BackColor = System.Drawing.Color.White;
            this.txtResizeSquare.Location = new System.Drawing.Point(331, 69);
            this.txtResizeSquare.Name = "txtResizeSquare";
            this.txtResizeSquare.ReadOnly = true;
            this.txtResizeSquare.Size = new System.Drawing.Size(45, 20);
            this.txtResizeSquare.TabIndex = 29;
            // 
            // txtTolerance
            // 
            this.txtTolerance.BackColor = System.Drawing.Color.White;
            this.txtTolerance.Location = new System.Drawing.Point(331, 113);
            this.txtTolerance.Name = "txtTolerance";
            this.txtTolerance.ReadOnly = true;
            this.txtTolerance.Size = new System.Drawing.Size(45, 20);
            this.txtTolerance.TabIndex = 30;
            // 
            // txtMedianSquare
            // 
            this.txtMedianSquare.BackColor = System.Drawing.Color.White;
            this.txtMedianSquare.Location = new System.Drawing.Point(331, 157);
            this.txtMedianSquare.Name = "txtMedianSquare";
            this.txtMedianSquare.ReadOnly = true;
            this.txtMedianSquare.Size = new System.Drawing.Size(45, 20);
            this.txtMedianSquare.TabIndex = 31;
            // 
            // txtMinimalDistanceBetweenTwoObjects
            // 
            this.txtMinimalDistanceBetweenTwoObjects.BackColor = System.Drawing.Color.White;
            this.txtMinimalDistanceBetweenTwoObjects.Location = new System.Drawing.Point(331, 201);
            this.txtMinimalDistanceBetweenTwoObjects.Name = "txtMinimalDistanceBetweenTwoObjects";
            this.txtMinimalDistanceBetweenTwoObjects.ReadOnly = true;
            this.txtMinimalDistanceBetweenTwoObjects.Size = new System.Drawing.Size(45, 20);
            this.txtMinimalDistanceBetweenTwoObjects.TabIndex = 32;
            // 
            // txtMaximalAmountOfObjectsToFind
            // 
            this.txtMaximalAmountOfObjectsToFind.BackColor = System.Drawing.Color.White;
            this.txtMaximalAmountOfObjectsToFind.Location = new System.Drawing.Point(331, 245);
            this.txtMaximalAmountOfObjectsToFind.Name = "txtMaximalAmountOfObjectsToFind";
            this.txtMaximalAmountOfObjectsToFind.ReadOnly = true;
            this.txtMaximalAmountOfObjectsToFind.Size = new System.Drawing.Size(45, 20);
            this.txtMaximalAmountOfObjectsToFind.TabIndex = 33;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(134, 276);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(83, 13);
            this.label7.TabIndex = 38;
            this.label7.Text = "Camera Position";
            // 
            // cbCameraPosition
            // 
            this.cbCameraPosition.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbCameraPosition.FormattingEnabled = true;
            this.cbCameraPosition.Items.AddRange(new object[] {
            "In Front Of The Target",
            "Behind The Target"});
            this.cbCameraPosition.Location = new System.Drawing.Point(136, 292);
            this.cbCameraPosition.Name = "cbCameraPosition";
            this.cbCameraPosition.Size = new System.Drawing.Size(131, 21);
            this.cbCameraPosition.TabIndex = 37;
            this.cbCameraPosition.SelectedIndexChanged += new System.EventHandler(this.cbCameraPosition_SelectedIndexChanged);
            // 
            // bDefaults
            // 
            this.bDefaults.Location = new System.Drawing.Point(274, 291);
            this.bDefaults.Name = "bDefaults";
            this.bDefaults.Size = new System.Drawing.Size(102, 23);
            this.bDefaults.TabIndex = 39;
            this.bDefaults.Text = "Restore Defaults";
            this.bDefaults.UseVisualStyleBackColor = true;
            this.bDefaults.Click += new System.EventHandler(this.bDefaults_Click);
            // 
            // caCalibration
            // 
            this.caCalibration.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.caCalibration.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.caCalibration.Location = new System.Drawing.Point(382, 5);
            this.caCalibration.Name = "caCalibration";
            this.caCalibration.Size = new System.Drawing.Size(409, 308);
            this.caCalibration.TabIndex = 36;
            // 
            // timer
            // 
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // SettingsFrm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(796, 318);
            this.Controls.Add(this.bDefaults);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.cbCameraPosition);
            this.Controls.Add(this.caCalibration);
            this.Controls.Add(this.txtMaximalAmountOfObjectsToFind);
            this.Controls.Add(this.txtMinimalDistanceBetweenTwoObjects);
            this.Controls.Add(this.txtMedianSquare);
            this.Controls.Add(this.txtTolerance);
            this.Controls.Add(this.txtResizeSquare);
            this.Controls.Add(this.txtAverageBufferSizeInFrames);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.cbWhatToShowType);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.tbMaximalAmountOfObjectsToFind);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tbMinimalDistanceBetweenTwoObjects);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbMedianSquare);
            this.Controls.Add(this.tbTolerance);
            this.Controls.Add(this.tbResizeSquare);
            this.Controls.Add(this.tbAverageBufferSizeInFrames);
            this.DoubleBuffered = true;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(812, 357);
            this.Name = "SettingsFrm";
            this.Text = "Settings";
            ((System.ComponentModel.ISupportInitialize)(this.tbAverageBufferSizeInFrames)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbResizeSquare)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbTolerance)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbMedianSquare)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbMinimalDistanceBetweenTwoObjects)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbMaximalAmountOfObjectsToFind)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar tbAverageBufferSizeInFrames;
        private System.Windows.Forms.TrackBar tbResizeSquare;
        private System.Windows.Forms.TrackBar tbTolerance;
        private System.Windows.Forms.TrackBar tbMedianSquare;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TrackBar tbMinimalDistanceBetweenTwoObjects;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TrackBar tbMaximalAmountOfObjectsToFind;
        private System.Windows.Forms.ComboBox cbWhatToShowType;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox txtAverageBufferSizeInFrames;
        private System.Windows.Forms.TextBox txtResizeSquare;
        private System.Windows.Forms.TextBox txtTolerance;
        private System.Windows.Forms.TextBox txtMedianSquare;
        private System.Windows.Forms.TextBox txtMinimalDistanceBetweenTwoObjects;
        private System.Windows.Forms.TextBox txtMaximalAmountOfObjectsToFind;
        private ucCalibration caCalibration;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox cbCameraPosition;
        private System.Windows.Forms.Button bDefaults;
        private System.Windows.Forms.Timer timer;
    }
}