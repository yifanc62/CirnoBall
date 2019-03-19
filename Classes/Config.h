#ifndef __Config_H__
#define __Config_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
#include "cocostudio/CCActionManagerEx.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;

using namespace cocos2d;
class Config : public Layer {
public:
	~Config();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Config);
	//onClickÉùÃ÷
	void onClick3(Ref*, TouchEventType type);
	void checkSelectEvent_bg(Ref *pSender, CheckBoxEventType eventtype);
	void checkSelectEvent_effect(Ref *pSender, CheckBoxEventType eventtype);
	void sliderPercentChangeEvent_bg(Ref*pSender, SliderEventType type);
	void sliderPercentChangeEvent_effect(Ref*pSender, SliderEventType type);
	ActionObject *object;
};
#endif