namespace ShootingRange
{
    partial class ucCalibration
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.pbCalibrationWindow = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pbCalibrationWindow)).BeginInit();
            this.SuspendLayout();
            // 
            // pbCalibrationWindow
            // 
            this.pbCalibrationWindow.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pbCalibrationWindow.BackColor = System.Drawing.Color.Gray;
            this.pbCalibrationWindow.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbCalibrationWindow.Location = new System.Drawing.Point(0, 0);
            this.pbCalibrationWindow.Name = "pbCalibrationWindow";
            this.pbCalibrationWindow.Size = new System.Drawing.Size(343, 267);
            this.pbCalibrationWindow.TabIndex = 28;
            this.pbCalibrationWindow.TabStop = false;
            this.pbCalibrationWindow.Paint += new System.Windows.Forms.PaintEventHandler(this.pbCalibrationWindow_Paint);
            this.pbCalibrationWindow.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pbCalibrationWindow_MouseDown);
            this.pbCalibrationWindow.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pbCalibrationWindow_MouseMove);
            this.pbCalibrationWindow.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pbCalibrationWindow_MouseUp);
            // 
            // ucCalibration
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pbCalibrationWindow);
            this.Name = "ucCalibration";
            this.Size = new System.Drawing.Size(343, 267);
            this.Resize += new System.EventHandler(this.ucCalibration_Resize);
            ((System.ComponentModel.ISupportInitialize)(this.pbCalibrationWindow)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pbCalibrationWindow;
    }
}
