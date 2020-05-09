#include "PlayFunc.h"
#include "SimpleAudioEngine.h"
#include "time.h"
#include "PlayScene.h"
#include "ShopScene.h"
#include "../MenuScene.h"
//#include "2d/CCDrawingPrimitives.h" line
#include "../gpgs/GameSharing.h"
///////////////////////////////////////////////////////////////////////
void PlayFunc::SideMenu(Ref* pSender)
{
	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();

	switch (nSelectedItem)
	{
	case GAME_MENU_START:
	case GAME_MENU_CONTINUE:
	{
		nResetToast = 5;
		///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////
		Sound(_sounds_Bubble);
		///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////
		mdInfo.nPlayCount++;
		if (mdInfo.nPlayCount >= 4)
		{
			bFullCountAd = true;
			ReadyReward();
			mdInfo.nPlayCount = -4;
		}
		else if (nSelectedItem == GAME_MENU_CONTINUE)
		{
			ReadyReward();
		}

		///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////
		if (nGameNumber % 4 == 0)
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(_bgm_feelinggood, true);
		else if (nGameNumber % 4 == 1)
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(_bgm_stepstepstep, true);
		else if (nGameNumber % 4 == 2)
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(_bgm_happytune, true);
		else if (nGameNumber % 4 == 3)
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(_bgm_forest, true);

		if (MyDataManager::getInstance()->mPlayerData.bBGMSwitch == false)
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		///////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////


		auto bgPop = (Sprite*)pLayer->getChildByTag(GAME_BG_POP);
		if (bgPop != NULL)
		{
			float nTime = 0.8f;

			bgPop->stopAllActions();
			auto action = Sequence::create(
				ScaleTo::create(nTime, 0.0f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, nSelectedItem)),
				NULL);
			bgPop->runAction(action);

			ShowHideMenu(false);


			auto bgCircle = (Sprite*)pLayer->getChildByTag(GAME_BG_CIRCLE);
			if (bgCircle != NULL)
			{
				bgCircle->stopAllActions();
				auto action = Sequence::create(
					ScaleTo::create(nTime, 7.0f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, GAME_BG_CIRCLE_HIDE)),
					NULL);
				bgCircle->runAction(action);
			}
		}


	}
	break;

	case GAME_SIDE_EXIT:
	{
		if (bGameMenuPop == false)
		{
			ChangeLife(-100000000, true, false);
			CheckGameOver();
		}
		else if (bGameMenuPop == true)
		{
			ShowHideMenu(false);
			//	bGameOver = true;
			ExitGame();
		}
	}
	break;

	case GAME_SIDE_RESET:
	{
		nResetToast--;
		if (nResetToast < 0)
		{
			Sound(_sounds_Steam);
			ToastMessage(mdp.bEnglish ? "It has already been initialized." : UTF8PCHAR("이미 초기화되었습니다."));
		}
		else if (nResetToast == 0)
		{
			bRecordMode = false;
			nStageLevel = 0;
			mdp.SaveGameStageLevel(nGameNumber, nStageLevel);
			__nStagePoint__ = 0;
			mdp.SaveGamePoint(nGameNumber, __nStagePoint__);
			ShowHideMenu(true);

			Sound(_sounds_Steam);
			ToastMessage(mdp.bEnglish ? "It has been initialized." : UTF8PCHAR("초기화되었습니다."));
		}
		else
		{
			Sound(_sounds_Touch);
			ToastMessage(StringUtils::format(mdp.bEnglish ? "Press %d times to initialize the training." : UTF8PCHAR("%d번 더 누르면 훈련이 초기화됩니다."), nResetToast));
		}
	}
	break;

	case GAME_SIDE_SHOP:
	{
		Sound(_sounds_Bubble);
		ShowHideMenu(false);
		GoShop();
	}
	break;


	}

}

void PlayFunc::ToastMessage(std::string str)
{
	auto bgToast = (Sprite*)pLayer->getChildByTag(BG_TOAST);
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
	auto labelToast = (Label*)pLayer->getChildByTag(LABEL_TOAST);
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

void PlayFunc::ExitGame()
{
	auto bgCircle = (Sprite*)pLayer->getChildByTag(GAME_BG_CIRCLE);
	bgCircle->setVisible(true);
	auto bgAction = Sequence::create(
		ScaleTo::create(0.5f, 1.0f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, GAME_BG_CIRCLE_EXIT)),
		NULL);
	bgCircle->runAction(bgAction);
}

void PlayFunc::GoShop()
{
	auto bgCircle = (Sprite*)pLayer->getChildByTag(GAME_BG_CIRCLE);
	bgCircle->setVisible(true);
	auto bgAction = Sequence::create(
		ScaleTo::create(0.5f, 1.0f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, GAME_BG_CIRCLE_SHOP)),
		NULL);
	bgCircle->runAction(bgAction);


}

void PlayFunc::ResetSprite(Ref* sender)
{
	auto spr = (BlockSprite*)sender;
	spr->setVisible(false);


	switch (nGameNumber + 1)
	{
	case 5:
	{
		if (spr->nType == BLUE_CIRCLE)
		{
			ChangeLife(-20);
			UpdateCombo(-1, -1, 0);

			Particle_Bang(spr->getPosition());
		}
	}
	break;
	case 1:
	{
		if (spr->nType == GREEN_CIRCLE || spr->nType == RED_CIRCLE || spr->nType == BLUE_CIRCLE)
		{
			ChangeLife(-20);
			UpdateCombo(-1, -1, 0);

			Particle_Bang(spr->getPosition());
		}
	}
	break;
	case 15:
	case 7:
	case 11:
	{
		if (spr->nType == BLUE_SQUARE)
		{
			ChangeLife(-20);
			UpdateCombo(-1, -1, 0);

			Particle_Bang(spr->getPosition());
		}
	}
	break;

	}
}

