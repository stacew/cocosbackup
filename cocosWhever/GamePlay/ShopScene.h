#pragma once
#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__


#include "cocos2d.h"
USING_NS_CC;

#include "../Util/define.h"
#include "../Util/MyDataManager.h"

#ifdef PLUGIN_AD_MOB
#include  "pluginadmob/pluginadmob.h"
#endif

class ShopScene : public Layer
#ifdef PLUGIN_AD_MOB
	, public sdkbox::AdMobListener
#endif
{

	Size visibleSize;
	Size gameSize;

public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ShopScene);



	void UpdateLabel();

	void ExitShop();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	void SideMenu(Ref* pSender);
	void Callback(Ref* sender, int nType);
	void RemoveListener();


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
	//	cocos2d::MessageBox(msg.c_str(), "adViewDidFailToReceiveAdWithError");
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
	int nMoney = 100000;
	nMoney = nMoney * (1 + (UP_PER_POINT * mdp.nUpPoint));
	cocos2d::MessageBox(StringUtils::format("%d P", nMoney).c_str(), "++");
	mdp.nMoney += nMoney;
	mdp.SaveShop();
	UpdateLabel();
}
#endif

};

#endif // __SHOP_SCENE_H__