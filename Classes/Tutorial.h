#ifndef __TUTORIAL_H__
#define __TUTORIAL_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CCActionManagerEx.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;

class Tutorial : public Layer {
public:
	static Scene* createScene();
	~Tutorial();
	virtual bool init();
	CREATE_FUNC(Tutorial);
	//onClickÉùÃ÷
	void onClick_back(Ref*, TouchEventType type);
	void onClick_start(Ref*, TouchEventType type);
	ActionObject *object;
};
#endif