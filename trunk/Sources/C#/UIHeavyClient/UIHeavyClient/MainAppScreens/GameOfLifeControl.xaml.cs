using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms.Integration;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace UIHeavyClient.MainAppScreens
{
    /// <summary>
    /// Interaction logic for GameOfLifeControl.xaml
    /// </summary>
    public partial class GameOfLifeControl : UserControl, IRenderingControl
    {
        WindowsFormsHost mWindowsFormsHost;
        public GameOfLifeControl(WindowsFormsHost pWindowsFormsHost)
        {
            InitializeComponent();
            mWindowsFormsHost = pWindowsFormsHost;
        }

        public void ActivateRendering()
        {
            mGrid.Children.Add(mWindowsFormsHost);
            Grid.SetColumn(mWindowsFormsHost, 0);
            Grid.SetRow(mWindowsFormsHost, 0);
            Grid.SetColumnSpan(mWindowsFormsHost, 2);
            Grid.SetRowSpan(mWindowsFormsHost, 2);
        }

        public void DeactivateRendering()
        {
            mGrid.Children.Remove(mWindowsFormsHost);
        }
    }
}
