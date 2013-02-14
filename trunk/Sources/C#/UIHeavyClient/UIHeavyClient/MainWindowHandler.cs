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
    static class MainWindowHandler
    {
        static MainWindow mContext;

        public static MainWindow Context
        {
            get { return mContext; }
            set { mContext = value; }
        }

        public static void GoToEditionMode()
        {
            Context.WindowContentControl.Content = Context.EditionModeControl;
            Context.PlayModeControl.RemoveOpenGL();
            Context.EditionModeControl.AppendOpenGL();
        }

        public static void GoToPlayMode()
        {
            Context.WindowContentControl.Content = Context.PlayModeControl;
            Context.EditionModeControl.RemoveOpenGL();
            Context.PlayModeControl.AppendOpenGL();
        }

        public static void GoToMainMenu()
        {
            Context.WindowContentControl.Content = Context.MainMenuControl;
            Context.EditionModeControl.RemoveOpenGL();
            Context.PlayModeControl.RemoveOpenGL();
        }

        public static void GoToTournamentMenu()
        {
            Context.WindowContentControl.Content = Context.TournamentControl;
        }

        public static void GoToOnlineLobby()
        {
            Context.WindowContentControl.Content = Context.OnlineLobbyControl;
        }

        public static void GoToOptionsMenu()
        {
            Context.WindowContentControl.Content = Context.OptionsControl;
        }

        public static void GoToRadioOptions()
        {
            Context.WindowContentControl.Content = Context.RadioOptionControl;
        }

        public static void GoToAIOptions()
        {
            Context.WindowContentControl.Content = Context.AIOptionControl;
        }
    }
}
