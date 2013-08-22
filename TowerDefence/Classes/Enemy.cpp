//
//  Enemy.cpp
//  TowerDefence
//
//  Created by Kirill Muzykov on 8/22/13.
//
//

#include "Enemy.h"
#include "GameLayer.h"
#include "Waypoint.h"
#include "Tower.h"
#include "SimpleAudioEngine.h"

#define HEALTH_BAR_WIDTH    20
#define HEALTH_BAR_ORIGIN   -10
#define PLAYER_AWARD        200

void Enemy::setTheGame(GameLayer *game)
{
    theGame = game;
}

void Enemy::setDestinationWaypoint(Waypoint *wp)
{
    destinationWaypoint = wp;
}

bool Enemy::initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated)
{
    if (!CCSprite::initWithTexture(pTexture, rect, rotated))
    {
        return false;
    }
    
    maxHP = 40;
    currentHP = maxHP;
    active = false;
    walkingSpeed = 1.0f;
    
    this->scheduleUpdate();
    
    return true;
}

void Enemy::update(float delta)
{
    if (!active)
        return;
    
    CCAssert(theGame != NULL, "You have to set the game for the Enemy before you can use it");
    
    //Checking if we've reached destination waypoint.
    if (theGame->checkCirclesCollide(this->getPosition(), 1.0f, destinationWaypoint->getPosition(), 1.0f))
    {
        if (destinationWaypoint->getNextWaypoint())
        {
            //This is not the end of the way. Taking next waypoint and moving towards it.
            destinationWaypoint = destinationWaypoint->getNextWaypoint();
        }
        else
        {
            //We've reached the end of the path. Means we're reached player base. Making some damage.
            theGame->getHpDamage();
            this->getRemoved();
        }
    }
    
    //Actual moving to next waypoint.
    CCPoint targetPoint = destinationWaypoint->getPosition();
    CCPoint normalized = ccpNormalize(ccpSub(targetPoint, this->getPosition()));
    
    float movementSpeed = walkingSpeed;
    float angle = CC_RADIANS_TO_DEGREES(atan2f(normalized.y, -normalized.x));
    CCPoint desiredPosition = ccpAdd(this->getPosition(), ccpMult(normalized, movementSpeed));
    
    this->setPosition(desiredPosition);
    this->setRotation(angle);
}

void Enemy::doActivate()
{
    active = true;
}

void Enemy::getRemoved()
{
    //Notifying each tower that they should pick other target, since we're dead.
    CCObject* obj = NULL;
    CCARRAY_FOREACH(&attackedBy, obj)
    {
        Tower* tower = (Tower*)obj;
        tower->targetKilled();
    }
    
    //Notifying game layer & removing self.
    theGame->enemyGotKilled(this);
    this->removeFromParentAndCleanup(true);
}

void Enemy::getAttacked(Tower* attacker)
{
    attackedBy.addObject(attacker);
}

void Enemy::gotLostSight(Tower* attacker)
{
    attackedBy.removeObject(attacker);
}

void Enemy::getDamaged(int damage)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("laser_shoot.wav", false);
    
    currentHP -= damage;
    if (currentHP <= 0)
    {
        //We're killed. Need to reward player and remove self.
        theGame->awardGold(PLAYER_AWARD);
        this->getRemoved();
    }
}

void Enemy::draw()
{
    CCPoint localPos = CCNode::convertToNodeSpace(this->getPosition());
    
    ccDrawSolidRect(ccp(localPos.x+HEALTH_BAR_ORIGIN,
                        localPos.y+16),
                    ccp(localPos.x+HEALTH_BAR_ORIGIN+HEALTH_BAR_WIDTH,
                        localPos.y+14),
                    ccc4f(1.0, 0, 0, 1.0));
    
    ccDrawSolidRect(ccp(localPos.x+HEALTH_BAR_ORIGIN,
                        localPos.y+16),
                    ccp(localPos.x+HEALTH_BAR_ORIGIN + (float)(currentHP * HEALTH_BAR_WIDTH)/maxHP,
                        localPos.y+14),
                    ccc4f(0, 1.0, 0, 1.0));
    
    CCSprite::draw();
}

