#include "StageSelectScene.h"
#include "PlayScene.h"
#include "../MenuScene.h"
#include "../Util/define.h"
#include "../Util/MyDataManager.h"
#include "SimpleAudioEngine.h"


void StageSelectScene::initBG()
{
	auto node = ParallaxNode::create();
	this->addChild(node, 0);

	auto BG = Sprite::createWithSpriteFrameName("BG.png");
	BG->setAnchorPoint(Point::ZERO);
	BG->setScale(2.0f);
	node->addChild(BG, 0, Point(0, 0), Point::ZERO);
}
void StageSelectScene::initBGM()
{
}
void StageSelectScene::initParticle()
{
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();

	auto particle = ParticleSystemQuad::create(_particle_BG_rain);
	particle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	particle->setScale(2.0f);
	this->addChild(particle, 2);
}

Scene* StageSelectScene::createScene()
{
	auto *scene = Scene::create();

	auto *layer = StageSelectScene::create();
	scene->addChild(layer);

	return scene;
}

void StageSelectScene::Back()
{
	this->getEventDispatcher()->removeAllEventListeners();
	this->removeAllChildren();
	Director::getInstance()->replaceScene(TransitionFade::create(0.1f, MenuScene::createScene()));
}

void StageSelectScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	// Back button pressed
	if (keyCode == EventKeyboard::KeyCode::KEY_Z
		|| keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
		|| keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		Back();
	}
}

