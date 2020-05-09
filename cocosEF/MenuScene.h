#pragma once
#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;



#include"Util/define.h"
#ifdef PLUGIN_CHART_BOOST
#include "PluginChartBoost/PluginChartboost.h"
#endif
#ifdef PLUGIN_AD_COLONY
#include "pluginadcolony/pluginadcolony.h"
#endif
#ifdef PLUGIN_AD_MOB
#include  "pluginadmob/pluginadmob.h"
#endif
#ifdef PLUGIN_SHARE
#include "pluginshare/PluginShare.h"
#endif




class MenuScene : public Layer
#ifdef PLUGIN_CHART_BOOST
	, public sdkbox::ChartboostListener
#endif
#ifdef PLUGIN_AD_COLONY
	, public sdkbox::AdColonyListener
#endif
#ifdef PLUGIN_AD_MOB
	, public sdkbox::AdMobListener
#endif
#ifdef PLUGIN_SHARE
	, public sdkbox::ShareListener
#endif
{
public:


	int nAddNum;
	void ReadyAdd();
	void ShowAdd();

	void UpdateClicker();


	int nEasterStarCount;
    static Scene* createScene();
  
	
	virtual bool init();
	void setLang();
	void initBGM();
	void initBG();
	void initSound();
	void initParticle();
	void SideMenu(Ref* pSender);
	void menuCallback(Ref* pSender);
	CREATE_FUNC(MenuScene);

	void RemoveListener();
	void ExitGame();
	void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	int nKeyBackSpace = 0;



#ifdef PLUGIN_CHART_BOOST
	virtual void onChartboostCached(const std::string& name) {}
	/*!
	* should Chartboost display
	*/
	virtual bool onChartboostShouldDisplay(const std::string& name) {}
	/*!
	* Charboost ad has displayed
	*/
	virtual void onChartboostDisplay(const std::string& name) {}
	/*!
	* Chartboost ad has been dismissed
	*/
	virtual void onChartboostDismiss(const std::string& name) {}
	/*!
	* Chartboost is not running
	*/
	virtual void onChartboostClose(const std::string& name) {}
	/*!
	* Chartboost ad was clicked on
	*/
	virtual void onChartboostClick(const std::string& name) {}
	/*!
	* Chartboost reward was given
	*/
	virtual void onChartboostReward(const std::string& name, int reward) {}
	/*!
	* Chartboost failed to load
	*/
	virtual void onChartboostFailedToLoad(const std::string& name, sdkbox::CB_LoadError e) {}
	/*!
	* Chartboost failed to record click
	*/
	virtual void onChartboostFailToRecordClick(const std::string& name, sdkbox::CB_ClickError e) {}
	/*!
	* Chartboost confirmation
	*/
	virtual void onChartboostConfirmation() {}
	/*!
	* Chartboost complete store
	*/
	virtual void onChartboostCompleteStore() {}
#endif

#ifdef PLUGIN_AD_COLONY
	virtual void onAdColonyChange(const sdkbox::AdColonyAdInfo& info, bool available) {}
	/*!
	* reward was received.
	*/
	virtual void onAdColonyReward(const sdkbox::AdColonyAdInfo& info, const std::string& currencyName, int amount, bool success) {}
	/*!
	* showing an ad has started.
	*/
	virtual void onAdColonyStarted(const sdkbox::AdColonyAdInfo& info) {}
	/*!
	* showing an ad has finished.
	*/
	virtual void onAdColonyFinished(const sdkbox::AdColonyAdInfo& info) {}
#endif

#ifdef PLUGIN_AD_MOB
	//광고보기가 광고를 수신함
	virtual void adViewDidReceiveAd(const std::string &name)
	{
		//cocos2d::MessageBox(name.c_str(), "adViewDidReceiveAd"); 
	}
	//광고보기가 오류로 인해 광고를받지 못했습니다.
	virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg)
	{
		//cocos2d::MessageBox(name.c_str(), "adViewDidFailToReceiveAdWithError"); 
	}
	//광고보기가 나타납니다.화면
	virtual void adViewWillPresentScreen(const std::string &name)
	{
		//cocos2d::MessageBox(name.c_str(), "adViewWillPresentScreen"); 
	}
	//광고보기가 화면을 닫았습니다
	virtual void adViewDidDismissScreen(const std::string &name)
	{
		//cocos2d::MessageBox(name.c_str(), "adViewDidDismissScreen"); 
	}
	//광고보기가 화면을 닫을 것입니다.
	virtual void adViewWillDismissScreen(const std::string &name)
	{
		//cocos2d::MessageBox(name.c_str(), "adViewWillDismissScreen"); 
	}
	//광고보기가 응용 프로그램을 종료합니다.
	virtual void adViewWillLeaveApplication(const std::string &name)
	{
		if (name.compare("dfdfds") == 0)
		{
			;
		}
		else if (name.compare("adasda") == 0)
		{
			;
		}
		//cocos2d::MessageBox(name.c_str(), "adViewWillLeaveApplication");
	}
#endif

#ifdef PLUGIN_SHARE
	virtual void onShareState(const sdkbox::SocialShareResponse& response)
	{
		switch (response.state)
		{
			case sdkbox::SocialShareState::SocialShareStateNone: {
			
				CCLOG("SharePlugin::onShareState none");
				break;
			}
			case sdkbox::SocialShareState::SocialShareStateUnkonw: {	
				CCLOG("SharePlugin::onShareState unkonw");
				break;
			}
			case sdkbox::SocialShareState::SocialShareStateBegin: {			
				CCLOG("SharePlugin::onShareState begin");
				break;
			}
			case sdkbox::SocialShareState::SocialShareStateSuccess: {		
				CCLOG("SharePlugin::onShareState Success");
				break;
			}
			case sdkbox::SocialShareState::SocialShareStateFail: {		
				CCLOG("SharePlugin::onShareState fail, error:%s", response.error.c_str());
				break;
			}
			case sdkbox::SocialShareState::SocialShareStateCancelled: {		
				CCLOG("SharePlugin::onShareState cancelled");
				break;
			}
			case sdkbox::SocialShareStateSelectShow: {		
				CCLOG("SharePlugin::onShareState show pancel %d", response.platform);
				break;
			}
			case sdkbox::SocialShareStateSelectCancelled: {			
				CCLOG("SharePlugin::onShareState show pancel cancelled %d", response.platform);
				break;
			}
			case sdkbox::SocialShareStateSelected: {		
				CCLOG("SharePlugin::onShareState show pancel selected %d", response.platform);
				break;
			}
			default: {	
				CCLOG("SharePlugin::onShareState");
				break;
			}
		}
	}
#endif


};

#endif // __MENU_SCENE_H__
