#include "Config.h"
#include "MainMenu.h"
#include "res.h"
using namespace cocostudio;
using namespace CocosDenshion;
#include "cocos-ext.h"
using namespace cocos2d::extension;
USING_NS_CC;

//���ذ�ť
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
//bgm��ѡ��
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
//��Ч��ѡ��
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
//bgm������
void Config::sliderPercentChangeEvent_bg(Ref*pSender, SliderEventType type){
	if (type == SLIDER_PERCENTCHANGED){
		Slider* pSlider = (Slider*)pSender;
		BGMVolume = pSlider->getPercent();
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(BGMVolume / 100);
	}
}
//��Ч������
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
	//Ԥ����bgm
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("MainMenu.wav");
	return scene;
}
bool Config::init(){
	if (!Layer::init())
	{
		return false;
	}
	//���ط�������UI
	auto UI3 = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("Config_1.ExportJson");
	this->addChild(UI3);
	//����UI����
	object = ActionManagerEx::getInstance()->playActionByName("Config_1.ExportJson", "Animation0");
	//��ť
	Button* Button_11 = (Button*)Helper::seekWidgetByName(UI3, "Button_11");//��ȡ�ؼ�����
	Button_11->addTouchEventListener(this, toucheventselector(Config::onClick3));	//��Ӱ�ť��������

	//��ѡ��
	CheckBox* CheckBox_bgm = (CheckBox*)Helper::seekWidgetByName(UI3, "CheckBox_6");  //��ȡ�ؼ�����
	CheckBox* CheckBox_effect = (CheckBox*)Helper::seekWidgetByName(UI3, "CheckBox_5");
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){  //���ó�ʼ״̬
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
	CheckBox_bgm->addEventListenerCheckBox(this, checkboxselectedeventselector(Config::checkSelectEvent_bg)); //��Ӽ���
	CheckBox_effect->addEventListenerCheckBox(this, checkboxselectedeventselector(Config::checkSelectEvent_effect));
	//������
	Slider * Slider_bgm = (Slider *)Helper::seekWidgetByName(UI3, "Slider_10");  //��ȡ����
	Slider * Slider_effect = (Slider *)Helper::seekWidgetByName(UI3, "Slider_10_0");
	Slider_bgm->setPercent(BGMVolume);  //��ʼ״̬
	Slider_effect->setPercent(EffectVolume);
	Slider_bgm->addEventListenerSlider(this, sliderpercentchangedselector(Config::sliderPercentChangeEvent_bg)); //����
	Slider_effect->addEventListenerSlider(this, sliderpercentchangedselector(Config::sliderPercentChangeEvent_effect));
	return true;
}
Config::~Config()
{
	object->stop();
}