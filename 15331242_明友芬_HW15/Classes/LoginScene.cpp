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
	//�û������밴ť
    textField = TextField::create("Player Name", "Arial", 30);
    textField->setPosition(Size(visibleWidth / 2, visibleHeight / 4 * 3));
    this->addChild(textField, 1);

	//ʵ�ֵ�¼
	auto button = Label::createWithTTF("Login", "fonts/arial.ttf", 48);
	button->setColor(Color3B(247, 147, 0));
	auto callback = MenuItemLabel::create(button, CC_CALLBACK_1(LoginScene::LoginLabelClick, this));
	Menu* Login = Menu::create(callback, NULL);
	Login->setPosition(Vec2(visibleWidth / 2, visibleHeight / 2));
	this->addChild(Login);
	return true;
}

//�������¼��ťʱ�����Ĳ���
void LoginScene::LoginLabelClick(Ref* pSender) {
	if (textField->getString() != "") {
		HttpRequest* request;
		// ʹ��HttpRequest�޲����Ĺ��캯������ʵ��
		request = new HttpRequest();
		//�������ӷ��������ͺʹ����ӵ�ַ
		request->setRequestType(HttpRequest::Type::POST);
		request->setUrl("localhost:8080/login");
		//���ûص�
		string data = "username=" + textField->getString();
		request->setRequestData(data.c_str(), data.size());
		request->setResponseCallback(
			CC_CALLBACK_2(LoginScene::startMenuCallBack, this));
		//�������HttpClient�����������
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}
}

//�Ե����ť֮��Ĳ���
void LoginScene::startMenuCallBack(HttpClient* sender, HttpResponse* response) {
	if (response == nullptr) {
		log("no reponse!");
		return;
	}
	if (response->isSucceed()) {
		string resHeader(response->getResponseHeader()->begin(), response->getResponseHeader()->end());
		//��ȡ�û���id��Ϣ
		Global::gameSessionId = Global::getSessionIdFromHeader(resHeader);
		database->setStringForKey("username", textField->getString());
		//��ת����Ϸ����
		Director::getInstance()->replaceScene(TransitionCrossFade::create((float)0.8, GameScene::createScene()));
	}
	else {
		log("login fail: %s", response->getErrorBuffer());
	}
}
