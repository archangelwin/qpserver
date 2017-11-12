#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_START           100               //��ʼ��ť
#define IDC_GIVE_UP           102               //������ť
#define IDC_FOLLOW            103               //��ע��ť
#define IDC_SHOW_HAND         104               //�����ť
#define IDC_ADD_TIMES1          105               //��ְ�ť
#define IDC_ADD_TIMES2          106               //��ְ�ť
#define IDC_ADD_TIMES3          107               //��ְ�ť
#define IDC_ADMIN           108               //��ť��ʶ
#define IDC_ADMIN_EX          109               //��ť��ʶ

//��ʱ����ʶ
#define IDI_SEND_CARD         107               //���ƶ�ʱ��
#define IDI_SHOW_CARD         108               //
#define IDI_USER_SHOW_HAND        109               //

#define SPEED_MOVE_CARD         30                //

#define TIME_USER_SHOW_HAND       3000              //

//���ƶ���
#define SEND_STEP_COUNT         6               //����
#define SPEED_SEND_CARD         30                //�����ٶ�

#define MAX_COUNT           5

#define SHOW_CARD_ANIMATE                       //���ÿ��ƶ���,���Ҫȡ������,��ע�͵�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
  ON_WM_CREATE()
  ON_BN_CLICKED(IDC_START, OnStart)
  ON_BN_CLICKED(IDC_FOLLOW, OnFollow)
  ON_BN_CLICKED(IDC_GIVE_UP, OnGiveUp)
  ON_BN_CLICKED(IDC_ADD_TIMES1, OnAddTimes1)
  ON_BN_CLICKED(IDC_ADD_TIMES2, OnAddTimes2)
  ON_BN_CLICKED(IDC_ADD_TIMES3, OnAddTimes3)
  ON_BN_CLICKED(IDC_SHOW_HAND, OnShowHand)
  ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
  ON_BN_CLICKED(IDC_ADMIN_EX,OpenAdminWndEx)
  ON_WM_TIMER()
  ON_WM_SETCURSOR()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void)
{
  //���ñ���
  m_lMaxGold=0L;
  m_lBasicGold=0L;
  m_bShowScoreControl = false;
  m_bUserShowHand = false;
  ZeroMemory(m_lUserGold,sizeof(m_lUserGold));

  //��������
  m_SendCardPos.SetPoint(0,0);
  m_SendCardCurPos = m_SendCardPos;
  m_nStepCount = SEND_STEP_COUNT;
  m_nXStep = 0;
  m_nYStep = 0;

  //����λͼ
  HINSTANCE hInstance=AfxGetInstanceHandle();
  //m_ImageCardBack.LoadImage(hInstance,TEXT("HAND_CARD"));
  m_ImageCardBack.LoadFromResource(hInstance,IDB_CARD);
  m_ImageTitle.LoadFromResource(hInstance,IDB_TITLE);
  m_ImageViewFill.LoadFromResource(hInstance,IDB_VIEW_FILL);
  m_ImageViewCenter.LoadFromResource(hInstance,IDB_BACK_GROUND);
  m_ImageScoreControlBack.LoadImage(hInstance, _T("SCORE_CONTROL_BACK"));
  m_ImageScoreTable.LoadFromResource(hInstance,IDB_SCORE_TABLE);
  m_PngShowHand.LoadImage(hInstance,TEXT("SHOW_HAND"));
  m_ImageTimeNumber.LoadImage(hInstance,TEXT("TIME_NUMBER"));
  m_ImageTimeBack.LoadImage(hInstance,TEXT("TIME_BACK"));
  m_ImageNickBack.LoadImage(hInstance,TEXT("NICNAME_BACK"));

  m_nXFace=48;
  m_nYFace=48;
  m_nXTimer=65;
  m_nYTimer=69;
  m_nXBorder=0;
  m_nYBorder=0;
  m_pClientControlDlg = NULL;
  m_pClientControlDlgEx = NULL;
  m_hInst = NULL;

  return;
}

