using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Jayrock.Json;
using Jayrock.Json.Conversion;

using System.Net;
using System.IO;
using System.Collections.Specialized;

namespace HttpHockeduRequests
{
    


    public class HttpManager
    {
        private WebClient mClient;
        public HttpManager()
        {
            mClient = new WebClient();
            System.Net.ServicePointManager.Expect100Continue = false;
        }

        public string getJsonFromRequest(string pUrl, NameValueCollection pParams = null)
        {
            WebClient client = new WebClient();
            if(pParams != null)
            {
                byte[] wResponse = client.UploadValues(pUrl, pParams);
                return Encoding.ASCII.GetString(wResponse);
            }
            else
            {
                return client.DownloadString(pUrl);
            }
            
        }




        public string downloadMap(int pUserId, int pMapId, string pAuthKey = "")
        {
            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add("user_id", pUserId.ToString());
            wPostData.Add("map_id", pMapId.ToString());
            string wJsonData = getJsonFromRequest("http://hockedu.com/remote/getmap", wPostData);

            UserMapLightJSON wMapLight = (UserMapLightJSON)JsonConvert.Import(typeof(UserMapLightJSON), wJsonData);

            // On sauvegarde le data dans un fichier XML et on retourne le path
            if(wMapLight.error != null)
            {
                return "";
            }
            else
            {
                string wCurrentDirectory = Directory.GetParent(Directory.GetCurrentDirectory()).ToString() + "asdasd";
                string wDestinationFilePath = wCurrentDirectory + Path.DirectorySeparatorChar + wMapLight.name + ".xml";
                try
                {
                    System.IO.File.WriteAllText(wDestinationFilePath, wMapLight.content);
                }
                catch (System.IO.IOException)
                {
                    try
                    {
                        // Si erreur, on le met dans C:temp/
                        wDestinationFilePath = "C:" + Path.DirectorySeparatorChar + "temp" + Path.DirectorySeparatorChar + wMapLight.name + ".xml";
                        System.IO.File.WriteAllText(wDestinationFilePath, wMapLight.content);
                    }
                    catch (Exception)
                    {
                        // Si encore une erreur, on retourne rien
                    	return "";
                    }
                }
                return wDestinationFilePath; // Retourne le file path ou le fichier a ete sauvegarde en local
            }
        }

        public List<UserMapDetailedJSON> getPublicMapList()
        {
            string wData = getJsonFromRequest("http://hockedu.com/remote/listmaps");
            if(wData.Length == 0)
            {
                return new List<UserMapDetailedJSON>();
            }
            MapListJSON wMapList = (MapListJSON)JsonConvert.Import(typeof(MapListJSON), wData);

            // Si pas d'erreur
            if (wMapList.error == null)
            {
                return new List<UserMapDetailedJSON>(wMapList.maps);
            }
            else
            {
                return new List<UserMapDetailedJSON>();
            }
        }
        public List<UserMapDetailedJSON> getUserPublicMapList(int pUserId)
        {
            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add("user_id", pUserId.ToString());
            string wData = getJsonFromRequest("http://hockedu.com/remote/listmaps", wPostData);

            if(wData.Length == 0)
            {
                return new List<UserMapDetailedJSON>();
            }
            MapListJSON wMapList = (MapListJSON)JsonConvert.Import(typeof(MapListJSON), wData);

            // Si pas d'erreur
            if (wMapList.error == null)
            {
                return new List<UserMapDetailedJSON>(wMapList.maps);
            }
            else
            {
                return new List<UserMapDetailedJSON>();
            }
        }



        public List<UserMapDetailedJSON> getUserMapList(int pUserId, string pAuthentication)
        {
            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add("user_id", pUserId.ToString());
            wPostData.Add("auth_key", pAuthentication.ToString());
            string wData = getJsonFromRequest("http://hockedu.com/remote/listmaps", wPostData);

            if(wData.Length == 0)
            {
                return new List<UserMapDetailedJSON>();
            }
            MapListJSON wMapList = (MapListJSON)JsonConvert.Import(typeof(MapListJSON), wData);

            // Si pas d'erreur
            if (wMapList.error == null)
            {
                return new List<UserMapDetailedJSON>(wMapList.maps);
            }
            else
            {
                return new List<UserMapDetailedJSON>();
            }
        }
    }
}
