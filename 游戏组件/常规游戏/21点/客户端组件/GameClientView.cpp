#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ť��ʶ
#define IDC_START           100               //��ʼ��ť
#define IDC_GET_CARD          102               //Ҫ�ư�ť
#define IDC_STOP_CARD         103               //ͣ�ư�ť
#define IDC_DOUBLE_CARD         104               //�ӱ���ť
#define IDC_SPLIT_CARD          105               //���ư�ť
#define IDC_INSURE_CARD         106               //���հ�ť
#define IDC_ADMIN           127

//��ʱ����ʶ
#define IDI_SEND_CARD         100               //���ƶ�ʱ��

//���ƶ���
#define SEND_STEP_COUNT         6               //����
#define SPEED_SEND_CARD         30                //�����ٶ�

#define NAME_WIDTH            120
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
  ON_WM_CREATE()
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
  //��������
  m_SendCardPos.SetPoint(0,0);
  m_SendCardCurPos = m_SendCardPos;
  m_nStepCount = SEND_STEP_COUNT;
  m_nXStep = 0;
  m_nYStep = 0;

  //���ݱ���
  m_lCellScore = 0L;

  ZeroMemory(m_cbUserCardType,sizeof(m_cbUserCardType));
  ZeroMemory(m_bStopCard,sizeof(m_bStopCard));
  ZeroMemory(m_bDoubleCard,sizeof(m_bDoubleCard));
  ZeroMemory(m_bInsureCard,sizeof(m_bInsureCard));

  m_cbWaitStatus = 0;

  m_wBankerUser = INVALID_CHAIR;

  m_pClientControlDlg = NULL;
  m_hInst = NULL;

  return;
}

