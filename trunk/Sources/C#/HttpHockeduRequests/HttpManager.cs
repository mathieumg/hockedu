using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Jayrock.Json;
using Jayrock.Json.Conversion;

using System.Net;
using System.IO;

namespace HttpHockeduRequests
{
    public class HttpManager
    {

        public string getJsonFromRequest(string pUrl)
        {
            StringBuilder wStringBuffer = new StringBuilder();
            byte[] wBuf = new byte[10240];

            HttpWebRequest wRequest = (HttpWebRequest)WebRequest.Create(pUrl);
            HttpWebResponse wResponse = (HttpWebResponse)wRequest.GetResponse();

            Stream wResponseStream = wResponse.GetResponseStream();
            string wBufferString = null;
            int wCount = 0;
            do
            {
                // On remplit le buffer
                wCount = wResponseStream.Read(wBuf, 0, wBuf.Length);

                // Verifie si on lit quelque chose
                if (wCount != 0)
                {
                    // Conversion du stream de byte en ASCII
                    wBufferString = Encoding.ASCII.GetString(wBuf, 0, wCount);

                    // Append a la fin
                    wStringBuffer.Append(wBufferString);
                }
            }
            while (wCount > 0); // Boucle tant qu'il y a des valeurs a lire dans le stream

            // Fin de la lecture
            return wStringBuffer.ToString();
        }





        public void retreiveUserListFromJson()
        {
            // Liste d'objets
            UserList wList = (UserList)JsonConvert.Import(typeof(UserList), getJsonFromRequest("http://hockedu.com/ajax/listusers"));
            
            Console.WriteLine(wList.ToString());
        }
    }
}
