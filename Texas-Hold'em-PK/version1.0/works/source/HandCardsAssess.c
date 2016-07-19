/******************************************************************************

                  ��Ȩ���� (C), 2015-2015, OUC

 ******************************************************************************
  �� �� ��   : ���������㷨.h
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2015��5��10��
  ����޸�   :
  ��������   : ���������㷨.cpp �ļ�
******************************************************************************/
#include <stdio.h>
#include "HandCardsAssess.h"
#include "Tables.h"

CardInfos All_Cards_Table[CARDS_NUM]=
{
	{Card2_prime,Spade,Card2},{Card2_prime,Heart,Card2},{Card2_prime,Diamond,Card2},{Card2_prime,Club,Card2},
	{Card3_prime,Spade,Card3},{Card3_prime,Heart,Card3},{Card3_prime,Diamond,Card3},{Card3_prime,Club,Card3},
	{Card4_prime,Spade,Card4},{Card4_prime,Heart,Card4},{Card4_prime,Diamond,Card4},{Card4_prime,Club,Card4},
	{Card5_prime,Spade,Card5},{Card5_prime,Heart,Card5},{Card5_prime,Diamond,Card5},{Card5_prime,Club,Card5},
	{Card6_prime,Spade,Card6},{Card6_prime,Heart,Card6},{Card6_prime,Diamond,Card6},{Card6_prime,Club,Card6},
	{Card7_prime,Spade,Card7},{Card7_prime,Heart,Card7},{Card7_prime,Diamond,Card7},{Card7_prime,Club,Card7},
	{Card8_prime,Spade,Card8},{Card8_prime,Heart,Card8},{Card8_prime,Diamond,Card8},{Card8_prime,Club,Card8},
	{Card9_prime,Spade,Card9},{Card9_prime,Heart,Card9},{Card9_prime,Diamond,Card9},{Card9_prime,Club,Card9},
	{CardT_prime,Spade,CardT},{CardT_prime,Heart,CardT},{CardT_prime,Diamond,CardT},{CardT_prime,Club,CardT},
	{CardJ_prime,Spade,CardJ},{CardJ_prime,Heart,CardJ},{CardJ_prime,Diamond,CardJ},{CardJ_prime,Club,CardJ},
	{CardQ_prime,Spade,CardQ},{CardQ_prime,Heart,CardQ},{CardQ_prime,Diamond,CardQ},{CardQ_prime,Club,CardQ},
	{CardK_prime,Spade,CardK},{CardK_prime,Heart,CardK},{CardK_prime,Diamond,CardK},{CardK_prime,Club,CardK},
	{CardA_prime,Spade,CardA},{CardA_prime,Heart,CardA},{CardA_prime,Diamond,CardA},{CardA_prime,Club,CardA}
};

/****************************************************************************
Function Name		��CalculateIdx
Function Describe	�������CardsInt32_Tables�������
In					��card_infos �����˿���Ϣ
Out					����
Call Function		����
Called Function		��
Return				�� 0~52����ֵ
Author				��wy
Finish Time			��2015��5��15��
****************************************************************************/
uint16_t CalculateIdx(CardInfos card_infos)
{
	uint16_t idx = 0;

	idx = (4 * card_infos.card_val + card_infos.color - 9);
	if(idx > 51)
	{
		idx = 51;
	}

	return idx;
}

