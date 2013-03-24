//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutPortail.cpp
/// @author Gabriel Couvrette
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatAjoutPortail.h"
#include "FacadeModele.h"
#include "Terrain.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutPortail::SourisEtatAjoutPortail()
///
/// Ce constructeur appelle le constructeur de base, cree un noeud de base afin de voir un affichage
///
/// @param[in] GestionnaireEtatAbstrait* : contexte.
///
/// @return Aucune (constructeur).
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutPortail::SourisEtatAjoutPortail(): portailConcerne_(NULL)
{
	genererNoeud();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutPortail::~SourisEtatAjoutMuret(void)
///
/// Ce destructeur supprime le noeud en cours d'ajout
///
/// @param[in] void
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutPortail::~SourisEtatAjoutPortail(void)
{
	if(portailConcerne_ != NULL)
	{
		FacadeModele::obtenirInstance()->obtenirTerrain()->effacerNoeudTemp(portailConcerne_);
		delete portailConcerne_;
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutPortail::genererNoeud(std::string objet, std::string parent)
///
/// Crée un nouveau noeud et l'ajoute au parent
///
/// @param[in]	objet	: Nom représentant l'objet à ajouter
/// 			parent	: parent auquel ajouter le nouveau noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutPortail::genererNoeud()
{
	portailConcerne_ =  FacadeModele::obtenirInstance()->obtenirTerrain()->obtenirArbreRendu()->creerNoeud(ArbreRenduINF2990::NOM_PORTAIL);
	if(portailConcerne_ != NULL)
		FacadeModele::obtenirInstance()->obtenirTerrain()->ajouterNoeudTemp(portailConcerne_);	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutPortail::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est enfoncé
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutPortail::sourisEnfoncee( EvenementSouris& evenementSouris )
{
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutPortail::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est relaché
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutPortail::sourisRelachee( EvenementSouris& evenementSouris )
{
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		if(portailConcerne_ != 0)
		{
			if(FacadeModele::obtenirInstance()->validerPositionNoeud(portailConcerne_))
			{
				FacadeModele::obtenirInstance()->obtenirTerrain()->transfererNoeud(portailConcerne_);
				genererNoeud();
				// La generation d'un noeud peut entrainer le noeud a etre invalide si on ne peut le creer
				if(portailConcerne_ != 0)
				{
					Vecteur2i position=evenementSouris.obtenirPosition();
					Vecteur3 positionVirtuelle;
					FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
					portailConcerne_->assignerPositionRelative(positionVirtuelle);
				}
			}
		}

	}

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutPortail::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est déplacée
///
/// @param[in]	evenementSouris	: événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutPortail::sourisDeplacee( EvenementSouris& evenementSouris )
{
	if(portailConcerne_!=0)
	{
		// Pour s'assurer que le noeud est dans le bon arbre pour les cas ou le terrain serait reinitialiser entre temps
		FacadeModele::obtenirInstance()->obtenirTerrain()->ajouterNoeudTemp(portailConcerne_);
		Vecteur2i position=evenementSouris.obtenirPosition();

		Vecteur3 positionVirtuelle;
		FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
		portailConcerne_->assignerPositionRelative(positionVirtuelle);
		FacadeModele::obtenirInstance()->validerPositionNoeud(portailConcerne_, true);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void SourisEtatAjoutPortail::obtenirNomEtatSouris()
///
/// Retourne le nom associé avec l'état de la souris
///
/// @param[in]	void
///
/// @return (int )NomEtatSouris	: nom correspondant à l'état
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatAjoutPortail::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_AJOUTER_PORTAIL;
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