void PlayFunc::ShowHideMenu(bool bOn)
{
	if (bOn == true)
	{
		if (bContinueClick == true || bFullCountAd == true)
		{
			bFullCountAd = false;
			ShowReward();
		}

		// 메뉴 출력
		auto menuPlay = (Menu*)pLayer->getChildByTag(GAME_MENU_MENU);
		if (menuPlay)
		{
			menuPlay->setVisible(true);
			//Continue 출력
			auto continueItem = (MenuItemSprite*)menuPlay->getChildByTag(GAME_MENU_CONTINUE);
			if (continueItem != NULL)
			{
				if (bShowContinueMenu == true
					&& bRecordMode == false
					&& __nStagePoint__ >= GAME_STAGE_POINT * 0.3)
				{
					continueItem->setEnabled(true);
				}
				else
				{
					continueItem->setEnabled(false);
				}
			}
		}

		auto menuSideUp = (Menu*)pLayer->getChildByTag(GAME_SIDE_MENU_UP);
		if (menuSideUp)
		{
			auto shopItem = (MenuItemSprite*)menuSideUp->getChildByTag(GAME_SIDE_SHOP);
			if (shopItem)
				shopItem->setVisible(true);
		}

		auto menuSideDown = (Menu*)pLayer->getChildByTag(GAME_SIDE_MENU_DOWN);
		if (menuSideDown)
		{
			auto resetItem = (MenuItemSprite*)menuSideDown->getChildByTag(GAME_SIDE_RESET);
			if (resetItem)
				resetItem->setVisible(true);
		}

		auto gb = (Sprite*)pLayer->getChildByTag(GAME_MENU_BG);
		if (gb)
		{
			gb->setVisible(true);
		}

		//name & tip
		auto labelName = (Label*)pLayer->getChildByTag(LABEL_NAME);
		if (labelName)
		{
			labelName->setVisible(true);
			std::string strTemp = getGameName(nGameNumber, mdp.bEnglish, true);
			labelName->setString(strTemp);
		}

		auto labelScore = (Label*)pLayer->getChildByTag(LABEL_SCORE);
		if (labelScore)
		{
			if (bRecordMode)
			{
				int nPoint = 0;

				if (bExtreme == true)
					nPoint = mdp.nGAME_EXTREME_POINT[nGameNumber];
				else
					nPoint = mdp.nGAME_NORMAL_POINT[nGameNumber];

				labelScore->setVisible(true);
				labelScore->setPositionY(gameSize.height * SCORE_TOP_RATE);
				std::string strTemp = getGameScore(nGameNumber, mdp.bEnglish, true);
				labelScore->setString(StringUtils::format(strTemp.c_str(), nPoint));
			}
			else
			{
				labelScore->setVisible(false);
			}

		}

		auto cardStar = (Menu*)pLayer->getChildByTag(LABEL_STAR_INFO);
		if (cardStar)
		{

			cardStar->setPositionY(gameSize.height * SCORE_TOP_RATE_STAR);
			cardStar->setVisible(true);
			for (int j = 0; j < mdp.nGAME_MAX_STAGE[nGameNumber]; j++)
			{
				((MenuItemSprite*)cardStar->getChildByTag(j))->unselected(); //star1

				if (bExtreme == true && j < mdp.nGAME_EXTREME_STAGE[nGameNumber])
					((MenuItemSprite*)cardStar->getChildByTag(j))->selected(); //star3
				else if (bExtreme == false && j < mdp.nGAME_NORMAL_STAGE[nGameNumber])
					((MenuItemSprite*)cardStar->getChildByTag(j))->selected(); //star2

			}
		}


		auto labelTip = (Label*)pLayer->getChildByTag(LABEL_TIP);
		if (labelTip)
		{
			labelTip->setVisible(true);
			labelTip->setString(mdp.bEnglish ? strTip[nGameNumber] : UTF8PCHAR(strKoTip[nGameNumber]));
		}

		auto label_Result = (Label*)pLayer->getChildByTag(LABEL_RESULT);
		if (label_Result)
		{
			if (bLevelUp == true)
			{
				label_Result->setVisible(true);
				label_Result->setString(StringUtils::format(mdp.bEnglish ? "[ Training Phase UP ]" : UTF8PCHAR("[ 훈련 단계 상승 ]"), __nStagePoint__));
			}
			else if (bRecordMode)
			{
				if (__nStagePoint__ != 0)// 스테이지 0 체크 이유는 제일 처음 작업
				{
					label_Result->setVisible(true);
					label_Result->setString(StringUtils::format(mdp.bEnglish ? "[ %d ]" : UTF8PCHAR("[ %d ]"), __nStagePoint__));

				}
			}
			else if (__nStagePoint__ != 0)
			{
				label_Result->setVisible(true);
				label_Result->setString(StringUtils::format(mdp.bEnglish ? "[ Failure of Training ]" : UTF8PCHAR("[ 훈련 실패 ]"), __nStagePoint__));
			}
			else
			{
				label_Result->setVisible(false);
			}
		}

	}
	else
	{
		auto menuPlay = (Menu*)pLayer->getChildByTag(GAME_MENU_MENU);
		if (menuPlay)
		{
			menuPlay->setVisible(false);
		}

		auto menuSideUp = (Menu*)pLayer->getChildByTag(GAME_SIDE_MENU_UP);
		if (menuSideUp)
		{
			auto shopItem = (MenuItemSprite*)menuSideUp->getChildByTag(GAME_SIDE_SHOP);
			if (shopItem)
				shopItem->setVisible(false);
		}

		auto menuSideDown = (Menu*)pLayer->getChildByTag(GAME_SIDE_MENU_DOWN);
		if (menuSideDown)
		{
			auto resetItem = (MenuItemSprite*)menuSideDown->getChildByTag(GAME_SIDE_RESET);
			if (resetItem)
				resetItem->setVisible(false);
		}

		auto gb = (Sprite*)pLayer->getChildByTag(GAME_MENU_BG);
		if (gb)
		{
			gb->setVisible(false);
		}

		auto labelName = (Label*)pLayer->getChildByTag(LABEL_NAME);
		if (labelName)
		{
			labelName->setVisible(false);
		}
		auto labelScore = (Label*)pLayer->getChildByTag(LABEL_SCORE);
		if (labelScore)
		{
			labelScore->setVisible(false);
		}

		auto cardStar = (Menu*)pLayer->getChildByTag(LABEL_STAR_INFO);
		if (cardStar)
		{
			cardStar->setVisible(false);
		}

		auto labelTip = (Label*)pLayer->getChildByTag(LABEL_TIP);
		if (labelTip)
		{
			labelTip->setVisible(false);
		}

		auto label_Result = (Label*)pLayer->getChildByTag(LABEL_RESULT);
		if (label_Result)
		{
			label_Result->setVisible(false);
		}
	}


}
void PlayFunc::Callback(Ref* sender, int nType)
{
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	if (nType == GAME_BG_CIRCLE_HIDE)
	{
		auto bgCircle = (Sprite*)sender;
		bgCircle->setVisible(false);
	}
	else if (nType == GAME_BG_CIRCLE_EXIT) //종료할 떄
	{
		ClearStage();
		bExitScene = true; // Using PlayScene
		return;
	}
	else if (nType == GAME_BG_CIRCLE_SHOP)
	{
		pLayer->getEventDispatcher()->removeAllEventListeners();
		pLayer->unscheduleUpdate();
		pLayer->removeAllChildren();

		Director::getInstance()->replaceScene(ShopScene::createScene());
	}

	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	if (nType == GAME_MENU_START)
	{
		bShowContinueMenu = true;
		bContinueClick = false;
		__nStagePoint__ = 0;

		mdp.nGAME_PLAY_COUNT[nGameNumber] ++;
		mdp.SaveGamePlayCount(nGameNumber);

	}
	else if (nType == GAME_MENU_CONTINUE)
	{
		bShowContinueMenu = false;
		bContinueClick = true;
	}

	if (nType == GAME_MENU_START || nType == GAME_MENU_CONTINUE)
	{
		if (nStageLevel == mdp.nGAME_MAX_STAGE[nGameNumber])
			bRecordMode = true;

		float nMaxLife = 100 + (UP_PER_LIFE * mdp.nUpLife);
		p1.Init(nMaxLife);
		p1.__ChangeLife__(50, true);
		UpdateProgress();
		Invincible(2.0f);

		bTouch = false;
		bPrevTouch = false;
		bCallBackDelay = false;
		bLevelUp = false;

		bNeedSetting = true;

		UpdateCombo(-1, -1, 0);
		nSumChangeLife = 0;
		nMainTick = 0;
		nTimeDifficulty = 0;

		bGameMenuPop = false;

		return;
	}
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	if (p1.GetLife() == 0) // 게임 종료 직후 올라오는 콜백 막고 초기화.
	{
		bCallBackDelay = false;
		return;
	}
	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	switch (nGameNumber + 1)
	{

	case 2:
	{
		if (nType == 0)
		{
			bCallBackDelay = false;
			if (nCallBackDelayLevel == nStageLevel)
				Set_UnifyType();
		}
	}
	break;


	case 6:
	{
		if (nType == 0)
		{
			bSettingDelay = true;
		}
		else if (nType == 1)
		{
			bCallBackDelay = false;
			if (nCallBackDelayLevel == nStageLevel)
				Set_MemorizeColors();
		}
	}
	break;


	case 910:
	{
		if (nType == 1)
		{
			Set_MakeWay();
			bCallBackDelay = false;
		}
		else if (nType == 2) // 카드 이동 액션 후
		{
			Sound(_sounds_Touch);
			bCallBackDelay = false;
			Draw_MakeWay();
		}
	}
	break;


	case 914:
	{
		if (nType == 5) // 첫 세팅 후
		{
			Shuffle_RotateSlide();
		}
		else if (nType == 1) // 정답 맞춘 후
		{
			Set_RotateSlide();
		}
		else if (nType == 0) // 셔플 완료 후
		{
			bCallBackDelay = false;
			Sound(_sounds_Bubble);
			Draw_RotateSlide(true);
		}
		else if (nType == 2) // 카드 이동 액션 후
		{
			Sound(_sounds_Touch);
			bCallBackDelay = false;
			Draw_RotateSlide(true);
		}
	}
	break;

	////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	}

}

void PlayFunc::DeleteSprite(Ref* sender)
{
	auto spr = (BlockSprite*)sender;
	pLayer->removeChild(spr);
}

void PlayFunc::ResetParticle(Ref* sender)
{
	auto particle = (ParticleSystemQuad*)sender;
	pLayer->removeChild(particle);
}

void PlayFunc::MenuPop()
{
	auto bgPop = (Sprite*)pLayer->getChildByTag(GAME_BG_POP);
	if (bgPop != NULL)
	{
		float nTime = 0.8f;
		bgPop->stopAllActions();
		auto action = Sequence::create(
			ScaleTo::create(nTime, 1.0f),
			CallFuncN::create(CC_CALLBACK_0(PlayFunc::MenuPopData, this)),
			NULL);

		bgPop->runAction(action);

		auto bgCircle = (Sprite*)pLayer->getChildByTag(GAME_BG_CIRCLE);
		bgCircle->setVisible(true);
		if (bgCircle != NULL)
		{
			bgCircle->stopAllActions();
			auto action = Sequence::create(
				ScaleTo::create(nTime, 4.0f),
				NULL);
			bgCircle->runAction(action);
		}
	}
}

void PlayFunc::MenuPopData()
{
	ShowHideMenu(true);
}

void PlayFunc::init(Layer* a_pLayer)
{
	bReadyAd = false;
	ReadyAd();

	srand(time(NULL));
	CCRANDOM_0_1();

	nResetToast = 5;
	//////////////////////////////////////////////
	pLayer = a_pLayer;
	director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	gameSize = Size(visibleSize.width, visibleSize.height - AD_HEIGHT_SIZE);

	origin = Vec2(0, 100);
	playSize.x = 600.0f;
	playSize.y = 800.0f;
	center = Vec2(playSize.x * 0.5f, origin.y + playSize.y * 0.5f);
	nMainTick = 0;
	//////////////////////////////////////////////
	bGameMenuPop = true;
	bExitScene = false;

	bShowContinueMenu = false;
	bContinueClick = false;
	bFullCountAd = false;
	//////////////////////////////////////////////
	bNeedSetting = true;
	//////////////////////////////////////////////
	bTouch = false;
	bPrevTouch = false;
	bCallBackDelay = false;
	bLevelUp = false;
	//////////////////////////////////////////////
	bExtreme = mdp.bExtreme;
	nGameNumber = mdInfo.nGameNumber;

	if (bExtreme == true)
		nStageLevel = mdp.nGAME_EXTREME_STAGE[nGameNumber];
	else
		nStageLevel = mdp.nGAME_NORMAL_STAGE[nGameNumber];

	bRecordMode = false;
	if (nStageLevel == mdp.nGAME_MAX_STAGE[nGameNumber])
		bRecordMode = true;

	__nStagePoint__ = 0;
	__nCombo__ = 0;
	nMaxCombo = mdp.nMaxCombo;
	////////////////////////////////////////////////////////////////////////
	//BGACTION()
	{
		Sprite* bgCircle = NULL;
		if (bExtreme)
			bgCircle = Sprite::createWithSpriteFrameName("bgCircle2.png");
		else
			bgCircle = Sprite::createWithSpriteFrameName("bgCircle.png");

		bgCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bgCircle->setPosition(gameSize.width*0.5, gameSize.height*0.5);
		bgCircle->setScale(1.0f);
		bgCircle->setTag(GAME_BG_CIRCLE);
		pLayer->addChild(bgCircle, 500);

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
		pLayer->addChild(bgAd, 10000);

		auto bg = Sprite::createWithSpriteFrameName("bgPlay.png");
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
		pLayer->addChild(bg, 0);
	}

	//initToast()
	{
		auto bgToast = Sprite::createWithSpriteFrameName("toast.png");
		bgToast->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bgToast->setPosition(gameSize.width*0.5, 48);
		bgToast->setTag(BG_TOAST);
		pLayer->addChild(bgToast, 10000);
		bgToast->setScale(0.0f);

		auto labelToast = Label::create();// createWithTTF("test", _fonts_Base, TOAST_FONT_SIZE);
		labelToast->setSystemFontSize(TOAST_FONT_SIZE);

		labelToast->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelToast->setPosition(gameSize.width*0.5, 48);
		labelToast->setTextColor(Color4B::BLACK);
		labelToast->setTag(LABEL_TOAST);
		pLayer->addChild(labelToast, 10001);
		labelToast->setScale(0.0f);
	}

	//initResultPop()
	{
		//bg action
		{
			auto bgPop = Sprite::createWithSpriteFrameName("bg.png");
			bgPop->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bgPop->setPosition(gameSize.width*0.5, gameSize.height*0.5);
			bgPop->setTag(GAME_BG_POP);
			pLayer->addChild(bgPop, 450);
		}

		{
			auto gm1 = Sprite::createWithSpriteFrameName("start.png");
			auto gb1 = Sprite::createWithSpriteFrameName("start_s.png");
			auto gm1Item = MenuItemSprite::create(gm1, gb1, CC_CALLBACK_1(PlayFunc::SideMenu, this));
			gm1Item->setTag(GAME_MENU_START);

			float nMenuScale = 0.75f;
			gm1Item->setScale(nMenuScale);

			//MENU
			auto menuPlay = Menu::create(gm1Item, NULL);
			menuPlay->setPosition(gameSize.width * 0.5, gameSize.height * GAME_MENU_RATE);
			menuPlay->setTag(GAME_MENU_MENU);
			menuPlay->setVisible(false);
			pLayer->addChild(menuPlay, 451);
		}


		//name tip
		auto labelName = Label::create();// createWithTTF("test", _fonts_Base, PLAY_FONT_SIZE, Size::ZERO, TextHAlignment::CENTER);
		labelName->setSystemFontSize(PLAY_FONT_SIZE);
		labelName->setAlignment(TextHAlignment::LEFT);

		labelName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		labelName->setPosition(gameSize.width*0.5, gameSize.height* NAME_RATE);
		labelName->setTextColor(Color4B::BLACK);
		labelName->setTag(LABEL_NAME);
		labelName->setVisible(false);
		pLayer->addChild(labelName, 452);


		auto labelTip = Label::create();// createWithTTF("test", _fonts_Base, PLAY_FONT_SIZE*0.55f, Size::ZERO, TextHAlignment::LEFT);
		labelTip->setSystemFontSize(PLAY_FONT_SIZE*0.55f);
		labelTip->setAlignment(TextHAlignment::LEFT);

		labelTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		labelTip->setPosition(gameSize.width*0.5, gameSize.height* TIP_RATE);
		labelTip->setTextColor(Color4B::BLACK);
		labelTip->setTag(LABEL_TIP);
		labelTip->setVisible(false);
		pLayer->addChild(labelTip, 452);



		auto label_Result = Label::create();// createWithTTF("test", _fonts_Base, PLAY_FONT_SIZE, Size::ZERO, TextHAlignment::LEFT);
		label_Result->setSystemFontSize(PLAY_FONT_SIZE);
		label_Result->setAlignment(TextHAlignment::LEFT);

		label_Result->setPosition(gameSize.width*0.5, gameSize.height* SCORE_BOTTOM_RATE);
		if (bExtreme == true)
			label_Result->setTextColor(Color4B::MAGENTA);
		else
			label_Result->setTextColor(Color4B::ORANGE);
		label_Result->setTag(LABEL_RESULT);
		label_Result->setVisible(false);
		pLayer->addChild(label_Result, 501);
	}


	////////////

	//initSideMenu()
	{
		auto exit = Sprite::createWithSpriteFrameName("exit.png");
		auto exit_s = Sprite::createWithSpriteFrameName("exit_s.png");
		auto exitItem = MenuItemSprite::create(exit, exit_s, CC_CALLBACK_1(PlayFunc::SideMenu, this));
		exitItem->setScale(SIDE_SCALE);
		exitItem->setTag(GAME_SIDE_EXIT);

		auto shop = Sprite::createWithSpriteFrameName("shop.png");
		auto shop_s = Sprite::createWithSpriteFrameName("shop_s.png");
		auto shopItem = MenuItemSprite::create(shop, shop_s, CC_CALLBACK_1(PlayFunc::SideMenu, this));
		shopItem->setScale(SIDE_SCALE);
		shopItem->setTag(GAME_SIDE_SHOP);

		auto menuSideUp = Menu::create(exitItem, shopItem, NULL);
		menuSideUp->setTag(GAME_SIDE_MENU_UP);
		menuSideUp->setPosition(gameSize.width * 0.5, gameSize.height * MENU_SIDE_UP_HEIGHT);
		menuSideUp->alignItemsHorizontallyWithPadding(400);
		pLayer->addChild(menuSideUp, 9999);

		/*
		auto reset = Sprite::createWithSpriteFrameName("reset.png");
		auto reset_s = Sprite::createWithSpriteFrameName("reset_s.png");
		auto resetItem = MenuItemSprite::create(reset, reset_s, CC_CALLBACK_1(PlayFunc::SideMenu, this));
		resetItem->setScale(SIDE_SCALE);
		resetItem->setTag(GAME_SIDE_RESET);

		auto menuSideDown = Menu::create(resetItem, NULL);
		menuSideDown->setTag(GAME_SIDE_MENU_DOWN);
		menuSideDown->setPosition(gameSize.width * 0.1f, gameSize.height * MENU_SIDE_DOWN_HEIGHT);
		pLayer->addChild(menuSideDown, 9999);
		*/
	}

	//InitProgress()
	{
		//Bar BG
		auto BarBG1 = Sprite::createWithSpriteFrameName("EmptyBar.png");
		BarBG1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		BarBG1->setPosition(Vec2(gameSize.width * 0.5, gameSize.height));
		pLayer->addChild(BarBG1, 400);

		//Life Bar Progress
		auto spriteLifeBar1 = Sprite::createWithSpriteFrameName("BlueBar.png");
		auto progressLifeTimer1 = ProgressTimer::create(spriteLifeBar1);	// ProgressTimer생성
		progressLifeTimer1->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
		progressLifeTimer1->setMidpoint(Vec2(0, 0));         // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
		progressLifeTimer1->setBarChangeRate(Vec2(1, 0));    // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
		progressLifeTimer1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		progressLifeTimer1->setPosition(BarBG1->getPosition());
		progressLifeTimer1->setTag(PLAYER_LIFE_BAR);
		progressLifeTimer1->setPercentage(0.0f);
		pLayer->addChild(progressLifeTimer1, 401);

		//Back Bar Progress
		auto spriteBackBar1 = Sprite::createWithSpriteFrameName("BackBar.png");
		auto progressBackTimer1 = ProgressTimer::create(spriteBackBar1);	// ProgressTimer생성
		progressBackTimer1->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
		progressBackTimer1->setMidpoint(Vec2(0, 0));         // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
		progressBackTimer1->setBarChangeRate(Vec2(1, 0));    // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
		progressBackTimer1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		progressBackTimer1->setPosition(BarBG1->getPosition());
		progressBackTimer1->setTag(PLAYER_BACK_BAR);
		progressBackTimer1->setPercentage(progressLifeTimer1->getPercentage());
		pLayer->addChild(progressBackTimer1, 402);
		progressBackTimer1->setVisible(false);

		//EdgeBar
		auto UpBar = Sprite::createWithSpriteFrameName("EdgeBar.png");
		UpBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		UpBar->setPosition(BarBG1->getPosition());
		pLayer->addChild(UpBar, 403);

		//CrashBar
		auto CrashBar = Sprite::createWithSpriteFrameName("CrashBar.png");
		CrashBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		CrashBar->setPosition(BarBG1->getPosition());
		CrashBar->setTag(CRASH_BAR);
		pLayer->addChild(CrashBar, 404);
		CrashBar->setVisible(false);

		//Crash
		auto CrashBG = Sprite::createWithSpriteFrameName("crash.png");
		CrashBG->setPosition(gameSize.width*0.5, gameSize.height*0.5);
		CrashBG->setTag(CRASH_BG);
		pLayer->addChild(CrashBG, 405);
		CrashBG->setVisible(false);
	}


	//InitStagePointProgress()
	{
		//Bar BG
		auto BarBG2 = Sprite::createWithSpriteFrameName("EmptyBar.png");
		BarBG2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		BarBG2->setPosition(Vec2(gameSize.width * 0.5, 0));
		pLayer->addChild(BarBG2, 400);

		Sprite* StageBar = NULL;
		if (bExtreme)
			StageBar = Sprite::createWithSpriteFrameName("RedBar.png");
		else
			StageBar = Sprite::createWithSpriteFrameName("YellowBar.png");

		auto progressStageTimer = ProgressTimer::create(StageBar);	// ProgressTimer생성
		progressStageTimer->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
		progressStageTimer->setMidpoint(Vec2(0, 0));         // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
		progressStageTimer->setBarChangeRate(Vec2(1, 0));    // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
		progressStageTimer->setAnchorPoint(BarBG2->getAnchorPoint());
		progressStageTimer->setPosition(BarBG2->getPosition());
		progressStageTimer->setTag(STAGE_BAR);
		progressStageTimer->setPercentage(0.0f);
		pLayer->addChild(progressStageTimer, 401);

		if (bRecordMode)
		{
			progressStageTimer->setPercentage(100);
		}

		//EdgeBar
		auto DownBar = Sprite::createWithSpriteFrameName("EdgeBar.png");
		DownBar->setAnchorPoint(BarBG2->getAnchorPoint());
		DownBar->setPosition(BarBG2->getPosition());
		pLayer->addChild(DownBar, 403);

	}

	//InitLabelStage()
	{
		auto labelScore = Label::create();// createWithTTF("", _fonts_Base, COMBO_FONT_SIZE * 0.8f);
		labelScore->setSystemFontSize(COMBO_FONT_SIZE * 0.8f);
		if (bExtreme)
			labelScore->setTextColor(Color4B::MAGENTA);
		else
			labelScore->setTextColor(Color4B::ORANGE);

		labelScore->setTag(LABEL_SCORE);
		labelScore->setPosition(Vec2(gameSize.width * 0.5, gameSize.height * SCORE_TOP_RATE));
		pLayer->addChild(labelScore, 499);

		labelScore->setVisible(false);


		MenuItemSprite* sprItem[MAX_STAR_PHASE] = { NULL };
		for (int i = 0; i < mdp.nGAME_MAX_STAGE[nGameNumber]; i++)
		{
			Sprite* spr1 = Sprite::createWithSpriteFrameName("star1.png");
			Sprite* spr2 = NULL;
			if (bExtreme == true)
				spr2 = Sprite::createWithSpriteFrameName("star3.png");
			else
				spr2 = Sprite::createWithSpriteFrameName("star2.png");

			sprItem[i] = MenuItemSprite::create(spr1, spr2);
			sprItem[i]->setEnabled(false);
			sprItem[i]->setTag(i);
			sprItem[i]->setScale(0.5f);
		}
		auto cardStar = Menu::create(
			sprItem[0], sprItem[1], sprItem[2], sprItem[3], sprItem[4],
			sprItem[5], sprItem[6], sprItem[7], NULL);
		cardStar->alignItemsHorizontallyWithPadding(2);
		cardStar->setPosition(Vec2(gameSize.width * 0.5, gameSize.height * SCORE_TOP_RATE_STAR));
		cardStar->setTag(LABEL_STAR_INFO);
		pLayer->addChild(cardStar, 499);

		cardStar->setVisible(false);
	}

	ShowHideMenu(true);
}

BlockSprite* PlayFunc::getInvisibleBlock(int nType, int nOrder)
{
	BlockSprite* block = NULL;

	int k = 0;

	for (k = 0; k < vBlock[nType].size(); k++)
	{
		block = vBlock[nType].at(k);
		if (block->isVisible() == false)
		{
			break;
		}
	}
	//쓰던게 없으면 새로 생성
	if (k == vBlock[nType].size())
	{
		block = BlockSprite::createWithSpriteFrameName(strBlockName[nType]);
		vBlock[nType].pushBack(block);
		pLayer->addChild(block, nOrder);
	}


	block->setVisible(true);
	block->nType = nType;
	return block;

}


bool PlayFunc::CheckGameOver()
{
	if (p1.GetLife() <= 0)
	{
		//Money
		int nMoney = __nStagePoint__;
		if (bExtreme)
			nMoney = nMoney * 4;

		cocos2d::MessageBox(StringUtils::format("%d P", nMoney).c_str(), "+");
		mdp.nMoney += nMoney;
		mdp.SaveShop();
		//
		__nStagePoint__ = 0;

		if (bRecordMode)
		{
			if (bExtreme == true && mdp.nGAME_EXTREME_POINT[nGameNumber] < __nStagePoint__
				|| bExtreme == false && mdp.nGAME_NORMAL_POINT[nGameNumber] < __nStagePoint__)
			{
				mdp.SaveGamePoint(nGameNumber, __nStagePoint__);
			}

		}

		if (mdp.nMaxCombo < nMaxCombo)
		{
			mdp.nMaxCombo = nMaxCombo;
			mdp.SaveMaxCombo();
		}

		UpdateCombo(-1, -1, 0);
		ClearStage();
		MenuPop();

		bLevelUp = false;
		bGameMenuPop = true;
		Sound(_sounds_Door);

		return true;
	}

	return false;
}

void PlayFunc::CheckStageLevelUp()
{
	if (bRecordMode)
		return;

	if (__nStagePoint__ > GAME_STAGE_POINT)
		__nStagePoint__ = GAME_STAGE_POINT;

	if (__nStagePoint__ >= GAME_STAGE_POINT)
	{
		//Money
		int nMoney = __nStagePoint__;
		if (bExtreme)
			nMoney = nMoney * 4;

		cocos2d::MessageBox(StringUtils::format("%d P", nMoney).c_str(), "+");
		mdp.nMoney += nMoney;
		mdp.SaveShop();
		//

		__nStagePoint__ = 0;



		nStageLevel++;
		if (nStageLevel >= mdp.nGAME_MAX_STAGE[nGameNumber])
		{
			nStageLevel = mdp.nGAME_MAX_STAGE[nGameNumber];
		}

		mdp.SaveGameStageLevel(nGameNumber, nStageLevel);
		mdp.SaveGamePoint(nGameNumber, 0);
		if (mdp.nMaxCombo < nMaxCombo)
		{
			mdp.nMaxCombo = nMaxCombo;
			mdp.SaveMaxCombo();
		}

#if USE_LEADERBOARD == 1
		if (GameSharing::IsGPGAvailable() == true)
		{
			if (mdp.GetPlayerLevel() <= mdp.GetMaxLevel())
				GameSharing::SubmitScore(mdp.GetPlayerLevel(), LEADERBOARD_STAR);

			if (mdp.GetExtremePlayerLevel() <= mdp.GetMaxLevel())
				GameSharing::SubmitScore(mdp.GetExtremePlayerLevel(), LEADERBOARD_STAR_EX);
		}

#endif

		UpdateCombo(-1, -1, 0);
		ClearStage();
		MenuPop();

		bLevelUp = true; //레벨업 라벨에 쓰임
		bGameMenuPop = true;
		Sound(_sounds_Sword);
	}

}

void PlayFunc::UpdateProgress()
{
	auto progressLifeTimer1 = (ProgressTimer*)pLayer->getChildByTag(PLAYER_LIFE_BAR);
	if (progressLifeTimer1)
	{
		progressLifeTimer1->setPercentage(p1.GetLifePercent());
	}

	auto progressBackTimer1 = (ProgressTimer*)pLayer->getChildByTag(PLAYER_BACK_BAR);
	if (progressBackTimer1)
	{
		progressBackTimer1->setPercentage(p1.GetLifePercent());
		if (bInvincible == true)
			progressBackTimer1->setVisible(true);
		else
			progressBackTimer1->setVisible(false);
	}


	//스테이지 정도 시각적 표시 방법 추가
	auto progressStageTimer = (ProgressTimer*)pLayer->getChildByTag(STAGE_BAR);
	if (progressStageTimer)
	{
		if (bRecordMode)
		{
			progressStageTimer->setPercentage(__nStagePoint__ / (GAME_RECORD_MAX_POINT / 100));
		}
		else
		{
			progressStageTimer->setPercentage(__nStagePoint__ / (GAME_STAGE_POINT / 100));
		}
	}

	auto labelScore = (Label*)pLayer->getChildByTag(LABEL_SCORE);
	if (labelScore)
	{
		if (bRecordMode)
		{
			labelScore->setPositionY(gameSize.height * SCORE_BOTTOM_RATE);
			labelScore->setVisible(true);
			labelScore->setString(StringUtils::format(mdp.bEnglish ? "%d" : UTF8PCHAR("%d"), __nStagePoint__));
		}
	}

	auto cardStar = (Menu*)pLayer->getChildByTag(LABEL_STAR_INFO);
	if (cardStar)
	{
		if (bRecordMode == false)
		{
			cardStar->setPositionY(gameSize.height * SCORE_BOTTOM_RATE_STAR);
			cardStar->setVisible(true);
		}
	}

}



void PlayFunc::Invincible(float nTime)
{

	bInvincible = true;
	pLayer->runAction(
		Sequence::create(
			DelayTime::create(nTime),
			CallFuncN::create(CC_CALLBACK_0(PlayFunc::ResetInvincible, this)),
			NULL));

	Sound(_sounds_Sword);
}

void PlayFunc::ResetInvincible()
{
	bInvincible = false;
}

void PlayFunc::ResetCrashBar()
{
	auto CrashBar = (Sprite*)pLayer->getChildByTag(CRASH_BAR);
	if (CrashBar)
		CrashBar->setVisible(false);

	auto CrashBG = (Sprite*)pLayer->getChildByTag(CRASH_BG);
	if (CrashBG)
		CrashBG->setVisible(false);
}


void PlayFunc::ClearStage()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

	for (int nType = BLOCK_BASE; nType < BLOCK_MAX_COUNT; nType++)
	{
		for (int i = 0; i < vBlock[nType].size(); i++)
		{
			vBlock[nType].at(i)->stopAllActions();
			vBlock[nType].at(i)->setVisible(false);
		}
		vBlock[nType].clear();
	}
}

