using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Jayrock.Json;
using Jayrock.Json.Conversion;

using System.Net;
using System.IO;
using System.Collections.Specialized;
using System.Threading;


namespace HttpHockeduRequests
{
    
    public delegate void MapsListLoadedCallBack( List<UserMapDetailedJSON> pList );
    public delegate void MapDownloadedCallBack( string pFilepath );

    public class DownloadWorkerParams
    {
        public DownloadWorkerParams(int pUserId, int pMapId, MapDownloadedCallBack pCallback)
        {
            userId   = pUserId;
            mapId    = pMapId;
            callback = pCallback;
        }
        public int userId;
        public int mapId;
        public MapDownloadedCallBack callback;
    }


    public class HttpManagerWorker
    {

        public static void retreiveMapList(object pCallback)
        {
            MapsListLoadedCallBack wCallback = (MapsListLoadedCallBack)pCallback;
            string wData = HttpManager.getJsonFromRequest("http://hockedu.com/remote/listmaps");
            if (wData.Length == 0)
            {
                wCallback(new List<UserMapDetailedJSON>());
            }
            MapListJSON wMapList = (MapListJSON)JsonConvert.Import(typeof(MapListJSON), wData);

            // Si pas d'erreur
            if (wMapList.error == null)
            {
                wCallback(new List<UserMapDetailedJSON>(wMapList.maps));
            }
            else
            {
                wCallback(new List<UserMapDetailedJSON>());
            }
        }

        public static void downloadMap(object pParams)
        {
            DownloadWorkerParams wParams = (DownloadWorkerParams)pParams;
            int wUserId = wParams.userId;
            int wMapId = wParams.mapId;
            MapDownloadedCallBack wCallback = wParams.callback;

            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add("user_id", wUserId.ToString());
            wPostData.Add("map_id", wMapId.ToString());
            string wJsonData = HttpManager.getJsonFromRequest("http://hockedu.com/remote/getmap", wPostData);

            UserMapLightJSON wMapLight = (UserMapLightJSON)JsonConvert.Import(typeof(UserMapLightJSON), wJsonData);

            // On sauvegarde le data dans un fichier XML et on retourne le path
            if (wMapLight.error != null)
            {
                wCallback("");
            }
            else
            {
                string wCurrentDirectory = Directory.GetParent(Directory.GetCurrentDirectory()).ToString();
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
                       wCallback("");
                    }
                }
                wCallback(wDestinationFilePath); // Retourne le file path ou le fichier a ete sauvegarde en local
            }
        }
    }


    public class HttpManager
    {
        private WebClient mClient;
        public HttpManager()
        {
            mClient = new WebClient();
            System.Net.ServicePointManager.Expect100Continue = false;
        }

        

        public static string getJsonFromRequest(string pUrl, NameValueCollection pParams = null)
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


        public void downloadMap(int pUserId, int pMapId, MapDownloadedCallBack pCallback)
        {
            DownloadWorkerParams wParams = new DownloadWorkerParams(pUserId, pMapId, pCallback);
            Thread newThread = new Thread(new ParameterizedThreadStart(HttpManagerWorker.downloadMap));
            newThread.Start(wParams);
        }

        public void getPublicMapList(MapsListLoadedCallBack pCallbackFunction)
        {
            // Faire dans un thread separe
            Thread newThread = new Thread(new ParameterizedThreadStart(HttpManagerWorker.retreiveMapList));
            newThread.Start(pCallbackFunction);
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
