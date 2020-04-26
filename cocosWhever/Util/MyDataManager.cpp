#include "MyDataManager.h"
#include "cocos2d.h"
#include <time.h>
USING_NS_CC;
using namespace std;


static MyDataManager* _MyDataManager = nullptr;


MyDataManager* MyDataManager::getInstance()
{
	if (!_MyDataManager)
	{
		_MyDataManager = new (nothrow) MyDataManager();
	}

	return _MyDataManager;
}

MyDataManager::~MyDataManager()
{
	if (_MyDataManager)
	{
		delete _MyDataManager;
	}
	_MyDataManager = nullptr;
}


PlayerData::PlayerData()
{
	init();
}

void PlayerData::init()
{
	bFirstGame = true;
	//시스템
	bBGMSwitch = true;
	bSoundSwitch = true;

	bMore = false;

	bEnglish = true;

#if ONESTORE
	bEnglish = false;
#endif

	bExtreme = false;
	nPageView = 0;

	nMoney = 0;
	nUpLife = 0;
	nUpRecovery = 0;
	nUpDefence = 0;
	nUpPoint = 0;

	nMaxCombo = 0;
	
	
	for (int i = 0; i < GAME_COUNT; i++)
	{
		nGAME_PLAY_COUNT[i] = 0;

		nGAME_NORMAL_STAGE[i] = 0;
		nGAME_NORMAL_POINT[i] = 0;

		nGAME_EXTREME_STAGE[i] = 0;
		nGAME_EXTREME_POINT[i] = 0;
	}

	nBoosterTime = 0;
}

void PlayerData::SaveBoosterTime()
{
	time_t oCurrentTime_t;
	time(&oCurrentTime_t);
	nBoosterTime = oCurrentTime_t;
	UserDefault::getInstance()->setIntegerForKey("nBoosterTime", nBoosterTime);
}

//nSavedTime에 a_nHour만큼 넘어갔다면 true
bool PlayerData::checkOvertime(int nSavedTime, int a_nHour, int a_nMinute, int a_nSecond)
{
	time_t oSavedTime_t = nSavedTime;
	struct tm* pSavedTimeStruct = localtime(&oSavedTime_t);
	pSavedTimeStruct->tm_hour = pSavedTimeStruct->tm_hour + a_nHour;
	pSavedTimeStruct->tm_min = pSavedTimeStruct->tm_min + a_nMinute;
	pSavedTimeStruct->tm_sec = pSavedTimeStruct->tm_sec + a_nSecond;

	time_t oOverTime_t = mktime(pSavedTimeStruct); // 제한 시간 만들기

	time_t oCurrentTime_t;
	time(&oCurrentTime_t); //현재 시간 가져오기

	if (difftime(oOverTime_t, oCurrentTime_t) <= 0) //현재 시간이 제한 시간을 넘어갔다면
		return true;
	else
		return false;
}

struct tm PlayerData::getRemainTime(int nSavedTime, int a_nHour, int a_nMinute, int a_nSecond)
{
	time_t oSavedTime_t = nSavedTime;
	struct tm oSavedTimeStruct(*localtime(&oSavedTime_t));
	oSavedTimeStruct.tm_hour = oSavedTimeStruct.tm_hour + a_nHour;
	oSavedTimeStruct.tm_min = oSavedTimeStruct.tm_min + a_nMinute;
	oSavedTimeStruct.tm_sec = oSavedTimeStruct.tm_sec + a_nSecond;
	time_t oOverTime_t = mktime(&oSavedTimeStruct); // 제한 시간 만들기

	time_t oCurrentTime_t;
	time(&oCurrentTime_t); //현재 시간 가져오기

	if (difftime(oOverTime_t, oCurrentTime_t) <= 0) //현재 시간이 제한 시간을 넘어갔다면
	{
		oSavedTimeStruct.tm_hour = 0;
		oSavedTimeStruct.tm_min = 0;
		oSavedTimeStruct.tm_sec = 0;
		return oSavedTimeStruct;
	}
	else
	{
		struct tm oCurrentTms(*localtime(&oCurrentTime_t));
		oSavedTimeStruct.tm_hour = oSavedTimeStruct.tm_hour - oCurrentTms.tm_hour;
		oSavedTimeStruct.tm_min = oSavedTimeStruct.tm_min - oCurrentTms.tm_min;
		oSavedTimeStruct.tm_sec = oSavedTimeStruct.tm_sec - oCurrentTms.tm_sec;
		time_t tt = mktime(&oSavedTimeStruct);
		struct tm oReturnTms(*localtime(&tt));
		return oReturnTms;
	}
}

