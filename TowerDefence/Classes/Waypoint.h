//
//  Waypoint.h
//  TowerDefence
//
//  Created by Kirill Muzykov on 8/22/13.
//
//

#ifndef __TowerDefence__Waypoint__
#define __TowerDefence__Waypoint__

#include "cocos2d.h"

using namespace cocos2d;

/** Waypoints are required for enemies to move. Enemy moves from one waypoint to another, 
    when enemy reaches waypoint it takes nextWaypoint and moves to it.
 */
class Waypoint : public CCNode
{
private:
    Waypoint* nextWaypoint;

public:
    virtual bool init();
    virtual void draw();
    
    CREATE_FUNC(Waypoint);
    
    //nextWaypoint property
    void setNextWaypoint(Waypoint* next)  {  nextWaypoint = next;  }
    Waypoint* getNextWaypoint()           {  return nextWaypoint;  }
};

#endif /* defined(__TowerDefence__Waypoint__) */
