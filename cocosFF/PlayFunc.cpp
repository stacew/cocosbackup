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


void Player::init()
{

	nMaxLife = 1;
	nMaxPower = BASE_POWER;
	nIgniteTime = BASE_BURNTIME;
	nRecovery = BASE_RECOVERY;

	nCurrentLife = nMaxLife;
	nCurrentPower = nMaxPower;

	bChargeMode = false;
	bFireMode = true;
}

/////////////////////////////////////////////

void PlayFunc::init(Layer* a_pLayer)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
	pLayer = a_pLayer;
	director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	origin = director->getVisibleOrigin();
	p1.init();

	nSelectedChar = mdInfo.nSelectedChar;
	InitBG();
	InitProgress();
	InitCharacterSprite();
	InitPrevScore();
	InitPause();
	InitInfo();
	bStart = false;

	//////////////// add stage
	nSetBlock_CloudTick = 0;

	nPlayerPosition = 0;

	////////////////////////////////////
	bIsTouch = false;
	bBooster = false;
	bJump = false;
	nDownSpeed = 0.0f;
	nCrashSoundTick = 0;
	nBurnSoundTick = 0;
	nBurnSoundId = 0;

	bGameOver = false;
	bUpdateGameOver = false;
	nGameoverTick = 0;



}

void PlayFunc::InitPause()
{
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

}

void PlayFunc::Pause(Ref* pSender)
{
	if (bUpdateGameOver == true)
		return;

	if (mdp.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);


	auto menu = (Menu*)pLayer->getChildByTag(GAME_PAUSE);
	menu->setVisible(false);
	menu = (Menu*)pLayer->getChildByTag(GAME_RESUME);
	menu->setVisible(true);


	auto pauseBG = (Sprite*)pLayer->getChildByTag(GAME_PAUSE_HIDE);
	pauseBG->setVisible(true);

	director->pause();

}
void PlayFunc::Resume(Ref* pSender)
{
	if (mdp.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);


	auto menu = (Menu*)pLayer->getChildByTag(GAME_PAUSE);
	menu->setVisible(true);
	menu = (Menu*)pLayer->getChildByTag(GAME_RESUME);
	menu->setVisible(false);

	auto pauseBG = (Sprite*)pLayer->getChildByTag(GAME_PAUSE_HIDE);
	pauseBG->setVisible(false);


	director->resume();

}
void PlayFunc::PauseExit(Ref* pSender)
{
	if (mdp.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);

	director->resume();
	ExitGame();
}



void PlayFunc::InitBG()
{
	auto sprPauseBG = Sprite::createWithSpriteFrameName("BGpause.png");
	sprPauseBG->setAnchorPoint(Point::ZERO);
	sprPauseBG->setVisible(false);
	sprPauseBG->setTag(GAME_PAUSE_HIDE);
	pLayer->addChild(sprPauseBG, 99);


	auto node = ParallaxNode::create();
	pLayer->addChild(node, 0);

	auto actionMoveBy = MoveBy::create(25.0f, Point(-767, 0));
	auto actionPlace = Place::create(Point::ZERO);
	auto actionSequence = Sequence::create(actionMoveBy, actionPlace, NULL);
	auto actionRepeat = RepeatForever::create(actionSequence);
	node->runAction(actionRepeat);

	auto spr_back1_1 = Sprite::createWithSpriteFrameName("BG.png");
	spr_back1_1->setAnchorPoint(Point::ZERO);
	node->addChild(spr_back1_1, 0, Point(1, 0), Point::ZERO);
	auto spr_back1_2 = Sprite::createWithSpriteFrameName("BG.png");
	spr_back1_2->setAnchorPoint(Point::ZERO);
	node->addChild(spr_back1_2, 0, Point(1, 0), Point(767, 0));


	auto node2 = ParallaxNode::create();
	pLayer->addChild(node2, 99);

	auto actionMoveBy2 = MoveBy::create(25.0f, Point(-767, 0));
	auto actionPlace2 = Place::create(Point::ZERO);
	auto actionSequence2 = Sequence::create(actionMoveBy2, actionPlace2, NULL);
	auto actionRepeat2 = RepeatForever::create(actionSequence2);
	node2->runAction(actionRepeat2);

	auto spr_back2 = Sprite::createWithSpriteFrameName("BG_2.png");
	spr_back2->setAnchorPoint(Point::ZERO);
	node2->addChild(spr_back2, 2, Point(0, 0), Point(0, 0));

	auto spr_back3_1 = Sprite::createWithSpriteFrameName("BG_2.png");
	spr_back3_1->setAnchorPoint(Point::ZERO);
	node2->addChild(spr_back3_1, 1, Point(2, 0), Point(0, 0));
	auto spr_back3_2 = Sprite::createWithSpriteFrameName("BG_2.png");
	spr_back3_2->setAnchorPoint(Point::ZERO);
	node2->addChild(spr_back3_2, 1, Point(2, 0), Point(767, 0));
	auto spr_back3_3 = Sprite::createWithSpriteFrameName("BG_2.png");
	spr_back3_3->setAnchorPoint(Point::ZERO);
	node2->addChild(spr_back3_3, 1, Point(2, 0), Point(767 * 2, 0));
}


