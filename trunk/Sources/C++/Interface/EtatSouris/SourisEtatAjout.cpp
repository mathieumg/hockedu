//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjout.cpp
/// @author Michael Ferris
/// @date 2013-03-23
/// @version 2.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatAjout.h"
#include "FacadeModele.h"
#include "Terrain.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjout::SourisEtatAjout()
///
/// Ce constructeur appelle le constructeur de base, cree un noeud de base afin de voir un affichage
///
/// @param[in] GestionnaireEtatAbstrait* : contexte.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjout::SourisEtatAjout(FieldModificationStrategyType modifType): 
    SourisEtatTransformation(modifType)
{
    FieldModificationStrategyEvent event;
    event.mPosition = Vecteur2(0,0);
    event.mType = FIELD_MODIFICATION_EVENT_CLICK;
    FacadeModele::getInstance()->getEditionField()->BeginModification(mModifType,event);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::~SourisEtatAjoutMuret(void)
///
/// Ce destructeur supprime le noeud en cours d'ajout
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjout::~SourisEtatAjout(void)
{
    FacadeModele::getInstance()->getEditionField()->cancelModification();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfoncé
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::sourisEnfoncee( EvenementSouris& evenementSouris )
{


}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relachée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        Vecteur3 position;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX],evenementSouris.obtenirPosition()[VY],position);
        FieldModificationStrategyEvent event;
        event.mPosition = position;
        event.mType = FIELD_MODIFICATION_EVENT_CLICK;
        FacadeModele::getInstance()->getEditionField()->ReceiveModificationEvent(event);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est deplacée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::sourisDeplacee( EvenementSouris& evenementSouris )
{
    Vecteur3 position;
    FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX],evenementSouris.obtenirPosition()[VY],position);
    FieldModificationStrategyEvent event;
    event.mPosition = position;
    event.mType = FIELD_MODIFICATION_EVENT_MOVE;
    FacadeModele::getInstance()->getEditionField()->ReceiveModificationEvent(event);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::toucheRelachee( EvenementClavier& evenementClavier )
///
/// /*Description*/
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::toucheRelachee( EvenementClavier& evenementClavier )
{

}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


