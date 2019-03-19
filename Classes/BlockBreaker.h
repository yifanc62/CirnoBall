#ifndef __BLOCKBREAKER_SCENE_H__
#define __BLOCKBREAKER_SCENE_H__
#define PI 3.14

#include "cocos2d.h"
#include "Player.h"
#include "cocostudio/CCArmature.h"
#include "ui/UIButton.h"

USING_NS_CC;
using namespace cocostudio;

class BlockBreaker : public cocos2d::Layer {
public:
	Sprite* ball; //球精灵
	Sprite* paddle; //挡板精灵
	Sprite* edge; //边框精灵
	Sprite* life; //生命值图标精灵
	Sprite* pauseCover; //暂停界面精灵
	Sprite* lifeLabel; //生命值标签图标精灵
	Label* lifeLabel2; //生命值数字标签
	Label* scoreLabel; //分数标签
	PhysicsWorld* world; //物理世界

	int difficulty;
	bool isPaused;
	bool isLaunched;
	bool isBallFirstContact;
	Vec2 ballSpeed;

	EventListenerMouse* mouseListener;
	EventListenerTouchOneByOne* clickListener;
	EventListenerPhysicsContact* contactListener;
	EventListenerKeyboard* keyboardListener;

	void setPhyWorld(PhysicsWorld* _world);
	void buildStage(int _stage);
	virtual void onEnter();
	void onMouseMoved(EventMouse* event);
	bool onClicked(Touch* touch, Event* event);
	void onSpacePressed(EventKeyboard::KeyCode keycode, Event* event);
	void movementCallback(Armature * armature, MovementEventType movementType, const std::string& movementID);//动画播放结束监听
	void movementCallback_block(Armature * armature, MovementEventType movementType, const std::string& movementID);
	void onClick_back(Ref*, ui::TouchEventType type);//返回按钮监听
	bool onContactBegin(const PhysicsContact& contact);
	void tick(float dt);
	void fixBallSpeed(float dt);
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(BlockBreaker);
};

#endif // __BLOCKBREAKER_SCENE_H__
