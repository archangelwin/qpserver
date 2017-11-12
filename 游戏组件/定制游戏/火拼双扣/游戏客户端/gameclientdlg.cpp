#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "cassert"
//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD          200               //���ƶ�ʱ��
#define IDI_START_GAME          201               //��ʼ��ʱ��
#define IDI_PASS_CARD         202               //���ƶ�ʱ��
#ifdef _DEBUG
#define TIME_OUT_CARD         30
#define TIME_PASS_CARD          2
#define TIME_START_GAME         20
#define TIME_AUTO_OUT         8               //�й�ʱ��
#else
#define TIME_OUT_CARD         30
#define TIME_PASS_CARD          2
#define TIME_START_GAME         60
#define TIME_AUTO_OUT         8               //�й�ʱ��
#endif


#define MAX_TIMEOUT_TIMES       3               //���ʱ����

//��Ϸ��ʱ��
#define IDI_WAIT_CLEAR          300               //����ȴ�
#define IDI_DISPATCH_CARD       301               //���ƶ�ʱ��
#define IDI_LAST_TURN_CARD        302               //���ֶ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
  ON_WM_TIMER()
  ON_MESSAGE(IDM_START,OnStart)
  ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
  ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
  ON_MESSAGE(IDM_CARD_TYPE,OnCardType)
  ON_MESSAGE(IDM_REQ_HUNTER,OnReqHunter)
  ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
  ON_MESSAGE(IDM_SORT_BY_COUNT,OnSortByCount)
  ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
  ON_MESSAGE(IDM_LEFT_DBHIT_CARD,OnLeftDBHitCard)
  ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
  ON_MESSAGE(IDM_LAST_TURN_CARD,OnLastTurnCard)
  ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
  ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
  //��Ϸ����
  m_bReqHumter=false;
  m_wBankerUser=INVALID_CHAIR;
  m_wCurrentUser=INVALID_CHAIR;

  //ʤ����Ϣ

  m_wWinOrder=0xFFFF;

  //���ñ���
  m_bStustee=false;
  m_bSortCount=false;
  m_dwCardHSpace=DEFAULT_PELS;

  //�˿˱���
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
  ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));
  ZeroMemory(m_cbUserCardData,sizeof(m_cbUserCardData));


  //���Ʊ���
  m_cbTurnCardCount=0;
  ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
  ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

  //�����˿�
  m_bLastTurn=false;
  ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
  memset(m_cbOutCardCount,255,sizeof(m_cbOutCardCount));

  ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

  //��������
  m_cbRemnantCardCount=0;
  m_cbDispatchCardCount=0;
  m_wMostUser=INVALID_CHAIR;
  m_wTimeOutCount=0;
  ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

  //��ʼ��Ϣ
  m_wGetRandCardID=INVALID_CHAIR;
  m_cbGetCardPosition=0xFF;
  m_cbRandCardData=0xFF;
  m_bHaveOutCard = false;
  return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{

  //����ͼ��
  HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
  m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
  SetIcon(hIcon,TRUE);
  SetIcon(hIcon,FALSE);
  //��ȡ����
  m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);
  //��������
  if((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS))
  {
    m_dwCardHSpace=DEFAULT_PELS;
  }
  return true;
}


