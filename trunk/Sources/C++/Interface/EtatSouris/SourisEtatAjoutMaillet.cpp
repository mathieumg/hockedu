//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutMaillet.cpp
/// @author Gabriel Couvrette
/// @date 2012-02-13
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAjoutMaillet.h"
#include "FacadeModele.h"
#include "Terrain.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutMaillet::SourisEtatAjoutMaillet()
///
/// Ce constructeur initialise les options requises afin d'avoir un �tat AjouterMaillet.
///
/// @param[in] GestionnaireEtatAbstrait * contexte Le contexte pr�sentemant utilis�.
///
/// @return Rien. C'est un constructeur.
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutMaillet::SourisEtatAjoutMaillet():mailletConcerne_(0)
{
	genererNoeud();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutMaillet::~SourisEtatAjoutMaillet(void) 
///
/// Destructeur de la classe qui supprime le noeud en cours d'ajout.
///
/// @param[in] void
///
/// @return Rien. C'est un destructeur.
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutMaillet::~SourisEtatAjoutMaillet(void)
{
	if(mailletConcerne_ != 0)
	{
		FacadeModele::obtenirInstance()->obtenirTerrain()->effacerNoeudTemp(mailletConcerne_);
		delete mailletConcerne_;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMaillet::genererNoeud(std::string objet, std::string parent)
///
/// Cr�e un nouveau noeud et l'ajoute au parent
///
/// @param[in] std::string objet : Nom repr�sentant l'objet � ajouter
/// @param[in] std::string parent : Parent auquel ajouter le nouveau noeud (NOEUD_TABLE par d�faut)
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMaillet::genererNoeud()
{
	mailletConcerne_ =  FacadeModele::obtenirInstance()->obtenirTerrain()->obtenirArbreRendu()->creerNoeud(ArbreRenduINF2990::NOM_MAILLET);
	if(mailletConcerne_ != 0)
		FacadeModele::obtenirInstance()->obtenirTerrain()->ajouterNoeudTemp(mailletConcerne_);	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMaillet::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Action � effectuer lorsqu'un bouton de la souris est enfonc�
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMaillet::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	// Rien � faire ici.
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMaillet::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Action � effectuer lorsqu'un bouton de la souris est relach�.
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMaillet::sourisRelachee( EvenementSouris& evenementSouris )
{
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		if(mailletConcerne_ != 0)
		{
			if(FacadeModele::obtenirInstance()->validerPositionNoeud(mailletConcerne_))
			{
				FacadeModele::obtenirInstance()->obtenirTerrain()->transfererNoeud(mailletConcerne_);
				genererNoeud();
				// La generation d'un noeud peut entrainer le noeud a etre invalide si on ne peut le creer
				if(mailletConcerne_ != 0)
				{
					Vecteur2i position=evenementSouris.obtenirPosition();
					Vecteur3 positionVirtuelle;
					FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
					mailletConcerne_->assignerPositionRelative(positionVirtuelle);
				}
			}
		}

	}
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutMaillet::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Action � effectuer lorsqu'on d�place la souris avec bouton enfonc�
///
/// @param[in] EvenementSouris & evenementSouris : �v�nement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutMaillet::sourisDeplacee( EvenementSouris& evenementSouris )
{
	if(mailletConcerne_!=0)
	{
		// Pour s'assurer que le noeud est dans le bon arbre pour les cas ou le terrain serait reinitialiser entre temps
		FacadeModele::obtenirInstance()->obtenirTerrain()->ajouterNoeudTemp(mailletConcerne_);
		Vecteur2i position=evenementSouris.obtenirPosition();

		Vecteur3 positionVirtuelle;
		FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
		mailletConcerne_->assignerPositionRelative(positionVirtuelle);
		FacadeModele::obtenirInstance()->validerPositionNoeud(mailletConcerne_, true);
	}
}
////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatAjoutMaillet::obtenirNomEtatSouris()
///
/// Retourne le nom associ� avec l'�tat de la souris
///
///
/// @return NomEtatSouris
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatAjoutMaillet::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_AJOUTER_MAILLET;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////



