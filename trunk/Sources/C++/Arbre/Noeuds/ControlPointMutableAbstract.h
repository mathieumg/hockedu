///////////////////////////////////////////////////////////////////////////
/// @file ControlPointMutableAbstract.h
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>

class NodeControlPoint;
///////////////////////////////////////////////////////////////////////////
/// @class ControlPointMutableAbstract
/// @brief Classe abstraite pour les noeuds qui sont manipulé par des points
///             de controles
///
/// @author Michael Ferris
/// @date 2013-02-16
///////////////////////////////////////////////////////////////////////////
class ControlPointMutableAbstract
{
public:
    /// Constructeurs par paramètres
    ControlPointMutableAbstract(unsigned int limit);
    ~ControlPointMutableAbstract();

    inline NodeControlPoint* getControlPoint(unsigned int i) const
    {
        if(i < getNBControlPoint())
        {
            return mPoints[i];
        }
        return 0;
    }
    bool isAnyPointSelected() const;

    virtual void modifierEchelle( float facteur ) = 0;
protected:
    virtual bool onAddControlPoint(NodeControlPoint* point) = 0;
    virtual void onRemoveControlPoint(NodeControlPoint* point) = 0;

private:
    /// Control points controlling the wall's corner position
    std::vector<NodeControlPoint*> mPoints;
    unsigned int mMaxControlPoints;

public:
    /// Accessors of mMaxControlPoints
    inline unsigned int getMaxControlPoints() const { return mMaxControlPoints; }
    inline void setMaxControlPoints(const unsigned int& pVal) { mMaxControlPoints = pVal; }
    inline unsigned int getNBControlPoint()const {return (unsigned int)mPoints.size();}
    bool addControlPoint(NodeControlPoint* point);
    void removeControlPoint(NodeControlPoint* point);
private:
    void removeControlPointInner( NodeControlPoint* point );
};

