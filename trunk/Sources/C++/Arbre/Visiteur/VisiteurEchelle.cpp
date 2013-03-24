//////////////////////////////////////////////////////////////////////////////
/// @file VisiteurEchelle.cpp
/// @author Mathieu Parent
/// @date 2012-02-06
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////



#include "VisiteurEchelle.h"
#include "NoeudMuret.h"
#include "NoeudBut.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "NoeudRondelle.h"
#include "NoeudTable.h"
#include "NoeudPoint.h"
#include "NoeudAccelerateur.h"
#include "NodeControlPoint.h"
#include "ControlPointMutableAbstract.h"


const float VisiteurEchelle::FACTEUR_ECHELLE = 1.005f;
const float VisiteurEchelle::FACTEUR_ECHELLE_MAX = 5;
const float VisiteurEchelle::FACTEUR_ECHELLE_MIN = 0.20f;



////////////////////////////////////////////////////////////////////////
///
/// @fn  VisiteurEchelle::VisiteurEchelle( float facteur, int axe /* = -1 */ )
///
/// Constructeur par paramètre
///
/// @param[in]	facteur			: facteur d'echelle a appliquer
///				axe				: axe par rapport auquel appliquer la rotation (tous pas defaut)
///				ignoreSelection	: ignore la selection et effectue l'echelle quand meme
///
/// @return Aucun
///
////////////////////////////////////////////////////////////////////////
VisiteurEchelle::VisiteurEchelle( float facteur, int axe /* = -1 */ )
{
	facteur_ = facteur;
	axe_ = axe;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn VisiteurEchelle::~VisiteurEchelle()
///
/// Destructeur vide.
///
/// @param[in] void
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
VisiteurEchelle::~VisiteurEchelle(void)
{
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudAbstrait( NoeudAbstrait* noeud )
///
/// Algortihme de base pour la mise à l'échelle
///
/// @param[in] NoeudAbstrait * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudAbstrait( NoeudAbstrait* noeud )
{
	Vecteur3 echelleCourante;
	noeud->getScale(echelleCourante);
	
    for(int i=0; i<3; ++i)
    {
        const float newVal = echelleCourante[i]*facteur_;
        if(axe_ & 1<<i  &&
            newVal <= FACTEUR_ECHELLE_MAX &&
            newVal >= FACTEUR_ECHELLE_MIN 
            )
        {
            echelleCourante[i] = newVal;
        }
    }

	noeud->setScale(echelleCourante);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudComposite( NoeudComposite* noeud )
///
/// Visitation des enfants d'un noeud composite
///
/// @param[in] NoeudComposite * n : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudComposite( NoeudComposite* noeud )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudMuret( NodeWallAbstract* noeud )
///
/// Echelle d'un noeud représentant un muret.
///
/// @param[in] NodeWallAbstract * n : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudMuret( NodeWallAbstract* noeud )
{
	int oldAxe = axe_;
	// Push
	axe_ = 1<<VY;
	visiterNoeudAbstrait(noeud);
	// pop
	axe_ = oldAxe;
    visiterNoeudComposite(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudBut( NoeudBut* noeud )
///
/// Echelle d'un noeud représentant un but.
///
/// @param[in] NoeudBut * n : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudBut( NoeudBut* noeud )
{
	noeud->updateLongueur(facteur_);
	// On donne au but adverse la même échelle
	noeud->obtenirButAdverse()->updateLongueur(facteur_);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudMaillet( NoeudMaillet* noeud ) 
///
/// Echelle d'un noeud représentant un maillet.
///
/// @param[in] NoeudMaillet * n : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudMaillet( NoeudMaillet* noeud )
{
	visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudPortail( NoeudPortail* noeud )
///
/// Echelle d'un noeud représentant un portail.
///
/// @param[in] NoeudPortail * n : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudPortail( NoeudPortail* noeud )
{
    int oldAxe = axe_;
    // Push
    axe_ = 1<<VX | 1<<VY;
    visiterNoeudAbstrait(noeud);
    // pop
    axe_ = oldAxe;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudRondelle( NoeudRondelle* noeud )
///
/// Echelle d'un noeud représentant une rondelle.
///
/// @param[in] NoeudRondelle * n : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudRondelle( NoeudRondelle* noeud )
{
	visiterNoeudAbstrait(noeud);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudTable( NoeudTable* noeud )
///
/// Echelle d'un noeud représentant une table.
///
/// @param[in] NoeudTable * n : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudTable( NoeudTable* noeud )
{
	// Pas d'echelle permise
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudPoint( NoeudPoint* noeud )
///
/// Echelle d'un noeud du point de la table
///
/// @param[in] NoeudTable * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudPoint( NoeudPoint* noeud )
{
	// Pas d'echelle permise

    /// mais ses enfants sont interesser a etre modifier
    for (unsigned int i=0; i<noeud->childCount(); i++)
    {
        noeud->find(i)->acceptVisitor(*this);
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
///
/// Echelle d'un noeud du point de la table
///
/// @param[in] NoeudAccelerateur * noeud : noeud à vérifier
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNoeudAccelerateur( NoeudAccelerateur* noeud )
{
    int oldAxe = axe_;
    // Push
    axe_ = 1<<VX | 1<<VY;
    visiterNoeudAbstrait(noeud);
    // pop
    axe_ = oldAxe;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void VisiteurEchelle::visiterNodeControlPoint( NodeControlPoint* noeud )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * noeud
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void VisiteurEchelle::visiterNodeControlPoint( NodeControlPoint* noeud )
{
    NoeudAbstrait* obj = dynamic_cast<NoeudAbstrait*>(noeud->getLinkedObject());
    if(obj)
    {
        obj->acceptVisitor(*this);
    }
}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


