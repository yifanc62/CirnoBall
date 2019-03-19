#include "BlockBreaker.h"
#include "SimpleAudioEngine.h"
#include "DataFileIO.h"
#include "Blocks.h"
#include "Items.h"
#include "res.h"
#include "GameOverScene.h"
#include "MainMenu.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* BlockBreaker::createScene() {
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	Vec2 gravity(0.0f, 0.0f);
	scene->getPhysicsWorld()->setGravity(gravity);
	BlockBreaker* layer = BlockBreaker::create();
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}
void BlockBreaker::setPhyWorld(PhysicsWorld* _world) {
	world = _world;
}
bool BlockBreaker::init() {
	if (!Layer::init()) {
		return false;
	}
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Game3.wav");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Game2.wav");
	if (isBGM){
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Game1.wav");//播放和预加载BGM
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//常量初始化
	difficulty = Mode;
	isPaused = false;
	//游戏背景图片
	auto gameBackGround = Sprite::create("mainback0.png");
	gameBackGround->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	gameBackGround->setTag(199);
	this->addChild(gameBackGround, 0);

	//动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo("NewAnimation.ExportJson");
	Armature *armature = Armature::create("NewAnimation");
	armature->setPosition(Point(1200, 768 / 5));
	this->addChild(armature);
	armature->setTag(233);
	armature->getAnimation()->play("Animation1");
	armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback));
	//加载边界
	edge = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(Size(1400.0f,738.0f), PHYSICSBODY_MATERIAL_DEFAULT, 25.0f);
	boundBody->getShape(0)->setRestitution(1.0f);
	boundBody->getShape(0)->setFriction(10.0f);
	boundBody->getShape(0)->setDensity(1.0f);
	boundBody->setGravityEnable(false);
	boundBody->setDynamic(false);
	boundBody->setCategoryBitmask(0x61);
	boundBody->setContactTestBitmask(0x12);
	boundBody->setCollisionBitmask(0x12);
	edge->setPosition(Point(visibleSize.width / 2, (visibleSize.height / 2) - 40));
	edge->setPhysicsBody(boundBody);
	edge->setTag(99);
	this->addChild(edge, 1);
	//加载球
	ball = Sprite::create("ball.png",Rect(0.0f,0.0f,27.0f,27.0f));
	ball->setPosition(visibleSize.width / 2, 70.0f);
	auto ballBody = PhysicsBody::createCircle(ball->getContentSize().width / 2.);
	ballBody->getShape(0)->setRestitution(1.0f);
	ballBody->getShape(0)->setFriction(0.0f);
	ballBody->getShape(0)->setDensity(1.0f);
	ballBody->setGravityEnable(false);
	ballBody->setCategoryBitmask(0x202);
	ballBody->setContactTestBitmask(0x101);
	ballBody->setCollisionBitmask(0x01);
	ballBody->setRotationEnable(false);
	ball->setPhysicsBody(ballBody);
	ball->setTag(97);
	this->addChild(ball, 2);
	//加载挡板
	paddle = Sprite::create("paddle.png");
	auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
	paddleBody->getShape(0)->setRestitution(1.0f);
	paddleBody->getShape(0)->setFriction(0.0f);
	paddleBody->getShape(0)->setDensity(10.0f);
	paddleBody->setGravityEnable(false);
	paddleBody->setDynamic(false);
	paddleBody->setCategoryBitmask(0x69);
	paddleBody->setContactTestBitmask(0x16);
	paddleBody->setCollisionBitmask(0x12);
	paddle->setPosition(visibleSize.width / 2, 40.0f);
	paddle->setPhysicsBody(paddleBody);
	paddle->setTag(98);
	this->addChild(paddle, 3);
	//加载游戏半透明下边框
	auto gameFrameDown = Sprite::create("framedown.png");
	gameFrameDown->setPosition(Point(visibleSize.width / 2, gameFrameDown->getContentSize().height / 2));
	this->addChild(gameFrameDown, 9);
	auto diffLabel = Label::createWithTTF(difficulty == 1 ? "Easy" : (difficulty == 2 ? "Normal" : (difficulty == 3 ? "Lunatic" : "SB")), "fonts/Marker Felt.ttf", 40);
	diffLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
	diffLabel->setPosition(Vec2(1175, 728));
	this->addChild(diffLabel, 6);
	//实例化玩家对象
	player = new Player(difficulty);
	std::stringstream ss;
	std::string str;
	//添加生命值相关标签
	life = Sprite::create("life.png");
	life->setPosition(1200, 732);
	lifeLabel = Sprite::create("lifelabel.png");
	lifeLabel->setPosition(1245, 732);
	ss << player->getLife();
	ss >> str;
	lifeLabel2 = Label::createWithTTF(str, "fonts/ARLRDBD.TTF", 40);
	lifeLabel2->setAnchorPoint(Vec2(0.0f, 0.5f));
	lifeLabel2->setPosition(1266, 732);
	scoreLabel = Label::createWithTTF("0", "fonts/ARLRDBD.TTF", 40);
	scoreLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	scoreLabel->setPosition(15, 732);
	this->addChild(life, 6);
	this->addChild(lifeLabel, 6);
	this->addChild(lifeLabel2, 6);
	this->addChild(scoreLabel, 6);
	//添加砖块
	this->buildStage(player->getStage());
	//添加监听胜利与监听算法
	this->schedule(schedule_selector(BlockBreaker::tick), 0);
	this->schedule(schedule_selector(BlockBreaker::fixBallSpeed), 0);
	return true;
}
void BlockBreaker::buildStage(int _stage){
	isLaunched = false;
	std::stringstream ss;
	std::string staFileName;
	ss << _stage;
	ss >> staFileName;
	if (_stage < 10) {
		staFileName = "0" + staFileName;
	}
	std::string staFilePathHead = "stage/";
	std::string staFilePathEnd = ".sta";
	std::string staFilePath = staFilePathHead + staFileName + staFilePathEnd;
	DataFileIO::getInstance()->loadFile(staFilePath.c_str());
	float lineNum = DataFileIO::getInstance()->getLineNum(staFilePath.c_str());
	for (int i = 0; i < lineNum; i++){
		int x = DataFileIO::getInstance()->getBlockX(i, staFilePath.c_str());
		int y = DataFileIO::getInstance()->getBlockY(i, staFilePath.c_str());
		int type = DataFileIO::getInstance()->getBlockType(i, staFilePath.c_str());
		int item = DataFileIO::getInstance()->getBlockItem(i, staFilePath.c_str());
		this->addChild(Blocks::getInstance()->createBlockSprite(x, y, type, item));
	}
	if (_stage > 1){
		this->removeChildByTag(130);
	}
	auto stageNum = Label::createWithTTF("STAGE  " + staFileName, "fonts/ROSEWOODSTD.OTF", 65);
	stageNum->setPosition(Vec2(683, 720));
	stageNum->setTag(130);
	this->addChild(stageNum, 6);
}
void BlockBreaker::onEnter() {
	Layer::onEnter();
	mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(BlockBreaker::onMouseMoved, this);
	clickListener = EventListenerTouchOneByOne::create();
	clickListener->onTouchBegan = CC_CALLBACK_2(BlockBreaker::onClicked, this);
	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(BlockBreaker::onContactBegin, this);
	keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(BlockBreaker::onSpacePressed, this);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(clickListener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}
void BlockBreaker::onMouseMoved(EventMouse* event) {
	float mouseX = event->getCursorX();
	float paddleWidth = paddle->getContentSize().width;
	if (!isLaunched) {
		if (mouseX < paddleWidth / 2 + 8) {
			paddle->setPositionX(paddleWidth / 2 + 8);
			ball->setPositionX(paddleWidth / 2 + 8);
		}
		else {
			if (mouseX > Director::getInstance()->getVisibleSize().width - paddleWidth / 2 - 8) {
				paddle->setPositionX(Director::getInstance()->getVisibleSize().width - paddleWidth / 2 - 8);
				ball->setPositionX(Director::getInstance()->getVisibleSize().width - paddleWidth / 2 - 8);
			}
			else {
				paddle->setPositionX(mouseX);
				ball->setPositionX(mouseX);
			}
		}
	}
	else {
		if (mouseX < paddleWidth / 2 + 8) {
			paddle->setPositionX(paddleWidth / 2 + 8);
		}
		else {
			if (mouseX > Director::getInstance()->getVisibleSize().width - paddleWidth / 2 - 8) {
				paddle->setPositionX(Director::getInstance()->getVisibleSize().width - paddleWidth / 2 - 8);
			}
			else {
				paddle->setPositionX(mouseX);
			}
		}
	}
}
bool BlockBreaker::onClicked(Touch* touch, Event* event) {
	float ballSpeedY = 100 * difficulty + 500;
	if (difficulty == 1) {
		ball->getPhysicsBody()->setVelocity(Vec2(0.0f, ballSpeedY));
	}
	else if (difficulty == 2) {
		ball->getPhysicsBody()->setVelocity(Vec2(0.0f, ballSpeedY));
	}
	else {
		ball->getPhysicsBody()->setVelocity(Vec2(0.0f, ballSpeedY));
	}
	isLaunched = true;
	isBallFirstContact = true;
	clickListener->setEnabled(false);
	return true;
}
void BlockBreaker::onSpacePressed(EventKeyboard::KeyCode keycode, Event* event) {
	if (keycode == EventKeyboard::KeyCode::KEY_SPACE) {
		if (!isPaused) {
			isPaused = true;
			ballSpeed = ball->getPhysicsBody()->getVelocity();
			ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
			Vector<PhysicsBody*> bodies = world->getAllBodies();
			for each(PhysicsBody* body in bodies) {
				int tag = body->getNode()->getTag();
				if (tag > 100 && tag < 110 ){
					body->setVelocity(Vec2(0,0)); //停止所有道具
				}
			}
			if (!isLaunched) {
				clickListener->setEnabled(false); //在待发射状态下停止点击监听
			}
			mouseListener->setEnabled(false);
			pauseCover = Sprite::create("pause.png");
			pauseCover->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
			this->addChild(pauseCover, 10);
			//返回按钮
			Button* button_back = Button::create("back1.png", "back2.png");
			button_back->setPosition(Point(1200, 768 / 5));
			button_back->setTag(160);
			button_back->addTouchEventListener(this, toucheventselector(BlockBreaker::onClick_back));
			this->addChild(button_back, 11);
			if (isBGM) {
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			}
		}
		else {
			isPaused = false;
			ball->getPhysicsBody()->setVelocity(ballSpeed);
			Vector<PhysicsBody*> bodies = world->getAllBodies();
			for each(PhysicsBody* body in bodies) {
				int tag = body->getNode()->getTag();
				if (tag > 100 && tag < 110){
					body->setVelocity(Vec2(0.0f, -180.0f - 40 * Mode));
				}
			}
			if (!isLaunched) {
				clickListener->setEnabled(true);
			}
			mouseListener->setEnabled(true);
			pauseCover->removeFromParentAndCleanup(true);
			this->removeChildByTag(160);
			if (isBGM) {
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			}
		}
	}
}
void BlockBreaker::onClick_back(Ref*, ui::TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (isEffect){
			SimpleAudioEngine::getInstance()->playEffect("b2.wav");
		}
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, MainMenu::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
//动画结束监听
void BlockBreaker::movementCallback(Armature * armature, MovementEventType movementType, const std::string& movementID) {
	if (movementType == COMPLETE)//如果动画播放状态是结束
	{
		armature->getAnimation()->play("Animation1");
	}
}
void BlockBreaker::movementCallback_block(Armature * armature, MovementEventType movementType, const std::string& movementID) {
	if (movementType == COMPLETE)//如果动画播放状态是结束
	{
		this->removeChild(armature);
	}
}
bool BlockBreaker::onContactBegin(const PhysicsContact& contact) {
	auto phyBodyA = contact.getShapeA()->getBody();
	auto phyBodyB = contact.getShapeB()->getBody();
	auto spriteA = (Sprite*)(phyBodyA->getNode());
	auto spriteB = (Sprite*)(phyBodyB->getNode());
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();
	float A_x = spriteA->getPositionX();
	float B_x = spriteB->getPositionX();
	std::stringstream ss;
	std::string str;
	auto armature = (Armature*)getChildByTag(233); //获取动画对象


	if (tagA > 100 || tagB > 100) { //检测到了道具精灵
		switch (tagA) {
		case 101: //加一条命
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1up.wav");
			}
			armature->getAnimation()->play("Animation5");
			Items::getInstance()->lifeUp(player);
			ss << player->getLife();
			ss >> str;
			lifeLabel2->setString(str);
			spriteA->removeFromParentAndCleanup(true);
			break;
		case 102: //球加速
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1down.wav");
			}
			armature->getAnimation()->play("Animation3");
			Items::getInstance()->ballSpeedUp(ball, difficulty);
			spriteA->removeFromParentAndCleanup(true);
			break;
		case 103: { //板加长
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1up.wav");
			}
			armature->getAnimation()->play("Animation5");
			auto newPaddle = Items::getInstance()->paddleLengthUp(paddle);
			this->removeChild(paddle);
			paddle = newPaddle;
			this->addChild(paddle, 3);
			spriteA->removeFromParentAndCleanup(true);
			break;
		}
		case 104: {//板缩短
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1down.wav");
			}
			armature->getAnimation()->play("Animation3");
			auto newPaddle = Items::getInstance()->paddleLengthDown(paddle);
			this->removeChild(paddle);
			paddle = newPaddle;
			this->addChild(paddle, 3);
			spriteA->removeFromParentAndCleanup(true);
			break;
		}
		case 105: //火球
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1up.wav");
			}
			armature->getAnimation()->play("Animation5");
			Items::getInstance()->superBall(ball);
			spriteA->removeFromParentAndCleanup(true);
			break;
		case 106: //透明
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1down.wav");
			}
			armature->getAnimation()->play("Animation3");
			Items::getInstance()->transparentBall(ball);
			spriteA->removeFromParentAndCleanup(true);
			break;
		case 107: //分裂
			spriteA->removeFromParentAndCleanup(true);
			break;
		case 108: //磁铁
			spriteA->removeFromParentAndCleanup(true);
			break;
		default:
			break;
		}
		switch (tagB) {
		case 101: //加一条命
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1up.wav");
			}
			armature->getAnimation()->play("Animation5");
			Items::getInstance()->lifeUp(player);
			ss << player->getLife();
			ss >> str;
			lifeLabel2->setString(str);
			spriteB->removeFromParentAndCleanup(true);
			break;
		case 102: //球加速
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1down.wav");
			}
			armature->getAnimation()->play("Animation3");
			Items::getInstance()->ballSpeedUp(ball, difficulty);
			spriteB->removeFromParentAndCleanup(true);
			break;
		case 103: { //板加长
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1up.wav");
			}
			armature->getAnimation()->play("Animation5");
			auto newPaddle = Items::getInstance()->paddleLengthUp(paddle);
			this->removeChild(paddle);
			paddle = newPaddle;
			this->addChild(paddle, 3);
			spriteB->removeFromParentAndCleanup(true);
			break;
		}
		case 104: { //板缩短
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1down.wav");
			}
			armature->getAnimation()->play("Animation3");
			auto newPaddle = Items::getInstance()->paddleLengthDown(paddle);
			this->removeChild(paddle);
			paddle = newPaddle;
			this->addChild(paddle, 3);
			spriteB->removeFromParentAndCleanup(true);
			break;
		}
		case 105: //火球
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1up.wav");
			}
			armature->getAnimation()->play("Animation5");
			Items::getInstance()->superBall(ball);
			spriteB->removeFromParentAndCleanup(true);
			break;
		case 106: //透明
			if (isEffect){
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("1down.wav");
			}
			armature->getAnimation()->play("Animation3");
			Items::getInstance()->transparentBall(ball);
			spriteB->removeFromParentAndCleanup(true);
			break;
		case 107: //分裂
			spriteB->removeFromParentAndCleanup(true);
			break;
		case 108: //磁铁
			spriteB->removeFromParentAndCleanup(true);
			break;
		default:
			break;
		}
		return true;
	}
	
	//下面处理板和球碰撞时的角度修正，具体算法可参照设计说明
	float angle;
	float length = paddle->getContentSize().width;
	if ((tagA == 97 || tagA == 96) && tagB == 98) { //96:超级球 97:普通 98:挡板
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Boing.wav");
		}
		player->emptyCombo();
		Vec2 speedBefore = Vec2(spriteA->getPhysicsBody()->getVelocity().x, spriteA->getPhysicsBody()->getVelocity().y); //原速度
		Vec2 speedRebound = Vec2(spriteA->getPhysicsBody()->getVelocity().x, -spriteA->getPhysicsBody()->getVelocity().y); //正确的反射速度
		Vec2 speedRebound2 = Vec2(-spriteA->getPhysicsBody()->getVelocity().x, -spriteA->getPhysicsBody()->getVelocity().y); //正确的回弹速度
		bool isFromLeft = speedRebound.x > 0; //判断球是否从左边击中挡板
		bool isFromRight = speedRebound.x < 0; //判断球是否从右边击中挡板
		if (speedRebound.x > 0.1 || speedRebound.x < -0.1) {
			if (isFromLeft) {
				if (speedRebound.y / speedRebound.x >= tan(70 / 360.0 * 2 * PI)) {
					if (A_x <= B_x - length / 2 + length / 8) {
						angle = (5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound2.x*cos(angle) - speedRebound2.y*sin(angle), speedRebound2.y*cos(angle) + speedRebound2.x*sin(angle)));
					}
					else if (A_x <= B_x - length / 2 + length * 2 / 8 && A_x > B_x - length / 2 + length / 8) {
						angle = rand() % 501 / 100.0f / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound2.x*cos(angle) - speedRebound2.y*sin(angle), speedRebound2.y*cos(angle) + speedRebound2.x*sin(angle)));
					}
					else if (A_x <= B_x - length / 2 + length * 6 / 8 && A_x > B_x - length / 2 + length * 2 / 8) {
						spriteA->getPhysicsBody()->setVelocity(speedRebound);
					}
					else if (A_x <= B_x - length / 2 + length * 7 / 8 && A_x > B_x - length / 2 + length * 6 / 8) {
						angle = -(10 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x <= B_x + length / 2 && A_x > B_x - length / 2 + length * 7 / 8) {
						angle = -(20 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
				}
				else if (speedRebound.y / speedRebound.x >= tan(20 / 360.0 * 2 * PI)) {
					if (A_x <= B_x - length / 2 + length / 8) {
						angle = (20 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x <= B_x - length / 2 + length * 3 / 8 && A_x > B_x - length / 2 + length / 8) {
						angle = (5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x <= B_x - length / 2 + length * 6 / 8 && A_x > B_x - length / 2 + length * 3 / 8) {
						spriteA->getPhysicsBody()->setVelocity(speedRebound);
					}
					else if (A_x <= B_x - length / 2 + length * 7 / 8 && A_x > B_x - length / 2 + length * 6 / 8) {
						angle = -rand() % 501 / 100.0f / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x <= B_x + length / 2 && A_x > B_x - length / 2 + length * 7 / 8) {
						angle = -(5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
				}
				else {
					if (A_x <= B_x - length / 2 + length / 5) {
						angle = (40 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x <= B_x - length / 2 + length * 2 / 5 && A_x > B_x - length / 2 + length / 5) {
						angle = (30 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x <= B_x - length / 2 + length * 3 / 5 && A_x > B_x - length / 2 + length * 2 / 5) {
						angle = (20 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x <= B_x - length / 2 + length * 4 / 5 && A_x > B_x - length / 2 + length * 3 / 5) {
						angle = (10 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x >  B_x - length / 2 + length * 4 / 5) {
						spriteA->getPhysicsBody()->setVelocity(speedRebound);
					}
				}
			}
			else if (isFromRight) {
				if (speedRebound2.y / speedRebound2.x >= tan(70 / 360.0 * 2 * PI)) {
					if (A_x >= B_x + length / 2 - length / 8) {
						angle = -(5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound2.x*cos(angle) - speedRebound2.y*sin(angle), speedRebound2.y*cos(angle) + speedRebound2.x*sin(angle)));
					}
					else if (A_x >= B_x + length / 2 - length * 3 / 8 && A_x < B_x + length / 2 - length / 8) {
						angle = -rand() % 501 / 100.0f / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound2.x*cos(angle) - speedRebound2.y*sin(angle), speedRebound2.y*cos(angle) + speedRebound2.x*sin(angle)));
					}
					else if (A_x >= B_x + length / 2 - length * 6 / 8 && A_x < B_x + length / 2 - length * 3 / 8) {
						spriteA->getPhysicsBody()->setVelocity(speedRebound);
					}
					else if (A_x >= B_x + length / 2 - length * 7 / 8 && A_x < B_x + length / 2 - length * 6 / 8) {
						angle = (10 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x >= B_x - length / 2 && A_x < B_x + length / 2 - length * 7 / 8) {
						angle = (20 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
				}
				else if (speedRebound2.y / speedRebound2.x >= tan(20 / 360.0 * 2 * PI)) {
					if (A_x >= B_x + length / 2 - length / 8) {
						angle = -(20 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x >= B_x + length / 2 - length * 3 / 8 && A_x < B_x + length / 2 - length / 8) {
						angle = -(5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x <= B_x - length / 2 + length * 6 / 8 && A_x > B_x - length / 2 + length * 3 / 8) {
						spriteA->getPhysicsBody()->setVelocity(speedRebound);
					}
					else if (A_x >= B_x + length / 2 - length * 7 / 8 && A_x < B_x + length / 2 - length * 6 / 8) {
						angle = rand() % 501 / 100.0f / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x >= B_x - length / 2 && A_x < B_x + length / 2 - length * 7 / 8) {
						angle = (5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
				}
				else {
					if (A_x >= B_x + length / 2 - length / 5) {
						angle = -(40 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x >= B_x + length / 2 - length * 2 / 5 && A_x < B_x + length / 2 - length / 5) {
						angle = -(30 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x >= B_x + length / 2 - length * 3 / 5 && A_x < B_x + length / 2 - length * 2 / 5) {
						angle = -(20 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x >= B_x + length / 2 - length * 4 / 5 && A_x < B_x + length / 2 - length * 3 / 5) {
						angle = -(10 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (A_x < B_x + length / 2 - length * 4 / 5) {
						spriteA->getPhysicsBody()->setVelocity(speedRebound);
					}
				}
			}
			isBallFirstContact = false;
		}
		else if (isBallFirstContact) { //判断球是否在游戏开始时垂直击中挡板
			angle = 60 / 360.0 * 2 * PI * (B_x - A_x) / (length / 2);
			spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
			isBallFirstContact = false;
		}
		else if (A_x <= B_x - length / 2 + length / 3 || A_x > B_x + length / 2 - length / 3){ //判断球是否在游戏中垂直击中挡板
			angle = 20 / 360.0 * 2 * PI * (B_x - A_x) / (length / 2);
			spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
		}
	}
	if (tagA == 98 && (tagB == 97 || tagB == 96)) { //96:超级球 97:普通 98:挡板
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Boing.wav");
		}
		player->emptyCombo();
		Vec2 speedBefore = Vec2(spriteB->getPhysicsBody()->getVelocity().x, spriteB->getPhysicsBody()->getVelocity().y); //原速度
		Vec2 speedRebound = Vec2(spriteB->getPhysicsBody()->getVelocity().x, -spriteB->getPhysicsBody()->getVelocity().y); //正确的反射速度
		Vec2 speedRebound2 = Vec2(-spriteB->getPhysicsBody()->getVelocity().x, -spriteB->getPhysicsBody()->getVelocity().y); //正确的回弹速度
		bool isFromLeft = speedRebound.x > 0; //判断球是否从左边击中挡板
		bool isFromRight = speedRebound.x < 0; //判断球是否从右边击中挡板
		if (speedRebound.x > 0.1 || speedRebound.x < -0.1) {
			if (isFromLeft) {
				if (speedRebound.y / speedRebound.x >= tan(70 / 360.0 * 2 * PI)) {
					if (B_x <= A_x - length / 2 + length / 8) {
						angle = (5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound2.x*cos(angle) - speedRebound2.y*sin(angle), speedRebound2.y*cos(angle) + speedRebound2.x*sin(angle)));
					}
					else if (B_x <= A_x - length / 2 + length * 2 / 8 && B_x > A_x - length / 2 + length / 8) {
						angle = rand() % 501 / 100.0f / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound2.x*cos(angle) - speedRebound2.y*sin(angle), speedRebound2.y*cos(angle) + speedRebound2.x*sin(angle)));
					}
					else if (B_x <= A_x - length / 2 + length * 6 / 8 && B_x > A_x - length / 2 + length * 2 / 8) {
						spriteB->getPhysicsBody()->setVelocity(speedRebound);
					}
					else if (B_x <= A_x - length / 2 + length * 7 / 8 && B_x > A_x - length / 2 + length * 6 / 8) {
						angle = -(10 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x <= A_x + length / 2 && B_x > A_x - length / 2 + length * 7 / 8) {
						angle = -(20 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
				}
				else if (speedRebound.y / speedRebound.x >= tan(20 / 360.0 * 2 * PI)) {
					if (B_x <= A_x - length / 2 + length / 8) {
						angle = (20 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x <= A_x - length / 2 + length * 3 / 8 && B_x > A_x - length / 2 + length / 8) {
						angle = (5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x <= A_x - length / 2 + length * 6 / 8 && B_x > A_x - length / 2 + length * 3 / 8) {
						spriteB->getPhysicsBody()->setVelocity(speedRebound);
					}
					else if (B_x <= A_x - length / 2 + length * 7 / 8 && B_x > A_x - length / 2 + length * 6 / 8) {
						angle = -rand() % 501 / 100.0f / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x <= A_x + length / 2 && B_x > A_x - length / 2 + length * 7 / 8) {
						angle = -(5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
				}
				else {
					if (B_x <= A_x - length / 2 + length / 5) {
						angle = (40 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x <= A_x - length / 2 + length * 2 / 5 && B_x > A_x - length / 2 + length / 5) {
						angle = (30 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x <= A_x - length / 2 + length * 3 / 5 && B_x > A_x - length / 2 + length * 2 / 5) {
						angle = (20 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x <= A_x - length / 2 + length * 4 / 5 && B_x > A_x - length / 2 + length * 3 / 5) {
						angle = (10 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x >  A_x - length / 2 + length * 4 / 5) {
						spriteB->getPhysicsBody()->setVelocity(speedRebound);
					}
				}
			}
			else if (isFromRight) {
				if (speedRebound2.y / speedRebound2.x >= tan(70 / 360.0 * 2 * PI)) {
					if (B_x >= A_x + length / 2 - length / 8) {
						angle = -(5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound2.x*cos(angle) - speedRebound2.y*sin(angle), speedRebound2.y*cos(angle) + speedRebound2.x*sin(angle)));
					}
					else if (B_x >= A_x + length / 2 - length * 2 / 8 && B_x < A_x + length / 2 - length / 8) {
						angle = -rand() % 501 / 100.0f / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound2.x*cos(angle) - speedRebound2.y*sin(angle), speedRebound2.y*cos(angle) + speedRebound2.x*sin(angle)));
					}
					else if (B_x >= A_x + length / 2 - length * 6 / 8 && B_x < A_x + length / 2 - length * 2 / 8) {
						spriteB->getPhysicsBody()->setVelocity(speedRebound);
					}
					else if (B_x >= A_x + length / 2 - length * 7 / 8 && B_x < A_x + length / 2 - length * 6 / 8) {
						angle = (10 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x >= A_x - length / 2 && B_x < A_x + length / 2 - length * 7 / 8) {
						angle = (20 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
				}
				else if (speedRebound2.y / speedRebound2.x >= tan(20 / 360.0 * 2 * PI)) {
					if (B_x >= A_x + length / 2 - length / 8) {
						angle = -(20 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x >= A_x + length / 2 - length * 3 / 8 && B_x < A_x + length / 2 - length / 8) {
						angle = -(5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x <= A_x - length / 2 + length * 6 / 8 && B_x > A_x - length / 2 + length * 3 / 8) {
						spriteB->getPhysicsBody()->setVelocity(speedRebound);
					}
					else if (B_x >= A_x + length / 2 - length * 7 / 8 && B_x < A_x + length / 2 - length * 6 / 8) {
						angle = rand() % 501 / 100.0f / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x >= A_x - length / 2 && B_x < A_x + length / 2 - length * 7 / 8) {
						angle = (5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
				}
				else {
					if (B_x >= A_x + length / 2 - length / 5) {
						angle = -(40 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x >= A_x + length / 2 - length * 2 / 5 && B_x < A_x + length / 2 - length / 5) {
						angle = -(30 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x >= A_x + length / 2 - length * 3 / 5 && B_x < A_x + length / 2 - length * 2 / 5) {
						angle = -(20 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x >= A_x + length / 2 - length * 4 / 5 && B_x < A_x + length / 2 - length * 3 / 5) {
						angle = -(10 + rand() % 1001 / 100.0f) / 360 * 2 * PI;
						spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
					}
					else if (B_x < A_x + length / 2 - length * 4 / 5) {
						spriteB->getPhysicsBody()->setVelocity(speedRebound);
					}
				}
			}
			isBallFirstContact = false;
		}
		else if (isBallFirstContact) { //判断球是否在游戏开始时垂直击中挡板
			angle = 60 / 360.0 * 2 * PI * (A_x - B_x) / (length / 2);
			spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
			isBallFirstContact = false;
		}
		else if (B_x <= A_x - length / 2 + length / 3 || B_x > A_x + length / 2 - length / 3){ //判断球是否在游戏中垂直击中挡板
			angle = 20 / 360.0 * 2 * PI * (A_x - B_x) / (length / 2);
			spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
		}
	}

	//角度检测算法：保证角度接近水平时可以在碰撞边缘时调整角度方向
	/*if ((tagA == 97 || tagA == 96) && tagB == 99 && spriteA->getPositionY() > 172 && spriteA->getPositionY() < 602) {
		Vec2 speedRebound = Vec2(-spriteA->getPhysicsBody()->getVelocity().x, spriteA->getPhysicsBody()->getVelocity().y); //正确的反射速度
		if ((speedRebound.y / speedRebound.x) > 0 && (speedRebound.y / speedRebound.x) < tan(20 / 360.0 * 2 * PI)) {
			angle = (3 + rand() % 501 / 100.0f) / 360 * 2 * PI;
			spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
		}
		else if ((speedRebound.y / speedRebound.x) < 0 && (speedRebound.y / speedRebound.x) > tan((-20) / 360.0 * 2 * PI)) {
			angle = -(3 + rand() % 501 / 100.0f) / 360 * 2 * PI;
			spriteA->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
		}
	}
	if (tagA == 99 && (tagB == 97 || tagB == 96) && spriteB->getPositionY() > 172 && spriteB->getPositionY() < 602) {
		Vec2 speedRebound = Vec2(-spriteB->getPhysicsBody()->getVelocity().x, spriteB->getPhysicsBody()->getVelocity().y); //正确的反射速度
		if ((speedRebound.y / speedRebound.x) > 0 && (speedRebound.y / speedRebound.x) < tan(20 / 360.0 * 2 * PI)) {
			angle = (5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
			spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
		}
		else if ((speedRebound.y / speedRebound.x) < 0 && (speedRebound.y / speedRebound.x) > tan((-20) / 360.0 * 2 * PI)) {
			angle = -(5 + rand() % 501 / 100.0f) / 360 * 2 * PI;
			spriteB->getPhysicsBody()->setVelocity(Vec2(speedRebound.x*cos(angle) - speedRebound.y*sin(angle), speedRebound.y*cos(angle) + speedRebound.x*sin(angle)));
		}
	}*/

	//反射角度修正算法：保证在一定区域内镜面反射正常进行
	if (ball->getPositionY() > 65){
		if (tagA == 99 || tagB == 99){
			/*float ballSpeedOrigin = 100 * difficulty + 500;
			ballSpeed = ball->getPhysicsBody()->getVelocity();
			if (ballSpeed.length() < ballSpeedOrigin - 60) {
				ball->getPhysicsBody()->setVelocity(ballSpeed * (1 + 10 / ballSpeed.length()));
			}*/
			if (ball->getPositionX() > 40 && ball->getPositionX() < 1366 - 40){
				Vec2 speedRebound = Vec2(ball->getPhysicsBody()->getVelocity().x, -ball->getPhysicsBody()->getVelocity().y);
				ball->getPhysicsBody()->setVelocity(speedRebound);
			}
			else{
				Vec2 speedRebound = Vec2(-ball->getPhysicsBody()->getVelocity().x, ball->getPhysicsBody()->getVelocity().y);
				ball->getPhysicsBody()->setVelocity(speedRebound);
			}
		}
	}
	
	//死亡判定
	if (ball->getPositionY() <= 65) {
		auto armature = (Armature*)getChildByTag(233);
		armature->getAnimation()->play("Animation2");
		if (isEffect){
			SimpleAudioEngine::getInstance()->playEffect("dead.wav");
		}
		if (player->getLife() >= 1) {
			player->minusOneLife();
			ball->setTexture("ball.png");
			ball->setOpacity(255);
			ball->getPhysicsBody()->setCategoryBitmask(0x202);
			ball->getPhysicsBody()->setContactTestBitmask(0x101);
			ball->getPhysicsBody()->setCollisionBitmask(0x01);
			ball->getPhysicsBody()->setVelocity(Vec2(0.0f, 0.0f));
			ball->getPhysicsBody()->setRotationOffset(0.0f);
			ball->runAction(Place::create(Vec2(paddle->getPositionX(), 70)));
			ball->setTag(97);
			auto renewPaddle = Sprite::create("paddle.png");
			auto visibleSize = Director::getInstance()->getVisibleSize();
			auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			paddleBody->getShape(0)->setRestitution(1.0f);
			paddleBody->getShape(0)->setFriction(0.0f);
			paddleBody->getShape(0)->setDensity(10.0f);
			paddleBody->setGravityEnable(false);
			paddleBody->setDynamic(false);
			paddleBody->setCategoryBitmask(0x69);
			paddleBody->setContactTestBitmask(0x16);
			paddleBody->setCollisionBitmask(0x12);
			renewPaddle->runAction(Place::create(Vec2(paddle->getPositionX(), 40.0f)));
			renewPaddle->setPhysicsBody(paddleBody);
			renewPaddle->setTag(98);
			this->removeChild(paddle);
			paddle = renewPaddle;
			this->addChild(paddle, 3);

			ss << player->getLife();
			ss >> str;
			lifeLabel2->setString(str);

			Vector<PhysicsBody*> bodies = world->getAllBodies();
			for each(PhysicsBody* body in bodies) {
				int tag = body->getNode()->getTag();
				if (tag > 100){
					body->getNode()->removeFromParentAndCleanup(true);
				}
			}

			clickListener->setEnabled(true);

			isLaunched = false;
		}
		else {
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameOverScene::createScene()));
		}
	}
	//下面处理砖块与球的碰撞事件
	if (tagA < 90 && (tagA % 10 == 0 || tagA % 10 == 1 || tagA % 10 == 2 || tagA % 10 == 3)) {
		if (player->getCombo() >= 2){
			player->addScore(100);
		}
		if (tagB == 97) {
			player->addScore(10 * difficulty);
		}
		ss << player->getScore();
		ss >> str;
		scoreLabel->setString(str);
		player->addOneCombo();
		auto item = Items::getInstance()->createItemSprite(tagA / 10, spriteA->getPosition());
		if (item != NULL && (rand() % 10) < (4 - difficulty)) {
			item->runAction(Place::create(spriteA->getPosition()));
			this->addChild(item, 5);
		}
		if (isEffect){
			SimpleAudioEngine::getInstance()->playEffect("pengNormal.wav");//撞击音效
		}
		if (tagA % 10 == 0) {
			//这里添加蓝块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("BlockFade.ExportJson");
			Armature* armature = Armature::create("BlockFade");
			this->addChild(armature);
			armature->setPosition(spriteA->getPosition());
			armature->getAnimation()->play("BlueFade");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
		}
		else if (tagA % 10 == 1) {
			//这里添加金块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("YellowFade.ExportJson");
			Armature* armature = Armature::create("YellowFade");
			this->addChild(armature);
			armature->setPosition(spriteA->getPosition());
			armature->getAnimation()->play("YellowFade");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
		}
		else if (tagA % 10 == 2) {
			//这里添加绿块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("BlockFade.ExportJson");
			Armature* armature = Armature::create("BlockFade");
			this->addChild(armature);
			armature->setPosition(spriteA->getPosition());
			armature->getAnimation()->play("GreenFade");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
		}
		else if (tagA % 10 == 3) {
			//这里添加木块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("BlockFade.ExportJson");
			Armature* armature = Armature::create("BlockFade");
			this->addChild(armature);
			armature->setPosition(spriteA->getPosition());
			armature->getAnimation()->play("WoodFade");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
		}
		spriteA->removeFromParentAndCleanup(true);
		return true;
	}
	else if (tagA < 90 && tagA % 10 == 4) {
		if (isEffect){
			SimpleAudioEngine::getInstance()->playEffect("pengNormal.wav");//撞击音效
		}
		if (tagB == 97) {
			player->addScore(10 * difficulty);
			spriteA->setTexture("blockswood2.png");
			spriteA->setTag(tagA - 1);
		}
		else if (tagB == 96) {
			player->addScore(100);
			player->addOneCombo();
			auto item = Items::getInstance()->createItemSprite(tagA / 10, spriteA->getPosition());
			if (item != NULL && (rand() % 10) < (4 - difficulty)) {
				item->runAction(Place::create(spriteA->getPosition()));
				this->addChild(item, 5);
			}
			//这里添加木块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("MoodFade.ExportJson");
			Armature* armature = Armature::create("MoodFade");
			this->addChild(armature);
			armature->setPosition(spriteA->getPosition());
			armature->getAnimation()->play("MoodFade2");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
			spriteA->removeFromParentAndCleanup(true);
		}
		ss << player->getScore();
		ss >> str;
		scoreLabel->setString(str);
		return true;
	}
	else if (tagA < 90 && tagA % 10 == 5) {
		if (isEffect){
			SimpleAudioEngine::getInstance()->playEffect("pengNormal.wav");//撞击音效
		}
		if (tagB == 97) {
			player->addScore(10 * difficulty);
			spriteA->setTexture("blockswood1.png");
			spriteA->setTag(tagA - 1);
		}
		else if (tagB == 96) {
			player->addScore(100);
			player->addOneCombo();
			auto item = Items::getInstance()->createItemSprite(tagA / 10, spriteA->getPosition());
			if (item != NULL && (rand() % 10) < (4 - difficulty)) {
				item->runAction(Place::create(spriteA->getPosition()));
				this->addChild(item, 5);
			}
			//这里添加木块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("MoodFade.ExportJson");
			Armature* armature = Armature::create("MoodFade");
			this->addChild(armature);
			armature->setPosition(spriteA->getPosition());
			armature->getAnimation()->play("MoodFade1");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
			spriteA->removeFromParentAndCleanup(true);
		}
		ss << player->getScore();
		ss >> str;
		scoreLabel->setString(str);
		return true;
	}
	else if (tagA < 90 && tagA % 10 == 6) {
		//这里添加铁块动画
		ArmatureDataManager::getInstance()->addArmatureFileInfo("IronFlash.ExportJson");
		Armature* armature = Armature::create("IronFlash");
		this->addChild(armature);
		armature->setPosition(spriteA->getPosition());
		armature->getAnimation()->play("IronFlash");
		if (isEffect){
			SimpleAudioEngine::getInstance()->playEffect("pengIron.wav");
		}
		armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
	}
	if (tagB < 90 && (tagB % 10 == 0 || tagB % 10 == 1 || tagB % 10 == 2 || tagB % 10 == 3)) {
		if (player->getCombo() >= 2){
			player->addScore(100);
		}
		if (tagA == 97) {
			player->addScore(10 * difficulty);
		}
		ss << player->getScore();
		ss >> str;
		scoreLabel->setString(str);
		player->addOneCombo();
		auto item = Items::getInstance()->createItemSprite(tagB / 10, spriteB->getPosition());
		if (item != NULL && (rand() % 10) < (4 - difficulty)) {
			item->runAction(Place::create(spriteB->getPosition()));
			this->addChild(item, 5);
		}
		if (isEffect){
			SimpleAudioEngine::getInstance()->playEffect("pengNormal.wav");//撞击音效
		}
		if (tagB % 10 == 0) {
			//这里添加蓝块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("BlockFade.ExportJson");
			Armature* armature = Armature::create("BlockFade");
			this->addChild(armature);
			armature->setPosition(spriteB->getPosition());
			armature->getAnimation()->play("BlueFade");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
		}
		else if (tagB % 10 == 1) {
			//这里添加金块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("YellowFade.ExportJson");
			Armature *armature = Armature::create("YellowFade");
			this->addChild(armature);
			armature->setPosition(spriteB->getPosition());
			armature->getAnimation()->play("YellowFade");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
		}
		else if (tagB % 10 == 2) {
			//这里添加绿块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("BlockFade.ExportJson");
			Armature *armature = Armature::create("BlockFade");
			this->addChild(armature);
			armature->setPosition(spriteB->getPosition());
			armature->getAnimation()->play("GreenFade");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
		}
		else if (tagB % 10 == 3) {
			//这里添加木块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("BlockFade.ExportJson");
			Armature *armature = Armature::create("BlockFade");
			this->addChild(armature);
			armature->setPosition(spriteB->getPosition());
			armature->getAnimation()->play("WoodFade");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
		}
		spriteB->removeFromParentAndCleanup(true);
		return true;
	}
	else if (tagB < 90 && tagB % 10 == 4) {
		if (isEffect){
			SimpleAudioEngine::getInstance()->playEffect("pengNormal.wav");//撞击音效
		}
		if (tagA == 97) {
			player->addScore(10 * difficulty);
			spriteB->setTexture("blockswood2.png");
			spriteB->setTag(tagB - 1);
		}
		else if (tagA == 96) {
			player->addScore(100);
			player->addOneCombo();
			auto item = Items::getInstance()->createItemSprite(tagB / 10, spriteB->getPosition());
			if (item != NULL && (rand() % 10) < (4 - difficulty)) {
				item->runAction(Place::create(spriteB->getPosition()));
				this->addChild(item, 5);
			}
			//这里添加木块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("MoodFade.ExportJson");
			Armature* armature = Armature::create("MoodFade");
			this->addChild(armature);
			armature->setPosition(spriteB->getPosition());
			armature->getAnimation()->play("MoodFade2");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
			spriteB->removeFromParentAndCleanup(true);
		}
		ss << player->getScore();
		ss >> str;
		scoreLabel->setString(str);


		return true;
	}
	else if (tagB < 90 && tagB % 10 == 5) {
		if (isEffect){
			SimpleAudioEngine::getInstance()->playEffect("pengNormal.wav");//撞击音效
		}
		if (tagA == 97) {
			player->addScore(10 * difficulty);
			spriteB->setTexture("blockswood1.png");
			spriteB->setTag(tagB - 1);
		}
		else if (tagA == 96) {
			player->addScore(100);
			player->addOneCombo();
			auto item = Items::getInstance()->createItemSprite(tagB / 10, spriteB->getPosition());
			if (item != NULL && (rand() % 10) < (4 - difficulty)) {
				item->runAction(Place::create(spriteB->getPosition()));
				this->addChild(item, 5);
			}
			//这里添加木块动画
			ArmatureDataManager::getInstance()->addArmatureFileInfo("MoodFade.ExportJson");
			Armature* armature = Armature::create("MoodFade");
			this->addChild(armature);
			armature->setPosition(spriteB->getPosition());
			armature->getAnimation()->play("MoodFade1");
			armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
			spriteB->removeFromParentAndCleanup(true);
		}
		ss << player->getScore();
		ss >> str;
		scoreLabel->setString(str);
		return true;
	}
	else if (tagB < 90 && tagB % 10 == 6) {
		//这里添加铁块动画
		ArmatureDataManager::getInstance()->addArmatureFileInfo("IronFlash.ExportJson");
		Armature *armature = Armature::create("IronFlash");
		this->addChild(armature);
		armature->setPosition(spriteB->getPosition());
		armature->getAnimation()->play("IronFlash");
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pengIron.wav");
		}
		armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BlockBreaker::movementCallback_block));
	}
	return true;
}
void BlockBreaker::tick(float dt) {
	bool isWin = true;
	Vector<PhysicsBody*> bodies = world->getAllBodies();
	for each(PhysicsBody* body in bodies) {
		int tag = body->getNode()->getTag();
		if ((tag < 90) && (tag % 10 >= 0) && (tag % 10 <= 5)){
			isWin = false;
		}
		if (tag > 100 && tag < 110 && body->getNode()->getPositionY() < 75){
			body->setVelocity(body->getVelocity() * (1 + 1 / body->getVelocity().length()));
		}
		if (tag > 100 && tag < 110 && body->getNode()->getPositionY() < -20){
			body->getNode()->removeFromParentAndCleanup(true);
		}
	}
	if (isWin == true) {
		if (player->getStage() < 15) {
			//切换背景图片
			auto backPic = (Sprite*)this->getChildByTag(199);
			std::string picNamePre = "mainback";
			std::string picNameEnd = ".png";
			std::stringstream ss2;
			std::string str2;
			ss2 << rand() % 7;
			ss2 >> str2;
			std::string picName = picNamePre + str2 + picNameEnd;
			backPic->setTexture(picName);
			//播放BGM
			if (player->getStage() == 6 && isBGM){
				SimpleAudioEngine::getInstance()->playBackgroundMusic("Game2.wav");
			}
			else if (player->getStage() == 11 && isBGM){
				SimpleAudioEngine::getInstance()->playBackgroundMusic("Game3.wav");
			}
			//动画
			auto armature = (Armature*)getChildByTag(233);
			armature->getAnimation()->play("Animation4");
			//音效
			if (isEffect){
				SimpleAudioEngine::getInstance()->playEffect("Clear.wav");
			}

			player->addOneLife();
			ball->setTexture("ball.png");
			ball->setOpacity(255);
			ball->getPhysicsBody()->setCategoryBitmask(0x202);
			ball->getPhysicsBody()->setContactTestBitmask(0x101);
			ball->getPhysicsBody()->setCollisionBitmask(0x01);
			ball->setTag(97);
			auto renewPaddle = Sprite::create("paddle.png");
			auto visibleSize = Director::getInstance()->getVisibleSize();
			auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
			paddleBody->getShape(0)->setRestitution(1.0f);
			paddleBody->getShape(0)->setFriction(0.0f);
			paddleBody->getShape(0)->setDensity(10.0f);
			paddleBody->setGravityEnable(false);
			paddleBody->setDynamic(false);
			paddleBody->setCategoryBitmask(0x69);
			paddleBody->setContactTestBitmask(0x16);
			paddleBody->setCollisionBitmask(0x12);
			renewPaddle->setPosition(visibleSize.width / 2, 40.0f);
			renewPaddle->setPhysicsBody(paddleBody);
			renewPaddle->setTag(98);
			this->removeChild(paddle);
			paddle = renewPaddle;
			this->addChild(paddle, 3);

			Vector<PhysicsBody*> bodies = world->getAllBodies();
			for each(PhysicsBody* body in bodies) {
				int tag = body->getNode()->getTag();
				if ((tag < 90 && tag % 10 == 6) || (tag > 100)){
					body->getNode()->removeFromParentAndCleanup(true);
				}
			}

			std::stringstream ss;
			std::string str;
			ss << player->getLife();
			ss >> str;
			lifeLabel2->setString(str);
			player->addOneStage();
			this->buildStage(player->getStage());
			clickListener->setEnabled(true);
			ball->setPosition(visibleSize.width / 2, 70);
			ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
		}
		else if (player->getStage() == 15){
			Director::getInstance()->replaceScene(
				TransitionFade::create(0.5f, ClearScene::createScene()));
		}
	}
}
void BlockBreaker::fixBallSpeed(float dt) {
	if (ball->getPositionY() > 125.0f && !isPaused) {
		float ballSpeedOrigin = 100 * difficulty + 500;
		ballSpeed = ball->getPhysicsBody()->getVelocity();
		if (ballSpeed.length() < ballSpeedOrigin - 60) {
			ball->getPhysicsBody()->setVelocity(ballSpeed * (1 + 10 / ballSpeed.length()));
		}
	}
}