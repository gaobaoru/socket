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
#include "command_decode.h"
/*===================================================

  *	函数名	:getline
  *	功     能	:从source 读取一行数据至target 并返回长度
  *	输     入	:const char* source	源字符串
  				          char* target	目标字符串
  *	返回值	:读取长度

  ===================================================*/
int get_line(const char* source,char* target)
{
	int i;
	for(i = 0;*(source + i) != '\n' && *(source + i) != 0;i++)
	{
		*(target + i) = *(source + i);	
	}
	if(*(source + i) == '\n')//最后读取到换行符
	{
		*(target + i) = 0;
		i++;
	}
	return i;
}

/*===================================================

  *	函数名	:cmd_decode
  
  *	功     能	:命令解析根据输入的字符串返回相应
  				 的指令代码。具体如下
  				 
  *	输     入	:char* str 指令字符串
  
  *	返回值	:对应指令代号(int型)
  
  *	对应关系
  					seat/		1			/seat		2	座次
  					blind/		3			/blind		4	盲注
  					hold/		5			/hold		6	手牌
  					inquire/		7			/inquire		8	询问
  					flop/		9			/flop		10	公牌
  					turn/		11			/turn		12	转牌
  					river/		13			/river		14	河牌
  					showdown/	15			/showdown	16	摊牌
  					common/		17			/common		18	公牌5
  					pot-win/		19			/pot-win		20	彩池
  					game-over	21							结束
  					notify/     22           /notify        23 通知 

  					如果不是上诉命令则返回0

  					PS.	命令相差比较大
  					       仅比较前三个字符

  ===================================================*/
int cmd_decode(char* str)
{
	
	switch(*str++)
	{
		case 'b':	//	blind/
			if(*str++ == 'l' && *str == 'i')
				return 3;
			break;
		case 'c':	//	common/
			if(*str++ == 'o' && *str == 'm')
				return 17;
			break;
		case 'f':	//	flop/
			if(*str++ == 'l' && *str == 'o')
				return 9;
			break;
		case 'g':	//	game-over/
			if(*str++ == 'a' && *str == 'm')
				return 21;
			break;
		case 'h':	//	hold/
			if(*str++ == 'o' && *str == 'l')
				return 5;
			break;
		case 'i':	//	inquire/
			if(*str++ == 'n' && *str == 'q')
				return 7;
			break;
		case 'n':	//	notify/
			if(*str++ == 'o' && *str == 't')
				return 22;
			break;
		case 'p':	//	pot-win/
			if(*str++ == 'o' && *str == 't')
				return 19;
			break;
		case 'r':	//	river/
			if(*str++ == 'i' && *str == 'v')
				return 13;
			break;
		case 's':	//	seat/			showdown/
			switch(*str++)
			{
				case 'e':
					if(*str++ == 'a' && *str == 't')
						return 1;
					break;
				case 'h':
					if(*str++ == 'o' && *str == 'w')
						return 15;
					break;
				default:
					return 0;
					break;
			}
			break;
		case 't':	//	turn/
			if(*str++ == 'u' && *str == 'r')
				return 11;
			break;
		case '/':
			switch(*str++)
			{
				case 'b':	//	/blind
					if(*str++ == 'l' && *str == 'i')
						return 4;
					break;
				case 'c':	//	/common
					if(*str++ == 'o' && *str == 'm')
						return 18;
					break;
				case 'f':	//	/flop
					if(*str++ == 'l' && *str == 'o')
						return 10;
					break;
				case 'g':	//	/game-over
					if(*str++ == 'a' && *str == 'm')
						return 22;
					break;
				case 'h':	//	/hold/
					if(*str++ == 'o' && *str == 'l')
						return 6;
					break;
				case 'i':	//	/inquire
					if(*str++ == 'n' && *str == 'q')
						return 8;
					break;
				case 'n':	//	/notify
					if(*str++ == 'o' && *str == 't')
						return 23;
					break;
				case 'p':	//	/pot-win
					if(*str++ == 'o' && *str == 't')
						return 20;
					break;
				case 'r':	//	/river
					if(*str++ == 'i' && *str == 'v')
						return 14;
					break;
				case 's':	//	/seat			/showdown
					switch(*str++)
					{
						case 'e':
							if(*str++ == 'a' && *str == 't')
								return 2;
							break;
						case 'h':
							if(*str++ == 'o' && *str == 'w')
								return 16;
							break;
						default:
							return 0;
							break;
					}
					break;
				case 't':	//	/turn
					if(*str++ == 'u' && *str == 'r')
						return 12;
					break;
				default:
					return 0;
			}
			break;
		default:
			return 0;
	}
	return 0;
}

