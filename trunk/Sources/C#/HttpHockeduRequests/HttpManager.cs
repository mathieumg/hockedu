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
    public enum UploadOperationStatus { UPLOAD_SUCCESS, UPLOAD_FAILED_USER_ID_MISSING, UPLOAD_FAILED_INVALID_FILE_PATH, UPLOAD_FAILED_AUTHENTICATION_REJECTED, UPLOAD_FAILED_FILE_RELATED, UPLOAD_FAILED_UNKNOWN_ERROR };
    public enum DownloadOperationStatus { DOWNLOAD_SUCCESS, DOWNLOAD_FAILED_INVALID_FILE_PATH, DOWNLOAD_FAILED_AUTHENTICATION_REJECTED, DOWNLOAD_FAILED_UNKNOWN_ERROR };

    // Declaration des delegates pour les callback des retours de fonction des workers
    public delegate void MapsListLoadedCallBack( List<UserMapDetailedJSON> pList );
    public delegate void MapDownloadedCallBack( string pFilepath, int pMapId = -1 );
    public delegate void MapUploadCallback( UploadOperationStatus pStatus, int pMapId = -1 );
    public delegate void AchievementsUploadCallback( UploadOperationStatus pStatus );
    public delegate void AchievementsDownloadedCallback( DownloadOperationStatus pStatus );
    public delegate void AuthentificationCallback( AuthentificationJSON response );


    ///////////////////////////////////////////////////////////////////////////
    /// @class WorkerParamsDownload
    /// @brief Classe pour contenir les parametres pour les appels de fonctions
    ///
    /// @author Mathieu Parent
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public class WorkerParamsDownload
    {
        public WorkerParamsDownload( MapDownloadedCallBack pCallback, int pUserId, int pMapId = -1 )
        {
            userId = pUserId;
            mapId = pMapId;
            callback = pCallback;
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
        public WorkerParamsList( MapsListLoadedCallBack pCallback, int pUserId, int pMapId = -1, string pAuthentification = "" )
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
    /// @class WorkerParamsListUpload
    /// @brief Classe pour contenir les parametres pour les appels de fonctions pour l'upload de la map
    ///
    /// @author Mathieu Parent
    /// @date 2013-03-26
    ///////////////////////////////////////////////////////////////////////////
    public class WorkerParamsUpload
    {
        public WorkerParamsUpload( MapUploadCallback pCallback, int pUserId, string pAuthenticationKey, string pMapName, string pMapDescription, bool pMapPublic, string pFilepath, int pMapId )
        {
            userId = pUserId;
            authenticationKey = pAuthenticationKey;
            mapName = pMapName;
            mapDescription = pMapDescription;
            mapPublic = pMapPublic;
            filepath = pFilepath;
            mapId = pMapId;
            callback = pCallback;
        }
        public int                  userId;
        public string               authenticationKey;
        public string               mapName;
        public string               mapDescription;
        public bool                 mapPublic;
        public string               filepath;
        public int                  mapId;
        public MapUploadCallback    callback;
    }

    public class WorkerAchievementParamsUpload
    {
        public WorkerAchievementParamsUpload( AchievementsUploadCallback pCallback, string pUserId, string pAuthenticationKey )
        {
            userId = pUserId;
            authenticationKey = pAuthenticationKey;
            callback = pCallback;
        }
        public string                  userId;
        public string               authenticationKey;
        public AchievementsUploadCallback    callback;
    }

    public class WorkerAchievementParamsDownload
    {
        public WorkerAchievementParamsDownload( AchievementsDownloadedCallback pCallback, string pUserId, string pAuthenticationKey )
        {
            userId = pUserId;
            authenticationKey = pAuthenticationKey;
            callback = pCallback;
        }
        public string                  userId;
        public string               authenticationKey;
        public AchievementsDownloadedCallback    callback;
    }

    public class WorkerAuthentificationParams
    {
        public WorkerAuthentificationParams( AuthentificationCallback pCallback, string pUsername, string pHashedPassword )
        {
            username = pUsername;
            hashedPassword = pHashedPassword;
            callback = pCallback;
        }
        public string               username;
        public string               hashedPassword;
        public AuthentificationCallback    callback;
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

        public static void getPublicMapList( object pCallback )
        {
            MapsListLoadedCallBack wCallback = (MapsListLoadedCallBack) pCallback;
            string wData = HttpManager.getJsonFromRequest( "http://hockedu.com/remote/listmaps" );
            if ( wData.Length == 0 )
            {
                wCallback( new List<UserMapDetailedJSON>() );
            }
            MapListJSON wMapList = (MapListJSON) JsonConvert.Import( typeof( MapListJSON ), wData );

            // Si pas d'erreur
            if ( wMapList.error == null )
            {
                wCallback( new List<UserMapDetailedJSON>( wMapList.maps ) );
            }
            else
            {
                wCallback( new List<UserMapDetailedJSON>() );
            }
        }

        public static void getUserPublicMapList( object pParams )
        {
            WorkerParamsList wParams = (WorkerParamsList) pParams;

            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add( "user_id", wParams.userId.ToString() );
            string wData = HttpManager.getJsonFromRequest( "http://hockedu.com/remote/listmaps", wPostData );

            if ( wData.Length == 0 )
            {
                wParams.callback( new List<UserMapDetailedJSON>() );
            }
            MapListJSON wMapList = (MapListJSON) JsonConvert.Import( typeof( MapListJSON ), wData );

            // Si pas d'erreur
            if ( wMapList.error == null )
            {
                wParams.callback( new List<UserMapDetailedJSON>( wMapList.maps ) );
            }
            else
            {
                wParams.callback( new List<UserMapDetailedJSON>() );
            }
        }


        public static void getUserMapList( object pParams )
        {
            WorkerParamsList wParams = (WorkerParamsList) pParams;

            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add( "user_id", wParams.userId.ToString() );
            wPostData.Add( "auth_key", wParams.authentication.ToString() );
            string wData = HttpManager.getJsonFromRequest( "http://hockedu.com/remote/listmaps", wPostData );

            if ( wData.Length == 0 )
            {
                wParams.callback( new List<UserMapDetailedJSON>() );
            }
            MapListJSON wMapList = (MapListJSON) JsonConvert.Import( typeof( MapListJSON ), wData );

            // Si pas d'erreur
            if ( wMapList.error == null )
            {
                wParams.callback( new List<UserMapDetailedJSON>( wMapList.maps ) );
            }
            else
            {
                wParams.callback( new List<UserMapDetailedJSON>() );
            }
        }


        public static void downloadMap( object pParams )
        {
            WorkerParamsDownload wParams = (WorkerParamsDownload) pParams;
            int wUserId = wParams.userId;
            int wMapId = wParams.mapId;
            MapDownloadedCallBack wCallback = wParams.callback;

            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add( "user_id", wUserId.ToString() );
            wPostData.Add( "map_id", wMapId.ToString() );
            string wJsonData = HttpManager.getJsonFromRequest( "http://hockedu.com/remote/getmap", wPostData );

            UserMapDownloadJSON wMapLight;
            try
            {
                wMapLight = (UserMapDownloadJSON) JsonConvert.Import( typeof( UserMapDownloadJSON ), wJsonData );
            }
            catch
            {
                wCallback("", wMapId);
                return;
            }
            // On sauvegarde le data dans un fichier XML et on retourne le path
            if ( wMapLight.error != null )
            {
                wCallback("", wMapId);
                return;
            }
            else
            {
                string wCurrentDirectory = Directory.GetCurrentDirectory();
                string wDestinationFilePath=wCurrentDirectory+Path.DirectorySeparatorChar+"OnlineMaps"+Path.DirectorySeparatorChar+wMapId+".xml";
                try
                {
                    System.IO.File.WriteAllText( wDestinationFilePath, wMapLight.content );
                }
                catch ( System.IO.IOException )
                {
                    try
                    {
                        // Si erreur, on le met dans C:/temp/
                        wDestinationFilePath = "C:" + Path.DirectorySeparatorChar + "temp" + Path.DirectorySeparatorChar + wMapId + ".xml";
                        System.IO.File.WriteAllText( wDestinationFilePath, wMapLight.content );
                    }
                    catch ( Exception )
                    {
                        // Si encore une erreur, on retourne rien
                        wCallback("", wMapId);
                        return;
                    }
                }
                wCallback( wDestinationFilePath, wMapId ); // Retourne le file path ou le fichier a ete sauvegarde en local
            }
        }

        public const string TemplateFormData = "--{0}\r\nContent-Disposition: form-data; name=\"{1}\"\r\n\r\n{2}\r\n";
        public const string TemplateFileData = "--{0}\r\nContent-Disposition: form-data; name=\"{1}\"; filename=\"{2}\"\r\nContent-Type: {3}\r\n\r\n";
        public static void sendMap( object pParams )
        {
            WorkerParamsUpload wParams = (WorkerParamsUpload) pParams;

            // On essaie d'aller chercher le contenu du fichier donne dans le filepath
            string wFileContentString = "";
            try
            {
                wFileContentString = System.IO.File.ReadAllText( wParams.filepath );
            }
            catch ( System.Exception )
            {
                // Cancel
                wParams.callback( UploadOperationStatus.UPLOAD_FAILED_INVALID_FILE_PATH );
                return;
            }

            // Escape file data

            try
            {
                // Basic infos
                HttpWebRequest wRequest = (HttpWebRequest) WebRequest.Create( "http://hockedu.com/remote/sendmap" );
                wRequest.Method = "POST";
                wRequest.KeepAlive = true;
                wRequest.AllowAutoRedirect = true;
                wRequest.Timeout = 120000;

                // Create boundary and set ContentType
                string wBoundary = "---------------------------" + DateTime.Now.Ticks.ToString( "x" );
                wRequest.ContentType = "multipart/form-data; boundary=" + wBoundary;

                // Get request Stream to edit it
                Stream wRequestStream = wRequest.GetRequestStream();

                // Ajouter les infos de la requete avant le contenu du fichier (formulaire)
                // user_id
                string item = String.Format( TemplateFormData, wBoundary, "user_id", wParams.userId );
                byte[] itemBytes = System.Text.Encoding.UTF8.GetBytes( item );
                wRequestStream.Write( itemBytes, 0, itemBytes.Length );

                // auth_key
                string itemAuthKey = String.Format( TemplateFormData, wBoundary, "auth_key", wParams.authenticationKey );
                byte[] itemBytesAuthKey = System.Text.Encoding.UTF8.GetBytes( itemAuthKey );
                wRequestStream.Write( itemBytesAuthKey, 0, itemBytesAuthKey.Length );

                // name
                string itemName = String.Format( TemplateFormData, wBoundary, "name", wParams.mapName );
                byte[] itemBytesName = System.Text.Encoding.UTF8.GetBytes( itemName );
                wRequestStream.Write( itemBytesName, 0, itemBytesName.Length );

                // description
                string itemDescription = String.Format( TemplateFormData, wBoundary, "description", wParams.mapDescription );
                byte[] itemBytesDescription = System.Text.Encoding.UTF8.GetBytes( itemDescription );
                wRequestStream.Write( itemBytesDescription, 0, itemBytesDescription.Length );


                // public
                if ( wParams.mapPublic )
                {
                    string itemPublic=String.Format( TemplateFormData, wBoundary, "public", wParams.mapPublic );
                    byte[] itemBytesPublic=System.Text.Encoding.UTF8.GetBytes( itemPublic );
                    wRequestStream.Write( itemBytesPublic, 0, itemBytesPublic.Length );
                }

                // MapId
                if ( wParams.mapId != -1 )
                {
                    string itemMapId=String.Format( TemplateFormData, wBoundary, "map_id", wParams.mapId );
                    byte[] itemMapIdBytes=System.Text.Encoding.UTF8.GetBytes( itemMapId );
                    wRequestStream.Write( itemMapIdBytes, 0, itemMapIdBytes.Length );
                }

                // Ajouter le data du fichier au stream
                // mapfile (header)
                string wFileName = wParams.filepath.Substring( wParams.filepath.LastIndexOf( Path.DirectorySeparatorChar ) + 1 );
                string header = String.Format( TemplateFileData, wBoundary, "mapfile", wFileName, "text/xml" );
                byte[] headerbytes = Encoding.UTF8.GetBytes( header );
                wRequestStream.Write( headerbytes, 0, headerbytes.Length );
                // mapfile (data)
                byte[] newlineBytes = Encoding.UTF8.GetBytes( wFileContentString );
                wRequestStream.Write( newlineBytes, 0, newlineBytes.Length );


                // Ajouter les bytes de fermeture a la fin de la request
                byte[] wEndBytes = System.Text.Encoding.UTF8.GetBytes( "--" + wBoundary + "--" );
                wRequestStream.Write( wEndBytes, 0, wEndBytes.Length );


                // On ferme le stream de requete
                wRequestStream.Close();

                // Lire la reponse du serveur
                using ( WebResponse wResponse = wRequest.GetResponse() )
                using ( StreamReader wReader = new StreamReader( wResponse.GetResponseStream() ) )
                {
                    string wResponseData = wReader.ReadToEnd();
                    // Retourne du JSON avec le map_id qui a ete cree
                    MapUploadJSON wMapUploaded = (MapUploadJSON) JsonConvert.Import( typeof( MapUploadJSON ), wResponseData );

                    if ( wMapUploaded.error == null )
                    {
                        wParams.callback( UploadOperationStatus.UPLOAD_SUCCESS, wMapUploaded.map_id );
                    }
                    else if ( wMapUploaded.error == "AuthKeyMissing" || wMapUploaded.error == "AuthKeyInvalid" || wMapUploaded.error == "UserIdMissing" )
                    {
                        wParams.callback( UploadOperationStatus.UPLOAD_FAILED_AUTHENTICATION_REJECTED );
                    }
                    else if ( wMapUploaded.error == "MapFileUploadError" || wMapUploaded.error == "MapFileInvalidType" || wMapUploaded.error == "MapFileCacheError" )
                    {
                        wParams.callback( UploadOperationStatus.UPLOAD_FAILED_FILE_RELATED );
                    }
                    else
                    {
                        wParams.callback( UploadOperationStatus.UPLOAD_FAILED_UNKNOWN_ERROR );
                    }
                };
            }
            catch ( System.Exception )
            {
                wParams.callback( UploadOperationStatus.UPLOAD_FAILED_UNKNOWN_ERROR );
            }
        }

        public static void achievementUpload( object pParams )
        {
            WorkerAchievementParamsUpload wParams = (WorkerAchievementParamsUpload) pParams;

            // On essaie d'aller chercher le contenu du fichier donne dans le filepath
            string wFileContentString = "";
            try
            {
                wFileContentString = System.IO.File.ReadAllText( "Achievements.xml" );
            }
            catch ( System.Exception )
            {
                // Cancel
                wParams.callback( UploadOperationStatus.UPLOAD_FAILED_INVALID_FILE_PATH );
            }

            // Escape file data

            try
            {
                // Basic infos
                HttpWebRequest wRequest = (HttpWebRequest) WebRequest.Create( "http://hockedu.com/remote/sendachievements" );
                wRequest.Method = "POST";
                wRequest.KeepAlive = true;
                wRequest.AllowAutoRedirect = true;
                wRequest.Timeout = 120000;

                // Create boundary and set ContentType
                string wBoundary = "---------------------------" + DateTime.Now.Ticks.ToString( "x" );
                wRequest.ContentType = "multipart/form-data; boundary=" + wBoundary;

                // Get request Stream to edit it
                Stream wRequestStream = wRequest.GetRequestStream();

                // Ajouter les infos de la requete avant le contenu du fichier (formulaire)
                // user_id
                string item = String.Format( TemplateFormData, wBoundary, "user_id", wParams.userId );
                byte[] itemBytes = System.Text.Encoding.UTF8.GetBytes( item );
                wRequestStream.Write( itemBytes, 0, itemBytes.Length );

                // auth_key
                string itemAuthKey = String.Format( TemplateFormData, wBoundary, "auth_key", wParams.authenticationKey );
                byte[] itemBytesAuthKey = System.Text.Encoding.UTF8.GetBytes( itemAuthKey );
                wRequestStream.Write( itemBytesAuthKey, 0, itemBytesAuthKey.Length );


                // Ajouter le data du fichier au stream
                // achievement (header)
                string wFileName = "Achievements.xml";
                string header = String.Format( TemplateFileData, wBoundary, "achievementsfile", wFileName, "text/xml" );
                byte[] headerbytes = Encoding.UTF8.GetBytes( header );
                wRequestStream.Write( headerbytes, 0, headerbytes.Length );
                // mapfile (data)
                byte[] newlineBytes = Encoding.UTF8.GetBytes( wFileContentString );
                wRequestStream.Write( newlineBytes, 0, newlineBytes.Length );


                // Ajouter les bytes de fermeture a la fin de la request
                byte[] wEndBytes = System.Text.Encoding.UTF8.GetBytes( "--" + wBoundary + "--" );
                wRequestStream.Write( wEndBytes, 0, wEndBytes.Length );


                // On ferme le stream de requete
                wRequestStream.Close();

                // Lire la reponse du serveur
                using ( WebResponse wResponse = wRequest.GetResponse() )
                using ( StreamReader wReader = new StreamReader( wResponse.GetResponseStream() ) )
                {
                    string wResponseData = wReader.ReadToEnd();
                    // Retourne du JSON avec le map_id qui a ete cree
                    AchievementUploadJSON wAchievementUploaded = (AchievementUploadJSON) JsonConvert.Import( typeof( AchievementUploadJSON ), wResponseData );
                    if ( wAchievementUploaded.error == null )
                    {
                        wParams.callback( UploadOperationStatus.UPLOAD_SUCCESS );
                    }
                    else if ( wAchievementUploaded.error == "AuthKeyMissing" || wAchievementUploaded.error == "AuthKeyInvalid" )
                    {
                        wParams.callback( UploadOperationStatus.UPLOAD_FAILED_AUTHENTICATION_REJECTED );
                    }
                    else
                    {
                        wParams.callback( UploadOperationStatus.UPLOAD_FAILED_UNKNOWN_ERROR );
                    }
                };

            }
            catch ( System.Exception ex )
            {
                wParams.callback( UploadOperationStatus.UPLOAD_FAILED_UNKNOWN_ERROR );
            }

        }


        public static void downloadAchievements( object pParams )
        {
            WorkerAchievementParamsDownload wParams = (WorkerAchievementParamsDownload) pParams;
            string wUserId = wParams.userId;
            string wAuthKey = wParams.authenticationKey;
            AchievementsDownloadedCallback wCallback = wParams.callback;

            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add( "user_id", wUserId );
            wPostData.Add( "auth_key", wAuthKey );
            string wJsonData = HttpManager.getJsonFromRequest( "http://hockedu.com/remote/getachievements", wPostData );

            UserAchievementDownloadJSON wAchievementLight;
            try
            {
                wAchievementLight = (UserAchievementDownloadJSON) JsonConvert.Import( typeof( UserAchievementDownloadJSON ), wJsonData );
            }
            catch
            {
                wCallback( DownloadOperationStatus.DOWNLOAD_FAILED_UNKNOWN_ERROR );
                return;
            }
            // On sauvegarde le data dans un fichier XML et on retourne le path
            if ( wAchievementLight.error != null )
            {
                wCallback( DownloadOperationStatus.DOWNLOAD_FAILED_UNKNOWN_ERROR );
                return;
            }
            else
            {
                string wCurrentDirectory = Directory.GetCurrentDirectory().ToString();
                string wDestinationFilePath=wCurrentDirectory + Path.DirectorySeparatorChar + "Achievements" + ".xml";
                try
                {
                    System.IO.File.WriteAllText( wDestinationFilePath, wAchievementLight.achievements );
                }
                catch ( System.IO.IOException )
                {
                    // Si encore une erreur, on retourne rien
                    wCallback( DownloadOperationStatus.DOWNLOAD_FAILED_UNKNOWN_ERROR );
                    return;
                }
                wCallback( DownloadOperationStatus.DOWNLOAD_SUCCESS ); // Retourne le file path ou le fichier a ete sauvegarde en local
            }
        }

        public static void authenticate( object pParams )
        {
            WorkerAuthentificationParams wParams = (WorkerAuthentificationParams) pParams;
            string wUserName = wParams.username;
            string wPassword = wParams.hashedPassword;
            AuthentificationCallback wCallback = wParams.callback;

            NameValueCollection wPostData = new NameValueCollection();
            wPostData.Add( "username", wUserName );
            wPostData.Add( "password", wPassword );
            string wJsonData = HttpManager.getJsonFromRequest( "http://hockedu.com/remote/authenticate", wPostData );

            AuthentificationJSON wAuthentificationResponse;
            try
            {
                wAuthentificationResponse = (AuthentificationJSON) JsonConvert.Import( typeof( AuthentificationJSON ), wJsonData );
            }
            catch
            {
                wAuthentificationResponse = new AuthentificationJSON();
                wAuthentificationResponse.error = "Unknown error";
                wCallback( wAuthentificationResponse );
                return;
            }
            wCallback( wAuthentificationResponse );
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
        public HttpManager()
        {
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
        public static string getJsonFromRequest( string pUrl, NameValueCollection pParams = null )
        {
            WebClient client = new WebClient();
            if ( pParams != null )
            {
                byte[] wResponse = client.UploadValues( pUrl, pParams );
                return Encoding.ASCII.GetString( wResponse );
            }
            else
            {
                return client.DownloadString( pUrl );
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
        public void downloadMap( int pUserId, int pMapId, MapDownloadedCallBack pCallback )
        {
            // Faire dans un thread separe
            WorkerParamsDownload wParams = new WorkerParamsDownload( pCallback, pUserId, pMapId );
            Thread newThread = new Thread( new ParameterizedThreadStart( HttpManagerWorker.downloadMap ) );
            newThread.Start( wParams );
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
        public void getPublicMapList( MapsListLoadedCallBack pCallbackFunction )
        {
            // Faire dans un thread separe
            Thread newThread = new Thread( new ParameterizedThreadStart( HttpManagerWorker.getPublicMapList ) );
            newThread.Start( pCallbackFunction );
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
        public void getUserPublicMapList( int pUserId, MapsListLoadedCallBack pCallback )
        {
            // Faire dans un thread separe
            WorkerParamsList wParams = new WorkerParamsList( pCallback, pUserId );
            Thread newThread = new Thread( new ParameterizedThreadStart( HttpManagerWorker.getUserPublicMapList ) );
            newThread.Start( wParams );
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
        public void getUserMapList( int pUserId, string pAuthentication, MapsListLoadedCallBack pCallback )
        {
            // Faire dans un thread separe
            WorkerParamsList wParams = new WorkerParamsList( pCallback, pUserId, -1, pAuthentication );
            Thread newThread = new Thread( new ParameterizedThreadStart( HttpManagerWorker.getUserMapList ) );
            newThread.Start( wParams );
        }




        ////////////////////////////////////////////////////////////////////////
        /// @fn void uploadNewMap(int pUserId, string pAuthenticationKey, string pMapName, string pMapDescription, bool pMapPublic, string pFilepath, MapUploadCallback pCallback)
        ///
        /// Methode pour uploader une map par le serveur web. ASYNC
        /// 
        /// @param[in] int                  pUserId             : UserId du proprietaire de la map
        /// @param[in] string               pAuthenticationKey  : Authentication key du user de la session courante
        /// @param[in] string               pMapName            : Nom a donner a la map
        /// @param[in] string               pMapDescription     : Description a donner a la map
        /// @param[in] bool                 pMapPublic          : Bool qui indique si la map doit etre publique ou non
        /// @param[in] string               pFilepath           : Path dans le filesystem qui contient le fichier
        /// @param[in] int                  pFilepath           : Id de la map si ce n'est qu'un update (seule difference entre les 2). Mettre -1 pour add new game
        /// @param[in] MapUploadCallBack    pCallback           : Fonction de callback a appeler une fois que l'upload est termine ou qu'il y a une erreur
        ///
        /// @return None.
        ////////////////////////////////////////////////////////////////////////
        public void sendMap( int pUserId, string pAuthenticationKey, string pMapName, string pMapDescription, bool pMapPublic, string pFilepath, int pMapId, MapUploadCallback pCallback )
        {
            // Faire dans un thread separe
            WorkerParamsUpload wParams = new WorkerParamsUpload( pCallback, pUserId, pAuthenticationKey, pMapName, pMapDescription, pMapPublic, pFilepath, pMapId );
            Thread newThread = new Thread( new ParameterizedThreadStart( HttpManagerWorker.sendMap ) );
            newThread.Start( wParams );
        }

        public void uploadAchievements( string pUserId, string pAuthenticationKey, AchievementsUploadCallback pCallback )
        {
            // Faire dans un thread separe
            WorkerAchievementParamsUpload wParams = new WorkerAchievementParamsUpload( pCallback, pUserId, pAuthenticationKey );
            Thread newThread = new Thread( new ParameterizedThreadStart( HttpManagerWorker.achievementUpload ) );
            newThread.Start( wParams );
        }

        public void downloadAchievements( string pUserId, string pAuthenticationKey, AchievementsDownloadedCallback pCallback )
        {
            // Faire dans un thread separe
            WorkerAchievementParamsDownload wParams = new WorkerAchievementParamsDownload( pCallback, pUserId, pAuthenticationKey );
            Thread newThread = new Thread( new ParameterizedThreadStart( HttpManagerWorker.downloadAchievements ) );
            newThread.Start( wParams );
        }

        public Thread authenticate( string pUserName, string pHashedPassword, AuthentificationCallback pCallback )
        {
            // Faire dans un thread separe
            WorkerAuthentificationParams wParams = new WorkerAuthentificationParams( pCallback, pUserName, pHashedPassword );
            Thread newThread = new Thread( new ParameterizedThreadStart( HttpManagerWorker.authenticate ));
            newThread.Start( wParams );
            return newThread;
        }

    }
}
