/*************************************************
*  pwm出力による buzzer制御ヘッダファイル
*    vBuzzer_init()  ----- PWM初期化
*    vStopBuzzer() ----- PWM shatdown
*    vRestartBuzzer() ----- PWM shatdown clear
*    vBeep() ----- Beep the buzzer
*************************************************/

/*****************************
** 定数変数定義
*****************************/
#if defined PWM_BUZZER_LIB
	const unsigned char ucOnkai[16] = {0,100,94,89,84,79,75,71,67,63,59,56,53} ; //波長比
	const unsigned char ucTenkai[6] = {1, 2, 1, 2, 1, 2} ;	    //　オクターブを算出する係数、1倍・2倍・4倍・8倍の周波数
	const unsigned char ucPrescal[6] = {0b11111110,0b11111110,0b11111101,0b11111101,0b11111100,0b11111100} ;	//　オクターブを算出する係数、TIMER2のPrescaler値
	const unsigned char ucNagasa[12] = {48,32,24,16,12,8,6,4,2,2,1,1} ;	// 拍子・音符の長さ。全分、・・・64分音符、
	unsigned short usGakufuIchi = 0;
	short sNagasa01 = 0;
	char cBuzzerState = 0;

    //----------------------------------------------------------------------------------------------
    //音楽データ　うさぎのダンス
    const unsigned short usOngaku01[128]={
38,681,1062,1321,1062,809,678,425,166,425,678,1065,1318,681,1061,38,681,1062,681,
1062,1321,182,441,694,185,438,185,1574,1321,1061,38,1065,694,697,438,185,1574,1321,
1062,681,1062,681,1062,1321,1061,38,681,1062,681,1062,1321,182,441,694,1065,1318,
1065,1318,441,181,37,37,37,37,37,
0xFFFF
} ;
    //音楽データ　うさぎのダンス　*テンポを11
    const unsigned char ucTempo01 = 6;

#else
	//extern unsigned char usOnkai[];
	//extern unsigned char ucTenkai[];
	//extern unsigned char ucNagasa[];
	extern unsigned short usGakufuIchi;
	extern short sNagasa01;
	extern char cBuzzerState;

#endif

/*****************************
*  プロトタイプ定義ファイル
*****************************/
void vBuzzer_init(void);
void vStopBuzzer(void);
void vRestartBuzzer(void);
void vBeep(void);
unsigned char ucBeep2(unsigned short usOnpu);
void vMelody01(void);

void vLEDBlink01(void);