void PlayFunc::InitProgress()
{
	//Bar BG
	auto BarBG1 = Sprite::createWithSpriteFrameName("EmptyBar.png");
	BarBG1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	BarBG1->setPosition(Vec2(visibleSize.width * 0.05, visibleSize.height* 0.5));
	pLayer->addChild(BarBG1, 10);

	//Power Bar Progress
	auto spritePowerBar1 = Sprite::createWithSpriteFrameName("RedBar.png");
	auto progressPowerTimer1 = ProgressTimer::create(spritePowerBar1);	// ProgressTimer생성
	progressPowerTimer1->setType(ProgressTimer::Type::BAR);   // 타입 결정. Bar형태와 Radial(원형) 두 가지 타입이 있다.
	progressPowerTimer1->setMidpoint(Vec2(0, 0));                    // 중심점 잡아주기. 0, 0으로 하면 축소 시 우측에서 좌측으로 줄어든다.
	progressPowerTimer1->setBarChangeRate(Vec2(0, 1));          // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
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
	progressChargeTimer1->setBarChangeRate(Vec2(0, 1));          // 이거 안해주면 앵커포인트 0, 0을 향해 세로로도 줄어든다.
	progressChargeTimer1->setAnchorPoint(BarBG1->getAnchorPoint());
	progressChargeTimer1->setPosition(BarBG1->getPosition());
	progressChargeTimer1->setTag(CHARGE_BAR_TAG1);
	progressChargeTimer1->setPercentage(0.0f);
	pLayer->addChild(progressChargeTimer1, 0);


	BarBG1->setScale(0.6f);
	progressPowerTimer1->setScale(BarBG1->getScale());
	progressChargeTimer1->setScale(BarBG1->getScale());
}
void PlayFunc::InitCharacterSprite()
{
	
	Sprite* sprPlayer = NULL;
	if (nSelectedChar == CHAR_1)
		sprPlayer = Sprite::createWithSpriteFrameName("char1_0.png");
	else if( nSelectedChar == CHAR_2 )
		sprPlayer = Sprite::createWithSpriteFrameName("char2_0.png");
	sprPlayer->setScale(CHAR_BASE_SCALE);
	sprPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sprPlayer->setPosition(Vec2(visibleSize.width * 0.2, visibleSize.height * 0.75));
	sprPlayer->setTag(PLAYER_TAG1);
	pLayer->addChild(sprPlayer, 3);

	{//char ani start
		auto animation = Animation::create();
		animation->setDelayPerUnit(CHAR_ANI_DELAY);
		for (int i = 0; i < 2; i++)
		{
			if (nSelectedChar == CHAR_1)
				animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("char1_%d.png", i + 1)));//1,2
			else if (nSelectedChar == CHAR_2)
				animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("char2_%d.png", i + 1)));//1,2
		}
		sprPlayer->runAction(RepeatForever::create(Animate::create(animation)));
	}

	Sprite* spriteDie = NULL;
	if (nSelectedChar == CHAR_1)
		spriteDie = Sprite::createWithSpriteFrameName("char1_die.png");
	else if (nSelectedChar == CHAR_2)
		spriteDie = Sprite::createWithSpriteFrameName("char2_die.png");

	spriteDie->setScale(CHAR_BASE_SCALE);
	spriteDie->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	spriteDie->setPosition(sprPlayer->getPosition());
	spriteDie->setTag(PLAYER_DIE_TAG1);
	spriteDie->setVisible(false);
	pLayer->addChild(spriteDie, 3);

}


