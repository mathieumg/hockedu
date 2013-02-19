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
#include "..\..\Interface\DebugRenderBox2D.h"
#include "..\..\Application\FacadeModele.h"
#include "..\..\..\..\Commun\Utilitaire\Utilitaire.h"

#ifdef MIKE_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif

CreateListDelegateImplementation(ControlPoint)
{
    GLuint liste = NULL;
    liste = glGenLists(1);
    glNewList(liste, GL_COMPILE);
#if BOX2D_INTEGRATED
        DebugRenderBox2D* debugRender = FacadeModele::getInstance()->getDebugRenderBox2D();
        debugRender->DrawSolidCircle(b2Vec2(0,0),0.5,b2Vec2(0,0),b2Color(1,0,1));
#endif
    glEndList();

    return liste;
}


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
NodeControlPoint::NodeControlPoint( const std::string& typeNoeud ):
Super(typeNoeud),mCanBeVisited(true)
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

#if BOX2D_INTEGRATED  
//     if(estAffiche())
//     {
//         DebugRenderBox2D* debugRender = FacadeModele::getInstance()->getDebugRenderBox2D();
//         b2Vec2 v;
//         utilitaire::VEC3_TO_B2VEC(getPosition(),v);
//         debugRender->DrawSolidCircle(v,0.5,b2Vec2(0,0),b2Color(1,0,1));
//     }
#endif

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
    Super::animer(temps);
//     mAngle = (float)((int)(mAngle+temps*500.0f)%360);
//     updateMatrice();
// 
//     glPushMatrix();
//     glLoadMatrixd(matrice_); // Chargement de la matrice du noeud
//     glRotated(90, 1.0, 0.0, 0.0);
//     glGetDoublev(GL_MODELVIEW_MATRIX, matrice_); // Savegarde de la matrice courante dans le noeud
//     glPopMatrix(); // Recuperation de la matrice d'origine
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

// permet d'empecher d'appeler assignerPositionAbsolue a partir de setPosition
// lorsque setPosition est appelée par assignerPositionAbsolue
bool PositionScopedBool = false;

////////////////////////////////////////////////////////////////////////
///
/// @fn void NodeControlPoint::setPosition( const Vecteur3& positionRelative )
///
/// /*Description*/
///
/// @param[in] const Vecteur3 & positionRelative
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NodeControlPoint::setPosition( const Vecteur3& positionRelative )
{
    Super::setPosition(positionRelative);
    signalObservers();
}


#ifdef MIKE_DEBUG
PRAGMA_ENABLE_OPTIMIZATION
#endif

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