//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
  //��Ϸ����
  m_wBankerUser=INVALID_CHAIR;

  //ɾ����ʱ
  KillGameClock(0);
  KillTimer(IDI_WAIT_CLEAR);
  KillTimer(IDI_DISPATCH_CARD);
  //��Ϸ����
  m_wCurrentUser=INVALID_CHAIR;
  //���ñ���
  m_bStustee=false;
  m_bSortCount=false;

  //ʤ����Ϣ
  m_wWinOrder=0xFFFF;

  //�˿˱���
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
  ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));
  ZeroMemory(m_cbUserCardData,sizeof(m_cbUserCardData));
  //���Ʊ���
  m_cbTurnCardCount=0;
  ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
  ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

  ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));

  //�����˿�
  m_bLastTurn=false;
  ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
  memset(m_cbOutCardCount,255,sizeof(m_cbOutCardCount));

  //��������
  m_cbRemnantCardCount=0;
  m_cbDispatchCardCount=0;
  m_wTimeOutCount=0;
  m_wMostUser=INVALID_CHAIR;
  ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));
  //��ʼ��Ϣ
  m_wGetRandCardID=INVALID_CHAIR;
  m_cbGetCardPosition=0xFF;
  m_cbRandCardData=0xFF;
  m_bHaveOutCard = false;
  return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
  return;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
  return true;
}
//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
  switch(wClockID)
  {
    case IDI_OUT_CARD:      //�û�����
    {

      if(IsLookonMode())
      {
        return true;
      }


      //�Զ�����
      if(wChairID == GetMeChairID() &&
         (nElapse <= 0 || (m_bStustee && (int)nElapse < TIME_AUTO_OUT - 2)))
      {

        //�Զ��й�
        if(!m_bStustee && nElapse == 0 && m_wTimeOutCount < 3)
        {
          m_wTimeOutCount++;
          if(m_wTimeOutCount == 3)
          {
            OnStusteeControl(0,0);
            if(m_pIStringMessage != NULL)
            {
              m_pIStringMessage->InsertSystemString(TEXT("��������ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
            }
            m_bStustee=true;
          }
        }

        //�Զ�����
        AutomatismOutCard();

        return true;
      }


      ////�����й�
      //if(wChairID==GetMeChairID())
      //{
      //  //���ʱ��ľ������Ϊ���й�״̬
      //  if(nElapse==0)
      //  {
      //    KillGameClock(IDI_OUT_CARD);
      //    m_wTimeOutCount++;
      //    AutomatismOutCard();
      //  }
      //  //�йܴ���
      //  if (m_wTimeOutCount>=MAX_TIMEOUT_TIMES&&!m_bStustee)
      //  {
      //    OnStusteeControl(0,0);
      //    if(m_pIStringMessage != NULL)
      //      m_pIStringMessage->InsertSystemString(TEXT("��������ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
      //    m_bStustee=true;
      //  }
      //
      //}




      //��������
      if((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
      }

      if((wChairID==GetMeChairID())&&(IsLookonMode()==false))
      {
        if(nElapse<5)
        {
          PlayGameSound(AfxGetInstanceHandle(),TEXT("DAO_TIMES"));
        }

      }

      return true;
    }
    case IDI_PASS_CARD:
    {
      if(nElapse==0)
      {
        OnPassCard(0,0);
      }
      return true;
    }
    case IDI_START_GAME:    //��ʼ��Ϸ
    {
      if(nElapse==0)
      {
        if((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
          //      AfxGetMainWnd()->PostMessage(WM_CLOSE);
        {
          return false;
        }
      }
      if((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
      }

      return true;
    }
  }

  return false;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
  //�Թ�����
  m_GameClientView.m_HandCardControl[GetMeChairID()].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));
  return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
  switch(wSubCmdID)
  {
    case SUB_S_GAME_START:    //��Ϸ��ʼ
    {
      return OnSubGameStart(pData,wDataSize);
    }
    case SUB_S_OUT_CARD:    //�û�����
    {
      return OnSubOutCard(pData,wDataSize);
    }
    case SUB_S_PASS_CARD:   //��������
    {
      return OnSubPassCard(pData,wDataSize);
    }
    case SUB_S_GAME_END:    //��Ϸ����
    {
      return OnSubGameEnd(pData,wDataSize);
    }
    case SUB_S_TRUSTEE:     //�û��й�
    {
      return OnSubTrustee(pData,wDataSize);
    }
    case SUB_S_ON_READY:    //�û�׼��
    {
      return OnSubUserReady(pData,wDataSize);
    }
    case SUB_S_ADMIN_COMMDN:    //����
    {
      return OnSubAdminControl(pData,wDataSize);
    }
    case SUB_S_CHEAT_CARD:    //��������
    {
      return OnSubCheatCard(pData,wDataSize);
    }
  }

  //�Ƿ���Ϣ
  ASSERT(FALSE);
  return true;
}


//��������
bool CGameClientEngine::OnSubCheatCard(VOID * pData, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_CheatCard));
  if(wDataSize!=sizeof(CMD_S_CheatCard))
  {
    return false;
  }

  //��������
  CMD_S_CheatCard * pCheatCard=(CMD_S_CheatCard *)pData;

  ASSERT(pCheatCard->cbUserCount <= GAME_PLAYER);
  if(pCheatCard->cbUserCount > GAME_PLAYER)
  {
    return false;
  }

  //m_bCheatRight = true;
  for(WORD i = 0; i < pCheatCard->cbUserCount ; i++)
  {
    //��ʾ�˿�
    //m_GameClientView.UserValidCard(SwitchViewChairID(pCheatCard->wCardUser[i]), pCheatCard->cbCardData[i], pCheatCard->cbCardCount[i]);

    if(i==GetMeChairID())
    {
      continue;
    }

    //m_GameClientView.m_HandCardControl[SwitchViewChairID(pCheatCard->wCardUser[i])].SetCardData(pCheatCard->cbCardData[i], pCheatCard->cbCardCount[i]);
    //m_GameClientView.m_HandCardControl[SwitchViewChairID(pCheatCard->wCardUser[i])].SetDisplayFlag(true);
    m_bUserCheat[pCheatCard->wCardUser[i]] = true;
    m_GameClientView.m_bUserCheat[SwitchViewChairID(pCheatCard->wCardUser[i])]=true;
    CopyMemory(m_cbUserCardData[pCheatCard->wCardUser[i]], pCheatCard->cbCardData[i], m_cbHandCardCount[pCheatCard->wCardUser[i]]);

  }
  //���½���
  CRect rcClient;
  GetClientRect(&rcClient);
  m_GameClientView.InvalidGameView(0,0,rcClient.Width(),rcClient.Height());
  return true;
}


//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
  switch(cbGameStatus)
  {
    case GS_SK_FREE:  //����״̬
    {
      //Ч������
      ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
      if(wDataSize!=sizeof(CMD_S_StatusFree))
      {
        return false;
      }

      CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

      if(IsLookonMode()==false)
      {
        if(GetMeUserItem()->GetUserStatus()!=US_READY)
        {
          //���ÿؼ�
          m_GameClientView.m_btStart.ShowWindow(TRUE);
          m_GameClientView.m_btStart.SetFocus();

          //����ʱ��
          SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
        }
        //�����˿�
        m_GameClientView.m_HandCardControl[GetMeChairID()].SetDisplayFlag(true);
        m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
      }

      m_GameClientView.m_lLastTurnScore[GetMeChairID()]=pStatusFree->lLastTurnScore;
      m_GameClientView.m_lAllTurnScore[GetMeChairID()]=pStatusFree->lAllTurnScore;

      //���ý���
      m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
      m_GameClientView.RefreshGameView();

      if(m_pIStringMessage != NULL)
      {
        m_pIStringMessage->InsertSystemString(TEXT("1��6��Ϊƽ�ۣ�7��11��Ϊ���ۣ�12������Ϊ˫�ۣ�"));
      }

      //����
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
      {
        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
      }

      return true;
    }
    case GS_SK_PLAYING:   //��Ϸ״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusPlay))
      {
        return false;
      }
      CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
      //��������
      WORD wMeChairID=GetMeChairID();
      //��ʼ����
      m_bLastTurn=false;
      ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
      memset(m_cbOutCardCount,255,sizeof(m_cbOutCardCount));
      //���ñ���
      m_wCurrentUser=pStatusPlay->wCurrentUser;
      //���Ʊ���
      m_cbTurnCardCount=pStatusPlay->cbTurnCardCount;
      CopyMemory(m_cbTurnCardData,pStatusPlay->cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

      //�˿�����
      CopyMemory(m_cbHandCardCount,pStatusPlay->cbHandCardCount,sizeof(m_cbHandCardCount));
      CopyMemory(&m_LastOutCard,&(pStatusPlay->LastOutCard),sizeof(tagAnalyseResult));
      CopyMemory(m_cbHandCardData,pStatusPlay->cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);

      //���ƽ���
      m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
      m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0xFFFF);

      //�����˿�
      BYTE cbCardData[MAX_COUNT];
      ZeroMemory(cbCardData,sizeof(cbCardData));

      m_wTimeOutCount=0;
      m_bStustee=pStatusPlay->bTrustee[GetMeChairID()];

      //�����˿�
      for(BYTE cbIndex=0; cbIndex<GAME_PLAYER; cbIndex++)
      {
        //��������
        WORD wViewChairID=SwitchViewChairID(cbIndex);
        //��������
        m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->cbHandCardCount[cbIndex]);
        //��������
        m_GameClientView.m_lAllTurnScore[wViewChairID]=pStatusPlay->lAllTurnScore[cbIndex];
        m_GameClientView.m_lLastTurnScore[wViewChairID]=pStatusPlay->lLastTurnScore[cbIndex];

        if(wViewChairID!=ME_VIEW_CHAIR || !IsAllowLookon())
        {
          m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,m_cbHandCardCount[cbIndex]);
        }
        else
        {
          m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[cbIndex]);
        }
        //�й�����
        m_GameClientView.SetTrustee(SwitchViewChairID(cbIndex),pStatusPlay->bTrustee[cbIndex]);
        //�����б�
        m_cbOutCardCount[cbIndex][0]=pStatusPlay->cbOutCardCount[cbIndex];
        CopyMemory(m_cbOutCardData[cbIndex][0],pStatusPlay->cbOutCardData[cbIndex],sizeof(BYTE)*m_cbOutCardCount[cbIndex][0]);
      }
      //�������
      if(!IsLookonMode())
      {
        if(pStatusPlay->wCurrentUser==GetMeChairID())
        {
          m_GameClientView.m_btOutCard.EnableWindow(FALSE);
          m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
          m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
          m_GameClientView.m_btPassCard.EnableWindow((m_cbTurnCardCount!=0)?TRUE:FALSE);
        }
        //�йܰ�ť
        m_GameClientView.m_btStusteeControl.SetButtonImage((m_bStustee==true)?IDB_BT_STOP_TRUSTEE:IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
        m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
        m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(true);
        m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag(true);
      }


      //��ť��ʾ
      if(IsLookonMode()==false)
      {
        //���Ͱ�ť
        m_GameClientView.m_btOne.EnableWindow(TRUE);
        m_GameClientView.m_btTwo.EnableWindow(TRUE);
        m_GameClientView.m_btThree.EnableWindow(TRUE);
        m_GameClientView.m_btFour.EnableWindow(TRUE);

        //����ť
        m_bSortCount=false;
        m_GameClientView.m_btSortByCount.SetButtonImage((m_bSortCount==true)?IDB_BT_SORT_BY_ORDER:IDB_BT_SORT_BY_COUNT,AfxGetInstanceHandle(),false,false);
        m_GameClientView.m_btSortByCount.EnableWindow(TRUE);
      }

      //��������
      if(m_cbTurnCardCount!=0)
      {
        WORD wViewChairID=SwitchViewChairID(pStatusPlay->wTurnWiner);
        m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbTurnCardData,m_cbTurnCardCount);
      }
      //���ö�ʱ��
      SetGameClock(pStatusPlay->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

      //��ʾ��ʾ��Ϣ
      CString strTemp;
      strTemp.Format(_T("��ǰ��Ԫ���֣�ƽ�ۣ�%I64d--���ۣ�%I64d--˫�ۣ�%I64d"),pStatusPlay->lCellScore[0],pStatusPlay->lCellScore[1],pStatusPlay->lCellScore[2]);
      if(m_pIStringMessage != NULL)
      {
        m_pIStringMessage->InsertSystemString(strTemp);
      }


      //����
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
      {
        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
      }


      if(m_cbHandCardCount[GetMeChairID()]>0)
      {
        BYTE cbHandCardData[MAX_COUNT];
        ZeroMemory(cbHandCardData,sizeof(cbHandCardData));
        CopyMemory(cbHandCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

        m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[GetMeChairID()]);
        m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(cbHandCardData,m_cbHandCardCount[GetMeChairID()]);


      }


      m_GameClientView.RefreshGameView();

      return true;
    }
  }
  return true;
}


//�û�״̬
VOID CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
}

