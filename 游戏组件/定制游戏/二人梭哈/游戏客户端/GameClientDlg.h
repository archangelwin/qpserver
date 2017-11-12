#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
  //��Ϸ����
protected:
  LONGLONG              m_lTurnMaxGold;           //�����ע
  LONGLONG              m_lTurnBasicGold;         //��ע��Ŀ
  LONGLONG              m_lBasicGold;           //��Ԫ��Ŀ
  BYTE              m_bPlayStatus[GAME_PLAYER];     //��Ϸ״̬
  WORD              m_wCurrentUser;           //
  LONGLONG              m_lShowHandScore;         //������߷�
  LONGLONG              m_lUserScore[GAME_PLAYER];      //


  //��������
protected:
  bool              m_bShowHand;            //�Ƿ����
  LONGLONG              m_lGoldShow;            //��ע����
  TCHAR             m_szName[GAME_PLAYER][LEN_ACCOUNTS];  //�������
  CMD_S_GameEnd         m_GameEnd;              //

  //�ؼ�����
public:
  CGameClientView         m_GameClientView;         //��Ϸ��ͼ
  CGlobalUnits *                  m_pGlobalUnits;                         //ȫ�ֵ�Ԫ

  //��������
public:
  //���캯��
  CGameClientEngine();
  //��������
  virtual ~CGameClientEngine();

  //����̳�
private:
  //��ʼ����
  virtual bool OnInitGameEngine();
  //���ÿ��
  virtual bool OnResetGameEngine();
  //��Ϸ����
  virtual void OnGameOptionSet();

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

  //��Ϣ����
protected:
  //��Ϸ��ʼ
  bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
  //�û���ע
  bool OnSubAddGold(const void * pBuffer, WORD wDataSize);
  //�û�����
  bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
  //������Ϣ
  bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
  //��Ϸ����
  bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
  //����
  bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);
  //����
  bool OnSubAdminControlEx(const void * pBuffer, WORD wDataSize);

  //��Ϣӳ��
protected:
  //��ʼ��ť
  LRESULT OnStart(WPARAM wParam, LPARAM lParam);
  //������ť
  LRESULT OnGiveUp(WPARAM wParam, LPARAM lParam);
  //��ע��ť
  LRESULT OnFollow(WPARAM wParam, LPARAM lParam);
  //�����ť
  LRESULT OnShowHand(WPARAM wParam, LPARAM lParam);
  //��ע��Ϣ
  LRESULT OnAddGold(WPARAM wParam, LPARAM lParam);
  //�������
  LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
  //����
  LRESULT OnAdminControl(WPARAM wParam, LPARAM lParam);
  //����
  LRESULT OnAdminControlEx(WPARAM wParam, LPARAM lParam);

private:
  //
  void UpdateScoreControl();
  //
  void HideScoreControl();
  //
  void PerformGameEnd();

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
