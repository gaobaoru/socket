 /******************************************************************************
  �� �� ��   : card_decode.h
  �� �� ��   : ����
  ��    ��   : GBR
  ��������   : 2015��5��20��
  ����޸�   :
  ��������   : ���ݽṹ����
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_decode.h"
/*===================================================

  *	������	:getline
  *	��     ��	:��source ��ȡһ��������target �����س���
  *	��     ��	:const char* source	Դ�ַ���
  				          char* target	Ŀ���ַ���
  *	����ֵ	:��ȡ����

  ===================================================*/
int get_line(const char* source,char* target)
{
	int i;
	for(i = 0;*(source + i) != '\n' && *(source + i) != 0;i++)
	{
		*(target + i) = *(source + i);	
	}
	if(*(source + i) == '\n')//����ȡ�����з�
	{
		*(target + i) = 0;
		i++;
	}
	return i;
}

/*===================================================

  *	������	:cmd_decode
  
  *	��     ��	:�����������������ַ���������Ӧ
  				 ��ָ����롣��������
  				 
  *	��     ��	:char* str ָ���ַ���
  
  *	����ֵ	:��Ӧָ�����(int��)
  
  *	��Ӧ��ϵ
  					seat/		1			/seat		2	����
  					blind/		3			/blind		4	äע
  					hold/		5			/hold		6	����
  					inquire/		7			/inquire		8	ѯ��
  					flop/		9			/flop		10	����
  					turn/		11			/turn		12	ת��
  					river/		13			/river		14	����
  					showdown/	15			/showdown	16	̯��
  					common/		17			/common		18	����5
  					pot-win/		19			/pot-win		20	�ʳ�
  					game-over	21							����
  					notify/     22           /notify        23 ֪ͨ 

  					����������������򷵻�0

  					PS.	�������Ƚϴ�
  					       ���Ƚ�ǰ�����ַ�

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

