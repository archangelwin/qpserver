#include "Stdafx.h"
#include "GameClient.h"
#include "CControlWnd.h"
#include "GameClientView.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//���Զ���
#define INVALID_ITEM          0xFFFF              //��Ч����

//��ť��ʶ
#define IDC_MAX_SCORE         100               //���ť
#define IDC_LESS_SCORE          101               //��С��ť
#define IDC_SEND_SCORE          102               //�ų���ť

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
  ON_WM_SIZE()
  ON_WM_PAINT()
  ON_WM_CREATE()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONUP()
  ON_WM_SETCURSOR()
  ON_BN_CLICKED(IDC_MAX_SCORE, OnBnClickedScoreMax)
  ON_BN_CLICKED(IDC_LESS_SCORE, OnBnClickedScoreLess)
  ON_BN_CLICKED(IDC_SEND_SCORE, OnBnClickedSendScore)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CControlWnd::CControlWnd()
{
  //���ñ���
  m_lScoreMax=0L;
  m_lScoreLess=0L;

  //״̬����
  m_wItemCount=0;
  m_lScoreCurrent=0L;
  m_wCurrentItem = INVALID_ITEM;

  //λ�ñ���
  m_BenchmarkPos.SetPoint(0,0);

  //������Դ
  HINSTANCE hResInstance=AfxGetInstanceHandle();
  m_ImageControlL.LoadFromResource(hResInstance,IDB_CONTROL_L);
  m_ImageControlM.LoadFromResource(hResInstance,IDB_CONTROL_M);
  m_ImageControlR.LoadFromResource(hResInstance,IDB_CONTROL_R);
  m_ImageArrow.LoadFromResource(hResInstance,IDB_CONTROL_ARROW);
  m_ImageNumber.LoadFromResource(hResInstance,IDB_CONTROL_NUMBER);



  m_SizeItem.SetSize(m_ImageControlM.GetWidth(),m_ImageControlM.GetHeight());
  m_SizeControl.SetSize(m_ImageControlL.GetWidth()+m_ImageControlR.GetWidth(),m_SizeItem.cy);

  //�����ߴ�
  m_ControlHead=m_ImageControlL.GetWidth();
  m_ControlTail=m_ImageControlR.GetWidth();

  return;
}

//��������
CControlWnd::~CControlWnd()
{
}

//����λ��
VOID CControlWnd::SetBenchmarkPos(INT nXPos, INT nYPos)
{
  //���ñ���
  m_BenchmarkPos.SetPoint(nXPos,nYPos);

  //�����ؼ�
  if(m_hWnd!=NULL)
  {
    //��ȡλ��
    CRect rcClient;
    GetClientRect(&rcClient);

    //�ƶ�����
    SetWindowPos(NULL,nXPos-rcClient.Width(),nYPos-rcClient.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
  }

  return;
}

//������Ϣ
VOID CControlWnd::SetScoreControl(LONGLONG lScoreMax, LONGLONG lScoreLess, LONGLONG lScoreCurrent)
{
  //���ñ���
  m_lScoreMax=lScoreMax;
  m_lScoreLess=lScoreLess;
  m_lScoreCurrent=lScoreCurrent;

  //״̬����
  m_wItemCount=0;
  m_lScoreCurrent=__min(m_lScoreMax,__max(m_lScoreCurrent,m_lScoreLess));
  m_wCurrentItem = INVALID_ITEM;

  //������Ŀ
  do
  {
    lScoreMax/=10L;
    m_wItemCount++;
  }
  while(lScoreMax>0L);

  //�ƶ�����
  INT nControlWidth=m_wItemCount*m_SizeItem.cx+m_SizeControl.cx;
  SetWindowPos(NULL,m_BenchmarkPos.x-nControlWidth,m_BenchmarkPos.y-m_SizeControl.cy,nControlWidth,m_SizeControl.cy,SWP_NOZORDER|SWP_SHOWWINDOW);

  //ˢ�½���
  Invalidate(FALSE);

  return;
}

//��������
VOID CControlWnd::PlayGoldSound()
{
  //��������
  CGameClientEngine * pGameClientDlg=(CGameClientEngine *)CGameFrameEngine::GetInstance();
  if(pGameClientDlg!=NULL)
  {
    pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("CHOOSE_SCORE"));
  }

  return;
}

