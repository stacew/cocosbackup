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
	float nMaxLife; //Life
	float nCurrentLife;

	float nMaxPower; // Fire Power
	float nCurrentPower;

	////////////
	float nIgniteTime; // Fire Power Charge Time
	bool bChargeMode; // Charge Mode

	bool bFireMode; // Normal
	/////////////////

	float nRecovery; // Recovery

	void init();
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
	Player p1;
	Vector<BlockSprite*> vCloud1;
	BlockSprite* getInvisibleBlock(int Type);

	int nPlayerPosition;
	void SetBlock(int nLevel); // 0 is Player->getPoint Rain
	float nSetBlock_CloudTick;

	int nSelectedChar;

	int nCombo;
	int nMaxCombo;
	int nPoint;


	bool bUpdateGameOver;
	bool bGameOver;
	int nGameoverTick;

	void init(Layer* a_pLayer);
	void InitBG();
	void InitProgress();
	void InitCharacterSprite();
	void InitPrevScore();
	void InitPause();
	void InitInfo();

	int nTouchTick;
	bool bIsTouch;
	bool bBooster;
	bool bJump;

	float nDownSpeed;
	int nCrashSoundTick;
	int nBurnSoundTick;
	int nBurnSoundId;
	void CheckJumpAndBooster();
	void MoveCharacter(int PlayerNum);

	void Stage();
	void Stage_Label();
	void Stage_SetBlock();

	void CrashBlock(int PlayerNum);
	void resetParticle(Ref* sender);
	void ResetSprite(Ref* sender);

	void UpdateProgress(int PlayerNum);
	void UpdateCharacter(int PlayerNum);

	void CheckGameOver();
	void ExitGame();

	bool TouchInOut(Touch *touch, bool bTouch);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);
	void update();


	bool bStart;
	void Pause(Ref* pSender);
	void Resume(Ref* pSender);
	void PauseExit(Ref* pSender);
};

#endif // __PLAY_FUNC__