bool  CGameClientEngine::OnTimerDispatchCard()
{
  //�ɷ�����
  if(m_cbRemnantCardCount>0)
  {
    //�˿�����
    BYTE cbUserCard[MAX_COUNT];
    ZeroMemory(cbUserCard,sizeof(cbUserCard));

    m_cbRemnantCardCount--;
    m_cbDispatchCardCount++;

    if(m_cbGetCardPosition==m_cbDispatchCardCount)
    {

      PlayGameSound(AfxGetInstanceHandle(),TEXT("LIANG_PAI"));


      IClientUserItem * pClientUserItem=GetTableUserItem(m_wGetRandCardID);
      TCHAR szBuffer[256];

      BYTE cbRandCard[]= {m_cbRandCardData};
      CString strRandCard=GetCardInfo(m_cbRandCardData);
      _sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ץ������ %s��"),pClientUserItem->GetNickName(),strRandCard);

      if(m_pIStringMessage != NULL)
      {
        m_pIStringMessage->InsertSystemString(szBuffer);

      }

      m_GameClientView.m_RandCardControl.SetCardData(NULL,0);



    }
    //���ý���
    WORD wViewChairID=0;
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      wViewChairID=SwitchViewChairID(i);
      if(wViewChairID==ME_VIEW_CHAIR)
      {
        m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount);
      }
      else
      {
        m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbUserCard,m_cbDispatchCardCount);
      }
    }
  }

  //��ֹ�ж�
  if(m_cbRemnantCardCount==0)
  {

    m_GameClientView.m_RandCardControl.SetCardData(NULL,0);

    //ɾ����ʱ��
    KillTimer(IDI_DISPATCH_CARD);
    //�˿���Ŀ
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      m_GameClientView.SetCardCount(SwitchViewChairID(i),m_cbHandCardCount[i]);
    }
    //��ǰ���
    if(!IsLookonMode())
    {
      if(m_wCurrentUser==GetMeChairID())
      {
        ActiveGameFrame();
        m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
        m_GameClientView.m_btOutCard.EnableWindow(FALSE);
        m_GameClientView.m_btPassCard.EnableWindow(FALSE);
        m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);

      }

      m_GameClientView.m_btHunter.ShowWindow(m_bReqHumter?SW_SHOW:SW_HIDE);
      m_GameClientView.m_btFour.EnableWindow(TRUE);
      m_GameClientView.m_btThree.EnableWindow(TRUE);
      m_GameClientView.m_btTwo.EnableWindow(TRUE);
      m_GameClientView.m_btOne.EnableWindow(TRUE);
      m_GameClientView.m_btSortByCount.EnableWindow(TRUE);
      //�����˿�
      m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()]);

      SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

      //������ʾ
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        if(m_bUserCheat[i])
        {
          //�����˿�
          m_GameLogic.SortCardList(m_cbUserCardData[i],m_cbHandCardCount[i],ST_ORDER);
          //�����˿�
          m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(m_cbUserCardData[i],m_cbHandCardCount[i]);
          m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetDisplayFlag(true);
        }
      }


      m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_cbHandCardData,MAX_COUNT);
      m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(IsLookonMode()==false);
    }
  }
  //���½���
  m_GameClientView.RefreshGameView();
  return true;
}


