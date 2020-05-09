#include "PlayScene.h"
#include "../Util/define.h"
#include "../Util/MyDataManager.h"
#include "SimpleAudioEngine.h"
#include "../MenuScene.h"
#include "PlayFunc.h"

Scene* PlayScene::createScene()
{
	auto *scene = Scene::create();

	auto *layer = PlayScene::create();
	scene->addChild(layer);

	return scene;
}



void PlayScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	// Back button pressed
	if (keyCode == EventKeyboard::KeyCode::KEY_Z
		|| keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
		|| keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		mPlay->PauseExit(NULL);
	}
}


bool PlayScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	auto director = Director::getInstance();

	auto k_listener = EventListenerKeyboard::create();
	k_listener->onKeyReleased = CC_CALLBACK_2(PlayScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(k_listener, 1);


	mPlay = new PlayFunc();
	mPlay->init(this);

	//터치 리스너
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(PlayScene::onTouchCancelled, this);
	director->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	// 스케줄
	this->scheduleUpdate();

	mdp.nAddCount--;
	if (mdp.nAddCount < 0)
		mdp.nAddCount = 0;

	mdp.SaveAddCount();

	return true;
}

PlayScene::~PlayScene()
{
	delete mPlay;
}

bool PlayScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	return mPlay->onTouchBegan(touch, unused_event);
}
void PlayScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	mPlay->onTouchMoved(touch, unused_event);
}
void PlayScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	mPlay->onTouchEnded(touch, unused_event);
}
void PlayScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
	mPlay->onTouchCancelled(touch, unused_event);
}

void PlayScene::update(float delta)
{
	mPlay->update();

	if (mPlay->bGameOver == true) // *caution : the bottom of update
		Exit();
}

void PlayScene::Exit()
{
	this->getEventDispatcher()->removeAllEventListeners();
	this->unscheduleUpdate();
	this->removeAllChildren();
	Director::getInstance()->replaceScene(TransitionFade::create(0.1f, MenuScene::createScene()));
}