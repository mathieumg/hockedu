//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationDeplacement.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatTransformationDeplacement.h"
#include "FacadeModele.h"
#include "Terrain.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationDeplacement::SourisEtatTransformationDeplacement(  )
///
/// Initialisation des attributs � leur valeur de base
///
/// @param[in] context : R�f�rence du gestionnaire d'�tat de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationDeplacement::SourisEtatTransformationDeplacement(  )
{
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationDeplacement::~SourisEtatTransformationDeplacement(void)
///
/// Lib�ration de la m�moire.
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationDeplacement::~SourisEtatTransformationDeplacement(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Action a faire lorsqu'une touche est enfonc�e. Ici, active le mode non-collision si la touche est ctrl.
///
/// @param[in] EvenementClavier & evenementClavier L'�v�nement du clavier qui a d�clench� cette m�thode.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::toucheEnfoncee( EvenementClavier& evenementClavier )
{

}



////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement lorsque une touche du clavier est relach�e. Si ctrl a �t� relach�, on r�active le mode collision.
///
/// @param[in] EvenementClavier & evenementClavier : �v�nement clavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::toucheRelachee( EvenementClavier& evenementClavier )
{

}



////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfonc�
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        Vecteur3 position;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX],evenementSouris.obtenirPosition()[VY],position);
        FieldModificationStrategyEvent event;
        event.mPosition = position;
        event.mType = FIELD_MODIFICATION_STRATEGY_EVENT_TYPE_CLICK;

        FacadeModele::getInstance()->getEditionField()->BeginModification(FIELD_MODIFICATION_STRATEGY_TYPE_MOVE_NODES,event);
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relach�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        FacadeModele::getInstance()->getEditionField()->EndModification();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est deplac�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::sourisDeplacee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        Vecteur3 position;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX],evenementSouris.obtenirPosition()[VY],position);
        FieldModificationStrategyEvent event;
        event.mPosition = position;
        event.mType = FIELD_MODIFICATION_STRATEGY_EVENT_TYPE_MOVE;
        FacadeModele::getInstance()->getEditionField()->ReceiveModificationEvent(event);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatTransformationDeplacement::obtenirNomEtatSouris()
///
/// Retourne l'�tat courant
///
///
/// @return L'�tat courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatTransformationDeplacement::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT;
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////