using System;
using System.Drawing;
using System.Drawing.Text;
using System.Runtime.InteropServices;

namespace ShootingRange
{
    public class clsCustomFont
    {
        #region External Declarations
        [DllImport("gdi32.dll")]
        private static extern IntPtr AddFontMemResourceEx(IntPtr pbFont, uint cbFont, IntPtr pdv, [In] ref uint pcFonts);
        #endregion

        #region Enums
        #endregion

        #region Variables
        private PrivateFontCollection pfc = new PrivateFontCollection();
        private FontFamily mFontFamily;
        #endregion

        #region Constructor / Destructor
        public clsCustomFont()
        {
            //https://www.daniweb.com/programming/software-development/threads/303779/c-embedding-fonts
            byte[] fontArray = global::ShootingRange.Properties.Resources.LCDFont;
            int fontArrayLength = global::ShootingRange.Properties.Resources.LCDFont.Length;
            IntPtr ptrData = Marshal.AllocCoTaskMem(fontArrayLength);
            Marshal.Copy(fontArray, 0, ptrData, fontArrayLength);
            uint cFonts = 0;
            AddFontMemResourceEx(ptrData, (uint)fontArray.Length, IntPtr.Zero, ref cFonts);
            PrivateFontCollection pfc = new PrivateFontCollection();
            pfc.AddMemoryFont(ptrData, fontArrayLength);
            Marshal.FreeCoTaskMem(ptrData);
            mFontFamily = pfc.Families[0];
        }
        #endregion

        #region Methods
        public Font GetFont(float emSize, FontStyle style)
        {
            Font retVal = null;
            if (mFontFamily != null)
            {
                retVal = new Font(mFontFamily, emSize, style);
            }
            return(retVal);
        }
        #endregion
    }
}