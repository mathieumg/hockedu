//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutMuretRelatif.cpp
/// @author Mathieu Parent
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatAjoutMuretRelatif.h"
#include "FacadeModele.h"
#include "NodeWallEdition.h"
#include "NodeControlPoint.h"
#include "SourisEtatAjout.h"
#include "Terrain.h"



////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutMuretRelatif::SourisEtatAjoutMuretRelatif()
///
/// Ce constructeur appelle le constructeur de base, cree un noeud de base afin de voir un affichage et initialise les attributs
///
/// @param[in] GestionnaireEtatAbstrait* : contexte.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutMuretRelatif::SourisEtatAjoutMuretRelatif():
    SourisEtatAjout(RazerGameUtilities::NAME_RELATIVE_WALL),mCurrentPoint(NULL)
{
    mControlPoints.clear();
    NodeWallEdition *wall = dynamic_cast<NodeWallEdition *>(noeud_);
    if (wall)
    {
        mControlPoints.resize(wall->getNBControlPoint());
        for(unsigned int i=0; i < wall->getNBControlPoint(); ++i)
        {
            mControlPoints[i] = wall->getControlPoint(i);
        }
        getNextControlPoint();
    }
}

	
////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutMuretRelatif::~SourisEtatAjoutMuretRelatif(void)
///
/// Ce destructeur supprime le noeud en cours d'ajout
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutMuretRelatif::~SourisEtatAjoutMuretRelatif(void)
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMuretRelatif::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est enfoncé
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMuretRelatif::sourisEnfoncee( EvenementSouris& evenementSouris )
{ 

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMuretRelatif::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est relaché
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMuretRelatif::sourisRelachee( EvenementSouris& evenementSouris )
{
	if(noeud_ == NULL)
		return;

	if(evenementSouris.obtenirBouton()!=BOUTON_SOURIS_GAUCHE)
		return;
	if(!FacadeModele::getInstance()->validerPositionNoeud(noeud_))
    {	
        showInvalidText(true);
		return;
	}
    showInvalidText(false);


    getNextControlPoint();
    if(!mCurrentPoint)
    {
        // nous avons utiliser tous les point de controle
        FacadeModele::getInstance()->getTerrain()->transfererNoeud(noeud_);
        genererNoeud();
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMuretRelatif::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est déplacée
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMuretRelatif::sourisDeplacee( EvenementSouris& evenementSouris )
{
	if(noeud_ == NULL)
		return;


    if(mCurrentPoint)
    {
        auto positionSouris = evenementSouris.obtenirPosition();
        Vecteur3 positionVirtuelle;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(positionSouris[VX],positionSouris[VY],positionVirtuelle);


        mCurrentPoint->setPosition(positionVirtuelle);
        std::for_each(mControlPoints.begin(),mControlPoints.end(),[&](NodeControlPoint* point)
            {
                point->setPosition(positionVirtuelle);
            });

		// Appel pour faire le surlignement
		FacadeModele::getInstance()->validerPositionNoeud(noeud_, true);
    }	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMuretRelatif::getNextControlPoint()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMuretRelatif::getNextControlPoint()
{
    if(!mControlPoints.empty())
    {
        mCurrentPoint = mControlPoints.back();
        mControlPoints.pop_back();
    }
    else
    {
        mCurrentPoint = NULL;
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMuretRelatif::genererNoeud()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMuretRelatif::genererNoeud()
{
    Super::genererNoeud();
    mControlPoints.clear();
    NodeWallEdition *wall = dynamic_cast<NodeWallEdition *>(noeud_);
    if (wall)
    {
        mControlPoints.resize(wall->getNBControlPoint());
    	for(unsigned int i=0; i < wall->getNBControlPoint(); ++i)
        {
            mControlPoints[i] = wall->getControlPoint(i);
        }
        getNextControlPoint();
    }
}





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


