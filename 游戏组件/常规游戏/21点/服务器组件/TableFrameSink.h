#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ServerControl.h"
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
  //��Ϸ����
protected:
  WORD              m_wBankerUser;              //ׯ��

  SCORE             m_lTableScore[GAME_PLAYER*2];     //������ע
  SCORE             m_lInsureScore[GAME_PLAYER*2];      //���ս�

  BYTE              m_cbPlayStatus[GAME_PLAYER];      //���״̬

  BYTE              m_bStopCard[GAME_PLAYER*2];       //�Ƿ�ͣ��
  BYTE              m_bDoubleCard[GAME_PLAYER*2];     //�Ƿ�ӱ�
  BYTE              m_bInsureCard[GAME_PLAYER*2];     //�Ƿ���

  //�˿˱���
protected:
  BYTE              m_cbCardCount[GAME_PLAYER*2];       //�˿���Ŀ
  BYTE              m_cbHandCardData[GAME_PLAYER*2][MAX_COUNT]; //�����˿�
  BYTE              m_cbRepertoryCard[FULL_COUNT];      //ʣ���˿�
  BYTE              m_cbRepertoryCount;           //ʣ���˿���

  //��ע��Ϣ
protected:
  SCORE             m_lMaxScore;              //�����ע
  SCORE             m_lCellScore;             //��Ԫ��ע
  SCORE             m_lMaxUserScore[GAME_PLAYER];
  SCORE             m_lTableScoreAll[GAME_PLAYER];

  //�������
protected:
  CGameLogic            m_GameLogic;              //��Ϸ�߼�
  ITableFrame           * m_pITableFrame;           //��ܽӿ�
  tagGameServiceOption *      m_pGameServiceOption;         //��Ϸ����
  tagGameServiceAttrib *      m_pGameServiceAttrib;         //��Ϸ����

  //�������
protected:
  HINSTANCE           m_hControlInst;
  IServerControl*         m_pServerControl;

  //��������
public:
  //���캯��
  CTableFrameSink();
  //��������
  virtual ~CTableFrameSink();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID Release();
  //�ӿڲ�ѯ
  virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

  //����ӿ�
public:
  //��λ�ӿ�
  virtual VOID RepositionSink();
  //���ýӿ�
  virtual bool Initialization(IUnknownEx * pIUnknownEx);
  //�����ӿ�
public:
  //���û���
  virtual void SetGameBaseScore(LONG lBaseScore) {};

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
  //��Ϸ��Ϣ
  virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //�����Ϣ
  virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

  //�û��¼�
public:
  //�û�����
  virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
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
  virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
  {
    return true;
  }
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
  //��Ϸ�¼�
protected:
  //�����¼�
  bool OnUserStopCard(WORD wChairID);
  //��ע�¼�
  bool OnUserAddScore(WORD wChairID, LONGLONG lScore);
  //��Ҽӱ�
  bool OnUserDoubleScore(WORD wChairID);
  //��ҷ���
  bool OnUserSplitCard(WORD wChairID);
  //����±���
  bool OnUserInsure(WORD wChairID);
  //���Ҫ��
  bool OnUserGetCard(WORD wChairID);
  //�˿˷���
  bool AnalyseCard(bool bContron);
  //�˿˷���
  bool AnalyseCardEx();
  //�˿˷���
  bool AnalyseGetCard(WORD wChairID, BYTE cbOperateIndex);
  //���Ϳ��
  void SendStorage(WORD wChairID = INVALID_CHAIR);
  //������Ϣ
  void SendInfo(CString str, WORD wChairID = INVALID_CHAIR);
  //��ȡ����
  void ReadConfigInformation();
  //���·����û���Ϣ
  void UpdateRoomUserInfo(IServerUserItem *pIServerUserItem, USERACTION userAction);
  //����ͬ���û�����
  void UpdateUserControl(IServerUserItem *pIServerUserItem);
  //�����û�����
  void TravelControlList(ROOMUSERCONTROL keyroomusercontrol);
  //�Ƿ������������
  void IsSatisfyControl(ROOMUSERINFO &userInfo, bool &bEnableControl);
  //���������û�����
  bool AnalyseRoomUserControl(ROOMUSERCONTROL &Keyroomusercontrol, POSITION &ptList);
  //��ȡ��������
  void GetControlTypeString(CONTROL_TYPE &controlType, CString &controlTypestr);
  //д��־�ļ�
  void WriteInfo(LPCTSTR pszString);
};

//////////////////////////////////////////////////////////////////////////////////

#endif