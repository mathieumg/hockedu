//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurSuppression.h
/// @author Michael Ferris
/// @date 2012-02-11
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "VisiteurSuppression.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"

////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurSuppression::VisiteurSuppression( void )
///
/// Constructeur vide
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurSuppression::VisiteurSuppression(void)
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurSuppression::~VisiteurSuppression( void )
///
/// Destructeur vide
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurSuppression::~VisiteurSuppression(void)
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// Visitation d'un noeud Abstrait
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
	effacerSiSelectionne(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurSuppression::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visitation d'un noeud Composite
///
/// @param[in] NoeudComposite * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudComposite( NoeudComposite* noeud )
{
	if(!effacerSiSelectionne(noeud))
		visiterEnfants(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Visitation d'un noeud Muret
///
/// @param[in] NodeWallAbstract * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudMuret( NodeWallAbstract* noeud )
{
	if(!effacerSiSelectionne(noeud))
		visiterEnfants(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterNoeudBut( NoeudBut* noeud )
///
/// Visitation d'un noeud But
///
/// @param[in] NoeudBut * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudBut( NoeudBut* noeud )
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Visitation d'un noeud Maillet
///
/// @param[in] NoeudMaillet * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	effacerSiSelectionne(noeud);	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Visitation d'un noeud Portail
///
/// @param[in] NoeudPortail * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudPortail( NoeudPortail* noeud )
{
	effacerSiSelectionne(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Visitation d'un noeud Rondelle
///
/// @param[in] NoeudRondelle * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudRondelle( NoeudRondelle* noeud )
{
	effacerSiSelectionne(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterNoeudTable( NoeudTable* noeud )
///
/// Visitation d'un noeud Table
///
/// @param[in] NoeudTable * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudTable( NoeudTable* noeud )
{
	visiterEnfants(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Visitation d'un noeud Point
///
/// @param[in] NoeudPoint * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudPoint( NoeudPoint* noeud )
{
	visiterEnfants(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Visitation d'un noeud Accelerateur
///
/// @param[in] NoeudAccelerateur * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
	effacerSiSelectionne(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool VisiteurSuppression::effacerSiSelectionne( NoeudAbstrait* noeud )
///
/// Algorithme de suppression des noeuds
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool VisiteurSuppression::effacerSiSelectionne( NoeudAbstrait* noeud )
{
	if(noeud->estSelectionne())
	{
		// On enleve les noeud selectionné et tous ces enfants.
		// Si on ne veut pas enlever les enfants il faudrait modifier
		// la méthode effacer() pour que les enfants soit relié au parent
		if(noeud->obtenirParent() != 0)
			noeud->obtenirParent()->effacer(noeud);
		else
		{
			noeud->vider();
			delete noeud;
			noeud = 0;
		}
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSuppression::visiterEnfants( NoeudComposite* noeud )
///
/// Algorithme de visitation des enfants
///
/// @param[in] NoeudComposite * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSuppression::visiterEnfants( NoeudComposite* noeud )
{
	unsigned int nbrEnfant = noeud->obtenirNombreEnfants();
	unsigned int i=0;
	for (; i<noeud->obtenirNombreEnfants(); i++)
	{
		if(nbrEnfant > noeud->obtenirNombreEnfants())
		{
			// il y a eu un enfant de supprimé au dernier passage
			i--;
			nbrEnfant = noeud->obtenirNombreEnfants();
		}
		noeud->chercher(i)->acceptVisitor(*this);
	}

	// Il faut faire la vérification une fois sorti de la boucle, car le test de la boucle for peut faire sortir
	// Pensant qu'il a fait tous les enfants, mais si l'avant dernier fut supprimer, le dernier serait ignoré.
	if(nbrEnfant > noeud->obtenirNombreEnfants())
	{
		// il y a eu un enfant de supprimé au dernier passage
		if(--i<noeud->obtenirNombreEnfants())
			noeud->chercher(i)->acceptVisitor(*this);
	}
}
