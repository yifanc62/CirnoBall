#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CCActionManagerEx.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "cocostudio/CCArmature.h"

using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class MainMenu : public cocos2d::Layer
{
public:
	~MainMenu();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
	//onClick声明
	void onClick_Score(Ref*, TouchEventType type);
	void onClick_Config(Ref*, TouchEventType type);
	void onClick_Exit(Ref*, TouchEventType type);
	void onClick_Start(Ref*, TouchEventType type);
	void onClick_About(Ref*, TouchEventType type);
	ActionObject *object;
	ActionObject *objectAni;
	void movementCallback(Armature * armature, MovementEventType movementType, const std::string& movementID);
};

#endif // __MAINMENU_SCENE_H__
