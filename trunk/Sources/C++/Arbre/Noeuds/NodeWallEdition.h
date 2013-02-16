///////////////////////////////////////////////////////////////////////////
/// @file NodeWallEdition.h
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include "NoeudMuretRelatif.h"

class NodeControlPoint;
///////////////////////////////////////////////////////////////////////////
/// @class NodeWallEdition
/// @brief Classe qui représente un muret relatif qui peut être manipuler
///         pour jouer sur le terrain
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class NodeWallEdition : public NoeudMuretRelatif
{
public:
    typedef NoeudMuretRelatif Super;
	/// Constructeurs par paramètres
	NodeWallEdition(const std::string& type);
    ~NodeWallEdition();

    inline NodeControlPoint* getControlPoint(int i)
    {
        if(i < NB_CONTROL_POINTS)
        {
            return mPoints[i];
        }
        return NULL;
    }

    /// Assigne le parent de ce noeud.
    virtual void assignerParent( NoeudComposite* parent );

    virtual void updateObserver( PositionSubject* pSubject );

    inline NodeControlPoint** getControlPoints() {return mPoints;}
    inline unsigned int getNBControlPoint()const {return NB_CONTROL_POINTS;}
private:
    static const unsigned int NB_CONTROL_POINTS = 2;
    /// Control points controlling the wall's corner position
    NodeControlPoint* mPoints[NB_CONTROL_POINTS];
};

