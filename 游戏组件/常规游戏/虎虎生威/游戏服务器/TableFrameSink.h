#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY     65                  //��ʷ����
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
  //����ע��
protected:
  LONGLONG            m_lAllJettonScore[AREA_COUNT+1];  //ȫ����ע

  //������ע
protected:
  LONGLONG            m_lUserJettonScore[AREA_COUNT+1][GAME_PLAYER];//������ע

  //���Ʊ���
protected:
  LONGLONG            m_lAreaLimitScore;            //��������
  LONGLONG            m_lUserLimitScore;            //��������
  LONGLONG            m_lApplyBankerCondition;        //��������

  //��ҳɼ�
protected:
  LONGLONG            m_lUserWinScore[GAME_PLAYER];     //��ҳɼ�
  LONGLONG            m_lUserReturnScore[GAME_PLAYER];    //������ע
  LONGLONG            m_lUserRevenue[GAME_PLAYER];      //���˰��
  BYTE              m_cbLeftCardCount;            //�˿���Ŀ
  bool              m_bContiueCard;             //��������
  BYTE              m_bcFirstPostCard;            //���°l���c��
  LONGLONG            m_lUserTotalScore[GAME_PLAYER];     //����ܳɼ�

  //���Ʋ���
protected:
  BYTE              m_cbControlArea;            //��������
  BYTE              m_cbControlTimes;           //���ƴ���

  //�˿���Ϣ
protected:
  BYTE              m_cbCardCount[1];           //�˿���Ŀ
  BYTE              m_cbTableCardArray[1][1];       //�����˿�
  BYTE              m_cbTableCard[2000];        //�����˿�
  int               m_TableCardLen;

  //״̬����
protected:
  DWORD             m_dwJettonTime;             //��ʼʱ��

  //ׯ����Ϣ
protected:
  CWHArray<WORD>          m_ApplyUserArray;           //�������
  WORD              m_wCurrentBanker;           //��ǰׯ��
  WORD              m_wBankerTime;              //��ׯ����
  LONGLONG            m_lBankerWinScore;            //�ۼƳɼ�
  LONGLONG            m_lBankerCurGameScore;          //��ǰ�ɼ�
  bool              m_bEnableSysBanker;           //ϵͳ��ׯ
  WORD              m_wBankerUser;              //��ׯ���
  LONGLONG            m_lBankerGold;              //��ҵ�һ����ׯʱ�Ľ��
  LONGLONG            m_lMinPercent;          //�����ׯ����Ǯ�İٷֱ���Сֵ
  LONGLONG            m_lMaxPercent;          //�����ׯ����Ǯ�İٷֱ����ֵ
  LONGLONG            m_lUserBankerPercent;         //�����ׯ����Ǯ�����հٷֱ�
  LONGLONG            m_DispatchGold;             //ÿ��������ɲʱ���
  LONGLONG            m_lBankerScore;             //

  //��¼����
protected:
  tagServerGameRecord       m_GameRecordArrary[MAX_SCORE_HISTORY];  //��Ϸ��¼
  int               m_nRecordFirst;             //��ʼ��¼
  int               m_nRecordLast;              //����¼
  CFile             m_FileRecord;             //��¼���
  DWORD             m_dwRecordCount;            //��¼��Ŀ
  int               m_CheckImage;
  LONGLONG            m_lStartStorage;            //��ʼ���
  LONGLONG            m_lServiceScore;            //�������

  //���Ʊ���
protected:
  LONGLONG            m_StorageStart;             //��������ÿ���ӵĿ����ֵ����ȡʧ�ܰ� 0 ����
  LONGLONG            m_StorageDeduct;            //ÿ����Ϸ������۳��Ŀ���������ȡʧ�ܰ� 1.00 ����
  LONGLONG            m_StorageDispatch;            //�ɲʱ���
  LONGLONG            m_StartScore;             //��¼����ʼֵ
  TCHAR             m_szConfigFileName[MAX_PATH];     //�����ļ�
  TCHAR             m_szGameRoomName[SERVER_LEN];     //��������
  LONGLONG            m_lControlStorage;            //���ƺ�����д��
  TCHAR             m_szControlName[LEN_NICKNAME];      //��������

  // 11/11/2013 [Wan.MY]
  LONGLONG            m_lStorageMax[2];             //���ⶥ
  WORD              m_wStorageMul[2];             //ϵͳ��Ǯ��¼

  //�����˿���
protected:
  int               m_nMaxChipRobot;            //�����Ŀ (��ע������)
  int               m_nChipRobotCount;            //����ͳ�� (��ע������)
  LONGLONG            m_lRobotAreaLimit;            //��������������
  LONGLONG            m_lRobotBetCount;           //��������ע����
  LONGLONG            m_lRobotAreaScore[AREA_COUNT+1];    //������������ע
  int               m_nRobotListMaxCount;       //�������

  //ׯ������
