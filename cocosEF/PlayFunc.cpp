#include "PlayFunc.h"
#include "../Util/define.h"
#include "../Util/MyDataManager.h"
#include "SimpleAudioEngine.h"


////////////////////////////////////////////////

BlockSprite* BlockSprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
	BlockSprite *sprite = new (std::nothrow) BlockSprite();
	if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

BlockSprite* BlockSprite::createWithSpriteFrameName(const std::string& spriteFrameName)
{
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);

#if COCOS2D_DEBUG > 0
	char msg[256] = { 0 };
	sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
	CCASSERT(frame != nullptr, msg);
#endif

	return createWithSpriteFrame(frame);
}


void Player::init(int a_nDirection)
{
	if (a_nDirection > 0)//1p
	{
		nMaxLife = mdp.nClearLevel[mdInfo.nStageLv] + 1;
		nMaxPower = mdInfo.nMaxPower1;
		nIgniteTime = mdInfo.nIgniteTime1;
		nSpeed = mdInfo.nSpeed1;
	}
	else
	{
		nMaxLife = mdp.nClearLevel[mdInfo.nStageLv] + 1;
		nMaxPower = mdInfo.nMaxPower2;
		nIgniteTime = mdInfo.nIgniteTime2;
		nSpeed = mdInfo.nSpeed2;
	}

	nCurrentLife = nMaxLife;
	nCurrentPower = nMaxPower;

	bChargeMode = false;
	bFireMode = true;
	bStarMode = false;
	nStarTime = 0;

	nDirection = a_nDirection;

	animate = NULL;
	bDieRedraw = false;

}

/////////////////////////////////////////////

void PlayFunc::init(Layer* a_pLayer)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
	pLayer = a_pLayer;
	director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	origin = director->getVisibleOrigin();
	nGameType = mdInfo.nPlayGameType;
	nStageLv = mdInfo.nStageLv;
	p1.init(+1);
	p2.init(-1);


	InitBG();
	InitClickLabel();
	InitProgress();
	InitParticle();
	InitCharacterSprite();


	//////////////// add stage
	nSetBlock_BaseRainTick = 0;
	nSetBlock_RainTick = 10000;
	nSetBlock_StarTick = 0;

	bPlayerPositionRain = CCRANDOM_0_1() > 0.5 ? true : false;

	bShowerStage = false;
	bHeavyRainStage = false;
	bStormStage = false;
	bUpdraftStage = false;
	bGrowStage = false;
	bDispersionStage = false;
	bCombineStage = false;
	bJumpStage = false;
	bTornadoCenter = true;
	////////////////////////////////////
	bStart = false; 
	bGameOver = false;
	bUpdateGameOver = false;
	nGameoverTick = 0;


	nPreClearLevel = 0;
	nPreClearLevel = mdp.nClearLevel[nStageLv];


	{
		auto StageLabel = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE * 1.5);
		StageLabel->setTextColor(Color4B::BLUE);
		StageLabel->enableOutline(Color4B::WHITE, 2);
		StageLabel->setPosition(Vec2(origin.x + visibleSize.width *0.5, origin.y + visibleSize.height*0.7));
		pLayer->addChild(StageLabel,1);
		StageLabel->setString(getStageString(nStageLv, mdp.bEnglish));

		nTimeTick = 90000;
		int nTemp = nTimeTick;
		int nMinute = nTemp / 60000;
		nTemp = nTemp - nMinute * 60000;
		int nSecond = nTemp / 1000;
		nTemp = nTemp - nSecond * 1000;
		int nCS = nTemp / 10;

		auto clearLabel = Label::createWithTTF(
			StringUtils::format("%d:%02d:%02d", nMinute, nSecond, nCS), _fonts_NanumPen, IN_GAME_FONT_SIZE * 1.5);
		clearLabel->setTextColor(Color4B::GREEN);
		clearLabel->enableOutline(Color4B::WHITE, 2);
		clearLabel->setPosition(Vec2(origin.x + visibleSize.width *0.5, origin.y + visibleSize.height*0.6));
		clearLabel->setTag(STAGE_CLEAR_LABEL);
		pLayer->addChild(clearLabel, 1);
	}



	auto pause = Sprite::createWithSpriteFrameName("pause.png");
	auto pause_s = Sprite::createWithSpriteFrameName("pause_s.png");
	auto pauseItem = MenuItemSprite::create(pause, pause_s, CC_CALLBACK_1(PlayFunc::Pause, this));
	auto menuPause = Menu::create(pauseItem, NULL);
	menuPause->setPosition(visibleSize.width * 0.9, visibleSize.height * 0.9);
	menuPause->setTag(GAME_PAUSE);
	pLayer->addChild(menuPause, 100);

	auto resume = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE * 1.5);
	resume->setTextColor(Color4B::YELLOW);
	resume->enableOutline(Color4B::WHITE, 2);
	auto exit = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE* 1.5);
	exit->setTextColor(Color4B::YELLOW);
	exit->enableOutline(Color4B::WHITE, 2);
	if (mdp.bEnglish)
	{
		resume->setString(en_resume);
		exit->setString(en_gameover);
	}
	else
	{
		resume->setString(UTF8PCHAR(ko_resume));
		exit->setString(UTF8PCHAR(ko_gameover));
	}

	auto resumeItem = MenuItemLabel::create(resume, CC_CALLBACK_1(PlayFunc::Resume, this));
	auto exitItem = MenuItemLabel::create(exit, CC_CALLBACK_1(PlayFunc::PauseExit, this));
	auto menuResume = Menu::create(resumeItem, exitItem, NULL);
	menuResume->alignItemsVerticallyWithPadding(30);
	menuResume->setPosition(visibleSize.width * 0.85, visibleSize.height * 0.85);
	menuResume->setTag(GAME_RESUME);
	menuResume->setVisible(false);
	pLayer->addChild(menuResume, 100);


	Sprite* info = NULL;
	if (mdp.bEnglish)
	{
		info = Sprite::createWithSpriteFrameName("en_info.png");
	}
	else
	{
		info = Sprite::createWithSpriteFrameName("ko_info.png");
	}
	info->setPosition(0, 0);
	info->setAnchorPoint(Vec2::ZERO);
	info->setTag(GAME_INFO);
	pLayer->addChild(info, 20);

	auto label1 = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE * 1.4);
	label1->setTextColor(Color4B::WHITE);
	label1->enableOutline(Color4B::YELLOW, 2);
	auto label2 = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE* 1.4);
	label2->setTextColor(Color4B::WHITE);
	label2->enableOutline(Color4B::YELLOW, 2);
	auto label3 = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE* 1.4);
	label3->setTextColor(Color4B::WHITE);
	label3->enableOutline(Color4B::YELLOW, 2);
	auto label4 = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE* 1.4);
	label4->setTextColor(Color4B::WHITE);
	label4->enableOutline(Color4B::YELLOW, 2);
	if (mdp.bEnglish)
	{
		label1->setString(en_easy); 
		label2->setString(en_normal);
		label3->setString(en_hard);
		label4->setString(en_hell);
	}
	else
	{
		label1->setString(UTF8PCHAR(ko_easy));
		label2->setString(UTF8PCHAR(ko_normal));
		label3->setString(UTF8PCHAR(ko_hard));
		label4->setString(UTF8PCHAR(ko_hell));
	}

	auto labelItem1 = MenuItemLabel::create(label1, CC_CALLBACK_1(PlayFunc::SelectDifficultyToStart, this));
	auto labelItem2 = MenuItemLabel::create(label2, CC_CALLBACK_1(PlayFunc::SelectDifficultyToStart, this));
	auto labelItem3 = MenuItemLabel::create(label3, CC_CALLBACK_1(PlayFunc::SelectDifficultyToStart, this));
	auto labelItem4 = MenuItemLabel::create(label4, CC_CALLBACK_1(PlayFunc::SelectDifficultyToStart, this));

	labelItem1->setTag(1);
	labelItem2->setTag(2);
	labelItem3->setTag(3);
	labelItem4->setTag(4);
	auto diMenu = Menu::create(labelItem1, labelItem2, labelItem3, labelItem4, NULL);
	diMenu->setPosition(visibleSize.width * 0.15, visibleSize.height* 0.7);
	diMenu->alignItemsVertically();
	diMenu->setTag(GAME_DIFFI);
	pLayer->addChild(diMenu, 21);


	auto click = Sprite::createWithSpriteFrameName("click.png");
	click->setPosition(visibleSize.width * 0.35, visibleSize.height* 0.8);
	click->setTag(GAME_CLICK);
	pLayer->addChild(click, 21);
	auto action1 = ScaleTo::create(1, 0.8f);
	auto action2 = ScaleTo::create(1, 1.3f);
	auto action = Sequence::create(action1, action2, NULL);
	click->runAction(RepeatForever::create(action));


}



