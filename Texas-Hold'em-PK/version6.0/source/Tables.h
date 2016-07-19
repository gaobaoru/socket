/******************************************************************************

                  ��Ȩ���� (C), 2015-2015, OUC

 ******************************************************************************
  �� �� ��   : Tables.h
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2015��5��11��
  ����޸�   :
  ��������   : Tables.cpp ��ͷ�ļ�
******************************************************************************/
#include "Common.h"

#define FOURS_FH_T_TP_OP_NUM 4888
#define FLUSH_NUM 7937
#define SKLANSKY_NUM 83
#define BILL_NUM 15
#define CARDS_NUM 52

/*52���˿˵�32λ�����Ʊ�ʾ��(������Ϊ��4*��ֵ+��ɫ-9)*/
extern const uint32_t CardsInt32_Tables[CARDS_NUM];

/*��������Bill Chen��(������Ϊ��ֵ)*/
extern const float_t Bill_Tables[BILL_NUM];

/*��������Sklansky��*/
extern const uint32_t Sklansky_Tables[SKLANSKY_NUM];

/*���Ƶȼ�����Ԫ�ر�ʾ��Sklansky_Tables������ͬ�������͵ĵȼ���*/
extern const uint16_t Rank_Tables[SKLANSKY_NUM];

/* ��ֵ�����˻��� */
extern const uint32_t Products_Tables[FOURS_FH_T_TP_OP_NUM];

/* ��������«�����������Ժ�һ�Ե���������Ԫ�ر�ʾ��Products_Tables������ͬ�������͵������� */
extern const uint16_t Values_Tables[FOURS_FH_T_TP_OP_NUM];

/* ͬ�������� */
extern const uint16_t Flush_Tables[FLUSH_NUM];

