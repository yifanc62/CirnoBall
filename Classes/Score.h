#ifndef __SCORE_H__
#define __SCORE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CCActionManagerEx.h"
USING_NS_CC;

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;

using namespace cocos2d;
class Score : public Layer {
public:
	static Scene* createScene();
	~Score();
	virtual bool init();
	CREATE_FUNC(Score);
	//onClick声明
	void onClick2(Ref*, TouchEventType type);
	ActionObject *object;
};
#endif