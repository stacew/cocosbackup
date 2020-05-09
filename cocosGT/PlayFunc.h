#pragma once
#ifndef __PLAY_FUNC__
#define __PLAY_FUNC__
#include "../Util/define.h"
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
////////////////////////////////////////////////



class PlayFunc
{
private:
	Layer *pLayer;
	Director* director;
	Size visibleSize;
	Vec2 origin;
public:
	void init(Layer* a_pLayer);
	void ResetSprite(Ref* sender);
	void DeleteSprite(Ref* sender);
	void ResetParticle(Ref* sender);


	Vector<BlockSprite*> vGreen;
	Vector<BlockSprite*> vBlue;
	Vector<BlockSprite*> vRed;
	BlockSprite* getInvisibleBlock(int Type);

	int nGameType;

	int nPoint = 0;

	////////SPEED
	int nRemainTime; // 15~0
	int nPosition; // 0,1,2,3
	int nNewCount;
	//
	void SpeedTest();
	void SpeedNewPosition();

	////////REACTIVE
	int nRemainTest; // 10 ~ 1
	bool bAnswerGreen;
	int nReactiveTick; 
	int nPrevPoint;
	//
	void ReactiveTest();


	/////////TIMING
	int nRemainTouch; // 15 ~ 1
	int nTimingTick;
	int nPrevTouch;
	int nTimingPoint;
	//
	void TimingTest();

	///////ACCURACY
	float nACRemainGreen; // 150~1
	int nACTick;
	//
	void ACTest();
	Point GetPoint(int nNum);

	//////DV
	bool bExplain;
	int nDV_Stage; // 1 ~ 15
	bool bNeedNewBlock;
	bool bFirstPlay;

	int nReplayTick;
	bool bReplay;
	int nReplayCount; //replay & incorrectCount;
	int nCurrentShape;
	//
	void DVTest();
	void DVButton(Ref* pSender);
	void DVReplay();


	/////
	Point prevTouch;
	//
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void onTouchCancelled(Touch *touch, Event *unused_event);
	void update();
	void Back(Ref* pSender);


	////
	bool bUpdateGameOver;
	bool bGameOver;
	//
	void CheckGameOver();
	void ExitGame();

};

#endif // __PLAY_FUNC__