/****************************************************************************
Function Name		��CardsIs_FS_F_S_H
Function Describe	���ж��Ƿ�Ϊͬ��˳��ͬ����˳�ӻ����
In					��pCards ����˿����ݵ��׵�ַ(�ɵ����ű�֤��Ч)
Out					��pCardPower ����������׵�ַ(�ɵ����ű�֤��Ч)
					��*pCardPower��ȡֵ��ΧΪ��1~10��323~1599��1600~1609,6186~7462����0��
Call Function		����
Called Function		��
Return				��STRAIGHT_FLUSH ͬ��˳�������ʼ�ͬ��˳��
					  FLUSH ͬ��
					  STRAIGHT ˳��
					  HIGHT_CARD ����
					  OTHER ����
Author				��wy
Finish Time			��2015��5��11��
****************************************************************************/
CardKinds CardsIs_FS_F_S_H(uint32_t *pCards, uint16_t *pCardPower)
{
	uint16_t i;
	uint16_t card_power = 0; 
	uint32_t color = 0;
	int isflush = 0;

	color = pCards[0] & pCards[1] & pCards[2] & pCards[3] & pCards[4] & 0xf000;
	if(0 != color)
	{
		isflush = 1;
	}
	/*��������*/
	i = (pCards[0]|pCards[1]|pCards[2]|pCards[3]|pCards[4])>>16;
	card_power = Flush_Tables[i];
	/*����*/
	if(0 == card_power)
	{
		*pCardPower = 0;
		return OTHER;
	}
	/*����*/
	if((0 == isflush) && (card_power > 10))
	{
		//ͬ������ֵ��ͬ���ȸ��ƴ�5863
		*pCardPower = card_power + 5863;
		return HIGHT_CARD;
	}
	/*˳��*/
	if((0 == isflush) && (card_power <= 10))
	{
		//ͬ������ֵ��ͬ��˳��˳�Ӵ�1599
		*pCardPower = card_power + 1599;
		return STRAIGHT;
	}
	/*ͬ��*/
	if((1 == isflush) && (card_power > 10))
	{
		*pCardPower = card_power;
		return FLUSH;
	}
	/*ͬ��˳*/
	if((1 == isflush) && (card_power <= 10))
	{
		*pCardPower = card_power;
		return STRAIGHT_FLUSH;
	}
}

/****************************************************************************
Function Name		��HalfFindMethod
Function Describe	����Products_Tables�����ö��ֲ��ҷ�����ֵ��Ӧ������
In					��val �����׵�ַ
					  value �����ҵ�ֵ
					  table_Len ��ĳ���
Out					����
Call Function		����
Called Function		��
Return				���ɹ����Ǹ�����ֵ
					  ʧ�ܣ�-1
Author				��wy
Finish Time			��2015��5��13��
****************************************************************************/
int16_t HalfFindMethod(const uint32_t val[],uint32_t value, uint32_t table_Len)
{
	int16_t start = 0;
	int16_t end = table_Len - 1;
	int16_t mid = (start + end) / 2;

	if(NULL == val)
	{
		return -1;
	}

	while((val[mid] != value) && (start < end))
	{
		if (val[mid] > value)
		{
			end = mid - 1;
		}
		else if (val[mid] < value)
		{
			start = mid + 1;
		}
		mid = ( start + end ) / 2; 
	}
	if (val[mid] == value)
	{
		return mid;
	}
	else 
	{
		return -1;
	}
}

/****************************************************************************
Function Name		��CardsIs_FOUR_FH_T_TP_OP
Function Describe	���ж��Ƿ�Ϊ��������«�����������Ի�һ��
In					��pCards ����˿����ݵ��׵�ַ(�ɵ����ű�֤��Ч)					  
Out					��pCardPower ����������׵�ַ(�ɵ����ű�֤��Ч��*pCardPower��ȡֵ��ΧΪ��10~166,1610~3325����0��
Call Function		����
Called Function		��
Return				��FOUR ����
					  FULL_HOUSE ��«
					  THREE ����
					  TWO_PAIRS ����
					  ONE_PAIR һ��
					  OTHER ����
Author				��wy
Finish Time			��2015��5��13��
****************************************************************************/
CardKinds CardsIs_FOUR_FH_T_TP_OP(uint32_t *pCards, uint16_t *pCardPower)
{
	uint32_t Prime;
	uint32_t v = 0xff;
	int16_t idx;
	uint32_t power;

	/*��������*/
	Prime = (pCards[0] & v) * (pCards[1] & v) * (pCards[2] & v) * (pCards[3] & v) * (pCards[4] & v);
	idx = HalfFindMethod(Products_Tables,Prime,FOURS_FH_T_TP_OP_NUM);
	/*��������*/
	if(-1 == idx)
	{
		*pCardPower = 0;
		return OTHER;
	}
	power = Values_Tables[idx];
	*pCardPower = power;
	/*����*/
	if(power <= 166)
	{
		return FOUR;
	}
	/*��«*/
	if(power <= 322)
	{
		return FULL_HOUSE;
	}
	/*����*/
	if(power <= 2467)
	{
		return THREE;
	}
	/*����*/
	if(power <= 3325)
	{
		return TWO_PAIRS;
	}
	/*һ��*/
	if(power <= 6185)
	{
		return ONE_PAIR;
	}
}

