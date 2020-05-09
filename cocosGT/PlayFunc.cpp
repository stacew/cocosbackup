#include "PlayFunc.h"
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

/////////////////////////////////////////////

void PlayFunc::Back(Ref* pSender)
{
	/*
	if (nPoint > MAX_GQ)
		nPoint = MAX_GQ;

	if (nGameType == MENU_SPEED)
	{
		if (nRemainTime < 60 * 7)
			mdp.nTest[MENU_SPEED - MENU_BASE] = nPoint;
	}
	else if (nGameType == MENU_REACTIVE)
	{
		if (nRemainTest < 5)
			mdp.nTest[MENU_REACTIVE - MENU_BASE] = nPoint;
	}
	else if (nGameType == MENU_TIMING)
	{
		if (nRemainTouch < 7)
			mdp.nTest[MENU_TIMING - MENU_BASE] = nPoint;
	}
	else if (nGameType == MENU_ACCURACY)
	{
		if (nACRemainGreen < AC_MAX_GREEN / 2)
			mdp.nTest[MENU_ACCURACY - MENU_BASE] = nPoint;
	}
	else if (nGameType == MENU_DYNAMIC_VISUAL)
	{
		if (nDV_Stage > 7)
			mdp.nTest[MENU_DYNAMIC_VISUAL - MENU_BASE] = nPoint;
	}
	*/

	ExitGame();
}

void PlayFunc::ExitGame()
{
	bGameOver = true; // Using PlayScene
	mdp.SaveGameResultData();
	vBlue.clear();
	vRed.clear();
	vGreen.clear();
}

