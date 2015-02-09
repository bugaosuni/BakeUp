#include "UtilHelper.h"
#include "StringConst.h"
#include "DataMgr.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#include <jni.h>  
#include "platform/android/jni/JniHelper.h"  
#include <android/log.h>  
#include "MobClickCpp.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    //��������java���еİ���+�����������»�������
    void Java_com_gamefunny_mountain_racing_AppActivity_rate(JNIEnv*  env, jobject thiz, jboolean a)
    {
        UtilHelper::writeToBool(RATE, a);
    }
}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    //��������java���еİ���+�����������»�������
    void Java_com_gamefunny_mountain_racing_AppActivity_quit(JNIEnv*  env, jobject thiz, jboolean a)
    {
        umeng::MobClickCpp::end();
        Director::getInstance()->end();
    }
}
#endif

void UtilHelper::updateAdConf(string response)
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("adConf.json");
    FILE* file = fopen(path.c_str(), "wb");
    if (file)
    {
        fputs(response.c_str(), file);
        fclose(file);
    }
}

vector<AdConfData> UtilHelper::readAdConfData()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("adConf.json");
    rapidjson::Document doc;
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(path));
    {
        log("json file is not find [%s]",path.c_str());  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(path);
    vector<AdConfData> adConfData;
    adConfData.clear();
    if (data != "")
    {
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
        if (doc.HasParseError() || !doc.IsArray())  
        {  
            log("get json data err!"); 
        }

        //�ӵ�2�п�ʼ����Ϊ��һ��������  
        for(unsigned int i=1;i<doc.Size();i++)
        {  
            //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
            rapidjson::Value &v=doc[i];  
            AdConfData adData;

            //���±���ȡ  
            int a=0;
            adData.package = v[a++].GetString();
            adData.icon = v[a++].GetString();
            adData.version = v[a++].GetInt();
            adConfData.push_back(adData);
        }
    }
    return adConfData;
}

vector<AdConfData> UtilHelper::getAdConfDataFromNet(string response)
{
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(response.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!"); 
        //return ;
    }

    vector<AdConfData> adConfData;
    adConfData.clear();
    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  
        AdConfData adData;

        //���±���ȡ  
        int a=0;
        adData.package = v[a++].GetString();
        adData.icon = v[a++].GetString();
        adData.version = v[a++].GetInt();
        adConfData.push_back(adData);
    }
    return adConfData;
}

void UtilHelper::showStartAppAd(int type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "com/gamefunny/mountain/racing/AppActivity", "showStartAppAd", "(I)V"))
    {

        t.env->CallStaticVoidMethod(t.classID, t.methodID, type);
        if ( t.env->ExceptionOccurred() )
        {
            t.env->ExceptionDescribe();
            t.env->ExceptionClear();
            return;
        }
        t.env->DeleteLocalRef(t.classID);

    }
#endif
}

void UtilHelper::showOwnAds(int type, string pkg)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	umeng::MobClickCpp::event("click_ad", pkg.c_str());

    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "com/gamefunny/mountain/racing/AppActivity", "showOwnAds", "(ILjava/lang/String;)V"))
    {
        jstring jmsg = t.env->NewStringUTF(pkg.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, type, jmsg);
        if ( t.env->ExceptionOccurred() )
        {
            t.env->ExceptionDescribe();
            t.env->ExceptionClear();
            return;
        }
        t.env->DeleteLocalRef(t.classID);

    }
#endif
}

bool UtilHelper::isFileExist(string path)
{
	/*
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (!FileUtilsAndroid::getInstance()->isFileExistInternal(path))
	{
		log("json file is not find [%s]",path.c_str());  
		return false;  
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (!FileUtilsWin32::getInstance()->isFileExistInternal(path))
	{
		log("json file is not find [%s]",path.c_str());  
		return false;  
	}
#endif
	*/
	return true;
}

bool UtilHelper::writePkgDataToSD(string filename)
{
	auto sdpath = FileUtils::getInstance()->getWritablePath();
	sdpath.append(filename);
	string _filename = "data/" + filename;
	auto path = FileUtils::getInstance()->fullPathForFilename(_filename);
	rapidjson::Document doc;
	doc.SetObject();
	//�ж��ļ��Ƿ����
	if (!isFileExist(path))
	{
		return false;
	}
	//��ȡ�ļ�����ʼ��doc
	string data = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	//�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
	if (doc.HasParseError() || !doc.IsArray())  
	{  
		log("get json data err!");
		return false;
	}
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	FILE* file = fopen(sdpath.c_str(), "wb");
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
		return true;
	}
	return false;
}

