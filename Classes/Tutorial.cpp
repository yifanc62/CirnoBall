#include "Tutorial.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "res.h"
#include "ModeSelect.h"
#include "BlockBreaker.h"
using namespace cocostudio;

using namespace cocos2d::extension;
USING_NS_CC;

void Tutorial::onClick_back(Ref*, TouchEventType type){
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
			TransitionFade::create(0.5f, ModeSelect::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
void Tutorial::onClick_start(Ref*, TouchEventType type){
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
			TransitionFade::create(0.5f, BlockBreaker::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
Scene* Tutorial::createScene(){
	auto scene = Scene::create();
	auto layer = Tutorial::create();
	scene->addChild(layer);
	return scene;
}
bool Tutorial::init(){
	if (!Layer::init())
	{
		return false;
	}
	//加载分数排行UI
	auto UIT = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("TeachMe_1.ExportJson");
	this->addChild(UIT);
	//加载UI动画
	object = ActionManagerEx::getInstance()->playActionByName("TeachMe_1.ExportJson", "Animation0");
	//获取控件对象
	Button* Button_back = (Button*)Helper::seekWidgetByName(UIT, "back");
	Button* Button_start = (Button*)Helper::seekWidgetByName(UIT, "start");
	//添加按钮单击监听
	Button_back->addTouchEventListener(this, toucheventselector(Tutorial::onClick_back));
	Button_start->addTouchEventListener(this, toucheventselector(Tutorial::onClick_start));
	return true;
}

Tutorial::~Tutorial()
{
	object->stop();
}