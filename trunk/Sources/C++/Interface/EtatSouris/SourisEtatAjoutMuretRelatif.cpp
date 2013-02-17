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

#ifdef MIKE_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif

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
    if(!createNextControlPoint())
    {
        if(noeud_)
        {
            if(noeud_->GetTerrain())
            {
                noeud_->GetTerrain()->retirerNoeudTemp(noeud_);
            }
            delete noeud_;
            noeud_ = NULL;
        }
        if(mCurrentPoint)
        {
            delete mCurrentPoint;
            mCurrentPoint = NULL;
        }
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


    if(!createNextControlPoint())
    {
        noeud_->GetTerrain()->transfererNoeud(noeud_);
        genererNoeud();
        if(!createNextControlPoint())
        {
            if(noeud_)
            {
                if(noeud_->GetTerrain())
                {
                    noeud_->GetTerrain()->retirerNoeudTemp(noeud_);
                }
                delete noeud_;
                noeud_ = NULL;
            }
            if(mCurrentPoint)
            {
                delete mCurrentPoint;
                mCurrentPoint = NULL;
            }
        }
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

		// Appel pour faire le surlignement
		FacadeModele::getInstance()->validerPositionNoeud(noeud_, true);
    }	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMuretRelatif::createNextControlPoint()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
bool SourisEtatAjoutMuretRelatif::createNextControlPoint()
{
    ControlPointMutableAbstract* controlNode = dynamic_cast<ControlPointMutableAbstract*>(noeud_);
    if(controlNode)
    {
        if(controlNode->getMaxControlPoints() == 0)
        {
            if(noeud_->GetTerrain())
            {
                noeud_->GetTerrain()->retirerNoeudTemp(noeud_);
                delete noeud_;
                noeud_  = NULL;
                return false;
            }
            delete noeud_; 
            return false;
        }
        if(controlNode->getNBControlPoint() < controlNode->getMaxControlPoints())
        {
            mCurrentPoint = new NodeControlPoint(RazerGameUtilities::NAME_CONTROL_POINT);
            bool res = noeud_->ajouter(mCurrentPoint);
            checkf(res);
            return res;
        }
    }
    return false;
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
}

#ifdef MIKE_DEBUG
PRAGMA_ENABLE_OPTIMIZATION
#endif


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


