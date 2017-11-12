#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϸ������\GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
  //��Ϸ����
protected:
  WORD              m_wBankerUser;            //ׯ���û�
  WORD              m_wOutCardUser;           //�������
  WORD              m_wCurrentUser;           //��ǰ�û�

  //��������
protected:
  tagAnalyseResult        m_OutCardInfo;            //��������
  tagAnalyseResult        m_LastOutCard;            //������

  //�˿˱���
protected:
  BYTE              m_cbTurnCardCount;          //������Ŀ
  BYTE              m_cbTurnCardData[MAX_COUNT];    //�����б�




  //�����˿�
protected:
  BYTE              m_cbHandCardData[GAME_PLAYER][MAX_COUNT]; //�����˿�
  BYTE              m_cbHandCardCount[GAME_PLAYER];   //�˿���Ŀ

  //��ʷ�˿�
protected:

  //�ؼ�����
protected:
  CGameLogic            m_GameLogic;            //��Ϸ�߼�
  IAndroidUserItem *        m_pIAndroidUserItem;        //�û��ӿ�

  //��������
public:
  //���캯��
  CAndroidUserItemSink();
  //��������
  virtual ~CAndroidUserItemSink();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID  Release()
  {
    delete this;
  }
  //�ӿڲ�ѯ
  virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);

  //���ƽӿ�
public:
  //��ʼ�ӿ�
  virtual bool  Initialization(IUnknownEx * pIUnknownEx);
  //���ýӿ�
  virtual bool  RepositionSink();

  //��Ϸ�¼�
public:
  //ʱ����Ϣ
  virtual bool OnEventTimer(UINT nTimerID);
  //��Ϸ��Ϣ
  virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
  //��Ϸ��Ϣ
  virtual bool OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
  //������Ϣ
  virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

  //�û��¼�
public:
  //�û�����
  virtual VOID OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
  //�û��뿪
  virtual VOID OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
  //�û�����
  virtual VOID OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
  //�û�״̬
  virtual VOID OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

  //��Ϣ����
protected:
  //��Ϸ��ʼ
  bool OnSubGameStart(const void * pData, WORD wDataSize);
  //�û�����
  bool OnSubOutCard(const void * pData, WORD wDataSize);
  //�û�����
  bool OnSubPassCard(const void * pData, WORD wDataSize);
  //��Ϸ����
  bool OnSubGameEnd(const void * pData, WORD wDataSize);

private:
  //�����ж�
  bool VerdictOutCard(BYTE cbOutCardData[], BYTE cbOutCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif