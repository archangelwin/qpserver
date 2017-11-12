#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientEngine.h"


//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER      100                 //������ʶ
#define IDI_SHOW_CHANGE_BANKER    101                 //�ֻ�ׯ��
#define IDI_DISPATCH_CARD     102                 //���Ʊ�ʶ
#define IDI_SHOWDISPATCH_CARD_TIP 103                 //������ʾ
#define IDI_OPENCARD        104                 //������ʾ
#define IDI_MOVECARD_END      105                 //�Ƅ��ƽY��
#define IDI_POSTCARD        106                 //������ʾ
#define IDI_OUT_CARD_TIME     107
#define IDI_JETTON_ANIMATION    108
#define IDI_HANDLELEAVE_ANIMATION 109
#define IDI_OPENBOX_ANIMATION   110
#define IDI_LEAVHANDLESOUND     111
#define IDI_PLEASEJETTONSOUND   112
#define IDI_SPEEKSOUND        113
#define IDI_FLASH_CARD        115                 //������ʶ
#define IDI_FLASH_RAND_SIDE     116                 //������ʶ
#define IDI_SHOW_CARD_RESULT    118

//��������ע,�������ж�ʱ����������(1000�Ժ�),ר�������ṩ
#define IDI_ANDROID_BET       1000

//��ť��ʶ
#define IDC_JETTON_BUTTON_100   200                 //��ť��ʶ
#define IDC_JETTON_BUTTON_1000    201                 //��ť��ʶ
#define IDC_JETTON_BUTTON_10000   202                 //��ť��ʶ
#define IDC_JETTON_BUTTON_100000  203                 //��ť��ʶ
#define IDC_JETTON_BUTTON_1000000 204                 //��ť��ʶ
#define IDC_JETTON_BUTTON_5000000 205                 //��ť��ʶ
#define IDC_APPY_BANKER       206                 //��ť��ʶ
#define IDC_CANCEL_BANKER     207                 //��ť��ʶ
#define IDC_SCORE_MOVE_L      209                 //��ť��ʶ
#define IDC_SCORE_MOVE_R      210                 //��ť��ʶ
#define IDC_VIEW_CHART        211                 //��ť��ʶ
#define IDC_JETTON_BUTTON_50000     219                 //��ť��ʶ
#define IDC_JETTON_BUTTON_500000  212                 //��ť��ʶ
#define IDC_AUTO_OPEN_CARD      213                 //��ť��ʶ
#define IDC_OPEN_CARD       214                 //��ť��ʶ
#define IDC_BANK          215                 //��ť��ʶ
#define IDC_CONTINUE_CARD     216                 //��ť��ʶ
#define IDC_BANK_STORAGE      217                 //��ť��ʶ
#define IDC_BANK_DRAW       218                 //��ť��ʶ
#define IDC_UP            223                 //��ť��ʶ
#define IDC_DOWN          224                 //��ť��ʶ
#define IDC_RADIO         225                 //��ť��ʶ
#define IDC_ADMIN         226                 //��ť��ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
  ON_WM_TIMER()
  ON_WM_CREATE()
  ON_WM_SETCURSOR()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONDOWN()
  ON_WM_MOUSEMOVE()
  ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
  ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
  ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
  ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
  ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
  ON_BN_CLICKED(IDC_OPEN_CARD, OnOpenCard)
  ON_BN_CLICKED(IDC_AUTO_OPEN_CARD,OnAutoOpenCard)
  ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
  ON_BN_CLICKED(IDC_UP, OnUp)
  ON_BN_CLICKED(IDC_DOWN, OnDown)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
  //��ע��Ϣ
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //ȫ����ע
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
  //���������ע
  ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));

  //ׯ����Ϣ
  m_wBankerUser=INVALID_CHAIR;
  m_wBankerTime=0;
  m_lBankerScore=0L;
  m_lBankerWinScore=0L;
  m_lTmpBankerWinScore=0;
  m_Out_Bao_y = 0;
  m_CarIndex = 0;
  m_lRobotMaxJetton = 5000000l;
  m_nShowValleyIndex = 0;

  //���ֳɼ�
  m_lMeCurGameScore=0L;
  m_lMeCurGameReturnScore=0L;
  m_lBankerCurGameScore=0L;
  m_lGameRevenue=0L;

  m_JettonQueIndex = 0;

  m_lLastJetton = 0;

  //״̬��Ϣ
  m_lCurrentJetton=0L;
  m_cbAreaFlash=0xFF;
  m_wMeChairID=INVALID_CHAIR;
  m_bShowChangeBanker=FALSE;
  m_bNeedSetGameRecord=FALSE;
  m_bWinTianMen=FALSE;
  m_bWinHuangMen=FALSE;
  m_bWinXuanMen=FALSE;
  m_bFlashResult=FALSE;
  m_blMoveFinish = FALSE;
  m_blAutoOpenCard = TRUE;
  m_bShowBao = FALSE;
  m_enDispatchCardTip=enDispatchCardTip_NULL;

  m_lMeCurGameScore=0L;
  m_lMeCurGameReturnScore=0L;
  m_lBankerCurGameScore=0L;
  m_blCanStore=false;

  m_lAreaLimitScore=0L;

  //λ����Ϣ
  m_nScoreHead = 0;
  m_nRecordFirst= 0;
  m_nRecordLast= 0;
  m_CheckImagIndex = 0;
  m_ShowImageIndex  = 0;

  //��ʷ�ɼ�
  m_lMeStatisticScore=0;

  //�ؼ�����
  m_pGameClientDlg=NULL;

  m_bEnablePlaceJetton = false;

  m_CurSelectColor =RGB(255,255,255);
  return;
}

//��������
CGameClientView::~CGameClientView(void)
{

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

  AfxSetResourceHandle(AfxGetInstanceHandle());


  //����λͼ
  HINSTANCE hInstance=AfxGetInstanceHandle();
  m_ImageViewFill.LoadFromResource(this,hInstance,IDB_VIEW_FILL);
  //m_ImageViewcentre.LoadFromResource(this,hInstance,IDB_CENTREBACK);
  //m_ImageBao.LoadFromResource(this,hInstance,IDB_CENTREBACK);


  m_ImageWinFlags.LoadImage(this,hInstance,TEXT("IDB_IDB_FLAG_PNG"));

  m_ImageJettonView.LoadImage(this,hInstance,TEXT("JETTOM_VIEW"));
  m_ImageScoreNumber.LoadImage(this,hInstance,TEXT("SCORE_NUMBER"));
  m_ImageMeScoreNumber.LoadImage(this,hInstance,TEXT("ME_SCORE_NUMBER"));
  m_ViewBackPng.LoadImage(this,hInstance,TEXT("IDB_VIEW_BACK_PNG"));
  m_idb_selPng.LoadImage(this,hInstance,TEXT("IDB_IDB_SEL_PNG"));

  for(int i = 0; i<5; i++)
  {
    m_ImageCardType[i].LoadFromResource(this,hInstance,IDB_CARDTYPE);
  }

  m_ImageMeBanker.LoadFromResource(this,hInstance, IDB_ME_BANKER);
  m_ImageChangeBanker.LoadFromResource(this,hInstance, IDB_CHANGE_BANKER);
  m_ImageNoBanker.LoadFromResource(this,hInstance, IDB_NO_BANKER);
  m_ImageFrameTianMen.LoadImage(this,hInstance,TEXT("TIAN_MEM"));
  m_ImageTimeFlagPng.LoadImage(this,hInstance,TEXT("IDB_TIME_PNG"));
  m_TimerCount_png.LoadImage(this,hInstance,TEXT("IDB_TIMERCOUNT_PNG"));
  m_ImageMeScoreNumberPng.LoadImage(this,hInstance,TEXT("IDB_MENUM_PNG"));
  m_ImageScoreNumberPng.LoadImage(this,hInstance,TEXT("IDB_SCORE_PNG"));
  m_pngGameEnd.LoadImage(this,hInstance,TEXT("IDB_GAME_END_PNG"));

  //for (int i = 0;i<AREA_COUNT;i++) m_sT_ShowInfo.blShow[i]=false;


  TCHAR szBuffer[128]=TEXT("");
  //�����Ʊ���Դ
  for(int i = 0; i<7; i++)
  {
    myprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_PUSHBOX%d"),i+1);
    m_PngPushBox[i].LoadImage(this,hInstance,szBuffer);
  }

  for(int i = 0; i<4; i++)
  {
    myprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_JETTON%d"),i+1);
    m_PngShowJetton[i].LoadImage(this,hInstance,szBuffer);
  }

  for(int i = 0; i<4; i++)
  {
    myprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_HANDLELEAVE%d"),i+1);
    m_PngShowLeaveHandle[i].LoadImage(this,hInstance,szBuffer);
  }


  for(int i = 0; i<4; i++)
  {
    myprintf(szBuffer,CountArray(szBuffer),TEXT("IDB_RESULT%d"),i+1);
    m_PngResult[i].LoadImage(this,hInstance,szBuffer);
  }

  //�����ؼ�
  CRect rcCreate(0,0,0,0);
  m_GameRecord.Create(IDD_DLG_GAME_RECORD,this);
  //m_ApplyUser.Create( IDD_DLG_GAME_RECORD , this );
  m_GameRecord.ShowWindow(SW_HIDE);

  //��ע��ť
  m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
  m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
  m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
  m_btJetton50000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_50000);
  m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);

  m_btJetton500000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_500000);
  m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
  m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);

  //���밴ť
  m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
  m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

  m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
  m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

  m_btAutoOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_AUTO_OPEN_CARD);
  m_btOpenCard.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_OPEN_CARD);


  m_btBankStorage.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANK_STORAGE);
  m_btBankDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);

  m_btUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_UP);
  m_btDown.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_DOWN);



  //���ð�ť
  HINSTANCE hResInstance=AfxGetInstanceHandle();
  /*m_btJetton100.LoadRgnImage(hResInstance,IDB_BT_JETTON_100,RGB(255,0,255));
  m_btJetton1000.LoadRgnImage(hResInstance,IDB_BT_JETTON_1000,RGB(255,0,255));
  m_btJetton10000.LoadRgnImage(hResInstance,IDB_BT_JETTON_10000,RGB(255,0,255));
  m_btJetton100000.LoadRgnImage(hResInstance,IDB_BT_JETTON_100000,RGB(255,0,255));
  m_btJetton1000000.LoadRgnImage(hResInstance,IDB_BT_JETTON_1000000,RGB(255,0,255));
  m_btJetton5000000.LoadRgnImage(hResInstance,IDB_BT_JETTON_5000000,RGB(255,0,255));*/


  m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false,false);
  m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false,false);
  m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false,false);
  m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false,false);
  m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,hResInstance,false,false);
  m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000,hResInstance,false,false);


  m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false,false);
  m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false,false);


  m_btUp.SetButtonImage(IDB_BT_BT_S,hResInstance,false,false);
  m_btDown.SetButtonImage(IDB_BT_BT_X,hResInstance,false,false);

  m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
  m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);

  m_btAutoOpenCard.SetButtonImage(IDB_BT_AUTO_OPEN_CARD,hResInstance,false,false);
  m_btOpenCard.SetButtonImage(IDB_BT_OPEN_CARD,hResInstance,false,false);

  m_btBankStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false,false);
  m_btBankDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false,false);

  m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
  m_btOpenAdmin.ShowWindow(SW_HIDE);

  SwitchToCheck();
  SetJettonHide(0);

  //����
  m_hInst = NULL;
  m_pAdminControl = NULL;
  m_hInst = LoadLibrary(TEXT("ZodiacBattleClientControl.dll"));
  if(m_hInst)
  {
    typedef void * (*CREATE)(CWnd* pParentWnd);
    CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl");
    if(ClientControl)
    {
      m_pAdminControl = static_cast<IClientControlDlg*>(ClientControl(this));
      m_pAdminControl->ShowWindow(SW_HIDE);
    }
  }

  m_btBankStorage.ShowWindow(SW_SHOW);
  m_btBankDraw.ShowWindow(SW_SHOW);

