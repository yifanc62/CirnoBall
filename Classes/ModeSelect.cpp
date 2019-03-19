#include "ModeSelect.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "res.h"
#include "Tutorial.h"
using namespace cocostudio;

using namespace cocos2d::extension;
USING_NS_CC;

void ModeSelect::onClick_Easy(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		Mode = 1;
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b1.wav");
		}
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, Tutorial::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
void ModeSelect::onClick_Normal(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		Mode = 2;
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b1.wav");
		}
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, Tutorial::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
void ModeSelect::onClick_Lunatic(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		Mode = 3;
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b1.wav");
		}
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, Tutorial::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
void ModeSelect::onClick_back(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b2.wav");
		}
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, MainMenu::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
Scene* ModeSelect::createScene(){
	auto scene = Scene::create();
	auto layer = ModeSelect::create();
	scene->addChild(layer);
	return scene;
}
bool ModeSelect::init(){
	if (!Layer::init())
	{
		return false;
	}
	//加载分数排行UI
	auto UIM = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("ModeSelect_1.ExportJson");
	this->addChild(UIM);
	//加载UI动画
	object = ActionManagerEx::getInstance()->playActionByName("ModeSelect_1.ExportJson", "Animation0");
	//获取控件对象
	Button* Button_Easy = (Button*)Helper::seekWidgetByName(UIM, "Easy");
	Button* Button_Normal = (Button*)Helper::seekWidgetByName(UIM, "Normal");
	Button* Button_Lunatic = (Button*)Helper::seekWidgetByName(UIM, "Lunatic");
	Button* Button_back = (Button*)Helper::seekWidgetByName(UIM, "back");
	//添加按钮单击监听
	Button_Easy->addTouchEventListener(this, toucheventselector(ModeSelect::onClick_Easy));
	Button_Normal->addTouchEventListener(this, toucheventselector(ModeSelect::onClick_Normal));
	Button_Lunatic->addTouchEventListener(this, toucheventselector(ModeSelect::onClick_Lunatic));
	Button_back->addTouchEventListener(this, toucheventselector(ModeSelect::onClick_back));
	return true;
}

ModeSelect::~ModeSelect()
{
	object->stop();
}