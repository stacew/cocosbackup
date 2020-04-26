#pragma once
#ifndef __DEFINE_H__
#define __DEFINE_H__

// IOS에서 SDKBOX_ENABLED 해제됨... gps 안받아서 그런지
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
#define SDKBOX_ENABLED
#endif

#ifdef SDKBOX_ENABLED
#define PLUGIN_AD_MOB

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define PLUGIN_GPG	
#endif
#endif

#define ONESTORE 0
#define DEVELOPER 0

#ifdef PLUGIN_AD_MOB
#define AD_MOB_INTERSTITIAL		"BrainTrainer"
#define AD_MOB_BANNER			"BrainTrainer_Banner"
#define AD_MOB_REWARD			"BrainTrainer_Reward"
#endif


#define SIDE_FACEBOOK_URL		"https://www.facebook.com/onebuttongames"
#define SIDE_GOOGLE_URL			"https://goo.gl/2t1Yt1"
#define SIDE_ONESTORE_URL		"http://onesto.re/O000719351"

#if ONESTORE == 1
#define SIDE_HOME_URL		SIDE_ONESTORE_URL
#define USE_LEADERBOARD	1
#else
#define USE_LEADERBOARD	1
#define SIDE_HOME_URL		SIDE_GOOGLE_URL
#endif

#define LEADERBOARD_STAR		0
#define LEADERBOARD_TOP			1
#define LEADERBOARD_ALL			2
#define LEADERBOARD_STAR_EX		3
#define LEADERBOARD_TOP_EX		4
#define LEADERBOARD_ALL_EX		5


#define SIDE_HOME_IOS_EN_URL	"https://itunes.apple.com/app/id1269069335"
#define SIDE_HOME_IOS_KO_URL	"https://itunes.apple.com/kr/app/id1269069335"

#define SIDE_GPGS_URL			"https://goo.gl/vhy5ca"


#define SIDE_GPGS_FAILED_EN "Failed"
#define SIDE_GPGS_FAILED_EN2 "'Google Play Games' App install and login required."
#define SIDE_GPGS_FAILED_KO "실패"
#define SIDE_GPGS_FAILED_KO2 "'구글 플레이 게임' 앱 설치 및 로그인이 필요합니다."

#include "cocos2d.h"
USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

static const std::string UTF8(const std::string& string)
{
	int bufflen = MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, NULL, 0);
	WCHAR* widebuff = new WCHAR[bufflen + 1];
	memset(widebuff, 0, sizeof(WCHAR)* (bufflen + 1));
	MultiByteToWideChar(CP_ACP, 0, string.c_str(), -1, widebuff, bufflen);

	bufflen = WideCharToMultiByte(CP_UTF8, 0, widebuff, -1, NULL, 0, NULL, NULL);
	char* buff = new char[bufflen + 1];
	memset(buff, 0, sizeof(char)* (bufflen + 1));
	WideCharToMultiByte(CP_UTF8, 0, widebuff, -1, buff, bufflen, NULL, NULL);

	std::string strResult(buff);

	delete[] widebuff;
	delete[] buff;

	return strResult;
}

#define UTF8PCHAR(string)    UTF8(string).c_str()

#else

#define UTF8(string)    string
#define UTF8PCHAR(string)    string

#endif

//font
#define _fonts_Base "fonts/NotoSansKR-Regular.otf"

//image
#define _image_plist	"image/imagePlist.plist"
#define _image_RP1		"image/RotatePuzzle1.png"
//#define _image_RPBG		"image/RotatePuzzleBG.png"		

//particle
#define _particle_steam				"particle/particle_steam.plist"	//
#define _particle_steam2			"particle/particle_steam2.plist"//2p
#define _particle_bang				"particle/particle_bang.plist"	//
#define _particle_heal				"particle/particle_heal.plist"	//
#define _particle_o					"particle/particle_o.plist"
#define _particle_x1				"particle/particle_x1.plist"
#define _particle_x2				"particle/particle_x2.plist"
#define _particle_x3				"particle/particle_x3.plist"
#define _particle_x4				"particle/particle_x4.plist"

//bgm
#define _bgm_calm				"bgm/calm.mp3"
#define _bgm_feelinggood		"bgm/feelinggood.mp3"
#define _bgm_stepstepstep		"bgm/stepstepstep.mp3"
#define _bgm_happytune			"bgm/happytune.mp3"
#define _bgm_forest				"bgm/forest.mp3"


