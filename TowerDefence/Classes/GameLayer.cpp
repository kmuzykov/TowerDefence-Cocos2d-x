//
//  GameLayer.cpp
//  TowerDefence
//
//  Created by Kirill Muzykov on 8/21/13.
//
//

#include "GameLayer.h"
#include "Tower.h"
#include "Waypoint.h"
#include "Enemy.h"
#include "SimpleAudioEngine.h"

#define TOWER_COST  300

const CCArray& GameLayer::getEnemies()
{
    return enemies;
}

bool GameLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    //Initial field values.
    wave = 0;
    playerHP = 5;
    playerGold = 1000;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* background = CCSprite::create("bg.png");
    background->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
    this->addChild(background);

    //HP
    ui_hp_lbl = CCLabelBMFont::create(CCString::createWithFormat("HP: %d", playerHP)->getCString(), "font_red_14.fnt");
    ui_hp_lbl->setPosition(ccp(35, winSize.height - 12));
    ui_hp_lbl->setAnchorPoint(ccp(0,0.5f));
    this->addChild(ui_hp_lbl);    
    
    //WAVE
    ui_wave_lbl = CCLabelBMFont::create("", "font_red_14.fnt");
    ui_wave_lbl->setPosition(ccp(400, winSize.height - 12));
    ui_wave_lbl->setAnchorPoint(ccp(0, 0.5f));
    this->addChild(ui_wave_lbl);
    
    //GOLD
    ui_gold_lbl = CCLabelBMFont::create("", "font_red_14.fnt");
    ui_gold_lbl->setPosition(ccp(135, winSize.height - 12));
    ui_gold_lbl->setAnchorPoint(ccp(0, 0.5f));
    this->addChild(ui_gold_lbl);
    this->awardGold(0); //just to display initial ammount

    this->loadTowerPositions();
    this->addWaypoints();
    this->loadWave();

    this->setTouchEnabled(true);
    
    //Playing background music
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("8bitDungeonLevel.mp3", true);
    
    return true;
}

void GameLayer::loadTowerPositions()
{
    CCArray* towerPositions = CCArray::createWithContentsOfFile("TowersPosition.plist");
    CCObject* obj;
    CCARRAY_FOREACH(towerPositions, obj)
    {
        //Converting to actual obj type
        CCDictionary* towerPos = (CCDictionary*)obj;
        
        //Creating sprite at position
        CCSprite* towerBase = CCSprite::create("open_spot.png");
        CCPoint p = ccp(towerPos->valueForKey("x")->intValue(),
                        towerPos->valueForKey("y")->intValue());
        towerBase->setPosition(p);
        
        //Adding to layer
        this->addChild(towerBase);
        
        //Adding to array
        towerBases.addObject(towerBase);
    }
}

void GameLayer::addWaypoints()
{
    //Adding path waypoints. Starting from our base and moving to start of enemies.
    Waypoint* wp1 = this->addWaypoint(ccp(420,35), NULL);
    Waypoint* wp2 = this->addWaypoint(ccp(35,35), wp1);
    Waypoint* wp3 = this->addWaypoint(ccp(35,130), wp2);
    Waypoint* wp4 = this->addWaypoint(ccp(445,130), wp3);
    Waypoint* wp5 = this->addWaypoint(ccp(445,220), wp4);
    this->addWaypoint(ccp(-40, 220), wp5);
}

Waypoint* GameLayer::addWaypoint(CCPoint pos, Waypoint *next)
{
    Waypoint* wp = Waypoint::create();
    wp->setPosition(pos);
    wp->setNextWaypoint(next);
    
    this->addChild(wp);
    waypoints.addObject(wp);
    
    return wp;
}

bool GameLayer::loadWave()
{
    CCArray* waveData = CCArray::createWithContentsOfFile("Waves.plist");    
    if (wave >= waveData->count())
    {
        return false;
    }
    
    CCArray* currentWaveData = CCArray::createWithArray((CCArray*)waveData->objectAtIndex(wave));
    
    CCObject* obj;
    CCARRAY_FOREACH(currentWaveData, obj)
    {
        CCDictionary* enemyData = (CCDictionary*)obj;
        Enemy* enemy = Enemy::create("enemy.png");
        enemy->setTheGame(this);
        
        //Finding starting waypoint (which is last in array).
        //Setting enemy to its position and directing to next waypoint.
        Waypoint* lastWp = (Waypoint*)waypoints.lastObject();
        enemy->setPosition(lastWp->getPosition());
        enemy->setDestinationWaypoint(lastWp->getNextWaypoint());
        enemy->schedule(schedule_selector(Enemy::doActivate), enemyData->valueForKey("spawnTime")->floatValue());
        
        this->addChild(enemy);
        enemies.addObject(enemy);
    }
    
    //Updagin counter & label.
    wave++;
    CCString* waveString = CCString::createWithFormat("WAVE: %d", wave);
    ui_wave_lbl->setString(waveString->getCString());
    
    return true;
}

bool GameLayer::canBuyTower()
{
    return (playerGold - TOWER_COST >= 0);
}

void GameLayer::enemyGotKilled(Enemy* enemy)
{
    enemies.removeObject(enemy);    
    if (enemies.count() <= 0)
    {
        if (!this->loadWave())
        {
            CCLOG("You win");
            CCDirector::sharedDirector()->replaceScene(CCTransitionSplitCols::create(1, GameLayer::scene()));
        }
    }
}

void GameLayer::getHpDamage()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("life_lose.wav", false);
    
    playerHP--;
    if (playerHP <= 0)
    {
        playerHP = 0;
        this->doGameOver();
    }
    
    ui_hp_lbl->setString(CCString::createWithFormat("HP: %d", playerHP)->getCString());
}

void GameLayer::awardGold(int gold)
{
    playerGold += gold;
    ui_gold_lbl->setString(CCString::createWithFormat("GOLD %d", playerGold)->getCString());
}

void GameLayer::doGameOver()
{
    if (!gameEnded)
    {
        gameEnded = true;
        CCDirector::sharedDirector()->replaceScene(CCTransitionRotoZoom::create(1.0f, GameLayer::scene()));
    }    
}

bool GameLayer::checkCirclesCollide(CCPoint center1, float radius1, CCPoint center2, float radius2)
{
    float distance = ccpDistance(center1, center2);
    return distance <= radius1 + radius2;
}

void GameLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (!this->canBuyTower())
        return;
    
    //Converting coords of touch point.
    CCTouch* touch =  (CCTouch*)pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    //Finding tower spot we've cliced on.
    CCObject* obj;
    CCARRAY_FOREACH(&towerBases, obj)
    {
        //Checking that there is no turret already.
        CCSprite* sprite = (CCSprite*)obj;
        if (!sprite->getUserData() && sprite->boundingBox().containsPoint(location))
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("tower_place.wav", false);
            
            Tower* tower = Tower::create("tower.png");
            tower->setTheGame(this);
            
            tower->setAnchorPoint(ccp(0.5, 0.5));
            CCPoint pos = sprite->getPosition();            
            tower->setPosition(pos);
            this->addChild(tower);
            sprite->setUserData(tower);
            
            //spending gold
            playerGold -= TOWER_COST;
            this->awardGold(0);
        }
    }
}

CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}