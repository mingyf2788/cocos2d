#include "Thunder.h"
#include "SimpleAudioEngine.h"
#include <algorithm>
USING_NS_CC;
using namespace CocosDenshion;

Scene* Thunder::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();
  // 'layer' is an autorelease object
  auto layer = Thunder::create();
  // add layer as a child to scene
  scene->addChild(layer);
  // return the scene
  return scene;
}

bool Thunder::init() {
  if (!Layer::init()) {
    return false;
  }
  Move = 0;
  time = 0;
  ismove = false;
  visibleSize = Director::getInstance()->getVisibleSize();
  // 创建背景
  auto bgsprite = Sprite::create("bg.jpg");
  bgsprite->setPosition(visibleSize / 2);
  bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
             visibleSize.height / bgsprite->getContentSize().height);
  this->addChild(bgsprite, 0);

  // 创建飞船
  player = Sprite::create("player.png");
  player->setAnchorPoint(Vec2(0.5, 0.5));
  player->setPosition(visibleSize.width / 2, player->getContentSize().height);
  player->setName("player");
  this->addChild(player, 1);

  // 显示陨石和子弹数量
  enemysNum = Label::createWithTTF("enemys: 0", "fonts/arial.TTF", 20);
  enemysNum->setColor(Color3B(255, 255, 255));
  enemysNum->setPosition(50, 60);
  this->addChild(enemysNum, 3);
  bulletsNum = Label::createWithTTF("bullets: 0", "fonts/arial.TTF", 20);
  bulletsNum->setColor(Color3B(255, 255, 255));
  bulletsNum->setPosition(50, 30);
  this->addChild(bulletsNum, 3);

  addEnemy(5);        // 初始化陨石
  preloadMusic();     // 预加载音乐
  playBgm();          // 播放背景音乐
  explosion();        // 创建爆炸帧动画

  // 添加监听器
  addTouchListener();
  addKeyboardListener();
  addCustomListener();

  // 调度器
  schedule(schedule_selector(Thunder::update), 0.05f, kRepeatForever, 0);
  return true;
}

void Thunder::preloadMusic() {
  //预加载音乐文件
  auto audio = SimpleAudioEngine::getInstance();
  //预加载背景
  audio->preloadBackgroundMusic("music/bgm.mp3");
  //预加载音效
  audio->preloadEffect("music/fire.wav");
  audio->preloadEffect("music/explore.wav");
}

void Thunder::playBgm() {
  //播放背景音乐
  auto audio = SimpleAudioEngine::getInstance();
  audio->playBackgroundMusic("music/bgm.mp3", true);
}


void Thunder::addEnemy(int n) {
  //初始化陨石
  enemys.clear();
  enemys.resize(n * 3);
  for (int i = 0; i < 3; ++i) {
    char enemyPath[20];
    sprintf(enemyPath, "stone%d.png", 3 - i);
    double width = visibleSize.width / (n + 1.0),
      height = visibleSize.height - (50 * (i + 1));
    for (int j = 0; j < n; ++j) {
      auto enemy = Sprite::create(enemyPath);
      enemy->setAnchorPoint(Vec2(0.5, 0.5));
      enemy->setScale(0.5, 0.5);
      enemy->setPosition(width * (j + 1), height);
      enemys.push_back(enemy);
      addChild(enemy, 1);
    }
  }
}


void Thunder::newEnemy() {
  // 陨石向下移动并生成新的一行
  for (auto temp = enemys.begin(); temp != enemys.end(); temp++) {
    if (*temp != NULL)
      (*temp)->setPosition((*temp)->getPosition() + Vec2(0, -50));
    //将原有的那些陨石不断往下移动位置实现动态的改变位置
  }
  static int stone = 1;
  char enemyPath[20];
  sprintf(enemyPath, "stone%d.png", stone);
  double width = visibleSize.width / 6.0;
  double height = visibleSize.height - 50;
  for (int j = 0; j < 5; ++j) {
    auto enemy = Sprite::create(enemyPath);
    enemy->setAnchorPoint(Vec2(0.5, 0.5));
    //设置缩放的比例，否则的话那些石头会比较大
    enemy->setScale(0.5, 0.5);
    enemy->setPosition(width * j + 65, height);
    enemys.push_back(enemy);
    addChild(enemy, 1);
  }
  stone = stone % 3 + 1;
}


