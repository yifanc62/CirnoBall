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
	Sprite* ball; //����
	Sprite* paddle; //���徫��
	Sprite* edge; //�߿���
	Sprite* life; //����ֵͼ�꾫��
	Sprite* pauseCover; //��ͣ���澫��
	Sprite* lifeLabel; //����ֵ��ǩͼ�꾫��
	Label* lifeLabel2; //����ֵ���ֱ�ǩ
	Label* scoreLabel; //������ǩ
	PhysicsWorld* world; //��������

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
	void movementCallback(Armature * armature, MovementEventType movementType, const std::string& movementID);//�������Ž�������
	void movementCallback_block(Armature * armature, MovementEventType movementType, const std::string& movementID);
	void onClick_back(Ref*, ui::TouchEventType type);//���ذ�ť����
	bool onContactBegin(const PhysicsContact& contact);
	void tick(float dt);
	void fixBallSpeed(float dt);
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(BlockBreaker);
};

#endif // __BLOCKBREAKER_SCENE_H__
