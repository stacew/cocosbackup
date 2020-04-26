#include "TwoPlayFunc.h"
#include "SimpleAudioEngine.h"
#include "time.h"
#include "TwoPlayScene.h"
#include "../Util/MyDataManager.h"


void TwoPlayFunc::init(Layer* a_pLayer)
{
	bReadyAd = false;
	ReadyAd();

	srand(time(NULL));
	CCRANDOM_0_1();

	pLayer = a_pLayer;
	director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	gameSize = Size(visibleSize.width, visibleSize.height - AD_HEIGHT_SIZE);

	origin = Vec2(0, 100);
	playSize.x = 600.0f;
	playSize.y = 800.0f;
	center = Vec2(playSize.x * 0.5f, origin.y + playSize.y * 0.5f);

	//////////////////////////////////////////////
	bExitScene = false;
	//////////////////////////////////////////////
	nGameNumber = mdInfo.nGameNumber;
	bExtreme = mdp.bExtreme;
	//////////////////////////////////////////////////////////////////////
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
			ScaleTo::create(0.8f, 7.0f),
			CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::Callback, this, GAME_BG_CIRCLE_HIDE)),
			NULL);
		bgCircle->runAction(bgAction);
	}

	//initBG()
	{
		auto bgAd = Sprite::createWithSpriteFrameName("bgPlayAd.png");
		bgAd->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		bgAd->setPosition(visibleSize.width *0.5, visibleSize.height);
		pLayer->addChild(bgAd, 10000);

		auto bg = Sprite::createWithSpriteFrameName("bgPlay2.png");
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
		pLayer->addChild(bg, 0);
	}
	////////////

	//initSideMenu()
	{
		auto exit = Sprite::createWithSpriteFrameName("exit.png");
		auto exit_s = Sprite::createWithSpriteFrameName("exit_s.png");
		auto exitItem = MenuItemSprite::create(exit, exit_s, CC_CALLBACK_1(TwoPlayFunc::SideMenu, this));
		exitItem->setScale(SIDE_SCALE);
		exitItem->setTag(GAME_SIDE_EXIT);

		auto replay = Sprite::createWithSpriteFrameName("replay.png");
		auto replay_s = Sprite::createWithSpriteFrameName("replay_s.png");
		auto replayItem = MenuItemSprite::create(replay, replay_s, CC_CALLBACK_1(TwoPlayFunc::SideMenu, this));
		replayItem->setScale(SIDE_SCALE);
		replayItem->setTag(GAME_SIDE_REPLAY);

		auto menuSideUp = Menu::create(exitItem, replayItem, NULL);
		menuSideUp->setTag(GAME_SIDE_MENU_UP);
		menuSideUp->setPosition(gameSize.width * 0.5f, gameSize.height * MENU_SIDE_UP_HEIGHT);
		menuSideUp->alignItemsHorizontallyWithPadding(400);
		pLayer->addChild(menuSideUp, 9999);
	}

	//InitProgress1()
	{
		//Bar BG
		auto BarBG1 = Sprite::createWithSpriteFrameName("EmptyBar.png");
		BarBG1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		BarBG1->setPosition(Vec2(gameSize.width * 0.5, 0));
		pLayer->addChild(BarBG1, 400);

		//Life Bar Progress
		auto spriteLifeBar1 = Sprite::createWithSpriteFrameName("BlueBar.png");
		auto progressLifeTimer1 = ProgressTimer::create(spriteLifeBar1);	// ProgressTimer생성
		progressLifeTimer1->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
		progressLifeTimer1->setMidpoint(Vec2(0, 0));         // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
		progressLifeTimer1->setBarChangeRate(Vec2(1, 0));    // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
		progressLifeTimer1->setAnchorPoint(BarBG1->getAnchorPoint());
		progressLifeTimer1->setPosition(BarBG1->getPosition());
		progressLifeTimer1->setTag(PLAYER_LIFE_BAR);
		progressLifeTimer1->setPercentage(0.0f);
		pLayer->addChild(progressLifeTimer1, 401);

		//EdgeBar
		auto UpBar = Sprite::createWithSpriteFrameName("EdgeBar.png");
		UpBar->setAnchorPoint(BarBG1->getAnchorPoint());
		UpBar->setPosition(BarBG1->getPosition());
		pLayer->addChild(UpBar, 403);

		//CrashBar
		auto CrashBar = Sprite::createWithSpriteFrameName("CrashBar.png");
		CrashBar->setAnchorPoint(BarBG1->getAnchorPoint());
		CrashBar->setPosition(BarBG1->getPosition());
		CrashBar->setTag(CRASH_BAR);
		pLayer->addChild(CrashBar, 404);
		CrashBar->setVisible(false);
	}

	//InitProgress2()
	{
		//Bar BG
		auto BarBG1 = Sprite::createWithSpriteFrameName("EmptyBar.png");
		BarBG1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		BarBG1->setPosition(Vec2(gameSize.width * 0.5, gameSize.height));
		pLayer->addChild(BarBG1, 400);

		//RED Bar Progress
		auto spriteLifeBar2 = Sprite::createWithSpriteFrameName("YellowBar.png");
		auto progressTimer2 = ProgressTimer::create(spriteLifeBar2);	// ProgressTimer생성
		progressTimer2->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
		progressTimer2->setMidpoint(Vec2(1, 0));         // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
		progressTimer2->setBarChangeRate(Vec2(1, 0));    // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
		progressTimer2->setAnchorPoint(BarBG1->getAnchorPoint());
		progressTimer2->setPosition(BarBG1->getPosition());
		progressTimer2->setTag(PLAYER_LIFE_BAR2);
		progressTimer2->setPercentage(0.0f);
		pLayer->addChild(progressTimer2, 401);

		//EdgeBar
		auto UpBar = Sprite::createWithSpriteFrameName("EdgeBar.png");
		UpBar->setAnchorPoint(BarBG1->getAnchorPoint());
		UpBar->setPosition(BarBG1->getPosition());
		pLayer->addChild(UpBar, 403);

		//CrashBar
		auto CrashBar2 = Sprite::createWithSpriteFrameName("CrashBar.png");
		CrashBar2->setAnchorPoint(BarBG1->getAnchorPoint());
		CrashBar2->setPosition(BarBG1->getPosition());
		CrashBar2->setTag(CRASH_BAR2);
		pLayer->addChild(CrashBar2, 404);
		CrashBar2->setVisible(false);
	}


	StartTwoGame();
}