void PlayFunc::SettingStage()
{
	switch (nGameNumber + 1)
	{

	case 1:
	case 5:
	{
		if (nStageLevel == 0)
			nLineCount = 3;
		else if (nStageLevel <= 1)
			nLineCount = 4;
		else
			nLineCount = 4;

		nMaxArise = nLineCount * 1.5;

		if (bExtreme)
		{
			nLineCount += 1;
			nMaxArise = nMaxArise + 2;
		}

	}
	break;

	case 2:
	{
		if (nStageLevel == 0)
			nLineCount = 2;
		else if (nStageLevel <= 1)
			nLineCount = 3;
		else if (nStageLevel <= 2)
			nLineCount = 4;
		else
			nLineCount = 4;
	}
	break;

	case 8:
	{
		if (nStageLevel == 0)
			nLineCount = 7;
		else if (nStageLevel <= 1)
			nLineCount = 8;
		else
			nLineCount = 8;
	}
	break;

	case 6:
	{
		if (nStageLevel == 0)
			nLineCount = 3;
		else if (nStageLevel <= 1)
			nLineCount = 4;
		else if (nStageLevel <= 2)
			nLineCount = 5;
		else if (nStageLevel <= 3)
			nLineCount = 6;
		else
			nLineCount = 6;

		if (bExtreme)
		{
			nLineCount += 2;
		}
	}
	break;

	case 10:
	{
		if (nStageLevel == 0)
			nLineCount = 3;
		else if (nStageLevel <= 1)
			nLineCount = 4;
		else if (nStageLevel <= 2)
			nLineCount = 5;
		else if (nStageLevel <= 3)
			nLineCount = 6;
		else
			nLineCount = 6;

		if (bExtreme)
		{
			nLineCount += 1;
		}
	}
	break;

	case 910:
	{
		if (nStageLevel <= 0)
			nLineCount = 6;
		else if (nStageLevel <= 1)
			nLineCount = 7;
		else if (nStageLevel <= 2)
			nLineCount = 8;
		else
			nLineCount = 8;
	}
	break;

	case 7:
	case 11:
	case 15:
	{
		if (nStageLevel <= 0)
			nLineCount = 3;
		else if (nStageLevel <= 1)
			nLineCount = 4;
		else if (nStageLevel <= 2)
			nLineCount = 5;
		else
			nLineCount = 5;

		nMaxArise = 3 + nStageLevel * 2;
	}
	break;

	case 914:
	{
		if (nStageLevel <= 0)
			nLineCount = 3;
		else if (nStageLevel <= 1)
			nLineCount = 4;
		else
			nLineCount = 4;
	}
	break;

	}


	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	nLineBlockScale = SCALE_LINE_8;
	if (nLineCount == 2)
		nLineBlockScale = SCALE_LINE_2;
	else if (nLineCount == 3)
		nLineBlockScale = SCALE_LINE_3;
	else if (nLineCount == 4)
		nLineBlockScale = SCALE_LINE_4;
	else if (nLineCount == 5)
		nLineBlockScale = SCALE_LINE_5;
	else if (nLineCount == 6)
		nLineBlockScale = SCALE_LINE_6;
	else if (nLineCount == 7)
		nLineBlockScale = SCALE_LINE_7;
	else if (nLineCount == 8)
		nLineBlockScale = SCALE_LINE_8;

	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			arrPos[i][j].x = ((float)(i * 2 + 1) / (nLineCount * 2)  * playSize.x) + origin.x;
			arrPos[i][j].y = ((float)(j * 2 + 1) / (nLineCount * 2)  * (playSize.x + playSize.y) / 2) + origin.y + (playSize.y - playSize.x) / 2 / 2;

			if (nGameNumber + 1 == 2
				|| nGameNumber + 1 == 6
				|| nGameNumber + 1 == 10
				|| nGameNumber + 1 == 910
				|| nGameNumber + 1 == 914)
			{

				/*
				auto bgPuzzle = (Sprite*)pLayer->getChildByTag(BG_PUZZLE);
				if (bgPuzzle == NULL)
				{
					bgPuzzle = Sprite::createWithSpriteFrameName("bgPlayPuzzle.png");
					bgPuzzle->setPosition(center);
					bgPuzzle->setTag(BG_PUZZLE);
					pLayer->addChild(bgPuzzle);
				}
				*/
				arrPos[i][j].x = ((float)(i * 2 + 1) / (nLineCount * 2)  * playSize.x) + origin.x;
				arrPos[i][j].y = ((float)(j * 2 + 1) / (nLineCount * 2)  * playSize.x) + origin.y + (playSize.y - playSize.x) / 2;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////


	switch (nGameNumber + 1)
	{

	case 7:
	case 11:
	case 15:
	{
		Sprite* bgRedEdge = (Sprite*)pLayer->getChildByTag(BG_RED_EDGE);
		if (bgRedEdge == NULL)
		{
			bgRedEdge = Sprite::createWithSpriteFrameName("bgPlayRed.png");
			bgRedEdge->setPosition(playSize.x * 0.5f, playSize.y * 0.5f + origin.y);
			bgRedEdge->setTag(BG_RED_EDGE);
			pLayer->addChild(bgRedEdge, 100);
		}

		auto blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("blue_c.png");
			blue->setTag(PLAYER_BLUE);
			pLayer->addChild(blue, 2);
		}
		blue->setVisible(true);
		blue->setPosition(playSize.x * 0.5, origin.y + playSize.y * 0.5);


		if (nGameNumber + 1 == 7)
		{
			nSpeedY = 0;
			nSpeedX = 0;
		}
		else if (nGameNumber + 1 == 11)
		{
			bMagnetS = false;
			prevTouchPoint = Vec2(blue->getPosition());

			Sprite* magnetS = (Sprite*)pLayer->getChildByTag(ETC1 + 0);
			if (magnetS == NULL)
			{
				magnetS = Sprite::createWithSpriteFrameName("magnetS.png");
				magnetS->setTag(ETC1 + 0);
				magnetS->setScale(PLAYER_BLUE_BASE_SCALE * 3);
				pLayer->addChild(magnetS, 1);
			}
			Sprite* magnetN = (Sprite*)pLayer->getChildByTag(ETC1 + 1);
			if (magnetN == NULL)
			{
				magnetN = Sprite::createWithSpriteFrameName("magnetN.png");
				magnetN->setTag(ETC1 + 1);
				magnetN->setScale(PLAYER_BLUE_BASE_SCALE * 3);
				pLayer->addChild(magnetN, 1);
			}
			magnetS->setPosition(prevTouchPoint);
			magnetN->setPosition(prevTouchPoint);

			auto dnLine = (DrawNode*)pLayer->getChildByTag(ETC2);
			if (dnLine == NULL)
			{
				dnLine = DrawNode::create();
				dnLine->setTag(ETC2);
				dnLine->setLineWidth(300);
				pLayer->addChild(dnLine, 0);
			}
			dnLine->setVisible(true);

			if (bMagnetS == true)
			{
				magnetS->setVisible(true);
				magnetN->setVisible(false);
			}
			else
			{
				magnetS->setVisible(false);
				magnetN->setVisible(true);
			}


		}
		else if (nGameNumber + 1 == 15)
		{
			nSpeed = 0;
			bTouch = false;
			bRight = false;
			nDegree = 0;
		}

	}
	break;


	case 3:
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("blue_c.png");
			blue->setTag(PLAYER_BLUE);
			pLayer->addChild(blue, 2);
		}
		blue->setVisible(true);
		blue->setPosition(playSize.x * 0.15f, playSize.y * 0.5f + origin.y);

		nSpeedY = 0;
	}
	break;

	case 12:
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("blue_c.png");
			blue->setTag(PLAYER_BLUE);
			pLayer->addChild(blue);
		}
		blue->setVisible(true);
		blue->setPosition(playSize.x * 0.5f, playSize.y * 0.5f + origin.y);

		nMaxArise = 10 + (nStageLevel * 5);
	}
	break;


	case 16:
	{
		nDegree = 0;

		auto blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("blue_c.png");
			blue->setTag(PLAYER_BLUE);
			pLayer->addChild(blue, 0);
		}
		blue->setVisible(true);
		blue->setPosition(playSize.x * 0.5, origin.y + playSize.y * 0.5);

		auto white = (Sprite*)pLayer->getChildByTag(ETC1);
		if (white == NULL)
		{
			white = Sprite::createWithSpriteFrameName("white_c.png");
			white->setTag(ETC1);
			pLayer->addChild(white, 1);
		}
		white->setVisible(true);
		/*
		auto dnLine = (DrawNode*)pLayer->getChildByTag(ETC1);
		if (dnLine == NULL)
		{
			dnLine = DrawNode::create();
			dnLine->setTag(ETC1);
			dnLine->setLineWidth(300);
			pLayer->addChild(dnLine);
		}
		*/
	}
	break;

	case 8:
	{
		Sprite* bgRedEdge = (Sprite*)pLayer->getChildByTag(BG_RED_EDGE);
		if (bgRedEdge == NULL)
		{
			bgRedEdge = Sprite::createWithSpriteFrameName("bgPlayRed.png");
			bgRedEdge->setPosition(playSize.x * 0.5f, origin.y - playSize.y * 0.48f);
			bgRedEdge->setTag(BG_RED_EDGE);
			pLayer->addChild(bgRedEdge, 100);
		}

		auto blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("blue_c.png");
			blue->setTag(PLAYER_BLUE);
			pLayer->addChild(blue, 0);
		}
		blue->setVisible(true);
		blue->setPosition(playSize.x * 0.5, origin.y + 25);
	}
	break;


	case 9:
	{
		auto blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("blue_s.png");
			blue->setTag(PLAYER_BLUE);
			pLayer->addChild(blue, 100);
		}
		blue->setVisible(true);
		blue->setPosition(playSize.x * 0.5f, origin.y + playSize.y * 0.5f);
	}
	break;


	case 13:
	{
		bShot = false;
		bFirstFire = true;
		auto aim = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (aim == NULL)
		{
			aim = Sprite::createWithSpriteFrameName("g1.png");
			aim->setTag(PLAYER_BLUE);
			pLayer->addChild(aim, 100);
		}
		aim->setVisible(true);
		aim->setPosition(playSize.x * 0.5f, origin.y + playSize.y * 0.5f);

	}
	break;

	case 17:
	{
		bShot = false;
		bFirstFire = true;
		nAiming = 0;
		nFireTick = 0;
		auto aim = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (aim == NULL)
		{
			aim = Sprite::createWithSpriteFrameName("g1.png");
			aim->setTag(PLAYER_BLUE);
			pLayer->addChild(aim, 100);
		}
		aim->setVisible(true);
		aim->setPosition(playSize.x * 0.5f, origin.y + playSize.y * 0.5f);
	}
	break;


	case 96:
	{
		bShot = false;
	}
	break;

	case 2:
	{
		Set_UnifyType();
	}
	break;

	case 4:
	{
		bCircle = true;
		nMaxClickCount = 120;
		nCurrentLimitCount = nMaxClickCount * 0.2f + nMaxClickCount * CCRANDOM_0_1();
		nCurrentClickCount = 0;
	}
	break;

	case 6:
	{
		Set_MemorizeColors();
	}
	break;

	case 10:
	{
		for (int i = 0; i < nLineCount; i++)
			for (int j = 0; j < nLineCount; j++)
				arrType[i][j] = -1;

		Set_HighCount();
	}
	break;


	case 910:
	{
		///////////////green bg
		auto green1 = (Sprite*)pLayer->getChildByTag(ETC1);
		if (green1 == NULL)
		{
			green1 = Sprite::createWithSpriteFrameName("line1g.png");
			green1->setTag(ETC1);
			pLayer->addChild(green1);
		}

		auto blue2 = (Sprite*)pLayer->getChildByTag(ETC2);
		if (blue2 == NULL)
		{
			blue2 = Sprite::createWithSpriteFrameName("line1b.png");
			blue2->setTag(ETC2);
			pLayer->addChild(blue2);
		}

		auto blue3 = (Sprite*)pLayer->getChildByTag(ETC3);
		if (blue3 == NULL)
		{
			blue3 = Sprite::createWithSpriteFrameName("line1b.png");
			blue3->setTag(ETC3);
			pLayer->addChild(blue3);
		}

		float nDistance = arrPos[0][1].y - arrPos[0][0].y;
		green1->setPosition(arrPos[0][0] - Vec2(0, nDistance));
		green1->setScale(nLineBlockScale);
		blue2->setPosition(arrPos[nLineCount - 1][nLineCount - 1] + Vec2(0, nDistance));
		blue2->setScale(nLineBlockScale);
		blue3->setPosition(arrPos[0][0] - Vec2(0, nDistance));
		blue3->setScale(nLineBlockScale);
		////////////////////

		Set_MakeWay();
	}
	break;

	case 914:
	{
		//이미지 생성 및 지우기
		Sprite* arrSprite[MAX_LINE_COUNT][MAX_LINE_COUNT] = { NULL };
		for (int i = 0; i < MAX_LINE_COUNT; i++)
		{
			for (int j = 0; j < MAX_LINE_COUNT; j++)
			{
				arrSprite[i][j] = (Sprite*)pLayer->getChildByTag(ETC1 + (i * nLineCount + j));
				if (arrSprite[i][j] == NULL)
				{
					arrSprite[i][j] = Sprite::create(_image_RP1);
					arrSprite[i][j]->setTag(ETC1 + (i * nLineCount + j));
					arrSprite[i][j]->setScale(0.9f);
					pLayer->addChild(arrSprite[i][j], 0);
				}
				arrSprite[i][j]->setVisible(false);
			}
		}

		Set_RotateSlide();
	}
	break;

	case 98:
	{
		nStartNumber = -5 - (nStageLevel * 2);
		nCurrentNumber = nStartNumber;
	}
	break;

	case 14:
	{
		nMentalCalc_PrevA = 0;
		Set_MentalCalc();
	}
	break;
	}

	bNeedSetting = false;
}

// 이미지 가져오기
Sprite* PlayFunc::getRSNumber(int nNumber)
{
	float nSize = 600 / nLineCount;

	Sprite* pSpr = (Sprite*)pLayer->getChildByTag(ETC1 + nNumber);
	if (pSpr == NULL)
	{
		return NULL;
	}
	int i = nNumber / nLineCount;
	int j = nNumber % nLineCount;
	pSpr->setTextureRect(Rect(i * nSize, (nLineCount - 1 - j) * nSize, nSize, nSize));
	pSpr->setVisible(true);


	return pSpr;
}


void PlayFunc::Set_RotateSlide()
{
	//초기화
	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			arrType[i][j] = i * nLineCount + j;
		}
	}

	//그리기
	Draw_RotateSlide(false);

	bCallBackDelay = true;
	auto action = Sequence::create(
		DelayTime::create(0.2f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 5)),
		NULL);
	pLayer->runAction(action);

}


void PlayFunc::Draw_RotateSlide(bool bCheck)
{

	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			int nType = arrType[i][j];
			if (nType == 0) // 0 안그림.
				continue;

			int nRotate = nType / 100;
			int nPos = nType % 100;

			Sprite* pBlock = getRSNumber(nPos);
			pBlock->setPosition(arrPos[i][j]);
			pBlock->setRotation(nRotate * 90);
		}
	}

	if (bCheck == true && Check_RotateSlide() == true)
	{
		ChangeStagePoint(POINT_TYPE7);
		Particle_Heal(Vec2(gameSize.width *0.5f, gameSize.height*0.5f), POINT_TYPE7);
		ChangeLife(50, true);


		bCallBackDelay = true;
		auto action = Sequence::create(
			DelayTime::create(0.5f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 1)),
			NULL);
		pLayer->runAction(action);
	}

}

void PlayFunc::Shuffle_RotateSlide()
{
	for (int k = 0; k < 150; k++)
	{
		//셔플 세팅
		for (int i = 0; i < nLineCount; i++)
		{
			for (int j = 0; j < nLineCount; j++)
			{
				if (arrType[i][j] != 0)
					continue;

				bool bTop, bBottom, bLeft, bRight;
				bTop = bBottom = bLeft = bRight = false;
				float nRand = CCRANDOM_0_1();
				if (nRand > 0.75f)
					bTop = true;
				else if (nRand > 0.5f)
					bBottom = true;
				else if (nRand > 0.25f)
					bLeft = true;
				else
					bRight = true;

				if (bBottom && j == nLineCount - 1)
					bBottom = false;
				if (bTop && j == 0)
					bTop = false;
				if (bLeft && i == nLineCount - 1)
					bLeft = false;
				if (bRight && i == 0)
					bRight = false;

				if (bTop)
				{
					arrType[i][j] = arrType[i][j - 1];
					arrType[i][j - 1] = 0;
				}
				if (bBottom)
				{
					arrType[i][j] = arrType[i][j + 1];
					arrType[i][j + 1] = 0;
				}
				if (bRight)
				{
					arrType[i][j] = arrType[i - 1][j];
					arrType[i - 1][j] = 0;
				}
				if (bLeft)
				{
					arrType[i][j] = arrType[i + 1][j];
					arrType[i + 1][j] = 0;
				}
			}
		}
	}

	//회전 세팅
	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			if (arrType[i][j] == 0)
				continue;

			int nRand = CCRANDOM_0_1() * 4;
			arrType[i][j] += nRand * 100; // 000 ~ 300 // 회전 값 주기
		}
	}

	bCallBackDelay = true;
	auto action = Sequence::create(
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 0)),
		NULL);
	pLayer->runAction(action);
}


bool PlayFunc::Check_RotateSlide()
{

	bool bAnswer = true;

	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			bool bCorrect = true;
			int nType = arrType[i][j];
			if (nType == 0)
				continue;

			int nRotate = nType / 100;
			if (nRotate != 0)
			{
				bCorrect = false;
			}
			int nPos = nType % 100;
			if (i* nLineCount + j != nPos)
			{
				bCorrect = false;
			}
			Sprite* pBlock = getRSNumber(nPos);
			if (bCorrect == true)
			{
				pBlock->setScale(1.0f);
			}
			else
			{
				pBlock->setScale(0.9f);
				bAnswer = false;
			}

		}
	}


	Sprite* pBlock = getRSNumber(0);
	if (bAnswer == true)
	{
		pBlock->setScale(1.0f);
		pBlock->setPosition(arrPos[0][0]);
		pBlock->setVisible(true);
	}
	else
	{
		pBlock->setScale(0.9f);
		pBlock->setPosition(arrPos[0][0].x, arrPos[0][0].y - (arrPos[0][1].y - arrPos[0][0].y));
		pBlock->setVisible(true);
	}

	return bAnswer;

}


