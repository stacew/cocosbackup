#include "MenuScene.h"
#include "GamePlay/PlayScene.h"
#include "GamePlay/TwoPlayScene.h"
#include "SimpleAudioEngine.h"

#include "gpgs/GameSharing.h"


Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}


class Parent
{
	int a;

	virtual void A1() {}
	void A2() {}
	virtual void A3() {}
	virtual void A4() {}
	virtual void A5() {}
};

class Child : public Parent
{
	int c;

	void A1() {}
	virtual void A2() {}
	virtual void A3() {}
	void A4() {}
	void A5() {}
	virtual void A6() {}
};

class CChi : public Child
{
	int d;

	void A2() {}
	void A3() {}
	void A6() {}

};


void MenuScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	Parent* pp = new Parent;
	Parent* cc = new Child;
	Parent* ccc = new CChi;
	Parent* ccc2 = new CChi;
	Parent* ccc3 = new CChi;
	if (this)
	{

	}


	// Back button pressed
	if (keyCode == EventKeyboard::KeyCode::KEY_Z
		|| keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
		|| keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{

		if (bExtend == true)
		{
			ExtendNote();
		}
		else
		{
			nKeyBackSpace--;
			if (nKeyBackSpace == 0)
			{
				ExitGame();
			}
			else
			{
				ToastMessage(StringUtils::format(mdp.bEnglish ? "Press the Back key %d times to exit." : UTF8PCHAR("뒤로가기를 %d번 더 누르면 종료합니다."), nKeyBackSpace));
			}
		}
	}
}

bool MenuScene::init()
{
	//
	nEventCount = 0;
	//
	bReadyAd = false;
	ReadyAd();
	ReadyBanner();
	ShowBanner();


	nKeyBackSpace = 2;

	if (!Layer::init())
	{
		return false;
	}

	auto director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	gameSize = Size(visibleSize.width, visibleSize.height - AD_HEIGHT_SIZE);

	auto k_listener = EventListenerKeyboard::create();
	k_listener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(k_listener, 1);


	initBG();
	initBGM();
	initSound();

	/////////////////////////////
	////////////////////////////////
	//사이드 메뉴
	auto exit = Sprite::createWithSpriteFrameName("exit.png");
	auto exit_s = Sprite::createWithSpriteFrameName("exit_s.png");

	auto extremeOn = Sprite::createWithSpriteFrameName("star3.png");
	auto extremeOn_s = Sprite::createWithSpriteFrameName("star3_s.png");
	auto extremeOff = Sprite::createWithSpriteFrameName("star2.png");
	auto extremeOff_s = Sprite::createWithSpriteFrameName("star2_s.png");

	auto exitItem = MenuItemSprite::create(exit, exit_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	exitItem->setScale(SIDE_SCALE);
	exitItem->setTag(SIDE_EXIT);

	auto extremeOnItem = MenuItemSprite::create(extremeOn, extremeOn_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	auto extremeOffItem = MenuItemSprite::create(extremeOff, extremeOff_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	auto extremeToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this, 0), extremeOnItem, extremeOffItem, NULL);
	if (mdp.bExtreme == true)
		extremeToggle->setSelectedIndex(0);
	else
		extremeToggle->setSelectedIndex(1);

	extremeToggle->setScale(SIDE_SCALE);
	extremeToggle->setTag(SIDE_EXTREME);

	auto menuSideUp = Menu::create(exitItem, extremeToggle, NULL);
	menuSideUp->setTag(SIDE_MENU_UP);
	menuSideUp->setPosition(gameSize.width * 0.5, gameSize.height * MENU_SIDE_UP_HEIGHT);
	menuSideUp->alignItemsHorizontallyWithPadding(400);
	this->addChild(menuSideUp, 950);

	///사이드 메뉴
	/////////////////////////////




	///////////////////////////////
	auto more = Sprite::createWithSpriteFrameName("more.png");
	auto more_s = Sprite::createWithSpriteFrameName("more_s.png");

	auto note = Sprite::createWithSpriteFrameName("extend.png");
	auto note_s = Sprite::createWithSpriteFrameName("extend_s.png");

	auto leaderboard = Sprite::createWithSpriteFrameName("leaderboard.png");
	auto home = Sprite::createWithSpriteFrameName("home.png");
	auto facebook = Sprite::createWithSpriteFrameName("facebook.png");
	auto musicOn = Sprite::createWithSpriteFrameName("musicOn.png");
	auto musicOff = Sprite::createWithSpriteFrameName("musicOff.png");
	auto audioOn = Sprite::createWithSpriteFrameName("audioOn.png");
	auto audioOff = Sprite::createWithSpriteFrameName("audioOff.png");

	auto leaderboard_s = Sprite::createWithSpriteFrameName("leaderboard_s.png");
	auto home_s = Sprite::createWithSpriteFrameName("home_s.png");
	auto facebook_s = Sprite::createWithSpriteFrameName("facebook_s.png");
	auto musicOn_s = Sprite::createWithSpriteFrameName("musicOn_s.png");
	auto musicOff_s = Sprite::createWithSpriteFrameName("musicOff_s.png");
	auto audioOn_s = Sprite::createWithSpriteFrameName("audioOn_s.png");
	auto audioOff_s = Sprite::createWithSpriteFrameName("audioOff_s.png");


	auto ko = Sprite::createWithSpriteFrameName("ko.png");
	auto ko_s = Sprite::createWithSpriteFrameName("ko_s.png");
	auto en = Sprite::createWithSpriteFrameName("en.png");
	auto en_s = Sprite::createWithSpriteFrameName("en_s.png");

	auto moreItem = MenuItemSprite::create(more, more_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	moreItem->setScale(SIDE_SCALE);
	moreItem->setTag(SIDE_MORE);

	auto noteItem = MenuItemSprite::create(note, note_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	noteItem->setScale(SIDE_SCALE);
	noteItem->setTag(SIDE_NOTE);


	auto leaderboardItem = MenuItemSprite::create(leaderboard, leaderboard_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	leaderboardItem->setScale(SIDE_SCALE);
	leaderboardItem->setTag(SIDE_LEADERBOARD);

	auto homeItem = MenuItemSprite::create(home, home_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	homeItem->setScale(SIDE_SCALE);
	homeItem->setTag(SIDE_HOME);

	auto facebookItem = MenuItemSprite::create(facebook, facebook_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	facebookItem->setScale(SIDE_SCALE);
	facebookItem->setTag(SIDE_FACEBOOK);

	auto musicOnItem = MenuItemSprite::create(musicOn, musicOn_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	auto musicOffItem = MenuItemSprite::create(musicOff, musicOff_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	MenuItemToggle* musicToggle = NULL;
	if (mdp.bBGMSwitch == true)
		musicToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this, 0), musicOnItem, musicOffItem, NULL);
	else
		musicToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this, 0), musicOffItem, musicOnItem, NULL);
	musicToggle->setScale(SIDE_SCALE);
	musicToggle->setTag(SIDE_MUSIC);

	auto audioOnItem = MenuItemSprite::create(audioOn, audioOn_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	auto audioOffItem = MenuItemSprite::create(audioOff, audioOff_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	MenuItemToggle* audioToggle = NULL;
	if (mdp.bSoundSwitch == true)
		audioToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this, 0), audioOnItem, audioOffItem, NULL);
	else
		audioToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this, 0), audioOffItem, audioOnItem, NULL);
	audioToggle->setScale(SIDE_SCALE);
	audioToggle->setTag(SIDE_AUDIO);

	auto koItem = MenuItemSprite::create(ko, ko_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	auto enItem = MenuItemSprite::create(en, en_s, CC_CALLBACK_1(MenuScene::SideMenu, this, 0));
	MenuItemToggle* langToggle = NULL;
	if (mdp.bEnglish == false)
		langToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this, 0), koItem, enItem, NULL);
	else
		langToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this, 0), enItem, koItem, NULL);
	langToggle->setScale(SIDE_SCALE);
	langToggle->setTag(SIDE_LANG);

	auto menuSideDown = Menu::create(
		moreItem,
		noteItem,
		homeItem,
		facebookItem,
		leaderboardItem,
		musicToggle,
		audioToggle,
		langToggle,
		NULL);
	menuSideDown->setTag(SIDE_MENU_DOWN);
	menuSideDown->alignItemsHorizontallyWithPadding(10);
	menuSideDown->setPosition(gameSize.width * 0.5, gameSize.height * MENU_SIDE_DOWN_HEIGHT);

	this->addChild(menuSideDown, 950);

