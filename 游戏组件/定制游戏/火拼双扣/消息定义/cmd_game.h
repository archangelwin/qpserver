#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//������
//��Ϸ����
#ifdef VIDEO_GAME
#define KIND_ID           233                 //��Ϸ I D
#define GAME_NAME         TEXT("��ƴ˫��" )         //��Ϸ����
#define GAME_PLAYER         2                 //��Ϸ����
#else
#define KIND_ID           233                 //��Ϸ I D
#define GAME_NAME         TEXT("��ƴ˫��" )         //��Ϸ����
#define GAME_PLAYER         2                 //��Ϸ����
#endif

#define VERSION_SERVER            PROCESS_VERSION(7,0,1)      //����汾
#define VERSION_CLIENT            PROCESS_VERSION(7,0,1)      //����汾

//////////////////////////////////////////////////////////////////////////
//��Ŀ����

#define MAX_COUNT         27                  //�����Ŀ
#define FULL_COUNT          108                 //ȫ����Ŀ
#define TYPE_COUNT          11                  //������Ŀ
#define CARD_TYPE_COUNT       15                  //��ֵ����
#define BOMB_TYPE_COUNT       9                 //ը������
//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define MASK_COLOR          0xF0                //��ɫ����
#define MASK_VALUE          0x0F                //��ֵ����


//////////////////////////////////////////////////////////////////////////
//״̬����

//��Ϸ״̬
#define GS_SK_FREE          GAME_STATUS_FREE          //�ȴ���ʼ
#define GS_SK_PLAYING       GAME_STATUS_PLAY          //��Ϸ״̬



//�����ṹ
struct tagAnalyseInitResult
{
  BYTE              cbBlockCount[8];          //�˿���Ŀ
  BYTE              cbCardData[8][MAX_COUNT];     //�˿�����
  BYTE                            cbKingCount;            //���ĸ���
  BYTE                            cbSubKingCount;           //���ĸ���
};

//�����ṹ
struct tagAnalyseResult
{
  //ԭʼ����
  BYTE  m_cbCardCount;
  //���ͷ���
  struct tagCardStyle
  {
    BYTE  m_cbCard;
    BYTE  m_cbCount;
  } m_CardStyle[CARD_TYPE_COUNT];         //ֽ�ƹ���

  int  m_nUnEqualCard;          //��������
  int  m_nJockerCount;          //��������

  //�������
  BYTE  m_cbCardType;           //ȡ������

  BYTE  m_cbMaxIndex;           //��ʼ����
  BYTE  m_cbMinIndex;           //��ֹ����
#define  NORINDEX_BEGIN 0   //��������ʼ����
#define  NORINDEX_END 12    //��������ֹ����
  int   m_nMaxCount;            //���������

  //ը����Ϣ
  BYTE  m_cbBombGrade;          //ը���ȼ�
};
//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_S_GAME_START      100                 //��Ϸ����
#define SUB_S_OUT_CARD        101                 //�û�����
#define SUB_S_PASS_CARD       102                 //�û�����
#define SUB_S_GAME_END        103                 //��Ϸ����
#define SUB_S_TRUSTEE       104                 //�û��й�
#define SUB_S_PLAYVOICE             106                                 //��������
#define SUB_S_ON_READY        107                 //�û�׼��
#define SUB_S_ADMIN_COMMDN      110                 //ϵͳ����
#define SUB_S_CHEAT_CARD      111                 //ϵͳ����



//�����˿�
struct CMD_S_CheatCard
{
  WORD              wCardUser[GAME_PLAYER ];        //�������
  BYTE              cbUserCount;              //��������
  BYTE              cbCardData[GAME_PLAYER ][MAX_COUNT];  //�˿��б�
  BYTE              cbCardCount[GAME_PLAYER ];        //�˿�����
};

struct CMD_S_OnReady
{
  WORD  wChairId;         //���ID

};
struct CMD_S_PlayVoice
{
  WORD  wChairId;         //�������
  BYTE  type;           //��������
  BYTE  VoiceId;          //�������
};

//����״̬
struct CMD_S_StatusFree
{
  //ׯ���û�
  WORD              wBankerUser;            //ׯ���û�
  //��ʷ����
  LONGLONG                        lAllTurnScore;            //�ֵܾ÷�
  LONGLONG                        lLastTurnScore;           //�Ͼֵ÷�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
  //��Ϸ����
  LONGLONG            lCellScore[3];            //��Ԫ����
  WORD              wCurrentUser;           //��ǰ���

  //��ʷ����
  LONGLONG                        lAllTurnScore[GAME_PLAYER];     //�ֵܾ÷�
  LONGLONG                        lLastTurnScore[GAME_PLAYER];    //�Ͼֵ÷�

  //ׯ���û�
  WORD              wBankerUser;            //ׯ���û�

  //��Ӧ��־
  bool              bTrustee[GAME_PLAYER];        //�Ƿ��й�

