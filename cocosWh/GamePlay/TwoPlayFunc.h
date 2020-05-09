#pragma once
#ifndef __TWO_PLAY_FUNC__
#define __TWO_PLAY_FUNC__
#include "../Util/define.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
////////////////////////////////////////////////
class BlockSprite : public Sprite
{
public:
	int nType;
	float nUDSpeed;
	float nLRSpeed;
	////////////////////
	static BlockSprite* createWithSpriteFrame(SpriteFrame *spriteFrame)
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
	//////////////////
	static BlockSprite* createWithSpriteFrameName(const std::string& spriteFrameName)
	{
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
#if COCOS2D_DEBUG > 0
		char msg[256] = { 0 };
		sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
		CCASSERT(frame != nullptr, msg);
#endif

		return createWithSpriteFrame(frame);
	}
	////////////////////
};
/////////////////////////////////////////////

class TwoPlayer
{
	float nMaxLife; //Life
	float nCurrentLife;
public:
	float nSpeedY;
	float nSpeedX;

	void Init(float a_nMaxLife)
	{
		nMaxLife = a_nMaxLife;
		nCurrentLife = 0;
		nSpeedY = 0;
		nSpeedX = 0;
	}


	void __ChangeLife__(float nLife, bool bPercent)
	{
		if (bPercent)
			nLife = nMaxLife / 100 * nLife;

		nCurrentLife += nLife;

		if (nCurrentLife > nMaxLife)
			nCurrentLife = nMaxLife;

		if (nCurrentLife < 0)
			nCurrentLife = 0;
	}

	float GetLife()
	{
		return nCurrentLife;
	}
	float GetLifePercent()
	{
		return nCurrentLife / nMaxLife * 100.0f;
	}
};
////////////////////////////////////////////////////////////

#ifdef PLUGIN_AD_MOB
#include  "pluginadmob/pluginadmob.h"
#endif

class TwoPlayFunc
#ifdef PLUGIN_AD_MOB
	: public sdkbox::AdMobListener
#endif
{
private:
	Layer *pLayer;
	Director* director;
	Size visibleSize;
	Size gameSize;

	Vec2 origin;
	Vec2 playSize;
	Vec2 center;


public:
	void init(Layer* a_pLayer);

	void ResetSprite(Ref* sender);
	void DeleteSprite(Ref* sender);
	void ResetParticle(Ref* sender);
	Vector<BlockSprite*> vBlock[BLOCK_MAX_COUNT];
	// visible false인 블록 찾아서 가져오거나, false가 없으면 새로 만듬, true 세팅 후 리턴
	BlockSprite* getInvisibleBlock(int nType, int nOrder = 0);


	int nMainTick;
	int nSoundTick;
	int nParticleTick;
	int nSoundTick2;
	int nParticleTick2;

	bool bExitScene;
	int nGameNumber;
	bool bExtreme;
	int nTimeDifficulty;

	TwoPlayer p1, p2;

	void StartTwoGame();
	void ExitGame();
	
	void SettingStage();
	void UpdateStage();
	void ClearStage();
	void UpdateProgress();
	void CheckGameOver();
	void ResetCrashBar(Ref* sender, int nPlayer);

	void ChangeLife(int nPlayer, float nDamage, bool bPercent = false, bool bRedFlash = true);
	//3. Jump Control

	//4. Reactive Push
	bool bCircle;
	int nMaxClickCount;
	int nCurrentLimitCount;
	int nCurrentClickCount;
	//7. Bounce Ball
	int nLineCount;
	float nLineBlockScale;
	Vec2 arrPos[MAX_LINE_COUNT][MAX_LINE_COUNT]; // 6 = nLineCount MAX
	int nMaxArise;

	//8. Break Bricks

	//14. Mental Calc.;
	bool bMentalCalc_Answer;
	int nMentalCalc_PrevA; // 이전 문제 중복 방지
	void Set_MentalCalc();
	void MentalCalcMenu(Ref* pSender);
	
	
	
	int nPlayerTurn;
	void ChangeLifePerTime(); // 초당 지속 피 감소

	void Sound(int nPlayer, char arr[20], bool bSoundDelay = false);
	void Particle_Steam(int nPlayer, Vec2 Pos, float nScale = 0.5f, bool bSound = true, bool bDelay = false);
	void Particle_Heal(int nPlayer, Vec2 Pos, float nScale = 0.5f, bool bSound = true, bool bDelay = false);
	void Particle_O(int nPlayer, Vec2 Pos, float nScale = 0.5f, bool bSound = true, bool bDelay = false);
	void Particle_X(int nPlayer, Vec2 Pos, float nScale = 0.5f, bool bSound = true, bool bDelay = false);

	bool bTouch1;
	bool bTouch2;
	Point startTouchPoint;
	Point startTouchPoint2;
	Point prevTouchPoint;
	Point prevTouchPoint2;

	void Callback(Ref* sender, int nType);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);
	void update();
	void SideMenu(Ref* pSender);



	bool bReadyAd;
	void ReadyAd();
	void ReadyVideo();
	void ShowVideo();

#ifdef PLUGIN_AD_MOB
	//광고보기가 광고를 수신함
	virtual void adViewDidReceiveAd(const std::string &name)
	{
		//	cocos2d::MessageBox(name.c_str(), "adViewDidReceiveAd"); 
	}
	//광고보기가 오류로 인해 광고를받지 못했습니다.
	virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg)
	{
		//	cocos2d::MessageBox(name.c_str(), "adViewDidFailToReceiveAdWithError"); 
	}
	//광고보기가 나타납니다.화면
	virtual void adViewWillPresentScreen(const std::string &name)
	{
		//	cocos2d::MessageBox(name.c_str(), "adViewWillPresentScreen"); 
	}
	//광고보기가 화면을 닫았습니다
	virtual void adViewDidDismissScreen(const std::string &name)
	{
		//	cocos2d::MessageBox(name.c_str(), "adViewDidDismissScreen"); 
	}
	//광고보기가 화면을 닫을 것입니다.
	virtual void adViewWillDismissScreen(const std::string &name)
	{
		//	cocos2d::MessageBox(name.c_str(), "adViewWillDismissScreen"); 
	}
	//광고보기가 응용 프로그램을 종료합니다.
	virtual void adViewWillLeaveApplication(const std::string &name)
	{
		//	cocos2d::MessageBox(name.c_str(), "adViewWillLeaveApplication");
	}
	virtual void reward(const std::string &name, const std::string &currency, double amount)
	{
		//	cocos2d::MessageBox(name.c_str(), "reward");
	}
#endif
};

#endif // __TWO_PLAY_FUNC__