void PlayerData::SaveFirstGame()
{
	bFirstGame = false;

	auto UserDef = UserDefault::getInstance();
	UserDef->setBoolForKey("bFirstGame", bFirstGame);
}


void PlayerData::SaveSetting()
{
	auto UserDef = UserDefault::getInstance();
	UserDef->setBoolForKey("bBGMSwitch", bBGMSwitch);
	UserDef->setBoolForKey("bSoundSwitch", bSoundSwitch);
	UserDef->setBoolForKey("bMore", bMore);
	UserDef->setBoolForKey("bEnglish", bEnglish);

	UserDef->setIntegerForKey("nPageView", nPageView);
	UserDef->setBoolForKey("bExtreme", bExtreme);
}

void PlayerData::SaveShop()
{
	auto UserDef = UserDefault::getInstance();
	UserDef->setIntegerForKey("nMoney", nMoney);
	UserDef->setIntegerForKey("nUpLife", nUpLife);
	UserDef->setIntegerForKey("nUpRecovery", nUpRecovery);
	UserDef->setIntegerForKey("nUpDefence", nUpDefence);
	UserDef->setIntegerForKey("nUpPoint", nUpPoint);
}

void PlayerData::SaveMaxCombo()
{
	auto UserDef = UserDefault::getInstance();
	UserDef->setIntegerForKey("nMaxCombo", nMaxCombo);
}

void PlayerData::SaveGamePlayCount(int GameNumber)
{
	auto UserDef = UserDefault::getInstance();
	UserDef->setIntegerForKey(StringUtils::format("nGPC%d", GameNumber).c_str(), nGAME_PLAY_COUNT[GameNumber]);
}

void PlayerData::SaveGameStageLevel(int GameNumber, int nStageLevel)
{
	auto UserDef = UserDefault::getInstance();
	if (bExtreme == true)
	{
		nGAME_EXTREME_STAGE[GameNumber] = nStageLevel;
		UserDef->setIntegerForKey(StringUtils::format("nGES%d", GameNumber).c_str(), nGAME_EXTREME_STAGE[GameNumber]);
	}
	else
	{
		nGAME_NORMAL_STAGE[GameNumber] = nStageLevel;
		UserDef->setIntegerForKey(StringUtils::format("nGBS%d", GameNumber).c_str(), nGAME_NORMAL_STAGE[GameNumber]);
	}

}

void PlayerData::SaveGamePoint(int GameNumber, int nPoint)
{
	auto UserDef = UserDefault::getInstance();
	if (bExtreme == true)
	{
		nGAME_EXTREME_POINT[GameNumber] = nPoint;
		UserDef->setIntegerForKey(StringUtils::format("nGEP%d", GameNumber).c_str(), nGAME_EXTREME_POINT[GameNumber]);
	}
	else
	{
		nGAME_NORMAL_POINT[GameNumber] = nPoint;
		UserDef->setIntegerForKey(StringUtils::format("nGIP%d", GameNumber).c_str(), nGAME_NORMAL_POINT[GameNumber]);
	}

}

