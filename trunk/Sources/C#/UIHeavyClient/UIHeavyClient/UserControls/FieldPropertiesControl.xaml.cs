using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
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

namespace UIHeavyClient.UserControls
{
    /// <summary>
    /// Interaction logic for FieldPropertiesControl.xaml
    /// </summary>
    public partial class FieldPropertiesControl : UserControl
    {
        List<UIElement> mPropertyPanels;

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        class FullProperties
        {
            public float mFriction;
            public float mZoneEditionX;
            public float mZoneEditionY;
            public float mScale;
            public float mAcceleration;
            public float mPositionX;
            public float mPositionY;
            public float mAttraction;
            public float mAngle;
            public float mRebound;
            public float mMinBonusSpawnTime;
            public float mMaxBonusSpawnTime;
            //public BonusProperties[] mBonusProperties;  // retirer pour le moment
            public int mPropertyFlagAssignment;
        }


        struct Property
        {
            public Property(string name, List<UIElement> boxes)
            {
                mName = name;
                mPropertyBoxes = boxes;
            }
            public List<UIElement> mPropertyBoxes;
            public string mName;
        }
        Dictionary<RazerKey, Property> mPropertiesByKey;
        FullProperties mFullProperties = new FullProperties();

        public FieldPropertiesControl()
        {
            InitializeComponent();

            mPropertyPanels = new List<UIElement>(RazerUtilities.FindTypedChildren<UIElement>(mPropertiesStackPanel, false));

            // hack pour ne pas prendre le save et refresh button dans cette liste
            mPropertyPanels.RemoveAt(mPropertyPanels.Count - 1);
            // Permet que notre structure pointe sur les meme données que le control de Bonus
            //mFullProperties.mBonusProperties = (BonusProperties[])mBonusProperties.mDataGrid.ItemsSource;

            // Initialise properties
            {
                mScaleProperty.LabelName = "Scale:";
                mScaleProperty.MinValue = 0.2f;
                mScaleProperty.MaxValue = 5;
                mScaleProperty.Increment = 0.1f;
                mScaleProperty.Value = 1;

                mBouncingProperty.LabelName = "Wall rebound ratio:";
                mBouncingProperty.MaxValue = 3;
                mBouncingProperty.Increment = 0.1f;
                mBouncingProperty.Value = 1;

                mAccelerationProperty.LabelName = "Acceleration ratio";
                mAccelerationProperty.MaxValue = 3;
                mAccelerationProperty.Increment = 0.1f;
                mAccelerationProperty.Value = 1;

                mAngleProperty.LabelName = "Angle:";
                mAngleProperty.MaxValue = 360;
                mAngleProperty.Increment = 5;
                mAngleProperty.Value = 0;

                mAttractionProperty.LabelName = "Attraction force:";
                mAttractionProperty.MaxValue = 3;
                mAttractionProperty.Increment = 0.1f;
                mAttractionProperty.Value = 1;

                mFrictionProperty.LabelName = "Friction:";
                mFrictionProperty.MaxValue = 1;
                mFrictionProperty.Increment = 0.1f;
                mFrictionProperty.Value = 0.5f;

                mZoneEditionX.LabelName = "X:";
                mZoneEditionX.MaxValue = 600;
                mZoneEditionX.MinValue = 75;
                mZoneEditionX.Increment = 5f;
                mZoneEditionX.Value = 200;

                mZoneEditionY.LabelName = "Y:";
                mZoneEditionY.MaxValue = 400;
                mZoneEditionY.MinValue = 50;
                mZoneEditionY.Increment = 5f;
                mZoneEditionY.Value = 150;

                mPositionX.LabelName = "X:";
                mPositionX.MaxValue = 600;
                mPositionX.MinValue = -600;
                mPositionX.Increment = 1f;
                mPositionX.Value = 0;

                mPositionY.LabelName = "Y:";
                mPositionY.MaxValue = 400;
                mPositionY.MinValue = -400;
                mPositionY.Increment = 1f;
                mPositionY.Value = 0;
            }

            mPropertiesByKey = new Dictionary<RazerKey, Property>()
            {
                {RazerKey.RAZER_KEY_NONE,new Property("Table",new List<UIElement>()
                {  
                    mBonusProperties,
                    mFrictionProperty,
                    mEditionZoneProperty,
                    mBouncingProperty,
                })},
                {RazerKey.RAZER_KEY_BOOST,new Property("Boost",new List<UIElement>()
                {  
                    mScaleProperty,
                    mAccelerationProperty,
                    mPositionProperty,
                    mAngleProperty,
                })},
                {RazerKey.RAZER_KEY_PORTAL,new Property("Portal",new List<UIElement>()
                {  
                    mPositionProperty,
                    mScaleProperty,
                    mAttractionProperty,
                    mAngleProperty,
                })},
                {RazerKey.RAZER_KEY_PUCK,new Property("Puck",new List<UIElement>()
                {  
                    mPositionProperty,
                    mScaleProperty,
                    mAngleProperty,
                })},
                {RazerKey.RAZER_KEY_MALLET,new Property("Mallet",new List<UIElement>()
                {  
                    mPositionProperty,
                    mScaleProperty,
                    mAngleProperty,
                })},
                {RazerKey.RAZER_KEY_BONUS,new Property("Bonus",new List<UIElement>()
                {  
                    mPositionProperty,
                    //mBonusProperties, // risque de ne pas faire la modification specifique des bonus
                    mScaleProperty,
                    mAngleProperty,
                })},
                /// Abuse le fait que ce type est utilisé uniquement pour les murets
                {RazerKey.RAZER_KEY_CONTROL_POINT,new Property("Wall",new List<UIElement>()
                {  
                    mPositionProperty,
                    mScaleProperty,
                    mBouncingProperty,
                    mAngleProperty,
                })},
                {RazerKey.RAZER_KEY_TABLE_CONTROL_POINT,new Property("Control Point",new List<UIElement>()
                {  
                    mPositionProperty,
                })},
            };

            DisplayProperties(RazerKey.RAZER_KEY_NONE);
        }