vector<WorldData> UtilHelper::readWorldData()
{
	auto path = FileUtils::getInstance()->getWritablePath();
	path.append("world.json");
	rapidjson::Document doc;
	vector<WorldData> worldData;
	worldData.clear();
	//�ж��ļ��Ƿ����
	if (isFileExist(path))
	{
		//��ȡ�ļ�����ʼ��doc
		string data = FileUtils::getInstance()->getStringFromFile(path);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		//�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
		if (doc.HasParseError() || !doc.IsArray())  
		{  
			log("get json data err!"); 
		}
		else
		{
			//�ӵ�2�п�ʼ����Ϊ��һ��������  
			for(unsigned int i=1;i<doc.Size();i++)
			{  
				//�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
				rapidjson::Value &v=doc[i];  

				WorldData data;
				//���±���ȡ  
				int a=0;  
				data.id=v[a++].GetInt();  
				data.name=v[a++].GetString();   
				data.icon=v[a++].GetString();
				data.open=v[a++].GetInt();  

				worldData.push_back(data);
			}
		}
	}
	return worldData;
}

bool UtilHelper::writeWorldData(int worldId, WorldModType modType, int modData)
{
	auto path = FileUtils::getInstance()->getWritablePath();
	path.append("world.json");
	rapidjson::Document doc;
	doc.SetObject();
	//�ж��ļ��Ƿ����
	if (!isFileExist(path))
	{
		return false;  
	}
	//��ȡ�ļ�����ʼ��doc
	string data = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	//�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
	if (doc.HasParseError() || !doc.IsArray())  
	{  
		log("get json data err!");  
	}
	else
	{
		//�ӵ�2�п�ʼ����Ϊ��һ��������  
		for(unsigned int i=1;i<doc.Size();i++)
		{  
			//���±���ȡ  
			int a=0;  
			auto id=doc[i][a++].GetInt();  
			if (id == worldId)
			{
				doc[i][modType].SetInt(modData);
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				doc.Accept(writer);
				FILE* file = fopen(path.c_str(), "wb");
				if (file)
				{
					fputs(buffer.GetString(), file);
					fclose(file);
					return true;
				}
			}
		}
	}
	return false;
}

map<int, vector<LevelData>> UtilHelper::readLevelData()
{
	auto path = FileUtils::getInstance()->getWritablePath();
	path.append("level.json");
	rapidjson::Document doc;
	map<int, vector<LevelData>> levelData;
	levelData.clear();
	//�ж��ļ��Ƿ����
	if (isFileExist(path))
	{
		//��ȡ�ļ�����ʼ��doc
		string data = FileUtils::getInstance()->getStringFromFile(path);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		//�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
		vector<LevelData> levelVec;
		levelVec.clear();
		if (doc.HasParseError() || !doc.IsArray())  
		{  
			log("get json data err!"); 
		}
		else
		{
			auto worldId = -1;
			DataMgr::allLevelCount = doc.Size() - 1;
			//�ӵ�2�п�ʼ����Ϊ��һ��������  
			for(unsigned int i=1;i<doc.Size();i++)
			{  
				//�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
				rapidjson::Value &v=doc[i]; 

				LevelData data;

				//���±���ȡ  
				int a=0;  
				data.id=v[a++].GetInt();
				data.world_id=v[a++].GetInt();
				data.level_id=v[a++].GetInt();
				data.icon=v[a++].GetString();
				data.open=v[a++].GetInt();
				data.star=v[a++].GetInt();

				if (data.world_id != worldId)//new
				{
					if (i > 1)
					{
						levelData[worldId] = levelVec;//save pre
					}
					worldId = data.world_id;
					levelVec.clear();
					levelVec.push_back(data);
				}
				else//old
				{
					levelVec.push_back(data);
				}
				if (i == doc.Size() - 1)//last line
				{
					levelData[worldId] = levelVec;//save curr
				}
			}
		}
	}
	return levelData;
}

