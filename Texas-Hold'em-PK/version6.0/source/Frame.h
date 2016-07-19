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
	Check = 5,//����
	Blind = 6 //äע 
}ActionStatue; 

/*���ֶ�ע��Ϣ��������ǰ����һ�ε���ע�ܶ*/
typedef struct
{
	int id;//��ǰ�����ע��Ķ���ID 
	int bet_last;//�Լ�����ע�� 
	int bet_now;//��ǰ�����ע�� 
}Bet_Infos;





/*���ָ�����Ϣ*/
typedef struct
{
	/*����Ϸ�׶�(����ǰ�����ơ�ת�ơ�����)��ע��־����GameStage��˳��,ÿ�ֿ�ʼ������*/
	int now_num[4];
	/*����Ϸ�׶�(����ǰ�����ơ�ת�ơ�����)��ע�ܴ�������GameStage��˳��
	call_all_num[i]���ֵΪ600����̯��ʱ������*/
	int all_num[4];
	/*����Ϸ�׶Σ�����ѡ����ƣ����Ӧ��ƽ������������ǰ������Ϊ1~9��������Ϊ0~1����̯��ʱ����ж���ID��
	��:	power_aver[i] = (power_aver[i] * all_num[i] + ��������)/��all_num[i] + 1��,
	��̯��ʱ���û�ж���ID���򲻱�*/ 
	float power_aver[4];
	/*���׶θ�ע����С20������*/
	float power_min20[4][20];
	/*���׶θ�ע�����20������*/ 
	float power_max20[4][20];
}History_Action;

/*���ָ��׶���ʷ������Ϣ*/
typedef struct
{
	int id;//����ID
	int showdown_num;//̯�ƴ��� 
	int fold_num[4];//����Ϸ�׶�(����ǰ�����ơ�ת�ơ�����)���ƴ���
	History_Action call;//������Ϣ
	History_Action raise;//��ע��Ϣ
	History_Action all_in;//ȫѹ��Ϣ
	History_Action check;//������Ϣ
}Opponent_Action_Infos;

/*������Ʊ�־*/ 
typedef struct
{
	int id;//����ID
	uint8_t fold_flg;//���Ʊ�־ 
}Opponent_Fold_Flg;


/*���ֶ�����¼����ģ׼��*/
struct OpponentActionRecord
{
	int id;
	int opponent_fold;
	int opponent_call;
	int opponent_raise;
	int opponent_all_in;
	int opponent_check;
}opponent_action[8];



///////////////////////////////////////////////////////////////////////////////////
/*������Ϣ*/
struct seat_info_msg
{
    int id;
    int jetton;
    int money;
}seat[5],seat_button,seat_blind[2];

/*äע��Ϣ*/
struct blind_info_msg
{
    int id;
    int bet;
}blind_infos[2];

/*ѯ���ж��������Ϣ,֪ͨ��Ϣ���ж��������Ϣ*/
struct inquire_msg
{
	int id;  
	int jetton;//���г��� 
	int money; //ʣ������ 
	int bet;  //�������ۼ�Ͷע��
	int action; 
}inquire_action[8],notify_action[8];


/*̯�ƹ�����Ϣ*/ 
struct showdown_rank 
{
	int rank; 
	int id;
	int nut_hand;//���� 
	Hand_Cards hand_cards;
}showdown[8];

/*�ʳط�����Ϣ*/
struct pot_win_msg
{
	int id;
	int num;
}pot_win[8]; 


//��������״̬����Ϊ����ʮһ��֮һ 
typedef enum
{
	DEF = 0, 
	SEAT = 1,
 	BLIND = 2, 
 	HOLD = 3, 
 	INQUIRE = 4,
 	FLOP = 5,
 	TURN = 6, 
 	RIVER = 7,
 	SHOWDOWN = 8, 
 	COMMON = 9, 
 	POTWIN = 10,
	NOTIFY = 11 
}STATE;