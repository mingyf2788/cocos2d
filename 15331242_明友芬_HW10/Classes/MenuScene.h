#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void startMenuCallBack(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
	//void update(float dt) override;
};

#endif // __HELLOWORLD_SCENE_H__
