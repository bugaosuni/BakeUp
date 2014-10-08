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
    static int worldId;
    static int levelIndex;
	static int allLevelCount;
	static int currStarCount;
    static bool bIsSpalshEnter;
    static bool bIsGameScene;
    static bool bIsPause;

    static DataMgr* getInstance();
	void setAdConfData(vector<AdConfData> data);
	vector<AdConfData> getAdConfData();
	void readAdConfData();
	bool initGameData();
    map<string, string> getTextData();
    vector<WorldData> getWorldData();
    void setWorldData(int index, WorldModType modType, int modData);
	map<int, vector<LevelData>> getLevelData();
	void setLevelData(int worldId, int index, LevelModType modType, int modData);
	int getAllCurrStar();
	int getWorldStar(int worldId);

private:
	vector<AdConfData> m_adConfData;
    map<string, string> m_textData;
    vector<WorldData> m_worldData;
    map<int, vector<LevelData>> m_levelData;
};

#endif // DATA_MGR
