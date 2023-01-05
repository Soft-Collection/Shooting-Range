using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ShootingRange
{
    public partial class ucDisplay : UserControl
    {
        #region Enums
        #endregion

        #region Variables
        private int mCount = 1;
        private int mSum = 0;
        private clsCustomFont mLCDFont = null;
        #endregion

        #region Constructor / Destructor
        public ucDisplay()
        {
            InitializeComponent();
            mLCDFont = new clsCustomFont();
            txtDisplay.Font = mLCDFont.GetFont(11.0f, FontStyle.Bold);
        }
        #endregion

        #region Methods
        public void AddHole(PointF hole) //Must be [-1,1],[-1,1].
        {
            float distanceFromCenterF = (float)Math.Sqrt(Math.Pow((double)hole.X, (double)2) + Math.Pow((double)hole.Y, (double)2));
            float distanceFromCenter15to0F = 15.0f - (distanceFromCenterF * 10.0f); //We need 15 to 0 and not 10 to 0 because we want to avoid negative numbers to be converted to integers. (-0.9 will be converted to 0 and not to 1).
            int distanceFromCenter10to0 = (int)(distanceFromCenter15to0F - 4.0f);
            if (distanceFromCenter10to0 > 10) distanceFromCenter10to0 = 10;
            mSum += distanceFromCenter10to0;
            if (mCount < 10)
            {
                txtDisplay.Text += "Shot " + mCount.ToString() + " Result Is " + distanceFromCenter10to0.ToString() + "\r\n";
            }
            else if (mCount == 10)
            {
                txtDisplay.Text += "Shot " + mCount.ToString() + " Result Is " + distanceFromCenter10to0.ToString() + "\r\n";
                txtDisplay.Text += "Summary " + mSum.ToString() + " From 100";
            }
            mCount++;
        }
        public void ClearHoles()
        {
            mCount = 1;
            mSum = 0;
            txtDisplay.Clear();
        }
        #endregion
    }
}