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
using System.Windows.Forms.Integration;

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour PlayModeControl.xaml
    /// </summary>
    public partial class PlayModeControl : UserControl
    {
        private WindowsFormsHost mWindowsFormsHost;

        public PlayModeControl(WindowsFormsHost pWindowsFormsHost)
        {
            InitializeComponent();
            mWindowsFormsHost = pWindowsFormsHost;
        }

        public void AppendOpenGL()
        {
            playControlGrid.Children.Add(mWindowsFormsHost);
            Grid.SetColumn(mWindowsFormsHost, 0);
            Grid.SetRow(mWindowsFormsHost, 0);
        }

        public void RemoveOpenGL()
        {
            playControlGrid.Children.Remove(mWindowsFormsHost);
        }
    }
}
