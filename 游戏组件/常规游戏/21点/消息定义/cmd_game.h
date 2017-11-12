#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(push)
#pragma pack(1)


//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID           16                  //��Ϸ I D
#define GAME_NAME         TEXT("21 ��")           //��Ϸ����

//�������
#define GAME_PLAYER         4                 //��Ϸ����
#define VERSION_SERVER        PROCESS_VERSION(7,0,1)        //����汾
#define VERSION_CLIENT        PROCESS_VERSION(7,0,1)        //����汾

//////////////////////////////////////////////////////////////////////////////////

//����ԭ��
#define GER_NO_PLAYER       0x10                //û�����
#define LEN_NICKNAME        32                  //�ǳƳ���

//״̬����
#define GAME_SCENE_FREE       GAME_STATUS_FREE          //�ȴ���ʼ
#define GAME_SCENE_ADD_SCORE    GAME_STATUS_PLAY          //�м���ע
#define GAME_SCENE_GET_CARD     (GAME_STATUS_PLAY+1)        //ׯ�Ҳ���

//������Ϣ
#define IDM_ADMIN_UPDATE_STORAGE    WM_USER+1001
#define IDM_ADMIN_MODIFY_STORAGE    WM_USER+1011
#define IDM_REQUEST_QUERY_USER      WM_USER+1012
#define IDM_USER_CONTROL        WM_USER+1013
#define IDM_REQUEST_UPDATE_ROOMINFO   WM_USER+1014
#define IDM_CLEAR_CURRENT_QUERYUSER   WM_USER+1015

//������¼
#define MAX_OPERATION_RECORD      20                  //������¼����
#define RECORD_LENGTH         128                 //ÿ����¼�ֳ�

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START        100                 //��Ϸ��ʼ
#define SUB_S_GAME_END          101                 //��Ϸ����
#define SUB_S_SEND_CARD         102                 //����
#define SUB_S_SPLIT_CARD        103                 //����
#define SUB_S_STOP_CARD         104                 //ͣ��
#define SUB_S_DOUBLE_SCORE        105                 //�ӱ�
#define SUB_S_INSURE          106                 //����
#define SUB_S_ADD_SCORE         107                 //��ע
#define SUB_S_GET_CARD          108                 //Ҫ��
#define SUB_S_CHEAT_CARD        109                 //��������

#define SUB_S_ADMIN_STORAGE_INFO    112                 //ˢ�¿��Ʒ����
#define SUB_S_REQUEST_QUERY_RESULT    113                 //��ѯ�û����
#define SUB_S_USER_CONTROL        114                 //�û�����
#define SUB_S_USER_CONTROL_COMPLETE   115                 //�û��������
#define SUB_S_OPERATION_RECORD        116                 //������¼
#define SUB_S_REQUEST_UDPATE_ROOMINFO_RESULT 117

//�����˴��ȡ��
struct tagCustomAndroid
{
  LONGLONG                  lRobotScoreMin;
  LONGLONG                  lRobotScoreMax;
  LONGLONG                                  lRobotBankGet;
  LONGLONG                  lRobotBankGetBanker;
  LONGLONG                  lRobotBankStoMul;
};

//��������
typedef enum {CONTINUE_WIN, CONTINUE_LOST, CONTINUE_CANCEL} CONTROL_TYPE;

//���ƽ��      ���Ƴɹ� ������ʧ�� ������ȡ���ɹ� ������ȡ����Ч
typedef enum {CONTROL_SUCCEED, CONTROL_FAIL, CONTROL_CANCEL_SUCCEED, CONTROL_CANCEL_INVALID} CONTROL_RESULT;

//�û���Ϊ
typedef enum {USER_SITDOWN, USER_STANDUP, USER_OFFLINE, USER_RECONNECT} USERACTION;

//������Ϣ
typedef struct
{
  CONTROL_TYPE            control_type;           //��������
  BYTE                cbControlCount;           //���ƾ���
  bool                  bCancelControl;           //ȡ����ʶ
} USERCONTROL;