void PlayFunc::SelectDifficultyToStart(Ref* pSender)
{
	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();

	switch (nSelectedItem)
	{
	case 1:
		nDifficulty = 1;
		nTimeTick = 45000; // easy 만 45
		nDifficultySpeed = 1.2f;
		break;
	case 2:
		nDifficulty = 2;
		nTimeTick = 60000; // 1분
		nDifficultySpeed = 0.6f;
		break;
	case 3:
		nDifficulty = 3;
		nTimeTick = 90000; // 1분 30초
		nDifficultySpeed = 0.3f; // *caution = Move duration is minus... void
	case 4:
		nDifficulty = 4;
		nTimeTick = 90000; // 1분 30초
		nDifficultySpeed = 0;
		break;
	default:
		break;
	}


	{//char ani start
		auto sprite1 = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		auto animation = Animation::create();
		animation->setDelayPerUnit(CHAR_ANI_DELAY);
		for (int i = 0; i < 2; i++)
		{
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("char1_%d.png", i + 1));
			animation->addSpriteFrame(frame);
		}
		p1.animate = Animate::create(animation);
		sprite1->runAction(RepeatForever::create(p1.animate));

		if (nGameType == MENU_DOUBLE || nGameType == MENU_BATTLE)
		{
			auto sprite2 = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);
			auto animation = Animation::create();
			animation->setDelayPerUnit(CHAR_ANI_DELAY);
			for (int i = 0; i < 2; i++)
			{
				auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("char2_%d.png", i + 1));
				animation->addSpriteFrame(frame);
			}
			p2.animate = Animate::create(animation);
			sprite2->runAction(RepeatForever::create(p2.animate));
		}
	}


	{//char life draw
		for (int i = 0; i < MAX_DIFFICULTY; i++)
		{
			auto star = Sprite::createWithSpriteFrameName("star1.png");
			star->setScale(0.5f);
			star->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			star->setPosition(30, visibleSize.height * 0.4 + i * 50);
			pLayer->addChild(star, 10);
		}

		for (int i = 0; i < MAX_DIFFICULTY; i++)
		{
			auto star = Sprite::createWithSpriteFrameName("star2.png");
			star->setScale(0.5f);
			star->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			star->setPosition(30, visibleSize.height * 0.4 + i * 50);
			star->setVisible(false);
			star->setTag(STAR_LIFE1 + i);
			pLayer->addChild(star, 11);
		}

		if (nGameType != MENU_SOLO)
		{
			for (int i = 0; i < MAX_DIFFICULTY; i++)
			{
				auto star = Sprite::createWithSpriteFrameName("star1.png");
				star->setScale(0.5f);
				star->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				star->setPosition(visibleSize.width - 30, visibleSize.height * 0.4 + i * 50);
				pLayer->addChild(star, 10);
			}

			for (int i = 0; i < MAX_DIFFICULTY; i++)
			{
				auto star = Sprite::createWithSpriteFrameName("star2.png");
				star->setScale(0.5f);
				star->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				star->setPosition(visibleSize.width - 30, visibleSize.height * 0.4 + i * 50);
				star->setVisible(false);
				star->setTag(STAR_LIFE2 + i);
				pLayer->addChild(star, 11);
			}
		}

	}


	auto diMenu = (Menu*)menuItem->getParent();
	diMenu->setVisible(false);
	auto info = (Sprite*)pLayer->getChildByTag(GAME_INFO);
	info->setVisible(false);
	auto click = (Sprite*)pLayer->getChildByTag(GAME_CLICK);
	click->setVisible(false);
	
	bStart = true;

	if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);

	InitBGM();
}

void PlayFunc::Pause(Ref* pSender)
{
	if (bUpdateGameOver == true)
		return;

	if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);


	auto menu = (Menu*)pLayer->getChildByTag(GAME_PAUSE);
	menu->setVisible(false);
	menu= (Menu*)pLayer->getChildByTag(GAME_RESUME);
	menu->setVisible(true);


	auto pauseBG = (Sprite*)pLayer->getChildByTag(GAME_PAUSE_HIDE);
	pauseBG->setVisible(true);

	if (bStart == false)
	{
		auto diff = (Menu*)pLayer->getChildByTag(GAME_DIFFI);
		diff->setVisible(false);
		auto click = (Sprite*)pLayer->getChildByTag(GAME_CLICK);
		click->setVisible(false);
		auto spr = (Sprite*)pLayer->getChildByTag(GAME_INFO);
		spr->setVisible(false);
	}
	else
	{
		director->pause();
	}
}
void PlayFunc::Resume(Ref* pSender)
{
	if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);


	auto menu = (Menu*)pLayer->getChildByTag(GAME_PAUSE);
	menu->setVisible(true);
	menu = (Menu*)pLayer->getChildByTag(GAME_RESUME);
	menu->setVisible(false);

	auto pauseBG = (Sprite*)pLayer->getChildByTag(GAME_PAUSE_HIDE);
	pauseBG->setVisible(false);

	if (bStart == false)
	{
		auto diff = (Menu*)pLayer->getChildByTag(GAME_DIFFI);
		diff->setVisible(true);
		auto click = (Sprite*)pLayer->getChildByTag(GAME_CLICK);
		click->setVisible(true);
		auto spr = (Sprite*)pLayer->getChildByTag(GAME_INFO);
		spr->setVisible(true);
	}
	else
	{
		director->resume();
	}
}
void PlayFunc::PauseExit(Ref* pSender)
{
	if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);

	director->resume();
	ExitGame();
}



void PlayFunc::InitBGM()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(_BGM_highenergy, true);
	if (MyDataManager::getInstance()->mPlayerData.bBGMSwitch == false)
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
void PlayFunc::EndBGM()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}


void PlayFunc::InitBG()
{
	auto sprBG1 = Sprite::createWithSpriteFrameName("BG1.png");
	sprBG1->setAnchorPoint(Point::ZERO);
	sprBG1->setScale(2.0f);
	pLayer->addChild(sprBG1, 0);

	auto sprForeBG1 = Sprite::createWithSpriteFrameName("BG2.png");
	sprForeBG1->setAnchorPoint(Point::ZERO);
	sprForeBG1->setScale(2.0f);
	pLayer->addChild(sprForeBG1, 5);

	auto sprPauseBG = Sprite::createWithSpriteFrameName("BGpause.png");
	sprPauseBG->setAnchorPoint(Point::ZERO);
	sprPauseBG->setVisible(false);
	sprPauseBG->setTag(GAME_PAUSE_HIDE);
	pLayer->addChild(sprPauseBG, 99);


	auto node2 = ParallaxNode::create();
	pLayer->addChild(node2, 19);

	auto actionMoveBy2 = MoveBy::create(25.0f, Point(-767, 0));
	auto actionPlace2 = Place::create(Point::ZERO);
	auto actionSequence2 = Sequence::create(actionMoveBy2, actionPlace2, NULL);
	auto actionRepeat2 = RepeatForever::create(actionSequence2);
	node2->runAction(actionRepeat2);

	auto spr_back3_1 = Sprite::createWithSpriteFrameName("BGcloud.png");
	spr_back3_1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	node2->addChild(spr_back3_1, 5, Point(1, 0), Point(0, visibleSize.height));

	auto spr_back3_2 = Sprite::createWithSpriteFrameName("BGcloud.png");
	spr_back3_2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	node2->addChild(spr_back3_2, 5, Point(1, 0), Point(767, visibleSize.height));

	auto spr_back3_3 = Sprite::createWithSpriteFrameName("BGcloud.png");
	spr_back3_3->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	node2->addChild(spr_back3_3, 5, Point(-1, 0), Point(0, visibleSize.height));

	auto spr_back3_4 = Sprite::createWithSpriteFrameName("BGcloud.png");
	spr_back3_4->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	node2->addChild(spr_back3_4, 5, Point(-1, 0), Point(-767, visibleSize.height));
}