void PlayFunc::InitPrevScore()
{
	auto prevComboLabel = Label::createWithTTF(StringUtils::format("+ %d", mdp.nMaxCombo), _fonts_NanumPen, IN_GAME_FONT_SIZE * 1.2);
	prevComboLabel->setTag(STAGE_PREV_COMBO_LABEL);
	prevComboLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	prevComboLabel->setTextColor(Color4B::WHITE);
	prevComboLabel->enableOutline(Color4B::YELLOW, 1);
	prevComboLabel->setPosition(visibleSize.width * 0.85, visibleSize.height * 0.92);
	pLayer->addChild(prevComboLabel);

	auto prevPointLabel = Label::createWithTTF(StringUtils::format("= %d", mdp.nMaxPoint), _fonts_NanumPen, IN_GAME_FONT_SIZE * 1.2 );
	prevPointLabel->setTag(STAGE_PREV_POINT_LABEL);
	prevPointLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	prevPointLabel->setTextColor(Color4B::WHITE);
	prevPointLabel->enableOutline(Color4B::YELLOW, 1);
	prevPointLabel->setPosition(visibleSize.width * 0.85, visibleSize.height * 0.87);
	pLayer->addChild(prevPointLabel);

}
void PlayFunc::InitInfo()
{
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
}


void PlayFunc::UpdateProgress(int PlayerNum)
{
	auto progressPowerTimer = (ProgressTimer*)pLayer->getChildByTag(POWER_BAR_TAG1);
	auto progressChargeTimer = (ProgressTimer*)pLayer->getChildByTag(CHARGE_BAR_TAG1);
	auto sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
	Player* pPlayer = &p1;


	////////////////////////////////////// 1p
	if (progressPowerTimer != NULL)
	{
		if (pPlayer->nCurrentLife <= 0)
		{
			pPlayer->bChargeMode = false;
			progressChargeTimer->stopAllActions();

			pPlayer->bFireMode = false;
		}
		else if (pPlayer->nCurrentLife > 0 && pPlayer->nCurrentPower <= 0 && pPlayer->bChargeMode == false)
		{
			pPlayer->bChargeMode = true;
			progressChargeTimer->setLocalZOrder(12);
			auto progressFromTo = ProgressFromTo::create(pPlayer->nIgniteTime, 0, 100.0f); // 퍼센트 수정 액션 방법
			progressChargeTimer->runAction(progressFromTo);

			pPlayer->bFireMode = false;


		}
		else if (pPlayer->bChargeMode == true && progressChargeTimer->getPercentage() == 100.0f) //Fire 발동시
		{
			pPlayer->bChargeMode = false;
			progressChargeTimer->setLocalZOrder(0);

			pPlayer->bFireMode = true;
			//pPlayer->nCurrentLife = pPlayer->nMaxLife;
			pPlayer->nCurrentPower = pPlayer->nMaxPower;

			if (mdp.bSoundSwitch == true)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_EightFire);

			if (sprPlayer != NULL)
			{
				auto particle = ParticleSystemQuad::create(_particle_char_fire);
				particle->setScale(sprPlayer->getScale()*4);
				particle->setPosition(sprPlayer->getPosition());
				pLayer->addChild(particle, 20);
				auto action = Sequence::create(DelayTime::create(2.0f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetParticle, this)),
					NULL);
				particle->runAction(action);
			}


		}


		if (pPlayer->nCurrentPower > 0 )
		{
			float nRecovery = p1.nRecovery;
			if (nSelectedChar == CHAR_2 && bBooster)
				nRecovery *= 32;
			pPlayer->nCurrentPower += nRecovery;
			if (pPlayer->nCurrentPower > pPlayer->nMaxPower)
				pPlayer->nCurrentPower = pPlayer->nMaxPower;
		}

		float PowerPercent = (pPlayer->nCurrentPower / pPlayer->nMaxPower) * 100.0f;
		progressPowerTimer->setPercentage(PowerPercent);


	}

}


