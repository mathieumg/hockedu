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

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour LoginWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {
        string mExpectedPassword = "TaMere";
        string mUserName = "";
        public string UserName
        {
            get { return mUserName; }
            set { mUserName = value; }
        }
        bool mUserConnected = false;
        public bool UserConnected
        {
            get { return mUserConnected; }
            set { mUserConnected = value; }
        }
        public LoginWindow()
        {
            InitializeComponent();
        }

        private void TryConnecting()
        {
            if (passwordInput.Password == mExpectedPassword)
            {
                mUserName = nickameInput.Text;
                mUserConnected = true;
                this.Close();
            }
            else
            {
                MessageBox.Show("Données de connexion invalides");
            }
        }

        private void loginButton_Click(object sender, RoutedEventArgs e)
        {
            TryConnecting();
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            
        }
    }
}