bool CGameClientEngine::OnCanReqHunter(BYTE cbCardData[],BYTE cbCardCount)
{
  if(m_GameLogic.HasNotBombCard(cbCardData,cbCardCount))
  {
    return true;
  }

  return false;
}
//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_GameStart));
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
    return false;
  }

  m_bHaveOutCard = false;
  m_bReqHumter=false;
  //��Ϣ����
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  ZeroMemory(&m_SearchInfo,sizeof(m_SearchInfo));
  ZeroMemory(m_bUserCheat,sizeof(m_bUserCheat));
  ZeroMemory(m_cbUserCardData,sizeof(m_cbUserCardData));


  //��ʾ��ʾ��Ϣ
  CString strTemp;
  strTemp.Format(_T("��ǰ��Ԫ���֣�ƽ�ۣ�%I64d--���ۣ�%I64d--˫�ۣ�%I64d"),pGameStart->lCellScore[0],pGameStart->lCellScore[1],pGameStart->lCellScore[2]);
  if(m_pIStringMessage != NULL)
  {
    m_pIStringMessage->InsertSystemString(strTemp);
  }

  //��Ϣ����
  if(m_pIStringMessage != NULL)
  {
    CString strTemp=_T("������Ϸ��ʼ��");
    m_pIStringMessage->InsertSystemString((LPCTSTR)strTemp);
    Sleep(300);
    CString strRandCard=GetCardInfo(pGameStart->cbStartRandCard);

    TCHAR szBuffer[256];

    _sntprintf(szBuffer,sizeof(szBuffer),TEXT("������������ %s��"),strRandCard);

    m_pIStringMessage->InsertSystemString((LPCTSTR)szBuffer);

    m_GameClientView.m_RandCardControl.SetCardData(&pGameStart->cbStartRandCard,1);

  }

  m_wBankerUser=pGameStart->wCurrentUser;
  //��Ϸ����
  m_wCurrentUser=pGameStart->wCurrentUser;
  //��Ϸ����
  m_wTimeOutCount=0;
  m_cbTurnCardCount=0;
  //��ʼ��Ϣ
  m_cbRandCardData=pGameStart->cbStartRandCard;
  m_wGetRandCardID=pGameStart->wGetRandCardID;
  m_cbGetCardPosition=pGameStart->cbGetCardPosition;
  //ʤ����Ϣ
  m_wWinOrder=0xFFFF;
  //���Ʊ���
  m_cbTurnCardCount=0;
  ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
  ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

  //�����˿�
  m_bLastTurn=false;
  ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
  memset(m_cbOutCardCount,255,sizeof(m_cbOutCardCount));

  //����˿�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��ȡλ��
    WORD wViewChairID=SwitchViewChairID(i);

    //�����˿�
    if(wViewChairID==ME_VIEW_CHAIR)
    {
      bool bShowCard=((IsLookonMode()==false)||(IsAllowLookon()==true));
      m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
      m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
      m_GameClientView.m_HandCardControl[wViewChairID].SetPositively(false);
      m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(bShowCard);
    }
    else
    {
      m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
      m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
      m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(false);
    }
  }


  //�����˿�
  WORD wMeChairID=GetMeChairID();
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_cbHandCardCount[i]=CountArray(pGameStart->cbCardData);
    m_GameClientView.SetCardCount(i,m_cbHandCardCount[i]);
  }
  CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);
  //�Ƿ���Գ���
  m_bReqHumter=OnCanReqHunter(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);

  CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);

  //�ɷ��˿�
  DispatchUserCard(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);

  //�й�����
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_GameClientView.SetTrustee(SwitchViewChairID(i),pGameStart->bTrustee[i]);
  }
  //���½���
  m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0xFFFF);
  //���½���
  m_GameClientView.RefreshGameView();
  //��������
  if((IsLookonMode()==false))
  {
    ActiveGameFrame();
  }
  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
  m_GameClientView.SetShowScoreView(false);

  return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_OutCard));
  if(wDataSize!=sizeof(CMD_S_OutCard))
  {
    return false;
  }
  //��������
  CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
  //ֹͣ����
  DoncludeDispatchCard();
  //ɾ����ʱ��
  KillTimer(IDI_WAIT_CLEAR);
  KillGameClock(IDI_OUT_CARD);
  //��������
  WORD wMeChairID=GetMeChairID();
  WORD wViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
  bool bPlayGameSound=((IsLookonMode()==false)&&(wMeChairID==pOutCard->wOutCardUser));
  //��ʷ����
  if(m_cbTurnCardCount==0)
  {
    //�û��˿�
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //��������
      if(i!=pOutCard->wOutCardUser)
      {
        if(m_cbOutCardCount[i][0]!=255)
        {
          //�����˿�
          m_cbOutCardCount[i][1]=m_cbOutCardCount[i][0];
          CopyMemory(m_cbOutCardData[i][1],m_cbOutCardData[i][0],sizeof(BYTE)*m_cbOutCardCount[i][0]);
        }
        //�����˿�
        m_cbOutCardCount[i][0]=255;
        ZeroMemory(m_cbOutCardData[i][0],sizeof(BYTE)*MAX_COUNT);

        WORD wViewChairID=SwitchViewChairID(i);
        m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
      }
    }
    //������־
    m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
  }

  //��ǰ����
  SwitchToCurrentCard();
  //���Ʊ���
  m_wCurrentUser=pOutCard->wCurrentUser;
  m_cbTurnCardCount=pOutCard->cbCardCount;
  CopyMemory(m_cbTurnCardData,pOutCard->cbCardData,sizeof(BYTE)*pOutCard->cbCardCount);

  //��ȡ����
  tagAnalyseResult analyseResult;
  memset(&analyseResult,0,sizeof(analyseResult));
  m_GameLogic.AnalysebCardData(pOutCard->cbCardData,pOutCard->cbCardCount,analyseResult);
  m_GameLogic.FindValidCard(analyseResult,m_LastOutCard);
  memcpy(&m_LastOutCard,&analyseResult,sizeof(tagAnalyseResult));

  //ը���ж�
  if(m_LastOutCard.m_cbCardType>=CT_BOMB)
  {
    //���Ŷ���
    m_GameClientView.SetBombEffect(true);
  }

  if(!PlayCardSound(pOutCard->wOutCardUser,m_LastOutCard))
  {
    if(m_LastOutCard.m_cbCardType>=CT_BOMB)
    {
      //��������
      PlayBombSound(pOutCard->wOutCardUser,4);
    }
    else
    {
      PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
    }

  }

  //���׷���
  for(int i=0; i<GAME_PLAYER; i++)
  {
    m_GameClientView.SetBombScore(SwitchViewChairID(i),pOutCard->lBombScore[i]);
  }

  //�����˿�
  if(m_cbOutCardCount[pOutCard->wOutCardUser][0]!=255)
  {
    //���Ƽ�¼
    m_cbOutCardCount[pOutCard->wOutCardUser][1]=m_cbOutCardCount[pOutCard->wOutCardUser][0];
    if(m_cbOutCardCount[pOutCard->wOutCardUser][0]!=0)
    {
      CopyMemory(m_cbOutCardData[pOutCard->wOutCardUser][1],m_cbOutCardData[pOutCard->wOutCardUser][0],sizeof(BYTE)*m_cbOutCardCount[pOutCard->wOutCardUser][0]);
    }
    else
    {
      ZeroMemory(m_cbOutCardData[pOutCard->wOutCardUser][1],MAX_COUNT*sizeof(BYTE));
    }

  }
  m_cbOutCardCount[pOutCard->wOutCardUser][0]=pOutCard->cbCardCount;
  CopyMemory(m_cbOutCardData[pOutCard->wOutCardUser][0],pOutCard->cbCardData,pOutCard->cbCardCount*sizeof(BYTE));

  //��������
  if((IsLookonMode()==true)||(wMeChairID!=pOutCard->wOutCardUser))
  {
    m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(pOutCard->cbCardData,pOutCard->cbCardCount);
    //ɾ���˿�
    if(wViewChairID==ME_VIEW_CHAIR)
    {
      //ɾ���˿�
      BYTE cbSourceCount=m_cbHandCardCount[wMeChairID];
      m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
      m_GameLogic.RemoveCardEx(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData,cbSourceCount);

      //�����˿�
      m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);


    }
    else if(m_bUserCheat[pOutCard->wOutCardUser] && wViewChairID!=ME_VIEW_CHAIR)
    {
      m_GameLogic.SortCardList(m_cbUserCardData[pOutCard->wOutCardUser],m_cbHandCardCount[pOutCard->wOutCardUser],ST_ORDER);
      //ɾ���˿�
      m_GameLogic.RemoveCardEx(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbUserCardData[pOutCard->wOutCardUser],m_cbHandCardCount[pOutCard->wOutCardUser]);
      m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
      //�����˿�
      m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(pOutCard->cbCheatCardData,m_cbHandCardCount[pOutCard->wOutCardUser]);
    }
    else
    {
      m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
      //�����˿�
      BYTE cbCardCount[MAX_COUNT];
      ZeroMemory(cbCardCount,sizeof(cbCardCount));
      m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardCount,m_cbHandCardCount[pOutCard->wOutCardUser]);
    }
  }

  m_GameClientView.SetCardCount(wViewChairID,m_cbHandCardCount[pOutCard->wOutCardUser]);

  //����ж�
  if(m_wCurrentUser==pOutCard->wOutCardUser)
  {
    //���ñ���
    m_cbTurnCardCount=0;
    m_wCurrentUser=INVALID_CHAIR;
    m_wMostUser=pOutCard->wCurrentUser;
    ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
    ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
    //��������
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //�û�����
      if(pOutCard->wOutCardUser==i)
      {
        continue;
      }
      //��������
      WORD wViewChairID=SwitchViewChairID(i);
      m_GameClientView.SetPassFlag(wViewChairID,true);
      m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
    }

    //��������
    if(bPlayGameSound==false)
    {
      bPlayGameSound=true;
      PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));
    }
    //���ö�ʱ��
    SetTimer(IDI_WAIT_CLEAR,3000,NULL);
    return true;
  }

  //�������
  if(m_wCurrentUser!=INVALID_CHAIR)
  {
    WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
    m_GameClientView.SetPassFlag(wViewChairID,false);
    m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

    //�����˿�
    if(m_cbOutCardCount[m_wCurrentUser][0]!=255)
    {
      //���Ƽ�¼
      m_cbOutCardCount[m_wCurrentUser][1]=m_cbOutCardCount[m_wCurrentUser][0];
      if(m_cbOutCardCount[m_wCurrentUser][0]!=0)
      {
        CopyMemory(m_cbOutCardData[m_wCurrentUser][1],m_cbOutCardData[m_wCurrentUser][0],sizeof(BYTE)*m_cbOutCardCount[m_wCurrentUser][0]);
      }
      else
      {
        ZeroMemory(m_cbOutCardData[m_wCurrentUser][1],MAX_COUNT*sizeof(BYTE));
      }
    }
    m_cbOutCardCount[m_wCurrentUser][0]=255;
  }

  //�������
  if((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
  {
    //������
    if(m_bStustee==false)
    {
      ActiveGameFrame();
    }
    BYTE cbCardCount=m_cbHandCardCount[pOutCard->wCurrentUser];
    if(cbCardCount==1&&pOutCard->cbCardCount>=2)
    {
      m_GameClientView.m_btPassCard.EnableWindow(TRUE);
      m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
      m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
      m_GameClientView.m_btOutCard.EnableWindow(FALSE);
      SetGameClock(m_wCurrentUser,IDI_PASS_CARD,TIME_PASS_CARD);
    }
    else
    {
      m_GameClientView.m_btPassCard.EnableWindow(TRUE);
      m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
      m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
      m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
      m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);

      if(!m_bStustee)
      {
        SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
      }
      else
      {
        SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_AUTO_OUT);
      }
    }

    if(!m_bHaveOutCard)
    {
      //�Ƿ���Գ���
      m_bReqHumter=OnCanReqHunter(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);
      m_GameClientView.m_btHunter.ShowWindow(m_bReqHumter?SW_SHOW:SW_HIDE);
    }
  }
  else
  {
    if(pOutCard->wCurrentUser!=INVALID_CHAIR)
    {
      if(!m_bStustee)
      {
        SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
      }
      else
      {
        SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_AUTO_OUT);
      }
    }
  }

  //���ư�ť
  m_GameClientView.m_btLastTurnCard.EnableWindow(TRUE);

  m_GameClientView.RefreshGameView();

  return true;
}

