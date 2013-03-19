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

namespace UIHeavyClient
{
    /// <summary>
    /// Interaction logic for BonusEditionControl.xaml
    /// </summary>
    public partial class BonusEditionControl : UserControl
    {
        public struct BonusProperties
        {
            public BonusProperties(string n, bool e, float d)
            {
                mName = n;
                mEnabled = e;
                mDuration = d;
            }
            private string mName;
            private bool mEnabled;
            private float mDuration;
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

        public List<BonusProperties> Bonuses = new List<BonusProperties>();

        public BonusEditionControl()
        {

            Array values = Enum.GetValues(typeof(BonusType));
            foreach (BonusType type in values )
            {
                if(type == BonusType.NB_BONUS_TYPE)
                    break;
                var NameFull = type.ToString();
                var name = NameFull.Substring(11, NameFull.Length-11).Replace('_', ' ').ToLowerInvariant();
                Bonuses.Add(new BonusProperties(name, true, 10));
            }

            InitializeComponent();
            mDataGrid.ItemsSource = Bonuses;
        }
    }
}
