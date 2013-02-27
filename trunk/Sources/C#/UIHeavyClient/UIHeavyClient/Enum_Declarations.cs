#if !WIN32 && !LINUX
namespace UIHeavyClient
{
#else
#pragma once
#endif

    enum ActionType
    {
        ACTION_EDITEUR_NOUVEAU     ,
        ACTION_REINITIALISER_PARTIE,
        ACTION_PAUSE_JEU           ,
        ACTION_REPLAY              ,
        ACTION_CAMERA              ,
        ACTION_ORBIT               ,
        ACTION_ZOOM_ELASTIQUE      ,
        ACTION_ZOOM_PROPORTIONNEL  ,
        ACTION_EDITEUR_SELECTION   ,
        ACTION_EDITEUR_DEPLACER    ,
        ACTION_EDITEUR_ROTATION    ,
        ACTION_EDITEUR_ECHELLE     ,
        ACTION_SUPPRIMER           ,
        ACTION_DUPLIQUER           ,
        ACTION_INSERER_PORTAIL     ,
        ACTION_INSERER_MURET       ,
        ACTION_INSERER_MAILLET     ,
        ACTION_INSERER_RONDELLE    ,
        ACTION_INSERER_ACCELERATEUR,
        ACTION_ALLER_MODE_EDITION  ,
        ACTION_ALLER_MODE_JEU      ,
        ACTION_ALLER_MODE_TOURNOI  ,
        ACTION_ALLER_MENU_PRINCIPAL,
        ACTION_CAMERA_FIXE         ,
        ACTION_CAMERA_ORBITE       ,
        ACTION_CAMERA_LIBRE        ,
        ACTION_CAMERA_SPLIT        ,
    };

    enum EventCodes
    {
        USER_ALREADY_CONNECTED,
        USER_DID_NOT_SEND_NAME_ON_CONNECTION,
        USER_CONNECTED,
        USER_DISCONNECTED,
        INVALID_USERNAME = USER_DISCONNECTED, // Pour eviter d'avoir a changer le handling pour le prototype. Mettre 2 enums separes pour la version finale
        CONNECTION_CANCELED,
        RECONNECTION_TIMEOUT,
        RECONNECTION_IN_PROGRESS,
        WRONG_PASSWORD,
        CHAT_MESSAGE_RECEIVED,
        SERVER_USER_CONNECTED,
        SERVER_USER_DISCONNECTED,
        SERVER_USER_CONNECTING,
        NB_EVENT_CODES // Must be always last !
    };

#if !WIN32 && !LINUX
}
#endif