void TwoPlayFunc::ResetSprite(Ref* sender)
{
	auto spr = (BlockSprite*)sender;
	spr->setVisible(false);

}


void TwoPlayFunc::DeleteSprite(Ref* sender)
{

	auto spr = (BlockSprite*)sender;
	pLayer->removeChild(spr);
}


void TwoPlayFunc::ResetParticle(Ref* sender)
{
	auto particle = (ParticleSystemQuad*)sender;
	pLayer->removeChild(particle);

}

BlockSprite* TwoPlayFunc::getInvisibleBlock(int nType, int nOrder)
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


void TwoPlayFunc::SettingStage()
{

	switch (nGameNumber + 1)
	{
	case 3:
	{
		auto blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("a_blue_c.png");
			blue->setTag(PLAYER_BLUE);
			pLayer->addChild(blue, 2);
		}
		blue->setVisible(true);
		blue->setPosition(playSize.x * 0.15f, playSize.y * 0.5f + origin.y);

		auto yellow = (Sprite*)pLayer->getChildByTag(PLAYER_YELLOW);
		if (yellow == NULL)
		{
			yellow = Sprite::createWithSpriteFrameName("a_yellow_c.png");
			yellow->setTag(PLAYER_YELLOW);
			pLayer->addChild(yellow, 2);
		}
		yellow->setVisible(true);
		yellow->setPosition(playSize.x * 0.15f, playSize.y * 0.5f + origin.y);
	}
	break;

	case 7:
	{
		nLineCount = 8;
		nMaxArise = 12;
		if (bExtreme)
		{
			nLineCount = 8;
			nMaxArise = 6;
		}

		auto blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("a_blue_c.png");
			blue->setTag(PLAYER_BLUE);
			pLayer->addChild(blue, 2);
		}
		blue->setVisible(true);
		blue->setPosition(playSize.x * 0.5f, playSize.y * 0.5f + origin.y);

		auto yellow = (Sprite*)pLayer->getChildByTag(PLAYER_YELLOW);
		if (yellow == NULL)
		{
			yellow = Sprite::createWithSpriteFrameName("a_yellow_c.png");
			yellow->setTag(PLAYER_YELLOW);
			pLayer->addChild(yellow, 2);
		}
		yellow->setVisible(true);
		yellow->setPosition(playSize.x * 0.5f, playSize.y * 0.5f + origin.y);

	}
	break;

	case 4:
	{
		bCircle = false;
		nMaxClickCount = 120;
		nCurrentLimitCount = nMaxClickCount * 0.2f + nMaxClickCount * CCRANDOM_0_1();
		nCurrentClickCount = 0;

		bTouch1 = false;
		bTouch2 = false;
	}
	break;

	case 8:
	{
		auto blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("a_blue_c.png");
			blue->setTag(PLAYER_BLUE);
			pLayer->addChild(blue, 0);
		}
		blue->setVisible(true);
		blue->setPosition(playSize.x * 0.5f, origin.y + 25);
		prevTouchPoint = blue->getPosition();

		auto yellow = (Sprite*)pLayer->getChildByTag(PLAYER_YELLOW);
		if (yellow == NULL)
		{
			yellow = Sprite::createWithSpriteFrameName("a_yellow_c.png");
			yellow->setTag(PLAYER_YELLOW);
			pLayer->addChild(yellow, 0);
		}
		yellow->setVisible(true);
		yellow->setPosition(playSize.x * 0.5f, origin.y + playSize.y - 25);
		prevTouchPoint2 = yellow->getPosition();


		Sprite* bgRedEdge = (Sprite*)pLayer->getChildByTag(BG_RED_EDGE);
		if (bgRedEdge == NULL)
		{
			bgRedEdge = Sprite::createWithSpriteFrameName("bgPlayRed.png");
			bgRedEdge->setPosition(playSize.x * 0.5f, origin.y - playSize.y * 0.48f);
			bgRedEdge->setTag(BG_RED_EDGE);
			pLayer->addChild(bgRedEdge, 100);
		}

		Sprite* bgRedEdge2 = (Sprite*)pLayer->getChildByTag(BG_RED_EDGE2);
		if (bgRedEdge2 == NULL)
		{
			bgRedEdge2 = Sprite::createWithSpriteFrameName("bgPlayRed.png");
			bgRedEdge2->setPosition(playSize.x * 0.5f, origin.y + playSize.y + playSize.y * 0.48f);
			bgRedEdge2->setTag(BG_RED_EDGE2);
			pLayer->addChild(bgRedEdge2, 100);
		}


	}
	break;

	case 14:
	{
		nMentalCalc_PrevA = 0;
		nPlayerTurn = 0;
		Set_MentalCalc();
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


}