void PlayFunc::InitClickLabel()
{
	if (nGameType == MENU_SOLO)
	{
		auto spr1 = Sprite::createWithSpriteFrameName("leftright.png");
		spr1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + PLAYER_BASE_Y / 2));
		pLayer->addChild(spr1, 6);
	}
	else
	{
		auto spr1 = Sprite::createWithSpriteFrameName("leftright.png");
		spr1->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + PLAYER_BASE_Y / 2));
		pLayer->addChild(spr1, 6);

		auto spr2 = Sprite::createWithSpriteFrameName("leftright.png");
		spr2->setPosition(Vec2(origin.x + visibleSize.width / 4 * 3, origin.y + PLAYER_BASE_Y / 2));
		pLayer->addChild(spr2, 6);
	}
}

void PlayFunc::InitParticle()
{
	auto particle = ParticleSystemQuad::create(_particle_BG_rain);
	particle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	particle->setScale(2.0f);
	pLayer->addChild(particle, 2);
}


void PlayFunc::InitProgress()
{
	//Bar BG
	auto BarBG1 = Sprite::createWithSpriteFrameName("EmptyBar.png");
	BarBG1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	BarBG1->setPosition(Vec2(origin.x, origin.y + PLAYER_BASE_Y));
	pLayer->addChild(BarBG1, 10);

	//Power Bar Progress
	auto spritePowerBar1 = Sprite::createWithSpriteFrameName("RedBar.png");
	auto progressPowerTimer1 = ProgressTimer::create(spritePowerBar1);	// ProgressTimer생성
	progressPowerTimer1->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
	progressPowerTimer1->setMidpoint(Vec2(0, 0));                    // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
	progressPowerTimer1->setBarChangeRate(Vec2(1, 0));          // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
	progressPowerTimer1->setAnchorPoint(BarBG1->getAnchorPoint());
	progressPowerTimer1->setPosition(BarBG1->getPosition());
	progressPowerTimer1->setTag(POWER_BAR_TAG1);
	progressPowerTimer1->setPercentage(100.0f);
	pLayer->addChild(progressPowerTimer1, 11);

	//Charge Bar Progress
	auto spriteChargeBar1 = Sprite::createWithSpriteFrameName("YellowBar.png");
	auto progressChargeTimer1 = ProgressTimer::create(spriteChargeBar1);	// ProgressTimer생성
	progressChargeTimer1->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
	progressChargeTimer1->setMidpoint(Vec2(0, 0));                    // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
	progressChargeTimer1->setBarChangeRate(Vec2(1, 0));          // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
	progressChargeTimer1->setAnchorPoint(BarBG1->getAnchorPoint());
	progressChargeTimer1->setPosition(BarBG1->getPosition());
	progressChargeTimer1->setTag(CHARGE_BAR_TAG1);
	progressChargeTimer1->setPercentage(0.0f);
	pLayer->addChild(progressChargeTimer1, 0);

	if (nGameType == MENU_SOLO)
	{
		BarBG1->setScaleX(visibleSize.width / BarBG1->getContentSize().width);
		progressPowerTimer1->setScaleX(BarBG1->getScaleX());
		progressChargeTimer1->setScaleX(BarBG1->getScaleX());

	}
	else if (nGameType == MENU_DOUBLE || nGameType == MENU_BATTLE)
	{
		//Bar BG2
		auto BarBG2 = Sprite::createWithSpriteFrameName("EmptyBar.png");
		BarBG2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		BarBG2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + PLAYER_BASE_Y));
		pLayer->addChild(BarBG2, 10);

		//Power Bar Progress
		auto spritePowerBar2 = Sprite::createWithSpriteFrameName("RedBar.png");
		auto progressPowerTimer2 = ProgressTimer::create(spritePowerBar2);	// ProgressTimer생성
		progressPowerTimer2->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
		progressPowerTimer2->setMidpoint(Vec2(0, 0));                    // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
		progressPowerTimer2->setBarChangeRate(Vec2(1, 0));          // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
		progressPowerTimer2->setAnchorPoint(BarBG2->getAnchorPoint());
		progressPowerTimer2->setPosition(BarBG2->getPosition());
		progressPowerTimer2->setPercentage(100.0f);
		progressPowerTimer2->setTag(POWER_BAR_TAG2);
		pLayer->addChild(progressPowerTimer2, 11);

		//Charge Bar Progress
		auto spriteChargeBar2 = Sprite::createWithSpriteFrameName("YellowBar.png");
		auto progressChargeTimer2 = ProgressTimer::create(spriteChargeBar2);	// ProgressTimer생성
		progressChargeTimer2->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
		progressChargeTimer2->setMidpoint(Vec2(0, 0));                    // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
		progressChargeTimer2->setBarChangeRate(Vec2(1, 0));          // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
		progressChargeTimer2->setAnchorPoint(BarBG2->getAnchorPoint());
		progressChargeTimer2->setPosition(BarBG2->getPosition());
		progressChargeTimer2->setTag(CHARGE_BAR_TAG2);
		progressChargeTimer2->setPercentage(0.0f);
		pLayer->addChild(progressChargeTimer2, 0);

		BarBG1->setScaleX(visibleSize.width / BarBG1->getContentSize().width / 2);
		progressPowerTimer1->setScaleX(BarBG1->getScaleX());
		progressChargeTimer1->setScaleX(BarBG1->getScaleX());

		BarBG2->setScaleX(visibleSize.width / BarBG2->getContentSize().width / 2);
		progressPowerTimer2->setScaleX(BarBG2->getScaleX());
		progressChargeTimer2->setScaleX(BarBG2->getScaleX());

	}
}

void PlayFunc::InitCharacterSprite()
{
	auto sprite1 = Sprite::createWithSpriteFrameName("char1_0.png");
	sprite1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	sprite1->setPosition(Vec2(origin.x + visibleSize.width / 4 * 1, origin.y + PLAYER_BASE_Y + BAR_Y));
	sprite1->setScaleX(p1.nDirection);
	sprite1->setTag(PLAYER_TAG1);
	pLayer->addChild(sprite1, 3);

	auto spriteDie = Sprite::createWithSpriteFrameName("char1_die.png");
	spriteDie->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	spriteDie->setTag(PLAYER_DIE_TAG1);
	spriteDie->setVisible(false);
	pLayer->addChild(spriteDie);


	if (nGameType == MENU_DOUBLE || nGameType == MENU_BATTLE)
	{
		auto sprite2 = Sprite::createWithSpriteFrameName("char2_0.png");
		sprite2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		sprite2->setPosition(Vec2(origin.x + visibleSize.width / 4 * 3, origin.y + PLAYER_BASE_Y + BAR_Y));
		sprite2->setScaleX(p2.nDirection);
		sprite2->setTag(PLAYER_TAG2);
		pLayer->addChild(sprite2, 3);

		auto spriteDie2 = Sprite::createWithSpriteFrameName("char2_die.png");
		spriteDie2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		spriteDie2->setTag(PLAYER_DIE_TAG2);
		spriteDie2->setVisible(false);
		pLayer->addChild(spriteDie2);

	}	
}