void PlayerData::LoadFullData()
{
	auto UserDef = UserDefault::getInstance();
	bFirstGame = UserDef->getBoolForKey("bFirstGame", bFirstGame);
	//
	bEnglish = UserDef->getBoolForKey("bEnglish", bEnglish);

	bBGMSwitch = UserDef->getBoolForKey("bBGMSwitch", bBGMSwitch);
	bSoundSwitch = UserDef->getBoolForKey("bSoundSwitch", bSoundSwitch);
	bMore = UserDef->getBoolForKey("bMore", bMore);
	//
	for (int i = 0; i < GAME_COUNT; i++)
	{
		nGAME_NORMAL_STAGE[i] = UserDef->getIntegerForKey(
			StringUtils::format("nGBS%d", i).c_str(), nGAME_NORMAL_STAGE[i]);
	}
	for (int i = 0; i < GAME_COUNT; i++)
	{
		nGAME_EXTREME_STAGE[i] = UserDef->getIntegerForKey(
			StringUtils::format("nGES%d", i).c_str(), nGAME_EXTREME_STAGE[i]);
	}
	//
	for (int i = 0; i < GAME_COUNT; i++)
	{
		nGAME_PLAY_COUNT[i] = UserDef->getIntegerForKey(
			StringUtils::format("nGPC%d", i).c_str(), nGAME_PLAY_COUNT[i]);
	}
	//
	for (int i = 0; i < GAME_COUNT; i++)
	{
		nGAME_NORMAL_POINT[i] = UserDef->getIntegerForKey(
			StringUtils::format("nGIP%d", i).c_str(), nGAME_NORMAL_POINT[i]);
	}
	for (int i = 0; i < GAME_COUNT; i++)
	{
		nGAME_EXTREME_POINT[i] = UserDef->getIntegerForKey(
			StringUtils::format("nGEP%d", i).c_str(), nGAME_EXTREME_POINT[i]);
	}
	//


	CheckGameBestStage();

	nMoney = UserDef->getIntegerForKey("nMoney", nMoney);
	nUpLife = UserDef->getIntegerForKey("nUpLife", nUpLife);
	nUpRecovery = UserDef->getIntegerForKey("nUpRecovery", nUpRecovery);
	nUpDefence = UserDef->getIntegerForKey("nUpDefence", nUpDefence);
	nUpPoint = UserDef->getIntegerForKey("nUpPoint", nUpPoint);
	//
	nMaxCombo = UserDef->getIntegerForKey("nMaxCombo", nMaxCombo);
	//
	nPageView = UserDef->getIntegerForKey("nPageView", nPageView);
	//
	bExtreme = UserDef->getBoolForKey("bExtreme", bExtreme);
	//

	nBoosterTime = UserDef->getIntegerForKey("nBoosterTime", nBoosterTime);
}

void PlayerData::CheckGameBestStage()
{
	for (int i = 0; i < GAME_COUNT; i++)
	{
		if (nGAME_NORMAL_STAGE[i] > nGAME_MAX_STAGE[i])
			nGAME_NORMAL_STAGE[i] = 0;

		if (nGAME_EXTREME_STAGE[i] > nGAME_MAX_STAGE[i])
			nGAME_EXTREME_STAGE[i] = 0;
	}
}



void PlayerData::SaveFullData()
{
	auto UserDef = UserDefault::getInstance();

	UserDef->setBoolForKey("bFirstGame", bFirstGame);
	//
	UserDef->setBoolForKey("bEnglish", bEnglish);

	UserDef->setBoolForKey("bBGMSwitch", bBGMSwitch);
	UserDef->setBoolForKey("bSoundSwitch", bSoundSwitch);
	UserDef->setBoolForKey("bMore", bMore);
	//
	for (int i = 0; i < GAME_COUNT; i++)
	{
		UserDef->setIntegerForKey(
			StringUtils::format("nGBS%d", i).c_str(), nGAME_NORMAL_STAGE[i]);
	}
	for (int i = 0; i < GAME_COUNT; i++)
	{
		UserDef->setIntegerForKey(
			StringUtils::format("nGES%d", i).c_str(), nGAME_EXTREME_STAGE[i]);
	}
	//
	for (int i = 0; i < GAME_COUNT; i++)
	{
		UserDef->setIntegerForKey(
			StringUtils::format("nGPC%d", i).c_str(), nGAME_PLAY_COUNT[i]);
	}
	//
	for (int i = 0; i < GAME_COUNT; i++)
	{
		UserDef->setIntegerForKey(
			StringUtils::format("nGIP%d", i).c_str(), nGAME_NORMAL_POINT[i]);
	}
	for (int i = 0; i < GAME_COUNT; i++)
	{
		UserDef->setIntegerForKey(
			StringUtils::format("nGEP%d", i).c_str(), nGAME_EXTREME_POINT[i]);
	}
	//

	UserDef->setIntegerForKey("nPageView", nPageView);
	UserDef->setBoolForKey("bExtreme", bExtreme);
	UserDef->setIntegerForKey("nMaxCombo", nMaxCombo);

	UserDef->setIntegerForKey("nMoney", nMoney);
	UserDef->setIntegerForKey("nUpLife", nUpLife);
	UserDef->setIntegerForKey("nUpRecovery", nUpRecovery);
	UserDef->setIntegerForKey("nUpDefence", nUpDefence);
	UserDef->setIntegerForKey("nUpPoint", nUpPoint);
}