//�����û���Ϣ
typedef struct
{
  WORD                wChairID;             //����ID
  WORD                wTableID;             //����ID
  DWORD               dwGameID;             //GAMEID
  bool                bAndroid;             //�����˱�ʶ
  TCHAR               szNickName[LEN_NICKNAME];     //�û��ǳ�
  BYTE                cbUserStatus;           //�û�״̬
  BYTE                cbGameStatus;           //��Ϸ״̬
} ROOMUSERINFO;

//�����û�����
typedef struct
{
  ROOMUSERINFO            roomUserInfo;           //�����û���Ϣ
  USERCONTROL             userControl;            //�û�����
} ROOMUSERCONTROL;

//�����˿�
struct CMD_S_CheatCard
{
  BYTE              cbCardData[GAME_PLAYER*2][11];  //�˿��б�
};

struct CMD_S_StatusFree
{
  LONGLONG              lRoomStorageStart;          //������ʼ���
  LONGLONG              lRoomStorageCurrent;        //���䵱ǰ���
  tagCustomAndroid          CustomAndroid;            //����������
};

//��Ϸ״̬
struct CMD_S_StatusAddScore
{
  LONGLONG              lCellScore;             //��Ԫע��
  LONGLONG              lMaxScore;              //���ע��

  WORD                wBankerUser;            //ׯ��

  LONGLONG              lTableScore[GAME_PLAYER];     //������ע
  LONGLONG              lRoomStorageStart;          //������ʼ���
  LONGLONG              lRoomStorageCurrent;        //���䵱ǰ���
  tagCustomAndroid          CustomAndroid;            //����������
};

//��Ϸ״̬
struct CMD_S_StatusGetCard
{
  SCORE               lCellScore;             //��Ԫע��

  WORD                wBankerUser;            //ׯ��

  SCORE               lTableScore[GAME_PLAYER];     //������ע

  //�˿���Ϣ
  BYTE                cbCardCount[GAME_PLAYER*2];     //�˿���Ŀ
  BYTE                cbHandCardData[GAME_PLAYER*2][11];  //�����˿�

  //
  BYTE                bStopCard[GAME_PLAYER*2];     //���ͣ��
  BYTE                bDoubleCard[GAME_PLAYER*2];     //��Ҽӱ�
  BYTE                bInsureCard[GAME_PLAYER*2];     //����±���
  LONGLONG              lRoomStorageStart;          //������ʼ���
  LONGLONG              lRoomStorageCurrent;        //���䵱ǰ���
  tagCustomAndroid          CustomAndroid;            //����������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
  //��ע��Ϣ
  SCORE               lCellScore;             //��Ԫ��ע
  SCORE               lMaxScore;              //�����ע
  //�û���Ϣ
  WORD                wBankerUser;            //��ǰ���
};

//��ע
struct CMD_S_AddScore
{
  WORD                wAddScoreUser;            //��ע���

  LONGLONG              lAddScore;              //��ע��
};

//Ҫ��
struct CMD_S_GetCard
{
  WORD                wGetCardUser;           //Ҫ�����
  BYTE                cbCardData;             //������
};

//����
struct CMD_S_SendCard
{
  BYTE                cbHandCardData[GAME_PLAYER][2];   //��������
  bool                bWin;               //�Ƿ�Ӯ��
};

//����
struct CMD_S_SplitCard
{
  WORD                wSplitUser;             //�������
  BYTE                bInsured;             //�Ƿ�֮ǰ���˱���

  LONGLONG              lAddScore;              //��ע��
  BYTE                cbCardData[2];            //������
};

//ͣ��
struct CMD_S_StopCard
{
  WORD                wStopCardUser;            //ͣ�����
  BYTE                bTurnBanker;            //�Ƿ��ֵ�ׯ��
};

//�ӱ�
struct CMD_S_DoubleScore
{
  WORD                wDoubleScoreUser;         //�ӱ����

  BYTE                cbCardData;             //������
  LONGLONG              lAddScore;              //��ע��
};

//����
struct CMD_S_Insure
{
  WORD                wInsureUser;            //�������

  LONGLONG              lInsureScore;           //���ս�
};

