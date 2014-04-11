///////////////////////////////////////////////////////////////////////////////
/// @file AIOptionControl.xaml.cs
/// @author Vincent Lemire
/// @date 2013-01-28
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

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

namespace UIHeavyClient
{
    ///////////////////////////////////////////////////////////////////////////
    /// @class CreditPopup
    /// @brief A credit window.
    ///
    /// @author Vincent Lemire
    /// @date 2013-04-06
    ///////////////////////////////////////////////////////////////////////////
    public partial class CreditPopup : Window
    {
        // Attributes
        private string mCreditText;

        ////////////////////////////////////////////////////////////////////////
        /// @fn CreditPopup.CreditPopup()
        ///
        /// Constructor.
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public CreditPopup()
        {
            InitializeComponent();

            mCreditText = "Credits";
            mCreditText += "";
        }
    }
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
