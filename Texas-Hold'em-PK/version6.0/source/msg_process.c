 /******************************************************************************
  文 件 名   : card_decode.h
  版 本 号   : 初稿
  作    者   : GBR
  生成日期   : 2015年5月20日
  最近修改   :
  功能描述   : 数据结构定义
******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include<unistd.h>
#include<fcntl.h>
#include<netdb.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<arpa/inet.h>

#include "msg_process.h"
#include "card_decode.h"
#include "command_decode.h"
#include "Action.h"

//翻牌，转牌，河牌阶段，加注金额对应的牌力，与g_Riase_Str表对应 
#define  RAISE_STAGE1 0.95
#define  RAISE_STAGE2 0.90
#define  RAISE_STAGE3 0.85

//玩家数（包括自己） 
#define PLAYERS_NUM 8 

extern int skfd;//TCP 
extern int client_id;

extern int debug_fd; // 调试文件 
char g_Str[100];//发送文件缓冲区 

CardInfos Public_Cards[5];//公共牌
uint16_t AllOpponentNum = 1;//开局时总的对手数
Hand_Cards My_Hand_Card;//手牌 

/*
自己是否弃牌标志，1为未弃牌，0为弃牌 
在ActionForMsg和ClearData中被调用 
*/
uint16_t g_owm_fold_flg = 1;

Opponent_Fold_Flg g_players_fold_flg[PLAYERS_NUM];//玩家弃牌标志，标志在 ClearData 中复位

uint8_t* p_Action = NULL; //动作指针
 
Bet_Infos g_Oppon_Bet_Infos; //在CountOpponentNum中初始化 

uint16_t g_Inquire_num = 0;//每次询问统计已动作数目 
uint16_t g_Seat_num = 0;//每局统计座位数目 
/*
是否为首局标志，用在第一局填充 g_Oppon_Act_Infos数组的玩家ID ,
在Fill_Players_ID函数中置为2 
*/
uint16_t g_Is_First_Game = 1;

/*询问当前底注信息*/
int inquire_totalpot;

/*通知当前底注信息*/
int notify_totalpot;

uint8_t g_Action_Str[6][10] = {{"fold"},{"fold"},{"call"},{"raise 0"},{"all_in"},{"check"}};
uint8_t g_Riase_Str[3][20] = {{"raise 100"},{"raise 200"},{"raise 400"}};
uint16_t g_Raise_Val[] = {100,200,400};//加注金额，与g_Riase_Str对应 

STATE state = DEF;//状态 
GameStage g_e_game_stage = Preflop;//下注的4个阶段  

int id_test_idx = 0;//测试用 /////////////////////////////////////////////////////// 

int process(int length, const char* buffer)
{
	int next = 0, len = 0,cmd,num,i;
	char str[100]={0};
		
	while(next < length)
	{
		len = get_line(buffer + next , str);	// 返回从buffer中读取的长度 并不是str的长度。 
											//比如 换行符被读取但并没在str中 
		next += len;
		cmd = 0; 
		cmd = cmd_decode(str);
		if(cmd > 0)	//是命令
	 	{
			switch(cmd)
		 	{
				case 1:
//for(i=0; i<8; i++)
//{
//	if(showdown[i].id != 0)
//	{
//FloatToStr((float)showdown[i].id , "PlayerId:", NULL);
//FloatToStr((float)showdown[i].hand_cards.card1.card_val , "H_C1:", NULL);
//FloatToStr((float)showdown[i].hand_cards.card2.card_val , "H_C2:", NULL);
//	}
//}
					ClearData();
					num = 0; 
					state = SEAT;
					g_e_game_stage = Preflop;
					break;
				case 3:
					num = 0;
					state = BLIND;
					break; 
				case 5:
					num = 0;
					state = HOLD;
					break;  
				case 7:
					num = 0;
					state = INQUIRE;
					g_Inquire_num = 0;//每次询问之前将动作对手数清零 
					break;
				case 9:
					num = 0;
					state = FLOP;
					g_e_game_stage = Flop;
					break; 
				case 11:
					num = 0;
					state = TURN;
					g_e_game_stage = Turn;
					break;  
				case 13:
					num = 0;
					state = RIVER;
					g_e_game_stage = River;
					break; 
				case 15:
					num = 0;
					state = SHOWDOWN;
					break;  
				case 17:
					num = 0;
					state = COMMON;
					break;
				case 19:
					num = 0;
					state = POTWIN;
					break; 
				case 22:
					num = 0;
					state = NOTIFY;
					break; 
				case 18://  /COMMON  指令 退回SHOWDOWN状态
					num = 0; 
					state = SHOWDOWN;
					break;
				case 2:   //    /seat  
					CountOpponentNum();
					if(1 == g_Is_First_Game)
					{
						Fill_Players_ID();//第一局记录各玩家ID 
					}
					g_game_Num++; 
write(debug_fd,"\n\n\n",3);	
FloatToStr((float)g_game_Num , "Game_num:", NULL);
FloatToStr((float)g_now_jetton , "Game_jetton:", NULL);		
					state = DEF; 
					break;  //	/****    指令 退回DEF状态 
				case 16:
					Showdown_Process(); 
if(g_opp_act_info[id_test_idx].check.all_num[3] == 20)//id == 3333的 
{
	for(i=0; i<20; i++)
	{
FloatToStr((float)g_opp_act_info[id_test_idx].check.power_min20[3][i] , "Min:", NULL);
	}
	for(i=0; i<20; i++)
	{
FloatToStr((float)g_opp_act_info[id_test_idx].check.power_max20[3][i] , "Max:", NULL);
	}
FloatToStr((float)g_opp_act_info[id_test_idx].check.power_aver[3] , "AVER:", NULL);
}
					break;
				case 8:
					state = DEF; 
					break; 
				case 4:
				case 6:
				case 10:
				case 12:
				case 14:
				case 20:
				case 23:
					state = DEF; 
					break;  
				case 21:// gameover 指令  返回 
					return -1; 
				default:
					state = DEF; 
			} 
		}
		else				//不是命令   根据命令执行语句 
	 	{
		 	switch(state)
		 	{
	 			case SEAT:
	 				num++;
					if(num==1)
					{
						seat_button_msg(str);
					}
					if(num==2 || num==3)
					{
						seat_blind_msg(str,num-1);
					}
					if(num>=4 && num<=PLAYERS_NUM)
					{
						seat_msg(str,num-3);
					}
					g_Seat_num=num;
	 				break; 
				case BLIND:
					num++;
					blind_msg(str,num);
 					break;
				case HOLD:
					num++;
					hd_cards_msg(str,num);
	 				break; 
				case INQUIRE:
					num++;
					if(*str != 't')
					{
						inquire_action_msg(str,num);
						g_Inquire_num=num;		
					}
					else
					{
						inquire_totalpot_msg(str);
						
						Updata_Opponent_Inqu_ActionNum();
						
						UpdataOpponentNum('X');	//询问消息跟新对手数 
						
						if(1 == g_owm_fold_flg) 
						{	 
							Updata_Players_Bet();//计算最大下注额 
							
							p_Action = ActionForMsg();
							write(skfd,p_Action,strlen(p_Action));	
						} 
						
//FloatToStr((float)inquire_totalpot, "inquire_totalpot :", NULL);
					}
 					break; 
				case FLOP:
					num++;
					pb_cards_msg(str,num);
	 				break; 
				case TURN:
					pb_cards_msg(str,num+4);
 					break; 
				case RIVER:
					pb_cards_msg(str,num+5);
	 				break; 
				case SHOWDOWN:
					num++;
					showdown_msg(str,num);
 					break; 
				case COMMON:
	 				num++;
					pb_cards_msg(str,num);	
	 				break; 
				case POTWIN:
					num++;
					potwin_dis_msg(str,num);
 					break; 
				case NOTIFY:
					num++;
					if(*str != 't')
					{
						notify_action_msg(str,num);		
					}
					else
					{
						notify_totalpot_msg(str);				
//FloatToStr((float)notify_totalpot, "notify_totalpot :", NULL);
						Updata_Opponent_Noti_ActionNum();
						UpdataOpponentNum('T');	//通知消息跟新对手数 
//FloatToStr((float)updata_AllOpponentNum, "updata_AllOpponentNum :", NULL);
//						for(j=0;j<8;j++)
//						{
//FloatToStr((float)notify_action[j].action,"notify_action :", NULL);
//FloatToStr((float)opponent_noti_action[j].opponent_fold,"Notify_fold_ActionNum:", NULL);
//						}
//FloatToStr((float)opponent_noti_action[0].opponent_check,"Notify_check_ActionNum:", NULL);
//FloatToStr((float)opponent_noti_action[0].opponent_call,"Notify_call_ActionNum:", NULL);
//FloatToStr((float)opponent_noti_action[0].opponent_all_in,"Notify_all_in_ActionNum:", NULL);
//FloatToStr((float)opponent_noti_action[0].opponent_raise,"Notify_raise_ActionNum:", NULL);
					}
 					break; 
				case DEF:
					break;
 				default:
// 					def_process(str,strlen(str));
 					break; 
	 		}
	 	}
	} 
	return 0; 
}
 
