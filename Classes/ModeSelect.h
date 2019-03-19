#ifndef __MODESELECT_H__
#define __MODESELECT_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CCActionManagerEx.h"
USING_NS_CC;

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;

using namespace cocos2d;
class ModeSelect : public Layer {
public:
	static Scene* createScene();
	~ModeSelect();
	virtual bool init();
	CREATE_FUNC(ModeSelect);
	//onClickÉùÃ÷
	void onClick_Easy(Ref*, TouchEventType type);
	void onClick_Normal(Ref*, TouchEventType type);
	void onClick_Lunatic(Ref*, TouchEventType type);
	void onClick_back(Ref*, TouchEventType type);
	ActionObject *object;
};
#endif