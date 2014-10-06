#include "TerrainView.h"
#include "VisibleRect.h"
#include "GameScene.h"

TerrainView::TerrainView()
{
    m_nOffsetX = 0;
    m_nFromKeyPoint = 0;
    m_nToKeyPoint = 0;
    m_nMapVertices = 0;
    m_nBorderVertices = 0;
    m_nLastGasStationPoint = 0;
    m_nLastCoinPoint = 0;
    m_nSegmentWidth = 10;
    m_fCoinOffset = 0.0f;

    m_pWorld = NULL;
    m_pMapGroundBody = NULL;
    surfaceSprite = NULL;
    groundSprite = NULL;

}

TerrainView::~TerrainView()
{
    CC_SAFE_RELEASE_NULL(surfaceSprite);
    CC_SAFE_RELEASE_NULL(groundSprite);
   // CC_SAFE_DELETE(m_pWorld);
}

TerrainView * TerrainView::createWithWorld(b2World *world)
{
    TerrainView *pRet = new TerrainView();

    if (pRet && pRet->initWithWorld(world))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool TerrainView::initWithWorld(b2World *world)
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!CCNode::init());
        m_pWorld = world;
        this->setShaderProgram(ShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
        this->generateHills();
        this->resetHillVertices();

        bRet = true;
    } while (0);

    return bRet;
}

void TerrainView::draw(Renderer *renderer, const Mat4 &transform, bool transformUpdated)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(TerrainView::onDraw, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
}

void TerrainView::onDraw(const Mat4 &transform, bool transformUpdated)
{
    CC_NODE_DRAW_SETUP();
    GL::bindTexture2D(groundSprite->getTexture()->getName());
    GL::enableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
    DrawPrimitives::setDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_arrfGroundVertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_arrfGroundTexCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)m_nMapVertices);

    
    GL::bindTexture2D(surfaceSprite->getTexture()->getName());
    GL::enableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
    DrawPrimitives::setDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_arrfSurfaceVertices);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_arrfSurfaceTexCoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)m_nMapVertices);


    // 调试物理引擎使用
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    //m_pWorld->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

}

void TerrainView::setOffsetX(const Vec2 pt)
{
    m_nOffsetX = pt.x * PTM_RATIO;
    float fOffsetY = 200 - pt.y * PTM_RATIO;
    if (fOffsetY > 300)
    {
        fOffsetY = 300;
    }
    this->setPosition(ccp(VisibleRect::right().x / 8 - m_nOffsetX * this->getScale(), fOffsetY));
    this->resetHillVertices();

}

void TerrainView::generateHills()
{
    mMapPoints = UtilHelper::readMapPointsData(DataMgr::mapIndex + 1);
    this->addStartAndEndBrand();
}