//��������
CGameClientView::~CGameClientView(void)
{
  if(m_pClientControlDlg)
  {
    delete m_pClientControlDlg;
    m_pClientControlDlg = NULL;
  }

  if(m_pClientControlDlgEx)
  {
    delete m_pClientControlDlgEx;
    m_pClientControlDlgEx = NULL;
  }

  if(m_hInst)
  {
    FreeLibrary(m_hInst);
    m_hInst = NULL;
  }
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(__super::OnCreate(lpCreateStruct)==-1)
  {
    return -1;
  }

  //�����ؼ�
  CRect CreateRect(0,0,0,0);
  m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,9);

  //������ť
  m_btStart.Create(NULL,WS_CHILD,CreateRect,this,IDC_START);
  m_btFollow.Create(NULL,WS_CHILD,CreateRect,this,IDC_FOLLOW);
  m_btGiveUp.Create(NULL,WS_CHILD,CreateRect,this,IDC_GIVE_UP);
  m_btShowHand.Create(NULL,WS_CHILD,CreateRect,this,IDC_SHOW_HAND);
  m_btAddTimes1.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_ADD_TIMES1);
  m_btAddTimes2.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_ADD_TIMES2);
  m_btAddTimes3.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CreateRect,this,IDC_ADD_TIMES3);

  //����λͼ
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_btStart.SetButtonImage(IDB_BT_START,hInstance,false,false);
  m_btFollow.SetButtonImage(IDB_BT_FOLLOW,hInstance,false,false);
  m_btGiveUp.SetButtonImage(IDB_BT_GIVE_UP,hInstance,false,false);
  m_btShowHand.SetButtonImage(IDB_BT_SHOW_HAND,hInstance,false,false);
  m_btAddTimes1.SetButtonImage(IDB_BT_ADD_GOLD,hInstance,false,false);
  m_btAddTimes2.SetButtonImage(IDB_BT_ADD_GOLD,hInstance,false,false);
  m_btAddTimes3.SetButtonImage(IDB_BT_ADD_GOLD,hInstance,false,false);

  m_DFontEx.CreateFont(this, TEXT("����"), 12, 600);

  m_TotalGoldView.OnCreate(this);
  for(int i=0; i<CountArray(m_GoldView); i++)
  {
    m_GoldView[i].OnCreate(this);
  }

  //����
  m_hInst = NULL;
  m_hInst = LoadLibrary(TEXT("ShowHandANExClientControl.dll"));

  m_pClientControlDlg = NULL;
  m_pClientControlDlgEx = NULL;

  if(m_hInst)
  {
    typedef void * (*CREATE)(CWnd* pParentWnd);
    CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl");
    if(ClientControl)
    {
      m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
      m_pClientControlDlg->ShowWindow(SW_HIDE);
    }

    typedef void * (*CREATEEX)(CWnd* pParentWnd);
    CREATE ClientControlEx = (CREATEEX)GetProcAddress(m_hInst,"CreateClientControlEx");
    if(ClientControlEx)
    {
      m_pClientControlDlgEx = static_cast<IClientControlDlg*>(ClientControlEx(this));
      m_pClientControlDlgEx->ShowWindow(SW_HIDE);
    }
  }
  //���ư�ť
  m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_ADMIN);
  m_btOpenAdmin.ShowWindow(SW_HIDE);

  m_btOpenAdminEx.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_ADMIN_EX);
  m_btOpenAdminEx.ShowWindow(SW_HIDE);

  return 0;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
  SendEngineMessage(IDM_START,0,0);
  return;
}

//��ע��ť
void CGameClientView::OnFollow()
{
  SendEngineMessage(IDM_FOLLOW,0,0);
  return;
}

//������ť
void CGameClientView::OnGiveUp()
{
  SendEngineMessage(IDM_GIVE_UP,0,0);
  return;
}

//��ע��ť
void CGameClientView::OnAddTimes1()
{
  SendEngineMessage(IDM_ADD_GOLD,1,0);
  return;
}

//��ע��ť
void CGameClientView::OnAddTimes2()
{
  SendEngineMessage(IDM_ADD_GOLD,2,0);
  return;
}

//��ע��ť
void CGameClientView::OnAddTimes3()
{
  SendEngineMessage(IDM_ADD_GOLD,3,0);
  return;
}

