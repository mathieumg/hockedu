///////////////////////////////////////////////////////////////////////////
/// @file NodePolygone.h
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "ControlPointMutableAbstract.h"
#include "NoeudComposite.h"
#include "PositionObserver.h"

///////////////////////////////////////////////////////////////////////////
/// @class NodePolygone
/// @brief Classe qui représente un polygone
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class NodePolygone : public NoeudComposite, public ControlPointMutableAbstract, public PositionObserver
{
public:
    typedef NoeudComposite Super;
	/// Constructeurs par paramètres
	NodePolygone();
    virtual ~NodePolygone();
    virtual void renderReal()const;
    /// Ajoute un noeud enfant.
    virtual bool add( NoeudAbstrait* enfant );
    virtual void unlinkChild( const NoeudAbstrait* enfant );

    virtual void updateObserver( const  PositionSubject* pSubject );
    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();

    ///////////////////////////////////////////////////////////////////////////
    /// Implementation interface ControlPointMutableAbstract
    virtual void modifierEchelle( float facteur );
protected:
    virtual bool onAddControlPoint(NodeControlPoint* point);
    virtual void onRemoveControlPoint(NodeControlPoint* point);
    void divideIntoTriangles() const;
    ///////////////////////////////////////////////////////////////////////////

};

