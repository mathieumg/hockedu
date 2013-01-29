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
using System.Threading;
using System.ComponentModel;

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitDLL(string username);

        string mUserName;
        LoginWindow mLoginWindow;
        bool mIsUserConnected;

        public string UserName
        {
            get { return mUserName; }
            set { mUserName = value; }
        }

        public bool IsUserConnected
        {
            get { return mIsUserConnected; }
            set { mIsUserConnected = value; }
        }


        ////////////////////////////////////////////////////////////////////////////////////
        //Callback to received event from C++
        //declare the callback prototype
        delegate bool MessageReceivedCallBack(IntPtr username, IntPtr message);
        [DllImport(@"INF2990.dll")]
        static extern void SetCallback(MessageReceivedCallBack callback);
        static bool MessageReceived(IntPtr pUsername, IntPtr pMessage)
        {
            string message = Marshal.PtrToStringAnsi(pMessage);
            string username = Marshal.PtrToStringAnsi(pUsername);
            Chat.UpdateChat(username, message);
            return true;
        }
        MessageReceivedCallBack mCallback = MessageReceived;
        ////////////////////////////////////////////////////////////////////////////////////

        private void bw_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            while(true)
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
            ShowWholeMessage();
        }
        BackgroundWorker mBgWorker = new BackgroundWorker();


        public MainWindow()
        {
            InitializeComponent();

            mUserName = "";
            mIsUserConnected = false;
            Chat.mContext = this;

            mLoginWindow = new LoginWindow();
            mLoginWindow.ShowDialog();
            IsUserConnected = mLoginWindow.UserConnected;
            UserName = mLoginWindow.UserName;
            if (!IsUserConnected)
            {
                Close();
            }
            SetCallback(mCallback);
            InitDLL(UserName);

            // Worker pour faire le rafraichissement de la fenetre
            BackgroundWorker mBgWorker = new BackgroundWorker();
            mBgWorker.WorkerReportsProgress = true;
            mBgWorker.DoWork +=
                new DoWorkEventHandler(bw_DoWork);
            mBgWorker.ProgressChanged +=
                new ProgressChangedEventHandler(bw_ProgressChanged);
            mBgWorker.RunWorkerAsync();
        }

        private void submitButton_Click(object sender, RoutedEventArgs e)
        {
            if (messageTextBox.Text != "" && mIsUserConnected)
            {
                Chat.SendNewMessage(mUserName, messageTextBox.Text);
                messageTextBox.Clear();
            }
        }

        public void ShowWholeMessage()
        {
            wholeMessageBox.Text = Chat.WholeMessage;
        }

        private void messageTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                submitButton_Click(sender, e);
        }

       

        private void Window_Closed(object sender, EventArgs e)
        {
            mLoginWindow.Close();
        }
        
    }
}
