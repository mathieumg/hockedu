#if !WIN32 && !__linux__ && !__APPLE__
namespace UIHeavyClient
{
#else
#pragma once
#endif


#if !WIN32 && !__linux__ && !__APPLE__
public
#endif
enum BoutonSouris{
    AUCUN_BOUTON_SOURIS	,
    BOUTON_SOURIS_GAUCHE,
    BOUTON_SOURIS_MILIEU,
    BOUTON_SOURIS_DROIT	,
    NB_BOUTON_SOURIS	};

#if !WIN32 && !__linux__ && !__APPLE__
    public
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
        ACTION_EDITEUR_UNDO        ,
        ACTION_EDITEUR_REDO        ,
        ACTION_SUPPRIMER           ,
        ACTION_DUPLIQUER           ,
        ACTION_INSERER_PORTAIL     ,
        ACTION_INSERER_BONUS       ,
        ACTION_INSERER_MURET       ,
        ACTION_INSERER_MAILLET     ,
        ACTION_INSERER_RONDELLE    ,
        ACTION_INSERER_ACCELERATEUR,
        ACTION_ALLER_MODE_EDITION  ,
        ACTION_ALLER_MODE_JEU      ,
        ACTION_ALLER_MODE_TOURNOI  ,
        ACTION_ALLER_MENU_PRINCIPAL,
        ACTION_ALLER_MODE_SIMULATION,
        ACTION_CAMERA_FIXE         ,
        ACTION_CAMERA_ORBITE       ,
        ACTION_CAMERA_LIBRE        ,
        ACTION_CAMERA_SPLIT        ,
    };

#if !WIN32 && !__linux__ && !__APPLE__
    public
#endif
    enum EventCodes
    {
        /// Network event category : begin
        SERVER_EVENT_BEGIN,
        USER_ALREADY_CONNECTED,
        USER_DID_NOT_SEND_NAME_ON_CONNECTION,
        USER_CONNECTED,
        USER_DISCONNECTED,
        INVALID_USERNAME = USER_DISCONNECTED, // Pour eviter d'avoir a changer le handling pour le prototype. Mettre 2 enums separes pour la version finale
        CONNECTION_CANCELED,
        SEND_PASSWORD_REQUEST,
        RECONNECTION_TIMEOUT,
        RECONNECTION_IN_PROGRESS,
        WRONG_PASSWORD,
        CHAT_MESSAGE_RECEIVED,
        SERVER_USER_CONNECTED,
        SERVER_USER_DISCONNECTED,
        SERVER_USER_CONNECTING,
        GAME_SERVER_AUTHENTICATION_REQUEST,
        GAME_SERVER_AUTHENTICATION_REPLY,
        SERVER_EVENT_END,
        /// Network event category : end


        /// Model event category : begin
        MODEL_EVENT_BEGIN,

        ENABLE_PUCK_CREATION,
        DISABLE_PUCK_CREATION,
        ENABLE_MALLET_CREATION,
        DISABLE_MALLET_CREATION,

        THERE_ARE_NODES_SELECTED,
        THERE_ARE_NO_NODE_SELECTED,

        CAN_UNDO,
        CANNOT_UNDO,
        CAN_REDO,
        CANNOT_REDO,

        MODEL_EVENT_END,
        /// Model event category : end
        
        

        NB_EVENT_CODES // Must be always last !
    };

#if !WIN32 && !__linux__ && !__APPLE__
    public
#endif
    enum BonusType
    {
        BONUS_TYPE_GO_THROUGH_WALL,
        BONUS_TYPE_BLOCK_GOAL,

        NB_BONUS_TYPE
    };


    // Enum to differentiate nodes and allows to find 3D models/Lists
#if !WIN32 && !__linux__ && !__APPLE__
    public
#endif
    enum RazerKey
    {
        RAZER_KEY_NONE                ,
        RAZER_KEY_BOOST               ,
        RAZER_KEY_HOUSE               ,
        RAZER_KEY_GOAL                 ,
        RAZER_KEY_WALL                ,
        RAZER_KEY_RINK_BOARD          ,
        RAZER_KEY_TABLE               ,
        RAZER_KEY_PORTAL              ,
        RAZER_KEY_PUCK                ,
        RAZER_KEY_MALLET              ,
        RAZER_KEY_TABLE_CONTROL_POINT ,
        RAZER_KEY_CONTROL_POINT       ,
        RAZER_KEY_GROUP               ,
        RAZER_KEY_POLYGON             ,
        RAZER_KEY_ZAMBONI             ,
        RAZER_KEY_EMPTY_BONUS         ,
        RAZER_KEY_BONUS               ,
        RAZER_KEY_GOALER              ,
    };


    /// Enum to check if a property has already been assigned
    /// and to verify if the value is the same for all the nodes
    /// Typically
    /// Node1 sets a value X in Scale and set the flag ASSIGNED_SCALE
    /// Node2 sees the flag ASSIGNED_SCALE and checks the current value
    /// if it doesn't match it sets the flag INVALID_SCALE to indicate not to use it
    ///
#if !WIN32 && !__linux__ && !__APPLE__
    public
#endif
    enum PropertyAssignmentValidation
    {
        ASSIGNED_FRICTION        ,
        INVALID_FRICTION         ,
        ASSIGNED_SCALE        ,
        INVALID_SCALE         ,
        ASSIGNED_ACCELERATION ,
        INVALID_ACCELERATION  ,
        ASSIGNED_POSITIONX     ,
        INVALID_POSITIONX      ,
        ASSIGNED_POSITIONY     ,
        INVALID_POSITIONY      ,
        ASSIGNED_ATTRACTION   ,
        INVALID_ATTRACTION    ,
        ASSIGNED_ANGLE        ,
        INVALID_ANGLE         ,
        ASSIGNED_REBOUND      ,
        INVALID_REBOUND       ,
        ASSIGNED_ZONE_X      ,
        INVALID_ZONE_X       ,
        ASSIGNED_ZONE_Y      ,
        INVALID_ZONE_Y       ,
        ASSIGNED_BONUS_MIN      ,
        INVALID_BONUS_MIN       ,
        ASSIGNED_BONUS_MAX      ,
        INVALID_BONUS_MAX       ,
        NB_PROPERTYASSIGNMENTVALIDATION
    };
    
    
    // max of 16 categories because box2D flag have only 16 bits
    /// Group 1 used to collide mallet with middle separation and goals
#if !WIN32 && !__linux__ && !__APPLE__
    public
#endif
    enum PhysicsCategory
    {
        CATEGORY_NONE     = 0x0000,
        CATEGORY_BOUNDARY = 0x0001,
        CATEGORY_PUCK     = 0x0002,
        CATEGORY_MALLET   = 0x0004,
        CATEGORY_PORTAL   = 0x0008,
        CATEGORY_BOOST    = 0x0010,
        CATEGORY_BONUS    = 0x0020,
        CATEGORY_WALL     = 0x0040,
        CATEGORY_GOALIE     = 0x0080,
    };

#if !WIN32 && !__linux__ && !__APPLE__
}
#endif
