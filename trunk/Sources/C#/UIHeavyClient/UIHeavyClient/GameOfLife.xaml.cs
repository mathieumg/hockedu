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

namespace UIHeavyClient
{
    /// <summary>
    /// Interaction logic for GameOfLife.xaml
    /// </summary>
    public partial class GameOfLife : UserControl
    {
        // Members
        private WindowsFormsHost mWindowsFormsHost;

        public GameOfLife()
        {
            InitializeComponent();
        }

        public void AppendOpenGL()
        {
            if (!mGrid.Children.Contains(mWindowsFormsHost))
            {
                mGrid.Children.Add(mWindowsFormsHost);
                mWindowsFormsHost.Focus();
            }
            Grid.SetColumnSpan(mWindowsFormsHost, 0);
            Grid.SetRowSpan(mWindowsFormsHost, 0);
            Grid.SetColumn(mWindowsFormsHost, 0);
            Grid.SetRow(mWindowsFormsHost, 0);
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn void EditionModeControl.RemoveOpenGL()
        ///
        /// Remove OpenGL from control.
        ///
        /// @return void.
        ////////////////////////////////////////////////////////////////////////
        public void RemoveOpenGL()
        {
            mGrid.Children.Remove(mWindowsFormsHost);
        }

    }
}