void PlayFunc::Stage()
{
	Stage_Label();//process to jump
	Stage_SetBlock();
}

void PlayFunc::Stage_Label()
{
	auto pointLabel = (Label*)pLayer->getChildByTag(STAGE_POINT_LABEL);
	if (pointLabel == NULL)
	{
		pointLabel = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE * 2);
		pointLabel->setTag(STAGE_POINT_LABEL);
		pointLabel->setTextColor(Color4B::WHITE);
		pointLabel->enableOutline(Color4B::RED, 2);
		pointLabel->setPosition(visibleSize.width * 0.5, visibleSize.height * 0.5);
		pLayer->addChild(pointLabel);
	}
	pointLabel->setString(StringUtils::format("= %d", nPoint));

	auto comboLabel = (Label*)pLayer->getChildByTag(STAGE_COMBO_LABEL);
	if (comboLabel == NULL)
	{
		comboLabel = Label::createWithTTF("", _fonts_NanumPen, IN_GAME_FONT_SIZE);
		comboLabel->setTag(STAGE_COMBO_LABEL);
		comboLabel->setTextColor(Color4B::WHITE);
		comboLabel->enableOutline(Color4B::RED, 2);
		pLayer->addChild(comboLabel);
	}

	if (nCombo == 0)
		comboLabel->setVisible(false);
	else
	{
		comboLabel->setVisible(true);
		Sprite* sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		comboLabel->setPosition(sprPlayer->getPosition().x + 100 ,sprPlayer->getPosition().y);
		comboLabel->setString(StringUtils::format("+ %d", nCombo));	
	}

}

void PlayFunc::ResetSprite(Ref* sender)
{
	auto spr = (BlockSprite*)sender;
	spr->setVisible(false);
}


void PlayFunc::Stage_SetBlock()
{

	float nTick = 1 + nCombo * 0.1f + nPoint * 0.001f;
	
	nSetBlock_CloudTick += nTick;

	if (nSetBlock_CloudTick > 120)
	{
		SetBlock(0);
		nSetBlock_CloudTick = 0;
		nPoint += nCombo / 10;
		nPoint += nPoint / 1000;
	}

}


BlockSprite* PlayFunc::getInvisibleBlock(int nType)
{
	BlockSprite* block = NULL;
	if (nType == 0)
	{
		int k;
		for (k = 0; k < vCloud1.size(); k++)
		{
			block = vCloud1.at(k);
			if (block->isVisible() == false)
			{
				block->setVisible(true);
				break;
			}
		}

		//없으면 새로 생성
		if (k == vCloud1.size())
		{
			block = BlockSprite::createWithSpriteFrameName("Cloud1.png");
			vCloud1.pushBack(block);
			block->setVisible(true);
			pLayer->addChild(block, 2);
		}

		block->nDamage = 1.0f;

	}
	else if (nType == 1)
	{
		;
	}

	return block;
}

void PlayFunc::SetBlock(int nLevel)
{
	bool bCloud = false;
	bool bWave = false;

	if (nLevel == 0)
		bCloud = true;
	else if (nLevel == 1)
		bWave = true;

	float createPositionY, createPositionX;

	if (bWave)
	{
		createPositionY = 0;
		createPositionX = visibleSize.width * 0.7 + visibleSize.width * 0.5 * CCRANDOM_0_1();
	}
	else if (bCloud)
	{
		createPositionX = visibleSize.width + 100;
		auto sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
		if (nPlayerPosition > 20 && sprPlayer != NULL && p1.nCurrentLife > 0)
		{
			nPlayerPosition = 0;
			createPositionY = sprPlayer->getPositionY();
		}
		else
		{
			createPositionY = CCRANDOM_0_1() * visibleSize.height;
		}

		nPlayerPosition++;
	}

	BlockSprite* block = NULL;
	if (bCloud)
		block = getInvisibleBlock(0);
	else
		block = getInvisibleBlock(1);

	block->nSpeed = 6.0f;
	block->setScale(0.2f);
	block->setPosition(createPositionX, createPositionY);

	if (bWave)
	{
		float nHeight = visibleSize.height / 3 +  CCRANDOM_0_1() * visibleSize.height / 2;
		auto action = Sequence::create(
			JumpBy::create(block->nSpeed, Vec2(-(visibleSize.width + 200), 0), nHeight, 2),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
			NULL);
		block->runAction(action);
	}
	else if (bCloud)
	{
		auto action = Sequence::create(
			MoveBy::create(block->nSpeed, Vec2(-(visibleSize.width + 200), 0)),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
			NULL);
		block->runAction(action);
	}
}