/****************************************************************************
Function Name		��CardsPower
Function Describe	������5���Ƶ�����
In					��pCards ����˿����ݵ��׵�ַ
Out					��pCardPower ������*pCardPower��ȡֵ��ΧΪ��1~7462��
Call Function		��CardsIs_FS_F_S_H
					  CardsIs_FOUR_FH_T_TP_OP
Called Function		��
Return				��STRAIGHT_FLUSH ͬ��˳�������ʼ�ͬ��˳��
					  FLUSH ͬ��
					  STRAIGHT ˳��
					  HIGHT_CARD ����
					  FOUR ����
					  FULL_HOUSE ��«
					  THREE ����
					  TWO_PAIRS ����
					  ONE_PAIR һ��
					  OTHER ����
Author				��wy
Finish Time			��2015��5��13��
****************************************************************************/
CardKinds CardsPower(uint32_t *pCards, uint16_t *pCardPower)
{
	CardKinds ck;
	uint16_t Card_Power = 0;

	/*�����Ϸ��Լ��*/
	if(NULL == pCards || NULL == pCardPower)
	{
		return OTHER;
	}

	ck = CardsIs_FS_F_S_H(pCards, &Card_Power);
	if(OTHER == ck)
	{
		ck = CardsIs_FOUR_FH_T_TP_OP(pCards, &Card_Power);
		
	}

	*pCardPower = Card_Power;
	return ck;
}

/****************************************************************************
Function Name		��OptimalPowerInt32
Function Describe	���������5����ʱ����������(����32λ�Ķ�������)
In					��pCards ����˿����ݵ��׵�ַ
					  CardsNum �˿˵�����(ȡֵΪ6����7)
Out					��pCardPower ������*pCardPower��ȡֵ��ΧΪ��1~7462��
Call Function		��CardsPower
Called Function		��HandPotential(���Ƕ��ⲿ�ļ��Ľӿڣ����ڸ��ļ���ʹ��)
Return				��STRAIGHT_FLUSH ͬ��˳�������ʼ�ͬ��˳��
					  FLUSH ͬ��
					  STRAIGHT ˳��
					  HIGHT_CARD ����
					  FOUR ����
					  FULL_HOUSE ��«
					  THREE ����
					  TWO_PAIRS ����
					  ONE_PAIR һ��
					  OTHER ����
Author				��wy
Finish Time			��2015��5��13��
****************************************************************************/
static CardKinds OptimalPowerInt32(uint32_t *pCards, uint16_t CardsNum, uint16_t *pCardPower)
{
	uint16_t opt_power = CARD_POWER_NUM;
	uint16_t card_power = 0;
	CardKinds card_kind;
	CardKinds opt_card_kind;
	uint16_t i,j,k,idx=0;
	uint32_t cards_table[5];

	if(NULL == pCards || NULL == pCardPower || CardsNum <= 5 || CardsNum > 7)
	{
		return OTHER;
	}

	if(7 == CardsNum)
	{
		for(i=0; i<CardsNum; i++)
		{
			for(j=i+1; j<CardsNum; j++)
			{
				idx=0;
				for(k=0; k<CardsNum; k++)
				{
					if(k != i && k != j)
					{
						cards_table[idx] = pCards[k];
						idx++;
					}
				}

				card_kind = CardsPower(cards_table, &card_power);
				if(card_power < opt_power)
				{
					opt_power = card_power;
					opt_card_kind = card_kind;
				}
			}
		}//End for
	}
	else 
	{
		for(i=0; i<CardsNum; i++)
		{
			idx=0;
			for(k=0; k<CardsNum; k++)
			{
				if(k != i)
				{
					cards_table[idx] = pCards[k];
					idx++;
				}
			}

			card_kind = CardsPower(cards_table, &card_power);
			if(card_power < opt_power)
			{
				opt_power = card_power;
				opt_card_kind = card_kind;
			}

		}//End for
	}


	*pCardPower = opt_power;
	return opt_card_kind;
}
/****************************************************************************
Function Name		��PreflopCardsSkl
Function Describe	��Preflop�׶�����Sklansky����
In					��Cards ����			  
Out					��Sklansky_Val �ȼ���ַ��*Sklansky_Val��ȡֵ��ΧΪ��1~9��
Call Function		��HalfFindMethod
Called Function		��
Return				��ERR  ʧ��
					  OK   �ɹ�
Author				��wy
Finish Time			��2015��5��14��
****************************************************************************/
ReturnStatus PreflopCardsSkl(Hand_Cards Cards, uint16_t *Sklansky_Val)
{
	int16_t idx;
	uint32_t value;

	/*����������*/
	if(NULL == Sklansky_Val)
	{
		return ERR;
	}
	/*���ͼ���*/
	if(Cards.card1.color == Cards.card2.color)
	{
		value = Cards.card1.prime * Cards.card2.prime;
	}
	else
	{
		//11�������ı��Ӧ
		value = (Cards.card1.card_val + 11) * (Cards.card2.card_val + 11);
	}
	/*���ֲ���,����ȼ�*/
	idx = HalfFindMethod(Sklansky_Tables,value,SKLANSKY_NUM);
	if(-1 == idx)
	{
		*Sklansky_Val = 9;
	}
	else
	{
		*Sklansky_Val = Rank_Tables[idx];
	}
	return OK;
}

