 /******************************************************************************
  文 件 名   : card_decode.h
  版 本 号   : 初稿
  作    者   : GBR
  生成日期   : 2015年5月20日
  最近修改   :
  功能描述   : 数据结构定义
******************************************************************************/
#ifndef CARD_DECODE_H_INCLUDED
#define CARD_DECODE_H_INCLUDED

/*===================================================

  *	函数名	:card_color_decode
  
  *	功     能	:牌花色解析根据输入的字符串返回相应
  				 的指令代码。具体如下
  				 
  *	输     入	:char* str 指令字符串
  
  *	返回值	:对应指令代号(int型)
  
  *	对应关系
  					//	Spade = 1,//黑桃
					//	Hearts = 2,//红桃
					//	Diamond = 3,//方片
					//	Club = 4//草花
  					如果不是上诉命令则返回0

  					PS.	命令相差比较大
  					       仅比较前二个字符

  ===================================================*/
extern int card_color_decode(char* str);
/*===================================================
  *	函数名	:card_point_decode
  
  *	功     能	:牌花色解析根据输入的字符串返回相应
  				 的指令代码。具体如下
  				 
  *	输     入	:char* str 指令字符串
  
  *	返回值	:对应指令代号(int型)
  
  *	对应关系
  					//	Card2 = 2,//2
					//	Card3 = 3,//3
					//	Card4 = 4,//4
					//	Card5 = 5,//5
					//	Card6 = 6,//6
					//	Card7 = 7,//7
					//	Card8 = 8,//8
					//	Card9 = 9,//9
					//	CardT = 10,//T
					//	CardJ = 11,//J
					//	CardQ = 12,//Q
					//	CardK = 13,//K
					//	CardA = 14,//A
  					如果不是上诉命令则返回0

  					PS.	命令相差比较大
  					       仅比较前一个字符
  ===================================================*/
extern int card_point_decode(char* str);

/*===================================================
  *	函数名	:card_prime_decode
  
  *	功     能	:牌花色解析根据输入的字符串返回相应
  				 的质数代码。具体如下
  				 
  *	输     入	:char* str 指令字符串
  
  *	返回值	:对应指令代号(int型)
  
  *	对应关系
				 //	Card2_prime = 2,
				//	Card3_prime = 3,
				//	Card4_prime = 5,
				//	Card5_prime = 7,
				//	Card6_prime = 11,
				//	Card7_prime = 13,
				//	Card8_prime = 17,
				//	Card9_prime = 19,
				//	CardT_prime = 23,
				//	CardJ_prime = 29,
				//	CardQ_prime = 31,
				//	CardK_prime = 37,
				//	CardA_prime = 41,
  					如果不是上诉命令则返回0

  					PS.	命令相差比较大
  					       仅比较前一个字符
  ===================================================*/
extern int card_prime_decode(char* str);

/*===================================================

  *	函数名	:action_decode
  
  *	功     能	:动作解析根据输入的字符串返回相应
  				 的动作代码。具体如下
  				 
  *	输     入	:char* str 指令字符串
  
  *	返回值	:对应指令代号(int型)
  
  *	对应关系
  					Fold = 1,//弃牌
					Call = 2,//跟牌
					Raise = 3,//加注
					ALL_IN = 4,//叫注
					Check = 5//看牌
					blind = 6 //盲注 
  					如果不是上诉命令则返回0

  					PS.	命令相差比较大
  					       仅比较前二个字符

  ===================================================*/
extern int action_decode(char* str);

/*===================================================

  *	函数名	:nut_hand_decode
  
  *	功     能	:牌型解析根据输入的字符串返回相应
  				 的牌型代码。具体如下
  				 
  *	输     入	:char* str 指令字符串
  
  *	返回值	:对应指令代号(int型)
  
  *	对应关系
  					ROYAL_FLUSH = 1,//皇家同花顺
					STRAIGHT_FLUSH = 2,//同花顺
					FOUR = 3,//四条
					FULL_HOUSE = 4,//葫芦（三带一对）
					FLUSH = 5,//同花
					STRAIGHT = 6,//顺子
					THREE = 7,//三条
					TWO_PAIRS = 8,//两对
					ONE_PAIR = 9,//一对
					HIGHT_CARD = 10,//高牌
					//OTHER = 0//作函数返回值时用
  					如果不是上诉命令则返回0

  					PS.	命令相差比较大
  					       仅比较前二个字符

  ===================================================*/
extern int nut_hand_decode(char* str);
#endif // CARD_DECODE_H_INCLUDED