//�����ť
void CGameClientView::OnShowHand()
{
  SendEngineMessage(IDM_SHOWHAND,0,0);
  return;
}

//���ý���
VOID CGameClientView::ResetGameView()
{
  //���ñ���
  m_lMaxGold=0L;
  m_lBasicGold=0L;
  m_bShowScoreControl = false;
  m_bUserShowHand = false;
  ZeroMemory(m_lUserGold,sizeof(m_lUserGold));

  //��������
  m_SendCardCurPos = m_SendCardPos;
  m_nStepCount = 0;
  m_nXStep = 0;
  m_nYStep = 0;
  m_SendCardItemArray.RemoveAll();

  //���ÿؼ�
  m_btStart.ShowWindow(SW_HIDE);
  m_btFollow.ShowWindow(SW_HIDE);
  m_btGiveUp.ShowWindow(SW_HIDE);
  m_btShowHand.ShowWindow(SW_HIDE);
  m_ScoreView.ShowWindow(SW_HIDE);

  //���ó���
  for(int i=0; i<CountArray(m_GoldView); i++)
  {
    m_GoldView[i].SetGold(0L);
  }

  //�����˿�
  for(int i=0; i<CountArray(m_CardControl); i++)
  {
    m_CardControl[i].SetCardData(NULL,0);
    m_CardControl[i].ShowFirstCard(false);
    m_CardControl[i].AllowPositively(false);
  }

  return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
  //��������
  m_ptAvatar[0].x=nWidth/2-90;
  m_ptAvatar[0].y=4+m_nYBorder;
  m_ptNickName[0].x=nWidth/2+m_nXFace+5-90;
  m_ptNickName[0].y=5+m_nYBorder;
  m_ptClock[0].x=nWidth/2+230;
  m_ptClock[0].y=nHeight/2-200;
  m_ptReady[0].x=nWidth/2 + 10;
  m_ptReady[0].y=160;
  //SetFlowerControlInfo( 0,nWidth/2-BIG_FACE_WIDTH/2,m_nYBorder+5+m_nYFace );

  m_ptAvatar[1].x=nWidth/2-90;
  m_ptAvatar[1].y=nHeight-m_nYBorder-m_nYFace-5-8;
  m_ptNickName[1].x=nWidth/2+m_nYFace+5-90;
  m_ptNickName[1].y=nHeight-m_nYBorder-30;
  m_ptClock[1].x=nWidth/2+230;
  m_ptClock[1].y=nHeight/2+145;
  m_ptReady[1].x=nWidth/2 + 10;
  m_ptReady[1].y=nHeight-180-m_nYBorder;
  //SetFlowerControlInfo( 1,nWidth/2-BIG_FACE_WIDTH/2,nHeight-m_nYBorder-m_nYFace-5-BIG_FACE_HEIGHT );

  //����ؼ�
  int nXCenterPos=nWidth/2;
  int nYCenterPos=nHeight/2;
  m_GoldStation[0].SetPoint(nXCenterPos,nYCenterPos-90);
  m_GoldStation[1].SetPoint(nXCenterPos+135,nYCenterPos-90);
  m_GoldStation[2].SetPoint(nXCenterPos,nYCenterPos+90);
  m_GoldStation[3].SetPoint(nXCenterPos-135,nYCenterPos+90);

  //��ע��λ��
  m_ptScoreControl.SetPoint(nWidth-220,nHeight-180);
  INT nXControl = m_ptScoreControl.x+9;
  INT nYControl = m_ptScoreControl.y+27;

  //��ť�ؼ�
  CRect rcButton;
  HDWP hDwp=BeginDeferWindowPos(32);
  const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

  //��ʼ��ť
  CRect rcStart;
  m_btStart.GetWindowRect(&rcStart);
  DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcStart.Width())/2+10,nHeight-225,0,0,uFlags);

  //���ư�ť
  m_btFollow.GetWindowRect(&rcButton);
  DeferWindowPos(hDwp,m_btAddTimes1,NULL,nXControl,nYControl,0,0,uFlags);
  DeferWindowPos(hDwp,m_btAddTimes2,NULL,nXControl,nYControl+rcButton.Height()+5,0,0,uFlags);
  DeferWindowPos(hDwp,m_btAddTimes3,NULL,nXControl,nYControl+rcButton.Height()*2+10,0,0,uFlags);
  DeferWindowPos(hDwp,m_btFollow,NULL,nXControl+rcButton.Width()+2,nYControl,0,0,uFlags);
  DeferWindowPos(hDwp,m_btShowHand,NULL,nXControl+rcButton.Width()+2,nYControl+rcButton.Height()+5,0,0,uFlags);
  DeferWindowPos(hDwp,m_btGiveUp,NULL,nXControl+rcButton.Width()+2,nYControl+rcButton.Height()*2+10,0,0,uFlags);
  //m_btFollow.GetWindowRect(&rcButton);
  //DeferWindowPos(hDwp,m_btFollow,NULL,nXControl,nYControl,0,0,uFlags);
  //DeferWindowPos(hDwp,m_btShowHand,NULL,nXControl+rcButton.Width()+1,nYControl,0,0,uFlags);
  //DeferWindowPos(hDwp,m_btAddTimes3,NULL,nXControl,nYControl+rcButton.Height()+1,0,0,uFlags);
  //DeferWindowPos(hDwp,m_btAddTimes2,NULL,nXControl+rcButton.Width()+1,nYControl+rcButton.Height()+1,0,0,uFlags);
  //DeferWindowPos(hDwp,m_btAddTimes1,NULL,nXControl,nYControl+rcButton.Height()*2+2,0,0,uFlags);
  //DeferWindowPos(hDwp,m_btGiveUp,NULL,nXControl+rcButton.Width()+1,nYControl+rcButton.Height()*2+2,0,0,uFlags);

  //�����ƶ�
  EndDeferWindowPos(hDwp);

  //������ư�ť
  m_btOpenAdmin.MoveWindow(4, 4, 7, 7);
  m_btOpenAdminEx.MoveWindow(nWidth-11, 4, 7, 7);

  //�ɼ�����
  if(NULL!=m_ScoreView.GetSafeHwnd())
  {
    CRect rcScoreView;
    m_ScoreView.GetWindowRect(&rcScoreView);
    m_ScoreView.MoveWindow((nWidth-rcScoreView.Width())/2,(nHeight-rcScoreView.Height())/2-10,rcScoreView.Width(),rcScoreView.Height());
  }

  //�˿˿ؼ�
  m_CardControl[0].SetBasicStation(CPoint(nWidth/2,52+m_nYBorder + 15),enXCenter,enYTop);
  m_CardControl[1].SetBasicStation(CPoint(nWidth/2,nHeight-55-m_nYBorder - 15),enXCenter,enYBottom);

  //������ʼλ��
  m_SendCardPos.SetPoint(nWidth/2-40,nHeight/2-52);

  return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
  DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
  //DrawViewImage(pDC,m_ImageViewCenter,DRAW_MODE_CENTENT);
  //m_ImageTitle.BitBlt(pDC->m_hDC,m_nXBorder,m_nYBorder);

  //
  m_ImageScoreTable.TransDrawImage(pDC,nWidth/2-m_ImageScoreTable.GetWidth()/2,nHeight/2-m_ImageScoreTable.GetHeight()/2,
                                   RGB(255,0,255));

  //�滭�û�
  WORD wUserTimer=0;
  TCHAR szBuffer[64];
  pDC->SetTextColor(RGB(255,255,0));
  for(WORD i=0; i<4; i++)
  {
    //��������
    IClientUserItem * pClientUserItem=GetClientUserItem(i);

    ////////////////////////////////////////////////////////
    //IClientUserItem *pClientUserItem = new IClientUserItem;
    //strcpy( pClientUserItem->szName, "dkfjdkfj" );
    //pClientUserItem->cbUserStatus = US_READY ;
    //wUserTimer = 50;
    //////////////////////////////////////////////////////////


    if(pClientUserItem!=NULL)
    {
      m_ImageNickBack.DrawImage(pDC,m_ptNickName[i].x,m_ptNickName[i].y-3);

      //�û�����
      pDC->SetTextAlign((TA_LEFT|TA_TOP));
      //pDC->TextOut(m_ptNickName[i].x,m_ptNickName[i].y,pClientUserItem->GetNickName(),lstrlen(pClientUserItem->GetNickName()));
      m_DFontEx.DrawText(pDC, pClientUserItem->GetNickName(), CRect(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x + 90,m_ptNickName[i].y + 15), RGB(255,255,255), (DT_CENTER|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS));


      ////�û����
      //LONGLONG lUserScore = (pClientUserItem->GetUserStatus()==US_PLAYING)?m_lUserGold[i]-m_GoldView[i*2].GetGold()-m_GoldView[i*2+1].GetGold():pClientUserItem->GetUserScore();
      //_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d"),lUserScore);
      ////pDC->TextOut(m_ptNickName[i].x,m_ptNickName[i].y+16,szBuffer,lstrlen(szBuffer));
      //m_DFontEx.DrawText( pDC, szBuffer, CRect(m_ptNickName[i].x,m_ptNickName[i].y + 16,m_ptNickName[i].x + 100,m_ptNickName[i].y + 16 + 15), RGB(255,255,0), (DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS) );


      //������Ϣ
      wUserTimer=GetUserClock(i);
      if(wUserTimer!=0)
      {
        //DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
        DrawTime(pDC, wUserTimer, m_ptClock[i].x,m_ptClock[i].y);
      }

      if(pClientUserItem->GetUserStatus()==US_READY)
      {
        DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
      }
      DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pClientUserItem);
    }
  }

  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    m_CardControl[i].DrawCardControl(pDC);
  }

  //�滭����
  LONGLONG lTotalGold=0L;
  for(DWORD i=0; i<CountArray(m_GoldView); i++)
  {
    lTotalGold+=m_GoldView[i].GetGold();
    m_GoldView[i].DrawGoldView(pDC,m_GoldStation[i].x,m_GoldStation[i].y,RGB(63,66,10));
  }

  //��Ϸ��Ϣ
  pDC->SetTextAlign(TA_LEFT|TA_TOP);
  pDC->SetTextColor(RGB(255,255,255));
  const UINT uDrawFormat=DT_END_ELLIPSIS|DT_CENTER|DT_SINGLELINE|DT_VCENTER;

  //��Ϸ��ע
  if(lTotalGold!=0L)
  {
    _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d"),lTotalGold);
    //pDC->DrawText(szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80,m_nYBorder+48,m_nXBorder+140,m_nYBorder+60),uDrawFormat);
    //m_DFontEx.DrawText(pDC,szBuffer,CRect(m_nXBorder+80,m_nYBorder+48,m_nXBorder+140,m_nYBorder+60),RGB(250,250,250),uDrawFormat);
    m_TotalGoldView.SetGold(lTotalGold);
    m_TotalGoldView.DrawGoldView(pDC,nWidth/2-1,nHeight/2+2,RGB(63,66,10));
  }

  ////�����ע
  //if (m_lMaxGold!=0)
  //{
  //  _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d"),m_lMaxGold);
  //  //pDC->DrawText(szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80,m_nYBorder+10,m_nXBorder+140,m_nYBorder+22),uDrawFormat);
  //  m_DFontEx.DrawText(pDC,szBuffer,CRect(m_nXBorder+80,m_nYBorder+10,m_nXBorder+140,m_nYBorder+22),RGB(250,250,250),uDrawFormat);
  //}

  ////������ע
  //if (m_lBasicGold!=0)
  //{
  //  _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d"),m_lBasicGold);
  //  //pDC->DrawText(szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80,m_nYBorder+29,m_nXBorder+140,m_nYBorder+41),uDrawFormat);
  //  m_DFontEx.DrawText(pDC,szBuffer,CRect(m_nXBorder+80,m_nYBorder+29,m_nXBorder+140,m_nYBorder+41),RGB(250,250,250),uDrawFormat);
  //}

  ////��Ϸ��ע
  //if (lTotalGold!=0L)
  //{
  //  _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d"),lTotalGold);
  //  //pDC->DrawText(szBuffer,lstrlen(szBuffer),CRect(m_nXBorder+80,m_nYBorder+48,m_nXBorder+140,m_nYBorder+60),uDrawFormat);
  //  m_DFontEx.DrawText(pDC,szBuffer,CRect(m_nXBorder+80,m_nYBorder+48,m_nXBorder+140,m_nYBorder+60),RGB(250,250,250),uDrawFormat);
  //}

  //��ʾ��ע���Ʊ���
  if(m_bShowScoreControl)
  {
    m_ImageScoreControlBack.DrawImage(pDC,m_ptScoreControl.x,m_ptScoreControl.y);
  }

  //�滭�˿�
  if(m_SendCardItemArray.GetCount()>0)
  {
    //��������
    tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

    //�滭�˿�
    INT nCardWidth = m_ImageCardBack.GetWidth()/13;
    INT nCardHeight = m_ImageCardBack.GetHeight()/5;
    //m_ImageCardBack.DrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nCardWidth,nCardHeight,nCardWidth*2,nCardHeight*4);
    m_ImageCardBack.TransDrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nCardWidth,nCardHeight,nCardWidth*2,nCardHeight*4,RGB(255,0,255));
  }

  //�û����
  if(m_bUserShowHand)
  {
    m_PngShowHand.DrawImage(pDC,(nWidth-m_PngShowHand.GetWidth())/2,(nHeight-m_PngShowHand.GetHeight())/2);
  }

  return;
}

