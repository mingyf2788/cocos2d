#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "GameScene.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;

using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <string>
#define database UserDefault :: getInstance()
using namespace  rapidjson;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;
	//用户名输入按钮
    textField = TextField::create("Player Name", "Arial", 30);
    textField->setPosition(Size(visibleWidth / 2, visibleHeight / 4 * 3));
    this->addChild(textField, 1);

	//实现登录
	auto button = Label::createWithTTF("Login", "fonts/arial.ttf", 48);
	button->setColor(Color3B(247, 147, 0));
	auto callback = MenuItemLabel::create(button, CC_CALLBACK_1(LoginScene::LoginLabelClick, this));
	Menu* Login = Menu::create(callback, NULL);
	Login->setPosition(Vec2(visibleWidth / 2, visibleHeight / 2));
	this->addChild(Login);
	return true;
}

//当点击登录按钮时所作的操作
void LoginScene::LoginLabelClick(Ref* pSender) {
	if (textField->getString() != "") {
		HttpRequest* request;
		// 使用HttpRequest无参数的构造函数构建实例
		request = new HttpRequest();
		//设置连接方法的类型和待连接地址
		request->setRequestType(HttpRequest::Type::POST);
		request->setUrl("localhost:8080/login");
		//设置回调
		string data = "username=" + textField->getString();
		request->setRequestData(data.c_str(), data.size());
		request->setResponseCallback(
			CC_CALLBACK_2(LoginScene::startMenuCallBack, this));
		//添加请求到HttpClient任务队列里面
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}
}

//对点击按钮之后的操作
void LoginScene::startMenuCallBack(HttpClient* sender, HttpResponse* response) {
	if (response == nullptr) {
		log("no reponse!");
		return;
	}
	if (response->isSucceed()) {
		string resHeader(response->getResponseHeader()->begin(), response->getResponseHeader()->end());
		//获取用户的id信息
		Global::gameSessionId = Global::getSessionIdFromHeader(resHeader);
		database->setStringForKey("username", textField->getString());
		//跳转到游戏界面
		Director::getInstance()->replaceScene(TransitionCrossFade::create((float)0.8, GameScene::createScene()));
	}
	else {
		log("login fail: %s", response->getErrorBuffer());
	}
}
