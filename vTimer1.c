/*************************************************
*  時間計測用Timer1ライブラリファイル
*    vLcd_cmd(cmd)  ----- コマンド出力
*    vLcd_data(data) ----- １文字表示出力
*    cDataTrans(data) ----- I2Cデータ出力共通関数
*************************************************/
#define TIMER1_LIB

#include <xc.h>         // XC8 General Include File
#include "vTimer1.h"

/*****************************
*  initialization
*****************************/
void vTimer1_init(void)
{
	cFlag10mSec = 0;
	sSecc = 0;
	cFlagSec = 0;

	T1CON	= 0x34;	// Prescaler 1/8 & Fosc/4 Internal Clock
	TMR1H	= 0xFF;
	TMR1L	= 0xF0;
	/* 割り込み許可 */
	TMR1IF	= 0;
	TMR1IE	= 1;
	TMR1ON	= 1;
}

/*******************************
*  タイマ0/1割り込み処理
*******************************/
/*
void isr01(void)
{
	// タイマ1割り込み処理  10msc毎
	if(TMR1IF){			// タイマ1の割り込みか

		TMR1H = 0xC5;				//10msec設定
		TMR1L = 0x68;				//10msec設定

		TMR1IF = 0;		// 割り込みフラグクリア
		cFlag10mSec = 1;

		// 時間計測用1/10秒をカウント
		sSecc++;
		if(sSecc == 100 ){			// 10msec割り込み発生数は100回で１秒とする。
			sSecc = 0;
			cFlagSec++;
		}
	}
}
*/

/*******************************
*  時間変数のクリア
*******************************/
void vClock01_Clear()
{
	sSecc = 0;
	cFlagSec = 0;
	ucSec01= ucMin01= ucHour01=0;
}
/*******************************
*  時間カウント処理
*******************************/
void vClock01()
{

	if(cFlagSec) {					// 1秒が立っていたら
		ucSec01 += cFlagSec;		// １秒加算
		cFlagSec = 0;
		if(ucSec01 > 59){			// 1分経過(test=5sec)
			ucSec01 = 0;
			ucMin01++;				// 1分加算
			if(ucMin01 > 59){
				ucMin01 = 0;
				ucHour01++;			// 1時間加算
				if(ucHour01 > 23){
					ucHour01 = 0;
				}
			}

//			cMinCountDown --;
//			if(cMinCountDown < 0) {
//				cMinCountDown  = 59;
//			}
		}

/*
		static unsigned char ucTest01;
		// 1秒周期の処理。テスト用にledの点滅
		if(ucTest01==0){
			ucTest01 = 1;
		}else{
			ucTest01 = 0;
		}

		if(ucTest01 == 0){			// SW1オンか？
			GPIO0 = 0;				// 出力low
		}else{
			GPIO0 = 1;				// 出力High
		}
*/

	}
}

