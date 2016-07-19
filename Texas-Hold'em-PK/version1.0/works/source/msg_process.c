 /******************************************************************************
  �� �� ��   : card_decode.h
  �� �� ��   : ����
  ��    ��   : GBR
  ��������   : 2015��5��20��
  ����޸�   :
  ��������   : ���ݽṹ����
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
#include "Message.h"

CardInfos Public_Cards[5];//������
uint16_t AllOpponentNum = 1;//�ܵĶ�����
uint16_t updata_AllOpponentNum;//���¶����� 
uint8_t* p_Action=NULL;
extern int skfd;

extern int debug_fd; // �����ļ� 
char g_Str[100];//�����ļ������� 
int g_game_Num = 0;//�ܾ��� 

//������עȦ��ѯ����Ϣǰ�������ע��Ϣ ,��CountBetMax����ĩβ������ 
Bet_Infos g_Oppon_Bet_Infos[8]; 

//�Ƿ�Ϊ�׾ֱ�־�����ڵ�һ����� g_Oppon_Act_Infos��������ID ,
//��Fill_Players_ID��������Ϊ2 
uint16_t g_Is_First_Game = 1;

uint8_t g_Action_Str[6][10] = {{"fold"},{"fold"},{"call"},{"raise 10"},{"all_in"},{"check"}};
uint8_t g_Riase_Str[3][10] = {{"raise 20"},{"raise 50"},{"raise 100"}};

enum STATE state = DEF;//״̬ 

int process(int length, const char* buffer)
{
	int next = 0, len = 0,cmd,num;
	char str[100];
	while(next < length)
	{
		len = get_line(buffer + next , str);	// ���ش�buffer�ж�ȡ�ĳ��� ������str�ĳ��ȡ� 
											//���� ���з�����ȡ����û��str�� 
		next += len;
		cmd = cmd_decode(str);
		if(cmd > 0)	//������
	 	{
			switch(cmd)
		 	{
				case 1:
					ClearData();
					num = 0; 
					state = SEAT;
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
					break;
				case 9:
					num = 0;
					state = FLOP;
					break; 
				case 11:
					num = 0;
					state = TURN;
					break;  
				case 13:
					num = 0;
					state = RIVER;
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
				case 18://  /COMMON  ָ�� �˻�SHOWDOWN״̬
					num = 0; 
					state = SHOWDOWN;
					break;
				case 2:
					CountOpponentNum();
					if(1 == g_Is_First_Game)
					{
						Fill_Players_ID();//��һ�ּ�¼�����ID 
					}
g_game_Num++; 
FloatToStr((float)g_game_Num , "Game_num:", "\n");
write(debug_fd,g_Str,strlen(g_Str));//////////////////1	
					state = DEF; 
					break;  //	/****    ָ�� �˻�DEF״̬ 
				
				case 8:
					Updata_Players_Bet();//���¶��ֶ�ע 
					CountBetMax();//�����עע���Updata_Players_Bet֮�󱻵��� 
FloatToStr((float)g_Money_Max , "Oppon_Bet_Infos:", "\n");
write(debug_fd,g_Str,strlen(g_Str));
write(debug_fd,"\n",1);					
					state = DEF; 
					break; 
				case 4:
				case 6:
				case 10:
				case 12:
				case 14:
				case 16:
				case 20:
					state = DEF; 
					break;  
				case 21:// gameover ָ��  ���� 
					return -1; 
				default:
					state = DEF; 
			} 
		}
		else				//��������   ��������ִ����� 
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
					if(num>=4 && num<=8)
					{
						seat_msg(str,num-3);
					}
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
					if(*str!='t')
					{
						inquire_action_msg(str,num);		
					}
					else
					{
						inquire_totalpot_msg(str);
						UpdataOpponentNum();
FloatToStr((float)updata_AllOpponentNum, "AllOpponentNum :", NULL);
write(debug_fd,g_Str,strlen(g_Str));
write(debug_fd,"\n",1);
						if(updata_AllOpponentNum==0) 
						{
							write(skfd,"call",4);
						}
						else
						{
							p_Action=ActionForMsg();
							write(skfd,p_Action,strlen(p_Action));	
						}
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
���ܣ���ͨ������Ϣ������ 
���룺������Ϣ��һ����Ϣ char* str��int num(�ڼ�λ)  
�������
˵����seat_info_msg seat;�ṹ�д�����Ӧ��Ϣ
*************************************************/
void seat_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char seat_id_num[10];
	char seat_jetton_num[10];
	char seat_money_num[10];
	
	num--;
	//��������Ϸ��Լ�� 
	if(NULL == str || num < 0 || num > 4)
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
                    *(seat_id_num+j)=*(str+j);
                }
                k=i+1;
                seat[num].id=atoi(seat_id_num);
            }
            if(Space_num==1)
            {
                for(j=0;*(str+j+k)!=32;j++)
                {
                    *(seat_jetton_num+j)=*(str+j+k);
                }
                k=i+1;
                seat[num].jetton=atoi(seat_jetton_num);
            }
            if(Space_num==2)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(seat_money_num+j)=*(str+j+k);
                }
                k=i+1;
                seat[num].money=atoi(seat_money_num);
            }
        }
    }
}
/***********************************************
���ܣ���ׯ�ҵ�������Ϣ�ֱ��Ӧ����ׯ�ҵĽṹ����
���룺������Ϣ��һ����Ϣ char* str
�������
˵����seat_info_msg seat_button;�ṹ�д�����Ӧ��Ϣ
*************************************************/
void seat_button_msg(char* str)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char seatButton_id_num[10];
	char seatButton_jetton_num[10];
	char seatButton_money_num[10];
	
	//��������Ϸ��Լ�� 
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
}

