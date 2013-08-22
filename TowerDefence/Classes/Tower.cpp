//
//  Tower.cpp
//  TowerDefence
//
//  Created by Kirill Muzykov on 8/21/13.
//
//

#include "Tower.h"
#include "GameLayer.h"
#include "Enemy.h"

void Tower::setTheGame(GameLayer *game)
{
    theGame = game;
}

bool Tower::initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated)
{
    if (!CCSprite::initWithTexture(pTexture, rect, rotated))
    {
        return false;
    }
    
    attackRange = 70;
    damage = 10;
    fireRate = 1.0f;
    
    this->scheduleUpdate();
    
    return true;
}

void Tower::attackEnemy()
{
    this->schedule(schedule_selector(Tower::shootWeapon), fireRate);
}

void Tower::chosenEnemyForAttack(Enemy *enemy)
{
    chosenEnemy = enemy;
    this->attackEnemy();
    enemy->getAttacked(this);
}

void Tower::shootWeapon()
{
    CCAssert(theGame != NULL, "You should set the game for Tower");
    
    CCSprite* bullet = CCSprite::create("bullet.png");
    bullet->setPosition(this->getPosition());
    theGame->addChild(bullet);
    
    //Bullet actions: Move from turret to enemy, then make some damage to enemy, and finally remove bullet sprite.
    CCMoveTo* move = CCMoveTo::create(0.1f, chosenEnemy->getPosition());
    CCCallFunc* damage = CCCallFunc::create(this, callfunc_selector(Tower::damageEnemy));
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(Tower::removeBullet));        
    CCSequence* actions = CCSequence::create(move, damage, remove, NULL);
    
    bullet->runAction(actions);    
}

void Tower::damageEnemy()
{
    if (chosenEnemy)
        chosenEnemy->getDamaged(damage);
}

void Tower::targetKilled()
{
    chosenEnemy = NULL;
    this->unschedule(schedule_selector(Tower::shootWeapon));
}

void Tower::lostSightOfEnemy()
{
    if (chosenEnemy)
    {
        chosenEnemy->gotLostSight(this);
        chosenEnemy = NULL;
    }
    
    this->unschedule(schedule_selector(Tower::shootWeapon));    
}

void Tower::removeBullet(CCSprite* bullet)
{
    bullet->removeFromParentAndCleanup(true);
}

void Tower::update(float delta)
{
    CCAssert(theGame != NULL, "You should set the game for Tower");
    
    if (chosenEnemy)
    {
        //Finding normalized vector enemy -> tower to calculate angle we should turn tower,
        CCPoint enemyPos = chosenEnemy->getPosition();
        CCPoint myPos = this->getPosition();
        CCPoint normalized = ccpNormalize(ccpSub(enemyPos, myPos));
        
        //Turning tower to face the enemy.
        float angle = CC_RADIANS_TO_DEGREES(atan2f(normalized.y, -normalized.x)) + 90;
        this->setRotation(angle);
        
        //Checking if enemy is still in attack range.
        if (!theGame->checkCirclesCollide(this->getPosition(), attackRange, chosenEnemy->getPosition(), 1.0f))
        {
            this->lostSightOfEnemy();
        }
    }
    else
    {
        //Currently we're not attaching anyone. Let's look if there are any enemies in our attack range.
        CCObject* obj;
        CCARRAY_FOREACH(&theGame->getEnemies(), obj)
        {
            Enemy* enemy = (Enemy*)obj;            
            if (theGame->checkCirclesCollide(this->getPosition(), attackRange, enemy->getPosition(), 1.0f))
            {
                //Found enemy in range. Attacking.
                this->chosenEnemyForAttack(enemy);
                break;
            }
        }
    }
}

void Tower::draw()
{
    //Uncomment to draw attack range circle. For debug purpose only.
    //    CCPoint pos = CCNode::convertToNodeSpace(this->getPosition());
    //    ccDrawColor4B(255, 255, 255, 255);
    //    ccDrawCircle(pos, attackRange, 360, 30, true);

    CCSprite::draw();
}