void PlayFunc::resetParticle(Ref* sender)
{
	auto particle = (ParticleSystemQuad*)sender;
	pLayer->removeChild(particle);
}


//충돌 체크 코드
void PlayFunc::CrashBlock(int PlayerNum)
{
	Sprite* sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);

	if ( sprPlayer->getBoundingBox().getMaxY() > BASE_TOP)
	{
		nCrashSoundTick++;
		if (mdp.bSoundSwitch == true && nCrashSoundTick >= 0)
		{
			nCrashSoundTick = -5;
			if( nSelectedChar == CHAR_1 && bBooster)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Steam);
		}
		auto particle = ParticleSystemQuad::create(_particle_steam);
		particle->setScale(sprPlayer->getScale() * 2);
		particle->setPosition(sprPlayer->getBoundingBox().getMidX(), sprPlayer->getBoundingBox().getMaxY());
		pLayer->addChild(particle, 100);
		auto action = Sequence::create(DelayTime::create(2.0f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetParticle, this)),
			NULL);
		particle->runAction(action);
		if (nSelectedChar == CHAR_1 && bBooster == true)
		{
			;
		}
		else
		{
			if (p1.nCurrentPower > 0)
			{
				p1.nCurrentPower -= 0.1f;
			}
			else
			{
				p1.nCurrentLife -= 1;
			}

			if (nMaxCombo < nCombo)
				nMaxCombo = nCombo;
			nCombo = 0;
		}


	}

	if ( sprPlayer->getBoundingBox().getMinY() < BASE_BOTTOM)
	{
		nCrashSoundTick++;
		if (mdp.bSoundSwitch == true && nCrashSoundTick >= 0)
		{
			nCrashSoundTick = -5;
			if (nSelectedChar == CHAR_1 && bBooster)
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Steam);
		}

		auto particle = ParticleSystemQuad::create(_particle_steam);
		particle->setScale(sprPlayer->getScale() * 2);
		particle->setPosition(sprPlayer->getBoundingBox().getMidX(), sprPlayer->getBoundingBox().getMinY());
		pLayer->addChild(particle, 100);
		auto action = Sequence::create(DelayTime::create(2.0f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetParticle, this)),
			NULL);
		particle->runAction(action);

		if (nSelectedChar == CHAR_1 && bBooster == true)
		{
			;
		}
		else
		{
			if (p1.nCurrentPower > 0)
			{
				p1.nCurrentPower -= 0.1f;
			}
			else
			{
				p1.nCurrentLife -= 1;
			}
			if (nMaxCombo < nCombo)
				nMaxCombo = nCombo;
			nCombo = 0;
		}


	}

	if (sprPlayer != NULL && p1.nCurrentLife > 0)
	{

		Rect rectPlayer = sprPlayer->getBoundingBox();

		BlockSprite* pSprRemoveBlock = NULL;
		for (BlockSprite* pSprBlock : vCloud1)
		{
			if (pSprBlock->isVisible() == false)
				continue;

			float nXSize = pSprBlock->getBoundingBox().getMaxX() - pSprBlock->getBoundingBox().getMinX();
			float nYSize = pSprBlock->getBoundingBox().getMaxY() - pSprBlock->getBoundingBox().getMinY();
			
			Rect rectBlock = Rect(
				pSprBlock->getBoundingBox().origin.x + nXSize * 0.35,
				pSprBlock->getBoundingBox().origin.y + nYSize * 0.35,
				nXSize - nXSize * 0.7,
				nYSize - nYSize * 0.7);


			if (rectPlayer.intersectsRect(rectBlock))
			{
				pSprRemoveBlock = pSprBlock;
				break;
			}
		}


		if (pSprRemoveBlock != NULL)
		{
			if (nSelectedChar == CHAR_1 && bBooster == true)
			{
				;
			}
			else
			{
				if (p1.nCurrentPower > 0)
				{
					p1.nCurrentPower -= pSprRemoveBlock->nDamage;;
				}
				else
				{
					p1.nCurrentLife -= pSprRemoveBlock->nDamage;;
				}

				if (nMaxCombo < nCombo)
					nMaxCombo = nCombo;
				nCombo = 0;
			}


			auto particle = ParticleSystemQuad::create(_particle_steam);
			particle->setScale(sprPlayer->getScale() * 2);
			particle->setPosition(pSprRemoveBlock->getPosition());
			pLayer->addChild(particle, 100);
			auto action = Sequence::create(DelayTime::create(1.0f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetParticle, this)),
				NULL);
			particle->runAction(action);

			if (mdp.bSoundSwitch == true)
			{
				if (nSelectedChar == CHAR_1 && bBooster)
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
				else
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Steam);
			}


			pSprRemoveBlock->stopAllActions();
			pSprRemoveBlock->setVisible(false);
		}

	}

}

