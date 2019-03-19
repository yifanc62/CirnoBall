#include "Score.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "res.h"
#include "DataFileIO.h"
using namespace cocostudio;

using namespace cocos2d::extension;
USING_NS_CC;

void Score::onClick2(Ref*, TouchEventType type){
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
Scene* Score::createScene(){
	auto scene = Scene::create();
	auto layer = Score::create();
	scene->addChild(layer);
	return scene;
}
bool Score::init(){
	if (!Layer::init())
	{
		return false;
	}
	//加载分数排行UI
	auto UI2 = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("IQ_1.ExportJson");
	this->addChild(UI2);
	//加载UI动画
	object = ActionManagerEx::getInstance()->playActionByName("IQ_1.ExportJson", "Animation0");
	//获取控件对象
	Button* Button_5 = (Button*)Helper::seekWidgetByName(UI2, "Button_5");
	//添加按钮单击监听
	Button_5->addTouchEventListener(this, toucheventselector(Score::onClick2));
	Label* rankTitle = Label::createWithTTF("Rank", "fonts/ARLRDBD.TTF", 45);
	rankTitle->setPosition(Vec2(80, 580));
	rankTitle->setColor(Color3B(100, 131, 186));
	Label* nameTitle = Label::createWithTTF("Name", "fonts/ARLRDBD.TTF", 45);
	nameTitle->setPosition(Vec2(270, 580));
	nameTitle->setColor(Color3B(100, 131, 186));
	Label* scoreTitle = Label::createWithTTF("Score", "fonts/ARLRDBD.TTF", 45);
	scoreTitle->setPosition(Vec2(460, 580));
	scoreTitle->setColor(Color3B(100, 131, 186));
	Label* diffTitle = Label::createWithTTF("Difficulty", "fonts/ARLRDBD.TTF", 45);
	diffTitle->setPosition(Vec2(650, 580));
	diffTitle->setColor(Color3B(100, 131, 186));
	this->addChild(rankTitle);
	this->addChild(nameTitle);
	this->addChild(scoreTitle);
	this->addChild(diffTitle);
	std::string path = FileUtils::getInstance()->getWritablePath() + "record.bin";
	DataFileIO::getInstance()->loadFile(path.c_str());
	for (int i = 0; i < DataFileIO::getInstance()->getLineNum(path.c_str()); i++) {
		Value rank = Value(i + 1);
		Value score = Value(DataFileIO::getInstance()->getRecordScore(i, path.c_str()));
		Value name = Value(DataFileIO::getInstance()->getRecordName(i, path.c_str()));
		Value difficulty = Value(DataFileIO::getInstance()->getRecordDifficulty(i, path.c_str()));
		Label* rankLabel = Label::createWithTTF(rank.asString(), "fonts/ARLRDBD.TTF", 35);
		Label* nameLabel = Label::createWithTTF(name.asString(), "fonts/ARLRDBD.TTF", 35);
		Label* scoreLabel = Label::createWithTTF(score.asString(), "fonts/ARLRDBD.TTF", 35);
		Label* diffLabel = NULL;
		if (difficulty.asInt() == 1) {
			diffLabel = Label::createWithTTF("Easy", "fonts/ARLRDBD.TTF", 37);
		}
		else if (difficulty.asInt() == 2) {
			diffLabel = Label::createWithTTF("Normal", "fonts/ARLRDBD.TTF", 37);
		}
		else if (difficulty.asInt() == 3) {
			diffLabel = Label::createWithTTF("Lunatic", "fonts/ARLRDBD.TTF", 37);
		}
		else {
			diffLabel = Label::createWithTTF("SB", "fonts/ARLRDBD.TTF", 37);
		}
		rankLabel->setPosition(Vec2(80, 580 - (i + 1) * 47));
		nameLabel->setPosition(Vec2(270, 580 - (i + 1) * 47));
		scoreLabel->setPosition(Vec2(460, 580 - (i + 1) * 47));
		diffLabel->setPosition(Vec2(650, 580 - (i + 1) * 47));
		rankLabel->setColor(Color3B(106, 178, 190));
		nameLabel->setColor(Color3B(106, 178, 190));
		scoreLabel->setColor(Color3B(106, 178, 190));
		diffLabel->setColor(Color3B(106, 178, 190));
		this->addChild(rankLabel);
		this->addChild(nameLabel);
		this->addChild(scoreLabel);
		this->addChild(diffLabel);
	}
	return true;
}

Score::~Score()
{
	object->stop();
}