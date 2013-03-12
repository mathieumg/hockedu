//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurSelectionOpenGL.cpp
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
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
    visiterNoeudAffichable(noeud);
    for (unsigned int i=0; i<noeud->obtenirNombreEnfants(); i++)
	{
		noeud->chercher(i)->acceptVisitor(*this);
	}
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
    GLuint id = noeud->obtenirGlId();
	if(noeudsAselectioner_->find(id) != noeudsAselectioner_->end())
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


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


