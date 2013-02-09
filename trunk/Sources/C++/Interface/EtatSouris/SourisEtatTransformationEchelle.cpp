//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatTransformationEchelle.cpp
/// @author Michael Ferris
/// @date 2012-01-21
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatTransformationEchelle.h"
#include "FacadeModele.h"
#include "VisiteurEchelle.h"
#include "VisiteurCollision.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatTransformationEchelle::SourisEtatTransformationEchelle(  )
///
/// Initialisation des attributs � leur valeur de base
///
/// @param[in] context : R�f�rence du gestionnaire d'�tat de la souris
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatTransformationEchelle::SourisEtatTransformationEchelle(  )
{
	estEnfoncee_ = false;
	positionPrecedente_ = NULL;
    noeudSelectionnes_.clear(); 
    FacadeModele::getInstance()->getSelectedNodes(noeudSelectionnes_);
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
/// Comportement lorsqu'un bouton de la souris est enfonc�
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationEchelle::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	// V�rifie si le bouton enfonc� est le bouton de gauche
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concern�s
		estEnfoncee_ = true;
		positionPrecedente_ = evenementSouris.obtenirPosition();
	}
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationEchelle::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Comportement lorsqu'un bouton de la souris est relach�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationEchelle::sourisRelachee( EvenementSouris& evenementSouris )
{
	// V�rifie si le bouton enfonc� est le bouton de gauche
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		// Assigne les attributs concern�s
		positionPrecedente_ = NULL;
		estEnfoncee_ = false;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatTransformationEchelle::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Comportement lorsque la souris est deplac�e
///
/// @param[in] evenementSouris : contient les informations de la souris
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void SourisEtatTransformationEchelle::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Verifie si le bouton est enfonc�
	if(estEnfoncee_)
	{
		// Calcul le d�placement relatif entre depuis le dernier appel de la m�thode
		int deplacementY = positionPrecedente_[VY]-evenementSouris.obtenirPosition()[VY];

		float facteur;
		// Effectue la mise a l'echelle
		if(deplacementY>0)
		{
			//FacadeModele::getInstance()->modifierEchelle(1.05);
			facteur = 1.05f;
		}
		else if (deplacementY<0)
		{
			//FacadeModele::getInstance()->modifierEchelle(0.95);
			facteur = 0.95f;
		}
		else
			return;
		
		VisiteurEchelle visiteurEchelle(facteur, -1);
		VisiteurEchelle visiteurEchelleInverse(1.0f/facteur, -1);

		bool echelleValide = true;
		int i=0; 
		for(; i<noeudSelectionnes_.size(); i++)
		{

			noeudSelectionnes_[i]->acceptVisitor(visiteurEchelle);
			// On verifie qu'elle n'a pas engendre de nouvelles collisions
			if(!FacadeModele::getInstance()->validerPositionNoeud(noeudSelectionnes_[i]))
			{
				echelleValide = false;
				i++;
				break;
			}

		}
		if(!echelleValide)
		{
			for(int j=0; j<i; j++)
			{
				// S'il y a une collision, on fait l'operation inverse
				noeudSelectionnes_[j]->acceptVisitor(visiteurEchelleInverse);
			}
		}


		// Mise � jour de la position
		positionPrecedente_ = evenementSouris.obtenirPosition();
		
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatTransformationEchelle::obtenirNomEtatSouris()
///
/// Retourne l'�tat courant
///
///
/// @return L'�tat courant.
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatTransformationEchelle::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_TRANSFORMATION_ECHELLE;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////