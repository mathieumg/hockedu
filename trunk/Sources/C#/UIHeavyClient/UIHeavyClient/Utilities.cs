using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace UIHeavyClient
{
    class Utilities
    {

        ////////////////////////////////////////////////////////////////////////
        /// @fn static bool IsIPv4(string value)
        ///
        /// Methode pour savoir si une adresse IP est une adresse IPv4 valide (test de syntaxe)
        /// 
        /// @param[in] string   value    : Adrese IP a tester
        ///
        /// @return bool : Valide ou non
        ////////////////////////////////////////////////////////////////////////
        public static bool IsIPv4(string value)
        {
            IPAddress address;

            Func<char, bool> myFunc = c => c == '.';
            int nbDot = value.Count(myFunc);
            if (nbDot == 3 && IPAddress.TryParse(value, out address))
            {
                if (address.AddressFamily == AddressFamily.InterNetwork)
                {
                    return true;
                }
            }
            return false;
        }



    }
}