void TwoPlayFunc::Set_MentalCalc()
{
	auto menuMentalCalc = (Menu*)pLayer->getChildByTag(ETC1);
	if (menuMentalCalc == NULL)
	{
		auto blue_o = Sprite::createWithSpriteFrameName("blue_o.png");
		auto blue_x = Sprite::createWithSpriteFrameName("blue_x.png");
		auto blue_o_s = Sprite::createWithSpriteFrameName("blue_o_s.png");
		auto blue_x_s = Sprite::createWithSpriteFrameName("blue_x_s.png");
		auto blue_o_Item = MenuItemSprite::create(blue_o, blue_o_s, CC_CALLBACK_1(TwoPlayFunc::MentalCalcMenu, this));
		auto blue_x_Item = MenuItemSprite::create(blue_x, blue_x_s, CC_CALLBACK_1(TwoPlayFunc::MentalCalcMenu, this));

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

	auto menuMentalCalc2 = (Menu*)pLayer->getChildByTag(ETC2);
	if (menuMentalCalc2 == NULL)
	{
		auto yellow_o = Sprite::createWithSpriteFrameName("yellow_o.png");
		auto yellow_x = Sprite::createWithSpriteFrameName("yellow_x.png");
		auto yellow_o_s = Sprite::createWithSpriteFrameName("yellow_o_s.png");
		auto yellow_x_s = Sprite::createWithSpriteFrameName("yellow_x_s.png");
		auto yellow_o_Item = MenuItemSprite::create(yellow_o, yellow_o_s, CC_CALLBACK_1(TwoPlayFunc::MentalCalcMenu, this));
		auto yellow_x_Item = MenuItemSprite::create(yellow_x, yellow_x_s, CC_CALLBACK_1(TwoPlayFunc::MentalCalcMenu, this));

		yellow_o_Item->setScale(SIDE_SCALE);
		yellow_x_Item->setScale(SIDE_SCALE);
		yellow_o_Item->setTag(ETC2 + 1);
		yellow_x_Item->setTag(ETC2 + 2);

		menuMentalCalc2 = Menu::create(yellow_x_Item, yellow_o_Item, NULL);
		menuMentalCalc2->setTag(ETC2);
		menuMentalCalc2->setPosition(playSize.x * 0.5f, origin.y + playSize.y * 0.8f);
		menuMentalCalc2->alignItemsHorizontallyWithPadding(200);
		pLayer->addChild(menuMentalCalc2, 100);
	}

	auto labelQuiz = (Label*)pLayer->getChildByTag(ETC3);
	if (labelQuiz == NULL)
	{
		labelQuiz = Label::create();// createWithTTF("", _fonts_Base, 90);
		labelQuiz->setSystemFontSize(90);
		labelQuiz->setTag(ETC3);
		labelQuiz->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelQuiz->setTextColor(Color4B(20, 20, 20, 255));
		labelQuiz->enableOutline(Color4B::WHITE, 2);
		labelQuiz->setPosition(playSize.x * 0.5f, origin.y + playSize.y * 0.5f);
		pLayer->addChild(labelQuiz, 100);
	}

	if (nPlayerTurn % 2 == 0)
	{
		menuMentalCalc->setVisible(true);
		menuMentalCalc2->setVisible(false);
		labelQuiz->setRotation(0.0f);
	}
	else
	{
		menuMentalCalc->setVisible(false);
		menuMentalCalc2->setVisible(true);
		labelQuiz->setRotation(180.0f);
	}

	int nDifficulty = 9;

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

	labelQuiz->setString(StringUtils::format("%d X %d = %d", a, b, c));
}

void TwoPlayFunc::MentalCalcMenu(Ref* pSender)
{
	if (p1.GetLife() <= 0 || p2.GetLife() <= 0)
		return;

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

	case ETC2 + 1: // yellow O
	{
		if (bMentalCalc_Answer == true)
		{
			bAnswer = true;
		}

	}
	break;

	case ETC2 + 2: // yellow X
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
		nPlayerTurn++;
		Set_MentalCalc();
		Particle_O(0, Vec2(gameSize.width *0.5f, gameSize.height*0.5f), 1.0f);
	}
	else
	{
		if (nPlayerTurn % 2 == 0)
			ChangeLife(0, -20);
		else
			ChangeLife(1, -20);

		Particle_X(0, Vec2(gameSize.width *0.5f, gameSize.height*0.5f), 1.0f);
	}

}

void TwoPlayFunc::Particle_Steam(int nPlayer, Vec2 Pos, float nScale, bool bSound, bool bDelay)
{
	if (bDelay == true)
	{
		if (nPlayer == 0)
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
		else
		{
			if (nParticleTick2 < 0)
			{
				return;
			}
			else
			{
				nParticleTick2 = -6;
			}
		}
	}

	float x = Pos.x;
	float y = Pos.y;

	auto particle = ParticleSystemQuad::create(_particle_steam2);
	particle->setScale(nScale);
	particle->setPosition(x, y);
	pLayer->addChild(particle, 498);

	auto action = Sequence::create(DelayTime::create(0.5f),
		CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::ResetParticle, this)),
		NULL);
	particle->runAction(action);


	if (bSound == true)
	{
		Sound(nPlayer, _sounds_Steam);
	}
}

void TwoPlayFunc::Particle_Heal(int nPlayer, Vec2 Pos, float nScale, bool bSound, bool bDelay)
{
	if (bDelay == true)
	{
		if (nPlayer == 0)
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
		else
		{
			if (nParticleTick2 < 0)
			{
				return;
			}
			else
			{
				nParticleTick2 = -6;
			}
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
	particle->setPosition(x,y);
	particle->setScale(nScale);
	pLayer->addChild(particle, 498);

	int nLifeBarTag = PLAYER_LIFE_BAR;
	if (nPlayer == 1)
		nLifeBarTag = PLAYER_LIFE_BAR2;

	auto lifeBar = (Sprite*)pLayer->getChildByTag(nLifeBarTag);
	if (lifeBar)
	{
		float toY = lifeBar->getPosition().y + lifeBar->getContentSize().height / 2;
		if (nPlayer == 1)
			toY = lifeBar->getPosition().y - lifeBar->getContentSize().height / 2;
		float toX = playSize.x * p1.GetLifePercent() / 100;
		if (nPlayer == 1)
			toX = playSize.x - (playSize.x * p2.GetLifePercent() / 100);

		auto action = Sequence::create(MoveTo::create(0.2f, Vec2(toX, toY)),
			DelayTime::create(0.7f),
			CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::ResetParticle, this)),
			NULL);

		particle->runAction(action);
	}





	if (bSound == true)
	{
		Sound(nPlayer, _sounds_Bubble);
	}
}

void TwoPlayFunc::Particle_O(int nPlayer, Vec2 Pos, float nScale, bool bSound, bool bDelay)
{
	if (bDelay == true)
	{
		if (nPlayer == 0)
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
		else
		{
			if (nParticleTick2 < 0)
			{
				return;
			}
			else
			{
				nParticleTick2 = -6;
			}
		}
	}


	float x = Pos.x;
	float y = Pos.y;

	auto particle = ParticleSystemQuad::create(_particle_o);
	particle->setPosition(x, y);
	particle->setScale(nScale);
	pLayer->addChild(particle, 498);

	auto action = Sequence::create(DelayTime::create(1.0f),
		CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::ResetParticle, this)),
		NULL);

	particle->runAction(action);


	if (bSound == true)
	{
		Sound(nPlayer, _sounds_Touch);
	}
}

