#include "GameSence.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameSence::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameSence::init()
{

	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//设置背景
	auto bg_sky = Sprite::create("level-background-0.jpg");
	bg_sky->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(bg_sky, 0);

	//设置石头层
	stoneLayer = Layer::create();
	stoneLayer->setAnchorPoint(Vec2::ZERO);
	stoneLayer->setPosition(Vec2::ZERO);
	this->addChild(stoneLayer, 1);
	//添加石头精灵
	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(560, 480));
	stoneLayer->addChild(stone);

	//设置老鼠层
	mouseLayer = Layer::create();
	mouseLayer->setAnchorPoint(Vec2(0, 0));
	mouseLayer->setPosition(Vec2(0, visibleSize.height / 2));
	this->addChild(mouseLayer, 1);
	//添加老鼠精灵
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("level-sheet.plist");
	mouse = Sprite::createWithSpriteFrameName("gem-mouse-0.png");
	mouse->setPosition(visibleSize.width / 2, 0);
	mouseLayer->addChild(mouse, 2);
	//最开始的时候老鼠的位置和toPos的位置是一样的
	toPos = Vec2(visibleSize.width / 2, 0);  
	//添加shoot
	auto label = Label::createWithTTF("shoot", "fonts/Marker Felt.ttf", 65);
	auto shootbegin = MenuItemLabel::create(label, CC_CALLBACK_1(GameSence::shootMenuCallback, this));
	Menu* shoot = Menu::create(shootbegin, NULL);
	shoot->setPosition(Vec2(740, 480));
	stoneLayer->addChild(shoot);

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {
	//获得点击时点击点的坐标，cocos2d-x的坐标
	auto location = touch->getLocation();
	//CCLog("%f %f", location.x, location.y);
	auto mouseposition = mouse->getPosition();
	if (mouseposition == toPos && location.y < 420) {
		//老鼠停下且点击位置在石头的下方才能放置奶酪
		Size visibleSize = Director::getInstance()->getVisibleSize();
		toPos = Vec2((int)location.x, (int)location.y - visibleSize.height / 2);
		//放置奶酪
		cheese = Sprite::create("cheese.png");
		cheese->setPosition(toPos);
		mouseLayer->addChild(cheese);

		//老鼠跑过去吃奶酪，并且奶酪淡出
		auto moveto = MoveTo::create(2.0, toPos);
		mouse->runAction(moveto);
		cheese->runAction(Sequence::create(ScaleTo::create(2.0, 1.0), FadeOut::create(1.0), nullptr));
	}
	return true;
}

void GameSence::shootMenuCallback(Ref* pSender) {
	Vec2 mouseposition = mouse->getPosition();
	if (mouseposition == toPos) {
		//老鼠停下的时候才能发射石头
		Size visibleSize = Director::getInstance()->getVisibleSize();

		//发射石头
		auto shootstone = Sprite::createWithSpriteFrameName("stone-0.png");
		Animate* stoneanimate = Animate::create(AnimationCache::getInstance()->getAnimation("stoneAnimation"));
		shootstone->runAction(RepeatForever::create(stoneanimate));
		shootstone->setPosition(stone->getPosition());
		this->addChild(shootstone, 1);
		auto seq = Sequence::create(MoveTo::create(1.5, Vec2(mouseposition.x, mouseposition.y + visibleSize.height / 2)), FadeOut::create(0.5), nullptr);
		shootstone->runAction(seq);
		//老鼠跑开留下钻石
		auto diamond = Sprite::create("diamond.png");
		diamond->setPosition(mouseposition);
		mouseLayer->addChild(diamond, 1);
		toPos.x = (int)(CCRANDOM_0_1() * 960);
		toPos.y = (int)(CCRANDOM_0_1() * 420-visibleSize.height / 2);
		auto mousemove = MoveTo::create(2.0, toPos);
		mouse->runAction(mousemove);
	}
}