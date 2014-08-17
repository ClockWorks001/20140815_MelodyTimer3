/*************************************************
*  Switch_Menu処理
*    vPushSwitch_init()  ----- 初期化
*    vSwitch1Check() ----- switch5 チェック＆repeat処理
*    vSwitch1UserHandling() ----- switch5が押された時の本体処理
*    vSwitch7Check() ----- switch7 チェック＆repeat処理
*    vSwitch7UserHandling() ----- switch7が押された時の本体処理
*************************************************/
#define SWITCH_MENU_LIB

#include <xc.h>         // XC8 General Include File
#include "vSwitch_menu.h"
#include "vTimer1.h"
#include "vBuzzer_pwm.h"


/*****************************
*  Initialize
*****************************/
void vPushSwitch_init(void)
{
//	ANSEL  = 0x00;				// すべてデジタルに設定
//	CMCON0 = 0x07;				// コンパレータ無効化
//	GPPU   = 1;					// プルアップ無効化
//	WPU    = 0x0;				// プルアップビット指定
//	TRISIO = 0x08;				// GP3入力、他は出力設定
//	GPIO   = 0x00;				// 全てlow出力設定
	TRISIO3 = 1;				// GP3入力、他は出力設定
}

/*****************************
* push Switch1 on/off check section
* this section is set at 10msec section.
*****************************/
static unsigned short usSw1pushTime = 0;
static unsigned char  ucSwOnState = 0;
void vSwitch1Check(void)
{
	if(!sw1){	//switch ON when an Input port is low.
		switch(usSw1pushTime)
		{
			case 0:
				ucSwOnState = 1;
				break;
			case 40:					//10msec*40 wait after the first push
				ucSwOnState = 2;
				vLEDClear();			//LED CLEAR
				break;
			case 70:					//Over Swich time
				vBuzzer_init();			//beep!
				break;
			default:
				break;
		}
		usSw1pushTime++;
	}else{
		switch(ucSwOnState)
		{
			case 1:
				//user handling here
				vSwitch1UserHandling();
				break;
			case 2:
				//user handling here
				vSwitch2UserHandling();
				break;
			default:
				break;
		}
		usSw1pushTime = 0;
		ucSwOnState = 0;
	}
}

/*****************************
* Switch1 User Handling
*****************************/
void vSwitch1UserHandling(void)
{
	cSw1_flag = SWITCH_ON;
}
/*****************************
* Switch1 test
*****************************/
char cSwitch1ON(void)
{
	if(cSw1_flag == SWITCH_ON){
		cSw1_flag = SWITCH_OFF;
		return 1;
	}
	return 0;
}

/*****************************
* Switch2 User Handling
*****************************/
void vSwitch2UserHandling(void)
{
	cSw2_flag = SWITCH_ON;
}
/*****************************
* Switch2 test
*****************************/
char cSwitch2ON(void)
{
	if(cSw2_flag == SWITCH_ON){
		cSw2_flag = SWITCH_OFF;
		return 1;
	}
	return 0;
}

/*****************************
* LED Clear
*****************************/
void vLEDClear(void)
{
	LED3min = 0;			//出力low
	LED5min = 0;			//出力low
	LED10min = 0;			//出力low
}

/*****************************
* eModeStatus1 clear
*****************************/
void vModeStatusClr(void)
{
	eModeStatus1 = modeS1_TimeSet;
}