//sound
#define _sounds_G1		"sounds/g1.wav"
#define _sounds_G2		"sounds/g2.wav"
#define _sounds_R1		"sounds/r1.wav"
#define _sounds_R2		"sounds/r2.wav"
#define _sounds_Bang		"sounds/bang.wav"
#define _sounds_Bbi			"sounds/bbi.wav" //bbi~
#define _sounds_Bubble		"sounds/bubble.wav" //bubble
#define _sounds_Door		"sounds/door.wav"
#define _sounds_Fire		"sounds/fire.wav"	//pha~~~
#define _sounds_Steam		"sounds/steam.wav"	//cheeeek~
#define _sounds_Sword		"sounds/sword.wav"
#define _sounds_Tong		"sounds/tong.wav"
#define _sounds_Touch		"sounds/touch.wav" //thuk~
#define _sounds_Coin		"sounds/coin.wav"

//mdp & game Tag
#define GAME_COUNT		16 //GAME_COUNT_FIX
#define PAGE_COUNT		((GAME_COUNT - 1) / 4 + 1)




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/////////Block Type
#define RED_CIRCLE		0
#define GREEN_CIRCLE	1
#define BLUE_CIRCLE		2
#define YELLOW_CIRCLE	3
#define WHITE_CIRCLE	4
#define BLACK_CIRCLE	5

#define RED_SQUARE		6
#define GREEN_SQUARE	7
#define BLUE_SQUARE		8
#define YELLOW_SQUARE	9
#define WHITE_SQUARE	10
#define BLACK_SQUARE	11

#define ALPHA_BLACK_CIRCLE	12
#define ALPHA_BLACK_SQUARE	13

#define BLOCK_LINE_0	14
#define BLOCK_LINE_1	15
#define BLOCK_LINE_2	16
#define BLOCK_LINE_3	17
#define BLOCK_LINE_4	18
#define BLOCK_LINE_5	19
#define BLOCK_LINE_1G	20
#define BLOCK_LINE_2G	21
#define BLOCK_LINE_3G	22
#define BLOCK_LINE_4G	23
#define BLOCK_LINE_5G	24
#define BLOCK_LINE_1B	25
#define BLOCK_LINE_2B	26
#define BLOCK_LINE_3B	27
#define BLOCK_LINE_4B	28
#define BLOCK_LINE_5B	29

#define G2				30

#define BLOCK_BASE			0
#define BLOCK_MAX_COUNT		31 //////////////

static const char* strBlockName[BLOCK_MAX_COUNT] = {
	"red_c.png"
	,"green_c.png"
	,"blue_c.png"
	,"yellow_c.png"
	,"white_c.png"
	,"black_c.png"

	,"red_s.png"
	,"green_s.png"
	,"blue_s.png"
	,"yellow_s.png"
	,"white_s.png"
	,"black_s.png"

	,"a_black_c.png"
	,"a_black_s.png"

	,"line0.png"
	,"line1.png"
	,"line2.png"
	,"line3.png"
	,"line4.png"
	,"line5.png"

	,"line1g.png"
	,"line2g.png"
	,"line3g.png"
	,"line4g.png"
	,"line5g.png"

	,"line1b.png"
	,"line2b.png"
	,"line3b.png"
	,"line4b.png"
	,"line5b.png"

	,"g2.png"
};
/////////Block Type

//game Constant
#define GAME_STAGE_POINT			10000
#define GAME_RECORD_MAX_POINT		1000000

#define PLAYER_BLUE_BASE_SCALE			0.1f
#define MISSILE_SCALE					0.15f
#define PATTERN_TICK					250


#define JUMP_POWER						12.0f
#define G_ACCEL							0.6f
#define EDGE_ELASTIC					0.5f


#define MAX_LINE_COUNT					8
#define SCALE_LINE_2					0.95f	
#define SCALE_LINE_3					0.6f
#define SCALE_LINE_4					0.45f
#define SCALE_LINE_5					0.35f
#define SCALE_LINE_6					0.3f
#define SCALE_LINE_7					0.27f
#define SCALE_LINE_8					0.23f

