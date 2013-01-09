//////////////////////////////////////////////////////////////////////////////
/// @file SourisEtatAjoutAccelerateur.cpp
/// @author Samuel Ledoux
/// @date 2012-02-10
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "SourisEtatAjoutAccelerateur.h"
#include "FacadeModele.h"
#include "ArbreRenduINF2990.h"
#include "ArbreNoeudLibre.h"
#include "NoeudAccelerateur.h"
#include "NoeudTable.h"
#include "Terrain.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  SourisEtatAjoutAccelerateur::SourisEtatAjoutAccelerateur( ArbreRenduINF2990* arbreRenduFinal, ArbreNoeudLibre* arbreLibre ) :SourisEtatAbstrait(contexte),arbreRenduFinal_(arbreRenduFinal), arbreLibre_(arbreLibre)
///
/// Ce constructeur appelle le constructeur de base, cree un noeud de base afin de voir un affichage 
///
/// @param[in] GestionnaireEtatAbstrait * contexte : Gestionnaire contenant cet état
/// @param[in] ArbreRenduINF2990 * arbreRenduFinal
/// @param[in] ArbreNoeudLibre * arbreLibre
///
/// @return rien (constructeur)
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutAccelerateur::SourisEtatAjoutAccelerateur( )
	: accelerateurConcerne_(0)
{
	genererNoeud();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutAccelerateur::~SourisEtatAjoutAccelerateur( void ) 
///
/// Desctructeur de la classe qui supprime le noeud en cours d'ajout
///
/// @param[in] void
///
/// @return rien (destructeur)
///
////////////////////////////////////////////////////////////////////////
SourisEtatAjoutAccelerateur::~SourisEtatAjoutAccelerateur( void )
{
	if(accelerateurConcerne_ != 0)
	{
		FacadeModele::obtenirInstance()->obtenirTerrain()->effacerNoeudTemp(accelerateurConcerne_);
		delete accelerateurConcerne_;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutAccelerateur::genererNoeud( )
///
/// Crée un nouveau noeud et l'ajoute au parent
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutAccelerateur::genererNoeud()
{
	accelerateurConcerne_= FacadeModele::obtenirInstance()->obtenirTerrain()->obtenirArbreRendu()->creerNoeud(ArbreRenduINF2990::NOM_ACCELERATEUR);
	FacadeModele::obtenirInstance()->obtenirTerrain()->ajouterNoeudTemp(accelerateurConcerne_);
	//arbreLibre_->ajouter(accelerateurConcerne_);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutAccelerateur::sourisEnfoncee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est enfoncé
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutAccelerateur::sourisEnfoncee( EvenementSouris& evenementSouris )
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutAccelerateur::sourisRelachee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'un bouton de la souris est relaché
///
/// @param[in] EvenementSouris & evenementSouris
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutAccelerateur::sourisRelachee( EvenementSouris& evenementSouris )
{
	if(evenementSouris.obtenirBouton()==BOUTON_SOURIS_GAUCHE)
	{
		if(FacadeModele::obtenirInstance()->validerPositionNoeud(accelerateurConcerne_))
		{
			FacadeModele::obtenirInstance()->obtenirTerrain()->transfererNoeud(accelerateurConcerne_);
			genererNoeud();
			Vecteur2i position=evenementSouris.obtenirPosition();

			Vecteur3 positionVirtuelle;
			FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
			accelerateurConcerne_->assignerPositionRelative(positionVirtuelle);
		}

	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutAccelerateur::sourisDeplacee( EvenementSouris& evenementSouris )
///
/// Action à effectuer lorsqu'on déplace la souris avec bouton enfoncé
///
/// @param[in] EvenementSouris & evenementSouris : événement de la souris correspondant
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void SourisEtatAjoutAccelerateur::sourisDeplacee( EvenementSouris& evenementSouris )
{
	// Pour s'assurer que le noeud est dans le bon arbre pour les cas ou le terrain serait reinitialiser entre temps
	FacadeModele::obtenirInstance()->obtenirTerrain()->ajouterNoeudTemp(accelerateurConcerne_);
	Vecteur2i position=evenementSouris.obtenirPosition();

	Vecteur3 positionVirtuelle;
	FacadeModele::obtenirInstance()->convertirClotureAVirtuelle(position[VX],position[VY],positionVirtuelle);
	accelerateurConcerne_->assignerPositionRelative(positionVirtuelle);
	FacadeModele::obtenirInstance()->validerPositionNoeud(accelerateurConcerne_, true);

	
}
////////////////////////////////////////////////////////////////////////
///
/// @fn SourisEtatAjoutAccelerateur::obtenirNomEtatSouris()
///
/// Retourne le nom associé avec l'état de la souris
///
///
/// @return NomEtatSouris
///
////////////////////////////////////////////////////////////////////////
NomEtatSouris SourisEtatAjoutAccelerateur::obtenirNomEtatSouris()
{
	return ETAT_SOURIS_AJOUTER_ACCELERATEUR;
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
