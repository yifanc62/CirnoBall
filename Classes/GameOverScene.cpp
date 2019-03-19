#include "GameOverScene.h"
#include "BlockBreaker.h"
#include "DataFileIO.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

void GameOverScene::onClick_OK(Ref*, TouchEventType type){
	auto UI1 = (Widget*)this->getChildByTag(120);
	TextField* m_input = static_cast<TextField*>(Helper::seekWidgetByName(UI1, "Name"));
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED: {
		if (isEffect) {
			SimpleAudioEngine::getInstance()->playEffect("b1.wav");
		}
		std::string path = FileUtils::getInstance()->getWritablePath() + "record.bin";
		Value score = Value(player->getScore());
		Value name = Value(m_input->getStringValue());
		Value difficulty = Value(Mode);
		bool isInsert = false;
		float lineNum = DataFileIO::getInstance()->getLineNum(path.c_str());
		FileData* data = DataFileIO::getInstance()->getData(path.c_str());
		ValueVector newLine;
		newLine.push_back(score);
		newLine.push_back(name);
		newLine.push_back(difficulty);
		for (int i = 0; i < lineNum; i++){
			if (DataFileIO::getInstance()->getRecordScore(i, path.c_str()) < player->getScore()) {
				data->insertLineData(i, newLine);
				isInsert = true;
				break;
			}
		}
		if (!isInsert) {
			data->addLineData(newLine);
		}
		if (lineNum > 7) {
			data->deleteEndLineData();
		}
		DataFileIO::getInstance()->writeFile(path.c_str(), data);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, MainMenu::createScene()));
		break;
	}
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
bool GameOverScene::init() {
	if (!Layer::init()) {
		return false;
	}
	Widget* UI1 = GUIReader::getInstance()->widgetFromJsonFile("shuru_1.ExportJson");
	UI1->setTag(120);
	this->addChild(UI1);
	TextField* m_input = static_cast<TextField*>(Helper::seekWidgetByName(UI1, "Name"));
	m_input->setTouchSize(Size(450, 90));
	m_input->setTouchAreaEnabled(true);
	m_input->setCursorEnabled(true);
	m_input->setFontSize(15);
	m_input->setText("Cirno");
	//获取控件对象
	Button* OK = (Button*)Helper::seekWidgetByName(UI1, "OK");
	//添加按钮单击监听
	OK->addTouchEventListener(this, toucheventselector(GameOverScene::onClick_OK));

	Value score = Value(player->getScore());
	auto scoreLabel = Label::createWithTTF(score.asString(), "fonts/BRLNSR.TTF", 115);
	scoreLabel->setPosition(Vec2(798, 433));
	scoreLabel->setColor(Color3B(68, 149, 238));
	this->addChild(scoreLabel,10);
	return true;
}
Scene* GameOverScene::createScene(){
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	scene->addChild(layer);
	if (isBGM){
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Lose1.wav", true);
	}
	return scene;
}
void ClearScene::onClick_OK(Ref*, TouchEventType type){
	auto UI_Clear = (Widget*)this->getChildByTag(154);
	TextField* m_input = static_cast<TextField*>(Helper::seekWidgetByName(UI_Clear, "name"));
	std::string text;
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED: {
		if (isEffect) {
			SimpleAudioEngine::getInstance()->playEffect("b1.wav");
		}
		std::string path = FileUtils::getInstance()->getWritablePath() + "record.bin";
		Value score = Value(player->getScore());
		Value name = Value(m_input->getStringValue());
		Value difficulty = Value(Mode);
		bool isInsert = false;
		float lineNum = DataFileIO::getInstance()->getLineNum(path.c_str());
		FileData* data = DataFileIO::getInstance()->getData(path.c_str());
		ValueVector newLine;
		newLine.push_back(score);
		newLine.push_back(name);
		newLine.push_back(difficulty);
		for (int i = 0; i < lineNum; i++){
			if (DataFileIO::getInstance()->getRecordScore(i, path.c_str()) < player->getScore()) {
				data->insertLineData(i, newLine);
				isInsert = true;
				break;
			}
		}
		if (!isInsert) {
			data->addLineData(newLine);
		}
		if (lineNum > 7) {
			data->deleteEndLineData();
		}
		DataFileIO::getInstance()->writeFile(path.c_str(), data);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		Director::getInstance()->replaceScene(
			TransitionFade::create(0.5f, MainMenu::createScene()));
		break;
	}
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;
	}
}
bool ClearScene::init() {
	if (!Layer::init())
	{
		return false;
	}

	Widget* UI_Clear = GUIReader::getInstance()->widgetFromJsonFile("Win_1.ExportJson");
	UI_Clear->setTag(154);
	this->addChild(UI_Clear);
	TextField* m_input = static_cast<TextField*>(Helper::seekWidgetByName(UI_Clear, "name"));
	m_input->setTouchSize(Size(450, 90));
	m_input->setTouchAreaEnabled(true);
	m_input->setCursorEnabled(true);
	m_input->setFontSize(15);
	m_input->setText("Cirno");
	//获取控件对象
	Button* OK = (Button*)Helper::seekWidgetByName(UI_Clear, "ok");
	//添加按钮单击监听
	OK->addTouchEventListener(this, toucheventselector(ClearScene::onClick_OK));

	Value score = Value(player->getScore());
	auto scoreLabel = Label::createWithTTF(score.asString(), "fonts/BRLNSR.TTF", 100);
	scoreLabel->setPosition(Vec2(798, 430));
	scoreLabel->setColor(Color3B(255, 255, 255));
	this->addChild(scoreLabel, 10);
	return true;
}
Scene* ClearScene::createScene(){
	auto scene = Scene::create();
	auto layer = ClearScene::create();
	scene->addChild(layer);
	if (isBGM){
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Win1.wav", true);
	}
	return scene;
}