//  if ( m_pAdminControl )
//  {
//    for ( int i = 0; i < CountArray(m_SkinListCtrl); ++i )
//    {
//      m_SkinListCtrl[i].Create(WS_CHILD, rcCreate, this, 2023 + i);
//      m_SkinListCtrl[i].ModifyStyle(0,LVS_REPORT);
//
//      m_SkinListCtrl[i].InsertColumn(0,TEXT("���"));
//      m_SkinListCtrl[i].InsertColumn(1,TEXT("���"));
//
//      m_SkinListCtrl[i].ShowWindow(SW_HIDE);
//    }
//  }

  return 0;
}

//���ý���
VOID CGameClientView::ResetGameView()
{
  //��ע��Ϣ
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //ȫ����ע
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
  //���������ע
  ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));

  //ׯ����Ϣ
  m_wBankerUser=INVALID_CHAIR;
  m_wBankerTime=0;
  m_lBankerScore=0L;
  m_lBankerWinScore=0L;
  m_lTmpBankerWinScore=0;
  m_nShowValleyIndex = 0;

  //���ֳɼ�
  m_lMeCurGameScore=0L;
  m_lMeCurGameReturnScore=0L;
  m_lBankerCurGameScore=0L;
  m_lGameRevenue=0L;

  //״̬��Ϣ
  m_lCurrentJetton=0L;
  m_cbAreaFlash=0xFF;
  m_wMeChairID=INVALID_CHAIR;
  m_bShowChangeBanker=false;
  m_bNeedSetGameRecord=false;
  m_bWinTianMen=false;
  m_bWinHuangMen=false;
  m_bWinXuanMen=false;
  m_bFlashResult=false;
  m_bShowGameResult=false;
  m_enDispatchCardTip=enDispatchCardTip_NULL;

  m_lMeCurGameScore=0L;
  m_lMeCurGameReturnScore=0L;
  m_lBankerCurGameScore=0L;

  m_lAreaLimitScore=0L;

  //λ����Ϣ
  m_nScoreHead = 0;
  m_nRecordFirst= 0;
  m_nRecordLast= 0;

  //��ʷ�ɼ�
  m_lMeStatisticScore=0;

  //����б�
  m_ValleysList.RemoveAll();

  //�������
  CleanUserJetton();

  //���ð�ť
  m_btApplyBanker.ShowWindow(SW_SHOW);
  m_btApplyBanker.EnableWindow(FALSE);
  m_btCancelBanker.ShowWindow(SW_HIDE);
  m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

  return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(int nWidth, int nHeight)
{
  //λ����Ϣ
  CSize Size;
  Size.cy = Size.cy/2;
  int iWidth =756;
  int iHeight =705;

  int LifeWidth = (nWidth-iWidth)/2;
  int TopHeight = (nHeight-iHeight)/2;
  int ix = nWidth/2+20;
  int iy = nHeight/2+6;
  int iAready = 52;
  int iAreadx =77;
  for(int i = 0,  j = 0; i<32; i++)
  {
    if(j ==0)
    {
      m_CarRect[i].top =iy-276-8;
      m_CarRect[i].left =ix-362-17+i*iAreadx-5;
    }
    else
    {
      if(j==1)
      {
        m_CarRect[i].top =iy-276-8+(i-8)*iAready;
        m_CarRect[i].left =ix-362-17+8*iAreadx-8+4;
      }
      else
      {
        if(j==2)
        {
          m_CarRect[i].top =iy-276-8+8*iAready;
          m_CarRect[i].left =ix-362-17+8*iAreadx-5-(i-(2*8))*iAreadx;
        }
        else
        {
          if(j==3)
          {
            m_CarRect[i].top =iy-284+8*iAready-(i-24)*iAready;
            m_CarRect[i].left =ix-381;
          }
        }
      }
    }
    j = i/8;
  }

  m_RectArea[0].SetRect(ix-279,iy-138,ix-151,iy);
  m_RectArea[1].SetRect(ix-151,iy-138,ix-21 ,iy);
  m_RectArea[2].SetRect(ix-21 ,iy-138,ix+109,iy);
  m_RectArea[3].SetRect(ix+109,iy-138,ix+239,iy);

  m_RectArea[4].SetRect(ix-279,iy,ix-151,iy+138);
  m_RectArea[5].SetRect(ix-151,iy,ix-21 ,iy+138);
  m_RectArea[6].SetRect(ix-21 ,iy,ix+109,iy+138);
  m_RectArea[7].SetRect(ix+109,iy,ix+239,iy+138);


  int ExcursionY=10;
  for(int i = 0; i<AREA_COUNT; i++)
  {
    m_PointJettonNumber[i].SetPoint((m_RectArea[i].right+m_RectArea[i].left)/2, (m_RectArea[i].bottom+m_RectArea[i].top)/2-ExcursionY);
    m_PointJetton[i].SetPoint(m_RectArea[i].left, m_RectArea[i].top);
  }


  m_CardTypePoint[0] =CPoint(nWidth/2+125,nHeight/2-355+135+62);

  //�ƶ��ؼ�
  HDWP hDwp=BeginDeferWindowPos(33);
  const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

  //m_ApplyUser.m_viewHandle = m_hWnd;

  //�б�ؼ�
  //DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2+194,547+nHeight/2-335,145,63,uFlags);
  DeferWindowPos(hDwp,m_btUp,NULL,LifeWidth+673,632+TopHeight,18,16,uFlags);
  DeferWindowPos(hDwp,m_btDown,NULL,LifeWidth+693,632+TopHeight,18,16,uFlags);

  m_MeInfoRect.top = TopHeight+22;
  m_MeInfoRect.left = LifeWidth+265+389;

  m_btBankStorage.EnableWindow(TRUE);

  m_BaoPosion.top = TopHeight;
  m_BaoPosion.left = nWidth/2;


  //���밴ť
  CRect rcJetton;
  m_btJetton100.GetWindowRect(&rcJetton);
  int nYPos =TopHeight+613;
  int nXPos =LifeWidth+197;
  int nSpace=15;

  m_TopHeight = TopHeight;
  m_LifeWidth = LifeWidth;

  m_nWinFlagsExcursionX = LifeWidth+220;
  m_nWinFlagsExcursionY = TopHeight+593;

  nSpace = 0;
  nYPos -=46;

  DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);


  DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace * 5 + rcJetton.Width() *5,nYPos,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btApplyBanker,NULL,m_LifeWidth+609-3-41-7,547+m_TopHeight+35-3+22+30,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btCancelBanker,NULL,m_LifeWidth+609-3-41-7,547+m_TopHeight+35-3+22+30,0,0,uFlags|SWP_NOSIZE);


  DeferWindowPos(hDwp,m_btScoreMoveL,NULL,LifeWidth+123+70,TopHeight+562+21+86-42,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btScoreMoveR,NULL,LifeWidth+606-63-44,TopHeight+562+21+86-42,0,0,uFlags|SWP_NOSIZE);

  //���ư�ť
  DeferWindowPos(hDwp,m_btAutoOpenCard,NULL,LifeWidth+644,TopHeight+198,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btOpenCard,NULL,LifeWidth+644,TopHeight+228,0,0,uFlags|SWP_NOSIZE);
  //������ť
  DeferWindowPos(hDwp,m_btBankStorage,NULL,nWidth/2+180,nHeight/2-300,0,0,uFlags|SWP_NOSIZE);
  DeferWindowPos(hDwp,m_btBankDraw,NULL,nWidth/2+240,nHeight/2-300,0,0,uFlags|SWP_NOSIZE);
  //�����ƶ�
  EndDeferWindowPos(hDwp);

