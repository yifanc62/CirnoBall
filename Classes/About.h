#ifndef __ABOUT_H__
#define __ABOUT_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CCActionManagerEx.h"
USING_NS_CC;

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;

class About : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(About);
	//onClick声明
	void onClick(Ref*, TouchEventType type);
};
#endif