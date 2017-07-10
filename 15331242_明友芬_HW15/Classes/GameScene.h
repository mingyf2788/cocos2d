#pragma once
#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::network;

class GameScene : public cocos2d::Layer {
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	void submitButtonClick(cocos2d::Ref * pSender);
	void rankButtonClick(cocos2d::Ref * pSender);
	virtual void submitMenuCallBack(HttpClient* sender, HttpResponse* response);
	virtual void rankMenuCallBack(HttpClient* sender, HttpResponse* response);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	float visibleHeight;
	float visibleWidth;
	TextField *score_field;
	Label* rank_field;
	TextField *top10;
	bool rankbutton_click;
	bool submitbutton_click;
};