#define POINT_TYPE05					0.5f
#define POINT_TYPE0						1
#define POINT_TYPE1						5
#define POINT_TYPE2						25
#define POINT_TYPE3						100
#define POINT_TYPE4						500
#define POINT_TYPE5						1000
#define POINT_TYPE6						2000
#define POINT_TYPE7						5000
//game tag
#define PLAYER_BLUE						100000
#define PLAYER_YELLOW					100001
#define PLAYER_LIFE_BAR					110000
#define PLAYER_LIFE_BAR2				110001//2p
#define PLAYER_BACK_BAR					120000
#define STAGE_BAR						130000
#define CRASH_BAR						140000
#define CRASH_BAR2						140001//2p
#define CRASH_BG						150000
#define BG_RED_EDGE						160000
#define BG_RED_EDGE2					160001//2p
#define BG_PUZZLE						170000

#define GAME_BG_POP						210000
#define GAME_BG_CIRCLE					220000
#define GAME_BG_CIRCLE_HIDE				220001
#define GAME_BG_CIRCLE_EXIT				220002
#define GAME_BG_CIRCLE_SHOP				220003
#define GAME_BG_CIRCLE2					230000

#define GAME_MENU_BG					200001
#define GAME_MENU_START					200002
#define GAME_MENU_CONTINUE				200004
#define GAME_MENU_MENU					200005
#define GAME_SIDE_MENU_UP				200006
#define GAME_SIDE_MENU_DOWN				200007
#define GAME_SIDE_EXIT					200008
#define GAME_SIDE_RESET					200009
#define GAME_SIDE_SHOP					200010
#define GAME_SIDE_REPLAY				200011//2p


#define UPMODE_LIFE						0
#define UPMODE_DAMAGE					1
#define UPMODE_POINT					2

#define NAME_RATE						0.80f
#define SCORE_TOP_RATE_STAR				0.70f
#define SCORE_TOP_RATE					0.63f
#define TIP_RATE						0.58f
#define GAME_MENU_RATE					0.25f
#define SCORE_BOTTOM_RATE				0.055f
#define SCORE_BOTTOM_RATE_STAR			0.05f



#define LABEL_NAME						300000
#define LABEL_TIP						300001
#define LABEL_SCORE						300002
#define LABEL_STAR_INFO					300003
#define LABEL_COMBO						300004
#define LABEL_CHANGE_LIFE				300005
#define LABEL_RESULT					300006
#define LABEL_TWO_PLAYER_RESULT_P1		300007//2p
#define LABEL_TWO_PLAYER_RESULT_P2		300008//2p

//game_etc_tag
#define ETC1							310000
#define ETC2							320000
#define ETC3							330000
#define ETC4							340000
#define ETC5							350000
#define ETC6							360000


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//common
#define BG_TOAST					999000000
#define LABEL_TOAST					999000001

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Font_Size
#define CARD_LABEL_SIZE 25
#define CARD_LABEL_SIZE2 18
#define CARD_LABEL_SIZE3 22
#define NOTE_FONT_SIZE 22
#define PLAY_FONT_SIZE 40
#define TOAST_FONT_SIZE 25
#define COMBO_FONT_SIZE	45

#define AD_HEIGHT_SIZE	100
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//shop
#define SHOP_SIDE_EXIT			8000000
#define SHOP_POINT				8000001
#define SHOP_SIDE_MOVIE			8000002

#define SHOP_UP_LIFE_MAX		100
#define SHOP_UP_RECOVERY_MAX	100
#define SHOP_UP_DEFENCE_MAX		100
#define SHOP_UP_POINT_MAX		100

#define UP_PER_LIFE				1
#define UP_PER_RECOVERY			0.005f
#define UP_PER_DEFENCE			0.005f
#define UP_PER_POINT			0.005f
////////////////////////////////////////////////////////////////////////////////
//sidemenu
#define SIDE_SCALE 0.6f

#define SIDE_HOME				1000
#define SIDE_LEADERBOARD		1001
#define SIDE_ACHIEVEMENT		1002//
#define SIDE_AUDIO				1003
#define SIDE_MUSIC				1004
#define SIDE_CREDIT				1005//
#define SIDE_LANG				1006
#define SIDE_SHARE				1007//
#define SIDE_FACEBOOK			1008
#define SIDE_EXIT				1009
#define SIDE_INFO				1010
#define SIDE_LEFT				1011
#define SIDE_RIGHT				1012
#define LABEL_PAGE					1013//
#define SIDE_NOTE				1014
#define SIDE_MENU_UP			1015
#define SIDE_MENU_DOWN			1016
#define SIDE_MORE				1017
#define SIDE_EXTREME			1018