//  if ( m_pAdminControl )
//  {
//
//    for ( int i = 0; i < CountArray(m_SkinListCtrl); ++i )
//    {
//      CRect m_WinRect(m_RectArea[i]);
//      m_WinRect.top += m_WinRect.Height() / 2;
//
//      m_WinRect.right-=2;
//      m_WinRect.OffsetRect(0, -77);
//
//      m_SkinListCtrl[i].MoveWindow(m_WinRect);
//      m_SkinListCtrl[i].SetColumnWidth( 0, m_WinRect.Width()/2 );
//      m_SkinListCtrl[i].SetColumnWidth( 1, m_WinRect.Width()/2 );
//    }
//
//  }

  return;
}
void CGameClientView::SwitchToCheck()
{
  SwithToNormalView();
  return;


  m_btAutoOpenCard.ShowWindow(SW_HIDE);
  m_btOpenCard.ShowWindow(SW_HIDE);
  //m_ApplyUser.ShowWindow(SW_HIDE);
  m_btApplyBanker.ShowWindow(SW_HIDE);
  m_btCancelBanker.ShowWindow(SW_HIDE);
  m_btScoreMoveL.ShowWindow(SW_HIDE);
  m_btScoreMoveR.ShowWindow(SW_HIDE);
  m_btJetton100.ShowWindow(SW_HIDE);
  m_btJetton1000.ShowWindow(SW_HIDE);
  m_btJetton10000.ShowWindow(SW_HIDE);
  m_btJetton50000.ShowWindow(SW_HIDE);
  m_btJetton100000.ShowWindow(SW_HIDE);
  m_btJetton500000.ShowWindow(SW_HIDE);
  m_btJetton1000000.ShowWindow(SW_HIDE);
  m_btJetton5000000.ShowWindow(SW_HIDE);
  m_DrawBack = false;
  SwithToNormalView();


}
void CGameClientView::SwithToNormalView()
{

  m_btAutoOpenCard.ShowWindow(SW_HIDE);
  m_btOpenCard.ShowWindow(SW_HIDE);
  //m_ApplyUser.ShowWindow(SW_SHOW);
  m_btApplyBanker.ShowWindow(SW_SHOW);
  m_btCancelBanker.ShowWindow(SW_SHOW);
  m_btScoreMoveL.ShowWindow(SW_SHOW);
  m_btScoreMoveR.ShowWindow(SW_SHOW);

  m_btJetton100.ShowWindow(SW_SHOW);
  m_btJetton1000.ShowWindow(SW_SHOW);
  m_btJetton10000.ShowWindow(SW_SHOW);
  m_btJetton50000.ShowWindow(SW_HIDE);
  m_btJetton100000.ShowWindow(SW_SHOW);
  m_btJetton500000.ShowWindow(SW_HIDE);
  m_btJetton1000000.ShowWindow(SW_SHOW);
  m_btJetton1000000.ShowWindow(SW_SHOW);
  m_btJetton5000000.ShowWindow(SW_SHOW);
  m_DrawBack = true;
}
//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{

  CDC* pDCBuff = pDC;

  //�滭����
  //DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);

  for(int iW = 0 ; iW < nWidth; iW += m_ImageViewFill.GetWidth())
  {
    for(int iH = 0;  iH < nHeight; iH += m_ImageViewFill.GetHeight())
    {
      m_ImageViewFill.BitBlt(pDC->GetSafeHdc(), iW, iH);
    }
  }



  IClientUserItem * pClientUserItem=m_wBankerUser==INVALID_CHAIR ? NULL :GetClientUserItem(m_wBankerUser);


  m_ViewBackPng.DrawImage(pDCBuff,(nWidth-m_ViewBackPng.GetWidth())/2,nHeight/2-m_ViewBackPng.GetHeight()/2,\
                          m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight(),0,0,m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight());

  if(m_blRungingCar)
  {
    m_idb_selPng.DrawImage(pDCBuff ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
                           m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());
  }
  if(m_bFlashrandShow)
  {
    m_idb_selPng.DrawImage(pDCBuff ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
                           m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());
  }

  //��ȡ״̬
  BYTE cbGameStatus=0;
  if(m_pGameClientDlg != NULL)
  {
    cbGameStatus=m_pGameClientDlg->GetGameStatus();
  }

  //״̬��ʾ
  CFont static InfoFont;
  InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
  CFont * pOldFont=pDC->SelectObject(&InfoFont);
  pDC->SetTextColor(RGB(255,255,0));
  m_CurSelectColor = RGB(255,255,0);
  pDC->SelectObject(pOldFont);
  InfoFont.DeleteObject();

  //ʱ����ʾ
  if(m_DrawBack)
  {
    int nTimeFlagWidth = m_ImageTimeFlagPng.GetWidth()/3;
    int nFlagIndex=0;
    if(cbGameStatus==GAME_STATUS_FREE)
    {
      nFlagIndex=0;
    }
    else if(cbGameStatus==GS_PLACE_JETTON)
    {
      nFlagIndex=1;
    }
    else if(cbGameStatus==GS_GAME_END)
    {
      nFlagIndex=2;
    }

    m_ImageTimeFlagPng.DrawImage(pDCBuff,nWidth/2-70, m_TopHeight+163, nTimeFlagWidth, m_ImageTimeFlagPng.GetHeight(),
                                 nFlagIndex*nTimeFlagWidth,0);

    WORD wUserTimer = ((m_wMeChairID!=INVALID_CHAIR)?GetUserClock(m_wMeChairID):0);

    if(wUserTimer!=0)
    {
      DrawNumberString(pDCBuff,wUserTimer,m_LifeWidth+410,m_TopHeight+172,true);
    }
  }
  //CRect rcDispatchCardTips(m_LifeWidth+612, m_TopHeight+297,m_LifeWidth+812, m_TopHeight+412);

  //ʤ���߿�
  FlashJettonAreaFrame(nWidth,nHeight,pDCBuff);

  //������Դ
  ////CImageHandle HandleJettonView(&m_ImageJettonView);
  CSize static SizeJettonItem(m_ImageJettonView.GetWidth()/6,m_ImageJettonView.GetHeight());

  //�滭����
  //IsJettonDraw(pDC);

  //�滭����
  for(INT i=0; i<AREA_COUNT; i++)
  {
    for(INT_PTR j=0; j<m_JettonInfoArray[i].GetCount(); j++)
    {
      //��ȡ��Ϣ
      tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];
      /*if(!pJettonInfo->bShow)
        continue;*/

      //�滭����
      m_ImageJettonView.DrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
                                  pJettonInfo->nYPos+m_PointJetton[i].y+16,SizeJettonItem.cx,SizeJettonItem.cy,
                                  pJettonInfo->cbJettonIndex*SizeJettonItem.cx,0);

    }
  }

  for(INT i=0; i<AREA_COUNT&&m_DrawBack; i++)
  {
    //��������
    LONGLONG lScoreCount=0L;
    LONGLONG static lScoreJetton[JETTON_COUNT]= {100L,1000L,10000L,100000L,1000000L,5000000L};

    //�滭����
    for(INT_PTR j=0; j<m_JettonInfoArray[i].GetCount(); j++)
    {
      //��ȡ��Ϣ
      tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

      //�ۼ�����
      ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
      lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];
    }
    //�滭����
    if(lScoreCount>0L)
    {
      DrawNumberString(pDCBuff,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
    }

  }

  for(int i=0; i<AREA_COUNT; i++)
  {
    switch(cbGameStatus)
    {
      case GS_PLACE_JETTON:
      {
        TCHAR Text[256];
        LONGLONG lCanJetton = GetUserMaxJetton(i+1);
        myprintf(Text,256,_TEXT("%I64d"),lCanJetton);
        DrawNumberString(pDCBuff,lCanJetton,m_RectArea[i].left+65,m_RectArea[i].top+123,true);
        break;
      }
      case GS_GAME_END:
      {
        CString szPrint=_T("����ʱ��");
        pDC->SetTextColor(RGB(255,0,0));
        m_CurSelectColor = RGB(255,0,0);
        //pDC->TextOut(m_RectArea[i].left+40,m_RectArea[i].top+118,szPrint);
        CDFontEx::DrawText(this,pDC,  12, 400, szPrint, m_RectArea[i].left+90,m_RectArea[i].top+118, m_CurSelectColor, TA_CENTER|TA_TOP);
        break;
      }
      default:
      {
        CString szPrint=_T("���·�");
        pDC->SetTextColor(RGB(255,0,0));
        m_CurSelectColor = RGB(255,0,0);
        //pDC->TextOut(m_RectArea[i].left+40,m_RectArea[i].top+118,szPrint);
        CDFontEx::DrawText(this,pDC,  12, 400, szPrint, m_RectArea[i].left+90,m_RectArea[i].top+118, m_CurSelectColor, TA_CENTER|TA_TOP);
        break;
      }
    }
  }

  //�滭ׯ��
  if(m_DrawBack)
  {
    DrawBankerInfo(pDCBuff,nWidth,nHeight);
    //�滭�û�
    DrawMeInfo(pDCBuff,nWidth,nHeight);
  }

  //�л�ׯ��
  if(m_bShowChangeBanker)
  {
    int nXPos = nWidth / 2 - 130;
    int nYPos = nHeight / 2 - 160+120;

    //������ׯ
    if(m_wMeChairID == m_wBankerUser)
    {
      //CImageHandle ImageHandleBanker(&m_ImageMeBanker);
      m_ImageMeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
    }
    else if(m_wBankerUser != INVALID_CHAIR)
    {
      //CImageHandle ImageHandleBanker(&m_ImageChangeBanker);
      m_ImageChangeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
    }
    else
    {
      //CImageHandle ImageHandleBanker(&m_ImageNoBanker);
      m_ImageNoBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
    }
  }

  //������ʾ
  m_enDispatchCardTip = enDispatchCardTip_NULL;//��ʱ����
  if(enDispatchCardTip_NULL!=m_enDispatchCardTip)
  {
    if(m_ImageDispatchCardTip.IsNull()==false)
    {
      m_ImageDispatchCardTip.Destroy();
    }
    if(enDispatchCardTip_Dispatch==m_enDispatchCardTip)
    {
      m_ImageDispatchCardTip.LoadFromResource(this,AfxGetInstanceHandle(),IDB_DISPATCH_CARD);
    }
    else
    {
      m_ImageDispatchCardTip.LoadFromResource(this,AfxGetInstanceHandle(),IDB_CONTINUE_CARD);
    }
    //CImageHandle ImageHandle(&m_ImageDispatchCardTip);
    m_ImageDispatchCardTip.BitBlt(pDC->GetSafeHdc(), (nWidth-m_ImageDispatchCardTip.GetWidth())/2, nHeight/2);
  }

  //�ҵ���ע
  DrawMeJettonNumber(pDCBuff);
  //ʤ����־
  DrawWinFlags(pDCBuff);

  DrawBankerList(pDCBuff,nWidth,nHeight);

  if(cbGameStatus!=GAME_STATUS_FREE)
  {
    m_Out_Bao_y = 6;
  }

  if(m_blMoveFinish&&cbGameStatus==GS_GAME_END)
  {
    ShowGameResult(pDCBuff, nWidth, nHeight);
  }

  if(m_DrawBack)
  {
    DrawMoveInfo(pDCBuff,m_rcTianMen);
  }

  //�滭����
  if(m_bShowBao&&0)
  {
    if(m_bEnableSysBanker==0&&pClientUserItem==NULL)
      ;
    else
    {
      if(0<=m_Out_Bao_y&&m_Out_Bao_y<7)
      {
        if(m_Out_Bao_y>=6)
        {
          m_PngPushBox[m_Out_Bao_y].DrawImage(pDCBuff,m_BaoPosion.left-m_PngPushBox[m_Out_Bao_y].GetWidth()/2 ,m_BaoPosion.top-120+6*45-50);

        }
        else
        {
          m_PngPushBox[m_Out_Bao_y].DrawImage(pDCBuff,m_BaoPosion.left-m_PngPushBox[m_Out_Bao_y].GetWidth()/2 ,m_BaoPosion.top-120+m_Out_Bao_y*20-30);
        }
      }

    }
  }
  if(m_bShowJettonAn&&cbGameStatus!=GAME_STATUS_FREE&&0)
  {
    if(0<=m_bShowJettonIndex&&m_bShowJettonIndex<4)
    {
      m_PngShowJetton[m_bShowJettonIndex].DrawImage(pDCBuff,m_BaoPosion.left-m_PngShowJetton[m_bShowJettonIndex].GetWidth()/2 ,m_BaoPosion.top-120+6*20-30);
    }
  }
  if(m_bShowLeaveHandleAn&&cbGameStatus==GS_GAME_END&&0)
  {
    if(0<=m_bShowLeaveHandleIndex&&m_bShowLeaveHandleIndex<4)
    {
      m_PngShowLeaveHandle[m_bShowLeaveHandleIndex].DrawImage(pDCBuff,m_BaoPosion.left-m_PngShowLeaveHandle[m_bShowLeaveHandleIndex].GetWidth()/2 ,m_BaoPosion.top-120+6*20-30);
    }

  }

  return;
}

//������Ϣ
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
  if(m_lMeMaxScore!=lMeMaxScore)
  {
    //���ñ���
    m_lMeMaxScore=lMeMaxScore;
  }

  return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
  if(m_lAreaLimitScore!= lAreaLimitScore)
  {
    //���ñ���
    m_lAreaLimitScore=lAreaLimitScore;
  }
}

//���ó���
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{
  //���ñ���
  ASSERT(lCurrentJetton>=0L);
  m_lCurrentJetton=lCurrentJetton;
  if(lCurrentJetton==0)
  {
    SetJettonHide(0);
  }
  return;
}


//��ʷ��¼
void CGameClientView::SetGameHistory(BYTE *bcResulte)
{
  //��������
  BYTE bcResulteTmp[AREA_COUNT];
  memcpy(bcResulteTmp,bcResulte,AREA_COUNT);
  tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];


  for(int i = 1; i<=AREA_COUNT; i++)
  {

    if(bcResulteTmp[i-1]>0)
    {
      GameRecord.enOperateMen[i]=enOperateResult_Win;

    }
    else
    {
      GameRecord.enOperateMen[i]=enOperateResult_Lost;

    }
    /*if (0==m_lUserJettonScore[i]) GameRecord.enOperateMen[i]=enOperateResult_NULL;
    else if (m_lUserJettonScore[i] > 0 && (bcResulte[i-1]==4)) GameRecord.enOperateMen[i]=enOperateResult_Win;
    else if (m_lUserJettonScore[i] > 0 && (0==bcResulte[i-1])) GameRecord.enOperateMen[i]=enOperateResult_Lost;*/

  }
  //�ƶ��±�
  m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
  if(m_nRecordLast == m_nRecordFirst)
  {
    m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
    if(m_nScoreHead < m_nRecordFirst)
    {
      m_nScoreHead = m_nRecordFirst;
    }
  }

  int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
  if(MAX_FALG_COUNT < nHistoryCount)
  {
    m_btScoreMoveR.EnableWindow(TRUE);
  }

  //�Ƶ����¼�¼
  if(MAX_FALG_COUNT < nHistoryCount)
  {
    m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
    m_btScoreMoveL.EnableWindow(TRUE);
    m_btScoreMoveR.EnableWindow(FALSE);
  }

  return;
}