/****************************************************************************
Function Name		��HandWinProbabilityThree
Function Describe	���������Ź�����֮�����ƻ�ʤ���ʼ���
In					��Cards ����	
					  pflopcards ��Ź����Ƶ��׵�ַ
					  OpponentNum ������
Out					��pWinPro ��Ż�ʤ���ʵĵ�ַ(ȡֵ��Χ0~1)
Call Function		��CardsPower
Called Function		��
Return				���Ƶ�����
Author				��wy
Finish Time			��2015��5��15��
****************************************************************************/
CardKinds HandWinProbabilityThree(Hand_Cards Cards, CardInfos *pflopcards, uint16_t OpponentNum, float_t *pWinPro)
{
	float_t WinPro;
	uint16_t own_power_rank;
	uint16_t other_power_rank;
	uint16_t lose_power_num = 0;
	uint16_t idx[5];
	uint32_t card_int32[5];
	uint32_t other_card_int32[5];
	CardKinds ck;
	uint16_t i,j;

	/*��������Ϸ��Լ��*/
	if(NULL == pflopcards || NULL == pWinPro)
	{
		return OTHER;
	}
	/*��������*/
	idx[0] = CalculateIdx(Cards.card1);
	idx[1] = CalculateIdx(Cards.card2);
	idx[2] = CalculateIdx(pflopcards[0]);
	idx[3] = CalculateIdx(pflopcards[1]);
	idx[4] = CalculateIdx(pflopcards[2]);
	/*�Լ���������*/
	card_int32[0] = CardsInt32_Tables[idx[0]];
	card_int32[1] = CardsInt32_Tables[idx[1]];
	card_int32[2] = CardsInt32_Tables[idx[2]];
	card_int32[3] = CardsInt32_Tables[idx[3]];
	card_int32[4] = CardsInt32_Tables[idx[4]];
	ck = CardsPower(card_int32, &own_power_rank);
	/*�������������Լ�����������*/
	other_card_int32[2] = card_int32[2];
	other_card_int32[3] = card_int32[3];
	other_card_int32[4] = card_int32[4];
	for(i=0; i<CARDS_NUM; i++)
	{
		if(CardsInt32_Tables[i] != card_int32[0] && CardsInt32_Tables[i] != card_int32[1]
		  && CardsInt32_Tables[i] != card_int32[2] && CardsInt32_Tables[i] != card_int32[3]
		  && CardsInt32_Tables[i] != card_int32[4])
		  {
			  other_card_int32[0] = CardsInt32_Tables[i];
			  for(j=i+1; j<CARDS_NUM; j++)
			  {
				  if(CardsInt32_Tables[j] != card_int32[0] && CardsInt32_Tables[j] != card_int32[1]
				  && CardsInt32_Tables[j] != card_int32[2] && CardsInt32_Tables[j] != card_int32[3]
				  && CardsInt32_Tables[j] != card_int32[4])
				  {
					  other_card_int32[1] = CardsInt32_Tables[j];
					  CardsPower(other_card_int32, &other_power_rank);
					  if(other_power_rank > own_power_rank)
					  {
						  lose_power_num++;
					  }
				  }
			  }
		  }
	}
	/*��ʤ���ʼ���(52���Ƴ�ȥ5�ź���ȡ���Ż���1081�����)*/
	WinPro = (float_t)lose_power_num / 1081;
	*pWinPro = 1;
	for(i=0; i<OpponentNum; i++)
	{
		(*pWinPro) *= WinPro;
	}

	return ck;
}


