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

//大盲注额 
#define BIG_BLIND 40

//盲注阶段下注阈值（myself） 
#define PREFLOP_MONEY_M 5*BIG_BLIND
#define PREFLOP_MONEY_N 3*BIG_BLIND
#define PREFLOP_MONEY_K 2*BIG_BLIND
#define PREFLOP_MONEY_H 1*BIG_BLIND

//翻牌、转牌、河牌阶段下注阈值（myself） 
#define MONEY_M 10*BIG_BLIND
#define MONEY_N 8*BIG_BLIND
#define MONEY_K 5*BIG_BLIND
#define MONEY_H 3*BIG_BLIND

/*
全局变量g_Money_Max为每局每个下注阶段自己需要的下注额 ，
它需要在询问消息中不断更新。
*/
uint16_t g_Money_Max = 0;

/*
全局变量g_own_last_action为上一次自己的动作消息，
在询问消息函数中被更新 
*/
ActionStatue g_own_last_action = Action_Null;

/*产生的随机数*/
uint16_t g_Rand_Data;

/*取胜概率*/
float_t g_winpro;

/*更新对手数 */
uint16_t updata_AllOpponentNum;

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
	//自己下注额大于200且对手数大于1个，直接弃牌 
	if(g_Money_Max > PREFLOP_MONEY_M && 1 < updata_AllOpponentNum)
	{
		return Fold;
	}
	
	if(PreflopCardsSkl(hand_cards, &sklansky_val) == OK)
	{
		randdata = RandData();
		
		//等级1
		if(1 == sklansky_val)
		{
			if(randdata < 50)
			{
				return Call; 
			}
			//以50%的概率加注 
			return Raise;
		}
		// 等级2~3 
		if(sklansky_val <= 3) 
		{
			if(g_Money_Max <= PREFLOP_MONEY_N)//自己手牌等级2~3，且自己需要下注小于120 
			{
				return Call;
			}
			else 
				return Fold;
		}
		
		//等级4 
		if(4 == sklansky_val) 
		{
			if(g_Money_Max <= PREFLOP_MONEY_K)//手牌等级4，且自己需要下注小于80 
			{
				return Call;
			}
			else 
				return Fold;
		}
		//等级5~6 
		if(sklansky_val <= 6) 
		{                             //对手数小于3，且自己需要下注小于80 
			if(3 > updata_AllOpponentNum && PREFLOP_MONEY_K >= g_Money_Max)
			{
				return Call;
			}
		}
		//其他因素考虑 
		if(g_Money_Max <=PREFLOP_MONEY_H)//自己需要下注额小于等于40，处理一下特殊情况 
		{
			if(My_Hand_Card.card1.card_val > CardK || My_Hand_Card.card2.card_val > CardK)//有一张A 
				return Call;
			if(My_Hand_Card.card1.card_val >= CardT && My_Hand_Card.card2.card_val >= CardT)//两张都在10以上 
				return Call;
			if(My_Hand_Card.card1.card_val == My_Hand_Card.card2.card_val)//一对 
				return Call;
			if(My_Hand_Card.card1.card_val > CardJ  || My_Hand_Card.card2.card_val > CardJ )
			{
				if(My_Hand_Card.card1.color == My_Hand_Card.card2.color)//一张11以上且两种同花色 
					return Call;
			}
		}		
		//自己是盲注，且自己需要下注额为零时选择看牌（用最少的筹码看最多的底牌） 
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
Author				：
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
	//0.8~0.9 
	if(winpro >= 0.8)
	{
		if(g_Money_Max >= 2*MONEY_M)//自己需要下注额大于400时，弃牌 
		{
			return Fold; 
		}
		return Call;
	}
	
	//0.7~0.8 
	if(winpro >= 0.7)
	{
		if(g_Money_Max >= MONEY_M)//自己需要下注额大于400时，弃牌 
		{
			return Fold; 
		}
		return Call;
	}

	//0.6~0.7 
	if(winpro >= 0.6)
	{
		if(g_Money_Max >= MONEY_N)//自己需要下注额大于320时，弃牌 
		{
			return Fold; 
		}
		return Call;
	}
	//0.5~0.6 
	if(winpro >= 0.5)
	{
		if(g_Money_Max >= MONEY_K)//自己需要下注额大于200时，弃牌 
		{
			return Fold; 
		}
		return Call;
	}
	//0.4~0.5 
	if(winpro >= 0.4)
	{
		if(g_Money_Max <= MONEY_H && updata_AllOpponentNum < 3)//自己需要下注额小于120且对手数小于3，跟注 
		{
			return Call; 
		}
  		//前面都让牌  自己也让牌  不用费金币 
		if(g_Money_Max == 0)
		{
			return Check; 
		}
		return Fold;
	}
	//牌力小于0.4
	//前面都让牌  自己也让牌  不用费金币 
	if(g_Money_Max == 0)
	{
		return Check; 
	}
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

