#include "DataMgr.h"
//#include "AudioEnginMgr.h"
#include "StringConst.h"
//#include "HttpClientMgr.h"

bool DataMgr::modalShow = false;
int DataMgr::worldId= 1;
int DataMgr::levelIndex = 0;
int DataMgr::allLevelCount = 0;
int DataMgr::currStarCount = 0;
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
       // HttpClientMgr::getInstance()->GetHttpFile("http://www.coolgameworld.com/root/com_gamefunny_hill_climb/conf.json", "adConf.json");
    }
    else
    {
        //check out the data, if equals, then do nothing; else, update the conf and img file 
       // HttpClientMgr::getInstance()->ReadHttpFile("http://www.coolgameworld.com/root/com_gamefunny_hill_climb/conf.json", "adConf.json");
    }
}

bool DataMgr::initGameData()
{
    m_adConfData =  UtilHelper::readAdConfData();//read ADdata from SD
    readAdConfData();//check adData via net
    if (!UtilHelper::getFromBool(WRITE_DATA))
    {
        if (UtilHelper::writePkgDataToSD("world.json") && UtilHelper::writePkgDataToSD("level.json"))
        {
            UtilHelper::writeToInteger(USER_GOLD, 2000);
            UtilHelper::writeToBool(WRITE_DATA, true);
        }
    }
    
	/*
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
    */
    m_textData = UtilHelper::readTextData();
    m_worldData = UtilHelper::readWorldData();
    m_levelData = UtilHelper::readLevelData();
    //AudioEnginMgr::getInstance()->initMusic();
    return true;
}

map<string, string> DataMgr::getTextData()
{
    return m_textData;
}

vector<WorldData> DataMgr::getWorldData()
{
    return m_worldData;
}

void DataMgr::setWorldData(int index, WorldModType modType, int modData)
{
    switch (modType)
    {
    case e_world_open:
        m_worldData[index].open = modData;
        break;
    default:
        break;
    }
}

map<int, vector<LevelData>> DataMgr::getLevelData()
{
    return m_levelData;
}

void DataMgr::setLevelData(int worldId, int index, LevelModType modType, int modData)
{
    switch (modType)
    {
    case e_level_open:
        m_levelData[worldId][index].open = modData;
        break;
    case e_level_star:
        m_levelData[worldId][index].star = modData;
        break;
    default:
        break;
    }
}

int DataMgr::getAllCurrStar()
{
	int star = 0;
	map<int, vector<LevelData>>::iterator it = m_levelData.begin();
	for (; it != m_levelData.end(); it++)
	{
		for (int i = 0; i < it->second.size(); i++)
		{
			star += it->second[i].star;
		}
	}
	return star;
}

int DataMgr::getWorldStar(int worldId)
{
	int star = 0;
	for (auto data: m_levelData[worldId])
	{
		star += data.star;
	}
	return star;
}