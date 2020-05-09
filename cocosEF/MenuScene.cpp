#include "MenuScene.h"
#include "GamePlay/StageSelectScene.h"
#include "Option/ShopScene.h"
#include "Option/CreditScene.h"
#include "Util/define.h"
#include "Util/MyDataManager.h"
#include "SimpleAudioEngine.h"

#ifdef PLUGIN_GPG
#include "PluginGPG/PluginGPG.h"
#endif

#include "gpgs/GameSharing.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
void MenuScene::ReadyAdd() {
	nAddNum = 0;

	if (mdp.nAddCount == 0)
	{
		mdp.nAddCount = ADD_COUNT;
#ifdef PLUGIN_AD_COLONY
		nAddNum = 2;
#endif
#ifdef PLUGIN_CHART_BOOST //crash -_-
		if (CCRANDOM_0_1() < 0.5f)
			nAddNum = 1;
#endif
#ifdef PLUGIN_AD_MOB
		if (CCRANDOM_0_1() < 0.5f)
			nAddNum = 3;
#endif
	}

	if (mdp.bPremium)
	{
		mdp.nAddCount = 0;
		nAddNum = 0;
	}


	if (nAddNum == 1)
	{
#ifdef PLUGIN_CHART_BOOST
		sdkbox::PluginChartboost::init();
		sdkbox::PluginChartboost::setListener(this);
		sdkbox::PluginChartboost::cache(sdkbox::CB_Location_Default);
#endif
		int test = 1;
	}
	else if (nAddNum == 2)
	{
#ifdef PLUGIN_AD_COLONY
		sdkbox::PluginAdColony::init();
		sdkbox::PluginAdColony::setListener(this);
#endif
		int test = 1;
	}
	else if (nAddNum == 3)
	{
#ifdef PLUGIN_AD_MOB
		//	sdkbox::PluginAdMob::setListener(this);
		//	sdkbox::PluginAdMob::init();
#endif
		int test = 1;
	}
}

void MenuScene::ShowAdd()
{
	if (nAddNum == 1)
	{
#ifdef PLUGIN_CHART_BOOST
		sdkbox::PluginChartboost::show(sdkbox::CB_Location_Default);
#endif
		int test = 1;
	}
	else if (nAddNum == 2)
	{
#ifdef PLUGIN_AD_COLONY
		sdkbox::PluginAdColony::show("video");
#endif
		int test = 1;
	}
	else if (nAddNum == 3)
	{
#ifdef PLUGIN_AD_MOB
		//		sdkbox::PluginAdMob::
#endif
		int test = 1;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}


void MenuScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	// Back button pressed
	if (keyCode == EventKeyboard::KeyCode::KEY_Z || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		nKeyBackSpace++;
		if (nKeyBackSpace == 2)
		{
			ExitGame();
		}

	}
}