//��Ϸ����
struct CMD_S_GameEnd
{
  LONGLONG              lGameTax[GAME_PLAYER];        //��Ϸ˰��
  LONGLONG              lGameScore[GAME_PLAYER];      //��Ϸ�÷�
  BYTE                cbCardData[GAME_PLAYER*2];      //�û��˿�
};

struct CMD_S_RequestQueryResult
{
  ROOMUSERINFO            userinfo;             //�û���Ϣ
  bool                bFind;                //�ҵ���ʶ
};

//�û�����
struct CMD_S_UserControl
{
  DWORD                 dwGameID;             //GAMEID
  TCHAR                 szNickName[LEN_NICKNAME];     //�û��ǳ�
  CONTROL_RESULT              controlResult;            //���ƽ��
  CONTROL_TYPE              controlType;            //��������
  BYTE                  cbControlCount;           //���ƾ���
};

//�û�����
struct CMD_S_UserControlComplete
{
  DWORD                 dwGameID;             //GAMEID
  TCHAR                 szNickName[LEN_NICKNAME];     //�û��ǳ�
  CONTROL_TYPE              controlType;            //��������
  BYTE                  cbRemainControlCount;       //ʣ����ƾ���
};

//���Ʒ���˿����Ϣ
struct CMD_S_ADMIN_STORAGE_INFO
{
  LONGLONG  lRoomStorageStart;            //������ʼ���
  LONGLONG  lRoomStorageCurrent;
  LONGLONG  lRoomStorageDeduct;
  LONGLONG  lMaxRoomStorage[2];
  WORD    wRoomStorageMul[2];
};

//������¼
struct CMD_S_Operation_Record
{
  TCHAR   szRecord[MAX_OPERATION_RECORD][RECORD_LENGTH];          //��¼���²�����20����¼
};

//������½��
struct CMD_S_RequestUpdateRoomInfo_Result
{
  LONGLONG              lRoomStorageCurrent;        //���䵱ǰ���
  ROOMUSERINFO            currentqueryuserinfo;       //��ǰ��ѯ�û���Ϣ
  bool                bExistControl;            //��ѯ�û����ڿ��Ʊ�ʶ
  USERCONTROL             currentusercontrol;
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_ADD_SCORE         1                 //�û���ע
#define SUB_C_GET_CARD          2                 //Ҫ��
#define SUB_C_DOUBLE_SCORE        3                 //�ӱ�
#define SUB_C_INSURE          4                 //����
#define SUB_C_SPLIT_CARD        5                 //����
#define SUB_C_STOP_CARD         6                 //������ע


#define SUB_C_STORAGE         10                  //���¿��
#define SUB_C_STORAGEMAXMUL       11                  //��������
#define SUB_C_REQUEST_QUERY_USER    12                  //�����ѯ�û�
#define SUB_C_USER_CONTROL        13                  //�û�����

//�����������
#define SUB_C_REQUEST_UDPATE_ROOMINFO 14                  //������·�����Ϣ
#define SUB_C_CLEAR_CURRENT_QUERYUSER 15

//�û���ע
struct CMD_C_AddScore
{
  LONGLONG                lScore;               //��ע��Ŀ
};

struct CMD_C_UpdateStorage
{
  LONGLONG            lRoomStorageCurrent;          //�����ֵ
  LONGLONG            lRoomStorageDeduct;           //�����ֵ
};

struct CMD_C_ModifyStorage
{
  LONGLONG            lMaxRoomStorage[2];             //�������
  WORD              wRoomStorageMul[2];             //Ӯ�ָ���
};

struct CMD_C_RequestQuery_User
{
  DWORD             dwGameID;               //��ѯ�û�GAMEID
  TCHAR             szNickName[LEN_NICKNAME];         //��ѯ�û��ǳ�
};

//�û�����
struct CMD_C_UserControl
{
  DWORD                 dwGameID;             //GAMEID
  TCHAR                 szNickName[LEN_NICKNAME];     //�û��ǳ�
  USERCONTROL               userControlInfo;          //
};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif