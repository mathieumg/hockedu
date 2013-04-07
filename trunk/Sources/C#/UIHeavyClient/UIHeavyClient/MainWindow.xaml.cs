///////////////////////////////////////////////////////////////////////////////
/// @file MainWindow.xaml.cs
/// @author Vincent Lemire and Micheal Ferris
/// @date 2013-02-26
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

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
using System.Diagnostics;
using System.Collections;

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class MainWindow
    /// @brief The chat window.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class MainWindow : Window
    {
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

        private CreditPopup mCreditPopup;

        private SaveServerMapPrompt mSaveServerMapPrompt;
        private ServerMapPrompt mServerMapPrompt;

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

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.TestCallbackMapDownloaded()
        ///
        /// Mao download test.
        /// 
        /// @param[in] string : The map local path.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void TestCallbackMapDownloaded(string pOutputPath)
        {
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                Console.WriteLine(pOutputPath);
            });
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.TestCallbackMapUploaded()
        ///
        /// Upload map test.
        /// 
        /// @param[in] UploadOperationStatus : status.
        /// @param[in] int : map id.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void TestCallbackMapUploaded(HttpHockeduRequests.UploadOperationStatus pStatus, int pMapId)
        {
            Console.WriteLine(pStatus);
            Console.WriteLine(pMapId);
        }
        public static void TestCallbackAchievementsUploaded( HttpHockeduRequests.UploadOperationStatus pStatus )
        {
            Console.WriteLine( pStatus );
        }
        public void TestCallbackAchievementsDownloaded( DownloadOperationStatus pStatus)
        {
            Console.WriteLine( pStatus );
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.Window_Closed()
        ///
        /// Close event.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] EventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void Window_Closed(object sender, EventArgs e)
        {
            OpenGLControl.mRenderTimer.Stop();
            OpenGLControl.mLogicalTimer.Stop();
            MainWindowHandler.Cleanup();
            FreeApplicationMemory();
            ConsoleManager.Hide();
            System.Windows.Application.Current.Shutdown();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.CreateUserControl()
        ///
        /// Initialize everything.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] EventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void CreateUserControl(object sender, EventArgs e)
        {
            InitDLL();

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

            mCreditPopup = new CreditPopup();

            mSaveServerMapPrompt = new SaveServerMapPrompt();
            mServerMapPrompt = new ServerMapPrompt();

            this.WindowContentControl.Content = mMainMenuControl;
            MainWindowHandler.InitCallbacks();
            MainWindowHandler.GoToMainMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.mWindowFormsHost_GotFocus()
        ///
        /// On focus, try giving focus to OpenGL.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void mWindowFormsHost_GotFocus( object sender, RoutedEventArgs e )
        {
            mOpenGLControl.Focus();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.MainWindow()
        ///
        /// Main constructor.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public MainWindow()
        {
            InitializeComponent();
            // make sure to show console before any call to the dll or we wont
            // see output
            ConsoleManager.Show();

            System.Windows.Application.Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;
#if !SHIPPING
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
            
            {
                System.Windows.Controls.MenuItem debugItem = new System.Windows.Controls.MenuItem();
                debugItem.Header = "Switch Play Mode";
                debugItem.Click += SwitchPlayMode;
                debugMenu.Items.Add( debugItem );
            }

            {
                System.Windows.Controls.MenuItem debugItem=new System.Windows.Controls.MenuItem();
                debugItem.Header="Test Trajectory Prediction";
                debugItem.Click+=TestTrajectoryPrediction;
                debugMenu.Items.Add(debugItem);
            }

			{
                System.Windows.Controls.MenuItem debugItem = new System.Windows.Controls.MenuItem();
                debugItem.Header = "Reset Achievements";
                debugItem.Click += ResetAchievements;
                debugMenu.Items.Add(debugItem);
            }
            {
                System.Windows.Controls.MenuItem debugItem = new System.Windows.Controls.MenuItem();
                debugItem.Header = "Test Send Achievements";
                debugItem.Click += TestAchievementUpload_Click;
                debugMenu.Items.Add( debugItem );
            }
            {
                System.Windows.Controls.MenuItem debugItem = new System.Windows.Controls.MenuItem();
                debugItem.Header = "Test Get Achievements";
                debugItem.Click += TestAchievementDownload_Click;
                debugMenu.Items.Add( debugItem );
            }
#endif
            SetAchievementUnlocked( mAchievementUnlockCallBack );

            this.Loaded += CreateUserControl;
            this.KeyDown += MainWindow_KeyDown;
            this.KeyUp += MainWindow_KeyUp;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.SwitchPlayMode()
        ///
        /// Go to play mode.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void SwitchPlayMode( object sender, RoutedEventArgs e )
        {
            MainWindowHandler.GoToPlayMode( ActionType.ACTION_ALLER_MODE_JEU );
        }
        void ResetAchievements(object sender, RoutedEventArgs e)
        {
            ResetAchievements();
        }
        [DllImport(@"RazerGame.dll")]
        public static extern void ResetAchievements();


        [DllImport(@"RazerGame.dll")]
        public static extern void TestTrajectoryPredictionDLL();
        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.SwitchPlayMode()
        ///
        /// Go to play mode.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void TestTrajectoryPrediction(object sender, RoutedEventArgs e)
        {
            TestTrajectoryPredictionDLL();
        }


        [DllImport(@"RazerGame.dll")]
        public static extern void ReloadModels();

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.ReloadModels_Click()
        ///
        /// Reload model.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void ReloadModels_Click(object sender, RoutedEventArgs e)
        {
            ReloadModels();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.TestJSON_Click()
        ///
        /// JSON test.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void TestJSON_Click(object sender, RoutedEventArgs e)
        {
            HttpManager wManager = new HttpManager();
            //wManager.getPublicMapList();
            //wManager.downloadMap(12, 1, TestCallbackMapDownloaded);
            wManager.sendMap(12, "cd13d808-9e93-11e2-b5d0-005056823b67", "TestMat9000", "UPDATE DESCRIPTION", true, "E:\\airhockeygit\\log3900-04_DO_NOT_MODIFY\\trunk\\Content\\Exe\\bobMapGrosMailet.xml", 15, TestCallbackMapUploaded);
        }

        public static void UploadAchievement()
        {
            HttpManager wManager = new HttpManager();
            wManager.uploadAchievements( LoginControl.mLoginInfo.mUserId, LoginControl.mLoginInfo.mAuthKey, TestCallbackAchievementsUploaded );
        }

        void TestAchievementUpload_Click( object sender, RoutedEventArgs e )
        {
            if ( LoginControl.mLoginInfo.mAuthOnWeb )
            {
                UploadAchievement();
            }
            else
            {
                WebLogin.CreateWebLoginWindow( UploadAchievement );
            }
        }
        void TestAchievementDownload_Click( object sender, RoutedEventArgs e )
        {
            HttpManager wManager = new HttpManager();
            wManager.downloadAchievements( "12", "cd13d808-9e93-11e2-b5d0-005056823b67", TestCallbackAchievementsDownloaded );
        }
        
        // Tests pour connection serveur jeu et client
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void connectServerGame(string pServerIP);

        // Tests pour connection sur une partie du serveur jeu
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void connectPartieServerGame(int pGameId, uint pServerId, string pInputPassword);

        // Tests pour connection UDP
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void testConnexionUDPCSharp();

        // Tests pour demande de creation d'une partie sur le serveur jeu
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGameCreationServerGame(string pGameName, string pMapName, int pMapId, string pPassword);

        // Tests pour mise en pause
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGamePause();

        // Tests pour mise en pause
        [DllImport(@"RazerGame.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void requestGameResume();

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.connexionServeurJeu_Click()
        ///
        /// Connection to game server.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void connexionServeurJeu_Click(object sender, RoutedEventArgs e)
        {
            // Tests pour connection serveur jeu et client
            connectServerGame("127.0.0.1");

        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.connexionPartieServeurJeu_Click()
        ///
        /// Connection to game server.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void connexionPartieServeurJeu_Click(object sender, RoutedEventArgs e)
        {
            // Tests pour connection serveur jeu et client
            connectPartieServerGame(1, 1, "");
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.testConnexionUDP_Click()
        ///
        /// UDP test.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void testConnexionUDP_Click(object sender, RoutedEventArgs e)
        {
            testConnexionUDPCSharp();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.requestGameCreationServeurJeu_Click()
        ///
        /// Game creation.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void requestGameCreationServeurJeu_Click(object sender, RoutedEventArgs e)
        {
            // Tests pour la creation d'une partie sur le serveur jeu
            requestGameCreationServerGame("Bob's Game", "MapEnCours.xml", 0, "");
        }


        [DllImport( @"RazerGame.dll" )]
        static extern bool ActionPerformed( ActionType action );

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.PauseGameClick()
        ///
        /// Pause game.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void PauseGameClick(object sender, RoutedEventArgs e)
        {
            ActionPerformed( ActionType.ACTION_PAUSE_JEU );
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.RestartGameClick()
        ///
        /// Game restart.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void RestartGameClick(object sender, RoutedEventArgs e)
        {
            ActionPerformed( ActionType.ACTION_REINITIALISER_PARTIE );
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.ResetPuckClick()
        ///
        /// Puck reset in case it is stuck.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ResetPuckClick(object sender, RoutedEventArgs e)
        {
            ActionPerformed( ActionType.ACTION_REINITIALISER_RONDELLE );
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.ReplayClick()
        ///
        /// 10 seconds replay.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void ReplayClick(object sender, RoutedEventArgs e)
        {
            ActionPerformed( ActionType.ACTION_REPLAY );
        }


        [DllImport(@"RazerGame.dll", CallingConvention=CallingConvention.Cdecl)]
        private static extern void AskForAIOpponentInNetworkGame();
        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.AddAIOpponentClick()
        ///
        /// Ask for an AI opponent when the oppenent disconnects
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void AddAIOpponentClick(object sender, RoutedEventArgs e)
        {
            AskForAIOpponentInNetworkGame();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.simulationMode_Click()
        ///
        /// Simulations for tests.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void simulationMode_Click(object sender, RoutedEventArgs e)
        {
            // Temp
            WindowContentControl.Content = PlayModeControl;
            EditionModeControl.RemoveOpenGL();
            PlayModeControl.AppendOpenGL();
            MainWindowHandler.ActionPerformed(ActionType.ACTION_ALLER_MODE_SIMULATION);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.SplitView_Click()
        ///
        /// Spliting view for tests.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void SplitView_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.ActionPerformed(ActionType.ACTION_CAMERA_SPLIT);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.BackToMainMenu()
        ///
        /// Return to main menu.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void BackToMainMenu(object sender, RoutedEventArgs e)
        {
            MessageBoxResult dr = System.Windows.MessageBox.Show("Are you sure?", "Return to main menu", MessageBoxButton.YesNo);

            if (dr == MessageBoxResult.Yes)
            {
                MainWindowHandler.GoToMainMenu();
            }   
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.OpenWebSite()
        ///
        /// Open Hockedu Web page.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void OpenWebSite(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.hockedu.com");
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.MainWindow_KeyUp()
        ///
        /// When a key is released.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] KeyEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.ConvertKeyEvent()
        ///
        /// Converter for key event.
        /// 
        /// @param[out] RoutedEventArgs : The event.
        ///
        /// @return KeyEventArgs.
        ////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.MainWindow_KeyDown()
        ///
        /// When a key is pressed.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] KeyEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
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

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.LoadMapFromLocal()
        ///
        /// Load a local map.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void LoadMapFromLocal(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.DialogLoadMapFromLocal();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.SaveMapToLocal()
        ///
        /// Local map save.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void SaveMapToLocal(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.DialogSaveMapToLocal();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.QuickSaveMapToLocal()
        ///
        /// Save local map.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void QuickSaveMapToLocal(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.QuickSaveMapToLocal();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.GetEditionUniqueElements()
        ///
        /// Get edition elements that must collapse.
        ///
        /// @return List<UIElement>.
        ////////////////////////////////////////////////////////////////////////
        public List<UIElement> GetEditionUniqueElements()
        {
            return new List<UIElement>() { mLoadMapItem, mLoadMapItem, mServerLoadMapItem, mQuickSaveMapItem, mSaveMapItem, mServerSaveMapItem, mResetMapItem, };
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.GetPlayUniqueElements()
        ///
        /// Get play elements that must collapse.
        ///
        /// @return List<UIElement>.
        ////////////////////////////////////////////////////////////////////////
        public List<UIElement> GetPlayUniqueElements()
        {
            return new List<UIElement>() { mGameActionsMenu, };
        }

        [DllImport( @"RazerGame.dll" )]
        static extern void SetAchievementUnlocked( AchievementUnlockCallBack callback );

        public delegate int AchievementUnlockCallBack( AchievementsType id, IntPtr message );
        static AchievementUnlockCallBack mAchievementUnlockCallBack = AchievementUnlocked;


        static bool AchievementDisplaying = false;
        static ArrayList mAchievementQueued = new ArrayList();
        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.AchievementUnlocked()
        ///
        /// Handle achievements.
        /// 
        /// @param[in] AchievementsType : Achievement id.
        /// @param[in] IntPtr : Achievement message.
        ///
        /// @return int.
        ////////////////////////////////////////////////////////////////////////
        static int AchievementUnlocked( AchievementsType id, IntPtr pMessage )
        {
            string achievementName = Marshal.PtrToStringAnsi( pMessage );
            DisplayAchievement( achievementName );
            return 1;
        }

        static AchievementUnlocked mAchievementPanel = null;
        static Storyboard mStoryboard = new Storyboard();

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.mWaitAnimation_Completed()
        ///
        /// Close achievement panel.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] EventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        static void mWaitAnimation_Completed( object sender, EventArgs e )
        {
            mAchievementPanel.Close();
            AchievementDisplaying = false;
            if (mAchievementQueued.Count != 0)
            {
                string name = mAchievementQueued[0] as string;
                mAchievementQueued.RemoveAt(0);
                DisplayAchievement(name);
            }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.DisplayAchievement()
        ///
        /// Display achievement panel.
        /// 
        /// @param[in] string : Achievement name.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        static void DisplayAchievement(string achievementName)
        {
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                if (AchievementDisplaying)
                {
                    mAchievementQueued.Add(achievementName);
                    return;
                }
                AchievementDisplaying = true;
                if (mAchievementPanel != null)
                {
                    mAchievementPanel.Close();
                }
                mAchievementPanel = new AchievementUnlocked();
                mAchievementPanel.AchievementName = achievementName;
                mAchievementPanel.Topmost = true;
                mAchievementPanel.Left = MainWindowHandler.Context.Left + MainWindowHandler.Context.Width - mAchievementPanel.Width;
                mAchievementPanel.Top = MainWindowHandler.Context.Top /*+ MainWindowHandler.Context.Height - mAchievementPanel.Height*/;
                mAchievementPanel.Show();

                DoubleAnimation beginAnimation = new DoubleAnimation();
                beginAnimation.From = 0;// mAchievementPanel.Top;
                beginAnimation.To = mAchievementPanel.Height;// mAchievementPanel.Top - ( mAchievementPanel.Height );
                beginAnimation.AutoReverse = true;
                beginAnimation.Completed += mWaitAnimation_Completed;
                beginAnimation.Duration = new TimeSpan(0, 0, 5);
                Storyboard.SetTargetProperty(beginAnimation, new PropertyPath(Window.HeightProperty));
                mAchievementPanel.Height = 0;

                mStoryboard.Children.Add(beginAnimation);
                mStoryboard.Begin(mAchievementPanel, HandoffBehavior.SnapshotAndReplace);
            });
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.ExitHockedu()
        ///
        /// Exit event.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void ExitHockedu(object sender, RoutedEventArgs e)
        {
            Close();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.DefaultMap()
        ///
        /// Reset to default map.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void DefaultMap(object sender, RoutedEventArgs e)
        {
            EditionModeControl.CallDefaultFieldFromMenu();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.ShowCreditsPopup()
        ///
        /// Credits.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        void ShowCreditsPopup(object sender, RoutedEventArgs e)
        {
            mCreditPopup.ShowDialog();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.SaveMapToServer()
        ///
        /// Send a map to server.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void SaveMapToServer(object sender, RoutedEventArgs e)
        {            
            if (LoginControl.mLoginInfo.mAuthOnWeb)
            {
                HttpRequestForSavingMap();
            }
            else
            {
                WebLogin.CreateWebLoginWindow(HttpRequestForSavingMap);
            } 
        }

        private void HttpRequestForSavingMap()
        {
            mSaveServerMapPrompt = new SaveServerMapPrompt();
            mSaveServerMapPrompt.GiveFocus();
            mSaveServerMapPrompt.ShowDialog();

            if (mSaveServerMapPrompt.OkIsClicked)
            {
                string mapPath = MainWindowHandler.CurrentMap == "" ? "tempFileMap.xml" : MainWindowHandler.CurrentMap;

                MainWindowHandler.SaveMapToLocal(mapPath);

                HttpManager wHttpManager = new HttpManager();
                wHttpManager.sendMap(Convert.ToInt32(LoginControl.mLoginInfo.mUserId), 
                    LoginControl.mLoginInfo.mAuthKey, 
                    mSaveServerMapPrompt.MapName, 
                    mSaveServerMapPrompt.MapDescription, 
                    mSaveServerMapPrompt.IsPublic,
                    mapPath, 
                    MainWindowHandler.MapId, 
                    DisplayMapSavedOnServer);
            }

            mSaveServerMapPrompt.Close();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.SaveMapToServer()
        ///
        /// Send a map to server.
        /// 
        /// @param[in] object : The sender.
        /// @param[in] RoutedEventArgs : The event.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        private void LoadMapFromServer(object sender, RoutedEventArgs e)
        {

            if (LoginControl.mLoginInfo.mAuthOnWeb)
            {
                HttpRequestForLoadingMap();
            }
            else
            {
                WebLogin.CreateWebLoginWindow(HttpRequestForLoadingMap);
            }
        }

        private void HttpRequestForLoadingMap()
        {
            mServerMapPrompt = new ServerMapPrompt();
            mServerMapPrompt.GetServerMaps();
            mServerMapPrompt.ShowDialog();

            if (mServerMapPrompt.OkIsClicked)
            {
                if (mServerMapPrompt.SelectedMap != null)
                {
                    HttpManager wHttpManager = new HttpManager();
                    wHttpManager.downloadMap(Convert.ToInt32(LoginControl.mLoginInfo.mUserId), mServerMapPrompt.SelectedMap.id, HandleDownloadedMap);
                }
            }

            mServerMapPrompt.Close();
        }

        public void HandleDownloadedMap(string pFilepath, int pMapId)
        {
            // Load the map to edition mode
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                MainWindowHandler.MapId = pMapId;
                MainWindowHandler.LoadMapFromLocal(pFilepath);
            });
        }

        public static void DisplayMapSavedOnServer(UploadOperationStatus pStatus, int pMapid)
        {
            MainWindowHandler.mTaskManager.ExecuteTask(() =>
            {
                MainWindowHandler.MapId = pMapid;
                if (pStatus == UploadOperationStatus.UPLOAD_SUCCESS)
                {
                    MainWindowHandler.Context.EditionModeControl.SetGuidanceInstuction("Your map has been saved online");
                }
                else
                {
                    MainWindowHandler.Context.EditionModeControl.SetGuidanceInstuction("Operation failed, please try again");
                }
            });
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void MainWindow.RestartGameMenuHandle()
        ///
        /// Show/Hide the restart game option.
        /// 
        /// @param[in] bool : hide or show.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void RestartGameMenuHandle(bool pMustBeCollapse)
        {
            mRestartGameMenuItem.Visibility = pMustBeCollapse ? Visibility.Collapsed : Visibility.Visible;
        }

        public void ReplayMenuHandle(bool pMustBeCollapse)
        {
            mReplayItem.Visibility=pMustBeCollapse?Visibility.Collapsed:Visibility.Visible;
        }

        public void AddAIOpponentHandle(bool pMustBeCollapse)
        {
            mAiOpponentItem.Visibility=pMustBeCollapse?Visibility.Collapsed:Visibility.Visible;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


