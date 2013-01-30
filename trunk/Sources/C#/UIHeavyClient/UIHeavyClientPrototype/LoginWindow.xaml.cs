///////////////////////////////////////////////////////////////////////////////
/// @file LoginWindow.xaml.cs
/// @author Vincent Lemire
/// @date 2013-01-28
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
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
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Threading;

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @struct Server
    /// @brief A server.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-29
    ///////////////////////////////////////////////////////////////////////////
    struct Server
    {
        public string mName;
        public string mIPAdress;
        public bool isAvailable;

        public Server(string pName, string pIPAdress)
        {
            mName = pName;
            mIPAdress = pIPAdress;
            isAvailable = true;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    /// @class LoginWindow
    /// @brief A simple window where the user will input his user name.
    ///        The server will be called to validate the name.
    ///
    /// @author Vincent Lemire
    /// @date 2013-01-28
    ///////////////////////////////////////////////////////////////////////////
    public partial class LoginWindow : Window
    {
        // sends a request to connect the user. Will not be necessarly connected when exiting this function
        // must wait for a callback indicating the status of this user's connection
        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RequestLogin( string pUsername, string pIpAdress );

        ////////////////////////////////////////////////////////////////////////////////////
        // Callback to received event messages from C++
        // declare the callback prototype
        // use this callback to validate user's connection
        public static LoginWindow mLoginWindow = null;
        public static ThreadData mData = new ThreadData(1);
        [DllImport(@"INF2990.dll")]
        static extern void SetEventCallback(MainWindow.EventReceivedCallBack callback);
        static bool EventReceived(int id, IntPtr pMessage)
        {
            if(id >= 0 && (int)MainWindow.EventType.NB_ELEM > id)
            {
                string message = Marshal.PtrToStringAnsi(pMessage);
                MainWindow.EventType type = (MainWindow.EventType)id;
                switch (type)
                {
                    case MainWindow.EventType.USER_CONNECTED:
                        mData.ExecuteTask(() =>
                        {
                            mLoginWindow.mUserName = mLoginWindow.userNameInput.Text;
                            mLoginWindow.mUserConnected = true;
                            Mouse.OverrideCursor = Cursors.Arrow;
                            mLoginWindow.Close(); 
                        });
                        break;
                    case MainWindow.EventType.USER_ALREADY_CONNECTED:
                        mData.ExecuteTask(() =>
                        {
                            mLoginWindow.errorMessageLabel.Content = "Ce pseudonyme est déjà utilisé.\nVeuillez en choisir un autre.";
                            mLoginWindow.UnBlockUIContent();
                        });
                        break;
                        break;
                    case MainWindow.EventType.USER_DID_NOT_SEND_NAME_ON_CONNECTION: break;
                    case MainWindow.EventType.USER_DISCONNECTED: break;
                    case MainWindow.EventType.RECONNECTION_TIMEOUT: break;
                    case MainWindow.EventType.RECONNECTION_IN_PROGRESS: break;
                    case MainWindow.EventType.WRONG_PASSWORD: break;
                    default: break;
                }
            }
            return true;
        }
        MainWindow.EventReceivedCallBack mEventCallback = EventReceived;
        ////////////////////////////////////////////////////////////////////////////////////

        // The user name input
        string mUserName = "";

        // The main window must know if the user is connected
        bool mUserConnected = false;

        // The server list
        Server[] listedServer;

        ////////////////////////////////////////////////////////////////////////
        /// @propertie string LoginWindow.UserName
        ///
        /// Propertie for the user name.
        ///
        /// @return The user name.
        ////////////////////////////////////////////////////////////////////////
        public string UserName
        {
            get { return mUserName; }
            set { mUserName = value; }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @propertie bool LoginWindow.UserConnected
        ///
        /// Propertie to know if user is connected.
        ///
        /// @return If the user is connected.
        ////////////////////////////////////////////////////////////////////////
        public bool UserConnected
        {
            get { return mUserConnected; }
            set { mUserConnected = value; }
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn LoginWindow.LoginWindow()
        ///
        /// Default constructor.
        ///
        /// @return None (constructor).
        ////////////////////////////////////////////////////////////////////////
        public LoginWindow()
        {
            mLoginWindow = this;
            SetEventCallback(mEventCallback);
            InitializeComponent();

            // Worker pour faire le rafraichissement de la fenetre
            BackgroundWorker mBgWorker = new BackgroundWorker();
            mBgWorker.WorkerReportsProgress = true;
            mBgWorker.DoWork +=
                new DoWorkEventHandler(bw_DoWork);
            mBgWorker.ProgressChanged +=
                new ProgressChangedEventHandler(bw_ProgressChanged);

            listedServer = new Server[]
            {
                new Server("Local", "127.0.0.1"),
                new Server("Chez Joe", "437.0.69.1"),
            };

            foreach (Server s in listedServer)
            {
                ComboBoxItem buffer = new ComboBoxItem();
                buffer.Content = s.mName + (s.isAvailable ? " (Actif)" : " (Inactif)");
                buffer.Background = serverComboBox.Background;
                buffer.Foreground = serverComboBox.Foreground;

                serverComboBox.Items.Add(buffer);
            }

            serverComboBox.SelectedIndex = 0;

            userNameInput.Focus();
        }


        private void bw_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            while (true)
            {
                if ((worker.CancellationPending == true))
                {
                    e.Cancel = true;
                    break;
                }
                else
                {
                    System.Threading.Thread.Sleep(100);
                    worker.ReportProgress(100);
                }
            }
        }
        private void bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            mUserName = userNameInput.Text;
            mUserConnected = true;
            Mouse.OverrideCursor = Cursors.Arrow;
            this.Close();
        }
        BackgroundWorker mBgWorker = new BackgroundWorker();

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginWindow.TryConnecting()
        ///
        /// Make necessary validations and registers inputs.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void TryConnecting()
        {
            // Block everything while connecting
            BlockUIContent();
            //mBgWorker.RunWorkerAsync(this);
            RequestLogin(userNameInput.Text, listedServer[serverComboBox.SelectedIndex].mIPAdress);
        }

        public void BlockUIContent()
        {
            userNameInput.IsEnabled = false;
            loginButton.IsEnabled = false;
            errorMessageLabel.Content = "Connecting to server, please wait...";
            serverComboBox.IsEnabled = false;
            Mouse.OverrideCursor = Cursors.Wait;
        }

        public void UnBlockUIContent()
        {
            // Unblock everything while connecting
            userNameInput.IsEnabled = true;
            loginButton.IsEnabled = true;
            serverComboBox.IsEnabled = true;
            Mouse.OverrideCursor = Cursors.Arrow;
        }
        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginWindow.loginButton_Click()
        ///
        /// Event when the connexion button is clicked.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void loginButton_Click(object sender, RoutedEventArgs e)
        {
            TryConnecting();
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void LoginWindow.messageTextBox_KeyDown()
        ///
        /// Event when the user pressed a key.
        /// 
        /// @param[in] object : The object related to the event.
        /// @param[in] RoutedEventArgs : The key event.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        private void messageTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            // If enter key is pressed, simulate a login button click
            if (e.Key == Key.Enter)
                loginButton_Click(sender, e);
        }

        public sealed class ThreadData : IDisposable
        {
            private readonly Dispatcher dispatcher;
            private readonly TaskScheduler scheduler;
            private readonly TaskFactory factory;
            private readonly CountdownEvent countdownEvent;

            // In this example, we initialize the countdown event with the total number
            // of child threads so that we know when all threads are finished scheduling
            // work.
            public ThreadData(int threadCount)
            {
                this.dispatcher = Dispatcher.CurrentDispatcher;
                SynchronizationContext context =
                    new DispatcherSynchronizationContext(this.dispatcher);
                SynchronizationContext.SetSynchronizationContext(context);
                this.scheduler = TaskScheduler.FromCurrentSynchronizationContext();
                this.factory = new TaskFactory(this.scheduler);
                this.countdownEvent = new CountdownEvent(threadCount);
            }

            // This method should be called by a child thread when it wants to invoke
            // an operation back on the main dispatcher thread.  This will block until
            // the method is done executing.
            public void ExecuteTask(Action action)
            {
                Task task = this.factory.StartNew(action);
                task.Wait();
            }

            // This method should be called by threads when they are done
            // scheduling work.
            public void OnThreadCompleted()
            {
                bool allThreadsFinished = this.countdownEvent.Signal();
                if (allThreadsFinished)
                {
                    this.dispatcher.InvokeShutdown();
                }
            }

            // This method should be called by the main thread so that it will begin
            // processing the work scheduled by child threads. It will return when
            // the dispatcher is shutdown.
            public void RunDispatcher()
            {
                Dispatcher.Run();
            }

            public void Dispose()
            {
                this.Dispose(true);
                GC.SuppressFinalize(this);
            }

            // Dispose all IDisposable resources.
            private void Dispose(bool disposing)
            {
                if (disposing)
                {
                    this.countdownEvent.Dispose();
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