//��������
CGameClientView::~CGameClientView()
{
  if(m_pClientControlDlg)
  {
    delete m_pClientControlDlg;
    m_pClientControlDlg = NULL;
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

  //������Դ
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_ImageCard.LoadFromResource(this,hInstance,IDB_CARD);
  m_ImageViewBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);
  m_ImageViewFill.LoadFromResource(this,hInstance,IDB_VIEW_FILL);
  m_ImageCellScore.LoadFromResource(this,hInstance,IDB_CELL_SCORE);
  m_ImageBanker.LoadFromResource(this, hInstance,IDB_BANKER_FLAG);
  m_ImageWaitBanker.LoadFromResource(this,hInstance,IDB_WAIT_BANKER);
  m_ImageWaitOthers.LoadFromResource(this,hInstance, IDB_WAIT_OTHERS);
  m_ImageWaitScore.LoadFromResource(this, hInstance,IDB_WAIT_SCORE);

  m_PngNumber.LoadImage(this,hInstance,TEXT("NUMBER"));
  m_PngCellNumber.LoadImage(this, hInstance,TEXT("CELL_NUMBER"));
  m_PngCardStatus.LoadImage(this, hInstance,TEXT("CARD_STATUS"));
  m_PngCardStatusBack.LoadImage(this, hInstance,TEXT("CARD_STATUS_BACK"));
  m_PngCardValue.LoadImage(this, hInstance,TEXT("CARD_VALUE_NUM"));
  m_PngCardValueTip.LoadImage(this, hInstance,TEXT("CARD_VALUE_TIP"));
  m_PngCardArrow.LoadImage(this, hInstance,TEXT("ARROW"));

  //�����ؼ�
  CRect rcCreate(0,0,0,0);
  m_GoldControl.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,20);
  m_GoldControl.SetGameClientHandler(this);

  //������ť
  m_btStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
  m_btGetCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_GET_CARD);
  m_btStopCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_STOP_CARD);
  m_btDoubleCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_DOUBLE_CARD);
  m_btSplitCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_SPLIT_CARD);
  m_btInsureCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_INSURE_CARD);

  //����λͼ
  m_btStart.SetButtonImage(IDB_BT_START,hInstance,false,false);
  m_btGetCard.SetButtonImage(IDB_BT_GET_CARD,hInstance,false,false);
  m_btStopCard.SetButtonImage(IDB_BT_STOP_CARD,hInstance,false,false);
  m_btSplitCard.SetButtonImage(IDB_BT_SPLIT_CARD,hInstance,false,false);
  m_btDoubleCard.SetButtonImage(IDB_BT_DOUBLE_CARD,hInstance,false,false);
  m_btInsureCard.SetButtonImage(IDB_BT_INSURE_CARD,hInstance,false,false);

  //�����ؼ�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_CardControl[i*2+1].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,30*i+1);
    m_CardControl[i*2].Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,30*i);
    m_PlayerJeton[i].LoadResource(this);
  }

  m_ScoreView.Create(NULL,NULL,WS_CHILD,rcCreate,this,10);

  //����
  m_hInst = NULL;
  m_pClientControlDlg = NULL;
  m_hInst = LoadLibrary(TEXT("BlackJackClientControl.dll"));
  if(m_hInst)
  {
    typedef void * (*CREATE)(CWnd* pParentWnd);
    CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl");
    if(ClientControl)
    {
      m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
      m_pClientControlDlg->ShowWindow(SW_HIDE);
    }
  }

  //���ư�ť
  m_btOpenAdmin.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(4, 4, 11, 11), this, IDC_ADMIN);
  m_btOpenAdmin.ShowWindow(SW_HIDE);

  return 0;
}
//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
  switch(LOWORD(wParam))
  {
    case IDC_START:       //��ʼ��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_START,0,0);

      return TRUE;
    }
    case IDC_GET_CARD:      //��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_GET_CARD,0,0);

      return TRUE;
    }
    case IDC_STOP_CARD:     //��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_STOP_CARD,0,0);

      return TRUE;
    }
    case IDC_DOUBLE_CARD:     //��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_DOUBLE_CARD,0,0);

      return TRUE;
    }
    case IDC_SPLIT_CARD:      //��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_SPLIT_CARD,0,0);

      return TRUE;
    }
    case IDC_INSURE_CARD:     //��ť
    {
      //������Ϣ
      SendEngineMessage(IDM_INSURE_CARD,0,0);

      return TRUE;
    }
  }

  return __super::OnCommand(wParam, lParam);
}
//���ý���
void CGameClientView::ResetGameView()
{
  //��������
  m_SendCardCurPos = m_SendCardPos;
  m_nStepCount = 0;
  m_nXStep = 0;
  m_nYStep = 0;
  m_SendCardItemArray.RemoveAll();

  //���ݱ���
  m_lCellScore = 0L;

  ZeroMemory(m_cbUserCardType,sizeof(m_cbUserCardType));
  ZeroMemory(m_bStopCard,sizeof(m_bStopCard));
  ZeroMemory(m_bDoubleCard,sizeof(m_bDoubleCard));
  ZeroMemory(m_bInsureCard,sizeof(m_bInsureCard));

  m_cbWaitStatus = 0;

  m_wBankerUser = INVALID_CHAIR;

  //���ó���
  for(WORD i=0; i<GAME_PLAYER+1; i++)
  {
    m_PlayerJeton[i].SetScore(0L);
  }

  //ɾ����ʱ��
  KillTimer(IDI_SEND_CARD);

  return;
}
//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
  int nFaceW = 50,nFaceH = 50;
  int nClockW = 65,nClockH = 70;
  int nWordH = 18 ;
  //��������
  m_ptAvatar[0].x=nWidth/2;
  m_ptAvatar[0].y=nHeight/2-345;
  m_ptNickName[0].x=m_ptAvatar[0].x+nFaceW;
  m_ptNickName[0].y=m_ptAvatar[0].y+5;
  m_ptClock[0].x=m_ptAvatar[0].x - nClockW/2;
  m_ptClock[0].y=m_ptAvatar[0].y+nClockH/2 ;
  m_ptReady[0].x=nWidth/2;
  m_ptReady[0].y=nHeight/2-120;
  m_ptBanker[0].SetPoint(m_ptAvatar[0].x+nFaceW,m_ptAvatar[0].y+nWordH*2);

  //��������
  m_ptAvatar[1].x=nWidth/2+310;
  m_ptAvatar[1].y=nHeight/2-120;
  m_ptNickName[1].x=m_ptAvatar[1].x+nFaceW-NAME_WIDTH;
  m_ptNickName[1].y=m_ptAvatar[1].y+nFaceH;
  m_ptClock[1].x=m_ptAvatar[1].x +nFaceW-nClockW/2;
  m_ptClock[1].y=m_ptAvatar[1].y - nClockH/2;
  m_ptReady[1].x=nWidth/2+200;
  m_ptReady[1].y=nHeight/2;
  m_ptBanker[1].SetPoint(m_ptAvatar[1].x+5,m_ptAvatar[1].y+nFaceH+nWordH*2);

  //��������
  m_ptAvatar[2].x=nWidth/2;
  m_ptAvatar[2].y=nHeight/2+290;
  m_ptNickName[2].x=m_ptAvatar[2].x-NAME_WIDTH;
  m_ptNickName[2].y=m_ptAvatar[2].y+5;
  m_ptClock[2].x=m_ptAvatar[2].x-NAME_WIDTH - nClockW/2;
  m_ptClock[2].y=m_ptAvatar[2].y+nFaceH-nClockH/2;
  m_ptReady[2].x=nWidth/2;
  m_ptReady[2].y=nHeight/2+108;
  m_ptBanker[2].SetPoint(m_ptAvatar[2].x-30,m_ptAvatar[2].y-25);

  //��������
  m_ptAvatar[3].x=nWidth/2-350;
  m_ptAvatar[3].y=nHeight/2-120;
  m_ptNickName[3].x=m_ptAvatar[3].x;
  m_ptNickName[3].y=m_ptAvatar[3].y+nFaceH;
  m_ptClock[3].x=m_ptAvatar[3].x+nClockW/2;
  m_ptClock[3].y=m_ptAvatar[3].y - nClockH/2;
  m_ptReady[3].x=nWidth/2-200;
  m_ptReady[3].y=nHeight/2;
  m_ptBanker[3].SetPoint(m_ptAvatar[3].x+5,m_ptAvatar[3].y+nFaceH+nWordH*2);

  //�û��˿�
  m_CardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-285,enXCenter,enYTop);
  m_CardControl[1].SetBenchmarkPos(nWidth/2,nHeight/2-255,enXCenter,enYTop);

  m_CardControl[2].SetBenchmarkPos(nWidth/2+252,nHeight/2-40,enXCenter,enYTop);
  m_CardControl[3].SetBenchmarkPos(nWidth/2+252,nHeight/2-10,enXCenter,enYTop);

  m_CardControl[4].SetBenchmarkPos(nWidth/2,nHeight/2+140,enXCenter,enYTop);
  m_CardControl[5].SetBenchmarkPos(nWidth/2,nHeight/2+170,enXCenter,enYTop);

  m_CardControl[6].SetBenchmarkPos(nWidth/2-242,nHeight/2-40,enXCenter,enYTop);
  m_CardControl[7].SetBenchmarkPos(nWidth/2-242,nHeight/2-10,enXCenter,enYTop);

  //��ע��λ��
  INT nXGoldControl = nWidth/2+150;
  INT nYGoldControl = nHeight/2+200;
  m_GoldControl.SetBenchmarkPos(nXGoldControl,nYGoldControl);

  //��ť�ؼ�
  CRect rcButton;
  HDWP hDwp=BeginDeferWindowPos(32);
  const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

  //��ʼ��ť
  m_btStart.GetWindowRect(&rcButton);
  DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2-rcButton.Width()/2,nHeight/2+100,0,0,uFlags);

  //���ư�ť
  m_btGetCard.GetWindowRect(&rcButton);
  DeferWindowPos(hDwp,m_btGetCard,NULL,nWidth/2+78,nHeight/2+283,0,0,uFlags);
  DeferWindowPos(hDwp,m_btStopCard,NULL,nWidth/2+147,nHeight/2+253,0,0,uFlags);
  DeferWindowPos(hDwp,m_btDoubleCard,NULL,nWidth/2+205,nHeight/2+207,0,0,uFlags);
  DeferWindowPos(hDwp,m_btInsureCard,NULL,nWidth/2+254,nHeight/2+151,0,0,uFlags);
  DeferWindowPos(hDwp,m_btSplitCard,NULL,nWidth/2+288,nHeight/2+86,0,0,uFlags);

  //�����ƶ�
  EndDeferWindowPos(hDwp);

  //��ҳ���
  m_PlayerJeton[0].SetBenchmarkPos(nWidth/2,nHeight/2-55);
  m_PlayerJeton[1].SetBenchmarkPos(nWidth/2+80,nHeight/2-5);
  m_PlayerJeton[2].SetBenchmarkPos(nWidth/2,nHeight/2+55);
  m_PlayerJeton[3].SetBenchmarkPos(nWidth/2-80,nHeight/2-5);

  //������ʼλ��
  m_SendCardPos.SetPoint(nWidth/2+150,nHeight/2-340);
  //������ͼ
  CRect rcControl;
  m_ScoreView.GetWindowRect(&rcControl);
  m_ScoreView.MoveWindow((nWidth-rcControl.Width())/2+5,nHeight/2-155,rcControl.Width(),rcControl.Height());

  return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
  //�滭����
  DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
  DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);
  //��Ԫע

  m_ImageCellScore.TransDrawImage(pDC,2,2,RGB(255,0,255));
  //��Ԫע��Ϣ
  if(m_lCellScore > 0L)
  {
    DrawNumberString(pDC,m_lCellScore,70,42,m_PngCellNumber,false);
  }

  //�滭�û�
  TCHAR szBuffer[64]=TEXT("");
  for(WORD i=0; i<GAME_PLAYER; i++)
  {

    WORD wUserTimer=GetUserClock(i);

    //ʱ����Ϣ
    if(wUserTimer!=0)
    {
      DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
    }

    IClientUserItem *pUserItem=GetClientUserItem(i);
    if(pUserItem == NULL)
    {
      continue;
    }

    //�û�����
    CRect rc;
    rc.SetRect(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+NAME_WIDTH,m_ptNickName[i].y+16);
    CDFontEx::DrawText(this, pDC, 14, 400,pUserItem->GetNickName(),rc, RGB(255,255,255),((i==3||i==2)?DT_RIGHT:DT_LEFT)|DT_END_ELLIPSIS);


    //�û����
    LONGLONG lLeaveScore=pUserItem->GetUserScore()-((pUserItem->GetUserStatus()==US_PLAYING)?m_PlayerJeton[i].GetScore():0);
    _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),lLeaveScore);
    rc.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+16,m_ptNickName[i].x+NAME_WIDTH,m_ptNickName[i].y+32);
    CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rc, RGB(255,255,255),((i==3||i==2)?DT_RIGHT:DT_LEFT)|DT_END_ELLIPSIS|DT_END_ELLIPSIS);

    if(pUserItem->GetUserStatus()==US_READY)
    {
      DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
    }
    //if ( !CD2DEngine::GetD2DEngine() )
    DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,GetClientUserItem(i));


    //��ҳ���
    m_PlayerJeton[i].DrawJettonControl(pDC);
    //��������
    LONGLONG lJetonScore = m_PlayerJeton[i].GetScore();

    if(lJetonScore > 0L)
    {
      CPoint pt = m_PlayerJeton[i].GetBenchmarkPos();
      CRect rcDraw(pt.x-50,pt.y+15,pt.x+50,pt.y+27);
      _sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),lJetonScore);
      CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rcDraw, RGB(255,255,255),DT_LEFT);
    }

  }

  //ׯ�ұ�־
  if(m_wBankerUser != INVALID_CHAIR)
  {
    m_ImageBanker.TransDrawImage(pDC,m_ptBanker[m_wBankerUser].x,m_ptBanker[m_wBankerUser].y,RGB(255,0,255));
  }

  //�ȴ���־
  if(m_cbWaitStatus > 0)
  {
    if(m_cbWaitStatus == 1)
    {
      m_ImageWaitScore.TransDrawImage(pDC,nWidth/2-m_ImageWaitScore.GetWidth()/2,
                                      nHeight/2-m_ImageWaitScore.GetHeight()/2-120,RGB(255,0,255));
    }
    else if(m_cbWaitStatus == 2)
    {
      m_ImageWaitOthers.TransDrawImage(pDC,nWidth/2-m_ImageWaitOthers.GetWidth()/2,
                                       nHeight/2-m_ImageWaitOthers.GetHeight()/2-120,RGB(255,0,255));

    }
    else
    {
      m_ImageWaitBanker.TransDrawImage(pDC,nWidth/2-m_ImageWaitBanker.GetWidth()/2,
                                       nHeight/2-m_ImageWaitBanker.GetHeight()/2-120,RGB(255,0,255));
    }
  }

  //�滭�˿�
  if(m_SendCardItemArray.GetCount()>0)
  {
    //��������
    tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

    //��ȡ��С
    int nItemWidth=m_ImageCard.GetWidth()/13;
    int nItemHeight=m_ImageCard.GetHeight()/5;

    //�滭�˿�
    m_ImageCard.TransDrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
  }

  //������ע
  LONGLONG lTableScore=0L;
  for(i = 0; i < GAME_PLAYER; i++)
  {
    lTableScore += m_PlayerJeton[i].GetScore();
  }
  //��ע��Ϣ
  if(lTableScore>0L)
  {
    DrawNumberString(pDC,lTableScore,nWidth/2,nHeight/2-12,m_PngNumber,false);
  }

  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    for(BYTE j = 0; j < 2; j++)
    {
      if(m_cbUserCardType[i*2+j] > 0 && m_CardControl[i*2+j].GetCardCount() > 1)
      {
        CRect rcCard;
        m_CardControl[i*2+j].GetControlRect(rcCard);

        //��ǰ������ʾ
        BYTE cbImageIndex = (m_bStopCard[i*2+j]||(j==1&&!m_bStopCard[i*2]))?0:2;
        if(i == 3 || i == 0)
        {
          cbImageIndex++;
        }
        INT nXPos1 = (i==1||i==2)?rcCard.left-m_PngCardArrow.GetWidth()/4-2:rcCard.right+2;
        INT nYPos = (j==0?rcCard.top:rcCard.bottom-m_PngCardValue.GetHeight());
        m_PngCardArrow.DrawImage(pDC,nXPos1,nYPos,m_PngCardArrow.GetWidth()/4,m_PngCardArrow.GetHeight(),
                                 cbImageIndex*m_PngCardArrow.GetWidth()/4,0);

        //�������
        if(m_cbUserCardType[i*2+j] == CT_BAOPAI || m_cbUserCardType[i*2+j] >= CT_BJ)
        {
          INT nImageWidth = m_PngCardValueTip.GetWidth()/3;
          INT nXPos = nXPos1+((i==0||i==3)?m_PngCardArrow.GetWidth()/4:0);
          nXPos += ((i==1||i==2)?-(nImageWidth+2):2);
          if(m_cbUserCardType[i*2+j] > CT_BJ && (i==1||i==2))
          {
            nXPos -= m_PngCardValue.GetWidth()/10*((m_cbUserCardType[i*2+j]-CT_BJ+4)>9?2:1);
          }

          //�滭����
          if(m_cbUserCardType[i*2+j] > CT_BJ)
          {
            BYTE cbNum = m_cbUserCardType[i*2+j]-CT_BJ+4;
            INT nNumWidth = m_PngCardValue.GetWidth()/10;
            DrawNumberString(pDC,cbNum,nXPos+(cbNum>9?nNumWidth:nNumWidth/2),nYPos+m_PngCardValue.GetHeight()/2,m_PngCardValue,false);
            nXPos += (cbNum>9?nNumWidth*2:nNumWidth);
          }

          BYTE cbImageIndex = 0;
          if(m_cbUserCardType[i*2+j] == CT_BJ)
          {
            cbImageIndex++;
          }
          else if(m_cbUserCardType[i*2+j] > CT_BJ)
          {
            cbImageIndex += 2;
          }
          m_PngCardValueTip.DrawImage(pDC,nXPos,nYPos,nImageWidth,m_PngCardValueTip.GetHeight(),cbImageIndex*nImageWidth,0);
        }
        else
        {
          //�滭����
          INT nImageWidth = m_PngCardValue.GetWidth()/10;
          INT nXPos = nXPos1+((i==0||i==3)?m_PngCardArrow.GetWidth()/4:0);
          nXPos += ((i==1||i==2)?-(nImageWidth+2):(nImageWidth+2));
          DrawNumberString(pDC,m_cbUserCardType[i*2+j],nXPos,nYPos+m_PngCardValue.GetHeight()/2,m_PngCardValue,false);
        }

        //�ӱ���ʾ
        if(m_bDoubleCard[i*2+j])
        {
          INT nXPos = (i==1||i==2)?rcCard.left-2-m_PngCardStatusBack.GetWidth():rcCard.right+2;
          if(j == 0 && m_CardControl[i*2+1].GetCardCount() > 1)
          {
            nYPos = rcCard.top+m_PngCardValue.GetHeight()+m_PngCardStatusBack.GetHeight()+2;
          }
          else
          {
            nYPos = rcCard.bottom-m_PngCardStatusBack.GetHeight()-(j==1?m_PngCardValue.GetHeight():0);
          }
          //�滭�ӱ���ʾ
          m_PngCardStatusBack.DrawImage(pDC,nXPos,nYPos-23);
          m_PngCardStatus.DrawImage(pDC,nXPos+2,nYPos-20,m_PngCardStatus.GetWidth()/2,m_PngCardStatus.GetHeight(),0,0);
        }
        //������ʾ
        if(m_bInsureCard[i*2+j])
        {
          INT nXPos = (i==1||i==2)?rcCard.left-2-m_PngCardStatusBack.GetWidth():rcCard.right+2;
          if(j == 0 && m_CardControl[i*2+1].GetCardCount() > 1)
          {
            nYPos = rcCard.top+m_PngCardValue.GetHeight()+2;
          }
          else
          {
            nYPos = rcCard.bottom-m_PngCardStatusBack.GetHeight()*2-(j==1?m_PngCardValue.GetHeight():0);
          }
          //�滭������ʾ
          m_PngCardStatusBack.DrawImage(pDC,nXPos,nYPos-23);
          INT nStatusWidth = m_PngCardStatus.GetWidth()/2;
          m_PngCardStatus.DrawImage(pDC,nXPos+2,nYPos-20,nStatusWidth,m_PngCardStatus.GetHeight(),nStatusWidth,0);
        }
      }
    }
  }

  return;
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

    if(i == m_SendCardItemArray.GetCount()-1)
    {
      SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);
    }
  }

  //ɾ������
  KillTimer(IDI_SEND_CARD);
  m_SendCardItemArray.RemoveAll();

  //���½���
  InvalidGameView(0,0,0,0);

  return;
}

