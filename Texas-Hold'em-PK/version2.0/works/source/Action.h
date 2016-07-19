/******************************************************************************

                  ��Ȩ���� (C), 2015-2015, OUC

 ******************************************************************************
  �� �� ��   : Action.h
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2015��5��23��
  ����޸�   :
  ��������   : Action.cpp��ͷ�ļ�
******************************************************************************/

#include "HandCardsAssess.h"

extern uint16_t g_Money_Max;
extern uint16_t g_Rand_Data;
extern float_t g_winpro;
extern Hand_Cards My_Hand_Card;//����
extern uint16_t updata_AllOpponentNum;//���¶����� 

typedef struct
{
	uint16_t raise9call_pro;//�������ڵ���0.9���ϼ��ѼӴ�עʱ���Լ��ĸ�ע���ʰٷ���
	uint16_t call9call_pro;//�������ڵ���0.9���ϼ�û�мӴ�עʱ���Լ��ĸ�ע���ʰٷ���
	uint16_t raise7call_pro;//����Ϊ[0.7��0.9)���ϼ��ѼӴ�עʱ���Լ��ĸ�ע���ʰٷ���
	uint16_t call7call_pro;//����Ϊ[0.7��0.9)���ϼ�û�мӴ�עʱ���Լ��ĸ�ע���ʰٷ���
	uint16_t raise5call_pro;//����Ϊ[0.5��0.7)���ϼ��ѼӴ�עʱ���Լ��ĸ�ע���ʰٷ���
	uint16_t raise5fold_pro;//����Ϊ[0.5��0.7)���ϼ��ѼӴ�עʱ���Լ������Ƹ��ʰٷ���
	uint16_t call5call_pro;//����Ϊ[0.5��0.7)���ϼ�û�мӴ�עʱ���Լ��ĸ�ע���ʰٷ���
	uint16_t raise0call_pro;//����С��0.5���ϼ��ѼӴ�עʱ���Լ��ĸ�ע���ʰٷ���
	uint16_t call0call_pro;//����С��0.5���ϼ�û�мӴ�עʱ���Լ��ĸ�ע���ʰٷ���
}ActionProbability;
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
uint16_t RandData(void);

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
ActionStatue PreflopAction(Hand_Cards hand_cards);

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
ActionStatue ActionForWinPro(float_t winpro, ActionProbability action_pro);

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
ActionStatue FlopAction(Hand_Cards hand_cards, CardInfos *pflop_card, uint16_t OpponentNum);

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
ActionStatue TurnAction(Hand_Cards hand_cards, CardInfos *pflop_card, uint16_t OpponentNum);

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
ActionStatue RiverAction(Hand_Cards hand_cards, CardInfos *pflop_card, uint16_t OpponentNum);
