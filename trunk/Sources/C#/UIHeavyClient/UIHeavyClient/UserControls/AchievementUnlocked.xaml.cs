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
using System.Windows.Shapes;

namespace UIHeavyClient.UserControls
{
    /// <summary>
    /// Interaction logic for AchievementUnlocked.xaml
    /// </summary>
    public partial class AchievementUnlocked : Window
    {
        public AchievementUnlocked()
        {
            InitializeComponent();
        }

        public string AchievementName
        {
            get { return (string) mAchievementName.Content; }
            set { mAchievementName.Content = value; }
        }
    }
}