bool MenuScene::init()
{
	ReadyAdd();

	if (!Layer::init())
	{
		return false;
	}

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	nKeyBackSpace = 0;
	auto k_listener = EventListenerKeyboard::create();
	k_listener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(k_listener, 1);


	initBG();
	initBGM();
	initSound();
	initParticle();

	/////////////////////////////
	//게임 제목 라벨
	auto mainLabel = Label::createWithTTF("Eight Fire", _fonts_NanumPen, TITLE_FONT_SIZE);
	mainLabel->setTextColor(Color4B::YELLOW);
	mainLabel->enableOutline(Color4B::RED, 2);
	mainLabel->enableBold();

	auto mainLabelItem = MenuItemLabel::create(mainLabel, CC_CALLBACK_1(MenuScene::menuCallback, this));
	mainLabelItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	mainLabelItem->setTag(MENU_TITLE_ITEM);

	auto menumainLabel = Menu::create(mainLabelItem, NULL);
	menumainLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height));
	menumainLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	menumainLabel->setTag(MENU_TITLE);
	this->addChild(menumainLabel, 1);
	nEasterStarCount = 0;

	/////////////////////////////
	//사이드 메뉴
	auto home = Sprite::createWithSpriteFrameName("home.png");
	auto share = Sprite::createWithSpriteFrameName("share.png");
	auto leaderboard = Sprite::createWithSpriteFrameName("leaderboard.png");
	auto achievement = Sprite::createWithSpriteFrameName("achievement.png");

	auto audioOn = Sprite::createWithSpriteFrameName("audioOn.png");
	auto audioOff = Sprite::createWithSpriteFrameName("audioOff.png");
	auto musicOn = Sprite::createWithSpriteFrameName("musicOn.png");
	auto musicOff = Sprite::createWithSpriteFrameName("musicOff.png");
	auto credit = Sprite::createWithSpriteFrameName("credit.png");
	auto LangEn = Sprite::createWithSpriteFrameName("en_lang.png");
	auto LangKor = Sprite::createWithSpriteFrameName("kor_lang.png");


	auto home_s = Sprite::createWithSpriteFrameName("home_s.png");
	auto share_s = Sprite::createWithSpriteFrameName("share_s.png");
	auto leaderboard_s = Sprite::createWithSpriteFrameName("leaderboard_s.png");
	auto achievement_s = Sprite::createWithSpriteFrameName("achievement_s.png");
	auto audioOn_s = Sprite::createWithSpriteFrameName("audioOn_s.png");
	auto audioOff_s = Sprite::createWithSpriteFrameName("audioOff_s.png");
	auto musicOn_s = Sprite::createWithSpriteFrameName("musicOn_s.png");
	auto musicOff_s = Sprite::createWithSpriteFrameName("musicOff_s.png");
	auto credit_s = Sprite::createWithSpriteFrameName("credit_s.png");
	auto LangEn_s = Sprite::createWithSpriteFrameName("en_lang_s.png");
	auto LangKor_s = Sprite::createWithSpriteFrameName("kor_lang_s.png");

	auto LangEnItem = MenuItemSprite::create(LangEn, LangEn_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	auto LangKorItem = MenuItemSprite::create(LangKor, LangKor_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	MenuItemToggle* Lang = NULL;
	if (mdp.bEnglish == true)
		Lang = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this), LangKorItem, LangEnItem, NULL);
	else
		Lang = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this), LangEnItem, LangKorItem, NULL);
	Lang->setScale(SIDE_SCALE);
	Lang->setTag(SIDE_LANG);

	auto homeItem = MenuItemSprite::create(home, home_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	homeItem->setScale(SIDE_SCALE);
	homeItem->setTag(SIDE_HOME);

	auto shareItem = MenuItemSprite::create(share, share_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	shareItem->setScale(SIDE_SCALE);
	shareItem->setTag(SIDE_SHARE);

	auto leaderboardItem = MenuItemSprite::create(leaderboard, leaderboard_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	leaderboardItem->setScale(SIDE_SCALE);
	leaderboardItem->setTag(SIDE_LEADERBOARD);

	auto achievementItem = MenuItemSprite::create(achievement, achievement_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	achievementItem->setScale(SIDE_SCALE);
	achievementItem->setTag(SIDE_ACHIEVEMENT);

	auto creditItem = MenuItemSprite::create(credit, credit_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	creditItem->setScale(SIDE_SCALE);
	creditItem->setTag(SIDE_CREDIT);

	auto audioOnItem = MenuItemSprite::create(audioOn, audioOn_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	auto audioOffItem = MenuItemSprite::create(audioOff, audioOff_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	MenuItemToggle* audio = NULL;
	if (mdp.bSoundSwitch == true)
		audio = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this), audioOnItem, audioOffItem, NULL);
	else
		audio = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this), audioOffItem, audioOnItem, NULL);
	audio->setScale(SIDE_SCALE);
	audio->setTag(SIDE_AUDIO);

	auto musicOnItem = MenuItemSprite::create(musicOn, musicOn_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	auto musicOffItem = MenuItemSprite::create(musicOff, musicOff_s, CC_CALLBACK_1(MenuScene::SideMenu, this));
	MenuItemToggle* music = NULL;
	if (mdp.bBGMSwitch == true)
		music = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this), musicOnItem, musicOffItem, NULL);
	else
		music = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::SideMenu, this), musicOffItem, musicOnItem, NULL);
	music->setScale(SIDE_SCALE);
	music->setTag(SIDE_MUSIC);



	auto sidemenu = Menu::create(
		Lang,
		homeItem,
		shareItem,
		leaderboardItem,
		achievementItem,
		creditItem,
		audio,
		music,
		NULL);
	sidemenu->alignItemsVerticallyWithPadding(10);
	sidemenu->setPosition(visibleSize.width * 0.1, visibleSize.height * 0.5);

	this->addChild(sidemenu, 20);
	///사이드 메뉴
	/////////////////////////////

	////////////////////////////////
	/// 게임 메뉴 en
	auto label1 = Label::createWithTTF("", _fonts_NanumPen, MENU_FONT_SIZE);
	label1->setTextColor(Color4B::BLUE);
	label1->enableBold();
	label1->enableOutline(Color4B::YELLOW, 2);

	auto label2 = Label::createWithTTF("", _fonts_NanumPen, MENU_FONT_SIZE);
	label2->setTextColor(Color4B::BLUE);
	label2->enableBold();
	label2->enableOutline(Color4B::YELLOW, 2);

	auto label3 = Label::createWithTTF("", _fonts_NanumPen, MENU_FONT_SIZE);
	label3->setTextColor(Color4B::BLUE);
	label3->enableBold();
	label3->enableOutline(Color4B::YELLOW, 2);

	auto label4 = Label::createWithTTF("", _fonts_NanumPen, MENU_FONT_SIZE);
	label4->setTextColor(Color4B::YELLOW);
	label4->enableBold();
	label4->enableOutline(Color4B::BLUE, 2);

	auto label5 = Label::createWithTTF("", _fonts_NanumPen, MENU_FONT_SIZE);
	label5->setTextColor(Color4B::WHITE);
	label5->enableBold();
	label5->enableOutline(Color4B::BLACK, 2);

	auto labelItem1 = MenuItemLabel::create(label1, CC_CALLBACK_1(MenuScene::menuCallback, this));
	auto labelItem2 = MenuItemLabel::create(label2, CC_CALLBACK_1(MenuScene::menuCallback, this));
	auto labelItem3 = MenuItemLabel::create(label3, CC_CALLBACK_1(MenuScene::menuCallback, this));
	auto labelItem4 = MenuItemLabel::create(label4, CC_CALLBACK_1(MenuScene::menuCallback, this));
	auto labelItem5 = MenuItemLabel::create(label5, CC_CALLBACK_1(MenuScene::menuCallback, this));

	labelItem1->setTag(MENU_SOLO);
	labelItem2->setTag(MENU_DOUBLE);
	labelItem3->setTag(MENU_BATTLE);
	labelItem4->setTag(MENU_SHOP);
	labelItem5->setTag(MENU_EXIT);

	auto menu = Menu::create(labelItem1, labelItem2, labelItem3, labelItem4, labelItem5, NULL);
	menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2.5));
	menu->alignItemsVerticallyWithPadding(MENU_FONT_SIZE);
	menu->setTag(MENU_MENU);
	this->addChild(menu, 1);
	/////////////////////////////

	setLang();
	UpdateClicker();
	ShowAdd();

	return true;
}

