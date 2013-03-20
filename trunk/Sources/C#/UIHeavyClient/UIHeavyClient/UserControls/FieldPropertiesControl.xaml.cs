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

namespace UIHeavyClient.UserControls
{
    

    /// <summary>
    /// Interaction logic for FieldPropertiesControl.xaml
    /// </summary>
    public partial class FieldPropertiesControl : UserControl
    {
        List<UIElement> mPropertyPanels;

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

        public FieldPropertiesControl()
        {
            InitializeComponent();

            mPropertyPanels = new List<UIElement>(RazerUtilities.FindTypedChildren<UIElement>(mPropertiesStackPanel, false));

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
                })},
                {RazerKey.RAZER_KEY_BOOST,new Property("Boost",new List<UIElement>()
                {  
                    mScaleProperty,
                    mAccelerationProperty,
                    mPositionProperty,
                })},
                {RazerKey.RAZER_KEY_PORTAL,new Property("Portal",new List<UIElement>()
                {  
                    mPositionProperty,
                    mScaleProperty,
                    mAttractionProperty,
                })},
                {RazerKey.RAZER_KEY_PUCK,new Property("Puck",new List<UIElement>()
                {  
                    mPositionProperty,
                    mScaleProperty,
                })},
                {RazerKey.RAZER_KEY_MALLET,new Property("Mallet",new List<UIElement>()
                {  
                    mPositionProperty,
                    mScaleProperty,
                })},
                {RazerKey.RAZER_KEY_BONUS,new Property("Bonus",new List<UIElement>()
                {  
                    mPositionProperty,
                    //mBonusProperties, // risque de ne pas faire la modification specifique des bonus
                    mScaleProperty,
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
        }

        
    }
}