//��������
bool CGameClientEngine::OnSubPassCard(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_PassCard));
  if(wDataSize!=sizeof(CMD_S_PassCard))
  {
    return false;
  }
  //��������
  CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;
  //ɾ����ʱ��
  KillGameClock(IDI_OUT_CARD);

  //���ñ���
  m_wCurrentUser=pPassCard->wCurrentUser;

  //��������
  if((IsLookonMode()==true)||(pPassCard->wPassCardUser!=GetMeChairID()))
  {
    WORD wPassCardUser=pPassCard->wPassCardUser;
    m_GameClientView.SetPassFlag(SwitchViewChairID(wPassCardUser),true);
  }

  //�����˿�
  if(m_cbOutCardCount[pPassCard->wPassCardUser][0]!=255)
  {
    //���Ƽ�¼
    m_cbOutCardCount[pPassCard->wPassCardUser][1]=m_cbOutCardCount[pPassCard->wPassCardUser][0];
    if(m_cbOutCardCount[pPassCard->wPassCardUser][0]!=0)
    {
      CopyMemory(m_cbOutCardData[pPassCard->wPassCardUser][1],m_cbOutCardData[pPassCard->wPassCardUser][0],sizeof(BYTE)*m_cbOutCardCount[pPassCard->wPassCardUser][0]);
    }
    else
    {
      ZeroMemory(m_cbOutCardData[pPassCard->wPassCardUser][1],MAX_COUNT*sizeof(BYTE));
    }
  }
  m_cbOutCardCount[pPassCard->wPassCardUser][0]=0;
  ZeroMemory(m_cbOutCardData[pPassCard->wPassCardUser][0],MAX_COUNT*sizeof(BYTE));
  if(m_wCurrentUser != INVALID_CHAIR)
  {
    //��ͼλ��
    WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
    //���ý���
    m_GameClientView.SetPassFlag(wViewChairID,false);
    m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
    //�����˿�
    if(m_cbOutCardCount[m_wCurrentUser][0]!=255)
    {
      //���Ƽ�¼
      m_cbOutCardCount[m_wCurrentUser][1]=m_cbOutCardCount[m_wCurrentUser][0];
      if(m_cbOutCardCount[m_wCurrentUser][0]!=0)
      {
        CopyMemory(m_cbOutCardData[m_wCurrentUser][1],m_cbOutCardData[m_wCurrentUser][0],sizeof(BYTE)*m_cbOutCardCount[m_wCurrentUser][0]);
      }
      else
      {
        ZeroMemory(m_cbOutCardData[m_wCurrentUser][1],MAX_COUNT*sizeof(BYTE));
      }
    }
    m_cbOutCardCount[m_wCurrentUser][0]=255;
  }

  //һ���ж�
  if(pPassCard->cbTurnOver==TRUE)
  {
    //���Ʊ���
    m_cbTurnCardCount=0;
    ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
    ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
    ////�������
    //m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
  }

  //�������
  if((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
  {
    //������
    ActiveGameFrame();
    //���ð�ť
    m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
    m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
    m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
    m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
    m_GameClientView.m_btHunter.ShowWindow(m_bReqHumter?SW_SHOW:SW_HIDE);
  }
  //��������
  if((IsLookonMode()==true)||(pPassCard->wPassCardUser!=GetMeChairID()))
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
  }
  if(!m_bStustee)
  {
    SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
  }
  else
  {
    SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_AUTO_OUT);
  }

  return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }
  //��Ϣ����
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
  //����״̬
  SetGameStatus(GS_SK_FREE);
  //ֹͣ����
  DoncludeDispatchCard();
  //ɾ����ʱ��
  KillTimer(IDI_WAIT_CLEAR);
  KillGameClock(IDI_OUT_CARD);
  KillTimer(IDI_DISPATCH_CARD);

  //���ؿؼ�
  m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btHunter.ShowWindow(SW_HIDE);

  //���ÿؼ�
  m_GameClientView.m_btOutCard.EnableWindow(FALSE);
  m_GameClientView.m_btPassCard.EnableWindow(FALSE);
  m_GameClientView.m_btLastTurnCard.EnableWindow(FALSE);

  m_GameClientView.m_btFour.EnableWindow(FALSE);
  m_GameClientView.m_btThree.EnableWindow(FALSE);
  m_GameClientView.m_btTwo.EnableWindow(FALSE);
  m_GameClientView.m_btOne.EnableWindow(FALSE);
  m_GameClientView.m_btSortByCount.EnableWindow(FALSE);

  if(pGameEnd->bHunter)
  {
    //��Ϣ����
    if(m_pIStringMessage != NULL)
    {
      m_pIStringMessage->InsertSystemString(TEXT("�û����ף���Ϸ������"));
    }
  }
  //��ǰ����
  SwitchToCurrentCard();

  m_GameClientView.m_ScoreView.SetBaseScore(pGameEnd->nMutile,pGameEnd->nBaseScore);
  //���û���
  CString strTemp=_T("");
  //���û���
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    IClientUserItem * pClientUserItem=GetTableUserItem(i);
    m_GameClientView.m_ScoreView.SetGameScore(i,pClientUserItem->GetNickName(),pGameEnd->lGameScore[i],pGameEnd->lBombScore[i]);

    if(i==1)
    {
      strTemp.AppendFormat(_T("\n"));
    }
    if(pGameEnd->lGameScore[i]>0)
    {
      strTemp.AppendFormat(_T("\n%s�� %+I64d"),pClientUserItem->GetNickName(),pGameEnd->lGameScore[i]);
    }
    else
    {
      strTemp.AppendFormat(_T("\n%s�� %I64d"),pClientUserItem->GetNickName(),pGameEnd->lGameScore[i]);
    }

    if(pGameEnd->lBombScore[i]>0)
    {
      strTemp.AppendFormat(_T("\n  ��ù��׵÷�Ϊ��%+I64d"),pGameEnd->lBombScore[i]);
    }
    else
    {
      strTemp.AppendFormat(_T("\n  ��ù��׵÷�Ϊ��%I64d"),pGameEnd->lBombScore[i]);
    }

    strTemp+=_T("  \n����ը��������£�");

    bool bFind=false;
    for(BYTE cbIndex=0; cbIndex<BOMB_TYPE_COUNT; cbIndex++)
    {
      if(pGameEnd->cbBombList[i][cbIndex]>0)
      {
        strTemp.AppendFormat(_T("\n  [%d��]X%d"),cbIndex+4,pGameEnd->cbBombList[i][cbIndex]);
        bFind=true;
      }
    }
    if(!bFind)
    {
      strTemp.AppendFormat(_T("\n  ��"));
    }
  }
  //��Ϣ����
  if(m_pIStringMessage != NULL)
  {
    m_pIStringMessage->InsertSystemString((LPCTSTR)strTemp);
    strTemp=_T("������Ϸ������");
    m_pIStringMessage->InsertSystemString((LPCTSTR)strTemp);
  }

  //m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

  m_GameClientView.SetShowScoreView(true);

  //�����˿�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    WORD wViewChairID=SwitchViewChairID(i);
    m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
    m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
  }

  //��ʾ�˿�
  if(IsLookonMode()==true)
  {
    m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag(true);
  }

  //��������
  WORD wMeChairID=GetMeChairID();
  LONGLONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
  //if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
  //else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
  //else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

  if(pGameEnd->cbResultType==1)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("PING_KOU"));
  }
  else if(pGameEnd->cbResultType==2)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("DAN_KOU"));
  }
  else if(pGameEnd->cbResultType==3)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("SHUNAG_KOU"));
  }
  //���ý���
  if(IsLookonMode()==false)
  {
    m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
    SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
  }
  m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(false);

  //ȡ���й�
  if(m_bStustee)
  {
    OnStusteeControl(0,0);
  }

  //ȡ������
  m_bSortCount=false;

  //���ý���
  for(BYTE i=0; i<GAME_PLAYER; i++)
  {
    WORD wViewChairID=SwitchViewChairID(i);
    m_GameClientView.m_lAllTurnScore[wViewChairID]=pGameEnd->lAllTurnScore[i];
    m_GameClientView.m_lLastTurnScore[wViewChairID]=pGameEnd->lLastTurnScore[i];
  }
  //��ʼ��Ϣ
  m_wGetRandCardID=INVALID_CHAIR;
  m_cbGetCardPosition=0xFF;
  m_cbRandCardData=0xFF;

  m_GameClientView.RefreshGameView();
  return true;
}

//�û��й�
bool CGameClientEngine::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_Trustee));
  if(wDataSize!=sizeof(CMD_S_Trustee))
  {
    return false;
  }
  //��Ϣ����
  CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
  m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
  if((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
  {
    IClientUserItem * pClientUserItem=GetTableUserItem(pTrustee->wChairID);
    TCHAR szBuffer[256];
    if(pTrustee->bTrustee==true)
    {
      _sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ѡ�����йܹ��ܡ�"),pClientUserItem->GetNickName());
    }
    else
    {
      _sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ȡ�����йܹ��ܡ�"),pClientUserItem->GetNickName());
    }
    if(m_pIStringMessage != NULL)
    {
      m_pIStringMessage->InsertSystemString(szBuffer);
    }
  }

  if(pTrustee->wChairID==GetMeChairID()&&pTrustee->bTrustee)
  {
    CSkinButton & btOutCard=m_GameClientView.m_btOutCard;
    if(btOutCard.IsWindowVisible())
    {
      AutomatismOutCard();

    }
  }
  return true;
}

//�û�׼��
bool CGameClientEngine::OnSubUserReady(const void * pBuffer,WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_OnReady));
  if(wDataSize!=sizeof(CMD_S_OnReady))
  {
    return false;
  }
  //��Ϣ����
  CMD_S_OnReady * pReady=(CMD_S_OnReady *)pBuffer;
  IClientUserItem * pClientUserItem=GetTableUserItem(pReady->wChairId);
  if(pClientUserItem!=NULL)
  {
    TCHAR szBuffer[256];
    _sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]�ѿ�ʼ��"),pClientUserItem->GetNickName());

    if(m_pIStringMessage != NULL)
    {
      m_pIStringMessage->InsertSystemString(szBuffer);
    }
  }
  return true;
}

