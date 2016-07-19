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
#include "card_decode.h"
/*===================================================

  *	������	:card_color_decode
  
  *	��     ��	:�ƻ�ɫ��������������ַ���������Ӧ
  				 �Ļ�ɫ���롣��������
  				 
  *	��     ��	:char* str ָ���ַ���
  
  *	����ֵ	:��Ӧָ�����(int��)
  
  *	��Ӧ��ϵ
  					//	Spade = 1,//����
					//	Hearts = 2,//����
					//	Diamond = 3,//��Ƭ
					//	Club = 4//�ݻ�
  					����������������򷵻�0

  					PS.	�������Ƚϴ�
  					       ���Ƚ�ǰ�����ַ�

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
  *	������	:card_point_decode
  
  *	��     ��	:�ƻ�ɫ��������������ַ���������Ӧ
  				 ����ֵ���롣��������
  				 
  *	��     ��	:char* str ָ���ַ���
  
  *	����ֵ	:��Ӧָ�����(int��)
  
  *	��Ӧ��ϵ
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
  					����������������򷵻�0

  					PS.	�������Ƚϴ�
  					       ���Ƚ�ǰһ���ַ�
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
  *	������	:card_prime_decode
  
  *	��     ��	:�ƻ�ɫ��������������ַ���������Ӧ
  				 ���������롣��������
  				 
  *	��     ��	:char* str ָ���ַ���
  
  *	����ֵ	:��Ӧָ�����(int��)
  
  *	��Ӧ��ϵ
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
  					����������������򷵻�0

  					PS.	�������Ƚϴ�
  					       ���Ƚ�ǰһ���ַ�
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

  *	������	:action_decode
  
  *	��     ��	:������������������ַ���������Ӧ
  				 �Ķ������롣��������
  				 
  *	��     ��	:char* str ָ���ַ���
  
  *	����ֵ	:��Ӧָ�����(int��)
  
  *	��Ӧ��ϵ
  					Fold = 1,//����
					Call = 2,//����
					Raise = 3,//��ע
					ALL_IN = 4,//��ע
					Check = 5//����
					blind = 6 //äע 
  					����������������򷵻�0

  					PS.	�������Ƚϴ�
  					       ���Ƚ�ǰ�����ַ�

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

  *	������	:nut_hand_decode
  
  *	��     ��	:���ͽ�������������ַ���������Ӧ
  				 �����ʹ��롣��������
  				 
  *	��     ��	:char* str ָ���ַ���
  
  *	����ֵ	:��Ӧָ�����(int��)
  
  *	��Ӧ��ϵ
  					ROYAL_FLUSH = 1,//�ʼ�ͬ��˳
					STRAIGHT_FLUSH = 2,//ͬ��˳
					FOUR = 3,//����
					FULL_HOUSE = 4,//��«������һ�ԣ�
					FLUSH = 5,//ͬ��
					STRAIGHT = 6,//˳��
					THREE = 7,//����
					TWO_PAIRS = 8,//����
					ONE_PAIR = 9,//һ��
					HIGHT_CARD = 10,//����
					//OTHER = 0//����������ֵʱ��
  					����������������򷵻�0

  					PS.	�������Ƚϴ�
  					       ���Ƚ�ǰ�����ַ�

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


