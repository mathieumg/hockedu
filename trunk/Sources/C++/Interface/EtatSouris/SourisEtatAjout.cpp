

//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjout.cpp
/// @author Gabriel Couvrette
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatAjout.h"
#include "FacadeModele.h"
#include "Terrain.h"
#include "ArbreRenduINF2990.h"
#include "HUDTexte.h"
#include "GestionnaireHUD.h"
#include "HUDElement.h"


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
SourisEtatAjout::SourisEtatAjout(std::string nomNoeudAjout): noeud_(NULL), nom_(nomNoeudAjout), hudTextPosInvalide(0)
{
	hudTextPosInvalide = new HUDTexte("Position invalide !", Vecteur4f(1,0,0,1),std::string("game_over_big_text"));
	hudTextPosInvalide->modifierVisibilite(false);
	hudTextPosInvalide->modifierPosition(0.45f,0.5f);

	GestionnaireHUD::obtenirInstance()->obtenirRacine(RACINE_EDITION)->add(hudTextPosInvalide);
	genererNoeud();
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
	if(noeud_ != NULL)
	{
		FacadeModele::obtenirInstance()->obtenirTerrain()->retirerNoeudTemp(noeud_);
		delete noeud_;
	}
	GestionnaireHUD::obtenirInstance()->obtenirRacine(RACINE_EDITION)->supprimerElement(hudTextPosInvalide);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::genererNoeud(std::string objet, std::string parent)
///
/// Crée un nouveau noeud et l'ajoute au parent
///
/// @param[in]	objet	: Nom représentant l'objet à ajouter
/// 			parent	: parent auquel ajouter le nouveau noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::genererNoeud()
{
	noeud_ =  FacadeModele::obtenirInstance()->obtenirTerrain()->obtenirArbreRendu()->creerNoeud(nom_);
	if(noeud_ != NULL)
		FacadeModele::obtenirInstance()->obtenirTerrain()->ajouterNoeudTemp(noeud_);	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est enfoncé
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::sourisEnfoncee( EvenementSouris& evenementSouris )
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est relaché
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::sourisRelachee( EvenementSouris& evenementSouris )
{
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		if(noeud_ != 0)
		{
			hudTextPosInvalide->modifierVisibilite(false);
			if(FacadeModele::obtenirInstance()->validerPositionNoeud(noeud_))
			{
				FacadeModele::obtenirInstance()->obtenirTerrain()->transfererNoeud(noeud_);
				genererNoeud();
				// La generation d'un noeud peut entrainer le noeud a etre invalide si on ne peut le creer
				if(noeud_ != 0)
				{
					Vecteur2i position=evenementSouris.obtenirPosition();
					Vecteur3 positionVirtuelle;
					FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
					noeud_->assignerPositionRelative(positionVirtuelle);
				}
			}
			else
			{
				hudTextPosInvalide->modifierVisibilite(true);
			}
		}

	}

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est déplacée
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjout::sourisDeplacee( EvenementSouris& evenementSouris )
{
	if(noeud_!=0)
	{
		// Pour s'assurer que le noeud est dans le bon arbre pour les cas ou le terrain serait reinitialiser entre temps
		FacadeModele::obtenirInstance()->obtenirTerrain()->ajouterNoeudTemp(noeud_);
		Vecteur2i position=evenementSouris.obtenirPosition();

		Vecteur3 positionVirtuelle;
		FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
		noeud_->assignerPositionRelative(positionVirtuelle);
		FacadeModele::obtenirInstance()->validerPositionNoeud(noeud_, true);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjout::obtenirNomEtatSouris()
///
/// Retourne le nom associé avec l'état de la souris
///
/// @param[in]	void
///
/// @return (int )NomEtatSouris	: nom correspondant à l'état
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatAjout::obtenirNomEtatSouris()
{
	if(nom_ == ArbreRenduINF2990::NOM_MAILLET)
		return ETAT_SOURIS_AJOUTER_MAILLET;
	if(nom_ == ArbreRenduINF2990::NOM_ACCELERATEUR)
		return ETAT_SOURIS_AJOUTER_ACCELERATEUR;
	if(nom_ == ArbreRenduINF2990::NOM_PORTAIL)
		return ETAT_SOURIS_AJOUTER_PORTAIL;
	if(nom_ == ArbreRenduINF2990::NOM_RONDELLE)
		return ETAT_SOURIS_AJOUTER_RONDELLE;
	if(nom_ == ArbreRenduINF2990::NOM_MURET)
		return ETAT_SOURIS_AJOUTER_MURET;
	return ETAT_SOURIS_INCONNU;
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


