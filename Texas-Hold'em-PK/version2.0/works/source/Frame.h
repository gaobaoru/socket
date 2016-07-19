/******************************************************************************

                  版权所有 (C), 2015-2015, OUC

 ******************************************************************************
  文 件 名   : Frame.h
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 2015年5月14日
  最近修改   :
  功能描述   : 数据结构定义
******************************************************************************/

//总的牌力数
#define CARD_POWER_NUM 7462

/* 函数返回状态 */
typedef enum
{
	ERR = 0,//失败
	OK = 1,//成功
}ReturnStatus;


/* 牌型 */
typedef enum
{
	ROYAL_FLUSH = 1,//皇家同花顺
	STRAIGHT_FLUSH = 2,//同花顺
	FOUR = 3,//四条
	FULL_HOUSE = 4,//葫芦（三带一对）
	FLUSH = 5,//同花
	STRAIGHT = 6,//顺子
	THREE = 7,//三条
	TWO_PAIRS = 8,//两对
	ONE_PAIR = 9,//一对
	HIGHT_CARD = 10,//高牌
	OTHER = 0//作函数返回值时用
}CardKinds;

/* 花色 */
typedef enum
{
	Spade = 1,//黑桃
	Heart = 2,//红桃
	Diamond = 3,//方片
	Club = 4//草花
}CardColor;

/* 牌值 */
typedef enum
{
	Card_Null = 0,//用于初始化
	Card2 = 2,//2
	Card3 = 3,//3
	Card4 = 4,//4
	Card5 = 5,//5
	Card6 = 6,//6
	Card7 = 7,//7
	Card8 = 8,//8
	Card9 = 9,//9
	CardT = 10,//T
	CardJ = 11,//J
	CardQ = 12,//Q
	CardK = 13,//K
	CardA = 14,//A
}CardValue;

/* 牌值对应的质数 */
typedef enum
{
	Card2_prime = 2,
	Card3_prime = 3,
	Card4_prime = 5,
	Card5_prime = 7,
	Card6_prime = 11,
	Card7_prime = 13,
	Card8_prime = 17,
	Card9_prime = 19,
	CardT_prime = 23,
	CardJ_prime = 29,
	CardQ_prime = 31,
	CardK_prime = 37,
	CardA_prime = 41,
}Card_Prime;

/* 单张扑克信息 */
typedef struct
{
	Card_Prime prime;//牌值对应的质数(2~A)~(2~41)
	CardColor color;//花色（S:黑桃，H:红桃，D:方片，C:草花）
	CardValue card_val;//牌值(2~A)~(2~14)
}CardInfos;

/*手牌信息*/
typedef struct
{
	CardInfos card1;
	CardInfos card2;
}Hand_Cards;

/* 游戏阶段 */
typedef enum
{
	GameStage_Null = 0,//用于初始化 
	Preflop = 1,//盲注 
	Flop = 2,//翻牌
	Turn = 3,//转牌
	River = 4,//河牌
	Showdown = 5//摊牌
}GameStage;

/* 操作状态 */
typedef enum
{
	Action_Null = 0,//用于初始化 
	Fold = 1,//弃牌
	Call = 2,//跟牌
	Raise = 3,//加注
	All_In = 4,//全压
	Check = 5//看牌
}ActionStatue; 

/*对手赌注信息（包括当前和上一次的下注总额）*/
typedef struct
{
	int id;//对手ID 
	int bet_last;//上一次总赌注 
	int bet_now;//当前总赌注 
}Bet_Infos;


/*对手跟牌信息*/
typedef struct
{
	/*各游戏阶段(翻牌前、翻牌、转牌、河牌)跟注次数，按GameStage的顺序*/
	int stage_call_num[4];
	/*上一局为止，各游戏阶段跟注次数，用于判断对手是否开牌，该字段在摊牌阶段被更新 ,
	在摊牌时如果有对手ID，则last_stage_call_num[i] = stage_call_num[i],
	在摊牌时如果没有对手ID，则stage_call_num[i] = last_stage_call_num[i]*/ 
	int last_stage_call_num[4];
	/*各游戏阶段，对手选择跟牌，其对应的平均牌力（翻牌前牌力分为1~9级，其它为0~1）
	在摊牌时如果有对手ID，则:
	call_power_aver[i] = (now_call_power[i] * last_stage_call_num[i] + now_call_power[i])/stage_call_num[i],
	在摊牌时如果没有对手ID，则不变*/ 
	float call_power_aver[4];
	/*各游戏阶段，本局跟牌牌力*/
	float now_call_power[4];
}Call_Infos;
/*对手加注信息*/
typedef struct
{
	int stage_raise_num[4];
	int last_stage_raise_num[4];
	float raise_power_aver[4];
	float now_raise_power[4];
}Raise_Infos;
/*对手全压信息*/
typedef struct
{
	int stage_allin_num[4];
	int last_stage_allin_num[4];
	float allin_power_aver[4];
	float now_allin_power[4];
}AllIn_Infos;
/*对手看牌信息*/
typedef struct
{
	int stage_check_num[4];
	int last_stage_check_num[4];
	float check_power_aver[4];
	float now_check_power[4];
}Check_Infos;
/*对手历史动作信息*/
typedef struct
{
	int all_live_num;//参与的总牌局数
	int id;//对手ID
	int fold_num[4];//各游戏阶段(翻牌前、翻牌、转牌、河牌)弃牌次数
	Call_Infos call;//跟牌信息
	Raise_Infos raise;//加注信息
	AllIn_Infos all_in;//全压信息
	Check_Infos check;//看牌信息
}Opponent_Action_Infos;
