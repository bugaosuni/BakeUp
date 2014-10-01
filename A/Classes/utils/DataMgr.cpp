#include "DataMgr.h"
#include "AudioEnginMgr.h"
#include "StringConst.h"
#include "HttpClientMgr.h"

bool DataMgr::modalShow = false;
int DataMgr::mapIndex = 0;
int DataMgr::carIndex = 0;
bool DataMgr::bIsGameScene = false;
bool DataMgr::bIsPause =false;

DataMgr* DataMgr::pInstance = new DataMgr();

DataMgr::DataMgr()
{

}

DataMgr::~DataMgr()
{
    if (pInstance)
        delete pInstance;
}

DataMgr* DataMgr::getInstance()
{
    return pInstance;
}

bool DataMgr::initGameData()
{
    HttpClientMgr::getInstance()->ReadHttpFile( HttpClientMgr::getInstance()->m_sUrlPre + "conf.json", "conf.json");

    if (!UtilHelper::getFromBool(WRITE_DATA))
    {
        if (UtilHelper::writeMapDataToSD() && UtilHelper::writeCarDataToSD() 
                && UtilHelper::writeDailyTaskDataToSD() && UtilHelper::writeAchievementDataToSD())
        {
            UtilHelper::writeToInteger(USER_GOLD, 2000);
            UtilHelper::writeToBool(WRITE_DATA, true);
        }
    }
    
    FileVersion fileViersion = UtilHelper::readFileVersionData();
    if (UtilHelper::getFromInteger(MAP_VERSION) != fileViersion.map_version)
    {
        if (UtilHelper::updateMapDate())
        {
            UtilHelper::writeToInteger(MAP_VERSION, fileViersion.map_version);
        }
    }

    if (UtilHelper::getFromInteger(CAR_VERSION) != fileViersion.car_version)
    {
        if (UtilHelper::updateCarDate())
        {
            UtilHelper::writeToInteger(CAR_VERSION, fileViersion.car_version);
        }
    }
    
    m_textData = UtilHelper::readTextData();
    m_mapData = UtilHelper::readMapData();
    m_carData = UtilHelper::readCarData();
    m_partCostData = UtilHelper::readPartCostData();
    m_carInfos = UtilHelper::readCarInfoData();
    m_carLevels = UtilHelper::readCarLevelData();
   // m_dailyTasks = UtilHelper::readDailyTaskData();
    
    AudioEnginMgr::getInstance()->initMusic();
    return true;
}

map<string, string> DataMgr::getTextData()
{
    return m_textData;
}

vector<MapData> DataMgr::getMapData()
{
    return m_mapData;
}

/*vector<DailyTask> DataMgr::getDailyTaskData()
{
    return m_dailyTasks;
}*/

void DataMgr::setMapData(int index, MapModType modType, int modData)
{
    switch (modType)
    {
    case e_map_open:
        m_mapData[index].open = modData;
        break;
    case e_level:
        m_mapData[index].level = modData;
        break;
    case e_bestDist:
        m_mapData[index].bestDist = modData;
        break;
    default:
        break;
    }
}

vector<CarData> DataMgr::getCarData()
{
    return m_carData;
}

void DataMgr::setCarData(int index, CarModType modType, int modData)
{
    switch (modType)
    {
    case e_car_open:
        m_carData[index].open = modData;
        break;
    case e_part1_level:
        m_carData[index].part1_level = modData;
        break;
    case e_part2_level:
        m_carData[index].part2_level = modData;
        break;
    case e_part3_level:
        m_carData[index].part3_level = modData;
        break;
    case e_part4_level:
        m_carData[index].part4_level = modData;
        break;
    default:
        break;
    }
}

map<int, vector<PartCost>> DataMgr::getPartCostData()
{
    return m_partCostData;
}

vector<CarInfo> DataMgr::getCarInfoData()
{
    return m_carInfos;
}

vector<CarLevel> DataMgr::getCarLevelData()
{
    return m_carLevels;
}

float DataMgr::getWheelFriction(int carId, int wheel_level)
{
    float friction;
    friction = m_carLevels[carId-1].wheel_friction1 + (float)wheel_level * 0.05;
    return friction;
}

float DataMgr::getRearSpeed(int carId, int engine_level)
{
    float rear_speed;
    rear_speed = m_carLevels[carId-1].rear_speed1 + engine_level * 10;
    return rear_speed;
}


float DataMgr::getFrontSpeed(int carId, int frontWheelDriveLevel)
{
    float frontSpeed;
    frontSpeed = m_carLevels[carId-1].front_speed1 + 5 * frontWheelDriveLevel;
    return frontSpeed;
}

float DataMgr::getWheelDensity(int carId, int suspensionLevel)
{
    float wheelDensity;
    wheelDensity = m_carLevels[carId-1].suspension_level1 + (float)suspensionLevel * 0.2;
    
    return wheelDensity;
}