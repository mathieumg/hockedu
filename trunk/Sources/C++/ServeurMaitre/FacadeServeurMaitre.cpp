///////////////////////////////////////////////////////////////////////////////
/// @file FacadeServeurMaitre.cpp
/// @author Mathieu Parent
/// @date 2013-02-11
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "FacadeServeurMaitre.h"

// Initialisations automatiques
SINGLETON_DECLARATION_CPP(FacadeServeurMaitre);

////////////////////////////////////////////////////////////////////////
///
/// @fn  FacadeServeurMaitre::FacadeServeurMaitre(  )
///
/// Constructeur
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
FacadeServeurMaitre::FacadeServeurMaitre()
{
    

}


////////////////////////////////////////////////////////////////////////
///
/// @fn  GestionnaireReseauClientLourd::~GestionnaireReseauClientLourd(  )
///
/// Destructeur 
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
FacadeServeurMaitre::~FacadeServeurMaitre()
{
    
}



bool FacadeServeurMaitre::saveJoueurConnecting( JoueurServeurs* pJoueur )
{
    // Retourne true si le joueur a bien pu etre sauvegarder, false si un joueur avec le meme nom est deja en train de se connecter
    return (mListeJoueursConnecting[pJoueur->getName()] = pJoueur) != pJoueur;
}




void FacadeServeurMaitre::updateGameStatus( const PartieServeurs& pPartieInfos )
{
    // On trouve la partie recherchee






}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////