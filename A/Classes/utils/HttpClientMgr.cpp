#include "HttpClientMgr.h"
#include "DataMgr.h"
#include <string>
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::network;

static HttpClientMgr *s_pHttpClientMgr = nullptr;

HttpClientMgr::HttpClientMgr(void)
{
    
}

HttpClientMgr::~HttpClientMgr(void)
{
    HttpClient::destroyInstance();
    CC_SAFE_DELETE(s_pHttpClientMgr);
}

HttpClientMgr* HttpClientMgr::getInstance()
{
    if (s_pHttpClientMgr == nullptr) {
        s_pHttpClientMgr = new HttpClientMgr();
    }
    
    return s_pHttpClientMgr;
}

void HttpClientMgr::GetHttpImg(std::string imgurl, std::string filename)
{
    HttpRequest* request = new HttpRequest();
    request->setUrl(imgurl.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(HttpClientMgr::onHttpRequestRptImg, this));
    request->setTag(filename.c_str());
    HttpClient::getInstance()->send(request);
    request->release();
}

void HttpClientMgr::onHttpRequestRptImg(HttpClient *sender, HttpResponse *response)
{
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        log("%s completed", response->getHttpRequest()->getTag());
    }
    

    long statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
    log("response code: %ld", statusCode);
    
    
    if (!response->isSucceed()) 
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += response->getHttpRequest()->getTag();

    log("path: %s",path.c_str()); 
    std::string bufffff(buffer->begin(),buffer->end());
    
    FILE *fp = fopen(path.c_str(), "wb"); 
    fwrite(bufffff.c_str(), 1,buffer->size(), fp);  fclose(fp);
}

void HttpClientMgr::GetHttpFile(std::string fileurlr, std::string filename)
{
    HttpRequest* request = new HttpRequest();
    // required fields
    request->setUrl(fileurlr.c_str());
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(HttpClientMgr::onHttpRequestRptFile, this));
    request->setTag(filename.c_str());
    HttpClient::getInstance()->send(request);
    request->release();
}

void HttpClientMgr::onHttpRequestRptFile(HttpClient *sender, HttpResponse *response)
{
    if (!response)
    {
        return;
    }
    
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag())) 
    {
        log("%s completed", response->getHttpRequest()->getTag());
    }
    
    long statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
    log("response code: %ld", statusCode);
    
    if (!response->isSucceed()) 
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    std::string res; 
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        res+=(*buffer)[i];
    }
    std::string path = FileUtils::getInstance()->getWritablePath();
    path += response->getHttpRequest()->getTag();

    log("path: %s",path.c_str()); 
    FILE* fp = fopen(path.c_str(), "wb");
    if (fp)
    {
        fputs(res.c_str(), fp);
        fclose(fp);
    }

	//save image to local directly
	auto adConfData = UtilHelper::readAdConfData();
	for (int i = 0; i < adConfData.size(); i++)
	{
		auto url = "http://www.coolgameworld.com/root/com_gamefunny_hill_climb/" + adConfData[i].icon;
		HttpClientMgr::getInstance()->GetHttpImg(url, adConfData[i].icon);
	}
}

void HttpClientMgr::ReadHttpFile(std::string fileurlr, std::string filename)
{
	HttpRequest* request = new HttpRequest();
	// required fields
	request->setUrl(fileurlr.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(HttpClientMgr::onHttpRequestReadRptFile, this));
	request->setTag(filename.c_str());
	HttpClient::getInstance()->send(request);
	request->release();
}

void HttpClientMgr::onHttpRequestReadRptFile(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag())) 
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	long statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
	log("response code: %ld", statusCode);

	if (!response->isSucceed()) 
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string res; 
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		res+=(*buffer)[i];
	}

	auto adConfData = DataMgr::getInstance()->getAdConfData();//old value
	auto adConfDataFromNet = UtilHelper::getAdConfDataFromNet(res);
	auto needUpdate = false;
	if (adConfData.size() != adConfDataFromNet.size())//update
	{
		for (int i = 0; i < adConfDataFromNet.size(); i++)
		{
			if (!FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + adConfDataFromNet[i].icon))
			{
				auto url = "http://www.coolgameworld.com/root/com_gamefunny_hill_climb/" + adConfDataFromNet[i].icon;
				HttpClientMgr::getInstance()->GetHttpImg(url, adConfDataFromNet[i].icon);
			}
		}
		needUpdate = true;
	}
	else
	{
		for (int i = 0; i < adConfDataFromNet.size(); i++)
		{
			if (adConfData[i].package != adConfDataFromNet[i].package 
				|| adConfData[i].version != adConfDataFromNet[i].version
				|| adConfData[i].icon != adConfDataFromNet[i].icon)
			{
				needUpdate = true;
				if (!FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + adConfDataFromNet[i].icon))
				{
					auto url = "http://www.coolgameworld.com/root/com_gamefunny_hill_climb/" + adConfDataFromNet[i].icon;
					HttpClientMgr::getInstance()->GetHttpImg(url, adConfDataFromNet[i].icon);
				}
			}
		}
	}
	if (needUpdate)
	{
		DataMgr::getInstance()->setAdConfData(adConfDataFromNet);
		UtilHelper::updateAdConf(res);
	}
}