//����
bool CGameClientEngine::OnSubAdminControl(const void * pBuffer, WORD wDataSize)
{
  ASSERT(wDataSize==sizeof(CMD_S_ControlReturns));
  if(wDataSize!=sizeof(CMD_S_ControlReturns))
  {
    return false;
  }

  if(m_GameClientView.m_pAdminControl!=NULL)
  {
    CMD_S_ControlReturns* pResult = (CMD_S_ControlReturns*)pBuffer;
    m_GameClientView.m_pAdminControl->UpdateControl(pResult);
  }
  return true;
}

//�����ж�
bool CGameClientEngine::VerdictOutCard()
{
  //״̬�ж�
  if(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE)
  {
    return false;
  }
  if(m_wCurrentUser!=GetMeChairID())
  {
    return false;
  }

  //��ȡ�˿�
  BYTE cbCardData[MAX_COUNT];
  BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(cbCardData,CountArray(cbCardData));
  //�����ж�
  if(cbShootCount>0L)
  {

#ifdef _DEBUG
    if(cbShootCount==7&&m_OutCardInfo.m_cbCardCount==7)
    {
      memset(&m_OutCardInfo,0,sizeof(m_OutCardInfo));
      m_GameLogic.AnalysebCardData(cbCardData,cbShootCount,m_OutCardInfo);

      return m_GameLogic.CompareCard(m_OutCardInfo,m_LastOutCard);
    }
#endif // _DEBUG

    memset(&m_OutCardInfo,0,sizeof(m_OutCardInfo));
    m_GameLogic.AnalysebCardData(cbCardData,cbShootCount,m_OutCardInfo);

    return m_GameLogic.CompareCard(m_OutCardInfo,m_LastOutCard);
  }
  return false;
}

//�����ж�
bool CGameClientEngine::VerdictPassCard()
{
  //�׳��ж�
  if(m_cbTurnCardCount==0)
  {
    return false;
  }
  return true;
}

//�Զ�����
bool CGameClientEngine::AutomatismOutCard()
{
  if((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))
  {
    return false;
  }

  //��ǰ����
  CSkinButton & btOutCard=m_GameClientView.m_btOutCard;
  if((btOutCard.IsWindowEnabled())&&(btOutCard.IsWindowVisible()))
  {
    OnOutCard(0,0);
    return true;
  }
  //���ƴ���
  if((m_cbTurnCardCount==0))
  {
    //�������
    tagOutCardResult OutCardResult;
    m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],m_cbTurnCardData,m_cbTurnCardCount,m_cbHandCardCount[(GetMeChairID()+1)%GAME_PLAYER],OutCardResult);

    //���ý���
    if(OutCardResult.cbCardCount>0)
    {
      //���ý���
      m_GameClientView.m_btOutCard.EnableWindow(TRUE);
      m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
      //���ƶ���
      OnOutCard(0,0);
      return true;
    }
  }
  //��������
  if(m_cbTurnCardCount>0)
  {
    OnPassCard(0,0);
  }
  return true;
}

//ֹͣ����
bool CGameClientEngine::DoncludeDispatchCard()
{
  //ֹͣ����
  if(m_cbRemnantCardCount>0)
  {
    //ɾ��ʱ��
    KillTimer(IDI_DISPATCH_CARD);

    //�˿�����
    BYTE cbUserCard[MAX_COUNT];
    ZeroMemory(cbUserCard,sizeof(cbUserCard));

    //���ý���
    WORD wViewChairID=0;
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      wViewChairID=SwitchViewChairID(i);
      if(wViewChairID ==ME_VIEW_CHAIR)
      {
        m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[i]);
      }
      else
      {
        m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbUserCard,MAX_COUNT);
      }
    }
    //���ñ���
    m_cbRemnantCardCount=0;
    m_cbDispatchCardCount=0;
    ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));
  }
  return true;
}

//�ɷ��˿�
bool CGameClientEngine::DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount)
{
  //���ñ���
  m_cbDispatchCardCount=0;
  m_cbRemnantCardCount=cbCardCount;
  CopyMemory(m_cbDispatchCardData,cbCardData,sizeof(BYTE)*cbCardCount);
  //���ö�ʱ��
  SetTimer(IDI_DISPATCH_CARD,100,NULL);
  return true;
}

//���ų�������
void CGameClientEngine::PlayBombSound(WORD wChairID, BYTE cbStarLevel)
{
  //�ж��Ա�
  IClientUserItem * pClientUserItem=GetTableUserItem(wChairID);
  if(pClientUserItem==0||cbStarLevel<4||cbStarLevel>12)
  {
    return;
  }
  CString strSoundName;
  strSoundName.Format(_T("%d"),cbStarLevel);
  if(pClientUserItem->GetGender()!=1)
  {
    strSoundName = _T("BOY_") +strSoundName;
  }
  else
  {
    strSoundName = _T("GIRL_") + strSoundName;
  }
  PlayGameSound(AfxGetInstanceHandle(), strSoundName);

}

//��ǰ�˿�
bool CGameClientEngine::SwitchToCurrentCard()
{
  //�����ж�
  if(m_bLastTurn==false)
  {
    return false;
  }

  //��������
  m_bLastTurn=false;
  KillTimer(IDI_LAST_TURN_CARD);
  m_GameClientView.SetLastTurnCard(false);

  //�����˿�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    WORD wViewChairID=SwitchViewChairID(i);
    if(m_cbOutCardCount[i][0]!=255)
    {
      if(m_cbOutCardCount[i][0]==0)
      {
        m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
        m_GameClientView.SetPassFlag(wViewChairID,true);
      }
      else
      {
        m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
      }
    }
    else
    {
      m_GameClientView.SetPassFlag(wViewChairID,false);
      m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
    }
  }

  return true;
}

