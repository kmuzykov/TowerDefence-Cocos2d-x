//
//  Tower.h
//  TowerDefence
//
//  Created by Kirill Muzykov on 8/21/13.
//
//

#ifndef __TowerDefence__Tower__
#define __TowerDefence__Tower__

#include "cocos2d.h"
using namespace cocos2d;

#define kTOWER_COST 300

class Enemy;
class GameLayer;

class Tower : public CCSprite
{
private:
    int attackRange;
    int damage;
    float fireRate;
    
    GameLayer* theGame;
    Enemy* chosenEnemy;
    
    void attackEnemy();
    void chosenEnemyForAttack(Enemy* enemy);
    void shootWeapon();
    
    void damageEnemy();
    void removeBullet(CCSprite* bullet);
    void lostSightOfEnemy();
    
public:
    void setTheGame(GameLayer* game);
    void targetKilled();
    
    bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated); //designated initializer (it will be called by create in the end)
    virtual void draw(void);
    virtual void update(float delta);
    
    //Looks like overriding static create method is the best way to subclass CCSprite.
    static Tower* create(const char *pszFileName)
    {        
        Tower *pobSprite = new Tower();
        if (pobSprite && pobSprite->initWithFile(pszFileName))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    };
};


#endif /* defined(__TowerDefence__Tower__) */