bool StageSelectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	auto k_listener = EventListenerKeyboard::create();
	k_listener->onKeyReleased = CC_CALLBACK_2(StageSelectScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(k_listener, 1);

	initBG();
	initBGM();
	initParticle();

	/////////////////////////////
	//게임 제목 라벨
	auto mainLabel = Label::createWithTTF(en_stage_select, _fonts_NanumPen, TITLE_FONT_SIZE* 0.8);
	if (mdp.bEnglish == false)
	{
		mainLabel->setString(UTF8PCHAR(ko_stage_select));
	}
	mainLabel->setTextColor(Color4B::YELLOW);
	mainLabel->enableOutline(Color4B::RED, 2);
	mainLabel->enableBold();
	mainLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	mainLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height));
	this->addChild(mainLabel, 1);
	/////////////////////////////



	nMaxPower = BASE_POWER + mdp.nPowerUp * SHOP_POWER_UP_RATE;
	nIgniteTime = BASE_BURNTIME - mdp.nIgniteTimeUp * SHOP_BURNTIME_UP_RATE;
	nSpeed = BASE_SPEED + mdp.nSpeedUp * SHOP_SPEED_UP_RATE;
	auto Power = Label::createWithTTF(StringUtils::format(en_s_power, nMaxPower), _fonts_NanumPen, STAGE_SELECT_FONT_SIZE * 0.7f);
	Power->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	Power->setPosition(Vec2(origin.x + visibleSize.width / 10 * 8, origin.y + visibleSize.height * 0.85));
	Power->setTextColor(Color4B::RED);
	Power->enableOutline(Color4B::WHITE, 1);

	auto Time = Label::createWithTTF(StringUtils::format(en_s_time, nIgniteTime), _fonts_NanumPen, STAGE_SELECT_FONT_SIZE* 0.7f);
	Time->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	Time->setPosition(Vec2(origin.x + visibleSize.width / 10 * 8, origin.y + visibleSize.height * 0.80));
	Time->setTextColor(Color4B::YELLOW);
	Time->enableOutline(Color4B::WHITE, 1);

	auto Speed = Label::createWithTTF(StringUtils::format(en_s_speed, nSpeed), _fonts_NanumPen, STAGE_SELECT_FONT_SIZE * 0.7f);
	Speed->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	Speed->setPosition(Vec2(origin.x + visibleSize.width / 10 * 8, origin.y + visibleSize.height * 0.75));
	Speed->setTextColor(Color4B::BLUE);
	Speed->enableOutline(Color4B::WHITE, 1);

	if (mdp.bEnglish == false)
	{
		Power->setString(UTF8(StringUtils::format(ko_s_power, nMaxPower)));
		Time->setString(UTF8(StringUtils::format(ko_s_time, nIgniteTime)));
		Speed->setString(UTF8(StringUtils::format(ko_s_speed, nSpeed)));
	}

	this->addChild(Power);
	this->addChild(Time);
	this->addChild(Speed);
	///////////////////////////////////////////////////////


	/////////////////////////////
	// Stage
	auto label1 = Label::createWithTTF("P", _fonts_NanumPen, STAGE_SELECT_FONT_SIZE);
	label1->setTextColor(Color4B::BLUE);
	label1->enableBold();
	label1->enableOutline(Color4B::WHITE, 2);

	auto label2 = Label::createWithTTF("P", _fonts_NanumPen, STAGE_SELECT_FONT_SIZE);
	label2->setTextColor(Color4B::BLUE);
	label2->enableBold();
	label2->enableOutline(Color4B::WHITE, 2);

	auto label3 = Label::createWithTTF("P", _fonts_NanumPen, STAGE_SELECT_FONT_SIZE);
	label3->setTextColor(Color4B::BLUE);
	label3->enableBold();
	label3->enableOutline(Color4B::WHITE, 2);

	auto label4 = Label::createWithTTF("P", _fonts_NanumPen, STAGE_SELECT_FONT_SIZE);
	label4->setTextColor(Color4B::BLUE);
	label4->enableBold();
	label4->enableOutline(Color4B::WHITE, 2);

	auto sprUp = Sprite::createWithSpriteFrameName("up.png");
	auto sprDown = Sprite::createWithSpriteFrameName("down.png");
	auto sprUp_s = Sprite::createWithSpriteFrameName("up_s.png");
	auto sprDown_s = Sprite::createWithSpriteFrameName("down_s.png");
	auto sprDown_d = Sprite::createWithSpriteFrameName("down_d.png");
	sprUp->setScale(0.8f);
	sprDown->setScale(0.8f);
	sprUp_s->setScale(0.8f);
	sprDown_s->setScale(0.8f);
	auto labelItem1 = MenuItemLabel::create(label1, CC_CALLBACK_1(StageSelectScene::menuCallback, this));
	auto labelItem2 = MenuItemLabel::create(label2, CC_CALLBACK_1(StageSelectScene::menuCallback, this));
	auto labelItem3 = MenuItemLabel::create(label3, CC_CALLBACK_1(StageSelectScene::menuCallback, this));
	auto labelItem4 = MenuItemLabel::create(label4, CC_CALLBACK_1(StageSelectScene::menuCallback, this));
	auto upItem = MenuItemSprite::create(sprUp, sprUp_s, CC_CALLBACK_1(StageSelectScene::menuCallback, this));
	auto downItem = MenuItemSprite::create(sprDown, sprDown_s, sprDown_d, CC_CALLBACK_1(StageSelectScene::menuCallback, this));
	labelItem1->setTag(1);
	labelItem2->setTag(2);
	labelItem3->setTag(3);
	labelItem4->setTag(4);
	upItem->setTag(STAGE_UP);
	downItem->setTag(STAGE_DOWN);

	auto stageMenu = Menu::create(
		upItem,
		labelItem1, labelItem2, labelItem3, labelItem4,
		downItem,
		NULL);

	stageMenu->setPosition(visibleSize.width * 0.3, visibleSize.height* 0.5);
	stageMenu->alignItemsVertically();
	stageMenu->setTag(STAGE_MENU);
	this->addChild(stageMenu);
	//////////////////////////////////////////////////////////////////////////////
	auto star1 = Sprite::createWithSpriteFrameName("star1.png");
	auto star2 = Sprite::createWithSpriteFrameName("star1.png");
	auto star3 = Sprite::createWithSpriteFrameName("star1.png");
	auto star4 = Sprite::createWithSpriteFrameName("star1.png");
	auto star5 = Sprite::createWithSpriteFrameName("star1.png");
	auto star6 = Sprite::createWithSpriteFrameName("star1.png");
	auto star7 = Sprite::createWithSpriteFrameName("star1.png");
	auto star8 = Sprite::createWithSpriteFrameName("star1.png");
	auto star9 = Sprite::createWithSpriteFrameName("star1.png");
	auto star10 = Sprite::createWithSpriteFrameName("star1.png");
	auto star11 = Sprite::createWithSpriteFrameName("star1.png");
	auto star12 = Sprite::createWithSpriteFrameName("star1.png");
	auto star13 = Sprite::createWithSpriteFrameName("star1.png");
	auto star14 = Sprite::createWithSpriteFrameName("star1.png");
	auto star15 = Sprite::createWithSpriteFrameName("star1.png");
	auto star16 = Sprite::createWithSpriteFrameName("star1.png");

	auto ystar1 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar2 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar3 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar4 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar5 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar6 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar7 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar8 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar9 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar10 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar11 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar12 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar13 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar14 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar15 = Sprite::createWithSpriteFrameName("star2.png");
	auto ystar16 = Sprite::createWithSpriteFrameName("star2.png");

	{
		auto item1 = MenuItemSprite::create(star1, NULL, NULL);
		auto item2 = MenuItemSprite::create(star2, NULL, NULL);
		auto item3 = MenuItemSprite::create(star3, NULL, NULL);
		auto item4 = MenuItemSprite::create(star4, NULL, NULL);
		auto starMenu1 = Menu::create(item1, item2, item3, item4, NULL);
		starMenu1->setPosition(visibleSize.width * 0.55, visibleSize.height* 0.5);
		starMenu1->alignItemsVerticallyWithPadding(-20);
		starMenu1->setEnabled(false);
		this->addChild(starMenu1);
	}
	{
		auto item1 = MenuItemSprite::create(star5, NULL, NULL);
		auto item2 = MenuItemSprite::create(star6, NULL, NULL);
		auto item3 = MenuItemSprite::create(star7, NULL, NULL);
		auto item4 = MenuItemSprite::create(star8, NULL, NULL);
		auto starMenu1 = Menu::create(item1, item2, item3, item4, NULL);
		starMenu1->setPosition(visibleSize.width * 0.65, visibleSize.height* 0.5);
		starMenu1->alignItemsVerticallyWithPadding(-20);
		starMenu1->setEnabled(false);
		this->addChild(starMenu1);
	}
	{
		auto item1 = MenuItemSprite::create(star9, NULL, NULL);
		auto item2 = MenuItemSprite::create(star10, NULL, NULL);
		auto item3 = MenuItemSprite::create(star11, NULL, NULL);
		auto item4 = MenuItemSprite::create(star12, NULL, NULL);
		auto starMenu1 = Menu::create(item1, item2, item3, item4, NULL);
		starMenu1->setPosition(visibleSize.width * 0.75, visibleSize.height* 0.5);
		starMenu1->alignItemsVerticallyWithPadding(-20);
		starMenu1->setEnabled(false);
		this->addChild(starMenu1);
	}
	{
		auto item1 = MenuItemSprite::create(star13, NULL, NULL);
		auto item2 = MenuItemSprite::create(star14, NULL, NULL);
		auto item3 = MenuItemSprite::create(star15, NULL, NULL);
		auto item4 = MenuItemSprite::create(star16, NULL, NULL);
		auto starMenu1 = Menu::create(item1, item2, item3, item4, NULL);
		starMenu1->setPosition(visibleSize.width * 0.85, visibleSize.height* 0.5);
		starMenu1->alignItemsVerticallyWithPadding(-20);
		starMenu1->setEnabled(false);
		this->addChild(starMenu1);
	}

	{
		auto item1 = MenuItemSprite::create(ystar1, NULL, NULL);
		item1->setTag(YSTAR1);
		auto item2 = MenuItemSprite::create(ystar2, NULL, NULL);
		item2->setTag(YSTAR2);
		auto item3 = MenuItemSprite::create(ystar3, NULL, NULL);
		item3->setTag(YSTAR3);
		auto item4 = MenuItemSprite::create(ystar4, NULL, NULL);
		item4->setTag(YSTAR4);
		auto ystarMenu1 = Menu::create(item1, item2, item3, item4, NULL);
		ystarMenu1->setPosition(visibleSize.width * 0.55, visibleSize.height* 0.5);
		ystarMenu1->alignItemsVerticallyWithPadding(-20);
		ystarMenu1->setEnabled(false);
		ystarMenu1->setTag(YSTAR_MENU1);
		this->addChild(ystarMenu1);
	}
	{
		auto item1 = MenuItemSprite::create(ystar5, NULL, NULL);
		item1->setTag(YSTAR5);
		auto item2 = MenuItemSprite::create(ystar6, NULL, NULL);
		item2->setTag(YSTAR6);
		auto item3 = MenuItemSprite::create(ystar7, NULL, NULL);
		item3->setTag(YSTAR7);
		auto item4 = MenuItemSprite::create(ystar8, NULL, NULL);
		item4->setTag(YSTAR8);
		auto ystarMenu1 = Menu::create(item1, item2, item3, item4, NULL);
		ystarMenu1->setPosition(visibleSize.width * 0.65, visibleSize.height* 0.5);
		ystarMenu1->alignItemsVerticallyWithPadding(-20);
		ystarMenu1->setEnabled(false);
		ystarMenu1->setTag(YSTAR_MENU2);
		this->addChild(ystarMenu1);
	}
	{
		auto item1 = MenuItemSprite::create(ystar9, NULL, NULL);
		item1->setTag(YSTAR9);
		auto item2 = MenuItemSprite::create(ystar10, NULL, NULL);
		item2->setTag(YSTAR10);
		auto item3 = MenuItemSprite::create(ystar11, NULL, NULL);
		item3->setTag(YSTAR11);
		auto item4 = MenuItemSprite::create(ystar12, NULL, NULL);
		item4->setTag(YSTAR12);
		auto ystarMenu1 = Menu::create(item1, item2, item3, item4, NULL);
		ystarMenu1->setPosition(visibleSize.width * 0.75, visibleSize.height* 0.5);
		ystarMenu1->alignItemsVerticallyWithPadding(-20);
		ystarMenu1->setEnabled(false);
		ystarMenu1->setTag(YSTAR_MENU3);
		this->addChild(ystarMenu1);
	}
	{
		auto item1 = MenuItemSprite::create(ystar13, NULL, NULL);
		item1->setTag(YSTAR13);
		auto item2 = MenuItemSprite::create(ystar14, NULL, NULL);
		item2->setTag(YSTAR14);
		auto item3 = MenuItemSprite::create(ystar15, NULL, NULL);
		item3->setTag(YSTAR15);
		auto item4 = MenuItemSprite::create(ystar16, NULL, NULL);
		item4->setTag(YSTAR16);
		auto ystarMenu1 = Menu::create(item1, item2, item3, item4, NULL);
		ystarMenu1->setPosition(visibleSize.width * 0.85, visibleSize.height* 0.5);
		ystarMenu1->alignItemsVerticallyWithPadding(-20);
		ystarMenu1->setEnabled(false);
		ystarMenu1->setTag(YSTAR_MENU4);
		this->addChild(ystarMenu1);
	}


	///////////////////////////////////////////////////////////////////////////
	auto labelStart = Label::createWithTTF("", _fonts_NanumPen, STAGE_SELECT_FONT_SIZE);
	labelStart->setTextColor(Color4B::BLUE);
	labelStart->enableBold();
	labelStart->enableOutline(Color4B::WHITE, 2);
	if (mdp.bEnglish)
		labelStart->setString(en_start);
	else
		labelStart->setString(UTF8PCHAR(ko_start));

	auto labelBack = Label::createWithTTF("", _fonts_NanumPen, STAGE_SELECT_FONT_SIZE);
	labelBack->setTextColor(Color4B::WHITE);
	labelBack->enableBold();
	labelBack->enableOutline(Color4B::BLACK, 2);
	if (mdp.bEnglish)
		labelBack->setString(en_back);
	else
		labelBack->setString(UTF8PCHAR(ko_back));
	auto labelStartItem = MenuItemLabel::create(labelStart, CC_CALLBACK_1(StageSelectScene::menuCallback, this));
	auto labelBackItem = MenuItemLabel::create(labelBack, CC_CALLBACK_1(StageSelectScene::menuCallback, this));
	labelStartItem->setTag(STAGE_START);
	labelBackItem->setTag(STAGE_BACK);
	auto menu2 = Menu::create(
		labelStartItem, labelBackItem, NULL);
	menu2->setPosition(visibleSize.width * 0.5, visibleSize.height* 0.15);
	menu2->alignItemsVerticallyWithPadding(50);
	this->addChild(menu2);


	for (int i = 0; i < mdp.nAddCount; i++)
	{
		auto star = Sprite::createWithSpriteFrameName("movie.png");
		star->setScale(0.5f);
		star->setPosition(visibleSize.width * (0.65 + i * 0.05), visibleSize.height* 0.21);
		this->addChild(star);
	}




	setLang();
	updateButton();
	updateClicker();

	return true;
}

