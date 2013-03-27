///////////////////////////////////////////////////////////////////////////
/// @file NodeControlPoint.h
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "NoeudComposite.h"
#include "PositionObserver.h"
#include "Solution_Defines.h"
#include <list>

class ControlPointMutableAbstract;

///////////////////////////////////////////////////////////////////////////
/// @class NodeControlPoint
/// @brief Classe qui représente des points de control pour l'édition relative d'objet
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class NodeControlPoint : public NoeudComposite, public PositionSubject
{
public:
    typedef NoeudComposite Super;
	/// Constructeur à partir du type du noeud.
	NodeControlPoint( const std::string& typeNoeud);
	/// Destructeur.
	virtual ~NodeControlPoint();

	/// Effectue l'animation
	virtual void tick( const float& temps );
	/// Accueil un visiteur
	virtual void acceptVisitor( VisiteurNoeud& v);
	/// Assigne la position relative du noeud et alerte ses observers
    virtual void setPosition( const Vecteur3& positionRelative);
    
    /// Creation du noeud XML d'un point
    virtual XmlElement* createXmlNode();
    /// Initialisation du NoeudPoint à partir d'un element XML
    virtual bool initFromXml(const XmlElement* element);

    void renderReal() const;
    /// Permet d'assigner les attribut nécessaire à la collision
    virtual void setCollisionVisitorAttributes(VisiteurCollision* v);

    /// Recreates the physics body according to current attributes
    virtual void updatePhysicBody();
private:
	/// list of other control points linked to the same object, they must
    /// all have the same life scope
	std::list<NodeControlPoint*> mAssociatedPoints;
    ControlPointMutableAbstract* mLinkedObject;
    bool mCanBeVisited;

    /// value to calculate animation movement in Z plane
    float mHeightAngle;

public:
    /// Accessors of mLinkedObject
    inline ControlPointMutableAbstract* getLinkedObject() const { return mLinkedObject; }
    inline void setLinkedObject(ControlPointMutableAbstract* pVal) { mLinkedObject = pVal; }

    /// Accessors of mAssociatedPoints
    inline const std::list<NodeControlPoint*>& getAssociatedPoints() const { return mAssociatedPoints; }
    inline void addAssociatedPoint(NodeControlPoint* pControlPoint)
    {
        checkf(pControlPoint != this);
        mAssociatedPoints.push_back(pControlPoint);
    }
    inline void removeAssociatedPoint(NodeControlPoint* pControlPoint)
    {
        mAssociatedPoints.remove(pControlPoint);
    }


    /// Use this to prevent a visitor from doing his work on multiple associated control points
    /// ie. when the visitation affects the linked object directly

    /// Do not use flag visitation with visitor that can be stopped midway
    void flagSelectedAssociatedPoints() const;

    inline bool canBeVisisted() const {return mCanBeVisited;}
    /// tells if this point can be visisted, and reset the flag for next time
    inline bool canBeVisitedAndRemoveFlag() 
    {
        /// returns the value of mCanBeVisited and set it to true afterward
        return mCanBeVisited++; 
    }
private:
    inline void flagVisitationIfSelected() { if(IsSelected()) mCanBeVisited = false;}
};





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