void TwoPlayFunc::Particle_X(int nPlayer, Vec2 Pos, float nScale, bool bSound, bool bDelay)
{
	if (bDelay == true)
	{
		if (nPlayer == 0)
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
		else
		{
			if (nParticleTick2 < 0)
			{
				return;
			}
			else
			{
				nParticleTick2 = -6;
			}
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
		CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::ResetParticle, this)),
		NULL);
	particle->runAction(action);

	auto particle2 = ParticleSystemQuad::create(_particle_x2);
	particle2->setPosition(x + nXSize, y + nYSize);
	particle2->setScale(nScale);
	pLayer->addChild(particle2, 498);
	auto action2 = Sequence::create(DelayTime::create(0.6f),
		CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::ResetParticle, this)),
		NULL);
	particle2->runAction(action2);

	auto particle3 = ParticleSystemQuad::create(_particle_x3);
	particle3->setPosition(x - nXSize, y - nYSize);
	particle3->setScale(nScale);
	pLayer->addChild(particle3, 498);
	auto action3 = Sequence::create(DelayTime::create(0.6f),
		CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::ResetParticle, this)),
		NULL);
	particle3->runAction(action3);

	auto particle4 = ParticleSystemQuad::create(_particle_x4);
	particle4->setPosition(x + nXSize, y - nYSize);
	particle4->setScale(nScale);
	pLayer->addChild(particle4, 498);
	auto action4 = Sequence::create(DelayTime::create(0.6f),
		CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::ResetParticle, this)),
		NULL);
	particle4->runAction(action4);


	if (bSound == true)
	{
		Sound(nPlayer, _sounds_Fire);
	}
}

