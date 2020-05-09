#pragma once
#ifndef __TWO_PLAY_SCENE_H__
#define __TWO_PLAY_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class TwoPlayFunc;
class TwoPlayScene : public Layer
{
private:
	TwoPlayFunc* mPlay;

public:

	static Scene* createScene();
	virtual bool init();
	~TwoPlayScene();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

	void update(float delta);

	CREATE_FUNC(TwoPlayScene);

	void Exit();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

};

#endif // __TWO_PLAY_SCENE_H__
