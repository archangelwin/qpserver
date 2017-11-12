#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID             19                  //��Ϸ��ʶ
#define GAME_PLAYER           2                 //��Ϸ����
#define GAME_NAME           TEXT("�������")          //��Ϸ����

#define MAX_COUNT         5                 //�����Ŀ

#define VERSION_SERVER            PROCESS_VERSION(7,0,1)        //����汾
#define VERSION_CLIENT            PROCESS_VERSION(7,0,1)        //����汾

//////////////////////////////////////////////////////////////////////////
//�����붨��

#define SUB_C_ADD_GOLD          1                 //�û���ע
#define SUB_C_GIVE_UP         2                 //������ע
#define SUB_C_AMDIN_COMMAND       3                 //ϵͳ����
#define SUB_C_AMDIN_COMMAND_EX      4                 //ϵͳ����


#define SUB_S_GAME_START        100                 //��Ϸ��ʼ
#define SUB_S_ADD_GOLD          101                 //��ע���
#define SUB_S_GIVE_UP         102                 //������ע
#define SUB_S_SEND_CARD         103                 //������Ϣ
#define SUB_S_GAME_END          104                 //��Ϸ����
#define SUB_S_ALL_CARD          105                 //������Ϣ
#define SUB_S_AMDIN_COMMAND       106                 //ϵͳ����
#define SUB_S_AMDIN_COMMAND_EX      107                 //ϵͳ����


#ifndef _UNICODE
#define myprintf  _snprintf
#define mystrcpy  strcpy
#define mystrlen  strlen
#define myscanf   _snscanf
#define myLPSTR   LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
#else
#define myprintf  swprintf
#define mystrcpy  wcscpy
#define mystrlen  wcslen
#define myscanf   _snwscanf
#define myLPSTR   LPWSTR
#define myatoi      _wtoi
#define myatoi64  _wtoi64
#endif

#define IDM_ADMIN_COMMDN        WM_USER+1000
#define IDM_ADMIN_COMMDN_EX       WM_USER+1001

//////////////////////////////////////////////////////////////////////////
//��Ϣ�ṹ��

//��Ϸ״̬
struct CMD_S_StatusFree
{
  LONGLONG              dwBasicGold;            //�������
  TCHAR               szGameRoomName[32];     //��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
  WORD                wCurrentUser;           //��ǰ���
  BYTE                bPlayStatus[GAME_PLAYER];     //��Ϸ״̬
  BYTE                bTableCardCount[GAME_PLAYER];   //�˿���Ŀ
  BYTE                bTableCardArray[GAME_PLAYER][5];  //�˿�����
  LONGLONG              lBasicGold;             //�������
  LONGLONG              lTurnMaxGold;           //�����ע
  LONGLONG              lTurnBasicGold;           //������ע
  LONGLONG              lTableGold[2*GAME_PLAYER];      //������
  LONGLONG              lShowHandScore;           //
  BYTE                bShowHand;              //�Ƿ����
  LONGLONG              lServiceScore;            //�������
  TCHAR               szGameRoomName[32];     //��������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
  BYTE                bFundusCard;            //�����˿�
  WORD                wCurrentUser;           //��ǰ���
  LONGLONG              lTurnMaxGold;           //�����ע
  LONGLONG              lTurnBasicGold;           //������ע
  LONGLONG              lBasicGold;             //��Ԫ��ע
  BYTE                bCardData[GAME_PLAYER];       //�û��˿�
  LONGLONG              lShowHandScore;           //
  LONGLONG              lServiceScore;            //�������
};

//�û�����
struct CMD_S_GiveUp
{
  WORD                wUserChairID;           //�����û�
  LONGLONG              lTurnMaxGold;           //
};

//�û���ע
struct CMD_C_AddGold
{
  LONGLONG              lGold;                //��ע��Ŀ
};

//��ע���
struct CMD_S_AddGold
{
  WORD                wLastChairID;           //��һ�û�
  WORD                wCurrentUser;           //��ǰ�û�
  LONGLONG              lLastAddGold;           //��ע��Ŀ
  LONGLONG              lCurrentLessGold;         //���ټ�ע
  bool                bShowHand;              //�Ƿ����
  bool                bNoGiveUp;              //�������Ƿ�������
  LONGLONG              lShowHandGold;            //������������
  bool                bAndroidLose;           //��������s
  bool                bCanShowHand;           //�Ƿ������
};

//�������ݰ�
struct CMD_R_SendCard
{
  LONGLONG              lMaxGold;             //�����ע
  WORD                wCurrentUser;           //��ǰ�û�
  WORD                wStartChairId;            //
  BYTE                cbSendCardCount;          //
  BYTE                bUserCard[GAME_PLAYER][2];      //�û��˿�
  bool                bNoGiveUp;              //�������Ƿ����
  bool                bAndroidLose;           //��������s
  LONGLONG              lShowHandGold;            //������������
  bool                bCanShowHand;           //�Ƿ������
};

//��Ϸ����
struct CMD_S_GameEnd
{
  LONGLONG              lTax;               //��Ϸ˰��
  LONGLONG              lGameGold[GAME_PLAYER];       //��Ϸ�÷�
  BYTE                bUserCard[GAME_PLAYER];       //�û��˿�
};

//�������ݰ�
struct CMD_S_AllCard
{
  bool                bAICount[GAME_PLAYER];
  BYTE                cbCardData[GAME_PLAYER][MAX_COUNT]; //�û��˿�
};

//////////////////////////////////////////////////////////////////////////
struct CMD_C_AdminReq
{
  BYTE cbReqType;
#define RQ_SET_WIN_AREA 1
#define RQ_RESET_CONTROL  2
#define RQ_PRINT_SYN    3
  BYTE cbExtendData[20];      //��������
  BYTE cbExcuteTimes;       //ִ�д���
};

//����ظ�
struct CMD_S_CommandResult
{
  BYTE cbAckType;         //�ظ�����
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
  BYTE cbResult;
#define CR_ACCEPT  2      //����
#define CR_REFUSAL 3      //�ܾ�
  BYTE cbExtendData[20];      //��������
};

//����������Ϣ
struct tagControlInfo
{
  INT  nAreaWin;    //��������
};

////////////////////////////////////////////////////////////////
struct CMD_C_AdminReqEx
{
  BYTE cbReqType;
#define RQ_UPDATE_STORAGE 1
#define RQ_SET_STORAGE    2
#define RQ_CLEAR_ALLSTORAGE 3
  WORD    wTableID;       //����
  LONGLONG  lStorage;       //���
};

//����ظ�
struct CMD_S_CommandResultEx
{
  BYTE cbAckType;         //�ظ�����
#define ACK_UPDATE_STORAGE  1
#define ACK_SET_STORAGE     2
#define ACK_CLEAR_ALLSTORAGE     3
  BYTE cbResult;
#define CR_ACCEPT  2      //����
#define CR_REFUSAL 3      //�ܾ�
  WORD    wTableID;       //����
  LONGLONG  lStorage;       //���
};

///////////////////////////////////////////////////////////////////
#endif
