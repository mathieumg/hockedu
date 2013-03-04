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
using System.Windows.Forms.Integration;

namespace UIHeavyClient
{
    /// <summary>
    /// Logique d'interaction pour EditionModeControl.xaml
    /// </summary>
    public partial class EditionModeControl : UserControl
    {
        private WindowsFormsHost mWindowsFormsHost;

        Dictionary<object, string> mGuidanceMessages;
        Dictionary<object, string> mGuidanceInstructions;
        Dictionary<object, ActionType> mActionPerformedStrings;

        EventReceivedCallBack mEventCallBack = EditionModeEventReceived;
        public EventReceivedCallBack EventCallBack
        {
            get{return mEventCallBack;}
        }

        static bool EditionModeEventReceived(EventCodes id, IntPtr pMessage)
        {
            EditionModeControl control = MainWindowHandler.Context.EditionModeControl;
            if (control != null)
            {
                string message = Marshal.PtrToStringAnsi(pMessage);
                switch (id)
                {
                    case EventCodes.ENABLE_PUCK_CREATION:
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            control.mPuckButton.IsEnabled = true;
                        });

                        break;
                    case EventCodes.DISABLE_PUCK_CREATION:
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            control.mPuckButton.IsEnabled = false;
                        });
                        break;
                    case EventCodes.ENABLE_MALLET_CREATION:
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            control.mMalletButton.IsEnabled = true;
                        });

                        break;
                    case EventCodes.DISABLE_MALLET_CREATION:
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            control.mMalletButton.IsEnabled = false;
                        });
                        break;
                    default:
                        break;
                }
            }
            return true;
        }


        public EditionModeControl(WindowsFormsHost pWindowsFormsHost)
        {
            InitializeComponent();
            mWindowsFormsHost = pWindowsFormsHost;

            mGuidanceMessages = new Dictionary<object, string>() 
            { 
                {mDeleteButton, "Delete selected objects"},
                {mCopyButton, "Copy selected objects"},
                {mUndoButton, "Cancel your last action"},
                {mRedoButton, "Redo the action you just cancelled"},

                {mPuckButton, "The puck for the hockey game (you can only create one)"},
                {mMalletButton, "The mallets for the hockey game (you can only create two)"},
                {mWallButton, "A wall that will block the puck"},
                {mBoosterButton, "A booster that will give an acceleration to the puck"},
                {mPortalButton, "Will warp the puck to another portal on the map"},
                {mBonusButton, "A random bonus that will sometime appear"},

                {mFreeStateRadio, "Move the camera"},
                {mMoveStateRadio, "Move selected objects"},
                {mRotateStateRadio, "Rotate selected objects"},
                {mScaleStateRadio, "Scale selected objects"},
                {mSelectStateRadio, "Select objects"},
                {mZoomStateRadio, "Zoom with the camera"},

                {mFreeCameraRadio, "Free camera that can move anywhere in the 3D map"},
                {mOrbitalCameraRadio, "Camera that can only turn around a fixed point"},
                {mSkyCameraRadio, "Fixed camera above the map"},
            };

            mGuidanceInstructions = new Dictionary<object, string>()
            {
                {mDeleteButton, "Objects deleted! Use the Undo button if you want those objects back."},
                {mCopyButton, "Objects copied!"},
                {mUndoButton, "Action undid!"},
                {mRedoButton, "Action redid!"},

                {mPuckButton, "Click on the map to place the puck. You may only place one."},
                {mMalletButton, "Click on the map to place the mallets. You may only place two."},
                {mWallButton, "Click on two points on the map to create a wall linking them. A wall will block the puck."},
                {mBoosterButton, "Click on the map to place a booster. A booster will accelerate the puck."},
                {mPortalButton, "Click on the map to place a portal. The puck will warp between the placed portals. You must place at least two of them to experiment their effect."},
                {mBonusButton, "Click on the map to choose a spot where a random bonus could appear. A random bonus can ???"},

                {mFreeStateRadio, "Drag the mouse to move tha camera."},
                {mMoveStateRadio, "Drag the mouse to move the selected objects."},
                {mRotateStateRadio, "Drag the mouse to rotate selected objects."},
                {mScaleStateRadio, "Drag the mouse to scale selected objects."},
                {mSelectStateRadio, "Click on objects to select them."},
                {mZoomStateRadio, "Drag the mouse to zoom. Draw a smaller rectangle to make a bigger zoom!"},

                {mFreeCameraRadio, "Free camera activated! Use the mouse and the arrow keys to move wherever you want."},
                {mOrbitalCameraRadio, "Orbital camera activated! Use the mouse to turn the camera around the fixed point."},
                {mSkyCameraRadio, "Sky camera activated! This camera is fixed, choose another one if you want to move it!"},
            };

            mActionPerformedStrings = new Dictionary<object, ActionType>()
            {
                {mDeleteButton, ActionType.ACTION_SUPPRIMER},
                {mCopyButton, ActionType.ACTION_DUPLIQUER},
                //{mUndoButton, ActionType.ACTION_},
                //{mRedoButton, ActionType.ACTION_},

                {mPuckButton, ActionType.ACTION_INSERER_RONDELLE},
                {mMalletButton, ActionType.ACTION_INSERER_MAILLET},
                {mWallButton, ActionType.ACTION_INSERER_MURET},
                {mBoosterButton, ActionType.ACTION_INSERER_ACCELERATEUR},
                {mPortalButton, ActionType.ACTION_INSERER_PORTAIL},
                //{mBonusButton, ActionType.ACTION_},

                {mFreeStateRadio, ActionType.ACTION_ORBIT}, // ActionType.ACTION_CAMERAActionType.ACTION_
                {mMoveStateRadio, ActionType.ACTION_EDITEUR_DEPLACER},
                {mRotateStateRadio, ActionType.ACTION_EDITEUR_ROTATION},
                {mScaleStateRadio, ActionType.ACTION_EDITEUR_ECHELLE},
                {mSelectStateRadio, ActionType.ACTION_EDITEUR_SELECTION},
                {mZoomStateRadio, ActionType.ACTION_ZOOM_ELASTIQUE},

                {mFreeCameraRadio, ActionType.ACTION_CAMERA_LIBRE},
                {mOrbitalCameraRadio, ActionType.ACTION_CAMERA_ORBITE},
                {mSkyCameraRadio, ActionType.ACTION_CAMERA_FIXE},
            };
        }

        #region Edition Tool Events

        [DllImport(@"RazerGame.dll")]
        static extern bool ActionPerformed(ActionType action);
        [DllImport(@"RazerGame.dll")]
        static extern bool IsGamePaused();
        [DllImport(@"RazerGame.dll")]
        static extern void PauseGame(bool doPause);

        [DllImport(@"RazerGame.dll")]
        static extern void GenerateDefaultField();
        [DllImport(@"RazerGame.dll")]
        static extern bool ValidateField();

        private void CallActionPerformed(object sender, RoutedEventArgs e)
        {
            if (mActionPerformedStrings != null)
            {
                ActionPerformed(mActionPerformedStrings[sender]);
            }

            DisplayGuidanceInstructions(sender, e);
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

        public void AppendOpenGL()
        {
            if (!editionControlGrid.Children.Contains(mWindowsFormsHost))
            {
                editionControlGrid.Children.Add(mWindowsFormsHost);
            }
            Grid.SetColumnSpan(mWindowsFormsHost, 3);
            Grid.SetRowSpan(mWindowsFormsHost, 2);
            Grid.SetColumn(mWindowsFormsHost, 1);
            Grid.SetRow(mWindowsFormsHost, 1);
        }

        public void RemoveOpenGL()
        {
            editionControlGrid.Children.Remove(mWindowsFormsHost);
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        private void DisplayGuidanceInstructions(object sender, RoutedEventArgs e)
        {
            if (mGuidanceInstructions != null && mGuidanceTextBlock != null)
            {
                mGuidanceTextBlock.Text = mGuidanceInstructions[sender];
            }
        }

        private void DisplayGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = mGuidanceMessages[sender];
        }

        private void ClearGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = "";
        }
    }
}
