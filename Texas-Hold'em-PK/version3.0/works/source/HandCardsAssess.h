/******************************************************************************

                  版权所有 (C), 2015-2015, OUC

 ******************************************************************************
  文 件 名   : 手牌评估算法.h
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 2015年5月10日
  最近修改   :
  功能描述   : 手牌评估算法.cpp 的头文件
******************************************************************************/
#include "Common.h"
#include "Frame.h"

extern CardInfos All_Cards_Table[52];

/****************************************************************************
Function Name		：CalculateIdx
Function Describe	：计算查CardsInt32_Tables表的索引
In					：card_infos 单张扑克信息
Out					：无
Call Function		：无
Called Function		：
Return				：0~52索引值
Author				：wy
Finish Time			：2015年5月15日
****************************************************************************/
uint16_t CalculateIdx(CardInfos card_infos);

/****************************************************************************
Function Name		：CardsIs_FS_F_S_H
Function Describe	：判断是否为同花顺、同花、顺子或高牌
In					：pCards 存放扑克数据的首地址(由调用着保证有效)
Out					：pCardPower 存放牌力的首地址(由调用着保证有效)
					（*pCardPower的取值范围为：1~10，323~1599，1600~1609,6186~7462或者0）
Call Function		：无
Called Function		：
Return				：STRAIGHT_FLUSH 同花顺（包括皇家同花顺）
					  FLUSH 同花
					  STRAIGHT 顺子
					  HIGHT_CARD 高牌
					  OTHER 其它
Author				：wy
Finish Time			：2015年5月11日
****************************************************************************/
CardKinds CardsIs_FS_F_S_H(uint32_t *pCards, uint16_t *pCardPower);

/****************************************************************************
Function Name		：HalfFindMethod
Function Describe	：在Products_Tables表中用二分查找法查找值对应的索引
In					：val 数据首地址
					  value 待查找的值
					  table_Len 表的长度
Out					：无
Call Function		：无
Called Function		：
Return				：成功：非负索引值
					  失败：-1
Author				：wy
Finish Time			：2015年5月13日
****************************************************************************/
int16_t HalfFindMethod(const uint32_t val[],uint32_t value, uint32_t table_Len);

/****************************************************************************
Function Name		：CardsIs_FOUR_FH_T_TP_OP
Function Describe	：判断是否为四条、葫芦、三条、两对或一对
In					：pCards 存放扑克数据的首地址(由调用着保证有效)						  
Out					：pCardPower 存放牌力的首地址(由调用着保证有效，*pCardPower的取值范围为：10~166,1610~3325或者0）
Call Function		：无
Called Function		：
Return				：FOUR 四条
					  FULL_HOUSE 葫芦
					  THREE 三条
					  TWO_PAIRS 两对
					  ONE_PAIR 一对
					  OTHER 其它
Author				：wy
Finish Time			：2015年5月13日
****************************************************************************/
CardKinds CardsIs_FOUR_FH_T_TP_OP(uint32_t *pCards, uint16_t *pCardPower);

/****************************************************************************
Function Name		：CardsPower
Function Describe	：计算5张牌的牌力
In					：pCards 存放扑克数据的首地址
Out					：pCardPower 牌力（*pCardPower的取值范围为：1~7462）
Call Function		：CardsIs_FS_F_S_H
					  CardsIs_FOUR_FH_T_TP_OP
Called Function		：
Return				：STRAIGHT_FLUSH 同花顺（包括皇家同花顺）
					  FLUSH 同花
					  STRAIGHT 顺子
					  HIGHT_CARD 高牌
					  FOUR 四条
					  FULL_HOUSE 葫芦
					  THREE 三条
					  TWO_PAIRS 两对
					  ONE_PAIR 一对
					  OTHER 其它
Author				：wy
Finish Time			：2015年5月13日
****************************************************************************/
CardKinds CardsPower(uint32_t *pCards, uint16_t *pCardPower);

/****************************************************************************
Function Name		：OptimalPowerInt32
Function Describe	：计算多于5张牌时的最优牌力(根据32位的二进制数)
In					：pCards 存放扑克数据的首地址
					  CardsNum 扑克的张数(取值为6或者7)
Out					：pCardPower 牌力（*pCardPower的取值范围为：1~7462）
Call Function		：CardsPower
Called Functions	：HandPotential(不是对外部文件的接口，仅在该文件内使用)
Return				：STRAIGHT_FLUSH 同花顺（包括皇家同花顺）
					  FLUSH 同花
					  STRAIGHT 顺子
					  HIGHT_CARD 高牌
					  FOUR 四条
					  FULL_HOUSE 葫芦
					  THREE 三条
					  TWO_PAIRS 两对
					  ONE_PAIR 一对
					  OTHER 其它
Author				：wy
Finish Time			：2015年5月13日
****************************************************************************/
static CardKinds OptimalPowerInt32(uint32_t *pCards, uint16_t CardsNum, uint16_t *pCardPower);

/****************************************************************************
Function Name		：PreflopCardsSkl
Function Describe	：Preflop阶段手牌Sklansky评估
In					：Cards 手牌			  
Out					：Sklansky_Val 等级地址（*Sklansky_Val的取值范围为：1~9）
Call Function		：HalfFindMethod
Called Function		：
Return				：ERR  失败
					  OK   成功
Author				：wy
Finish Time			：2015年5月14日
****************************************************************************/
ReturnStatus PreflopCardsSkl(Hand_Cards Cards, uint16_t *Sklansky_Val);

/****************************************************************************
Function Name		：HandWinProbabilityThree
Function Describe	：发出三张公共牌之后手牌获胜概率计算
In					：Cards 手牌	
					  pflopcards 存放公共牌的首地址
					  OpponentNum 对手数
Out					：pWinPro 存放获胜概率的地址(取值范围0~1)
Call Function		：CardsPower
Called Function		：
Return				：牌的类型
Author				：wy
Finish Time			：2015年5月15日
****************************************************************************/
CardKinds HandWinProbabilityThree(Hand_Cards Cards, CardInfos *pflopcards, uint16_t OpponentNum, float_t *pWinPro);

/****************************************************************************
Function Name		：HandWinProbabilityFour
Function Describe	：发出四张公共牌之后手牌获胜概率计算
In					：Cards 手牌	
					  pflopcards 存放公共牌的首地址
					  OpponentNum 对手数
Out					：pWinPro 存放获胜概率的地址(取值范围0~1)
Call Function		：OptimalPowerInt32
Called Function		：
Return				：最优牌型
Author				：wy
Finish Time			：2015年5月23日
****************************************************************************/
CardKinds HandWinProbabilityFour(Hand_Cards Cards, CardInfos *pflopcards, uint16_t OpponentNum, float_t *pWinPro);

/****************************************************************************
Function Name		：HandWinProbabilityFive
Function Describe	：发出五张公共牌之后手牌获胜概率计算
In					：Cards 手牌	
					  pflopcards 存放公共牌的首地址
					  OpponentNum 对手数
Out					：pWinPro 存放获胜概率的地址(取值范围0~1)
Call Function		：无
Called Function		：
Return				：最优牌型
Author				：wy
Finish Time			：2015年5月23日
****************************************************************************/
extern CardKinds HandWinProbabilityFive(Hand_Cards Cards, CardInfos *pflopcards, uint16_t OpponentNum, float_t *pWinPro);