/***********************************************
功能：普通座次信息处理函数 
输入：座次信息的一行信息 char* str和int num(第几位)  
输出：无
说明：seat_info_msg seat;结构中存入相应信息
*************************************************/
void seat_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char seat_id_num[10]={0,0,0,0,0,0,0,0,0,0};
	char seat_jetton_num[10]={0,0,0,0,0,0,0,0,0,0};
	char seat_money_num[10]={0,0,0,0,0,0,0,0,0,0};
	
	num--;
	//输入参数合法性检查 
	if(NULL == str || num < 0 || num > 4)
	{
		return;
	}

    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i) == 32 && *(str+i+1) != 32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                for(j=0; *(str+j)!=32; j++)
                {
                    *(seat_id_num+j)=*(str+j);
                }
                k=i+1;
                seat[num].id=atoi(seat_id_num);
            }
            if(Space_num==1)
            {
                for(j=0; *(str+j+k) != 32; j++)
                {
                    *(seat_jetton_num+j) = *(str+j+k);
                }
                k=i+1;
                seat[num].jetton=atoi(seat_jetton_num);
            }
            if(Space_num==2)
            {
                for(j=0; *(str+k+j)!=32; j++)
                {
                    *(seat_money_num+j)=*(str+j+k);
                }
                k=i+1;
                seat[num].money=atoi(seat_money_num);
            }
        }
    }
    if(seat[num].id == client_id)	//判断是否是自己ID，更新自己当前筹码
	{
		g_now_jetton= seat[num].jetton; 
	}
}
/***********************************************
功能：将庄家的座次信息分别对应存入庄家的结构体中
输入：座次信息的一行信息 char* str
输出：无
说明：seat_info_msg seat_button;结构中存入相应信息
*************************************************/
void seat_button_msg(char* str)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char seatButton_id_num[10]={0,0,0,0,0,0,0,0,0,0};
	char seatButton_jetton_num[10]={0,0,0,0,0,0,0,0,0,0};
	char seatButton_money_num[10]={0,0,0,0,0,0,0,0,0,0};
	
	//输入参数合法性检查 
	if(NULL == str)
	{
		return;
	}

    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i) == 32 && *(str+i+1) != 32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                k=i+1;
            }
            if(Space_num==1)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(seatButton_id_num+j)=*(str+j+k);
                }
                k=i+1;
                seat_button.id=atoi(seatButton_id_num);
            }
            if(Space_num==2)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(seatButton_jetton_num+j)=*(str+j+k);
                }
                k=i+1;
                seat_button.jetton=atoi(seatButton_jetton_num);
            }
            if(Space_num==3)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(seatButton_money_num+j)=*(str+j+k);
                }
                k=i+1;
                seat_button.money=atoi(seatButton_money_num);
            }
        }
    }
    if(seat_button.id == client_id)	//判断是否是自己ID，更新自己当前筹码
	{
		g_now_jetton= seat_button.jetton; 
	}
}