//MENU
#define MENU_SIDE_UP_HEIGHT			0.95f
#define MENU_SIDE_DOWN_HEIGHT		0.05f
#define MENU_HEIGHT_RATE			0.53f
#define MENU_MENU			10000000
#define MENU_CARD_LABEL		11000000
#define MENU_CARD_LABEL2	11100000
#define MENU_CARD_LABEL3	11200000
#define MENU_CARD_STAR		12000000
#define MENU_GAME1			10100001
#define MENU_GAME2			10100002
#define MENU_GAME3			10100003
#define MENU_GAME4			10100004

#define MENU_TWO_PLAYER		18000000
#define MENU_TWO_PLAYER_1	18100001
#define MENU_TWO_PLAYER_2	18100002
#define MENU_TWO_PLAYER_3	18100003
#define MENU_TWO_PLAYER_4	18100004

#define MENU_LEFT			13000000
#define MENU_RIGHT			14000000
#define MENU_PAGE1			15000000
#define MENU_PAGE2			16000000
#define NOTE				17000000
#define NOTE_LABEL			17100000
#define NOTE_NAME			17200000

#define NOTE_LABEL_COUNT	8
#define MAX_STAR_PHASE	8
static const char* strGameName[GAME_COUNT] = {
	"1. Circle Pop"
	,"2. Unify Type"
	,"3. Jump Control"
	,"4. Reactive Push"

	,"5. Fast Pop"
	,"6. Memorize Colors"
	,"7. Bounce Ball"
	,"8. Break Bricks"

	,"9. Eraser"
	,"10. High Count"
	,"11. Magnet Dribble"
	,"12. Drag Control"

	,"13. Clay Shooting"
	,"14. Mental Calc. OX"
	,"15. Driving"
	,"16. Blocking Control"



	//,"17. Random Fire"

	//  ,"96. Go Blank"
	//	,"98. Timing Number"
	//	,"910. Make Way"
	//,"914. Rotate & Slide"
};//GAME_COUNT_FIX

static const char* strGameName_ko[GAME_COUNT] = {
	//데미지는 시간 + 점수
	//난이도는 시간으로 증가해야함. 퍼즐류는 필요없음
	"1. 동그라미 터뜨리기" // diffi 증가필요
	,"2. 형태 맞추기" //
	,"3. 점프 컨트롤"  //  diffi 증가필요
	,"4. 순발력 누르기" //
	
	,"5. 빠르게 터뜨리기" // diffi 증가필요
	,"6. 색깔 기억하기" //
	,"7. 바운스 볼" // diffi 증가필요
	,"8. 벽돌 깨기"  //  diffi 증가필요

	,"9. 지우개" // diffi 증가필요
	,"10. 최고 개수"
	,"11. 자석 드리블" //  diffi 증가필요
	,"12. 드래그 컨트롤" // diffi 증가필요

	,"13. 클레이 사격"// diffi 증가필요
	,"14. 암산 OX"
	,"15. 운전" // diffi 증가필요
	,"16. 막기 컨트롤" // diffi 증가필요



	//,"17. 난사"

	//  ,"96. 빈 길 지나기" // diffi 증가필요
	//	,"98. 숫자 타이밍"
	// 	,"910. 길 만들기" //
	//	,"914. 회전 & 슬라이드"//
};//GAME_COUNT_FIX


static std::string getGameName(int nGameNumber, bool bEnglish, bool bRemoveNumber)
{
	if (nGameNumber >= GAME_COUNT)
		return bEnglish ? "Preparing" : UTF8PCHAR("준비중");

	std::string strTemp = bEnglish ? strGameName[nGameNumber] : UTF8PCHAR(strGameName_ko[nGameNumber]);

	if (bRemoveNumber)
	{
		int n = strTemp.find(' ');
		strTemp = strTemp.substr(n + 1, strTemp.size());
	}


	return strTemp;
}

static std::string getGameScore(int nGameNumber, bool bEnglish, bool bText)
{
	if (nGameNumber >= GAME_COUNT)
		return bEnglish ? "" : UTF8PCHAR("");

	std::string strTemp;


	if (bText == false)
		strTemp.append("%d");
	else
		strTemp.append(bEnglish ? "Best Score : %d" : UTF8PCHAR("최고 점수 : %d"));


	return strTemp;
}

#endif //__DEFINE_H__