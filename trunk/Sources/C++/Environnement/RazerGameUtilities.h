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


// Macro permettant de localiser la signature des fonctions et éviter des gros replace all en cas de changement
#define CreateListDelegateSignature(name) static unsigned int CreateListDelegate## name(class Modele3D* pModel)
#define CreateListDelegateImplementation(name) unsigned int RazerGameUtilities::CreateListDelegate## name(class Modele3D* pModel)
typedef unsigned int (*CreateListDelegate)(class Modele3D*);

class Terrain;
class Runnable;
struct b2FixtureDef;
struct PhysicsFilter
{
    PhysicsFilter(
		unsigned short category =0,
        unsigned short maskbit=0,
        unsigned short group=0,
        bool isSensor=false):
    mCategory(category),
        mMaskbit(maskbit),
        mGroup(group),
        mIsSensor(isSensor)
    {

    }

    unsigned short mCategory;
    unsigned short mMaskbit;
    unsigned short mGroup;
    bool mIsSensor;
};


class RazerGameUtilities {
public:
    static unsigned int CreateListSphereDefault(class Modele3D*, float radius);

    /// Effectue le chargement XML d'un terrain
    static void LoadFieldFromFile( const std::string& pFilePath, Terrain& pField );
    static void SaveFieldToFile( const std::string& nomFichier, Terrain& pField  );

    /// Permet d'exécuter du code sur un thread spécifique au moment opportun
    static void RunOnRenderThread(Runnable* run, bool pForceQueue = false);
    static void ExecuteRenderRunnables();
    static void Rendering(bool isRendering);
    static void RunOnUpdateThread(Runnable* run, bool pForceQueue = false);
    static void ExecuteUpdateRunnables();
    static void Updating(bool isUpdating);
    //////////////////////////////////////////////////////////////////////////

    /// La chaîne représentant un accelerateur.
    CreateListDelegateSignature(Boost);

    CreateListDelegateSignature(House);

    /// La chaîne représentant le dossier.
    static const std::string NOM_DOSSIER_MEDIA;
    static const std::string NOM_EXTENSION;

    /// La chaîne représentant le type des buts.
    CreateListDelegateSignature(Goal);

    /// La chaîne représentant le type des murets statique.
    CreateListDelegateSignature(Wall);

    /// La chaîne représentant le type des bande exterieur de la patinoire.

    /// La chaîne représentant le type de la table de jeu.
    CreateListDelegateSignature(Table);

    /// La chaîne représentant le type des portails.
    CreateListDelegateSignature(Portal);

    /// La chaîne représentant le type de la rondelle.
    CreateListDelegateSignature(Puck);
    CreateListDelegateSignature(PuckTroll);


    /// La chaîne représentant le type des maillets.
    CreateListDelegateSignature(Mallet);

    /// La chaîne représentant le type des points de controle de la table.
    CreateListDelegateSignature(TableControlPoint);

    /// La chaîne représentant le type des points de controle d'objet.
    CreateListDelegateSignature(ControlPoint);

    /// La chaîne représentant le type des Zamboni.
    CreateListDelegateSignature(Zamboni);

    /// La chaîne représentant le type des bonus.
    CreateListDelegateSignature(EmptyBonus);
    CreateListDelegateSignature(Bonus);

    CreateListDelegateSignature(Goaler);
    CreateListDelegateSignature(Fan);

    static const char* RazerKeyToString[NB_RAZER_KEYS];

    // Converts an Key to its string representation, slow O(n) complexity
    // used for compatibility, avoid usage as much as possible.
    static const char* KeyToString(RazerKey key)
    {
        if(key<NB_RAZER_KEYS)return RazerKeyToString[key];
        return "Error";
    }


#if BOX2D_INTEGRATED  
    static void FillPhysicsFilters();
    static std::map<RazerKey,PhysicsFilter> EditionFilters;
    static std::map<RazerKey,PhysicsFilter> GameFilters;
    static void ApplyFilters(b2FixtureDef& def,RazerKey key, bool inGame);
#endif

};


#endif // __ENVIRONNEMENT_RAZERGAMEUTILITIES_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
