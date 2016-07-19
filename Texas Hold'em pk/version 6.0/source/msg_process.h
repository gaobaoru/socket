
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

/*跟新玩家摊牌次数*/
void Upd_Plays_Show_Num(void);


/*保存跟注牌力最小20个数;若g_opp_act_info[k].id == showdown[k].id，
则idx = k;data参与排序的数;stage四个阶段;action_code 动作代码*/
static void Save_Min_Data20(unsigned int idx,unsigned int stage, float data, int action_code); 

/*保存跟注牌力最大20个数;若g_opp_act_info[k].id == showdown[k].id，
则idx = k;data参与排序的数;stage四个阶段;action_code 动作代码*/
static void Save_Max_Data20(unsigned int idx,unsigned int stage, float data, int action_code); 

/*计算对手跟注牌力，仅被Showdown_Process调用，若g_opp_act_info[k].id == showdown[j].id，则idx = k,show_idx = j*/
static void Showdown_Cal_Rai_All_Che(unsigned int idx, unsigned int show_idx); 

/*摊牌阶段处理函数*/
static void Showdown_Process(void); 

/*
更新g_opp_act_info数组的数据,在Updata_Opponent_Noti_ActionNum
和Updata_Opponent_Inqu_ActionNum中被调用
输入： Id_idx玩家ID的索引 
	   action_code动作代码 
*/
static void Updata_Players_Actions(int Id_idx,int action_code); 

/*inquire_action清零*/
static void ClearInquireAction(void);

/*notify_action清零*/
static void ClearNotifyAction(void);

/*第一局时记录玩家ID，仅被Process中调用*/
static void Fill_Players_ID(void); 

/*更新玩家赌注，在询问消息命令尾部被调用*/
void Updata_Players_Bet(void);

/*当自己选择加注时，更新自己的赌注，在发出动作时被调用,idx加注额的索引*/
void Updata_Own_Bet(int idx);

/*更新对手动作数，在通知消息命令尾部被调用*/
void Updata_Opponent_Noti_ActionNum(void);

/*更新对手动作数，在询问消息命令尾部被调用*/
void Updata_Opponent_Inqu_ActionNum(void);

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

/*更新各个阶段的对手数，在询问消息和通知消息中调用,X_T = 'X'询问消息中调用，X_T = 'T'通知消息中调用*/
static void UpdataOpponentNum(uint8_t X_T);

/*每局结束数据清零*/
static void ClearData(void); 

/*缓冲数据处理函数*/
extern int process(int length, const char* buffer);


#endif // MSG_PROCESS_H_INCLUDED
