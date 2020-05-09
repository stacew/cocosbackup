#pragma once
#ifndef __PLAY_FUNC__
#define __PLAY_FUNC__
#include "cocos2d.h"
USING_NS_CC;
using namespace std;


////////////////////////////////////////////////
class BlockSprite : public Sprite
{
public:
	float nDamage;
	float nSpeed;
	static BlockSprite* createWithSpriteFrame(SpriteFrame *spriteFrame);
	static BlockSprite* createWithSpriteFrameName(const std::string& spriteFrameName);
};
/////////////////////////////////////////////
class Player
{
public:
	float nMaxLife;
	float nCurrentLife;
	float nMaxPower;
	float nCurrentPower;
	float nIgniteTime;

	bool bChargeMode;
	bool bFireMode;
	bool bStarMode;
	float nStarTime;

	float nSpeed;
	int nDirection;


	Animate* animate;
	bool bDieRedraw;

	void init(int a_nDirection);
};
////////////////////////////////////////////////



class PlayFunc
{
private:
	Layer *pLayer;
	Director* director;
	Size visibleSize;
	Vec2 origin;
public:
	bool bTurn;
	Player p1,p2;
	Vector<BlockSprite*> vRain1;
	BlockSprite* getInvisibleRain(int Type);
	Vector<BlockSprite*> vStar1;
	BlockSprite* getInvisibleStar(int Type);


	int nGameType;
	int nStageLv;
	int nDifficulty;
	float nDifficultySpeed;
	float nSetBlock_BaseRainTick;
	float nSetBlock_RainTick;
	float nSetBlock_StarTick;

	int nTimeTick;

	bool bUpdateGameOver;
	bool bGameOver;
	int nGameoverTick;

	void init(Layer* a_pLayer);
	void InitBGM();
	void EndBGM();
	void InitBG();
	void InitClickLabel();
	void InitParticle();
	void InitProgress();
	void InitCharacterSprite();
	void MoveCharacter(int PlayerNum);


	void Stage();
	void Stage_Label();
	void Stage_SetBlock();


	bool bShowerStage;
	bool bHeavyRainStage;
	bool bStormStage;
	bool bUpdraftStage;
	bool bGrowStage;
	bool bDispersionStage;
	bool bCombineStage;
	bool bJumpStage;

	bool bPlayerPositionStar;
	void SetBlock_Star(int nLevel = 1);
	bool bPlayerPositionRain;
	void SetBlock_Rain(int nLevel = 1); // 0 is Player->getPoint Rain
	void SetBlock_Tornado(int nLevel = 1);
	bool bTornadoCenter;
	void SetBlock_Tsunami(int nLevel = 1);
	void UpdateCombine();
	void SetBlock_Jump(int nLevel = 1);

	void CrashStar(int PlayerNum);
	void CrashBlock(int PlayerNum);
	void resetPlayerCrashParticle(Ref* sender);
	void ResetSprite(Ref* sender);
	void DispersionRain(Ref* sender);

	void UpdateStarModeDelay();


	void UpdateProgress(int PlayerNum);
	void UpdateStarLife(int PlayerNum);
	void UpdateCharacter(int PlayerNum);



	bool bStart;
	void CheckGameOver();
	void ExitGame();

	bool TouchInOut(Touch *touch, bool bTouch);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);
	void update();


	void Pause(Ref* pSender);
	void Resume(Ref* pSender);
	void PauseExit(Ref* pSender);

	void SelectDifficultyToStart(Ref* pSender);
	int nPreClearLevel;
};

#endif // __PLAY_FUNC__