void TerrainView::resetHillVertices()
{
    static int prevFromKeyPointI = -1;
    static int prevToKeyPointI = -1;
    m_nFromKeyPoint = 0;
    m_nToKeyPoint = 0;
    while (mMapPoints[m_nFromKeyPoint + 1].x < m_nOffsetX - VisibleRect::right().x / 8 / this->getScale())
    {
        m_nFromKeyPoint++;
    }
    
    while (mMapPoints[m_nToKeyPoint].x < m_nOffsetX + VisibleRect::right().x * 9 / 8 / this->getScale())
    {
        m_nToKeyPoint++;
    }

    if (prevFromKeyPointI != m_nFromKeyPoint || prevToKeyPointI != m_nToKeyPoint)
    {
        // 屏幕显示区域
        m_nMapVertices = 0;
        m_nBorderVertices = 0;
        Vec2 p0, p1, pt0, pt1;
        p0 = Vec2(mMapPoints[m_nFromKeyPoint].x, mMapPoints[m_nFromKeyPoint].y);
        if (mMapPoints[m_nToKeyPoint].x /5000 == 1)
        {
            m_nSegmentWidth = 16;
        }
        else if (mMapPoints[m_nToKeyPoint].x / 5000 == 2)
        {
            m_nSegmentWidth = 22;
        }
        else if (mMapPoints[m_nToKeyPoint].x / 5000 == 3)
        {
            m_nSegmentWidth = 28;
        }
        else if (mMapPoints[m_nToKeyPoint].x / 5000 == 4)
        {
            m_nSegmentWidth = 35;
        }

        for (int i = m_nFromKeyPoint + 1; i < m_nToKeyPoint + 1; ++i)
        {
            p1 = Vec2(mMapPoints[i].x, mMapPoints[i].y);

            // triangle strip between p0 and p1
           // int hSegments = floorf((p1.x - p0.x) / kHillSegmentWidth);
            int hSegments = floorf((p1.x - p0.x) / m_nSegmentWidth);


            float dx = (p1.x - p0.x) / hSegments;
            float da = M_PI / hSegments;
            float ymid = (p0.y + p1.y) / 2;
            float ampl = (p0.y - p1.y) / 2;
            pt0 = p0;
            m_arrfBorderVertices[m_nBorderVertices++] = pt0;
            for (int j = 1; j < hSegments + 1; ++j)
            {
                pt1.x = p0.x + j * dx;
                pt1.y = ymid + ampl * cosf(da * j);
                m_arrfBorderVertices[m_nBorderVertices++] = pt1;

                float fTexCoords_pt0 = float(pt0.y - 200) / 512;
                float fTexCoords_pt1 = float(pt1.y - 200) / 512;

                 // 土壤
                m_arrfGroundVertices[m_nMapVertices] = Vec2(pt0.x, -312);
                m_arrfGroundTexCoords[m_nMapVertices] = Vec2(pt0.x / 512, 1.0f);
                // 草坪
                m_arrfSurfaceVertices[m_nMapVertices] = Vec2(pt0.x, pt0.y - 32);
                m_arrfSurfaceTexCoords[m_nMapVertices] = Vec2(pt0.x / 512, 1.0f);

                // 下一个点
                m_nMapVertices += 1;

                m_arrfGroundVertices[m_nMapVertices] = Vec2(pt1.x, -312);
                m_arrfGroundTexCoords[m_nMapVertices] = Vec2(pt1.x / 512, 1.0f);
                m_arrfSurfaceVertices[m_nMapVertices] = Vec2(pt1.x, pt1.y - 32);
                m_arrfSurfaceTexCoords[m_nMapVertices] = Vec2(pt1.x / 512, 1.0f);

                 m_nMapVertices += 1;

                m_arrfGroundVertices[m_nMapVertices] = Vec2(pt0.x, pt0.y - 16);
                m_arrfGroundTexCoords[m_nMapVertices] = Vec2(pt0.x / 512, 0 - fTexCoords_pt0);
                m_arrfSurfaceVertices[m_nMapVertices] = Vec2(pt0.x, pt0.y);
                m_arrfSurfaceTexCoords[m_nMapVertices] = Vec2(pt0.x / 512, 0);

                 m_nMapVertices += 1;

                m_arrfGroundVertices[m_nMapVertices] = Vec2(pt1.x, pt1.y - 16);
                m_arrfGroundTexCoords[m_nMapVertices] = Vec2(pt1.x / 512,0 - fTexCoords_pt1);
                m_arrfSurfaceVertices[m_nMapVertices] = Vec2(pt1.x, pt1.y);
                m_arrfSurfaceTexCoords[m_nMapVertices] = Vec2(pt1.x / 512, 0);

                m_nMapVertices += 1;

                pt0 = pt1;
            }
            p0 = p1;
        }
        prevFromKeyPointI = m_nFromKeyPoint;
        prevToKeyPointI = m_nToKeyPoint;
        this->resetBox2DBody();
        if (m_nToKeyPoint % 25 == 0  && m_nToKeyPoint != m_nLastGasStationPoint)
        {
            m_nLastGasStationPoint = m_nToKeyPoint;
            this->addGasStation(Vec2(mMapPoints[m_nToKeyPoint].x, mMapPoints[m_nToKeyPoint].y));
        }
        
        if (m_nToKeyPoint % 7 == 0 &&  m_nToKeyPoint != m_nLastCoinPoint)
        {
            m_nLastCoinPoint = m_nToKeyPoint;
            m_fCoinOffset = float ( mMapPoints[m_nToKeyPoint + 1].y - mMapPoints[m_nToKeyPoint].y) / float (mMapPoints[m_nToKeyPoint + 1].x - mMapPoints[m_nToKeyPoint].x);
            this->addCoins(Vec2(mMapPoints[m_nToKeyPoint].x, mMapPoints[m_nToKeyPoint].y));
        }
    }
}