void PlayFunc::ResetSprite(Ref* sender)
{
	auto spr = (BlockSprite*)sender;
	spr->setVisible(false);
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



void PlayFunc::init(Layer* a_pLayer)
{
	srand(time(NULL));

	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
	pLayer = a_pLayer;
	director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	//initBG()
	{
		auto bg = Sprite::createWithSpriteFrameName("bg.png");
		bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		pLayer->addChild(bg, 0);
	}
	//initBackButtonLabel()
	{
		auto back = Sprite::createWithSpriteFrameName("back.png");
		auto back_s = Sprite::createWithSpriteFrameName("back_s.png");
		auto backItem = MenuItemSprite::create(back, back_s, CC_CALLBACK_1(PlayFunc::Back, this));
		auto backMenu = Menu::create(backItem, NULL);
		backMenu->setPosition(visibleSize.width*0.05, visibleSize.height*0.95);
		pLayer->addChild(backMenu, 6);
	}
	nGameType = mdInfo.nGameType;



	//speed
	if (nGameType == MENU_SPEED)
	{
		nRemainTime = 60 * 15;
		nNewCount = 0;
		SpeedNewPosition();
	}

	//reactive
	if (nGameType == MENU_REACTIVE)
	{
		nRemainTest = 10;
		nReactiveTick = 0;
	}

	//timing
	if (nGameType == MENU_TIMING)
	{
		nRemainTouch = 15;
		nTimingTick = 0;
		nTimingPoint = 1;
		nPrevTouch = 0;
	}

	//accuracy
	if (nGameType == MENU_ACCURACY)
	{
		nACRemainGreen = AC_MAX_GREEN;
		nACTick = 0;
	}


	//DV
	if (nGameType == MENU_DYNAMIC_VISUAL)
	{
		auto circle = Sprite::createWithSpriteFrameName("circle.png");
		auto circle_s = Sprite::createWithSpriteFrameName("circle_s.png");
		auto circleItem = MenuItemSprite::create(circle, circle_s, CC_CALLBACK_1(PlayFunc::DVButton, this));
		circleItem->setTag(BUTTON_CIRCLE);
		circleItem->setScale(0.5f);

		auto cross = Sprite::createWithSpriteFrameName("cross.png");
		auto cross_s = Sprite::createWithSpriteFrameName("cross_s.png");
		auto crossItem = MenuItemSprite::create(cross, cross_s, CC_CALLBACK_1(PlayFunc::DVButton, this));
		crossItem->setTag(BUTTON_CROSS);
		crossItem->setScale(0.5f);

		auto heart = Sprite::createWithSpriteFrameName("heart.png");
		auto heart_s = Sprite::createWithSpriteFrameName("heart_s.png");
		auto heartItem = MenuItemSprite::create(heart, heart_s, CC_CALLBACK_1(PlayFunc::DVButton, this));
		heartItem->setTag(BUTTON_HEART);
		heartItem->setScale(0.5f);

		auto leftMenu = Menu::create(circleItem, crossItem, heartItem, NULL);
		leftMenu->alignItemsVerticallyWithPadding(25);
		leftMenu->setPosition(visibleSize.width*0.1, visibleSize.height*0.3);
		pLayer->addChild(leftMenu, 6);


		auto triangle = Sprite::createWithSpriteFrameName("triangle.png");
		auto triangle_s = Sprite::createWithSpriteFrameName("triangle_s.png");
		auto triangleItem = MenuItemSprite::create(triangle, triangle_s, CC_CALLBACK_1(PlayFunc::DVButton, this));
		triangleItem->setTag(BUTTON_TRIANGLE);
		triangleItem->setScale(0.5f);

		auto square = Sprite::createWithSpriteFrameName("square.png");
		auto square_s = Sprite::createWithSpriteFrameName("square_s.png");
		auto squareItem = MenuItemSprite::create(square, square_s, CC_CALLBACK_1(PlayFunc::DVButton, this));
		squareItem->setTag(BUTTON_SQUARE);
		squareItem->setScale(0.5f);

		auto rhombus = Sprite::createWithSpriteFrameName("rhombus.png");
		auto rhombus_s = Sprite::createWithSpriteFrameName("rhombus_s.png");
		auto rhombusItem = MenuItemSprite::create(rhombus, rhombus_s, CC_CALLBACK_1(PlayFunc::DVButton, this));
		rhombusItem->setTag(BUTTON_RHOMBUS);
		rhombusItem->setScale(0.5f);

		auto rightMenu = Menu::create(triangleItem, squareItem, rhombusItem, NULL);
		rightMenu->alignItemsVerticallyWithPadding(25);
		rightMenu->setPosition(visibleSize.width*0.9, visibleSize.height*0.3);
		pLayer->addChild(rightMenu, 6);


		bExplain = true;
		bNeedNewBlock = false;
		nReplayCount = 0;

		auto dvExplain = Sprite::createWithSpriteFrameName("dvExplain.png");
		dvExplain->setTag(DV_EXPLAIN);
		dvExplain->setPosition(0, 0);
		dvExplain->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		pLayer->addChild(dvExplain, 100);
	}

	nPoint = 0;
	bUpdateGameOver = false;
	bGameOver = false;
}

void PlayFunc::ACTest()
{
	auto questionLabel = (Label*)pLayer->getChildByTag(QUESTION_LABEL);
	if (questionLabel == NULL)
	{
		questionLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		questionLabel->setTag(QUESTION_LABEL);
		questionLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		questionLabel->setTextColor(Color4B::YELLOW);
		questionLabel->setPosition(visibleSize.width* 0.95, visibleSize.height);
		pLayer->addChild(questionLabel);
	}
	questionLabel->setString(StringUtils::format("Remain Green : %2d", (int)nACRemainGreen));

	auto pointLabel = (Label*)pLayer->getChildByTag(POINT_LABEL);
	if (pointLabel == NULL)
	{
		pointLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		pointLabel->setTag(POINT_LABEL);
		pointLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		pointLabel->setTextColor(Color4B::WHITE);
		pointLabel->setPosition(visibleSize.width* 0.95, visibleSize.height - 40);
		pLayer->addChild(pointLabel);
	}
	pointLabel->setString(StringUtils::format("Total Point : %2d", nPoint));

	if (nACRemainGreen <= 0)
	{
		for (int i = 0; i < vGreen.size(); i++)
		{
			BlockSprite* green = vGreen.at(i);
			if (green->isVisible() == true)
				return;
		}

		mdp.nTest[MENU_ACCURACY - MENU_BASE] = nPoint;
		ExitGame();
	}

	nACTick--;

	if (nACTick <= 0)
	{
		nACTick = AC_TICK_BASE + nACRemainGreen / 15;

		bool bGreen = true;
		if (CCRANDOM_0_1() < 0.4f)
		{
			bGreen = false;
		}

		BlockSprite* block = NULL;
		if (bGreen)
			block = getInvisibleBlock(0);
		else
			block = getInvisibleBlock(1);

		block->setScale(0.8f);////////////////
		auto action = Sequence::create(
			ScaleTo::create(0.5f + nACRemainGreen / 200 , 0),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetSprite, this)),
			NULL);
		block->runAction(action);


		bool bCreate = false;
		while (bCreate == false)
		{
			int nRand = (int)(CCRANDOM_0_1() * 9);
			if (nRand > 8)
				nRand = 8;

			block->setPosition(GetPoint(nRand));

			/////////////////// check position
			bCreate = true;
			Rect rectBlock = block->getBoundingBox();
			for (int i = 0; i < vGreen.size(); i++)
			{
				BlockSprite* green = vGreen.at(i);
				if (green == block || green->isVisible() == false)
					continue;

				if (green->getBoundingBox().intersectsRect(rectBlock) == true)
				{
					bCreate = false;
					break;
				}
			}

			if (bCreate == true)
			{
				for (int i = 0; i < vRed.size(); i++)
				{
					BlockSprite* red = vRed.at(i);
					if (red == block || red->isVisible() == false)
						continue;

					if (red->getBoundingBox().intersectsRect(rectBlock) == true)
					{
						bCreate = false;
						break;
					}
				}
			}
			///////////////////////
		}

		if (bGreen)
			nACRemainGreen--;
	}
}


Point PlayFunc::GetPoint(int nNum)
{
	Point p;
	float vx = visibleSize.width;
	float vy = visibleSize.height;
	switch (nNum)
	{
	case 0:
		p.x = vx * 0.2;
		p.y = vy * 0.7;
		break;
	case 1:
		p.x = vx * 0.5;
		p.y = vy * 0.7;
		break;
	case 2:
		p.x = vx * 0.8;
		p.y = vy * 0.7;
		break;
	case 3:
		p.x = vx * 0.2;
		p.y = vy * 0.4;
		break;
	case 4:
		p.x = vx * 0.5;
		p.y = vy * 0.4;
		break;
	case 5:
		p.x = vx * 0.8;
		p.y = vy * 0.4;
		break;
	case 6:
		p.x = vx * 0.2;
		p.y = vy * 0.1;
		break;
	case 7:
		p.x = vx * 0.5;
		p.y = vy * 0.1;
		break;
	case 8:
		p.x = vx * 0.8;
		p.y = vy * 0.1;
		break;
	default:
		p.x = vx * 0.5;
		p.y = vy * 0.4;
		break;
	}

	return p;
}