/***********************************************
功能：将盲注的座次信息分别对应存入庄家的结构体中
输入：座次信息的一行信息 char* str
输出：无
说明：seat_info_msg seat_blind;结构中存入相应信息
*************************************************/
void seat_blind_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char seatBlind_id_num[10]={0,0,0,0,0,0,0,0,0,0};
	char seatBlind_jetton_num[10]={0,0,0,0,0,0,0,0,0,0};
	char seatBlind_money_num[10]={0,0,0,0,0,0,0,0,0,0};

	num--;
	//输入参数合法性检查 
	if(NULL == str || num < 0 || num > 1)
	{
		return;
	}
	
    for(i = 0; i<strlen(str); i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                k=i+1;
            }
            if(Space_num==1)
            {
                k=i+1;
            }
            if(Space_num==2)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(seatBlind_id_num+j)=*(str+j+k);
                }
                k=i+1;
                seat_blind[num].id=atoi(seatBlind_id_num);
            }
            if(Space_num==3)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(seatBlind_jetton_num+j)=*(str+j+k);
                }
                k=i+1;
                seat_blind[num].jetton=atoi(seatBlind_jetton_num);
            }
            if(Space_num==4)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(seatBlind_money_num+j)=*(str+j+k);
                }
                k=i+1;
                seat_blind[num].money=atoi(seatBlind_money_num);
            }
        }
    }
    if(seat_blind[num].id == client_id)	//判断是否是自己ID，更新自己当前筹码
	{
		g_now_jetton= seat_blind[num].jetton; 
	}
}

/***********************************************
功能：将获取的盲注信息中的一行信息分别对应存入盲注信息
输入：盲注信息的一行信息 char* str
输出：无
说明：blind__info_msg blind;结构中存入相应信息
*************************************************/
void blind_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char blind_id_num[10]={0,0,0,0,0,0,0,0,0,0};
	char blind_bet_num[10]={0,0,0,0,0,0,0,0,0,0};
	
	num--;
	//输入参数合法性检查 
	if(NULL == str || num < 0 || num > 1)
	{
		return;
	}

    for(i = 0; i<strlen(str); i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                for(j=0;*(str+j)!=32;j++)
                {
                    *(blind_id_num+j)=*(str+j);
                }
                k=i+1;
                blind_infos[num].id=atoi(blind_id_num);
            }
            if(Space_num==1)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(blind_bet_num+j)=*(str+j+k);
                }
                k=i+1;
                blind_infos[num].bet=atoi(blind_bet_num);
            }
        }
    }
}
/***********************************************
功能：手牌消息处理函数 
输入：牌型信息的一行信息 char* str
输出：无
说明：struct cards_msg hold_card,public_card;结构中存入相应信息
*************************************************/
void hd_cards_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char hold_card_color[10]={0,0,0,0,0,0,0,0,0,0};
	char hold_card_point[10]={0,0,0,0,0,0,0,0,0,0};
	
	//输入参数合法性检查 
	if(NULL == str || num < 1 || num > 2)
	{
		return;
	}

    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                for(j=0;*(str+j)!=32;j++)
                {
                    *(hold_card_color+j)=*(str+j);
                }
                k=i+1;
				if(1 == num)
				{
					My_Hand_Card.card1.color = (CardColor)(card_color_decode(hold_card_color));
				}
				else if(2 == num)
				{
					My_Hand_Card.card2.color = (CardColor)(card_color_decode(hold_card_color));
				}
            }
            if(Space_num==1)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(hold_card_point+j)=*(str+j+k);
                }
                k=i+1;
				if(1 == num)
				{
					My_Hand_Card.card1.card_val = (CardValue)(card_point_decode(hold_card_point));
					My_Hand_Card.card1.prime = (Card_Prime)(card_prime_decode(hold_card_point));
//FloatToStr((float)(My_Hand_Card.card1.card_val), "hand_card1: ", NULL);
				}
				else if(2 == num)
				{
					My_Hand_Card.card2.card_val = (CardValue)(card_point_decode(hold_card_point));
					My_Hand_Card.card2.prime = (Card_Prime)(card_prime_decode(hold_card_point));
//FloatToStr((float)(My_Hand_Card.card2.card_val), "hand_card2: ", NULL);
				}
            }
        }
    }
}
/***********************************************
功能：公共牌消息处理函数 
输入：牌型信息的一行信息 char* str
输出：无
说明：struct cards_msg hold_card,public_card;结构中存入相应信息
*************************************************/
void pb_cards_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char public_card_color[10]={0,0,0,0,0,0,0,0,0,0};
	char public_card_point[10]={0,0,0,0,0,0,0,0,0,0};
	
	//输入参数合法性检查 
	if(NULL == str || num < 1 || num > 5)
	{
		return;
	}

    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                for(j=0; *(str+j)!=32; j++)
                {
                    *(public_card_color+j)=*(str+j);
                }
                k=i+1;
				Public_Cards[num-1].color = (CardColor)(card_color_decode(public_card_color));
            }
            if(Space_num==1)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(public_card_point+j)=*(str+j+k);
                }
                k=i+1;
				Public_Cards[num-1].card_val = (CardValue)(card_point_decode(public_card_point));
				Public_Cards[num-1].prime = (Card_Prime)(card_prime_decode(public_card_point));
            }
        }
    }
}

