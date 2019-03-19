#include "Items.h"
#include "res.h"

Sprite* Items::createItemSprite(int type, Vec2 position) {
	Sprite* item;
	switch (type) {
	case 1:
		item = Sprite::create("items1.png");
		item->setTag(101);
		break;
	case 2:
		item = Sprite::create("items2.png");
		item->setTag(102);
		break;
	case 3:
		item = Sprite::create("items3.png");
		item->setTag(103);
		break;
	case 4:
		item = Sprite::create("items4.png");
		item->setTag(104);
		break;
	case 5:
		item = Sprite::create("items5.png");
		item->setTag(105);
		break;
	case 6:
		item = Sprite::create("items6.png");
		item->setTag(106);
		break;
	case 7:
		item = Sprite::create("items7.png");
		item->setTag(107);
		break;
	case 8:
		item = Sprite::create("items8.png");
		item->setTag(108);
		break;
	default:
		item = NULL;
		break;
	}
	if (item != NULL) {
		auto itemBody = PhysicsBody::createBox(item->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		itemBody->getShape(0)->setDensity(1.0f);
		itemBody->getShape(0)->setFriction(0.0f);
		itemBody->getShape(0)->setRestitution(1.0f);
		itemBody->setGravityEnable(false);
		//道具的掩码设定使道具只能与砖块产生碰撞事件，与任何物件都没有碰撞效果
		itemBody->setCategoryBitmask(0x04);
		itemBody->setContactTestBitmask(0x08);
		itemBody->setCollisionBitmask(0x00);
		//设置道具下落速度与难度的关系
		itemBody->setVelocity(Vec2(0.0f, -180.0f - 40 * Mode));
		item->setPosition(position);
		item->setPhysicsBody(itemBody);
	}
	return item;
}
void Items::lifeUp(Player* player) {
	player->addOneLife();
}
void Items::ballSpeedUp(Sprite* ball, int difficulty) {
	Vec2 ballSpeed = ball->getPhysicsBody()->getVelocity();
	ball->setTexture("speedball.png");
	if (difficulty == 1) {
		ball->getPhysicsBody()->setVelocity(ballSpeed * (850 / ballSpeed.length()));
	}
	else if (difficulty == 2) {
		ball->getPhysicsBody()->setVelocity(ballSpeed * (925 / ballSpeed.length()));
	}
	else {
		ball->getPhysicsBody()->setVelocity(ballSpeed * (1000 / ballSpeed.length()));
	}
}
Sprite* Items::paddleLengthUp(Sprite* paddle) {
	Sprite* longPaddle = Sprite::create("paddlelong.png");
	auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	paddleBody->getShape(0)->setRestitution(1.0f);
	paddleBody->getShape(0)->setFriction(0.0f);
	paddleBody->getShape(0)->setDensity(10.0f);
	paddleBody->setGravityEnable(false);
	paddleBody->setDynamic(false);
	paddleBody->setCategoryBitmask(0x69);
	paddleBody->setContactTestBitmask(0x16);
	paddleBody->setCollisionBitmask(0x12);
	longPaddle->runAction(Place::create(paddle->getPosition()));
	//longPaddle->setPosition(paddle->getPosition());
	longPaddle->setPhysicsBody(paddleBody);
	longPaddle->setTag(98);
	return longPaddle;
}
Sprite* Items::paddleLengthDown(Sprite* paddle) {
	Sprite* shortPaddle = Sprite::create("paddleshort.png");
	auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	paddleBody->getShape(0)->setRestitution(1.0f);
	paddleBody->getShape(0)->setFriction(0.0f);
	paddleBody->getShape(0)->setDensity(10.0f);
	paddleBody->setGravityEnable(false);
	paddleBody->setDynamic(false);
	paddleBody->setCategoryBitmask(0x69);
	paddleBody->setContactTestBitmask(0x16);
	paddleBody->setCollisionBitmask(0x12);
	shortPaddle->runAction(Place::create(paddle->getPosition()));
	shortPaddle->setPosition(paddle->getPosition());
	shortPaddle->setPhysicsBody(paddleBody);
	shortPaddle->setTag(98);
	return shortPaddle;
}
void Items::superBall(Sprite* ball) {
	ball->setTexture("superball.png");
	ball->getPhysicsBody()->setCategoryBitmask(0x210);
	ball->getPhysicsBody()->setContactTestBitmask(0x120);
	ball->getPhysicsBody()->setCollisionBitmask(0x40);
	ball->setTag(96);
}
void Items::transparentBall(Sprite* ball) {
	ball->setOpacity(108);
	//ball->setTexture("transball.png");
}
void Items::multiBall(Sprite* ball) {
	//分裂球，暂未实现
}
void Items::magnetBall(Sprite* ball) {
	//磁力球，暂未实现
}
Items* Items::m_Items = NULL;
Items* Items::getInstance() {
	if (m_Items == NULL) {
		m_Items = new Items();
		if (m_Items && m_Items->init()) {
			m_Items->autorelease();
			m_Items->retain();
		}
		else{
			CC_SAFE_DELETE(m_Items);
			m_Items = NULL;
		}
	}
	return m_Items;
}
bool Items::init() {
	return true;
}