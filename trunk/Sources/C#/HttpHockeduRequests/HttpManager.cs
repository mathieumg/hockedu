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
    // Declaration des delegates pour les callback des retours de fonction des workers
    public delegate void MapsListLoadedCallBack( List<UserMapDetailedJSON> pList );
    public delegate void MapDownloadedCallBack( string pFilepath );


    ///////////////////////////////////////////////////////////////////////////
    /// @class WorkerParamsDownload
    /// @brief Classe pour contenir les parametres pour les appels de fonctions
    ///
    /// @author Mathieu Parent
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public class WorkerParamsDownload
    {
        public WorkerParamsDownload(MapDownloadedCallBack pCallback, int pUserId, int pMapId = -1)
        {
            userId         = pUserId;
            mapId          = pMapId;
            callback       = pCallback;
        }

        public int                      userId;
        public int                      mapId;
        public MapDownloadedCallBack    callback;
    }


    ///////////////////////////////////////////////////////////////////////////
    /// @class WorkerParamsList
    /// @brief Classe pour contenir les parametres pour les appels de fonctions
    ///
    /// @author Mathieu Parent
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public class WorkerParamsList
    {
        public WorkerParamsList(MapsListLoadedCallBack pCallback, int pUserId, int pMapId = -1, string pAuthentification = "")
        {
            userId = pUserId;
            mapId = pMapId;
            authentication = pAuthentification;
            callback = pCallback;
        }

        public int userId;
        public int mapId;
        public string authentication;
        public MapsListLoadedCallBack callback;
    }


    ///////////////////////////////////////////////////////////////////////////
    /// @class HttpManagerWorker
    /// @brief Worker qui contient les fonctions a appeler pour executer dans des threads seperes
    ///
    /// @author Mathieu Parent
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public class HttpManagerWorker
    {

        public static void getPublicMapList(object pCallback)
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

        public static void getUserPublicMapList(object pParams)
        {
            WorkerParamsList wParams = (WorkerParamsList)pParams;

            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add("user_id", wParams.userId.ToString());
            string wData = HttpManager.getJsonFromRequest("http://hockedu.com/remote/listmaps", wPostData);

            if (wData.Length == 0)
            {
                wParams.callback(new List<UserMapDetailedJSON>());
            }
            MapListJSON wMapList = (MapListJSON)JsonConvert.Import(typeof(MapListJSON), wData);

            // Si pas d'erreur
            if (wMapList.error == null)
            {
                wParams.callback(new List<UserMapDetailedJSON>(wMapList.maps));
            }
            else
            {
                wParams.callback(new List<UserMapDetailedJSON>());
            }
        }


        public static void getUserMapList(object pParams)
        {
            WorkerParamsList wParams = (WorkerParamsList)pParams;

            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add("user_id", wParams.userId.ToString());
            wPostData.Add("auth_key", wParams.authentication.ToString());
            string wData = HttpManager.getJsonFromRequest("http://hockedu.com/remote/listmaps", wPostData);

            if (wData.Length == 0)
            {
                wParams.callback(new List<UserMapDetailedJSON>());
            }
            MapListJSON wMapList = (MapListJSON)JsonConvert.Import(typeof(MapListJSON), wData);

            // Si pas d'erreur
            if (wMapList.error == null)
            {
                wParams.callback(new List<UserMapDetailedJSON>(wMapList.maps));
            }
            else
            {
                wParams.callback(new List<UserMapDetailedJSON>());
            }
        }


        public static void downloadMap(object pParams)
        {
            WorkerParamsDownload wParams = (WorkerParamsDownload)pParams;
            int wUserId = wParams.userId;
            int wMapId = wParams.mapId;
            MapDownloadedCallBack wCallback = wParams.callback;

            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add("user_id", wUserId.ToString());
            wPostData.Add("map_id", wMapId.ToString());
            string wJsonData = HttpManager.getJsonFromRequest("http://hockedu.com/remote/getmap", wPostData);

            UserMapDownloadJSON wMapLight = (UserMapDownloadJSON)JsonConvert.Import(typeof(UserMapDownloadJSON), wJsonData);

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





    ///////////////////////////////////////////////////////////////////////////
    /// @class HttpManager
    /// @brief Manager pour les appels par des requetes HTTP. Essayer de garder le plus possible les fonctionnalites ASYNC!!
    ///
    /// @author Mathieu Parent
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public class HttpManager
    {
        private WebClient mClient;
        public HttpManager()
        {
            mClient = new WebClient();
            System.Net.ServicePointManager.Expect100Continue = false;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @fn static string getJsonFromRequest(string pUrl, NameValueCollection pParams = null)
        ///
        /// Methode pour effectuer un appel a une requete HTTP avec des parametres pour la requete en POST. 
        /// Retourne un string contenant du JSON pour le retour de la requete
        /// 
        /// @param[in] string               pUrl    : Url pour la requete
        /// @param[in] NameValueCollection  pParams : Parametres a passer par la requete HTTP
        ///
        /// @return string : Chaine JSON comtenant le retour de la requete
        ////////////////////////////////////////////////////////////////////////
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


        ////////////////////////////////////////////////////////////////////////
        /// @fn void downloadMap(int pUserId, int pMapId, MapDownloadedCallBack pCallback)
        ///
        /// Methode pour telecharger une map par le serveur web. ASYNC
        /// 
        /// @param[in] int                      pUserId     : UserId du proprietaire de la map
        /// @param[in] int                      pMapId      : MapId de la map a telecharger
        /// @param[in] MapDownloadedCallBack    pCallback   : Fonction de callback a appeler une fois que le telechargement est termine
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void downloadMap(int pUserId, int pMapId, MapDownloadedCallBack pCallback)
        {
            // Faire dans un thread separe
            WorkerParamsDownload wParams = new WorkerParamsDownload(pCallback, pUserId, pMapId);
            Thread newThread = new Thread(new ParameterizedThreadStart(HttpManagerWorker.downloadMap));
            newThread.Start(wParams);
        }


        ////////////////////////////////////////////////////////////////////////
        /// @fn void getPublicMapList(MapsListLoadedCallBack pCallbackFunction)
        ///
        /// Methode pour obtenir la liste de toutes les maps publiques de tous les utilisateurs contenus dans la base de donnees
        /// 
        /// @param[in] MapsListLoadedCallBack   pCallbackFunction   : Callback a appeler une fois que la liste a ete telechargee
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void getPublicMapList(MapsListLoadedCallBack pCallbackFunction)
        {
            // Faire dans un thread separe
            Thread newThread = new Thread(new ParameterizedThreadStart(HttpManagerWorker.getPublicMapList));
            newThread.Start(pCallbackFunction);
        }


        ////////////////////////////////////////////////////////////////////////
        /// @fn void getUserPublicMapList(int pUserId, MapsListLoadedCallBack pCallback)
        ///
        /// Methode pour obtenir la liste des map publiques pour un utilisateur
        /// 
        /// @param[in] int                      pUserId     : UserId du user dont on veut les map publiques
        /// @param[in] MapsListLoadedCallBack   pCallback   : Callback a appeler une fois que la liste a ete telechargee
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void getUserPublicMapList(int pUserId, MapsListLoadedCallBack pCallback)
        {
            // Faire dans un thread separe
            WorkerParamsList wParams = new WorkerParamsList(pCallback, pUserId);
            Thread newThread = new Thread(new ParameterizedThreadStart(HttpManagerWorker.getUserPublicMapList));
            newThread.Start(wParams);
        }


        ////////////////////////////////////////////////////////////////////////
        /// @fn void getUserMapList(int pUserId, string pAuthentication, MapsListLoadedCallBack pCallback)
        ///
        /// Methode pour obtenir la liste des maps privees et publiques pour un utilisateur
        /// 
        /// @param[in] int                      pUserId         : UserId du user dont on veut les map publiques
        /// @param[in] string                   pAuthentication : Chaine d'authentification obtenue lors de la connexion au serveur 
        /// @param[in] MapsListLoadedCallBack   pCallback       : Callback a appeler une fois que la liste a ete telechargee
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void getUserMapList(int pUserId, string pAuthentication, MapsListLoadedCallBack pCallback)
        {
            // Faire dans un thread separe
            WorkerParamsList wParams = new WorkerParamsList(pCallback, pUserId, -1, pAuthentication);
            Thread newThread = new Thread(new ParameterizedThreadStart(HttpManagerWorker.getUserMapList));
            newThread.Start(wParams);
        }
    }
}
