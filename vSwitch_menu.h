/*************************************************
*  Switch_Menu処理ヘッダファイル
*    vPushSwitch_init()  ----- 初期化
*    vSwitch1Check() ----- switch5 チェック＆repeat処理
*    vSwitch1UserHandling() ----- switch5が押された時のユーザ処理を記述する
*    char cSwitch1ON() ----- switch5の状態を返信する。フラグをクリアする。
*    vSwitch7Check() ----- switch7 チェック＆repeat処理
*    vSwitch7UserHandling() ----- switch5が押された時のユーザ処理を記述する
*************************************************/

//pushbuttons define
#define sw1				GPIO3
#define SWITCH_ON		1
#define SWITCH_OFF		0

#define LED3min			GPIO4	//Red
#define LED5min			GPIO5	//Yelow
#define LED10min		GPIO0	//Green

#define d3min			3	//wait time
#define d5min			5	//wait time
#define d10min			10	//wait time

/*****************************
** 定数変数定義
*****************************/
#if defined SWITCH_MENU_LIB
	char cSw1_flag = 0;		// global変数はstaticは、要らない
	char cSw2_flag = 0;		// global変数はstaticは、要らない

	//モード・遷移設定、
	enum eModeS1 {
		modeS1_Timer,
			modeS1_Pre_CountDown,
			modeS1_CountDown,
			modeS1_Pre_Melody,
			modeS1_Melody,
			modeS1_Wait,
			modeS1_Go_Timer_Start,

		modeS1_TimeSet,
			modeS1_TimeSet3min,
			modeS1_TimeSet5min,
			modeS1_TimeSet10min,
			modeS1_Post_TimeSet,

		modeS1_Sleep,
		modeS1_end
		};
	enum eModeS1 eModeStatus1;


#else
	//extern char ucTestCount = 0;
#endif


/*****************************
*  プロトタイプ定義ファイル
*****************************/
void vPushSwitch_init(void);
void vSwitch1Check(void);
void vSwitch1UserHandling(void);
char cSwitch1ON(void);

void vSwitch2Check(void);
void vSwitch2UserHandling(void);
char cSwitch2ON(void);

void vLEDClear(void);

void vModeControl01(void);
void vModeStatusClr(void);