//ת������
WORD CControlWnd::SwitchToIndex(CPoint MousePoint)
{
  //��ȡλ��
  CRect rcClient;
  GetClientRect(&rcClient);

  //λ���ж�
  if((MousePoint.x>m_ControlHead)&&(MousePoint.x<(rcClient.Width()-m_ControlTail))&&
     MousePoint.y>5&&MousePoint.y<rcClient.Height()-5)
  {
    //��������
    INT nXPos=MousePoint.x-m_ControlHead;

    //Ѱ������
    return m_wItemCount-WORD(nXPos/m_SizeItem.cx)-1;
  }

  return INVALID_ITEM;
}

//���ť
VOID CControlWnd::OnBnClickedScoreMax()
{
  //�����ж�
  if(m_lScoreCurrent!=m_lScoreMax)
  {
    //���ñ���
    m_lScoreCurrent=m_lScoreMax;

    //��������
    PlayGoldSound();

    //���½���
    Invalidate(FALSE);
  }

  return;
}

//��С��ť
VOID CControlWnd::OnBnClickedScoreLess()
{
  //�����ж�
  if(m_lScoreCurrent!=m_lScoreLess)
  {
    //���ñ���
    m_lScoreCurrent=m_lScoreLess;

    //��������
    PlayGoldSound();

    //���½���
    Invalidate(FALSE);
  }

  return;
}

//�ų���ť
VOID CControlWnd::OnBnClickedSendScore()
{
  if(pGameClientView !=NULL)
  {
    pGameClientView->PostEngineMessage(IDM_ADD_SCORE,0,(LPARAM)m_lScoreCurrent);
  }
  return;
}

//�ػ�����
VOID CControlWnd::OnPaint()
{
  CPaintDC dc(this);

  //��ȡλ��
  CRect rcClient;
  GetClientRect(&rcClient);

  //��������
  CDC BufferDC;
  CBitmap BufferImage;
  BufferDC.CreateCompatibleDC(&dc);
  BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

  //���ñ���
  BufferDC.SetBkMode(TRANSPARENT);
  BufferDC.SelectObject(&BufferImage);

  //�滭����
  m_ImageControlL.BitBlt(BufferDC,0,0);
  m_ImageControlR.BitBlt(BufferDC,rcClient.Width()-m_ImageControlR.GetWidth(),0);

  //��������
  LONGLONG lScoreCurrent=m_lScoreCurrent;
  INT nNumberHeight=m_ImageNumber.GetHeight();
  INT nNumberWidht=m_ImageNumber.GetWidth()/10;
  INT nXItemPos=rcClient.Width()-m_ImageControlR.GetWidth();

  //�滭����
  for(WORD i=0; i<m_wItemCount; i++)
  {
    //�滭����
    nXItemPos-=m_SizeItem.cx;
    m_ImageControlM.BitBlt(BufferDC,nXItemPos,0);

    //�滭����
    LONGLONG lNumber=(LONGLONG)(lScoreCurrent%10L);
    m_ImageNumber.BitBlt(BufferDC.m_hDC,nXItemPos,12,nNumberWidht,nNumberHeight,INT(lNumber)*nNumberWidht,0);

    //��ǰ����
    if(m_wCurrentItem == i)
    {
      m_ImageArrow.TransDrawImage(&BufferDC,nXItemPos+6,4,RGB(255,0,255));
    }

    //���ñ���
    lScoreCurrent/=10L;
  }

  //�滭����
  dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

  //������Դ
  BufferDC.DeleteDC();
  BufferImage.DeleteObject();

  return;
}

