///////////////////////////////////////////////////////////////////////////////
/// @file NodeWallEdition.cpp
/// @author Michael Ferris
/// @date 2012-02-25
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////////

#include "NodeWallEdition.h"
#include "Utilitaire.h"
#include "NodeControlPoint.h"

#ifdef MIKE_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeWallEdition::NodeWallEdition( NoeudPoint* n1, NoeudPoint* n2 )
///
/// Constructeur par paramètre.
///
/// @param[in] NoeudPoint * n1 : premier point associé
/// @param[in] NoeudPoint * n2 : deuxieme point associé
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
NodeWallEdition::NodeWallEdition( const std::string& type ):
    Super(type),ControlPointMutableAbstract(2)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeWallEdition::~NodeWallEdition()
///
/// destructor
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NodeWallEdition::~NodeWallEdition()
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::ajouter( NoeudAbstrait* enfant )
///
/// Ajoute un noeud enfant.
///
/// @param[in] NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeWallEdition::ajouter( NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = dynamic_cast<NodeControlPoint*>(enfant);
    if(controlPoint)
    {
        return addControlPoint(controlPoint);
    }
    return Super::ajouter(enfant);
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::detacherEnfant( const NoeudAbstrait* enfant )
///
/// /*Description*/
///
/// @param[in] const NoeudAbstrait * enfant
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::detacherEnfant( const NoeudAbstrait* enfant )
{
    NodeControlPoint* controlPoint = const_cast<NodeControlPoint*>(dynamic_cast<const NodeControlPoint*>(enfant));
    if(controlPoint)
    {
        removeControlPoint(controlPoint);
    }
    else
    {
        Super::detacherEnfant(enfant);
    }
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::modifierEchelle( float facteur )
///
/// /*Description*/
///
/// @param[in] float facteur
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::modifierEchelle( float facteur )
{
    Vecteur3 echelle;
    obtenirEchelleCourante(echelle);
    echelle[VY] *= facteur;
    modifierEchelleCourante(echelle);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::afficherConcret()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::afficherConcret() const
{
    const Vecteur3& corner1 = obtenirCoin1(), corner2 = obtenirCoin2();
    Vecteur2 vecteurEntre(corner1-corner2);
    vecteurEntre = vecteurEntre.tournerMoinsPiSur2();
    vecteurEntre.normaliser();
    float halfHeight = echelleCourante_[VY]*DEFAULT_SIZE[VY]/2.f;
    vecteurEntre *= halfHeight;
    glPushMatrix();
    glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT);
    // Assignation du mode d'affichage des polygones
    glPolygonMode( GL_FRONT_AND_BACK, modePolygones_ );
    glTranslatef(vecteurEntre[VX],vecteurEntre[VY],0);
    NoeudAbstrait::afficherConcret();

    // Restauration
    glPopAttrib();
    glPopMatrix();

    DrawChild();
//     vecteurEntre.tourner(90,Vecteur3(0,0,1));
//     vecteurEntre.
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool NodeWallEdition::onAddControlPoint( NodeControlPoint* point )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * point
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
bool NodeWallEdition::onAddControlPoint( NodeControlPoint* point )
{
    if(Super::ajouter(point))
    {
        point->attach(this);
        coins_[getNBControlPoint()-1] = &point->getPosition();
        updateWallProperties();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeWallEdition::onRemoveControlPoint( NodeControlPoint* point )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * point
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeWallEdition::onRemoveControlPoint( NodeControlPoint* point )
{
    Super::detacherEnfant(point);
    point->detach(this);
    coins_[getNBControlPoint()] = NULL;
    updateWallProperties();
}


#ifdef MIKE_BUILD
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////