void Thunder::movePlane(char c) {
  // 移动飞船,获取当前的飞船的位置
  auto pos = player->getPosition();
  switch (c) {
  case 'A':
    player->runAction(MoveBy::create(0.2f, Vec2(-min(player->getBoundingBox().getMinX(), 20.0f), 0)));
    break;
  case 'D':
    player->runAction(MoveBy::create(0.2f, Vec2(min(visibleSize.width - player->getBoundingBox().getMaxX(), 20.0f), 0)));
    break;
  }
}


void Thunder::fire() {
  //发射子弹
  auto bullet = Sprite::create("bullet.png");
  bullet->setAnchorPoint(Vec2(0.5, 0.5));
  bullets.push_back(bullet);
  bullet->setPosition(player->getPosition());
  addChild(bullet, 1);
  SimpleAudioEngine::getInstance()->playEffect("music/fire.wav", false);
  // 移除飞出屏幕外的子弹
  //这里将飞出屏幕的子弹移出，并且在子弹队列里面也移除
  bullet->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, visibleSize.height)),
    CallFunc::create([this, bullet]() {
    bullet->removeFromParentAndCleanup(true);
    this->bullets.remove(bullet);
  }), nullptr));
}

void Thunder::explosion() {
  // 切割爆炸动画帧
  auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
  explore.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(2, 0, 140, 140))));
}

void Thunder::update(float f) {
  // 实时更新页面内陨石和子弹数量(不得删除)
  // 要求数量显示正确(加分项)
  char str[15];
  sprintf(str, "enemys: %d", enemys.size() / 2);
  enemysNum->setString(str);
  sprintf(str, "bullets: %d", bullets.size());
  bulletsNum->setString(str);

  // 飞船移动
  if (ismove)
    this->movePlane(movekey);

  static int ct = 0;
  static int dir = 4;
  ++ct;
  if (ct == 120)
    ct = 40, dir = -dir;
  else if (ct == 80) {
    dir = -dir;
    newEnemy();  // 陨石向下移动并生成新的一行(加分项)
  }
  else if (ct == 20)
    ct = 40, dir = -dir;

  //陨石左右移动
  for (Sprite* s : enemys) {
    if (s != NULL) {
      s->setPosition(s->getPosition() + Vec2(dir, 0));
    }
  }

  // 分发自定义事件
  EventCustom e("meet");
  _eventDispatcher->dispatchEvent(&e);
}

// 自定义碰撞事件
void Thunder::meet(EventCustom * event) {
  // 判断子弹是否打中陨石并执行对应操作
  Sprite* temp;
  bool boom = false;
  for (auto begin = bullets.begin(); begin != bullets.end(); ) {
    boom = false;
    for (auto begin1 = enemys.begin(); begin1 != enemys.end(); begin1++) {
      //要确保这两个子弹是存在的
      if (*begin && *begin1) {
        int distance = (*begin)->getPosition().getDistance((*begin1)->getPosition());
        if (distance < 20) {
          temp = *begin1;
          //将子弹移出屏幕
          (*begin)->removeFromParentAndCleanup(true);
          //实现陨石的爆炸
          (*begin1)->runAction(Sequence::create(Animate::create(Animation::createWithSpriteFrames(explore, 0.05f, 1)),
            CallFunc::create([this, temp] {
            temp->removeFromParentAndCleanup(true);
          }), nullptr));
          SimpleAudioEngine::getInstance()->playEffect("music/explore.wav", false);
          boom = true;
          begin = bullets.erase(begin);
          begin1 = enemys.erase(begin1);
          break;
        }
      }
    }
    if (!boom)
      ++begin;
  }

  // 判断游戏是否结束并执行对应操作
  for (auto begin = enemys.begin(); begin != enemys.end(); ++begin) {
    if (*begin) {
      if ((*begin)->getBoundingBox().getMinY() <= player->getBoundingBox().getMinY()) {
        //当飞机所在的位置高于陨石所在的位置的时候游戏就结束；
        temp = player;
        player->runAction(Sequence::create(Animate::create(Animation::createWithSpriteFrames(explore, 0.05f, 1)),
          CallFunc::create([this, temp] {
          temp->removeFromParentAndCleanup(true);
          auto gameover = Sprite::create("gameOver.png");
          gameover->setAnchorPoint(Vec2(0.5, 0.5));
          gameover->setPosition(visibleSize / 2);
          this->addChild(gameover, 1);
        }), nullptr));

        SimpleAudioEngine::getInstance()->playEffect("music/explore.wav", false);
        unschedule(schedule_selector(Thunder::update));
        this->getEventDispatcher()->removeAllEventListeners();
        return;
      }
    }
  }
}

