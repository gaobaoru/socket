/******************************************************************************

                  ��Ȩ���� (C), 2015-2015, OUC

 ******************************************************************************
  �� �� ��   : ���������㷨.h
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2015��5��10��
  ����޸�   :
  ��������   : ���������㷨.cpp ��ͷ�ļ�
******************************************************************************/
#include "Common.h"
#include "Frame.h"

extern CardInfos All_Cards_Table[52];

/****************************************************************************
Function Name		��CalculateIdx
Function Describe	�������CardsInt32_Tables�������
In					��card_infos �����˿���Ϣ
Out					����
Call Function		����
Called Function		��
Return				��0~52����ֵ
Author				��wy
Finish Time			��2015��5��15��
****************************************************************************/
uint16_t CalculateIdx(CardInfos card_infos);

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
CardKinds CardsIs_FS_F_S_H(uint32_t *pCards, uint16_t *pCardPower);

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
int16_t HalfFindMethod(const uint32_t val[],uint32_t value, uint32_t table_Len);

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
CardKinds CardsIs_FOUR_FH_T_TP_OP(uint32_t *pCards, uint16_t *pCardPower);

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
CardKinds CardsPower(uint32_t *pCards, uint16_t *pCardPower);

/****************************************************************************
Function Name		��OptimalPowerInt32
Function Describe	���������5����ʱ����������(����32λ�Ķ�������)
In					��pCards ����˿����ݵ��׵�ַ
					  CardsNum �˿˵�����(ȡֵΪ6����7)
Out					��pCardPower ������*pCardPower��ȡֵ��ΧΪ��1~7462��
Call Function		��CardsPower
Called Functions	��HandPotential(���Ƕ��ⲿ�ļ��Ľӿڣ����ڸ��ļ���ʹ��)
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
static CardKinds OptimalPowerInt32(uint32_t *pCards, uint16_t CardsNum, uint16_t *pCardPower);

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
ReturnStatus PreflopCardsSkl(Hand_Cards Cards, uint16_t *Sklansky_Val);

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
CardKinds HandWinProbabilityThree(Hand_Cards Cards, CardInfos *pflopcards, uint16_t OpponentNum, float_t *pWinPro);

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
CardKinds HandWinProbabilityFour(Hand_Cards Cards, CardInfos *pflopcards, uint16_t OpponentNum, float_t *pWinPro);

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
extern CardKinds HandWinProbabilityFive(Hand_Cards Cards, CardInfos *pflopcards, uint16_t OpponentNum, float_t *pWinPro);

