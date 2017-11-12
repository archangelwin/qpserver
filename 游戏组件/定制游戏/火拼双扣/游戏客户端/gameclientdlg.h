#pragma once

#include "Stdafx.h"
#include "..\��Ϸ������\GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////


//�����ṹ
struct SearchInfo
{
  BYTE              cbCurIndex[4];              //��������
  BYTE                            cbCurSelectFunc;            //ѡ����
};

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
  //��Ϸ����
protected:
  WORD              m_wBankerUser;            //ׯ���û�
  bool              m_bReqHumter;           //��������
  bool                            m_bHaveOutCard;           //�Ѿ�����

  bool              m_bUserCheat[GAME_PLAYER];      //���ױ�־
  //��ʼ��Ϣ
protected:
  WORD              m_wGetRandCardID;         //�����û�
  BYTE              m_cbGetCardPosition;        //����λ��
  BYTE              m_cbRandCardData;         //����˿�

  //��Ϸ����
protected:
  WORD              m_wCurrentUser;           //��ǰ�û�

  //ʤ����Ϣ
protected:
  WORD              m_wWinOrder;            //ʤ�����

  //�˿˱���
protected:
  BYTE              m_cbHandCardData[MAX_COUNT];    //�����˿�
  BYTE              m_cbUserCardData[GAME_PLAYER][MAX_COUNT];   //�����˿�
  BYTE              m_cbHandCardCount[GAME_PLAYER];   //�˿���Ŀ
  //���Ʊ���
protected:
  BYTE              m_cbTurnCardCount;          //������Ŀ
  BYTE              m_cbTurnCardData[MAX_COUNT];    //�����б�
  tagAnalyseResult        m_OutCardInfo;            //��������
  tagAnalyseResult        m_LastOutCard;            //������

  SearchInfo            m_SearchInfo;           //������Ϣ

  //�����˿�
protected:
  bool              m_bLastTurn;                //���ֱ�־
  BYTE              m_cbOutCardCount[GAME_PLAYER][2];     //������Ŀ
  BYTE              m_cbOutCardData[GAME_PLAYER][2][MAX_COUNT]; //�����б�


  //���ñ���
protected:
  bool              m_bStustee;             //�йܱ�־
  bool              m_bSortCount;           //��Ŀ����
  bool              m_bHaveVoiceCard;                   //�������
  DWORD             m_dwCardHSpace;           //�˿�����

  //��������
protected:
  WORD              m_wMostUser;            //������
  WORD              m_wTimeOutCount;          //��ʱ����
  BYTE              m_cbRemnantCardCount;       //ʣ����Ŀ
  BYTE              m_cbDispatchCardCount;        //�ɷ���Ŀ
  BYTE              m_cbDispatchCardData[MAX_COUNT];  //�ɷ��˿�
  //�ؼ�����
protected:
  CGameLogic            m_GameLogic;            //��Ϸ�߼�
  CGameClientView         m_GameClientView;         //��Ϸ��ͼ
  //CGlobalUnits *                  m_pGlobalUnits;                         //ȫ�ֵ�Ԫ

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
  //�û�״̬
  virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);

  //��ʱ������
protected:
  bool  OnTimerDispatchCard();
  //��Ϣ����
protected:
  //��Ϸ��ʼ
  bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
  //�û�����
  bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
  //��������
  bool OnSubPassCard(const void * pBuffer, WORD wDataSize);
  //��Ϸ����
  bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
  //�û��й�
  bool OnSubTrustee(const void * pBuffer,WORD wDataSize);
  //�û�׼��
  bool OnSubUserReady(const void * pBuffer,WORD wDataSize);
  //����
  bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);

  //��������
  bool OnSubCheatCard(VOID * pData, WORD wDataSize);

  //��������
protected:
  //�����ж�
  bool VerdictOutCard();
  //�����ж�
  bool VerdictPassCard();
  //�Զ�����
  bool AutomatismOutCard();
  //��ǰ�˿�
  bool SwitchToCurrentCard();
  //ֹͣ����
  bool DoncludeDispatchCard();
  //ը������
  void PlayBombSound(WORD wChairID, BYTE cbStarLevel);
  //�ɷ��˿�
  bool DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount);
  //�����ж�
  bool OnCanReqHunter(BYTE cbCardData[],BYTE cbCardCount);
  //���ų�������
  bool PlayCardSound(WORD wChairID,tagAnalyseResult &OutCard);
  //��ȡ�˿���Ϣ
  CString GetCardInfo(BYTE cbCardData);
  //��Ϣӳ��
protected:
  //��ʱ����Ϣ
  afx_msg void OnTimer(UINT nIDEvent);
  //��ʼ��Ϣ
  LRESULT OnStart(WPARAM wParam, LPARAM lParam);
  //������ʾ
  LRESULT OnCardType(WPARAM wParam,LPARAM lParam);
  //������Ϣ
  LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
  //��������
  LRESULT OnPassCard(WPARAM wParam, LPARAM lParam);
  //������ʾ
  LRESULT OnOutPrompt(WPARAM wParam, LPARAM lParam);
  //���볭��
  LRESULT OnReqHunter(WPARAM wParam, LPARAM lParam);
  //��Ŀ����
  LRESULT OnSortByCount(WPARAM wParam, LPARAM lParam);
  //�Ҽ��˿�
  LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
  //����˿�
  LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
  //���˫��
  LRESULT OnLeftDBHitCard(WPARAM wParam, LPARAM lParam);
  //�����˿�
  LRESULT OnLastTurnCard(WPARAM wParam, LPARAM lParam);
  //��Ƶ����ʧ��
  //LRESULT OnVoiceCreateFail(WPARAM wParam,LPARAM lParam);
  //�Ϲܿ���
  LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
  //����
  LRESULT  OnAdminControl(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
