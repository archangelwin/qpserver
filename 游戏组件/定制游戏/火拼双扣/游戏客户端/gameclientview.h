#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "ClientControl.h"
//��������
#define ME_VIEW_CHAIR       1                 //�Լ�λ��


//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START         (WM_USER+100)           //��ʼ��Ϣ
#define IDM_OUT_CARD        (WM_USER+101)           //������Ϣ
#define IDM_PASS_CARD       (WM_USER+102)           //��������
#define IDM_REQ_HUNTER        (WM_USER+103)           //���볭��
#define IDM_OUT_PROMPT        (WM_USER+104)           //��ʾ����
#define IDM_SORT_BY_COUNT     (WM_USER+105)           //��Ŀ����
#define IDM_TRUSTEE_CONTROL     (WM_USER+106)           //�йܿ���
#define IDM_CARD_TYPE       (WM_USER+107)           //������ʾ    
#define IDM_LAST_TURN_CARD      (WM_USER+108)           //��һ�ֱ�־

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
  //λ����Ϣ
protected:
  CPoint              m_PointTrustee[GAME_PLAYER];    //�й�λ��
  CPoint              m_PointUserPass[GAME_PLAYER];   //����λ��
  CPoint              m_PointWinOrder[GAME_PLAYER];   //��Ӯ����
  CPoint                          m_PointMessageDlg[GAME_PLAYER];   //������Ϣ
  CPoint                          m_PtBombScore[GAME_PLAYER];
  INT               m_nXFace;
  INT               m_nYFace;
  INT               m_nXTimer;
  INT               m_nYTimer;
  INT               m_nXBorder;
  INT               m_nYBorder;
  CPoint              m_ptScoreView;            //����λ��
  //�ƶ�����
protected:
  bool              m_bMoveMouse;           //�ƶ���־
  bool              m_bSwitchCard;            //������־
  bool              m_bSelectCard;            //ѡ���־
  WORD              m_wHoverCardItem;         //��������
  WORD              m_wMouseDownItem;         //�������


  //���ֱ�־
protected:
  bool              m_bLastTurnCard;          //�����˿�
  //״̬����
public:
  bool              m_bShowScore;           //����״̬
  bool              m_bPass[GAME_PLAYER];       //��������
  bool              m_bTrustee[GAME_PLAYER];      //�Ƿ��й�
  WORD              m_wWinOrder[GAME_PLAYER];     //ʤ���б�
  BYTE              m_bCardCount[GAME_PLAYER];      //�˿���Ŀ
  bool              m_bShowScoreView;         //��ʾ����
  LONGLONG            m_lBombScore[GAME_PLAYER];      //���׷���
  bool              m_bUserCheat[GAME_PLAYER];      //���ױ�־

  //��ʷ����
public:
  LONGLONG                        m_lAllTurnScore[GAME_PLAYER];   //�ֵܾ÷�
  LONGLONG                        m_lLastTurnScore[GAME_PLAYER];    //�Ͼֵ÷�

  //��ը����
protected:
  bool              m_bBombEffect;            //��ըЧ��
  BYTE              m_cbBombFrameIndex;         //֡������

  //λͼ����
protected:
  CPngImage           m_ImageTrustee;           //�йܱ�־
  CBitImage           m_ImageViewFill;          //����λͼ
  CBitImage           m_ImageViewBack;          //����λͼ
  CPngImage           m_ImageUserPass;          //������־
  CBitImage           m_ImageBombEffect;          //��ըλͼ
  //CBitImage           m_ImageHistoryScore;        //����λͼ

  CPngImage           m_PngNumber;            //����ͼƬ
  CPngImage           m_PngHistoryScore;          //��ʷ����
  CPngImage           m_PngAvtarBack;           //ͷ�񱳾�
  CPngImage           m_PngLastTurn;            //���ֱ�־
  CPngImage           m_TextureClock;           //ʱ����Դ
  CPngImage           m_TextureClockNumber;       //ʱ������
  CPngImage           m_PngUserFrame;             //�����Դ


  //ͼƬ��С
private:
  CSize             m_sizeHistory;            //���ִ�С

  //��ť�ؼ�
