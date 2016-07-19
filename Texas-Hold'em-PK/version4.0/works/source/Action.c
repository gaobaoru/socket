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

//äע��ֵ
#define PREFLOP_MONEY_M 500
#define PREFLOP_MONEY_N 200
#define PREFLOP_MONEY_K 100
#define PREFLOP_MONEY_H 50

//���ơ�ת�ơ����ƽ׶���ע��ֵ
#define MONEY_M 500
#define MONEY_N 300
#define MONEY_K 200
#define MONEY_H 100

/*
ȫ�ֱ���g_Money_MaxΪÿ��ÿ����ע�׶��ϼҵ��������ע�� ������Ҫ��ѯ����Ϣ�в��ϸ��¡�
*/
uint16_t g_Money_Max = 0;

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
			if(g_Money_Max >= PREFLOP_MONEY_N)
			{
				//��5%�ĸ��ʸ�ע
				if(randdata < 5)
				{
					return Raise; 
				}
				//��95%�ĸ�������
				return Call;
			}
			return Call;
		}
		//�ȼ�4 
		if(4 == sklansky_val) 
		{
			if(g_Money_Max >= PREFLOP_MONEY_K)
			{
				//��10%�ĸ������� 
				if(randdata < 10)
				{
					return Fold; 
				}
				//��95%�ĸ��ʸ�ע 
				return Call;
			}
			return Call;
		}
		//�ȼ�5~6 
		if(sklansky_val <= 6) 
		{
			if(3 > updata_AllOpponentNum && PREFLOP_MONEY_K >= g_Money_Max)
			{
				return Call;
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
Author				��wy
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
	//��������0.8
	if(winpro >= 0.8)
	{
		return Call;
	}
	
	//�������ڵ���0.7
	if(winpro >= 0.7)
	{
		if(g_Money_Max <= MONEY_H && updata_AllOpponentNum >= 5)
		{
			return Call; 
		}
		if(g_Money_Max <= MONEY_K && updata_AllOpponentNum >= 3)
		{
			return Call; 
		}
		if(g_Money_Max <= MONEY_N && updata_AllOpponentNum >= 1)
		{
			return Call; 
		}
		return Fold;
	}

	//�������ڵ���0.6
	if(winpro >= 0.6)
	{
		if(g_Money_Max <= MONEY_K  && updata_AllOpponentNum < 3)
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
		//�������ڵ���0.5
	if(winpro >= 0.5)
	{
		if(g_Money_Max <= MONEY_H  && updata_AllOpponentNum < 3)
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
	//����С��0.5
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