void StageSelectScene::updateClicker()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto menu = (Menu*)this->getChildByTag(STAGE_MENU);

	auto item1 = (MenuItemLabel*)menu->getChildByTag(1);
	auto item2 = (MenuItemLabel*)menu->getChildByTag(2);
	auto item3 = (MenuItemLabel*)menu->getChildByTag(3);
	auto item4 = (MenuItemLabel*)menu->getChildByTag(4);

	auto click = (Sprite*)this->getChildByTag(STAGE_CLICK);
	if (click == NULL)
	{
		click = Sprite::createWithSpriteFrameName("click.png");
		click->setScale(-0.1f);
		click->setTag(STAGE_CLICK);
		this->addChild(click);

		auto action1 = ScaleTo::create(1, -0.5f);
		auto action2 = ScaleTo::create(1, -0.4f);
		auto action = Sequence::create(action1, action2, NULL);
		click->runAction(RepeatForever::create(action));
	}

	mdInfo.nStageLv = mdInfo.nStageView * 4 + mdInfo.nStageSelect;

	float nHeightRate = CLICK_HEIGHT_RATE1;


	item1->setEnabled(true);
	item2->setEnabled(true);
	item3->setEnabled(true);
	item4->setEnabled(true);
	

	if (mdInfo.nStageSelect == 0)
	{
		nHeightRate = CLICK_HEIGHT_RATE1;
		item1->setEnabled(false);
	}
	else if (mdInfo.nStageSelect == 1)
	{
		nHeightRate = CLICK_HEIGHT_RATE2;
		item2->setEnabled(false);
	}
	else if (mdInfo.nStageSelect == 2)
	{
		nHeightRate = CLICK_HEIGHT_RATE3;
		item3->setEnabled(false);
	}
	else if (mdInfo.nStageSelect == 3)
	{
		nHeightRate = CLICK_HEIGHT_RATE4;
		item4->setEnabled(false);
	}

	click->setPosition(visibleSize.width*0.1, visibleSize.height * nHeightRate);

}


