//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationEchelle.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatTransformationEchelle.h"
#include "FacadeModele.h"
#include "VisiteurEchelle.h"
#include "VisiteurCollision.h"
#include "Terrain.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationEchelle::SourisEtatTransformationEchelle(  )
///
/// Initialisation des attributs à leur valeur de base
///
/// @param[in] context : Référence du gestionnaire d'état de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationEchelle::SourisEtatTransformationEchelle(  )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationEchelle::~SourisEtatTransformationEchelle(void)
///
/// Ce destructeur ne fait rien
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationEchelle::~SourisEtatTransformationEchelle(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationEchelle::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfoncé
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationEchelle::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        Vecteur3 position;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX],evenementSouris.obtenirPosition()[VY],position);
        FieldModificationStrategyEvent event;
        event.mPosition = position;
        event.mType = FIELD_MODIFICATION_EVENT_CLICK;

        FacadeModele::getInstance()->getEditionField()->BeginModification(FIELD_MODIFICATION_SCALE,event);
    }

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationEchelle::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relachée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationEchelle::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        FacadeModele::getInstance()->getEditionField()->EndModification();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationEchelle::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est deplacée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationEchelle::sourisDeplacee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        Vecteur3 position;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX],evenementSouris.obtenirPosition()[VY],position);
        FieldModificationStrategyEvent event;
        event.mPosition = position;
        event.mType = FIELD_MODIFICATION_EVENT_MOVE;
        FacadeModele::getInstance()->getEditionField()->ReceiveModificationEvent(event);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatTransformationEchelle::obtenirNomEtatSouris()
///
/// Retourne l'état courant
///
///
/// @return L'état courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatTransformationEchelle::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_TRANSFORMATION_ECHELLE;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationEchelle::toucheRelachee( EvenementClavier& evenementClavier )
///
/// /*Description*/
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationEchelle::toucheRelachee( EvenementClavier& evenementClavier )
{
    if(evenementClavier.obtenirTouche() == VJAK_ESCAPE)
    {
        FacadeModele::getInstance()->getEditionField()->cancelModification();
    }
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////