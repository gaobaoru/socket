
 /******************************************************************************
  文 件 名   : msg_process.h
  版 本 号   : 初稿
  作    者   : GBR 
  生成日期   : 2015年5月19日
  最近修改   :
  功能描述   : 数据结构定义
******************************************************************************/
#ifndef MSG_PROCESS_H_INCLUDED
#define MSG_PROCESS_H_INCLUDED

/*将float型转成字符串*/ 
extern void FloatToStr(float data , char *pchar_for , char *pchar_back);

/*第一局时记录玩家ID，仅被Process中调用*/
static void Fill_Players_ID(void); 

/*更新玩家赌注，在询问消息命令尾部被调用*/
void Updata_Players_Bet(void);

/*计算每轮下注对手的最大下注数（g_Money_Max），在询问消息命令尾部Updata_Players_Bet之后被调用*/
void CountBetMax(void);





/*普通座次信息函数*/ 
extern void seat_msg(char* str,int num);
/*庄家座次信息函数*/ 
extern void seat_button_msg(char* str);
/*盲注座次信息函数*/ 
extern void seat_blind_msg(char* str,int num);

/*盲注信息函数*/ 
extern void blind_msg(char* str,int num);

/*底牌信息函数*/
extern void hd_cards_msg(char* str,int num);
/*公共牌信息函数*/
extern void pb_cards_msg(char* str,int num);

/*询问消息行动过的玩家信息函数*/
extern void inquire_action_msg(char* str,int num);
/*询问消息中当前底池消息函数*/
extern void inquire_totalpot_msg(char* str);

/*通知消息行动过的玩家信息函数*/
extern void notify_action_msg(char* str,int num);
/*通知消息中当前底池消息函数*/
extern void notify_totalpot_msg(char* str);

/*摊牌消息函数*/
extern void showdown_msg(char* str,int num);

/*彩池分配消息函数*/
extern void potwin_dis_msg(char* str,int num); 

/*动作函数*/
extern uint8_t* ActionForMsg(void); 

/*计算每局开始时的对手数，在process中调用*/
static void CountOpponentNum(void); 

/*更新各个阶段的对手数，在process中调用*/
static void UpdataOpponentNum(void); 

/*每局结束数据清零*/
static void ClearData(void); 

/*缓冲数据处理函数*/
extern int process(int length, const char* buffer);


#endif // MSG_PROCESS_H_INCLUDED
