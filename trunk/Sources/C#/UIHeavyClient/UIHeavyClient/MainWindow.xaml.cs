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
        private OpenGLControl mOpenGLControl;
        private EditionModeControl mEditionModeControl;
        private MainMenuControl mMainMenuControl;
        private PlayModeControl mPlayModeControl;
        private TournamentControl mTournamentControl;
        private OnlineLobbyControl mOnlineLobbyControl;
        private OptionsControl mOptionControl;

        private WindowsFormsHost mWindowFormsHost;

        // Properties
        public OpenGLControl OpenGLControl
        {
            get { return mOpenGLControl; }
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

        public MainWindow()
        {
            InitializeComponent();
            ConsoleManager.Show();
            this.Loaded += CreateUserControl;
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            ConsoleManager.Hide();
        }

        public void CreateUserControl(object sender, EventArgs e)
        {
            MainWindowHandler.Context = this;

            mOpenGLControl = new OpenGLControl();
            
            mWindowFormsHost = new WindowsFormsHost();
            mWindowFormsHost.Name = "windowsFormsHost1";

            mWindowFormsHost.Child = mOpenGLControl;

            mMainMenuControl = new MainMenuControl();
            mEditionModeControl = new EditionModeControl(mWindowFormsHost);
            mPlayModeControl = new PlayModeControl(mWindowFormsHost);
            mTournamentControl = new TournamentControl();
            mOnlineLobbyControl = new OnlineLobbyControl();
            mOptionControl = new OptionsControl();

            this.WindowContentControl.Content = mMainMenuControl;
        }

        private void BackToMainMenu(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }
    }
}