public:
  CSkinButton           m_btScore;              //���ְ�ť
  CSkinButton           m_btStart;              //��ʼ��ť
  CSkinButton           m_btOutCard;            //���ư�ť
  CSkinButton           m_btHunter;             //���װ�ť
  CSkinButton           m_btPassCard;           //������ť
  CSkinButton           m_btLastTurnCard;         //�����˿�
  CSkinButton           m_btStusteeControl;         //�Ϲܿ���

  CSkinButton           m_btOne;              //1�Ű�ť
  CSkinButton           m_btTwo;              //2�Ű�ť
  CSkinButton           m_btThree;              //3�Ű�ť
  CSkinButton           m_btFour;             //ը����ť
  CSkinButton           m_btSortByCount;          //��Ŀ����

  CButton             m_btOpenAdmin;            //ϵͳ����

  //��Ϸ�ؼ�
public:
  CCardControl          m_HandCardControl[GAME_PLAYER];   //�����˿�
  CCardControl          m_UserCardControl[GAME_PLAYER];   //�˿���ͼ
  CScoreView            m_ScoreView;            //������ͼ
  CCardControl          m_RandCardControl;
  HINSTANCE           m_hInst;
  IClientControlDlg         *m_pAdminControl;         //���ƿؼ�

#ifdef VIDEO_GAME
  //��Ƶ���
private:
  CVideoServiceControl      m_DlgVedioService[GAME_PLAYER];   //��Ƶ����
#endif

  //��������
protected:
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame,CRect*rect);
  //��������
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
  //�滭�ַ�
  VOID DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, INT nDrawFormat);
  //�滭�ַ�
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,int nWidth, int nWeight,UINT nFormat);
  //�滭����
  void DrawSymbolNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
  //ʱ������
  void DrawNumber_Clock(CDC *pDC,int x,int y,int iNum);
  //�滭ʱ��
  void DrawUserTimer(CDC *pDC, INT nXPos, INT nYPos, WORD wTime);

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
  //�Ƿ��������
  virtual bool RealizeWIN7()
  {
    return true;
  }

  //���ܺ���
public:
  //���÷���
  void SetPassFlag(WORD wChairID, bool bPass);
  //�˿���Ŀ
  void SetCardCount(WORD wChairID, BYTE bCardCount);
  //��������
  bool SetUserWinOrder(WORD wChairID, WORD wWinOrder);
  //�����й�
  void SetTrustee(WORD wTrusteeUser,bool bTrustee);
  //���ñ�ը
  bool SetBombEffect(bool bBombEffect);
  //��������
  void SetLastTurnCard(bool bLastTurnCard);
  //������ͼ
  void RefreshGameView();
  //������ʾ
  void SetShowScoreView(bool bShow);
  //���׷���
  void SetBombScore(WORD wChairID,LONGLONG lBombScore);

  //��Ϣӳ��
protected:
  //��ʼ��ť
  afx_msg void OnStart();
  //���ư�ť
  afx_msg void OnOutCard();
  //���볭��
  afx_msg void OnReqHunter();
  //������ť
  afx_msg void OnPassCard();
  //�����˿�
  afx_msg void OnLastTurnCard();
  //�Ϲܿ���
  afx_msg void OnStusteeControl();
  //���ְ�ť
  afx_msg void OnBnClickedScore();
  //�رհ�ť
  afx_msg void OnBnClickedScoreClose();
  //1�Ű�ť
  afx_msg void OnBnClickedOne();
  //2�Ű�ť
  afx_msg void OnBnClickedTwo();
  //3�Ű�ť
  afx_msg void OnBnClickedThree();
  //4�Ű�ť
  afx_msg void OnBnClickedFour();
  //��Ŀ����
  void OnSortByCount();


  //��������
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //�����Ϣ
  afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
  //���˫��
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  //����Ա����
  afx_msg void OpenAdminWnd();

  //�����Ϣ
  VOID OnMouseMove(UINT nFlags, CPoint Point);
  //�����Ϣ
  VOID OnLButtonUp(UINT nFlags, CPoint Point);
  //�����Ϣ
  VOID OnLButtonDown(UINT nFlags, CPoint Point);
  //�����Ϣ
  BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnTimer(UINT nIDEvent);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//////////////////////////////////////////////////////////////////////////
