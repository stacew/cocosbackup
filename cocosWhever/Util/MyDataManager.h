#pragma once
#ifndef __MY_DATA_MANGER_H__
#define __MY_DATA_MANGER_H__

#include "define.h"

#define mdp MyDataManager::getInstance()->mPlayerData
#define mdInfo MyDataManager::getInstance()->mGameInfoData

//사용자 데이터
class PlayerData
{
private:
public:

	//GAME_COUNT_FIX
	int nGAME_MAX_STAGE[GAME_COUNT] = {
		2, 3, 2, 2,
		2, 4, 3, 2,
		2, 3, 3, 3,
		2, 2, 3, 2
	};

	bool bTWO_PLAY[GAME_COUNT] = {
		false, false, true, true,
		false, false, true, true,
		false, false, false, false,
		false, true, false, false,
	};
	
	
	/////////////////////////
	bool bFirstGame;

	//시스템
	bool bBGMSwitch;
	bool bSoundSwitch;
	bool bMore;
	bool bEnglish;

	bool bExtreme;
	int nPageView;

	//int nUpMode;
	int nMoney;
	int nUpLife;
	int nUpRecovery;
	int nUpDefence;
	int nUpPoint;

	//MAX COMBO
	int nMaxCombo;

	//GAME PLAY COUNT
	int nGAME_PLAY_COUNT[GAME_COUNT];

	//GAME 최고 난이도
	int nGAME_NORMAL_STAGE[GAME_COUNT];
	int nGAME_NORMAL_POINT[GAME_COUNT];

	//EXTREME
	int nGAME_EXTREME_STAGE[GAME_COUNT];
	int nGAME_EXTREME_POINT[GAME_COUNT];


	//time
	int nBoosterTime;
	void SaveBoosterTime();

	bool checkOvertime(int nSavedTime, int a_nHour, int a_nMinute = 0, int a_nSecond = 0);
	struct tm getRemainTime(int nSavedTime, int a_nHour, int a_nMinute = 0, int a_nSecond = 0);
	//

	//
	PlayerData();
	void init();
	void SaveFirstGame();
	//
	void SaveSetting();
	void LoadFullData();
	void SaveFullData();

	void SaveShop();
	//
	void SaveMaxCombo();
	void SaveGamePlayCount(int GameNumber);
	void SaveGameStageLevel(int GameNumber, int nStageLevel);
	void SaveGamePoint(int GameNumber, int nPoint);

	//
	void CheckGameBestStage();

	////////////// 별 획득 갯수
	int GetPlayerLevel() 
	{
		int nLevel = 0;

		for (int i = 0; i < GAME_COUNT; i++)
		{
			nLevel += nGAME_NORMAL_STAGE[i];
		}

		return nLevel;
	}
	int GetTopPoint()
	{
		int nNumber = -1;
		int nMaxScore = 0;
		for (int i = 0; i < GAME_COUNT; i++)
		{
			if (nGAME_NORMAL_POINT[i] > nMaxScore)
			{
				nMaxScore = nGAME_NORMAL_POINT[i];
				nNumber = i;
			}
		}
		return nMaxScore;
	}
	int GetTopPointGameNumber()
	{
		int nNumber = -1;
		int nMaxScore = 0;
		for (int i = 0; i < GAME_COUNT; i++)
		{
			if (nGAME_NORMAL_POINT[i] > nMaxScore)
			{
				nMaxScore = nGAME_NORMAL_POINT[i];
				nNumber = i;
			}
		}
		return nNumber + 1;
	}
	int GetALLPoint()
	{
		int nTotal = 0;
		for (int i = 0; i < GAME_COUNT; i++)
		{
			nTotal += nGAME_NORMAL_POINT[i];
		}

		return nTotal;
	}
	int GetTrainingCompletedGame()
	{
		int nCount = 0;
		for (int i = 0; i < GAME_COUNT; i++)
		{
			if (nGAME_NORMAL_STAGE[i] == nGAME_MAX_STAGE[i])
				nCount++;

		}

		return nCount;
	}
	//////////////
	int GetExtremePlayerLevel()
	{
		int nLevel = 0;

		for (int i = 0; i < GAME_COUNT; i++)
		{
			nLevel += nGAME_EXTREME_STAGE[i];
		}

		return nLevel;
	}	
	int GetExtremeTopPoint()
	{
		int nNumber = -1;
		int nMaxScore = 0;
		for (int i = 0; i < GAME_COUNT; i++)
		{
			if (nGAME_EXTREME_POINT[i] > nMaxScore)
			{
				nMaxScore = nGAME_EXTREME_POINT[i];
				nNumber = i;
			}
		}
		return nMaxScore;
	}
	int GetExtremeTopPointGameNumber()
	{
		int nNumber = -1;
		int nMaxScore = 0;
		for (int i = 0; i < GAME_COUNT; i++)
		{
			if (nGAME_EXTREME_POINT[i] > nMaxScore)
			{
				nMaxScore = nGAME_EXTREME_POINT[i];
				nNumber = i;
			}
		}
		return nNumber + 1;
	}
	int GetExtremeALLPoint()
	{
		int nTotal = 0;
		for (int i = 0; i < GAME_COUNT; i++)
		{
			nTotal += nGAME_EXTREME_POINT[i];
		}

		return nTotal;
	}
	int GetExtremeTrainingCompletedGame()
	{
		int nCount = 0;
		for (int i = 0; i < GAME_COUNT; i++)
		{
			if (nGAME_EXTREME_STAGE[i] == nGAME_MAX_STAGE[i])
				nCount++;

		}

		return nCount;
	}
	//////////////

	int GetMaxLevel()
	{
		int nLevel = 0;

		for (int i = 0; i < GAME_COUNT; i++)
		{
			nLevel += nGAME_MAX_STAGE[i];
		}

		return nLevel;
	}



	
	int GetMostedPlayGameNumber()
	{
		int nNumber = -1;
		int nMaxPlayCount = 0;

		for (int i = 0; i < GAME_COUNT; i++)
		{
			if (nGAME_PLAY_COUNT[i] > nMaxPlayCount)
			{
				nMaxPlayCount = nGAME_PLAY_COUNT[i];
				nNumber = i;
			}
		}

		return nNumber + 1;
	}

	int GetMostedPlayCount()
	{
		int nNumber = -1;
		int nMaxPlayCount = 0;

		for (int i = 0; i < GAME_COUNT; i++)
		{
			if (nGAME_PLAY_COUNT[i] > nMaxPlayCount)
			{
				nMaxPlayCount = nGAME_PLAY_COUNT[i];
				nNumber = i;
			}
		}

		return nMaxPlayCount;
	}

	int GetTotalPlayCount()
	{
		int nTotal = 0;
		for (int i = 0; i < GAME_COUNT; i++)
		{
			nTotal += nGAME_PLAY_COUNT[i];
		}
		return nTotal;
	}

	
};



//게임 전달에 필요한 데이터
class GameInfoData
{
public:
	GameInfoData()
	{
		nGameNumber = 0;
		nPlayCount = 0;
	}
	/////////////////////////
	int nGameNumber;
	//
	int nPlayCount;
	//
};



class MyDataManager
{
public:

	static MyDataManager* getInstance();
	~MyDataManager();

	PlayerData mPlayerData;
	GameInfoData mGameInfoData;
};

#endif __MY_DATA_MANGER_H__