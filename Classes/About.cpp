#include "About.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "res.h"

using namespace cocostudio;
using namespace cocos2d::extension;
USING_NS_CC;

void About::onClick(Ref*, TouchEventType type){
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
Scene* About::createScene(){
	auto scene = Scene::create();
	auto layer = About::create();
	scene->addChild(layer);
	return scene;
}
bool About::init(){
	if (!Layer::init())
	{
		return false;
	}
	//加载分数排行UI
	auto UI2 = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("About_1.ExportJson");
	this->addChild(UI2);
	//获取控件对象
	Button* backButton = (Button*)Helper::seekWidgetByName(UI2, "back");
	//添加按钮单击监听
	backButton->addTouchEventListener(this, toucheventselector(About::onClick));
	return true;
}
