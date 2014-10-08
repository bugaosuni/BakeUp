#include "GameMap.h"
#include "GameScene.h"
#include "Bullet.h"
#include "Enemy.h"

GameMap* GameMap::m_pGameMap;

GameMap* GameMap::getGameMap()
{
    return m_pGameMap;
}

GameMap::GameMap()
{
    m_pGameMap = NULL;
    landLayer = NULL;
    m_heroBirthPos = Point(0.0f, 0.0f);
    m_tileSize = Size(0.0f, 0.0f);
    m_mapSize = Size(0.0f, 0.0f);
    m_pBreakSpriteArray = Array::createWithCapacity(4);
    m_pBreakSpriteArray->retain();

    m_pEnemyArray = Array::create();
    m_pEnemyArray->retain();

    m_pBulletArray = Array::create();
    m_pBulletArray->retain();


}

GameMap::~GameMap()
{
    this->unscheduleAllSelectors();
}

GameMap* GameMap::create(const char *tmxFile)
{
    GameMap *pGameMap = new GameMap();
    if (pGameMap && pGameMap->initWithTMXFile(tmxFile))
    {
        pGameMap->extraInit();
        pGameMap->autorelease();
        return pGameMap;
    }
    CC_SAFE_DELETE(pGameMap);
    return NULL;
}

void GameMap::extraInit()
{
    m_pGameMap = this;
    
    m_tileSize = this->getTileSize();
    m_mapSize = this->getMapSize();

    landLayer = this->getLayer("land");
    coinLayer = this->getLayer("coin");
    objectLayer = this->getObjectGroup("objects");

    this->initObjects();
    this->launchEnemyInMap();
    this->scheduleUpdate();
}

void GameMap::update(float dt)
{
    this->enemyVSHero();
    this->bulletVSEnemy();
}

void GameMap::initObjects()
{
    ValueVector tempArray = objectLayer->getObjects();

    float x, y, w, h;
    Value objPointMap;
    for (auto objPointMap : tempArray)
    {
        ValueMap objPoint = objPointMap.asValueMap();
        int posX = objPoint.at("x").asFloat();
        int posY = objPoint.at("y").asFloat();
        posY -= this->getTileSize().height;
        Point tileXY = this->positionToTileCoord(Point(posX, posY));
        std::string name = objPoint.at("name").asString();
        std::string type = objPoint.at("type").asString();
       
       if (name == "others")
        {
            if (type == "BirthPoint")
            {
                // 马里奥在当前地图上的出生点
                m_heroBirthPos = this->tilecoordToPosition(tileXY);
                m_heroBirthPos.x += this->getTileSize().width / 2;
            }
        }
        else if (name == "enemy")
        {
            Enemy *pEnemy = NULL;
            if (type == "tortoise")
            {
                // 地图上预先标记好的乌龟都是向左的
                pEnemy = new EnemyTortoise(0);
            }
            else if (type == "tortoise_round")
            {
                std::string dis = objPoint.at("roundDis").asString();
                int roundDis = atof(dis.c_str());
                pEnemy = new EnemyTortoiseRound(roundDis);
            }
            else if (type == "flower")
            {
                pEnemy = new EnemyFlower();
            }
            if (pEnemy != NULL)
            {
                pEnemy->setTileCoord(tileXY);
                pEnemy->setEnemyPos(Point(posX, posY));
                m_pEnemyArray->addObject(pEnemy);
            }
        }
    }
}


void GameMap::launchEnemyInMap()
{
    Enemy *tempEnemy = NULL;
    unsigned int enemyCount = m_pEnemyArray->count();
    for (unsigned int idx = 0; idx < enemyCount; ++idx)
    {
        tempEnemy = (Enemy *)m_pEnemyArray->objectAtIndex(idx);
        tempEnemy->setPosition(tempEnemy->getEnemyPos());
        switch (tempEnemy->getEnemyType())
        {
        case eEnemy_flower:
        case eEnemy_AddMushroom:
            this->addChild(tempEnemy, 1);
            break;
        default:
            this->addChild(tempEnemy, 7);
            break;
        }
        tempEnemy->launchEnemy();
    }
}