void TwoPlayFunc::UpdateStage()
{
	switch (nGameNumber + 1)
	{

	case 7:
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			return;
		}
		Sprite* yellow = (Sprite*)pLayer->getChildByTag(PLAYER_YELLOW);
		if (yellow == NULL)
		{
			return;
		}
		float nLimitUp = playSize.y + origin.y;
		float nLimitDown = origin.y;

		// 1p blue 이동
		{
			float toY = blue->getPositionY() + p1.nSpeedY;
			blue->setPositionY(toY);
			float toX = blue->getPositionX() + p1.nSpeedX;
			blue->setPositionX(toX);

			p1.nSpeedX = p1.nSpeedX - p1.nSpeedX * 0.001f;
			p1.nSpeedY -= G_ACCEL;

			Rect rectPlayer = blue->getBoundingBox();
			if (rectPlayer.getMaxY() > nLimitUp)
			{
				if (p1.nSpeedY > 0)
				{
					ChangeLife(0, -5);
					Particle_Steam(0, blue->getPosition(), blue->getScale() * 4, true, true);

					p1.nSpeedY = p1.nSpeedY * -EDGE_ELASTIC;
				}

			}
			else if (rectPlayer.getMinY() < nLimitDown)
			{
				if (p1.nSpeedY < 0)
				{
					ChangeLife(0, -5);
					Particle_Steam(0, blue->getPosition(), blue->getScale() * 4, true, true);

					p1.nSpeedY = p1.nSpeedY * -EDGE_ELASTIC;
				}
			}

			if (rectPlayer.getMaxX() > playSize.x)
			{
				if (p1.nSpeedX > 0)
				{
					ChangeLife(0, -5);
					Particle_Steam(0, blue->getPosition(), blue->getScale() * 4, true, true);

					p1.nSpeedX = p1.nSpeedX * -EDGE_ELASTIC;
				}
			}
			else if (rectPlayer.getMinX() < 0)
			{
				if (p1.nSpeedX < 0)
				{
					ChangeLife(0, -5);
					Particle_Steam(0, blue->getPosition(), blue->getScale() * 4, true, true);

					p1.nSpeedX = p1.nSpeedX * -EDGE_ELASTIC;
				}
			}
		}

		//2p yellow 이동
		{
			float toY = yellow->getPositionY() - p2.nSpeedY;
			yellow->setPositionY(toY);
			float toX = yellow->getPositionX() + p2.nSpeedX;
			yellow->setPositionX(toX);

			p2.nSpeedX = p2.nSpeedX - p2.nSpeedX * 0.001f;
			p2.nSpeedY -= G_ACCEL;

			Rect rectPlayer = yellow->getBoundingBox();
			if (rectPlayer.getMaxY() > nLimitUp)
			{
				if (p2.nSpeedY < 0)
				{
					ChangeLife(1, -5);
					Particle_Steam(1, yellow->getPosition(), yellow->getScale() * 4, true, true);

					p2.nSpeedY = p2.nSpeedY * -EDGE_ELASTIC;
				}
			}
			else if (rectPlayer.getMinY() < nLimitDown)
			{

				if (p2.nSpeedY > 0)
				{
					ChangeLife(1, -5);
					Particle_Steam(1, yellow->getPosition(), yellow->getScale() * 4, true, true);

					p2.nSpeedY = p2.nSpeedY * -EDGE_ELASTIC;
				}
			}

			if (rectPlayer.getMaxX() > playSize.x)
			{
				if (p2.nSpeedX > 0)
				{
					ChangeLife(1, -5);
					Particle_Steam(1, yellow->getPosition(), yellow->getScale() * 4, true, true);

					p2.nSpeedX = p2.nSpeedX * -EDGE_ELASTIC;
				}
			}
			else if (rectPlayer.getMinX() < 0)
			{
				if (p2.nSpeedX < 0)
				{
					ChangeLife(1, -5);
					Particle_Steam(1, yellow->getPosition(), yellow->getScale() * 4, true, true);

					p2.nSpeedX = p2.nSpeedX * -EDGE_ELASTIC;
				}
			}
		}

		//충돌 확인 - 상자 먹기
		for (int i = 0; i < 2; i++)
		{
			TwoPlayer* player = NULL;
			Sprite* sprPlayer = NULL;
			if (i == 0)
			{
				player = &p1;
				sprPlayer = blue;
			}
			else
			{
				player = &p2;
				sprPlayer = yellow;
			}


			Rect rectPlayer = sprPlayer->getBoundingBox();
			int nType = GREEN_SQUARE;
			for (BlockSprite* pBlock : vBlock[nType])
			{
				if (pBlock == NULL || pBlock->isVisible() == false)
					continue;

				Rect rectBlock = pBlock->getBoundingBox();
				float nRadius = rectPlayer.getMidX() - rectPlayer.getMinX();
				if (rectBlock.intersectsCircle(sprPlayer->getPosition(), nRadius * 0.9f))
				{
					pBlock->stopAllActions();
					pBlock->setVisible(false);

					ChangeLife(i, 5);
					Particle_Heal(i, sprPlayer->getPosition(), sprPlayer->getScale() * 4, true, true);
				}
			}
		}


		//충돌 확인2 - 플레이어간
		if (blue->getBoundingBox().intersectsRect(yellow->getBoundingBox()))
		{
			ChangeLife(0, -5);
			ChangeLife(1, -5);
			Vec2 nPos = blue->getPosition() + yellow->getPosition();
			nPos = nPos / 2;
		
			Particle_Steam(0, nPos, 0.5f, false, true);
			Sound(0, _sounds_Sword, true);
		}



		//크기 조절
		for (int i = 0; i < 2; i++)
		{
			TwoPlayer* player = NULL;
			Sprite* sprPlayer = NULL;
			if (i == 0)
			{
				player = &p1;
				sprPlayer = blue;
			}
			else
			{
				player = &p2;
				sprPlayer = yellow;
			}


			if (player->GetLife() == 0)
			{
				sprPlayer->setVisible(false);
			}
			else
			{
				sprPlayer->setVisible(true);
				//사이즈 조절
				sprPlayer->setScale(PLAYER_BLUE_BASE_SCALE + player->GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 2);
			}
		}

		// 장애물 생성 - return 때문에 맨 마지막에
		{
			int nVisibleCount = 0;
			int nType = GREEN_SQUARE;
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
			float posX = Pos.x;
			float posY = Pos.y;

			float nCreateDistance = 100;

			if (blue->getPositionX() - nCreateDistance < posX && posX < blue->getPositionX() + nCreateDistance
				&& blue->getPositionY() - nCreateDistance < posY && posY < blue->getPositionY() + nCreateDistance)
				return;

			if (yellow->getPositionX() - nCreateDistance < posX && posX < yellow->getPositionX() + nCreateDistance
				&& yellow->getPositionY() - nCreateDistance < posY && posY < yellow->getPositionY() + nCreateDistance)
				return;

			for (int i = 0; i < vBlock[nType].size(); i++)
			{
				BlockSprite* block = vBlock[nType].at(i);
				if (block->getPosition() == Pos && block->isVisible() == true)
					return;
			}

			//block 세팅.
			BlockSprite* block = getInvisibleBlock(nType);
			block->setScale(nLineBlockScale);
			block->setPosition(Pos);
		}

		


	}
	break;

	case 3:
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			return;
		}
		Sprite* yellow = (Sprite*)pLayer->getChildByTag(PLAYER_YELLOW);
		if (yellow == NULL)
		{
			return;
		}
		float nLimitUp = playSize.y + origin.y;
		float nLimitDown = origin.y;

		// 1p blue 이동
		{
			float toY = blue->getPositionY() + p1.nSpeedY;
			blue->setPositionY(toY);

			p1.nSpeedY -= G_ACCEL;

			Rect rectBlue = blue->getBoundingBox();
			if (rectBlue.getMaxY() > nLimitUp)
			{
				if (p1.nSpeedY > 0)
				{
					if (p1.nSpeedY > 5)
						Sound(0, _sounds_Tong, true);

					p1.nSpeedY = p1.nSpeedY * -EDGE_ELASTIC;
				}

			}
			else if (rectBlue.getMinY() < nLimitDown)
			{
				if (p1.nSpeedY < 0)
				{
					if (p1.nSpeedY < -5)
						Sound(0, _sounds_Tong, true);

					p1.nSpeedY = p1.nSpeedY * -EDGE_ELASTIC;
				}
			}
		}

		// 2p Yellow 이동
		{
			float toY = yellow->getPositionY() - p2.nSpeedY;
			yellow->setPositionY(toY);

			p2.nSpeedY -= G_ACCEL;

			Rect rectBlue = yellow->getBoundingBox();
			if (rectBlue.getMaxY() > nLimitUp)
			{
				if (p2.nSpeedY < 0)
				{
					if (p2.nSpeedY < -5)
						Sound(1, _sounds_Tong, true);

					p2.nSpeedY = p2.nSpeedY * -EDGE_ELASTIC;
				}

			}
			else if (rectBlue.getMinY() < nLimitDown)
			{
				if (p2.nSpeedY > 0)
				{
					if (p2.nSpeedY > 5)
						Sound(1, _sounds_Tong, true);

					p2.nSpeedY = p2.nSpeedY * -EDGE_ELASTIC;
				}
			}

		}



		// 장애물 생성
		int nTick = 100;
		int nReduce = 10;
		if (bExtreme)
		{
			nReduce = nReduce * 3.0f;
		}

		nTick = nTick - nReduce;
		if (nMainTick % nTick == 0)
		{
			float nScaleX = 0.6f;

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
		}


		//장애물 이동
		int nType = RED_SQUARE;
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


		// 충돌 확인.
		for (int i = 0; i < 2; i++)
		{
			TwoPlayer* player = NULL;
			Sprite* sprPlayer = NULL;
			if (i == 0)
			{
				player = &p1;
				sprPlayer = blue;
			}
			else
			{
				player = &p2;
				sprPlayer = yellow;
			}


			int nType = RED_SQUARE;

			Rect rectPlayer = sprPlayer->getBoundingBox();
			for (BlockSprite* pBlock : vBlock[nType])
			{
				if (pBlock == NULL || pBlock->isVisible() == false)
					continue;

				Rect rectBlock = pBlock->getBoundingBox();
				float nRadius = rectPlayer.getMidX() - rectPlayer.getMinX();
				if (rectBlock.intersectsCircle(sprPlayer->getPosition(), nRadius * 0.9f))
				{

					float nDamage = -1.0f;
					if (bExtreme)
						nDamage = -5.0f;

					ChangeLife(i , nDamage);

					Particle_Steam(i, sprPlayer->getPosition(), sprPlayer->getScale() * 4, true, true);

				}
			}

		}


		for (int i = 0; i < 2; i++)
		{
			TwoPlayer* player = NULL;
			Sprite* sprPlayer = NULL;
			if (i == 0)
			{
				player = &p1;
				sprPlayer = blue;
			}
			else
			{
				player = &p2;
				sprPlayer = yellow;
			}


			if (player->GetLife() == 0)
			{
				sprPlayer->setVisible(false);
			}
			else
			{
				sprPlayer->setVisible(true);
				//사이즈 조절
				sprPlayer->setScale(PLAYER_BLUE_BASE_SCALE + player->GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 2);
			}
		}


	}
	break;



	case 4:
	{
		auto green_o = (Sprite*)pLayer->getChildByTag(ETC1);
		auto red_x = (Sprite*)pLayer->getChildByTag(ETC2);
		if (green_o == NULL)
		{
			green_o = Sprite::createWithSpriteFrameName("green_o.png");
			green_o->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			green_o->setPosition(center);
			green_o->setScale(1.5f);
			green_o->setTag(ETC1);
			pLayer->addChild(green_o, 1);
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
			green_o->setVisible(true);
			red_x->setVisible(false);
		}
		else
		{
			green_o->setVisible(false);
			red_x->setVisible(true);
		}


		if (bTouch1 == true)
		{
			if (bCircle)
			{
				ChangeLife(0, 1, true);
				Particle_Heal(0, prevTouchPoint, POINT_TYPE3, true, true);
			}
			else
			{
				ChangeLife(0, -5);
				Particle_X(0, prevTouchPoint, 0.7f, true, true);
			}
		}



		if (bTouch2 == true)
		{
			if (bCircle)
			{
				ChangeLife(1, 1, true);
				Particle_Heal(1, prevTouchPoint2, POINT_TYPE3, true, true);
			}
			else
			{
				ChangeLife(1, -5);
				Particle_X(1, prevTouchPoint2, 0.7f, true, true);
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



	case 8:
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(PLAYER_BLUE);
		if (blue == NULL)
		{
			return;
		}
		Sprite* yellow = (Sprite*)pLayer->getChildByTag(PLAYER_YELLOW);
		if (yellow == NULL)
		{
			return;
		}
		float nBlueX = blue->getPositionX();
		float nBlueY = blue->getPositionY();
		float nYellowX = yellow->getPositionX();
		float nYellowY = yellow->getPositionY();

		blue->setPositionX(prevTouchPoint.x - (prevTouchPoint.x - nBlueX) / 2);
		blue->setPositionY(prevTouchPoint.y - (prevTouchPoint.y - nBlueY) / 2);
		yellow->setPositionX(prevTouchPoint2.x - (prevTouchPoint2.x - nYellowX) / 2);
		yellow->setPositionY(prevTouchPoint2.y - (prevTouchPoint2.y - nYellowY) / 2);

		//볼생성
		int nTime = 8;
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
			red->setPosition(playSize.x * 0.5f, origin.y + playSize.y * 0.5f);
			red->nUDSpeed = 0;
			red->nLRSpeed = 0;
		}

		float nSpeed = 10 + nTimeDifficulty * 0.1f;

		//볼-1p2p충돌
		for (int i = 0; i < 2; i++)
		{
			TwoPlayer* player = NULL;
			Sprite* sprPlayer = NULL;
			if (i == 0)
			{
				player = &p1;
				sprPlayer = blue;
			}
			else
			{
				player = &p2;
				sprPlayer = yellow;
			}

			Rect rectPlayer = sprPlayer->getBoundingBox();
			for (int i = 0; i < nRedCount; i++)
			{
				BlockSprite* red = vBlock[RED_CIRCLE].at(i);
				if (red == NULL || red->isVisible() == false)
					continue;

				Rect rectBlock = red->getBoundingBox();
				float nRadius = rectPlayer.getMidX() - rectPlayer.getMinX();
				if (rectBlock.intersectsCircle(sprPlayer->getPosition(), nRadius * 0.9f))
				{
					float redX = red->getPositionX();
					float redY = red->getPositionY();

					float nDisX = redX - sprPlayer->getPositionX();
					if (nDisX >= 0 && nDisX < 1)
						nDisX = 1;
					else if (nDisX < 0 && nDisX > -1)
						nDisX = -1;

					float nDisY = redY - sprPlayer->getPositionY();
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

					Sound(i, _sounds_Tong, true);
				}
			}

		}


		//볼 이동 - 벽 충돌
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

			if (toY < origin.y) // 1p 마이너스
			{
				toY = origin.y;
				red->nUDSpeed *= -1.0f;
				{
					ChangeLife(0, -5);
					Particle_Steam(0, red->getPosition(), 0.5f);
				}
			}
			else if (toY > origin.y + playSize.y) // 2p 마이너스
			{
				toY = origin.y + playSize.y;
				red->nUDSpeed *= -1.0f;
				{
					ChangeLife(1, -5);
					Particle_Steam(1, red->getPosition(), 0.5f);
				}
			}

			red->setPosition(toX, toY);
		}


		// 1p - 2p 사이즈 조절
		for (int i = 0; i < 2; i++)
		{
			TwoPlayer* player = NULL;
			Sprite* sprPlayer = NULL;
			if (i == 0)
			{
				player = &p1;
				sprPlayer = blue;
			}
			else
			{
				player = &p2;
				sprPlayer = yellow;
			}

			if (player->GetLife() == 0)
			{
				sprPlayer->setVisible(false);
			}
			else
			{
				sprPlayer->setVisible(true);
				//사이즈 조절
				sprPlayer->setScale((PLAYER_BLUE_BASE_SCALE * 3 - player->GetLifePercent() / 100 * PLAYER_BLUE_BASE_SCALE * 1) * 3);
			}
		}
	}
	break;



	}
}

