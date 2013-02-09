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

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        

        //static System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();
        private OpenGLControl mOpenGLControl;
        public MainWindow()
        {
            InitializeComponent();
            //windowsFormsHost1.Loaded += CreateUserControl;
        }

//         public void CreateUserControl(object sender, EventArgs e)
//         {
//             mOpenGLControl = new OpenGLControl();
//             windowsFormsHost1.Child = mOpenGLControl;
//             
//         }

    }
}
