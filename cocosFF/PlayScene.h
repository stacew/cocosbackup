#pragma once
#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class PlayFunc;
class PlayScene : public Layer

{
private:
	PlayFunc* mPlay;

public:

	static Scene* createScene();
	virtual bool init();
	~PlayScene();


	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

	void update(float delta);

	CREATE_FUNC(PlayScene);

	void Exit();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

};

#endif // __PLAY_SCENE_H__
