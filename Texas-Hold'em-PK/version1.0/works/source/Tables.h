/******************************************************************************

                  版权所有 (C), 2015-2015, OUC

 ******************************************************************************
  文 件 名   : Tables.h
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 2015年5月11日
  最近修改   :
  功能描述   : Tables.cpp 的头文件
******************************************************************************/
#include "Common.h"

#define FOURS_FH_T_TP_OP_NUM 4888
#define FLUSH_NUM 7937
#define SKLANSKY_NUM 83
#define BILL_NUM 15
#define CARDS_NUM 52

/*52张扑克的32位二进制表示表(其索引为：4*牌值+花色-9)*/
extern const uint32_t CardsInt32_Tables[CARDS_NUM];

/*手牌评估Bill Chen表(其索引为牌值)*/
extern const float_t Bill_Tables[BILL_NUM];

/*手牌评估Sklansky表*/
extern const uint32_t Sklansky_Tables[SKLANSKY_NUM];

/*手牌等级表（其元素表示与Sklansky_Tables表中相同索引牌型的等级）*/
extern const uint16_t Rank_Tables[SKLANSKY_NUM];

/* 牌值质数乘积表 */
extern const uint32_t Products_Tables[FOURS_FH_T_TP_OP_NUM];

/* 四条、葫芦、三条、两对和一对的牌力表（其元素表示与Products_Tables表中相同索引牌型的牌力） */
extern const uint16_t Values_Tables[FOURS_FH_T_TP_OP_NUM];

/* 同花牌力表 */
extern const uint16_t Flush_Tables[FLUSH_NUM];