//��ұ���
void CGameClientView::SetGoldTitleInfo(LONGLONG lMaxGold, LONGLONG lBasicGold)
{
  //���ñ���
  m_lMaxGold=lMaxGold;
  m_lBasicGold=lBasicGold;

  //CFont font;
  //font.CreateFont(8,10,0,0,200,0,0,0,0,0,0,0,0,_T("����"));

  //���°�ť����
  TCHAR szNum[16];
  _sntprintf(szNum,CountArray(szNum),TEXT("%I64d"),m_lBasicGold);
  m_btAddTimes1.SetWindowText(szNum);
  //m_btAddTimes1.SetButtonColor(RGB(0,0,0));
  //m_btAddTimes1.SetFont(&font);
  _sntprintf(szNum,CountArray(szNum),TEXT("%I64d"),m_lBasicGold*2);
  m_btAddTimes2.SetWindowText(szNum);
  _sntprintf(szNum,CountArray(szNum),TEXT("%I64d"),m_lBasicGold*3);
  m_btAddTimes3.SetWindowText(szNum);

  //������ͼ
  RefreshGameView();

  return;
}

//�û�����
void CGameClientView::SetUserGoldInfo(WORD wViewChairID, bool bTableGold, LONGLONG dwGold)
{
  //�����ж�
  WORD wGoldPos=wViewChairID*2+((bTableGold==true)?1:0);
  if(wGoldPos>=CountArray(m_GoldStation))
  {
    return;
  }

  //���ñ���
  LONGLONG m_dwAllGold=0L;
  m_GoldView[wGoldPos].SetGold(dwGold);
  for(int i=0; i<CountArray(m_GoldStation); i++)
  {
    m_dwAllGold+=m_GoldView[i].GetGold();
  }

  //������ͼ
  RefreshGameView();

  return;
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
  //���ʹ���
  if(nIDEvent==IDI_SEND_CARD)
  {
    //����λ��
    m_SendCardCurPos.x += m_nXStep;
    m_SendCardCurPos.y += m_nYStep;

    //���½���
    RefreshGameView();

    //ֹͣ�ж�
    if(--m_nStepCount == 0)
    {
      //��ȡ�˿�
      BYTE cbCardData[MAX_COUNT];
      WORD wChairID=m_SendCardItemArray[0].wChairID;
      BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

      //�����˿�
      if(cbCardCount<MAX_COUNT)
      {
        cbCardData[cbCardCount++]=m_SendCardItemArray[0].cbCardData;
        m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
      }

      //ɾ������
      m_SendCardItemArray.RemoveAt(0);

      //��������
      if(m_SendCardItemArray.GetCount()>0)
      {
        //
        wChairID = m_SendCardItemArray[0].wChairID;
        m_nStepCount = SEND_STEP_COUNT;
        m_SendCardCurPos = m_SendCardPos;
        m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
        m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

        //��������
        CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
        pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

        return;
      }
      else
      {
        //��ɴ���
        KillTimer(IDI_SEND_CARD);
        SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

        return;
      }
    }

    return;
  }
#ifdef SHOW_CARD_ANIMATE
  else if(nIDEvent == IDI_SHOW_CARD)
  {
    if(!m_CardControl[MYSELF_VIEW_ID].PlayMoveCard())
    {
      KillTimer(IDI_SHOW_CARD);
    }
    RefreshGameView();
    return;
  }
#endif
  else if(nIDEvent == IDI_USER_SHOW_HAND)
  {
    KillTimer(IDI_USER_SHOW_HAND);
    m_bUserShowHand = false;
    RefreshGameView();
    return;
  }
  __super::OnTimer(nIDEvent);
}

