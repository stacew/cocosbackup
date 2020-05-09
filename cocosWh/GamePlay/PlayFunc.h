#pragma once
#ifndef __PLAY_FUNC__
#define __PLAY_FUNC__
#include "../Util/define.h"
#include "../Util/MyDataManager.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
////////////////////////////////////////////////


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



class Player
{
	float nMaxLife; //Life
	float nCurrentLife;
public:
	void Init(float a_nMaxLife)
	{
		nMaxLife = a_nMaxLife;
		nCurrentLife = 0;
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
////////////////////////////////////////////////


#ifdef PLUGIN_AD_MOB
#include  "pluginadmob/pluginadmob.h"
#endif

class PlayFunc
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
	void MenuPop();
	void MenuPopData();

	void init(Layer* a_pLayer);

	void ResetSprite(Ref* sender);
	void DeleteSprite(Ref* sender);
	void ResetParticle(Ref* sender);

	Vector<BlockSprite*> vBlock[BLOCK_MAX_COUNT];
	// visible false인 블록 찾아서 가져오거나, false가 없으면 새로 만듬, true 세팅 후 리턴
	BlockSprite* getInvisibleBlock(int nType, int nOrder = 0);

	void CheckStageLevelUp();
	void UpdateProgress();
	void Invincible(float nTime);
	void ResetInvincible();
	void ResetCrashBar();

	bool bInvincible;


	int nMainTick;
	int nSoundTick;
	int nParticleTick;

	float nTimeDifficulty;

	bool bRecordMode;
	Player p1;

	bool bExtreme;
	int nGameNumber;
	int nStageLevel;
	int __nStagePoint__;
	int __nCombo__;
	int nMaxCombo;
	//////////////////////////////////////
	int	nResetToast;
	void ToastMessage(std::string);
	////////////////////////////////////common
	void ClearStage();//result popup과 continue 때문에 필요. //렙업 후 세팅 시 x

	void SettingStage();
	void UpdateStage();

	void ChangeLifePerTime(); // 초당 지속 피 감소
	void ChangePointPerTime(); // 초당 체력 유지 점수
	////////////////////////////////////common
	bool bNeedSetting;
	////////// common
	int nMaxArise;
	float nSumChangeLife;
	////////// common

	//1. Circle Pop 
	int nLineCount; //~1:4 ~4:9 ~8:16 ~13:25 ~19:36
	float nLineBlockScale;
	Vec2 arrPos[MAX_LINE_COUNT][MAX_LINE_COUNT]; // 6 = nLineCount MAX
	//2. Unify Type
	void Set_UnifyType();
	int arrType[MAX_LINE_COUNT][MAX_LINE_COUNT]; // 6 = nLineCount MAX
	//3. Jump Control
	float nSpeedY;
	//4. Watch & Touch
	bool bCircle;
	int nMaxClickCount;
	int nCurrentLimitCount;
	int nCurrentClickCount;


	//5. Fast Pop
	//6. Memorize Colors
	void Set_MemorizeColors();
	int nVisibleNumber;
	bool bSettingDelay;
	//7. Bounce Ball
	float nSpeedX;
	//8. break bricks
	bool bTouch;
	


	//9. Eraser
	//10. High Count
	void Set_HighCount();
	void Draw_HighCount();
	int arrCount[5];
	bool Check_HighCount(int nType);
	//11. Magnet dribble 
	bool bMagnetS;
	//12. Drag Control
	


	//13. Clay Shooting
	bool bShot;
	bool bFirstFire;
	//14. Mental Calc.;
	bool bMentalCalc_Answer;
	int nMentalCalc_PrevA; // 이전 문제 중복 방지
	void Set_MentalCalc();
	void MentalCalcMenu(Ref* pSender);
	//15. Driving
	float nSpeed;
	bool bRight;
	float nDegree;
	//16 Blocking Red



	//17. Random Fire
	int nAiming;
	int nFireTick;




	//96. Go Blank
	// bShot
	//98. Timing Touch
	int nStartNumber;
	int nCurrentNumber;

	//910. Make Way
	void Set_MakeWay();
	void Draw_MakeWay();
	void Check_MakeWay(int x, int y, int nType);
	//914. Rotate&Slide
	Sprite* getRSNumber(int nNumber);
	void Set_RotateSlide();
	void Shuffle_RotateSlide();
	void Draw_RotateSlide(bool bCheck);
	bool Check_RotateSlide();
	//////////////////////////////Callback
	void ShowHideMenu(bool bOn);
	void Callback(Ref* sender, int nType);
	//////////////////////////////


	///////
	void ChangeLife(float nDamage, bool bPercent = false, bool bRedFlash = true);
	void ChangeStagePoint(float nPoint);
	void UpdateCombo(float x, float y, int nChangeCombo = 1);

	void Sound(char arr[20], bool bSoundDelay = false);
	
	
	void Particle_Heal(Vec2 Pos, float nScale = 0.5f, bool bSound = true, bool bDelay = false);
	void Particle_Bang(Vec2 Pos, float nScale = 0.5f, bool bSound = true, bool bDelay = false);
	void Particle_Steam(Vec2 Pos, float nScale = 0.5f, bool bSound = true, bool bDelay = false);

	void Particle_O(Vec2 Pos, float nScale = 0.5f, bool bSound = true, bool bDelay = false);
	void Particle_X(Vec2 Pos, float nScale = 0.5f, bool bSound = true, bool bDelay = false);

	//////////////////////////////////////////////////
	Point prevTouchPoint;
	bool bPrevTouch; // 두 번 터치 되는 거 막을 때 사용
	bool bCallBackDelay; //효과 딜레이 주는 동안 터치 못하도록 사용
	int nCallBackDelayLevel; //효과 딜레이가 레벨업 후에 날아오는 경우 막기 위해
	//////////////////////////////////////////////////
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);
	void update();
	void SideMenu(Ref* pSender);