void TwoPlayFunc::ClearStage()
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


void TwoPlayFunc::UpdateProgress()
{
	auto progressLifeTimer1 = (ProgressTimer*)pLayer->getChildByTag(PLAYER_LIFE_BAR);
	if (progressLifeTimer1)
	{
		progressLifeTimer1->setPercentage(p1.GetLifePercent());
	}

	auto progressLifeTimer2 = (ProgressTimer*)pLayer->getChildByTag(PLAYER_LIFE_BAR2);
	if (progressLifeTimer2)
	{
		progressLifeTimer2->setPercentage(p2.GetLifePercent());
	}


}







void TwoPlayFunc::Callback(Ref* sender, int nType)
{
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

}



bool TwoPlayFunc::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (p1.GetLife() <= 0 || p2.GetLife() <= 0)
		return false;

	float y = touch->getLocation().y;

	switch (nGameNumber + 1)
	{
	case 3:
	{

		float nJumpPower = JUMP_POWER;

		if (y < gameSize.height *0.5)
		{
			if (p1.nSpeedY > nJumpPower)
				p1.nSpeedY += 0;
			else
				p1.nSpeedY = nJumpPower;
		}
		else
		{
			if (p2.nSpeedY > nJumpPower)
				p2.nSpeedY += 0;
			else
				p2.nSpeedY = nJumpPower;
		}

		Sound(0, _sounds_Tong);
	}
	break;

	case 7:
	{

		float nJumpPower = JUMP_POWER;
		if (y < gameSize.height *0.5)
		{
			if (p1.nSpeedY > nJumpPower)
				p1.nSpeedY += 0;
			else
				p1.nSpeedY = nJumpPower;

			if (p1.nSpeedX > 0)
				p1.nSpeedX = -nJumpPower / 2;
			else
				p1.nSpeedX = nJumpPower / 2;
		}
		else
		{
			if (p2.nSpeedY > nJumpPower)
				p2.nSpeedY += 0;
			else
				p2.nSpeedY = nJumpPower;

			if (p2.nSpeedX > 0)
				p2.nSpeedX = -nJumpPower / 2;
			else
				p2.nSpeedX = nJumpPower / 2;
		}

		Sound(0, _sounds_Tong);
	}
	break;

	case 4:
	{
		if (y < gameSize.height * 0.5)
		{
			startTouchPoint = prevTouchPoint = touch->getLocation();
			bTouch1 = true;
		}
		else
		{
			startTouchPoint2 = prevTouchPoint2 = touch->getLocation();
			bTouch2 = true;
		}
		return true;
	}
	break;

	case 8:
	{
		if (y < gameSize.height *0.5)
		{
			startTouchPoint = prevTouchPoint = touch->getLocation();
		}
		else
		{
			startTouchPoint2 = prevTouchPoint2 = touch->getLocation();
		}
		return true;
	}
	break;
	}


	return true;
}