void MenuScene::UpdateClicker()
{
	auto click = (Sprite*)this->getChildByTag(MENU_CLICK);
	if (click == NULL)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		click = Sprite::createWithSpriteFrameName("click.png");
		click->setPosition(visibleSize.width*0.9f, visibleSize.height * 0.28f);
		click->setScale(0.1f);
		click->setTag(MENU_CLICK);
		this->addChild(click);

		auto action1 = ScaleTo::create(1, 0.6f);
		auto action2 = ScaleTo::create(1, 0.3f);
		auto action = Sequence::create(action1, action2, NULL);
		click->runAction(RepeatForever::create(action));
	}


	if (mdp.nStarPoint > 0)
		click->setVisible(true);
	else
		click->setVisible(false);

}





void MenuScene::setLang()
{
	auto menu = (Menu*)this->getChildByTag(MENU_MENU);
	auto item1 = (MenuItemLabel*)menu->getChildByTag(MENU_SOLO);
	auto item2 = (MenuItemLabel*)menu->getChildByTag(MENU_DOUBLE);
	auto item3 = (MenuItemLabel*)menu->getChildByTag(MENU_BATTLE);
	auto item4 = (MenuItemLabel*)menu->getChildByTag(MENU_SHOP);
	auto item5 = (MenuItemLabel*)menu->getChildByTag(MENU_EXIT);

	menu = (Menu*)this->getChildByTag(MENU_TITLE);
	auto title = (MenuItemLabel*)menu->getChildByTag(MENU_TITLE_ITEM);
	if (mdp.bEnglish == true)
	{
		item1->setString(en_solo);
		item2->setString(en_double);
		item3->setString(en_battle);
		item4->setString(en_shop);
		item5->setString(en_exit);
		title->setString(en_title);
	}
	else
	{
		item1->setString(UTF8PCHAR(ko_solo));
		item2->setString(UTF8PCHAR(ko_double));
		item3->setString(UTF8PCHAR(ko_battle));
		item4->setString(UTF8PCHAR(ko_shop));
		item5->setString(UTF8PCHAR(ko_exit));
		title->setString(UTF8PCHAR(ko_title));
	}

}

