using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Security.Permissions;
using System.Windows.Forms;

namespace ShootingRange
{
    public partial class MainFrm : Form
    {
        #region Main
        public MainFrm()
        {
            InitializeComponent();
            Initialize();
            Init();
        }
        #endregion

        #region Template
        #region Initialize
        //Initialize.
        private void Initialize()
        {
            this.Text = GetAssemblyInfo.AssemblyTitle;
            this.Icon = global::ShootingRange.Properties.Resources.program_icon;
            trayNotifyIcon.Icon = global::ShootingRange.Properties.Resources.program_icon;
            //Load Settings from Registry.
            this.Location = Settings.Location;
            this.Size = Settings.Size;
            this.Visible = Settings.Visible;
            this.TopMost = Settings.AlwaysOnTop;
            //Apply these settings.
            SetVisible();
            SetAlwaysOnTop();
            SetRunAtStartup();
        }
        #endregion

        #region Menu Events
        //When clicked on Hide or Show.
        private void hideOrShowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeVisible();
        }
        //When clicked on Run At Startup.
        private void runAtStartupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeRunAtStartup();
        }
        //When clicked on Always On Top.
        private void alwaysOnTopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeAlwaysOnTop();
        }
        private void ExitProg()
        {
            if (mDevice != null)
            {
                mDevice.StopServer();
            }
            DeInit();
            Application.Exit();
        }
        //When clicked on Help Topics.
        private void helpTopicsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start(CommonPathes.ProgramFilesPath + "\\SoftCollectionShootingRange.pdf");
        }
        //When clicked on Help Video.
        private void helpVideoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://youtu.be/T7rQLo0dbJE");
        }
        //When clicked on Visit Website.
        private void visitWebsiteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.soft-collection.com");
        }
        //When clicked on About.
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox about = new AboutBox();
            about.TopMost = true;
            about.ShowDialog();
        }
        //When clicked on Exit.
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExitProg();
        }
        private void trayNotifyIcon_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left) ChangeVisible();
        }
        #endregion

        #region Set Settings and Update Menu Items
        //Changes Visible state.
        private void ChangeVisible()
        {
            this.Visible = !this.Visible;
            SetVisible();
        }
        //Apply current Visible value.
        private void SetVisible()
        {
            Settings.Visible = this.Visible;
            if (this.Visible)
            {
                hideOrShowToolStripMenuItem.Text = "Hide";
                trayNotifyIcon.Text = "Hide " + GetAssemblyInfo.AssemblyProduct;

                this.Opacity = 1;
            }
            else
            {
                hideOrShowToolStripMenuItem.Text = "Show";
                trayNotifyIcon.Text = "Show " + GetAssemblyInfo.AssemblyProduct;
                this.Opacity = 0;

            }
        }
        //Changes Run At Startup state.
        private void ChangeRunAtStartup()
        {
            runAtStartupToolStripMenuItem.Checked = !runAtStartupToolStripMenuItem.Checked;
            Settings.RunAtStartup = runAtStartupToolStripMenuItem.Checked;
        }
        //Apply current Run At Startup value.
        private void SetRunAtStartup()
        {
            runAtStartupToolStripMenuItem.Checked = Settings.RunAtStartup;
        }
        //Changes Always On Top state.
        private void ChangeAlwaysOnTop()
        {
            this.TopMost = !this.TopMost;
            if (!(msSettingsFrm == null || msSettingsFrm.IsDisposed))
            {
                msSettingsFrm.TopMost = this.TopMost;
            }
            SetAlwaysOnTop();
        }
        //Apply current Always On Top value.
        private void SetAlwaysOnTop()
        {
            Settings.AlwaysOnTop = this.TopMost;
            alwaysOnTopToolStripMenuItem.Checked = this.TopMost;
        }
        #endregion

        #region Main Form Events
        private void MainFrm_Move(object sender, EventArgs e)
        {
            Settings.Location = this.Location;
        }
        private void MainFrm_Resize(object sender, EventArgs e)
        {
            Settings.Size = this.Size;
        }
        private void MainFrm_FormClosing(object sender, FormClosingEventArgs e)
        {
            ExitProg();
        }
        #endregion

        #region WinProc
        private const int WM_SYSCOMMAND = 0x0112;
        private const int SC_MINIMIZE = 0xF020;
        [SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.UnmanagedCode)]
        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WM_SYSCOMMAND:
                    int command = m.WParam.ToInt32() & 0xfff0;
                    if (command == SC_MINIMIZE)
                    {
                        try
                        {
                            ChangeVisible();
                            return;
                        }
                        catch
                        {
                        }
                    }
                    break;
            }
            base.WndProc(ref m);
        }
        #endregion
        #endregion

        #region ShootingRange

        #region Inner Classes
        private class SoundProducer
        {
            #region Delegates
            public delegate void dOnSayEvent();
            #endregion

            #region Events
            public event dOnSayEvent OnSayEvent = null;
            #endregion

            #region Variables
            private clsSayWords mSayWords = null;
            private clsSayWords.eAvailableWords mWord;
            private DateTime mCommandTime = DateTime.MinValue;
            private int mDelayInMS = 0;
            private bool mUsed = true;
            private bool mBefore = false;
            #endregion

            #region Constructor/Destructor
            public SoundProducer(clsSayWords.eAvailableWords word)
            {
                mWord = word;
                mSayWords = new clsSayWords();
            }
            #endregion

            #region Methods
            public void OnTimer()
            {
                if (DateTime.Now.Subtract(mCommandTime).TotalMilliseconds > mDelayInMS)
                {
                    if (!mUsed)
                    {
                        if (mBefore)
                        {
                            if (OnSayEvent != null)
                            {
                                OnSayEvent();
                            }
                        }
                        else //After
                        {
                            mSayWords.Say(mWord);
                        }
                        mUsed = true;
                    }
                }
            }
            public void SayBeforeEvent(int delayInMS)
            {
                mBefore = true;
                mDelayInMS = delayInMS;
                mCommandTime = DateTime.Now;
                mUsed = false;
                mSayWords.Say(mWord);
            }
            public void SayAfterDelay(int delayInMS)
            {
                mBefore = false;
                mDelayInMS = delayInMS;
                mCommandTime = DateTime.Now;
                mUsed = false;
            }
            public void CancelSay()
            {
                mUsed = true;
            }
            #endregion
        }
        #endregion

        #region Enums
        public enum eGUIStates
        {
            NotSetYet = 0,
            SourceTypeRTSPCameraSelected = 1,
            SourceTypeWebCameraSelected = 2,
            SourceTypeSimulatorSelected = 3,
            URLSelected = 4,
            NotConnected = 5,
            Connecting = 6,
            Connected = 7,
            UnableToConnect = 8,
            InGame = 9,
            GameFinished = 10
        }
        #endregion

        #region Variables
        private clsDevice mDevice = null;
        private clsCustomFont mLCDFont = null;
        private SoundProducer mSoundProducerReady = null;
        private SoundProducer mSoundProducerMagnum = null;
        private SoundProducer mSoundProducerFinish = null;
        private Common.eConnectionStates mConnectionState = Common.eConnectionStates.NotSetYet;
        private Common.eConnectionStates mLastConnectionState = Common.eConnectionStates.NotConnected; //Must be different from mConnectionState.
        private eGUIStates mGUIState = eGUIStates.NotSetYet;
        private eGUIStates mLastGUIState = eGUIStates.NotConnected; //Must be different from mGUIState.
        private static SettingsFrm msSettingsFrm = null;
        private PointF mShotCoordinates;
        private bool mShotCoordinatesAlreadyUsed = true;
        private object mShotCoordinatesLock = new object();
        private int mShootCounter = 0;
        #endregion

        #region Initialize
        private void Init()
        {
            mDevice = new clsDevice();
            mDevice.Set_Draw_Windows_TargetWindow(pbVideoWindow.Handle);
            mDevice.OnShot += device_OnShot;
            mLCDFont = new clsCustomFont();
            //----------------------------------------------------------------------------------------------
            mSoundProducerReady = new SoundProducer(clsSayWords.eAvailableWords.Ready);
            mSoundProducerReady.OnSayEvent += mSoundProducerReady_OnSayEvent;
            mSoundProducerMagnum = new SoundProducer(clsSayWords.eAvailableWords.Magnum);
            mSoundProducerFinish = new SoundProducer(clsSayWords.eAvailableWords.Finish);
            //----------------------------------------------------------------------------------------------
            bNewGame.Font = mLCDFont.GetFont(14.0f, FontStyle.Bold);
            timer.Start();
            //----------------------------------------------------------------------------------------------
        }
        private void DeInit()
        {
            if (mDevice != null)
            {
                mDevice.Dispose();
            }
        }
        #endregion

        #region Menu Events
        private void printTargetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start(CommonPathes.ProgramFilesPath + "\\SoftCollectionShootingRangeTarget.pdf");
        }
        private void tsbSettings_Click(object sender, EventArgs e)
        {
            if (msSettingsFrm == null || msSettingsFrm.IsDisposed)
            {
                msSettingsFrm = new SettingsFrm();
                msSettingsFrm.Icon = global::ShootingRange.Properties.Resources.program_icon;
                msSettingsFrm.TopMost = Settings.AlwaysOnTop;
                msSettingsFrm.SetDevice(mDevice);
                msSettingsFrm.Show();
            }
        }
        #endregion

        #region Main Form Events
        private void tscbSource_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (tscbSource.SelectedIndex)
            {
                case 0: //RTSP Camera.
                    {
                        tscbURL.Items.Clear();
                        List<string> tempList = new List<string>();
                        if (!Directory.Exists(CommonPathes.ProgramDataPath))
                        {
                            Directory.CreateDirectory(CommonPathes.ProgramDataPath);
                        }
                        if (!File.Exists(CommonPathes.ProgramDataPath + @"\IPCameras.lst"))
                        {
                            FileStream fs = File.Create(CommonPathes.ProgramDataPath + @"\IPCameras.lst");
                            fs.Close();
                        }
                        using (StreamReader sr = new StreamReader(CommonPathes.ProgramDataPath + @"\IPCameras.lst"))
                        {
                            while (sr.Peek() >= 0)
                            {
                                string tempString = sr.ReadLine();
                                if (!string.IsNullOrEmpty(tempString))
                                {
                                    tscbURL.Items.Add(tempString);
                                }
                            }
                            sr.Close();
                        }
                        mGUIState = eGUIStates.SourceTypeRTSPCameraSelected;
                    }
                    break;
                case 1: //Web Camera.
                    {
                        tscbURL.Items.Clear();
                        tscbURL.Items.Add("0");
                        tscbURL.Items.Add("1");
                        tscbURL.Items.Add("2");
                        mGUIState = eGUIStates.SourceTypeWebCameraSelected;
                    }
                    break;
                case 2: //Simulator.
                    {
                        tscbURL.Items.Clear();
                        string[] fileEntries = Directory.GetFiles(CommonPathes.ProgramFilesPath);
                        foreach (string fileName in fileEntries)
                        {
                            if ((Path.GetExtension(fileName) == ".264") ||
                                (Path.GetExtension(fileName) == ".mp4") ||
                                (Path.GetExtension(fileName) == ".mpg") ||
                                (Path.GetExtension(fileName) == ".avi"))
                            {
                                tscbURL.Items.Add(@"rtsp://127.0.0.1/" + Path.GetFileName(fileName));
                            }
                        }
                        mGUIState = eGUIStates.SourceTypeSimulatorSelected;
                    }
                    break;
            }
        }
        private void tscbURL_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mDevice != null)
            {
                bool isWebCam = false;
                if (tscbSource.SelectedIndex == 1) isWebCam = true;
                mDevice.Set_Connection_URL((string)tscbURL.SelectedItem, isWebCam);
                mGUIState = eGUIStates.URLSelected;
            }
        }
        private void tsbEditCameraList_Click(object sender, EventArgs e)
        {
            if (!Directory.Exists(CommonPathes.ProgramDataPath))
            {
                Directory.CreateDirectory(CommonPathes.ProgramDataPath);
            }
            if (!File.Exists(CommonPathes.ProgramDataPath + @"\IPCameras.lst"))
            {
                FileStream fs = File.Create(CommonPathes.ProgramDataPath + @"\IPCameras.lst");
                fs.Close();
            }
            StartApp("Notepad", CommonPathes.ProgramDataPath + @"\IPCameras.lst");
            mGUIState = eGUIStates.NotSetYet;
        }
        private void tsbConnectDisconnect_Click(object sender, EventArgs e)
        {
            switch (mGUIState)
            {
                case eGUIStates.NotSetYet:
                    //Button is Disabled.
                    break;
                case eGUIStates.SourceTypeRTSPCameraSelected:
                    //Button is Disabled.
                    break;
                case eGUIStates.SourceTypeWebCameraSelected:
                    //Button is Disabled.
                    break;
                case eGUIStates.SourceTypeSimulatorSelected:
                    //Button is Disabled.
                    break;
                case eGUIStates.URLSelected:
                    if (mDevice != null)
                    {
                        if (tscbSource.SelectedIndex == 2) //Simulator.
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.ConnectStartServer);
                        }
                        else
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.Connect);
                        }
                    }
                    break;
                case eGUIStates.NotConnected:
                    if (mDevice != null)
                    {
                        if (tscbSource.SelectedIndex == 2) //Simulator.
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.ConnectStartServer);
                        }
                        else
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.Connect);
                        }
                    }
                    break;
                case eGUIStates.Connecting:
                    //Button is Disabled.
                    break;
                case eGUIStates.Connected:
                    if (mDevice != null)
                    {
                        if (tscbSource.SelectedIndex == 2) //Simulator.
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.DisconnectStopServer);
                        }
                        else
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.Disconnect);
                        }
                    }
                    break;
                case eGUIStates.UnableToConnect:
                    if (mDevice != null)
                    {
                        if (tscbSource.SelectedIndex == 2) //Simulator.
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.ConnectStartServer);
                        }
                        else
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.Connect);
                        }
                    }
                    break;
                case eGUIStates.InGame:
                    if (mDevice != null)
                    {
                        if (tscbSource.SelectedIndex == 2) //Simulator.
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.DisconnectStopServer);
                        }
                        else
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.Disconnect);
                        }
                    }
                    break;
                case eGUIStates.GameFinished:
                    if (mDevice != null)
                    {
                        if (tscbSource.SelectedIndex == 2) //Simulator.
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.DisconnectStopServer);
                        }
                        else
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.Disconnect);
                        }
                    }
                    break;
            }
        }
        private void tsbEditCameraList_MouseEnter(object sender, EventArgs e)
        {
            toolStrip.Focus();
        }
        private void tsbConnectDisconnect_MouseEnter(object sender, EventArgs e)
        {
            toolStrip.Focus();
        }
        private void tsbSettings_MouseEnter(object sender, EventArgs e)
        {
            toolStrip.Focus();
        }
        private void bNewGame_Click(object sender, EventArgs e)
        {
            lock (mShotCoordinatesLock)
            {
                mShootCounter = 0;
                mShotCoordinatesAlreadyUsed = true;
            }
            //--------------------------------------------------
            mGUIState = eGUIStates.Connected;
            tgTarget.ClearHoles();
            tgTarget.Refresh();
            dpDisplay.ClearHoles();
            if (mSoundProducerFinish != null) mSoundProducerFinish.CancelSay();
            if (mSoundProducerReady != null) mSoundProducerReady.SayBeforeEvent(1000);
        }
        private void timer_Tick(object sender, EventArgs e)
        {
            if (mDevice != null)
            {
                mConnectionState = mDevice.GetConnectionState();
            }
            //------------------------------------------------------------
            if (mLastConnectionState != mConnectionState)
            {
                OnConnectionStateChanged(mConnectionState);
                mLastConnectionState = mConnectionState;
            }
            //------------------------------------------------------------
            if (mLastGUIState != mGUIState)
            {
                OnGUIStateChanged(mGUIState);
                mLastGUIState = mGUIState;
            }
            //------------------------------------------------------------
            if (mGUIState == eGUIStates.InGame)
            {
                CheckForNewShot();
            }
            //------------------------------------------------------------
            if (mSoundProducerReady != null) mSoundProducerReady.OnTimer();
            if (mSoundProducerMagnum != null) mSoundProducerMagnum.OnTimer();
            if (mSoundProducerFinish != null) mSoundProducerFinish.OnTimer();
        }
        public void device_OnShot(PointF shotCoordinates, PointF[] shotBorder, DateTime time)
        {
            lock(mShotCoordinatesLock)
            {
                mShotCoordinates = shotCoordinates;
                mShotCoordinatesAlreadyUsed = false;
            }
        }
        void mSoundProducerReady_OnSayEvent()
        {
            mGUIState = eGUIStates.InGame;
        }
        #endregion

        #region Methods
        private void CheckForNewShot()
        {
            PointF tempShotCoordinates = new PointF();
            bool tempShotPerformed = false;
            lock (mShotCoordinatesLock)
            {
                if (!mShotCoordinatesAlreadyUsed)
                {
                    tempShotCoordinates = mShotCoordinates;
                    tempShotPerformed = true;
                    mShotCoordinatesAlreadyUsed = true;
                }
            }
            if (tempShotPerformed)
            {
                if (mDevice != null)
                {
                    if (mDevice.VMD_HorizontalFlip) tempShotCoordinates = clsGraphicsCommon.Translate_0P1_P10(tempShotCoordinates);
                    bool isShotInsideTarget = Vector.Translate_TargetBorder_0P1(mDevice.Draw_TargetBorder_Border, tempShotCoordinates, 0.001f, out tempShotCoordinates);
                    if (!isShotInsideTarget) return;
                    tempShotCoordinates = clsGraphicsCommon.Translate_0P1_M1P1(tempShotCoordinates);
                    //----------------------------------------------------------------------------------
                    tgTarget.AddHole(tempShotCoordinates);
                    tgTarget.Refresh();
                    dpDisplay.AddHole(tempShotCoordinates);
                    mShootCounter++;
                    if (mSoundProducerMagnum != null) mSoundProducerMagnum.SayBeforeEvent(0);
                    if (mShootCounter == 10) mGUIState = eGUIStates.GameFinished;
                }
            }
        }
        private void OnConnectionStateChanged(Common.eConnectionStates connectionState)
        {
            switch (connectionState)
            {
                case Common.eConnectionStates.NotSetYet:
                    mGUIState = eGUIStates.NotSetYet;
                    break;
                case Common.eConnectionStates.NotConnected:
                    mGUIState = eGUIStates.NotConnected;
                    break;
                case Common.eConnectionStates.Connecting:
                    mGUIState = eGUIStates.Connecting;
                    break;
                case Common.eConnectionStates.Connected:
                    mGUIState = eGUIStates.Connected;
                    break;
                case Common.eConnectionStates.UnableToConnect:
                    mGUIState = eGUIStates.UnableToConnect;
                    break;
            }
        }
        private void OnGUIStateChanged(eGUIStates guiState)
        {
            switch (guiState)
            {
                case eGUIStates.NotConnected:
                    //Stop Server after simulator ends.
                    if (mDevice != null)
                    {
                        if (tscbSource.SelectedIndex == 2) //Simulator.
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.DisconnectStopServer);
                        }
                        else
                        {
                            mDevice.EnqueueCommand(clsDevice.DeviceCommand.eCommandTypes.Disconnect);
                        }
                    }
                    break;
            }
            SetGUIState(guiState);
        }
        private void SetGUIState(eGUIStates guiState)
        {
            switch (guiState)
            {
                case eGUIStates.NotSetYet:
                    tscbSource.Enabled = true;
                    tscbSource.SelectedIndex = (-1);
                    tscbURL.Enabled = false;
                    tscbURL.SelectedIndex = (-1);
                    tsbEditCameraList.Enabled = false;
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.play;
                    tsbConnectDisconnect.Text = "Connect";
                    tsbConnectDisconnect.Enabled = false;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.gray;
                    tsbStatus.Text = "Not Connected";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = false;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.SourceTypeRTSPCameraSelected:
                    tscbSource.Enabled = true;
                    tscbURL.Enabled = true;
                    tscbURL.SelectedIndex = (-1);
                    tsbEditCameraList.Enabled = true;
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.play;
                    tsbConnectDisconnect.Text = "Connect";
                    tsbConnectDisconnect.Enabled = false;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.gray;
                    tsbStatus.Text = "Not Connected";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = false;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.SourceTypeWebCameraSelected:
                    tscbSource.Enabled = true;
                    tscbURL.Enabled = true;
                    tscbURL.SelectedIndex = (-1);
                    tsbEditCameraList.Enabled = false;
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.play;
                    tsbConnectDisconnect.Text = "Connect";
                    tsbConnectDisconnect.Enabled = false;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.gray;
                    tsbStatus.Text = "Not Connected";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = false;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.SourceTypeSimulatorSelected:
                    tscbSource.Enabled = true;
                    tscbURL.Enabled = true;
                    tscbURL.SelectedIndex = (-1);
                    tsbEditCameraList.Enabled = false;
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.play;
                    tsbConnectDisconnect.Text = "Connect";
                    tsbConnectDisconnect.Enabled = false;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.gray;
                    tsbStatus.Text = "Not Connected";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = false;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.URLSelected:
                    tscbSource.Enabled = true;
                    tscbURL.Enabled = true;
                    tsbEditCameraList.Enabled = (tscbSource.SelectedIndex == 0); //RTSP Camera.
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.play;
                    tsbConnectDisconnect.Text = "Connect";
                    tsbConnectDisconnect.Enabled = true;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.gray;
                    tsbStatus.Text = "Not Connected";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = false;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.NotConnected:
                    tscbSource.Enabled = true;
                    tscbURL.Enabled = true;
                    tsbEditCameraList.Enabled = (tscbSource.SelectedIndex == 0); //RTSP Camera.
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.play;
                    tsbConnectDisconnect.Text = "Connect";
                    tsbConnectDisconnect.Enabled = true;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.gray;
                    tsbStatus.Text = "Not Connected";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = false;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.Connecting:
                    tscbSource.Enabled = true;
                    tscbURL.Enabled = true;
                    tsbEditCameraList.Enabled = (tscbSource.SelectedIndex == 0); //RTSP Camera.
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.play;
                    tsbConnectDisconnect.Text = "Connecting";
                    tsbConnectDisconnect.Enabled = false;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.blue;
                    tsbStatus.Text = "Connecting";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = false;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.Connected:
                    tscbSource.Enabled = false;
                    tscbURL.Enabled = false;
                    tsbEditCameraList.Enabled = false;
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.stop;
                    tsbConnectDisconnect.Text = "Disconnect";
                    tsbConnectDisconnect.Enabled = true;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.green;
                    tsbStatus.Text = "Connected";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = true;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.UnableToConnect:
                    tscbSource.Enabled = true;
                    tscbURL.Enabled = true;
                    tsbEditCameraList.Enabled = (tscbSource.SelectedIndex == 0); //RTSP Camera.
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.play;
                    tsbConnectDisconnect.Text = "Connect";
                    tsbConnectDisconnect.Enabled = true;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.red;
                    tsbStatus.Text = "Unable To Connect";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = false;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.InGame:
                    tscbSource.Enabled = false;
                    tscbURL.Enabled = false;
                    tsbEditCameraList.Enabled = false;
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.stop;
                    tsbConnectDisconnect.Text = "Disconnect";
                    tsbConnectDisconnect.Enabled = true;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.green;
                    tsbStatus.Text = "Connected";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = true;
                    pbVideoWindow.Refresh();
                    break;
                case eGUIStates.GameFinished:
                    tscbSource.Enabled = false;
                    tscbURL.Enabled = false;
                    tsbEditCameraList.Enabled = false;
                    tsbConnectDisconnect.Image = global::ShootingRange.Properties.Resources.stop;
                    tsbConnectDisconnect.Text = "Disconnect";
                    tsbConnectDisconnect.Enabled = true;
                    tsbStatus.Image = global::ShootingRange.Properties.Resources.green;
                    tsbStatus.Text = "Connected";
                    tsbSettings.Enabled = true;
                    bNewGame.Enabled = true;
                    pbVideoWindow.Refresh();
                    //--------------------------------------------------
                    if (mSoundProducerFinish != null) mSoundProducerFinish.SayAfterDelay(2000);
                    break;
            }
        }
        public static void StartApp(string fileName, string parameters)
        {
            Process process = new Process();
            process.StartInfo.FileName = fileName;
            process.StartInfo.Arguments = parameters;
            process.StartInfo.WorkingDirectory = Path.GetDirectoryName(fileName);
            process.Start();
        }
        #endregion

        #endregion
    }
}