void TwoPlayFunc::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (p1.GetLife() <= 0 || p2.GetLife() <= 0)
		return;

	bool bPlayer1 = false;
	bool bPlayer2 = false;

	if (touch->getStartLocation() == startTouchPoint)
		bPlayer1 = true;

	if (touch->getStartLocation() == startTouchPoint2)
		bPlayer2 = true;

	Point currentTouchPoint = touch->getLocation();
	switch (nGameNumber + 1)
	{

	case 8:
	{
		if (bPlayer1 == true)
		{
			prevTouchPoint = currentTouchPoint;
		}
		
		if( bPlayer2 == true)
		{
			prevTouchPoint2 = currentTouchPoint;
		}
	}
	break;

	}

}


void TwoPlayFunc::onTouchEnded(Touch *touch, Event *unused_event)
{
	bool bPlayer1 = false;
	bool bPlayer2 = false;

	if (touch->getStartLocation() == startTouchPoint)
		bPlayer1 = true;

	if (touch->getStartLocation() == startTouchPoint2)
		bPlayer2 = true;


	Point currentTouchPoint = touch->getLocation();
	switch (nGameNumber + 1)
	{

	case 4:
	{
		if (bPlayer1 == true)
		{
			bTouch1 = false;
		}

		if (bPlayer2 == true)
		{
			bTouch2 = false;
		}
	}
	break;

	}

}

void TwoPlayFunc::onTouchCancelled(Touch *touch, Event *unused_event)
{


}

void TwoPlayFunc::update()
{
	if (p1.GetLife() > 0 && p2.GetLife() > 0)
	{
		nMainTick++;
		nSoundTick++;
		nParticleTick++;
		nSoundTick2++;
		nParticleTick2++;

		nTimeDifficulty = nMainTick / (60 * 10); // 10초마다 난이도 조금씩 1씩 증가.


		UpdateStage();

		//1초당 지속적으로 깍이는 체력
		ChangeLifePerTime();
	}
}


void TwoPlayFunc::ChangeLifePerTime()
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
		nReduceHp = -2.5f;
		ChangeLife(0, nReduceHp);
		ChangeLife(1, nReduceHp);
	}
	break;

	case 14:
	{
		nReduceHp = -0.25f * (nPlayerTurn + 1);
		if (nPlayerTurn % 2 == 0)
		{
			ChangeLife(0, nReduceHp);
		}
		else
		{
			ChangeLife(1, nReduceHp);
		}
	}
	break;
	}

}

void TwoPlayFunc::ResetCrashBar(Ref* sender, int nPlayer)
{
	if (nPlayer == 0)
	{
		auto CrashBar = (Sprite*)pLayer->getChildByTag(CRASH_BAR);
		if (CrashBar)
			CrashBar->setVisible(false);
	}

	if (nPlayer == 1)
	{
		auto CrashBar2 = (Sprite*)pLayer->getChildByTag(CRASH_BAR2);
		if (CrashBar2)
			CrashBar2->setVisible(false);
	}
}

