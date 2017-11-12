#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "DlgCustomRule.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////

//����ԭ��
#define GER_NO_PLAYER           0x10            //û�����

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
  //��Ϸ����
protected:
  WORD              m_wOperaCount;              //��������
  WORD              m_wCurrentUser;             //��ǰ�û�
  BYTE              m_bSendCardCount;           //������Ŀ
  BYTE              m_cbCardCount[GAME_PLAYER];       //�˿���Ŀ
  BYTE              m_cbHandCardData[GAME_PLAYER][MAX_COUNT];   //�����˿�
  BYTE              m_cbGameEndReason;            //��Ϸ��������
  bool              m_bGameEnd;               //�Ƿ����
  bool              m_bShowHand;              //�Ƿ����
  bool              m_bAndroidLose;             //����������С�����

  //�����Ϣ
protected:
  LONGLONG              m_lPlayBeginGold[GAME_PLAYER];      //��ҿ�ʼ����
  LONGLONG              m_lMaxGold;               //�����ע
  LONGLONG              m_lTurnMaxGold;             //���������ע
  LONGLONG              m_lBasicGold;             //��Ԫ��ע
  LONGLONG              m_lTurnBasicGold;           //������ע
  LONGLONG              m_lTableGold[2*GAME_PLAYER];      //��ע���
  LONGLONG              m_lUserScore[GAME_PLAYER];        //��ҽ��
  LONGLONG              m_lLostScore[GAME_PLAYER];        //��ҿ۷�

  //��������
protected:
  BYTE              m_cbPlayStatus[GAME_PLAYER];      //��Ϸ״̬

  //AI����
protected:
  WORD              m_wTableCount;              //������Ŀ
  WORD              m_wMeTableID;             //���Ӻ���
  static LONGLONG *       m_plStockScore;             //����Ӯ��
  LONGLONG            m_lStorageDeduct;           //�ؿ۱���
  LONGLONG            m_lStockLimit;              //����Ӯ��
  LONGLONG            m_lStorageMin;              //�������
  LONGLONG            m_lStorageMax;              //���ⶥ
  int               m_nStorageMul;              //ϵͳ��Ǯ����

  //�������
protected:
  HINSTANCE           m_hControlInst;
  IServerControl*         m_pServerContro;

  //�������
protected:
  CGameLogic            m_GameLogic;              //��Ϸ�߼�
  ITableFrame           * m_pITableFrame;           //��ܽӿ�
  const tagGameServiceOption    * m_pGameServiceOption;         //���ò���
  tagGameServiceAttrib *      m_pGameServiceAttrib;         //��Ϸ����
  tagCustomRule *         m_pGameCustomRule;            //�Զ�����

  //���Ա���
protected:
  static const WORD       m_wPlayerCount;             //��Ϸ����
  static const BYTE       m_GameStartMode;            //��ʼģʽ

  //��������
public:
  //���캯��
  CTableFrameSink();
  //��������
  virtual ~CTableFrameSink();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID  Release();
  //�Ƿ���Ч
  virtual bool  IsValid()
  {
    return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false;
  }
  //�ӿڲ�ѯ
  virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

  //����ӿ�
public:
  //��ʼ��
  virtual bool Initialization(IUnknownEx * pIUnknownEx);
  //��λ����
  virtual VOID RepositionSink();

  //��ѯ�ӿ�
public:
  //��ѯ�޶�
  virtual LONGLONG QueryConsumeQuota(IServerUserItem * pIServerUserItem);
  //���ٻ���
  virtual LONGLONG QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
  //��ѯ�Ƿ�۷����
  virtual bool QueryBuckleServiceCharge(WORD wChairID)
  {
    return true;
  }

  //�����ӿ�
public:
  //���û���
  virtual void SetGameBaseScore(LONG lBaseScore) {};


  //��Ϣ�ӿ�
public:
  //��ʼģʽ
  virtual BYTE  GetGameStartMode();
  //��Ϸ״̬
  virtual bool  IsUserPlaying(WORD wChairID);

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
  //ʱ���¼�
  virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
  //�����¼�
  virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
  //�����¼�
  virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

  //����ӿ�
public:
  //��Ϸ��Ϣ
  virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //�����Ϣ
  virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

  //�û��¼�
public:
  //�û�����
  virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //�û�����
  virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //�û�����
  virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�����
  virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�ͬ��
  virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
  {
    return true;
  }

  //��Ϸ�¼�
protected:
  //�����¼�
  bool OnUserGiveUp(WORD wChairID,bool bLeave=false);
  //��ע�¼�
  bool OnUserAddGold(WORD wChairID, LONGLONG lGold, bool bCancel);

  //��Ϸ����
protected:
  //�ƶ�ʤ��
  WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
  //�˿˷���
  bool AnalyseCard();
  //�˿˷���
  bool AnalyseCardEX();
};

//////////////////////////////////////////////////////////////////////////

#endif