//�������
void CGameClientView::CleanUserJetton()
{
  //��������
  for(BYTE i=0; i<CountArray(m_JettonInfoArray); i++)
  {
    m_JettonInfoArray[i].RemoveAll();
  }
//  for ( int i = 0; i < CountArray(m_SkinListCtrl); ++i )
//  {
//    if ( m_SkinListCtrl[i].GetSafeHwnd() )
//      m_SkinListCtrl[i].DeleteAllItems();
//  }

  //���������ע
  ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));
  //��ע��Ϣ
  ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

  //ȫ����ע
  ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));


  ZeroMemory(m_JettonQue,sizeof(m_JettonQue));


  ZeroMemory(m_JettonQueArea,sizeof(m_JettonQueArea));




  m_JettonQueIndex = 0;


  if(m_pAdminControl)
  {
    m_pAdminControl->ClearClist();
  }


  //���½���
  RefreshGameView();

  return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
  //Ч�����
  ASSERT(cbViewIndex<=AREA_COUNT);
  if(cbViewIndex>AREA_COUNT)
  {
    return;
  }

  m_lUserJettonScore[cbViewIndex]=lJettonCount;

  bool blHave = false;
  for(int i = 0; i<18; i++)
  {
    if(m_JettonQueArea[i]==cbViewIndex&&m_JettonQueIndex>i)
    {
      m_JettonQue[i] = m_lUserJettonScore[cbViewIndex];

      blHave= true;
      break;

    }
  }
  if(blHave==false)
  {
    if(m_JettonQueIndex>COUNT_DRAWINFO-1)
    {
      for(int i = 0; i<COUNT_DRAWINFO-1; i++)
      {
        m_JettonQueArea[i]= m_JettonQueArea[i+1];
        m_JettonQue[i] = m_JettonQue[i+1];

      }
      m_JettonQue[COUNT_DRAWINFO-1]=m_lUserJettonScore[cbViewIndex];
      m_JettonQueArea[COUNT_DRAWINFO-1] = cbViewIndex;
    }
    else
    {
      m_JettonQueArea[m_JettonQueIndex]= cbViewIndex;
      m_JettonQue[m_JettonQueIndex++] = m_lUserJettonScore[cbViewIndex];

    }

    //m_JettonQueIndex = m_JettonQueIndex%16;

  }





  //���½���
  RefreshGameView();
}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[1][1])
{
  if(cbTableCardArray!=NULL)
  {
    CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

    //��ʼ����
    DispatchCard();
  }
  else
  {
    ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
  }
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
  //Ч�����
  ASSERT(cbViewIndex<=AREA_COUNT);
  if(cbViewIndex>AREA_COUNT)
  {
    return;
  }

  //��������
  bool bPlaceJetton = false;
  LONGLONG lScoreIndex[JETTON_COUNT] = {100L,1000L,10000L,100000L,1000000L,5000000L};

  //�߿���
  int nFrameWidth=0, nFrameHeight=0;
  int nBorderWidth=6;

  m_lAllJettonScore[cbViewIndex] += lScoreCount;
  nFrameWidth = m_RectArea [cbViewIndex-1].right-m_RectArea [cbViewIndex-1].left;
  nFrameHeight = m_RectArea[cbViewIndex-1].bottom-m_RectArea [cbViewIndex-1].top;


  //�����ж�
  bool bAddJetton = lScoreCount>0?true:false;

  if(lScoreCount < 0)
  {
    lScoreCount = -lScoreCount;
  }

  //���ӳ���
  for(BYTE i=0; i<CountArray(lScoreIndex); i++)
  {
    //������Ŀ
    BYTE cbScoreIndex = JETTON_COUNT-i-1;
    LONGLONG lCellCount = lScoreCount / lScoreIndex[cbScoreIndex];

    //�������
    if(lCellCount==0L)
    {
      continue;
    }

    //�������
    for(LONGLONG j=0; j<lCellCount; j++)
    {
      if(true == bAddJetton)
      {
        //�������
        tagJettonInfo JettonInfo;
        int nJettonSize=55;
        JettonInfo.cbJettonIndex=cbScoreIndex;
        int iWSize = nFrameWidth-nJettonSize-5;
        int iHSize =  nFrameHeight-nJettonSize-40;
        JettonInfo.nXPos=rand()%(iWSize);
        JettonInfo.nYPos=rand()%(iHSize);

        //��������
        bPlaceJetton = true;
        m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
      }
      else
      {
        for(int nIndex=0; nIndex<m_JettonInfoArray[cbViewIndex-1].GetCount(); ++nIndex)
        {
          //�Ƴ��ж�
          tagJettonInfo &JettonInfo=m_JettonInfoArray[cbViewIndex-1][nIndex];
          if(JettonInfo.cbJettonIndex==cbScoreIndex)
          {
            m_JettonInfoArray[cbViewIndex-1].RemoveAt(nIndex);
            break;
          }
        }
      }
    }

    //������Ŀ
    lScoreCount -= lCellCount*lScoreIndex[cbScoreIndex];
  }

  //���½���
  if(bPlaceJetton==true)
  {
    RefreshGameView();
  }

  return;
}

//��������ע
void CGameClientView::AndroidBet(BYTE cbViewIndex, LONGLONG lScoreCount)
{
  //Ч�����
  ASSERT(cbViewIndex<=AREA_COUNT);
  if(cbViewIndex>AREA_COUNT)
  {
    return;
  }

  if(lScoreCount <= 0L)
  {
    return;
  }

  tagAndroidBet Androi;
  Androi.cbJettonArea = cbViewIndex;
  Androi.lJettonScore = lScoreCount;
  m_ArrayAndroid.Add(Androi);
  SetTimer(IDI_ANDROID_BET,100,NULL);
  int nHaveCount = 0;
  for(int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
  {
    if(m_ArrayAndroid[i].lJettonScore > 0)
    {
      nHaveCount++;
    }
  }
  UINT nElapse = 0;
  if(nHaveCount <= 1)
  {
    nElapse = 260;
  }
  else if(nHaveCount <= 2)
  {
    nElapse = 160;
  }
  else
  {
    nElapse = 100;
  }

  SetTimer(IDI_ANDROID_BET+m_ArrayAndroid.GetCount(),nElapse,NULL);
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
  m_lMeCurGameScore=lMeCurGameScore;
  m_lMeCurGameReturnScore=lMeCurGameReturnScore;
  m_lBankerCurGameScore=lBankerCurGameScore;
  m_lGameRevenue=lGameRevenue;
}

//����ʤ��
void CGameClientView::SetWinnerSide(bool blWin[], bool bSet)
{
  //����ʱ��
  for(int i= 0; i<AREA_COUNT; i++)
  {

    m_bWinFlag[i]=blWin[i];
  }
  if(true==bSet)
  {
    //���ö�ʱ��
    SetTimer(IDI_FLASH_WINNER,500,NULL);

    //ȫʤ�ж�
    bool blWinAll = true;

    for(int i= 0; i<AREA_COUNT; i++)
    {

      if(m_bWinFlag[i]==true)
      {
        blWinAll = false;
      }
    }
    if(blWinAll)
    {
      //������Դ
      HINSTANCE hInstance=AfxGetInstanceHandle();
    }
  }
  else
  {
    //�����ʱ��
    KillTimer(IDI_FLASH_WINNER);

    //ȫʤ�ж�
    bool blWinAll = true;

    for(int i= 0; i<AREA_COUNT; i++)
    {

      if(m_bWinFlag[i]==true)
      {
        blWinAll = false;
      }
    }
    if(blWinAll)
    {
    }
  }

  //���ñ���
  m_bFlashResult=bSet;
  m_bShowGameResult=bSet;
  m_cbAreaFlash=0xFF;

  //���½���
  //RefreshGameView();

  return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
  for(int i = 0; i<AREA_COUNT; i++)
  {
    if(m_RectArea[i].PtInRect(MousePoint))
    {
      return i+1;
    }
  }
  return 0xFF;
}
//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool blTimer,bool bMeScore)
{
  //������Դ
  //CImageHandle HandleScoreNumber(&m_ImageScoreNumber);
  //CImageHandle HandleMeScoreNumber(&m_ImageMeScoreNumber);
  CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/11,m_ImageScoreNumber.GetHeight());

  if(bMeScore)
  {
    SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/11,m_ImageMeScoreNumber.GetHeight());
  }

  if(blTimer)
  {
    SizeScoreNumber.SetSize(m_TimerCount_png.GetWidth()/10, m_TimerCount_png.GetHeight());
  }

  //������Ŀ
  LONGLONG lNumberCount=0;
  LONGLONG lNumberTemp=lNumber;
  do
  {
    lNumberCount++;
    lNumberTemp/=10;
  }
  while(lNumberTemp>0);

  //λ�ö���
  INT nOffset=8;
  INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
  INT nXDrawPos=(INT)(nXPos+lNumberCount*SizeScoreNumber.cx/2+lNumberCount/4*(SizeScoreNumber.cx-nOffset)/2-SizeScoreNumber.cx);


  //�滭����
  for(LONGLONG i=0; i<lNumberCount; i++)
  {
    //�滭����
    if(i!=0&&i%3==0)
    {
      if(!blTimer)
      {
        if(bMeScore)
        {
          m_ImageMeScoreNumber.DrawImage(pDC,nXDrawPos+nOffset,nYDrawPos,SizeScoreNumber.cx-nOffset,SizeScoreNumber.cy,
                                         10*SizeScoreNumber.cx+nOffset/2,0);
        }
        else
        {
          m_ImageScoreNumber.DrawImage(pDC,nXDrawPos+nOffset,nYDrawPos,SizeScoreNumber.cx-nOffset,SizeScoreNumber.cy,
                                       10*SizeScoreNumber.cx+nOffset/2,0);
        }
        nXDrawPos-=(SizeScoreNumber.cx-nOffset);
      }
    }
    LONG lCellNumber=(LONG)(lNumber%10);
    if(blTimer)
    {
      m_TimerCount_png.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
                                 lCellNumber*SizeScoreNumber.cx,0);
    }
    else
    {
      if(bMeScore)
      {
        m_ImageMeScoreNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
                                       lCellNumber*SizeScoreNumber.cx,0);
      }
      else
      {
        m_ImageScoreNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
                                     lCellNumber*SizeScoreNumber.cx,0);
      }

    }
    //���ñ���
    lNumber/=10;
    nXDrawPos-=SizeScoreNumber.cx;
  };
  return;
}

//�滭����
void CGameClientView::DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber,CRect&rcPrint,INT nFormat)
{
  LONGLONG lTmpNumber = lNumber;
  CString strNumber;
  CString strTmpNumber1;
  CString strTmpNumber2;
  bool blfirst = true;
  bool bLongNum = false;
  int nNumberCount = 0;

  strTmpNumber1.Empty();
  strTmpNumber2.Empty();
  strNumber.Empty();

  if(lNumber == 0)
  {
    strNumber=TEXT("0");
  }

  if(lNumber < 0)
  {
    lNumber =- lNumber;
  }

  if(lNumber >= 100)
  {
    bLongNum = true;
  }

  while(lNumber > 0)
  {
    strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
    nNumberCount++;
    strTmpNumber2 = strTmpNumber1+strTmpNumber2;

    if(nNumberCount == 3)
    {
      if(blfirst)
      {
        strTmpNumber2 += (TEXT("") +strNumber);
        blfirst = false;
      }
      else
      {
        strTmpNumber2 += (TEXT(",") +strNumber);
      }

      strNumber = strTmpNumber2;
      nNumberCount = 0;
      strTmpNumber2 = TEXT("");
    }
    lNumber /= 10;
  }

  if(strTmpNumber2.IsEmpty() == FALSE)
  {
    if(bLongNum)
    {
      strTmpNumber2 += (TEXT(",") +strNumber);
    }
    else
    {
      strTmpNumber2 += strNumber;
    }

    strNumber = strTmpNumber2;
  }

  if(lTmpNumber < 0)
  {
    strNumber = TEXT("-") + strNumber;
  }
  //�������
  /*if (nFormat==-1) pDC->DrawText(strNumber,rcPrint,DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
  else pDC->DrawText(strNumber,rcPrint,nFormat);*/

  if(nFormat==-1)
  {
    CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcPrint, m_CurSelectColor, DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
  }
  else
  {
    CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcPrint, m_CurSelectColor, nFormat);
  }
}

