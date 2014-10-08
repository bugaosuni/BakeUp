#ifndef __UTILHELPER_H__
#define __UTILHELPER_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum WorldModType
{
    e_world_open	= 3
};

enum LevelModType
{
	e_level_open	= 4,
	e_level_star	= 5
};


struct WorldData
{
	int			id;
	string		name;
	string		icon;
	int			open;
};

struct LevelData
{
	int			id;
	int			world_id;
	int			level_id;
	string		icon;
	int			open;
	int			star;
};
/*
struct MapData
{
    int			id;
    string		name;
    string		image;
    int			open;
    int			level;
    int			bestDist;
    int			cost;
    MapData()
    {
        name = "map1";
        image = "map1.png";
        open = 0;
        level = 1;
        bestDist = 0;
        cost = 0;
    }
};

enum CarModType
{
    e_car_open		= 3,
    e_part1_level	= 7,
    e_part2_level	= 10,
    e_part3_level	= 13,
    e_part4_level	= 16
};

struct CarData
{
    int			id;
    string		name;
    string		image;
    int			open;
    int			cost;
    string		part1_name;
    int			part1_maxlevel;
    int			part1_level;
    string		part2_name;
    int			part2_maxlevel;
    int			part2_level;
    string		part3_name;
    int			part3_maxlevel;
    int			part3_level;
    string		part4_name;
    int			part4_maxlevel;
    int			part4_level;
    CarData()
    {
        name = "car1";
        image = "car1.png";
        open = 0;
        cost = 0;
    }
};

struct PartCost
{
    int			id;
    int			level;
    int			car_id;
    int			part1_upgrade_cost;
    int			part2_upgrade_cost;
    int			part3_upgrade_cost;
    int			part4_upgrade_cost;
};

struct MapPoints
{
    int x;
    int y;
    MapPoints()
    {
        x = 200;
        y = 200;
    }
};

struct CarInfo
{
    int car_id;

    float wheel1_x;
    float wheel1_y;
    float wheel2_x;
    float wheel2_y;

    float driver_body_x;
    float driver_body_y;
    float driver_head_x;
    float driver_head_y;

    float linearDamping; // 空气阻力
    float wheel_density; // 密度
    float wheel_friction; // 摩擦力
    float wheel_restitution; // 弹性

    float wheel1_motorSpeed; //车轮初始速度
    float wheel2_motorSpeed;
    float wheel1_maxMotorTorque; // 车轮最大动力
    float wheel2_maxMotorTorque;

    bool wheel1_enableMotor; // 是否可以驱动
    bool wheel2_enableMotor;

    float wheel1_frequencyHz; //频率 
    float wheel2_frequencyHz;
    float wheel1_dampingRatio; // 减震比率 1没有震动
    float wheel2_dampingRatio;

    float antenna_x; //天线坐标
    float antenna_y;


    
    CarInfo()
    {
        car_id = 0;

        wheel1_x = 0;
        wheel1_y = 0;
        wheel2_x = 0;
        wheel2_y = 0;

        driver_body_x = 0;
        driver_body_y = 0;
        driver_head_x = 0;
        driver_head_y = 0;
        
        linearDamping = 0.2;
        wheel_density = 2;
        wheel_friction = 1;
        wheel_restitution = 0;
        
        wheel1_motorSpeed = 0;
        wheel2_motorSpeed = 0;
        wheel1_maxMotorTorque = 50;
        wheel2_maxMotorTorque = 10;

        wheel1_enableMotor = true;
        wheel2_enableMotor = false;
        
        wheel1_frequencyHz = 4;
        wheel2_frequencyHz = 4;
        wheel1_dampingRatio = 0.7;
        wheel2_dampingRatio = 0.7;

        antenna_x = 0;
        antenna_y =0;
    }
};

struct CarLevel
{
    int car_id;
    
    float rear_speed1;
    float rear_speed2;
    float rear_speed3;
    float rear_speed4;
    float rear_speed5;

    float suspension_level1;
    float suspension_level2;
    float suspension_level3;
    float suspension_level4;
    float suspension_level5;
    
    float wheel_friction1;
    float wheel_friction2;
    float wheel_friction3;
    float wheel_friction4;
    float wheel_friction5;

    float front_speed1;
    float front_speed2;
    float front_speed3;
    float front_speed4;
    float front_speed5;
    
    CarLevel()
    {
        car_id = 0;

        rear_speed1 = 0.0f;
        rear_speed2 = 0.0f;
        rear_speed3 = 0.0f;
        rear_speed4 = 0.0f;
        rear_speed5 = 0.0f;

        suspension_level1 = 0.0f;
        suspension_level2 = 0.0f;
        suspension_level3 = 0.0f;
        suspension_level4 = 0.0f;
        suspension_level5 = 0.0f;

        wheel_friction1 = 0.0f;
        wheel_friction2 = 0.0f;
        wheel_friction3 = 0.0f;
        wheel_friction4 = 0.0f;
        wheel_friction5 = 0.0f;

        front_speed1 = 0.0f;
        front_speed2 = 0.0f;
        front_speed3 = 0.0f;
        front_speed4 = 0.0f;
        front_speed5 = 0.0f;
    }
};

struct FileVersion
{
    int		map_version;
    int		car_version;

    FileVersion()
    {
        map_version = 0;
        car_version = 0;
    }
};

*/
struct AdConfData
{
    string		package;
    string		icon;
    int			version;
    AdConfData()
    {
        package = "";
        icon = "icon1.png";
        version = 1;
    }
};
class UtilHelper
{
public:
	static void updateAdConf(string response);
	static vector<AdConfData> readAdConfData();
	static vector<AdConfData> getAdConfDataFromNet(string response);
	static void showStartAppAd(int type);
	static void showOwnAds(int type, string pkg);
	static bool isFileExist(string path);
	static bool writePkgDataToSD(string filename);
	static vector<WorldData> readWorldData();
	static bool writeWorldData(int worldId, WorldModType modType, int modData);
	static map<int, vector<LevelData>> readLevelData();
	static bool writeLevelData(int worldId, int levelId, WorldModType modType, int modData);
	static map<string, string> readTextData();
	/*
    static vector<MapData> readMapData();
    static bool writeMapData(int mapid, MapModType modType, int modData);
    static bool writeCarData(int carid, CarModType modType, int modData);
    static vector<CarData> readCarData();
    static map<int, vector<PartCost>> readPartCostData();
    static vector<MapPoints> readMapPointsData(int mapId);
    static vector<CarInfo> readCarInfoData();
    static vector<CarLevel> readCarLevelData();
    static FileVersion readFileVersionData();
    static bool updateMapDate();
    static bool updateCarDate();*/
    static void writeToString(const char* key, string value);
    static void writeToInteger(const char* key, int value);
    static void writeToFloat(const char* key, float value);
    static void writeToDouble(const char* key, double value);
    static void writeToBool(const char* key, bool value);
    static string getFromString(const char* key);
    static int getFromInteger(const char* key);
    static float getFromFloat(const char* key);
    static double getFromDouble(const char* key);
    static bool getFromBool(const char* key);
    static void flushData();
};

#endif /* __UTILHELPER_H__ */
