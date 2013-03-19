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
    /// Interaction logic for NumericalInputBox.xaml
    /// </summary>
    public partial class NumericalInputBox : UserControl
    {
        public string LabelName
        {
            get { return (string)InputLabel.Content; }
            set { InputLabel.Content = value; }
        }
        public float Value
        {
            get
            {
                if (InputBox.Value != null)
                {
                    return (float)InputBox.Value;
                }
                return 0;
            }
            set
            {
                InputBox.Value = new decimal(value);
            }
        }

        public float MinValue
        {
            get
            {
                if (InputBox.Minimum != null)
                {
                    return (float)InputBox.Minimum;
                }
                return 0;
            }
            set
            {
                InputBox.Minimum = new decimal(value);
            }
        }

        public float MaxValue
        {
            get
            {
                if (InputBox.Maximum != null)
                {
                    return (float)InputBox.Maximum;
                }
                return 0;
            }
            set
            {
                InputBox.Maximum = new decimal(value);
            }
        }

        public float Increment
        {
            get
            {
                if (InputBox.Increment != null)
                {
                    return (float)InputBox.Increment;
                }
                return 0;
            }
            set
            {
                InputBox.Increment = new decimal(value);
            }
        }

        public NumericalInputBox()
        {
            InitializeComponent();
        }
    }
}