bool UtilHelper::writeLevelData(int worldId, int levelId, WorldModType modType, int modData)
{
	auto path = FileUtils::getInstance()->getWritablePath();
	path.append("level.json");
	rapidjson::Document doc;
	doc.SetObject();
	//�ж��ļ��Ƿ����
	if (!isFileExist(path))
	{
		return false;  
	}
	//��ȡ�ļ�����ʼ��doc
	string data = FileUtils::getInstance()->getStringFromFile(path);
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	//�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
	if (doc.HasParseError() || !doc.IsArray())  
	{  
		log("get json data err!");  
	}
	else
	{
		//�ӵ�2�п�ʼ����Ϊ��һ��������  
		for(unsigned int i=1;i<doc.Size();i++)
		{  
			//���±���ȡ  
			int a=0;  
			auto id=doc[i][a++].GetInt(); 
			auto worldid=doc[i][a++].GetInt(); 
			auto levelid=doc[i][a++].GetInt(); 
			if (worldid == worldId && levelid == levelId)
			{
				doc[i][modType].SetInt(modData);
				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				doc.Accept(writer);
				FILE* file = fopen(path.c_str(), "wb");
				if (file)
				{
					fputs(buffer.GetString(), file);
					fclose(file);
					return true;
				}
			}
		}
	}
	return false;
}

map<string, string> UtilHelper::readTextData()
{
	string filename = "data/text.json";
	auto path = FileUtils::getInstance()->fullPathForFilename(filename);
	rapidjson::Document doc;
	map<string, string> textData;
	textData.clear();
	//�ж��ļ��Ƿ����
	if (isFileExist(path))
	{
		//��ȡ�ļ�����ʼ��doc
		string data = FileUtils::getInstance()->getStringFromFile(path);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		//�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
		if (doc.HasParseError() || !doc.IsArray())  
		{  
			log("get json data err!");  
		}
		else
		{
			//�ӵ�2�п�ʼ����Ϊ��һ��������  
			for(unsigned int i=1;i<doc.Size();i++)
			{  
				//�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
				rapidjson::Value &v=doc[i];  

				//���±���ȡ  
				int a=0;
				auto id = v[a++].GetInt();
				textData[v[a++].GetString()] = v[a++].GetString();
			}
		}
	}
	return textData;
}

