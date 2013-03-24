//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformation.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatTransformation.h"
#include "FacadeModele.h"
#include "Terrain.h"
#include "FieldModificationStrategyAbstract.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformation::SourisEtatTransformation(  )
///
/// Initialisation des attributs à leur valeur de base
///
/// @param[in] context : Référence du gestionnaire d'état de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformation::SourisEtatTransformation( FieldModificationStrategyType modifType ):
mModifType(modifType)
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformation::~SourisEtatTransformation(void)
///
/// Ce destructeur libère la mémoire utilisée.
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformation::~SourisEtatTransformation(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformation::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement lorsque une touche du claiver est relachée (Ici, désactive le mode non-collosion)
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformation::toucheRelachee( EvenementClavier& evenementClavier )
{
    if(evenementClavier.obtenirTouche() == VJAK_ESCAPE)
    {
        FacadeModele::getInstance()->getEditionField()->cancelModification();
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformation::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfoncée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformation::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        Vecteur3 position;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX],evenementSouris.obtenirPosition()[VY],position);
        FieldModificationStrategyEvent event;
        event.mPosition = position;
        event.mType = FIELD_MODIFICATION_EVENT_CLICK;

        FacadeModele::getInstance()->getEditionField()->BeginModification(mModifType,event);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformation::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relachée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformation::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        FacadeModele::getInstance()->getEditionField()->EndModification();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformation::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est deplacée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformation::sourisDeplacee( EvenementSouris& evenementSouris )
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
/// @fn NomEtatSouris SourisEtatTransformation::obtenirNomEtatSouris()
///
/// Retourne l'état courant
///
///
/// @return L'état courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatTransformation::obtenirNomEtatSouris()
{
    switch (mModifType)
    {
    case FIELD_MODIFICATION_MOVE:
        return ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT;

    case FIELD_MODIFICATION_ROTATE:
        return ETAT_SOURIS_TRANSFORMATION_ROTATION;

    case FIELD_MODIFICATION_SCALE:
        return ETAT_SOURIS_TRANSFORMATION_ECHELLE;

    case FIELD_MODIFICATION_ADD_PORTAL:
        return ETAT_SOURIS_AJOUTER_PORTAIL;

    case FIELD_MODIFICATION_ADD_BOOST:
        return ETAT_SOURIS_AJOUTER_ACCELERATEUR;

    case FIELD_MODIFICATION_ADD_WALL:
        return ETAT_SOURIS_AJOUTER_MURET;

    case FIELD_MODIFICATION_ADD_MALLET:
        return ETAT_SOURIS_AJOUTER_MAILLET;

    case FIELD_MODIFICATION_ADD_PUCK:
        return ETAT_SOURIS_AJOUTER_RONDELLE;

    case FIELD_MODIFICATION_ADD_BONUS:
        return ETAT_SOURIS_AJOUTER_BONUS;
    }

    return ETAT_SOURIS_INCONNU;
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
