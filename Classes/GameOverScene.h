#ifndef __BLOCKBREAKER_GAMEOVERSCENE_H__
#define __BLOCKBREAKER_GAMEOVERSCENE_H__
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CCActionManagerEx.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "res.h"
#include "MainMenu.h"
USING_NS_CC;

using namespace cocos2d::ui;
using namespace cocostudio;

class GameOverScene : public cocos2d::Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameOverScene);
	void onClick_OK(Ref*, TouchEventType type);
};
class ClearScene : public cocos2d::Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ClearScene);
	void onClick_OK(Ref*, TouchEventType type);
};
#endif //__BLOCKBREAKER_GAMEOVERSCENE_H__