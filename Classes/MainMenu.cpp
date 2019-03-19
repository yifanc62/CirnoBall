#include "MainMenu.h"
#include "Score.h"
#include "About.h"
#include "Config.h"
#include "SimpleAudioEngine.h"
#include "BlockBreaker.h"
#include "res.h"
#include "ModeSelect.h"
using namespace cocostudio;
USING_NS_CC;

void MainMenu::onClick_Start(Ref*, TouchEventType type){
	auto armature = (Armature*)getChildByTag(233);
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b1.wav");
			}
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, ModeSelect::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
//Out黑幕结束监听
void MainMenu::movementCallback(Armature * armature, MovementEventType movementType, const std::string& movementID)
{
	if (movementType == COMPLETE) { //如果动画播放状态是结束
		CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
		Director::getInstance()->end();
	}
}
void MainMenu::onClick_Score(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b1.wav");
		}
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f,Score::createScene()));	
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
void MainMenu::onClick_Config(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b1.wav");
		}
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, Config::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
void MainMenu::onClick_Exit(Ref*, TouchEventType type){
	ArmatureDataManager::getInstance()->addArmatureFileInfo("BlackOut.ExportJson");
	Armature *armatureOut = Armature::create("BlackOut");
	this->addChild(armatureOut);
	armatureOut->setPosition(Point(1366 / 2, 768 / 2));
	armatureOut->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(MainMenu::movementCallback));
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b1.wav");
		}
		armatureOut->getAnimation()->play("BlackOut");
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
void MainMenu::onClick_About(Ref*, TouchEventType type){
	auto armature = (Armature*)getChildByTag(233);
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b1.wav");
		}
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, About::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);
	//预加载bgm
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("nirinsou.wav");
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//播放bgm
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
	}
	else if (isBGM){
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("MainMenu.wav", true);
	}
	//加载UI
	auto UI1 = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("Cirno_Ball_1.ExportJson");
	this->addChild(UI1);
	
	//加载UI动画
	object = ActionManagerEx::getInstance()->playActionByName("Cirno_Ball_1.ExportJson", "Animation0");

	//入场黑幕动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo("SceneInOut.ExportJson");
	Armature *armatureIn = Armature::create("SceneInOut");
	armatureIn->setPosition(Point(1366 / 2, 768 / 2));
	this->addChild(armatureIn);
	armatureIn->getAnimation()->play("In");

	//获取控件对象
	Button* Button_6 = (Button*)Helper::seekWidgetByName(UI1, "Button_6");
	Button* Button_9 = (Button*)Helper::seekWidgetByName(UI1, "Button_9");
	Button* Button_7 = (Button*)Helper::seekWidgetByName(UI1, "Button_7");
	Button* Button_4 = (Button*)Helper::seekWidgetByName(UI1, "Button_4");
	Button* Button_8 = (Button*)Helper::seekWidgetByName(UI1, "Button_8");
	//添加按钮单击监听
	Button_6->addTouchEventListener(this, toucheventselector(MainMenu::onClick_Score));
	Button_9->addTouchEventListener(this, toucheventselector(MainMenu::onClick_Config));
	Button_7->addTouchEventListener(this, toucheventselector(MainMenu::onClick_Exit));
	Button_4->addTouchEventListener(this, toucheventselector(MainMenu::onClick_Start));
	Button_8->addTouchEventListener(this, toucheventselector(MainMenu::onClick_About));
    return true;
}
MainMenu::~MainMenu()
{
	object->stop();
}