static unsigned char cCountDownTime = d3min;
/*****************************
* Menu mode control
*****************************/
void vModeControl01(void)
{
	// mode change
	if (cSwitch2ON()) {
		if (eModeStatus1 < modeS1_TimeSet) {
			eModeStatus1 = modeS1_TimeSet;
		}else{
			eModeStatus1 = modeS1_Timer;
		}
	}

	// status add　to next
	if (cSwitch1ON()) {
		eModeStatus1++;
	}

	switch (eModeStatus1) {
	// ---------------------------------------------------
	case  	modeS1_Timer :
		vLEDClear();				//LED CLEAR
		vClock01_Clear();			//CLOCK 初期化

		eModeStatus1++;				//次へ進む
		break;

	case  	modeS1_Pre_CountDown :
		vClock01_Clear();				//CLOCK 初期化
		cMinCountDown = cCountDownTime;	//カウントダウンタイマ初期化
		vStopBuzzer();					//ブザーストップ
		eModeStatus1++;					//次へ進む
		break;

	case  	modeS1_CountDown :		//カウントダウン
		if (cMinCountDown <= 0) {	//Time Up
			eModeStatus1++;
		}

		if (sSecc < 6) {			//LED Blink
			if (cMinCountDown <= d3min) {
					LED3min = 1;
			}else if (cMinCountDown <= d5min) {
					LED5min = 1;
			}else{
					LED10min = 1;
			}
		}else{
			vLEDClear();			//LED CLEAR
		}
		break;

	case  	modeS1_Pre_Melody :
		vClock01_Clear();			//CLOCK 初期化
		usGakufuIchi = 0;			//楽譜位置初期化
		eModeStatus1++;
		break;

	case  	modeS1_Melody :			//音楽演奏
		vMelody01();

		if (ucMin01 > 10 && usGakufuIchi == 0) {			//音楽が10分以上経過し演奏が終わったら停止
			eModeStatus1 = modeS1_Sleep;
		}

		break;

	case  	modeS1_Wait :
		//Wait
//		vStopBuzzer();				//音楽停止
//		vLEDClear();				//LED CLEAR
		eModeStatus1 = modeS1_Sleep;
		break;

	case  	modeS1_Go_Timer_Start :
//		vStopBuzzer();					//音楽停止
		eModeStatus1 = modeS1_Timer;	//先頭処理へ戻る
		break;



	// ---------------------------------------------------
	case  	modeS1_TimeSet :
		vClock01_Clear();			//CLOCK 初期化
		vStopBuzzer();				//音楽停止
		switch (cCountDownTime) {
		case  	d3min :
			eModeStatus1 = modeS1_TimeSet3min ;
		break;
		case  	d5min :
			eModeStatus1 = modeS1_TimeSet5min ;
		break;
		case  	d10min :
			eModeStatus1 = modeS1_TimeSet10min ;
		break;
		default:
			eModeStatus1++;
		}
		break;
	case  	modeS1_TimeSet3min :
		cCountDownTime = d3min;		//カウントダウン時間を3分に設定
		vLEDClear();				//LED CLEAR
		LED3min = 1;				//出力low
		if (ucSec01 > 10) {			//未操作が10秒以上経過したら停止
			eModeStatus1 = modeS1_Sleep;
		}
		break;

	case  	modeS1_TimeSet5min :
		cCountDownTime = d5min;		//カウントダウン時間を5分に設定
		vLEDClear();				//LED CLEAR
		LED5min = 1;				//出力low
		if (ucSec01 > 10) {			//未操作が10秒以上経過したら停止
			eModeStatus1 = modeS1_Sleep;
		}
		break;

	case  	modeS1_TimeSet10min :
		cCountDownTime = d10min;	//カウントダウン時間を10分に設定
		vLEDClear();				//LED CLEAR
		LED10min = 1;				//出力low
		if (ucSec01 > 10) {			//未操作が10秒以上経過したら停止
			eModeStatus1 = modeS1_Sleep;
		}
		break;

	case  	modeS1_Post_TimeSet :
		eModeStatus1 = modeS1_TimeSet3min;	//3分設定へ戻る
		break;

	// ---------------------------------------------------
	case  	modeS1_Sleep :
		vStopBuzzer();				//音楽停止
		vLEDClear();				//LED CLEAR

		IOC3	= 1;				// GPIO3の割込み許可
		GPIF	= 0;				// io_pin全体の割込みフラグのクリア
		GPIE	= 1;				// io_pin全体の割込み許可
		GIE		= 1;				// 全体の割込み許可
		SLEEP();
		NOP();
		GPIF	= 0;				// io_pin全体の割込みフラグのクリア
		IOC3	= 0;				// GPIO3の割込み許可

	// wake up 時の戻り先
//		eModeStatus1 = modeS1_Timer;	//先頭処理へ戻る
		eModeStatus1 = modeS1_TimeSet;	//TimerSet処理へ戻る
		break;


	default:
		eModeStatus1 = modeS1_Timer;
	}

}



