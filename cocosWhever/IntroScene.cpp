#include "IntroScene.h"
#include "MenuScene.h"
#include "Util/define.h"
#include "Util/MyDataManager.h"
#include "SimpleAudioEngine.h"

Scene* IntroScene::createScene()
{
	auto *scene = Scene::create();

	auto *layer = IntroScene::create();
	scene->addChild(layer);

	return scene;
}


void IntroScene::initBG()
{
	Sprite* bgCircle = NULL;
	if( mdp.bExtreme == true )
		bgCircle = Sprite::createWithSpriteFrameName("bgCircle2.png");
	else
		bgCircle = Sprite::createWithSpriteFrameName("bgCircle.png");
	bgCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bgCircle->setPosition(gameSize.width*0.5, gameSize.height*0.5);
	bgCircle->setScale(7.0f);
	bgCircle->setTag(GAME_BG_CIRCLE);
	this->addChild(bgCircle, 500);

	auto bg2 = Sprite::createWithSpriteFrameName("introIcon.png");
	bg2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bg2->setPosition(gameSize.width*0.5, gameSize.height*0.5);
	bg2->setScale(1.05f);
	this->addChild(bg2, 501);

}


bool IntroScene::init()
{
	if (!LayerColor::initWithColor(Color4B::WHITE))
	{
		return false;
	}

	mdp.LoadFullData();
	if (mdp.bFirstGame == true)
	{
		mdp.init();
		mdp.SaveFirstGame();
	}
	srand(time(NULL));
	CCRANDOM_0_1();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(_image_plist);

	auto director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	gameSize = Size(visibleSize.width, visibleSize.height - AD_HEIGHT_SIZE);

	bEnabledTouch = true;
	initBG();


	/*auto label1 = Label::create(); //createWithTTF("Whever", _fonts_Base, 120, Size(visibleSize.width, 0), TextHAlignment::CENTER);
	label1->setSystemFontSize(120);
	label1->setAlignment(TextHAlignment::CENTER);
	label1->setString("Whever");
	//	if (mdp.bEnglish == false)
	//	{
	//		label1->setString(UTF8PCHAR("Whever"));
	//	}
	label1->setTextColor(Color4B::WHITE);
	label1->enableOutline(Color4B::BLACK, 2);
	label1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	label1->setPosition(Vec2(visibleSize.width *0.52, visibleSize.height * 0.5));
	label1->setTag(10001);
	this->addChild(label1);
	*/


	auto label2 = Label::create();// createWithTTF("Touch To Start", _fonts_Base, 40, Size(visibleSize.width, 0), TextHAlignment::CENTER);
	label2->setSystemFontSize(40);
	label2->setAlignment(TextHAlignment::CENTER);
	label2->setString("Touch To Start");
	if (mdp.bEnglish == false)
	{
		label2->setString(UTF8PCHAR("터치하여 시작하세요"));
	}
	label2->setTextColor(Color4B::BLACK);
	label2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	label2->setPosition(Vec2(visibleSize.width *0.5, 128));
	label2->setTag(10002);
	this->addChild(label2, 2);
	

	auto label3 = Label::create();// createWithTTF("Touch To Start", _fonts_Base, 40, Size(visibleSize.width, 0), TextHAlignment::CENTER);
	label3->setSystemFontSize(20);
	label3->setAlignment(TextHAlignment::CENTER);
	label3->setString("OneButton Games\nBGM composer 'Syncopika'\nIcon 'Kenny'");
	if (mdp.bEnglish == false)
	{
		label2->setString(UTF8PCHAR("터치하여 시작하세요"));
	}
	label3->setTextColor(Color4B::BLACK);
	label3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	label3->setPosition(Vec2(visibleSize.width *0.5, 32));
	this->addChild(label3, 2);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(IntroScene::onTouchBegan, this);
	director->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	return true;
}


void IntroScene::scheduleCallback(float delta)
{
	NextScene();
}

void IntroScene::Callback(Ref* sender, int nType)
{
	 if (nType == 1)
	{
		if (mdp.bSoundSwitch == true)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
		}
		NextScene();
	}

}


bool IntroScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (bEnabledTouch == false)
		return false;

	bEnabledTouch = false;

	if (mdp.bSoundSwitch == true)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
	}

	auto label2 = (Label*)this->getChildByTag(10002);
	label2->setVisible(false);



	float nTime = 1.0f;
	float nTime2 = 1.4f;

	auto bgCircle = (Sprite*)this->getChildByTag(GAME_BG_CIRCLE);
	bgCircle->runAction(ScaleTo::create(nTime, 1.0f));


	auto actionNext = Sequence::create(
		DelayTime::create(nTime2),
		CallFuncN::create(CC_CALLBACK_1(IntroScene::Callback, this, 1)),
		NULL);
	this->runAction(actionNext);

	return false;
}

void IntroScene::NextScene()
{
	this->getEventDispatcher()->removeAllEventListeners();
	this->unschedule(schedule_selector(IntroScene::scheduleCallback));
	this->removeAllChildren();

//	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MenuScene::createScene(), Color3B::BLACK));
	Director::getInstance()->replaceScene(MenuScene::createScene());
}