void GameMap::enemyVSHero()
{
    Enemy *tempEnemy = NULL;
    EnemyVSHero vsRet;
    unsigned int enemyCount = m_pEnemyArray->count();
    for (unsigned int idx = 0; idx < enemyCount; ++idx)
    {
        tempEnemy = (Enemy *)m_pEnemyArray->objectAtIndex(idx);
        if (tempEnemy->getEnemyState() == eEnemyState_active)
        {
            vsRet = tempEnemy->checkCollisionWithHero();
            switch (vsRet)
            {
            case eVS_heroKilled:
            {
                if (!Hero::getInstance()->getIsSafeTime())
                {
                    Hero::getInstance()->changeForGotEnemy();
                }
                break;
            }
            case eVS_enemyKilled:
            {
                tempEnemy->forKilledByHero();
                break;
            }
            default:
                break;
            }
        }
    }
}

void GameMap::bulletVSEnemy()
{
    unsigned int bulletCount = m_pBulletArray->count();
    unsigned int enemyCount = m_pEnemyArray->count();
    Bullet *pBullet = NULL;
    Enemy *pEnemy = NULL;
    Array *delBullet = Array::create();
    delBullet->retain();
    Array *delEnemy = NULL;
    Rect bulletRect;
    Rect enemyRect;

    for (unsigned int idxBullet = 0; idxBullet < bulletCount; ++idxBullet)
    {
        pBullet = (Bullet *)m_pBulletArray->objectAtIndex(idxBullet);
        if (pBullet->getBulletState() == eBulletState_nonactive)
        {
            delBullet->addObject(pBullet);
            continue;
        }
        bulletRect = pBullet->getBulletRect();

        for (unsigned int idxEnemy = 0; idxEnemy < enemyCount; ++idxEnemy)
        {
            pEnemy = (Enemy *)m_pEnemyArray->objectAtIndex(idxEnemy);
            switch (pEnemy->getEnemyType())
            {
                // 火串是不能被任何子弹打掉的
                // 魂斗罗的炮台发射的子弹不会被任何子弹打掉
            case eEnemy_BatteryBullet:
            case eEnemy_fireString:
            case eEnemy_Lighting:
            case eEnemy_DarkCloud:
                continue;
                break;
            }
            if (pBullet->getBulletType() == eBullet_common &&
                pEnemy->getEnemyType() == eEnemy_Boss)
            {
                // 普通的子弹对Boss没有作用
                continue;
            }
            if (pBullet->getBulletType() == eBullet_common &&
                pEnemy->getEnemyType() == eEnemy_BossFire)
            {
                // 普通子弹对Boss发出的火球也是没有作用的
                continue;
            }

            if (pEnemy->getEnemyState() == eEnemyState_active)
            {
                enemyRect = pEnemy->getEnemyRect();

                if (bulletRect.intersectsRect(enemyRect))
                {
                    pBullet->forKilledEnemy();
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("HuoQiuDaDaoGuaiWu.ogg");
                    pEnemy->forKilledByBullet();
                }
            }
        }
    }

    unsigned int delCount = delBullet->count();
    for (unsigned int idxDel = 0; idxDel < delCount; ++idxDel)
    {
        pBullet = (Bullet *)delBullet->objectAtIndex(idxDel);
        m_pBulletArray->removeObject(pBullet, true);
        this->removeChild(pBullet, true);
    }
    delBullet->release();
}

TMXLayer* GameMap::getLandLayer()const
{
    return landLayer;
}

TMXLayer* GameMap::getCoinLayer()const
{
    return coinLayer;
}

CCTMXObjectGroup* GameMap::getObjectLayer()const
{
    return objectLayer;
}


Point GameMap::getHeroBirthPos()
{
    return m_heroBirthPos;
}

// 接受一个Tile坐标系下的点，给出该点对应在图中的图块元素类型
TileType GameMap::tileTypeforPos(Point tileCoord)
{
    int GID = landLayer->tileGIDAt(tileCoord);
    if (GID > 0)
    {
        return eTile_Land;
    }
    GID = coinLayer->tileGIDAt(tileCoord);
    if (GID > 0)
    {
        return eTile_Coin;
    }
    return eTile_NoneH;
}

// 在block层检测 哪些block砖块是可有被顶碎的？
// 哪些砖块是闪烁的问号，头顶一下会弹出一个金币来
// 在地图的图素属性中，用blockType来表示
// 1: 闪烁的问号
// 2: 普通的一顶即碎的砖块
bool GameMap::crossedHeadBlock(Point tileCoord, BodyType _bodyType)
{
    int gID = landLayer->tileGIDAt(tileCoord);

    Value pD;
    pD = this->propertiesForGID(gID);

    if (pD.isNull() == false)
    {
        int value = pD.asValueMap().at("blockType").asInt();
        if (value)
        {
            int blockType = value;
            log("blockType---------------->%d" , blockType);
            if (blockType == 1)
            {
                switch (_bodyType)
                {
                case eBody_Small:
                {
                    // 大号马里奥:砖块被顶碎成四块，然后消失掉
                    this->showBlockBroken(tileCoord);
                    landLayer->removeTileAt(tileCoord);
                }
                    break;
                    break;
                default:
                    break;
                }
            }
            else if (blockType == 2)
            {
               // return true;
            }
        }
    }
    return false;
}