//λ����Ϣ
VOID CControlWnd::OnSize(UINT nType, INT cx, INT cy)
{
  __super::OnSize(nType, cx, cy);

  //�ƶ��ؼ�
  HDWP hDwp=BeginDeferWindowPos(32);
  const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

  //�����ؼ�
  DeferWindowPos(hDwp,m_btMaxScore,NULL,cx-71,4,0,0,uFlags);
  DeferWindowPos(hDwp,m_btLessScore,NULL,cx-71,26,0,0,uFlags);
  DeferWindowPos(hDwp,m_btSendScore,NULL,cx-43,10,0,0,uFlags);

  //�����ƶ�
  EndDeferWindowPos(hDwp);

  return;
}

//������Ϣ
INT CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(__super::OnCreate(lpCreateStruct)==-1)
  {
    return -1;
  }

  //������ť
  CRect rcCreate(0,0,0,0);
  m_btMaxScore.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MAX_SCORE);
  m_btLessScore.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_LESS_SCORE);
  m_btSendScore.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SEND_SCORE);

  //���ð�ť
  HINSTANCE hResInstance=AfxGetInstanceHandle();
  m_btMaxScore.SetButtonImage(IDB_BT_CONTROL_MAX,hResInstance,false,false);
  m_btLessScore.SetButtonImage(IDB_BT_CONTROL_LESS,hResInstance,false,false);
  m_btSendScore.SetButtonImage(IDB_BT_CONTROL_SEND,hResInstance,false,false);

  return 0;
}

//�����Ϣ
VOID CControlWnd::OnLButtonUp(UINT nFlags, CPoint Point)
{
  __super::OnLButtonUp(nFlags, Point);

  //��ȡ����
  WORD wItemIndex=SwitchToIndex(Point);

  //���ñ���
  if(wItemIndex!=INVALID_ITEM)
  {
    //��������
    LONGLONG lIncrease=1L;
    for(WORD i=0; i<wItemIndex; i++)
    {
      lIncrease*=10;
    }

    //��������
    LONGLONG lScoreCurrent=m_lScoreCurrent+lIncrease;
    lScoreCurrent=__min(m_lScoreMax,__max(lScoreCurrent,m_lScoreLess));

    //���ÿؼ�
    if(lScoreCurrent!=m_lScoreCurrent)
    {
      //���ñ���
      m_lScoreCurrent=lScoreCurrent;

      //��������
      PlayGoldSound();

      //���½���
      Invalidate(FALSE);
    }

    return;
  }

  return;
}

//�����Ϣ
VOID CControlWnd::OnRButtonUp(UINT nFlags, CPoint Point)
{
  __super::OnRButtonUp(nFlags, Point);

  //��ȡ����
  WORD wItemIndex=SwitchToIndex(Point);

  //���ñ���
  if(wItemIndex!=INVALID_ITEM)
  {
    //��������
    LONGLONG lDecrease=1L;
    for(WORD i=0; i<wItemIndex; i++)
    {
      lDecrease*=10;
    }

    //��������
    LONGLONG lScoreCurrent=m_lScoreCurrent-lDecrease;
    lScoreCurrent=__min(m_lScoreMax,__max(lScoreCurrent,m_lScoreLess));

    //���ÿؼ�
    if(lScoreCurrent!=m_lScoreCurrent)
    {
      //���ñ���
      m_lScoreCurrent=lScoreCurrent;

      //��������
      PlayGoldSound();

      //���½���
      Invalidate(FALSE);
    }

    return;
  }

  return;
}

//�����Ϣ
BOOL CControlWnd::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
  //��ȡ���
  CPoint MousePoint;
  GetCursorPos(&MousePoint);
  ScreenToClient(&MousePoint);

  //ת������
  m_wCurrentItem=SwitchToIndex(MousePoint);

  Invalidate(FALSE);
  //���ù��
  if(m_wCurrentItem!=INVALID_ITEM)
  {
    SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
    return TRUE;
  }

  return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//////////////////////////////////////////////////////////////////////////