void PlayFunc::CheckGameOver()
{
	auto sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
	auto sprDiePlayer = (Sprite*)pLayer->getChildByTag(PLAYER_DIE_TAG1);

	if (p1.nCurrentLife <= 0) // Lose
	{
		bUpdateGameOver = true;
		if (sprDiePlayer->isVisible() == false)
		{
			sprDiePlayer->setVisible(true);
			sprDiePlayer->setPosition(sprPlayer->getPosition());
			sprDiePlayer->setScale(sprPlayer->getScale());
			sprPlayer->setVisible(false);
		}
		else
		{
			sprPlayer->setPosition(sprDiePlayer->getPosition());
		}


		nDownSpeed -= G_ACCEL;
		auto pMoveDown = MoveBy::create(0, Vec2(0, nDownSpeed));
		sprDiePlayer->runAction(pMoveDown);

	}

	if (sprDiePlayer->getBoundingBox().getMaxY() < 0)
	{
		ExitGame();
	}

	if (sprPlayer->getBoundingBox().getMaxY() < 0)
	{
		ExitGame();
	}

}


void PlayFunc::ExitGame()
{
	bGameOver = true; // Using PlayScene
	vCloud1.clear();
	mdInfo.nMaxCombo = nMaxCombo;
	mdInfo.nMaxPoint = nPoint;
}




void PlayFunc::MoveCharacter(int PlayerNum)
{
	Sprite* sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);

	if (nSelectedChar == CHAR_2 && 	sprPlayer->getScale() > CHAR_2_BASE_SCALE)
	{
		sprPlayer->setScale(sprPlayer->getScale() *  0.99f);
	}

	if (bJump == true)
	{
		nCombo++;
		nPoint += nCombo;
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(nBurnSoundId);
		nBurnSoundTick = 0;

		nDownSpeed = 0.0f;

		float nJump = PLAYER_JUMP;
		if (BASE_TOP <= sprPlayer->getPosition().y + nJump)
		{
			nJump = BASE_TOP - sprPlayer->getPosition().y;
		}

		sprPlayer->runAction(MoveBy::create(0.15f, Vec2(0, nJump)));

		auto particleJump = ParticleSystemQuad::create(_particle_jump);
		particleJump->setScale(sprPlayer->getScale() * 2);
		particleJump->setPosition(sprPlayer->getBoundingBox().getMidX(), sprPlayer->getBoundingBox().getMinY());
		pLayer->addChild(particleJump, 20);
		auto action2 = Sequence::create(DelayTime::create(0.5f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetParticle, this)),
			NULL);

		particleJump->runAction(action2);

		if (mdp.bSoundSwitch == true)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
		}

	}
	else if (bBooster == true)
	{	
		nDownSpeed = 0.0f;

		if (nSelectedChar == CHAR_1)
		{
			auto particleBooster = ParticleSystemQuad::create(_particle_booster);
			particleBooster->setScale(sprPlayer->getScale() * 4);
			particleBooster->setPosition(sprPlayer->getPosition());
			pLayer->addChild(particleBooster, 99);
			auto action2 = Sequence::create(DelayTime::create(0.3f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::resetParticle, this)),
				NULL);
			particleBooster->runAction(action2);

			
			p1.nCurrentPower -= 0.005f;
		}
		else if (nSelectedChar == CHAR_2)
		{
			sprPlayer->setScale(sprPlayer->getScale() * 1.08f);
		}

		nBurnSoundTick++;
		if (mdp.bSoundSwitch == true && nBurnSoundTick >= 0)
		{
			nBurnSoundTick = -50;
			nBurnSoundId = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_burn);
		}
	}
	else
	{

		CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(nBurnSoundId);
		nBurnSoundTick = 0;

		nDownSpeed -= G_ACCEL;

		//auto pMoveDown = MoveBy::create(0, Vec2(0, nDownSpeed));
		//sprPlayer->runAction(pMoveDown); //rake... 
		sprPlayer->setPosition(sprPlayer->getPosition().x, sprPlayer->getPosition().y + nDownSpeed);
	}

}

