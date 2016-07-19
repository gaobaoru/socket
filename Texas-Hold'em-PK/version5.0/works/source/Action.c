/******************************************************************************

                  ��Ȩ���� (C), 2015-2015, OUC

 ******************************************************************************
  �� �� ��   : Action.cpp
  �� �� ��   : ����
  ��    ��   : GBR
  ��������   : 2015��5��23��
  ����޸�   :
  ��������   : �������߶���
******************************************************************************/
#include <stdio.h>
#include "Action.h"
#include <stdlib.h>
#include <time.h>   

//��äע�� 
#define BIG_BLIND 40

//äע�׶���ע��ֵ��myself�� 
#define PREFLOP_MONEY_M 5*BIG_BLIND
#define PREFLOP_MONEY_N 3*BIG_BLIND
#define PREFLOP_MONEY_K 2*BIG_BLIND
#define PREFLOP_MONEY_H 1*BIG_BLIND

//���ơ�ת�ơ����ƽ׶���ע��ֵ��myself�� 
#define MONEY_M 10*BIG_BLIND
#define MONEY_N 8*BIG_BLIND
#define MONEY_K 5*BIG_BLIND
#define MONEY_H 3*BIG_BLIND

/*
ȫ�ֱ���g_Money_MaxΪÿ��ÿ����ע�׶��Լ���Ҫ����ע�� ��
����Ҫ��ѯ����Ϣ�в��ϸ��¡�
*/
uint16_t g_Money_Max = 0;

/*
ȫ�ֱ���g_own_last_actionΪ��һ���Լ��Ķ�����Ϣ��
��ѯ����Ϣ�����б����� 
*/
ActionStatue g_own_last_action = Action_Null;

/*�����������*/
uint16_t g_Rand_Data;

/*ȡʤ����*/
float_t g_winpro;

/*���¶����� */
uint16_t updata_AllOpponentNum;

/****************************************************************************
Function Name		��RandData
Function Describe	���������0��100��������
In					����
Out					����
Call Function		��srand
					  rand
Called Function		��
Return				��0��100��������
Author				��wy
Finish Time			��2015��5��23��
****************************************************************************/
uint16_t RandData(void)
{
	srand( (unsigned)time( NULL ) );          //ʹ�õ�ǰʱ�������ӳ�ʼ�������
	g_Rand_Data = (rand() % 100);
    return g_Rand_Data;
}

/****************************************************************************
Function Name		��PreflopAction
Function Describe	������ǰ��ע����
In					��hand_cards ����
Out					����
Call Function		��PreflopCardsSkl
Called Function		��
Return				��������Ϣ
Author				��wy
Finish Time			��2015��5��23��
****************************************************************************/
ActionStatue PreflopAction(Hand_Cards hand_cards)
{
	uint16_t sklansky_val = 10;
	uint16_t randdata = 100;
	//�Լ���ע�����200�Ҷ���������1����ֱ������ 
	if(g_Money_Max > PREFLOP_MONEY_M && 1 < updata_AllOpponentNum)
	{
		return Fold;
	}
	
	if(PreflopCardsSkl(hand_cards, &sklansky_val) == OK)
	{
		randdata = RandData();
		
		//�ȼ�1
		if(1 == sklansky_val)
		{
			if(randdata < 50)
			{
				return Call; 
			}
			//��50%�ĸ��ʼ�ע 
			return Raise;
		}
		// �ȼ�2~3 
		if(sklansky_val <= 3) 
		{
			if(g_Money_Max <= PREFLOP_MONEY_N)//�Լ����Ƶȼ�2~3�����Լ���Ҫ��עС��120 
			{
				return Call;
			}
			else 
				return Fold;
		}
		
		//�ȼ�4 
		if(4 == sklansky_val) 
		{
			if(g_Money_Max <= PREFLOP_MONEY_K)//���Ƶȼ�4�����Լ���Ҫ��עС��80 
			{
				return Call;
			}
			else 
				return Fold;
		}
		//�ȼ�5~6 
		if(sklansky_val <= 6) 
		{                             //������С��3�����Լ���Ҫ��עС��80 
			if(3 > updata_AllOpponentNum && PREFLOP_MONEY_K >= g_Money_Max)
			{
				return Call;
			}
		}
		//�������ؿ��� 
		if(g_Money_Max <=PREFLOP_MONEY_H)//�Լ���Ҫ��ע��С�ڵ���40������һ��������� 
		{
			if(My_Hand_Card.card1.card_val > CardK || My_Hand_Card.card2.card_val > CardK)//��һ��A 
				return Call;
			if(My_Hand_Card.card1.card_val >= CardT && My_Hand_Card.card2.card_val >= CardT)//���Ŷ���10���� 
				return Call;
			if(My_Hand_Card.card1.card_val == My_Hand_Card.card2.card_val)//һ�� 
				return Call;
			if(My_Hand_Card.card1.card_val > CardJ  || My_Hand_Card.card2.card_val > CardJ )
			{
				if(My_Hand_Card.card1.color == My_Hand_Card.card2.color)//һ��11����������ͬ��ɫ 
					return Call;
			}
		}		
		//�Լ���äע�����Լ���Ҫ��ע��Ϊ��ʱѡ���ƣ������ٵĳ��뿴���ĵ��ƣ� 
		if(Blind == g_own_last_action)
		{
			if(g_Money_Max == 0)
			{
				return Check;
			}
		}		
		return Fold;
	}
	else
	{
		//�ڲ���������
		return Fold;
	}
}

