 /******************************************************************************
  文 件 名   : card_decode.h
  版 本 号   : 初稿
  作    者   : GBR
  生成日期   : 2015年5月20日
  最近修改   :
  功能描述   : 数据结构定义
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "card_decode.h"
/*===================================================

  *	函数名	:card_color_decode
  
  *	功     能	:牌花色解析根据输入的字符串返回相应
  				 的花色代码。具体如下
  				 
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
int card_color_decode(char* str)
{
	
	switch(*str++)
	{
		case 'S':	//	SPADE/
			if(*str == 'P')
				return 1;
			break;
		case 'H':	//	HEARTS/
			if(*str == 'E')
				return 2;
			break;
		case 'D':	//	DIAMOND/
			if(*str == 'I')
				return 3;
			break;
		case 'C':	//	CLUBS/
			if(*str == 'L')
				return 4;
			break;
		default:
			return 0;
	}
	return 0;
}

/*===================================================
  *	函数名	:card_point_decode
  
  *	功     能	:牌花色解析根据输入的字符串返回相应
  				 的牌值代码。具体如下
  				 
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
int card_point_decode(char* str)
{
	
	switch(*str++)
	{
		case '2':	//	Card2/
				return 2;
			break;
		case '3':	//	Card3/
				return 3;
			break;
		case '4':	//	Card4/
				return 4;
			break;
		case '5':	//	Card5/
				return 5;
			break;
		case '6':	//	Card6/
				return 6;
			break;
		case '7':	//	Card7/
				return 7;
			break;
		case '8':	//	Card8/
				return 8;
			break;
		case '9':	//	Card9/
				return 9;
			break;
		case '1':	//	Card10/
			if(*str=='0')
				return 10;
			break;
		case 'J':	//	Card11/
				return 11;
			break;
		case 'Q':	//	Card12/
				return 12;
			break;
		case 'K':	//	Card13/
				return 13;
			break;
		case 'A':	//	Card14/
				return 14;
			break;
		default:
			return 0;
	}
	return 0;
}
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
int card_prime_decode(char* str)
{
	
	switch(*str++)
	{
		case '2':	//	Card2/
				return 2;
			break;
		case '3':	//	Card3/
				return 3;
			break;
		case '4':	//	Card4/
				return 5;
			break;
		case '5':	//	Card5/
				return 7;
			break;
		case '6':	//	Card6/
				return 11;
			break;
		case '7':	//	Card7/
				return 13;
			break;
		case '8':	//	Card8/
				return 17;
			break;
		case '9':	//	Card9/
				return 19;
			break;
		case '1':	//	Card10/
			if(*str=='0')
				return 23;
			break;
		case 'J':	//	Card11/
				return 29;
			break;
		case 'Q':	//	Card12/
				return 31;
			break;
		case 'K':	//	Card13/
				return 37;
			break;
		case 'A':	//	Card14/
				return 41;
			break;
		default:
			return 0;
	}
	return 0;
}

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
int action_decode(char* str)
{
	
	switch(*str++)
	{
		case 'f':	//	fold/
			if(*str == 'o')
				return 1;
			break;
		case 'c':	//	call/ 
			if(*str == 'a')
				return 2;
			else	//   check/
				return 5;
			break;
		case 'r':	//	raise/
			if(*str == 'a')
				return 3;
			break;
		case 'a':	//	all_in/
			if(*str == 'l')
				return 4;
			break;
		case 'b':	//	blind/
			if(*str == 'l')
				return 6;
			break;
		default:
			return 0;
	}
	return 0;
}


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
int nut_hand_decode(char* str)
{
	
	switch(*str++)
	{
		case 'R':	//	ROYAL_FLUSH/
			if(*str == 'O')
				return 1;
			break;
		case 'S':	//	STRAIGHT_FLUSH/       
			if(*(str+8)=='F')
				return 2;
			else	//STRAIGHT/
				return 6;
			break;
		case 'F':	//	FOUR/
			if(*str == 'O')
				return 3;
			else if(*str=='U')
				return 4;   //FULL_HOUSE/
			else
				return 5;   //FLUSH/
			break;
		case 'T':	//	THREE/
			if(*str == 'H')
				return 7;
			else	//	TWO_PAIRS/
				return 8;
			break;
		case 'O':	//	ONE_PAIR/
			if(*str == 'N')
				return 9;
			break;
		case 'H':	//	HIGHT_CARD/
			if(*str == 'I')
				return 10;
			break;
		default:
			return 0;
	}
	return 0;
}


