using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace ShootingRange
{
    public partial class ucTarget : UserControl
    {
        #region Enums
        #endregion

        #region Variables
        private List<PointF> mHoles = new List<PointF>();
        private object mHolesLock = new object();
        #endregion

        #region Constructor / Destructor
        public ucTarget()
        {
            InitializeComponent();
        }
        #endregion

        #region Form Events
        private void pbTarget_Paint(object sender, PaintEventArgs e)
        {
            Draw(e.Graphics);
        }
        #endregion

        #region Methods
        public void AddHole(PointF hole) //Must be [-1,1],[-1,1].
        {
            lock (mHolesLock)
            {
                mHoles.Add(hole);
            }
        }
        public void ClearHoles()
        {
            lock(mHolesLock)
            {
                mHoles.Clear();
            }
        }
        public void Draw(Graphics g)
        {
            lock (mHolesLock)
            {
                foreach(PointF hole in mHoles)
                {
                    float size = 10;
                    float left = ((hole.X + 1) * ((float)pbTarget.Width / 2)) - (size / 2);
                    float top = ((hole.Y + 1) * ((float)pbTarget.Height / 2)) - (size / 2);
                    GraphicsPath graphicsPath = new GraphicsPath();
                    graphicsPath.AddEllipse(left, top, size, size);
                    PathGradientBrush pathGradientBrush = new PathGradientBrush(graphicsPath);
                    pathGradientBrush.CenterPoint = new PointF(left + (size / 3), Top + (size / 3)); //Glare center.
                    pathGradientBrush.CenterColor = Color.White; //Glare color.
                    pathGradientBrush.SurroundColors = new Color[] { Color.Black };
                    g.FillEllipse(pathGradientBrush, left, top, size, size);
                    g.DrawEllipse(new Pen(Color.Black), left, top, size, size);
                }
            }
        }
        #endregion
    }
}