//�����ж�
bool CGameClientView::IsDispatchCard()
{
  return m_SendCardItemArray.GetCount()>0;
}

//��ɷ���
void CGameClientView::FinishDispatchCard()
{
  //����ж�
  if(m_SendCardItemArray.GetCount()==0)
  {
    return;
  }

  //��ɶ���
  for(INT_PTR i=0; i<m_SendCardItemArray.GetCount(); i++)
  {
    //��ȡ����
    tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

    //��ȡ�˿�
    BYTE cbCardData[MAX_COUNT];
    WORD wChairID=pSendCardItem->wChairID;
    BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

    //�����˿�
    if(cbCardCount<MAX_COUNT)
    {
      cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
      m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
    }
  }

  //ɾ������
  KillTimer(IDI_SEND_CARD);
  m_SendCardItemArray.RemoveAll();

  SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

  //���½���
  RefreshGameView();

  return;
}

//�����˿�
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
  //Ч�����
  ASSERT(wChairID<GAME_PLAYER);
  if(wChairID>=GAME_PLAYER)
  {
    return;
  }

  //���ñ���
  tagSendCardItem SendCardItem;
  SendCardItem.wChairID=wChairID;
  SendCardItem.cbCardData=cbCardData;
  m_SendCardItemArray.Add(SendCardItem);

  //��������
  if(m_SendCardItemArray.GetCount()==1)
  {
    //���ö�ʱ��
    SetTimer(IDI_SEND_CARD,SPEED_SEND_CARD,NULL);

    //
    m_nStepCount = SEND_STEP_COUNT;
    m_SendCardCurPos = m_SendCardPos;
    m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
    m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

    //
    RefreshGameView();

    //��������
    CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
    pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
  }

  return;
}