/***********************************************
功能：询问消息处理函数 
输入：座次信息的一行信息 char* str
输出：无
说明：inquire_msg inquire_action;结构中存入相应信息
*************************************************/
void inquire_action_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char inquire_action_id[10]={0,0,0,0,0,0,0,0,0,0};
	char inquire_action_jetton[10]={0,0,0,0,0,0,0,0,0,0};
	char inquire_action_money[10]={0,0,0,0,0,0,0,0,0,0};
	char inquire_action_bet[10]={0,0,0,0,0,0,0,0,0,0};
	char inquire_action_action[10]={0,0,0,0,0,0,0,0,0,0};
	
	num--;
	//输入参数合法性检查 
	if(NULL == str || num < 0 || num > 7)
	{
		return;
	}
    
    if(0 == num)
	{
		ClearInquireAction();//inquire_action清零 
	} 
	
    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                for(j=0;*(str+j)!=32;j++)
                {
                    *(inquire_action_id+j)=*(str+j);
                }
                k=i+1;
                inquire_action[num].id=atoi(inquire_action_id);
            }
            if(Space_num==1)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(inquire_action_jetton+j)=*(str+j+k);
                }
                k=i+1;
                inquire_action[num].jetton=atoi(inquire_action_jetton);
            }
            if(Space_num==2)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(inquire_action_money+j)=*(str+j+k);
                }
                k=i+1;
                inquire_action[num].money=atoi(inquire_action_money);
            }
            if(Space_num==3)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(inquire_action_bet+j)=*(str+j+k);
                }
                k=i+1;
                inquire_action[num].bet=atoi(inquire_action_bet);
            }
            if(Space_num==4)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(inquire_action_action+j)=*(str+j+k);
                }
                k=i+1;
                inquire_action[num].action=action_decode(inquire_action_action);
                
                if(client_id == inquire_action[num].id)
                {
                	g_own_last_action = inquire_action[num].action;
                }
            }
        }
    }
//FloatToStr((float)inquire_action[num].id, "inquire_id :", NULL);
//FloatToStr((float)inquire_action[num].bet, "inquire_bet :", NULL);
}

/***********************************************
功能：底池总金额消息处理函数(询问消息中) 
输入：座次信息的一行信息 char* str
输出：无
说明：int inquire_totalpot;;变量中存入相应信息
*************************************************/
void inquire_totalpot_msg(char* str)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char inquire_pot_num[10]={0,0,0,0,0,0,0,0,0,0};
    
    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                k=i+1;
            }
            if(Space_num==1)
            {
                k=i+1;
            }
            if(Space_num==2)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(inquire_pot_num+j)=*(str+j+k);
                }
                k=i+1;
                inquire_totalpot=atoi(inquire_pot_num);
            }
        }
    }
}

/***********************************************
功能：通知消息处理函数 
输入：座次信息的一行信息 char* str
输出：无
说明：notify_msg notify_action;结构中存入相应信息
*************************************************/
void notify_action_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char notify_action_id[10]={0,0,0,0,0,0,0,0,0,0};
	char notify_action_jetton[10]={0,0,0,0,0,0,0,0,0,0};
	char notify_action_money[10]={0,0,0,0,0,0,0,0,0,0};
	char notify_action_bet[10]={0,0,0,0,0,0,0,0,0,0};
	char notify_action_action[10]={0,0,0,0,0,0,0,0,0,0};
	
	num--;
	//输入参数合法性检查 
	if(NULL == str || num < 0 || num > 7)
	{
		return;
	}
    
    if(0 == num)
	{
		ClearNotifyAction();//notify_action清零 
	} 
	
    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                for(j=0;*(str+j)!=32;j++)
                {
                    *(notify_action_id+j)=*(str+j);
                }
                k=i+1;
                notify_action[num].id=atoi(notify_action_id);
            }
            if(Space_num==1)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(notify_action_jetton+j)=*(str+j+k);
                }
                k=i+1;
                notify_action[num].jetton=atoi(notify_action_jetton);
            }
            if(Space_num==2)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(notify_action_money+j)=*(str+j+k);
                }
                k=i+1;
                notify_action[num].money=atoi(notify_action_money);
            }
            if(Space_num==3)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(notify_action_bet+j)=*(str+j+k);
                }
                k=i+1;
                notify_action[num].bet=atoi(notify_action_bet);
            }
            if(Space_num==4)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(notify_action_action+j)=*(str+j+k);
                }
                k=i+1;
                notify_action[num].action=action_decode(notify_action_action);
            }
        }
    }
}

/***********************************************
功能：底池总金额消息处理函数(询问消息中) 
输入：座次信息的一行信息 char* str
输出：无
说明：int notify_totalpot;变量中存入相应信息
*************************************************/
void notify_totalpot_msg(char* str)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char notify_pot_num[10]={0,0,0,0,0,0,0,0,0,0};
	
	//输入参数合法性检查 
	if(NULL == str)
	{
		return;
	}
    
    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                k=i+1;
            }
            if(Space_num==1)
            {
                k=i+1;
            }
            if(Space_num==2)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(notify_pot_num+j)=*(str+j+k);
                }
                k=i+1;
                notify_totalpot=atoi(notify_pot_num);
            }
        }
    }
}

/***********************************************
功能：彩池分配信息处理 
输入：座次信息的一行信息 char* str
输出：无
说明：pot_win_msg pot_win;结构中存入相应信息
*************************************************/
void potwin_dis_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char pot_win_id[10]={0,0,0,0,0,0,0,0,0,0};
	char pot_win_num[10]={0,0,0,0,0,0,0,0,0,0};
	
	num--;
	//输入参数合法性检查 
	if(NULL == str || num < 0 || num > 7)
	{
		return;
	}

    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                for(j=0;*(str+j)!=32;j++)
                {
                    *(pot_win_id+j)=*(str+j);
                }
                k=i+1;
                pot_win[num].id=atoi(pot_win_id);
            }
            if(Space_num==1)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(pot_win_num+j)=*(str+j+k);
                }
                k=i+1;
                pot_win[num].num=atoi(pot_win_num);
            }
        }
    }
}