void TwoPlayFunc::CheckGameOver()
{
	if (p1.GetLife() > 0 && p2.GetLife() > 0)
	{
		return;
	}

	auto labelP1 = (Label*)pLayer->getChildByTag(LABEL_TWO_PLAYER_RESULT_P1);
	auto labelP2 = (Label*)pLayer->getChildByTag(LABEL_TWO_PLAYER_RESULT_P2);
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	Sound(0, _sounds_Sword);

	if (p1.GetLife() <= 0 && p2.GetLife() <= 0) //DRAW
	{
		labelP1->setString(mdp.bEnglish ? "DRAW" : UTF8PCHAR("무승부"));
		labelP2->setString(mdp.bEnglish ? "DRAW" : UTF8PCHAR("무승부"));
		labelP1->setSystemFontSize(100);
		labelP2->setSystemFontSize(100);
	}
	else if (p2.GetLife() <= 0) // p1 win
	{
		labelP1->setString(mdp.bEnglish ? "WIN" : UTF8PCHAR("승리"));
		labelP2->setString(mdp.bEnglish ? "LOSE" : UTF8PCHAR("패배"));
		labelP1->setSystemFontSize(150);
		labelP2->setSystemFontSize(50);
	}
	else if (p1.GetLife() <= 0) // p2 win
	{
		labelP1->setString(mdp.bEnglish ? "LOSE" : UTF8PCHAR("패배"));
		labelP2->setString(mdp.bEnglish ? "WIN" : UTF8PCHAR("승리"));
		labelP1->setSystemFontSize(50);
		labelP2->setSystemFontSize(150);
	}

}


void TwoPlayFunc::SideMenu(Ref* pSender)
{
	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();

	switch (nSelectedItem)
	{

	case GAME_SIDE_EXIT:
	{
		ExitGame();
	}
	break;


	case GAME_SIDE_REPLAY:
	{
		Sound(0, _sounds_Bubble);
		StartTwoGame();
	}
	break;



	}

}

void TwoPlayFunc::StartTwoGame()
{
	ClearStage();

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



	auto labelP1 = (Label*)pLayer->getChildByTag(LABEL_TWO_PLAYER_RESULT_P1);
	if (labelP1 == NULL)
	{
		labelP1 = Label::create();;// createWithTTF("", _fonts_Base, COMBO_FONT_SIZE);
		labelP1->setSystemFontSize(COMBO_FONT_SIZE);
		labelP1->setTag(LABEL_TWO_PLAYER_RESULT_P1);
		labelP1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelP1->setTextColor(Color4B::WHITE);
		labelP1->setPosition(gameSize.width*0.5, gameSize.height* 0.5f - 250);
		pLayer->addChild(labelP1, 499);
	}

	auto labelP2 = (Label*)pLayer->getChildByTag(LABEL_TWO_PLAYER_RESULT_P2);
	if (labelP2 == NULL)
	{
		labelP2 = Label::create();// createWithTTF("", _fonts_Base, COMBO_FONT_SIZE);
		labelP2->setSystemFontSize(COMBO_FONT_SIZE);
		labelP2->setTag(LABEL_TWO_PLAYER_RESULT_P2);
		labelP2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		labelP2->setTextColor(Color4B::WHITE);
		labelP2->setPosition(gameSize.width*0.5, gameSize.height* 0.5f + 250);
		labelP2->setRotation(180.0f);
		pLayer->addChild(labelP2, 499);
	}
	labelP1->setString("");
	labelP2->setString("");

	////////////////////////////////////////////////////////////////////////

	nMainTick = 0;
	nSoundTick = 0;
	nParticleTick = 0;
	nSoundTick2 = 0;
	nParticleTick2 = 0;
	nTimeDifficulty = 0;

	p1.Init(100);
	p2.Init(100);
	p1.__ChangeLife__(100, true);
	p2.__ChangeLife__(100, true);
	UpdateProgress();

	SettingStage();
}

void TwoPlayFunc::ExitGame()
{
	auto bgCircle = (Sprite*)pLayer->getChildByTag(GAME_BG_CIRCLE);
	bgCircle->setVisible(true);
	auto bgAction = Sequence::create(
		ScaleTo::create(0.7f, 1.0f),
		CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::Callback, this, GAME_BG_CIRCLE_EXIT)),
		NULL);
	bgCircle->runAction(bgAction);
}
void TwoPlayFunc::ChangeLife(int nPlayer, float nDamage, bool bPercent, bool bRedFlash)
{
	if (nPlayer == 0)
	{
		p1.__ChangeLife__(nDamage, bPercent);
	}

	else if (nPlayer == 1)
	{
		p2.__ChangeLife__(nDamage, bPercent);
	}

	if (nDamage < 0 && bRedFlash)
	{
		int nTag = (nPlayer == 0) ? CRASH_BAR : CRASH_BAR2;
		auto CrashBar = (Sprite*)pLayer->getChildByTag(nTag);
		CrashBar->setVisible(true);

		pLayer->runAction(
			Sequence::create(
				DelayTime::create(0.2f),
				CallFuncN::create(CC_CALLBACK_1(TwoPlayFunc::ResetCrashBar, this, nPlayer)),
				NULL));

	}


	//HP Bar변화 그리기.
	UpdateProgress();

	CheckGameOver();
}

void TwoPlayFunc::Sound(int nPlayer, char arr[20], bool bSoundDelay)
{
	if (mdp.bSoundSwitch == false)
		return;

	if (bSoundDelay == true)
	{
		if (nPlayer == 0)
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
		else
		{
			if (nSoundTick2 < 0)
			{
				return;
			}
			else
			{
				nSoundTick2 = -6;
			}
		}
	}

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(arr);
}




/////////////////////////////////////////////////////////////////////////////////////////////////
void TwoPlayFunc::ReadyAd()
{
	if (bReadyAd == false)
	{
		bReadyAd = true;
#ifdef PLUGIN_AD_MOB
		sdkbox::PluginAdMob::setListener(this);
#endif
	}
}

void TwoPlayFunc::ReadyVideo()
{
	if (bReadyAd == true)
	{
#ifdef PLUGIN_AD_MOB
		if (sdkbox::PluginAdMob::isAvailable(AD_MOB_INTERSTITIAL) == false)
		{
			sdkbox::PluginAdMob::cache(AD_MOB_INTERSTITIAL);
		}
#endif
	}
}

void TwoPlayFunc::ShowVideo()
{
	if (bReadyAd == true)
	{
#ifdef PLUGIN_AD_MOB
		if (sdkbox::PluginAdMob::isAvailable(AD_MOB_INTERSTITIAL) == true)
		{
			sdkbox::PluginAdMob::show(AD_MOB_INTERSTITIAL);
		}
#endif
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////