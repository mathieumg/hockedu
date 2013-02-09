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
            ConsoleManager.Show();
            windowsFormsHost1.Loaded += CreateUserControl;
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            ConsoleManager.Hide();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            mOpenGLControl.Visible = !mOpenGLControl.Visible;
            
            System.Windows.Controls.Button b = sender as System.Windows.Controls.Button;
            if (mOpenGLControl.Visible)
            {
                b.Content = "Hide";
            }
            else
            {
                b.Content = "Show";
            }
        }

        public void CreateUserControl(object sender, EventArgs e)
        {
            mOpenGLControl = new OpenGLControl();
            windowsFormsHost1.Child = mOpenGLControl;
            
        }

    }
}