/***********************************************
功能：摊牌消息处理函数 
输入：座次信息的一行信息 char* str
输出：无
说明：showdown_rank showdown;结构中存入相应信息
*************************************************/
void showdown_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char showdown_id_rank[10]={0,0,0,0,0,0,0,0,0,0};
	char showdown_id[10]={0,0,0,0,0,0,0,0,0,0};
	char showdown_card1_color[10]={0,0,0,0,0,0,0,0,0,0};
	char showdown_card1_point[10]={0,0,0,0,0,0,0,0,0,0};
	char showdown_card2_color[10]={0,0,0,0,0,0,0,0,0,0};
	char showdown_card2_point[10]={0,0,0,0,0,0,0,0,0,0};
	char showdown_nut_hand[10]={0,0,0,0,0,0,0,0,0,0};
	
	num--;
	//输入参数合法性检查 
	if(NULL == str || num < 0 || num > 7)
	{
		return;
	}
    
    for(i = 0;i<strlen(str);i++)
    {
        if(*(str+i)==32 && *(str+i+1)!=32)
        {
            sum++;
            Space_num=sum-1;
            if(Space_num==0)
            {
                for(j=0;*(str+j)!=32;j++)
                {
                    *(showdown_id_rank+j)=*(str+j);
                }
                k=i+1;
                showdown[num].rank=atoi(showdown_id_rank);
            }
            if(Space_num==1)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(showdown_id+j)=*(str+j+k);
                }
                k=i+1;
                showdown[num].id=atoi(showdown_id);
            }
            if(Space_num==2)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(showdown_card1_color+j)=*(str+j+k);
                }
                k=i+1;
                showdown[num].hand_cards.card1.color = card_color_decode(showdown_card1_color);
            }
            if(Space_num==3)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(showdown_card1_point+j)=*(str+j+k);
                }
                k=i+1;
                showdown[num].hand_cards.card1.card_val = card_point_decode(showdown_card1_point);
                showdown[num].hand_cards.card1.prime = card_prime_decode(showdown_card1_point);
            }
            if(Space_num==4)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(showdown_card2_color+j)=*(str+j+k);
                }
                k=i+1;
                showdown[num].hand_cards.card2.color = card_color_decode(showdown_card2_color);
            }
            if(Space_num==5)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(showdown_card2_point+j)=*(str+j+k);
                }
                k=i+1;
                showdown[num].hand_cards.card2.card_val = card_point_decode(showdown_card2_point);
                showdown[num].hand_cards.card2.prime = card_prime_decode(showdown_card2_point);
            }
            if(Space_num==6)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(showdown_nut_hand+j)=*(str+j+k);
                }
                k=i+1;
                showdown[num].nut_hand=nut_hand_decode(showdown_nut_hand);
            }
        }
    }
}

/****************************************************************************
Function Name		：ActionForMsg
Function Describe	：接收到询问消息时返回相应动作
In					：无
Out					：无
Call Function		：PreflopAction
					  FlopAction
					  TurnAction
					  RiverAction
Called Function		：
Return				：决策动作
Author				：wy
Finish Time			：2015年5月24日
****************************************************************************/
uint8_t* ActionForMsg(void)
{
	uint8_t *pact_str = g_Action_Str[0];
	uint16_t Idx = 0;
	uint16_t i;
	uint16_t Sklansky_Val;
	
	if(0 >= updata_AllOpponentNum) 
	{
		return g_Action_Str[2];
	}
	
	if(Card_Null != Public_Cards[4].card_val)//河牌圈
	{
		Idx = (uint16_t)(RiverAction(My_Hand_Card, Public_Cards, updata_AllOpponentNum));
		pact_str = g_Action_Str[Idx];
	}
	else if(Card_Null != Public_Cards[3].card_val)//转牌圈
	{
		Idx = (uint16_t)(TurnAction(My_Hand_Card, Public_Cards, updata_AllOpponentNum));
		pact_str = g_Action_Str[Idx];
	}
	else if(Card_Null != Public_Cards[0].card_val)//翻牌圈
	{
		Idx = (uint16_t)(FlopAction(My_Hand_Card, Public_Cards, updata_AllOpponentNum));
		pact_str = g_Action_Str[Idx];
	}
	else if(Card_Null != My_Hand_Card.card1.card_val)//盲注圈
	{
		g_winpro = 0;
		Idx = (uint16_t)(PreflopAction(My_Hand_Card));
		pact_str = g_Action_Str[Idx];
	}
	
	if(3 == Idx)
	{
		if(g_winpro >= RAISE_STAGE1)
		{
			pact_str = g_Riase_Str[2];
			Idx = 2;
		}
		else if(g_winpro >= RAISE_STAGE2)
		{
			pact_str = g_Riase_Str[1];
			Idx = 1;
		}
			
		else if(g_winpro >= RAISE_STAGE3)
		{
			pact_str = g_Riase_Str[0];
			Idx = 0;
		}
		
		if(g_winpro >= RAISE_STAGE3)
		{
			//自己选择加注时，更新自己的下注额，以便算出正确的 g_Money_Max
			g_Oppon_Bet_Infos.bet_last += g_Raise_Val[Idx];
		}
	} 
	
	if('f' == *pact_str) 
	{
		g_owm_fold_flg = 0;//弃牌标志 
	}
	
	return pact_str;
}
/****************************************************************************
Function Name		：ClearData
Function Describe	：每局结束，把相应数据清零以接收下一局数据
In					：无
Out					：无
Call Function		：无
Called Function		：
Return				：无
Author				：wy
Finish Time			：2015年5月24日
****************************************************************************/
void ClearData(void)
{
	uint16_t i;
	struct inquire_msg clear_inq = {0,0,0,0,0};
	struct seat_info_msg clear_seatinfo = {0,0,0};

	g_Seat_num=0;//每次开局将座次数清零
	g_now_jetton=0;//每次开局将上局筹码清零
	
	seat_button = clear_seatinfo;
	seat_blind[0] = clear_seatinfo;
	seat_blind[1] = clear_seatinfo;
	
	My_Hand_Card.card2.card_val = Card_Null;
	My_Hand_Card.card1.card_val = Card_Null;
	
	g_Oppon_Bet_Infos.bet_now = 0;//对手最大下注额清零
	g_Oppon_Bet_Infos.bet_last = 0;//自己最大下注额清零 
	
	g_own_last_action = Action_Null;
	g_owm_fold_flg = 1;//未弃牌 
	
	for(i=0; i<PLAYERS_NUM; i++)
	{
		inquire_action[i] = clear_inq;
		showdown[i].id = 0;
		g_players_fold_flg[i].fold_flg = 'N';//都没有弃牌 	
	}
	
	for(i=0; i<5; i++)
	{
		Public_Cards[i].card_val = Card_Null;
		seat[i] = clear_seatinfo;
	}
}

