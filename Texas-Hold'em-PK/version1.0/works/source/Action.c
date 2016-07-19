/******************************************************************************

                  版权所有 (C), 2015-2015, OUC

 ******************************************************************************
  文 件 名   : Action.cpp
  版 本 号   : 初稿
  作    者   : GBR
  生成日期   : 2015年5月23日
  最近修改   :
  功能描述   : 产生决策动作
******************************************************************************/
#include <stdio.h>
#include "Action.h"
#include <stdlib.h>
#include <time.h>   

//盲注阈值
#define PREFLOP_MONEY_M 500
#define PREFLOP_MONEY_N 200
#define PREFLOP_MONEY_K 100
#define PREFLOP_MONEY_H 50

//翻牌、转牌、河牌阶段下注阈值
#define MONEY_M 500
#define MONEY_N 200
#define MONEY_K 100
#define MONEY_H 50

/*
全局变量g_Money_Max为每局每个下注阶段上家下注的最大值，它需要在询问消息中不断更新。
*/
uint16_t g_Money_Max = 0;
Hand_Cards My_Hand_Card;//手牌
/*产生的随机数*/
uint16_t g_Rand_Data;
/*取胜概率*/
float_t g_winpro;

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
uint16_t RandData(void)
{
	srand( (unsigned)time( NULL ) );          //使用当前时钟做种子初始化随机数
	g_Rand_Data = (rand() % 100);
    return g_Rand_Data;
}

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
ActionStatue PreflopAction(Hand_Cards hand_cards)
{
	uint16_t sklansky_val = 10;
	uint16_t randdata = 100;

	if(PreflopCardsSkl(hand_cards, &sklansky_val) == OK)
	{
		randdata = RandData();
		
		//等级1
		if(sklansky_val == 1)
		{
				if(My_Hand_Card.card1.card_val >= CardJ && My_Hand_Card.card2.card_val >= CardJ)
				{
					if(randdata < 50)
					{
						return Call; 
					}
					//以50%的概率加注 
					return Raise;
				}
				else 
					return Fold;
		}
		// 等级2~3 
		if(sklansky_val <= 3) 
		{
			if(g_Money_Max >= PREFLOP_MONEY_N)
			{
				//以5%的概率跟注
				if(randdata < 5)
				{
					return Raise; 
				}
				//以95%的概率弃牌
				return Call;
			}
			return Call;
		}
		//等级4 
		if(sklansky_val == 4) 
		{
			if(g_Money_Max >= PREFLOP_MONEY_K)
			{
				//以10%的概率弃牌 
				if(randdata < 10)
				{
					return Fold; 
				}
				//以95%的概率跟注 
				return Call;
			}
			return Call;
		}
		return Fold;
	}
	else
	{
		//内部错误，弃牌
		return Fold;
	}
}

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
ActionStatue ActionForWinPro(float_t winpro, ActionProbability action_pro)
{
	uint16_t randdata = 100;
	g_winpro =  winpro; 
	randdata = RandData();
//printf("取胜概率: %f\n",winpro);
	//牌力大于等于0.9
	if(winpro == 1)
	{
		return All_In;
	}
	//牌力大于等于0.9
	if(winpro >= 0.9)
	{
		return Raise;
	}
	//牌力大于0.8
	if(winpro >= 0.8)
	{
		return Call;
	}
	
	//牌力大于等于0.7
	if(winpro >= 0.7)
	{
		if(g_Money_Max >= MONEY_N)
		{
			//以action_pro.raise7call_pro%的概率跟注
			if(randdata < action_pro.raise7call_pro)
			{
				return Call; 
			}
			//以1-action_pro.raise7call_pro%的概率加注
			return Raise;
		}
		//以action_pro.call7call_pro%的概率跟注
		if(randdata < action_pro.call7call_pro)
		{
			return Call; 
		}
		//以1-action_pro.call7call_pro%的概率加注
		return Raise;
	}

	//牌力大于等于0.5
	if(winpro >= 0.5)
	{
		if(g_Money_Max >= MONEY_K)
		{
			//以action_pro.raise5call_pro%的概率跟注
			if(randdata < action_pro.raise5call_pro)
			{
				return Call; 
			}
			//以action_pro.raise5fold_pro%-action_pro.raise5call_pro%的概率弃牌
			if(randdata < action_pro.raise5fold_pro)
			{
				return Check; 
			}
			//以1-action_pro.raise5fold_pro%-action_pro.raise5call_pro%的概率加注
			return Raise;
		}
		//以action_pro.call5call_pro%的概率跟注
		if(randdata < action_pro.call5call_pro)
		{
			return Call; 
		}
		//以1-action_pro.call5call_pro%的概率加注
		return Raise;
	}

	//牌力小于0.5
	return Fold; 
}

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
		//内部错误，弃牌
		return Fold; 
	}
}

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
		//内部错误，弃牌
		return Fold; 
	}
}

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
		//内部错误，弃牌
		return Fold; 
	}
}