/****************************************************************************
Function Name		��HandWinProbabilityFour
Function Describe	���������Ź�����֮�����ƻ�ʤ���ʼ���
In					��Cards ����	
					  pflopcards ��Ź����Ƶ��׵�ַ
					  OpponentNum ������
Out					��pWinPro ��Ż�ʤ���ʵĵ�ַ(ȡֵ��Χ0~1)
Call Function		��OptimalPowerInt32
Called Function		��
Return				����������
Author				��wy
Finish Time			��2015��5��23��
****************************************************************************/
CardKinds HandWinProbabilityFour(Hand_Cards Cards, CardInfos *pflopcards, uint16_t OpponentNum, float_t *pWinPro)
{
	float_t WinPro = 0;
	uint16_t own_power_rank = 0;
	uint16_t other_power_rank = 0;
	uint16_t lose_power_num = 0;
	uint16_t idx[6];
	uint32_t card_int32[6];
	uint32_t other_card_int32[6];
	CardKinds ck;
	uint16_t i,j;

	/*��������Ϸ��Լ��*/
	if(NULL == pflopcards || NULL == pWinPro)
	{
		return OTHER;
	}
	/*��������*/
	idx[0] = CalculateIdx(Cards.card1);
	idx[1] = CalculateIdx(Cards.card2);
	idx[2] = CalculateIdx(pflopcards[0]);
	idx[3] = CalculateIdx(pflopcards[1]);
	idx[4] = CalculateIdx(pflopcards[2]);
	idx[5] = CalculateIdx(pflopcards[3]);
	/*�Լ���������*/
	card_int32[0] = CardsInt32_Tables[idx[0]];
	card_int32[1] = CardsInt32_Tables[idx[1]];
	card_int32[2] = CardsInt32_Tables[idx[2]];
	card_int32[3] = CardsInt32_Tables[idx[3]];
	card_int32[4] = CardsInt32_Tables[idx[4]];
	card_int32[5] = CardsInt32_Tables[idx[5]];
	ck = OptimalPowerInt32(card_int32, 6, &own_power_rank);
	/*�������������Լ�����������*/
	other_card_int32[2] = card_int32[2];
	other_card_int32[3] = card_int32[3];
	other_card_int32[4] = card_int32[4];
	other_card_int32[5] = card_int32[5];
	for(i=0; i<CARDS_NUM; i++)
	{
		if(CardsInt32_Tables[i] != card_int32[0] && CardsInt32_Tables[i] != card_int32[1]
		  && CardsInt32_Tables[i] != card_int32[2] && CardsInt32_Tables[i] != card_int32[3]
		  && CardsInt32_Tables[i] != card_int32[4] && CardsInt32_Tables[i] != card_int32[5])
		  {
			  other_card_int32[0] = CardsInt32_Tables[i];
			  for(j=i+1; j<CARDS_NUM; j++)
			  {
				  if(CardsInt32_Tables[j] != card_int32[0] && CardsInt32_Tables[j] != card_int32[1]
				  && CardsInt32_Tables[j] != card_int32[2] && CardsInt32_Tables[j] != card_int32[3]
				  && CardsInt32_Tables[j] != card_int32[4] && CardsInt32_Tables[j] != card_int32[5])
				  {
					  other_card_int32[1] = CardsInt32_Tables[j];

					  OptimalPowerInt32(other_card_int32, 6, &other_power_rank);
					  if(other_power_rank > own_power_rank)
					  {
						  lose_power_num++;
					  }
				  }
			  }
		  }
	}
	/*��ʤ���ʼ���(52���Ƴ�ȥ6�ź���ȡ���Ż���1035�����)*/
	WinPro = (float_t)lose_power_num / 1035;
	*pWinPro = 1;
	for(i=0; i<OpponentNum; i++)
	{
		(*pWinPro) *= WinPro;
	}

	return ck;
}

