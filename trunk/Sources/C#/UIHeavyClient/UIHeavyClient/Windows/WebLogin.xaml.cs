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
    public delegate void LoginCallback();
    /// <summary>
    /// Interaction logic for WebLogin.xaml
    /// </summary>
    public partial class WebLogin : Window
    {
        static WebLogin instance = null;
        LoginCallback mLoginCallback = null;
        public LoginControl mLoginControl;


        /// can be null
        public static WebLogin getInstance()
        {
            return instance;
        }
        public static void CreateWebLoginWindow( LoginCallback loginCallback )
        {
            if ( instance != null )
            {
                if(LoginControl.mCurrentRequest != null )
                {
                    LoginControl.mCurrentRequest.CancelRequest();
                }
            }
            else
            {
                instance = new WebLogin();
            }

            instance.mLoginCallback = loginCallback;
            instance.Show();
        }
        private WebLogin(  )
        {
            InitializeComponent();

            instance = this;
            mLoginControl = new LoginControl( Completed, true );
            mLoginContente.Content = mLoginControl;
        }

        public static void Completed()
        {
            MainWindowHandler.mTaskManager.ExecuteTask( () =>
            {
                Mouse.OverrideCursor = Cursors.Arrow;
                if ( instance != null )
                {
                    instance.Hide();
                    if ( instance.mLoginCallback != null )
                    {
                        instance.mLoginCallback();
                    }
                    instance.Close();
                }
                instance = null;
            } );
        }
    }
}