//�滭����
//void CGameClientView::DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
//{
//  LONGLONG lTmpNumber = lNumber;
//  CString strNumber;
//  CString strTmpNumber1;
//  CString strTmpNumber2;
//  bool blfirst = true;
//  bool bLongNum = false;
//  int nNumberCount = 0;
//
//  strTmpNumber1.Empty();
//  strTmpNumber2.Empty();
//  strNumber.Empty();
//
//  if ( lNumber == 0 )
//    strNumber=TEXT("0");
//
//  if ( lNumber < 0 )
//    lNumber =- lNumber;
//
//  if( lNumber >= 100 )
//    bLongNum = true;
//
//  while(lNumber > 0)
//  {
//    strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
//    nNumberCount++;
//    strTmpNumber2 = strTmpNumber1+strTmpNumber2;
//
//    if ( nNumberCount == 3 )
//    {
//      if(blfirst)
//      {
//        strTmpNumber2 += (TEXT("") +strNumber);
//        blfirst = false;
//      }
//      else
//      {
//        strTmpNumber2 += (TEXT(",") +strNumber);
//      }
//
//      strNumber = strTmpNumber2;
//      nNumberCount = 0;
//      strTmpNumber2 = TEXT("");
//    }
//    lNumber /= 10;
//  }
//
//  if ( strTmpNumber2.IsEmpty() == FALSE )
//  {
//    if( bLongNum )
//      strTmpNumber2 += (TEXT(",") +strNumber);
//    else
//      strTmpNumber2 += strNumber;
//
//    strNumber = strTmpNumber2;
//  }
//
//  if ( lTmpNumber < 0 )
//    strNumber = TEXT("-") + strNumber;
//  //�������
//  pDC->TextOut(nXPos,nYPos,strNumber);
//}
//�滭����
CString CGameClientView::NumberStringWithSpace(LONGLONG lNumber)
{

  CString static strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
  strTmpNumber1.Empty();
  strTmpNumber2.Empty();
  strNumber.Empty();
  if(lNumber==0)
  {
    strNumber=TEXT("0");
  }
  int nNumberCount=0;
  LONGLONG lTmpNumber=lNumber;
  if(lNumber<0)
  {
    lNumber=-lNumber;
  }
  bool blfirst = true;
  while(lNumber>0)
  {
    strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
    nNumberCount++;
    strTmpNumber2 = strTmpNumber1+strTmpNumber2;

    if(nNumberCount==3)
    {
      if(blfirst)
      {
        strTmpNumber2 +=strNumber;
        blfirst = false;

      }
      else
      {
        strTmpNumber2 += (TEXT(",") +strNumber);
      }

      strNumber=strTmpNumber2;
      nNumberCount=0;
      strTmpNumber2=TEXT("");
    }
    lNumber/=10;
  }

  if(strTmpNumber2.IsEmpty()==FALSE)
  {
    if(blfirst==false)
    {
      strTmpNumber2 += (TEXT(",") +strNumber);
    }
    else
    {
      strTmpNumber2 += strNumber;
    }

    strNumber=strTmpNumber2;
  }

  if(lTmpNumber<0)
  {
    strNumber=TEXT("-")+strNumber;
  }

  return  strNumber;


}

//������
void CGameClientView::IsJettonDraw()
{
  //��������
  CRgn JettonRegion;        //��ʾ����
  CRgn SignedRegion;        //��������
  CRgn SignedAndJettonRegion;   //��������
  SignedAndJettonRegion.CreateRectRgn(0,0,0,0);

  //ͼƬ����
  CSize static SizeJettonItem(m_ImageJettonView.GetWidth()/6,m_ImageJettonView.GetHeight());

  //��������
  for(INT i = 0; i < AREA_COUNT; i++)
  {
    for(INT_PTR j = m_JettonInfoArray[i].GetCount() - 1; j >= 0; j--)
    {
      //��ȡ��Ϣ
      tagJettonInfo * pJettonInfo = &m_JettonInfoArray[i][j];
      //pJettonInfo->bShow = false;
      CRect rcRect(pJettonInfo->nXPos+m_PointJetton[i].x,
                   pJettonInfo->nYPos+m_PointJetton[i].y+16,
                   pJettonInfo->nXPos+m_PointJetton[i].x + m_ImageJettonView.GetWidth()/6,
                   pJettonInfo->nYPos+m_PointJetton[i].y+16 + m_ImageJettonView.GetHeight());

      if(pJettonInfo->bShow == false)
      {
        continue;
      }

      //�״�����
      if(!JettonRegion.GetSafeHandle())
      {
        JettonRegion.CreateEllipticRgnIndirect(rcRect);
        pJettonInfo->bShow = true;
        continue;
      }

      //�����ж�
      SignedRegion.CreateEllipticRgnIndirect(rcRect);
      SignedAndJettonRegion.CombineRgn(&JettonRegion,&SignedRegion, RGN_AND);
      if(SignedAndJettonRegion.EqualRgn(&SignedRegion))
      {
        pJettonInfo->bShow = false;
      }
      else
      {
        pJettonInfo->bShow = true;
        JettonRegion.CombineRgn(&JettonRegion,&SignedRegion,RGN_OR);
      }

      //�ϲ�����
      SignedRegion.DeleteObject();
    }

    //ɾ����ʾ����
    if(JettonRegion.GetSafeHandle())
    {
      JettonRegion.DeleteObject();
    }
  }

  //ɾ������
  if(JettonRegion.GetSafeHandle())
  {
    JettonRegion.DeleteObject();
  }
  if(SignedRegion.GetSafeHandle())
  {
    SignedRegion.DeleteObject();
  }
  if(SignedAndJettonRegion.GetSafeHandle())
  {
    SignedAndJettonRegion.DeleteObject();
  }
}
//void CGameClientView::SetMoveCardTimer()
//{
//  KillTimer(IDI_POSTCARD);
//  SetTimer(IDI_POSTCARD,300,NULL);
//  SetTimer(IDI_DISPATCH_CARD,5000,NULL);
//
//}
void CGameClientView::KillCardTime()
{
  KillTimer(IDI_FLASH_WINNER);
  KillTimer(IDI_POSTCARD);
  KillTimer(IDI_OPENCARD);
  KillTimer(IDI_DISPATCH_CARD);
  KillTimer(IDI_SHOWDISPATCH_CARD_TIP);

}
void CGameClientView::StartRunCar(int iTimer)
{
  m_bFlashrandShow = false;
  KillTimer(IDI_FLASH_RAND_SIDE);
  KillTimer(IDI_FLASH_CARD);
  KillTimer(IDI_SHOW_CARD_RESULT);
  iTimerStep = 400;
  SetTimer(IDI_FLASH_CARD,iTimer,NULL);
  ifirstTimer = 0;
  iOpenSide = m_cbTableCardArray[0][0];
  iTotoalRun = iOpenSide+32*3-2;
  iRunIndex = 0;
  m_CarIndex = 1;
  m_blRungingCar = true;
  SetTimer(IDI_SHOW_CARD_RESULT,20*1000,NULL);


}
void CGameClientView::RuningCar(int iTimer)
{
  if(iRunIndex<10)
  {
    iTimerStep-=43;

  }
  if(iRunIndex >= iTotoalRun-15)
  {
    iTimerStep+=47 /*8*(iRunIndex-iTotoalRun+15)*/;
  }
  if(iRunIndex==iTotoalRun)
  {
    KillTimer(IDI_FLASH_CARD);
    KillTimer(IDI_SHOW_CARD_RESULT);
    m_blRungingCar = false;
    m_blMoveFinish = true;
    //���ö�ʱ��
    this->DispatchCard();
    FinishDispatchCard();
    return ;

  }
  if(iTimerStep<0)
  {
    return ;
  }
  KillTimer(IDI_FLASH_CARD);
  SetTimer(IDI_FLASH_CARD,iTimer,NULL);

}
void CGameClientView::FlashWinOpenSide()
{

}
void CGameClientView::StartRandShowSide()
{
  KillTimer(IDI_FLASH_RAND_SIDE);
  iTimerStep = 100;
  m_bFlashrandShow = true;
  SetTimer(IDI_FLASH_RAND_SIDE,iTimerStep,NULL);


}
//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
  if(IDI_FLASH_CARD == nIDEvent)
  {
    m_CarIndex = (m_CarIndex+1)%32;
    iRunIndex++;
    RuningCar(iTimerStep);
    SendEngineMessage(IDM_SOUND,3,3);

    //���½���
    RefreshGameView();
  }
  if(IDI_SHOW_CARD_RESULT==nIDEvent)
  {
    for(int i = iRunIndex; i<iTotoalRun; i++)
    {
      m_CarIndex = (m_CarIndex+1)%32;
      iRunIndex++;
      if(iRunIndex==iTotoalRun)
      {
        KillTimer(IDI_FLASH_CARD);
        KillTimer(IDI_SHOW_CARD_RESULT);
        m_blRungingCar = false;
        m_blMoveFinish = true;
        //���ö�ʱ��
        this->DispatchCard();
        FinishDispatchCard();
        return ;
      }
    }
  }

  if(IDI_FLASH_RAND_SIDE ==nIDEvent)
  {
    m_bFlashrandShow=!m_bFlashrandShow;
    m_CarIndex = rand()%32;
    //���½���
    RefreshGameView();
  }
  //����ʤ��
  if(nIDEvent==IDI_FLASH_WINNER)
  {
    //���ñ���
    m_bFlashResult=!m_bFlashResult;

    //���½���
    RefreshGameView();
    return;
  }
  //�ֻ�ׯ��
  else if(nIDEvent == IDI_SHOW_CHANGE_BANKER)
  {
    ShowChangeBanker(false);
    return;
  }
  else if(nIDEvent==IDI_DISPATCH_CARD)
  {
    return;
  }
  else if(IDI_SHOWDISPATCH_CARD_TIP==nIDEvent)
  {
    SetDispatchCardTip(enDispatchCardTip_NULL);
  }
  else if(nIDEvent == IDI_ANDROID_BET)
  {
    //���½���
    RefreshGameView();
    return;
  }
  else if(nIDEvent >= (UINT)(IDI_ANDROID_BET + 1) && nIDEvent < (UINT)(IDI_ANDROID_BET + m_ArrayAndroid.GetCount() + 1))
  {
    INT_PTR Index = nIDEvent - IDI_ANDROID_BET - 1;
    if(Index < 0 || Index >= m_ArrayAndroid.GetCount())
    {
      ASSERT(FALSE);
      KillTimer(nIDEvent);
      return;
    }

    if(m_ArrayAndroid[Index].lJettonScore > 0)
    {
      LONGLONG lScoreIndex[] = {5000000L,1000000L,100000L,10000L,1000L,100L};
      BYTE cbViewIndex = m_ArrayAndroid[Index].cbJettonArea;

      //���ӳ���
      for(BYTE i=0; i<CountArray(lScoreIndex); i++)
      {
        if(lScoreIndex[i] > m_lRobotMaxJetton)
        {
          continue;
        }

        if(m_ArrayAndroid[Index].lJettonScore >= lScoreIndex[i])
        {
          m_ArrayAndroid[Index].lJettonScore -= lScoreIndex[i];
          m_lAllJettonScore[cbViewIndex] += lScoreIndex[i];

          tagJettonInfo JettonInfo;
          int iWSize = m_RectArea[cbViewIndex-1].right - m_RectArea[cbViewIndex-1].left - 60;
          int iHSize = m_RectArea[cbViewIndex-1].bottom - m_RectArea[cbViewIndex-1].top - 95;
          JettonInfo.nXPos=rand()%(iWSize);
          JettonInfo.nYPos=rand()%(iHSize);
          JettonInfo.cbJettonIndex = JETTON_COUNT - i - 1;
          JettonInfo.bShow=true;

          //��������
          m_JettonInfoArray[cbViewIndex-1].Add(JettonInfo);
          //��������
          AfxGetMainWnd()->PostMessage(IDM_SOUND,7,7);
          break;
        }
      }
    }
    return;
  }

  __super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
  if(m_lCurrentJetton!=0L)
  {
    int iTimer = 1;
    //��ע����
    BYTE cbJettonArea=GetJettonArea(Point);

    //�����ע
    LONGLONG lMaxJettonScore=GetUserMaxJetton(cbJettonArea);

    if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
    {
      return ;
    }

    //�Ϸ��ж�
    if(lMaxJettonScore < m_lCurrentJetton)
    {
      SetJettonHide(0);
      return ;
    }

    //������Ϣ
    if(cbJettonArea!=0xFF)
    {
      SendEngineMessage(IDM_PLACE_JETTON,cbJettonArea,(LPARAM)(&m_lCurrentJetton));
    }
  }
  RefreshGameView();

  __super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
  __super::OnLButtonUp(nFlags,Point);
}
//���������ؼ���������Ϣ
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
  CPoint *pPoint = (CPoint*)lParam;

  ScreenToClient(pPoint);
  OnLButtonUp(1,*pPoint);
  return 1;
}
void CGameClientView::SetEnablePlaceJetton(bool bEnablePlaceJetton)
{
  m_bEnablePlaceJetton = bEnablePlaceJetton;
}
void CGameClientView::UpdataJettonButton()
{
  if(m_CurArea==0xFF||m_bEnablePlaceJetton == false)
  {
    return ;
  }
  //�������
  LONGLONG lCurrentJetton=GetCurrentJetton();
  LONGLONG lLeaveScore=m_lMeMaxScore;
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lLeaveScore -= m_lUserJettonScore[nAreaIndex];
  }
  //�����ע
  LONGLONG lUserMaxJetton = 0;

  lUserMaxJetton = GetUserMaxJetton(m_CurArea);

  lLeaveScore = min((lLeaveScore),lUserMaxJetton); //�û����·� �����ֱȽ� �������屶


  lCurrentJetton = m_lLastJetton ;

  //���ù��
  if(lCurrentJetton>lLeaveScore)
  {
    if(lLeaveScore>=5000000L)
    {
      SetCurrentJetton(5000000L);
    }
    else if(lLeaveScore>=1000000L)
    {
      SetCurrentJetton(1000000L);
    }
    else if(lLeaveScore>=100000L)
    {
      SetCurrentJetton(100000L);
    }
    else if(lLeaveScore>=10000L)
    {
      SetCurrentJetton(10000L);
    }
    else if(lLeaveScore>=1000L)
    {
      SetCurrentJetton(1000L);
    }
    else if(lLeaveScore>=100L)
    {
      SetCurrentJetton(100L);
    }
    else
    {
      SetCurrentJetton(0L);
    }
  }
  else
  {
    SetCurrentJetton(m_lLastJetton);
  }
}
//����ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
  return __super::OnMouseMove(nFlags,point);
}
//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
  //���ñ���
  m_lCurrentJetton=0L;

  if(m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
  {
    m_cbAreaFlash=0xFF;
    SetJettonHide(0);
    RefreshGameView();
  }
  m_lLastJetton = 0L;

  __super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::SetJettonHide(int ID)
{
  CSkinButton *btJetton[]= {NULL,&m_btJetton100,&m_btJetton1000,&m_btJetton10000,&m_btJetton100000,&m_btJetton1000000,&m_btJetton5000000};

  if(1<=ID&&ID<=6)
  {
    for(int i = 1; i<=6; i++)
    {
      if(i!=ID)
      {
        btJetton[i]->ShowWindow(SW_SHOW);
      }
    }
    btJetton[ID]->ShowWindow(SW_HIDE);

    OutputDebugString(TEXT("����ID\r\n"));

  }
  else
  {
    for(int i = 1; i<=6; i++)
    {
      btJetton[i]->ShowWindow(SW_SHOW);
    }
  }
}
//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
  if(m_lCurrentJetton!=0L)
  {
    //��ȡ����
    CPoint MousePoint;
    GetCursorPos(&MousePoint);
    ScreenToClient(&MousePoint);
    BYTE cbJettonArea=GetJettonArea(MousePoint);

    //���ñ���
    if(m_cbAreaFlash != cbJettonArea)
    {
      m_cbAreaFlash = cbJettonArea;
      RefreshGameView();
    }


    //�����ж�
    if(cbJettonArea==0xFF)
    {
      SetCursor(LoadCursor(NULL,IDC_ARROW));
      SetJettonHide(0);
      return TRUE;
    }

    //�����ע
    LONGLONG lMaxJettonScore=GetUserMaxJetton(cbJettonArea);

    //�Ϸ��ж�
    int iTimer = 1;

    if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
    {
      SetJettonHide(0);
      SetCursor(LoadCursor(NULL,IDC_ARROW));
      return TRUE;
    }

    if(lMaxJettonScore< m_lCurrentJetton*iTimer)
    {
      SetJettonHide(0);
      SetCursor(LoadCursor(NULL,IDC_ARROW));
      return TRUE;
    }
    //���ù��
    switch(m_lCurrentJetton)
    {
      case 100:
      {
        SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
        return TRUE;
      }
      case 1000:
      {
        SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));
        return TRUE;
      }
      case 10000:
      {
        SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)));
        return TRUE;
      }
      case 100000:
      {
        SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
        return TRUE;
      }
      case 1000000:
      {
        SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));
        return TRUE;
      }
      case 5000000:
      {
        SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)));
        return TRUE;
      }
      RefreshGameView();
    }
  }
  else
  {
    ClearAreaFlash();
  }

  return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}