//
VOID CGameClientView::ShowScoreControl(bool bShow)
{
  int nCmdShow = bShow?SW_SHOW:SW_HIDE;
  //���ý���
  m_btFollow.ShowWindow(nCmdShow);
  m_btGiveUp.ShowWindow(nCmdShow);
  m_btShowHand.ShowWindow(nCmdShow);
  m_btAddTimes1.ShowWindow(nCmdShow);
  m_btAddTimes2.ShowWindow(nCmdShow);
  m_btAddTimes3.ShowWindow(nCmdShow);

  m_bShowScoreControl = bShow;
  RefreshGameView();
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
  //��ȡ���
  CPoint MousePoint;
  GetCursorPos(&MousePoint);
  ScreenToClient(&MousePoint);

  //�˿˲���
  if(m_CardControl[MYSELF_VIEW_ID].OnEventSetCursor(MousePoint)==true)
  {
    return TRUE;
  }

  return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
  __super::OnLButtonDown(nFlags, Point);

  //��ȡ���
  CPoint MousePoint;
  GetCursorPos(&MousePoint);
  ScreenToClient(&MousePoint);

  //�˿˵��
  if(m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseDown(MousePoint)==true)
  {
#ifdef  SHOW_CARD_ANIMATE
    if(m_CardControl[MYSELF_VIEW_ID].BeginMoveCard())
    {
      SetTimer(IDI_SHOW_CARD,SPEED_MOVE_CARD,NULL);
      //���½���
      RefreshGameView();
    }
#else
    m_CardControl[MYSELF_VIEW_ID].ShowFirstCard(true);
    RefreshGameView();
#endif
  }

  return;
}