//�����˿�
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
  //Ч�����
  ASSERT(wChairID<GAME_PLAYER*2);
  if(wChairID>=GAME_PLAYER*2)
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
    InvalidGameView(0,0,0,0);

    //��������
    CGameClientEngine * pGameClientDlg=(CGameClientEngine *)CGameFrameEngine::GetInstance();
    if(pGameClientDlg!=NULL)
    {
      pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
    }
  }

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
    InvalidGameView(0,0,0,0);

    //ֹͣ�ж�
    if(--m_nStepCount == 0)
    {
      //��ȡ�˿�
      BYTE cbCardData[MAX_COUNT] = {0};
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
        CGameClientEngine * pGameClientDlg=(CGameClientEngine *)CGameFrameEngine::GetInstance();
        if(pGameClientDlg!=NULL)
        {
          pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
        }

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

  __super::OnTimer(nIDEvent);
}

////��������
//void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
//{
//  //��������
//  int nStringLength=lstrlen(pszString);
//  int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
//  int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
//
//  //�滭�߿�
//  pDC->SetTextColor(crFrame);
//  for (int i=0;i<CountArray(nXExcursion);i++)
//  {
//    pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
//  }
//
//  //�滭����
//  pDC->SetTextColor(crText);
//  pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
//
//  return;
//}
//
////��������
//void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
//{
//  //��������
//  int nStringLength=lstrlen(pszString);
//  int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
//  int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
//
//  //�滭�߿�
//  pDC->SetTextColor(crFrame);
//  CRect rcDraw;
//  for (int i=0;i<CountArray(nXExcursion);i++)
//  {
//    rcDraw.CopyRect(lpRect);
//    rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
//    pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
//  }
//
//  //�滭����
//  rcDraw.CopyRect(lpRect);
//  pDC->SetTextColor(crText);
//  pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
//
//  return;
//}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, CPngImageEx &PngNumber, bool bHasSign)
{
  //������Դ
  INT nNumberHeight=PngNumber.GetHeight();
  INT nNumberWidth=PngNumber.GetWidth()/(10+(bHasSign?2:0));

  //������Ŀ
  INT nNumberCount=0;
  LONGLONG lNumberTemp=lNumber;
  do
  {
    nNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp!=0);

  //λ�ö���
  INT nYDrawPos=INT(nYPos-nNumberHeight/2);
  INT nXDrawPos=INT(nXPos+(nNumberCount+(bHasSign?1:0))*nNumberWidth/2-nNumberWidth);

  BYTE bySignCount = bHasSign?2:0;
  //�滭����
  if(lNumber < 0L)
  {
    lNumberTemp = -lNumber;
  }
  else
  {
    lNumberTemp = lNumber;
  }
  for(INT i=0; i<nNumberCount; i++)
  {
    //�滭����
    INT lCellNumber=INT(lNumberTemp%10);
    PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,(lCellNumber+bySignCount)*nNumberWidth,0);

    //���ñ���
    lNumberTemp/=10;
    nXDrawPos-=nNumberWidth;
  }
  //�滭������
  if(bHasSign)
  {
    PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,(lNumber>=0?0:1)*nNumberWidth,0);
  }

  return;
}

