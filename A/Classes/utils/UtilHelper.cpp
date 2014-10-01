#include "UtilHelper.h"
#include "StringConst.h"
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
    //��������java���еİ���+���������»�������
    void Java_com_game_infinite_racing_AppActivity_rate(JNIEnv*  env, jobject thiz, jboolean a)
    {
        UtilHelper::writeToBool(RATE, a);
    }
}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    //��������java���еİ���+���������»�������
    void Java_com_game_infinite_racing_AppActivity_quit(JNIEnv*  env, jobject thiz, jboolean a)
    {
        umeng::MobClickCpp::end();
        Director::getInstance()->end();
    }
}
#endif


vector<AdConfData> UtilHelper::readAdConfData()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("ad.json");
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
            adConfData.push_back(adData);
        }
    }
    return adConfData;
}

ConfData UtilHelper::getConfDataFromNet(string response)
{
    ConfData netConfData;
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(response.c_str());
    //�ж϶�ȡ�ɹ���� �� �Ƿ�Ϊ��������  
    if (doc.HasParseError() || !doc.IsArray())  
    {  
       // log("get json data err! %s",response.c_str()); 
        return netConfData;
    }

    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  
        AdConfData adData;

        //���±���ȡ  
        int a=0;
        netConfData.airpush = v[a++].GetInt();
        netConfData.startapp = v[a++].GetInt();
        netConfData.ad_version = v[a++].GetInt();
    }
    return netConfData;
}

void UtilHelper::showStartAppAd(int type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "com/game/infinite/racing/AppActivity", "showStartAppAd", "(I)V"))
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
    if(JniHelper::getStaticMethodInfo(t, "com/game/infinite/racing/AppActivity", "showOwnAds", "(ILjava/lang/String;)V"))
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


bool UtilHelper::screenShoot()
{
    Size winSize = Director::getInstance()->getWinSize();

    //����һ����Ļ��С����Ⱦ����
    RenderTexture* renderTexture = RenderTexture::create(winSize.width, winSize.height, Texture2D::PixelFormat::RGBA8888);

    Scene* curScene = Director::getInstance()->getRunningScene();
    //Point ancPos = curScene->getAnchorPoint();

    //��Ⱦ���?ʼ��׽
    renderTexture->begin();

    // ��С��Ļ��������
    //curScene->setScale(0.5f);
    //curScene->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    //���Ƶ�ǰ����
    curScene->visit();

    //����
    renderTexture->end();

    //����png
    renderTexture->saveToFile("screenshoot.png", Image::Format::PNG);

    // �ָ���Ļ�ߴ�
    //curScene->setScale(1.0f);
    //curScene->setAnchorPoint(ancPos);

    return true;
}

bool UtilHelper::writeMapDataToSD()
{
    auto sdpath = FileUtils::getInstance()->getWritablePath();
    //write map data
    sdpath.append("map.json");
    string filename = "data/map.json";
    auto path = FileUtils::getInstance()->fullPathForFilename(filename);
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


bool UtilHelper::writeDailyTaskDataToSD()
{
    auto sdpath = FileUtils::getInstance()->getWritablePath();
    //write map data
    sdpath.append("dailyTask.json");
    string filename = "data/dailyTask.json";
    auto path = FileUtils::getInstance()->fullPathForFilename(filename);
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

bool UtilHelper::writeAchievementDataToSD()
{
    auto sdpath = FileUtils::getInstance()->getWritablePath();
    //write map data
    sdpath.append("achievement.json");
    string filename = "data/achievement.json";
    auto path = FileUtils::getInstance()->fullPathForFilename(filename);
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

bool UtilHelper::writeCarDataToSD()
{
    auto sdpath = FileUtils::getInstance()->getWritablePath();
    //write map data
    sdpath.append("car.json");
    string filename = "data/car.json";
    auto path = FileUtils::getInstance()->fullPathForFilename(filename);
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

map<string, string> UtilHelper::readTextData()
{
    string filename = "data/text.json";
    auto path = FileUtils::getInstance()->fullPathForFilename(filename);
    rapidjson::Document doc;
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

    map<string, string> textData;
    textData.clear();
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
    return textData;
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

bool UtilHelper::writeDailyTaskData(int taskid)
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("dailyTask.json");
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
        if (i == taskid + 1)
        {
            doc[i][3].SetInt(1);
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

bool UtilHelper::writeAchievementData(int taskid)
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("achievement.json");
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
        if (i == taskid + 1)
        {
            doc[i][3].SetInt(1);
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

vector<DailyTask> UtilHelper::readDailyTaskData()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("dailyTask.json");
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

    vector<DailyTask> dailyTasks;
    dailyTasks.clear();

    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  
        int a = 0;
        DailyTask dailyTask;
        dailyTask.conis = v[a++].GetInt();
        dailyTask.type = v[a++].GetInt();
        dailyTask.target = v[a++].GetInt();
        dailyTask.receive = v[a++].GetInt();
        dailyTasks.push_back(dailyTask);
    }
    return dailyTasks;
}

vector<Achievement> UtilHelper::readAchievementData()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("achievement.json");
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

    vector<Achievement> achievements;
    achievements.clear();

    //�ӵ�2�п�ʼ����Ϊ��һ��������  
    for(unsigned int i=1;i<doc.Size();i++)
    {  
        //�����ȡ����Ԫ�أ������ı�������Ϊ���ã�  
        rapidjson::Value &v=doc[i];  
        int a = 0;
        Achievement achievement;
        achievement.conis = v[a++].GetInt();
        achievement.type = v[a++].GetInt();
        achievement.target = v[a++].GetInt();
        achievement.receive = v[a++].GetInt();
        achievements.push_back(achievement);
    }
    return achievements;
}

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

string UtilHelper::getResetTimes()
{
    string resetTime = "Reset in ";
     int day = 0;
     int hour = 0;
     int minute = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    struct timeval now;
    gettimeofday(&now, NULL);
    struct tm *tm;
    time_t tp = now.tv_sec;
    tm = localtime(&tp);
    day = tm->tm_mday;
    hour = tm->tm_hour;
    minute = tm->tm_min;
#endif
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    struct tm *tm;  
    time_t timep;  
    time(&timep);  
    tm = localtime(&timep);
    //int year = tm->tm_year + 1900;  
    //int month = tm->tm_mon + 1;  
     day = tm->tm_mday;
     hour = tm->tm_hour;
     minute = tm->tm_min;

    //int second=tm->tm_sec;
    /*char time[64] = { 0 };
    sprintf(time, "%d-%02d-%02d %02d:%02d:%02d", (int) tm->tm_year + 1900,
        (int) tm->tm_mon + 1, (int) tm->tm_mday, (int) tm->tm_hour,
        (int) tm->tm_min, (int) tm->tm_sec);*/
#endif
    if(UtilHelper::getFromInteger(NOW_DAY) != day)
    {
        UtilHelper::writeToInteger(NOW_DAY, day);
        UtilHelper::writeToInteger(TODAY_COUNT, 0);
        UtilHelper::writeToInteger(TODAY_DISTANCE, 0);
        UtilHelper::writeDailyTaskDataToSD();
    }
    
    if (hour != 24)
    {
        resetTime += String::createWithFormat("%d",23 - hour)->_string + " hours";
    }
    else
    {
        resetTime += String::createWithFormat("%d",59 - minute)->_string + " mins";
    }
    return resetTime;
}