void PlayFunc::UpdateCharacter(int PlayerNum)
{
	Sprite* pSpriteDie = NULL;
	Sprite* pSprChar = NULL;
	Player* pPlayer = NULL;

	if (PlayerNum == 1)
	{
		pSpriteDie = (Sprite*)pLayer->getChildByTag(PLAYER_DIE_TAG1);
		pSprChar = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		pPlayer = &p1;
	}
	else if (PlayerNum == 2)
	{
		pSpriteDie = (Sprite*)pLayer->getChildByTag(PLAYER_DIE_TAG2);
		pSprChar = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);
		pPlayer = &p2;
	}



	if (pSpriteDie != NULL && pSprChar != NULL)
	{
		if (pPlayer->nCurrentLife == 0 && pPlayer->bDieRedraw == false)
		{
			pSpriteDie->setPosition(pSprChar->getPosition());
			pSpriteDie->setScaleX(pSprChar->getScaleX());
			pSpriteDie->setVisible(true);
			pSprChar->setVisible(false);
			pPlayer->bDieRedraw = true;
		}
		else if (pPlayer->nCurrentLife > 0 && pPlayer->bDieRedraw == true)
		{
			pSpriteDie->setVisible(false);
			pSprChar->setVisible(true);
			pPlayer->bDieRedraw = false;
		}
	}


	ParticleSystemQuad* particleEightMode = NULL;
	if (PlayerNum == 1)
	{
		particleEightMode = (ParticleSystemQuad*)pLayer->getChildByTag(PARTICLE_EIGHT1);
		if (particleEightMode == NULL)
		{
			particleEightMode = ParticleSystemQuad::create(_particle_char_eightmode);
			particleEightMode->setTag(PARTICLE_EIGHT1);
			particleEightMode->setScale(0.7f);
			pLayer->addChild(particleEightMode, 0);
		}
	}
	else if (PlayerNum == 2)
	{
		particleEightMode = (ParticleSystemQuad*)pLayer->getChildByTag(PARTICLE_EIGHT2);
		if (particleEightMode == NULL)
		{
			particleEightMode = ParticleSystemQuad::create(_particle_char_eightmode2);
			particleEightMode->setTag(PARTICLE_EIGHT2);
			particleEightMode->setScale(0.7f);
			pLayer->addChild(particleEightMode, 0);
		}
	}

	if (pSprChar != NULL)
	{
		particleEightMode->setPosition(pSprChar->getPosition());
		if (pPlayer->bFireMode == true)
		{
			particleEightMode->setVisible(true);
			particleEightMode->setScale(0.6f);
		}
		else
		{
			particleEightMode->setVisible(false);
		}
	}
	else
	{
		particleEightMode->setVisible(false);
	}

	ParticleSystemQuad* particleStar = NULL;
	if (PlayerNum == 1)
	{
		particleStar = (ParticleSystemQuad*)pLayer->getChildByTag(PARTICLE_STAR1);
		if (particleStar == NULL)
		{
			particleStar = ParticleSystemQuad::create(_particle_starmode);
			particleStar->setTag(PARTICLE_STAR1);
			particleStar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			pLayer->addChild(particleStar, 0);
		}
	}
	else if (PlayerNum == 2)
	{
		particleStar = (ParticleSystemQuad*)pLayer->getChildByTag(PARTICLE_STAR2);
		if (particleStar == NULL)
		{
			particleStar = ParticleSystemQuad::create(_particle_starmode);
			particleStar->setTag(PARTICLE_STAR2);
			particleStar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			pLayer->addChild(particleStar, 0);
		}
	}

	if (pSprChar != NULL)
	{
		particleStar->setPosition(pSprChar->getPosition());
		if (pPlayer->bStarMode == true)
		{
			particleStar->setVisible(true);
			particleEightMode->setVisible(false);
		}
		else
		{
			particleStar->setVisible(false);
		}
	}
	else
	{
		particleStar->setVisible(false);
	}

}


void PlayFunc::UpdateProgress(int PlayerNum)
{
	Sprite* pSprChar = NULL;
	ProgressTimer* progressPowerTimer = NULL;
	ProgressTimer* progressChargeTimer = NULL;
	ProgressTimer* otherPowerTimer = NULL;
	ProgressTimer* otherChargeTimer = NULL;
	Player* pPlayer = NULL;
	Player* pOther = NULL;
	if (PlayerNum == 1)
	{
		pSprChar = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		progressPowerTimer = (ProgressTimer*)pLayer->getChildByTag(POWER_BAR_TAG1);
		progressChargeTimer = (ProgressTimer*)pLayer->getChildByTag(CHARGE_BAR_TAG1);
		otherPowerTimer = (ProgressTimer*)pLayer->getChildByTag(POWER_BAR_TAG2);
		otherChargeTimer = (ProgressTimer*)pLayer->getChildByTag(CHARGE_BAR_TAG2);
		pPlayer = &p1;
		pOther = &p2;

	}
	else if (PlayerNum == 2)
	{
		pSprChar = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);
		progressPowerTimer = (ProgressTimer*)pLayer->getChildByTag(POWER_BAR_TAG2);
		progressChargeTimer = (ProgressTimer*)pLayer->getChildByTag(CHARGE_BAR_TAG2);
		otherPowerTimer = (ProgressTimer*)pLayer->getChildByTag(POWER_BAR_TAG1);
		otherChargeTimer = (ProgressTimer*)pLayer->getChildByTag(CHARGE_BAR_TAG1);
		pPlayer = &p2;
		pOther = &p1;
	}

	////////////////////////////////////// 1p
	if (progressPowerTimer != NULL)
	{
		if (pPlayer->nCurrentLife <= 0)
		{
			pPlayer->bChargeMode = false;
			progressChargeTimer->stopAllActions();
			
			pPlayer->bFireMode = false;
		}
		else if (pPlayer->nCurrentLife > 0 && pPlayer->nCurrentPower <= 0 && pPlayer->bChargeMode == false )
		{
			pPlayer->bChargeMode = true;
			progressChargeTimer->setLocalZOrder(12);
			auto progressFromTo = ProgressFromTo::create(pPlayer->nIgniteTime, 0, 100.0f); // 퍼센트 수정 액션 방법
			progressChargeTimer->runAction(progressFromTo);

			pPlayer->bFireMode = false;


			if (nGameType == MENU_SOLO)
			{
				nSetBlock_BaseRainTick = 10000;
				nSetBlock_RainTick = 10000;

				BlockSprite* pSprRemoveBlock = NULL;
				for (int i = 0; i < vRain1.size(); i++)
				{
					pSprRemoveBlock = vRain1.at(i);
					if (pSprRemoveBlock->isVisible())
					{

						auto particle = ParticleSystemQuad::create(_particle_steam);
						particle->setScale(pSprRemoveBlock->getScale() * 8);
						particle->setPosition(pSprRemoveBlock->getPosition());
						pLayer->addChild(particle, 20);
						auto action = Sequence::create(DelayTime::create(1.0f),
							CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetPlayerCrashParticle, this)),
							NULL);
						particle->runAction(action);

						pSprRemoveBlock->stopAllActions();
						pSprRemoveBlock->setVisible(false);
					}
				}


				if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_EightFire);

				if (pSprChar != NULL)
				{
					auto particle = ParticleSystemQuad::create(_particle_char_fire2);
					particle->setScale(1.0f);
					particle->setPosition(pSprChar->getPosition());
					pLayer->addChild(particle, 20);
					auto action = Sequence::create(DelayTime::create(2.0f),
						CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetPlayerCrashParticle, this)),
						NULL);
					particle->runAction(action);
				}
			}
			

		}
		else if (pPlayer->bChargeMode == true && progressChargeTimer->getPercentage() == 100.0f) //Fire 발동시
		{
			pPlayer->bChargeMode = false;
			progressChargeTimer->setLocalZOrder(0);

			pPlayer->bFireMode = true;
			//pPlayer->nCurrentLife = pPlayer->nMaxLife;
			pPlayer->nCurrentPower = pPlayer->nMaxPower;
			if (nGameType == MENU_DOUBLE)
			{
				pOther->bChargeMode = false;
				otherChargeTimer->setLocalZOrder(0);

				pOther->bFireMode = true;
				if (pOther->nCurrentLife == 0)
					pOther->nCurrentLife = 1;
				//pOther->nCurrentLife = pOther->nMaxLife;
				pOther->nCurrentPower = pOther->nMaxPower;
			}

			if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_EightFire);

			if (pSprChar != NULL)
			{
				auto particle = ParticleSystemQuad::create(_particle_char_fire);
				particle->setScale(1.0f);
				particle->setPosition(pSprChar->getPosition());
				pLayer->addChild(particle, 20);
				auto action = Sequence::create(DelayTime::create(2.0f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetPlayerCrashParticle, this)),
					NULL);
				particle->runAction(action);
			}

		}


		if (pPlayer->nCurrentPower > 0)
		{
			pPlayer->nCurrentPower += 0.001f;
			if (pPlayer->nCurrentPower > pPlayer->nMaxPower)
				pPlayer->nCurrentPower = pPlayer->nMaxPower;
		}
		if (pOther->nCurrentPower > 0)
		{
			pOther->nCurrentPower += 0.001f;
			if (pOther->nCurrentPower > pOther->nMaxPower)
				pOther->nCurrentPower = pOther->nMaxPower;
		}

		float PowerPercent = (pPlayer->nCurrentPower / pPlayer->nMaxPower) * 100.0f;
		progressPowerTimer->setPercentage(PowerPercent);
		if (nGameType != MENU_SOLO)
		{
			PowerPercent = (pOther->nCurrentPower / pOther->nMaxPower) * 100.0f;
			otherPowerTimer->setPercentage(PowerPercent);
		}


	}

}