/***********************************************
���ܣ���äע��������Ϣ�ֱ��Ӧ����ׯ�ҵĽṹ����
���룺������Ϣ��һ����Ϣ char* str
�������
˵����seat_info_msg seat_blind;�ṹ�д�����Ӧ��Ϣ
*************************************************/
void seat_blind_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char seatBlind_id_num[10];
	char seatBlind_jetton_num[10];
	char seatBlind_money_num[10];

	num--;
	//��������Ϸ��Լ�� 
	if(NULL == str || num < 0 || num > 1)
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
}

/***********************************************
���ܣ�����ȡ��äע��Ϣ�е�һ����Ϣ�ֱ��Ӧ����äע��Ϣ
���룺äע��Ϣ��һ����Ϣ char* str
�������
˵����blind__info_msg blind;�ṹ�д�����Ӧ��Ϣ
*************************************************/
void blind_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char blind_id_num[10];
	char blind_bet_num[10];
	
	num--;
	//��������Ϸ��Լ�� 
	if(NULL == str || num < 0 || num > 1)
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
���ܣ�������Ϣ������ 
���룺������Ϣ��һ����Ϣ char* str
�������
˵����struct cards_msg hold_card,public_card;�ṹ�д�����Ӧ��Ϣ
*************************************************/
void hd_cards_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char hold_card_color[10];
	char hold_card_point[10];
	
	//��������Ϸ��Լ�� 
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
FloatToStr((float)(My_Hand_Card.card1.card_val), "hand_card1: ", NULL);
write(debug_fd,g_Str,strlen(g_Str));//////////////////19
write(debug_fd,"\n\n",2);//////////////////20
				}
				else if(2 == num)
				{
					My_Hand_Card.card2.card_val = (CardValue)(card_point_decode(hold_card_point));
					My_Hand_Card.card2.prime = (Card_Prime)(card_prime_decode(hold_card_point));
FloatToStr((float)(My_Hand_Card.card2.card_val), "hand_card2: ", NULL);
write(debug_fd,g_Str,strlen(g_Str));//////////////////19
write(debug_fd,"\n\n",2);//////////////////20
				}
            }
        }
    }
}
/***********************************************
���ܣ���������Ϣ������ 
���룺������Ϣ��һ����Ϣ char* str
�������
˵����struct cards_msg hold_card,public_card;�ṹ�д�����Ӧ��Ϣ
*************************************************/
void pb_cards_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char public_card_color[10];
	char public_card_point[10];
	
	//��������Ϸ��Լ�� 
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
                for(j=0;*(str+j)!=32;j++)
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
���ܣ�ѯ����Ϣ������ 
���룺������Ϣ��һ����Ϣ char* str
�������
˵����inquire_msg inquire_action;�ṹ�д�����Ӧ��Ϣ
*************************************************/
void inquire_action_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char inquire_action_id[10];
	char inquire_action_jetton[10];
	char inquire_action_money[10];
	char inquire_action_bet[10];
	char inquire_action_action[10];
	
	num--;
	//��������Ϸ��Լ�� 
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
            }
        }
    }
}