void PlayFunc::UpdateStage()
{

	switch (nGameNumber + 1)
	{

	case 16:
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			return;
		}
		float nBlueX = blue->getPositionX();
		float nBlueY = blue->getPositionY();

		Sprite* white = (Sprite*)pLayer->getChildByTag(ETC1);
		if (white == NULL)
		{
			return;
		}

		float nDegreeSpeed = 0.05f;
		if (bExtreme)
			nDegreeSpeed = nDegreeSpeed * pow(0.5, nStageLevel + 1);


		nDegree += nDegreeSpeed;

		float nLineLength = 150;
		float nY = nLineLength * cos(nDegree);
		float nX = nLineLength * sin(nDegree);
		white->setPosition(nBlueX + nX, nBlueY + nY);

		/*
		float nLineLength = 250;
		float nPointY2 = nLineLength * cos(nDegree);
		float nPointX2 = nLineLength * sin(nDegree);
		float nPointY1 = nPointY2 * 0.3f;
		float nPointX1 = nPointX2 * 0.3f;
		nPointY2 = nBlueY + nPointY2;
		nPointX2 = nBlueX + nPointX2;
		nPointY1 = nBlueY + nPointY1;
		nPointX1 = nBlueX + nPointX1;

		auto dnLine = (DrawNode*)pLayer->getChildByTag(ETC1);
		if (dnLine == NULL)
		{
			return;
		}
		dnLine->clear();
		dnLine->drawSegment(Vec2(nPointX1, nPointY1), Vec2(nPointX2, nPointY2), 4, Color4F::BLACK);
		*/


		//미사일 생성 랜덤하게 위 아래에서만.
		if (nMainTick % 12 == 0)
		{
			/////////////////////////////////Position
			float nRand = CCRANDOM_0_1();
			bool bTop, bBottom;
			bTop = bBottom = false;

			if (nRand > 0.5)
				bTop = true;
			else
				bBottom = true;

			float nCreateX, nCreateY;
			if (bTop)
			{
				nCreateY = (origin.y + playSize.y) + 50;
				nCreateX = gameSize.width * CCRANDOM_0_1();
			}
			else if (bBottom)
			{
				nCreateY = origin.y - 50;
				nCreateX = gameSize.width * CCRANDOM_0_1();
			}
			/////////////////////////////////////////////////////////

			int nType = BLUE_SQUARE;
			if (CCRANDOM_0_1() < 0.5f)
				nType = RED_SQUARE;

			BlockSprite* ball = getInvisibleBlock(nType);
			ball->setPosition(nCreateX, nCreateY);
			ball->setScale(MISSILE_SCALE);
		}


		//미사일 이동
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;

			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite* block = vBlock[nType].at(i);
				if (block->isVisible() == false)
					continue;
				float nDisX = nBlueX - block->getPositionX();
				float nDisY = nBlueY - block->getPositionY();
				bool bMinusX = false;
				bool bMinusY = false;
				if (nDisX < 0)
				{
					bMinusX = true;
					nDisX *= -1;
				}
				if (nDisY < 0)
				{
					bMinusY = true;
					nDisY *= -1;
				}
				float nSpeed = 5 + nTimeDifficulty * 0.05f;
				float nSpeedX = nSpeed * (nDisX / (nDisX + nDisY));
				if (bMinusX)
					nSpeedX *= -1;
				float nSpeedY = nSpeed * (nDisY / (nDisX + nDisY));
				if (bMinusY)
					nSpeedY *= -1;

				block->setPositionX(block->getPositionX() + nSpeedX);
				block->setPositionY(block->getPositionY() + nSpeedY);
			}
		}

		//미사일 - 선 충돌
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;

			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite* block = vBlock[nType].at(i);
				if (block->isVisible() == false)
					continue;
				Rect rectBlock = block->getBoundingBox();

				Rect rectWhite = white->getBoundingBox();
				float nRadius = rectWhite.getMidX() - rectWhite.getMinX();
				if (rectBlock.intersectsCircle(white->getPosition(), nRadius))
				{
					block->stopAllActions();
					block->setVisible(false);
					Particle_Bang(block->getPosition(), 0.5f, true, true);
					break;
				}
			}
		}

		//미사일 - 플레이어 충돌
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;

			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite* block = vBlock[nType].at(i);
				if (block->isVisible() == false)
					continue;
				Rect rectBlock = block->getBoundingBox();

				Rect rectBlue = blue->getBoundingBox();
				float nRadius = rectBlue.getMidX() - rectBlue.getMinX();
				if (rectBlock.intersectsCircle(blue->getPosition(), nRadius))
				{
					block->stopAllActions();
					block->setVisible(false);

					if (nType == RED_SQUARE)
					{
						ChangeLife(-20);
						UpdateCombo(-1, -1, 0);
						Particle_Steam(blue->getPosition());
					}
					else if (nType == BLUE_SQUARE)
					{
						UpdateCombo(-1, -1, 1);
						ChangeLife(3, true);
						ChangeStagePoint(POINT_TYPE2);
						Particle_Heal(blue->getPosition(), POINT_TYPE2);
					}
				}
			}
		}


		if (p1.GetLife() == 0)
		{
			blue->setVisible(false);
		}
		else
		{
			blue->setVisible(true);
			//Blue 사이즈 조절
			blue->setScale(PLAYER_BLUE_BASE_SCALE + p1.GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 2);
		}

		if (p1.GetLife() == 0)
		{
			white->setVisible(false);
		}
		else
		{
			white->setVisible(true);
			//Blue 사이즈 조절
			white->setScale(PLAYER_BLUE_BASE_SCALE * 6 - p1.GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 3);
		}
	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 9:
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			return;
		}
		//blue 이동
		if (bTouch == true)
		{
			float nDistanceX = prevTouchPoint.x - blue->getPositionX();
			blue->setPositionX(blue->getPositionX() + nDistanceX / 3);

			float nDistanceY = prevTouchPoint.y - blue->getPositionY();
			blue->setPositionY(blue->getPositionY() + nDistanceY / 3);
		}

		//블록 생성 코드 위에서 아래로.
		int nTick = 6;
		if (nMainTick % nTick == 0)
		{
			float nCreateY = (origin.y + playSize.y) + 250;
			float nCreateX = 50 + (gameSize.width - 100) * CCRANDOM_0_1();
			float nMoveX = 50 + (gameSize.width - 100) * CCRANDOM_0_1();
			float nSpeed = 3.0f;

			float nReduce = 0.5f + nTimeDifficulty * 0.005f;
			if (bExtreme == true)
			{
				nReduce = nReduce * pow(1.3f, nStageLevel + 1);
			}

			nSpeed = nSpeed - nReduce;


			int nType = RED_SQUARE;

			BlockSprite* ball = getInvisibleBlock(nType);
			ball->setPosition(nCreateX, nCreateY);
			ball->setScale(MISSILE_SCALE * 1.5f);

			auto action = Sequence::create(
				MoveTo::create(nSpeed, Vec2(nMoveX, 0)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
				NULL);

			ball->runAction(action);
		}

		//블록 - 플레이어 충돌코드
		Rect rectBlue = blue->getBoundingBox();
		for (int i = 0; i < vBlock[RED_SQUARE].size(); i++)
		{
			BlockSprite* pBlock = vBlock[RED_SQUARE].at(i);
			if (pBlock->isVisible() == false)
				continue;

			if (pBlock->getBoundingBox().intersectsRect(rectBlue) == true)
			{
				pBlock->stopAllActions();
				pBlock->setVisible(false);

				UpdateCombo(-1, -1, 1);
				ChangeStagePoint(POINT_TYPE0);
				ChangeLife(1, true);
				Particle_Heal(pBlock->getPosition(), POINT_TYPE0);
			}
		}


		//블록 플레이어 베이스 코드 //ETC2
		auto baseCamp = (Sprite*)pLayer->getChildByTag(ETC2);
		if (baseCamp == NULL)
		{
			baseCamp = Sprite::createWithSpriteFrameName("blue_s.png");
			baseCamp->setTag(ETC2);
			pLayer->addChild(baseCamp, 0);
			baseCamp->setScale(2.0f);
			baseCamp->setPosition(playSize.x / 2, origin.y);
		}

		float nBaseY = origin.y - 300; //피가 0일 때.
		baseCamp->setPositionY(nBaseY + 300 * p1.GetLifePercent() / 100);

		//블록- 베이스 충돌 코드
		for (int i = 0; i < vBlock[RED_SQUARE].size(); i++)
		{
			BlockSprite* block = vBlock[RED_SQUARE].at(i);
			if (block->isVisible() == false)
				continue;
			Rect rectBlock = block->getBoundingBox();
			Rect rectBase = baseCamp->getBoundingBox();
			if (rectBlock.intersectsRect(rectBase))
			{
				block->stopAllActions();
				block->setVisible(false);

				ChangeLife(-20);
				UpdateCombo(-1, -1, 0);
				Particle_Steam(block->getPosition());
			}
		}

		if (p1.GetLife() == 0)
		{
			blue->setVisible(false);
		}
		else
		{
			blue->setVisible(true);
			//Blue 사이즈 조절
			blue->setScale((PLAYER_BLUE_BASE_SCALE * 3 - p1.GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 1));
		}
	}
	break;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 17:
	{
		auto aim = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (aim == NULL)
			return;
		float nAimX = aim->getPositionX();
		float nAimY = aim->getPositionY();
		aim->setScale(PLAYER_BLUE_BASE_SCALE + nAiming * 0.006f);

		if (bTouch == true)
		{
			nAiming -= 10;
			if (nAiming < 0)
				nAiming = 0;

			if (bFirstFire == true)
			{
				Sound(_sounds_R1);
				bFirstFire = false;
			}
		}
		else if (bShot == true)
		{
			nAiming += 3;
			if (nAiming > 300)
				nAiming = 300;

			if (bFirstFire == false)
			{
				bFirstFire = true;
				nFireTick = 0;
			}

			if (nFireTick % 15 == 0)
			{
				float nShotX, nShotY;
				nShotX = nAimX + ((nAiming*0.8 / 2) - (CCRANDOM_0_1() * nAiming*0.8));
				nShotY = nAimY + ((nAiming*0.8 / 2) - (CCRANDOM_0_1() * nAiming*0.8));
				Sound(_sounds_G1);

				auto g2 = getInvisibleBlock(G2, 0);
				g2->setPosition(nShotX, nShotY);
				g2->setScale(PLAYER_BLUE_BASE_SCALE * 2);
				auto action = Sequence::create(ScaleTo::create(1.0f, 0),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
					NULL);
				g2->runAction(action);

				// 총알 - 블록 크래시 코드
				for (int k = 0; k < 2; k++)
				{
					int nType = 0;
					if (k == 0)
						nType = RED_SQUARE;
					else
						nType = BLUE_SQUARE;

					for (int i = 0; i < vBlock[nType].size(); i++)
					{
						BlockSprite* block = vBlock[nType].at(i);
						if (block->isVisible() == false)
							continue;

						Rect rectBlock = block->getBoundingBox();
						if (rectBlock.containsPoint(Vec2(nShotX, nShotY)))
						{
							block->stopAllActions();
							block->setVisible(false);
							Particle_Bang(Vec2(nShotX, nShotY));
						}
					}

				}

			}

		}

	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	case 13:
	{

		auto aim = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (aim == NULL)
			return;
		float nAimX = aim->getPositionX();
		float nAimY = aim->getPositionY();
		aim->setScale(PLAYER_BLUE_BASE_SCALE);

		if (bTouch == true)
		{
			if (bFirstFire == true)
			{
				Sound(_sounds_R1);
				bFirstFire = false;
			}
		}
		else if (bShot == true)
		{
			if (bFirstFire == false)
			{
				bFirstFire = true;
				nFireTick = 0;
				float nShotX, nShotY;
				nShotX = nAimX;
				nShotY = nAimY;
				Sound(_sounds_G1);

				auto g2 = getInvisibleBlock(G2, 0);
				g2->setPosition(nShotX, nShotY);
				g2->setScale(PLAYER_BLUE_BASE_SCALE * 2);
				auto action = Sequence::create(ScaleTo::create(1.0f, 0),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
					NULL);
				g2->runAction(action);

				// 총알 - 블록 크래시 코드
				for (int k = 0; k < 2; k++)
				{
					int nType = 0;
					if (k == 0)
						nType = RED_SQUARE;
					else
						nType = BLUE_SQUARE;

					for (int i = 0; i < vBlock[nType].size(); i++)
					{
						BlockSprite* block = vBlock[nType].at(i);
						if (block->isVisible() == false)
							continue;

						Rect rectBlock = block->getBoundingBox();
						if (rectBlock.containsPoint(Vec2(nShotX, nShotY)))
						{
							block->stopAllActions();
							block->setVisible(false);
							if (nType == RED_SQUARE)
							{
								UpdateCombo(-1, -1, 1);
								ChangeLife(2, true);
								ChangeStagePoint(POINT_TYPE2);
								Particle_Heal(Vec2(nShotX, nShotY), POINT_TYPE2);
							}
							else
							{
								ChangeLife(-20);
								UpdateCombo(-1, -1, 0);
								Particle_Bang(Vec2(nShotX, nShotY));
							}
						}
					}

				}

			}

		}


		//블록 생성 코드 위에서 아래로.
		if (nMainTick % 45 == 0)
		{
			float nCreateY = (origin.y + playSize.y) + 250;
			float nCreateX = 50 + (gameSize.width - 100) * CCRANDOM_0_1();
			float nMoveX = 50 + (gameSize.width - 100) * CCRANDOM_0_1();
			float nSpeed = 2.5f + nTimeDifficulty * 0.025f;

			int nType = BLUE_SQUARE;
			if (CCRANDOM_0_1() < 0.7f)
				nType = RED_SQUARE;

			BlockSprite* ball = getInvisibleBlock(nType);
			ball->setPosition(nCreateX, nCreateY);
			ball->setScale(MISSILE_SCALE * 2);
			if (bExtreme)
			{
				ball->setScale(ball->getScale() * pow(0.7, nStageLevel + 1));
			}

			auto action = Sequence::create(
				MoveTo::create(nSpeed, Vec2(nMoveX, 0)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
				NULL);

			ball->runAction(action);
		}


		//블록 플레이어 베이스 코드 //ETC2
		auto baseCamp = (Sprite*)pLayer->getChildByTag(ETC2);
		if (baseCamp == NULL)
		{
			baseCamp = Sprite::createWithSpriteFrameName("blue_s.png");
			baseCamp->setTag(ETC2);
			pLayer->addChild(baseCamp, 0);
			baseCamp->setScale(2.0f);
			baseCamp->setPosition(playSize.x / 2, origin.y);
		}

		float nBaseY = origin.y - 300; //피가 0일 때.
		baseCamp->setPositionY(nBaseY + 300 * p1.GetLifePercent() / 100);



		//블록- 베이스 충돌 코드
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;

			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite* block = vBlock[nType].at(i);
				if (block->isVisible() == false)
					continue;
				Rect rectBlock = block->getBoundingBox();
				Rect rectBase = baseCamp->getBoundingBox();
				if (rectBlock.intersectsRect(rectBase))
				{
					block->stopAllActions();
					block->setVisible(false);
					if (nType == RED_SQUARE)
					{
						ChangeLife(-20);
						UpdateCombo(-1, -1, 0);
						Particle_Steam(block->getPosition());
					}
					else if (nType == BLUE_SQUARE)
					{
						UpdateCombo(-1, -1, 1);
						ChangeLife(1, true);
						ChangeStagePoint(POINT_TYPE2);
						Particle_Heal(block->getPosition(), POINT_TYPE2);
					}
				}
			}
		}


	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 12:
	{
		// Blue 생성
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			return;
		}


		// Red 생성
		int nVisibleCount = 0;
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;

			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				if (vBlock[nType].at(i)->isVisible() == true)
					nVisibleCount++;
			}
		}

		int nCreateCount = nMaxArise - nVisibleCount;
		for (int i = 0; i < nCreateCount; i++)
		{
			/////////////////////////////////Position
			float nRand = CCRANDOM_0_1();
			bool bTop, bBottom, bLeft, bRight;
			bTop = bBottom = bLeft = bRight = false;
			if (nRand > 0.5)
			{
				if (nRand > 0.75)
					bTop = true;
				else
					bBottom = true;
			}
			else
			{
				if (nRand > 0.25)
					bRight = true;
				else
					bLeft = true;
			}

			float nCreateX, nCreateY, nMoveX, nMoveY;
			if (bTop)
			{
				nCreateY = (origin.y + playSize.y) + 100;
				nMoveY = origin.y - 100;
				nCreateX = gameSize.width * CCRANDOM_0_1();
				nMoveX = gameSize.width * CCRANDOM_0_1();
			}
			else if (bBottom)
			{
				nCreateY = origin.y - 100;
				nMoveY = (origin.y + playSize.y) + 100;
				nCreateX = gameSize.width * CCRANDOM_0_1();
				nMoveX = gameSize.width * CCRANDOM_0_1();
			}
			else if (bRight)
			{
				nCreateX = gameSize.width + 100;
				nMoveX = -100;
				nCreateY = playSize.y * CCRANDOM_0_1() + origin.y;
				nMoveY = playSize.y * CCRANDOM_0_1() + origin.y;
			}
			else if (bLeft)
			{
				nCreateX = -100;
				nMoveX = gameSize.width + 100;
				nCreateY = playSize.y * CCRANDOM_0_1() + origin.y;
				nMoveY = playSize.y * CCRANDOM_0_1() + origin.y;
			}
			/////////////////////////////////////////////////////////

			int nType = BLUE_SQUARE;
			if (CCRANDOM_0_1() < 0.5f)
				nType = RED_SQUARE;

			BlockSprite* ball = getInvisibleBlock(nType);
			ball->setPosition(nCreateX, nCreateY);
			ball->setScale(MISSILE_SCALE);
			if (bExtreme)
				ball->setScale(ball->getScale() * pow(1.3, nStageLevel + 1));

			float nSpeedTime = 8 - nTimeDifficulty * 0.08f;

			Sequence* patternAction = NULL;
			{
				float nLeftRight = playSize.x;
				float nUpDown = 0;
				if (bLeft || bRight)
				{
					nLeftRight = 0;
					nUpDown = playSize.y;
				}
				ccBezierConfig bezierConfig1;
				bezierConfig1.controlPoint_1 = Point((nCreateX + nMoveX) *0.85 - nLeftRight, (nCreateY + nMoveY) * 0.85 - nUpDown);
				bezierConfig1.controlPoint_2 = Point((nCreateX + nMoveX) *0.7 + nLeftRight, (nCreateY + nMoveY) * 0.7 + nUpDown);
				bezierConfig1.endPosition = Point(nMoveX*0.5, nMoveY*0.5);
				auto Bezier1 = BezierTo::create(nSpeedTime / 2, bezierConfig1);
				ccBezierConfig bezierConfig2;
				bezierConfig2.controlPoint_1 = Point((nCreateX + nMoveX) *0.3 - nLeftRight, (nCreateY + nMoveY) * 0.3 - nUpDown);
				bezierConfig2.controlPoint_2 = Point((nCreateX + nMoveX) *0.15 + nLeftRight, (nCreateY + nMoveY) * 0.15 + nUpDown);
				bezierConfig2.endPosition = Point(nMoveX, nMoveY);
				auto Bezier2 = BezierTo::create(nSpeedTime / 2, bezierConfig2);
				patternAction = Sequence::create(
					Bezier1, Bezier2,
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
					NULL);
			}
			auto action = Sequence::create(
				DelayTime::create(0.25f * i),
				patternAction,
				NULL);

			ball->runAction(action);
		}

		//충돌 확인.
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;

			Rect rectBlue = blue->getBoundingBox();
			for (BlockSprite* pBlock : vBlock[nType])
			{
				if (pBlock == NULL)
					continue;

				if (pBlock->isVisible() == false)
					continue;

				Rect rectBlock = pBlock->getBoundingBox();
				float nRadius = rectBlue.getMidX() - rectBlue.getMinX();
				if (rectBlock.intersectsCircle(blue->getPosition(), nRadius * 0.9f))
				{
					pBlock->stopAllActions();
					pBlock->setVisible(false);

					if (nType == RED_SQUARE)
					{
						UpdateCombo(-1, -1, 0);
						ChangeLife(-20, false);

						Particle_Steam(pBlock->getPosition(), pBlock->getScale() * 4);

					}
					else if (nType == BLUE_SQUARE)
					{
						UpdateCombo(-1, -1, 1);
						ChangeLife(3, true);
						ChangeStagePoint(POINT_TYPE2);

						Particle_Heal(pBlock->getPosition(), POINT_TYPE2);

					}

				}
			}
		}

		//Blue 사이즈 조절
		if (p1.GetLife() == 0)
		{
			blue->setVisible(false);
		}
		else
		{
			blue->setVisible(true);
			//Blue 사이즈 조절
			blue->setScale(PLAYER_BLUE_BASE_SCALE + p1.GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 2);
		}
	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 3:
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			return;
		}
		float nLimitUp = playSize.y + origin.y;
		float nLimitDown = origin.y;

		// Blue 이동
		float toY = blue->getPositionY() + nSpeedY;
		blue->setPositionY(toY);

		nSpeedY -= G_ACCEL;

		Rect rectBlue = blue->getBoundingBox();
		if (rectBlue.getMaxY() > nLimitUp)
		{
			if (nSpeedY > 0)
			{
				if (nSpeedY > 5)
					Sound(_sounds_Tong, true);

				nSpeedY = nSpeedY * -EDGE_ELASTIC;
			}

		}
		else if (rectBlue.getMinY() < nLimitDown)
		{
			if (nSpeedY < 0)
			{
				if (nSpeedY < -5)
					Sound(_sounds_Tong, true);

				nSpeedY = nSpeedY * -EDGE_ELASTIC;
			}
		}

		// 장애물 생성
		int nTick = 100;
		int nReduce = 10;
		if (bExtreme)
		{
			nReduce = nReduce * pow(1.3f, nStageLevel + 1);
		}

		nTick = nTick - nReduce;
		if (nMainTick % nTick == 0)
		{

			if (nMainTick % (nTick * 10) == 0)
			{
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						BlockSprite* blue = getInvisibleBlock(BLUE_SQUARE, 0);
						blue->setScale(PLAYER_BLUE_BASE_SCALE * 2);
						blue->setPosition(playSize.x + 50 + (65 * i), origin.y + 300 + (65 * j));

					}
				}
			}
			else
			{
				float nScaleX = 0.6f;
				BlockSprite* blue = getInvisibleBlock(BLUE_SQUARE, 0);
				blue->setScale(PLAYER_BLUE_BASE_SCALE * 2);

				BlockSprite* red1 = getInvisibleBlock(RED_SQUARE, 1);
				red1->setScale(nScaleX);

				BlockSprite* red2 = getInvisibleBlock(RED_SQUARE, 1);
				red2->setScale(nScaleX);


				int nRand = CCRANDOM_0_1() * 10;
				if (nRand == 10)
					nRand = 9;

				if (nRand == 0)
					nRand = 1;

				float nRate = 1.9f;
				float nPos = nRate * 300 / 10;

				red1->setScaleY(nRate - (nRand * 0.19f));
				red1->setPosition(playSize.x + 100, origin.y + playSize.y - (nPos / 2 * (10 - nRand)));

				red2->setScaleY(nRand * 0.19f);
				red2->setPosition(playSize.x + 100, origin.y + (nPos / 2 * nRand));

				blue->setPosition(playSize.x + 100, origin.y + nPos * nRand);
			}
		}

		//장애물 이동
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;

			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite *red_s = vBlock[nType].at(i);
				if (red_s->isVisible() == true)
				{
					float nSpeed = -5.0f - (nTimeDifficulty * 0.05f);

					float toX = red_s->getPositionX() + nSpeed;

					if (toX < -100)
						red_s->setVisible(false);
					else
						red_s->setPositionX(toX);
				}
			}
		}


		// 충돌 확인.
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;

			Rect rectBlue = blue->getBoundingBox();
			for (BlockSprite* pBlock : vBlock[nType])
			{
				if (pBlock == NULL)
					continue;

				if (pBlock->isVisible() == false)
					continue;

				Rect rectBlock = pBlock->getBoundingBox();
				float nRadius = rectBlue.getMidX() - rectBlue.getMinX();
				if (rectBlock.intersectsCircle(blue->getPosition(), nRadius * 0.9f))
				{
					if (nType == BLUE_SQUARE)
					{
						pBlock->stopAllActions();
						pBlock->setVisible(false);

						UpdateCombo(-1, -1, 1);
						ChangeLife(1, true);
						ChangeStagePoint(POINT_TYPE2);

						Particle_Heal(pBlock->getPosition(), POINT_TYPE2);
					}
					else if (nType == RED_SQUARE)
					{
						UpdateCombo(-1, -1, 0);
						ChangeLife(-5, false);

						Particle_Steam(blue->getPosition(), blue->getScale() * 4, true, true);
					}
				}
			}
		}

		if (p1.GetLife() == 0)
		{
			blue->setVisible(false);
		}
		else
		{
			blue->setVisible(true);
			//Blue 사이즈 조절
			blue->setScale(PLAYER_BLUE_BASE_SCALE + p1.GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 2);
		}

	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 15:
	case 7:
	case 11:
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			return;
		}
		float nBlueX = blue->getPositionX();
		float nBlueY = blue->getPositionY();
		///////////////////////////////////blue 이동
		if (nGameNumber + 1 == 11)
		{
			Sprite* magnetS = (Sprite*)pLayer->getChildByTag(ETC1 + 0);
			Sprite* magnetN = (Sprite*)pLayer->getChildByTag(ETC1 + 1);
			if (magnetS == NULL || magnetN == NULL)
			{
				return;
			}

			//blue 이동
			if (bTouch == true)
			{
				float nDistanceX = prevTouchPoint.x - magnetS->getPositionX();
				magnetS->setPositionX(magnetS->getPositionX() + nDistanceX / 5);

				float nDistanceY = prevTouchPoint.y - magnetS->getPositionY();
				magnetS->setPositionY(magnetS->getPositionY() + nDistanceY / 5);

				magnetN->setPosition(magnetS->getPosition());
			}

			if (nMainTick % (60 * 6) == 0) //6초마다 변경
			{
				Sound(_sounds_Bbi);
				bMagnetS = !bMagnetS;
			}

			if (bMagnetS == true)
			{
				magnetS->setVisible(true);
				magnetN->setVisible(false);
			}
			else
			{
				magnetS->setVisible(false);
				magnetN->setVisible(true);
			}

			auto dnLine = (DrawNode*)pLayer->getChildByTag(ETC2);
			if (dnLine)
			{
				dnLine->clear();
				dnLine->drawSegment(magnetN->getPosition(), blue->getPosition(), 2, bMagnetS ? Color4F::BLUE : Color4F::RED);
			}

			nSpeedY = 0;
			nSpeedX = 0;
			float nMagnetPower = 100000;
			float nMaxPower = 25;
			float nMinDis = 0.001f;
			float toX = 0;
			float toY = 0;
			if (bMagnetS == true)
			{
				//
				bool bMinusX = false;
				float nDisX = nBlueX - prevTouchPoint.x;
				if (nDisX < 0)
				{
					bMinusX = true;
					nDisX *= -1;
				}

				if (nDisX < nMinDis)
					nDisX = nMinDis;
				//
				bool bMinusY = false;
				float nDisY = nBlueY - prevTouchPoint.y;
				if (nDisY < 0)
				{
					bMinusY = true;
					nDisY *= -1;
				}

				if (nDisY < nMinDis)
					nDisY = nMinDis;

				///////////
				nMagnetPower = nMagnetPower / ((nDisX * nDisX) + (nDisY * nDisY));
				if (nMagnetPower > nMaxPower)
					nMagnetPower = nMaxPower;
				///////////
				nSpeedX = nMagnetPower * (nDisX / (nDisX + nDisY));
				if (bMinusX)
					nSpeedX *= -1;
				toX = nBlueX + nSpeedX;
				//
				nSpeedY = nMagnetPower * (nDisY / (nDisX + nDisY));
				if (bMinusY)
					nSpeedY *= -1;
				toY = nBlueY + nSpeedY;
			}
			else
			{
				//
				bool bMinusX = false;
				float nDisX = prevTouchPoint.x - nBlueX;
				if (nDisX < 0)
				{
					bMinusX = true;
					nDisX *= -1;
				}

				if (nDisX < nMinDis)
					nDisX = nMinDis;
				//
				bool bMinusY = false;
				float nDisY = prevTouchPoint.y - nBlueY;
				if (nDisY < 0)
				{
					bMinusY = true;
					nDisY *= -1;
				}

				if (nDisY < nMinDis)
					nDisY = nMinDis;
				/////////////////
				nMagnetPower = nMagnetPower / ((nDisX * nDisX) + (nDisY * nDisY));
				if (nMagnetPower > nMaxPower)
					nMagnetPower = nMaxPower;
				/////////////////
				nSpeedX = nMagnetPower * (nDisX / (nDisX + nDisY));
				if (bMinusX)
					nSpeedX *= -1;
				toX = nBlueX + nSpeedX;
				if (bMinusX == false && toX > prevTouchPoint.x)
					toX = nBlueX;
				else if (bMinusX == true && toX < prevTouchPoint.x)
					toX = nBlueX;
				//
				nSpeedY = nMagnetPower * (nDisY / (nDisX + nDisY));
				if (bMinusY)
					nSpeedY *= -1;
				toY = nBlueY + nSpeedY;
				if (bMinusY == false && toY > prevTouchPoint.y)
					toY = nBlueY;
				else if (bMinusY == true && toY < prevTouchPoint.y)
					toY = nBlueY;
			}

			if (toY > origin.y + 20 && toY < origin.y + playSize.y - 20)
				blue->setPositionY(toY);
			else
			{
				UpdateCombo(-1, -1, 0);
				ChangeLife(-5, false);
				Particle_Steam(blue->getPosition(), blue->getScale() * 4, true, true);
			}

			if (toX > 20 && toX < playSize.x - 20)
				blue->setPositionX(toX);
			else
			{
				UpdateCombo(-1, -1, 0);
				ChangeLife(-5, false);
				Particle_Steam(blue->getPosition(), blue->getScale() * 4, true, true);
			}

		}
		if (nGameNumber + 1 == 15)
		{

			if (bTouch == true)
			{
				if (nSpeed == 0) //최초만 속도 제로
					nSpeed = 5;

				if (prevTouchPoint.x > playSize.x * 0.5f)
					bRight = true;
				else
					bRight = false;
			}

			if (nSpeed != 0)
			{
				if (bRight == true)
				{
					if (bTouch == true)
						nDegree += 0.1f;
					else
						nDegree -= 0.1f;
				}
				else
				{
					if (bTouch == true)
						nDegree -= 0.1f;
					else
						nDegree += 0.1f;
				}


				//	if (nDegree >= 360)
				//		nDegree = 0;
				//	else if (nDegree <= 0)
				///		nDegree = 360;

			}

			// Blue 이동
			nSpeedY = nSpeed * cos(nDegree);
			nSpeedX = nSpeed * sin(nDegree);

			float toY = blue->getPositionY() + nSpeedY;
			if (toY > origin.y + 20 && toY < origin.y + playSize.y - 20)
				blue->setPositionY(toY);
			else
			{
				UpdateCombo(-1, -1, 0);
				ChangeLife(-5, false);
				Particle_Steam(blue->getPosition(), blue->getScale() * 4, true, true);
			}

			float toX = blue->getPositionX() + nSpeedX;
			if (toX > 20 && toX < playSize.x - 20)
				blue->setPositionX(toX);
			else
			{
				UpdateCombo(-1, -1, 0);
				ChangeLife(-5, false);
				Particle_Steam(blue->getPosition(), blue->getScale() * 4, true, true);
			}
		}
		else if (nGameNumber + 1 == 7)
		{
			float nLimitUp = playSize.y + origin.y;
			float nLimitDown = origin.y;

			// Blue 이동
			float toY = blue->getPositionY() + nSpeedY;
			blue->setPositionY(toY);
			float toX = blue->getPositionX() + nSpeedX;
			blue->setPositionX(toX);

			nSpeedX = nSpeedX - nSpeedX * 0.001f;
			nSpeedY -= G_ACCEL;

			Rect rectBlue = blue->getBoundingBox();
			if (rectBlue.getMaxY() > nLimitUp)
			{
				if (nSpeedY > 0)
				{
					UpdateCombo(-1, -1, 0);
					ChangeLife(-5, false);
					Particle_Steam(blue->getPosition(), blue->getScale() * 4, true, true);

					nSpeedY = nSpeedY * -EDGE_ELASTIC;
				}

			}
			else if (rectBlue.getMinY() < nLimitDown)
			{
				if (nSpeedY < 0)
				{
					UpdateCombo(-1, -1, 0);
					ChangeLife(-5, false);
					Particle_Steam(blue->getPosition(), blue->getScale() * 4, true, true);

					nSpeedY = nSpeedY * -EDGE_ELASTIC;
				}
			}

			if (rectBlue.getMaxX() > playSize.x)
			{
				if (nSpeedX > 0)
				{
					UpdateCombo(-1, -1, 0);
					ChangeLife(-5, false);
					Particle_Steam(blue->getPosition(), blue->getScale() * 4, true, true);

					nSpeedX = nSpeedX * -EDGE_ELASTIC;
				}
			}
			else if (rectBlue.getMinX() < 0)
			{
				if (nSpeedX < 0)
				{
					UpdateCombo(-1, -1, 0);
					ChangeLife(-5, false);
					Particle_Steam(blue->getPosition(), blue->getScale() * 4, true, true);

					nSpeedX = nSpeedX * -EDGE_ELASTIC;
				}
			}
		}
		/////////////////////////////////
		//충돌 체크
		Rect rectBlue = blue->getBoundingBox();
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;


			for (BlockSprite* pBlock : vBlock[nType])
			{
				if (pBlock == NULL)
					continue;

				if (pBlock->isVisible() == false)
					continue;

				Rect rectBlock = pBlock->getBoundingBox();
				float nRadius = rectBlue.getMidX() - rectBlue.getMinX();
				if (rectBlock.intersectsCircle(blue->getPosition(), nRadius * 0.9))
				{
					pBlock->stopAllActions();
					pBlock->setVisible(false);

					if (nType == RED_SQUARE)
					{
						UpdateCombo(-1, -1, 0);
						ChangeLife(-20);

						Particle_Steam(blue->getPosition(), blue->getScale() * 4);
					}
					else if (nType == BLUE_SQUARE)
					{

						UpdateCombo(-1, -1, 1);
						ChangeLife(3, true);
						ChangeStagePoint(POINT_TYPE2);

						Particle_Heal(pBlock->getPosition(), POINT_TYPE2);
					}
				}

			}
		}

		///////
		if (p1.GetLife() == 0)
		{
			blue->setVisible(false);
		}
		else
		{
			blue->setVisible(true);
			//Blue 사이즈 조절
			blue->setScale(PLAYER_BLUE_BASE_SCALE + p1.GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 2);
		}



		// 장애물 생성 공이 없는 부분에 10초간 생성 후 사라짐?
		int nVisibleCount = 0;
		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;


			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				if (vBlock[nType].at(i)->isVisible() == true)
					nVisibleCount++;
			}
		}

		if (nVisibleCount >= nMaxArise)
			return;

		float nRand = CCRANDOM_0_1();
		nRand = (nRand == 1 ? 0 : nRand);
		//pos 구하고 위치 확인. 안되면 return.
		int nPos = nRand * nLineCount * nLineCount;
		Vec2 Pos = arrPos[nPos / nLineCount][nPos % nLineCount];
		float posX = Pos.x;
		float posY = Pos.y;

		float nCreateDistance = 200;

		if (nBlueX - nCreateDistance < posX && posX < nBlueX + nCreateDistance
			&& nBlueY - nCreateDistance < posY && posY < nBlueY + nCreateDistance)
			return;

		for (int k = 0; k < 2; k++)
		{
			int nType = 0;
			if (k == 0)
				nType = RED_SQUARE;
			else
				nType = BLUE_SQUARE;

			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite* block = vBlock[nType].at(i);
				if (block->getPosition() == Pos && block->isVisible() == true)
					return;
			}
		}

		//block 세팅.
		BlockSprite* block = NULL;
		float nRemainTime = 25.0f;
		if (CCRANDOM_0_1() < 0.7f)
		{
			block = getInvisibleBlock(BLUE_SQUARE);
			nRemainTime = 15.0f;
			float nReduce = 3.0f + nTimeDifficulty * 0.03f;
			if (bExtreme)
			{
				nReduce = nReduce * pow(1.3f, nStageLevel + 1);
			}

			nRemainTime = nRemainTime - nReduce;
		}
		else
		{
			block = getInvisibleBlock(RED_SQUARE);
			nRemainTime = 2.0f;
		}


		block->setPosition(Pos);
		block->setScale(0.0f);

		auto action = Sequence::create(
			ScaleTo::create(0.5f, nLineBlockScale),
			ScaleTo::create(nRemainTime, nLineBlockScale / 2),
			ScaleTo::create(0.5f, 0),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
			NULL);
		block->runAction(action);
	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 5:
	{
		int nVisibleCount = 0;

		int nType = BLUE_CIRCLE;

		for (int i = 0; i < vBlock[nType].size(); i++)
		{
			if (vBlock[nType].at(i)->isVisible() == true)
				nVisibleCount++;
		}


		if (nVisibleCount >= nMaxArise)
			return;

		float nRand = CCRANDOM_0_1();
		nRand = (nRand == 1 ? 0 : nRand);

		//pos 구하고 위치 확인. 안되면 return.
		int nPos = nRand * nLineCount * nLineCount;
		Vec2 Pos = arrPos[nPos / nLineCount][nPos % nLineCount];
		//Vec2 Pos = PlaySize * arrPos[nPos / nLineCount][nPos % nLineCount] + origin; //origin 확인

		for (int i = 0; i < vBlock[nType].size(); i++)
		{
			BlockSprite* block = vBlock[nType].at(i);
			if (block->getPosition() == Pos && block->isVisible() == true)
				return;
		}


		//block 세팅.
		BlockSprite* block = NULL;
		block = getInvisibleBlock(nType);

		block->setPosition(Pos);
		block->setScale(0.0f);

		float nRemainTime = 1.5f - (nTimeDifficulty * 0.015f);

		if (bExtreme)
		{
			nRemainTime = nRemainTime * 0.8f;
		}

		auto action = Sequence::create(
			ScaleTo::create(nRemainTime / 2, nLineBlockScale),
			ScaleTo::create(nRemainTime / 2, 0),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
			NULL);
		block->runAction(action);
	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 1:
	{
		int nVisibleCount = 0;

		for (int k = 0; k < 2; k++)
		{
			int nBaseType = 0;
			if (k == 0)
				nBaseType = RED_CIRCLE;
			else
				nBaseType = RED_SQUARE;

			for (int t = 0; t < 3; t++)
			{
				int nType = nBaseType + t;

				for (int i = 0; i < vBlock[nType].size(); i++)
				{
					if (vBlock[nType].at(i)->isVisible() == true)
						nVisibleCount++;
				}
			}
		}

		if (nVisibleCount >= nMaxArise)
			return;

		float nRand = CCRANDOM_0_1();
		nRand = (nRand == 1 ? 0 : nRand);

		//pos 구하고 위치 확인. 안되면 return.
		int nPos = nRand * nLineCount * nLineCount;
		Vec2 Pos = arrPos[nPos / nLineCount][nPos % nLineCount];
		//Vec2 Pos = PlaySize * arrPos[nPos / nLineCount][nPos % nLineCount] + origin; //origin 확인

		for (int k = 0; k < 2; k++)
		{
			int nBaseType = 0;
			if (k == 0)
				nBaseType = RED_CIRCLE;
			else
				nBaseType = RED_SQUARE;

			for (int t = 0; t < 3; t++)
			{
				int nType = nBaseType + t;

				for (int i = 0; i < vBlock[nType].size(); i++)
				{
					BlockSprite* pBlock = vBlock[nType].at(i);
					if (pBlock->getPosition() == Pos && pBlock->isVisible() == true)
						return;
				}
			}
		}



		//block 세팅.
		BlockSprite* block = NULL;

		nRand = CCRANDOM_0_1();
		int nType = RED_CIRCLE;

		if (nRand < 0.15f)
			nType = RED_CIRCLE;
		else if (nRand < 0.3f)
			nType = GREEN_CIRCLE;
		else if (nRand < 0.5f)
			nType = BLUE_CIRCLE;
		else if (nRand < 0.7f)
			nType = RED_SQUARE;
		else if (nRand < 0.85f)
			nType = GREEN_SQUARE;
		else
			nType = BLUE_SQUARE;

		block = getInvisibleBlock(nType);

		block->setPosition(Pos);
		block->setScale(0.0f);

		float nRemainTime = 1.5f - (nTimeDifficulty * 0.015f);

		if (bExtreme)
		{
			nRemainTime = nRemainTime * 0.8f;
		}

		auto action = Sequence::create(
			ScaleTo::create(nRemainTime / 2, nLineBlockScale),
			ScaleTo::create(nRemainTime / 2, 0),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
			NULL);
		block->runAction(action);
	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 4:
	{
		auto blue_o = (Sprite*)pLayer->getChildByTag(ETC1);
		auto red_x = (Sprite*)pLayer->getChildByTag(ETC2);
		if (blue_o == NULL)
		{
			blue_o = Sprite::createWithSpriteFrameName("blue_o.png");
			blue_o->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			blue_o->setPosition(center);
			blue_o->setScale(1.5f);
			blue_o->setTag(ETC1);
			pLayer->addChild(blue_o, 1);
		}
		if (red_x == NULL)
		{
			red_x = Sprite::createWithSpriteFrameName("red_x.png");
			red_x->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			red_x->setPosition(center);
			red_x->setScale(1.5f);
			red_x->setTag(ETC2);
			pLayer->addChild(red_x);
			red_x->setVisible(true);
		}

		if (bCircle == true)
		{
			blue_o->setVisible(true);
			red_x->setVisible(false);
		}
		else
		{
			blue_o->setVisible(false);
			red_x->setVisible(true);
		}


		if (bTouch == true)
		{
			if (bCircle == true)
			{
				UpdateCombo(-1, -1, 1);
				ChangeLife(0.25f, true);
				ChangeStagePoint(POINT_TYPE05);
				Particle_Heal(prevTouchPoint, POINT_TYPE3, true, true);

			}
			else
			{
				float nDamage = -3;
				if (bExtreme)
				{
					nDamage = nDamage * pow(1.3f, nStageLevel + 1);
				}

				ChangeLife(nDamage, false);
				UpdateCombo(-1, -1, 0);
				Particle_X(prevTouchPoint, 0.7f, true, true);
			}
		}

		nCurrentClickCount++;
		if (nCurrentClickCount >= nCurrentLimitCount)
		{
			bCircle = !bCircle;
			nCurrentLimitCount = nMaxClickCount * 0.2f + nMaxClickCount * CCRANDOM_0_1();
			nCurrentClickCount = 0;
		}
	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 98:
	{
		auto labelNumber = (Label*)pLayer->getChildByTag(ETC1);
		if (labelNumber == NULL)
		{
			labelNumber = Label::create();//createWithTTF("10", _fonts_Base, 222);
			labelNumber->setSystemFontSize(222);
			labelNumber->setPosition(gameSize.width * 0.8, gameSize.height*0.5);
			labelNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);//right
			labelNumber->setTag(ETC1);
			pLayer->addChild(labelNumber);
		}

		auto labelPM = (Label*)pLayer->getChildByTag(ETC2);
		if (labelPM == NULL)
		{
			labelPM = Label::create();// createWithTTF("+", _fonts_Base, 222);
			labelPM->setSystemFontSize(222);
			labelPM->setPosition(gameSize.width *0.3, gameSize.height*0.5);
			labelPM->setTag(ETC2);
			pLayer->addChild(labelPM);
		}


		if (nCurrentNumber > 0)
		{
			labelPM->setString("+");
			labelPM->setTextColor(Color4B::GREEN);
			labelNumber->setString(StringUtils::format("%3d", nCurrentNumber));
			labelNumber->setTextColor(Color4B::GREEN);
		}
		else
		{
			labelPM->setString("-");
			labelPM->setTextColor(Color4B::RED);
			labelNumber->setString(StringUtils::format("%3d", -nCurrentNumber));
			labelNumber->setTextColor(Color4B::RED);
		}


		int nTimeTick = 12 - ((float)nStageLevel * 0.5f);
		if (nTimeTick < 5)
			nTimeTick = 5;

		if (nMainTick % nTimeTick == 0)
		{
			nCurrentNumber++;
			if (nCurrentNumber == 0)
				nCurrentNumber = 10;
			else if (nCurrentNumber >= 10)
				nCurrentNumber = nStartNumber;
		}


	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 96:
	{
		// Blue 생성
		Vec2 posBlue = Vec2(playSize.x * 0.5, 50) + origin;
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("blue_c.png");
			blue->setTag(PLAYER_BLUE);
			blue->setScale(PLAYER_BLUE_BASE_SCALE);
			blue->setPosition(posBlue);
			pLayer->addChild(blue, 1);
		}
		Vec2 posGreen = Vec2(playSize.x * 0.5, playSize.y) + origin;
		Sprite* green = (Sprite*)pLayer->getChildByTag(ETC1);
		if (green == NULL)
		{
			green = Sprite::createWithSpriteFrameName("green_s.png");
			green->setTag(ETC1);
			green->setScale(PLAYER_BLUE_BASE_SCALE * 4);
			green->setPosition(posGreen);
			pLayer->addChild(green);
		}


		int nRows = 5;
		int nCols = 3;
		while (vBlock[RED_SQUARE].size() < nRows * nCols)
		{
			int nNumber = vBlock[RED_SQUARE].size();
			BlockSprite* pBlock = getInvisibleBlock(RED_SQUARE);
			int x = nNumber % nCols; // 0,1,2 / 0,1,2 / 0,1,2 / 0,1,2 / 0,1,2
			int y = nNumber / nCols; // 0,0,0 / 1,1,1 / 2,2,2 / 3,3,3 / 4,4,4

			pBlock->setScaleX(0.5f);
			pBlock->setScaleY(0.25f + y * 0.05f);

			Vec2 Pos = Vec2((playSize.x / 2) * x, 150 * y + 300);
			pBlock->setPosition(Pos);
		}

		for (int i = 0; i < nRows; i++)
		{
			for (int j = 0; j < nCols; j++)
			{
				BlockSprite* pBlock = vBlock[RED_SQUARE].at(i * nCols + j);
				float x = pBlock->getPositionX();

				float nSpeed = 1.5f + nStageLevel * 0.5f + (0.4f * i);

				if (i % 2 == 0)
				{
					nSpeed = nSpeed * -1;
				}
				x = x + nSpeed;

				if (x < -(playSize.x * 0.25f))
					x = (playSize.x * 1.25f);
				else if (x >(playSize.x * 1.25f))
					x = -(playSize.x * 0.25f);

				pBlock->setPositionX(x);
			}
		}

		//Red Blue 충돌 확인.
		if (blue != NULL &&p1.GetLife() > 0)
		{
			Rect rectBlue = blue->getBoundingBox();

			for (BlockSprite* pBlock : vBlock[RED_SQUARE])
			{
				if (pBlock == NULL)
					continue;

				if (pBlock->isVisible() == false)
					continue;

				Rect rectBlock = pBlock->getBoundingBox();
				float nRadius = rectBlue.getMidX() - rectBlue.getMinX();
				if (rectBlock.intersectsCircle(blue->getPosition(), nRadius *0.9f))
				{
					blue->setPosition(posBlue);
					bShot = false;
					UpdateCombo(-1, -1, 0);
					ChangeLife(-30);
					Particle_Steam(blue->getPosition(), blue->getScale() * 4);
					break;
				}
			}
		}


		if (blue != NULL &&p1.GetLife() > 0)
		{
			Rect rectBlue = blue->getBoundingBox();
			Rect rectGreen = green->getBoundingBox();
			if (rectGreen.intersectsRect(rectBlue))
			{
				blue->setPosition(posBlue);
				bShot = false;
				UpdateCombo(-1, -1, 1);
				ChangeLife(20, true);
				ChangeStagePoint(POINT_TYPE6);
				Particle_Heal(blue->getPosition(), POINT_TYPE6);
			}
		}

		if (p1.GetLife() == 0)
		{
			blue->setVisible(false);
		}
		else
		{
			blue->setVisible(true);
			//Blue 사이즈 조절
			blue->setScale(PLAYER_BLUE_BASE_SCALE + p1.GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 2);
		}
	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 6:
	{
		if (bSettingDelay == true)
		{
			bSettingDelay = false;

			int nCards = nLineCount * nLineCount;
			float nRand = CCRANDOM_0_1();
			if (nRand == 1)
				nRand = 0;

			nVisibleNumber = nRand * nCards;
			for (int i = 0; i < nCards; i++)
				vBlock[BLACK_SQUARE].at(i)->setVisible(true);

			vBlock[BLACK_SQUARE].at(nVisibleNumber)->setVisible(false);
			Vec2 nVisiblePos = arrPos[nVisibleNumber / nLineCount][nVisibleNumber % nLineCount];
			Particle_O(nVisiblePos, 1.0f);
		}
	}
	break;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case 8:
	{
		// Blue 생성
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			return;
		}
		float nBlueX = blue->getPositionX();
		float nBlueY = blue->getPositionY();

		float nSpeed = 10 + nTimeDifficulty * 0.1f;

		//Ball 생성
		int nTime = 10;
		if (bExtreme)
			nTime = 3;

		int nRedCount = 1 + nMainTick / (60 * nTime); // nTime 초당 1개
		int nVisibleRedCount = 0;
		for (int i = 0; i < vBlock[RED_CIRCLE].size(); i++)
		{
			if (vBlock[RED_CIRCLE].at(i)->isVisible())
				nVisibleRedCount++;
		}

		int nCreateRedCount = nRedCount - nVisibleRedCount;
		for (int i = 0; i < nCreateRedCount; i++)
		{
			BlockSprite* red = getInvisibleBlock(RED_CIRCLE, 1);
			red->setScale(MISSILE_SCALE);
			red->setPosition(playSize.x * 0.5f, origin.y + playSize.y - 25);
			red->nUDSpeed = -nSpeed;
			red->nLRSpeed = 0;
		}


		//Player - red 충돌 방향변경
		Rect rectBlue = blue->getBoundingBox();
		for (int i = 0; i < nRedCount; i++)
		{
			BlockSprite* red = vBlock[RED_CIRCLE].at(i);
			if (red == NULL || red->isVisible() == false)
				continue;

			Rect rectBlock = red->getBoundingBox();
			float nRadius = rectBlue.getMidX() - rectBlue.getMinX();
			if (rectBlock.intersectsCircle(blue->getPosition(), nRadius * 0.9f))
			{
				float redX = red->getPositionX();
				float redY = red->getPositionY();

				float nDisX = redX - nBlueX;
				if (nDisX >= 0 && nDisX < 1)
					nDisX = 1;
				else if (nDisX < 0 && nDisX > -1)
					nDisX = -1;

				float nDisY = redY - nBlueY;
				if (nDisY >= 0 && nDisY < 1)
					nDisY = 1;
				else if (nDisY < 0 && nDisY > -1)
					nDisY = -1;

				bool bMinusX = false;
				bool bMinusY = false;
				if (nDisX < 0)
				{
					bMinusX = true;
					nDisX *= -1;
				}
				if (nDisY < 0)
				{
					bMinusY = true;
					nDisY *= -1;
				}

				red->nLRSpeed = nSpeed * nDisX / (nDisX + nDisY);
				if (bMinusX)
					red->nLRSpeed *= -1;

				red->nUDSpeed = nSpeed * nDisY / (nDisX + nDisY);
				if (bMinusY)
					red->nUDSpeed *= -1;

				Sound(_sounds_Tong, true);
			}
		}

		//Block - red 충돌
		for (int i = 0; i < nRedCount; i++)
		{
			BlockSprite* red = vBlock[RED_CIRCLE].at(i);
			if (red == NULL || red->isVisible() == false)
				continue;

			Rect rectRed = red->getBoundingBox();
			float redX = red->getPositionX();
			float redY = red->getPositionY();
			bool bNextRed = false;

			for (BlockSprite* pBlock : vBlock[ALPHA_BLACK_SQUARE])
			{
				if (bNextRed == true)
					break;

				if (pBlock->isVisible() == false)
					continue;

				Rect rectBlock = pBlock->getBoundingBox();
				float nRadius = rectRed.getMidX() - rectRed.getMinX();
				if (rectBlock.intersectsCircle(red->getPosition(), nRadius * 0.9f))
				{

					pBlock->setVisible(false);

					ChangeLife(0.1f, true);
					//	UpdateCombo(-1, -1, 1);
					ChangeStagePoint(POINT_TYPE1);
					Particle_Heal(red->getPosition(), POINT_TYPE1, true, true);
					float nDisX = redX - pBlock->getPositionX();
					float nDisY = redY - pBlock->getPositionY();
					if (nDisX < 0) //절대값
						nDisX *= -1;
					if (nDisY < 0) //절대값
						nDisY *= -1;

					if (nDisX > nDisY)
						red->nLRSpeed *= -1.0f;
					else
						red->nUDSpeed *= -1.0f;

					bNextRed = true;
				}
			}
		}

		//Ball 이동 + 벽 충돌
		for (int i = 0; i < nRedCount; i++)
		{
			BlockSprite* red = vBlock[RED_CIRCLE].at(i);
			if (red == NULL || red->isVisible() == false)
				continue;


			float toX = red->getPositionX() + red->nLRSpeed;
			float toY = red->getPositionY() + red->nUDSpeed;
			if (toX < 0)
			{
				toX = 0;
				red->nLRSpeed *= -1.0f;
			}
			else if (toX > playSize.x)
			{
				toX = playSize.x;
				red->nLRSpeed *= -1.0f;
			}

			if (toY < origin.y)
			{
				toY = origin.y;
				red->nUDSpeed *= -1.0f;
				{

					UpdateCombo(-1, -1, 0);
					ChangeLife(-5, false);
					Particle_Steam(red->getPosition(), 0.5f);

				}
			}
			else if (toY > origin.y + playSize.y)
			{
				toY = origin.y + playSize.y;
				red->nUDSpeed *= -1.0f;
			}

			red->setPosition(toX, toY);
		}

		//
		if (p1.GetLife() == 0)
		{
			blue->setVisible(false);
		}
		else
		{
			blue->setVisible(true);
			blue->setScale((PLAYER_BLUE_BASE_SCALE * 3 - p1.GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 1) * 3);
		}

		//Stage 생성
		// 장애물 생성 공이 없는 부분에 10초간 생성 후 사라짐?
		int nVisibleCount = 0;
		for (int i = 0; i < vBlock[ALPHA_BLACK_SQUARE].size(); i++)
		{
			if (vBlock[ALPHA_BLACK_SQUARE].at(i)->isVisible() == true)
				nVisibleCount++;
		}


		nMaxArise = nLineCount * 8;

		if (nVisibleCount >= nMaxArise)
			return;


		float nRand = CCRANDOM_0_1();
		nRand = (nRand == 1 ? 0 : nRand);
		//pos 구하고 위치 확인. 안되면 return.
		int nPos = nRand * nLineCount * nLineCount;
		if (nPos % nLineCount < 2)
			return;

		Vec2 Pos = arrPos[nPos / nLineCount][nPos % nLineCount];
		for (int i = 0; i < vBlock[ALPHA_BLACK_SQUARE].size(); i++)
		{
			BlockSprite* block = vBlock[ALPHA_BLACK_SQUARE].at(i);
			if (block->getPosition() == Pos && block->isVisible() == true)
				return;
		}


		//block 세팅.
		BlockSprite* block = NULL;
		for (int i = 0; i < 8; i++)
		{
			block = getInvisibleBlock(ALPHA_BLACK_SQUARE);
			block->setPosition(Pos);
			block->setScale(nLineBlockScale);

		}

	}
	break;
	}
}

