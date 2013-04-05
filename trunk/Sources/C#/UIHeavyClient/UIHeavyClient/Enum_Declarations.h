#if !WIN32 && !__linux__ && !__APPLE__
#define CSHARP
#endif



#if CSHARP
namespace UIHeavyClient
{
#else
#pragma once
#endif


#if CSHARP
public enum BoutonSouris
#else
typedef enum
#endif
{
    AUCUN_BOUTON_SOURIS	,
    BOUTON_SOURIS_GAUCHE,
    BOUTON_SOURIS_MILIEU,
    BOUTON_SOURIS_DROIT	,
    NB_BOUTON_SOURIS	}
#if !CSHARP
BoutonSouris
#endif
;

#if CSHARP
public enum ActionType
#else
typedef enum
#endif
    {
        ACTION_EDITEUR_NOUVEAU     ,
        ACTION_REINITIALISER_PARTIE,
        ACTION_REINITIALISER_RONDELLE,
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
}
#if !CSHARP
ActionType
#endif
;

#if CSHARP
public enum EventCodes
#else
typedef enum
#endif
    {
        /// Network event category : begin
        SERVER_EVENT_BEGIN,
        USER_ALREADY_CONNECTED,
        USER_DID_NOT_SEND_NAME_ON_CONNECTION,
        USER_CONNECTED,
        USER_DISCONNECTED,
        INVALID_USERNAME = USER_DISCONNECTED, // Pour eviter d'avoia changer le handling pour le prototype. Mettre 2 enums separes pour la version finale
        CONNECTION_CANCELED,
        SEND_PASSWORD_REQUEST,
        RECONNECTION_TIMEOUT,
        RECONNECTION_IN_PROGRESS,
        WRONG_PASSWORD,
        CHAT_MESSAGE_RECEIVED,
        CHAT_MESSAGE_RECEIVED_INGAME,
        SERVER_USER_CONNECTED,
        SERVER_USER_DISCONNECTED,
        SERVER_USER_CONNECTING,
        GAME_SERVER_AUTHENTICATION_REQUEST,
        GAME_SERVER_AUTHENTICATION_REPLY,
        GAMES_LIST_REQUEST,
        GAME_ADDED,
        GAME_CONNECTION_RESPONSE_SUCCESS,
        GAME_CONNECTION_RESPONSE_ALREADY_CONNECTED,
        GAME_CONNECTION_RESPONSE_GAME_FULL,
        GAME_CONNECTION_RESPONSE_GAME_NOT_FOUND,
        SERVER_EVENT_END,
        /// Network event category : end    


        NB_EVENT_CODES // Must be always last !
    }
#if !CSHARP
EventCodes
#endif
;

#if CSHARP
public enum EditionEventCodes
#else
typedef enum
#endif
{
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

    NB_EDITION_EVENT_CODES // Must be always last !
}
#if !CSHARP
EditionEventCodes
#endif
    ;

#if CSHARP
public enum BonusType
#else
typedef enum
#endif
    
    {
        BONUS_TYPE_GO_THROUGH_WALL,
        BONUS_TYPE_BLOCK_GOAL,
        BONUS_TYPE_FAN,
        BONUS_TYPE_CHANGE_ZONE,

        NB_BONUS_TYPE
    }
#if !CSHARP
BonusType
#endif
;


