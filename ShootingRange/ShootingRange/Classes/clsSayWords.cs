using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.IO;
using System.Text;
using System.Threading;

namespace ShootingRange
{
    public class clsSayWords
    {
        #region External Declarations
        [Flags]
        private enum SoundFlags : int
        {
            SND_SYNC = 0x0000,            // play synchronously (default)
            SND_ASYNC = 0x0001,           // play asynchronously
            SND_NODEFAULT = 0x0002,       // silence (!default) if sound not found
            SND_MEMORY = 0x0004,          // pszSound points to a memory file
            SND_LOOP = 0x0008,            // loop the sound until next sndPlaySound
            SND_NOSTOP = 0x0010,          // don't stop any currently playing sound
            SND_PURGE = 0x0040,           // purge non-static events
            SND_NOWAIT = 0x00002000,      // don't wait if the driver is busy
            SND_ALIAS = 0x00010000,       // name is a registry alias
            SND_ALIAS_ID = 0x00110000,    // alias is a predefined id
            SND_FILENAME = 0x00020000     // name is file name
        }

        [DllImport("winmm.dll", SetLastError = true)]
        private static extern bool PlaySound(byte[] ptrToSound, System.UIntPtr hmod, uint fdwSound);

        [DllImport("winmm.dll", SetLastError = true)]
        private static extern bool PlaySound(IntPtr ptrToSound, System.UIntPtr hmod, uint fdwSound);
        #endregion

        #region Inner Classes
        private class SoundData
        {
            private Stream[] mStream = null;
            private byte[] mByteArray = null;
            private GCHandle? mGCHandle = null;
            private bool mDisposed = false;
            public readonly IntPtr Handle = IntPtr.Zero;
            public SoundData(Stream stream)
            {
                if (stream != null)
                {
                    mByteArray = new byte[stream.Length];
                    stream.Read(mByteArray, 0, (int)stream.Length);
                    mGCHandle = GCHandle.Alloc(mByteArray, GCHandleType.Pinned);
                    Handle = mGCHandle.Value.AddrOfPinnedObject();
                }
            }
            ~SoundData()
            {
                Dispose(false);
            }
            public void Dispose()
            {
                try
                {
                    Dispose(true);
                    GC.SuppressFinalize(this);
                }
                catch
                {
                }
            }
            protected virtual void Dispose(bool disposing)
            {
                try
                {
                    if (!this.mDisposed)
                    {
                        if (disposing)
                        {
                        }
                        //-------------------------------------------------------
                        if (mGCHandle != null)
                        {
                            PlaySound((byte[])null, (UIntPtr)0, (uint)0);
                            mGCHandle.Value.Free();
                            mGCHandle = null;
                        }
                        //-------------------------------------------------------
                        mDisposed = true;
                    }
                }
                catch
                {
                }
            }
        }
        #endregion

        #region Enums
        public enum eAvailableWords
        {
            Magnum = 0,
            Ready = 1,
            Next = 2,
            Finish = 3,
        }
        #endregion

        #region Variables
        SoundData[] mSounds = {new SoundData(global::ShootingRange.Properties.Resources.magnum),
                               new SoundData(global::ShootingRange.Properties.Resources.ready),
                               new SoundData(global::ShootingRange.Properties.Resources.next),
                               new SoundData(global::ShootingRange.Properties.Resources.finish)};
        #endregion
        
        #region Methods
        public void Say(eAvailableWords wordID)
        {
            PlaySound(mSounds[(int)wordID].Handle, (UIntPtr)0, (uint)SoundFlags.SND_ASYNC | (uint)SoundFlags.SND_MEMORY);
            //PlaySound((byte[])null, (UIntPtr)0, (uint)SoundFlags.SND_ASYNC | (uint)SoundFlags.SND_MEMORY);
        }
        public void Stop() 
        { 
            PlaySound(null, (UIntPtr)0, (int)SoundFlags.SND_PURGE); 
        }
        #endregion
    }
}