//나중에 데이터 설정이랑 드로우 설정이랑 분리
void PlayFunc::Set_UnifyType()
{
	for (int k = 0; k < 2; k++)
	{
		int nBaseType = 0;
		if (k == 0)
			nBaseType = RED_CIRCLE;
		else
			nBaseType = RED_SQUARE;

		for (int t = 0; t < 2; t++)
		{
			int nType = nBaseType + t;
			while (vBlock[nType].size() < nLineCount * nLineCount)
			{
				getInvisibleBlock(nType);
			}
		}
	}

	// 포인트 번호당 도형 1,2,3,4 위치 시키기
	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			int nNumber = i * nLineCount + j;

			Vec2 Pos = arrPos[nNumber / nLineCount][nNumber % nLineCount];
			//Vec2 Pos = PlaySize * arrPos[nNumber / nLineCount][nNumber % nLineCount] + origin;
			BlockSprite* red_c = vBlock[RED_CIRCLE].at(nNumber);
			red_c->setPosition(Pos);
			red_c->setVisible(false);
			red_c->setScale(nLineBlockScale);

			BlockSprite* red_s = vBlock[RED_SQUARE].at(nNumber);
			red_s->setPosition(Pos);
			red_s->setVisible(false);
			red_s->setScale(nLineBlockScale);

			BlockSprite* green_c = vBlock[GREEN_CIRCLE].at(nNumber);
			green_c->setPosition(Pos);
			green_c->setVisible(false);
			green_c->setScale(nLineBlockScale);

			BlockSprite* green_s = vBlock[GREEN_SQUARE].at(nNumber);
			green_s->setPosition(Pos);
			green_s->setVisible(false);
			green_s->setScale(nLineBlockScale);

			float nRand = CCRANDOM_0_1();
			arrType[i][j] = ((int)(nRand * 10) % 4);

			if (i == nLineCount - 1 && j == nLineCount - 1) // 다 같은 값 나오는거 방지.
			{
				arrType[i][j] = (arrType[0][0] + 1) % 4;
			}
			if (arrType[i][j] == 0)
			{
				red_c->setVisible(true); red_s->setVisible(false); green_c->setVisible(false); green_s->setVisible(false);
			}
			else if (arrType[i][j] == 1)
			{
				red_c->setVisible(false); red_s->setVisible(true); green_c->setVisible(false); green_s->setVisible(false);
			}
			else if (arrType[i][j] == 2)
			{
				red_c->setVisible(false); red_s->setVisible(false); green_c->setVisible(true); green_s->setVisible(false);
			}
			else
			{
				red_c->setVisible(false); red_s->setVisible(false); green_c->setVisible(false); green_s->setVisible(true);
			}
		}
	}
}


