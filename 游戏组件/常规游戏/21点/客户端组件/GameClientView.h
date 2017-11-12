#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "JettonControl.h"
#include "CControlWnd.h"
#include "GameLogic.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_START         WM_USER+100             //��ʼ��Ϣ
#define IDM_ADD_SCORE       WM_USER+101             //��ע��Ϣ
#define IDM_GET_CARD        WM_USER+102             //Ҫ����Ϣ  
#define IDM_STOP_CARD       WM_USER+103             //ͣ����Ϣ
#define IDM_DOUBLE_CARD       WM_USER+104             //�ӱ���Ϣ
#define IDM_SPLIT_CARD        WM_USER+105             //������Ϣ
#define IDM_INSURE_CARD       WM_USER+106             //������Ϣ
#define IDM_SEND_CARD_FINISH    WM_USER+107             //�������
//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
  WORD              wChairID;             //�����û�
  BYTE              cbCardData;             //��������
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
protected:
  LONGLONG            m_lCellScore;           //��Ԫע

  BYTE              m_cbUserCardType[GAME_PLAYER*2];  //
  BYTE              m_bStopCard[GAME_PLAYER*2];     //�Ƿ�ͣ��
  BYTE              m_bDoubleCard[GAME_PLAYER*2];   //�Ƿ�ӱ�
  BYTE              m_bInsureCard[GAME_PLAYER*2];   //�Ƿ���

  BYTE              m_cbWaitStatus;           //�ȴ���ʶ

  WORD              m_wBankerUser;            //ׯ��

  //
  CPoint              m_ptBanker[GAME_PLAYER];      //

  //��������
protected:
  CPoint              m_SendCardPos;            //����λ��
  CPoint              m_SendCardCurPos;         //���Ƶ�ǰλ��
  INT               m_nStepCount;           //����
  INT               m_nXStep;             //����
  INT               m_nYStep;             //����
  CSendCardItemArray        m_SendCardItemArray;        //��������

  //��ť�ؼ�
public:
  CSkinButton           m_btStart;              //��ʼ��ť
  CSkinButton           m_btGetCard;            //Ҫ�ư�ť
  CSkinButton           m_btStopCard;           //ͣ�ư�ť
  CSkinButton           m_btDoubleCard;           //�ӱ���ť
  CSkinButton           m_btSplitCard;            //���ư�ť
  CSkinButton           m_btInsureCard;           //���հ�ť

  //�ؼ�����
public:
  CScoreView            m_ScoreView;            //�ɼ�����
  CCardControl          m_CardControl[GAME_PLAYER*2];   //�û��˿�
  CJettonControl          m_PlayerJeton[GAME_PLAYER];     //��ҳ���
  CControlWnd           m_GoldControl;            //��ע��
  CGameLogic            m_GameLogic;            //

  //����
public:
  HINSTANCE           m_hInst;
  IClientControlDlg*        m_pClientControlDlg;
  CButton             m_btOpenAdmin;            //ϵͳ����

  //�������
protected:
  CBitImageEx           m_ImageCard;            //�˿���Դ
  CBitImageEx           m_ImageViewBack;          //������Դ
  CBitImageEx           m_ImageViewFill;          //�����Դ
  CBitImageEx           m_ImageCellScore;         //��Ԫ����
  CBitImageEx           m_ImageBanker;            //
  CBitImageEx           m_ImageWaitBanker;          //
  CBitImageEx           m_ImageWaitOthers;          //
  CBitImageEx           m_ImageWaitScore;         //
  CPngImageEx           m_PngNumber;            //����λͼ
  CPngImageEx           m_PngCellNumber;          //��Ԫ����
  CPngImageEx           m_PngCardStatus;          //
  CPngImageEx           m_PngCardStatusBack;        //
  CPngImageEx           m_PngCardValue;           //
  CPngImageEx           m_PngCardValueTip;          //
  CPngImageEx           m_PngCardArrow;           //
  //��������
public:
  //���캯��
  CGameClientView();
  //��������
  virtual ~CGameClientView();
protected:
  //�����
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  //���غ���
private:
  //���ý���
  virtual void ResetGameView();
  //�����ؼ�
  virtual VOID RectifyControl(INT nWidth, INT nHeight);
  //�滭����
  virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

  virtual bool RealizeWIN7()
  {
    return true;
  }

  //���ܺ���
public:
  //�����ж�
  bool IsDispatchCard();
  //��ɷ���
  void FinishDispatchCard();
  //�����˿�
  void DispatchUserCard(WORD wChairID, BYTE cbCardData);
  //���õ�Ԫע
  void SetCellScore(LONGLONG lCellScore);
  //���ͣ��
  void SetUserStopCard(WORD wChairID);
  //��Ҽӱ�
  void SetUserDoubleCard(WORD wChairID);
  //����±���
  void SetUserInsureCard(WORD wChairID, WORD wOperIndex);
  //���õȴ�
  void SetWaitStatus(BYTE cbWaitStatus);
  //����ׯ��
  void SetBankerUser(WORD wChairId);
  //
  void SetUserCardType(WORD wChairId, WORD wOperIndex, BYTE cbCardType);

  //��������
protected:
  ////��������
  //void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
  ////��������
  //void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
  //������
  void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, CPngImageEx &PngNumber, bool bHasSign);

  //��Ϣӳ��
protected:
  //��������
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //��ʱ����Ϣ
  afx_msg void OnTimer(UINT nIDEvent);
  //����Ա����
  afx_msg void OpenAdminWnd();

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif