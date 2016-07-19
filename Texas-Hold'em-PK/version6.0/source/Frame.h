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
	Check = 5,//看牌
	Blind = 6 //盲注 
}ActionStatue; 

/*对手赌注信息（包括当前和上一次的下注总额）*/
typedef struct
{
	int id;//当前最大下注额的对手ID 
	int bet_last;//自己已下注额 
	int bet_now;//当前最大下注额 
}Bet_Infos;





/*对手跟牌信息*/
typedef struct
{
	/*各游戏阶段(翻牌前、翻牌、转牌、河牌)跟注标志，按GameStage的顺序,每局开始被清零*/
	int now_num[4];
	/*各游戏阶段(翻牌前、翻牌、转牌、河牌)跟注总次数，按GameStage的顺序，
	call_all_num[i]最大值为600，在摊牌时被跟新*/
	int all_num[4];
	/*各游戏阶段，对手选择跟牌，其对应的平均牌力（翻牌前牌力分为1~9级，其它为0~1）在摊牌时如果有对手ID，
	则:	power_aver[i] = (power_aver[i] * all_num[i] + 本局牌力)/（all_num[i] + 1）,
	在摊牌时如果没有对手ID，则不变*/ 
	float power_aver[4];
	/*各阶段跟注的最小20局牌力*/
	float power_min20[4][20];
	/*各阶段跟注的最大20局牌力*/ 
	float power_max20[4][20];
}History_Action;

/*对手各阶段历史动作信息*/
typedef struct
{
	int id;//对手ID
	int showdown_num;//摊牌次数 
	int fold_num[4];//各游戏阶段(翻牌前、翻牌、转牌、河牌)弃牌次数
	History_Action call;//跟牌信息
	History_Action raise;//加注信息
	History_Action all_in;//全压信息
	History_Action check;//看牌信息
}Opponent_Action_Infos;

/*玩家弃牌标志*/ 
typedef struct
{
	int id;//对手ID
	uint8_t fold_flg;//弃牌标志 
}Opponent_Fold_Flg;


/*对手动作记录，建模准备*/
struct OpponentActionRecord
{
	int id;
	int opponent_fold;
	int opponent_call;
	int opponent_raise;
	int opponent_all_in;
	int opponent_check;
}opponent_action[8];



///////////////////////////////////////////////////////////////////////////////////
/*座次信息*/
struct seat_info_msg
{
    int id;
    int jetton;
    int money;
}seat[5],seat_button,seat_blind[2];

/*盲注信息*/
struct blind_info_msg
{
    int id;
    int bet;
}blind_infos[2];

/*询问行动过玩家信息,通知消息中行动过玩家信息*/
struct inquire_msg
{
	int id;  
	int jetton;//手中筹码 
	int money; //剩余金币数 
	int bet;  //本手牌累计投注额
	int action; 
}inquire_action[8],notify_action[8];


/*摊牌公共消息*/ 
struct showdown_rank 
{
	int rank; 
	int id;
	int nut_hand;//牌型 
	Hand_Cards hand_cards;
}showdown[8];

/*彩池分配消息*/
struct pot_win_msg
{
	int id;
	int num;
}pot_win[8]; 


//程序运行状态，必为以下十一种之一 
typedef enum
{
	DEF = 0, 
	SEAT = 1,
 	BLIND = 2, 
 	HOLD = 3, 
 	INQUIRE = 4,
 	FLOP = 5,
 	TURN = 6, 
 	RIVER = 7,
 	SHOWDOWN = 8, 
 	COMMON = 9, 
 	POTWIN = 10,
	NOTIFY = 11 
}STATE;