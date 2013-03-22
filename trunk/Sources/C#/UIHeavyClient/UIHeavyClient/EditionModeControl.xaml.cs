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

        Button mLastClickedButton;

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
                    case EventCodes.THERE_ARE_NODES_SELECTED:
                        RazerKey key = GetSelectedNodeUniqueKey();
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            if (TerrainHasDeletable())
                            {
                                control.mDeleteButton.IsEnabled = true;
                            }
                            else
                            {
                                control.mDeleteButton.IsEnabled = false;
                            }
                            control.mPropertiesGroupBox.DisplayProperties(key);
                        });
                        break;
                    case EventCodes.THERE_ARE_NO_NODE_SELECTED:
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                            control.mDeleteButton.IsEnabled = false;
                            control.mPropertiesGroupBox.DisplayProperties(RazerKey.RAZER_KEY_NONE);
                        });
                        break;
                    case EventCodes.CAN_UNDO:
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                        control.mUndoButton.IsEnabled = true;
                        });
                        break;
                    case EventCodes.CANNOT_UNDO:
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                        control.mUndoButton.IsEnabled = false;
                        });
                        break;
                    case EventCodes.CAN_REDO:
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                        control.mRedoButton.IsEnabled = true;
                        });
                        break;
                    case EventCodes.CANNOT_REDO:
                        MainWindowHandler.mTaskManager.ExecuteTask(() =>
                        {
                        control.mRedoButton.IsEnabled = false;
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
            mWindowsFormsHost.Focus();

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

                {mFreeStateButton, "Move the camera"},
                {mMoveStateButton, "Move selected objects"},
                {mRotateStateButton, "Rotate selected objects"},
                {mScaleStateButton, "Scale selected objects"},
                {mSelectStateButton, "Select objects"},
                {mZoomStateButton, "Zoom with the camera"},

                {mFreeCameraRadio, "Free camera that can move anywhere in the 3D map"},
                {mOrbitalCameraRadio, "Camera that can only turn around a fixed point"},
                {mSkyCameraRadio, "Fixed camera above the map"},
                {mCamareResetButton, "Re-center the camera on the map"},

                {mMainMenuButton, "Return to main menu"},
                {mDefaultFieldButton, "Reset the map to default"},
                {mValidateFieldButton, "Will validate if your map is certified to be played on"},
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

                {mFreeStateButton, "Drag the mouse to move tha camera."},
                {mMoveStateButton, "Drag the mouse to move the selected objects."},
                {mRotateStateButton, "Drag the mouse to rotate selected objects."},
                {mScaleStateButton, "Drag the mouse to scale selected objects."},
                {mSelectStateButton, "Click on objects to select them."},
                {mZoomStateButton, "Drag the mouse to zoom. Draw a smaller rectangle to make a bigger zoom!"},

                {mFreeCameraRadio, "Free camera activated! Use the mouse and the arrow keys to move wherever you want."},
                {mOrbitalCameraRadio, "Orbital camera activated! Use the mouse to turn the camera around the fixed point."},
                {mSkyCameraRadio, "Sky camera activated! This camera is fixed, choose another one if you want to move it!"},
            };

            mActionPerformedStrings = new Dictionary<object, ActionType>()
            {
                {mDeleteButton, ActionType.ACTION_SUPPRIMER},
                {mCopyButton, ActionType.ACTION_DUPLIQUER},
                {mUndoButton, ActionType.ACTION_EDITEUR_UNDO},
                {mRedoButton, ActionType.ACTION_EDITEUR_REDO},

                {mPuckButton, ActionType.ACTION_INSERER_RONDELLE},
                {mMalletButton, ActionType.ACTION_INSERER_MAILLET},
                {mWallButton, ActionType.ACTION_INSERER_MURET},
                {mBoosterButton, ActionType.ACTION_INSERER_ACCELERATEUR},
                {mPortalButton, ActionType.ACTION_INSERER_PORTAIL},
                {mBonusButton, ActionType.ACTION_INSERER_BONUS},

                {mFreeStateButton, ActionType.ACTION_ORBIT}, // ActionType.ACTION_CAMERAActionType.ACTION_
                {mMoveStateButton, ActionType.ACTION_EDITEUR_DEPLACER},
                {mRotateStateButton, ActionType.ACTION_EDITEUR_ROTATION},
                {mScaleStateButton, ActionType.ACTION_EDITEUR_ECHELLE},
                {mSelectStateButton, ActionType.ACTION_EDITEUR_SELECTION},
                {mZoomStateButton, ActionType.ACTION_ZOOM_ELASTIQUE},

                {mFreeCameraRadio, ActionType.ACTION_CAMERA_LIBRE},
                {mOrbitalCameraRadio, ActionType.ACTION_CAMERA_ORBITE},
                {mSkyCameraRadio, ActionType.ACTION_CAMERA_FIXE},
            };

            var buttons = RazerUtilities.FindTypedChildren<Button>(this, true);
            // delete button is disabled by default and it is assigned before the callback can be seted
            mDeleteButton.Foreground = Brushes.Black;
            foreach (var button in buttons)
            {
                button.Background = Brushes.Black;
                button.Foreground = Brushes.White;

                button.IsEnabledChanged += button_IsEnabledChanged;
                button.MouseEnter += DisplayGuidanceMessages;
                button.MouseLeave += ClearGuidanceMessages;

                /// the button will no longer flash after a click
                button.Focusable = false;
            }
        }

        void button_IsEnabledChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            Control control = sender as Control;
            if ((bool)e.NewValue)
            {
                control.Foreground = Brushes.White;
            }
            else
            {
                control.Foreground = Brushes.Black;
            }
        }

        #region Edition Tool Events

        // Event
        [DllImport(@"RazerGame.dll")]
        static extern bool ActionPerformed(ActionType action);
        [DllImport(@"RazerGame.dll")]
        static extern bool IsGamePaused();
        [DllImport(@"RazerGame.dll")]
        static extern void PauseGame(bool doPause);
        [DllImport(@"RazerGame.dll")]
        static extern bool TerrainHasDeletable();
        [DllImport(@"RazerGame.dll")]
        static extern RazerKey GetSelectedNodeUniqueKey();
        [DllImport(@"RazerGame.dll")]
        static extern void ResetCamera();

        // Field
        [DllImport(@"RazerGame.dll")]
        static extern void GenerateDefaultField();
        [DllImport(@"RazerGame.dll")]
        static extern bool ValidateField();


        private void HandleDeleteButton(object sender, RoutedEventArgs e)
        {
            MessageBoxResult dr = MessageBox.Show("Are you sure?", "Delete", MessageBoxButton.YesNo);

            if (dr == MessageBoxResult.Yes)
            {
                CallActionPerformed(sender, e);
            }
        }

        private void HandleStateButton(object sender, RoutedEventArgs e)
        {
            if(sender is Button)
                ChangeClickedButton((sender as Button));

            CallActionPerformed(sender, e);
        }

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
            MessageBoxResult dr = MessageBox.Show("Are you sure?", "Default Map", MessageBoxButton.YesNo);

            if (dr == MessageBoxResult.Yes)
            {            
                GenerateDefaultField();
            }
        }

        private void OnValidateField(object sender, RoutedEventArgs e)
        {
            if (ValidateField())
            {
                MessageBoxResult dr = MessageBox.Show("Map is valid to play", "Map Validation", MessageBoxButton.OK);
            }
        }
        
        #endregion

        private void MainMenuBtnClick(object sender, RoutedEventArgs e)
        {
            MainWindowHandler.GoToMainMenu();
        }

        public void AppendOpenGL()
        {
            if (!editionControlGrid.Children.Contains(mWindowsFormsHost))
            {
                editionControlGrid.Children.Add(mWindowsFormsHost);
                mWindowsFormsHost.Focus();
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

        private void DisplayGuidanceInstructions(object sender, RoutedEventArgs e)
        {
            if (mGuidanceInstructions != null && mGuidanceTextBlock != null)
            {
                string m;
                if (mGuidanceInstructions.TryGetValue(sender, out m))
                {
                    SetGuidanceInstuction(m);
                }
            }
        }

        private void DisplayGuidanceMessages(object sender, MouseEventArgs e)
        {
            string m;
            if (mGuidanceMessages.TryGetValue(sender,out m))
            {
                mGuidanceLabel.Content = m;
            }
        }

        private void ClearGuidanceMessages(object sender, MouseEventArgs e)
        {
            mGuidanceLabel.Content = "";
        }

        public void InitButtons()
        {
            HandleStateButton(mFreeStateButton, new RoutedEventArgs());
        }

        private void ChangeClickedButton(Button pButton)
        {
            if (mLastClickedButton != null)
            {
                mLastClickedButton.Background = Brushes.Black;
            }

            pButton.Background = new SolidColorBrush(Color.FromRgb(0x00, 0xD5, 0x00));
            mLastClickedButton = pButton;
        }

        private void mCamareResetButton_Click(object sender, RoutedEventArgs e)
        {
            ResetCamera();
        }

        public void SetGuidanceInstuction(string pInstruction)
        {
            mGuidanceTextBlock.Text = pInstruction;
        }
    }
}