void TerrainView::resetBox2DBody()
{
    if (m_pMapGroundBody)
    {
        m_pWorld->DestroyBody(m_pMapGroundBody);
    }

    b2BodyDef bd;
    bd.position.Set(0, 0);
    m_pMapGroundBody = m_pWorld->CreateBody(&bd);

    b2EdgeShape shape;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.0f;
    fd.friction = 1.0f;

    b2Vec2 p1, p2;
    for (int i = 0; i < m_nBorderVertices - 1; ++i)
    {
        p1 = b2Vec2(m_arrfBorderVertices[i].x / PTM_RATIO, (m_arrfBorderVertices[i].y - 16)/ PTM_RATIO);
        p2 = b2Vec2(m_arrfBorderVertices[i + 1].x / PTM_RATIO, (m_arrfBorderVertices[i + 1].y - 16) / PTM_RATIO);
        shape.Set(p1, p2);
        m_pMapGroundBody->CreateFixture(&fd);
    }

}

void TerrainView::addBridge(const Vec2 pt1, const Vec2 pt2)
{
    b2Body* ground = NULL;
    {
        b2BodyDef bd;
        ground = m_pWorld->CreateBody(&bd);
        b2EdgeShape shape;
        shape.Set(b2Vec2(pt1.x/ PTM_RATIO, 0.0f), b2Vec2(pt2.x / PTM_RATIO, 0.0f));
        ground->CreateFixture(&shape, 0.0f);
    }

    b2PolygonShape shape;
    shape.SetAsBox(1.0f, 0.125f);

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 20.0f;
    fd.friction = 0.2f;

    b2RevoluteJointDef jd;
    

    int e_count = (pt2.x - pt1.x) / 64;
    b2Body* prevBody = ground;
    for (int32 i = 0; i < e_count; ++i)
    {
        auto bridge_sprite = Sprite::create("bridge.png");
        bridge_sprite->setTag(SPRITE_CAR);
        bridge_sprite->setPosition((pt1.x + 32 + 64.0f * i), pt1.y - 16.0f);
        this->addChild(bridge_sprite);
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.userData = bridge_sprite;
        bd.position.Set((pt1.x + 32+ 64.0f * i) / PTM_RATIO, (pt1.y - 16.0f) / PTM_RATIO);
        b2Body* body = m_pWorld->CreateBody(&bd);
        body->CreateFixture(&fd);

        b2Vec2 anchor((pt1.x + 64.0f * i) / PTM_RATIO, (pt1.y - 16.0f)/ PTM_RATIO);
        jd.Initialize(prevBody, body, anchor);
        m_pWorld->CreateJoint(&jd);
        prevBody = body;
    }

    b2Vec2 anchor((pt1.x + 64.0f * e_count) / PTM_RATIO, (pt1.y - 16.0f) / PTM_RATIO);
    jd.Initialize(prevBody, ground, anchor);
    m_pWorld->CreateJoint(&jd);
}

void TerrainView::addGasStation(const Vec2 pt)
{
    auto gasStationSprite = Sprite::createWithSpriteFrameName("icon_oil.png");
    gasStationSprite->setTag(SPRITE_GAS_STATION);
    gasStationSprite->setPosition(pt.x, (pt.y + 30));
    this->addChild(gasStationSprite);
}

void TerrainView::addCoins(const Vec2 pt)
{
    if (pt.x > 32000)
    {
        return;
    }
    int count = rand() % 4 + 1;
    for (int i = 0 ; i < count ; i++)
    {
        int value = rand() % 4 + 1;
        auto coinSprite = Sprite::createWithSpriteFrameName("coin" + String::createWithFormat("%d",value)->_string + ".png");
        coinSprite->setTag(SPRITE_COIN1 + value - 1);
        coinSprite->setPosition(pt.x + i * 60, (pt.y + 30 + (m_fCoinOffset * 30) * i));
        this->addChild(coinSprite);
    }
}

void TerrainView::addStartAndEndBrand()
{
    auto startBrandSprite = Sprite::createWithSpriteFrameName("start.png");
    startBrandSprite->setPosition(250, 50);
    this->addChild(startBrandSprite, -2);

    auto endBrandSprite = Sprite::createWithSpriteFrameName("end.png");
    endBrandSprite->setPosition(32750, 50);
    this->addChild(endBrandSprite, -2);
}