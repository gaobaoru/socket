/******************************************************************************
  �� �� ��   : Message.h
  �� �� ��   : ����
  ��    ��   : GBR 
  ��������   : 2015��5��19��
  ����޸�   :
  ��������   : ���ݽṹ����
******************************************************************************/
#include "Action.h"

#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

/*��ر�������*/

/*������Ϣ*/
struct seat_info_msg
{
    int id;
    int jetton;
    int money;
}seat[5],seat_button,seat_blind[2];
/*äע��Ϣ*/
struct blind_info_msg
{
    int id;
    int bet;
}blind_infos[2];

/*ѯ���ж��������Ϣ*/
struct inquire_msg
{
	int id;  
	int jetton;//���г��� 
	int money; //ʣ������ 
	int bet;  //�������ۼ�Ͷע��
	int action; 
}inquire_action[8];

/*ѯ�ʵ�ǰ��ע��Ϣ*/
int inquire_totalpot;

/*֪ͨ��Ϣ���ж��������Ϣ*/
struct notify_msg
{
	int id;  
	int jetton;//���г��� 
	int money; //ʣ������ 
	int bet;  //�������ۼ�Ͷע��
	int action; 
}notify_action[8];

/*֪ͨ��ǰ��ע��Ϣ*/
int notify_totalpot;

/*̯�ƹ�����Ϣ*/ 
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

/*�ʳط�����Ϣ*/
struct pot_win_msg
{
	int id;
	int num;
}pot_win[8]; 


//��������״̬����Ϊ����ʮһ��֮һ 
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