void MenuScene::initBG()
{
	auto node = ParallaxNode::create();
	this->addChild(node, 0);

	auto BG = Sprite::createWithSpriteFrameName("BG.png");
	BG->setAnchorPoint(Point::ZERO);
	BG->setScale(2.0f);
	node->addChild(BG, 0, Point(0, 0), Point::ZERO);
}

void MenuScene::initBGM()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.3f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false)
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(_BGM_copycat, true);

	if (mdp.bBGMSwitch == false)
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
void MenuScene::initSound()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(_sounds_Touch);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(_sounds_Steam);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(_sounds_EightFire);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(_sounds_receive);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(_sounds_bubble);
}
void MenuScene::initParticle()
{
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();

	auto particle = ParticleSystemQuad::create(_particle_BG_sun);
	particle->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.9));
	this->addChild(particle, 0);
}

void MenuScene::menuCallback(Ref* pSender)
{
	if (mdp.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();

	switch (nSelectedItem)
	{
	case MENU_TITLE_ITEM:
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
		nEasterStarCount++;
		if (nEasterStarCount == 8 && mdp.bEasterStar1 == false)
		{
			if (mdp.bSoundSwitch == true)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_receive);

			auto particle = ParticleSystemQuad::create(_particle_char_fire);
			particle->setScale(3.0f);
			particle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
			this->addChild(particle, 20);

			mdp.bEasterStar1 = true;
			mdp.nStarPoint++;
			mdp.SaveEasterStar();
			UpdateClicker();
		}
		break;

	case MENU_SOLO:
	case MENU_DOUBLE:
	case MENU_BATTLE:
	{

		mdInfo.nPlayGameType = nSelectedItem;

		RemoveListener();

		director->replaceScene(TransitionFade::create(0.2f, StageSelectScene::createScene()));
		break;
	}
	case MENU_SHOP:



		RemoveListener();

		director->replaceScene(TransitionFade::create(0.2f, ShopScene::createScene()));

		break;
	case MENU_EXIT:
		ExitGame();

		break;
	default:
		break;
	}
}

void MenuScene::ExitGame()
{
	mdp.SaveFullData();
	RemoveListener();

	////////////////Close 기본 관련
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
	////////////////Close 기본 관련
}