// 添加自定义监听器
void Thunder::addCustomListener() {
  auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
  _eventDispatcher->addEventListenerWithFixedPriority(meetListener, 1);
}

// 添加键盘事件监听器
void Thunder::addKeyboardListener() {
  //关于CC_CALLBACK的一点说明：第一个参数是要回调的函数名，第二个是绑定的参数一般为this
  //关于CC_CALLBACK_N， N的作用是第二个对象用第一个函数时绑定第N个参数后面的参数值
  //事实上在此处并没有第二个参数，所以不存在对第二个参数后面的值的绑定
  auto keyListener = EventListenerKeyboard::create();
  keyListener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
  keyListener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_A:
    player->setPosition(Vec2(player->getPositionX() - 5, player->getPositionY()));
    movekey = 'A';
    Move -= 5;
    ismove = true;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
  case EventKeyboard::KeyCode::KEY_D:
    player->setPosition(player->getPositionX() + 5, player->getPositionY());
    movekey = 'D';
    Move += 5;
    ismove = true;
    break;
  case EventKeyboard::KeyCode::KEY_SPACE:
    fire();
    break;
  default:
    break;
  }
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
    player->setPosition(Vec2(player->getPositionX() - 5, player->getPositionY()));
    Move += 5;
    ismove = false;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
    player->setPosition(player->getPositionX() + 5, player->getPositionY());
    Move -= 5;
    ismove = false;
    break;
  default:
    break;
  }
}

// 添加触摸事件监听器
void Thunder::addTouchListener() {
  this->setTouchEnabled(true);
  auto touchListerner = EventListenerTouchOneByOne::create();
  touchListerner->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
  touchListerner->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
  touchListerner->onTouchEnded = CC_CALLBACK_2(Thunder::onTouchEnded, this);
  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListerner, this);
}

// 鼠标点击发射炮弹
bool Thunder::onTouchBegan(Touch *touch, Event *event) {
  //点击的地方只能限制在屏幕之内
  //将精灵移动到点击的位置
  isClick = player->getBoundingBox().containsPoint(touch->getLocation());
  //如果要实现在点击飞机时实现发射子弹只需要调用发射子弹的函数即可
  fire();
  return true;
}

void Thunder::onTouchEnded(Touch *touch, Event *event) {
  Vec2 pos = touch->getLocation();
  if (pos.x > origin.x + visibleSize.width)
    pos.x = origin.x + visibleSize.width;
  else if (pos.x < origin.x)
    pos.x = origin.x;
  player->setPosition(Vec2(pos.x, player->getPositionY()));
  isClick = false;
}

// 当鼠标按住飞船后可控制飞船移动 (加分项)
void Thunder::onTouchMoved(Touch *touch, Event *event) {
  Vec2 pos = touch->getLocation();
  if (pos.x > origin.x + visibleSize.width)
    pos.x = origin.x + visibleSize.width;
  else if (pos.x < origin.x)
    pos.x = origin.x;
  player->setPosition(Vec2(pos.x, player->getPositionY()));
}
