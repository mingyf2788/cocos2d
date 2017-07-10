#include "HelloWorldScene.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;
using namespace std;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	this->visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	//使用ttfconfig预先设置好我们所预想的字体格式，然后使用ttfconfig来创建ttflabel
	TTFConfig ttfconfig;
	ttfconfig.fontFilePath = "fonts/arial.ttf";
	ttfconfig.fontSize = 36;
	//添加倒计时
	time = Label::createWithTTF(ttfconfig, "03:00");
	//时间调度器周期性的调度使用
	schedule(schedule_selector(HelloWorld::updatetime), 1.0f, kRepeatForever, 0);
	dtime = 180;
	time->setPosition(Vec2(origin.x + visibleSize.width - time->getContentSize().width, origin.y + visibleSize.height - time->getContentSize().height));
	time->setColor(Color3B(0, 100, 200));
	this->addChild(time, 1);

	
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
	pT->setPosition(Vec2(origin.x + 14 * pT->getContentSize().width, origin.y + visibleSize.height - 2 * pT->getContentSize().height));
	this->addChild(pT, 1);

	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	this->addChild(sp0, 0);

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	//从贴图中以像素单位切割，创建关键帧
	frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 68, 101)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(player, 2);

	//运动动画,高68，宽101
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}
	run.pushBack(frame0);

	// 攻击动画
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(113 * i, 0, 113, 113)));
		attack.pushBack(frame);
	}
	attack.pushBack(frame0);

	// 仿照攻击动画,死亡动画(帧数：22帧，高：90，宽：79）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}
	dead.pushBack(frame0);

	//标签
	auto labelX = Label::createWithTTF(ttfconfig, "X");
	auto labelY = Label::createWithTTF(ttfconfig, "Y");
	auto labelW = Label::createWithTTF(ttfconfig, "W");
	auto labelS = Label::createWithTTF(ttfconfig, "S");
	auto labelD = Label::createWithTTF(ttfconfig, "D");
	auto labelA = Label::createWithTTF(ttfconfig, "A");
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
	menus->setPosition(Vec2(origin.x + menus->getContentSize().width * 2, menus->getContentSize().height));
	menuw->setPosition(Vec2(origin.x + menus->getContentSize().width * 2, menux->getPosition().y));
	menua->setPosition(Vec2(origin.x + menua->getContentSize().width, menua->getContentSize().height));
	menud->setPosition(Vec2(origin.x + menud->getContentSize().width * 3, menud->getContentSize().height));
	auto menu = Menu::create(menuw, menus, menua, menud, menux, menuy, NULL);
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu, 1);

	//添加键盘响应事件
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [this](EventKeyboard::KeyCode keycode, Event* event) {
		//经过查询得知键盘上的上下左右的键码分别为28,29，26,27
		//X为147， Y为148
		switch ((int)keycode) {
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
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void HelloWorld::moveEvent(Ref*, char cid) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage()) {
		auto nowPos = player->getPosition();
		switch (cid) {
		case 'W':
			runkAnimate = Animate::create(Animation::createWithSpriteFrames(run, 0.05f, 1));
			//向上运动的距离若在正常情况下为40.0f.若是距离小于可以运动的距离40.0f,
			//那么就设置为可见距离减去当前位置的纵坐标在减去精灵的高度除以2
			player->runAction(Spawn::create(runkAnimate, MoveBy::create(0.45f, Vec2(0, min(visibleSize.height - nowPos.y - player->getContentSize().height / 2, 40.0f))), nullptr));
			break;
		case 'A':
			runkAnimate = Animate::create(Animation::createWithSpriteFrames(run, 0.05f, 1));
			//向左运动的距离若在正常情况下为40.0f.若是距离小于可以运动的距离40.0f,
			//那么向左运动的距离就由当前位置和精灵的宽度决定了
			player->runAction(Spawn::create(runkAnimate, MoveBy::create(0.45f, Vec2(-min(nowPos.x - player->getContentSize().width / 2, 40.0f), 0)), nullptr));
			break;
		case 'S':
			runkAnimate = Animate::create(Animation::createWithSpriteFrames(run, 0.05f, 1));
			//向下运动的距离若在正常情况下为40.0f.若是距离小于可以运动的距离40.0f,
			//那么向下运动的距离就由当前位置和精灵的高度决定了
			player->runAction(Spawn::create(runkAnimate, MoveBy::create(0.45f, Vec2(0, -min(nowPos.y - player->getContentSize().height / 2, 40.0f))), nullptr));
			break;
		case 'D':
			runkAnimate = Animate::create(Animation::createWithSpriteFrames(run, 0.05f, 1));
			//向右运动的距离若在正常情况下为40.0f.若是距离小于可以运动的距离40.0f,
			//那么向右运动的距离就由当前位置和精灵的高度决定了
			player->runAction(Spawn::create(runkAnimate, MoveBy::create(0.45f, Vec2(min(visibleSize.width - nowPos.x - player->getContentSize().width / 2, 40.0f), 0)), nullptr));
			break;
		}
	}

}


void HelloWorld::actionEvent(Ref*, char cid) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage()) {
		switch (cid) {
			case 'X':
				//若血条不够那么，精灵死亡，就不能再做攻击的动作了
				if (pT->getPercentage() <= 20)
					dead.popBack();
				deadAnimate = Animate::create(Animation::createWithSpriteFrames(dead, 0.1f, 1));
				//精灵做死亡的动作
				player->runAction(deadAnimate);
				//血条的长度减20
				pT->runAction(CCProgressTo::create(2, pT->getPercentage() - 20));
				break;
			case 'Y':
				attackAnimate = Animate::create(Animation::createWithSpriteFrames(attack, 0.1f, 1));
				//精灵做动作
				player->runAction(attackAnimate);
				//只要长度可以加，就加20
				if (pT->getPercentage() != 180)
					pT->runAction(CCProgressTo::create(1.8f, pT->getPercentage() + 20));
				break;
		}
	}
}

void HelloWorld::updatetime(float dt) {
	if (pT->getPercentage() == 0) {
		//若是血条已经为空，就直接返回，不需要再有其他的操作
		return;
	}
	//只要是以更新时间，就减
	--dtime;
	if (dtime < 0) {
		//时间一到就不再调用更新时间的函数，只需要将精灵从活的状态变为已经死去的状态就可以了
		unschedule(schedule_selector(HelloWorld::updatetime));
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