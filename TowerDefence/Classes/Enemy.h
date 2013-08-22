//
//  Enemy.h
//  TowerDefence
//
//  Created by Kirill Muzykov on 8/22/13.
//
//

#ifndef __TowerDefence__Enemy__
#define __TowerDefence__Enemy__

#include "cocos2d.h"

using namespace cocos2d;

class GameLayer;
class Waypoint;
class Tower;

class Enemy : public CCSprite
{
private:
    int maxHP;
    int currentHP;
    bool active;
    float walkingSpeed;
    
    GameLayer* theGame;
    Waypoint* destinationWaypoint;
    
    CCArray attackedBy;

public:
    void doActivate();
    void getRemoved();
    
    void getAttacked(Tower* attacker);
    void gotLostSight(Tower* attacker);
    void getDamaged(int damage);
    
    void setTheGame(GameLayer* game);
    void setDestinationWaypoint(Waypoint* wp);
    
    bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated);
    virtual void draw(void);
    virtual void update(float delta);
    
    static Enemy* create(const char *pszFileName)
    {
        Enemy *pobSprite = new Enemy();
        if (pobSprite && pobSprite->initWithFile(pszFileName))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    };
};

#endif /* defined(__TowerDefence__Enemy__) */
