//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutRondelle.cpp
/// @author Gabriel Couvrette
/// @date 2012-02-13
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SourisEtatAjoutRondelle.h"
#include "FacadeModele.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutRondelle::SourisEtatAjoutRondelle()
///
/// Ce constructeur appelle le constructeur de base, cree un noeud de base afin de voir un affichage. 
///
/// @param[in] GestionnaireEtatAbstrait * contexte
///
/// @return ien. Constructeur.
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutRondelle::SourisEtatAjoutRondelle()
{
	arbreDeRendu_=FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990();
	genererNoeud(ArbreRenduINF2990::NOM_RONDELLE);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutRondelle::~SourisEtatAjoutRondelle(void) 
///
/// Destructeur de la classe qui supprime le noeud en cours d'ajout.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutRondelle::~SourisEtatAjoutRondelle(void)
{
	FacadeModele::obtenirInstance()->obtenirArbreRenduINF2990()->effacer(rondelleConcerne_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutRondelle::genererNoeud(std::string objet, std::string parent)
///
/// Crée un nouveau noeud et l'ajoute au parent
///
/// @param[in] std::string objet : Nom représentant l'objet à ajouter
/// @param[in] std::string parent : Parent auquel ajouter le nouveau noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutRondelle::genererNoeud()
{
	rondelleConcerne_=arbreDeRendu_->ajouterNouveauNoeud(parent,objet);
	if(rondelleConcerne_ != 0)
		rondelleConcerne_->assignerEstEnregistrable(false);		
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutRondelle::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est enfoncé
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutRondelle::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	// Rien à faire ici.
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutRondelle::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est relaché.
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutRondelle::sourisRelachee( EvenementSouris& evenementSouris )
{
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		if(rondelleConcerne_!=0 && FacadeModele::obtenirInstance()->validerPositionNoeud(rondelleConcerne_))
		{
			Vecteur2i position=evenementSouris.obtenirPosition();

			Vecteur3 positionVirtuelle;
			FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
		
			if(FacadeModele::obtenirInstance()->validerPositionNoeud(rondelleConcerne_))
			{
				rondelleConcerne_->assignerPositionRelative(positionVirtuelle);
				rondelleConcerne_->assignerEstEnregistrable(true);					
				genererNoeud(ArbreRenduINF2990::NOM_RONDELLE);
			}
		}
	}
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutRondelle::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'on déplace la souris avec bouton enfoncé
///
/// @param[in] EvenementSouris & evenementSouris : événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutRondelle::sourisDeplacee( EvenementSouris& evenementSouris )
{
	if(rondelleConcerne_!=0)
	{
		Vecteur2i position=evenementSouris.obtenirPosition();

		Vecteur3 positionVirtuelle;
		FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
		rondelleConcerne_->assignerPositionRelative(positionVirtuelle);
		FacadeModele::obtenirInstance()->validerPositionNoeud(rondelleConcerne_, true);
	}
}
////////////////////////////////////////////////////////////////////////
///
/// @fn NomEtatSouris SourisEtatAjoutRondelle::obtenirNomEtatSouris()
///
/// Retourne le nom associé avec l'état de la souris
///
///
/// @return NomEtatSouris
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatAjoutRondelle::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_AJOUTER_RONDELLE;
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////