  //������Ϣ
  WORD              wTurnWiner;                //ʤ�����
  BYTE              cbTurnCardCount;             //������Ŀ
  BYTE              cbTurnCardData[MAX_COUNT];         //��������
  BYTE              cbOutCardCount[GAME_PLAYER];       //������Ŀ
  BYTE              cbOutCardData[GAME_PLAYER][MAX_COUNT]; //�����б�
  tagAnalyseResult        LastOutCard;               //������

  //�˿���Ϣ
  BYTE              cbHandCardData[MAX_COUNT];      //�����˿�
  BYTE              cbHandCardCount[GAME_PLAYER];   //�˿���Ŀ
};



//�����˿�
struct CMD_S_GameStart
{
  WORD              wCurrentUser;           //��ǰ���
  LONGLONG            lCellScore[3];            //��Ԫ����
  BYTE              cbCardData[MAX_COUNT];        //�˿��б�
  bool              bTrustee[GAME_PLAYER];        //�Ƿ��й�
  BYTE              cbStartRandCard;          //����˿�
  WORD              wGetRandCardID;           //�����û�
  BYTE              cbGetCardPosition;          //����λ��
  BYTE              cbPlayerCardData[MAX_COUNT];    //�Է��˿ˣ�ֻ����������
};

//�û�����
struct CMD_S_OutCard
{
  BYTE              cbCardCount;            //������Ŀ
  WORD              wCurrentUser;           //��ǰ���
  WORD              wOutCardUser;           //�������
  BYTE              cbCardData[MAX_COUNT];        //�˿��б�
  LONGLONG            lBombScore[GAME_PLAYER];      //���׷���
  BYTE              cbCheatCardData[MAX_COUNT];     //��������
};

//��������
struct CMD_S_PassCard
{
  BYTE              cbTurnOver;             //һ�ֽ���
  WORD              wCurrentUser;           //��ǰ���
  WORD              wPassCardUser;            //�������
};

//��Ϸ����
struct CMD_S_GameEnd
{
  //��Ϸ�ɼ�
  LONGLONG            lGameScore[GAME_PLAYER];      //��Ϸ����
  LONGLONG            lAllTurnScore[GAME_PLAYER];     //�ֵܾ÷�
  LONGLONG            lLastTurnScore[GAME_PLAYER];    //�Ͼֵ÷�
  LONGLONG            lGameTax;             //��Ϸ˰��
  //�÷���ϸ
  LONGLONG            lBombScore[GAME_PLAYER];                    //ը���÷�
  BYTE              cbBombList[GAME_PLAYER][BOMB_TYPE_COUNT];   //ը������

  //�˿���Ϣ
  BYTE              cbCardCount[GAME_PLAYER];     //�˿���Ŀ
  BYTE              cbCardData[GAME_PLAYER][MAX_COUNT]; //�˿��б�
  BYTE                            bHunter;              //�û�����
  BYTE                            cbResultType;           //�������

  int                             nMutile;              //����
  int                             nBaseScore;             //������
};
//�û��й�
struct CMD_S_Trustee
{
  bool              bTrustee;             //�Ƿ��й�
  WORD              wChairID;             //�й��û�
};
//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_C_OUT_CARD        1                 //�û�����
#define SUB_C_PASS_CARD       2                 //�û�����
#define SUB_C_TRUSTEE       5                 //�û��й�
#define SUB_C_PLAYVOICE             6                 //��������
#define SUB_C_REQ_HUNTERS     7                 //���볭��
#define SUB_C_ADMIN_COMMDN      8                 //ϵͳ����


#define IDM_ADMIN_COMMDN WM_USER+1000


struct CMD_C_PlayVoice
{
  BYTE  type;
  BYTE  VoiceId;
};

//�û�����
struct CMD_C_OutCard
{
  BYTE              cbCardCount;            //������Ŀ
  BYTE              cbCardData[MAX_COUNT];        //�˿�����
};
//�û��й�
struct CMD_C_Trustee
{
  bool              bTrustee;             //�Ƿ��й�
};


//���������Ʒ���
#define  S_CR_FAILURE       0   //ʧ��
#define  S_CR_UPDATE_SUCCES     1   //���³ɹ�
#define  S_CR_SET_SUCCESS     2   //���óɹ�
#define  S_CR_CANCEL_SUCCESS    3   //ȡ���ɹ�

struct CMD_S_ControlReturns
{
  BYTE cbReturnsType;       //�ظ�����
  BYTE cbControlStyle;      //���Ʒ�ʽ
  BYTE cbControlTimes;      //���ƴ���
};


//��������
#define  C_CA_UPDATE        1   //����
#define  C_CA_SET         2   //����
#define  C_CA_CANCELS       3   //ȡ��

//���Ʒ�ʽ
#define   CS_BANKER_LOSE    1
#define   CS_BANKER_WIN   2

struct CMD_C_ControlApplication
{
  BYTE cbControlAppType;      //��������
  BYTE cbControlStyle;      //���Ʒ�ʽ
  BYTE cbControlTimes;      //���ƴ���

};

//////////////////////////////////////////////////////////////////////////

#endif