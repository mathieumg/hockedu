///////////////////////////////////////////////////////////////////////////
/// @file ControlPointMutableAbstract.cpp
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////

#include "ControlPointMutableAbstract.h"
#include "NodeControlPoint.h"
#include <algorithm>


////////////////////////////////////////////////////////////////////////
///
/// @fn  ControlPointMutableAbstract::ControlPointMutableAbstract( unsigned int limit )
///
/// /*Description*/
///
/// @param[in] unsigned int limit
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ControlPointMutableAbstract::ControlPointMutableAbstract( unsigned int limit ):
    mMaxControlPoints(limit)
{

}

////////////////////////////////////////////////////////////////////////
///
/// @fn  ControlPointMutableAbstract::~ControlPointMutableAbstract()
///
/// /*Description*/
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ControlPointMutableAbstract::~ControlPointMutableAbstract()
{

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ControlPointMutableAbstract::addControlPoint( NodeControlPoint* point )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * point
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
bool ControlPointMutableAbstract::addControlPoint( NodeControlPoint* point )
{
    if(mPoints.size() < getMaxControlPoints())
    {
        point->setLinkedObject(this);
        // ajout tous les points dejà existant comme point associé
        for(int i=0; i<(int)mPoints.size();++i)
        {
            NodeControlPoint* associatedPoint = mPoints[i];
        
            point->addAssociatedPoint(associatedPoint);
            associatedPoint->addAssociatedPoint(point);
        }
        mPoints.push_back(point);

        // doing call after so the node can use the new info to update itself
        if(onAddControlPoint(point))
        {
            return true;
        }
        
        // something went wrong, remove it
        removeControlPointInner(point);
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ControlPointMutableAbstract::removeControlPoint( NodeControlPoint* point )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * point
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ControlPointMutableAbstract::removeControlPoint( NodeControlPoint* point )
{
    removeControlPointInner(point);
    // doing call after so the node can use the new info to update itself
    onRemoveControlPoint(point);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ControlPointMutableAbstract::removeControlPointInner( NodeControlPoint* point )
///
/// /*Description*/
///
/// @param[in] NodeControlPoint * point
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ControlPointMutableAbstract::removeControlPointInner( NodeControlPoint* point )
{
    auto it = find(mPoints.begin(),mPoints.end(), point);
    if(it != mPoints.end())
    {
        mPoints.erase(it);
    }

    for(int i=0; i<(int)mPoints.size();++i)
    {
        NodeControlPoint* associatedPoint = mPoints[i];
        
        point->removeAssociatedPoint(associatedPoint);
        associatedPoint->removeAssociatedPoint(point);
    }
    
    point->setLinkedObject(NULL);
}