    // Enum to differentiate nodes and allows to find 3D models/Lists
#if CSHARP
public enum RazerKey
#else
typedef enum
#endif
    {
        RAZER_KEY_NONE                 ,
        RAZER_KEY_BOOST                ,
        RAZER_KEY_HOUSE                ,
        RAZER_KEY_GOAL                 ,
        RAZER_KEY_WALL                 ,
        RAZER_KEY_RINK_BOARD           ,
        RAZER_KEY_TABLE                ,
        RAZER_KEY_PORTAL               ,
        RAZER_KEY_PUCK                 ,
        RAZER_KEY_PUCK_CATCHER                 ,
        RAZER_KEY_PUCK_TROLL           ,
        RAZER_KEY_MALLET               ,
        RAZER_KEY_TABLE_CONTROL_POINT  ,
        RAZER_KEY_CONTROL_POINT        ,
        RAZER_KEY_GROUP                ,
        RAZER_KEY_POLYGON              ,
        RAZER_KEY_ZAMBONI              ,
        RAZER_KEY_EMPTY_BONUS          ,
        RAZER_KEY_BONUS                ,
        RAZER_KEY_GOALER               ,
        RAZER_KEY_FAN                  ,
        RAZER_KEY_PAUSE                ,
        RAZER_KEY_MODEL_1              ,
        RAZER_KEY_MODEL_2              ,
        RAZER_KEY_MODEL_3              ,
        RAZER_KEY_ROOT_TREE            ,
        RAZER_KEY_ZONE_EDITION            ,
        RAZER_KEY_SELECTION_BODY            ,
        RAZER_KEY_FORCE_FIELD           ,

        NB_RAZER_KEYS
    }
#if !CSHARP
RazerKey
#endif
;


    /// Enum to check if a property has already been assigned
    /// and to verify if the value is the same for all the nodes
    /// Typically
    /// Node1 sets a value X in Scale and set the flag ASSIGNED_SCALE
    /// Node2 sees the flag ASSIGNED_SCALE and checks the current value
    /// if it doesn't match it sets the flag INVALID_SCALE to indicate not to use it
    ///
#if CSHARP
public enum PropertyAssignmentValidation
#else
typedef enum
#endif
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
}
#if !CSHARP
PropertyAssignmentValidation
#endif
;
    
    // max of 16 categories because box2D flag have only 16 bits
    /// Group 1 used to collide mallet with middle separation and goals
#if CSHARP
public enum PhysicsCategory
#else
typedef enum
#endif
    {
        CATEGORY_NONE     = 0x0000,
        CATEGORY_BOUNDARY = 0x0001,      /// User data not predetermined, might have to dynamic cast
        CATEGORY_PUCK     = 0x0002,      /// Expected to have NoeudAbstrait* as user data 
        CATEGORY_MALLET   = 0x0004,      /// Expected to have NoeudAbstrait* as user data 
        CATEGORY_PORTAL   = 0x0008,      /// Expected to have NoeudAbstrait* as user data 
        CATEGORY_BOOST    = 0x0010,      /// Expected to have NoeudAbstrait* as user data 
        CATEGORY_BONUS    = 0x0020,      /// Expected to have NoeudAbstrait* as user data 
        CATEGORY_WALL     = 0x0040,      /// Expected to have NoeudAbstrait* as user data 
        CATEGORY_FORCE_FIELD = 0x0080,   /// Expected to have ForceField* as user data
        CATEGORY_MIDLANE = 0x0100,   /// Expected to have NoeudTable* as user data
        CATEGORY_SELECTION = 0x0200,   /// Used to select node inedition
    }
#if !CSHARP
PhysicsCategory
#endif
    ;

#if CSHARP
public enum FieldModificationStrategyEventType
#else
typedef enum
#endif
{
    FIELD_MODIFICATION_EVENT_CLICK,
    FIELD_MODIFICATION_EVENT_MOVE,
}
#if !CSHARP
FieldModificationStrategyEventType
#endif
;

#if CSHARP
public enum FieldModificationStrategyType
#else
typedef enum
#endif
{
    FIELD_MODIFICATION_NONE      ,
    FIELD_MODIFICATION_MOVE      ,
    FIELD_MODIFICATION_ROTATE    ,
    FIELD_MODIFICATION_SCALE     ,
    FIELD_MODIFICATION_ADD_PORTAL,
    FIELD_MODIFICATION_ADD_BOOST ,
    FIELD_MODIFICATION_ADD_WALL  ,
    FIELD_MODIFICATION_ADD_MALLET,
    FIELD_MODIFICATION_ADD_PUCK  ,
    FIELD_MODIFICATION_ADD_BONUS ,
}
#if !CSHARP
FieldModificationStrategyType
#endif
;

#if CSHARP
}
#endif
