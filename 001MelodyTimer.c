/************************************************
* MelodyTimer 3　
*  MPLAB X IDE v2.15
*  XC8 v1.32
*************************************************/
#include <xc.h>         // XC8 General Include File

#include "vTimer1.h"
#include "vBuzzer_pwm.h"
#include "vSwitch_menu.h"

/***** CONFIG SETTINGS ********/
#pragma config  FOSC = INTOSCIO, WDTE = OFF, PWRTE = OFF, MCLRE = OFF, CP = OFF, BOREN = OFF, IESO = OFF, FCMEN = OFF, CPD = OFF


/*** グローバル変数の定義  ****/
//#define	_XTAL_FREQ	2000000

void interrupt ISR(void)
{
	// タイマ1割り込み処理  10msc毎
	if(TMR1IF){			// タイマ1の割り込みか

		TMR1H = 0xFD;				//10msec設定
		TMR1L = 0x8E;				//10msec設定

		TMR1IF = 0;		// 割り込みフラグクリア
		cFlag10mSec = 1;

		// 時間計測用1/100秒をカウント
		sSecc++;
		if(sSecc == 100 ){			// 10msec割り込み発生数は100回で１秒とする。
			sSecc = 0;
			cFlagSec++;
		}
	}

	if(GPIF){			// IOの割り込みか

		GPIF = 0;		// 割り込みフラグクリア
	}
}


/************ メイン関数  ***************/
void main(void)
{
//	OSCCON = 0x70;				// クロック8MHzに設定
	OSCCON = 0x50;				// クロック2MHzに設定
	ANSEL  = 0x00;				// すべてデジタルに設定
	CMCON0 = 0x07;				// コンパレータ無効化
	OPTION_REGbits.nGPPU   = 1;             // プルアップ無効化
	WPU    = 0x0;				// プルアップビット指定
	TRISIO = 0x08;				// GP3入力、他は出力設定
	GPIO   = 0x00;				// 全てlow出力設定
	// 初期値セット

	// 割込み許可
	PEIE	= 1;	// モジュール全体の割込み許可
	GPIE	= 1;	// io_pin全体の割込み許可
	GIE		= 1;	// 全体の割込み許可

	// モジュール初期化
	vTimer1_init();
	vBuzzer_init();
	vStopBuzzer();
	vModeStatusClr();

	/********* メインループ  **********/
	while(1)
	{
		vClock01();

		// ** 10msec周期処理 **
		if(cFlag10mSec == 1){		// 10msecのチェック処理
			cFlag10mSec = 0;		// 10msecのチェック処理フラグクリア。

			// ** スイッチ入力処理 **
			vSwitch1Check();
			// ** モードコントロール処理 **
			vModeControl01();

		}
	}
}
