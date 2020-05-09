#pragma once
#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;


#include "Util/define.h"
#include "Util/MyDataManager.h"

#ifdef PLUGIN_AD_MOB
#include  "pluginadmob/pluginadmob.h"
#endif

class MenuScene : public Layer
#ifdef PLUGIN_AD_MOB
	, public sdkbox::AdMobListener
#endif
{
	Size visibleSize;
	Size gameSize;
public:
	//
	int nEventCount;
	//

    static Scene* createScene();
	
	virtual bool init();
	void initBGM();
	void initBG();
	void initSound();
	void SideMenu(Ref* pSender, int nCallNumber);
	void initBGAction();
	void MenuOnOff(bool bOn);

	void InitMenu();
	void UpdateMenu();

	bool bExtend;
	void ExtendNote();

	void ToastMessage(std::string str);

	void Callback(Ref* sender, int nType);
	void menuCallback(Ref* pSender);


	CREATE_FUNC(MenuScene);

	void RemoveListener();
	void ExitGame();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	int nKeyBackSpace;



	Point prevTouchPoint;
	Point prevTouchPoint_Drag;
	float MenuBaseX;
	float MenuLabelBaseX;
	float MenuLabelBaseX2;
	float MenuInfoBaseX;
	float MenuLabelBaseX3;

	float twoPlayerBaseX;

	bool bPrevTouch;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

	void update(float delta);
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	bool bReadyAd;
	void ReadyAd();
	void ReadyBanner();
	void ShowBanner();
	void HideBanner();

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

#endif // __MENU_SCENE_H__
