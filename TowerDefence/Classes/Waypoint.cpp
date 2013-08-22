//
//  Waypoint.cpp
//  TowerDefence
//
//  Created by Kirill Muzykov on 8/22/13.
//
//

#include "Waypoint.h"

bool Waypoint::init()
{
    if (!CCNode::init())
    {
        return false;
    }
    
    return true;
}

void Waypoint::draw()
{
    //Uncomment to draw waypoint circles and lines between them. For debug purpose only.
    //    ccDrawColor4B(0, 255, 2, 255);
    //    CCPoint localCenter = CCNode::convertToNodeSpace(this->getPosition());
    //    ccDrawCircle(localCenter, 6, 360, 30, false);
    //    ccDrawCircle(localCenter, 2, 360, 30, false);    
    //    if (nextWaypoint)
    //    {
    //        CCPoint nextWpPos = CCNode::convertToNodeSpace(nextWaypoint->getPosition());
    //        ccDrawLine(localCenter, nextWpPos);
    //    }
    
    CCNode::draw();
}
