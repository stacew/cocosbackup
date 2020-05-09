#pragma once
#ifndef __STAGE_SELECT_SCENE_H__
#define __STAGE_SELECT_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class StageSelectScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();

	float nMaxPower;
	float nIgniteTime;
	float nSpeed;

	int nP1handy, nP2handy;
	void setLang();
	void updateButton();
	void updateClicker();
	void initBG();
	void initBGM();
	void initParticle();
	void menuCallback(Ref* pSender);
	CREATE_FUNC(StageSelectScene);

	void Back();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
};

#endif // __STAGE_SELECT_SCENE_H__