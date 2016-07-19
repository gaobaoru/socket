/******************************************************************************
  文 件 名   : Message.h
  版 本 号   : 初稿
  作    者   : GBR 
  生成日期   : 2015年5月19日
  最近修改   :
  功能描述   : 数据结构定义
******************************************************************************/
#include "Action.h"

#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

/*相关变量定义*/

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

/*询问行动过玩家信息*/
struct inquire_msg
{
	int id;  
	int jetton;//手中筹码 
	int money; //剩余金币数 
	int bet;  //本手牌累计投注额
	int action; 
}inquire_action[8];

/*询问当前底注信息*/
int inquire_totalpot;

/*通知消息中行动过玩家信息*/
struct notify_msg
{
	int id;  
	int jetton;//手中筹码 
	int money; //剩余金币数 
	int bet;  //本手牌累计投注额
	int action; 
}notify_action[8];

/*通知当前底注信息*/
int notify_totalpot;

/*摊牌公共消息*/ 
struct showdown_rank 
{
	int rank;
	int id;
//	int card1_color;
//	int card1_point;
//	int card2_color;
//	int card2_point;
	int nut_hand;
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

#endif // MSG_PROCESS_H_INCLUDED