#if USE_LEADERBOARD == 1
	menuSideDown->getChildByTag(SIDE_LEADERBOARD)->setVisible(true);
#else
	menuSideDown->getChildByTag(SIDE_LEADERBOARD)->setVisible(false);
#endif




	//터치 리스너
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MenuScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MenuScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MenuScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(MenuScene::onTouchCancelled, this);
	director->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
	bPrevTouch = false;


	// 스케줄
	this->scheduleUpdate();

	//Update
	{
		bExtend = false;
		InitMenu();
		UpdateMenu();
	}

	initBGAction();

	return true;
}

void MenuScene::update(float delta)
{

}
void MenuScene::InitMenu()
{

	//Two Player
	{
		Menu* towPlayerMenu[PAGE_COUNT] = { NULL };
		for (int p = 0; p < PAGE_COUNT; p++)
		{
			auto twoPlayer1 = Sprite::createWithSpriteFrameName("2p.png");
			auto twoPlayer1_s = Sprite::createWithSpriteFrameName("2p_s.png");
			auto twoPlayer2 = Sprite::createWithSpriteFrameName("2p.png");
			auto twoPlayer2_s = Sprite::createWithSpriteFrameName("2p_s.png");
			auto twoPlayer3 = Sprite::createWithSpriteFrameName("2p.png");
			auto twoPlayer3_s = Sprite::createWithSpriteFrameName("2p_s.png");
			auto twoPlayer4 = Sprite::createWithSpriteFrameName("2p.png");
			auto twoPlayer4_s = Sprite::createWithSpriteFrameName("2p_s.png");
			auto twoPlayerItem1 = MenuItemSprite::create(twoPlayer1, twoPlayer1_s, CC_CALLBACK_1(MenuScene::menuCallback, this));
			twoPlayerItem1->setScale(0.8f);
			twoPlayerItem1->setTag(MENU_TWO_PLAYER_1);
			auto twoPlayerItem2 = MenuItemSprite::create(twoPlayer2, twoPlayer2_s, CC_CALLBACK_1(MenuScene::menuCallback, this));
			twoPlayerItem2->setScale(0.8f);
			twoPlayerItem2->setTag(MENU_TWO_PLAYER_2);
			auto twoPlayerItem3 = MenuItemSprite::create(twoPlayer3, twoPlayer3_s, CC_CALLBACK_1(MenuScene::menuCallback, this));
			twoPlayerItem3->setScale(0.8f);
			twoPlayerItem3->setTag(MENU_TWO_PLAYER_3);
			auto twoPlayerItem4 = MenuItemSprite::create(twoPlayer4, twoPlayer4_s, CC_CALLBACK_1(MenuScene::menuCallback, this));
			twoPlayerItem4->setScale(0.8f);
			twoPlayerItem4->setTag(MENU_TWO_PLAYER_4);

			towPlayerMenu[p] = Menu::create(twoPlayerItem1, twoPlayerItem2, twoPlayerItem3, twoPlayerItem4, NULL);
			towPlayerMenu[p]->alignItemsVerticallyWithPadding(65);

			twoPlayerBaseX = gameSize.width * 0.9f;
			int n = p - mdp.nPageView;
			towPlayerMenu[p]->setPosition(twoPlayerBaseX + gameSize.width * n, gameSize.height * MENU_HEIGHT_RATE);

			towPlayerMenu[p]->setTag(MENU_TWO_PLAYER + p);
			this->addChild(towPlayerMenu[p], 1);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	{
		//card
		Menu* cardMenu[PAGE_COUNT] = { NULL };
		for (int p = 0; p < PAGE_COUNT; p++)
		{
			auto card1 = Sprite::createWithSpriteFrameName("card1.png");
			auto card2 = Sprite::createWithSpriteFrameName("card2.png");
			auto card3 = Sprite::createWithSpriteFrameName("card3.png");
			auto card4 = Sprite::createWithSpriteFrameName("card4.png");
			auto card1_s = Sprite::createWithSpriteFrameName("card1_s.png");
			auto card2_s = Sprite::createWithSpriteFrameName("card2_s.png");
			auto card3_s = Sprite::createWithSpriteFrameName("card3_s.png");
			auto card4_s = Sprite::createWithSpriteFrameName("card4_s.png");
			auto card1_0 = Sprite::createWithSpriteFrameName("card0.png");
			auto card2_0 = Sprite::createWithSpriteFrameName("card0.png");
			auto card3_0 = Sprite::createWithSpriteFrameName("card0.png");
			auto card4_0 = Sprite::createWithSpriteFrameName("card0.png");
			auto card1Item = MenuItemSprite::create(card1, card1_s, card1_0, CC_CALLBACK_1(MenuScene::menuCallback, this));
			auto card2Item = MenuItemSprite::create(card2, card2_s, card2_0, CC_CALLBACK_1(MenuScene::menuCallback, this));
			auto card3Item = MenuItemSprite::create(card3, card3_s, card3_0, CC_CALLBACK_1(MenuScene::menuCallback, this));
			auto card4Item = MenuItemSprite::create(card4, card4_s, card4_0, CC_CALLBACK_1(MenuScene::menuCallback, this));
			card1Item->setTag(MENU_GAME1);
			card2Item->setTag(MENU_GAME2);
			card3Item->setTag(MENU_GAME3);
			card4Item->setTag(MENU_GAME4);
			cardMenu[p] = Menu::create(card1Item, card2Item, card3Item, card4Item, NULL);
			cardMenu[p]->alignItemsVerticallyWithPadding(24);
		
			MenuBaseX = gameSize.width * 0.5;
			int n = p - mdp.nPageView;
			cardMenu[p]->setPosition(MenuBaseX + gameSize.width * n, gameSize.height * MENU_HEIGHT_RATE);
			
			cardMenu[p]->setTag(MENU_MENU + p);
			this->addChild(cardMenu[p], 1);

		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	//card Label
	Label* labelCard1[PAGE_COUNT][4] = { NULL };
	Label* labelCard2[PAGE_COUNT][4] = { NULL };
	Label* labelCard3[PAGE_COUNT][4] = { NULL };
	Menu* cardStar[PAGE_COUNT][4] = { NULL };
	float nLabelHeight = gameSize.height * MENU_HEIGHT_RATE + 60;
	MenuLabelBaseX = gameSize.width * 0.2f;
	MenuLabelBaseX2 = gameSize.width * 0.8f;
	float nInfoHeight = gameSize.height * MENU_HEIGHT_RATE + 8;
	MenuInfoBaseX = gameSize.width * 0.4f; //wkrdjq
	MenuLabelBaseX3 = gameSize.width * 0.8f;

	for (int p = 0; p < PAGE_COUNT; p++)
	{
		for (int i = 0; i < 4; i++)
		{
			int nGameNumber = p * 4 + i;
			labelCard1[p][i] = Label::create();//createWithTTF("", _fonts_Base, CARD_LABEL_SIZE);
			labelCard1[p][i]->setSystemFontSize(CARD_LABEL_SIZE);

			labelCard1[p][i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		//	if (nGameNumber >= GAME_COUNT)
		//		labelCard1[p][i]->setTextColor(Color4B::WHITE);
		//	else
		//	{
		//		labelCard1[p][i]->setTextColor(Color4B::BLACK);
		//	}
			
			int n = p - mdp.nPageView;
			labelCard1[p][i]->setPosition(MenuLabelBaseX + gameSize.width * n, nLabelHeight + 180 - (144 * i));
			labelCard1[p][i]->setTag(MENU_CARD_LABEL + nGameNumber);
			this->addChild(labelCard1[p][i], 2);
		}


		for (int i = 0; i < 4; i++)
		{
			int nGameNumber = p * 4 + i;
			labelCard2[p][i] = Label::create();// createWithTTF("", _fonts_Base, CARD_LABEL_SIZE2);
			labelCard2[p][i]->setSystemFontSize(CARD_LABEL_SIZE2);

			labelCard2[p][i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			labelCard2[p][i]->setTextColor(Color4B::BLACK);
			int n = p - mdp.nPageView;
			labelCard2[p][i]->setPosition(MenuLabelBaseX2 + gameSize.width * n, nLabelHeight + 180 - (144 * i));
			labelCard2[p][i]->setTag(MENU_CARD_LABEL2 + nGameNumber);
			this->addChild(labelCard2[p][i], 2);
		}


		for (int i = 0; i < 4; i++)
		{
			int nGameNumber = p * 4 + i;
			labelCard3[p][i] = Label::create();// createWithTTF("", _fonts_Base, CARD_LABEL_SIZE3);
			labelCard3[p][i]->setSystemFontSize(CARD_LABEL_SIZE3);

			labelCard3[p][i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			labelCard3[p][i]->setTextColor(Color4B::WHITE);
			int n = p - mdp.nPageView;
			labelCard3[p][i]->setPosition(MenuLabelBaseX3 + gameSize.width * n, nInfoHeight + 180 - (144 * i));
			labelCard3[p][i]->setTag(MENU_CARD_LABEL3 + nGameNumber);
			this->addChild(labelCard3[p][i], 2);
		}

		for (int i = 0; i < 4; i++)
		{
			int nGameNumber = p * 4 + i;

			MenuItemSprite* sprItem[MAX_STAR_PHASE] = { NULL };
			for (int j = 0; j < MAX_STAR_PHASE; j++)
			{
				Sprite* spr1 = Sprite::createWithSpriteFrameName("star1.png");
				Sprite* spr2 = Sprite::createWithSpriteFrameName("star2.png");
				Sprite* spr3 = Sprite::createWithSpriteFrameName("star3.png");
				sprItem[j] = MenuItemSprite::create(spr1, spr2, spr3);
				sprItem[j]->setEnabled(false);
				sprItem[j]->setTag(j);
				sprItem[j]->setScale(0.3f);
			}
			cardStar[p][i] = Menu::create(
				sprItem[0], sprItem[1], sprItem[2], sprItem[3], sprItem[4],
				sprItem[5],	sprItem[6],	sprItem[7],	NULL);
			cardStar[p][i]->alignItemsHorizontallyWithPadding(2);

			int n = p - mdp.nPageView;
			cardStar[p][i]->setPosition(MenuInfoBaseX + gameSize.width * n, nInfoHeight + 180 - (144 * i));
			cardStar[p][i]->setTag(MENU_CARD_STAR + nGameNumber);
			this->addChild(cardStar[p][i], 2);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	//page left right
	auto p1 = Sprite::createWithSpriteFrameName("page1.png");
	p1->setTag(MENU_PAGE1);
	this->addChild(p1,1);

	for (int p = 0; p < PAGE_COUNT; p++)
	{
		auto p2 = Sprite::createWithSpriteFrameName("page2.png");
		p2->setTag(MENU_PAGE2 + p);

		float n = p - (PAGE_COUNT / 2);
		p2->setPosition(Vec2(
			gameSize.width * 0.5
			+ (40 * n)
			+ ((PAGE_COUNT % 2 == 0) ? (40 * 0.5f) : 0) // 페이지 카운트 짝수 일때
			, gameSize.height * 0.18));
		this->addChild(p2);
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	//extend Note
	auto labelNoteName = Label::create();// createWithTTF("", _fonts_Base, NOTE_FONT_SIZE * 1.5f);
	labelNoteName->setSystemFontSize(NOTE_FONT_SIZE * 1.5f);

	labelNoteName->setTag(NOTE_NAME);
	labelNoteName->setPosition(gameSize.width *0.5f, gameSize.height*0.80f);
	this->addChild(labelNoteName, 1000);

	Label* labelNote[NOTE_LABEL_COUNT] = { NULL };
	for (int i = 0; i < NOTE_LABEL_COUNT; i++)
	{
		labelNote[i] = Label::create();// createWithTTF("", _fonts_Base, NOTE_FONT_SIZE);
		labelNote[i]->setSystemFontSize(NOTE_FONT_SIZE);

		labelNote[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

		labelNote[i]->setPosition( gameSize.width * 0.15f, gameSize.height * 0.7f - (i * 55));
		labelNote[i]->setTag(NOTE_LABEL + i);
		this->addChild(labelNote[i], 1000);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
}


void MenuScene::UpdateMenu()
{
	//
	{
		//card
		Menu* menuTwoPlayer[PAGE_COUNT] = { NULL };
		for (int p = 0; p < PAGE_COUNT; p++)
		{
			menuTwoPlayer[p] = (Menu*) this->getChildByTag(MENU_TWO_PLAYER + p);
			menuTwoPlayer[p]->setVisible(true);

			auto card1Item = (MenuItemSprite*)menuTwoPlayer[p]->getChildByTag(MENU_TWO_PLAYER_1);
			auto card2Item = (MenuItemSprite*)menuTwoPlayer[p]->getChildByTag(MENU_TWO_PLAYER_2);
			auto card3Item = (MenuItemSprite*)menuTwoPlayer[p]->getChildByTag(MENU_TWO_PLAYER_3);
			auto card4Item = (MenuItemSprite*)menuTwoPlayer[p]->getChildByTag(MENU_TWO_PLAYER_4);
			if (p * 4 + 0 >= GAME_COUNT || mdp.bTWO_PLAY[p * 4 + 0] == false)
				card1Item->setVisible(false);
			else
				card1Item->setVisible(true);

			if (p * 4 + 1 >= GAME_COUNT || mdp.bTWO_PLAY[p * 4 + 1] == false)
				card2Item->setVisible(false);
			else
				card2Item->setVisible(true);

			if (p * 4 + 2 >= GAME_COUNT || mdp.bTWO_PLAY[p * 4 + 2] == false)
				card3Item->setVisible(false);
			else
				card3Item->setVisible(true);

			if (p * 4 + 3 >= GAME_COUNT || mdp.bTWO_PLAY[p * 4 + 3] == false)
				card4Item->setVisible(false);
			else
				card4Item->setVisible(true);

			if (bExtend == true)
			{
				menuTwoPlayer[p]->setVisible(false);
			}

			int n = p - mdp.nPageView;
			menuTwoPlayer[p]->stopAllActions();
			menuTwoPlayer[p]->runAction(MoveTo::create(0.2f, Vec2(twoPlayerBaseX + gameSize.width * n, menuTwoPlayer[p]->getPositionY())));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	{
		//card
		Menu* cardMenu[PAGE_COUNT] = { NULL };
		for (int p = 0; p < PAGE_COUNT; p++)
		{
			cardMenu[p] = (Menu*) this->getChildByTag(MENU_MENU + p);
			cardMenu[p]->setVisible(true);

			auto card1Item = (MenuItemSprite*)cardMenu[p]->getChildByTag(MENU_GAME1);
			auto card2Item = (MenuItemSprite*)cardMenu[p]->getChildByTag(MENU_GAME2);
			auto card3Item = (MenuItemSprite*)cardMenu[p]->getChildByTag(MENU_GAME3);
			auto card4Item = (MenuItemSprite*)cardMenu[p]->getChildByTag(MENU_GAME4);
			if (p * 4 + 0 >= GAME_COUNT)
				card1Item->setEnabled(false);
			else
				card1Item->setEnabled(true);
			if (p * 4 + 1 >= GAME_COUNT)
				card2Item->setEnabled(false);
			else
				card2Item->setEnabled(true);
			if (p * 4 + 2 >= GAME_COUNT)
				card3Item->setEnabled(false);
			else
				card3Item->setEnabled(true);
			if (p * 4 + 3 >= GAME_COUNT)
				card4Item->setEnabled(false);
			else
				card4Item->setEnabled(true);

			if (bExtend == true)
			{
				cardMenu[p]->setVisible(false);
			}

			int n = p - mdp.nPageView;
			cardMenu[p]->stopAllActions();
			cardMenu[p]->runAction(MoveTo::create(0.2f, Vec2(MenuBaseX + gameSize.width * n, cardMenu[p]->getPositionY())));
		}
	}
//	auto action = MoveTo::create(0.5f, Vec2(MenuBaseX, cardMenu->getPositionY()));
	//cardMenu->setPositionX(MenuBaseX);
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	{
		//card Label
		Label* labelCard1[PAGE_COUNT][4] = { NULL };
		for (int p = 0; p < PAGE_COUNT; p++)
		{
			for (int i = 0; i < 4; i++)
			{
				int nGameNumber = p * 4 + i;
				labelCard1[p][i] = (Label*)this->getChildByTag(MENU_CARD_LABEL + nGameNumber);
				labelCard1[p][i]->setVisible(true);

				std::string strTemp = getGameName(nGameNumber, mdp.bEnglish, false);
				labelCard1[p][i]->setString(strTemp);
				if (bExtend == true)
				{
					labelCard1[p][i]->setVisible(false);
				}

				int n = p - mdp.nPageView;
				labelCard1[p][i]->stopAllActions();
				labelCard1[p][i]->runAction(MoveTo::create(0.2f, Vec2(MenuLabelBaseX + gameSize.width * n, labelCard1[p][i]->getPositionY())));
			}
		}

		Label* labelCard2[PAGE_COUNT][4] = { NULL };
		for (int p = 0; p < PAGE_COUNT; p++)
		{
			for (int i = 0; i < 4; i++)
			{
				int nGameNumber = p * 4 + i;
				labelCard2[p][i] = (Label*)this->getChildByTag(MENU_CARD_LABEL2 + nGameNumber);
				labelCard2[p][i]->setVisible(true);

				labelCard2[p][i]->setString(StringUtils::format(mdp.bEnglish ? "[Play : %d]" : UTF8PCHAR("[플레이 : %d]"), mdp.nGAME_PLAY_COUNT[nGameNumber]));

				if (bExtend == true)
				{
					labelCard2[p][i]->setVisible(false);
				}

				int n = p - mdp.nPageView;
				labelCard2[p][i]->stopAllActions();
				labelCard2[p][i]->runAction(MoveTo::create(0.2f, Vec2(MenuLabelBaseX3 + gameSize.width * n, labelCard2[p][i]->getPositionY())));
		
			
				//PlayLabel 안씀
				labelCard2[p][i]->setVisible(false);
			}
		}


		Label* labelCard3[PAGE_COUNT][4] = { NULL };
		for (int p = 0; p < PAGE_COUNT; p++)
		{
			for (int i = 0; i < 4; i++)
			{

				int nGameNumber = p * 4 + i;
				labelCard3[p][i] = (Label*)this->getChildByTag(MENU_CARD_LABEL3 + nGameNumber);
				labelCard3[p][i]->setVisible(false);


				int nPoint = 0;
				if (mdp.bExtreme)
				{
					if (nGameNumber <= GAME_COUNT && mdp.nGAME_EXTREME_STAGE[nGameNumber] == mdp.nGAME_MAX_STAGE[nGameNumber])
					{
						labelCard3[p][i]->setVisible(true);
						labelCard3[p][i]->setTextColor(Color4B::MAGENTA);
						nPoint = mdp.nGAME_EXTREME_POINT[nGameNumber];
					}
				}
				else
				{
					if (nGameNumber <= GAME_COUNT && mdp.nGAME_NORMAL_STAGE[nGameNumber] == mdp.nGAME_MAX_STAGE[nGameNumber])
					{
						labelCard3[p][i]->setVisible(true);
						labelCard3[p][i]->setTextColor(Color4B::ORANGE);
						nPoint = mdp.nGAME_NORMAL_POINT[nGameNumber];
					}
				}
				std::string strTemp = getGameScore(nGameNumber, mdp.bEnglish, false);
				labelCard3[p][i]->setString(StringUtils::format(strTemp.c_str(), nPoint));



				if (bExtend == true)
				{
					labelCard3[p][i]->setVisible(false);
				}

				int n = p - mdp.nPageView;
				labelCard3[p][i]->stopAllActions();
				labelCard3[p][i]->runAction(MoveTo::create(0.2f, Vec2(MenuLabelBaseX2 + gameSize.width * n, labelCard3[p][i]->getPositionY())));

			}
		}


		//card info
		Menu* cardStar[PAGE_COUNT][4] = { NULL };
		for (int p = 0; p < PAGE_COUNT; p++)
		{
			for (int i = 0; i < 4; i++)
			{
				int nGameNumber = p * 4 + i;
				cardStar[p][i] = (Menu*)this->getChildByTag(MENU_CARD_STAR + nGameNumber);
				if (nGameNumber >= GAME_COUNT)
				{
					cardStar[p][i]->setVisible(false);
				}
				else
				{
					cardStar[p][i]->setVisible(true);
					for (int j = 0; j < MAX_STAR_PHASE; j++)
					{
						if (j < mdp.nGAME_MAX_STAGE[nGameNumber])
						{
							((MenuItemSprite*)cardStar[p][i]->getChildByTag(j))->setEnabled(true);
							((MenuItemSprite*)cardStar[p][i]->getChildByTag(j))->unselected(); //star1

							if (mdp.bExtreme == true && j < mdp.nGAME_EXTREME_STAGE[nGameNumber])
							{
								((MenuItemSprite*)cardStar[p][i]->getChildByTag(j))->setEnabled(false); //star3
							}
							else if (mdp.bExtreme == false && j < mdp.nGAME_NORMAL_STAGE[nGameNumber])
							{
								((MenuItemSprite*)cardStar[p][i]->getChildByTag(j))->selected(); // star 2
							}

						}
						else
						{
							((MenuItemSprite*)cardStar[p][i]->getChildByTag(j))->setVisible(false);
						}
					}
				}

				if (bExtend == true)
				{
					cardStar[p][i]->setVisible(false);
				}

				int n = p - mdp.nPageView;
				cardStar[p][i]->stopAllActions();
				cardStar[p][i]->runAction(MoveTo::create(0.2f, Vec2(MenuInfoBaseX + gameSize.width * n, cardStar[p][i]->getPositionY())));
			}
		}


	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	//page left right
	auto p1 = (Sprite*) this->getChildByTag(MENU_PAGE1);
	auto p2 = (Sprite*) this->getChildByTag(MENU_PAGE2 + mdp.nPageView);
	p1->setPosition(p2->getPosition());

	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	//extend Note
	auto labelNoteName = (Label*) this->getChildByTag(NOTE_NAME);
	labelNoteName->setVisible(false);
	if (bExtend == true)
	{
		labelNoteName->setVisible(true);
	}
	labelNoteName->setString(mdp.bEnglish ? "Game Log" : UTF8PCHAR("게임 일지"));


	//tag 전부 찾고 visible false
	Label* labelNote[NOTE_LABEL_COUNT] = { NULL };
	for (int i = 0; i < NOTE_LABEL_COUNT; i++)
	{
		labelNote[i] = (Label*) this->getChildByTag(NOTE_LABEL + i);
		labelNote[i]->setVisible(false);
		if (bExtend == true)
		{
			labelNote[i]->setVisible(true);
		}
	}

	if (mdp.bExtreme == true)
	{
		labelNote[0]->setString(StringUtils::format(mdp.bEnglish ?
			"- Extreme Completed : %d / %d [ %.1f%% ]" : UTF8PCHAR("- 익스트림 완료 : %d / %d [ %.1f%% ]"),
			mdp.GetExtremeTrainingCompletedGame(), GAME_COUNT,
			(float)mdp.GetExtremeTrainingCompletedGame() * 100 / GAME_COUNT));

		labelNote[1]->setString(StringUtils::format(mdp.bEnglish ?
			"- Extreme Stars : %d / %d [ %.1f%% ]" : UTF8PCHAR("- 익스트림 별 : %d / %d [ %.1f%% ]"),
			mdp.GetExtremePlayerLevel(), mdp.GetMaxLevel(),
			(float)mdp.GetExtremePlayerLevel() * 100 / mdp.GetMaxLevel()));
		labelNote[1]->enableOutline(Color4B::RED, 1);
	}
	else
	{
		labelNote[0]->setString(StringUtils::format(mdp.bEnglish ?
			"- Training Completed : %d / %d [ %.1f%% ]" : UTF8PCHAR("- 훈련 완료 : %d / %d [ %.1f%% ]"),
			mdp.GetTrainingCompletedGame(), GAME_COUNT,
			(float)mdp.GetTrainingCompletedGame() * 100 / GAME_COUNT));

		labelNote[1]->setString(StringUtils::format(mdp.bEnglish ?
			"- Number of Stars : %d / %d [ %.1f%% ]" : UTF8PCHAR("- 별 개수 : %d / %d [ %.1f%% ]"),
			mdp.GetPlayerLevel(), mdp.GetMaxLevel(),
			(float)mdp.GetPlayerLevel() * 100 / mdp.GetMaxLevel()));
		labelNote[1]->enableOutline(Color4B::YELLOW, 1);
	}

//	labelNote[2]->setString(StringUtils::format(mdp.bEnglish ?
//		"- Max Life : %d" : UTF8PCHAR("- 최대 생명력 : %d"), (int)mdp.GetPlayerMaxLife()));

	labelNote[3]->setString(StringUtils::format(mdp.bEnglish ?
		"- Total Played : %d" : UTF8PCHAR("- 총 플레이 : %d"), mdp.GetTotalPlayCount()));

	labelNote[4]->setString(StringUtils::format(mdp.bEnglish ?
		"- Most Played : %d [ Game No. %d ]" : UTF8PCHAR("- 최대 플레이 : %d [ %d번 게임 ]"),
		mdp.GetMostedPlayCount(), mdp.GetMostedPlayGameNumber()));

	labelNote[5]->setString(StringUtils::format(mdp.bEnglish ?
		"- Best Combo : %d " : UTF8PCHAR("- 최고 콤보 : %d"), mdp.nMaxCombo));

	if (mdp.bExtreme)
	{
		labelNote[6]->setString(StringUtils::format(mdp.bEnglish ?
			"- Extreme TOP : %d [ Game No. %d ]" : UTF8PCHAR("- 익스트림 최고 : %d [ %d번 게임 ]"),
			mdp.GetExtremeTopPoint(), mdp.GetExtremeTopPointGameNumber()));
		labelNote[6]->enableOutline(Color4B::MAGENTA, 1);

		labelNote[7]->setString(StringUtils::format(mdp.bEnglish ?
			"- Extreme ALL : %d" : UTF8PCHAR("- 익스트림 합계 : %d"), mdp.GetExtremeALLPoint()));
		labelNote[7]->enableOutline(Color4B::MAGENTA, 1);
	}
	else
	{
		labelNote[6]->setString(StringUtils::format(mdp.bEnglish ?
			"- TOP Score : %d [ Game No. %d ]" : UTF8PCHAR("- 최고 점수 : %d [ %d번 게임 ]"),
			mdp.GetTopPoint(), mdp.GetTopPointGameNumber()));
		labelNote[6]->enableOutline(Color4B::ORANGE, 1);

		labelNote[7]->setString(StringUtils::format(mdp.bEnglish ?
			"- ALL Score : %d" : UTF8PCHAR("- 합계 점수 : %d"), mdp.GetALLPoint()));
		labelNote[7]->enableOutline(Color4B::ORANGE, 1);
	}

	/////////////////////////
	if (mdp.bExtreme == true)
	{
		labelNote[0]->setString(StringUtils::format(mdp.bEnglish ?
			"- Extreme Stars : %d / %d [ %.1f%% ]" : UTF8PCHAR("- 익스트림 별 : %d / %d [ %.1f%% ]"),
			mdp.GetExtremePlayerLevel(), mdp.GetMaxLevel(),
			(float)mdp.GetExtremePlayerLevel() * 100 / mdp.GetMaxLevel()));
		labelNote[0]->enableOutline(Color4B::RED, 1);

		labelNote[1]->setString(StringUtils::format(mdp.bEnglish ?
			"- Extreme TOP : %d [ Game No. %d ]" : UTF8PCHAR("- 익스트림 최고 : %d [ %d번 게임 ]"),
			mdp.GetExtremeTopPoint(), mdp.GetExtremeTopPointGameNumber()));
		labelNote[1]->enableOutline(Color4B::MAGENTA, 1);

		labelNote[2]->setString(StringUtils::format(mdp.bEnglish ?
			"- Extreme ALL : %d" : UTF8PCHAR("- 익스트림 합계 : %d"), mdp.GetExtremeALLPoint()));
		labelNote[2]->enableOutline(Color4B::MAGENTA, 1);
	}
	else
	{
		labelNote[0]->setString(StringUtils::format(mdp.bEnglish ?
			"- Number of Stars : %d / %d [ %.1f%% ]" : UTF8PCHAR("- 별 개수 : %d / %d [ %.1f%% ]"),
			mdp.GetPlayerLevel(), mdp.GetMaxLevel(),
			(float)mdp.GetPlayerLevel() * 100 / mdp.GetMaxLevel()));
		labelNote[0]->enableOutline(Color4B::YELLOW, 1);

		labelNote[1]->setString(StringUtils::format(mdp.bEnglish ?
			"- TOP Score : %d [ Game No. %d ]" : UTF8PCHAR("- 최고 점수 : %d [ %d번 게임 ]"),
			mdp.GetTopPoint(), mdp.GetTopPointGameNumber()));
		labelNote[1]->enableOutline(Color4B::ORANGE, 1);

		labelNote[2]->setString(StringUtils::format(mdp.bEnglish ?
			"- ALL Score : %d" : UTF8PCHAR("- 합계 점수 : %d"), mdp.GetALLPoint()));
		labelNote[2]->enableOutline(Color4B::ORANGE, 1);
	}

	labelNote[3]->setVisible(false);
	labelNote[4]->setVisible(false);
	labelNote[5]->setVisible(false);
	labelNote[6]->setVisible(false);
	labelNote[7]->setVisible(false);
	//////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	auto menuSideDown = (Menu*)this->getChildByTag(SIDE_MENU_DOWN);
	if (mdp.bMore == true)
	{
		menuSideDown->getChildByTag(SIDE_NOTE)->setVisible(true);
		menuSideDown->getChildByTag(SIDE_HOME)->setVisible(true);
		menuSideDown->getChildByTag(SIDE_FACEBOOK)->setVisible(true);
#if USE_LEADERBOARD == 1
		menuSideDown->getChildByTag(SIDE_LEADERBOARD)->setVisible(true);
#else
		menuSideDown->getChildByTag(SIDE_LEADERBOARD)->setVisible(false);
#endif

//		menuSideDown->getChildByTag(SIDE_LANG)->setVisible(true);
		menuSideDown->getChildByTag(SIDE_MUSIC)->setVisible(true);
		menuSideDown->getChildByTag(SIDE_AUDIO)->setVisible(true);
	}
	else
	{
		menuSideDown->getChildByTag(SIDE_NOTE)->setVisible(false);
		menuSideDown->getChildByTag(SIDE_HOME)->setVisible(false);
		menuSideDown->getChildByTag(SIDE_FACEBOOK)->setVisible(false);
		menuSideDown->getChildByTag(SIDE_LEADERBOARD)->setVisible(false);
//		menuSideDown->getChildByTag(SIDE_LANG)->setVisible(false);
		menuSideDown->getChildByTag(SIDE_MUSIC)->setVisible(false);
		menuSideDown->getChildByTag(SIDE_AUDIO)->setVisible(false);
	}
}


void MenuScene::ExtendNote()
{
	bExtend = !bExtend;

	auto bg2 = (Sprite*)this->getChildByTag(NOTE);
	bg2->stopAllActions();

	if (bExtend == true)
	{
		auto action = Sequence::create(
			MoveTo::create(0.3f, Vec2(gameSize.width*0.5, gameSize.height*0.5)),
			CallFuncN::create(CC_CALLBACK_0(MenuScene::UpdateMenu, this)),
			NULL);
		bg2->runAction(action);
	}
	else
	{
		auto action = Sequence::create(
			CallFuncN::create(CC_CALLBACK_0(MenuScene::UpdateMenu, this)),
			MoveTo::create(0.3f, Vec2(gameSize.width*0.5, gameSize.height* 1.5)),
			NULL);
		bg2->runAction(action);
	}
}

void MenuScene::initBG()
{
	auto bg = Sprite::createWithSpriteFrameName("bg.png");
	bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bg->setPosition(gameSize.width*0.5, gameSize.height*0.5);
	this->addChild(bg, 0);

	auto bgAd = Sprite::createWithSpriteFrameName("bgPlayAd.png");
	bgAd->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	bgAd->setPosition(visibleSize.width *0.5, visibleSize.height);
	this->addChild(bgAd, 10000);

	auto bg2 = Sprite::createWithSpriteFrameName("bgNote.png");
	bg2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bg2->setPosition(gameSize.width * 0.5, gameSize.height * 1.5);
	bg2->setTag(NOTE);
	this->addChild(bg2, 999);

	//initToast()
	{
		auto bgToast = Sprite::createWithSpriteFrameName("toast.png");
		bgToast->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bgToast->setPosition(gameSize.width*0.5, 48);
		bgToast->setTag(BG_TOAST);
		this->addChild(bgToast, 10000);
		bgToast->setScale(0.0f);

		auto labelToast = Label::create();// createWithTTF("test", _fonts_Base, TOAST_FONT_SIZE);
		labelToast->setSystemFontSize(TOAST_FONT_SIZE);

		labelToast->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelToast->setPosition(gameSize.width*0.5, 48);
		labelToast->setTextColor(Color4B::BLACK);
		labelToast->setTag(LABEL_TOAST);
		this->addChild(labelToast, 10001);
		labelToast->setScale(0.0f);
	}
}

void MenuScene::initBGAction()
{
	//////////////
	auto bgCircle = Sprite::createWithSpriteFrameName("bgCircle.png");
	bgCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bgCircle->setPosition(gameSize.width*0.5, gameSize.height*0.5);
	bgCircle->setScale(1.0f);
	bgCircle->setTag(GAME_BG_CIRCLE);
	this->addChild(bgCircle, 500);

	MenuOnOff(false);
	auto bgAction = Sequence::create(
		ScaleTo::create(0.4f, 4.0f),
		CallFuncN::create(CC_CALLBACK_1(MenuScene::Callback, this, 1)),
		NULL);
	bgCircle->runAction(bgAction);
	//////////////

	auto bgCircle2 = Sprite::createWithSpriteFrameName("bgCircle2.png");
	bgCircle2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bgCircle2->setPosition(gameSize.width*0.5, gameSize.height*0.5);
	bgCircle2->setScale(7.0f);
	bgCircle2->setTag(GAME_BG_CIRCLE2);
	this->addChild(bgCircle2, 501);


	if (mdp.bExtreme == true)
	{
		bgCircle2->setScale(1.0f);
		bgCircle2->stopAllActions();
		auto bgAction2 = Sequence::create(
			ScaleTo::create(0.4f, 4.0f),
			NULL);
		bgCircle2->runAction(bgAction2);
		//////////////
	}

}

void MenuScene::MenuOnOff(bool bOn)
{
	auto menuSideUp = (Menu*)this->getChildByTag(SIDE_MENU_UP);
	auto menuSideDown = (Menu*)this->getChildByTag(SIDE_MENU_DOWN);

	menuSideUp->setEnabled(bOn);
	menuSideDown->setEnabled(bOn);

	Menu* cardMenu[PAGE_COUNT] = { NULL };
	for (int p = 0; p < PAGE_COUNT; p++)
	{
		cardMenu[p] = (Menu*) this->getChildByTag(MENU_MENU + p);
		cardMenu[p]->setEnabled(bOn);
	}
}


void MenuScene::Callback(Ref* sender, int nType)
{
	if (nType == 1)
	{
		MenuOnOff(true);
	}
	else if (nType == 2)
	{
		RemoveListener();//Replace 전에 써야됨. Replace 등록 후에 된 리스너도 삭제함.
		Director::getInstance()->replaceScene(PlayScene::createScene());
	}
	else if (nType == 3)
	{
		RemoveListener();
		Director::getInstance()->replaceScene(TwoPlayScene::createScene());
	}
}

void MenuScene::initBGM()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false)
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(_bgm_calm, true);

	if (mdp.bBGMSwitch == false)
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void MenuScene::initSound()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(_sounds_Touch);
}


void MenuScene::menuCallback(Ref* pSender)
{
	if (bExtend == true)
		return;

	if (mdp.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);


	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();
	switch (nSelectedItem)
	{
	case MENU_GAME1:
	case MENU_GAME2:
	case MENU_GAME3:
	case MENU_GAME4:
	{
		int nGameNumber = mdp.nPageView * 4 + (nSelectedItem - MENU_GAME1);
		mdInfo.nGameNumber = nGameNumber;


		MenuOnOff(false);
		auto bgCircle = (Sprite*)this->getChildByTag(GAME_BG_CIRCLE);
		auto bgAction = Sequence::create(
			ScaleTo::create(0.4f, 1.0f),
			CallFuncN::create(CC_CALLBACK_1(MenuScene::Callback, this, 2)),
			NULL);
		bgCircle->runAction(bgAction);


		if (mdp.bExtreme == true)
		{
			auto bgCircle2 = (Sprite*)this->getChildByTag(GAME_BG_CIRCLE2);
			bgCircle2->stopAllActions();
			auto bgAction2 = Sequence::create(
				ScaleTo::create(0.4f, 1.0f),
				NULL);
			bgCircle2->runAction(bgAction2);
		}

	}
	break;

	case MENU_TWO_PLAYER_1:
	case MENU_TWO_PLAYER_2:
	case MENU_TWO_PLAYER_3:
	case MENU_TWO_PLAYER_4:
	{
		int nGameNumber = mdp.nPageView * 4 + (nSelectedItem - MENU_TWO_PLAYER_1);
		mdInfo.nGameNumber = nGameNumber;

		MenuOnOff(false);
		auto bgCircle = (Sprite*)this->getChildByTag(GAME_BG_CIRCLE);
		auto bgAction = Sequence::create(
			ScaleTo::create(0.4f, 1.0f),
			CallFuncN::create(CC_CALLBACK_1(MenuScene::Callback, this, 3)),
			NULL);
		bgCircle->runAction(bgAction);


		if (mdp.bExtreme == true)
		{
			auto bgCircle2 = (Sprite*)this->getChildByTag(GAME_BG_CIRCLE2);
			bgCircle2->stopAllActions();
			auto bgAction2 = Sequence::create(
				ScaleTo::create(0.4f, 1.0f),
				NULL);
			bgCircle2->runAction(bgAction2);
		}
	}
	break;

	default:
		break;

	}
}

void MenuScene::ExitGame()
{
	//submit
#if USE_LEADERBOARD == 1
	if (GameSharing::IsGPGAvailable() == true)
	{
		if (mdp.GetPlayerLevel() <= mdp.GetMaxLevel())
			GameSharing::SubmitScore(mdp.GetPlayerLevel(), LEADERBOARD_STAR);

		GameSharing::SubmitScore((mdp.GetTopPoint() * 100) + mdp.GetTopPointGameNumber(), LEADERBOARD_TOP); //fixed 2 Point
		GameSharing::SubmitScore(mdp.GetALLPoint(), LEADERBOARD_ALL);


		//		if (mdp.GetExtremePlayerLevel() <= mdp.GetMaxLevel())
		//			GameSharing::SubmitScore(mdp.GetExtremePlayerLevel(), LEADERBOARD_STAR_EX);

		//		GameSharing::SubmitScore((mdp.GetExtremeTopPoint() * 100) + mdp.GetExtremeTopPointGameNumber(), LEADERBOARD_TOP_EX); //fixed 2 Point
		//		GameSharing::SubmitScore(mdp.GetExtremeALLPoint(), LEADERBOARD_ALL_EX);
	}
#endif

	mdp.SaveFullData();
	RemoveListener();

	////////////////Close 기본 관련
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
	/*To navigate back to native iOS screen(if present) without quitting the application ,
	do not use Director::getInstance()->end() and exit(0)
	as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
	////////////////Close 기본 관련
}

void MenuScene::SideMenu(Ref* pSender, int nCallNumber)
{

	if (mdp.bSoundSwitch == true)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
	}

	int nSelectedItem = 0;
	if (pSender != NULL)
	{
		auto menuItem = (MenuItem*)pSender;
		nSelectedItem = menuItem->getTag();
	}

	if (nCallNumber != 0)
		nSelectedItem = nCallNumber;

	switch (nSelectedItem)
	{
	case SIDE_HOME:
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		Application::getInstance()->openURL(SIDE_HOME_URL);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		if (mdp.bEnglish)
			Application::getInstance()->openURL(SIDE_HOME_IOS_EN_URL);
		else
			Application::getInstance()->openURL(SIDE_HOME_IOS_KO_URL);
#endif
		break;

	case SIDE_LEADERBOARD:

#if USE_LEADERBOARD == 1
		if (GameSharing::IsGPGAvailable() == true)
		{
			if (mdp.GetPlayerLevel() <= mdp.GetMaxLevel())
				GameSharing::SubmitScore(mdp.GetPlayerLevel(), LEADERBOARD_STAR);

			GameSharing::SubmitScore((mdp.GetTopPoint() * 100) + mdp.GetTopPointGameNumber(), LEADERBOARD_TOP); //fixed 2 Point
			GameSharing::SubmitScore(mdp.GetALLPoint(), LEADERBOARD_ALL);


			if (mdp.GetExtremePlayerLevel() <= mdp.GetMaxLevel())
				GameSharing::SubmitScore(mdp.GetExtremePlayerLevel(), LEADERBOARD_STAR_EX);

			GameSharing::SubmitScore((mdp.GetExtremeTopPoint() * 100) + mdp.GetExtremeTopPointGameNumber(), LEADERBOARD_TOP_EX); //fixed 2 Point
			GameSharing::SubmitScore(mdp.GetExtremeALLPoint(), LEADERBOARD_ALL_EX);
		}


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		if (GameSharing::IsGPGAvailable() == true)
		{
			GameSharing::ShowLeaderboards(-1);
		}
		else
		{
			if (mdp.bEnglish == true)
				MessageBox(SIDE_GPGS_FAILED_EN, SIDE_GPGS_FAILED_EN2);
			else
				MessageBox(SIDE_GPGS_FAILED_KO, SIDE_GPGS_FAILED_KO2);

			Application::getInstance()->openURL(SIDE_GPGS_URL);
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		if (GameSharing::IsGPGAvailable() == true)
		{
			GameSharing::ShowLeaderboards(0);
		}
#endif

#endif //USE_LEADERBOARD == 1

		break;

	case SIDE_ACHIEVEMENT:
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		if (GameSharing::IsGPGAvailable() == true)
		{
			GameSharing::ShowAchievementsUI();
		}
		else
		{
			if (mdp.bEnglish == true)
				MessageBox(SIDE_GPGS_FAILED_EN, SIDE_GPGS_FAILED_EN2);
			else
				MessageBox(SIDE_GPGS_FAILED_KO, SIDE_GPGS_FAILED_KO2);

			Application::getInstance()->openURL(SIDE_GPGS_URL);
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		GameSharing::ShowAchievementsUI();
#endif
		break;

	case SIDE_MUSIC:
		mdp.bBGMSwitch = !mdp.bBGMSwitch;
		if (mdp.bBGMSwitch == true)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		else
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
		break;
		mdp.SaveSetting();
		break;

	case SIDE_AUDIO:
		mdp.bSoundSwitch = !mdp.bSoundSwitch;
		mdp.SaveSetting();
		break;

	case SIDE_FACEBOOK:
		Application::getInstance()->openURL(SIDE_FACEBOOK_URL);
		break;


	case SIDE_LANG:
	{
		mdp.bEnglish = !mdp.bEnglish;
		mdp.SaveSetting();
		UpdateMenu();
	}
	break;

	case SIDE_LEFT:
	{
		if (mdp.nPageView > 0)
		{
			mdp.nPageView--;
			mdp.SaveSetting();
		}
	}
	break;

	case SIDE_RIGHT:
	{
		if (mdp.nPageView < (GAME_COUNT - 1) / 4)
		{
			mdp.nPageView++;
			mdp.SaveSetting();
		}
	}
	break;


	case SIDE_EXTREME:
	{
		if ( mdp.GetPlayerLevel() >= GAME_COUNT )
		{
			if (mdp.bSoundSwitch == true)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Sword);

			mdp.bExtreme = !mdp.bExtreme;
			UpdateMenu();

			if (mdp.bExtreme == true)
			{
				auto bgCircle2 = (Sprite*)this->getChildByTag(GAME_BG_CIRCLE2);
				bgCircle2->stopAllActions();
				auto bgAction2 = Sequence::create(
					ScaleTo::create(0.4f, 4.0f),
					NULL);
				bgCircle2->runAction(bgAction2);

				ToastMessage(mdp.bEnglish ? "Extreme difficulty" : UTF8PCHAR("익스트림 난이도"));

			}
			else
			{
				auto bgCircle2 = (Sprite*)this->getChildByTag(GAME_BG_CIRCLE2);
				bgCircle2->stopAllActions();
				auto bgAction2 = Sequence::create(
					ScaleTo::create(0.4f, 7.0f),
					NULL);
				bgCircle2->runAction(bgAction2);

				ToastMessage(mdp.bEnglish ? "Normal difficulty" : UTF8PCHAR("일반 난이도"));
			}
		}
		else
		{
			ToastMessage(StringUtils::format(mdp.bEnglish ? "Extreme difficulty is available after %d star." : UTF8PCHAR("익스트림은 별 %d개 이상부터 가능합니다."), GAME_COUNT * 2));

			auto menuSideUp = (Menu*)this->getChildByTag(SIDE_MENU_UP);
			((MenuItemToggle*)menuSideUp->getChildByTag(SIDE_EXTREME))->setSelectedIndex(1);
		}

	}
	break;



	case SIDE_EXIT:
	{
		if (bExtend == true)
		{
			ExtendNote();
		}
		else
		{
			nKeyBackSpace--;
			if (nKeyBackSpace == 0)
			{
				ExitGame();
			}
			else
			{
				ToastMessage(StringUtils::format(mdp.bEnglish ? "Press the Back key %d times to exit." : UTF8PCHAR("뒤로가기를 %d번 더 누르면 종료합니다."), nKeyBackSpace));
			}
		}
	}
	break;

	case SIDE_MORE:
	{
		//side menu down
		mdp.bMore = !mdp.bMore;
		mdp.SaveSetting();
		if (bExtend == true && mdp.bMore == false)
			ExtendNote();

		UpdateMenu();
	}
	break;

	case SIDE_NOTE:
	{
		ExtendNote();
	}
	break;

	default:
		break;

	}

}


void MenuScene::RemoveListener()
{
	this->getEventDispatcher()->removeAllEventListeners();
	this->unscheduleUpdate();
	this->removeAllChildren();
}



bool MenuScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (bPrevTouch == true)
		return false;

	prevTouchPoint = touch->getLocation();
	prevTouchPoint_Drag = touch->getLocation();
	bPrevTouch = true;

	return true;
}
void MenuScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	Point currentTouchPoint = touch->getLocation();
	float x = currentTouchPoint.x - prevTouchPoint_Drag.x;

	Menu* menu[PAGE_COUNT] = { NULL };

	Menu* menuTwoPlayer[PAGE_COUNT] = { NULL };
	Label* labelCard1[PAGE_COUNT][4] = { NULL };
	Label* labelCard2[PAGE_COUNT][4] = { NULL };
	Label* labelCard3[PAGE_COUNT][4] = { NULL };
	Menu* menuGameInfo[PAGE_COUNT][4] = { NULL };
	for (int p = 0; p < PAGE_COUNT; p++)
	{


		menu[p] = (Menu*)this->getChildByTag(MENU_MENU + p);
		float nPos = menu[p]->getPositionX() + x;
		menu[p]->setPositionX(nPos);

		menuTwoPlayer[p] = (Menu*)this->getChildByTag(MENU_TWO_PLAYER + p);
		nPos = menuTwoPlayer[p]->getPositionX() + x;
		menuTwoPlayer[p]->setPositionX(nPos);

		for (int i = 0; i < 4; i++)
		{
			int nGameNumber = p * 4 + i;
			labelCard1[p][i] = (Label*)this->getChildByTag(MENU_CARD_LABEL + nGameNumber);
			float nPos = labelCard1[p][i]->getPositionX() + x;
			labelCard1[p][i]->setPositionX(nPos);
		}

		for (int i = 0; i < 4; i++)
		{
			int nGameNumber = p * 4 + i;
			labelCard2[p][i] = (Label*)this->getChildByTag(MENU_CARD_LABEL2 + nGameNumber);
			float nPos = labelCard2[p][i]->getPositionX() + x;
			labelCard2[p][i]->setPositionX(nPos);
		}

		for (int i = 0; i < 4; i++)
		{
			int nGameNumber = p * 4 + i;
			labelCard3[p][i] = (Label*)this->getChildByTag(MENU_CARD_LABEL3 + nGameNumber);
			float nPos = labelCard3[p][i]->getPositionX() + x;
			labelCard3[p][i]->setPositionX(nPos);
		}

		for (int i = 0; i < 4; i++)
		{
			int nGameNumber = p * 4 + i;
			menuGameInfo[p][i] = (Menu*)this->getChildByTag(MENU_CARD_STAR + nGameNumber);
			float nPos = menuGameInfo[p][i]->getPositionX() + x;
			menuGameInfo[p][i]->setPositionX(nPos);
		}
	}

	prevTouchPoint_Drag = currentTouchPoint;
}

void MenuScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	bPrevTouch = false;
	Point currentTouchPoint = touch->getLocation();

	float x = currentTouchPoint.x - prevTouchPoint.x;
	float y = currentTouchPoint.y - prevTouchPoint.y;
	float absx = x, absy = y;
	absx = x < 0 ? x * -1 : x;
	absy = y < 0 ? y * -1 : y;

	bool bLeft, bRight, bTop, bBottom;
	bLeft = bRight = bTop = bBottom = false;
	if (absx == 0 || (absy / absx < 1.0f))
	{
		if (x > 0)
		{
			bRight = true;

		}
		else if (x < 0)
		{
			bLeft = true;

		}
	}
	else
	{
		if (y > 0)
		{
			bTop = true;

		}
		else if (y < 0)
		{
			bBottom = true;
		}
	}

	if ((bLeft || bRight) && absx < 25)
	{
		bLeft = bRight = false;
	}
	else if ((bTop || bBottom) && absy < 25)
	{
		bTop = bBottom = false;
	}



	if (bLeft)
	{
		if (bExtend == false)
			SideMenu(NULL, SIDE_RIGHT);
	}
	else if (bRight)
	{
		if (bExtend == false)
			SideMenu(NULL, SIDE_LEFT);
	}
	else if (bTop)
	{
		if (bExtend == true)
			SideMenu(NULL, SIDE_NOTE);
	}
	else if (bBottom)
	{
	//	if (bExtend == true)
	//		SideMenu(NULL, SIDE_NOTE);
	}

	UpdateMenu();
}
void MenuScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
	;
}

void MenuScene::ToastMessage(std::string str)
{
	auto bgToast = (Sprite*)this->getChildByTag(BG_TOAST);
	if (bgToast != NULL)
	{
		bgToast->stopAllActions();
		auto action = Sequence::create(
			ScaleTo::create(0.0f, 1.0f),
			DelayTime::create(1.0f),
			ScaleTo::create(0.4f, 0.0f),
			NULL);
		bgToast->runAction(action);
	}
	auto labelToast = (Label*)this->getChildByTag(LABEL_TOAST);
	if (labelToast != NULL)
	{
		labelToast->setString(str);
		labelToast->stopAllActions();
		auto action = Sequence::create(
			ScaleTo::create(0.0f, 1.0f),
			DelayTime::create(1.0f),
			ScaleTo::create(0.4f, 0.0f),
			NULL);
		labelToast->runAction(action);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void MenuScene::ReadyAd()
{
	if (bReadyAd == false)
	{
		bReadyAd = true;
#ifdef PLUGIN_AD_MOB
		sdkbox::PluginAdMob::setListener(this);
#endif
	}
}

void MenuScene::ReadyBanner()
{
	if (bReadyAd == true)
	{
#ifdef PLUGIN_AD_MOB
		if (sdkbox::PluginAdMob::isAvailable(AD_MOB_BANNER) == false)
		{
			sdkbox::PluginAdMob::cache(AD_MOB_BANNER);
		}
#endif
	}
}

void MenuScene::ShowBanner()
{
	if (bReadyAd == true)
	{
#ifdef PLUGIN_AD_MOB
		sdkbox::PluginAdMob::show(AD_MOB_BANNER);
#endif
	}
}

void MenuScene::HideBanner()
{
	if (bReadyAd == true)
	{
#ifdef PLUGIN_AD_MOB
		sdkbox::PluginAdMob::hide(AD_MOB_BANNER);
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////