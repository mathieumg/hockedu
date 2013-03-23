//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationRotation.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatTransformationRotation.h"
#include "FacadeModele.h"
#include "Terrain.h"
#include "FieldModificationStrategyAbstract.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationRotation::SourisEtatTransformationRotation(  )
///
/// Initialisation des attributs à leur valeur de base
///
/// @param[in] context : Référence du gestionnaire d'état de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationRotation::SourisEtatTransformationRotation(  ):
rotationInverse_(0),ignoreCollision_(false)
{
	estEnfoncee_ = false;
	positionPrecedente_ = NULL;
	centreRot_ = NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationRotation::~SourisEtatTransformationRotation(void)
///
/// Ce destructeur libère la mémoire utilisée.
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationRotation::~SourisEtatTransformationRotation(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Comportement lorsque une touche du claiver est enfoncée (Active le mode sans collisions si ctrl est enfoncé.)
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::toucheEnfoncee( EvenementClavier& evenementClavier )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement lorsque une touche du claiver est relachée (Ici, désactive le mode non-collosion)
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::toucheRelachee( EvenementClavier& evenementClavier )
{
    if(evenementClavier.obtenirTouche() == VJAK_ESCAPE)
    {
        FacadeModele::getInstance()->getEditionField()->cancelModification();
    }
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfoncée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::sourisEnfoncee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        Vecteur3 position;
        FacadeModele::getInstance()->convertirClotureAVirtuelle(evenementSouris.obtenirPosition()[VX],evenementSouris.obtenirPosition()[VY],position);
        FieldModificationStrategyEvent event;
        event.mPosition = position;
        event.mType = FIELD_MODIFICATION_EVENT_CLICK;

        FacadeModele::getInstance()->getEditionField()->BeginModification(FIELD_MODIFICATION_ROTATE,event);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relachée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::sourisRelachee( EvenementSouris& evenementSouris )
{
    if(evenementSouris.obtenirBouton() == BOUTON_SOURIS_GAUCHE)
    {
        FacadeModele::getInstance()->getEditionField()->EndModification();
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est deplacée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::sourisDeplacee( EvenementSouris& evenementSouris )
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
/// @fn NomEtatSouris SourisEtatTransformationRotation::obtenirNomEtatSouris()
///
/// Retourne l'état courant
///
///
/// @return L'état courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatTransformationRotation::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_TRANSFORMATION_ROTATION;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Vecteur2 SourisEtatTransformationRotation::obtenirCentreRot()
///
/// Algortihme pour obtenir le centre de rotation
/// S'assurer que les noeuds selectionnée ont été chargé avant d'appeler cette méthode.
///
/// @return Vecteur2
///
////////////////////////////////////////////////////////////////////////
Vecteur2 SourisEtatTransformationRotation::obtenirCentreRot() const
{
	if(noeudsSelectionnes_.size()!=0)
	{
		float min[2], max[2];
		Vecteur3 positionAbsolue = noeudsSelectionnes_[0]->getPosition();
		min[VX] = positionAbsolue[VX];
		min[VY] = positionAbsolue[VY];
		max[VX] = positionAbsolue[VX];
		max[VY] = positionAbsolue[VY];


		for(int i=1; i<noeudsSelectionnes_.size(); i++)
		{
			Vecteur3 positionAbsolue = noeudsSelectionnes_[i]->getPosition();
			if(positionAbsolue[VX]<min[VX])
				min[VX] = positionAbsolue[VX];
			if(positionAbsolue[VY]<min[VY])
				min[VY] = positionAbsolue[VY];
			if(positionAbsolue[VX]>max[VX])
				min[VX] = positionAbsolue[VX];
			if(positionAbsolue[VY]>max[VY])
				min[VY] = positionAbsolue[VY];
		}

		Vecteur2 centreRot((max[VX]-min[VX])/2.0f+min[VX], (max[VY]-min[VY])/2.0f+min[VY]);
		return centreRot;
	}
	else
	{
		return Vecteur2(0, 0);
	}
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