//��ʱ����Ϣ
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
  switch(nIDEvent)
  {
    case IDI_WAIT_CLEAR:
    {
      //��������
      WORD wCurrentUser=m_wMostUser;
      m_wMostUser=INVALID_CHAIR;
      //ɾ����ʱ��
      KillTimer(IDI_WAIT_CLEAR);
      //���ý���
      m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
      }

      //�������
      if((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
      {
        ActiveGameFrame();
        m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
        m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
        m_GameClientView.m_btPassCard.EnableWindow(FALSE);
        m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
        m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
      }
      //����ʱ��
      SetGameClock(wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
      break;
    }
    case IDI_DISPATCH_CARD:
      OnTimerDispatchCard();
      break;
    case IDI_LAST_TURN_CARD:
      SwitchToCurrentCard();
      break;
    default:
      break;
  }
  __super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{

//#ifdef _DEBUG
//
//  //��ȡ�˿�
//  BYTE cbCardData1[7]={0x03,0x03,0x13,0x13,0x23,0x23,0x33};
//  BYTE cbShootCount1=7;
//
//  BYTE cbCardData2[7]={0x09,0x19,0x29,0x39,0x09,0x4E,0x4F};
//  BYTE cbShootCount2=7;
//
//
//  BYTE cbCard[17]={0x04,0x14,0x24,
//    0x02,0x12,0x22,
//    0x03,0x13,0x23,0x33,0x03,
//    0x05,0x15,0x25,0x35,
//    0x4E,0x4F};
//
//
//  tagAnalyseResult CurOutCard,LastOutCard;
//  memset(&CurOutCard,0,sizeof(CurOutCard));
//  memset(&LastOutCard,0,sizeof(LastOutCard));
//
//  //��ȡ����
//  tagAnalyseResult analyseResult;
//  memset(&analyseResult,0,sizeof(analyseResult));
//  m_GameLogic.AnalysebCardData(cbCardData2,7,analyseResult);
//  m_GameLogic.FindValidCard(analyseResult,LastOutCard);
//  memcpy(&LastOutCard,&analyseResult,sizeof(tagAnalyseResult));
//
//
//
//  m_GameLogic.AnalysebCardData(cbCardData1,cbShootCount1,CurOutCard);
//
//  if(m_GameLogic.CompareCard(CurOutCard,LastOutCard))
//  {
//    AfxMessageBox(TEXT("����"));
//  }
//  else
//  {
//    AfxMessageBox(TEXT("С��"));
//  }
//  return 0;
//#endif

//#ifdef _DEBUG
//
//  BYTE cbCardData1[MAX_COUNT]={0x0A,0x0B,0x0C,0x0D,0x01,0x02,0x24,0x24,0x34,0x34,0x15,0x25,0x25,0x35,0x35};
//
//  if(m_GameLogic.IsLinkCardData(cbCardData1,6))
//  {
//    AfxMessageBox(TEXT("����"));
//  }
//  else
//  {
//    AfxMessageBox(TEXT("������"));
//  }
//  return 0;
//
//#endif

  //#ifdef _DEBUG
  //
  //  BYTE cbCardData1[BOMB_TYPE_COUNT]={4,0,0,0,0,0,0,0,0};
  //

  //  //�����÷ַ���
  //  int cbBombGrade=m_GameLogic.GetMaxBombGrade(cbCardData1);
  //  //5����ʼ���㷭��
  //  int  nMutile=(int)pow(2,cbBombGrade);

  //  {
  //    CString strInfo=TEXT("");
  //
  //    strInfo.Format(TEXT("nMutile=%d "),nMutile);
  //
  //
  //    AfxMessageBox(strInfo);
  //  }
  //
  //  return 0;
  //
  //#endif


  /*#ifdef _DEBUG

    BYTE cbCardData1[BOMB_TYPE_COUNT]={6,0,0,0,0,0,0,0,0};

    if(m_GameLogic.TransBombGrade(cbCardData1))
    {
      CString strInfo=TEXT("");
      for (int i=0;i<BOMB_TYPE_COUNT;i++)
      {
        CString strOne=TEXT("");
        strOne.Format(TEXT("%d��:%x�� "),i+4,cbCardData1[i]);
        strInfo+=strOne;
      }

      AfxMessageBox(strInfo);
    }
    else
    {
      AfxMessageBox(TEXT("ʧ��"));
    }
    return 0;

  #endif*/

  /*#ifdef _DEBUG
  BYTE cbHandCardData[MAX_COUNT];
  ZeroMemory(cbHandCardData,sizeof(cbHandCardData));
  cbHandCardData[0]=0x4E;
  cbHandCardData[1]=0x4F;
  cbHandCardData[2]=0x0D;
  cbHandCardData[3]=0x1C;
  cbHandCardData[4]=0x0B;
  cbHandCardData[5]=0x1B;
  cbHandCardData[6]=0x2B;
  cbHandCardData[7]=0x3B;
  cbHandCardData[8]=0x4E;


  m_GameLogic.SortCardList(cbHandCardData,9,ST_COUNT);

  CString strInfo=TEXT("");
  for (int i=0;i<MAX_COUNT;i++)
  {
    CString strOne=TEXT("");
    strOne.Format(TEXT("%x "),cbHandCardData[i]);
    strInfo+=strOne;
  }
  AfxMessageBox(strInfo);
  #endif*/


  //ʤ����Ϣ
  m_wWinOrder=0xFFFF;
  //���ñ���
  m_dwCardHSpace=DEFAULT_PELS;
  //�˿˱���
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
  ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

  //���Ʊ���
  m_cbTurnCardCount=0;
  ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
  ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

  //���ý���
  KillGameClock(IDI_START_GAME);
  m_GameClientView.SetCardCount(INVALID_CHAIR,0);
  m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
  m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0xFFFF);

  //���׷���
  for(int i=0; i<GAME_PLAYER; i++)
  {
    m_GameClientView.SetBombScore(SwitchViewChairID(i),0);
  }

  m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

  //���ؿؼ�
  m_GameClientView.SetShowScoreView(false);

  //�����˿�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
    m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
  }
  m_GameClientView.m_HandCardControl[i].SetPositively(false);

  //������Ϣ
  SendUserReady(NULL,0);
  return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
  m_bReqHumter=false;
  //״̬�ж�
  if(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE)
  {
    return 0;
  }
  if(m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE)
  {
    return 0;
  }

  //���ý���
  KillGameClock(IDI_OUT_CARD);
  m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btOutCard.EnableWindow(FALSE);
  m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btPassCard.EnableWindow(FALSE);
  m_GameClientView.m_btHunter.ShowWindow(SW_HIDE);

  //��ȡ�˿�
  BYTE cbCardData[MAX_COUNT];
  BYTE cbCardCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(cbCardData,CountArray(cbCardData));

  //�����˿�
  //m_GameLogic.SortCardList(cbCardData,cbCardCount);

  //��������
  WORD wMeChairID=GetMeChairID();
  BYTE cbSourceCount=m_cbHandCardCount[wMeChairID];

  m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(cbCardData,cbCardCount);

  //ɾ���˿�
  m_cbHandCardCount[wMeChairID]-=cbCardCount;
  m_GameLogic.RemoveCardEx(cbCardData,cbCardCount,m_cbHandCardData,cbSourceCount);



  BYTE cbHandCardData[MAX_COUNT];
  CopyMemory(cbHandCardData,m_cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);
  //m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[wMeChairID]);
  //m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(cbHandCardData,m_cbHandCardCount[wMeChairID]);



  //BYTE cbHandCardData[MAX_COUNT];
  ZeroMemory(cbHandCardData,sizeof(cbHandCardData));
  CopyMemory(cbHandCardData,m_cbHandCardData,sizeof(m_cbHandCardData));
  if(m_bSortCount)
  {
    m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[GetMeChairID()],ST_COUNT);
  }
  else
  {
    m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[GetMeChairID()]);
  }
  m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(cbHandCardData,m_cbHandCardCount[GetMeChairID()]);



  //��������
  CMD_C_OutCard OutCard;
  OutCard.cbCardCount=cbCardCount;
  CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount);
  SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(CMD_C_OutCard));

  ZeroMemory(&m_SearchInfo,sizeof(m_SearchInfo));

  m_bHaveOutCard = true;
  //���½���
  m_GameClientView.RefreshGameView();
  return 0;
}

//��������
LRESULT CGameClientEngine::OnPassCard(WPARAM wParam, LPARAM lParam)
{

  m_bReqHumter=false;
  //״̬�ж�
  if(m_GameClientView.m_btPassCard.IsWindowVisible()==FALSE)
  {
    return 0;
  }
  if(m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE)
  {
    return 0;
  }

  //���ý���
  KillGameClock(IDI_OUT_CARD);
  m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btOutCard.EnableWindow(FALSE);
  m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btPassCard.EnableWindow(FALSE);
  m_GameClientView.m_btHunter.ShowWindow(SW_HIDE);

  //��������
  SendSocketData(SUB_C_PASS_CARD);

  //Ԥ����ʾ
  m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(NULL,0);
  m_GameClientView.SetPassFlag(ME_VIEW_CHAIR,true);

  PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

  return 0;
}

//������ʾ
LRESULT CGameClientEngine::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
  //�������
  tagOutCardResult OutCardResult= {0};
  //���ý���
  if(OutCardResult.cbCardCount>0)
  {
    //���ý���
    m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

    //���ÿؼ�
    bool bOutCard=VerdictOutCard();
    m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

    //���½���
    m_GameClientView.RefreshGameView();
  }
  else
  {
    //��������
    OnPassCard(0,0);
  }
  return 0;
}


LRESULT CGameClientEngine::OnReqHunter(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_REQ_HUNTERS,NULL,0);
  return 1;
}

//��Ŀ����
LRESULT CGameClientEngine::OnSortByCount(WPARAM wParam, LPARAM lParam)
{
  //����״̬
  if(m_bSortCount==true)
  {
    m_bSortCount=false;
    m_GameClientView.m_btSortByCount.SetButtonImage(IDB_BT_SORT_BY_COUNT,AfxGetInstanceHandle(),false,false);
  }
  else
  {
    m_bSortCount=true;
    m_GameClientView.m_btSortByCount.SetButtonImage(IDB_BT_SORT_BY_ORDER,AfxGetInstanceHandle(),false,false);
  }
  if(m_cbHandCardCount[GetMeChairID()]>0)
  {
    BYTE cbHandCardData[MAX_COUNT];
    ZeroMemory(cbHandCardData,sizeof(cbHandCardData));
    CopyMemory(cbHandCardData,m_cbHandCardData,sizeof(m_cbHandCardData));
    if(m_bSortCount)
    {
      m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[GetMeChairID()],ST_COUNT);
    }
    else
    {
      m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[GetMeChairID()]);
    }
    m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(cbHandCardData,m_cbHandCardCount[GetMeChairID()]);

    PlayGameSound(AfxGetInstanceHandle(),TEXT("SORT_CARD"));
  }

  m_GameClientView.RefreshGameView();
  return 0;
}