	//
	bool CheckGameOver();
	void ExitGame();
	void GoShop();

	/////////////////////////////////////////////////////////
	bool bLevelUp;
	bool bGameMenuPop;
	bool bExitScene;

	bool bShowContinueMenu;
	bool bContinueClick;
	bool bFullCountAd;
	/////////////////////////////////////////////////////////

	//GAME_COUNT_FIX
	const char* strKoTip[GAME_COUNT] = {
"동그라미를 놓치지 말고, 눌러 터뜨리세요."

,"도형을 누르면 형태가 변경돼요.\n\
모두 같은 형태가 되도록 하세요."

,"파란 공을 점프하려면 화면을 터치하세요.\n\
빨간 상자는 피하고 파란 상자는 잡으세요."

,"O 일 때만 화면을 누르세요."
//4

,"전부 빠르게 터뜨리세요."

,"처음 화면의 색을 기억하세요.\n\
2초 후 카드가 뒤집혔을 때 같은 색을 전부 찾으세요."

,"파란 공을 튕기려면 화면을 터치하세요.\n\
빨간 상자는 피하고 파란 상자는 잡으세요.\n\
파란 상자는 놓치면 안돼요."

,"파란 공을 이용하여 빨간 공을 컨트롤하세요.\n\
빨간 공이 아래에 닿지 않게하세요."
//8
,"빨간 상자가 땅에 떨어지기 전에 모두 지우세요."

, "가장 많은 갯수 도형을 클릭하세요."

,"자석을 이용하여 파란 공을 컨트롤 하세요.\n\
자석의 N극, S극은 주기적으로 변경돼요.\n\
빨간 상자는 피하고 파란 상자는 잡으세요.\n\
파란 상자는 놓치면 안돼요."

,"파란 공을 움직이려면 화면을 드래그하세요.\n\
빨간 상자는 피하고 파란 상자는 잡으세요."
//12

,"손가락을 놓으면 총을 발사 해요.\n\
파란 상자는 쏘지 말고, 빨간 상자만 쏘세요."//13

,"정확하고 빠르게 계산하세요." //14

,"화면을 누르거나 놓아서 조절할수 있어요\n\
화면의 왼쪽 / 오른쪽 방향이 달라요.\n\
빨간 상자는 피하고 파란 상자는 잡으세요.\n\
파란 상자는 놓치면 안돼요."

,"파란 공을 움직이려면 화면을 드래그하세요.\n\
하얀 공을 이용해 빨간 상자를 막으세요.\n\
빨간 상자는 피하고 파란 상자는 잡으세요."
//16

//,"조준점을 움직이면 사격을 시작해요.\n\
연속으로 사격 할 수록, 정확도가 떨어져요.\n\
파란 상자는 쏘지 말고, 빨간 상자만 쏘세요." // 17

//,"화면을 밀면 카드를 이동 시킬 수 있어요.\n\
화면 위, 아래의 색을 연결 할 수 길을 만드세요." //910
};


	const char* strTip[GAME_COUNT] = {
"Don't miss the circle, just push it."

,"Click on the shape to change its type.\n\
Make them all the same."

,"Touch the screen to jump the blue ball.\n\
Avoid the red box and catch the blue box."

,"O only press the screen."
//4

,"just push it."

,"Memorize the color of the first screen.\n\
When the card is turned overed after 2 seconds,\n\
find all the same colors."

,"Touch the screen to bounce the blue ball.\n\
Avoid the red box and catch the blue box.\n\
Don't miss the blue box."

,"Use the blue ball to control the red ball.\n\
Don't let the red ball touch below."
//8
,"Clear all the red boxes before they fall on the ground."

,"Click on the largest number of shapes."

,"Use the magnet to control the blue ball.\n\
The N and S poles are periodically changed.\n\
Avoid the red box and catch the blue box.\n\
Don't miss the blue box."

,"Drag the screen to move the blue ball.\n\
Avoid the red box and catch the blue box."
//12

,"Release your fingers and fire the gun.\n\
Don't shoot the blue box, Only shoot the red box." // 13

,"Calculate accurately and quickly."// 14

,"Pressing and releasing to adjust direction.\n\
The Left/Right of the screen is different direction.\n\
Avoid the red box and catch the blue box.\n\
Don't miss the blue box."

,"Drag the screen to move the blue ball.\n\
Use a white ball to block the red box.\n\
Avoid the red box and catch the blue box."
//16

//,"When you move the aiming point, you start shooting.\n\
The more you fire continuously, the less accurate.\n\
Don't shoot the blue box, Only shoot the red box."//17
//,"You can move the card by sliding the screen.\n\
Create a way to connect the top and bottom Color." // 910
	};


	bool bReadyAd;
	void ReadyAd();
	void ReadyReward();
	void ShowReward();

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
		int nMoney = 200000;
		nMoney = nMoney * (1 + (UP_PER_POINT * mdp.nUpPoint));
		cocos2d::MessageBox(StringUtils::format("%d P", nMoney).c_str(), "+++");
		mdp.nMoney += nMoney;
		mdp.SaveShop();
	}
#endif

};

#endif // __PLAY_FUNC__z