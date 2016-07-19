 /******************************************************************************
  文 件 名   : command_decode.h
  版 本 号   : 初稿
  作    者   : GBR 
  生成日期   : 2015年5月21日
  最近修改   :
  功能描述   : 数据结构定义
******************************************************************************/
#ifndef COMMAND_DECODE_H_INCLUDED
#define COMMAND_DECODE_H_INCLUDED
/*===================================================

  *	函数名	:getline
  *	功     能	:从source 读取一行数据至target 并返回长度
  *	输     入	:const char* source	源字符串
  				          char* target	目标字符串
  *	返回值	:读取长度

  ===================================================*/
extern int get_line(const char* source,char* target);

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

  					如果不是上诉命令则返回0

  					PS.	命令相差比较大
  					       仅比较前三个字符

  ===================================================*/
extern int cmd_decode(char* str);

#endif // COMMAND_DECODE_H_INCLUDED

