#include"Monster.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame(){
	auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
	monsterDead.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(258-48*i,0,42,42)));
		monsterDead.pushBack(frame);
	}
}

Sprite* Factory::createMonster() {
	Sprite* mons = Sprite::create("Monster.png", CC_RECT_PIXELS_TO_POINTS(Rect(364,0,42,42)));
	monster.pushBack(mons);
	return mons;
}

void Factory::removeMonster(Sprite* sp) {
	Animation* anim = Animation::createWithSpriteFrames(monsterDead, 0.1f);
	Animate* ani = Animate::create(anim);
	Sequence* seq = Sequence::create(ani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), NULL);
	sp->runAction(seq);
	monster.eraseObject(sp);

}
void Factory::moveMonster(Vec2 playerPos,float time){
	//只要是在屏幕上的怪物就都要移动
	for each(Sprite* it in monster) {
		Vec2 mons = it->getPosition();
		Vec2 direction = playerPos - mons;
		direction.normalize();
		//怪物移动time长的时间，方向是direction*30
		it->runAction(MoveBy::create(time, direction * 30));
	}
}

Sprite* Factory::collider(Rect rect) {
	//判断碰撞
	for each (Sprite* it in monster) {
		if (rect.containsPoint(it->getPosition()))
			return it;
	}
}

Sprite* Factory::hitMonster(Rect rect) {
	//首先规定一个矩阵的大小，只有在这个矩阵范畴内的才算是撞击到了，否则都不算
	Rect area = Rect(rect.getMinX() - 30, rect.getMinY() - 20,
		rect.getMaxX() - rect.getMinX() + 60, rect.getMaxY() - rect.getMinY() + 40);
	for each (Sprite* sp in monster) {
		if (area.containsPoint(sp->getPosition()))
			return sp;
	}
}