/*计算每局开始时的对手数*/
void CountOpponentNum(void)
{
	if(0 != seat[4].id) 
	{
		AllOpponentNum = 7;
	}
	else if(0 != seat[3].id) 
	{
		AllOpponentNum = 6;
	}
	else if(0 != seat[2].id) 
	{
		AllOpponentNum = 5;
	}
	else if(0 != seat[1].id) 
	{
		AllOpponentNum = 4;
	}
	else if(0 != seat[0].id) 
	{
		AllOpponentNum = 3;
	}
	else if(0 != seat_blind[1].id) 
	{
		AllOpponentNum = 2;
	}
	else
	{
		AllOpponentNum = 1;
	}
	
	updata_AllOpponentNum = AllOpponentNum;
} 

/*更新各个阶段的对手数，在询问消息和通知消息中调用,X_T = 'X'询问消息中调用，X_T = 'T'通知消息中调用*/
void UpdataOpponentNum(uint8_t X_T)
{
	uint16_t i,j;
	
	for(i=0; i<PLAYERS_NUM; i++)
	{		
		if('X' == X_T)
		{
			for(j=0; j<PLAYERS_NUM; j++)
			{
				if(inquire_action[i].id == g_players_fold_flg[j].id)
				{
					break;
				}
			}
			
			if(1 == inquire_action[i].action && 'N' == g_players_fold_flg[j].fold_flg) 
			{ 
				updata_AllOpponentNum--;	
				g_players_fold_flg[j].fold_flg = 'Y';
			}
		} 
		else if('T' == X_T)
		{
			for(j=0; j<PLAYERS_NUM; j++)
			{
				if(inquire_action[i].id == notify_action[j].id)
				{
					break;
				}
			}
			
			if(1 == notify_action[i].action && 'N' == g_players_fold_flg[j].fold_flg) 
			{ 
				updata_AllOpponentNum--;	
				g_players_fold_flg[j].fold_flg = 'Y';
			}
		}
		
	}
} 

void FloatToStr(float data , char *pchar_for , char *pchar_back)
{
	*g_Str='\0';
	char str1[10]={0,0,0,0,0,0,0,0,0,0};
	
	if(pchar_for != NULL && data >= 0 && pchar_back != NULL)
	{
		strcat(g_Str,pchar_for);
		strcat(g_Str," ");
		sprintf(str1,"%5.2f",data);
		strcat(g_Str,str1);
		strcat(g_Str," ");
		strcat(g_Str,pchar_back);
		strcat(g_Str,"\n");
	}
	else if(pchar_back == NULL)
	{
		strcat(g_Str,pchar_for);
		strcat(g_Str," ");
		sprintf(str1,"%5.2f",data);
		strcat(g_Str,str1);
		strcat(g_Str,"\n");
	}
	else if(pchar_for == NULL)
	{
		sprintf(str1,"%5.2f",data);
		strcat(g_Str,str1);
		strcat(g_Str," ");
		strcat(g_Str,pchar_back);
		strcat(g_Str,"\n");
	}
	else if(pchar_for == NULL && pchar_back == NULL)
	{
		sprintf(str1,"%f",data);
		strcat(g_Str,str1);
		strcat(g_Str,"\n");
	}
	else if(data == -1)
	{
		strcat(g_Str,pchar_for);
		strcat(g_Str," ");
		strcat(g_Str,pchar_back);
		strcat(g_Str,"\n");
	}
	else
	{ 
		strcat(g_Str,"\n");
	}
		
	write(debug_fd,g_Str,strlen(g_Str));
}

/*第一局时记录玩家ID(为跟注信息处理和对手动作记录处理准备)*/
void Fill_Players_ID(void)
{
	uint16_t i;
	
	g_Is_First_Game = 2;//更改标志变量，防止ID被更改 
	
	if(0 != seat_button.id)
	{
		opponent_action[0].id = seat_button.id;
		g_opp_act_info[0].id = seat_button.id;
		g_players_fold_flg[0].id = seat_button.id;
	}
	
	for(i=0; i<2; i++)
	{
		if(0 != seat_blind[i].id) 
		{ 
			opponent_action[i + 1].id = seat_blind[i].id;
			g_opp_act_info[i + 1].id =  seat_blind[i].id;
			g_players_fold_flg[i + 1].id = seat_blind[i].id;
		}
	}
	
	for(i=0; i<5; i++)
	{
		if(0 != seat[i].id) 
		{ 
			opponent_action[i + 3].id = seat[i].id;
			g_opp_act_info[i + 3].id = seat[i].id;
			g_players_fold_flg[i + 3].id = seat[i].id;
		}
	}
}

/*更新玩家赌注，且计算最大下注额，在询问消息命令尾部被调用*/
void Updata_Players_Bet(void)
{
	uint16_t i;
	
	g_Money_Max = 0;
	
	for(i=0; i<8; i++)
	{	
		if((g_Oppon_Bet_Infos.bet_now < inquire_action[i].bet) && (0 != inquire_action[i].id))
		{
			g_Oppon_Bet_Infos.bet_now = inquire_action[i].bet;
			g_Oppon_Bet_Infos.id = inquire_action[i].id;
		}
		
		if((client_id == inquire_action[i].id) && (0 != inquire_action[i].id))
		{
			if(inquire_action[i].bet > g_Oppon_Bet_Infos.bet_last)
			{
				g_Oppon_Bet_Infos.bet_last = inquire_action[i].bet;
			}
		}
	}
	
	if(g_Oppon_Bet_Infos.bet_now > g_Oppon_Bet_Infos.bet_last)
	{
		g_Money_Max = g_Oppon_Bet_Infos.bet_now - g_Oppon_Bet_Infos.bet_last;
	}
	
//FloatToStr((float)g_e_game_stage, "g_e_game_stage: ", NULL);
//FloatToStr((float)(g_Money_Max), "g_Money_Max: ", NULL);
}

