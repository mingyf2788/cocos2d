#include "HelloWorldScene.h"
#include "Monster.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;
using namespace std;

Scene* HelloWorld::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

void HelloWorld::createItem() {
	//使用ttfconfig预先设置好我们所预想的字体格式，然后使用ttfconfig来创建ttflabel
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;

	auto labelX = Label::createWithTTF(ttfConfig, "X");
	auto labelY = Label::createWithTTF(ttfConfig, "Y");
	auto labelW = Label::createWithTTF(ttfConfig, "W");
	auto labelS = Label::createWithTTF(ttfConfig, "S");
	auto labelD = Label::createWithTTF(ttfConfig, "D");
	auto labelA = Label::createWithTTF(ttfConfig, "A");
	//菜单栏
	auto menux = MenuItemLabel::create(labelX, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'X'));
	auto menuy = MenuItemLabel::create(labelY, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'Y'));
	auto menuw = MenuItemLabel::create(labelW, CC_CALLBACK_1(HelloWorld::moveEvent, this, 'W'));
	auto menus = MenuItemLabel::create(labelS, CC_CALLBACK_1(HelloWorld::moveEvent, this, 'S'));
	auto menud = MenuItemLabel::create(labelD, CC_CALLBACK_1(HelloWorld::moveEvent, this, 'D'));
	auto menua = MenuItemLabel::create(labelA, CC_CALLBACK_1(HelloWorld::moveEvent, this, 'A'));
	menux->setPosition(Vec2(origin.x + visibleSize.width - menux->getContentSize().width * 3,
		menux->getContentSize().height * 2));
	menuy->setPosition(Vec2(menux->getPosition().x - menuy->getContentSize().width,
		menuy->getContentSize().height));
	menuw->setPosition(Vec2(origin.x + menuw->getContentSize().width, menux->getPosition().y));
	menua->setPosition(Vec2(origin.x + menua->getContentSize().width - 10, menua->getContentSize().height));
	menus->setPosition(Vec2(origin.x + menuw->getContentSize().width, menus->getContentSize().height));
	menud->setPosition(Vec2(origin.x + menud->getContentSize().width * 2 + 5, menud->getContentSize().height));
	auto menu = Menu::create(menux, menuy, menuw, menus, menud, menua, NULL);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu, 1); 

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	auto scale = visibleSize.width / 2.4 / sp0->getContentSize().width;
	sp0->setScaleX(scale);
	sp0->setScaleY(scale);
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleY(scale);
	pT->setScaleX(88.8 * scale);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x + 61, origin.y + visibleSize.height - 42));
	this->addChild(pT, 1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - 57));
	this->addChild(sp0, 0);
}

void HelloWorld::createPlayer() {
	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	//从贴图中以像素单位切割，创建关键帧
	frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 68, 101)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(player, 2);

	//运动动画,宽度为68，高度为101
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}
	run.pushBack(frame0);

	//攻击动画
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(113 * i, 0, 113, 113)));
		attack.pushBack(frame);
	}
	attack.pushBack(frame0);

	//死亡动画，可以仿照攻击动画来实现，宽度为70，高度为90
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}
	dead.pushBack(frame0);

	// 键盘响应事件
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event) {
		//根据资料可以知道键盘上的上下左右的键码分别为28,29,26,27，x和y的键码为147和148
		switch ((int)keyCode) {
		case 28:
			this->moveEvent(this, 'W');
			break;
		case 26:
			this->moveEvent(this, 'A');
			break;
		case 29:
			this->moveEvent(this, 'S');
			break;
		case 27:
			this->moveEvent(this, 'D');
			break;
		case 147:
			this->actionEvent(this, 'X');
			break;
		case 148:
			this->actionEvent(this, 'Y');
			break;
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
}

bool HelloWorld::init() {
	if (!Layer::init()) {
		return false;
	}
	this->visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;
	//设置倒计时
	time = Label::createWithTTF(ttfConfig, "03:00");   //倒计时
	//倒计时周期性的调用
	schedule(schedule_selector(HelloWorld::updateTime), 1.0f, kRepeatForever, 0);
	dtime = 180; 
	time->setPosition(Vec2(origin.x + visibleSize.width - time->getContentSize().width,
		origin.y + visibleSize.height - 42));
	time->setColor(Color3B(0, 100, 200));
	this->addChild(time, 1);

	//初始话
	createItem();
	createPlayer();
	//怪物的随机产生和一直被怪物攻击是一直都有的进行的
	schedule(schedule_selector(HelloWorld::createMonster), 3.0f, kRepeatForever, 0);
	schedule(schedule_selector(HelloWorld::hitByMonster), 0.05f, kRepeatForever, 0);

	//设置背景地图
	TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
	tmx->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(visibleSize.height / tmx->getContentSize().height);
	this->addChild(tmx, 0);
	return true;
}

void HelloWorld::moveEvent(Ref*, char cid) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage()) {
		auto nowPos = player->getPosition();
		switch (cid) {
			case 'W':
				runkAnimate = Animate::create(Animation::createWithSpriteFrames(run, 0.05f, 1));
				//在空间位置足够的情况下，每一次移动的距离为40.0f，如果距离不够那么就移动剩余的距离
				player->runAction(Spawn::create(runkAnimate, MoveBy::create(0.45f, Vec2(0, min(visibleSize.height - nowPos.y - player->getContentSize().height / 2, 40.0f))), nullptr));
				break;
			case 'A':
				runkAnimate = Animate::create(Animation::createWithSpriteFrames(run, 0.05f, 1));
				//在空间位置足够的情况下，每一次移动的距离为40.0f，如果距离不够那么
				//向左移动的距离就由当前的横坐标和横坐标到最左边的距离以及精灵本身的大小决定
				player->runAction(Spawn::create(runkAnimate, MoveBy::create(0.45f, Vec2(-min(nowPos.x - player->getContentSize().width / 2, 40.0f), 0)), nullptr));
				break;
			case 'S':
				runkAnimate = Animate::create(Animation::createWithSpriteFrames(run, 0.05f, 1));
				//在空间位置足够的情况下，每一次移动的距离为40.0f，如果距离不够那么
				//向下移动的距离由当前位置的纵坐标和精灵的高度决定
				player->runAction(Spawn::create(runkAnimate, MoveBy::create(0.45f, Vec2(0, -min(nowPos.y - player->getContentSize().height / 2, 40.0f))), nullptr));
				break;
			case 'D':
				runkAnimate = Animate::create(Animation::createWithSpriteFrames(run, 0.05f, 1));
				//在空间位置足够的情况下，每一次移动的距离为40.0f，如果距离不够那么
				//就移动剩余的距离移动的距离由当前的横坐标，精灵的宽度和精灵到最右边的距离决定
				player->runAction(Spawn::create(runkAnimate, MoveBy::create(0.45f, Vec2(min(visibleSize.width - nowPos.x - player->getContentSize().width / 2, 40.0f), 0)), nullptr));
				break;
			}
	}
}

