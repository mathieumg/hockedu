﻿using System;
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

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour MainMenuControl.xaml
    /// </summary>
    public partial class MainMenuControl : UserControl
    {
        public MainMenuControl()
        {
            InitializeComponent();
        }

        private void quickPlayButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToPlayMode();
        }

        private void editionModeButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToEditionMode();
        }

        private void tournamentButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToTournamentMenu();
        }

        private void onlineModeButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToOnlineLobby();
        }

        private void optionButton_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToOptionsMenu();
        }
    }
}