void PlayFunc::Set_MakeWay()
{
	auto blue3 = (Sprite*)pLayer->getChildByTag(ETC3);
	blue3->setVisible(false);
	//초기화
	for (int i = 0; i < nLineCount; i++)
		for (int j = 0; j < nLineCount; j++)
			arrType[i][j] = -1;


	arrType[0][0] = arrType[nLineCount - 1][nLineCount - 1] = BLOCK_LINE_0;
	arrType[0][nLineCount - 1] = BLOCK_LINE_5;
	arrType[nLineCount - 1][0] = BLOCK_LINE_4;
	arrType[nLineCount / 2][nLineCount / 2] = 0;

	bool bPattern = CCRANDOM_0_1() > 0.5f; // 1,2,3,1,2,3 <-> 1,3,2,1,3,2
	int nFirst = (CCRANDOM_0_1() > 0.65f) ? 0 : ((CCRANDOM_0_1() > 0.5f) ? 1 : 2);
	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			if (arrType[i][j] == -1)
			{
				if (nFirst % 3 == 0)
					arrType[i][j] = BLOCK_LINE_1;
				else if (nFirst % 3 == 1)
					arrType[i][j] = BLOCK_LINE_2;
				else if (nFirst % 3 == 2)
					arrType[i][j] = BLOCK_LINE_3;

				nFirst += bPattern ? 1 : 2;

				if (CCRANDOM_0_1() > 0.8f) //고정 패턴 깨기
					nFirst += 1;
			}
		}
	}


	Draw_MakeWay();
}

void PlayFunc::Draw_MakeWay()
{
	for (int nType = BLOCK_LINE_0; nType <= BLOCK_LINE_5B; nType++)
	{
		for (int i = 0; i < vBlock[nType].size(); i++)
		{
			vBlock[nType].at(i)->setVisible(false);
		}
	}


	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			if (arrType[i][j] == 0)
				continue;

			BlockSprite* pBlock = getInvisibleBlock(arrType[i][j], 0);
			pBlock->setPosition(arrPos[i][j]);
			pBlock->setScale(nLineBlockScale);
		}
	}
	//yellow draw
	//////////////////////////////////////////////////////////////////////////////
	bool bAnswer = false;
	if (arrType[nLineCount - 1][nLineCount - 1] == BLOCK_LINE_1
		|| arrType[nLineCount - 1][nLineCount - 1] == BLOCK_LINE_3
		|| arrType[nLineCount - 1][nLineCount - 1] == BLOCK_LINE_5)
	{
		arrType[nLineCount - 1][nLineCount - 1] += 100;
		Check_MakeWay(nLineCount - 1, nLineCount - 1, arrType[nLineCount - 1][nLineCount - 1]);
	}

	//정답 체크
	if (arrType[0][0] == 100 + BLOCK_LINE_1 ||
		arrType[0][0] == 100 + BLOCK_LINE_3 ||
		arrType[0][0] == 100 + BLOCK_LINE_4)
		bAnswer = true;

	//위 Draw
	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			if (arrType[i][j] >= 100)
			{
				arrType[i][j] -= 100;
				BlockSprite* pBlock = getInvisibleBlock(arrType[i][j] + (BLOCK_LINE_1B - BLOCK_LINE_1), 1);
				pBlock->setPosition(arrPos[i][j]);
				pBlock->setScale(nLineBlockScale + nLineBlockScale*0.1f);
			}
		}
	}

	if (bAnswer)
	{
		auto blue3 = (Sprite*)pLayer->getChildByTag(ETC3);
		blue3->setVisible(true);
	}
	if (bAnswer == false) //정답이면 아래는 안그림
	{
		if (arrType[0][0] == BLOCK_LINE_1
			|| arrType[0][0] == BLOCK_LINE_3
			|| arrType[0][0] == BLOCK_LINE_4)
		{
			arrType[0][0] += 100;
			Check_MakeWay(0, 0, arrType[0][0]);
		}

		//아래 Draw
		for (int i = 0; i < nLineCount; i++)
		{
			for (int j = 0; j < nLineCount; j++)
			{
				if (arrType[i][j] >= 100)
				{
					arrType[i][j] -= 100;
					BlockSprite* pBlock = getInvisibleBlock(arrType[i][j] + (BLOCK_LINE_1G - BLOCK_LINE_1), 1);
					pBlock->setPosition(arrPos[i][j]);
					pBlock->setScale(nLineBlockScale + nLineBlockScale*0.1f);
				}
			}
		}
	}


	if (bAnswer == true)
	{
		ChangeLife(25, true);
		ChangeStagePoint(POINT_TYPE6);
		Particle_Heal(Vec2(gameSize.width *0.5f, gameSize.height*0.5f), POINT_TYPE6);
		bCallBackDelay = true;
		auto action = Sequence::create(
			DelayTime::create(0.5f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 1)),
			NULL);
		pLayer->runAction(action);
	}
}

void PlayFunc::Check_MakeWay(int x, int y, int nType)
{
	if (x < 0 || x > nLineCount - 1
		|| y < 0 || y > nLineCount - 1)
		return;

	nType = nType - 100;

	if (x > 0)
	{
		if (arrType[x - 1][y] == BLOCK_LINE_2
			|| arrType[x - 1][y] == BLOCK_LINE_3
			|| arrType[x - 1][y] == BLOCK_LINE_4)
		{
			if (nType == BLOCK_LINE_2 || nType == BLOCK_LINE_3 || nType == BLOCK_LINE_5)
			{
				arrType[x - 1][y] += 100;
				Check_MakeWay(x - 1, y, arrType[x - 1][y]);
			}
		}
	}

	if (x < nLineCount - 1)
	{
		if (arrType[x + 1][y] == BLOCK_LINE_2
			|| arrType[x + 1][y] == BLOCK_LINE_3
			|| arrType[x + 1][y] == BLOCK_LINE_5)
		{
			if (nType == BLOCK_LINE_2 || nType == BLOCK_LINE_3 || nType == BLOCK_LINE_4)
			{
				arrType[x + 1][y] += 100;
				Check_MakeWay(x + 1, y, arrType[x + 1][y]);
			}
		}
	}

	if (y > 0)
	{
		if (arrType[x][y - 1] == BLOCK_LINE_1
			|| arrType[x][y - 1] == BLOCK_LINE_3
			|| arrType[x][y - 1] == BLOCK_LINE_5)
		{
			if (nType == BLOCK_LINE_1 || nType == BLOCK_LINE_3 || nType == BLOCK_LINE_4)
			{
				arrType[x][y - 1] += 100;
				Check_MakeWay(x, y - 1, arrType[x][y - 1]);
			}
		}
	}

	if (y < nLineCount - 1)
	{
		if (arrType[x][y + 1] == BLOCK_LINE_1
			|| arrType[x][y + 1] == BLOCK_LINE_3
			|| arrType[x][y + 1] == BLOCK_LINE_4)
		{
			if (nType == BLOCK_LINE_1 || nType == BLOCK_LINE_3 || nType == BLOCK_LINE_5)
			{
				arrType[x][y + 1] += 100;
				Check_MakeWay(x, y + 1, arrType[x][y + 1]);
			}
		}
	}
}