//���õ�Ԫע
void CGameClientView::SetCellScore(LONGLONG lCellScore)
{
  if(m_lCellScore == lCellScore)
  {
    return;
  }
  m_lCellScore = lCellScore;
  InvalidGameView(0,0,0,0);
}

//���ͣ��
void CGameClientView::SetUserStopCard(WORD wChairID)
{
  if(INVALID_CHAIR == wChairID)
  {
    ZeroMemory(m_bStopCard,sizeof(m_bStopCard));
    InvalidGameView(0,0,0,0);
    return;
  }
  if(!m_bStopCard[wChairID*2])
  {
    m_bStopCard[wChairID*2] = TRUE;
  }
  else
  {
    m_bStopCard[wChairID*2+1] = TRUE;
  }
  InvalidGameView(0,0,0,0);
  return;
}

//��Ҽӱ�
void CGameClientView::SetUserDoubleCard(WORD wChairID)
{
  if(INVALID_CHAIR == wChairID)
  {
    ZeroMemory(m_bDoubleCard,sizeof(m_bDoubleCard));
    InvalidGameView(0,0,0,0);;
    return;
  }
  if(!m_bStopCard[wChairID*2])
  {
    m_bDoubleCard[wChairID*2] = TRUE;
  }
  else
  {
    m_bDoubleCard[wChairID*2+1] = TRUE;
  }
  InvalidGameView(0,0,0,0);
  return;
}

