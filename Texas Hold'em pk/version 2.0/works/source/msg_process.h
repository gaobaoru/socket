
 /******************************************************************************
  �� �� ��   : msg_process.h
  �� �� ��   : ����
  ��    ��   : GBR 
  ��������   : 2015��5��19��
  ����޸�   :
  ��������   : ���ݽṹ����
******************************************************************************/
#ifndef MSG_PROCESS_H_INCLUDED
#define MSG_PROCESS_H_INCLUDED

/*��float��ת���ַ���*/ 
extern void FloatToStr(float data , char *pchar_for , char *pchar_back);

/*��һ��ʱ��¼���ID������Process�е���*/
static void Fill_Players_ID(void); 

/*������Ҷ�ע����ѯ����Ϣ����β��������*/
void Updata_Players_Bet(void);

/*����ÿ����ע���ֵ������ע����g_Money_Max������ѯ����Ϣ����β��Updata_Players_Bet֮�󱻵���*/
void CountBetMax(void);





/*��ͨ������Ϣ����*/ 
extern void seat_msg(char* str,int num);
/*ׯ��������Ϣ����*/ 
extern void seat_button_msg(char* str);
/*äע������Ϣ����*/ 
extern void seat_blind_msg(char* str,int num);

/*äע��Ϣ����*/ 
extern void blind_msg(char* str,int num);

/*������Ϣ����*/
extern void hd_cards_msg(char* str,int num);
/*��������Ϣ����*/
extern void pb_cards_msg(char* str,int num);

/*ѯ����Ϣ�ж����������Ϣ����*/
extern void inquire_action_msg(char* str,int num);
/*ѯ����Ϣ�е�ǰ�׳���Ϣ����*/
extern void inquire_totalpot_msg(char* str);

/*֪ͨ��Ϣ�ж����������Ϣ����*/
extern void notify_action_msg(char* str,int num);
/*֪ͨ��Ϣ�е�ǰ�׳���Ϣ����*/
extern void notify_totalpot_msg(char* str);

/*̯����Ϣ����*/
extern void showdown_msg(char* str,int num);

/*�ʳط�����Ϣ����*/
extern void potwin_dis_msg(char* str,int num); 

/*��������*/
extern uint8_t* ActionForMsg(void); 

/*����ÿ�ֿ�ʼʱ�Ķ���������process�е���*/
static void CountOpponentNum(void); 

/*���¸����׶εĶ���������process�е���*/
static void UpdataOpponentNum(void); 

/*ÿ�ֽ�����������*/
static void ClearData(void); 

/*�������ݴ�����*/
extern int process(int length, const char* buffer);


#endif // MSG_PROCESS_H_INCLUDED
