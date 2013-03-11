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
    
};


#endif // __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
