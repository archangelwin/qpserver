#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientEngine : public CGameFrameEngine
{
  //���ñ���
protected:
  DWORD             m_dwCardHSpace;             //�˿�����

  WORD              m_wBankerUser;              //ׯ��

  BYTE              m_bStopCard[GAME_PLAYER*2];       //�Ƿ�ͣ��
  BYTE              m_bInsureCard[2];           //�Ƿ���
  BYTE              m_bDoubleCard[2];           //
  BYTE              m_cbCardCount[2];           //��������Ŀ
  BYTE              m_cbHandCardData[2][MAX_COUNT];     //������

  BYTE              m_bTurnBanker;              //�Ƿ��ֵ�ׯ��
  BYTE              m_bSpliting;              //�Ƿ����ڷ���
  WORD              m_wSplitingUser;            //���ڷ������

  //��ע��Ϣ
  LONGLONG            m_lMaxScore;              //�����ע
  LONGLONG            m_lCellScore;             //��Ԫ��ע

  //״̬����
  BYTE              m_cbPlayStatus[GAME_PLAYER];      //��Ϸ״̬
  TCHAR             m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS];  //�������

  //�������
public:
  CGameClientView         m_GameClientView;         //��Ϸ��ͼ
  CGameLogic            m_GameLogic;              //��Ϸ�߼�
  //��������
public:
  //���캯��
  CGameClientEngine();
  //��������
  virtual ~CGameClientEngine();

  //ϵͳ�¼�
public:
  //��������
  virtual bool OnInitGameEngine();
  //���ú���
  virtual bool OnResetGameEngine();

  //ʱ���¼�
public:
  //ʱ��ɾ��
  virtual bool OnEventGameClockKill(WORD wChairID);
  //ʱ����Ϣ
  virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

  //��Ϸ�¼�
public:
  //�Թ���Ϣ
  virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
  //��Ϸ��Ϣ
  virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
  //������Ϣ
  virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
  //������Ϣ
  virtual bool OnEventInsureMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
  {
    return true;
  }

  //��Ϣ����
protected:
  //��Ϸ��ʼ
  bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
  //������Ϣ
  bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
  //��Ϸ����
  bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
  //��ע��Ϣ
  bool OnSubAddScore(const void *pBuffer, WORD wDataSize);
  //ͣ����Ϣ
  bool OnSubStopCard(const void *pBuffer, WORD wDataSize);
  //�ӱ���Ϣ
  bool OnSubDoubleCard(const void *pBuffer, WORD wDataSize);
  //������Ϣ
  bool OnSubSplitCard(const void *pBuffer, WORD wDataSize);
  //������Ϣ
  bool OnSubInsureCard(const void *pBuffer, WORD wDataSize);
  //Ҫ����Ϣ
  bool OnSubGetCard(const void *pBuffer, WORD wDataSize);

  bool OnSubGMCard(const void * pData, WORD wDataSize);

  //��������
protected:
  //���ؿ���
  void HideScoreControl();
  //���¿���
  BOOL UpdateScoreControl();

  //��Ϣӳ��
protected:
  //��ʼ��ť
  LRESULT OnStart(WPARAM wParam, LPARAM lParam);
  //ͣ�ư�ť
  LRESULT OnStopCard(WPARAM wParam, LPARAM lParam);
  //�ӱ���ť
  LRESULT OnDoubleCard(WPARAM wParam, LPARAM lParam);
  //���հ�ť
  LRESULT OnInsureCard(WPARAM wParam, LPARAM lParam);
  //���ư�ť
  LRESULT OnSplitCard(WPARAM wParam, LPARAM lParam);
  //Ҫ�ư�ť
  LRESULT OnGetCard(WPARAM wParam, LPARAM lParam);
  //��ע��ť
  LRESULT OnAddScore(WPARAM wParam, LPARAM lParam);

  //�������
  LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
  LRESULT OnStorage(WPARAM wParam, LPARAM lParam);
  LRESULT OnStorageInfo(WPARAM wParam, LPARAM lParam);
  LRESULT OnRequestQueryUser(WPARAM wParam, LPARAM lParam);
  //�û�����
  LRESULT OnUserControl(WPARAM wParam, LPARAM lParam);
  //������·�����Ϣ
  LRESULT OnRequestUpdateRoomInfo(WPARAM wParam, LPARAM lParam);
  //��յ�ǰ��ѯ�û�
  LRESULT OnClearCurrentQueryUser(WPARAM wParam, LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif