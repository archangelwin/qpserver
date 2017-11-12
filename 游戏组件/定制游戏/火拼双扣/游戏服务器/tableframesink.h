#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

#include "GameLogic.h"
#include "ServerControl.h"
#include "CheatServer.h"

#define GER_HUNTER      0x04       //���׽���
#define MAX_SCOREGRADE    100        //��ߵȼ�
struct tagStack
{
  WORD wChairID;
  LONGLONG wBombScore;
};

//////////////////////////////////////////////////////////////////////////
//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
  //��Ϸ����
protected:
  BYTE              m_cbCurGrade;           //���ֵȼ�
  WORD              m_wBankerUser;            //ׯ���û�
  WORD              m_wCurrentUser;           //��ǰ���
  BYTE              m_cbRandCard[FULL_COUNT];     //�����˿�
  bool              m_bTrustee[GAME_PLAYER];      //�Ƿ��й�
  bool              m_bHasReqHunter;          //�Ƿ񳭵�
  LONGLONG            m_lBombScore[GAME_PLAYER];      //���׷���

  //��ʷ����
protected:
  LONGLONG            m_lAllTurnScore[GAME_PLAYER];   //�ֵܾ÷�
  LONGLONG            m_lLastTurnScore[GAME_PLAYER];    //�Ͼֵ÷�


  //ʤ����Ϣ
protected:
  WORD              m_wGameWiner;                 //ʤ���б�
  BYTE              m_cbBombList[GAME_PLAYER][BOMB_TYPE_COUNT];   //ը������,���12��

  //������Ϣ
protected:
  WORD              m_wTurnWiner;               //ʤ�����
  BYTE              m_cbTurnCardCount;              //������Ŀ
  BYTE              m_cbTurnCardData[MAX_COUNT];        //��������
  BYTE              m_cbOutCardCount[GAME_PLAYER];        //������Ŀ
  BYTE              m_cbOutCardData[GAME_PLAYER][MAX_COUNT];  //�����б�
  tagAnalyseResult        m_LastOutCard;                //������

  //�˿���Ϣ
protected:
  BYTE              m_cbHandCardCount[GAME_PLAYER];       //�˿���Ŀ
  BYTE              m_cbHandCardData[GAME_PLAYER][MAX_COUNT]; //�����˿�

  //�������
protected:
  CGameLogic            m_GameLogic;                //��Ϸ�߼�
  ITableFrame           * m_pITableFrame;             //��ܽӿ�
  tagGameServiceAttrib *      m_pGameServiceAttrib;           //��Ϸ����
  tagGameServiceOption *      m_pGameServiceOption;           //��Ϸ����

  //�������
protected:
  HINSTANCE           m_hInst;
  IServerControl*         m_pServerControl;
  ICheatServer*         m_pCheatServer;
  WORD                            m_wAdminChairID;

  //���Ա���
protected:
  static const WORD       m_wPlayerCount;               //��Ϸ����
  static const BYTE       m_GameStartMode;              //��ʼģʽ
  //������Ϣ
protected:
  int               m_nScoreGrade;
  LONGLONG            m_lScoreList[MAX_SCOREGRADE][13];     //�����б�
  TCHAR             m_szGameRoomName[LEN_SERVER];       //��������
  TCHAR               m_szConfigFileName[MAX_PATH];       //�����ļ�
  int                             m_nBombPercent[11];             //ը������
  int                             m_nKingPercent[7];              //���ļ���
  //��������
public:
  //���캯��
  CTableFrameSink();
  //��������
  virtual ~CTableFrameSink();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID Release()
  {
    delete this;
  }
  //�ӿڲ�ѯ
  virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);


  //����ӿ�
public:
  //��ʼ��
  virtual bool Initialization(IUnknownEx * pIUnknownEx);
  //��λ����
  virtual VOID RepositionSink();


  //��Ϣ�ӿ�
public:
  //��ʼģʽ
  virtual BYTE GetGameStartMode();
  //��Ϸ״̬
  virtual bool IsUserPlaying(WORD wChairID);

  //��Ϸ�¼�
public:
  //��Ϸ��ʼ
  virtual bool OnEventGameStart();
  //��Ϸ����
  virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
  //���ͳ���
  virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

  //�¼��ӿ�
public:
  //��ʱ���¼�
  virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
  //�����¼�
  virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);
  //��Ϸ��Ϣ����
  virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //�����Ϣ����
  virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

  //�û��¼�
public:
  //�û�����
  virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //�û�����
  virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //�û�����
  virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�����
  virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�ͬ��
  virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize);
  //��ѯ�ӿ�
public:
  //��ѯ�޶�
  virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //���ٻ���
  virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //�����¼�
  virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
  {
    return false;
  }
  //�����¼�
  virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
  {
    return false;
  }
  //��ѯ�Ƿ�۷����
  virtual bool QueryBuckleServiceCharge(WORD wChairID)
  {
    return true;
  }
  //�����ӿ�
public:
  //���û���
  virtual void SetGameBaseScore(LONG lBaseScore) {}


  //��Ϸ�¼�
protected:
  //�û�����
  bool OnSubPassCard(WORD wChairID);
  //�û��й�
  bool OnSubTrustee(WORD wChairID,bool bTrustee);
  //�û�����
  bool OnSubOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount);
  //��ҳ���
  bool OnSubReqHunter(WORD wChairID,IServerUserItem * pIServerUserItem);

  //�����û�
  bool OnCheatCard(IServerUserItem * pIServerUserItem);
  //����������Ϣ
  void SendCheatCard();

  //��������
protected:
  //ը��ת��
  bool OnBombTrans(BYTE cbBombInfo[]);
  //��ȡ��ֵȼ�
  BYTE GetUserMinGrade(BYTE& cbGrade);
  //��ȡ���׷�
  bool GetBombScore(LONGLONG *lBombScore);
  //����÷�
  bool CalculationRates(tagScoreInfo ScoreInfo[]);
  //������Ϣ
  bool SendTableMessage(LPCTSTR lpszMessage, WORD wMessageType);
  //��ȡ����
  bool ReadConfigInformation(bool bReadFresh);

  //˽�к���
private:
  //���ը����������
  BYTE GetRandBombSetIndex();
  //���ƿ���
  void DispatchTableCard();
  //�õ�����
  BYTE GetRandomKingCount();
  //��̽���ж�
  bool ProbeJudge(int ControlType);
  //�������ϵ��Ƶõ�ը���б�
  void GetBombListByHandCard(BYTE cbBombList[GAME_PLAYER][BOMB_TYPE_COUNT]);
};

//////////////////////////////////////////////////////////////////////////

#endif