void PlayFunc::Set_HighCount()
{
	// BLUE_SQUARE 0
	// GREEN_SQUARE 1
	// RED_CIRCLE 2
	// YELLOW_CIRLE 3
	// WHITE_SQUARE 4
	for (int i = 0; i < 5; i++)
		arrCount[i] = 0;//////////////////////////////////////////

	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			if (arrType[i][j] == -1)
			{

				if (bExtreme)
				{
					int nRand = CCRANDOM_0_1() * 5;
					if (nRand == 5)
						nRand = 4;
					arrType[i][j] = nRand;
				}
				else
				{
					int nRand = CCRANDOM_0_1() * 4;
					if (nRand == 4)
						nRand = 3;
					arrType[i][j] = nRand;
				}
			}

			arrCount[arrType[i][j]]++;///////////////////
		}
	}

	Draw_HighCount();
}

void PlayFunc::Draw_HighCount()
{
	// BLUE_SQUARE 0
	// GREEN_SQUARE 1
	// RED_CIRCLE 2
	// YELLOW_CIRLE 3
	// WHITE_SQUARE 4
	for (int nType = BLOCK_BASE; nType < BLOCK_MAX_COUNT; nType++)
	{
		for (int i = 0; i < vBlock[nType].size(); i++)
		{
			vBlock[nType].at(i)->setVisible(false);
		}
	}

	for (int i = 0; i < nLineCount; i++)
	{
		for (int j = 0; j < nLineCount; j++)
		{
			int nType = 0;
			if (arrType[i][j] == 0)
			{
				nType = BLUE_SQUARE;
			}
			else if (arrType[i][j] == 1)
			{
				nType = GREEN_SQUARE;
			}
			else if (arrType[i][j] == 2)
			{
				nType = RED_CIRCLE;
			}
			else if (arrType[i][j] == 3)
			{
				nType = YELLOW_CIRCLE;
			}
			else if (arrType[i][j] == 4)
			{
				nType = WHITE_SQUARE;
			}

			Vec2 Pos = arrPos[i][j];
			BlockSprite* pBlock = getInvisibleBlock(nType);
			pBlock->setPosition(Pos);
			pBlock->setScale(nLineBlockScale);
		}
	}

}

bool PlayFunc::Check_HighCount(int nType)
{
	// BLUE_SQUARE 0
	// GREEN_SQUARE 1
	// RED_CIRCLE 2
	// YELLOW_CIRLE 3
	// WHITE_SQUARE 4

	int nCountType;
	if (nType == BLUE_SQUARE)
		nCountType = 0;
	else if ( nType == GREEN_SQUARE)
		nCountType = 1;
	else if (nType == RED_CIRCLE)	
		nCountType = 2;
	else if (nType == YELLOW_CIRCLE)	
		nCountType = 3;
	else if (nType == WHITE_SQUARE)
		nCountType = 4;

	int nMaxCount = 0;
	for (int i = 0; i < 5; i++)
	{
		if (nMaxCount < arrCount[i])
			nMaxCount = arrCount[i];
	}

	if (arrCount[nCountType] == nMaxCount)
	{
		for (int i = 0; i < nLineCount; i++)
			for (int j = 0; j < nLineCount; j++)
				if (arrType[i][j] == nCountType)
					arrType[i][j] = -1;

		Set_HighCount();

		return true;
	}
	else
		return false;
}


void PlayFunc::Set_MemorizeColors()
{
	// 칸 갯수 구하고
	int nCards = nLineCount * nLineCount;

	// 빨,파,흰,검 갯수만큼 생성하고 각 위치에 맞게 배치

	for (int k = 0; k < 4; k++)
	{
		int nType = 0;
		if (k == 0)
			nType = RED_SQUARE;
		else if (k == 1)
			nType = BLUE_SQUARE;
		else if (k == 2)
			nType = WHITE_SQUARE;
		else
			nType = BLACK_SQUARE;

		for (int i = 0; i < nCards; i++)
		{
			Vec2 Pos = arrPos[i / nLineCount][i % nLineCount];
			//Vec2 Pos = PlaySize * arrPos[i / nLineCount][i % nLineCount] + origin;

			if (vBlock[nType].size() < nCards)
				getInvisibleBlock(nType);
			vBlock[nType].at(i)->setPosition(Pos);
			vBlock[nType].at(i)->setVisible(true);
			vBlock[nType].at(i)->setScale(nLineBlockScale);
		}
	}

	//랜덤 색 정하기
	int nArrCount = 0;
	int nRandomColorCount = 0;
	int nRandomColor = 0;

	for (int i = 0; i < nCards; i++)
	{
		arrType[i / nLineCount][i%nLineCount] = -1;
	}

	int nArrPos = 0;
	while (1)
	{
		if (CCRANDOM_0_1() > 0.7f && arrType[nArrPos / nLineCount][nArrPos%nLineCount] == -1)
		{
			arrType[nArrPos / nLineCount][nArrPos%nLineCount] = nRandomColor;
			nRandomColorCount++;
			if (nRandomColorCount > (nCards / 3 - 1) && nRandomColor < 2)
			{
				nRandomColorCount = 0;
				nRandomColor++;
			}

			nArrCount++;
			if (nArrCount == nCards)
				break;
		}
		nArrPos++;
		if (nArrPos > nCards)
			nArrPos = 0;
	}

	// 랜덤 색 visible 세팅 초기화
	for (int i = 0; i < nCards; i++)
	{
		vBlock[RED_SQUARE].at(i)->setVisible(false);
		vBlock[BLUE_SQUARE].at(i)->setVisible(false);
		vBlock[WHITE_SQUARE].at(i)->setVisible(false);
		vBlock[BLACK_SQUARE].at(i)->setVisible(false);
	}

	// 랜덤 색 visible 세팅
	for (int i = 0; i < nCards; i++)
	{
		if (arrType[i / nLineCount][i % nLineCount] == 0)
			vBlock[RED_SQUARE].at(i)->setVisible(true);
		else if (arrType[i / nLineCount][i % nLineCount] == 1)
			vBlock[BLUE_SQUARE].at(i)->setVisible(true);
		else if (arrType[i / nLineCount][i % nLineCount] == 2)
			vBlock[WHITE_SQUARE].at(i)->setVisible(true);
	}

	//2초 후
	auto action = Sequence::create(
		DelayTime::create(2.0f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 0)),
		NULL);
	pLayer->runAction(action);

}

void PlayFunc::MentalCalcMenu(Ref* pSender)
{
	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();

	bool bAnswer = false;

	switch (nSelectedItem)
	{
	case ETC1 + 1: // blue O
	{
		if (bMentalCalc_Answer == true)
		{
			bAnswer = true;
		}

	}
	break;
	case ETC1 + 2: // blue X
	{
		if (bMentalCalc_Answer == false)
		{
			bAnswer = true;
		}
	}
	break;
	}

	if (bAnswer)
	{
		ChangeLife(3, true);
		UpdateCombo(-1, -1, 1);
		ChangeStagePoint(POINT_TYPE1);
		Particle_Heal(Vec2(gameSize.width *0.5f, gameSize.height*0.5f), POINT_TYPE1);
		Set_MentalCalc();
	}
	else
	{
		ChangeLife(-30);
		UpdateCombo(-1, -1, 0);
		Particle_X(Vec2(gameSize.width *0.5f, gameSize.height*0.5f));
	}

}

void PlayFunc::Set_MentalCalc()
{
	auto menuMentalCalc = (Menu*)pLayer->getChildByTag(ETC1);
	if (menuMentalCalc == NULL)
	{
		auto blue_o = Sprite::createWithSpriteFrameName("blue_o.png");
		auto blue_x = Sprite::createWithSpriteFrameName("blue_x.png");
		auto blue_o_s = Sprite::createWithSpriteFrameName("blue_o_s.png");
		auto blue_x_s = Sprite::createWithSpriteFrameName("blue_x_s.png");
		auto blue_o_Item = MenuItemSprite::create(blue_o, blue_o_s, CC_CALLBACK_1(PlayFunc::MentalCalcMenu, this));
		auto blue_x_Item = MenuItemSprite::create(blue_x, blue_x_s, CC_CALLBACK_1(PlayFunc::MentalCalcMenu, this));

		blue_o_Item->setScale(SIDE_SCALE);
		blue_x_Item->setScale(SIDE_SCALE);
		blue_o_Item->setTag(ETC1 + 1);
		blue_x_Item->setTag(ETC1 + 2);

		menuMentalCalc = Menu::create(blue_o_Item, blue_x_Item, NULL);
		menuMentalCalc->setTag(ETC1);
		menuMentalCalc->setPosition(playSize.x * 0.5f, origin.y + playSize.y * 0.2f);
		menuMentalCalc->alignItemsHorizontallyWithPadding(200);
		pLayer->addChild(menuMentalCalc, 100);
	}

	int nDifficulty = 9;

	auto LabelQuiz = (Label*)pLayer->getChildByTag(ETC3);
	if (LabelQuiz == NULL)
	{
		LabelQuiz = Label::create();// createWithTTF("", _fonts_Base, 90);
		LabelQuiz->setSystemFontSize(90);
		LabelQuiz->setTag(ETC3);
		LabelQuiz->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		LabelQuiz->setTextColor(Color4B(20, 20, 20, 255));
		LabelQuiz->enableOutline(Color4B::WHITE, 2);
		LabelQuiz->setPosition(gameSize.width * 0.5, gameSize.height * 0.5);
		pLayer->addChild(LabelQuiz, 100);
	}

	int a, b, c;
	////////////////////////////
	a = nDifficulty;

	float nRand = CCRANDOM_0_1();
	nRand = (nRand == 1 ? 0 : nRand);
	if (nRand < 0.5f)
	{
		a = a - nRand * 10;
	}

	if (a == nMentalCalc_PrevA)
	{
		a = a - 1;
	}

	if (a < 2)
		a = 2;
	else if (a % 10 == 0)
		a = a - 1;

	nMentalCalc_PrevA = a;


	///////////////////////////
	nRand = CCRANDOM_0_1();
	nRand = (nRand == 1 ? 0 : nRand);
	if (nDifficulty < 10)
	{
		b = nRand * 10;
	}
	else
	{
		b = nRand * nDifficulty;
	}

	if (b < 2)
		b = 2;
	else if (b % 10 == 0)
		b = b - 1;


	/////////////////
	if (bExtreme)
	{
		a = a + 10;
		b = b + 10;
	}

	c = a * b;
	bMentalCalc_Answer = true;

	nRand = CCRANDOM_0_1();
	if (nRand < 0.25)
	{
		if (nRand < 0.125)
		{
			c = a * (b + 1);
		}
		else
		{
			c = (a - 1) * b;
		}

		bMentalCalc_Answer = false;
	}
	else if (nRand < 0.5)
	{
		if (nRand < 0.3)
		{
			c = a * b + nRand * 4 + 1;
		}
		else
		{
			c = a * b - 10;
			if (a * b <= 10)
			{
				c = a * b + 1;
			}

		}
		bMentalCalc_Answer = false;
	}

	LabelQuiz->setString(StringUtils::format("%d X %d = %d", a, b, c));
}

bool PlayFunc::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (bGameMenuPop == true)
		return false;

	if (bCallBackDelay == true) //Delay줄 경우 터치 입력 안되도록.
		return false;


	float x = touch->getLocation().x;
	float y = touch->getLocation().y;

	switch (nGameNumber + 1)
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
	case 9:
	case 8:
	case 4:
	case 16:
	case 12:
	case 13:
	case 910:
	case 914:
	case 11:
	case 96:
	case 15:
	{
		if (nGameNumber + 1 == 9)
			onTouchMoved(touch, unused_event);

		if (nGameNumber + 1 == 96 || nGameNumber + 1 == 13)
		{
			bShot = true;	// 96 go blank는 공이 순간적으로 위치가 변했을 경우를 대비함.
							// 13 clay shot은 처음은 사격을 안하기 위해서
		}



		bTouch = true; // Update에서 움직이기 위해.

		if (bPrevTouch == true)// 이전 터치를 잡고 있을 때, 다음 터치가 무브, 엔드로 들어가지 않도록 : 두 손가락 무브 기능 안됨.
			return false;

		prevTouchPoint = touch->getLocation();

		bPrevTouch = true;

		return true; // bPrevTouch가 true일때 return false이면 다시 터치 못하는 문제 발생.
	}
	break;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	case 3:
	{
		float nJumpPower = JUMP_POWER;
		if (nSpeedY > nJumpPower)
			nSpeedY += 0;
		else
			nSpeedY = nJumpPower;

		Sound(_sounds_Tong);
	}
	break;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	case 7:
	{
		float nJumpPower = JUMP_POWER;
		if (nSpeedY > nJumpPower)
			nSpeedY += 0;
		else
			nSpeedY = nJumpPower;

		if (nSpeedX > 0)
			nSpeedX = -nJumpPower / 2;
		else
			nSpeedX = nJumpPower / 2;

		Sound(_sounds_Tong);
	}
	break;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	case 98:
	{
		if (nCurrentNumber == 10)
		{
			UpdateCombo(-1, -1, 1);

			ChangeLife(nCurrentNumber, true);
			ChangeStagePoint(POINT_TYPE2);
			Particle_Heal(Vec2(x, y), POINT_TYPE2);
		}
		else
		{
			ChangeLife(nCurrentNumber, false);
			UpdateCombo(-1, -1, 0);
			Particle_X(Vec2(x, y));
		}

		nCurrentNumber = nStartNumber;
	}
	break;

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	case 5:
	{
		Rect touchRect;
		touchRect.setRect(x - 30, y - 30, 60, 60);

		for (int i = 0; i < vBlock[BLUE_CIRCLE].size(); i++)
		{
			BlockSprite* pBlock = vBlock[BLUE_CIRCLE].at(i);
			if (pBlock->isVisible() == false)
				continue;

			if (pBlock->getBoundingBox().intersectsRect(touchRect) == true)
			{
				pBlock->stopAllActions();
				pBlock->setVisible(false);

				ChangeLife(1, true);
				UpdateCombo(-1, -1, 1);
				ChangeStagePoint(POINT_TYPE1);
				Particle_Heal(Vec2(x, y), POINT_TYPE1);

				continue;
			}
		}

	}
	break;

	case 10:
	{
		bool bBreak = false;
		for (int nType = RED_CIRCLE; nType <= BLACK_SQUARE; nType++)
		{
			if (bBreak == true)
				return false;

			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite* pBlock = vBlock[nType].at(i);
				if (pBlock->isVisible() == false)
					continue;

				if (pBlock->getBoundingBox().containsPoint(Point(x, y)))
				{
					if (Check_HighCount(nType)) // 맞췄을 경우  점수+효과
					{
						ChangeLife(3, true);
						UpdateCombo(-1, -1, 1);
						ChangeStagePoint(POINT_TYPE2);
						Particle_Heal(Vec2(x, y), POINT_TYPE2);

						bBreak = true;
						break;
					}
					else // 틀렸을 경우 점수 + 효과 
					{
						ChangeLife(-20);
						UpdateCombo(-1, -1, 0);
						Particle_X(Vec2(x, y));

						bBreak = true;
						break;
					}
				}

			}
		}

	}
	break;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	case 1:
	{
		Rect touchRect;
		touchRect.setRect(x - 30, y - 30, 60, 60);
		//check crash
		for (int nType = RED_CIRCLE; nType <= BLUE_CIRCLE; nType++)
		{
			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite* pBlock = vBlock[nType].at(i);
				if (pBlock->isVisible() == false)
					continue;

				if (pBlock->getBoundingBox().intersectsRect(touchRect) == true)
				{
					pBlock->stopAllActions();
					pBlock->setVisible(false);

					ChangeLife(1, true);
					UpdateCombo(-1, -1, 1);
					ChangeStagePoint(POINT_TYPE1);
					Particle_Heal(Vec2(x, y), POINT_TYPE1);

					continue;
				}
			}
		}

		for (int nType = RED_SQUARE; nType <= BLUE_SQUARE; nType++)
		{
			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite* pBlock = vBlock[nType].at(i);
				if (pBlock->isVisible() == false)
					continue;

				if (pBlock->getBoundingBox().intersectsRect(touchRect) == true)
				{
					pBlock->stopAllActions();
					pBlock->setVisible(false);

					ChangeLife(-20);
					UpdateCombo(-1, -1, 0);
					Particle_X(Vec2(x, y));

					continue;
				}
			}
		}
	}
	break;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	case 2:
	{
		//check Position
		int nNumber = -1;
		for (int i = 0; i < nLineCount * nLineCount; i++)
		{
			BlockSprite* red_c = vBlock[RED_CIRCLE].at(i);
			if (red_c->getBoundingBox().containsPoint(Vec2(x, y)) == true)
			{
				nNumber = i;
				Particle_O(Vec2(x, y), 0.2f);
				if (bExtreme)
					ChangeLife(-2, false);

				break;
			}
		}


		if (nNumber != -1)
		{
			int i = nNumber / nLineCount;
			int j = nNumber % nLineCount;
			arrType[i][j]++;
			if (arrType[i][j] % 4 == 0)
				arrType[i][j] = 0;

			BlockSprite* red_c = vBlock[RED_CIRCLE].at(nNumber);
			BlockSprite* red_s = vBlock[RED_SQUARE].at(nNumber);
			BlockSprite* green_c = vBlock[GREEN_CIRCLE].at(nNumber);
			BlockSprite* green_s = vBlock[GREEN_SQUARE].at(nNumber);

			if (arrType[i][j] == 0)
			{
				red_c->setVisible(true); red_s->setVisible(false); green_c->setVisible(false); green_s->setVisible(false);
			}
			else if (arrType[i][j] == 1)
			{
				red_c->setVisible(false); red_s->setVisible(true); green_c->setVisible(false); green_s->setVisible(false);
			}
			else if (arrType[i][j] == 2)
			{
				red_c->setVisible(false); red_s->setVisible(false); green_c->setVisible(true); green_s->setVisible(false);
			}
			else
			{
				red_c->setVisible(false); red_s->setVisible(false); green_c->setVisible(false); green_s->setVisible(true);
			}

			int nType = arrType[0][0];
			bool bAnswer = true;
			for (int i = 0; bAnswer == true && i < nLineCount; i++)
			{
				for (int j = 0; j < nLineCount; j++)
				{
					if (nType != arrType[i][j])
					{
						bAnswer = false;
						break;
					}
				}
			}

			if (bAnswer == true)
			{
				ChangeLife(nLineCount * nLineCount, true);
				ChangeStagePoint(POINT_TYPE5);
				Particle_Heal(Vec2(gameSize.width *0.5f, gameSize.height*0.5f), POINT_TYPE5, true, true);
				bCallBackDelay = true;
				nCallBackDelayLevel = nStageLevel;
				auto action = Sequence::create(
					DelayTime::create(0.5f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 0)),
					NULL);
				pLayer->runAction(action);
			}
		}

	}
	break;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	case 6:
	{
		if (bSettingDelay == false)
		{
			//check crash
			for (int i = 0; i < vBlock[BLACK_SQUARE].size(); i++)
			{
				BlockSprite* black_s = vBlock[BLACK_SQUARE].at(i);
				if (black_s->isVisible() == true
					&& black_s->getBoundingBox().containsPoint(Vec2(x, y)) == true)
				{
					black_s->setVisible(false);

					if (arrType[i / nLineCount][i % nLineCount] == arrType[nVisibleNumber / nLineCount][nVisibleNumber % nLineCount])
					{
						arrType[i / nLineCount][i % nLineCount] = -1;
						UpdateCombo(-1, -1, 1);
						ChangeStagePoint(POINT_TYPE1);
						ChangeLife(1, true);
						Particle_Heal(Vec2(x, y), POINT_TYPE1);
					}
					else
					{
						UpdateCombo(-1, -1, 0);
						ChangeLife(-10, false);
						Particle_X(Vec2(x, y));
					}
					break;
				}
			}


			bool bAnswer = true;
			for (int i = 0; i < nLineCount*nLineCount; i++)
			{
				if (i != nVisibleNumber && arrType[i / nLineCount][i % nLineCount] == arrType[nVisibleNumber / nLineCount][nVisibleNumber % nLineCount])
				{
					bAnswer = false;
					break;
				}
			}

			if (bAnswer == true)
			{
				ChangeLife(nLineCount*nLineCount, true);
				ChangeStagePoint(POINT_TYPE3);
				Particle_Heal(Vec2(gameSize.width *0.5f, gameSize.height*0.5f), POINT_TYPE3);
				bCallBackDelay = true;
				nCallBackDelayLevel = nStageLevel;
				auto action = Sequence::create(
					DelayTime::create(0.5f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 1)),
					NULL);
				pLayer->runAction(action);
			}

		}

	}
	break;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	}


	return false;
}

void PlayFunc::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (bGameMenuPop == true)
	{
		return;
	}

	Point currentTouchPoint = touch->getLocation();

	switch (nGameNumber + 1)
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
	case 8:
	case 12:
	case 13:
	case 16:
	case 96:
	{
		if (nGameNumber + 1 == 96 && bShot == false) //96 go blank
			return;


		auto blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
			return;

		float x = currentTouchPoint.x - prevTouchPoint.x;
		float y = currentTouchPoint.y - prevTouchPoint.y;

		if (blue)
		{
			if (blue->getPosition().x + x < 10
				|| blue->getPosition().x + x > gameSize.width - 10
				|| blue->getPosition().y + y < origin.y + 10
				|| blue->getPosition().y + y >(origin.y + playSize.y) - 10)
			{
				;
			}
			else
			{
				blue->setPosition(blue->getPosition().x + x, blue->getPosition().y + y);
			}
		}


		prevTouchPoint = currentTouchPoint;
	}
	break;
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	case 4:
	case 9:
	case 11:
	case 15:
	{
		bTouch = true;
		prevTouchPoint = currentTouchPoint;
	}
	break;

	}
}

void PlayFunc::onTouchEnded(Touch *touch, Event *unused_event)
{

	bTouch = false;

	bPrevTouch = false;
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	Point currentTouchPoint = touch->getLocation();

	float cpX = currentTouchPoint.x - prevTouchPoint.x;
	float cpY = currentTouchPoint.y - prevTouchPoint.y;
	float absx = cpX, absy = cpY;
	absx = (cpX < 0 ? cpX * -1 : cpX);
	absy = (cpY < 0 ? cpY * -1 : cpY);

	bool bLeft, bRight, bTop, bBottom;
	bLeft = bRight = bTop = bBottom = false;
	if (absx == 0 || (absy / absx < 1.0f))
	{
		if (cpX > 0)
		{
			bRight = true;

		}
		else if (cpX < 0)
		{
			bLeft = true;

		}
	}
	else
	{
		if (cpY > 0)
		{
			bTop = true;

		}
		else if (cpY < 0)
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

	bool bSlideGesture = false;
	if (bTop || bBottom || bRight || bLeft)
	{
		bSlideGesture = true;
	}
	//swipe
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////


	switch (nGameNumber + 1)
	{
	case 910:
	case 914:
	{
		int x, y;
		for (int i = 0; i < nLineCount; i++)
		{
			for (int j = 0; j < nLineCount; j++)
			{
				if (arrType[i][j] == 0)
				{
					x = i;
					y = j;
					break;
				}
			}
		}


		if (bBottom && y == nLineCount - 1)
			bBottom = false;
		if (bTop && y == 0)
			bTop = false;
		if (bLeft && x == nLineCount - 1)
			bLeft = false;
		if (bRight && x == 0)
			bRight = false;

		if (bTop)
		{
			arrType[x][y] = arrType[x][y - 1];
			arrType[x][y - 1] = 0;
		}
		if (bBottom)
		{
			arrType[x][y] = arrType[x][y + 1];
			arrType[x][y + 1] = 0;
		}
		if (bRight)
		{
			arrType[x][y] = arrType[x - 1][y];
			arrType[x - 1][y] = 0;
		}
		if (bLeft)
		{
			arrType[x][y] = arrType[x + 1][y];
			arrType[x + 1][y] = 0;
		}


		float nActionTime = 0.05f;
		if (nGameNumber + 1 == 910)
		{
			if (bTop || bBottom || bRight || bLeft)
			{
				float nDistance = arrPos[0][1].y - arrPos[0][0].y;
				float nDisX, nDisY;
				nDisX = nDisY = 0;
				int nPosX, nPosY;
				nPosX = x;
				nPosY = y;
				if (bTop)
				{
					nDisY = nDistance;
					nPosY -= 1;
				}
				else if (bBottom)
				{
					nDisY = -nDistance;
					nPosY += 1;
				}
				else if (bRight)
				{
					nDisX = nDistance;
					nPosX -= 1;
				}
				else if (bLeft)
				{
					nDisX = -nDistance;
					nPosX += 1;
				}

				bool bColor = false;
				for (int nType = BLOCK_LINE_5B; nType >= BLOCK_LINE_0; nType--)
				{
					for (int i = 0; i < vBlock[nType].size(); i++)
					{
						if (vBlock[nType].at(i)->isVisible())
						{
							Rect rectBlock = vBlock[nType].at(i)->getBoundingBox();
							if (rectBlock.containsPoint(arrPos[nPosX][nPosY]))
							{
								if (bColor == true)
								{
									vBlock[nType].at(i)->setVisible(false);
									break;
								}

								if (nType >= BLOCK_LINE_1G)
								{
									bColor = true;
								}

								bCallBackDelay = true;
								auto action = Sequence::create(
									MoveBy::create(nActionTime, Vec2(nDisX, nDisY)),
									CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 2)),
									NULL);
								vBlock[nType].at(i)->runAction(action);
								//
								float nDamage = -1.0f;
								if (bExtreme)
								{
									nDamage = nDamage * pow(1.3f, nStageLevel + 1);
								}
								ChangeLife(nDamage, false);
								//
								break;
							}
						}
					}
				}

			}
		}
		else if (nGameNumber + 1 == 914)
		{

			if (bTop || bBottom || bRight || bLeft)
			{
				float nDistance = arrPos[0][1].y - arrPos[0][0].y;
				float nDisX, nDisY;
				nDisX = nDisY = 0;
				if (bTop)
					nDisY = nDistance;
				else if (bBottom)
					nDisY = -nDistance;
				else if (bRight)
					nDisX = nDistance;
				else if (bLeft)
					nDisX = -nDistance;

				int nType = arrType[x][y] % 100;

				Sprite* arrSprite = getRSNumber(nType);
				bCallBackDelay = true;
				auto action = Sequence::create(
					MoveBy::create(nActionTime, Vec2(nDisX, nDisY)),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 2)),
					NULL);
				arrSprite->runAction(action);

				//
				float nDamage = -1.0f;
				if (bExtreme)
				{
					nDamage = nDamage * pow(1.3f, nStageLevel + 1);
				}
				ChangeLife(nDamage, false);
				//
			}
			else if (bSlideGesture == false)
			{
				Vec2 Pos(currentTouchPoint.x, currentTouchPoint.y);

				for (int i = 0; i < nLineCount; i++)
				{
					for (int j = 0; j < nLineCount; j++)
					{
						if (i == 0 && j == 0)
							continue;

						int nType = i * nLineCount + j;
						Sprite* arrSprite = getRSNumber(nType);
						Rect block = arrSprite->getBoundingBox();
						if (block.containsPoint(Pos))
						{
							//데이터 수정
							for (int p = 0; p < nLineCount; p++)
							{
								for (int q = 0; q < nLineCount; q++)
								{
									if (arrType[p][q] % 100 == nType)
									{

										arrType[p][q] += 100;

										if (arrType[p][q] >= 400)
											arrType[p][q] -= 400;

									}
								}
							}

							//회전 액션 후 [콜백2에서 드로우]
							bCallBackDelay = true;
							auto action = Sequence::create(
								RotateBy::create(nActionTime, 90),
								CallFuncN::create(CC_CALLBACK_1(PlayFunc::Callback, this, 2)),
								NULL);
							arrSprite->runAction(action);

							//반복문 나가기
							i = nLineCount;
							break;
						}
					}
				}


			}
		}
	}
	break;
	}


}

void PlayFunc::onTouchCancelled(Touch *touch, Event *unused_event)
{
	bPrevTouch = false;

	if (bGameMenuPop == true)
		return;
}


void PlayFunc::update()
{
	if (bGameMenuPop == false)
	{
		nSoundTick++;
		nParticleTick++;
		nMainTick++;
		nFireTick++;


		nTimeDifficulty = nMainTick / (30 * 10); // 5초마다 난이도 조금씩 1씩 증가.


		if (bNeedSetting == true)
			SettingStage();

		UpdateStage();

		//1초당 지속적으로 깍이는 체력
		ChangeLifePerTime();

		//유지 HP 당 주는 포인트
		ChangePointPerTime();

		if (CheckGameOver() == false)  //게임오버 되지 않았다면
		{
			CheckStageLevelUp();
		}
	}

}

void PlayFunc::ChangeLifePerTime()
{
	if (nMainTick % 30 != 0) //0.5초당
	{
		return;
	}

	float nReduceHp = 0.0f;

	switch (nGameNumber + 1)
	{

	case 4:
	{
		nReduceHp = -0.5f * (nStageLevel + 1);
		ChangeLife(nReduceHp, false, false);//지속 체력 감소
	}
	break;

	case 14:
	{
		nReduceHp = -0.5f * (nStageLevel + 1);
		ChangeLife(nReduceHp, false, false);//지속 체력 감소
	}
	break;

	case 98:
	case 2:
	case 6:
	case 10:
	case 910:
	case 914:
	{
		nReduceHp = -0.05f;
		ChangeLife(nReduceHp, false, false);//지속 체력 감소
	}
	break;

	//
	case 9999999:
	{
		float nIncrease = 25.0f;
		nIncrease = nIncrease - (p1.GetLifePercent() / 100 * 5.0f);
		ChangeLife(nIncrease, true); //초당 5~1% 체력 증가
	}
	break;


	}

}
void PlayFunc::ChangePointPerTime()
{
	switch (nGameNumber + 1)
	{
	case 9999999:
	{
		float nLife = 100; // 초당 100점
		if (nMainTick % 60 == 0)
		{
			ChangeStagePoint(nLife);
		}
	}
	break;
	}



}

void PlayFunc::ChangeLife(float nDamage, bool bPercent, bool bRedFlash)
{
	if (nDamage < 0)
	{

		nDamage = nDamage + (nDamage * nTimeDifficulty * 0.1f); // 시간 난이도마다 데미지 증가 10%


		if (bExtreme == false && bRecordMode == false)
		{
			if (__nStagePoint__ < 5000)
				nDamage = nDamage * 0.5f;
		}

		nDamage = nDamage * (1 - (UP_PER_DEFENCE * mdp.nUpDefence));


		if (bInvincible == true)
			nDamage *= 0.01f; // 0이 아닌 이유는 닫기 버튼 종료 하기 위해서.



		if (p1.GetLife() > 0 && bRedFlash == true) // 깍일때 체력바 깜박임 추가.
		{
			auto CrashBar = (Sprite*)pLayer->getChildByTag(CRASH_BAR);
			if (CrashBar)
			{
				CrashBar->setVisible(true);

				auto CrashBG = (Sprite*)pLayer->getChildByTag(CRASH_BG);
				if (CrashBG)
					CrashBG->setVisible(true);

				pLayer->runAction(
					Sequence::create(
						DelayTime::create(0.15f),
						CallFuncN::create(CC_CALLBACK_0(PlayFunc::ResetCrashBar, this)),
						NULL));
			}
		}
	}
	else
	{
		nDamage = nDamage * (1 + (UP_PER_RECOVERY * mdp.nUpRecovery));
	}


	p1.__ChangeLife__(nDamage, bPercent);


	//HP Bar변화 그리기.
	UpdateProgress();

}



void PlayFunc::ChangeStagePoint(float nPoint)
{
	if (p1.GetLife() <= 0)
		return;


	nPoint = nPoint * (100 + p1.GetLifePercent()) / 100; // 1배 ~ 2배

	if (__nCombo__ != 0)
		nPoint = nPoint * (__nCombo__ > 20 ? 20 : __nCombo__);


	nPoint = nPoint * (1 + (UP_PER_POINT * mdp.nUpPoint));

	__nStagePoint__ += nPoint;

	if (__nStagePoint__ >= GAME_RECORD_MAX_POINT)
		__nStagePoint__ = GAME_RECORD_MAX_POINT;


	//스테이지 바 그리기.
	UpdateProgress();
}

void PlayFunc::UpdateCombo(float x, float y, int nChangeCombo)
{
	if (nChangeCombo == 0)
		__nCombo__ = 0;
	else
		__nCombo__ += nChangeCombo;

	if (nMaxCombo < __nCombo__)
		nMaxCombo = __nCombo__;

	auto labelCombo = (Label*)pLayer->getChildByTag(LABEL_COMBO);
	if (labelCombo == NULL)
	{
		labelCombo = Label::create();// createWithTTF("", _fonts_Base, COMBO_FONT_SIZE);
		labelCombo->setSystemFontSize(COMBO_FONT_SIZE);
		labelCombo->setTag(LABEL_COMBO);
		labelCombo->enableBold();
		labelCombo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelCombo->setTextColor(Color4B::WHITE);
		labelCombo->setPosition(gameSize.width*0.5, gameSize.height* 0.95f);
		pLayer->addChild(labelCombo, 499);
	}

	if (__nCombo__ == 0)
		labelCombo->setVisible(false);
	else
		labelCombo->setVisible(true);

	if (x != -1)
		labelCombo->setPosition(x, y);

	labelCombo->setString(StringUtils::format(mdp.bEnglish ? "Combo %d" : UTF8PCHAR("콤보 %d"), __nCombo__));
}

void PlayFunc::Particle_Bang(Vec2 Pos, float nScale, bool bSound, bool bDelay)
{
	if (bDelay == true)
	{
		if (nParticleTick < 0)
		{
			return;
		}
		else
		{
			nParticleTick = -6;
		}
	}

	float x = Pos.x;
	float y = Pos.y;

	auto particle = ParticleSystemQuad::create(_particle_bang);
	particle->setPosition(x, y);
	particle->setScale(nScale);
	pLayer->addChild(particle, 498);

	auto action = Sequence::create(DelayTime::create(0.5f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
		NULL);
	particle->runAction(action);

	if (bSound == true)
	{
		Sound(_sounds_Bang);
	}
}



void PlayFunc::Particle_Heal(Vec2 Pos, float nScale, bool bSound, bool bDelay)
{
	if (bDelay == true)
	{
		if (nParticleTick < 0)
		{
			return;
		}
		else
		{
			nParticleTick = -6;
		}
	}


	float x = Pos.x;
	float y = Pos.y;

	if (nScale <= POINT_TYPE2)
		nScale = 0.1f;
	else if (nScale <= POINT_TYPE4)
		nScale = 0.3f;
	else
		nScale = 0.5f;

	auto particle = ParticleSystemQuad::create(_particle_heal);
	particle->setPosition(x, y);
	particle->setScale(nScale);
	pLayer->addChild(particle, 498);

	auto redBar = (Sprite*)pLayer->getChildByTag(PLAYER_LIFE_BAR);
	float toY = redBar->getPosition().y - redBar->getContentSize().height / 2;
	float toX = playSize.x * p1.GetLifePercent() / 100;
	auto action = Sequence::create(MoveTo::create(0.2f, Vec2(toX, toY)),
		DelayTime::create(0.7f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
		NULL);

	particle->runAction(action);


	if (bSound == true)
	{
		Sound(_sounds_Bubble);
	}
}

void PlayFunc::Particle_O(Vec2 Pos, float nScale, bool bSound, bool bDelay)
{
	if (bDelay == true)
	{
		if (nParticleTick < 0)
		{
			return;
		}
		else
		{
			nParticleTick = -6;
		}
	}


	float x = Pos.x;
	float y = Pos.y;

	auto particle = ParticleSystemQuad::create(_particle_o);
	particle->setPosition(x, y);
	particle->setScale(nScale);
	pLayer->addChild(particle, 498);

	auto action = Sequence::create(DelayTime::create(1.0f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
		NULL);

	particle->runAction(action);


	if (bSound == true)
	{
		Sound(_sounds_Touch);
	}
}

void PlayFunc::Particle_X(Vec2 Pos, float nScale, bool bSound, bool bDelay)
{
	if (bDelay == true)
	{
		if (nParticleTick < 0)
		{
			return;
		}
		else
		{
			nParticleTick = -6;
		}
	}


	float x = Pos.x;
	float y = Pos.y;

	float nXSize = 60;
	float nYSize = 80;
	auto particle = ParticleSystemQuad::create(_particle_x1);
	particle->setPosition(x - nXSize, y + nYSize);
	particle->setScale(nScale);
	pLayer->addChild(particle, 498);

	auto action = Sequence::create(DelayTime::create(0.6f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
		NULL);
	particle->runAction(action);

	auto particle2 = ParticleSystemQuad::create(_particle_x2);
	particle2->setPosition(x + nXSize, y + nYSize);
	particle2->setScale(nScale);
	pLayer->addChild(particle2, 498);
	auto action2 = Sequence::create(DelayTime::create(0.6f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
		NULL);
	particle2->runAction(action2);

	auto particle3 = ParticleSystemQuad::create(_particle_x3);
	particle3->setPosition(x - nXSize, y - nYSize);
	particle3->setScale(nScale);
	pLayer->addChild(particle3, 498);
	auto action3 = Sequence::create(DelayTime::create(0.6f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
		NULL);
	particle3->runAction(action3);

	auto particle4 = ParticleSystemQuad::create(_particle_x4);
	particle4->setPosition(x + nXSize, y - nYSize);
	particle4->setScale(nScale);
	pLayer->addChild(particle4, 498);
	auto action4 = Sequence::create(DelayTime::create(0.6f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
		NULL);
	particle4->runAction(action4);


	if (bSound == true)
	{
		Sound(_sounds_Fire);
	}
}

void PlayFunc::Particle_Steam(Vec2 Pos, float nScale, bool bSound, bool bDelay)
{

	if (bDelay == true)
	{
		if (nParticleTick < 0)
		{
			return;
		}
		else
		{
			nParticleTick = -6;
		}
	}

	float x = Pos.x;
	float y = Pos.y;

	auto particle = ParticleSystemQuad::create(_particle_steam);
	particle->setScale(nScale);
	particle->setPosition(x, y);
	pLayer->addChild(particle, 498);

	auto action = Sequence::create(DelayTime::create(0.5f),
		CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
		NULL);
	particle->runAction(action);


	if (bSound == true)
	{
		Sound(_sounds_Steam);
	}
}


void PlayFunc::Sound(char arr[20], bool bSoundDelay)
{
	if (mdp.bSoundSwitch == false)
		return;

	if (bSoundDelay == true)
	{
		if (nSoundTick < 0)
		{
			return;
		}
		else
		{
			nSoundTick = -6;
		}
	}


	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(arr);

}


/////////////////////////////////////////////////////////////////////////////////////////////////
void PlayFunc::ReadyAd()
{
	if (bReadyAd == false)
	{
		bReadyAd = true;
#ifdef PLUGIN_AD_MOB
		sdkbox::PluginAdMob::setListener(this);
#endif
	}
}

void PlayFunc::ReadyReward()
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

void PlayFunc::ShowReward()
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