void MenuScene::SideMenu(Ref* pSender)
{
	if (mdp.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();

	switch (nSelectedItem)
	{
	case SIDE_HOME:
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		Application::getInstance()->openURL("https://play.google.com/store/apps/details?id=com.onebutton.eightfire");
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		if (mdp.bEnglish)
			Application::getInstance()->openURL("https://itunes.apple.com/app/id1187814664");
		else
			Application::getInstance()->openURL("https://itunes.apple.com/kr/app/id1187814664");
#endif
		break;

	case SIDE_SHARE:
	{
		if (CCRANDOM_0_1() > 0.8f  && mdp.bEasterStar3 == false)
		{
			if (mdp.bSoundSwitch == true)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_receive);
			auto particle = ParticleSystemQuad::create(_particle_char_fire);
			particle->setScale(3.0f);
			particle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
			this->addChild(particle, 20);

			mdp.bEasterStar3 = true;
			mdp.nStarPoint++;
			mdp.SaveEasterStar();
			UpdateClicker();
		}


#ifdef PLUGIN_SHARE

		sdkbox::PluginShare::init();
		sdkbox::PluginShare::setListener(this);

		sdkbox::SocialShareInfo info;
		info.title = "Eight Fire\n";

		if (mdp.bEnglish == true)
			info.text = en_share;
		else
			info.text = ko_share;

		if (mdp.bEnglish)
			info.link = "GOOGLE STORE - https://play.google.com/store/apps/details?id=com.onebutton.eightfire\n\n APPLE STORE - https://itunes.apple.com/app/id1187814664";
		else
			info.link = "GOOGLE STORE - https://play.google.com/store/apps/details?id=com.onebutton.eightfire\n\n APPLE STORE - https://itunes.apple.com/kr/app/id1187814664";

		info.platform = sdkbox::SocialPlatform::Platform_Select;
		info.showDialog = true;
		sdkbox::PluginShare::share(info);

#endif
	}
	break;

	case SIDE_LEADERBOARD:
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		if (GameSharing::IsGPGAvailable() == true)
		{
			GameSharing::SubmitScore(mdp.AllPoint(), 0);
			GameSharing::ShowLeaderboards(0);
		}
		else
		{
			if (mdp.bEnglish == true)
				MessageBox("Failed", "Google Play Games App login required.");
			else
				MessageBox("실패", "구글 플레이 앱 로그인이 필요합니다.");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		GameSharing::SubmitScore(mdp.AllPoint(), 0);
		GameSharing::ShowLeaderboards(0);
#endif
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
				MessageBox("Failed", "Google Play Games App login required.");
			else
				MessageBox("실패", "구글 플레이 앱 로그인이 필요합니다.");
		}
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		GameSharing::ShowAchievementsUI();
#endif
		break;

	case SIDE_AUDIO:
		mdp.bSoundSwitch = mdp.bSoundSwitch ? false : true;
		break;

	case SIDE_MUSIC:
		mdp.bBGMSwitch = mdp.bBGMSwitch ? false : true;
		if (mdp.bBGMSwitch == true)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		else
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
		break;

	case SIDE_CREDIT:
		RemoveListener();
		director->replaceScene(TransitionFade::create(0.2f, CreditScene::createScene()));
		break;

	case SIDE_LANG:
	{
		mdp.bEnglish = mdp.bEnglish ? false : true;
		setLang();
	}
	break;

	default:
		break;
	}

}

void MenuScene::RemoveListener()
{
#ifdef	PLUGIN_AD_COLONY
	sdkbox::PluginAdColony::removeListener();
#endif
#ifdef PLUGIN_AD_MOB
	sdkbox::PluginAdMob::removeListener();
#endif
#ifdef PLUGIN_SHARE
	sdkbox::PluginShare::removeListener();
#endif

	this->getEventDispatcher()->removeAllEventListeners();
	this->removeAllChildren();
}