void PlayFunc::UpdateStarLife(int PlayerNum)
{
	
	for (int i = 0; i < MAX_DIFFICULTY; i++)
	{
		Sprite* star = (Sprite*)pLayer->getChildByTag(STAR_LIFE1 + i);
		if (star && p1.nCurrentLife - 1 > i)
		{
			star->setVisible(true);
		}
		else
		{
			star->setVisible(false);
		}
	}

	if (nGameType != MENU_SOLO)
	{
		for (int i = 0; i < MAX_DIFFICULTY; i++)
		{
			Sprite* star = (Sprite*)pLayer->getChildByTag(STAR_LIFE2 + i);
			if (star && p2.nCurrentLife - 1 > i)
			{
				star->setVisible(true);
			}
			else
			{
				star->setVisible(false);
			}
		}
	}
}

void PlayFunc::UpdateStarModeDelay()
{
	if (p1.bStarMode == true)
	{
		p1.nStarTime++;
		if (p1.nStarTime > STARMODE_TICK)
		{
			p1.bStarMode = false;
			p1.nStarTime = 0;
		}
	}

	if (nGameType != MENU_SOLO)
	{
		if (p2.bStarMode == true)
		{
			p2.nStarTime++;
			if (p2.nStarTime > STARMODE_TICK)
			{
				p2.bStarMode = false;
				p2.nStarTime = 0;
			}
		}
	}
}

void PlayFunc::MoveCharacter(int PlayerNum)
{
	Sprite* sprPlayer = NULL;
	Sprite* sprOther = NULL;
	Player* pPlayer = NULL;
	Player* pOther = NULL;
	if (PlayerNum == 1)
	{
		sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		sprOther = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);
		pPlayer = &p1;
		pOther = &p2;
	}
	else if (PlayerNum == 2)
	{
		sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);
		sprOther = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		pPlayer = &p2;
		pOther = &p1;
	}


	if (sprPlayer != NULL && pPlayer->nCurrentLife > 0)
	{
		int nMoveX = pPlayer->nSpeed * pPlayer->nDirection;

		if (sprPlayer->getBoundingBox().getMinX() + nMoveX
			<= origin.x + sprPlayer->getBoundingBox().size.width / 2)
		{
			;
		}
		else if (sprPlayer->getBoundingBox().getMaxX() + nMoveX
			>= visibleSize.width - sprPlayer->getBoundingBox().size.width / 2)
		{
			;
		}
		else if (nGameType == MENU_BATTLE
			&& sprOther != NULL
			&& sprPlayer->getBoundingBox().getMidX() < sprOther->getBoundingBox().getMidX()
			&& nMoveX > 0
			&& sprPlayer->getBoundingBox().getMaxX() + (nMoveX + 1) * 2 > sprOther->getBoundingBox().getMinX())
		{
			;
		}
		else if (nGameType == MENU_BATTLE
			&& sprOther != NULL
			&& sprPlayer->getBoundingBox().getMidX() > sprOther->getBoundingBox().getMidX()
			&& nMoveX < 0
			&& sprPlayer->getBoundingBox().getMinX() + (nMoveX + 1) * 2 < sprOther->getBoundingBox().getMaxX())
		{
			;
		}
		else
		{
			auto pMoveBy = MoveBy::create(0, Vec2(nMoveX, 0));
			sprPlayer->runAction(pMoveBy);
		}
	}
}


void PlayFunc::Stage()
{
	Stage_Label();
	Stage_SetBlock();
}

void PlayFunc::Stage_Label()
{
	auto clearLabel = (Label*)pLayer->getChildByTag(STAGE_CLEAR_LABEL);
	if (clearLabel != NULL)
	{
		int nTemp = nTimeTick;
		int nMinute = nTemp / 60000;
		nTemp = nTemp - nMinute * 60000;
		int nSecond = nTemp / 1000;
		nTemp = nTemp - nSecond * 1000;
		int nCS = nTemp / 10;

		clearLabel->setString(
			StringUtils::format("%d:%02d:%02d", nMinute, nSecond, nCS));

		if (nTimeTick < 10000)
			clearLabel->setScale(2.0f);
	}
}

