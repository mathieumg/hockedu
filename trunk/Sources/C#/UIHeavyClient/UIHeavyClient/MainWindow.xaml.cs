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
using UIHeavyClient.UserControls;
using System.Windows.Media.Animation;
using HttpHockeduRequests;

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

        private LoginWindow mLoginWindow;

        private OpenGLControl mOpenGLControl;
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

        public void TestCallbackMapDownloaded(string pOutputPath)
        {
            
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                Console.WriteLine(pOutputPath);
            });
        }

        public void TestCallbackMapUploaded(HttpHockeduRequests.UploadOperationStatus pStatus, int pMapId)
        {
            Console.WriteLine(pStatus);
            Console.WriteLine(pMapId);
        }
        


        private void Window_Closed(object sender, EventArgs e)
        {
            OpenGLControl.mRenderTimer.Stop();
            OpenGLControl.mLogicalTimer.Stop();
            MainWindowHandler.Cleanup();
            FreeApplicationMemory();
            ConsoleManager.Hide();
            System.Windows.Application.Current.Shutdown();
        }

        public void CreateUserControl(object sender, EventArgs e)
        {
            MainWindowHandler.Context = this;

            mOpenGLControl = new OpenGLControl();
            
            mWindowFormsHost = new WindowsFormsHost();
            mWindowFormsHost.Name = "windowsFormsHost1";
            mWindowFormsHost.GotFocus += mWindowFormsHost_GotFocus;

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

            mLoginWindow = new LoginWindow();

            this.WindowContentControl.Content = mMainMenuControl;
            MainWindowHandler.GoToMainMenu();

        }

        void mWindowFormsHost_GotFocus( object sender, RoutedEventArgs e )
        {
            mOpenGLControl.Focus();
        }

            
        public MainWindow()
        {
            InitializeComponent();
            // make sure to show console before any call to the dll or we wont
            // see output
            ConsoleManager.Show();
            System.Windows.Application.Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;
#if DEBUG || true // Trolol
            System.Windows.Controls.MenuItem debugMenu = new System.Windows.Controls.MenuItem();
            debugMenu.Header = "Debug";
            MenuBar.Items.Add(debugMenu);

            {
                System.Windows.Controls.MenuItem SplitView = new System.Windows.Controls.MenuItem();
                SplitView.Header = "Split View";
                SplitView.Click += SplitView_Click;
                debugMenu.Items.Add(SplitView);
            }

            {
                System.Windows.Controls.MenuItem simulationMode = new System.Windows.Controls.MenuItem();
                simulationMode.Header = "Mode Simulation";
                simulationMode.Click += simulationMode_Click;
                debugMenu.Items.Add(simulationMode);
            }

            {
                System.Windows.Controls.MenuItem connexionServeurJeu = new System.Windows.Controls.MenuItem();
                connexionServeurJeu.Header = "Connexion Serveur Jeu";
                connexionServeurJeu.Click += connexionServeurJeu_Click;
                debugMenu.Items.Add(connexionServeurJeu);
            }

            {
                System.Windows.Controls.MenuItem requestGameCreationServeurJeu = new System.Windows.Controls.MenuItem();
                requestGameCreationServeurJeu.Header = "Demarrer Nouvelle Partie Serveur Jeu";
                requestGameCreationServeurJeu.Click += requestGameCreationServeurJeu_Click;
                debugMenu.Items.Add(requestGameCreationServeurJeu);
            }

            {
                System.Windows.Controls.MenuItem connexionPartieBidonServeurJeu = new System.Windows.Controls.MenuItem();
                connexionPartieBidonServeurJeu.Header = "Connexion Partie Serveur Jeu";
                connexionPartieBidonServeurJeu.Click += connexionPartieServeurJeu_Click;
                debugMenu.Items.Add(connexionPartieBidonServeurJeu);
            }

            {
                System.Windows.Controls.MenuItem testConnexionUDP = new System.Windows.Controls.MenuItem();
                testConnexionUDP.Header = "Test Connexion UDP";
                testConnexionUDP.Click += testConnexionUDP_Click;
                debugMenu.Items.Add(testConnexionUDP);
            }

            {
                System.Windows.Controls.MenuItem testPauseNetwork = new System.Windows.Controls.MenuItem();
                testPauseNetwork.Header = "Test Pause Game";
                testPauseNetwork.Click += requestGamePause_Click;
                debugMenu.Items.Add(testPauseNetwork);
            }

            {
                System.Windows.Controls.MenuItem testResumeNetwork = new System.Windows.Controls.MenuItem();
                testResumeNetwork.Header = "Test Resume Game";
                testResumeNetwork.Click += requestGameResume_Click;
                debugMenu.Items.Add(testResumeNetwork);
            }

            {
                System.Windows.Controls.MenuItem debugItem = new System.Windows.Controls.MenuItem();
                debugItem.Header = "Reload Models";
                debugItem.Click += ReloadModels_Click;
                debugMenu.Items.Add(debugItem);
            }

            {
                System.Windows.Controls.MenuItem debugItem = new System.Windows.Controls.MenuItem();
                debugItem.Header = "Test JSON";
                debugItem.Click += TestJSON_Click;
                debugMenu.Items.Add(debugItem);
            }

            

#endif

            InitDLL();
            SetAchievementUnlocked( mAchievementUnlockCallBack );

            this.Loaded += CreateUserControl;
            this.KeyDown += MainWindow_KeyDown;
            this.KeyUp += MainWindow_KeyUp;
        }
        [DllImport(@"RazerGame.dll")]
        public static extern void ReloadModels();
        void ReloadModels_Click(object sender, RoutedEventArgs e)
        {
            ReloadModels();
        }

        void TestJSON_Click(object sender, RoutedEventArgs e)
        {
            HttpManager wManager = new HttpManager();
            //wManager.getPublicMapList();
            //wManager.downloadMap(12, 1, TestCallbackMapDownloaded);
            wManager.uploadNewMap(12, "05237e69-8d18-11e2-b5d0-005056823b67", "TestMat4", "Test Upload HTTP", true, "D:\\AirHockeyGit\\log3900-04_Cloned2\\trunk\\Content\\cs_italy.xml", TestCallbackMapUploaded);

        }
        

        // Tests pour connection serveur jeu et client
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void connectServerGame(string pServerIP);

        // Tests pour connection sur une partie du serveur jeu
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void connectPartieServerGame(int pGameId);

        // Tests pour connection UDP
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void testConnexionUDPCSharp();

        // Tests pour demande de creation d'une partie sur le serveur jeu
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGameCreationServerGame(string pGameName);

        // Tests pour mise en pause
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGamePause();

        // Tests pour mise en pause
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGameResume();

        private void connexionServeurJeu_Click(object sender, RoutedEventArgs e)
        {
            // Tests pour connection serveur jeu et client
            connectServerGame("127.0.0.1");

        }

        private void connexionPartieServeurJeu_Click(object sender, RoutedEventArgs e)
        {
            // Tests pour connection serveur jeu et client
            connectPartieServerGame(1);

        }

        private void testConnexionUDP_Click(object sender, RoutedEventArgs e)
        {

            testConnexionUDPCSharp();

        }

        private void requestGameCreationServeurJeu_Click(object sender, RoutedEventArgs e)
        {
            // Tests pour la creation d'une partie sur le serveur jeu
            requestGameCreationServerGame("Bob's Game");

        }

        private void requestGamePause_Click(object sender, RoutedEventArgs e)
        {
            // Tests pour la creation d'une partie sur le serveur jeu
            requestGamePause();

        }

        private void requestGameResume_Click(object sender, RoutedEventArgs e)
        {
            // Tests pour la creation d'une partie sur le serveur jeu
            requestGameResume();

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


        private void ConnectToServer(object sender, RoutedEventArgs e)
        {
            mLoginWindow.ShowDialog();
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

        void LoadMapFromLocal(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.DialogLoadMapFromLocal();
        }

        void SaveMapToLocal(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.DialogSaveMapToLocal();
        }

        void QuickSaveMapToLocal(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.QuickSaveMapToLocal();
        }

        public void HandleEditionMenuItem(bool pMustBeEnabled)
        {
            mLoadMapItem.IsEnabled = pMustBeEnabled;
            mServerLoadMapItem.IsEnabled = pMustBeEnabled;
            mQuickSaveMapItem.IsEnabled = pMustBeEnabled;
            mSaveMapItem.IsEnabled = pMustBeEnabled;
            mServerSaveMapItem.IsEnabled = pMustBeEnabled;
            mResetMapItem.IsEnabled = pMustBeEnabled;
            mTestMapItem.IsEnabled = pMustBeEnabled;
        }


        [DllImport( @"RazerGame.dll" )]
        static extern void SetAchievementUnlocked( AchievementUnlockCallBack callback );

        public delegate int AchievementUnlockCallBack( AchievementsType id, IntPtr message );
        static AchievementUnlockCallBack mAchievementUnlockCallBack = AchievementUnlocked;

        static int AchievementUnlocked( AchievementsType id, IntPtr pMessage )
        {
            string achievementName = Marshal.PtrToStringAnsi( pMessage );
            DisplayAchievement( achievementName );
            return 1;
        }

        static AchievementUnlocked mAchievementPanel = null;
        static Storyboard mStoryboard = new Storyboard();
        static void mWaitAnimation_Completed( object sender, EventArgs e )
        {
            mAchievementPanel.Close();
        }

        static void DisplayAchievement(string achievementName)
        {
            if ( mAchievementPanel != null )
            {
                mAchievementPanel.Close();
            }
            mAchievementPanel = new AchievementUnlocked();
            mAchievementPanel.AchievementName = achievementName;
            mAchievementPanel.Topmost = true;
            mAchievementPanel.Left = MainWindowHandler.Context.Left + MainWindowHandler.Context.Width - mAchievementPanel.Width;
            mAchievementPanel.Top = MainWindowHandler.Context.Top + MainWindowHandler.Context.Height;
            mAchievementPanel.Show();

            DoubleAnimation beginAnimation = new DoubleAnimation();
            beginAnimation.To = mAchievementPanel.Top - ( mAchievementPanel.Height );
            beginAnimation.From = mAchievementPanel.Top;
            beginAnimation.AutoReverse = true;
            beginAnimation.Completed += mWaitAnimation_Completed;
            beginAnimation.Duration = new TimeSpan( 0, 0, 3 );
            Storyboard.SetTargetProperty( beginAnimation, new PropertyPath( Window.TopProperty ) );

            mStoryboard.Children.Add( beginAnimation );
            mStoryboard.Begin( mAchievementPanel, HandoffBehavior.SnapshotAndReplace );
        }
    }
}