//
VOID CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
  __super::OnLButtonUp(nFlags, Point);

  //��ȡ���
  CPoint MousePoint;
  GetCursorPos(&MousePoint);
  ScreenToClient(&MousePoint);

  //�˿˵��
  if(m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseUp(MousePoint)==true)
  {
#ifndef SHOW_CARD_ANIMATE
    m_CardControl[MYSELF_VIEW_ID].ShowFirstCard(false);
    RefreshGameView();
    PostMessage(WM_SETCURSOR);
#endif
  }
}

//
void CGameClientView::SetUserShowHand(bool bShowHand)
{
  if(m_bUserShowHand != bShowHand)
  {
    m_bUserShowHand = bShowHand;

    //
    if(m_bUserShowHand)
    {
      SetTimer(IDI_USER_SHOW_HAND,TIME_USER_SHOW_HAND,NULL);
    }

    RefreshGameView();
  }
}

//
void CGameClientView::SetUserGold(WORD wChairId, LONGLONG lGold)
{
  if(wChairId == INVALID_CHAIR)
  {
    ZeroMemory(m_lUserGold,sizeof(m_lUserGold));
  }
  else
  {
    m_lUserGold[wChairId] = lGold;
  }
  RefreshGameView();
}

//������ͼ
void CGameClientView::RefreshGameView()
{
  CRect rect;
  GetClientRect(&rect);
  InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

  return;
}

