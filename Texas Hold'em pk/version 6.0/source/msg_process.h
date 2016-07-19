
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

/*�������̯�ƴ���*/
void Upd_Plays_Show_Num(void);


/*�����ע������С20����;��g_opp_act_info[k].id == showdown[k].id��
��idx = k;data�����������;stage�ĸ��׶�;action_code ��������*/
static void Save_Min_Data20(unsigned int idx,unsigned int stage, float data, int action_code); 

/*�����ע�������20����;��g_opp_act_info[k].id == showdown[k].id��
��idx = k;data�����������;stage�ĸ��׶�;action_code ��������*/
static void Save_Max_Data20(unsigned int idx,unsigned int stage, float data, int action_code); 

/*������ָ�ע����������Showdown_Process���ã���g_opp_act_info[k].id == showdown[j].id����idx = k,show_idx = j*/
static void Showdown_Cal_Rai_All_Che(unsigned int idx, unsigned int show_idx); 

/*̯�ƽ׶δ�����*/
static void Showdown_Process(void); 

/*
����g_opp_act_info���������,��Updata_Opponent_Noti_ActionNum
��Updata_Opponent_Inqu_ActionNum�б�����
���룺 Id_idx���ID������ 
	   action_code�������� 
*/
static void Updata_Players_Actions(int Id_idx,int action_code); 

/*inquire_action����*/
static void ClearInquireAction(void);

/*notify_action����*/
static void ClearNotifyAction(void);

/*��һ��ʱ��¼���ID������Process�е���*/
static void Fill_Players_ID(void); 

/*������Ҷ�ע����ѯ����Ϣ����β��������*/
void Updata_Players_Bet(void);

/*���Լ�ѡ���עʱ�������Լ��Ķ�ע���ڷ�������ʱ������,idx��ע�������*/
void Updata_Own_Bet(int idx);

/*���¶��ֶ���������֪ͨ��Ϣ����β��������*/
void Updata_Opponent_Noti_ActionNum(void);

/*���¶��ֶ���������ѯ����Ϣ����β��������*/
void Updata_Opponent_Inqu_ActionNum(void);

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

/*���¸����׶εĶ���������ѯ����Ϣ��֪ͨ��Ϣ�е���,X_T = 'X'ѯ����Ϣ�е��ã�X_T = 'T'֪ͨ��Ϣ�е���*/
static void UpdataOpponentNum(uint8_t X_T);

/*ÿ�ֽ�����������*/
static void ClearData(void); 

/*�������ݴ�����*/
extern int process(int length, const char* buffer);


#endif // MSG_PROCESS_H_INCLUDED