/****************************************************************************
Function Name		��ActionForWinPro
Function Describe	�����ݻ�ʤ���ʾ��߶���
In					��winpro ��ʤ����
					  action_pro ��������
Out					����
Call Function		����
Called Function		��
Return				��������Ϣ
Author				��
Finish Time			��2015��5��23��
****************************************************************************/
ActionStatue ActionForWinPro(float_t winpro, ActionProbability action_pro)
{
	uint16_t randdata = 100;
	g_winpro =  winpro; 
	
	randdata = RandData();
//printf("ȡʤ����: %f\n",winpro);
	//�������ڵ���0.9
	if(winpro == 1)
	{
		return All_In;
	}
	//�������ڵ���0.9
	if(winpro >= 0.9)
	{
		return Raise;
	}
	//0.8~0.9 
	if(winpro >= 0.8)
	{
		if(g_Money_Max >= 2*MONEY_M)//�Լ���Ҫ��ע�����400ʱ������ 
		{
			return Fold; 
		}
		return Call;
	}
	
	//0.7~0.8 
	if(winpro >= 0.7)
	{
		if(g_Money_Max >= MONEY_M)//�Լ���Ҫ��ע�����400ʱ������ 
		{
			return Fold; 
		}
		return Call;
	}

	//0.6~0.7 
	if(winpro >= 0.6)
	{
		if(g_Money_Max >= MONEY_N)//�Լ���Ҫ��ע�����320ʱ������ 
		{
			return Fold; 
		}
		return Call;
	}
	//0.5~0.6 
	if(winpro >= 0.5)
	{
		if(g_Money_Max >= MONEY_K)//�Լ���Ҫ��ע�����200ʱ������ 
		{
			return Fold; 
		}
		return Call;
	}
	//0.4~0.5 
	if(winpro >= 0.4)
	{
		if(g_Money_Max <= MONEY_H && updata_AllOpponentNum < 3)//�Լ���Ҫ��ע��С��120�Ҷ�����С��3����ע 
		{
			return Call; 
		}
  		//ǰ�涼����  �Լ�Ҳ����  ���÷ѽ�� 
		if(g_Money_Max == 0)
		{
			return Check; 
		}
		return Fold;
	}
	//����С��0.4
	//ǰ�涼����  �Լ�Ҳ����  ���÷ѽ�� 
	if(g_Money_Max == 0)
	{
		return Check; 
	}
	return Fold; 
}

/****************************************************************************
Function Name		��FlopAction
Function Describe	�����ƽ׶���ע����
In					��hand_cards ����
					  pflop_card ǰ���Ź����׵�ַ
					  OpponentNum δ���ƵĶ�����
Out					����
Call Function		��HandWinProbabilityThree
Called Function		��
Return				��������Ϣ
Author				��wy
Finish Time			��2015��5��23��
****************************************************************************/
ActionStatue FlopAction(Hand_Cards hand_cards, CardInfos *pflop_card, uint16_t OpponentNum)
{
	float_t winpro = 0;
	ActionProbability action_pro = {10, 50, 80, 85, 80, 95, 90, 5, 40};

	if(HandWinProbabilityThree(hand_cards, pflop_card, OpponentNum, &winpro) != OTHER)
	{
		return (ActionForWinPro(winpro, action_pro));
	}
	else
	{
		//�ڲ���������
		return Fold; 
	}
}

/****************************************************************************
Function Name		��TurnAction
Function Describe	��ת�ƽ׶���ע����
In					��hand_cards ����
					  pflop_card ǰ���Ź����׵�ַ
					  OpponentNum δ���ƵĶ�����
Out					����
Call Function		��HandWinProbabilityFour
Called Function		��
Return				��������Ϣ
Author				��wy
Finish Time			��2015��5��23��
****************************************************************************/
ActionStatue TurnAction(Hand_Cards hand_cards, CardInfos *pflop_card, uint16_t OpponentNum)
{
	float_t winpro = 0;
	ActionProbability action_pro = {30, 70, 85, 75, 85, 90, 85, 5, 30};

	if(HandWinProbabilityFour(hand_cards, pflop_card, OpponentNum, &winpro) != OTHER)
	{
		return (ActionForWinPro(winpro, action_pro));
	}
	else
	{
		//�ڲ���������
		return Fold; 
	}
}

/****************************************************************************
Function Name		��RiverAction
Function Describe	�����ƽ׶���ע����
In					��hand_cards ����
					  pflop_card ǰ���Ź����׵�ַ
					  OpponentNum δ���ƵĶ�����
Out					����
Call Function		��HandWinProbabilityFive
Called Function		��
Return				��������Ϣ
Author				��wy
Finish Time			��2015��5��23��
****************************************************************************/
ActionStatue RiverAction(Hand_Cards hand_cards, CardInfos *pflop_card, uint16_t OpponentNum)
{
	float_t winpro = 0;
	ActionProbability action_pro = {50, 90, 80, 70, 82, 85, 80, 2, 20};

	if(HandWinProbabilityFive(hand_cards, pflop_card, OpponentNum, &winpro) != OTHER)
	{
		return (ActionForWinPro(winpro, action_pro));
	}
	else
	{
		//�ڲ���������
		return Fold; 
	}
}

