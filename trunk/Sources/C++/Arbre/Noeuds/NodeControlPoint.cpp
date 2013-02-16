///////////////////////////////////////////////////////////////////////////
/// @file NodeControlPoint.cpp
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "NodeControlPoint.h"
#include "VisiteurNoeud.h"
#include <algorithm>

#ifdef MIKE_BUILD
PRAGMA_DISABLE_OPTIMIZATION
#endif

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeControlPoint::NodeControlPoint( const std::string& typeNoeud )
///
/// /*Description*/
///
/// @param[in] const std::string & typeNoeud
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NodeControlPoint::NodeControlPoint( const std::string& typeNoeud, NoeudAbstrait* pLinkedObject/*= NULL*/  ):
mLinkedObject(pLinkedObject),mCanBeVisited(true)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn  NodeControlPoint::~NodeControlPoint()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
NodeControlPoint::~NodeControlPoint()
{

}

void NodeControlPoint::afficherConcret() const
{
    // Appel à la version de la classe de base pour l'affichage des enfants.
    NoeudAbstrait::afficherConcret();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::animer( const float& temps )
///
/// /*Description*/
///
/// @param[in] const float & temps
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::animer( const float& temps )
{
    mAngle = (float)((int)(mAngle+temps*500.0f)%360);
    updateMatrice();

    glPushMatrix();
    glLoadMatrixd(matrice_); // Chargement de la matrice du noeud
    glRotated(90, 1.0, 0.0, 0.0);
    glGetDoublev(GL_MODELVIEW_MATRIX, matrice_); // Savegarde de la matrice courante dans le noeud
    glPopMatrix(); // Recuperation de la matrice d'origine
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::acceptVisitor( VisiteurNoeud& v )
///
/// /*Description*/
///
/// @param[in] VisiteurNoeud & v
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::acceptVisitor( VisiteurNoeud& v )
{
    v.visiterNodeControlPoint(this);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::flagSelectedAssociatedPoints()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::flagSelectedAssociatedPoints() const
{
    std::for_each(
        mAssociatedPoints.begin(),
        mAssociatedPoints.end(), 
        [](NodeControlPoint* point)
        {
            point->flagVisitationIfSelected();
        }
    );
}

// permet d'empecher d'appeler assignerPositionAbsolue a partir de assignerPositionRelative
// lorsque assignerPositionRelative est appelée par assignerPositionAbsolue
bool PositionScopedBool = false;

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::assignerPositionRelative( const Vecteur3& positionRelative )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::assignerPositionRelative( const Vecteur3& positionRelative )
{
    Super::assignerPositionRelative(positionRelative);
    //mPointPosition = getPosition();
    signalObservers();

//     if(!mLinkedObject || PositionScopedBool)
//     {
//         // assigner la position du point en premier pour que la table puisse l'utiliser à sa mise a jour
//         Super::assignerPositionRelative(positionRelative);
//     }
//     else
//     {
//         // appel independant de position absolue, recalcule la position absolue 
//         Vecteur3 pos = mLinkedObject->getPosition();
//         assignerPositionAbsolue(pos+positionRelative);
//     }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::assignerPositionAbsolue( const Vecteur3& positionAbsolue )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & positionAbsolue
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::assignerPositionAbsolue( const Vecteur3& positionAbsolue )
{
//     if(mLinkedObject)
//     {
//         PositionScopedBool = true;
//         // met a jour ses observateurs
//         mPointPosition = positionAbsolue;
// 
//         // la position relative sera mise a joue par l'objet lié
//         PositionSubject::signalObservers();
// 
//         // met a jour sa position relative en fonction de la nouvelle position de son parent
//         //Super::assignerPositionAbsolue(positionAbsolue);
//         PositionScopedBool = false;
//     }
//     else
    {
        assignerPositionRelative(positionAbsolue);
        //positionRelative_ = positionAbsolue;
//         mPointPosition = positionAbsolue;
//         Super::assignerPositionAbsolue(positionAbsolue);
    }
}







#ifdef MIKE_BUILD
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