void StageSelectScene::updateButton()
{

	Menu* stageMenu =	(Menu*)this->getChildByTag(STAGE_MENU);
	MenuItemSprite* pUp = (MenuItemSprite*)stageMenu->getChildByTag(STAGE_UP);
	MenuItemSprite* pDown = (MenuItemSprite*)stageMenu->getChildByTag(STAGE_DOWN);

	if (mdInfo.nStageView == 0)
	{
		pUp->setVisible(false);
	}
	else
	{
		pUp->setVisible(true);
	}

	if (mdInfo.nStageView == 2)
	{
		pDown->setVisible(false);
	}
	else
	{
		pDown->setVisible(true);
	}

	int nDownEnabled = 0;
	for (int i = 0; i < 4; i++)
	{
		if (mdInfo.nStageView * 4 + i < STAGE_COUNT)
			nDownEnabled += mdp.nClearLevel[mdInfo.nStageView * 4 + i];
	}
	

	if (nDownEnabled >= 5)
	{
		pDown->setEnabled(true);
	}
	else
	{
		pDown->setEnabled(false);
	}

}


void StageSelectScene::setLang()
{
	auto menu = (Menu*)this->getChildByTag(STAGE_MENU);
	auto item1 = (MenuItemLabel*)menu->getChildByTag(1);
	auto item2 = (MenuItemLabel*)menu->getChildByTag(2);
	auto item3 = (MenuItemLabel*)menu->getChildByTag(3);
	auto item4 = (MenuItemLabel*)menu->getChildByTag(4);
	auto start = (MenuItemLabel*)menu->getChildByTag(STAGE_START);
	auto back = (MenuItemLabel*)menu->getChildByTag(STAGE_BACK);

	item1->setString(getStageString(mdInfo.nStageView * 4 + 0, mdp.bEnglish));
	item2->setString(getStageString(mdInfo.nStageView * 4 + 1, mdp.bEnglish));
	item3->setString(getStageString(mdInfo.nStageView * 4 + 2, mdp.bEnglish));
	item4->setString(getStageString(mdInfo.nStageView * 4 + 3, mdp.bEnglish));


	MenuItemSprite* star1[4];
	MenuItemSprite* star2[4];
	MenuItemSprite* star3[4];
	MenuItemSprite* star4[4];
	auto menu1 = (Menu*)this->getChildByTag(YSTAR_MENU1);
	star1[0] = (MenuItemSprite*)menu1->getChildByTag(YSTAR1);
	star2[0]= (MenuItemSprite*)menu1->getChildByTag(YSTAR2);
	star3[0] = (MenuItemSprite*)menu1->getChildByTag(YSTAR3);
	star4[0] = (MenuItemSprite*)menu1->getChildByTag(YSTAR4);

	auto menu2 = (Menu*)this->getChildByTag(YSTAR_MENU2);
	star1[1] = (MenuItemSprite*)menu2->getChildByTag(YSTAR5);
	star2[1] = (MenuItemSprite*)menu2->getChildByTag(YSTAR6);
	star3[1] = (MenuItemSprite*)menu2->getChildByTag(YSTAR7);
	star4[1] = (MenuItemSprite*)menu2->getChildByTag(YSTAR8);

	auto menu3 = (Menu*)this->getChildByTag(YSTAR_MENU3);
	star1[2] = (MenuItemSprite*)menu3->getChildByTag(YSTAR9);
	star2[2] = (MenuItemSprite*)menu3->getChildByTag(YSTAR10);
	star3[2] = (MenuItemSprite*)menu3->getChildByTag(YSTAR11);
	star4[2] = (MenuItemSprite*)menu3->getChildByTag(YSTAR12);

	auto menu4 = (Menu*)this->getChildByTag(YSTAR_MENU4);
	star1[3] = (MenuItemSprite*)menu4->getChildByTag(YSTAR13);
	star2[3] = (MenuItemSprite*)menu4->getChildByTag(YSTAR14);
	star3[3] = (MenuItemSprite*)menu4->getChildByTag(YSTAR15);
	star4[3] = (MenuItemSprite*)menu4->getChildByTag(YSTAR16);

	for (int i = 0; i < 4; i++)
	{
		star1[i]->setVisible(false);
		star2[i]->setVisible(false);
		star3[i]->setVisible(false);
		star4[i]->setVisible(false);
	}

	

	if (mdInfo.nStageView * 4 + 0 < STAGE_COUNT)
	{
		for (int i = 0; i < mdp.nClearLevel[mdInfo.nStageView * 4 + 0]; i++)
		{
			star1[i]->setVisible(true);
		}
	}

	if (mdInfo.nStageView * 4 + 1 < STAGE_COUNT)
	{
		for (int i = 0; i < mdp.nClearLevel[mdInfo.nStageView * 4 + 1]; i++)
		{
			star2[i]->setVisible(true);
		}
	}

	if (mdInfo.nStageView * 4 + 2 < STAGE_COUNT)
	{
		for (int i = 0; i < mdp.nClearLevel[mdInfo.nStageView * 4 + 2]; i++)
		{
			star3[i]->setVisible(true);
		}
	}

	if (mdInfo.nStageView * 4 + 3 < STAGE_COUNT)
	{
		for (int i = 0; i < mdp.nClearLevel[mdInfo.nStageView * 4 + 3]; i++)
		{
			star4[i]->setVisible(true);
		}
	}
}