void PlayFunc::SpeedTest()
{
	nRemainTime--;
	auto questionLabel = (Label*)pLayer->getChildByTag(QUESTION_LABEL);
	if (questionLabel == NULL)
	{
		questionLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		questionLabel->setTag(QUESTION_LABEL);
		questionLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		questionLabel->setTextColor(Color4B::YELLOW);
		questionLabel->setPosition(visibleSize.width* 0.95, visibleSize.height);
		pLayer->addChild(questionLabel);
	}
	questionLabel->setString(StringUtils::format("Time : %2d", nRemainTime / 60));

	auto pointLabel = (Label*)pLayer->getChildByTag(POINT_LABEL);
	if (pointLabel == NULL)
	{
		pointLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		pointLabel->setTag(POINT_LABEL);
		pointLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		pointLabel->setTextColor(Color4B::WHITE);
		pointLabel->setPosition(visibleSize.width *0.95, visibleSize.height - 40);
		pLayer->addChild(pointLabel);
	}
	pointLabel->setString(StringUtils::format("Total Point : %2d", nPoint));


	if (nRemainTime <= 0)
	{
		if (nPoint > MAX_GQ)
			nPoint = MAX_GQ;

		mdp.nTest[MENU_SPEED - MENU_BASE] = nPoint;
		ExitGame();
	}

}
void PlayFunc::SpeedNewPosition()
{
	float nRand = CCRANDOM_0_1();
	int nTemp;
	if (nRand > 0.75f)
		nTemp = 0;
	else if (nRand > 0.5f)
		nTemp = 1;
	else if (nRand > 0.25f)
		nTemp = 2;
	else
		nTemp = 3;

	if (nTemp == nPosition)
	{
		nPosition = (nPosition + 1) % 4;
	}
	else
	{
		nPosition = nTemp;
	}


	Sprite* green = (Sprite*)pLayer->getChildByTag(SPEED_GREEN);
	if (green == NULL)
	{
		green = Sprite::createWithSpriteFrameName("green.png");
		green->setTag(SPEED_GREEN);
		pLayer->addChild(green, 0);
	}


	if (nPosition == 0)
	{
		green->setPosition(visibleSize.width * 0.25, visibleSize.height*0.75);
	}
	else if (nPosition == 1)
	{
		green->setPosition(visibleSize.width * 0.75, visibleSize.height*0.75);
	}
	else if (nPosition == 2)
	{
		green->setPosition(visibleSize.width * 0.25, visibleSize.height*0.25);
	}
	else
	{
		green->setPosition(visibleSize.width * 0.75, visibleSize.height*0.25);
	}

	float nGreenScale = 1.5f;
	nGreenScale = nGreenScale - 0.075f * nNewCount;
	green->setScale(nGreenScale);

	nNewCount++;
}

void PlayFunc::ReactiveTest()
{

	if (nRemainTest == 0)
	{
		mdp.nTest[MENU_REACTIVE - MENU_BASE] = nPoint;
		ExitGame();
	}


	Sprite* green = (Sprite*)pLayer->getChildByTag(REACTIVE_GREEN);
	if (green == NULL)
	{
		green = Sprite::createWithSpriteFrameName("green.png");
		green->setTag(REACTIVE_GREEN);
		green->setVisible(false);
		green->setScale(0.0f);
		green->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
		pLayer->addChild(green, 2);
	}

	Sprite* red = (Sprite*)pLayer->getChildByTag(REACTIVE_RED);
	if (red == NULL)
	{
		red = Sprite::createWithSpriteFrameName("red.png");
		red->setTag(REACTIVE_RED);
		red->setVisible(false);
		red->setScale(0.0f);
		red->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
		pLayer->addChild(red, 2);
	}

	auto questionLabel = (Label*)pLayer->getChildByTag(QUESTION_LABEL);
	if (questionLabel == NULL)
	{
		questionLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		questionLabel->setTag(QUESTION_LABEL);
		questionLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		questionLabel->setTextColor(Color4B::YELLOW);
		questionLabel->setPosition(visibleSize.width * 0.95, visibleSize.height);
		pLayer->addChild(questionLabel);
	}
	questionLabel->setString(StringUtils::format("Remain Green : %2d", nRemainTest));

	auto pointLabel = (Label*)pLayer->getChildByTag(POINT_LABEL);
	if (pointLabel == NULL)
	{
		pointLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		pointLabel->setTag(POINT_LABEL);
		pointLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		pointLabel->setTextColor(Color4B::WHITE);
		pointLabel->setPosition(visibleSize.width * 0.95, visibleSize.height - 40);
		pLayer->addChild(pointLabel);
	}
	pointLabel->setString(StringUtils::format("Total Point : %2d", nPoint));

	auto prevPointLabel = (Label*)pLayer->getChildByTag(REACTIVE_PREV_POINT);
	if (prevPointLabel == NULL)
	{
		prevPointLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		prevPointLabel->setTag(REACTIVE_PREV_POINT);
		prevPointLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		prevPointLabel->setTextColor(Color4B::GREEN);
		prevPointLabel->setPosition(visibleSize.width * 0.95, visibleSize.height - 80);
		pLayer->addChild(prevPointLabel);
	}
	prevPointLabel->setString(StringUtils::format("Prev Point : %2d", nPrevPoint));

	auto testLabel = (Label*)pLayer->getChildByTag(REACTIVE_TEXT_LABEL);
	if (testLabel == NULL)
	{
		testLabel = Label::createWithTTF("READY", _fonts_Base, IN_GAME_FONT_SIZE * 4);
		testLabel->setTag(REACTIVE_TEXT_LABEL);
		testLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		testLabel->setTextColor(Color4B::WHITE);
		testLabel->setPosition(visibleSize.width * 0.5, visibleSize.height * 0.5);
		pLayer->addChild(testLabel);
	}
	if (nRemainTest == 9)
		testLabel->setVisible(false);

	if (green->getScale() == 0.0f && red->getScale() == 0.0f)
	{
		nReactiveTick--;
		if (nReactiveTick < 0)
		{
			nReactiveTick = 60 + CCRANDOM_0_1() * 240;
			bAnswerGreen = true;
			if (nRemainTest != 10 && CCRANDOM_0_1() < 0.2f)
				bAnswerGreen = false;

		}
		else if (nReactiveTick == 0)
		{
			float nScale = 3.0f;
			green->setScale(nScale);
			red->setScale(nScale);

			if (bAnswerGreen == true)
			{
				green->setVisible(true);
			}
			else
			{
				red->setVisible(true);
			}
		}
	}
	else
	{
		float nScale = green->getScale() - 0.05f;
		if (nScale <= 0.0f)
		{
			nScale = 0.0f;
			if (bAnswerGreen == true)
				nRemainTest = nRemainTest - 1;

			green->setVisible(false);
			red->setVisible(false);
		}

		green->setScale(nScale);
		red->setScale(nScale);
	}
}