/*更新对手动作数，在通知消息命令尾部被调用*/
void Updata_Opponent_Noti_ActionNum(void)
{
	uint16_t i,j;
	
	for(i=0; i<PLAYERS_NUM; i++)
	{
		for(j=0; j<PLAYERS_NUM; j++)
		{
			if(notify_action[i].id == opponent_action[j].id)
			{
				switch(notify_action[i].action)
				{
					case 1:
						opponent_action[j].opponent_fold++;
						Updata_Players_Actions(j, 1);
//FloatToStr((float)j,"the order :", NULL);
//FloatToStr((float)notify_action[i].action,"notify_action :", NULL);
//FloatToStr((float)opponent_action[j].opponent_fold,"Notify_fold_ActionNum:", NULL);
						break;
					case 2:
						opponent_action[j].opponent_call++;
						Updata_Players_Actions(j, 2);
						break;
					case 3:
						opponent_action[j].opponent_raise++;
						Updata_Players_Actions(j, 3);
						break;
					case 4:
						opponent_action[j].opponent_all_in++;
						Updata_Players_Actions(j, 4);
						break;
					case 5:
						opponent_action[j].opponent_check++;
						Updata_Players_Actions(j, 5);
						break;
					default:
						break;
				}
			}
		}
	}
}

/*更新对手动作数，在询问消息命令尾部被调用*/
void Updata_Opponent_Inqu_ActionNum(void)
{
	uint16_t i,j;
	
	if(g_Inquire_num == g_Seat_num)//若数目不同则不更新 
	{
		for(i=0; i<PLAYERS_NUM; i++)
		{
			for(j=0; j<PLAYERS_NUM; j++)
			{
				if(inquire_action[i].id == opponent_action[j].id)
				{
					switch(inquire_action[i].action)
					{
						case 1:
							opponent_action[j].opponent_fold++;
							Updata_Players_Actions(j, 1);
//FloatToStr((float)j,"the order :", NULL);
//FloatToStr((float)notify_action[i].action,"notify_action :", NULL);
//FloatToStr((float)opponent_action[j].opponent_fold,"Notify_fold_ActionNum:", NULL);
							break;
						case 2:
							opponent_action[j].opponent_call++;
							Updata_Players_Actions(j, 2);
							break;
						case 3:
							opponent_action[j].opponent_raise++;
							Updata_Players_Actions(j, 3);
							break;
						case 4:
							opponent_action[j].opponent_all_in++;
							Updata_Players_Actions(j, 4);
							break;
						case 5:
							opponent_action[j].opponent_check++;
							Updata_Players_Actions(j, 5);
							break;
						default:
							break;
					}
				}
			}
		}
	} 

}

/*更新g_opp_act_info数组的数据*/
void Updata_Players_Actions(int Id_idx,int action_code)
{
	if(Id_idx < 0 || Id_idx > 7) 
	{
		return;
	}
	
	switch(action_code)
	{
		case 1:
			g_opp_act_info[Id_idx].fold_num[g_e_game_stage - 1]++;
			break; 
		case 2:
			g_opp_act_info[Id_idx].call.now_num[g_e_game_stage - 1]++;
//if(0 == Id_idx && 1 == g_e_game_stage)
//{
//FloatToStr((float)g_opp_act_info[0].call.stage_call_num[0],"the stage_call_num[0] :", NULL);
//}
			break; 
		case 3:
			g_opp_act_info[Id_idx].raise.now_num[g_e_game_stage - 1]++;
			break; 
		case 4:
			g_opp_act_info[Id_idx].all_in.now_num[g_e_game_stage - 1]++;
			break; 
		case 5:
			g_opp_act_info[Id_idx].check.now_num[g_e_game_stage - 1]++;
			break; 
		default:
			break;
	}
}

/*inquire_action清零*/
void ClearInquireAction(void)
{
	uint16_t i;
	struct inquire_msg clear_inq = {0,0,0,0,0};

	for(i=0; i<PLAYERS_NUM; i++)
	{
		inquire_action[i] = clear_inq;
	}
}

/*notify_action清零*/
void ClearNotifyAction(void)
{
	uint16_t i;
	struct inquire_msg clear_inq = {0,0,0,0,0};

	for(i=0; i<PLAYERS_NUM; i++)
	{
		notify_action[i] = clear_inq;
	}
}
////////////////////////////////////////////////////////////////////////////////
/*摊牌阶段处理函数,在摊牌消息末尾调用*/
void Showdown_Process(void)
{
	uint16_t i,j;
	
	for(i=0; i<PLAYERS_NUM; i++)
	{
		if(0 != showdown[i].id)
		{
			for(j=0; j<PLAYERS_NUM; j++)
			{
				if(showdown[i].id == g_opp_act_info[j].id)
				{
					Showdown_Cal_Rai_All_Che(j, i);
				}
			}
		}
	}
} 

/*计算对手跟注牌力，仅被Showdown_Process调用，若g_opp_act_info[k].id == showdown[j].id，则idx = k,show_idx = j*/
static void Showdown_Cal_Rai_All_Che(unsigned int idx, unsigned int show_idx)
{
	uint16_t j,i;
	uint16_t sklansky_val = 0;//手牌等级 
	float win_pro = 0;
	int *pnow = NULL;
	float *ppower_aver = NULL;
	int *pall_num = NULL;
	
	for(j=0; j<4; j++)//Call、Raise 、All_in、Check
	{
		if(0 == j)
		{
			pnow = g_opp_act_info[idx].call.now_num;
			ppower_aver = g_opp_act_info[idx].call.power_aver;
			pall_num = g_opp_act_info[idx].call.all_num;
		}
		else if(1 == j)
		{
			pnow = g_opp_act_info[idx].raise.now_num;
			ppower_aver = g_opp_act_info[idx].raise.power_aver;
			pall_num = g_opp_act_info[idx].raise.all_num;
		}
		else if(2 == j)
		{
			pnow = g_opp_act_info[idx].all_in.now_num;
			ppower_aver = g_opp_act_info[idx].all_in.power_aver;
			pall_num = g_opp_act_info[idx].all_in.all_num;
		}
		else if(3 == j)
		{
			pnow = g_opp_act_info[idx].check.now_num;
			ppower_aver = g_opp_act_info[idx].check.power_aver;
			pall_num = g_opp_act_info[idx].check.all_num;
		}
		
		for(i=0; i<4; i++)//Preflop、Flop、Turn、River
		{
			if(0 != pnow[i])
			{	
				if(0 == i)//盲注阶段 
				{
					/*手牌等级计算*/
					PreflopCardsSkl(showdown[show_idx].hand_cards, &sklansky_val); 
					win_pro = (float)sklansky_val;
				} 
				else if(1 == i)//翻牌圈  
				{
					/*计算获胜概率*/
					HandWinProbabilityThree(showdown[show_idx].hand_cards, Public_Cards, updata_AllOpponentNum, &win_pro);
				}
				else if(2 == i)//转牌圈  
				{
					/*计算获胜概率*/
					HandWinProbabilityFour(showdown[show_idx].hand_cards, Public_Cards, updata_AllOpponentNum, &win_pro);
				}
				else if(3 == i)//河牌圈  
				{
					/*计算获胜概率*/
					HandWinProbabilityFive(showdown[show_idx].hand_cards, Public_Cards, updata_AllOpponentNum, &win_pro);
if(3333 == showdown[show_idx].id && 0 != g_opp_act_info[idx].check.now_num[i])
{
id_test_idx = idx;
FloatToStr(win_pro,"win_pro :", NULL);	
FloatToStr((float)g_opp_act_info[idx].check.power_aver[3] , "aver:", NULL);
}
				}
				
				/*更新平均获胜概率*/
				ppower_aver[i] = (float)(ppower_aver[i] * pall_num[i] + win_pro) / (pall_num[i] + 1);
				//更新20局最小的跟注牌力
				Save_Min_Data20(idx, i, (float)win_pro, j); 
				//更新20局最大的跟注牌力
				Save_Max_Data20(idx, i, (float)win_pro, j);
				//更新盲注阶段的跟注次数 
				pall_num[i]++;
				//now_num[i]清零，为下一次标注做准备 
				pnow[i] = 0;
			}
		}//End for i
	}//End for j
} 

