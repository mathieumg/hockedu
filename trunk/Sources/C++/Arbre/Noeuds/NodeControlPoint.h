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
#include "PositionSubject.h"
#include <list>

class ControlPointMutableAbstract;

///////////////////////////////////////////////////////////////////////////
/// @class NodeControlPoint
/// @brief Classe qui repr�sente des points de control pour l'�dition relative d'objet
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class NodeControlPoint : public NoeudComposite, public PositionSubject
{
public:
    typedef NoeudComposite Super;
	/// Constructeur � partir du type du noeud.
	NodeControlPoint( const std::string& typeNoeud);
	/// Destructeur.
	~NodeControlPoint();

	/// Effectue l'animation
	virtual void animer( const float& temps );
	/// Accueil un visiteur
	virtual void acceptVisitor( VisiteurNoeud& v);
	/// Assigne la position relative du noeud et alerte ses observers
    virtual void setPosition( const Vecteur3& positionRelative);

    //virtual const std::string& get3DModelKey()const { return RazerGameUtilities::NAME_TABLE_CONTROL_POINT; }

    /// hack les control point ne connaissent pas leur parent, mais les parent les connaissent
    void afficherConcret() const;

private:
	/// list of other control points linked to the same object, they must
    /// all have the same life scope
	std::list<NodeControlPoint*> mAssociatedPoints;
    ControlPointMutableAbstract* mLinkedObject;
    bool mCanBeVisited;

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
    inline void flagVisitationIfSelected() { if(estSelectionne()) mCanBeVisited = false;}
};





///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