void GameMap::showBlockBroken(Point tileCoord)
{

    CCTexture2D *pTexture = TextureCache::getInstance()->addImage("image/stoneAni_1.png");
    
    Point pos = this->tilecoordToPosition(tileCoord);
    pos.x += this->getTileSize().width / 2;
    pos.y += this->getTileSize().height / 2;

    SpriteFrame *frame1 = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 22, 22));
    Sprite *pBreakSprite1 = Sprite::createWithSpriteFrame(frame1);
    pBreakSprite1->setPosition(pos);
    m_pBreakSpriteArray->addObject(pBreakSprite1);

    SpriteFrame *frame2 = SpriteFrame::createWithTexture(pTexture, CCRectMake(22, 0, 22, 22));
    Sprite *pBreakSprite2 = Sprite::createWithSpriteFrame(frame2);
    pBreakSprite2->setPosition(pos);
    m_pBreakSpriteArray->addObject(pBreakSprite2);
    
    SpriteFrame *frame3 = SpriteFrame::createWithTexture(pTexture, CCRectMake(44, 0, 22, 22));
    Sprite *pBreakSprite3 = Sprite::createWithSpriteFrame(frame3);
    pBreakSprite3->setPosition(pos);
    m_pBreakSpriteArray->addObject(pBreakSprite3);

    SpriteFrame *frame4 = SpriteFrame::createWithTexture(pTexture, CCRectMake(66, 0, 22, 22));
    Sprite *pBreakSprite4 = Sprite::createWithSpriteFrame(frame4);
    pBreakSprite4->setPosition(pos);
    m_pBreakSpriteArray->addObject(pBreakSprite4);

    this->addChild(pBreakSprite1);
    this->addChild(pBreakSprite2);
    this->addChild(pBreakSprite3);
    this->addChild(pBreakSprite4);

    ActionInterval *pLeftUp = JumpBy::create(0.5f,
        Point(-this->getTileSize().width * 2, this->getTileSize().height), 10, 1);
    ActionInterval *pRightUp = JumpBy::create(0.5f,
        Point(this->getTileSize().width * 2, this->getTileSize().height), 10, 1);
    ActionInterval *pLeftDown = JumpBy::create(0.5f,
        Point(-this->getTileSize().width * 3, -this->getTileSize().height), 5, 1);
    ActionInterval *pRightDown = JumpBy::create(0.5f,
        Point(this->getTileSize().width * 3, -this->getTileSize().height), 5, 1);

    pBreakSprite1->runAction(pLeftUp);
    pBreakSprite2->runAction(pRightUp);
    pBreakSprite3->runAction(pLeftDown);
    pBreakSprite4->runAction(Sequence::create(pRightDown,
        CallFunc::create(this, callfunc_selector(GameMap::clearSpriteArray)), NULL));
}

void GameMap::clearSpriteArray()
{
    Sprite *pS = NULL;
    for (unsigned int idx = 0; idx < m_pBreakSpriteArray->count(); ++idx)
    {
        pS = (Sprite *)m_pBreakSpriteArray->objectAtIndex(idx);
        pS->removeFromParentAndCleanup(true);
    }
}


Point GameMap::positionToTileCoord(Point pos)
{
    int x = pos.x / this->getTileSize().width;
    int y = (this->getMapSize().height - 1  ) - pos.y / this->getTileSize().height;
    return Point(x, y);
}

Point GameMap::tilecoordToPosition(Point tileCoord)
{
    float x = tileCoord.x * this->getTileSize().width;
    float y = (this->getMapSize().height - 1 - tileCoord.y) * this->getTileSize().height;
    return Point(x, y);
}

void GameMap::createNewBullet()
{
    Bullet *pBullet = NULL;
    pBullet = new BulletCommon();

    m_pBulletArray->addObject(pBullet);

    pBullet->setPosition(pBullet->getStartPos());
    this->addChild(pBullet, 7);
    pBullet->launchBullet();
}