void PlayFunc::ResetSprite(Ref* sender)
{
	auto spr = (BlockSprite*)sender;
	spr->setVisible(false);
}
void PlayFunc::DispersionRain(Ref* sender)
{
	BlockSprite* rain = (BlockSprite*)sender;
	rain->setVisible(false);
	float nX = rain->getPositionX();
	float nY = rain->getPositionY();
	float nRand = CCRANDOM_0_1();
	int nType = 1;
	if (nRand > 0.7f)
		nType = 1;
	else if (nRand > 0.3f)
		nType = 2;
	else
		nType = 3;

	if (nType == 1)
	{
		for (int i = 0; i < 2; i++)
		{
			rain = getInvisibleRain(1);
			rain->setPosition(nX, nY);
			rain->nSpeed = 1.5f;
			rain->setScale(0.1f);

			float nXX = nX;
			if (i == 0)
			{
				nXX = nX - 250;
			}
			else if (i == 1)
			{
				nXX = nX + 250;
			}

			auto action = Sequence::create(
				MoveTo::create(rain->nSpeed + nDifficultySpeed, Vec2(nXX, 0)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
				NULL);
			rain->runAction(action);
		}
	}
	else if (nType == 2)
	{
		for (int i = 0; i < 3; i++)
		{
			rain = getInvisibleRain(1);
			rain->setPosition(nX, nY);
			rain->nSpeed = 1.5f;
			rain->setScale(0.1f);

			float nXX = nX;
			if (i == 0)
			{
				nXX = nX - 250;
			}
			else if (i == 2)
			{
				nXX = nX + 250;
			}

			auto action = Sequence::create(
				MoveTo::create(rain->nSpeed + nDifficultySpeed, Vec2(nXX, 0)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
				NULL);
			rain->runAction(action);
		}
	}
	else if (nType == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			rain = getInvisibleRain(1);
			rain->setPosition(nX, nY);
			rain->nSpeed = 1.5f;
			rain->setScale(0.1f);

			float nXX = nX;
			if (i == 0)
				nXX = nX - 350;
			else if (i == 1)
				nXX = nX - 200;
			else if (i == 2)
				nXX = nX + 200;
			else
				nXX = nX + 350;

			auto action = Sequence::create(
				MoveTo::create(rain->nSpeed + nDifficultySpeed, Vec2(nXX, 0)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
				NULL);
			rain->runAction(action);
		}
	}
	
}


void PlayFunc::Stage_SetBlock()
{
	if (nTimeTick > 0)
	{
		nTimeTick -= 21;
		if (nTimeTick < 0)
			nTimeTick = 0;
	}


	nSetBlock_StarTick += 1;
	if (nSetBlock_StarTick > 1000) // 30s per 1500
	{
		SetBlock_Star(0);
		nSetBlock_StarTick = 0;
	}


	nSetBlock_BaseRainTick += 1;
	if ( nSetBlock_BaseRainTick > 50)
	{
		SetBlock_Rain(0);
		nSetBlock_BaseRainTick = 0;
	}


	nSetBlock_RainTick += nDifficulty * 1.5;
	switch (nStageLv)
	{
	case 0: // drizzle

		if (nSetBlock_RainTick > 20)
		{
			SetBlock_Rain(1);
			nSetBlock_RainTick = 0;
		}
		break;

	case 1: // tornado

		if (nSetBlock_RainTick > 350)
		{
			SetBlock_Tornado();
			nSetBlock_RainTick = 0;
		}
		break;

	case 2: // shower

		if (nSetBlock_RainTick > 50)
		{
			bShowerStage = true;
			SetBlock_Rain(1);
			nSetBlock_RainTick = 0;
		}
		break;

	case 3: // storm
	
		if (nSetBlock_RainTick > 30)
		{
			bStormStage = true;
			SetBlock_Rain(1);
			nSetBlock_RainTick = 0;
		}
		break;

	case 4: // tsunami -> 4 easy

		if (nSetBlock_RainTick > 700)
		{
			SetBlock_Tsunami();
			nSetBlock_RainTick = 0;
		}
		break;

	case 5: //heavy rain

		if (nSetBlock_RainTick > 150)
		{
			bHeavyRainStage = true;
			SetBlock_Rain(1);
			nSetBlock_RainTick = 0;
		}
		break;

	case 6: // updraft

		if (nSetBlock_RainTick > 100)
		{
			bUpdraftStage = true;
			SetBlock_Rain(1);
			nSetBlock_RainTick = 0;
		}
		break;

	case 7: // Grow
	
		if (nSetBlock_RainTick > 70)
		{
			bGrowStage = true;
			SetBlock_Rain(1);
			nSetBlock_RainTick = 0;
		}
		break;

	case 8: // dispersion

		if (nSetBlock_RainTick > 100)
		{
			bDispersionStage = true;
			SetBlock_Rain(1);
			nSetBlock_RainTick = 0;
		}
		break;

	case 9: //combine

		if (nSetBlock_RainTick > 20)
		{
			bCombineStage = true;
			bStormStage = true;
			SetBlock_Rain(1);
			nSetBlock_RainTick = 0;
		}
		break;

	case 10: //jump
		if (nSetBlock_RainTick > 450)
		{
			bJumpStage = true;
			SetBlock_Jump(1);
			nSetBlock_RainTick = 0;
		}

		break;

	default: //add stage Fix
		break;
	}


}


BlockSprite* PlayFunc::getInvisibleRain(int nType)
{
	BlockSprite* rain = NULL;
	if (nType == 1)
	{
		int k;
		for (k = 0; k < vRain1.size(); k++)
		{
			rain = vRain1.at(k);
			if (rain->isVisible() == false)
			{
				rain->setVisible(true);
				break;
			}
		}

		//없으면 새로 생성
		if (k == vRain1.size())
		{
			rain = BlockSprite::createWithSpriteFrameName("Rain1.png");
			vRain1.pushBack(rain);
			rain->setVisible(true);
			pLayer->addChild(rain, 2);
		}

		rain->nDamage = 1.0f;

	}

	return rain;
}


BlockSprite* PlayFunc::getInvisibleStar(int nType)
{
	BlockSprite* Star = NULL;
	if (nType == 1)
	{
		int k;
		for (k = 0; k < vStar1.size(); k++)
		{
			Star = vStar1.at(k);
			if (Star->isVisible() == false)
			{
				Star->setVisible(true);
				break;
			}
		}

		//없으면 새로 생성
		if (k == vStar1.size())
		{
			Star = BlockSprite::createWithSpriteFrameName("star2.png");
			Star->setScale(0.5f);
			vStar1.pushBack(Star);
			Star->setVisible(true);
			pLayer->addChild(Star, 2);
		}

		Star->nDamage = 1.0f;

	}

	return Star;
}

void PlayFunc::SetBlock_Star(int nLevel)
{
	float createPositionY = visibleSize.height;
	float createPositionX = 100 + ( visibleSize.width - 200 ) * CCRANDOM_0_1();

	if (nLevel == 0)
	{
		BlockSprite* star = getInvisibleStar(1);
		star->nSpeed = 3.0f;
		star->setPosition(createPositionX, createPositionY);

		auto action = Sequence::create(
			RepeatForever::create(RotateTo::create(star->nSpeed, 1800)),
			MoveTo::create(star->nSpeed, Vec2(createPositionX, PLAYER_BASE_Y + BAR_Y * 1.2f)),
			NULL);
		star->runAction(action);

	}
}

void PlayFunc::SetBlock_Rain(int nLevel)
{
	float createPositionY = visibleSize.height;
	float createPositionX = origin.x + CCRANDOM_0_1() * visibleSize.width;


	if (nLevel == 0)
	{
		auto sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		auto sprOther = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);

		if (bPlayerPositionRain && sprPlayer != NULL && p1.nCurrentLife > 0)
		{
			createPositionX = sprPlayer->getPositionX();
		}
		if (bPlayerPositionRain == false && sprOther != NULL && p2.nCurrentLife > 0)
		{
			createPositionX = sprOther->getPositionX();
		}

		bPlayerPositionRain = bPlayerPositionRain ? false : true;
	}



	float ToMoveX = createPositionX;
	if ( bStormStage == true )
	{
		float nRandomStorm = CCRANDOM_0_1();
		if (nRandomStorm > 0.5f)
			ToMoveX = ToMoveX - 100;
		else
			ToMoveX = ToMoveX + 100;

	}



	int j = ( bHeavyRainStage == true ) ? 4 : 1;
	for (int i = 0; i < j; i++)
	{

		BlockSprite* rain = NULL;
		rain = getInvisibleRain(1);
		rain->nSpeed = 2.0f;
		rain->setPosition(createPositionX, createPositionY);




		if ( bShowerStage == true)
		{
			rain->nSpeed = rain->nSpeed * 0.35f;
		}
		else if(bStormStage == true)
		{
			rain->nSpeed = rain->nSpeed - (rain->nSpeed  * 0.50f * CCRANDOM_0_1());
		}

		if (bHeavyRainStage)
		{
			rain->setScale(0.35f);
			rain->nSpeed = rain->nSpeed * 2.0f;
		}
		else
		{
			rain->setScale(0.1f);
		}


		if (bUpdraftStage)
		{
			float nRandom = CCRANDOM_0_1();
			auto action = Sequence::create(
				MoveTo::create((rain->nSpeed + (i * 0.05f) + nDifficultySpeed) / 2, Vec2(ToMoveX, PLAYER_BASE_Y + BAR_Y * 1.8f)),
				DelayTime::create(0.2f),
				MoveTo::create((rain->nSpeed + (i * 0.05f) + nDifficultySpeed) / 2, Vec2(ToMoveX, PLAYER_BASE_Y * 4 + PLAYER_BASE_Y * 2 * nRandom)),
				MoveTo::create((rain->nSpeed + (i * 0.05f) + nDifficultySpeed), Vec2(ToMoveX, 0)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
				NULL);
			rain->runAction(action);
		}
		else if (bGrowStage)
		{
			float nRandom = CCRANDOM_0_1();
			auto action = Sequence::create(
				MoveTo::create((rain->nSpeed + (i * 0.05f) + nDifficultySpeed) / 2, Vec2(ToMoveX, PLAYER_BASE_Y * 2 + PLAYER_BASE_Y*4 * nRandom)),
				ScaleTo::create(0.2f,0.3f),
				MoveTo::create((rain->nSpeed + (i * 0.05f) + nDifficultySpeed) / 2 + 1 * nRandom, Vec2(ToMoveX, 0)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
				NULL);
			rain->runAction(action);
		}
		else if (bDispersionStage)
		{
			rain->setScale(0.25f);
			float nRandom = CCRANDOM_0_1();
			auto action = Sequence::create(
				MoveTo::create( 1.5f , Vec2(ToMoveX, PLAYER_BASE_Y * 4 + PLAYER_BASE_Y * nRandom)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::DispersionRain/*caution*/, this)),
				NULL);
			rain->runAction(action);
		}
		else if (bJumpStage)
		{
			float nRandom = CCRANDOM_0_1();
			bool bLeftJump = false;
			if (nRandom < 0.5f)
			{
				bLeftJump = true;
				nRandom *= 2;
			}
			nRandom = nRandom + 0.5f;
			float ToMoveX2 = visibleSize.width * 0.3 * nRandom;
			if (bLeftJump)
				ToMoveX2 *= -1;

			auto action = Sequence::create(
				MoveTo::create((rain->nSpeed + (i * 0.05f) + nDifficultySpeed), Vec2(ToMoveX, PLAYER_BASE_Y + BAR_Y)),
				JumpBy::create((rain->nSpeed + (i * 0.05f) + nDifficultySpeed) * 2, Vec2(ToMoveX2, 0), nRandom * visibleSize.height *0.5, 2),
				MoveBy::create(0.2f, Vec2(0, -(PLAYER_BASE_Y + BAR_Y + 100))),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
				NULL);
			rain->runAction(action);
		}
		else
		{
			auto action = Sequence::create(
				MoveTo::create(rain->nSpeed + (i * 0.05f) + nDifficultySpeed, Vec2(ToMoveX, 0)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
				NULL);
			rain->runAction(action);
		}
	}

}


void PlayFunc::SetBlock_Tornado(int nLevel)
{
	float createPositionY = visibleSize.height;
	float createPositionX;
	if (bTornadoCenter == true)
	{
		createPositionX = origin.x + visibleSize.width / 2;
	}
	else
	{
		createPositionX = origin.x + ((CCRANDOM_0_1() > 0.5f) ? visibleSize.width * 3 / 4 : visibleSize.width * 1 / 4);

	}
	bTornadoCenter = bTornadoCenter ? false : true;

	float nStageRainScale = 1.0f;

	int nDirection = -1;
	for (int i = 0; i < 20; i++)
	{
		auto rain = getInvisibleRain(1);
		rain->setPosition(createPositionX, createPositionY);
		rain->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		rain->setScale(0.2f * nStageRainScale);

		ccBezierConfig bezierConfig1;
		bezierConfig1.controlPoint_1 = Point(createPositionX - (TORNADO_X_RADIUS * nDirection), createPositionY * 13 / 15);
		bezierConfig1.controlPoint_2 = Point(createPositionX + (TORNADO_X_RADIUS * nDirection), createPositionY * 11 / 15);
		bezierConfig1.endPosition = Point(createPositionX, createPositionY * 9 / 15);
		ccBezierConfig bezierConfig2;
		bezierConfig2.controlPoint_1 = Point(createPositionX - (TORNADO_X_RADIUS * nDirection), createPositionY * 7 / 15);
		bezierConfig2.controlPoint_2 = Point(createPositionX + (TORNADO_X_RADIUS * nDirection), createPositionY * 5 / 15);
		bezierConfig2.endPosition = Point(createPositionX, 0);
		auto Bezier1 = BezierTo::create(1.5f + nDifficultySpeed, bezierConfig1);
		auto Bezier2 = BezierTo::create(0.6f, bezierConfig2);
		auto action = Sequence::create(DelayTime::create(i * 0.05f),
			Bezier1, Bezier2,
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
			NULL);
		rain->runAction(action);

		nDirection = nDirection * -1;
	}

}
void PlayFunc::SetBlock_Tsunami(int nLevel)
{

	int nDirection = CCRANDOM_0_1() > 0.5f ? 1 : -1;
	float createPositionX = (nDirection > 0) ? -200 : visibleSize.width + 200;

	float  j = 0, k = 0;
	for (int i = 0; i < 60; i++)
	{
		if (j == 20)
		{
			j = 0;
			k++;
		}

		auto rain = getInvisibleRain(1);
		rain->setPosition(createPositionX, PLAYER_BASE_Y * 3);
		rain->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		rain->setScale(0.5f);

		float ToX = 0.0f;
		if (nDirection > 0)
			ToX = visibleSize.width * (j / 60);
		else
			ToX = visibleSize.width - (visibleSize.width * (j / 60));

		
		auto MoveBy = MoveTo::create(0.5f + nDifficultySpeed, Point(ToX, (PLAYER_BASE_Y * 2) + (j * 12)));
		float Tox2 = 0.0f;
		if (nDirection > 0)
			Tox2 = 200;
		else
			Tox2 = -200;
		
		auto MoveBy2 = MoveTo::create(1.0f , Point(ToX + Tox2, 0));

		auto action = Sequence::create(
			DelayTime::create(k * 0.5),
			MoveBy,
			MoveBy2,
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
			NULL);

		rain->runAction(action);

		j++;
	}
}

void PlayFunc::SetBlock_Jump(int nLevel)
{

	int nDirection = CCRANDOM_0_1() > 0.5f ? 1 : -1;
	float createPositionX = (nDirection > 0) ? -100 : visibleSize.width + 100;
	float ToX = (nDirection > 0) ? visibleSize.width : 0;
	int nJumpCount = CCRANDOM_0_1() > 0.5f ? 5 : 3;
	for (int i = 0; i < 20; i++)
	{

		auto rain = getInvisibleRain(1);
		rain->setPosition(createPositionX, PLAYER_BASE_Y + BAR_Y);
		rain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		rain->setScale(0.2f);

		auto JumpTo = JumpTo::create(6.0f + 6.0f * nDifficultySpeed,
			Point(ToX, 0),
			visibleSize.height*0.6, nJumpCount);

		auto action = Sequence::create(
			DelayTime::create(i * 0.01f),
			JumpTo,
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
			NULL);

		rain->runAction(action);
	}
}


void PlayFunc::resetPlayerCrashParticle(Ref* sender)
{
	auto particle = (ParticleSystemQuad*)sender;
	pLayer->removeChild(particle);
}

void PlayFunc::CrashStar(int PlayerNum)
{
	Sprite* sprPlayer = NULL;
	Sprite* sprOther = NULL;
	Player* player = NULL;
	Player* other = NULL;
	bool bOtherCrash = false;


	if (PlayerNum == 1)
	{
		sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		sprOther = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);
		player = &p1;
		other = &p2;

	}
	else if (PlayerNum == 2)
	{
		sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);
		sprOther = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		player = &p2;
		other = &p1;
	}




	if (sprPlayer != NULL && player->nCurrentLife > 0)
	{

		Rect rectPlayer = Rect(
			sprPlayer->getBoundingBox().origin.x + sprPlayer->getContentSize().width * 0.35,
			sprPlayer->getBoundingBox().origin.y + sprPlayer->getContentSize().width * 0.35,
			sprPlayer->getContentSize().width - sprPlayer->getContentSize().width * 0.7,
			sprPlayer->getContentSize().height - sprPlayer->getContentSize().width * 0.7);

		BlockSprite* pSprRemoveStar = NULL;
		for (BlockSprite* pSprStar : vStar1)
		{
			if (pSprStar->isVisible() == false)
				continue;

			Rect rectBlock = pSprStar->getBoundingBox();
			if (rectPlayer.intersectsRect(rectBlock))
			{
				pSprRemoveStar = pSprStar;
				///////////////// 동시 데미지
				if (sprOther != NULL)
				{
					Rect rectOther = Rect(
						sprOther->getBoundingBox().origin.x + sprOther->getContentSize().width * 0.35,
						sprOther->getBoundingBox().origin.y + sprOther->getContentSize().width * 0.35,
						sprOther->getContentSize().width - sprOther->getContentSize().width * 0.7,
						sprOther->getContentSize().height - sprOther->getContentSize().width * 0.7);
					if (rectOther.intersectsRect(rectBlock))
						bOtherCrash = true;
				}
				////////
				break;
			}
		}


		if (pSprRemoveStar != NULL)
		{

			if (player->nCurrentLife > 0)
			{
				player->bStarMode = true;
				player->nStarTime = 0;
			}

			///////////////// 동시 데미지
			if (bOtherCrash)
			{
				if (other->nCurrentLife > 0)
				{
					other->bStarMode = true;
					other->nStarTime = 0;
				}
			}
			////////////////////
			if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_receive);

			pSprRemoveStar->stopAllActions();
			pSprRemoveStar->setVisible(false);
		}

	}




}
//충돌 체크 코드
void PlayFunc::CrashBlock(int PlayerNum)
{
	Sprite* sprPlayer = NULL;
	Sprite* sprOther = NULL;
	Player* player = NULL;
	Player* other = NULL;
	bool bOtherCrash = false;


	if (PlayerNum == 1)
	{
		sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		sprOther = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);
		player = &p1;
		other = &p2;

	}
	else if (PlayerNum == 2)
	{
		sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG2);
		sprOther = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		player = &p2;
		other = &p1;
	}

	if (sprPlayer != NULL && player->nCurrentLife > 0)
	{

		Rect rectPlayer = Rect(
			sprPlayer->getBoundingBox().origin.x + sprPlayer->getContentSize().width * 0.35,
			sprPlayer->getBoundingBox().origin.y + sprPlayer->getContentSize().width * 0.35,
			sprPlayer->getContentSize().width - sprPlayer->getContentSize().width * 0.7,
			sprPlayer->getContentSize().height - sprPlayer->getContentSize().width * 0.7);

		BlockSprite* pSprRemoveBlock = NULL;
		for (BlockSprite* pSprBlock : vRain1)
		{
			if (pSprBlock->isVisible() == false)
				continue;

			Rect rectBlock = pSprBlock->getBoundingBox();
			if (rectPlayer.intersectsRect(rectBlock))
			{
				pSprRemoveBlock = pSprBlock;
				///////////////// 동시 데미지
				if (sprOther != NULL)
				{
					Rect rectOther = Rect(
						sprOther->getBoundingBox().origin.x + sprOther->getContentSize().width * 0.35,
						sprOther->getBoundingBox().origin.y + sprOther->getContentSize().width * 0.35,
						sprOther->getContentSize().width - sprOther->getContentSize().width * 0.7,
						sprOther->getContentSize().height - sprOther->getContentSize().width * 0.7);
					if (rectOther.intersectsRect(rectBlock))
						bOtherCrash = true;
				}
				////////
				break;
			}
		}


		if (pSprRemoveBlock != NULL)
		{
			if (player->bStarMode != true)
			{
				if (player->nCurrentPower > 0)
				{
					player->nCurrentPower -= pSprRemoveBlock->nDamage;;
				}
				else
				{
					player->nCurrentLife -= pSprRemoveBlock->nDamage;;
				}
			}

			///////////////// 동시 데미지
			if (other->bStarMode != true && bOtherCrash)
			{
				if (other->nCurrentPower > 0)
				{
					other->nCurrentPower -= pSprRemoveBlock->nDamage;;
				}
				else
				{
					other->nCurrentLife -= pSprRemoveBlock->nDamage;;
				}
			}
			////////////////////

				auto particle = ParticleSystemQuad::create(_particle_steam);
				particle->setScale(pSprRemoveBlock->getScale() * 8);
				particle->setPosition(pSprRemoveBlock->getPosition());
				pLayer->addChild(particle, 20);
				auto action = Sequence::create(DelayTime::create(1.0f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetPlayerCrashParticle, this)),
					NULL);
				particle->runAction(action);

				if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
				{
					if(player->bStarMode)
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_bubble);
					else
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Steam);

				}


			pSprRemoveBlock->stopAllActions();
			pSprRemoveBlock->setVisible(false);
		}

	}
}

