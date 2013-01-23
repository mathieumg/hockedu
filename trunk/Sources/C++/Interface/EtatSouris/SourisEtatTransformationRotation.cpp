//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationRotation.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatTransformationRotation.h"
#include "FacadeModele.h"
#include "VisiteurRotation.h"
#include "ArbreRenduINF2990.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationRotation::SourisEtatTransformationRotation(  )
///
/// Initialisation des attributs � leur valeur de base
///
/// @param[in] context : R�f�rence du gestionnaire d'�tat de la souris
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
/// Ce destructeur lib�re la m�moire utilis�e.
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationRotation::~SourisEtatTransformationRotation(void)
{
	if(rotationInverse_!=0)
		delete[] rotationInverse_;
	rotationInverse_ = 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Comportement lorsque une touche du claiver est enfonc�e (Active le mode sans collisions si ctrl est enfonc�.)
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::toucheEnfoncee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_CONTROL)
		ignoreCollision_ = true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement lorsque une touche du claiver est relach�e (Ici, d�sactive le mode non-collosion)
///
/// @param[in] EvenementClavier & evenementClavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::toucheRelachee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_CONTROL)
	{
		ignoreCollision_ = false;
		if(estEnfoncee_)
		{
			for(int i=0; i<noeudsSelectionnes_.size(); i++)
			{
				if(rotationInverse_[i] != 0)
				{
					VisiteurRotation visiteurRotationInverse((float)rotationInverse_[i],centreRot_);
					noeudsSelectionnes_[i]->acceptVisitor(visiteurRotationInverse);
					rotationInverse_[i] = 0;
				}
			}
			FacadeModele::getInstance()->ajusterElementSurTableEnCollision();
		}
	}
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfonc�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	// V�rifie si le bouton enfonc� est le bouton de gauche
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concern�s
		estEnfoncee_ = true;
		positionPrecedente_ = evenementSouris.obtenirPosition();
		noeudsSelectionnes_ = FacadeModele::getInstance()->obtenirArbreRenduINF2990()->obtenirNoeudsSelectionnes();
		centreRot_ = obtenirCentreRot();
		rotationInverse_ = new int[noeudsSelectionnes_.size()];
		for (int i = 0; i < noeudsSelectionnes_.size() ; i++)
		{
			rotationInverse_[i] = 0;
		}
	}
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relach�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::sourisRelachee( EvenementSouris& evenementSouris )
{
	// V�rifie si le bouton enfonc� est le bouton de gauche
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concern�s
		positionPrecedente_ = NULL;
		centreRot_ = NULL;
		estEnfoncee_ = false;

		for(int i=0; i<noeudsSelectionnes_.size(); i++)
		{
			if(rotationInverse_[i] != 0)
			{
				VisiteurRotation visiteurRotationInverse((float)rotationInverse_[i],centreRot_);
				noeudsSelectionnes_[i]->acceptVisitor(visiteurRotationInverse);
				rotationInverse_[i] = 0;
			}
		}
		FacadeModele::getInstance()->ajusterElementSurTableEnCollision();

		delete[] rotationInverse_;
		rotationInverse_ = 0;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationRotation::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est deplac�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationRotation::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Verifie si le bouton est enfonc�
	if(estEnfoncee_)
	{
		// Calcul le d�placement relatif depuis le dernier appel de la m�thode
		int angle = -positionPrecedente_[VY]+evenementSouris.obtenirPosition()[VY];
		//int deplacementX = positionPrecedente_[VX]-evenementSouris.obtenirPosition()[VX];


		// Effectue la rotation		
		VisiteurRotation visiteurRotation((float)angle, centreRot_);
		VisiteurRotation visiteurRotationInverse((float)-angle, centreRot_);
		bool rotationValide = true;
		
		int i=0; 
		for(; i<noeudsSelectionnes_.size(); i++)
		{
			noeudsSelectionnes_[i]->acceptVisitor(visiteurRotation);
			// On verifie qu'elle n'a pas engendre de nouvelles collisions
			if(!FacadeModele::getInstance()->validerPositionNoeud(noeudsSelectionnes_[i]))
			{
				if(ignoreCollision_)
				{
					rotationInverse_[i] -= angle;
				}
				else
					noeudsSelectionnes_[i]->acceptVisitor(visiteurRotationInverse);
			}
			else
				rotationInverse_[i] = 0;
		}
		
		// Mise � jour de la position
		positionPrecedente_ = evenementSouris.obtenirPosition();
		
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatTransformationRotation::obtenirNomEtatSouris()
///
/// Retourne l'�tat courant
///
///
/// @return L'�tat courant.
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
/// S'assurer que les noeuds selectionn�e ont �t� charg� avant d'appeler cette m�thode.
///
/// @return Vecteur2
///
////////////////////////////////////////////////////////////////////////
Vecteur2 SourisEtatTransformationRotation::obtenirCentreRot() const
{
	if(noeudsSelectionnes_.size()!=0)
	{
		float min[2], max[2];
		Vecteur3 positionAbsolue = static_cast<NoeudComposite*>(noeudsSelectionnes_[0])->obtenirPositionAbsolue();
		min[VX] = positionAbsolue[VX];
		min[VY] = positionAbsolue[VY];
		max[VX] = positionAbsolue[VX];
		max[VY] = positionAbsolue[VY];


		for(int i=0; i<noeudsSelectionnes_.size(); i++)
		{
			Vecteur3 positionAbsolue = noeudsSelectionnes_[i]->obtenirPositionAbsolue();
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