/*
bool UtilHelper::updateMapDate()
{
    auto sdcardPath = FileUtils::getInstance()->getWritablePath();
    sdcardPath.append("map.json");

    rapidjson::Document oldDoc;
    oldDoc.SetObject();
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(sdcardPath))
    {
        log("json file is not find [%s]",sdcardPath.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string oldData = FileUtils::getInstance()->getStringFromFile(sdcardPath);
    oldDoc.Parse<rapidjson::kParseDefaultFlags>(oldData.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (oldDoc.HasParseError() || !oldDoc.IsArray())  
    {  
        log("get json data err!");  
    }

    string filename = "data/map.json";
    auto assertPath = FileUtils::getInstance()->fullPathForFilename(filename);

    rapidjson::Document newDoc;
    newDoc.SetObject();
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(assertPath))
    {
        log("json file is not find [%s]",assertPath.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string newData = FileUtils::getInstance()->getStringFromFile(assertPath);
    newDoc.Parse<rapidjson::kParseDefaultFlags>(newData.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (newDoc.HasParseError() || !newDoc.IsArray())  
    {  
        log("get json data err!");  
    }

    int idIndex = 0;


     //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<newDoc.Size();i++)
    {
        for(unsigned int j=1;j<oldDoc.Size();j++)
        {
            if (newDoc[i][idIndex].GetInt() == oldDoc[j][idIndex].GetInt())
            {
                newDoc[i][e_map_open].SetInt( oldDoc[j][e_map_open].GetInt());
                newDoc[i][e_bestDist].SetInt( oldDoc[j][e_bestDist].GetInt());
                break;
            }
        }
    }

    rapidjson::StringBuffer buffer;
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    newDoc.Accept(writer);
    FILE* file = fopen(sdcardPath.c_str(), "wb");
    if (file)
    {
        fputs(buffer.GetString(), file);
        fclose(file);
        return true;
    }
    return false;
}

bool UtilHelper::updateCarDate()
{
    auto sdcardPath = FileUtils::getInstance()->getWritablePath();
    sdcardPath.append("car.json");

    rapidjson::Document oldDoc;
    oldDoc.SetObject();
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(sdcardPath))
    {
        log("json file is not find [%s]",sdcardPath.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string oldData = FileUtils::getInstance()->getStringFromFile(sdcardPath);
    oldDoc.Parse<rapidjson::kParseDefaultFlags>(oldData.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (oldDoc.HasParseError() || !oldDoc.IsArray())  
    {  
        log("get json data err!");  
    }

    string filename = "data/car.json";
    auto assertPath = FileUtils::getInstance()->fullPathForFilename(filename);

    rapidjson::Document newDoc;
    newDoc.SetObject();
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(assertPath))
    {
        log("json file is not find [%s]",assertPath.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string newData = FileUtils::getInstance()->getStringFromFile(assertPath);
    newDoc.Parse<rapidjson::kParseDefaultFlags>(newData.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (newDoc.HasParseError() || !newDoc.IsArray())  
    {  
        log("get json data err!");  
    }

    int idIndex = 0;


     //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<newDoc.Size();i++)
    {
        for(unsigned int j=1;j<oldDoc.Size();j++)
        {
            if (newDoc[i][idIndex].GetInt() == oldDoc[j][idIndex].GetInt())
            {
                newDoc[i][e_car_open].SetInt( oldDoc[j][e_car_open].GetInt());
                newDoc[i][e_part1_level].SetInt( oldDoc[j][e_part1_level].GetInt());
                newDoc[i][e_part2_level].SetInt( oldDoc[j][e_part2_level].GetInt());
                newDoc[i][e_part3_level].SetInt( oldDoc[j][e_part3_level].GetInt());
                newDoc[i][e_part4_level].SetInt( oldDoc[j][e_part4_level].GetInt());
                break;
            }
        }
    }

    rapidjson::StringBuffer buffer;
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    newDoc.Accept(writer);
    FILE* file = fopen(sdcardPath.c_str(), "wb");
    if (file)
    {
        fputs(buffer.GetString(), file);
        fclose(file);
        return true;
    }
    return false;
}

vector<MapData> UtilHelper::readMapData()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("map.json");
    rapidjson::Document doc;
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(path))
    {
        log("json file is not find [%s]",path.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(path);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!");  
    }

    vector<MapData> mapData;
    mapData.clear();
    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  
   
        MapData data;
        //���±���ȡ  
        int a=0;  
        data.id=v[a++].GetInt();  
        data.name=v[a++].GetString();   
        data.image=v[a++].GetString();
        data.open=v[a++].GetInt();  
        data.level=v[a++].GetInt();  
        data.bestDist=v[a++].GetInt();  
        data.cost=v[a++].GetInt();  
   
        mapData.push_back(data);
    }
    return mapData;
}
bool UtilHelper::writeMapData(int mapid, MapModType modType, int modData)
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("map.json");
    rapidjson::Document doc;
    doc.SetObject();
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(path))
    {
        log("json file is not find [%s]",path.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(path);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!");  
    }
    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
         //���±���ȡ  
         int a=0;  
         auto id=doc[i][a++].GetInt();  
         if (id == mapid)
         {
             doc[i][modType].SetInt(modData);
             rapidjson::StringBuffer buffer;
             rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
             doc.Accept(writer);
             FILE* file = fopen(path.c_str(), "wb");
             if (file)
             {
                 fputs(buffer.GetString(), file);
                 fclose(file);
                 return true;
             }
         }
    }
    return false;
}
bool UtilHelper::writeCarData(int carid, CarModType modType, int modData)
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("car.json");
    rapidjson::Document doc;
    doc.SetObject();
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(path));
    {
        log("json file is not find [%s]",path.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(path);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!");  
    }

    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //���±���ȡ  
        int a=0;  
        auto id=doc[i][a++].GetInt();  
        if (id == carid)
        {
            doc[i][modType].SetInt(modData);
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            doc.Accept(writer);

            FILE* file = fopen(path.c_str(), "wb");
            if (file)
            {
                fputs(buffer.GetString(), file);
                fclose(file);
                return true;
            }
        }
    }
    return false;
}
vector<CarData> UtilHelper::readCarData()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("car.json");
    rapidjson::Document doc;
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(path))
    {
        log("json file is not find [%s]",path.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(path);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!");  
    }

    vector<CarData> carData;
    carData.clear();
    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  

        CarData data;
        //���±���ȡ  
        int a=0;  
        data.id=v[a++].GetInt();  
        data.name=v[a++].GetString();   
        data.image=v[a++].GetString();
        data.open=v[a++].GetInt(); 
        data.cost=v[a++].GetInt();
        data.part1_name=v[a++].GetString();
        data.part1_maxlevel=v[a++].GetInt(); 
        data.part1_level=v[a++].GetInt();
        data.part2_name=v[a++].GetString();
        data.part2_maxlevel=v[a++].GetInt(); 
        data.part2_level=v[a++].GetInt();
        data.part3_name=v[a++].GetString();
        data.part3_maxlevel=v[a++].GetInt(); 
        data.part3_level=v[a++].GetInt();
        data.part4_name=v[a++].GetString();
        data.part4_maxlevel=v[a++].GetInt(); 
        data.part4_level=v[a++].GetInt();

        carData.push_back(data);
    }
    return carData;
}

map<int, vector<PartCost>> UtilHelper::readPartCostData()
{
    string filename = "data/partCost.json";
    auto path = FileUtils::getInstance()->fullPathForFilename(filename);
    rapidjson::Document doc;
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(path))
    {
        log("json file is not find [%s]",path.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(path);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!");  
    }

    map<int, vector<PartCost>> partCostMap;
    partCostMap.clear();
    vector<PartCost> partCostVec;
    partCostVec.clear();
    auto carId = -1;
    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i]; 

        PartCost data;

        //���±���ȡ  
        int a=0;
        data.id = v[a++].GetInt();
        data.level = v[a++].GetInt();
        data.car_id = v[a++].GetInt();
        data.part1_upgrade_cost = v[a++].GetInt();
        data.part2_upgrade_cost = v[a++].GetInt();
        data.part3_upgrade_cost = v[a++].GetInt();
        data.part4_upgrade_cost = v[a++].GetInt();

        if (data.car_id != carId)//new
        {
            if (i > 1)
            {
                partCostMap[carId] = partCostVec;//save pre
            }
            carId = data.car_id;
            partCostVec.clear();
            partCostVec.push_back(data);
        }
        else//old
        {
            partCostVec.push_back(data);
        }
        if (i == doc.Size() - 1)//last line
        {
            partCostMap[carId] = partCostVec;//save curr
        }
    }
    return partCostMap;
}

vector<MapPoints> UtilHelper::readMapPointsData(int mapId)
{
    string filename = "data/mapPoints" + String::createWithFormat("%d",mapId)->_string +".json";
    auto path = FileUtils::getInstance()->fullPathForFilename(filename);
    rapidjson::Document doc;
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(path))
    {
        log("json file is not find [%s]",path.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(path);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!");  
    }

    vector<MapPoints> mapPoints;
    mapPoints.clear();

    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  
        int a = 0;
        MapPoints mapPoint;
        mapPoint.x = v[a++].GetInt();
        mapPoint.y = v[a++].GetInt();
        mapPoints.push_back(mapPoint);
    }
    return mapPoints;
}
 
vector<CarInfo> UtilHelper::readCarInfoData()
{
    string filename = "data/carInfo.json";
    rapidjson::Document doc;
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(filename))
    {
        log("json file is not find [%s]",filename.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(filename);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!");  
    }

    vector<CarInfo> carInfos;
    carInfos.clear();

    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  
        int a = 0;
        CarInfo carInfo;
        carInfo.car_id = v[a++].GetInt();

        carInfo.wheel1_x = v[a++].GetDouble();
        carInfo.wheel1_y = v[a++].GetDouble();
        carInfo.wheel2_x = v[a++].GetDouble();
        carInfo.wheel2_y = v[a++].GetDouble();

        carInfo.driver_body_x = v[a++].GetDouble();
        carInfo.driver_body_y = v[a++].GetDouble();
        carInfo.driver_head_x = v[a++].GetDouble();
        carInfo.driver_head_y = v[a++].GetDouble();

        carInfo.linearDamping = v[a++].GetDouble();
        carInfo.wheel_density = v[a++].GetDouble();
        carInfo.wheel_friction = v[a++].GetDouble();
        carInfo.wheel_restitution = v[a++].GetDouble();

        carInfo.wheel1_motorSpeed = v[a++].GetDouble();
        carInfo.wheel2_motorSpeed = v[a++].GetDouble();
        carInfo.wheel1_maxMotorTorque = v[a++].GetDouble();
        carInfo.wheel2_maxMotorTorque = v[a++].GetDouble();

        carInfo.wheel1_enableMotor = v[a++].GetBool();
        carInfo.wheel2_enableMotor = v[a++].GetBool();
        
        carInfo.wheel1_frequencyHz = v[a++].GetDouble();
        carInfo.wheel2_frequencyHz = v[a++].GetDouble();
        carInfo.wheel1_dampingRatio = v[a++].GetDouble();
        carInfo.wheel2_dampingRatio = v[a++].GetDouble();

        carInfo.antenna_x = v[a++].GetDouble();
        carInfo.antenna_y = v[a++].GetDouble();

        carInfos.push_back(carInfo);
    }

    return carInfos;
}

vector<CarLevel> UtilHelper::readCarLevelData()
{
    
    string filename = "data/carLevel.json";
    rapidjson::Document doc;
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(filename))
    {
        log("json file is not find [%s]",filename.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(filename);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!");  
    }

    vector<CarLevel> carLevels;
    carLevels.clear();

    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  
        int a = 0;
        CarLevel carLevel;
        carLevel.car_id = v[a++].GetInt();

        carLevel.rear_speed1 = v[a++].GetDouble();
        carLevel.rear_speed2 = v[a++].GetDouble();
        carLevel.rear_speed3 = v[a++].GetDouble();
        carLevel.rear_speed4 = v[a++].GetDouble();
        carLevel.rear_speed5 = v[a++].GetDouble();

        carLevel.suspension_level1 = v[a++].GetDouble();
        carLevel.suspension_level2 = v[a++].GetDouble();
        carLevel.suspension_level3 = v[a++].GetDouble();
        carLevel.suspension_level4 = v[a++].GetDouble();
        carLevel.suspension_level5 = v[a++].GetDouble();

        carLevel.wheel_friction1 = v[a++].GetDouble();
        carLevel.wheel_friction2 = v[a++].GetDouble();
        carLevel.wheel_friction3 = v[a++].GetDouble();
        carLevel.wheel_friction4 = v[a++].GetDouble();
        carLevel.wheel_friction5 = v[a++].GetDouble();

        carLevel.front_speed1 = v[a++].GetDouble();
        carLevel.front_speed2 = v[a++].GetDouble();
        carLevel.front_speed3 = v[a++].GetDouble();
        carLevel.front_speed4 = v[a++].GetDouble();
        carLevel.front_speed5 = v[a++].GetDouble();


        carLevels.push_back(carLevel);
    }

    return carLevels;
}

FileVersion UtilHelper::readFileVersionData()
{
    string filename = "data/fileVersion.json";
    rapidjson::Document doc;
    //�ж��ļ��Ƿ����
    if (!FileUtils::getInstance()->isFileExist(filename))
    {
        log("json file is not find [%s]",filename.c_str());  
        //return false;  
    }
    //��ȡ�ļ�����ʼ��doc
    string data = FileUtils::getInstance()->getStringFromFile(filename);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
        log("get json data err!");  
    }

    FileVersion fileVersion;
    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  
        int a = 0;
        fileVersion.map_version = v[a++].GetInt();
        fileVersion.car_version = v[a++].GetInt();

    }
    return fileVersion;
}
*/
void UtilHelper::writeToString(const char* key, string value)
{
    UserDefault::getInstance()->setStringForKey(key, value);
    flushData();
}
void UtilHelper::writeToInteger(const char* key, int value)
{
    UserDefault::getInstance()->setIntegerForKey(key, value);
    flushData();
}
void UtilHelper::writeToFloat(const char* key, float value)
{
    UserDefault::getInstance()->setFloatForKey(key, value);
    flushData();
}
void UtilHelper::writeToDouble(const char* key, double value)
{
    UserDefault::getInstance()->setDoubleForKey(key, value);
    flushData();
}
void UtilHelper::writeToBool(const char* key, bool value)
{
    UserDefault::getInstance()->setBoolForKey(key, value);
    flushData();
}
string UtilHelper::getFromString(const char* key)
{
    return UserDefault::getInstance()->getStringForKey(key);
}
int UtilHelper::getFromInteger(const char* key)
{
    return UserDefault::getInstance()->getIntegerForKey(key);
}
float UtilHelper::getFromFloat(const char* key)
{
    return UserDefault::getInstance()->getFloatForKey(key);
}
double UtilHelper::getFromDouble(const char* key)
{
    return UserDefault::getInstance()->getDoubleForKey(key);
}
bool UtilHelper::getFromBool(const char* key)
{
    return UserDefault::getInstance()->getBoolForKey(key);
}
void UtilHelper::flushData()
{
    UserDefault::getInstance()->flush();
}