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
    public class BonusProperties
    {
        public BonusProperties(string name)
        {
            this.Name = name;
        }

        public string Name
        {
            get;
            private set;
        }
        public bool Enabled
        {
            get;
            set;
        }
        public float Duration
        {
            get;
            set;
        }
        public int Type
        {
            get;
            set;
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
                Bonuses[i++] = (new BonusProperties(name) { Enabled=true, Duration=10, Type=(int)type });
            }

            InitializeComponent();
            

            mMinSpawnTime.LabelName = "Min Spawn Time:";
            mMaxSpawnTime.LabelName = "Max Spawn Time:";

            // delegate adding by code because the callback will be done
            // before both items have been initialised
            mMaxSpawnTime.InputBox.ValueChanged += SpawnTime_ValueChanged;
            mMinSpawnTime.InputBox.ValueChanged += SpawnTime_ValueChanged;

            ///// Desactivation de la grille pour le moment car je n'arrive pas a envoyé les données
            mDataGrid.Visibility = Visibility.Collapsed;
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
            mMaxSpawnTime.Value = top;
            mMinSpawnTime.Value = bot;
        }
    }
}
