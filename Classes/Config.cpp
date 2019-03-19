#include "Config.h"
#include "MainMenu.h"
#include "res.h"
using namespace cocostudio;
using namespace CocosDenshion;
#include "cocos-ext.h"
using namespace cocos2d::extension;
USING_NS_CC;

//返回按钮
void Config::onClick3(Ref*, TouchEventType type){
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (isEffect){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("b2.wav");
			SimpleAudioEngine::getInstance()->setEffectsVolume(EffectVolume / 100);
		}
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, MainMenu::createScene()));
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
//bgm复选框
void Config::checkSelectEvent_bg(Ref *pSender, CheckBoxEventType eventtype)
{
	switch (eventtype)
	{
	case CheckBoxEventType::CHECKBOX_STATE_EVENT_SELECTED:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("MainMenu.wav",true);
		isBGM = true;
		break;
	case CheckBoxEventType::CHECKBOX_STATE_EVENT_UNSELECTED:
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		isBGM = false;
		break;
	default:
		break;
	}
}
//音效复选框
void Config::checkSelectEvent_effect(Ref *pSender, CheckBoxEventType eventtype)
{
	switch (eventtype)
	{
	case CheckBoxEventType::CHECKBOX_STATE_EVENT_SELECTED:
		isEffect = true;
		break;
	case CheckBoxEventType::CHECKBOX_STATE_EVENT_UNSELECTED:
		isEffect = false;
		break;
	default:
		break;
	}
}
//bgm滑动条
void Config::sliderPercentChangeEvent_bg(Ref*pSender, SliderEventType type){
	if (type == SLIDER_PERCENTCHANGED){
		Slider* pSlider = (Slider*)pSender;
		BGMVolume = pSlider->getPercent();
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(BGMVolume / 100);
	}
}
//音效滑动条
void Config::sliderPercentChangeEvent_effect(Ref*pSender, SliderEventType type){
	if (type == SLIDER_PERCENTCHANGED){
		Slider* pSlider = (Slider*)pSender;
		EffectVolume = pSlider->getPercent();
	}
}
Scene* Config::createScene(){
	auto scene = Scene::create();
	auto layer = Config::create();
	scene->addChild(layer);
	//预加载bgm
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MainMenu.wav");
	return scene;
}
bool Config::init(){
	if (!Layer::init())
	{
		return false;
	}
	//加载分数排行UI
	auto UI3 = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("Config_1.ExportJson");
	this->addChild(UI3);
	//加载UI动画
	object = ActionManagerEx::getInstance()->playActionByName("Config_1.ExportJson", "Animation0");
	//按钮
	Button* Button_11 = (Button*)Helper::seekWidgetByName(UI3, "Button_11");//获取控件对象
	Button_11->addTouchEventListener(this, toucheventselector(Config::onClick3));	//添加按钮单击监听

	//复选框
	CheckBox* CheckBox_bgm = (CheckBox*)Helper::seekWidgetByName(UI3, "CheckBox_6");  //获取控件对象
	CheckBox* CheckBox_effect = (CheckBox*)Helper::seekWidgetByName(UI3, "CheckBox_5");
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){  //设置初始状态
		CheckBox_bgm->setSelectedState(true);
	}
	else{
		CheckBox_bgm->setSelectedState(false);
	}
	if (isEffect){
		CheckBox_effect->setSelectedState(true);
	}
	else{
		CheckBox_effect->setSelectedState(false);
	}
	CheckBox_bgm->addEventListenerCheckBox(this, checkboxselectedeventselector(Config::checkSelectEvent_bg)); //添加监听
	CheckBox_effect->addEventListenerCheckBox(this, checkboxselectedeventselector(Config::checkSelectEvent_effect));
	//滑动条
	Slider * Slider_bgm = (Slider *)Helper::seekWidgetByName(UI3, "Slider_10");  //获取对象
	Slider * Slider_effect = (Slider *)Helper::seekWidgetByName(UI3, "Slider_10_0");
	Slider_bgm->setPercent(BGMVolume);  //初始状态
	Slider_effect->setPercent(EffectVolume);
	Slider_bgm->addEventListenerSlider(this, sliderpercentchangedselector(Config::sliderPercentChangeEvent_bg)); //监听
	Slider_effect->addEventListenerSlider(this, sliderpercentchangedselector(Config::sliderPercentChangeEvent_effect));
	return true;
}
Config::~Config()
{
	object->stop();
}