//�ֻ�ׯ��
void CGameClientView::ShowChangeBanker(bool bChangeBanker)
{
  //�ֻ�ׯ��
  if(bChangeBanker)
  {
    SetTimer(IDI_SHOW_CHANGE_BANKER, 3000, NULL);
    m_bShowChangeBanker = true;
  }
  else
  {
    KillTimer(IDI_SHOW_CHANGE_BANKER);
    m_bShowChangeBanker = false ;
  }

  //���½���
  RefreshGameView();
}

//��ׯ��ť
void CGameClientView::OnApplyBanker()
{
  SendEngineMessage(IDM_APPLY_BANKER,1,0);

}

//��ׯ��ť
void CGameClientView::OnCancelBanker()
{
  SendEngineMessage(IDM_APPLY_BANKER,0,0);
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
  //��������
  int nStringLength=lstrlen(pszString);
  int nXExcursion[8]= {1,1,1,0,-1,-1,-1,0};
  int nYExcursion[8]= {-1,0,1,1,1,0,-1,-1};

  //�滭�߿�
  pDC->SetTextColor(crFrame);
  for(int i=0; i<CountArray(nXExcursion); i++)
  {
    //pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
    CDFontEx::DrawText(this,pDC,  12, 400, pszString, nXPos+nXExcursion[i], nYPos+nYExcursion[i], crFrame, TA_CENTER|TA_TOP);
  }

  //�滭����
  pDC->SetTextColor(crText);
  //pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
  CDFontEx::DrawText(this,pDC,  12, 400, pszString, nXPos, nYPos, crText, TA_CENTER|TA_TOP);

  return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat)
{
  //��������
  INT nStringLength=lstrlen(pszString);
  INT nXExcursion[8]= {1,1,1,0,-1,-1,-1,0};
  INT nYExcursion[8]= {-1,0,1,1,1,0,-1,-1};

  //�滭�߿�
  for(INT i=0; i<CountArray(nXExcursion); i++)
  {
    //����λ��
    CRect rcFrame;
    rcFrame.top=rcRect.top+nYExcursion[i];
    rcFrame.left=rcRect.left+nXExcursion[i];
    rcFrame.right=rcRect.right+nXExcursion[i];
    rcFrame.bottom=rcRect.bottom+nYExcursion[i];

    //�滭�ַ�
    CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcFrame, crFrame, nDrawFormat);
  }

  //�滭�ַ�
  CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcRect, crText, nDrawFormat);
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
  LONGLONG lTmpNumber = lNumber;
  CString strNumber;
  CString strTmpNumber1;
  CString strTmpNumber2;
  bool blfirst = true;
  bool bLongNum = false;
  int nNumberCount = 0;

  strTmpNumber1.Empty();
  strTmpNumber2.Empty();
  strNumber.Empty();

  if(lNumber == 0)
  {
    strNumber=TEXT("0");
  }

  if(lNumber < 0)
  {
    lNumber =- lNumber;
  }

  if(lNumber >= 100)
  {
    bLongNum = true;
  }

  while(lNumber > 0)
  {
    strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
    nNumberCount++;
    strTmpNumber2 = strTmpNumber1+strTmpNumber2;

    if(nNumberCount == 3)
    {
      if(blfirst)
      {
        strTmpNumber2 += (TEXT("") +strNumber);
        blfirst = false;
      }
      else
      {
        strTmpNumber2 += (TEXT(",") +strNumber);
      }

      strNumber = strTmpNumber2;
      nNumberCount = 0;
      strTmpNumber2 = TEXT("");
    }
    lNumber /= 10;
  }

  if(strTmpNumber2.IsEmpty() == FALSE)
  {
    if(bLongNum)
    {
      strTmpNumber2 += (TEXT(",") +strNumber);
    }
    else
    {
      strTmpNumber2 += strNumber;
    }

    strNumber = strTmpNumber2;
  }

  if(lTmpNumber < 0)
  {
    strNumber = TEXT("-") + strNumber;
  }

  //�������
  if(nFormat==-1)
  {
    CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, RGB(255,255,255), DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
  }
  else
  {
    CDFontEx::DrawText(this,pDC,  12, 400, strNumber, rcRect, RGB(255,255,255), nFormat);
  }
}
//ׯ����Ϣ
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore)
{
  //ׯ�����Ӻ�
  WORD wBankerUser=INVALID_CHAIR;

  //�������Ӻ�
  if(INVALID_CHAIR!=dwBankerUserID)
  {
    for(WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
    {
      //tagUserData const *pUserData=GetUserInfo(wChairID);
      IClientUserItem * pClientUserItem=GetClientUserItem(wChairID);
      if(NULL!=pClientUserItem && dwBankerUserID==pClientUserItem->GetChairID())
      {
        wBankerUser=wChairID;
        break;
      }
    }
  }

  //�л��ж�
  if(m_wBankerUser!=wBankerUser)
  {
    m_wBankerUser=wBankerUser;
    m_wBankerTime=0L;
    m_lBankerWinScore=0L;
    m_lTmpBankerWinScore=0L;
  }
  m_lBankerScore=lBankerScore;
}
void CGameClientView::DrawMoveInfo(CDC* pDC,CRect rcRect)
{
  return ;
}
//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{
  //�ǿ��ж�
  if(m_nRecordLast==m_nRecordFirst)
  {
    return;
  }
  int nIndex = m_nScoreHead;
  COLORREF static clrOld ;
  clrOld = pDC->SetTextColor(RGB(52,116,23));

  m_CurSelectColor = RGB(52,116,23);
  int nDrawCount=0;
  while(nIndex != m_nRecordLast && (m_nRecordLast!=m_nRecordFirst) && nDrawCount < MAX_FALG_COUNT)
  {
    //ʤ����ʶ
    tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
    //λ�ñ���
    int static nYPos=0,nXPos=0;
    nYPos=m_nWinFlagsExcursionY+27;
    nXPos=m_nWinFlagsExcursionX+((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY)*33+2;

    //ʤ����ʶ
    int static nFlagsIndex=0;

    for(int i = 1; i<=AREA_COUNT; i++)
    {
      if(ClientGameRecord.enOperateMen[i]==enOperateResult_Win)
      {
        switch(i)
        {
          case 5:
            nFlagsIndex=1;
            break;
          case 6:
            nFlagsIndex=3;
            break;
          case 7:
            nFlagsIndex=5;
            break;
          case 8:
            nFlagsIndex=7;
            break;
          default:
            nFlagsIndex=(i-1)*2;
        }
      }
    }
    //�滭��ʶ
    m_ImageWinFlags.DrawImage(pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/8, \
                              m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/8 * nFlagsIndex, 0);
    //�ƶ��±�
    nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
    nDrawCount++;
  }
  pDC->SetTextColor(clrOld);
}
//�ֹ�����
afx_msg void CGameClientView::OnOpenCard()
{
  SendEngineMessage(IDM_OPEN_CARD,0,0);

}
//�Զ�����
afx_msg void CGameClientView::OnAutoOpenCard()
{
  SendEngineMessage(IDM_AUTO_OPEN_CARD,0,0);

}
//�ƶ���ť
void CGameClientView::OnScoreMoveL()
{
  if(m_nRecordFirst == m_nScoreHead)
  {
    return;
  }

  m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
  if(m_nScoreHead == m_nRecordFirst)
  {
    m_btScoreMoveL.EnableWindow(FALSE);
  }

  m_btScoreMoveR.EnableWindow(TRUE);

  //���½���
  RefreshGameView();
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
  int nHistoryCount = (m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
  if(nHistoryCount == MAX_FALG_COUNT)
  {
    return;
  }

  m_nScoreHead = (m_nScoreHead + 1) % MAX_SCORE_HISTORY;
  if(nHistoryCount-1 == MAX_FALG_COUNT)
  {
    m_btScoreMoveR.EnableWindow(FALSE);
  }

  m_btScoreMoveL.EnableWindow(TRUE);

  //���½���
  RefreshGameView();
}

//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{

  if(false==m_bShowGameResult)
  {
    return;
  }

  int nXPos = nWidth / 2 - 200+20+30;
  int nYPos = nHeight / 2 - 208;
  m_pngGameEnd.DrawImage(pDC, nXPos+2-20-30, nYPos+60,
                         m_pngGameEnd.GetWidth(), m_pngGameEnd.GetHeight(),0, 0);
  pDC->SetTextColor(RGB(255,255,255));
  m_CurSelectColor = RGB(255,255,255);

  CRect rcMeWinScore, rcMeReturnScore;
  rcMeWinScore.left = nXPos+2 + 40+30;
  rcMeWinScore.top = nYPos+70 + 32+10+10;
  rcMeWinScore.right = rcMeWinScore.left + 111;
  rcMeWinScore.bottom = rcMeWinScore.top + 34;

  rcMeReturnScore.left = nXPos+2 + 150+50;
  rcMeReturnScore.top = nYPos+70 + 32+10+10;
  rcMeReturnScore.right = rcMeReturnScore.left + 111;
  rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

  CString strMeGameScore, strMeReturnScore;
  DrawNumStrWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
  DrawNumStrWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

  CRect rcBankerWinScore;
  rcBankerWinScore.left = nXPos+2 + 40+30;
  rcBankerWinScore.top =  nYPos+70 + 32+10+35+10;
  rcBankerWinScore.right = rcBankerWinScore.left + 111;
  rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

  CString strBankerCurGameScore;
  strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
  DrawNumStrWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//͸���滭
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
  ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
  if(pDC == 0 || lpRect == 0 || clr == CLR_NONE)
  {
    return false;
  }
  //ȫ͸��
  if(abs(fAlpha) <= 0.000001)
  {
    return true;
  }

  for(LONG l=lpRect->top; l<lpRect->bottom; l++)
  {
    for(LONG k=lpRect->left; k<lpRect->right; k++)
    {
      COLORREF clrBk = pDC->GetPixel(k, l);
      COLORREF clrBlend = RGB(GetRValue(clrBk)*(1-fAlpha)+GetRValue(clr)*fAlpha,
                              GetGValue(clrBk)*(1-fAlpha)+GetGValue(clr)*fAlpha,
                              GetBValue(clrBk)*(1-fAlpha)+GetBValue(clr)*fAlpha);
      pDC->SetPixel(k, l, clrBlend);
    }
  }

  return true;
}
void   CGameClientView::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
  if(InArea==0xFF)
  {
    return ;
  }
  ZeroMemory(bcWinArea,bcAreaCount);


  LONGLONG lMaxSocre = 0;

  for(int i = 0; i<32; i++)
  {
    BYTE bcOutCadDataWin[AREA_COUNT];
    BYTE bcData[1];
    bcData[0]=i+1;
    m_GameLogic.GetCardType(bcData,1,bcOutCadDataWin);
    for(int j= 0; j<AREA_COUNT; j++)
    {

      if(bcOutCadDataWin[j]>0&&j==InArea-1)
      {
        LONGLONG Score = 0;
        for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
        {
          if(bcOutCadDataWin[nAreaIndex-1]>1)
          {
            Score += m_lAllJettonScore[nAreaIndex]*(bcOutCadDataWin[nAreaIndex-1]);
          }
        }
        if(Score>=lMaxSocre)
        {
          lMaxSocre = Score;
          CopyMemory(bcWinArea,bcOutCadDataWin,bcAreaCount);

        }
        break;
      }
    }
  }
}
//�����ע
LONGLONG CGameClientView::GetUserMaxJetton(BYTE cbJettonArea)
{
  if(cbJettonArea==0xFF)
  {
    return 0;
  }

  //����ע��
  LONGLONG lNowJetton = 0;
  ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lNowJetton += m_lUserJettonScore[nAreaIndex];
  }
  //ׯ�ҽ��
  LONGLONG lBankerScore = 0x7fffffffffffffff;
  if(m_wBankerUser!=INVALID_CHAIR)
  {
    lBankerScore = m_lBankerScore;
  }

  BYTE bcWinArea[AREA_COUNT];
  LONGLONG LosScore = 0;
  LONGLONG WinScore = 0;

  GetAllWinArea(bcWinArea,AREA_COUNT,cbJettonArea);

  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    if(bcWinArea[nAreaIndex-1]>1)
    {
      LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
    }
    else
    {
      if(bcWinArea[nAreaIndex-1]==0)
      {
        WinScore+=m_lAllJettonScore[nAreaIndex];
      }
    }
  }

  LONGLONG lTemp = lBankerScore;
  lBankerScore = lBankerScore + WinScore - LosScore;

  if(lBankerScore < 0)
  {
    if(m_wBankerUser!=INVALID_CHAIR)
    {
      lBankerScore = m_lBankerScore;
    }
    else
    {
      lBankerScore = 0x7fffffffffffffff;
    }
  }

  //��������
  LONGLONG lMeMaxScore;

  if((m_lMeMaxScore - lNowJetton)>m_lAreaLimitScore)
  {
    lMeMaxScore= m_lAreaLimitScore;
  }
  else
  {
    lMeMaxScore = m_lMeMaxScore-lNowJetton;
    lMeMaxScore = lMeMaxScore;
  }

  //ׯ������
  lMeMaxScore=min(lMeMaxScore,(lBankerScore)/(bcWinArea[cbJettonArea-1]));

  //��������
  lMeMaxScore = max(lMeMaxScore, 0);

  return lMeMaxScore;
}