void PlayFunc::UpdateCombine()
{
	for (int i = 0; i < vRain1.size(); i++)
	{
		BlockSprite* pRain1 = vRain1.at(i);
		if (pRain1->isVisible() == false)
		{
			continue;
		}

		Rect oRain1(pRain1->getBoundingBox());

		for (int j = 0; j < vRain1.size(); j++)
		{
			BlockSprite* pRain2 = vRain1.at(j);
			if (pRain2->isVisible() == false || (pRain1 == pRain2))
			{
				continue;
			}

			Rect oRain2(pRain2->getBoundingBox());
			if (oRain1.intersectsRect(oRain2))
			{
				BlockSprite* pRain = NULL;
				BlockSprite* pDeleteRain = NULL;
				if (pRain1->getScale() > pRain2->getScale())
				{
					pRain = pRain1;
					pDeleteRain = pRain2;
				}
				else if(pRain1->getScale() < pRain2->getScale())
				{
					pRain = pRain2;
					pDeleteRain = pRain1;
				}
				else
				{
					if (pRain1->getPositionY() > pRain2->getPositionY())
					{
						pRain = pRain1;
						pDeleteRain = pRain2;
					}
					else
					{
						pRain = pRain2;
						pDeleteRain = pRain1;
					}
				}



				pDeleteRain->stopAllActions();
				pDeleteRain->setVisible(false);

				pRain->setScale((pRain->getScaleX() + pDeleteRain->getScaleX()) );

				break;
			}
		}
	}

}