/***********************************************
���ܣ��׳��ܽ����Ϣ������ 
���룺������Ϣ��һ����Ϣ char* str
�������
˵����int inquire_totalpot;;�����д�����Ӧ��Ϣ
*************************************************/
void inquire_totalpot_msg(char* str)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char inquire_pot_num[10];
    
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
���ܣ��ʳط�����Ϣ���� 
���룺������Ϣ��һ����Ϣ char* str
�������
˵����pot_win_msg pot_win;�ṹ�д�����Ӧ��Ϣ
*************************************************/
void potwin_dis_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char pot_win_id[10];
	char pot_win_num[10];

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
���ܣ�̯����Ϣ������ 
���룺������Ϣ��һ����Ϣ char* str
�������
˵����showdown_rank showdown;�ṹ�д�����Ӧ��Ϣ
*************************************************/
void showdown_msg(char* str,int num)
{
    int i,j=0,k=0;
    int Space_num=0,sum=0;

	char showdown_id_rank[10];
	char showdown_id[10];
	char showdown_card1_color[10];
	char showdown_card1_point[10];
	char showdown_card2_color[10];
	char showdown_card2_point[10];
	char showdown_nut_hand[10];
    
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
                showdown[num].card1_color=card_color_decode(showdown_card1_color);
            }
            if(Space_num==3)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(showdown_card1_point+j)=*(str+j+k);
                }
                k=i+1;
                showdown[num].card1_point=card_point_decode(showdown_card1_point);
            }
            if(Space_num==4)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(showdown_card2_color+j)=*(str+j+k);
                }
                k=i+1;
                showdown[num].card2_color=card_color_decode(showdown_card2_color);
            }
            if(Space_num==5)
            {
                for(j=0;*(str+k+j)!=32;j++)
                {
                    *(showdown_card2_point+j)=*(str+j+k);
                }
                k=i+1;
                showdown[num].card2_point=card_point_decode(showdown_card2_point);
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
Function Name		��ActionForMsg
Function Describe	�����յ�ѯ����Ϣʱ������Ӧ����
In					����
Out					����
Call Function		��PreflopAction
					  FlopAction
					  TurnAction
					  RiverAction
Called Function		��
Return				�����߶���
Author				��wy
Finish Time			��2015��5��24��
****************************************************************************/
uint8_t* ActionForMsg(void)
{
	uint8_t *pact_str = g_Action_Str[0];
	uint16_t Idx = 0;
	uint16_t i;
	uint16_t Sklansky_Val;
	
	if(Card_Null != Public_Cards[4].card_val)//����Ȧ
	{
		Idx = (uint16_t)(RiverAction(My_Hand_Card, Public_Cards, updata_AllOpponentNum));
		if(Idx == 3 && g_winpro >=0.98)
			pact_str = g_Riase_Str[2];
		else if(Idx == 3 && g_winpro >=0.95)
			pact_str = g_Riase_Str[1];
		else if(Idx == 3 && g_winpro >=0.92)
			pact_str = g_Riase_Str[0];
		else
			pact_str = g_Action_Str[Idx];
	}
	else if(Card_Null != Public_Cards[3].card_val)//ת��Ȧ
	{
		Idx = (uint16_t)(TurnAction(My_Hand_Card, Public_Cards, updata_AllOpponentNum));
		if(Idx == 3 && g_winpro >=0.98)
			pact_str = g_Riase_Str[2];
		else if(Idx == 3 && g_winpro >=0.95)
			pact_str = g_Riase_Str[1];
		else if(Idx == 3 && g_winpro >=0.92)
			pact_str = g_Riase_Str[0];
		else
			pact_str = g_Action_Str[Idx];
	}
	else if(Card_Null != Public_Cards[0].card_val)//����Ȧ
	{
		Idx = (uint16_t)(FlopAction(My_Hand_Card, Public_Cards, updata_AllOpponentNum));
		if(Idx == 3 && g_winpro >=0.98)
			pact_str = g_Riase_Str[2];
		else if(Idx == 3 && g_winpro >=0.95)
			pact_str = g_Riase_Str[1];
		else if(Idx == 3 && g_winpro >=0.92)
			pact_str = g_Riase_Str[0];
		else
			pact_str = g_Action_Str[Idx];
	}
	else if(Card_Null != My_Hand_Card.card1.card_val)//äעȦ
	{
		Idx = (uint16_t)(PreflopAction(My_Hand_Card));
		pact_str = g_Action_Str[Idx];
		
PreflopCardsSkl(My_Hand_Card, &Sklansky_Val);
FloatToStr((float)Sklansky_Val, "Hand Stage:", "     ");
write(debug_fd,g_Str,strlen(g_Str));//////////////////2
//
//FloatToStr((float)g_Rand_Data, "Rand Data:", "     ");
//write(debug_fd,g_Str,strlen(g_Str));//////////////////3
//
FloatToStr((float)(-1), "Action: ", pact_str);
write(debug_fd,g_Str,strlen(g_Str));
write(debug_fd,"\n",1);
	}
	

	return pact_str;
}
/****************************************************************************
Function Name		��ClearData
Function Describe	��ÿ�ֽ���������Ӧ���������Խ�����һ������
In					����
Out					����
Call Function		����
Called Function		��
Return				����
Author				��wy
Finish Time			��2015��5��24��
****************************************************************************/
void ClearData(void)
{
	uint16_t i;
	struct inquire_msg clear_inq = {0,0,0,0,0};
	struct seat_info_msg clear_seatinfo = {0,0,0};

	seat_button = clear_seatinfo;
	seat_blind[0] = clear_seatinfo;
	seat_blind[1] = clear_seatinfo;
	My_Hand_Card.card2.card_val = Card_Null;
	My_Hand_Card.card1.card_val = Card_Null;
	
	for(i=0; i<8; i++)
	{
		inquire_action[i] = clear_inq;
	}
	
	for(i=0; i<5; i++)
	{
		Public_Cards[i].card_val = Card_Null;
		seat[i] = clear_seatinfo;
	}
}

/*����ÿ�ֿ�ʼʱ�Ķ�����*/
void CountOpponentNum(void)
{
	if(0 != seat[4].jetton) 
	{
		AllOpponentNum = 7;
	}
	else if(0 != seat[3].jetton) 
	{
		AllOpponentNum = 6;
	}
	else if(0 != seat[2].jetton) 
	{
		AllOpponentNum = 5;
	}
	else if(0 != seat[1].jetton) 
	{
		AllOpponentNum = 4;
	}
	else if(0 != seat[0].jetton) 
	{
		AllOpponentNum = 3;
	}
	else if(0 != seat_blind[1].jetton) 
	{
		AllOpponentNum = 2;
	}
	else
	{
		AllOpponentNum = 1;
	}	
} 

/*���¸����׶εĶ���������ѯ����Ϣʱ����*/
void UpdataOpponentNum(void)
{
	uint16_t i;
	updata_AllOpponentNum=AllOpponentNum;
	for(i=0; i<8; i++)
	{
		if(1 == inquire_action[i].action) 
		{ 
			updata_AllOpponentNum--;	
		}
	}
} 

void FloatToStr(float data , char *pchar_for , char *pchar_back)
{
	*g_Str='\0';
	char str1[10];
	
	if(pchar_for != NULL && data >= 0 && pchar_back != NULL)
	{
		strcat(g_Str,pchar_for);
		strcat(g_Str," ");
		sprintf(str1,"%5.4f",data);
		strcat(g_Str,str1);
		strcat(g_Str," ");
		strcat(g_Str,pchar_back);
		strcat(g_Str,"\n");
	}
	else if(pchar_back == NULL)
	{
		strcat(g_Str,pchar_for);
		strcat(g_Str," ");
		sprintf(str1,"%5.4f",data);
		strcat(g_Str,str1);
		strcat(g_Str,"\n");
	}
	else if(pchar_for == NULL)
	{
		sprintf(str1,"%5.4f",data);
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
		strcat(g_Str,"\n");
}

/*��һ��ʱ��¼���ID*/
void Fill_Players_ID(void)
{
	uint16_t i;
	
	g_Is_First_Game = 2;//���ı�־��������ֹID������ 
	
	if(0 != seat_button.id)
	{
		g_Oppon_Bet_Infos[0].id = seat_button.id;
	}
	
	for(i=0; i<2; i++)
	{
		if(0 != seat_blind[i].id) 
		{ 
			g_Oppon_Bet_Infos[i + 1].id = seat_blind[i].id;
		}
	}
	
	for(i=0; i<5; i++)
	{
		if(0 != seat[i].id) 
		{ 
			g_Oppon_Bet_Infos[i + 3].id = seat[i].id;
		}
	}
}

/*������Ҷ�ע����ѯ����Ϣ����β��������*/
void Updata_Players_Bet(void)
{
	uint16_t i,j;
	
	for(i=0; i<8; i++)
	{
		for(j=0; j<8; j++)
		{
			if((inquire_action[i].id == g_Oppon_Bet_Infos[j].id)
			    && (Fold != inquire_action[i].action)
				&& (Action_Null != inquire_action[i].action))
			    {
    				g_Oppon_Bet_Infos[j].bet_last = g_Oppon_Bet_Infos[j].bet_now;
    				g_Oppon_Bet_Infos[j].bet_now = inquire_action[i].bet;
    				return;
    			}
		}
	}
}

/*����ÿ����ע���ֵ������ע����g_Money_Max������ѯ����Ϣ����β��Updata_Players_Bet֮�󱻵���*/
void CountBetMax(void)
{
	uint16_t i,j;
	int money;
	Bet_Infos clr = {0,0,0};
	
	g_Money_Max = 0;
		
	for(i=0; i<8; i++)
	{
		money = g_Oppon_Bet_Infos[i].bet_now - g_Oppon_Bet_Infos[i].bet_last;
				
		if(money > g_Money_Max)
		{
			for(j=0; j<8; j++)
			{
				//�ж������ע��������Ƿ����� 
				if(inquire_action[j].id == g_Oppon_Bet_Infos[i].id
				&& (Fold != inquire_action[j].action)
				&& (Action_Null != inquire_action[j].action))
				{
					g_Money_Max = (uint16_t)money;
					break;
				}
			}	
		}
		
		g_Oppon_Bet_Infos[i] = clr;
	}
}
