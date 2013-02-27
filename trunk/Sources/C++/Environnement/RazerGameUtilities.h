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

// Macro permettant de localiser la signature des fonctions et �viter des gros replace all en cas de changement
#define CreateListDelegateSignature(name) static unsigned int CreateListDelegate##name##(class Modele3D* pModel)
#define CreateListDelegateImplementation(name) unsigned int RazerGameUtilities::CreateListDelegate##name##(Modele3D* pModel)
typedef unsigned int (*CreateListDelegate)(class Modele3D*);

class Terrain;

class RazerGameUtilities {
public:
    static unsigned int CreateListSphereDefault(class Modele3D*, float radius);

    /// Effectue le chargement XML d'un terrain
    static void LoadFieldFromFile( const std::string& pFilePath, Terrain& pField );
    static void SaveFieldToFile( const std::string& nomFichier, Terrain& pField  );

    /// Effectue une conversion entre une cha�ne Java et une cha�ne C++.
    static std::string obtenirChaineISO(void* env, void* chaine);

    /// La cha�ne repr�sentant un accelerateur.
    static const std::string NOM_ACCELERATEUR;
    CreateListDelegateSignature(Boost);

    /// La cha�ne repr�sentant le dossier.
    static const std::string NOM_DOSSIER_MEDIA;

    /// La cha�ne repr�sentant l'extension.
    static const std::string NOM_EXTENSION;

    /// La cha�ne repr�sentant le type de la piece.
    static const std::string NOM_HOUSE;
    CreateListDelegateSignature(House);

    /// La cha�ne repr�sentant le type des buts.
    static const std::string NOM_BUT;
    CreateListDelegateSignature(Goal);

    /// La cha�ne repr�sentant le type des murets statique.
    static const std::string NOM_MURET;
    CreateListDelegateSignature(Wall);

    /// La cha�ne repr�sentant le type des bande exterieur de la patinoire.
    static const std::string NAME_RINK_BOARD;

    /// La cha�ne repr�sentant le type de muret relatif pour l'�dition
    static const std::string NAME_RELATIVE_WALL;

    /// La cha�ne repr�sentant le type de la table de jeu.
    static const std::string NOM_TABLE;
    CreateListDelegateSignature(Table);

    /// La cha�ne repr�sentant le type des portails.
    static const std::string NOM_PORTAIL;
    CreateListDelegateSignature(Portal);

    /// La cha�ne repr�sentant le type de la rondelle.
    static const std::string NOM_RONDELLE;
    CreateListDelegateSignature(Puck);

    /// La cha�ne repr�sentant le type des maillets.
    static const std::string NOM_MAILLET;
    CreateListDelegateSignature(Mallet);

    /// La cha�ne repr�sentant le type des points de controle de la table.
    static const std::string NAME_TABLE_CONTROL_POINT;
    CreateListDelegateSignature(TableControlPoint);

    /// La cha�ne repr�sentant le type des points de controle d'objet.
    static const std::string NAME_CONTROL_POINT;
    CreateListDelegateSignature(ControlPoint);

    /// La cha�ne repr�sentant le type des Groupes.
    static const std::string NOM_GROUPE;

    static const std::string NAME_POLYGONE;

};


#endif // __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
