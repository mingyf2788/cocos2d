#include "HelloWorldScene.h"
#include<iostream>
#include<SimpleAudioEngine.h>
using namespace CocosDenshion;
using std::string;

USING_NS_CC;

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

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	auto clickme = MenuItemImage::create(
		"music.png", "music1.png", CC_CALLBACK_1(HelloWorld::playmusic, this));
	clickme->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2 + 40));
	// create menu, it's an autorelease object
	auto menu1 = Menu::create(clickme, NULL);
	menu1->setPosition(Vec2::ZERO);
	this -> addChild(menu1, 1);
	
	

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	//auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	auto *strings = Dictionary::createWithContentsOfFile("fonts/string.xml");
	const char *str = ((String*)strings->objectForKey("name"))->getCString();
	auto labelname = Label::create(str, "fonts/simhei.ttf", 15);
	auto labelid = Label::create("15331242", "fonts/simsun.ttc", 15);
	// position the label on the center of the screen
	labelname->setPosition(Vec2(origin.x + visibleSize.width / 2,
	origin.y + visibleSize.height - labelname->getContentSize().height));
	labelid->setPosition(Vec2(origin.x + visibleSize.width / 2,
	origin.y + visibleSize.height - labelid->getContentSize().height - 20));
	this->addChild(labelname, 1);
	this->addChild(labelid, 1);
	

	// add the label as a child to this layer
	//this->addChild(label, 1);
	
	// add "HelloWorld" splash screen"
	//auto sprite = Sprite::create("HelloWorld.png");
	auto sprite = Sprite::create("hello.jpg");
	//position the sprite on the center of the screen
	if (sprite != NULL)
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	return true;
}

void HelloWorld::playmusic(Ref* pSender) {
	Director::getInstance()->startAnimation();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music.mp3", true);
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