//�ɼ�����
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
  m_lMeCurGameScore=lMeCurGameScore;
  m_lMeCurGameReturnScore=lMeCurGameReturnScore;
  m_lBankerCurGameScore=lBankerCurGameScore;
}

void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
  //�滭����
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    if(m_lUserJettonScore[nAreaIndex] > 0)
    {
      DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointJettonNumber[nAreaIndex-1].x,m_PointJettonNumber[nAreaIndex-1].y+25, false,true);
    }
  }
}

//��ʼ����
void CGameClientView::DispatchCard()
{

  //���ñ�ʶ
  m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard(bool bRecord /*= true*/)
{
  //����ж�
  if(m_bNeedSetGameRecord==false)
  {
    return;
  }

  //���ñ�ʶ
  m_bNeedSetGameRecord=false;

  //ɾ����ʱ��
  KillTimer(IDI_DISPATCH_CARD);

  BYTE  bcResulteOut[AREA_COUNT];
  memset(bcResulteOut,0,AREA_COUNT);

  m_GameLogic.GetCardType(&m_cbTableCardArray[0][0],1,bcResulteOut);

  //�����¼
  if(bRecord)
  {
    SetGameHistory(bcResulteOut);
  }

  //�ۼƻ���
  m_lMeStatisticScore+=m_lMeCurGameScore;
  m_lBankerWinScore=m_lTmpBankerWinScore;


  bool blWin[AREA_COUNT];
  for(int i = 0; i<AREA_COUNT; i++)
  {

    if(bcResulteOut[i]>0)
    {
      blWin[i]=true;
    }
    else
    {
      blWin[i]=false;
    }
  }

  //����Ӯ��
  SetWinnerSide(blWin, true);

  //��������
  if(m_lMeCurGameScore>0)
  {
    SendEngineMessage(IDM_SOUND,5,5);

  }
  else if(m_lMeCurGameScore<0)
  {
    SendEngineMessage(IDM_SOUND,4,4);

  }
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
  //�Ϸ��ж�
  if(m_bFlashResult&&!m_blRungingCar)
  {
    m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
                           m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());
  }

  if(m_cbAreaFlash==0xFF && false==m_bFlashResult)
  {
    return;
  }
  //��ע�ж�
  if(false==m_bFlashResult)
  {
    if(m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)
    {
      //pDC->Draw3dRect(m_RectArea[m_cbAreaFlash-1].left,m_RectArea[m_cbAreaFlash-1].top,m_RectArea[m_cbAreaFlash-1].Width(),m_RectArea[m_cbAreaFlash-1].Height(),RGB(255,255,0),RGB(255,255,0));
      m_ImageFrameTianMen.DrawImage(pDC,m_RectArea[m_cbAreaFlash-1].left,m_RectArea[m_cbAreaFlash-1].top);
    }
  }
  else
  {
    for(int i = 0; i<AREA_COUNT; i++)
    {
      if(m_bWinFlag[i])
      {
        m_ImageFrameTianMen.DrawImage(pDC,m_RectArea[i].left,m_RectArea[i].top);
        //pDC->Draw3dRect(m_RectArea[i].left,m_RectArea[i].top,m_RectArea[i].Width(),m_RectArea[i].Height(),RGB(255,255,0),RGB(255,255,0));
      }
    }
  }
}

//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(bool bWinMen[])
{
  BYTE bcData = m_cbTableCardArray[0][0];
  if(1==bcData||bcData==2||bcData==9||bcData==10||bcData==17||bcData==18||bcData==25||bcData==26)
  {
    if(bcData%2==1)
    {
      bWinMen[0]= true;
    }
    else
    {
      bWinMen[1]= true;
    }

  }
  else if(3==bcData||bcData==4||bcData==3+8||bcData==4+8||bcData==3+2*8||bcData==4+2*8||bcData==3+3*8||bcData==4+3*8)
  {
    if(bcData%2==1)
    {
      bWinMen[2]= true;
    }
    else
    {
      bWinMen[3]= true;
    }



  }
  else if(5==bcData||bcData==6||bcData==5+8||bcData==6+8||bcData==5+2*8||bcData==6+2*8||bcData==5+3*8||bcData==6+3*8)
  {
    if(bcData%2==1)
    {
      bWinMen[4]= true;
    }
    else
    {
      bWinMen[5]= true;
    }

  }
  else if(7==bcData||bcData==8||bcData==7+8||bcData==8+8||bcData==7+2*8||bcData==8+2*8||bcData==7+3*8||bcData==8+3*8)
  {
    if(bcData%2==1)
    {
      bWinMen[6]= true;
    }
    else
    {
      bWinMen[7]= true;
    }

  }
}

//�ؼ�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
  //��ȡID
  WORD wControlID=LOWORD(wParam);

  //�ؼ��ж�
  switch(wControlID)
  {
    case IDC_JETTON_BUTTON_100:
    {
      //���ñ���
      m_lCurrentJetton=100L;
      break;
    }
    case IDC_JETTON_BUTTON_1000:
    {
      //���ñ���
      m_lCurrentJetton=1000L;
      break;
    }
    case IDC_JETTON_BUTTON_10000:
    {
      //���ñ���
      m_lCurrentJetton=10000L;
      break;
    }
    case IDC_JETTON_BUTTON_50000:
    {
      //���ñ���
      m_lCurrentJetton=50000L;
      break;
    }
    case IDC_JETTON_BUTTON_100000:
    {
      //���ñ���
      m_lCurrentJetton=100000L;
      break;
    }
    case IDC_JETTON_BUTTON_500000:
    {
      //���ñ���
      m_lCurrentJetton=500000L;
      break;
    }
    case IDC_JETTON_BUTTON_1000000:
    {
      //���ñ���
      m_lCurrentJetton=1000000L;
      break;
    }
    case IDC_JETTON_BUTTON_5000000:
    {
      //���ñ���
      m_lCurrentJetton=5000000L;
      break;
    }
    case IDC_AUTO_OPEN_CARD:
    {
      break;
    }
    case IDC_OPEN_CARD:
    {
      break;
    }
    case IDC_BANK:
    {
      break;
    }
    case IDC_CONTINUE_CARD:
    {
      SwithToNormalView();
      SendEngineMessage(IDM_CONTINUE_CARD,0,0);
      break;
    }
    case IDC_RADIO:
    {
      m_CheckImagIndex = 0;

    }

    break;
    case IDC_RADIO+1:
    {
      m_CheckImagIndex = 1;
    }
    break;
    case IDC_RADIO+2:
    {
      m_CheckImagIndex = 2;

    }
    break;
    case IDC_RADIO+3:
    {
      m_CheckImagIndex = 3;

    }
    break;
    case IDC_BANK_DRAW:
    {
      OnBankDraw();
      break;
    }
    case IDC_BANK_STORAGE:
    {
      OnBankStorage();
      break;
    }

  }

  m_lLastJetton = m_lCurrentJetton;

  return CGameFrameViewGDI::OnCommand(wParam, lParam);
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
  //�������Ӻ�
  for(WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
  {
    IClientUserItem * pClientUserItem=GetClientUserItem(wChairID);
    if(NULL!=pClientUserItem && dwMeUserID==pClientUserItem->GetChairID())
    {
      m_wMeChairID=wChairID;
      break;
    }
  }
}

