#include "TwoPlayScene.h"
#include "../Util/define.h"
#include "SimpleAudioEngine.h"
#include "../MenuScene.h"
#include "TwoPlayFunc.h"

Scene* TwoPlayScene::createScene()
{
	auto *scene = Scene::create();

	auto *layer = TwoPlayScene::create();
	scene->addChild(layer);

	return scene;
}

void TwoPlayScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	// Back button pressed
	if (keyCode == EventKeyboard::KeyCode::KEY_Z
		|| keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
		|| keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{

	}

}

bool TwoPlayScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	auto director = Director::getInstance();

	auto k_listener = EventListenerKeyboard::create();
	k_listener->onKeyReleased = CC_CALLBACK_2(TwoPlayScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(k_listener, 1);


	mPlay = new TwoPlayFunc();
	mPlay->init(this);

	//터치 리스너
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TwoPlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TwoPlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TwoPlayScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(TwoPlayScene::onTouchCancelled, this);
	director->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	// 스케줄
	this->scheduleUpdate();

	return true;
}

TwoPlayScene::~TwoPlayScene()
{
	delete mPlay;
}

bool TwoPlayScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	return mPlay->onTouchBegan(touch, unused_event);
}
void TwoPlayScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	mPlay->onTouchMoved(touch, unused_event);
}
void TwoPlayScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	mPlay->onTouchEnded(touch, unused_event);
}
void TwoPlayScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
	mPlay->onTouchCancelled(touch, unused_event);
}

void TwoPlayScene::update(float delta)
{
	mPlay->update();

	if (mPlay->bExitScene == true) // *caution : below update()
		Exit();
}

void TwoPlayScene::Exit()
{
	this->getEventDispatcher()->removeAllEventListeners();
	this->unscheduleUpdate();
	this->removeAllChildren();
	if (mdp.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Door);

	Director::getInstance()->replaceScene(MenuScene::createScene());
}