using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.Windows.Interop;
using System.Windows.Forms;
using System.Windows.Forms.Integration;

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        
        //static System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();

        // Controls
        private EditionModeControl mEditionModeControl;
        private MainMenuControl mMainMenuControl;
        private PlayModeControl mPlayModeControl;
        private TournamentControl mTournamentControl;
        private OnlineLobbyControl mOnlineLobbyControl;
        private OptionsControl mOptionControl;
        private RadioOptionControl mRadioOptionControl;
        private AIOptionControl mAIOptionControl;
        private KeyboardOptionControl mKeyboardOptionControl;

        private OpenGLControl mOpenGLControl;
        private WindowsFormsHost mWindowFormsHost;

        // Properties
        public OpenGLControl OpenGLControl
        {
            get { return mOpenGLControl; }
        }
        private void Window_Closed(object sender, EventArgs e)
        {
            FreeApplicationMemory();
            ConsoleManager.Hide();
        }
        public EditionModeControl EditionModeControl
        {
            get { return mEditionModeControl; }
        }
        public PlayModeControl PlayModeControl
        {
            get { return mPlayModeControl; }
        }
        public TournamentControl TournamentControl
        {
            get { return mTournamentControl; }
        }
        public MainMenuControl MainMenuControl
        {
            get { return mMainMenuControl; }
        }
        public OnlineLobbyControl OnlineLobbyControl
        {
            get { return mOnlineLobbyControl; }
        }
        public OptionsControl OptionsControl
        {
            get { return mOptionControl; }
        }
        public ContentControl WindowContentControl
        {
            get { return windowContentControl; }
        }
        public RadioOptionControl RadioOptionControl
        {
            get { return mRadioOptionControl; }
        }
        public AIOptionControl AIOptionControl
        {
            get { return mAIOptionControl; }
        }
        public KeyboardOptionControl KeyboardOptionControl
        {
            get { return mKeyboardOptionControl; }
        }

        // C++ function to initialise C# controller on that side
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitDLL();
        [DllImport(@"RazerGame.dll")]
        public static extern void FreeApplicationMemory();

        public void CreateUserControl(object sender, EventArgs e)
        {
            MainWindowHandler.Context = this;

            mOpenGLControl = new OpenGLControl();
            
            mWindowFormsHost = new WindowsFormsHost();
            mWindowFormsHost.Name = "windowsFormsHost1";

            mWindowFormsHost.Child = mOpenGLControl;
            mOpenGLControl.InitializeOpenGL(mOpenGLControl,new EventArgs());

            mMainMenuControl = new MainMenuControl();
            mPlayModeControl = new PlayModeControl(mWindowFormsHost);
            mEditionModeControl = new EditionModeControl(mWindowFormsHost);
            mTournamentControl = new TournamentControl();
            mOnlineLobbyControl = new OnlineLobbyControl();
            mOptionControl = new OptionsControl();
            mRadioOptionControl = new RadioOptionControl();
            mAIOptionControl = new AIOptionControl();
            mKeyboardOptionControl = new KeyboardOptionControl();

            this.WindowContentControl.Content = mMainMenuControl;
            MainWindowHandler.GoToMainMenu();
        }
            
        public MainWindow()
        {
            InitializeComponent();
            // make sure to show console before any call to the dll or we wont
            // see output
            ConsoleManager.Show();

#if DEBUG
            System.Windows.Controls.MenuItem debugMenu = new System.Windows.Controls.MenuItem();
            debugMenu.Header = "Debug";
            MenuBar.Items.Add(debugMenu);

            System.Windows.Controls.MenuItem SplitView = new System.Windows.Controls.MenuItem();
            SplitView.Header = "Split View";
            SplitView.Click += SplitView_Click;
            debugMenu.Items.Add(SplitView);

            System.Windows.Controls.MenuItem simulationMode = new System.Windows.Controls.MenuItem();
            simulationMode.Header = "Mode Simulation";
            simulationMode.Click += simulationMode_Click;
            debugMenu.Items.Add(simulationMode);
#endif

            InitDLL();
            this.Loaded += CreateUserControl;
            this.KeyDown += MainWindow_KeyDown;
            this.KeyUp += MainWindow_KeyUp;
        }

        void simulationMode_Click(object sender, RoutedEventArgs e)
        {
            // temp
            WindowContentControl.Content = PlayModeControl;
            EditionModeControl.RemoveOpenGL();
            PlayModeControl.AppendOpenGL();
            MainWindowHandler.ActionPerformed(ActionType.ACTION_ALLER_MODE_SIMULATION);
        }

        void SplitView_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.ActionPerformed(ActionType.ACTION_CAMERA_SPLIT);
        }
        
        private void BackToMainMenu(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        private void OpenWebSite(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.hockedu.com");
        }
      

        void MainWindow_KeyUp(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (mOpenGLControl != null)
            {
                if (mOpenGLControl.Focused)
                {
                    System.Windows.Forms.KeyEventArgs keyEv = ConvertKeyEvent(ref e);
                    
                    if (keyEv != null)
                    {
                        mOpenGLControl.OpenGLControl_KeyUp(sender, keyEv);
                        e.Handled = true;
                    }
                    
                }
            }
        }

        static System.Windows.Forms.KeyEventArgs ConvertKeyEvent(ref System.Windows.Input.KeyEventArgs e)
        {
            System.Windows.Forms.KeyEventArgs keyEv = null;
            switch (e.Key)
            {
                case Key.Up: keyEv = new System.Windows.Forms.KeyEventArgs(System.Windows.Forms.Keys.Up); break;
                case Key.Down: keyEv = new System.Windows.Forms.KeyEventArgs(System.Windows.Forms.Keys.Down); break;
                case Key.Left: keyEv = new System.Windows.Forms.KeyEventArgs(System.Windows.Forms.Keys.Left); break;
                case Key.Right: keyEv = new System.Windows.Forms.KeyEventArgs(System.Windows.Forms.Keys.Right); break;
                case Key.LeftAlt: keyEv = new System.Windows.Forms.KeyEventArgs(System.Windows.Forms.Keys.Alt); break;
                case Key.RightAlt: keyEv = new System.Windows.Forms.KeyEventArgs(System.Windows.Forms.Keys.Alt); break;
            }
            return keyEv;
        }
        void MainWindow_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (mOpenGLControl != null)
            {
                if (mOpenGLControl.Focused)
                {
                    System.Windows.Forms.KeyEventArgs keyEv = ConvertKeyEvent(ref e);

                    if (keyEv != null)
                    {
                        mOpenGLControl.OpenGLControl_KeyDown(sender, keyEv);
                        e.Handled = true;
                    }
                }
            }
        }

    }
}