//����±���
void CGameClientView::SetUserInsureCard(WORD wChairID, WORD wOperIndex)
{
  if(INVALID_CHAIR == wChairID)
  {
    ZeroMemory(m_bInsureCard,sizeof(m_bInsureCard));
    InvalidGameView(0,0,0,0);
    return;
  }
  m_bInsureCard[wChairID*2+wOperIndex] = TRUE;
  InvalidGameView(0,0,0,0);
  return;
}

//���õȴ�
void CGameClientView::SetWaitStatus(BYTE cbWaitStatus)
{
  //0:��;1:�ȴ��м���ע;2:�ȴ��мҲ�����;3:�ȴ�ׯ�Ҳ�����
  m_cbWaitStatus = cbWaitStatus;
  InvalidGameView(0,0,0,0);
}

//����ׯ��
void CGameClientView::SetBankerUser(WORD wChairId)
{
  m_wBankerUser = wChairId;
  InvalidGameView(0,0,0,0);
}

//
void CGameClientView::SetUserCardType(WORD wChairId, WORD wOperIndex, BYTE cbCardType)
{
  if(INVALID_CHAIR == wChairId)
  {
    ZeroMemory(m_cbUserCardType,sizeof(m_cbUserCardType));
    InvalidGameView(0,0,0,0);
    return;
  }
  m_cbUserCardType[wChairId*2+wOperIndex] = cbCardType;
  InvalidGameView(0,0,0,0);
  return;
}

//����Ա����
void CGameClientView::OpenAdminWnd()
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
//////////////////////////////////////////////////////////////////////////

