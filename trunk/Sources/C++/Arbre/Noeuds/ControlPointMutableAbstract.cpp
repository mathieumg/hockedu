///////////////////////////////////////////////////////////////////////////
/// @file ControlPointMutableAbstract.cpp
/// @author Michael Ferris
/// @date 2013-02-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "ControlPointMutableAbstract.h"
#include "NodeControlPoint.h"
#include <algorithm>


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
        std::for_each(mPoints.begin(),mPoints.end(),[&](NodeControlPoint* associatedPoint)
        {
            point->addAssociatedPoint(associatedPoint);
            associatedPoint->addAssociatedPoint(point);
        });
        mPoints.push_back(point);
        return true;
    }
    return false;
}

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