void PlayFunc::CheckGameOver()
{
	if (nTimeTick == 0)
	{
		bUpdateGameOver = true;

		//Clear
		Sprite* ClearStar = (Sprite*)pLayer->getChildByTag(GAME_CLEAR_STAR);
		if (ClearStar == NULL)
		{
			ClearStar = Sprite::createWithSpriteFrameName("star2.png");
			ClearStar->setPosition(visibleSize.width * 0.5, visibleSize.height * 0.6);
			ClearStar->setTag(GAME_CLEAR_STAR);
			pLayer->addChild(ClearStar, 30);

			auto action = Sequence::create(RotateTo::create(5.0f, 3600), NULL);
			ClearStar->runAction(action);

			auto label = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE * 3);
			label->setPosition(visibleSize.width * 0.5, visibleSize.height * 0.6);
			label->setTextColor(Color4B::RED);
			label->enableBold();
			label->enableOutline(Color4B::RED, 2);
			if (mdp.bEnglish)
				label->setString(en_clear);
			else {
				if(nDifficulty == 1)
					label->setString(UTF8PCHAR(ko_clear1));
				else if (nDifficulty == 2)
					label->setString(UTF8PCHAR(ko_clear2));
				else if (nDifficulty == 3)
					label->setString(UTF8PCHAR(ko_clear3));
				else if (nDifficulty == 4)
					label->setString(UTF8PCHAR(ko_clear4));
			}

			pLayer->addChild(label, 50);
		}

		ClearStar->setScale(0.3f* nGameoverTick);
		nGameoverTick++;
		if (nGameoverTick == 100)
		{
			if (nDifficulty > mdp.nClearLevel[nStageLv])
			{	
				mdp.nStarPoint = nDifficulty - mdp.nClearLevel[nStageLv];
				mdp.nClearLevel[nStageLv] = nDifficulty;
				mdp.SaveGameResultData(nStageLv);
			}

			if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_receive);

			ExitGame();
		}

		return;
	}

	if (nGameType == MENU_SOLO && p1.nCurrentLife <= 0)
	{
		bUpdateGameOver = true;
	}
	else if (nGameType == MENU_DOUBLE && p1.nCurrentLife <= 0 && p2.nCurrentLife <= 0)
	{
		bUpdateGameOver = true;
	}
	else if (nGameType == MENU_BATTLE && (p1.nCurrentLife <= 0 || p2.nCurrentLife <= 0))
	{
		bUpdateGameOver = true;
	}


	if (bUpdateGameOver == true)
	{
		nGameoverTick++;

		if (nGameoverTick == 50)
		{
			ExitGame();
		}

	}

}


void PlayFunc::ExitGame()
{
	bGameOver = true; // Using PlayScene
	vRain1.clear();
	EndBGM();
	mdInfo.nStageLv = -1;
}



bool PlayFunc::TouchInOut(Touch *touch, bool bTouch)
{
	if (bUpdateGameOver)
		return false;

	if (bStart == false)
		return false;

	Point touches = touch->getLocation();

	if (touches.y >= PLAYER_BASE_Y)
	{
		return false; //onTouchEnded가 호출되지 않음
	}


	auto sprP1 = (Label*)pLayer->getChildByTag(PLAYER_TAG1);
	auto sprP2 = (Label*)pLayer->getChildByTag(PLAYER_TAG2);

	if (nGameType == MENU_SOLO)
	{
		if (touches.x >= 0 && touches.x <= visibleSize.width && touches.y >= 0 && touches.y <= PLAYER_BASE_Y * 1.2)
		{

			p1.nDirection = p1.nDirection * -1;
			sprP1->setScaleX(p1.nDirection);
		}
	}
	else
	{
		if (touches.x >= 0 && touches.x <= visibleSize.width / 2 && touches.y >= 0 && touches.y <= PLAYER_BASE_Y * 1.2)
		{

			p1.nDirection = p1.nDirection * -1;
			sprP1->setScaleX(p1.nDirection);

		}
		else if (touches.x >= visibleSize.width / 2 && touches.x <= visibleSize.width && touches.y >= 0 && touches.y <= PLAYER_BASE_Y * 1.2)
		{

			p2.nDirection = p2.nDirection * -1;
			sprP2->setScaleX(p2.nDirection);

		}
	}

	return false;
}


bool PlayFunc::onTouchBegan(Touch *touch, Event *unused_event)
{
	return 	TouchInOut(touch, true);
}

void PlayFunc::onTouchMoved(Touch *touch, Event *unused_event)
{
	;
}

void PlayFunc::onTouchEnded(Touch *touch, Event *unused_event)
{
	;// TouchInOut(touch, false);
}

void PlayFunc::onTouchCancelled(Touch *touch, Event *unused_event)
{
	;// TouchInOut(touch, false);
}

void PlayFunc::update()
{
	if ( bStart == false )
	{
		return;
	}

	if (bCombineStage)
		UpdateCombine();

	if (bUpdateGameOver == false) //Game 종료 후 화면 출력 위해 사용
	{
		//스테이지
		Stage();

		bTurn = bTurn ? false : true;
		if (bTurn)
		{
			MoveCharacter(1);		//Player 이동
			MoveCharacter(2);
			CrashStar(1);
			CrashStar(2);
			CrashBlock(1);		//충돌 체크
			CrashBlock(2);
			UpdateProgress(1);	//EightMode체크
			UpdateProgress(2);
			UpdateCharacter(1); //캐릭터 애니메이션-죽음애니메이션-파티클
			UpdateCharacter(2);
		}
		else
		{
			MoveCharacter(2);		//Player 이동
			MoveCharacter(1);
			CrashStar(2);
			CrashStar(1);
			CrashBlock(2);
			CrashBlock(1);
			UpdateProgress(2);
			UpdateProgress(1);
			UpdateCharacter(2);
			UpdateCharacter(1);
		}


		UpdateStarLife(0);
		UpdateStarModeDelay();
	}

		
	//Check GameOver
	CheckGameOver();
}

