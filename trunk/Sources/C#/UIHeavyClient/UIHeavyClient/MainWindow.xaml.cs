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
        //[DllImport("../../../../../../Exe/DLL/INF2990.dll")]
        //public static extern int GlobalTestForCS(int a);

        public MainWindow()
        {
            InitializeComponent();
            //label1.Content = GlobalTestForCS(19);
        }
        
    }
}