protected:
  //��ׯ�������ã���ׯ�������趨�ľ���֮��(m_lBankerMAX)��
  //�������ֵ��������������ׯ���б�����������ҽ��ʱ��
  //�����ټ���ׯm_lBankerAdd�֣���ׯ���������á�
  LONGLONG            m_lBankerMAX;             //���ׯ����
  LONGLONG            m_lBankerAdd;             //ׯ��������

  //��ҳ���m_lBankerScoreMAX֮�󣬾�����������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_lBankerScoreAdd�֡�
  LONGLONG            m_lBankerScoreMAX;            //ׯ��Ǯ
  LONGLONG            m_lBankerScoreAdd;            //ׯ��Ǯ��ʱ,��ׯ������

  //���ׯ����
  LONGLONG            m_lPlayerBankerMAX;           //������ׯ����

  //��ׯ
  bool              m_bExchangeBanker;            //����ׯ��

  //ʱ������
protected:
  BYTE              m_cbFreeTime;             //����ʱ��
  BYTE              m_cbBetTime;              //��עʱ��
  BYTE              m_cbEndTime;              //����ʱ��


  //�������
protected:
  //�������
protected:
  HINSTANCE           m_hInst;
  IServerControl*         m_pServerControl;
  bool              m_bControl;               //�Ƿ����

  CGameLogic            m_GameLogic;              //��Ϸ�߼�

  //����ӿ�
protected:
  ITableFrame *         m_pITableFrame;             //��ܽӿ�
  tagGameServiceOption *      m_pGameServiceOption;         //��������
  tagGameServiceAttrib *      m_pGameServiceAttrib;         //��Ϸ����

  //���Ա���
protected:
  static const WORD       m_wPlayerCount;             //��Ϸ����

  //��������
public:
  //���캯��
  CTableFrameSink();
  //��������
  virtual ~CTableFrameSink();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID  Release()
  {
    delete this;
  }

  //�ӿڲ�ѯ
  virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

  //����ӿ�
public:
  //��ʼ��
  virtual bool Initialization(IUnknownEx * pIUnknownEx);
  //��λ����
  virtual VOID RepositionSink();

  //��ѯ�ӿ�
public:
  //��ѯ�޶�
  virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
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
  virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
  //��Ϣ�ӿ�
public:
  //��Ϸ״̬
  virtual bool IsUserPlaying(WORD wChairID);


  //��Ϸ�¼�
public:
  //��Ϸ��ʼ
  virtual bool OnEventGameStart();
  //��Ϸ����
  virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
  //���ͳ���
  virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

  //�¼��ӿ�
public:
  //��ʱ���¼�
  virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
  //��Ϸ��Ϣ����
  virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //�����Ϣ����
  virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //�����¼�
  virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);
  //�۳������
  virtual bool QueryBuckleServiceCharge(WORD wChairID);
  //���û���
  virtual void SetGameBaseScore(LONG lBaseScore) {};

  //�����¼�
public:
  //�û�����
  virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;

  //�û�����
  virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }

  //�û�����
  virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�����
  virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�ͬ��
  virtual bool  OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
  {
    return true;
  }

#ifdef __SPECIAL___
  //���в���
public:
  virtual bool  OnActionUserBank(WORD wChairID, IServerUserItem * pIServerUserItem);////
#endif

  //����
protected:
  bool OnAdminControl(CMD_C_ControlApplication* pData, IServerUserItem * pIServerUserItem);

  //��Ϸ�¼�
protected:
  //��ע�¼�
  bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore);
  //����ׯ��
  bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
  //ȡ������
  bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

  //��������
private:
  bool FindSuitBanker();
  //�����˿�
  bool DispatchTableCard();
  //����ׯ��
  void SendApplyUser(IServerUserItem *pServerUserItem);
  //����ׯ��
  bool ChangeBanker(bool bCancelCurrentBanker);
  //���ͼ�¼
  void SendGameRecord(IServerUserItem *pIServerUserItem);
  //������Ϣ
  void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
  //��ȡ����
  void ReadConfigInformation(bool bReadFresh);

  //��ע����
private:
  void   GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);
  //�����ע
  LONGLONG GetUserMaxJetton(WORD wChairID, BYTE Area);

  void RandList(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //�Ƿ�˥��
  bool NeedDeductStorage();

  //��Ϸͳ��
private:
  //����÷�
  LONGLONG CalculateScore();
  //��̽���ж�
  bool ProbeJudge(bool& bSystemLost);
  // ��¼����
  void WriteInfo(LPCTSTR pszString);
  // ����Ա������־
  void AdminOperateInfo(LPCTSTR pszString,IServerUserItem *pRcvServerUserItem);

};

//////////////////////////////////////////////////////////////////////////

#endif
