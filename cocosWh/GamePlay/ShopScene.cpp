#include "ShopScene.h"
#include "SimpleAudioEngine.h"
#include "PlayScene.h"

Scene* ShopScene::createScene()
{
	auto *scene = Scene::create();

	auto *layer = ShopScene::create();
	scene->addChild(layer);

	return scene;
}



void ShopScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	// Back button pressed
	if (keyCode == EventKeyboard::KeyCode::KEY_Z
		|| keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
		|| keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		ExitShop();
	}
}

void ShopScene::ExitShop()
{
	auto bgCircle = (Sprite*)this->getChildByTag(GAME_BG_CIRCLE);
	bgCircle->setVisible(true);
	auto bgAction = Sequence::create(
		ScaleTo::create(0.5f, 1.0f),
		CallFuncN::create(CC_CALLBACK_1(ShopScene::Callback, this, GAME_BG_CIRCLE_EXIT)),
		NULL);
	bgCircle->runAction(bgAction);

}



bool ShopScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	bReadyAd = false;
	ReadyAd();
	ReadyReward();

	auto director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	gameSize = Size(visibleSize.width, visibleSize.height - AD_HEIGHT_SIZE);

	auto k_listener = EventListenerKeyboard::create();
	k_listener->onKeyReleased = CC_CALLBACK_2(ShopScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(k_listener, 1);


	//BGACTION()
	{
		Sprite* bgCircle = NULL;
		if (mdp.bExtreme)
			bgCircle = Sprite::createWithSpriteFrameName("bgCircle2.png");
		else
			bgCircle = Sprite::createWithSpriteFrameName("bgCircle.png");

		bgCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bgCircle->setPosition(gameSize.width*0.5, gameSize.height*0.5);
		bgCircle->setScale(1.0f);
		bgCircle->setTag(GAME_BG_CIRCLE);
		this->addChild(bgCircle, 500);

		auto bgAction = Sequence::create(
			ScaleTo::create(0.4f, 4.0f),
			NULL);
		bgCircle->runAction(bgAction);
	}

	//initBG()
	{
		auto bgAd = Sprite::createWithSpriteFrameName("bgPlayAd.png");
		bgAd->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		bgAd->setPosition(visibleSize.width *0.5, visibleSize.height);
		this->addChild(bgAd, 10000);

		auto bg = Sprite::createWithSpriteFrameName("bgPlay.png");
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
		this->addChild(bg, 0);
	}

	//initSideMenu()
	{
		auto exit = Sprite::createWithSpriteFrameName("exit.png");
		auto exit_s = Sprite::createWithSpriteFrameName("exit_s.png");
		auto exitItem = MenuItemSprite::create(exit, exit_s, CC_CALLBACK_1(ShopScene::SideMenu, this));
		exitItem->setScale(SIDE_SCALE);
		exitItem->setTag(SHOP_SIDE_EXIT);

		auto movie = Sprite::createWithSpriteFrameName("movie.png");
		auto movie_s = Sprite::createWithSpriteFrameName("movie_s.png");
		auto movieItem = MenuItemSprite::create(movie, movie_s, CC_CALLBACK_1(ShopScene::SideMenu, this));
		movieItem->setScale(SIDE_SCALE);
		movieItem->setTag(SHOP_SIDE_MOVIE);

		auto menuSideUp = Menu::create(exitItem, movieItem, NULL);
		menuSideUp->setTag(GAME_SIDE_MENU_UP);
		menuSideUp->setPosition(gameSize.width * 0.5, gameSize.height * MENU_SIDE_UP_HEIGHT);
		menuSideUp->alignItemsHorizontallyWithPadding(400);
		this->addChild(menuSideUp, 9999);
	}

	//card
	{
		auto card1 = Sprite::createWithSpriteFrameName("card5.png");
		auto card2 = Sprite::createWithSpriteFrameName("card5.png");
		auto card3 = Sprite::createWithSpriteFrameName("card5.png");
		auto card4 = Sprite::createWithSpriteFrameName("card5.png");
		auto card1_s = Sprite::createWithSpriteFrameName("card5_s.png");
		auto card2_s = Sprite::createWithSpriteFrameName("card5_s.png");
		auto card3_s = Sprite::createWithSpriteFrameName("card5_s.png");
		auto card4_s = Sprite::createWithSpriteFrameName("card5_s.png");
		auto card1_0 = Sprite::createWithSpriteFrameName("card0.png");
		auto card2_0 = Sprite::createWithSpriteFrameName("card0.png");
		auto card3_0 = Sprite::createWithSpriteFrameName("card0.png");
		auto card4_0 = Sprite::createWithSpriteFrameName("card0.png");
		auto card1Item = MenuItemSprite::create(card1, card1_s, card1_0, CC_CALLBACK_1(ShopScene::SideMenu, this));
		auto card2Item = MenuItemSprite::create(card2, card2_s, card2_0, CC_CALLBACK_1(ShopScene::SideMenu, this));
		auto card3Item = MenuItemSprite::create(card3, card3_s, card3_0, CC_CALLBACK_1(ShopScene::SideMenu, this));
		auto card4Item = MenuItemSprite::create(card4, card4_s, card4_0, CC_CALLBACK_1(ShopScene::SideMenu, this));
		card1Item->setTag(MENU_GAME1);
		card2Item->setTag(MENU_GAME2);
		card3Item->setTag(MENU_GAME3);
		card4Item->setTag(MENU_GAME4);
		auto cardMenu = Menu::create(card1Item, card2Item, card3Item, card4Item, NULL);
		cardMenu->alignItemsVerticallyWithPadding(24);
		cardMenu->setPosition(gameSize.width * 0.5f, gameSize.height * MENU_HEIGHT_RATE);
		cardMenu->setTag(MENU_MENU);
		this->addChild(cardMenu, 1);
	}

	//card Label
	Label* labelCard1[4] = { NULL };
	Label* labelCard2[4] = { NULL };
	Label* labelCard3[4] = { NULL };
	float nLabelHeight = gameSize.height * MENU_HEIGHT_RATE + 60;
	float nInfoHeight = gameSize.height * MENU_HEIGHT_RATE + 8;
	for (int i = 0; i < 4; i++)
	{
		labelCard1[i] = Label::create();//createWithTTF("", _fonts_Base, CARD_LABEL_SIZE);
		labelCard1[i]->setSystemFontSize(CARD_LABEL_SIZE);
		labelCard1[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		labelCard1[i]->setPosition(gameSize.width * 0.2f, nLabelHeight + 180 - (144 * i));
		labelCard1[i]->setTag(MENU_CARD_LABEL + i);
		this->addChild(labelCard1[i], 2);
	}

	for (int i = 0; i < 4; i++)
	{
		labelCard2[i] = Label::create();// createWithTTF("", _fonts_Base, CARD_LABEL_SIZE2);
		labelCard2[i]->setSystemFontSize(CARD_LABEL_SIZE3);
		labelCard2[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		labelCard2[i]->setTextColor(Color4B::WHITE);
		labelCard2[i]->setPosition(gameSize.width * 0.8f, nLabelHeight + 180 - (144 * i));
		labelCard2[i]->setTag(MENU_CARD_LABEL2 + i);
		this->addChild(labelCard2[i], 2);
	}

	for (int i = 0; i < 4; i++)
	{
		labelCard3[i] = Label::create();// createWithTTF("", _fonts_Base, CARD_LABEL_SIZE3);
		labelCard3[i]->setSystemFontSize(CARD_LABEL_SIZE3);

		labelCard3[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		labelCard3[i]->setTextColor(Color4B::BLACK);
		labelCard3[i]->setPosition(gameSize.width * 0.8f, nInfoHeight + 180 - (144 * i));
		labelCard3[i]->setTag(MENU_CARD_LABEL3 + i);
		this->addChild(labelCard3[i], 2);
	}

	UpdateLabel();

	return true;
}

void ShopScene::UpdateLabel()
{
	// card Menu Check Enabled
	auto cardMenu = (Menu*)this->getChildByTag(MENU_MENU);
	auto card1Item = (MenuItemSprite*)cardMenu->getChildByTag(MENU_GAME1);
	auto card2Item = (MenuItemSprite*)cardMenu->getChildByTag(MENU_GAME2);
	auto card3Item = (MenuItemSprite*)cardMenu->getChildByTag(MENU_GAME3);
	auto card4Item = (MenuItemSprite*)cardMenu->getChildByTag(MENU_GAME4);

	if (mdp.nUpLife * 1000 <= mdp.nMoney && mdp.nUpLife < SHOP_UP_LIFE_MAX)
		card1Item->setEnabled(true);
	else
		card1Item->setEnabled(false);

	if (mdp.nUpRecovery * 1000 <= mdp.nMoney && mdp.nUpRecovery < SHOP_UP_RECOVERY_MAX)
		card2Item->setEnabled(true);
	else
		card2Item->setEnabled(false);

	if (mdp.nUpDefence * 1000 <= mdp.nMoney && mdp.nUpDefence < SHOP_UP_DEFENCE_MAX)
		card3Item->setEnabled(true);
	else
		card3Item->setEnabled(false);

	if (mdp.nUpPoint * 1000 <= mdp.nMoney && mdp.nUpPoint < SHOP_UP_POINT_MAX)
		card4Item->setEnabled(true);
	else
		card4Item->setEnabled(false);



	//

	// label update
	Label* labelCard1[4] = { NULL };
	for (int i = 0; i < 4; i++)
	{
		labelCard1[i] = (Label*)this->getChildByTag(MENU_CARD_LABEL + i);
	}
	labelCard1[0]->setString(StringUtils::format(mdp.bEnglish ? "MAX LIFE" : UTF8PCHAR("최대 생명력")));
	labelCard1[1]->setString(StringUtils::format(mdp.bEnglish ? "RECOVERY" : UTF8PCHAR("회복량 증가")));
	labelCard1[2]->setString(StringUtils::format(mdp.bEnglish ? "DEFENCE" : UTF8PCHAR("방어력 증가")));
	labelCard1[3]->setString(StringUtils::format(mdp.bEnglish ? "POINT" : UTF8PCHAR("포인트 증가")));


	Label* labelCard2[4] = { NULL };
	for (int i = 0; i < 4; i++)
	{
		labelCard2[i] = (Label*)this->getChildByTag(MENU_CARD_LABEL2 + i);
	}
	labelCard2[0]->setString(StringUtils::format(mdp.bEnglish ? "%d" : UTF8PCHAR("%d"), 100 + mdp.nUpLife * UP_PER_LIFE));
	labelCard2[1]->setString(StringUtils::format(mdp.bEnglish ? "+ %.1f%%" : UTF8PCHAR("+ %.1f%%"), mdp.nUpRecovery * UP_PER_RECOVERY * 100 ));
	labelCard2[2]->setString(StringUtils::format(mdp.bEnglish ? "+ %.1f%%" : UTF8PCHAR("+ %.1f%%"), mdp.nUpDefence * UP_PER_DEFENCE * 100));
	labelCard2[3]->setString(StringUtils::format(mdp.bEnglish ? "+ %.1f%%" : UTF8PCHAR("+ %.1f%%"), mdp.nUpPoint * UP_PER_POINT * 100));

	Label* labelCard3[4] = { NULL };
	for (int i = 0; i < 4; i++)
	{
		labelCard3[i] = (Label*)this->getChildByTag(MENU_CARD_LABEL3 + i);
	}
	labelCard3[0]->setString(StringUtils::format(mdp.bEnglish ? "%d P" : UTF8PCHAR("%d P"), mdp.nUpLife * 1000));
	labelCard3[1]->setString(StringUtils::format(mdp.bEnglish ? "%d P" : UTF8PCHAR("%d P"), mdp.nUpRecovery * 1000));
	labelCard3[2]->setString(StringUtils::format(mdp.bEnglish ? "%d P" : UTF8PCHAR("%d P"), mdp.nUpDefence * 1000));
	labelCard3[3]->setString(StringUtils::format(mdp.bEnglish ? "%d P" : UTF8PCHAR("%d P"), mdp.nUpPoint * 1000));

	if (mdp.nUpLife >= SHOP_UP_LIFE_MAX)
		labelCard3[0]->setString(mdp.bEnglish ? "Maximum" : UTF8PCHAR("최대치"));
	if (mdp.nUpRecovery >= SHOP_UP_RECOVERY_MAX)
		labelCard3[1]->setString(mdp.bEnglish ? "Maximum" : UTF8PCHAR("최대치"));
	if (mdp.nUpDefence >= SHOP_UP_DEFENCE_MAX)
		labelCard3[2]->setString(mdp.bEnglish ? "Maximum" : UTF8PCHAR("최대치"));
	if (mdp.nUpPoint >= SHOP_UP_POINT_MAX)
		labelCard3[3]->setString(mdp.bEnglish ? "Maximum" : UTF8PCHAR("최대치"));

	auto labelPoint = (Label*)this->getChildByTag(SHOP_POINT);
	if (labelPoint == NULL)
	{

		labelPoint = Label::create();// createWithTTF("", _fonts_Base, CARD_LABEL_SIZE3);
		labelPoint->setSystemFontSize(CARD_LABEL_SIZE * 2);
		labelPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelPoint->setTextColor(Color4B::BLACK);
		labelPoint->setPosition(gameSize.width * 0.5f, gameSize.height * 0.18f);
		labelPoint->setTag(SHOP_POINT);
		this->addChild(labelPoint, 2);
	}
	labelPoint->setString(StringUtils::format(mdp.bEnglish ? "%d P" : UTF8PCHAR("%d P"), mdp.nMoney));


}


void ShopScene::SideMenu(Ref* pSender)
{
	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();

	switch (nSelectedItem)
	{
	case SHOP_SIDE_EXIT:
	{
		ExitShop();
	}
	break;
	case SHOP_SIDE_MOVIE:
	{
		ShowReward();
	}
	break;
	case MENU_GAME1:
	{
		mdp.nMoney -= mdp.nUpLife * 1000;
		mdp.nUpLife += 1;

		if (mdp.bSoundSwitch == true)
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Coin);
		mdp.SaveShop();
		UpdateLabel();
	}
	break;
	case MENU_GAME2:
	{
		mdp.nMoney -= mdp.nUpRecovery * 1000;
		mdp.nUpRecovery += 1;

		if (mdp.bSoundSwitch == true)
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Coin);
		mdp.SaveShop();
		UpdateLabel();
	}
	break;
	case MENU_GAME3:
	{
		mdp.nMoney -= mdp.nUpDefence * 1000;
		mdp.nUpDefence += 1;

		if (mdp.bSoundSwitch == true)
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Coin);
		mdp.SaveShop();
		UpdateLabel();
	}
	break;
	case MENU_GAME4:
	{
		mdp.nMoney -= mdp.nUpPoint * 1000;
		mdp.nUpPoint += 1;

		if (mdp.bSoundSwitch == true)
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Coin);
		mdp.SaveShop();
		UpdateLabel();
	}
	break;

	}

}

void ShopScene::Callback(Ref* sender, int nType)
{
	if (nType == GAME_BG_CIRCLE_EXIT)
	{
		RemoveListener();//Replace 전에 써야됨. Replace 등록 후에 된 리스너도 삭제함.

		if (mdp.bSoundSwitch == true)
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Door);
		Director::getInstance()->replaceScene(PlayScene::createScene());
	}

}

void ShopScene::RemoveListener()
{
	this->getEventDispatcher()->removeAllEventListeners();
	this->unscheduleUpdate();
	this->removeAllChildren();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void ShopScene::ReadyAd()
{
	if (bReadyAd == false)
	{
		bReadyAd = true;
#ifdef PLUGIN_AD_MOB
		sdkbox::PluginAdMob::setListener(this);
#endif
	}
}

void ShopScene::ReadyReward()
{
	if (bReadyAd == true)
	{
#ifdef PLUGIN_AD_MOB
		if (sdkbox::PluginAdMob::isAvailable(AD_MOB_REWARD) == false)
		{
			sdkbox::PluginAdMob::cache(AD_MOB_REWARD);
		}
#endif
	}
}

void ShopScene::ShowReward()
{
	if (bReadyAd == true)
	{
#ifdef PLUGIN_AD_MOB
		if (sdkbox::PluginAdMob::isAvailable(AD_MOB_REWARD) == true)
		{
			sdkbox::PluginAdMob::show(AD_MOB_REWARD);
		}
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////