void HelloWorld::actionEvent(Ref*, char cid) {
	bool deaded = dead.contains(player->getSpriteFrame()) && player->getSpriteFrame() != frame0;
	bool attacked = attack.contains(player->getSpriteFrame()) && player->getSpriteFrame() != frame0;
	if (pT->getPercentage() > 0) {
		switch (cid) {
		case 'X':
			if (deaded == false) {
				//如果血条不够，就不用做什么操作了
				if (pT->getPercentage() <= 20)
					dead.popBack();
				deadAnimate = Animate::create(Animation::createWithSpriteFrames(dead, 0.1f, 1));
				//精灵进行死亡的动作
				player->runAction(deadAnimate);
				//血条减少20
				Sequence* seq = Sequence::create(CCProgressTo::create(2, pT->getPercentage() - 20), CallFunc::create([this]() {
					if (pT->getPercentage() <= 0) {
						//游戏结束
						unschedule(schedule_selector(HelloWorld::createMonster));
						unschedule(schedule_selector(HelloWorld::hitByMonster));
					}
				}), nullptr);
				pT->runAction(seq);
			}
			break;
		case 'Y':
			if (attacked == false && deaded == false) {
				//停止掉所有的操作
				player->stopAllActions();
				attackAnimate = Animate::create(Animation::createWithSpriteFrames(attack, 0.1f, 1));
				//精灵攻击
				player->runAction(attackAnimate);
				hitMonster();
				//增加血条的功能在hitMonster（）里面实现
			}
			break;
		}
	}
}

void HelloWorld::updateTime(float dt) {
	if (pT->getPercentage() == 0) {
		//如果精灵的血条不够，那么就直接返回
		return;
	}
	--dtime;
	if (dtime < 0) {
		//如果时间不够了，那就直接停止操作
		unschedule(schedule_selector(HelloWorld::updateTime));
		dead.popBack();
		deadAnimate = Animate::create(Animation::createWithSpriteFrames(dead, 0.1f, 1));
		player->runAction(deadAnimate);
		return;
	}
	string t = "0";
	char minute[5], second[5];
	_itoa(dtime / 60, minute, 10);
	_itoa(dtime % 60, second, 10);
	t += minute;
	t += ':';
	if (dtime % 60 < 10)
		t += '0';
	t += second;
	time->setString(t);
}

void HelloWorld::createMonster(float flo) {
	auto fac = Factory ::getInstance();
	auto m = fac->createMonster();
	//将怪物随机的放在屏幕上的一个位置上
	m->setPosition(random(origin.x, visibleSize.width), random(origin.y, visibleSize.height));
	this->addChild(m, 1);
	fac->moveMonster(player->getPosition(), 2.0f);
}

void HelloWorld::hitByMonster(float flo) {
	//isKeyDown();
	bool deaded = dead.contains(player->getSpriteFrame()) && player->getSpriteFrame() != frame0;
	bool attacked = attack.contains(player->getSpriteFrame()) && player->getSpriteFrame() != frame0;
	if (deaded == false && attacked == false) {
		//当攻击和死亡者两个事件有一个在进行的时候就不进行攻击事件
		auto fac = Factory::getInstance();
		//判断是否有碰撞
		Sprite* clloison = fac->collider(player->getBoundingBox());
		if (clloison != nullptr) {
			//在发生撞击事件之前要停止正在进行的所有操作
			player->stopAllActions();
			//将怪物移出屏幕
			fac->removeMonster(clloison);
			//在移出怪物的同时，精灵本身也要出血
			this->actionEvent(this, 'X');
		}
	}
}

void HelloWorld::hitMonster() {
	auto fac = Factory::getInstance();
	//查看周围是否有怪物
	Sprite* collison = fac->hitMonster(player->getBoundingBox());
	if (collison != nullptr) {
		//将怪物移走
		fac->removeMonster(collison);
		if (pT->getPercentage() != 100) {
			//只要血量没有达到最大就充血
			pT->runAction(CCProgressTo::create(1.8f, pT->getPercentage() + 20));
		}
	}
}