void StageSelectScene::menuCallback(Ref* pSender)
{
	if (MyDataManager::getInstance()->mPlayerData.bSoundSwitch == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_sounds_Touch);

	auto menuItem = (MenuItem*)pSender;
	int nSelectedItem = menuItem->getTag();
	auto director = Director::getInstance();

	switch (nSelectedItem)
	{
	case 1:
		mdInfo.nStageSelect = 0;
		updateClicker();
		break;
	case 2:
		mdInfo.nStageSelect = 1;
		updateClicker();
		break;
	case 3:
		mdInfo.nStageSelect = 2;
		updateClicker();
		break;
	case 4:
		mdInfo.nStageSelect = 3;
		updateClicker();
		break;
	case STAGE_UP:
		if (mdInfo.nStageView > 0)
		{
			mdInfo.nStageView--;
			setLang();
			updateButton();
			updateClicker();
		}
		break;
	case STAGE_DOWN:	
		if (mdInfo.nStageView < 2)
		{
			mdInfo.nStageView++;
			setLang();
			updateButton();
			updateClicker();
		}
		break;

	case STAGE_START:

		if (mdInfo.nStageLv < 0 || mdInfo.nStageLv > STAGE_COUNT - 1)
		{
			return;
		}

		mdInfo.nDifficulty = 0; // In Game Select

		mdInfo.nMaxPower1 = nMaxPower;
		mdInfo.nIgniteTime1 = nIgniteTime;
		mdInfo.nSpeed1 = nSpeed;
		mdInfo.nMaxPower2 = nMaxPower;
		mdInfo.nIgniteTime2 = nIgniteTime;
		mdInfo.nSpeed2 = nSpeed;


		this->getEventDispatcher()->removeAllEventListeners();
		this->removeAllChildren();
		director->replaceScene(TransitionFade::create(0.1f, PlayScene::createScene()));
		break;


	case STAGE_BACK:
		Back();
		break;

	default:
		break;
	}


}