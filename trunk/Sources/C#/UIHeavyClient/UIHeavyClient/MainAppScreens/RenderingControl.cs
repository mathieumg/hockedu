using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows.Forms.Integration;

namespace UIHeavyClient.MainAppScreens
{
    public interface IRenderingControl
    {
        void ActivateRendering();
        void DeactivateRendering();
    }
}