void PlayFunc::UpdateCharacter(int PlayerNum)
{
	Sprite* sprPlayer = (Sprite*)pLayer->getChildByTag(PLAYER_TAG1);
	Sprite* sprDiePlayer = (Sprite*)pLayer->getChildByTag(PLAYER_DIE_TAG1);
	/////////////////////////////////// Fire Mode Update;
	auto particleEightMode = (ParticleSystemQuad*)pLayer->getChildByTag(PARTICLE_EIGHT1);
	if (particleEightMode == NULL)
	{
		particleEightMode = ParticleSystemQuad::create(_particle_char_eightmode);
		particleEightMode->setTag(PARTICLE_EIGHT1);
		particleEightMode->setScale(sprPlayer->getScale() * 3);
		pLayer->addChild(particleEightMode, 0);
	}
	particleEightMode->setPosition(sprPlayer->getPosition());
	if (p1.bFireMode == true)
	{
		particleEightMode->setVisible(true);
		particleEightMode->setScale(sprPlayer->getScale() * 3);
	}
	else
	{
		particleEightMode->setVisible(false);
	}
	////////////////////////////////
}

void PlayFunc::CheckJumpAndBooster()
{
	if (bIsTouch == true)
	{
		nTouchTick++;
	}


	bBooster = false;
	bJump = false;
	if (nTouchTick == 0)
	{
		;
	}
	else if (bIsTouch == true && nTouchTick >= 15 && p1.nCurrentPower > 0)
	{
		bBooster = true;
	}
	else if (bIsTouch == false && nTouchTick < 15)
	{
		bJump = true;
	}


	if (bIsTouch == false)
	{
		nTouchTick = 0;
	}
}


bool PlayFunc::TouchInOut(Touch *touch, bool bTouch)
{
	if (bStart == false)
	{
		auto sprInfo = (Sprite*)pLayer->getChildByTag(GAME_INFO);
		sprInfo->setVisible(false);
		bStart = true;
	}
		if (bGameOver)
			return false;


		Point touches = touch->getLocation();


		if (bTouch == true)
		{
			bIsTouch = true;
		}
		else if (bTouch == false)
		{
			bIsTouch = false;
		}


		return true;
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
	TouchInOut(touch, false);
}

void PlayFunc::onTouchCancelled(Touch *touch, Event *unused_event)
{
	 TouchInOut(touch, false);
}

void PlayFunc::update()
{
	if (bStart == false)
		return;

	if (bUpdateGameOver == false) //Game 종료 후 화면 출력 위해 사용
	{
		//스테이지
		Stage(); // 타이머 + 블락 생성 + 아이템 생성
		CheckJumpAndBooster(); //
		MoveCharacter(1);//Player 이동
	}
	CrashBlock(1);	//충돌 체크
	UpdateProgress(1);	//EightMode체크
	UpdateCharacter(1); // char EightMode particle


	//Check GameOver
	CheckGameOver();
}

