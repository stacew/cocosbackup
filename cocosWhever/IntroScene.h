#pragma once
#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class IntroScene : public LayerColor
{
public:
	Size visibleSize;
	Size gameSize;

	static Scene* createScene();
	virtual bool init();
	void initBG();
	CREATE_FUNC(IntroScene);


	bool bEnabledTouch;
	int nRotate;
	void NextScene();

	void Callback(Ref* sender, int nType);


	void scheduleCallback(float delta);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
};

#endif // __INTRO_SCENE_H__