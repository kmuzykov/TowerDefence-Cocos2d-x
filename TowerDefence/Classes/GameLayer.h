//
//  GameLayer.h
//  TowerDefence
//
//  Created by Kirill Muzykov on 8/21/13.
//
//

#ifndef __TowerDefence__GameLayer__
#define __TowerDefence__GameLayer__

#include "cocos2d.h"

using namespace cocos2d;

class Waypoint;
class Enemy;

class GameLayer : public cocos2d::CCLayer
{
private:
    int wave;
    int playerHP;
    int playerGold;
    bool gameEnded;

    CCLabelBMFont* ui_wave_lbl;
    CCLabelBMFont* ui_hp_lbl;
    CCLabelBMFont* ui_gold_lbl;

    CCArray towerBases;
    CCArray waypoints;
    CCArray enemies;
    
    void loadTowerPositions();
    bool loadWave();
    void addWaypoints();
    Waypoint* addWaypoint(CCPoint pos, Waypoint* next);

    bool canBuyTower();
    void doGameOver();
    
public:
    const CCArray& getEnemies();
    void enemyGotKilled(Enemy* enemy);
    void getHpDamage();
    void awardGold(int gold);
    
    bool checkCirclesCollide(CCPoint center1, float radius1, CCPoint center2, float radius2);
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameLayer);
};

#endif /* defined(__TowerDefence__GameLayer__) */
