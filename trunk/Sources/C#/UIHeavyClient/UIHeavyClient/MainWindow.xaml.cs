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

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int TestCSCall(int a);


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

        public MainWindow()
        {
            InitializeComponent();

            mUserName = "";
            mIsUserConnected = false;

            Chat.mContext = this;

            mLoginWindow = new LoginWindow(this);
            mLoginWindow.ShowDialog();
        }

        private void submitButton_Click(object sender, RoutedEventArgs e)
        {
            if (messageTextBox.Text != "" && mIsUserConnected)
            {
                Chat.UpdateChat(mUserName, messageTextBox.Text);
                messageTextBox.Clear();
                ShowWholeMessage();
            }
        }

        private void ShowWholeMessage()
        {
            wholeMessageBox.Text = Chat.WholeMessage;
        }

        private void messageTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                submitButton_Click(sender, e);
        }

        private void messageTextBox_TouchEnter(object sender, TouchEventArgs e)
        {

        }

        private void Window_Closed(object sender, EventArgs e)
        {
            mLoginWindow.Close();
        }
        
    }
}
