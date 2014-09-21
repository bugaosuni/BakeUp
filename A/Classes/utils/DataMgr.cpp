#include "DataMgr.h"
#include "AudioEnginMgr.h"
#include "StringConst.h"
#include "HttpClientMgr.h"

bool DataMgr::modalShow = false;
int DataMgr::mapIndex = 0;
int DataMgr::carIndex = 0;
bool DataMgr::bIsSpalshEnter = true;
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

void DataMgr::setAdConfData(vector<AdConfData> data)
{
    m_adConfData = data;
}

vector<AdConfData> DataMgr::getAdConfData()
{
    return m_adConfData;
}

void DataMgr::readAdConfData()
{
    if (m_adConfData.empty())//local not exist or data is null
    {
        //save file to local
        HttpClientMgr::getInstance()->GetHttpFile("http://www.coolgameworld.com/root/com_gamefunny_hill_climb/conf.json", "adConf.json");
    }
    else
    {
        //check out the data, if equals, then do nothing; else, update the conf and img file 
        HttpClientMgr::getInstance()->ReadHttpFile("http://www.coolgameworld.com/root/com_gamefunny_hill_climb/conf.json", "adConf.json");
    }
}

bool DataMgr::initGameData()
{

    m_adConfData =  UtilHelper::readAdConfData();
    readAdConfData();
    if (!UtilHelper::getFromBool(WRITE_DATA))
    {
        if (UtilHelper::writeMapDataToSD() && UtilHelper::writeCarDataToSD())
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
    switch (wheel_level)
        {
        case 1:
            friction = m_carLevels[carId-1].wheel_friction1;
            break;

        case 2:
            friction = m_carLevels[carId-1].wheel_friction2;
            break;

        case 3:
            friction = m_carLevels[carId-1].wheel_friction3;
            break;

        case 4:
            friction = m_carLevels[carId-1].wheel_friction4;;
            break;
        case 5:
            friction = m_carLevels[carId-1].wheel_friction5;
            break;
        default:
            friction = m_carLevels[carId-1].wheel_friction5;
        }
    return friction;
}

float DataMgr::getRearSpeed(int carId, int engine_level)
{
    float rear_speed;
    switch (engine_level)
        {
        case 1:
            rear_speed = m_carLevels[carId-1].rear_speed1;
            break;

        case 2:
            rear_speed = m_carLevels[carId-1].rear_speed2;
            break;

        case 3:
            rear_speed = m_carLevels[carId-1].rear_speed3;
            break;

        case 4:
            rear_speed = m_carLevels[carId-1].rear_speed4;;
            break;
        case 5:
            rear_speed = m_carLevels[carId-1].rear_speed5;
            break;
        default:
            rear_speed = m_carLevels[carId-1].rear_speed5;
        }
    return rear_speed;
}


float DataMgr::getFrontSpeed(int carId, int frontWheelDriveLevel)
{
    float frontSpeed;
    switch (frontWheelDriveLevel)
        {
        case 1:
            frontSpeed = m_carLevels[carId-1].front_speed1;
            break;

        case 2:
            frontSpeed = m_carLevels[carId-1].front_speed2;
            break;

        case 3:
            frontSpeed = m_carLevels[carId-1].front_speed3;
            break;

        case 4:
            frontSpeed = m_carLevels[carId-1].front_speed4;;
            break;
        case 5:
            frontSpeed = m_carLevels[carId-1].front_speed5;
            break;
        default:
            frontSpeed = m_carLevels[carId-1].front_speed5;
        }
    return frontSpeed;
}

float DataMgr::getWheelDensity(int carId, int suspensionLevel)
{
    float wheelDensity;
    switch (suspensionLevel)
        {
        case 1:
            wheelDensity = m_carLevels[carId-1].suspension_level1;
            break;

        case 2:
            wheelDensity = m_carLevels[carId-1].suspension_level2;
            break;

        case 3:
            wheelDensity = m_carLevels[carId-1].suspension_level3;
            break;

        case 4:
            wheelDensity = m_carLevels[carId-1].suspension_level4;;
            break;
        case 5:
            wheelDensity = m_carLevels[carId-1].suspension_level5;
            break;
        default:
            wheelDensity = m_carLevels[carId-1].suspension_level5;
        }
    return wheelDensity;
}