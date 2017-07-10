#include "GameScene.h"
#include "Global.h"
#include "LoginScene.h"
#include "network/HttpClient.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#define database UserDefault :: getInstance()
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

bool GameScene::init() {
	if (!Layer::init())
	{
		return false;
	}
	rankbutton_click = false;
	submitbutton_click = false;
	Size size = Director::getInstance()->getVisibleSize();
	visibleHeight = size.height;
	visibleWidth = size.width;

	score_field = TextField::create("Score", "Arial", 30);
	score_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 3));
	this->addChild(score_field);

	//实现提交按钮，和之前的登录按钮的实现差不多
	auto submit = Label::createWithTTF("Submit", "fonts/arial.ttf", 48);
	submit->setColor(Color3B(247, 147, 0));
	auto callback = MenuItemLabel::create(submit, CC_CALLBACK_1(GameScene::submitButtonClick, this));
	Menu* Submit = Menu::create(callback, NULL);
	Submit->setPosition(Vec2(visibleWidth / 4, visibleHeight / 4));
	this->addChild(Submit);

	//实现排名按钮
	auto rank = Label::createWithTTF("Rank", "fonts/arial.ttf", 48);
	rank->setColor(Color3B(247, 147, 0));
	auto callback1 = MenuItemLabel::create(rank, CC_CALLBACK_1(GameScene::rankButtonClick, this));
	Menu* Rank = Menu::create(callback1, NULL);
	Rank->setPosition(Vec2(visibleWidth / 4 * 3, visibleHeight / 4));
	this->addChild(Rank);

	//显示等级的界面
	rank_field = Label::createWithTTF("", "fonts/arial.ttf", 30);
	rank_field->setPosition(visibleWidth / 5 * 4, visibleHeight / 5 * 4);
	this->addChild(rank_field);

	//database->setIntegerForKey("Score", 0);
	return true;
}

void GameScene::submitButtonClick(cocos2d::Ref * pSender) {
	if (score_field->getString() != "") {
		submitbutton_click = true;
		HttpRequest* request;
		request = new HttpRequest();
		request->setRequestType(HttpRequest::Type::POST);
		request->setUrl("localhost:8080/submit");
		request->setResponseCallback(
			CC_CALLBACK_2(GameScene::submitMenuCallBack, this));
		//需要在请求的Header部分加入GAMESESSIONID
		//cookie的作用就 是为了解决HTTP协议无状态的缺陷所作的努力
		vector<string> header;
		header.push_back("Cookie: " + Global::gameSessionId);
		request->setHeaders(header);
		string data = "score=" + score_field->getString();
		request->setRequestData(data.c_str(), data.size());
		//添加请求到HttpClient任务队列里面
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}
	else {
		log("no input");
	}
}

void GameScene::rankButtonClick(cocos2d::Ref * pSender) {
	HttpRequest* request;
	request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::GET);
	request->setUrl("localhost:8080/rank?top=10");
	request->setResponseCallback(
		CC_CALLBACK_2(GameScene::rankMenuCallBack, this));
	//需要在请求的Header部分加入GAMESESSIONID
	vector<string> header;
	header.push_back("Cookie: " + Global::gameSessionId);
	request->setHeaders(header);
	//添加请求到HttpClient任务队列里面
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameScene::submitMenuCallBack(HttpClient* sender, HttpResponse* response) {
	if (response == nullptr)
		return;
	if (response->isSucceed()) {
		rankButtonClick(nullptr);
	}
	else {
		log("response fail");
	}
}

void GameScene::rankMenuCallBack(HttpClient* sender, HttpResponse* response) {
	if (response == nullptr) {
		log("no response");
		return;
	}
	if (response->isSucceed()) {
		string responsedata(response->getResponseData()->begin(), response->getResponseData()->end());
		rapidjson::Document json;
		//json½âÊÍ
		json.Parse(responsedata.c_str());
		if (!json.HasParseError() && json.HasMember("result") && json["result"].GetBool()) {
			//result 为true info保存最好的成绩,result为false， info保存原因
			string reason = json["info"].GetString();
			//使用erase函数删除掉获得的字符串的第一个字符
			reason.erase(0, 1);
			for (size_t i = 0; i < reason.length(); i++) {
				if (i && reason[i] == '|')
					reason[i] = '\n';
			}
			if (!submitbutton_click) {
				rank_field->setString(reason);
			}
			else {
				submitbutton_click = false;
			}
			
		}
	}
	else {
		log("Login fail");
	}
}
