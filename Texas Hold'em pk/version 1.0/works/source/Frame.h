/******************************************************************************

                  ��Ȩ���� (C), 2015-2015, OUC

 ******************************************************************************
  �� �� ��   : Frame.h
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2015��5��14��
  ����޸�   :
  ��������   : ���ݽṹ����
******************************************************************************/

//�ܵ�������
#define CARD_POWER_NUM 7462

/* ��������״̬ */
typedef enum
{
	ERR = 0,//ʧ��
	OK = 1,//�ɹ�
}ReturnStatus;


/* ���� */
typedef enum
{
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
	OTHER = 0//����������ֵʱ��
}CardKinds;

/* ��ɫ */
typedef enum
{
	Spade = 1,//����
	Heart = 2,//����
	Diamond = 3,//��Ƭ
	Club = 4//�ݻ�
}CardColor;

/* ��ֵ */
typedef enum
{
	Card_Null = 0,//���ڳ�ʼ��
	Card2 = 2,//2
	Card3 = 3,//3
	Card4 = 4,//4
	Card5 = 5,//5
	Card6 = 6,//6
	Card7 = 7,//7
	Card8 = 8,//8
	Card9 = 9,//9
	CardT = 10,//T
	CardJ = 11,//J
	CardQ = 12,//Q
	CardK = 13,//K
	CardA = 14,//A
}CardValue;

/* ��ֵ��Ӧ������ */
typedef enum
{
	Card2_prime = 2,
	Card3_prime = 3,
	Card4_prime = 5,
	Card5_prime = 7,
	Card6_prime = 11,
	Card7_prime = 13,
	Card8_prime = 17,
	Card9_prime = 19,
	CardT_prime = 23,
	CardJ_prime = 29,
	CardQ_prime = 31,
	CardK_prime = 37,
	CardA_prime = 41,
}Card_Prime;

/* �����˿���Ϣ */
typedef struct
{
	Card_Prime prime;//��ֵ��Ӧ������(2~A)~(2~41)
	CardColor color;//��ɫ��S:���ң�H:���ң�D:��Ƭ��C:�ݻ���
	CardValue card_val;//��ֵ(2~A)~(2~14)
}CardInfos;

/*������Ϣ*/
typedef struct
{
	CardInfos card1;
	CardInfos card2;
}Hand_Cards;

/* ��Ϸ�׶� */
typedef enum
{
	GameStage_Null = 0,//���ڳ�ʼ�� 
	Preflop = 1,//äע 
	Flop = 2,//����
	Turn = 3,//ת��
	River = 4,//����
	Showdown = 5//̯��
}GameStage;

/* ����״̬ */
typedef enum
{
	Action_Null = 0,//���ڳ�ʼ�� 
	Fold = 1,//����
	Call = 2,//����
	Raise = 3,//��ע
	All_In = 4,//ȫѹ
	Check = 5//����
}ActionStatue; 

/*���ֶ�ע��Ϣ��������ǰ����һ�ε���ע�ܶ*/
typedef struct
{
	int id;//����ID 
	int bet_last;//��һ���ܶ�ע 
	int bet_now;//��ǰ�ܶ�ע 
}Bet_Infos;


/*���ָ�����Ϣ*/
typedef struct
{
	/*����Ϸ�׶�(����ǰ�����ơ�ת�ơ�����)��ע��������GameStage��˳��*/
	int stage_call_num[4];
	/*��һ��Ϊֹ������Ϸ�׶θ�ע�����������ж϶����Ƿ��ƣ����ֶ���̯�ƽ׶α����� ,
	��̯��ʱ����ж���ID����last_stage_call_num[i] = stage_call_num[i],
	��̯��ʱ���û�ж���ID����stage_call_num[i] = last_stage_call_num[i]*/ 
	int last_stage_call_num[4];
	/*����Ϸ�׶Σ�����ѡ����ƣ����Ӧ��ƽ������������ǰ������Ϊ1~9��������Ϊ0~1��
	��̯��ʱ����ж���ID����:
	call_power_aver[i] = (now_call_power[i] * last_stage_call_num[i] + now_call_power[i])/stage_call_num[i],
	��̯��ʱ���û�ж���ID���򲻱�*/ 
	float call_power_aver[4];
	/*����Ϸ�׶Σ����ָ�������*/
	float now_call_power[4];
}Call_Infos;
/*���ּ�ע��Ϣ*/
typedef struct
{
	int stage_raise_num[4];
	int last_stage_raise_num[4];
	float raise_power_aver[4];
	float now_raise_power[4];
}Raise_Infos;
/*����ȫѹ��Ϣ*/
typedef struct
{
	int stage_allin_num[4];
	int last_stage_allin_num[4];
	float allin_power_aver[4];
	float now_allin_power[4];
}AllIn_Infos;
/*���ֿ�����Ϣ*/
typedef struct
{
	int stage_check_num[4];
	int last_stage_check_num[4];
	float check_power_aver[4];
	float now_check_power[4];
}Check_Infos;
/*������ʷ������Ϣ*/
typedef struct
{
	int all_live_num;//��������ƾ���
	int id;//����ID
	int fold_num[4];//����Ϸ�׶�(����ǰ�����ơ�ת�ơ�����)���ƴ���
	Call_Infos call;//������Ϣ
	Raise_Infos raise;//��ע��Ϣ
	AllIn_Infos all_in;//ȫѹ��Ϣ
	Check_Infos check;//������Ϣ
}Opponent_Action_Infos;
