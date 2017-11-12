#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "ClientControl.h"
//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_START           WM_USER+100           //��ʼ��Ϣ
#define IDM_ADD_GOLD          WM_USER+101           //��ע��Ϣ
#define IDM_FOLLOW            WM_USER+102           //��ע��Ϣ
#define IDM_GIVE_UP           WM_USER+103           //������Ϣ
#define IDM_SHOWHAND          WM_USER+104           //�����Ϣ
#define IDM_SEND_CARD_FINISH      WM_USER+106           //�������

#define MYSELF_VIEW_ID          1               //�Լ���λ

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

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
  //��Ԫ����
  friend class CGameClientDlg;

  //��ͼ����
protected:
  LONGLONG            m_lUserGold[GAME_PLAYER];     //
  LONGLONG            m_lMaxGold;             //�����ע
  CPoint              m_GoldStation[GAME_PLAYER*2];   //����λ��
  bool              m_bShowScoreControl;        //
  bool              m_bUserShowHand;          //
public:
  LONGLONG            m_lBasicGold;           //���ٷ���

  //��������
protected:
  CPoint              m_SendCardPos;            //����λ��
  CPoint              m_SendCardCurPos;         //���Ƶ�ǰλ��
  INT               m_nStepCount;           //����
  INT               m_nXStep;             //����
  INT               m_nYStep;             //����
  CSendCardItemArray        m_SendCardItemArray;        //��������

  INT               m_nXFace;
  INT               m_nYFace;
  INT               m_nXTimer;
  INT               m_nYTimer;
  INT               m_nXBorder;
  INT               m_nYBorder;

  //�������
protected:
  //CPngImage           m_ImageCardBack;          //�˿���Դ
  CBitImage           m_ImageCardBack;          //�˿���Դ
  CBitImage           m_ImageTitle;           //����λͼ
  CBitImage           m_ImageViewFill;          //����λͼ
  CBitImage           m_ImageViewCenter;          //����λͼ
  CPngImage           m_ImageScoreControlBack;      //
  CBitImage           m_ImageScoreTable;          //
  CPngImage           m_PngShowHand;            //
  CPngImage           m_ImageTimeBack;          //
  CPngImage           m_ImageTimeNumber;          //
  CPngImage           m_ImageNickBack;


  CDFontEx            m_DFontEx;              //����

  //�ؼ�����
public:
  CSkinButton           m_btStart;              //��ʼ��ť
  CSkinButton           m_btFollow;             //��ע��ť
  CSkinButton           m_btGiveUp;             //������ť
  CSkinButton           m_btShowHand;           //�����ť
  CSkinButton           m_btAddTimes3;            //
  CSkinButton           m_btAddTimes2;            //
  CSkinButton           m_btAddTimes1;            //
  CGoldView           m_GoldView[GAME_PLAYER*2];      //�û�����
  CGoldView           m_TotalGoldView;          //
  CScoreView            m_ScoreView;            //������ͼ
  CCardControl          m_CardControl[GAME_PLAYER];     //�˿˿ؼ�

  //�ؼ�λ��
protected:
  CPoint              m_ptScoreControl;         //
  //����
public:
  CButton             m_btOpenAdmin;            //ϵͳ����
  IClientControlDlg*        m_pClientControlDlg;
  HINSTANCE           m_hInst;

  CButton             m_btOpenAdminEx;            //ϵͳ����
  IClientControlDlg*        m_pClientControlDlgEx;

  //��������
public:
  //���캯��
  CGameClientView();
  //��������
  virtual ~CGameClientView();

  //�̳к���
private:
  //���ý���
  virtual VOID ResetGameView();
  //�����ؼ�
  virtual VOID RectifyControl(INT nWidth, INT nHeight);
  //�滭����
  virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
  //WIN7֧��
  virtual bool RealizeWIN7()
  {
    return true;
  }

  //���ܺ���
public:
  //��ұ���
  void SetGoldTitleInfo(LONGLONG lMaxGold, LONGLONG lBasicGold);
  //�û�����
  void SetUserGoldInfo(WORD wViewChairID, bool bTableGold, LONGLONG dwGold);
  //�����ж�
  bool IsDispatchCard();
  //��ɷ���
  void FinishDispatchCard();
  //�����˿�
  void DispatchUserCard(WORD wChairID, BYTE cbCardData);
  //
  VOID ShowScoreControl(bool bShow);
  //
  void SetUserShowHand(bool bShowHand);
  //
  void SetUserGold(WORD wChairId, LONGLONG lGold);
  //������ͼ
  void RefreshGameView();
  //����Ա����
  void OpenAdminWnd();
  //����Ա����
  void OpenAdminWndEx();

  //�滭����
public:
  // �滭����
  void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
  // �滭����
  void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
  // �滭����
  void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
  // �滭ʱ��
  void DrawTime(CDC * pDC, WORD wUserTime,INT nXPos, INT nYPos);


  //��Ϣӳ��
protected:
  //��ʼ��ť
  afx_msg void OnStart();
  //��ע��ť
  afx_msg void OnFollow();
  //������ť
  afx_msg void OnGiveUp();
  //��ע��ť
  afx_msg void OnAddTimes1();
  //��ע��ť
  afx_msg void OnAddTimes2();
  //��ע��ť
  afx_msg void OnAddTimes3();
  //�����ť
  afx_msg void OnShowHand();
  //��������
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //��ʱ����Ϣ
  afx_msg void OnTimer(UINT nIDEvent);
  //�����Ϣ
  BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
  //�����Ϣ
  VOID OnLButtonDown(UINT nFlags, CPoint Point);
  //
  VOID OnLButtonUp(UINT nFlags, CPoint Point);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
