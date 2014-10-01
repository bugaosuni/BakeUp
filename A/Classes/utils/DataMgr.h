#ifndef DATA_MGR
#define DATA_MGR

#include "UtilHelper.h"

class DataMgr
{
private:
    DataMgr();
    virtual ~DataMgr();
    static DataMgr* pInstance;

    class Garbo
    {
    public:
        ~Garbo()
        {
            if (DataMgr::pInstance)
            {
                delete DataMgr::pInstance;
            }
        }
    };
    static Garbo* garbo;
public:
    static bool modalShow;
    static int mapIndex;
    static int carIndex;
    static bool bIsGameScene;
    static bool bIsPause;

    static DataMgr* getInstance();
    bool initGameData();
    map<string, string> getTextData();
    vector<MapData> getMapData();
    void setMapData(int index, MapModType modType, int modData);
    vector<CarData> getCarData();
    void setCarData(int index, CarModType modType, int modData);
    map<int, vector<PartCost>> getPartCostData();

    vector<CarInfo> getCarInfoData();
    vector<CarLevel> getCarLevelData();
    //vector<DailyTask> getDailyTaskData();
    
    float getRearSpeed(int carId, int engineLevel);
    float getWheelDensity(int carId, int suspensionLevel);
    float getWheelFriction(int carId, int wheelLevel);
    float getFrontSpeed(int carId, int fourWheelDriveLevel);
    

private:
    vector<AdConfData> m_adConfData;
    map<string, string> m_textData;
    vector<MapData> m_mapData;
    vector<CarData> m_carData;
    map<int, vector<PartCost>> m_partCostData;
    vector<CarInfo> m_carInfos;
    vector<CarLevel> m_carLevels;
    //vector<DailyTask> m_dailyTasks;
};

#endif // DATA_MGR
