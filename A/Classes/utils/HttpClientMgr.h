#ifndef __HttpGetImg__
#define __HttpGetImg__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
USING_NS_CC;

class HttpClientMgr : public cocos2d::Layer
{
public:
    
    HttpClientMgr();
    ~HttpClientMgr();

    static HttpClientMgr *getInstance();

    void GetHttpImg(std::string imgurl, std::string filename);
    
    void onHttpRequestRptImg(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

    void GetHttpFile(std::string file_url, std::string filename);
    
    void onHttpRequestRptFile(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void ReadHttpFile(std::string file_url, std::string filename);

	void onHttpRequestReadRptFile(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
};

#endif /* HttpGetImg */