//������ʾ
void CGameClientView::SetDispatchCardTip(enDispatchCardTip DispatchCardTip)
{
  //���ñ���
  m_enDispatchCardTip=DispatchCardTip;

  //���ö�ʱ��
  if(enDispatchCardTip_NULL!=DispatchCardTip)
  {
    SetTimer(IDI_SHOWDISPATCH_CARD_TIP,2*1000,NULL);
  }
  else
  {
    KillTimer(IDI_SHOWDISPATCH_CARD_TIP);
  }

  //���½���
  RefreshGameView();
}

//�滭ׯ��
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{
  CBrush brush;
  brush.CreateSolidBrush(RGB(255,0,0));
  //ׯ����Ϣ
  pDC->SetTextColor(RGB(255,255,0));
  m_CurSelectColor = RGB(255,255,0);
  //��ȡ���
  //tagUserData const *pUserData = m_wBankerUser==INVALID_CHAIR ? NULL : GetUserInfo(m_wBankerUser);

  IClientUserItem * pClientUserItem=m_wBankerUser==INVALID_CHAIR ? NULL :GetClientUserItem(m_wBankerUser);

  CRect  StrRect;
  StrRect.left = m_LifeWidth+89;
  StrRect.top  = m_TopHeight+55;
  StrRect.right = StrRect.left + 90;
  StrRect.bottom = StrRect.top + 15;

  /*tagUserInfo* pUserInfo=NULL;

  if (pClientUserItem!=NULL)
  {
    pUserInfo =  pClientUserItem->GetUserInfo();
  }*/
  //ׯ������
  TCHAR Text[256];
  if(m_bEnableSysBanker)
  {
    myprintf(Text,256,_TEXT("%s"),pClientUserItem==NULL?(TEXT("ϵͳ��ׯ")):pClientUserItem->GetNickName());
    //pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );

    CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, m_CurSelectColor, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE);
  }
  else
  {
    myprintf(Text,256,_TEXT("%s"),pClientUserItem==NULL?(TEXT("������ׯ")):pClientUserItem->GetNickName());
    //pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );

    CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, m_CurSelectColor, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE);
  }

  StrRect.left = StrRect.left+150;
  StrRect.right = StrRect.left +30;
  myprintf(Text,256,_TEXT("%d"),m_wBankerTime);
  //pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );

  CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, m_CurSelectColor, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE);

  StrRect.left = StrRect.left+90;
  StrRect.right = StrRect.left +80;
  DrawNumStrWithSpace(pDC,pClientUserItem==NULL?0:pClientUserItem->GetUserScore(),StrRect);

  StrRect.left = StrRect.left+96+45;
  StrRect.right = StrRect.left +85;
  DrawNumStrWithSpace(pDC,m_lBankerWinScore,StrRect);
}

// �滭��ׯ�б�
void CGameClientView::DrawBankerList(CDC *pDC, int nWidth, int nHeight)
{
  // �������
  CPoint ptBegin(0,0);
  // ������ׯ�б�
  ptBegin.SetPoint(nWidth/2 + 195, nHeight/2+195);

  int   nCount = 0;
  CRect rect(0, 0, 0, 0);
  rect.SetRect(ptBegin.x , ptBegin.y , ptBegin.x + 143, ptBegin.y + 57);

  for(int i = m_nShowValleyIndex; i < m_ValleysList.GetCount(); ++i)
  {
    IClientUserItem* pIClientUserItem = GetClientUserItem(m_ValleysList[i]);
    if(pIClientUserItem == NULL)
    {
      continue;
    }
    // ����
    CRect rectName(rect);
    rectName.left = ptBegin.x;
    rectName.right = ptBegin.x + 75;
    DrawTextString(pDC, pIClientUserItem->GetNickName(), RGB(255,234,0) , RGB(38,26,2), rectName, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

    // ����
    CRect rectScore(rect);
    rectScore.left = ptBegin.x + 80;
    DrawNumberStringWithSpace(pDC,pIClientUserItem->GetUserScore(), rectScore, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

    // λ�õ���
    rect.OffsetRect(0, 16);

    // λ������
    nCount++;

    if(nCount == 4)
    {
      break;
    }
  }

}


void CGameClientView::SetFirstShowCard(BYTE bcCard)
{


}
//�滭���
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
  if(INVALID_CHAIR==m_wMeChairID)
  {
    return;
  }
  //ׯ����Ϣ
  pDC->SetTextColor(RGB(255,255,0));

  m_CurSelectColor = RGB(255,255,0);
  //��ȡ���
  //tagUserData const *pUserData =  GetUserInfo(m_wMeChairID);

  IClientUserItem * pClientUserItem=GetClientUserItem(m_wMeChairID);

  CRect static StrRect;
  StrRect.left = m_LifeWidth+77;
  StrRect.top = m_TopHeight+581;
  StrRect.right =StrRect.left+87;
  StrRect.bottom=StrRect.top+20;

  //ׯ������
  TCHAR Text[256];
  tagUserInfo* pUserInfo =  pClientUserItem->GetUserInfo();
  myprintf(Text,256,_TEXT("%s"),pUserInfo->szNickName);
  //pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );

  CDFontEx::DrawText(this,pDC,  12, 400, Text, StrRect, m_CurSelectColor, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE);

  StrRect.left = StrRect.left+15;
  StrRect.top = StrRect.bottom;
  StrRect.right = StrRect.left + 87;
  StrRect.bottom = StrRect.top + 20;
  LONGLONG lMeJetton=0L;
  for(int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
  {
    lMeJetton += m_lUserJettonScore[nAreaIndex];
  }
  DrawNumStrWithSpace(pDC,pClientUserItem->GetUserScore(),StrRect);

  StrRect.top = StrRect.bottom;
  StrRect.right = StrRect.left + 87;
  StrRect.bottom = StrRect.top + 20;
  DrawNumStrWithSpace(pDC,m_lMeStatisticScore,StrRect);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//���д��
void CGameClientView::OnBankStorage()
{
  //��ȡ�ӿ�
  CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
  IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

  if(NULL!=pIClientKernel)
  {
    CRect btRect;
    m_btBankStorage.GetWindowRect(&btRect);
    ShowInsureSave(pIClientKernel,CPoint(btRect.left-100,5*btRect.top));
  }
}

//����ȡ��
void CGameClientView::OnBankDraw()
{
  //��ȡ�ӿ�
  CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
  IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

  if(NULL!=pIClientKernel)
  {
    CRect btRect;
    m_btBankDraw.GetWindowRect(&btRect);
    ShowInsureGet(pIClientKernel,CPoint(btRect.left-100,5*btRect.top));
  }
}


void CGameClientView::OnUp()
{
  if(m_nShowValleyIndex > 0)
  {
    m_nShowValleyIndex--;
  }

  //���½���
  InvalidGameView(0,0,0,0);

}
void CGameClientView::OnDown()
{
  if(m_nShowValleyIndex < m_ValleysList.GetCount() - 1)
  {
    m_nShowValleyIndex++;
  }

  //���½���
  InvalidGameView(0,0,0,0);
};
//void CGameClientView::StartMove()
//{
//  SetTimer(IDI_MOVECARD_END,250,NULL);
//  m_Out_Bao_y = 0;
//  m_bShowJettonIndex = 0;
//  m_bShowLeaveHandleIndex = 0;
//  m_bShowBao = TRUE;
//  m_bShowJettonAn = false;
//  m_bShowLeaveHandleAn = false;
//  m_bOPenBoxAn = false;
//  m_bOPenBoxIndex = 0;
//  m_blShowLastResult = false;
//  m_bShowResult = false;
//
//}
//void CGameClientView::StartJetton_AniMationN()
//{
//  SetTimer(IDI_JETTON_ANIMATION,400,NULL);
//  m_bShowJettonAn = true;
//  m_bShowJettonIndex = 0;
//
//
//}

//void CGameClientView::StartOPenBox()
//{
//  m_bOPenBoxAn = true;
//  m_bOPenBoxIndex = 0;
//  m_bShowBao = false;
//  m_Out_Bao_y = 6;
//  SetJettonHide(0);
//  SetTimer(IDI_OPENBOX_ANIMATION,400,NULL);
//
//}

//void CGameClientView::StartHandle_Leave()
//{
//  m_bShowLeaveHandleAn = true;
//  m_bShowLeaveHandleIndex = 0;
//  SetTimer(IDI_HANDLELEAVE_ANIMATION,400,NULL);
//
//  SendEngineMessage(IDM_SOUND,0,1);
//
//
//}
//void CGameClientView::StarShowResult()
//{
//  m_bShowResult = true;
//
//}
//�滭ʱ��
void CGameClientView::MyDrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
  //������Դ
  CPngImageEx ImageTimeBack;
  CPngImageEx ImageTimeNumber;
  ImageTimeBack.LoadImage(this,GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_BACK"));
  ImageTimeNumber.LoadImage(this,GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("TIME_NUMBER"));

  //��ȡ����
  const INT nNumberHeight=ImageTimeNumber.GetHeight();
  const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

  //������Ŀ
  LONGLONG lNumberCount=0;
  WORD wNumberTemp=wTime;
  do
  {
    lNumberCount++;
    wNumberTemp/=10;
  }
  while(wNumberTemp>0L);

  //λ�ö���
  INT nYDrawPos=nYPos-nNumberHeight/2+1;
  INT nXDrawPos=static_cast<INT>(nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth);

  //�滭����
  CSize SizeTimeBack(ImageTimeBack.GetWidth(),ImageTimeBack.GetHeight());
  ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

  //�滭����
  for(LONGLONG i=0; i<lNumberCount; i++)
  {
    //�滭����
    WORD wCellNumber=wTime%10;
    ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

    //���ñ���
    wTime/=10;
    nXDrawPos-=nNumberWidth;
  };

  return;
}

//����Ա����
void CGameClientView::OpenAdminWnd()
{
  //��Ȩ��
  if(m_pAdminControl!=NULL)
  {
    if(!m_pAdminControl->IsWindowVisible())
    {
      m_pAdminControl->ShowWindow(SW_SHOW);
    }
    else
    {
      m_pAdminControl->ShowWindow(SW_HIDE);
    }
  }
}

//ִ��ʣ�����еĻ��嶯��
void CGameClientView::PerformAllBetAnimation()
{
  KillTimer(IDI_ANDROID_BET);
  for(int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
  {
    KillTimer(IDI_ANDROID_BET + i + 1);
    PlaceUserJetton(m_ArrayAndroid[i].cbJettonArea, m_ArrayAndroid[i].lJettonScore);
  }
  m_ArrayAndroid.RemoveAll();
}

//������ע���
void CGameClientView::ControlBetDetection(BYTE cbViewIndex, LONGLONG lScoreCount, WORD wViewChair, WORD wChair)
{
  if(m_pAdminControl == NULL || wChair >= GAME_PLAYER)
  {
    return;
  }

  //��ȡ���
  m_lAllPlayBet[wViewChair][cbViewIndex-1] += lScoreCount;
  IClientUserItem* pUserData = GetClientUserItem(wViewChair);
  m_pAdminControl->UpdateClistCtrl(m_lAllPlayBet,cbViewIndex,lScoreCount,wChair,wViewChair,pUserData);


//  //�����Ϣ
//  IClientUserItem* pUserData = GetClientUserItem(wChair);
//  int nListIndex = cbViewIndex-1;
//
//  //�������
//  LVFINDINFO lvFindInfo;
//  ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
//  lvFindInfo.flags = LVFI_STRING;
//  lvFindInfo.psz = (LPCTSTR)pUserData->GetNickName();
//
//  //��������
//  int nItem = m_SkinListCtrl[nListIndex].FindItem( &lvFindInfo );
//  if ( nItem == -1 )
//  {
//    nItem = m_SkinListCtrl[nListIndex].InsertItem(m_SkinListCtrl[nListIndex].GetItemCount(), pUserData->GetNickName());//������
//  }
//
//
//  CString strScore;
//  strScore.Format(TEXT("%I64d"), m_lAllPlayBet[wChair][cbViewIndex]);
//  m_SkinListCtrl[nListIndex].SetItemText(nItem, 1, strScore);   //��������
}

//������ͼ
void CGameClientView::RefreshGameView()
{
  CRect rect;
  GetClientRect(&rect);
  InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

  return;
}