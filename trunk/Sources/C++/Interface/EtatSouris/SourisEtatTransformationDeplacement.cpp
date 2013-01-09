//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationDeplacement.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatTransformationDeplacement.h"
#include "FacadeModele.h"
#include "VisiteurDeplacement.h"
#include "ArbreRenduINF2990.h"
#include "VisiteurCollision.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationDeplacement::SourisEtatTransformationDeplacement(  )
///
/// Initialisation des attributs à leur valeur de base
///
/// @param[in] context : Référence du gestionnaire d'état de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationDeplacement::SourisEtatTransformationDeplacement(  ):
ignoreCollision_(false),deplacementInverse_(0)
{
	estEnfoncee_ = false;
	positionPrecedente_ = NULL;

	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationDeplacement::~SourisEtatTransformationDeplacement(void)
///
/// Libération de la mémoire.
///
///
/// @return Aucune (destructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationDeplacement::~SourisEtatTransformationDeplacement(void)
{
	if(deplacementInverse_ != 0)
		delete[] deplacementInverse_;
	deplacementInverse_ = 0;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::toucheEnfoncee( EvenementClavier& evenementClavier )
///
/// Action a faire lorsqu'une touche est enfoncée. Ici, active le mode non-collision si la touche est ctrl.
///
/// @param[in] EvenementClavier & evenementClavier L'événement du clavier qui a déclenché cette méthode.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::toucheEnfoncee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_CONTROL)
		ignoreCollision_ = true;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::toucheRelachee( EvenementClavier& evenementClavier )
///
/// Comportement lorsque une touche du clavier est relachée. Si ctrl a été relaché, on réactive le mode collision.
///
/// @param[in] EvenementClavier & evenementClavier : événement clavier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::toucheRelachee( EvenementClavier& evenementClavier )
{
	if(evenementClavier.obtenirTouche() == VJAK_CONTROL)
	{
		ignoreCollision_ = false;
		if(estEnfoncee_)
		{
			for(int i=0; i<noeudsSelectionnes_.size(); i++)
			{
				if(!deplacementInverse_[i].estNul())
				{
					VisiteurDeplacement visiteurDeplacementInverse(deplacementInverse_[i]);
					noeudsSelectionnes_[i]->accueillirVisiteurNoeud(visiteurDeplacementInverse);
					deplacementInverse_[i].remetAZero();
				}
			}
			FacadeModele::obtenirInstance()->ajusterElementSurTableEnCollision();
		}
	}
}




////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est enfoncé
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		Vecteur3 positionVirtuelle;
		Vecteur2i positionCloture = evenementSouris.obtenirPosition();
		FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(positionCloture[VX], positionCloture[VY], positionVirtuelle);

		VisiteurCollision visiteurCollision(positionVirtuelle.convertir<2>(), false,50.0);
		FacadeModele::obtenirInstance()->visiterArbre(&visiteurCollision);
		bool objetSous = false;
		ConteneurNoeuds conteneurColision;
		if(visiteurCollision.collisionPresente())
		{
			visiteurCollision.obtenirListeCollision(conteneurColision);
			noeudsSelectionnes_ = FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990()->obtenirNoeudsSelectionnes();
			for(ConteneurNoeuds::iterator it1 = conteneurColision.begin(); it1!=conteneurColision.end(); it1++)
			{
				for(ConteneurNoeuds::iterator it2 = noeudsSelectionnes_.begin(); it2!=noeudsSelectionnes_.end(); it2++)
				{
					if((*it1)==(*it2))
					{
						objetSous = true;
						//break;
					}
				}
			}
			//delete visiteurCollision;
			// Vérifie si le bouton enfoncé est le bouton de gauche et qu'il y a un objet selectionne sous la souris
			if(objetSous)
			{
				// Assigne les attributs concernés
				estEnfoncee_ = true;
				positionPrecedente_ = evenementSouris.obtenirPosition();
				deplacementInverse_ = new Vecteur2[noeudsSelectionnes_.size()];
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relachée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::sourisRelachee( EvenementSouris& evenementSouris )
{
	// Vérifie si le bouton enfoncé est le bouton de gauche
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE && estEnfoncee_)
	{
		// Assigne les attributs concernés
		positionPrecedente_ = NULL;
		estEnfoncee_ = false;
		for(int i=0; i<noeudsSelectionnes_.size(); i++)
		{
			if(!deplacementInverse_[i].estNul())
			{
				VisiteurDeplacement visiteurDeplacementInverse(deplacementInverse_[i]);
				noeudsSelectionnes_[i]->accueillirVisiteurNoeud(visiteurDeplacementInverse);
				deplacementInverse_[i].remetAZero();
			}
		}
		FacadeModele::obtenirInstance()->ajusterElementSurTableEnCollision();
		delete[] deplacementInverse_;
		deplacementInverse_ = 0;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationDeplacement::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est deplacée
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationDeplacement::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Verifie si le bouton est enfoncé
	if(estEnfoncee_ && evenementSouris.obtenirPosition()!=positionPrecedente_)
	{
		// Calcul le déplacement relatif entre depuis le dernier appel de la méthode
		int deplacementX = positionPrecedente_[VX]-evenementSouris.obtenirPosition()[VX];
		int deplacementY = positionPrecedente_[VY]-evenementSouris.obtenirPosition()[VY];

		Vecteur2 deplacementCloture(deplacementX, deplacementY);
		Vecteur2 deplacementVirt = FacadeModele::obtenirInstance()->convertirDeplacementClotureAVirtuelle(positionPrecedente_, evenementSouris.obtenirPosition());
		Vecteur2 deplacementVirtInverse = Vecteur2(-deplacementVirt[VX], -deplacementVirt[VY]);

		VisiteurDeplacement visiteurDeplacement(deplacementVirt);
		VisiteurDeplacement visiteurDeplacementInverse(deplacementVirtInverse);

		for(int i=0; i<noeudsSelectionnes_.size(); i++)
		{
			noeudsSelectionnes_[i]->accueillirVisiteurNoeud(visiteurDeplacement);
			// On verifie qu'elle n'a pas engendre de nouvelles collisions
			if(!FacadeModele::obtenirInstance()->validerPositionNoeud(noeudsSelectionnes_[i]))
			{
				if(ignoreCollision_)
				{
					deplacementInverse_[i] += deplacementVirtInverse;
				}
				else
					noeudsSelectionnes_[i]->accueillirVisiteurNoeud(visiteurDeplacementInverse);
			}
			else
				deplacementInverse_[i].remetAZero();
		}

		// Mise à jour de la position
		positionPrecedente_ = evenementSouris.obtenirPosition();
		
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatTransformationDeplacement::obtenirNomEtatSouris()
///
/// Retourne l'état courant
///
///
/// @return L'état courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatTransformationDeplacement::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_TRANSFORMATION_DEPLACEMENT;
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////