        public void DisplayProperties(RazerKey key)
        {
            foreach (var panel in mPropertyPanels)
            {
                panel.Visibility = Visibility.Collapsed;
            }
            if (!mPropertiesByKey.ContainsKey(key))
            {
                key = RazerKey.RAZER_KEY_NONE;
            }
            Property p = new Property();
            if (mPropertiesByKey.TryGetValue(key, out p))
            {
                mPropertiesGroupBox.Header = "Special Properties : " + p.mName;
                foreach (var panel in p.mPropertyBoxes)
                {
                    panel.Visibility = Visibility.Visible;
                }
            }
            GetData();
        }

        [DllImport(@"RazerGame.dll")]
        static extern bool GetFieldProperties( [In,Out] FullProperties fullProperties);
        [DllImport(@"RazerGame.dll")]
        static extern bool SendFieldProperties( [In,Out] FullProperties fullProperties);

        /// Pas capable d'envoyer l'array de bonus. Too bad for now
        [DllImport(@"RazerGame.dll")]
        static extern bool SendTest( [In,Out] BonusProperties[] fullProperties);


        /*
         * ex mFullProperties.mPropertyFlagAssignment = 
         *     ???? ???? ???? ??YX ???? ???? ???? ????
         *     
         * YX : bits désiré
         * X:Assigné, Y:Invalide
         * property = index of X
         * 
         * (mFullProperties.mPropertyFlagAssignment >> (int)property ) = 
         *  0000 0000 0000 0000 ???? ???? ???? ??YX
         * 
         * & 3  = 0000 0000 0000 0000 0000 0000 0000 00YX
         * 
         * == 1   =>  true if   X == 1 && Y == 0 
         */
        public bool IsPropertyValid(PropertyAssignmentValidation property)
        {
            return ((mFullProperties.mPropertyFlagAssignment >> (int)property )& 3 ) == 1;
        }

        public void GetData()
        {
            // resets the flags
            mFullProperties.mPropertyFlagAssignment = 0;

            var b = GetFieldProperties(mFullProperties);
            if (b)
            {
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_SCALE       ))  mScaleProperty.Value = mFullProperties.mScale;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_REBOUND     ))  mBouncingProperty.Value = mFullProperties.mRebound;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_ACCELERATION))  mAccelerationProperty.Value = mFullProperties.mAcceleration;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_ANGLE       ))  mAngleProperty.Value = mFullProperties.mAngle;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_ATTRACTION  ))  mAttractionProperty.Value = mFullProperties.mAttraction;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_FRICTION    ))  mFrictionProperty.Value = mFullProperties.mFriction;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_ZONE_X      ))  mZoneEditionX.Value = mFullProperties.mZoneEditionX;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_ZONE_Y      ))  mZoneEditionY.Value = mFullProperties.mZoneEditionY;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_POSITIONX   ))  mPositionX.Value = mFullProperties.mPositionX;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_POSITIONY   ))  mPositionY.Value = mFullProperties.mPositionY;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_BONUS_MIN   ))  mBonusProperties.mMinSpawnTime.Value = mFullProperties.mMinBonusSpawnTime;
                if(IsPropertyValid(PropertyAssignmentValidation.ASSIGNED_BONUS_MAX    ))  mBonusProperties.mMaxSpawnTime.Value = mFullProperties.mMaxBonusSpawnTime;
            }
            // resets the flags
            mFullProperties.mPropertyFlagAssignment = 0;
            mRefreshWarning.Visibility = Visibility.Collapsed;
        }
        
        public void RefreshData(object sender, RoutedEventArgs e)
        {
            GetData();
        }
        public void SendData(object sender, RoutedEventArgs e)
        {

            mFullProperties.mScale        = mScaleProperty.Value        ;
            mFullProperties.mRebound      = mBouncingProperty.Value     ;
            mFullProperties.mAcceleration = mAccelerationProperty.Value ;
            mFullProperties.mAngle        = mAngleProperty.Value        ;
            mFullProperties.mAttraction   = mAttractionProperty.Value   ;
            mFullProperties.mFriction     = mFrictionProperty.Value     ;
            mFullProperties.mZoneEditionX = mZoneEditionX.Value         ;
            mFullProperties.mZoneEditionY = mZoneEditionY.Value         ;
            mFullProperties.mPositionX    = mPositionX.Value            ;
            mFullProperties.mPositionY    = mPositionY.Value            ;
            mFullProperties.mMinBonusSpawnTime = mBonusProperties.mMinSpawnTime.Value;
            mFullProperties.mMaxBonusSpawnTime = mBonusProperties.mMaxSpawnTime.Value;

            SendFieldProperties(mFullProperties);

            // make sure to have valid updated info
            GetData();
        }
    }
}
