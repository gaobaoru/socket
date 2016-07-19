 /******************************************************************************
  �� �� ��   : command_decode.h
  �� �� ��   : ����
  ��    ��   : GBR 
  ��������   : 2015��5��21��
  ����޸�   :
  ��������   : ���ݽṹ����
******************************************************************************/
#ifndef COMMAND_DECODE_H_INCLUDED
#define COMMAND_DECODE_H_INCLUDED
/*===================================================

  *	������	:getline
  *	��     ��	:��source ��ȡһ��������target �����س���
  *	��     ��	:const char* source	Դ�ַ���
  				          char* target	Ŀ���ַ���
  *	����ֵ	:��ȡ����

  ===================================================*/
extern int get_line(const char* source,char* target);

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

  					����������������򷵻�0

  					PS.	�������Ƚϴ�
  					       ���Ƚ�ǰ�����ַ�

  ===================================================*/
extern int cmd_decode(char* str);

#endif // COMMAND_DECODE_H_INCLUDED