//����Ա����
void CGameClientView::OpenAdminWnd()
{
  //��Ȩ��
  {
    if(!m_pClientControlDlg->IsWindowVisible())
    {
      m_pClientControlDlg->ShowWindow(SW_SHOW);
    }
    else
    {
      m_pClientControlDlg->ShowWindow(SW_HIDE);
    }
  }
}

//����Ա����
void CGameClientView::OpenAdminWndEx()
{
  //��Ȩ��
  {
    if(!m_pClientControlDlgEx->IsWindowVisible())
    {
      m_pClientControlDlgEx->ShowWindow(SW_SHOW);
    }
    else
    {
      m_pClientControlDlgEx->ShowWindow(SW_HIDE);
    }
  }
}


// �滭ʱ��
void CGameClientView::DrawTime(CDC * pDC, WORD wUserTime,INT nXPos, INT nYPos)
{
  //��������
  m_ImageTimeBack.DrawImage(pDC, nXPos, nYPos - m_ImageTimeBack.GetHeight()/2);

  TCHAR szOutNum[128] = {0};
  if(wUserTime > 99)
  {
    _sntprintf(szOutNum,CountArray(szOutNum),TEXT("99"));
  }
  else if(wUserTime < 10)
  {
    _sntprintf(szOutNum,CountArray(szOutNum),TEXT("0%d"), wUserTime);
  }
  else
  {
    _sntprintf(szOutNum,CountArray(szOutNum),TEXT("%d"), wUserTime);
  }

  //��������
  DrawNumber(pDC, &m_ImageTimeNumber, TEXT("0123456789"), szOutNum, nXPos + m_ImageTimeBack.GetWidth()/2, nYPos - 14, DT_CENTER);
}

// �滭����
void CGameClientView::DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/)
{
  TCHAR szOutNum[128] = {0};
  _sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);
  DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CGameClientView::DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/)
{
  TCHAR szOutNumT[128] = {0};
  _sntprintf(szOutNumT,CountArray(szOutNumT),TEXT("%s"),szOutNum);
  DrawNumber(pDC, ImageNumber, szImageNum, szOutNumT, nXPos, nYPos, uFormat);
}


// �滭����
void CGameClientView::DrawNumber(CDC * pDC , CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_LEFT*/)
{
  // ������Դ
  INT nNumberHeight=ImageNumber->GetHeight();
  INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

  if(uFormat == DT_CENTER)
  {
    nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
  }
  else if(uFormat == DT_RIGHT)
  {
    nXPos -= lstrlen(szOutNum) * nNumberWidth;
  }

  for(INT i = 0; i < lstrlen(szOutNum); ++i)
  {
    for(INT j = 0; j < lstrlen(szImageNum); ++j)
    {
      if(szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0')
      {
        ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
        nXPos += nNumberWidth;
        break;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
