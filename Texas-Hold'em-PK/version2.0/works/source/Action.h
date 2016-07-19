/******************************************************************************

                  版权所有 (C), 2015-2015, OUC

 ******************************************************************************
  文 件 名   : Action.h
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 2015年5月23日
  最近修改   :
  功能描述   : Action.cpp的头文件
******************************************************************************/

#include "HandCardsAssess.h"

extern uint16_t g_Money_Max;
extern uint16_t g_Rand_Data;
extern float_t g_winpro;
extern Hand_Cards My_Hand_Card;//手牌
extern uint16_t updata_AllOpponentNum;//更新对手数 

typedef struct
{
	uint16_t raise9call_pro;//牌力大于等于0.9且上家已加大注时，自己的跟注概率百分数
	uint16_t call9call_pro;//牌力大于等于0.9且上家没有加大注时，自己的跟注概率百分数
	uint16_t raise7call_pro;//牌力为[0.7，0.9)且上家已加大注时，自己的跟注概率百分数
	uint16_t call7call_pro;//牌力为[0.7，0.9)且上家没有加大注时，自己的跟注概率百分数
	uint16_t raise5call_pro;//牌力为[0.5，0.7)且上家已加大注时，自己的跟注概率百分数
	uint16_t raise5fold_pro;//牌力为[0.5，0.7)且上家已加大注时，自己的弃牌概率百分数
	uint16_t call5call_pro;//牌力为[0.5，0.7)且上家没有加大注时，自己的跟注概率百分数
	uint16_t raise0call_pro;//牌力小于0.5且上家已加大注时，自己的跟注概率百分数
	uint16_t call0call_pro;//牌力小于0.5且上家没有加大注时，自己的跟注概率百分数
}ActionProbability;
/****************************************************************************
Function Name		：RandData
Function Describe	：随机产生0到100间的随机数
In					：无
Out					：无
Call Function		：srand
					  rand
Called Function		：
Return				：0至100间的随机数
Author				：wy
Finish Time			：2015年5月23日
****************************************************************************/
uint16_t RandData(void);

/****************************************************************************
Function Name		：PreflopAction
Function Describe	：翻牌前下注策略
In					：hand_cards 手牌
Out					：无
Call Function		：PreflopCardsSkl
Called Function		：
Return				：动作信息
Author				：wy
Finish Time			：2015年5月23日
****************************************************************************/
ActionStatue PreflopAction(Hand_Cards hand_cards);

/****************************************************************************
Function Name		：ActionForWinPro
Function Describe	：根据获胜概率决策动作
In					：winpro 获胜概率
					  action_pro 动作概率
Out					：无
Call Function		：无
Called Function		：
Return				：动作信息
Author				：wy
Finish Time			：2015年5月23日
****************************************************************************/
ActionStatue ActionForWinPro(float_t winpro, ActionProbability action_pro);

/****************************************************************************
Function Name		：FlopAction
Function Describe	：翻牌阶段下注策略
In					：hand_cards 手牌
					  pflop_card 前三张公牌首地址
					  OpponentNum 未弃牌的对手数
Out					：无
Call Function		：HandWinProbabilityThree
Called Function		：
Return				：动作信息
Author				：wy
Finish Time			：2015年5月23日
****************************************************************************/
ActionStatue FlopAction(Hand_Cards hand_cards, CardInfos *pflop_card, uint16_t OpponentNum);

/****************************************************************************
Function Name		：TurnAction
Function Describe	：转牌阶段下注策略
In					：hand_cards 手牌
					  pflop_card 前四张公牌首地址
					  OpponentNum 未弃牌的对手数
Out					：无
Call Function		：HandWinProbabilityFour
Called Function		：
Return				：动作信息
Author				：wy
Finish Time			：2015年5月23日
****************************************************************************/
ActionStatue TurnAction(Hand_Cards hand_cards, CardInfos *pflop_card, uint16_t OpponentNum);

/****************************************************************************
Function Name		：RiverAction
Function Describe	：河牌阶段下注策略
In					：hand_cards 手牌
					  pflop_card 前五张公牌首地址
					  OpponentNum 未弃牌的对手数
Out					：无
Call Function		：HandWinProbabilityFive
Called Function		：
Return				：动作信息
Author				：wy
Finish Time			：2015年5月23日
****************************************************************************/
ActionStatue RiverAction(Hand_Cards hand_cards, CardInfos *pflop_card, uint16_t OpponentNum);
