#ifndef __GAME_SCENE2_1_H__
#define __GAME_SCENE2_1_H__

#include "cocos2d.h"
using namespace cocos2d;
USING_NS_CC;

class GameScene2_1 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	static PhysicsWorld* world;
	void setPhysicsWorld(PhysicsWorld * world);
	void addPlayer();
	void addBox();
	void addWall();
	void moveUp(Ref* sender);
	void moveDown(Ref* sender);
	void moveLeft(Ref* sender);
	void moveRight(Ref* sender);
	void addKeyboardListener();
	void onKeyPressed(EventKeyboard::KeyCode code, Event * event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event * event);
	void judgeWin(float dt);
	void nextCallback(Ref * pSender);
	void restartCallback(Ref * pSender);
	CREATE_FUNC(GameScene2_1);
	void meet(EventCustom* event);
	void addCustomListener();
	void update(float f);


private:
	cocos2d::Size visibleSize;
	PhysicsWorld* m_world;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	int dtime;
	cocos2d::ProgressTimer* pT;
	char lastCid;
	cocos2d::Sprite* player;
	cocos2d::Sprite* box1;
	cocos2d::Sprite* box2;
	cocos2d::Sprite* light1;
	cocos2d::Sprite* light2;
	cocos2d::Vector<SpriteFrame*> run;

	cocos2d::MenuItemImage* Switch;
	cocos2d::Sprite* trap;
	cocos2d::Sprite* trap1;
};

#endif
