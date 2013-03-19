using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
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
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct BonusProperties
    {
        public BonusProperties(string n, bool e, float d, BonusType type)
        {
            mName = n;
            mEnabled = e;
            mDuration = d;
            mType = type;
        }
        
        private string mName;
        private bool mEnabled;
        private float mDuration;
        private BonusType mType;

        public string Name
        {
            get { return mName; }
            private set { mName = value; }
        }
        public bool Enabled
        {
            get { return mEnabled; }
            set { mEnabled = value; }
        }
        public float Duration
        {
            get { return mDuration; }
            set { mDuration = value; }
        }
    }

    /// <summary>
    /// Interaction logic for BonusEditionControl.xaml
    /// </summary>
    public partial class BonusEditionControl : UserControl
    {
        public BonusProperties[] Bonuses = new BonusProperties[(int)BonusType.NB_BONUS_TYPE];

        public BonusEditionControl()
        {
            Array values = Enum.GetValues(typeof(BonusType));
            int i = 0;
            foreach (BonusType type in values )
            {
                if(type == BonusType.NB_BONUS_TYPE)
                    break;
                var NameFull = type.ToString();
                var name = NameFull.Substring(11, NameFull.Length-11).Replace('_', ' ').ToLowerInvariant();
                Bonuses[i++] = (new BonusProperties(name, true, 10,type));
            }

            InitializeComponent();

            // delegate adding by code because the callback will be done
            // before both items have been initialised
            mMaxSpawnTime.ValueChanged += SpawnTime_ValueChanged;
            mMinSpawnTime.ValueChanged += SpawnTime_ValueChanged;

            mDataGrid.ItemsSource = Bonuses;
        }

        private static bool IsTextAllowed(string text)
        {
            Regex regex = new Regex("[^0-9.-]+"); //regex that matches disallowed text
            return !regex.IsMatch(text);
        }

        private void mMinTextBox_PreviewTextInput_1(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !IsTextAllowed(e.Text);
        }

        // Use the DataObject.Pasting Handler 
        private void TextBoxPasting(object sender, DataObjectPastingEventArgs e)
        {
            if (e.DataObject.GetDataPresent(typeof(String)))
            {
                String text = (String)e.DataObject.GetData(typeof(String));
                if (!IsTextAllowed(text))
                {
                    e.CancelCommand();
                }
            }
            else
            {
                e.CancelCommand();
            }
        }

        private void SpawnTime_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            var top = Math.Max((float)mMinSpawnTime.Value, (float)mMaxSpawnTime.Value);
            var bot = Math.Min((float)mMinSpawnTime.Value, (float)mMaxSpawnTime.Value);
            mMaxSpawnTime.Value = new decimal(top);
            mMinSpawnTime.Value = new decimal(bot);
        }
    }
}
