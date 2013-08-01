using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace UIHeavyClient
{
    class TcpPing
    {

        public static bool ping(string address)
        {
            return RunApp("tcping.exe", address+" 25566");
        }

        private static bool RunApp(string InstallApp, string InstallArgs)
        {
            System.Diagnostics.Process installProcess = new System.Diagnostics.Process();
            //settings up parameters for the install process
            installProcess.StartInfo.FileName = InstallApp;
            installProcess.StartInfo.Arguments = InstallArgs;
            installProcess.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;

            installProcess.Start();

            installProcess.WaitForExit();
            // Check for sucessful completion
            return (installProcess.ExitCode == 0) ? true : false;
        }
    }
}