void PlayFunc::TimingTest()
{
	auto questionLabel = (Label*)pLayer->getChildByTag(QUESTION_LABEL);
	if (questionLabel == NULL)
	{
		questionLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		questionLabel->setTag(QUESTION_LABEL);
		questionLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		questionLabel->setTextColor(Color4B::YELLOW);
		questionLabel->setPosition(visibleSize.width* 0.95, visibleSize.height);
		pLayer->addChild(questionLabel);
	}
	questionLabel->setString(StringUtils::format("Remain Touches : %2d", nRemainTouch));

	auto pointLabel = (Label*)pLayer->getChildByTag(POINT_LABEL);
	if (pointLabel == NULL)
	{
		pointLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		pointLabel->setTag(POINT_LABEL);
		pointLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		pointLabel->setTextColor(Color4B::WHITE);
		pointLabel->setPosition(visibleSize.width* 0.95, visibleSize.height - 40);
		pLayer->addChild(pointLabel);
	}
	pointLabel->setString(StringUtils::format("Total Point : %2d", nPoint));

	auto prevPointLabel = (Label*)pLayer->getChildByTag(TIMING_PREV_CLICK);
	if (prevPointLabel == NULL)
	{
		prevPointLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		prevPointLabel->setTag(TIMING_PREV_CLICK);
		prevPointLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		prevPointLabel->setTextColor(Color4B::GREEN);
		prevPointLabel->setPosition(visibleSize.width* 0.95, visibleSize.height - 80);
		pLayer->addChild(prevPointLabel);
	}
	prevPointLabel->setString(StringUtils::format("Prev Touch : %2d", nPrevTouch));

	nTimingTick++;
	if (nTimingTick > 11 - (15 - nRemainTouch) / 2)
	{
		nTimingTick = 0;
		nTimingPoint++;
		if (nTimingPoint > 10)
			nTimingPoint = 1;
	}

	auto testLabel = (Label*)pLayer->getChildByTag(TIMING_TEXT_LABEL);
	if (testLabel == NULL)
	{
		testLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE * 8);
		testLabel->setTag(TIMING_TEXT_LABEL);
		testLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		testLabel->setTextColor(Color4B::GREEN);
		testLabel->setPosition(visibleSize.width * 0.15, visibleSize.height * 0.5);
		pLayer->addChild(testLabel);
	}

	testLabel->setString(StringUtils::format("+%2d", nTimingPoint));

}


BlockSprite* PlayFunc::getInvisibleBlock(int nType)
{
	BlockSprite* block = NULL;
	if (nType == 0)
	{
		int k;
		for (k = 0; k < vGreen.size(); k++)
		{
			block = vGreen.at(k);
			if (block->isVisible() == false)
			{
				block->setVisible(true);
				break;
			}
		}

		//없으면 새로 생성
		if (k == vGreen.size())
		{
			block = BlockSprite::createWithSpriteFrameName("green.png");
			vGreen.pushBack(block);
			block->setVisible(true);
			pLayer->addChild(block, 2);
		}

		block->nDamage = 1.0f;
	}
	else if (nType == 1)
	{
		int k;
		for (k = 0; k < vRed.size(); k++)
		{
			block = vRed.at(k);
			if (block->isVisible() == false)
			{
				block->setVisible(true);
				break;
			}
		}

		//없으면 새로 생성
		if (k == vRed.size())
		{
			block = BlockSprite::createWithSpriteFrameName("red.png");
			vRed.pushBack(block);
			block->setVisible(true);
			pLayer->addChild(block, 2);
		}

		block->nDamage = 1.0f;
	}

	return block;
}


void PlayFunc::CheckGameOver()
{

}




