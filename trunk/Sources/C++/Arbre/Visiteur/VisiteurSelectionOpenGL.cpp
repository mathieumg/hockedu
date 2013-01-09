//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurSelectionOpenGL.cpp
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "VisiteurSelectionOpenGL.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "FacadeModele.h"
#include "NoeudAccelerateur.h"


////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurSelectionOpenGL::VisiteurSelectionOpenGL( Vecteur2 positionMin, Vecteur2 positionMax )
///
/// Constructeur par paramètres
///
/// @param[in] const ConteneurIdNoeuds& liste : liste contenant les id des noeuds a selectionner
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurSelectionOpenGL::VisiteurSelectionOpenGL( const IdNoeuds* liste, bool ctrlOn ):
noeudsAselectioner_(liste), ctrlOn_(ctrlOn)
{
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurSelectionOpenGL::~VisiteurSelectionOpenGL()
///
/// Destructeur vide.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurSelectionOpenGL::~VisiteurSelectionOpenGL(void)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurSelectionOpenGL::visiterNoeudAbstrait()
///
/// Visitation d'un noeud Abstrait
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
	visiterNoeudAffichable(noeud);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visitation d'un noeud composite.
///
/// @param[in] NoeudComposite * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudComposite( NoeudComposite* noeud )
{
	for (unsigned int i=0; i<noeud->obtenirNombreEnfants(); i++)
	{
		noeud->chercher(i)->accueillirVisiteurNoeud(*this);
	}
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudMuret( NoeudMuret* noeud )
///
/// Visitation d'un noeud muret.
///
/// @param[in] NoeudMuret * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudMuret( NoeudMuret* noeud )
{
	visiterNoeudAffichable(noeud);
	visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudBut( NoeudBut* noeud )
///
/// Écriture d'un noeud représentant un but.
///
/// @param[in] NoeudBut * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudBut( NoeudBut* noeud )
{
	visiterNoeudAffichable(noeud);
	visiterNoeudComposite(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudMaillet( NoeudMaillet* noeud )
///
/// Écriture d'un noeud représentant un maillet.
///
/// @param[in] NoeudMaillet * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	visiterNoeudAffichable(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Écriture d'un noeud représentant un portail.
///
/// @param[in] NoeudPortail * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudPortail( NoeudPortail* noeud )
{
	visiterNoeudAffichable(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Écriture d'un noeud représentant une rondelle.
///
/// @param[in] NoeudRondelle * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudRondelle( NoeudRondelle* noeud )
{
	visiterNoeudAffichable(noeud);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudTable( NoeudTable* noeud )
///
/// Écriture d'un noeud représentant une table.
///
/// @param[in] NoeudTable * noeud : noeud è vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudTable( NoeudTable* noeud )
{
	visiterNoeudComposite(noeud);
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Visiteur de selection pour le noeudPoint
///
/// @param[in] NoeudPoint * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudPoint( NoeudPoint* noeud )
{
	visiterNoeudAffichable(noeud);
	visiterNoeudComposite(noeud);

}



////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudAffichable( NoeudAbstrait* noeud )
///
/// Algortihme de selection des noeuds
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudAffichable( NoeudAbstrait* noeud )
{
	
	// Quitte si le noeud n'est pas selectionnable
	if(!noeud->estSelectionnable())
		return;

	if(noeudsAselectioner_->find(noeud->obtenirGlId()) != noeudsAselectioner_->end())
	{
		if(!ctrlOn_)
		{
			noeud->assignerSelection(true);
		}
		else
		{
			if(noeud->estSelectionne())
			{
				noeud->assignerSelection(false);
			}
			else
			{
				noeud->assignerSelection(true);
			}
		}
		
	}
		
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurSelectionOpenGL::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Visiteur de selection pour le noeudAccelerateur
///
/// @param[in] NoeudAccelerateur * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurSelectionOpenGL::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
	visiterNoeudAffichable(noeud);
}



///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


