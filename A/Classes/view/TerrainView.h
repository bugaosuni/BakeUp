#ifndef __TERRAIN_MAP_H__
#define __TERRAIN_MAP_H__
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "utils/GLES-Render.h"
#include "UtilHelper.h"

USING_NS_CC;

#define kMaxHillKeyPoints 200
#define kHillSegmentWidth 10
#define kMaxHillVertices 10000
#define kMaxBorderVertices 2000


class TerrainView : public Node
{
public:

    TerrainView();
    ~TerrainView();
    CREATE_FUNC(TerrainView);

    static TerrainView *createWithWorld(b2World *world);
    bool initWithWorld(b2World *world);
    void setOffsetX(const Vec2 pt);
private:
    void generateHills();
    void resetHillVertices();
    void resetBox2DBody();

    void addBridge(const Vec2 pt1, const Vec2 pt2);
    
    void addGasStation(const Vec2 pt);
    
    void addCoins(const Vec2 pt);

    void addStartAndEndBrand();

    virtual void draw(Renderer *renderer, const Mat4 &transform, bool transformUpdated) override;

    void onDraw(const Mat4 &transform, bool transformUpdated);

    CustomCommand _customCommand;

    CC_SYNTHESIZE_RETAIN(Sprite*, surfaceSprite, Surface);
    CC_SYNTHESIZE_RETAIN(Sprite*, groundSprite, Ground);

    vector<MapPoints> mMapPoints;

private:

    int m_nOffsetX;
    int m_nFromKeyPoint;
    int m_nToKeyPoint;
    int m_nMapVertices;
    int m_nBorderVertices;
    int m_nLastGasStationPoint;
    int m_nLastCoinPoint;
    int m_nSegmentWidth;

    float m_fCoinOffset;

    b2World* m_pWorld;
    b2Body* m_pMapGroundBody;

    Vec2 m_arrfGroundVertices[kMaxHillVertices];
    Vec2 m_arrfGroundTexCoords[kMaxHillVertices];
    Vec2 m_arrfSurfaceVertices[kMaxHillVertices];
    Vec2 m_arrfSurfaceTexCoords[kMaxHillVertices];
    Vec2 m_arrfBorderVertices[kMaxBorderVertices];

};

#endif // __TERRAIN_MAP_H__