//�Ҽ��˿�
LRESULT CGameClientEngine::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
  //���ÿؼ�
  BYTE cbCardData[MAX_COUNT];
  ZeroMemory(cbCardData,sizeof(cbCardData));
  WORD byCardCount = m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(cbCardData,MAX_COUNT);
  //����˳��
  if(byCardCount == 2)
  {

    tagOutCardResult OutCardResult;
    if(m_GameLogic.SearchLinkCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],cbCardData[0],cbCardData[1],OutCardResult))
    {
      m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].ShootAllCard(false);
      m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
    }
  }

  //���ÿؼ�
  bool bOutCard=VerdictOutCard();
  m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);
  return 0;
}

//���˫��
LRESULT CGameClientEngine::OnLeftDBHitCard(WPARAM wParam, LPARAM lParam)
{

  BYTE cbCardData[MAX_COUNT];
  ZeroMemory(cbCardData,sizeof(cbCardData));
  WORD byCardCount = m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(cbCardData,MAX_COUNT);

  WORD cbSelectCard=(WORD)wParam;
  //����ͬ���˿�
  {
    tagOutCardResult OutCardResult;
    ZeroMemory(&OutCardResult,sizeof(OutCardResult));
    if(m_GameLogic.SearchSamePointCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],cbSelectCard,OutCardResult))
    {
      m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].ContinueShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
    }
  }
  //���ÿؼ�
  bool bOutCard=VerdictOutCard();
  m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);
  return 0;
}

//����˿�
LRESULT CGameClientEngine::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
  //�û�����
  OnOutCard(0,0);
  return 0;
}
//�Ϲܿ���
LRESULT CGameClientEngine::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
  //����״̬
  if(m_bStustee==true)
  {
    m_bStustee=false;
    m_wTimeOutCount=0;
    m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
    if(m_pIStringMessage != NULL)
    {
      m_pIStringMessage->InsertSystemString(_T("��ȡ�����йܹ���."));
    }
    CMD_C_Trustee Trustee;
    Trustee.bTrustee = false;
    SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

  }
  else
  {
    m_bStustee=true;
    m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
    if(m_pIStringMessage != NULL)
    {
      m_pIStringMessage->InsertSystemString(_T("��ѡ�����йܹ���."));
    }
    CMD_C_Trustee Trustee;
    Trustee.bTrustee = true;
    SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
  }
  return 0;
}


//����
LRESULT CGameClientEngine::OnAdminControl(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));
  return true;
}


//������ʾ
LRESULT CGameClientEngine::OnCardType(WPARAM wParam,LPARAM lParam)
{

  //״̬�ж�
  if(m_GameClientView.m_btOne.IsWindowVisible()==FALSE)
  {
    return 0;
  }
  if(m_GameClientView.m_btOne.IsWindowEnabled()==FALSE)
  {
    return 0;
  }

  //��Ŀ����
  BYTE cbCardCount=(BYTE)wParam;
  if(cbCardCount>m_cbHandCardCount[GetMeChairID()])
  {
    return 0;
  }
  ASSERT(cbCardCount>=1 &&cbCardCount<=4);
  if(cbCardCount<1 ||cbCardCount>4)
  {
    return 0;
  }

  if(m_SearchInfo.cbCurSelectFunc!=cbCardCount)
  {
    ZeroMemory(&m_SearchInfo,sizeof(m_SearchInfo));
    m_SearchInfo.cbCurSelectFunc=cbCardCount;

  }

  m_SearchInfo.cbCurIndex[cbCardCount-1]++;


  //�����˿�
  tagAnalyseInitResult AnalyseResult;
  m_GameLogic.AnalyseInitCardData(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],AnalyseResult);

  if(m_SearchInfo.cbCurIndex[cbCardCount-1]>AnalyseResult.cbBlockCount[cbCardCount-1])
  {
    m_SearchInfo.cbCurIndex[cbCardCount-1]=1;
  }

  m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].ShootAllCard(false);

  int nSelectIndex=AnalyseResult.cbBlockCount[cbCardCount-1]-(m_SearchInfo.cbCurIndex[cbCardCount-1]-1)-1;
  m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetShootCard(&AnalyseResult.cbCardData[cbCardCount-1][nSelectIndex*cbCardCount],
      cbCardCount);

  OnLeftHitCard(0,0);

  m_GameClientView.RefreshGameView();

  return 0;
}
//�����˿�
LRESULT CGameClientEngine::OnLastTurnCard(WPARAM wParam, LPARAM lParam)
{
  if(m_bLastTurn==false)
  {
    //�����˿�
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      WORD wViewChairID=SwitchViewChairID(i);
      if(m_cbOutCardCount[i][1]!=255)
      {
        if(m_cbOutCardCount[i][1]==0)
        {
          m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
          m_GameClientView.SetPassFlag(wViewChairID,true);
        }
        else
        {
          m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][1],m_cbOutCardCount[i][1]);
        }
      }
      else
      {
        m_GameClientView.SetPassFlag(wViewChairID,false);
        m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
      }
    }

    //��������
    m_bLastTurn=true;
    m_GameClientView.SetLastTurnCard(true);
    m_GameClientView.RefreshGameView();
    SetTimer(IDI_LAST_TURN_CARD,1000,NULL);
  }
  else
  {
    SwitchToCurrentCard();
  }
  return 0;
}


//���ų�������
bool CGameClientEngine::PlayCardSound(WORD wChairID,tagAnalyseResult &OutCard)
{

  if(wChairID < 0 || wChairID > 3)
  {
    return false;
  }
  CString strValue=TEXT("");

  //��ը
  if(OutCard.m_cbCardType==CT_BOMB_LINK)
  {
    if(OutCard.m_nUnEqualCard==3)
    {
      strValue=TEXT("THREE_LINK_BOMB");
    }
    else if(OutCard.m_nUnEqualCard==4)
    {
      strValue=TEXT("FOUR_LINK_BOMB");
    }
    else if(OutCard.m_nUnEqualCard==5)
    {
      strValue=TEXT("FIVE_LINK_BOMB");
    }
  }
  else if(OutCard.m_cbCardType==CT_BOMB_3W)
  {
    strValue=TEXT("SIX_XIANG");
  }
  else if(OutCard.m_cbCardType==CT_BOMB_TW)
  {
    strValue=TEXT("SEVEN_XIANG");
  }
  else
  {
    if(OutCard.m_cbCardType==CT_BOMB)
    {
      if(OutCard.m_cbCardCount==5)
      {
        strValue=TEXT("FIVE_XIANG");
      }
      else if(OutCard.m_cbCardCount==6)
      {
        strValue=TEXT("SIX_XIANG");
      }
      else if(OutCard.m_cbCardCount==7)
      {
        strValue=TEXT("SEVEN_XIANG");
      }
      else if(OutCard.m_cbCardCount==8)
      {
        strValue=TEXT("EIGHT_XIANG");
      }
      else if(OutCard.m_cbCardCount>=9)
      {
        strValue=TEXT("NINE_YISHANG_XIANG");
      }
    }

  }
  if(strValue==TEXT(""))
  {
    return false;
  }

  PlayGameSound(AfxGetInstanceHandle(), strValue);
  return true;
}
//��ȡ�˿���Ϣ
CString CGameClientEngine::GetCardInfo(BYTE cbCardData)
{
  CString strInfo=TEXT(""),strColor=TEXT(""),strData=TEXT("");

  BYTE cbColor=m_GameLogic.GetCardColor(cbCardData);
  BYTE cbData=m_GameLogic.GetCardValue(cbCardData);

  if(cbCardData==0x4E)
  {
    strInfo=TEXT("����");
    return strInfo;
  }
  if(cbCardData==0x4F)
  {
    strInfo=TEXT("С��");
    return strInfo;
  }

  switch(cbColor)
  {
    case 0x00:
      strColor=TEXT("����");
      break;
    case 0x10:
      strColor=TEXT("÷��");
      break;
    case 0x20:
      strColor=TEXT("����");
      break;
    case 0x30:
      strColor=TEXT("����");
      break;
  }
  switch(cbData)
  {
    case 1:
      strData=TEXT("A");
      break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      strData.Format(TEXT("%d"),cbData);
      break;
    case 11:
      strData=TEXT("J");
      break;
    case 12:
      strData=TEXT("Q");
      break;
    case 13:
      strData=TEXT("K");
      break;
  }
  strInfo=strColor+strData;
  return strInfo;
}

//////////////////////////////////////////////////////////////////////////