/****************************************************************************
Function Name		��HandWinProbabilityFive
Function Describe	���������Ź�����֮�����ƻ�ʤ���ʼ���
In					��Cards ����	
					  pflopcards ��Ź����Ƶ��׵�ַ
					  OpponentNum ������
Out					��pWinPro ��Ż�ʤ���ʵĵ�ַ(ȡֵ��Χ0~1)
Call Function		����
Called Function		��
Return				����������
Author				��wy
Finish Time			��2015��5��23��
****************************************************************************/
CardKinds HandWinProbabilityFive(Hand_Cards Cards, CardInfos *pflopcards, uint16_t OpponentNum, float_t *pWinPro)
{
	float_t WinPro = 0;
	uint16_t own_power_rank = 0;
	uint16_t other_power_rank = 0;
	uint16_t lose_power_num = 0;
	uint16_t idx[7];
	uint32_t card_int32[7];
	uint32_t other_card_int32[7];
	CardKinds ck;
	uint16_t i,j;

	/*��������Ϸ��Լ��*/
	if(NULL == pflopcards || NULL == pWinPro)
	{
		return OTHER;
	}
	/*��������*/
	idx[0] = CalculateIdx(Cards.card1);
	idx[1] = CalculateIdx(Cards.card2);
	idx[2] = CalculateIdx(pflopcards[0]);
	idx[3] = CalculateIdx(pflopcards[1]);
	idx[4] = CalculateIdx(pflopcards[2]);
	idx[5] = CalculateIdx(pflopcards[3]);
	idx[6] = CalculateIdx(pflopcards[4]);
	/*�Լ���������*/
	card_int32[0] = CardsInt32_Tables[idx[0]];
	card_int32[1] = CardsInt32_Tables[idx[1]];
	card_int32[2] = CardsInt32_Tables[idx[2]];
	card_int32[3] = CardsInt32_Tables[idx[3]];
	card_int32[4] = CardsInt32_Tables[idx[4]];
	card_int32[5] = CardsInt32_Tables[idx[5]];
	card_int32[6] = CardsInt32_Tables[idx[6]];
	ck = OptimalPowerInt32(card_int32, 7, &own_power_rank);
	/*�������������Լ�����������*/
	other_card_int32[2] = card_int32[2];
	other_card_int32[3] = card_int32[3];
	other_card_int32[4] = card_int32[4];
	other_card_int32[5] = card_int32[5];
	other_card_int32[6] = card_int32[6];
	for(i=0; i<CARDS_NUM; i++)
	{
		if(CardsInt32_Tables[i] != card_int32[0] && CardsInt32_Tables[i] != card_int32[1]
		  && CardsInt32_Tables[i] != card_int32[2] && CardsInt32_Tables[i] != card_int32[3]
		  && CardsInt32_Tables[i] != card_int32[4] && CardsInt32_Tables[i] != card_int32[5]
		  && CardsInt32_Tables[i] != card_int32[6])
		  {
			  other_card_int32[0] = CardsInt32_Tables[i];
			  for(j=i+1; j<CARDS_NUM; j++)
			  {
				  if(CardsInt32_Tables[j] != card_int32[0] && CardsInt32_Tables[j] != card_int32[1]
				  && CardsInt32_Tables[j] != card_int32[2] && CardsInt32_Tables[j] != card_int32[3]
				  && CardsInt32_Tables[j] != card_int32[4] && CardsInt32_Tables[j] != card_int32[5]
				  && CardsInt32_Tables[j] != card_int32[6])
				  {
					  other_card_int32[1] = CardsInt32_Tables[j];

					  OptimalPowerInt32(other_card_int32, 7, &other_power_rank);
					  if(other_power_rank < own_power_rank)
					  {
						  lose_power_num++;
					  }
				  }
			  }
		  }
	}
	/*��ʤ���ʼ���(52���Ƴ�ȥ7�ź���ȡ���Ż���990�����)*/
	WinPro = 1.0 - (float_t)lose_power_num / 990;
	*pWinPro = 1;
	for(i=0; i<OpponentNum; i++)
	{
		(*pWinPro) *= WinPro;
	}

	return ck;
}
