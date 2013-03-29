///////////////////////////////////////////////////////////////////////////
/// @file RazerGameUtilities.h
/// @author Martin Bisson, Michael Ferris
/// @date 2013-02-09
/// @version 2.0
///
/// 
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__
#define __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__

#include <string>
#include "XMLUtils.h"
#include "Enum_Declarations.h"
#include <map>
#define Map std::map
#if WIN32

#elif __APPLE__
#define GLdouble GLfloat
#define glPushAttrib(x)
#define glPopAttrib()
#define glPolygonMode(x,y)
#define glGetDoublev glGetFloatv
#define glMultMatrixd glMultMatrixf
#else
typedef float GLdouble;
typedef unsigned int GLuint;
#define GLenum unsigned int
#endif

// Macro permettant de localiser la signature des fonctions et éviter des gros replace all en cas de changement
#define CreateListDelegateSignature(name) static unsigned int CreateListDelegate## name(class Modele3D* pModel)
#define CreateListDelegateImplementation(name) unsigned int RazerGameUtilities::CreateListDelegate## name(class Modele3D* pModel)
typedef unsigned int (*CreateListDelegate)(class Modele3D*);

class Terrain;
class Runnable;


class RazerGameUtilities {
public:
    static unsigned int CreateListSphereDefault(class Modele3D*, float radius);

    /// Effectue le chargement XML d'un terrain
    static void LoadFieldFromFile( const std::string& pFilePath, Terrain& pField );
    static void SaveFieldToFile( const std::string& nomFichier, Terrain& pField  );

    /// Effectue une conversion entre une chaîne Java et une chaîne C++.
    static std::string obtenirChaineISO(void* env, void* chaine);

    /// Permet d'exécuter du code sur un thread spécifique au moment opportun
    static void RunOnRenderThread(Runnable* run, bool pForceQueue = false);
    static void ExecuteRenderRunnables();
    static void Rendering(bool isRendering);
    static void RunOnUpdateThread(Runnable* run, bool pForceQueue = false);
    static void ExecuteUpdateRunnables();
    static void Updating(bool isUpdating);
    //////////////////////////////////////////////////////////////////////////



    /// La chaîne représentant un accelerateur.
    static const std::string NOM_ACCELERATEUR;
    CreateListDelegateSignature(Boost);

    /// La chaîne représentant le dossier.
    static const std::string NOM_DOSSIER_MEDIA;

    /// La chaîne représentant l'extension.
    static const std::string NOM_EXTENSION;

    /// La chaîne représentant le type de la piece.
    static const std::string NOM_HOUSE;
    CreateListDelegateSignature(House);

    /// La chaîne représentant le type des buts.
    static const std::string NOM_BUT;
    CreateListDelegateSignature(Goal);

    /// La chaîne représentant le type des murets statique.
    static const std::string NOM_MURET;
    CreateListDelegateSignature(Wall);

    /// La chaîne représentant le type des bande exterieur de la patinoire.
    static const std::string NAME_RINK_BOARD;

    /// La chaîne représentant le type de la table de jeu.
    static const std::string NOM_TABLE;
    CreateListDelegateSignature(Table);

    /// La chaîne représentant le type des portails.
    static const std::string NOM_PORTAIL;
    CreateListDelegateSignature(Portal);

    /// La chaîne représentant le type de la rondelle.
    static const std::string NOM_RONDELLE;
    CreateListDelegateSignature(Puck);

    /// La chaîne représentant le type des maillets.
    static const std::string NOM_MAILLET;
    CreateListDelegateSignature(Mallet);

    /// La chaîne représentant le type des points de controle de la table.
    static const std::string NAME_TABLE_CONTROL_POINT;
    CreateListDelegateSignature(TableControlPoint);

    /// La chaîne représentant le type des points de controle d'objet.
    static const std::string NAME_CONTROL_POINT;
    CreateListDelegateSignature(ControlPoint);

    /// La chaîne représentant le type des Groupes.
    static const std::string NOM_GROUPE;

    /// La chaîne représentant le type des Polygones.
    static const std::string NAME_POLYGONE;
    
    /// La chaîne représentant le type des Zamboni.
    static const std::string NAME_ZAMBONI;
    CreateListDelegateSignature(Zamboni);

    /// La chaîne représentant le type des bonus.
    static const std::string NAME_EMPTY_BONUS;
    static const std::string NAME_BONUS;
    CreateListDelegateSignature(EmptyBonus);
    CreateListDelegateSignature(Bonus);

    static const std::string NAME_GOALER;
    CreateListDelegateSignature(Goaler);
    CreateListDelegateSignature(Fan);

    typedef Map<std::string,RazerKey> StringToKeyMap;
    /// Memory leak !! but wth
    static const StringToKeyMap* createStringToKeyMap()
    {
        StringToKeyMap* map = new StringToKeyMap();
        (*map)[""]                       = RAZER_KEY_NONE                 ;
        (*map)[NOM_ACCELERATEUR]         = RAZER_KEY_BOOST                ;
        (*map)[NOM_HOUSE]                = RAZER_KEY_HOUSE                ;
        (*map)[NOM_BUT]                  = RAZER_KEY_GOAL                 ;
        (*map)[NOM_MURET]                = RAZER_KEY_WALL                 ;
        (*map)[NAME_RINK_BOARD]          = RAZER_KEY_RINK_BOARD           ;
        (*map)[NOM_TABLE]                = RAZER_KEY_TABLE                ;
        (*map)[NOM_PORTAIL]              = RAZER_KEY_PORTAL               ;
        (*map)[NOM_RONDELLE]             = RAZER_KEY_PUCK                 ;
        (*map)[NOM_MAILLET]              = RAZER_KEY_MALLET               ;
        (*map)[NAME_TABLE_CONTROL_POINT] = RAZER_KEY_TABLE_CONTROL_POINT  ;
        (*map)[NAME_CONTROL_POINT]       = RAZER_KEY_CONTROL_POINT        ;
        (*map)[NOM_GROUPE]               = RAZER_KEY_GROUP                ;
        (*map)[NAME_POLYGONE]            = RAZER_KEY_POLYGON              ;
        (*map)[NAME_ZAMBONI]             = RAZER_KEY_ZAMBONI              ;
        (*map)[NAME_EMPTY_BONUS]         = RAZER_KEY_EMPTY_BONUS          ;
        (*map)[NAME_BONUS]               = RAZER_KEY_BONUS                ;
        (*map)[NAME_GOALER]              = RAZER_KEY_GOALER               ;
        return map;
    }
    static const StringToKeyMap* StringTypeToEnum;



    // Converts an Key to its string representation, slow O(n) complexity
    // used for compatibility, avoid usage as much as possible.
    static const std::string& KeyToString(RazerKey key)
    {
        if(!StringTypeToEnum)
        {
            StringTypeToEnum  = createStringToKeyMap();
        }
        static const std::string EMPTYSTRING = "";
        for(auto it=StringTypeToEnum->begin(); it != StringTypeToEnum->end(); ++it)
        {
            if(it->second == key)
                return it->first;
        }
        return EMPTYSTRING;
    }
    // Converts a string 
    static RazerKey StringToKey(const std::string& s)
    {
        if(!StringTypeToEnum)
        {
            StringTypeToEnum  = createStringToKeyMap();
        }
        auto it = StringTypeToEnum->find(s);
        if(it != StringTypeToEnum->end())
        {
            return it->second;
        }
        return RAZER_KEY_NONE;
    }
};


#endif // __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
