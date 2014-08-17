/*************************************************
*  pwm出力による buzzer制御ライブラリファイル
*    vBuzzer_init()  ----- PWM初期化
*    vStopBuzzer() ----- PWM shatdown
*    vRestartBuzzer() ----- PWM shatdown clear
*    vBeep() ----- Beep the buzzer
*************************************************/
#define PWM_BUZZER_LIB

#include <xc.h>         // XC8 General Include File
#include "vTimer1.h"
#include "vBuzzer_pwm.h"
#include "vSwitch_menu.h"


/*****************************
*  initialization
*****************************/
void vBuzzer_init(void)
{
	// TMR2
	T2CON 	= 0x05;			//TMR2_ON & prescaler:1/4 postscaler:1/1
	PR2		= 0x8C;			//period(1Khz)
	TMR2IE 	= 0;			//0 = Disables the TMR2 to PR2 match interrupt

	// CCP
	CCP1CON	= 0x0C;			//PWM mode; PWM mode active-high
	CCPR1L	= 0x8C / 2;		//rate 50%
	CCPR1H	= 0x00;
	CCP1IE	= 0;			//0 = Disables the CCP1 interrupt

	TRISIO1	= 0;			//GP1 is output

}

/*******************************
*  stop buzzer
*******************************/
void vStopBuzzer(void)
{
	CCP1CON	= 0;			// CCP off
	TMR2ON	= 0;			// TMR2 is off
	GPIObits.GP2 = 0;			// out put low
}

/*******************************
*  restart buzzer
*******************************/
void vRestartBuzzer(void)
{
	vBuzzer_init();
}

/*******************************
*  vBeep
*  call this routine by 10msec interval
*******************************/
void vBeep(void)
{

	if(cBuzzerState == 0){		//until 500msec
		vRestartBuzzer();		//buzzer on
		cBuzzerState++;			//timer count up

	}else if(cBuzzerState < 10){	//until 500msec

		cBuzzerState++;			//timer count up

	}else if(cBuzzerState < 100){	//buzzer off 500msec
		vStopBuzzer();
		cBuzzerState++;

	}else{
		cBuzzerState = 0;
	}
}


/*******************************
*  vBeep2
*******************************/
	unsigned char ucKaiten, ucOnkaiNum;
//	unsigned char test1, test2, test3;
unsigned char ucBeep2(unsigned short usOnpu)
{

	ucKaiten = (usOnpu >> 4) & 0b00000111;
	ucOnkaiNum = (usOnpu >> 7) & 0b00001111;

	if(ucOnkaiNum == 0){
		vStopBuzzer();			//PWM_OFF
	}else{
//		test1 = ucOnkai[ucOnkaiNum];
//		test2 = ucTenkai[ucKaiten] ;
//		test3 = (239 * test1 /test2) /100 - 1;
		PR2 =   (239 * ucOnkai[ucOnkaiNum])/ucTenkai[ucKaiten]/100 - 1;
		CCPR1L	= PR2 / 2;		//rate 50%

		// CCP
		CCP1CON	= 0x0C;			//PWM mode; PWM mode active-high
		CCPR1H	= 0x00;
		// TMR2
		//T2CON 	= 0x07;			//TMR2_ON & prescaler:1/4 postscaler:1/1
		T2CON 	= 0x07 & ucPrescal[ucKaiten];		//TMR2_ON & prescaler
	}
	vLEDBlink01();
	//***曲のテンポをここで決める。×係数(ucTempo01)が少ないとテンポが速くなる***
	return (ucNagasa[usOnpu & 0b00001111]* ucTempo01 ); //音の長さをリターン。10msecを何回まつかをリターン
}

/*******************************
*  vMelody01
*  楽譜データを読み込んで、
*  音を設定し、
*  指定の時間音を出す、
*  10msed毎に実行される処理、
*******************************/
void vMelody01(void)
{

	if(sNagasa01 <= 0){
		if(usOngaku01[usGakufuIchi] == 0xFFFF) {	//255を曲終了のコードとする。
			usGakufuIchi = 0;
		}else{
			sNagasa01 = ucBeep2(usOngaku01[usGakufuIchi]); //音の長さをリターン
			usGakufuIchi++;
		}
	}else if( sNagasa01 < 3) {
		vStopBuzzer();			//PWM_OFF
	}
	sNagasa01--;				//音の長さ分待つ。
}

static char cWk1State = 0;
/*****************************
* LED Blink 01
* 曲に合わせてLEDを点滅させる。
*****************************/
void vLEDBlink01(void)
{
	LED3min = 0;			//出力low
	LED5min = 0;			//出力low
	LED10min = 0;			//出力low
	switch (cWk1State) {
	case  	1 :
		LED3min = 1;			//出力low
		break;

	case  	2 :
		LED5min = 1;			//出力low
		break;

	case  	3 :
		LED10min = 1;			//出力low
		break;

	case  	4 :
		LED5min = 1;			//出力low
		cWk1State = 0;
		break;

	default:
		cWk1State = 0;
	}
	cWk1State++;
}