bool PlayFunc::onTouchBegan(Touch *touch, Event *unused_event)
{

	if (nGameType == MENU_ACCURACY)
	{
		Sprite* blue = (Sprite*)pLayer->getChildByTag(AC_BLUE);
		if (blue == NULL)
		{
			blue = Sprite::createWithSpriteFrameName("blue.png");
			blue->setTag(AC_BLUE);
			blue->setVisible(true);
			blue->setScale(0.2f);
			pLayer->addChild(blue, 0);
		}
		float nX = touch->getLocation().x;
		float nY = touch->getLocation().y;
		blue->setPosition(nX, nY);

		//check crash
		Rect rectBlue = blue->getBoundingBox();
		for (int i = 0; i < vGreen.size(); i++)
		{
			BlockSprite* green = vGreen.at(i);
			if (green->isVisible() == false)
				continue;

			if (green->getBoundingBox().intersectsRect(rectBlue) == true)
			{
				green->stopAllActions();
				green->setVisible(false);

				nPoint += 1;


					if (mdp.bSoundSwitch == true)
					{
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
					}
					auto particle = ParticleSystemQuad::create(_particle_correct);
					particle->setPosition(nX, nY);
					particle->setScale(0.5f);
					pLayer->addChild(particle, 100);
					auto action = Sequence::create(DelayTime::create(1.0f),
						CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
						NULL);
					particle->runAction(action);


				break;
			}
		}


		for (int i = 0; i < vRed.size(); i++)
		{
			BlockSprite* red = vRed.at(i);
			if (red->isVisible() == false)
				continue;

			if (red->getBoundingBox().intersectsRect(rectBlue) == true)
			{
				red->stopAllActions();
				red->setVisible(false);
				nACRemainGreen -= 3;


				if (mdp.bSoundSwitch == true)
				{
					CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
				}
				auto particle = ParticleSystemQuad::create(_particle_incorrect1);
				particle->setPosition(visibleSize.width* 0.4, visibleSize.height*0.65);
				pLayer->addChild(particle, 100);
				auto action = Sequence::create(DelayTime::create(0.6f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
					NULL);
				particle->runAction(action);

				auto particle2 = ParticleSystemQuad::create(_particle_incorrect2);
				particle2->setPosition(visibleSize.width* 0.6, visibleSize.height*0.65);
				pLayer->addChild(particle2, 100);
				auto action2 = Sequence::create(DelayTime::create(0.6f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
					NULL);
				particle2->runAction(action2);

				auto particle3 = ParticleSystemQuad::create(_particle_incorrect3);
				particle3->setPosition(visibleSize.width* 0.4, visibleSize.height*0.4);
				pLayer->addChild(particle3, 100);
				auto action3 = Sequence::create(DelayTime::create(0.6f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
					NULL);
				particle3->runAction(action3);

				auto particle4 = ParticleSystemQuad::create(_particle_incorrect4);
				particle4->setPosition(visibleSize.width* 0.6, visibleSize.height*0.4);
				pLayer->addChild(particle4, 100);
				auto action4 = Sequence::create(DelayTime::create(0.6f),
					CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
					NULL);
				particle4->runAction(action4);


				break;
			}
		}


		//move 
		blue->setPosition(-100, -100);

	}



	if (nGameType == MENU_REACTIVE)
	{
		Sprite* green = (Sprite*)pLayer->getChildByTag(REACTIVE_GREEN);
		Sprite* red = (Sprite*)pLayer->getChildByTag(REACTIVE_RED);
		if (green == NULL || red == NULL)
			return false;


		if (bAnswerGreen == true && green->getScale() != 0.0f)
		{
			nPrevPoint = green->getScale() * 7;

			nPoint += nPrevPoint;


			if (mdp.bSoundSwitch == true)
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
			}
			auto particle = ParticleSystemQuad::create(_particle_correct);
			particle->setPosition(visibleSize.width* 0.5, visibleSize.height*0.5);
			pLayer->addChild(particle, 100);
			auto action = Sequence::create(DelayTime::create(1.0f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle->runAction(action);

		}
		else
		{
			nPrevPoint = 0;

			if (mdp.bSoundSwitch == true)
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
			}
			auto particle = ParticleSystemQuad::create(_particle_incorrect1);
			particle->setPosition(visibleSize.width* 0.4, visibleSize.height*0.65);
			pLayer->addChild(particle, 100);
			auto action = Sequence::create(DelayTime::create(0.6f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle->runAction(action);

			auto particle2 = ParticleSystemQuad::create(_particle_incorrect2);
			particle2->setPosition(visibleSize.width* 0.6, visibleSize.height*0.65);
			pLayer->addChild(particle2, 100);
			auto action2 = Sequence::create(DelayTime::create(0.6f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle2->runAction(action2);

			auto particle3 = ParticleSystemQuad::create(_particle_incorrect3);
			particle3->setPosition(visibleSize.width* 0.4, visibleSize.height*0.4);
			pLayer->addChild(particle3, 100);
			auto action3 = Sequence::create(DelayTime::create(0.6f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle3->runAction(action3);

			auto particle4 = ParticleSystemQuad::create(_particle_incorrect4);
			particle4->setPosition(visibleSize.width* 0.6, visibleSize.height*0.4);
			pLayer->addChild(particle4, 100);
			auto action4 = Sequence::create(DelayTime::create(0.6f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle4->runAction(action4);
		}


		green->setScale(0.0f);
		green->setVisible(false);
		red->setScale(0.0f);
		red->setVisible(false);
		nRemainTest -= 1;
		nReactiveTick = 0;
		bAnswerGreen = false;

	}


	if (nGameType == MENU_SPEED)
	{
		float nX = touch->getLocation().x;
		float nY = touch->getLocation().y;
		bool bTouch = false;
		if (nPosition == 0)
		{
			if (nX < visibleSize.width *0.5)
				if (nY >visibleSize.height *0.5)
					bTouch = true;
		}
		else if (nPosition == 1)
		{
			if (nX > visibleSize.width *0.5)
				if (nY > visibleSize.height *0.5)
					bTouch = true;
		}
		else if (nPosition == 2)
		{
			if (nX < visibleSize.width *0.5)
				if (nY < visibleSize.height *0.5)
					bTouch = true;
		}
		else
		{
			if (nX > visibleSize.width *0.5)
				if (nY < visibleSize.height *0.5)
					bTouch = true;
		}
		if (bTouch == true)
		{
			if (mdp.bSoundSwitch == true)
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
			}
			auto particle = ParticleSystemQuad::create(_particle_correct);
			particle->setPosition(nX, nY);
			particle->setScale(0.5f);
			pLayer->addChild(particle, 100);
			auto action = Sequence::create(DelayTime::create(1.0f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle->runAction(action);

			nPoint++;
			Sprite* green = (Sprite*)pLayer->getChildByTag(SPEED_GREEN);
			if (green != NULL)
			{
				green->setScale(green->getScale() - 0.075f);
			}

			if (green->getScale() <= 0.075f)
			{
				SpeedNewPosition();
				nPoint += 3;
			}
		}
		else
		{
			if (mdp.bSoundSwitch == true)
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
			}
		}

	}


	if (nGameType == MENU_TIMING)
	{
		//
		nRemainTouch--;
		nPoint += nTimingPoint;

		if (nTimingPoint == 10)
		{
			if (mdp.bSoundSwitch == true)
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
			}
			auto particle = ParticleSystemQuad::create(_particle_correct);
			particle->setPosition(visibleSize.width* 0.5, visibleSize.height*0.5);
			pLayer->addChild(particle, 100);
			auto action = Sequence::create(DelayTime::create(1.0f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle->runAction(action);
		}
		else
		{
			if (mdp.bSoundSwitch == true)
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
			}
		}
		//
		nPrevTouch = nTimingPoint;
		//
		nTimingPoint = 1;
		nTimingTick = 0;
		//

		if (nRemainTouch == 0)
		{
			if (nPoint > MAX_GQ)
				nPoint = MAX_GQ;

			mdp.nTest[MENU_TIMING - MENU_BASE] = nPoint;
			ExitGame();
		}
	}

	return false;
}

void PlayFunc::onTouchMoved(Touch *touch, Event *unused_event)
{
}

void PlayFunc::onTouchEnded(Touch *touch, Event *unused_event)
{
}

void PlayFunc::onTouchCancelled(Touch *touch, Event *unused_event)
{
}


void PlayFunc::DVTest()
{
	auto questionLabel = (Label*)pLayer->getChildByTag(QUESTION_LABEL);
	if (questionLabel == NULL)
	{
		questionLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		questionLabel->setTag(QUESTION_LABEL);
		questionLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		questionLabel->setTextColor(Color4B::YELLOW);
		questionLabel->setPosition(visibleSize.width* 0.95, visibleSize.height);
		pLayer->addChild(questionLabel);
	}
	questionLabel->setString(StringUtils::format("Question No.%2d", nDV_Stage + 1));

	auto pointLabel = (Label*)pLayer->getChildByTag(POINT_LABEL);
	if (pointLabel == NULL)
	{
		pointLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		pointLabel->setTag(POINT_LABEL);
		pointLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		pointLabel->setTextColor(Color4B::WHITE);
		pointLabel->setPosition(visibleSize.width* 0.95, visibleSize.height - 40);
		pLayer->addChild(pointLabel);
	}
	pointLabel->setString(StringUtils::format("Total Point : %2d", nPoint));

	auto currentPointLabel = (Label*)pLayer->getChildByTag(DV_CURRENT_POINT);
	if (currentPointLabel == NULL)
	{
		currentPointLabel = Label::createWithTTF("", _fonts_Base, IN_GAME_FONT_SIZE);
		currentPointLabel->setTag(DV_CURRENT_POINT);
		currentPointLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		currentPointLabel->setTextColor(Color4B::GREEN);
		currentPointLabel->setPosition(visibleSize.width* 0.95, visibleSize.height - 80);
		pLayer->addChild(currentPointLabel);
	}
	int nCurrentPoint = 10 - nReplayCount;
	if (nCurrentPoint < 0)
		nCurrentPoint = 0;
	currentPointLabel->setString(StringUtils::format("Answer Point : %2d", nCurrentPoint));


	if (bNeedNewBlock)
	{
		float rand = CCRANDOM_0_1() * 6;

		if (rand > 5)
		{
			nCurrentShape = BUTTON_CIRCLE;
		}
		else if (rand > 4)
		{
			nCurrentShape = BUTTON_CROSS;
		}
		else if (rand > 3)
		{
			nCurrentShape = BUTTON_HEART;
		}
		else if (rand > 2)
		{
			nCurrentShape = BUTTON_TRIANGLE;
		}
		else if (rand > 1)
		{
			nCurrentShape = BUTTON_SQUARE;
		}
		else
		{
			nCurrentShape = BUTTON_RHOMBUS;
		}

		bReplay = true;
		nReplayCount = 0;
		nReplayTick = 0;

		bNeedNewBlock = false;
		bFirstPlay = true;
	}


	nReplayTick++;
	if (bReplay && nReplayTick > DV_TICK)
	{
		if (bFirstPlay == true)
			bFirstPlay = false;
		else
			nReplayCount++;

		BlockSprite* block = NULL;
		if (nCurrentShape == BUTTON_CIRCLE)
			block = BlockSprite::createWithSpriteFrameName("circle.png");
		else if (nCurrentShape == BUTTON_CROSS)
			block = BlockSprite::createWithSpriteFrameName("cross.png");
		else if (nCurrentShape == BUTTON_HEART)
			block = BlockSprite::createWithSpriteFrameName("heart.png");
		else if (nCurrentShape == BUTTON_TRIANGLE)
			block = BlockSprite::createWithSpriteFrameName("triangle.png");
		else if (nCurrentShape == BUTTON_SQUARE)
			block = BlockSprite::createWithSpriteFrameName("square.png");
		else if (nCurrentShape == BUTTON_RHOMBUS)
			block = BlockSprite::createWithSpriteFrameName("rhombus.png");

		pLayer->addChild(block, 2);

		block->setTag(CURRENT_SHAPE);
		block->setPosition(visibleSize.width* 0.5, visibleSize.width * 1.4);


		float nScale = 0.6f - (nDV_Stage * 0.04f) + (nReplayCount * 0.02f);
		if (nScale >= 0.5f)
			nScale = 0.5f;

		if (nScale <= 0.1f)
			nScale = 0.1f;

		float nTime = 1.0f - (nDV_Stage * 0.05) + (nReplayCount * 0.025f);

		block->setScale(nScale);
		auto action = Sequence::create(
			MoveBy::create(nTime, Vec2(0, -visibleSize.width * 2)),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::DeleteSprite, this)),
			CallFuncN::create(CC_CALLBACK_0(PlayFunc::DVReplay, this)),
			NULL);

		block->runAction(action);


		{
			float nRand = CCRANDOM_0_1() * 6;
			BlockSprite* topBlock = NULL;
			BlockSprite* bottomBlock = NULL;
			BlockSprite* topBlock2 = NULL;
			BlockSprite* bottomBlock2 = NULL;
			if (nRand > 5)
			{
				topBlock = BlockSprite::createWithSpriteFrameName("circle.png");
				bottomBlock = BlockSprite::createWithSpriteFrameName("cross.png");
				topBlock2 = BlockSprite::createWithSpriteFrameName("rhombus.png");
				bottomBlock2 = BlockSprite::createWithSpriteFrameName("square.png");
			}
			else if (nRand > 4)
			{
				topBlock = BlockSprite::createWithSpriteFrameName("cross.png");
				bottomBlock = BlockSprite::createWithSpriteFrameName("heart.png");
				topBlock2 = BlockSprite::createWithSpriteFrameName("square.png");
				bottomBlock2 = BlockSprite::createWithSpriteFrameName("triangle.png");
			}
			else if (nRand > 3)
			{
				topBlock = BlockSprite::createWithSpriteFrameName("heart.png");
				bottomBlock = BlockSprite::createWithSpriteFrameName("triangle.png");
				topBlock2 = BlockSprite::createWithSpriteFrameName("triangle.png");
				bottomBlock2 = BlockSprite::createWithSpriteFrameName("heart.png");
			}
			else if (nRand > 2)
			{
				topBlock = BlockSprite::createWithSpriteFrameName("triangle.png");
				bottomBlock = BlockSprite::createWithSpriteFrameName("square.png");
				topBlock2 = BlockSprite::createWithSpriteFrameName("heart.png");
				bottomBlock2 = BlockSprite::createWithSpriteFrameName("cross.png");
			}
			else if (nRand > 1)
			{
				topBlock = BlockSprite::createWithSpriteFrameName("square.png");
				bottomBlock = BlockSprite::createWithSpriteFrameName("rhombus.png");
				topBlock2 = BlockSprite::createWithSpriteFrameName("cross.png");
				bottomBlock2 = BlockSprite::createWithSpriteFrameName("circle.png");
			}
			else
			{
				topBlock = BlockSprite::createWithSpriteFrameName("rhombus.png");
				bottomBlock = BlockSprite::createWithSpriteFrameName("cross.png");
				topBlock2 = BlockSprite::createWithSpriteFrameName("circle.png");
				bottomBlock2 = BlockSprite::createWithSpriteFrameName("square.png");
			}

			pLayer->addChild(topBlock, 2);
			pLayer->addChild(bottomBlock, 2);
			pLayer->addChild(topBlock2, 2);
			pLayer->addChild(bottomBlock2, 2);

			topBlock2->setPosition(visibleSize.width* 0.5, visibleSize.width * 1.8);
			topBlock->setPosition(visibleSize.width* 0.5, visibleSize.width * 1.6);
			bottomBlock->setPosition(visibleSize.width* 0.5, visibleSize.width * 1.2);
			bottomBlock2->setPosition(visibleSize.width* 0.5, visibleSize.width * 1.0);

			topBlock2->setScale(nScale);
			topBlock->setScale(nScale);
			bottomBlock->setScale(nScale);
			bottomBlock2->setScale(nScale);


			auto action1 = Sequence::create(
				MoveBy::create(nTime, Vec2(0, -visibleSize.width * 2)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::DeleteSprite, this)),
				NULL);
			auto action2 = Sequence::create(
				MoveBy::create(nTime, Vec2(0, -visibleSize.width * 2)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::DeleteSprite, this)),
				NULL);
			auto action3 = Sequence::create(
				MoveBy::create(nTime, Vec2(0, -visibleSize.width * 2)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::DeleteSprite, this)),
				NULL);
			auto action4 = Sequence::create(
				MoveBy::create(nTime, Vec2(0, -visibleSize.width * 2)),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::DeleteSprite, this)),
				NULL);

			topBlock->runAction(action1);
			bottomBlock->runAction(action2);
			topBlock2->runAction(action3);
			bottomBlock2->runAction(action4);
		}


		bReplay = false;
	}


}

void PlayFunc::DVReplay()
{
	bReplay = true;
	nReplayTick = 0;
}


void PlayFunc::DVButton(Ref* pSender)
{
	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();

	if (bExplain == true)
	{
		if (nSelectedItem == BUTTON_RHOMBUS)
		{
			Sprite* dvExplain = (Sprite*)pLayer->getChildByTag(DV_EXPLAIN);
			pLayer->removeChild(dvExplain);
			if (mdp.bSoundSwitch == true)
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
			}
			auto particle = ParticleSystemQuad::create(_particle_correct);
			particle->setPosition(visibleSize.width* 0.5, visibleSize.height*0.5);
			pLayer->addChild(particle, 100);
			auto action = Sequence::create(DelayTime::create(1.0f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle->runAction(action);

			bExplain = false;
			bNeedNewBlock = true;
		}
		else
		{
			if (mdp.bSoundSwitch == true)
			{
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
			}
			auto particle = ParticleSystemQuad::create(_particle_incorrect1);
			particle->setPosition(visibleSize.width* 0.4, visibleSize.height*0.65);
			pLayer->addChild(particle, 100);
			auto action = Sequence::create(DelayTime::create(0.6f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle->runAction(action);

			auto particle2 = ParticleSystemQuad::create(_particle_incorrect2);
			particle2->setPosition(visibleSize.width* 0.6, visibleSize.height*0.65);
			pLayer->addChild(particle2, 100);
			auto action2 = Sequence::create(DelayTime::create(0.6f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle2->runAction(action2);

			auto particle3 = ParticleSystemQuad::create(_particle_incorrect3);
			particle3->setPosition(visibleSize.width* 0.4, visibleSize.height*0.4);
			pLayer->addChild(particle3, 100);
			auto action3 = Sequence::create(DelayTime::create(0.6f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle3->runAction(action3);

			auto particle4 = ParticleSystemQuad::create(_particle_incorrect4);
			particle4->setPosition(visibleSize.width* 0.6, visibleSize.height*0.4);
			pLayer->addChild(particle4, 100);
			auto action4 = Sequence::create(DelayTime::create(0.6f),
				CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
				NULL);
			particle4->runAction(action4);
		}
		return;
	}

	if (nCurrentShape == 0)
		return;

	if (nSelectedItem == nCurrentShape)
	{
		nCurrentShape = 0;
		nDV_Stage++;
		bNeedNewBlock = true;

		if (mdp.bSoundSwitch == true)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Bubble);
		}
		auto particle = ParticleSystemQuad::create(_particle_correct);
		particle->setPosition(visibleSize.width* 0.5, visibleSize.height*0.5);
		pLayer->addChild(particle, 100);
		auto action = Sequence::create(DelayTime::create(1.0f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
			NULL);
		particle->runAction(action);

		BlockSprite* block = (BlockSprite*)pLayer->getChildByTag(CURRENT_SHAPE);
		if (block != NULL)
		{
			block->stopAllActions();
			pLayer->removeChild(block);
		}


		if (nReplayCount < 0)
			nReplayCount = 0;
		else if (nReplayCount > 10)
			nReplayCount = 10;

		nPoint += 10 - nReplayCount;

		if (nDV_Stage > 14)
		{
			if (nPoint > MAX_GQ)
				nPoint = MAX_GQ;

			mdp.nTest[MENU_DYNAMIC_VISUAL - MENU_BASE] = nPoint;
			ExitGame();
		}
		else
		{
			DVTest();
		}

	}
	else
	{
		if (mdp.bSoundSwitch == true)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);
		}
		auto particle = ParticleSystemQuad::create(_particle_incorrect1);
		particle->setPosition(visibleSize.width* 0.4, visibleSize.height*0.65);
		pLayer->addChild(particle, 100);
		auto action = Sequence::create(DelayTime::create(0.6f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
			NULL);
		particle->runAction(action);

		auto particle2 = ParticleSystemQuad::create(_particle_incorrect2);
		particle2->setPosition(visibleSize.width* 0.6, visibleSize.height*0.65);
		pLayer->addChild(particle2, 100);
		auto action2 = Sequence::create(DelayTime::create(0.6f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
			NULL);
		particle2->runAction(action2);

		auto particle3 = ParticleSystemQuad::create(_particle_incorrect3);
		particle3->setPosition(visibleSize.width* 0.4, visibleSize.height*0.4);
		pLayer->addChild(particle3, 100);
		auto action3 = Sequence::create(DelayTime::create(0.6f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
			NULL);
		particle3->runAction(action3);

		auto particle4 = ParticleSystemQuad::create(_particle_incorrect4);
		particle4->setPosition(visibleSize.width* 0.6, visibleSize.height*0.4);
		pLayer->addChild(particle4, 100);
		auto action4 = Sequence::create(DelayTime::create(0.6f),
			CallFuncN::create(CC_CALLBACK_1(PlayFunc::ResetParticle, this)),
			NULL);
		particle4->runAction(action4);


		nReplayCount += 1;
	}
}

void PlayFunc::update()
{
	if (bUpdateGameOver == false) //Game 종료 후 화면 출력 위해 사용
	{
		//SPEED
		if (nGameType == MENU_SPEED)
			SpeedTest();

		//REACTIVE
		if (nGameType == MENU_REACTIVE)
			ReactiveTest();

		//Timing
		if (nGameType == MENU_TIMING)
			TimingTest();

		//ACURACY
		if (nGameType == MENU_ACCURACY)
			ACTest();

		//////DV
		if (nGameType == MENU_DYNAMIC_VISUAL)
			DVTest();
	}

	//Check GameOver
	CheckGameOver();
}