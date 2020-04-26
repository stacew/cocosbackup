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

		if (mPlay->bGameMenuPop == false)
		{
			mPlay->ChangeLife(-1000000000, true, false);
			mPlay->CheckGameOver();
		}
		else if (mPlay->bGameMenuPop == true)
		{
			mPlay->ShowHideMenu(false);
			//	mPlay->bGameOver = true;
			mPlay->ExitGame();
		}
	}


	//chunrans4 release 때 닫기.
	
#if DEVELOPER
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		Touch touch;
		touch.setTouchInfo(0, 768 * 0.25, 1024 * 0.5);
		onTouchBegan(&touch, NULL);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		Touch touch;
		touch.setTouchInfo(0, 768 * 0.75, 1024 * 0.5);
		onTouchBegan(&touch, NULL);
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		mPlay->p1.__ChangeLife__(100, true);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		mPlay->p1.__ChangeLife__(-30, false);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_X)
	{
		mPlay->__nStagePoint__ += 300;
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_C)
	{
		mPlay->__nStagePoint__ += 4500;
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_Q)
	{
		mdp.SaveGameStageLevel(mPlay->nGameNumber, 0);
		mPlay->__nStagePoint__ = 0;
		mdp.SaveGamePoint(mPlay->nGameNumber, 0);
	}
#endif

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

	if (mPlay->bExitScene == true) // *caution : below update()
		Exit();
}

void PlayScene::Exit()
{

	this->getEventDispatcher()->removeAllEventListeners();
	this->unscheduleUpdate();
	this->removeAllChildren();
	if (mdp.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Door);

	Director::getInstance()->replaceScene(MenuScene::createScene());
}