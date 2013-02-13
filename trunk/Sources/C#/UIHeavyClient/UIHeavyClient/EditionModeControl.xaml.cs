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

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour EditionModeControl.xaml
    /// </summary>
    public partial class EditionModeControl : UserControl
    {
        public EditionModeControl(OpenGLControl pOpenGLControl)
        {
            InitializeComponent();
            windowsFormsHost1.Child = pOpenGLControl;
        }

        #region Edition Tool Events
        [DllImport(@"RazerGame.dll")]
        static extern bool ActionPerformed(string action);
        [DllImport(@"RazerGame.dll")]
        static extern bool IsGamePaused();
        [DllImport(@"RazerGame.dll")]
        static extern void PauseGame(bool doPause);

        [DllImport(@"RazerGame.dll")]
        static extern void GenerateDefaultField();
        [DllImport(@"RazerGame.dll")]
        static extern bool ValidateField();

        private void SetCameraFixe(object sender, RoutedEventArgs e)
        {
            ActionPerformed("CAMERA_FIXE");
        }

        private void SetCameraOrbite(object sender, RoutedEventArgs e)
        {
            ActionPerformed("CAMERA_ORBITE");
        }

        private void SetCameraLibre(object sender, RoutedEventArgs e)
        {
            ActionPerformed("CAMERA_LIBRE");
        }

        private void CameraToolMove(object sender, RoutedEventArgs e)
        {
            ActionPerformed("CAMERA");
        }

        private void CameraToolRotation(object sender, RoutedEventArgs e)
        {
            ActionPerformed("ORBIT");
        }

        private void CameraToolZoom(object sender, RoutedEventArgs e)
        {
            ActionPerformed("ZOOM_ELASTIQUE");
        }

        private void EditToolSelection(object sender, RoutedEventArgs e)
        {
            ActionPerformed("EDITEUR_SELECTION");
        }

        private void EditToolMove(object sender, RoutedEventArgs e)
        {
            ActionPerformed("EDITEUR_DEPLACER");
        }

        private void EditToolRotation(object sender, RoutedEventArgs e)
        {
            ActionPerformed("EDITEUR_ROTATION");
        }

        private void EditToolScale(object sender, RoutedEventArgs e)
        {
            ActionPerformed("EDITEUR_ECHELLE");
        }

        private void EditToolDuplicate(object sender, RoutedEventArgs e)
        {
            ActionPerformed("DUPLIQUER");
        }

        private void EditToolDelete(object sender, RoutedEventArgs e)
        {
            ActionPerformed("SUPPRIMER");
        }

        private void AddObjectWall(object sender, RoutedEventArgs e)
        {
            ActionPerformed("INSERER_MURET");
        }

        private void AddObjectPortal(object sender, RoutedEventArgs e)
        {
            ActionPerformed("INSERER_PORTAIL");
        }

        private void AddObjectBoost(object sender, RoutedEventArgs e)
        {
            ActionPerformed("INSERER_ACCELERATEUR");
        }

        private void AddObjectPuck(object sender, RoutedEventArgs e)
        {
            ActionPerformed("INSERER_RONDELLE");
        }

        private void AddObjectMallet(object sender, RoutedEventArgs e)
        {
            ActionPerformed("INSERER_MAILLET");
        }

        private void GoToEditionMode(object sender, RoutedEventArgs e)
        {
            if (ActionPerformed("ALLER_MODE_EDITION"))
            {
                mEditionModeButton.IsEnabled = false;
                mGameModeButton.IsEnabled = true;
                mDefaultFieldButton.IsEnabled = true;
                mAddObjectCombo.IsEnabled = true;
                mEditObjectCombo.IsEnabled = true;
            }
        }

        private void GoToGameMode(object sender, RoutedEventArgs e)
        {
            if (ValidateField())
            {
                if (ActionPerformed("ALLER_MODE_JEU"))
                {
                    mEditionModeButton.IsEnabled = true;
                    mGameModeButton.IsEnabled = false;
                    mDefaultFieldButton.IsEnabled = false;
                    mAddObjectCombo.IsEnabled = false;
                    mEditObjectCombo.IsEnabled = false;
                }
            }
        }

        private void OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            System.Windows.Controls.ComboBox box = sender as System.Windows.Controls.ComboBox;
            box.SelectedIndex = -1;
        }

        private void OnGenerateField(object sender, RoutedEventArgs e)
        {
            GenerateDefaultField();
        }
        #endregion

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }
    }
}
