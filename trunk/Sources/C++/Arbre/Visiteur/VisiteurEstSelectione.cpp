//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurEstSelectione.cpp
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisiteurEstSelectione.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "FacadeModele.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurEstSelectione::VisiteurEstSelectione(  )
///
/// Constructeur par defaut
///
/// 
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurEstSelectione::VisiteurEstSelectione( )
{
	listeNoeuds_ = new ConteneurNoeuds();
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurEstSelectione::~VisiteurEstSelectione()
///
/// Destructeur vide.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurEstSelectione::~VisiteurEstSelectione(void)
{
	delete listeNoeuds_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurEstSelectione::visiterNoeudAbstrait()
///
/// Visitation d'un noeud Abstrait
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
	if(noeud->estSelectionnable() && noeud->estSelectionne())
	{
		listeNoeuds_->push_back(noeud);
	}
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEstSelectione::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visitation d'un noeud composite.
///
/// @param[in] NoeudComposite * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudComposite( NoeudComposite* noeud )
{
	for (unsigned int i=0; i<noeud->obtenirNombreEnfants(); i++)
	{
		noeud->chercher(i)->acceptVisitor(*this);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEstSelectione::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Visitation d'un noeud muret.
///
/// @param[in] NodeWallAbstract * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudMuret( NodeWallAbstract* noeud )
{
	visiterNoeudAbstrait(noeud);
    visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEstSelectione::visiterNoeudBut( NoeudBut* noeud )
///
/// Écriture d'un noeud représentant un but.
///
/// @param[in] NoeudBut * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudBut( NoeudBut* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEstSelectione::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Écriture d'un noeud représentant un maillet.
///
/// @param[in] NoeudMaillet * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	visiterNoeudAbstrait(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEstSelectione::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Écriture d'un noeud représentant un portail.
///
/// @param[in] NoeudPortail * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudPortail( NoeudPortail* noeud )
{
	visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEstSelectione::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Écriture d'un noeud représentant une rondelle.
///
/// @param[in] NoeudRondelle * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudRondelle( NoeudRondelle* noeud )
{
	visiterNoeudAbstrait(noeud);

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEstSelectione::visiterNoeudTable( NoeudTable* noeud )
///
/// Écriture d'un noeud représentant une table.
///
/// @param[in] NoeudTable * noeud : noeud è vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudTable( NoeudTable* noeud )
{
	visiterNoeudComposite(noeud);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEstSelectione::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Visiteur de selection pour le noeudPoint
///
/// @param[in] NoeudPoint * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudPoint( NoeudPoint* noeud )
{
	visiterNoeudAbstrait(noeud);
	visiterNoeudComposite(noeud);

}




////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEstSelectione::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Visiteur de selection pour le noeudAccelerateur
///
/// @param[in] NoeudAccelerateur * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEstSelectione::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
	visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn ConteneurNoeuds* VisiteurEstSelectione::obtenirListeNoeuds() const
///
/// /*Description*/
///
///
/// @return ConteneurNoeuds*
///
////////////////////////////////////////////////////////////////////////
ConteneurNoeuds* VisiteurEstSelectione::obtenirListeNoeuds() const
{
	FacadeModele::getInstance()->modifierNbNoeudSelect((unsigned int)listeNoeuds_->size());
	return listeNoeuds_;
}







///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