/*保存最小20个数;若g_opp_act_info[k].id == showdown[j].id，则idx = k;data参与排序的数;
stage四个阶段;action_code 动作代码*/
void Save_Min_Data20(unsigned int idx, unsigned int stage, float data, int action_code)
{
	uint16_t i,j;
	
	int *pall_num = NULL;
	float *ppower_min = NULL;
	float change;
	
	if(0 == action_code)
	{
		pall_num = g_opp_act_info[idx].call.all_num;
		ppower_min = &g_opp_act_info[idx].call.power_min20[stage][0];
	}
	else if(1 == action_code)
	{
		pall_num = g_opp_act_info[idx].raise.all_num;
		ppower_min = &g_opp_act_info[idx].raise.power_min20[stage][0];
	}
	else if(2 == action_code)
	{
		pall_num = g_opp_act_info[idx].all_in.all_num;
		ppower_min = &g_opp_act_info[idx].all_in.power_min20[stage][0];
	}
	else if(3 == action_code)
	{
		pall_num = g_opp_act_info[idx].check.all_num;
		ppower_min = &g_opp_act_info[idx].check.power_min20[stage][0];
	}
	
	//参与排序的数比最大的都大，则舍去 
	if(data > ppower_min[19] && pall_num[stage] >= 20)
	{
		return;
	}
	
	if(pall_num[stage] >= 20)	
	{
		for(j=19; j>0; j--)
		{	
			//data肯定比 g_opp_act_info[idx].call.pro_min20[stage][j]小 
			if(data < ppower_min[j - 1])
			{
				ppower_min[j] = ppower_min[j - 1];
				ppower_min[j - 1] = data;
			}
			else
			{
				ppower_min[j] = data;
				break;
			}
		}//End for
	}
	else
	{
		j = pall_num[stage];
		ppower_min[j] = data;
		
		if(19 == j)
		{
			//降序排序 
			for(i=0; i<20; i++)
			{
				for(j=0; j<19-i; j++)
				{
					if(ppower_min[j] < ppower_min[j + 1])
					{
						change = ppower_min[j];	
						ppower_min[j] = ppower_min[j + 1];
						ppower_min[j + 1] = change;
					}
				}
			}
		}
	}
}

/*保存最大20个数;若g_opp_act_info[k].id == showdown[j].id，则idx = k;data参与排序的数;stage四个阶段;action_code 动作代码*/
void Save_Max_Data20(unsigned int idx,unsigned int stage, float data, int action_code)
{
	uint16_t i,j;
	
	int *pall_num = NULL;
	float *ppower_max = NULL;
	float change;
	
	if(0 == action_code)
	{
		pall_num = g_opp_act_info[idx].call.all_num;
		ppower_max = &g_opp_act_info[idx].call.power_max20[stage][0];
	}
	else if(1 == action_code)
	{
		pall_num = g_opp_act_info[idx].raise.all_num;
		ppower_max = &g_opp_act_info[idx].raise.power_max20[stage][0];
	}
	else if(2 == action_code)
	{
		pall_num = g_opp_act_info[idx].all_in.all_num;
		ppower_max = &g_opp_act_info[idx].all_in.power_max20[stage][0];
	}
	else if(3 == action_code)
	{
		pall_num = g_opp_act_info[idx].check.all_num;
		ppower_max = &g_opp_act_info[idx].check.power_max20[stage][0];
	}
	
	//参与排序的数比最小的都小，则舍去 
	if(data < ppower_max[0] && pall_num[stage] >= 20)
	{
		return;
	}
		
	if(pall_num[stage] >= 20)
	{
		for(j=0; j<19; j++)
		{	
			//data肯定比 g_opp_act_info[idx].call.pro_max20[stage][j]大 
			if(data > ppower_max[j + 1])
			{
				ppower_max[j] = ppower_max[j + 1];
				ppower_max[j + 1] = data;
			}
			else
			{
				ppower_max[j] = data;
				break;
			}
		}//End for
	}//End if
	else
	{
		j = pall_num[stage];
		ppower_max[j] = data;
		
		if(19 == j)
		{
			//降序排序 
			for(i=0; i<20; i++)
			{
				for(j=0; j<19-i; j++)
				{
					if(ppower_max[j] < ppower_max[j + 1])
					{
						change = ppower_max[j];
						ppower_max[j] = ppower_max[j + 1];						
						ppower_max[j + 1] = change;
					}
				}
			}
		}
	}
}



//***************************************************************************/// 