#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_START_GAME          200               //��ʼ��ʱ��
#define IDI_GIVE_UP           201               //������ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
  ON_MESSAGE(IDM_START,OnStart)
  ON_MESSAGE(IDM_FOLLOW,OnFollow)
  ON_MESSAGE(IDM_GIVE_UP,OnGiveUp)
  ON_MESSAGE(IDM_SHOWHAND,OnShowHand)
  ON_MESSAGE(IDM_ADD_GOLD,OnAddGold)
  ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
  ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)
  ON_MESSAGE(IDM_ADMIN_COMMDN_EX, OnAdminControlEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
  //��Ϸ����
  m_lTurnMaxGold=0L;
  m_lTurnBasicGold=0L;
  m_lBasicGold = 0L;
  memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
  m_lShowHandScore = 0L;
  ZeroMemory(m_lUserScore,sizeof(m_lUserScore));


  //��������
  m_lGoldShow=0L;
  m_bShowHand=false;
  memset(m_szName,0,sizeof(m_szName));
  ZeroMemory(&m_GameEnd,sizeof(m_GameEnd));

  return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
  //ȫ�ֶ���
  CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
  ASSERT(m_pGlobalUnits!=NULL);

  //����ͼ��
  HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
  m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
  SetIcon(hIcon,TRUE);
  SetIcon(hIcon,FALSE);

  return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
  //��Ϸ����
  m_lTurnMaxGold=0L;
  m_lTurnBasicGold=0L;
  m_lBasicGold = 0L;
  memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
  m_wCurrentUser = INVALID_CHAIR;
  m_lShowHandScore = 0L;
  ZeroMemory(m_lUserScore,sizeof(m_lUserScore));


  //��������
  m_lGoldShow=0L;
  m_bShowHand=false;
  memset(m_szName,0,sizeof(m_szName));
  ZeroMemory(&m_GameEnd,sizeof(m_GameEnd));

  return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
  //��������
  CGameOption GameOption;
  GameOption.m_bEnableSound=m_pGlobalUnits->m_bAllowSound;
  GameOption.m_bAllowLookon=m_pGlobalUnits->m_bAllowLookon;

  //��������
  //if (GameOption.DoModal()==IDOK)
  //{
  //  EnableSound(GameOption.m_bEnableSound);
  //  AllowUserLookon(0,GameOption.m_bAllowLookon);
  //}

  //��������
  m_pGlobalUnits->m_bAllowSound=GameOption.m_bEnableSound;
  m_pGlobalUnits->m_bAllowLookon=GameOption.m_bAllowLookon;

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
    case IDI_START_GAME:    //������Ϸ��ʱ��
    {
      if(nElapse==0)
      {
        AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
        return false;
      }
      if(nElapse<=10)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
      }

      return true;
    }
    case IDI_GIVE_UP:     //������ʱ��
    {
      WORD wViewChairID=SwitchViewChairID(wChairID);
      if(nElapse==0)
      {
        if((IsLookonMode()==false)&&(wViewChairID==MYSELF_VIEW_ID))
        {
          OnGiveUp(0,0);
        }
        return false;
      }
      if((nElapse<=10)&&(wViewChairID==MYSELF_VIEW_ID)&&(IsLookonMode()==false))
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
  return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
  switch(wSubCmdID)
  {
    case SUB_S_GAME_START:  //��Ϸ��ʼ
    {
      return OnSubGameStart(pData,wDataSize);
    }
    case SUB_S_ADD_GOLD:  //�û���ע
    {
      m_GameClientView.FinishDispatchCard();
      return OnSubAddGold(pData,wDataSize);
    }
    case SUB_S_GIVE_UP:   //�û�����
    {
      m_GameClientView.FinishDispatchCard();
      return OnSubGiveUp(pData,wDataSize);
    }
    case SUB_S_SEND_CARD: //������Ϣ
    {
      m_GameClientView.FinishDispatchCard();
      return OnSubSendCard(pData,wDataSize);
    }
    case SUB_S_GAME_END:  //��Ϸ����
    {
      return OnSubGameEnd(pData,wDataSize);
    }
    case SUB_S_AMDIN_COMMAND:   //����
    {
      return OnSubAdminControl(pData,wDataSize);
    }
    case SUB_S_AMDIN_COMMAND_EX:    //����
    {
      return OnSubAdminControlEx(pData,wDataSize);
    }
    //case SUB_S_ALL_CARD:  //�˿���Ϣ
    //  {
    //    return true;
    //  }
  }

  return false;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:    //����״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusFree))
      {
        return false;
      }
      CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

      //��������
      m_bShowHand=false;

      //���ý���
      m_lGoldShow=0L;
      m_lTurnBasicGold=0L;
      m_lBasicGold = pStatusFree->dwBasicGold;
      m_GameClientView.SetGoldTitleInfo(0,pStatusFree->dwBasicGold);

      //�������
      IClientUserItem * pIClientUserItem=GetMeUserItem();
      if((IsLookonMode()==false)&&(pIClientUserItem->GetUserStatus()!=US_READY))
      {
        //���ð�ť
        m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
        m_GameClientView.m_btStart.SetFocus();

        //����ʱ��
        SetGameClock(GetMeChairID(),IDI_START_GAME,30);
      }
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
      {
        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
        m_GameClientView.m_btOpenAdminEx.ShowWindow(SW_SHOW);
      }

      return true;
    }
    case GAME_STATUS_PLAY:  //��Ϸ״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusPlay))
      {
        return false;
      }
      CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

      //���ñ���
      m_lTurnMaxGold=pStatusPlay->lTurnMaxGold;
      m_lTurnBasicGold=pStatusPlay->lTurnBasicGold;
      m_lBasicGold = pStatusPlay->lBasicGold;
      CopyMemory(m_bPlayStatus,pStatusPlay->bPlayStatus,sizeof(m_bPlayStatus));
      m_wCurrentUser = pStatusPlay->wCurrentUser;
      m_lShowHandScore = pStatusPlay->lShowHandScore;
      m_bShowHand = pStatusPlay->bShowHand?true:false;

      //���ý���
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        WORD wViewStation=SwitchViewChairID(i);
        IClientUserItem * pClientUserItem=GetTableUserItem(i);
        if(pClientUserItem!=NULL)
        {
          m_lUserScore[i] = pClientUserItem->GetUserScore()-pStatusPlay->lServiceScore;
          lstrcpyn(m_szName[i],pClientUserItem->GetNickName(),CountArray(m_szName[i]));
          if(m_bPlayStatus[i]==TRUE)
          {
            m_GameClientView.m_CardControl[wViewStation].SetCardData(pStatusPlay->bTableCardArray[i],pStatusPlay->bTableCardCount[i]);
          }
          m_GameClientView.SetUserGold(wViewStation,pClientUserItem->GetUserScore());
        }
        m_GameClientView.SetUserGoldInfo(wViewStation,false,pStatusPlay->lTableGold[2*i]);
        m_GameClientView.SetUserGoldInfo(wViewStation,true,pStatusPlay->lTableGold[2*i+1]);
      }
      m_GameClientView.SetGoldTitleInfo(pStatusPlay->lTurnMaxGold,pStatusPlay->lBasicGold);

      //�ж��Ƿ�ۿ�
      if((IsLookonMode()==false)||(IsAllowLookon()==true))
      {
        m_GameClientView.m_CardControl[MYSELF_VIEW_ID].AllowPositively(true);
      }

      //�ж��Ƿ��Լ���ע
      if((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
      {
        UpdateScoreControl();
      }
      SetGameClock(pStatusPlay->wCurrentUser,IDI_GIVE_UP,30);

      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
      {
        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
        m_GameClientView.m_btOpenAdminEx.ShowWindow(SW_SHOW);
      }

      return true;
    }
  }

  return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
    return false;
  }
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  //��Ϸ����
  m_lTurnMaxGold=pGameStart->lTurnMaxGold;
  m_lTurnBasicGold=pGameStart->lTurnBasicGold;
  m_lBasicGold = pGameStart->lBasicGold;
  memset(m_szName,0,sizeof(m_szName));
  memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
  m_wCurrentUser = pGameStart->wCurrentUser;
  m_lShowHandScore = pGameStart->lShowHandScore;

  //��������
  m_lGoldShow=0L;
  m_bShowHand=false;

  //��������
  WORD wMeChairID=GetMeChairID();
  bool bLookonMode=IsLookonMode();
  LONGLONG lBaseGold=m_lBasicGold;
  m_GameClientView.SetGoldTitleInfo(pGameStart->lTurnMaxGold,m_lBasicGold);

  //����״̬
  SetGameStatus(GAME_STATUS_PLAY);

  //���ý���
  m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��������
    WORD wViewChairID=SwitchViewChairID(i);
    IClientUserItem * pClientUserItem=GetTableUserItem(i);

    //���ý���
    if(pClientUserItem!=NULL)
    {
      m_bPlayStatus[i]=TRUE;
      m_lUserScore[i] = pClientUserItem->GetUserScore()-pGameStart->lServiceScore;
      m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
      m_GameClientView.SetUserGoldInfo(wViewChairID,true,lBaseGold);
      m_GameClientView.m_GoldView[i*2].SetGoldIndex(1);
      lstrcpyn(m_szName[i],pClientUserItem->GetNickName(),CountArray(m_szName[i]));
      m_GameClientView.SetUserGold(wViewChairID,pClientUserItem->GetUserScore());
    }
    else
    {
      m_bPlayStatus[i]=FALSE;
      m_lUserScore[i] = 0L;
      m_GameClientView.SetUserGoldInfo(wViewChairID,true,0L);
      m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
    }

    //���ÿؼ�
    m_GameClientView.m_CardControl[wViewChairID].SetCardData(NULL,0);
    m_GameClientView.m_CardControl[wViewChairID].ShowFirstCard(false);
  }
  if(bLookonMode==false)
  {
    ActiveGameFrame();
    m_GameClientView.m_CardControl[MYSELF_VIEW_ID].AllowPositively(true);
  }

  //�ɷ��˿�
  for(BYTE cbIndex=0; cbIndex<2; cbIndex++)
  {
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      if(m_bPlayStatus[i]==TRUE)
      {
        //��������
        WORD wViewChairID=SwitchViewChairID(i);
        BYTE cbCardData[2]= {0,pGameStart->bCardData[i]};

        //�ɷ��˿�
        cbCardData[0]=(GetMeChairID()==i)?pGameStart->bFundusCard:0;
        m_GameClientView.DispatchUserCard(wViewChairID,cbCardData[cbIndex]);
      }
    }
  }

  //����
  PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

  return true;
}

//�û���ע
bool CGameClientEngine::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_AddGold))
  {
    return false;
  }
  CMD_S_AddGold * pAddGold=(CMD_S_AddGold *)pBuffer;

  //��������
  WORD wMeChairID=GetMeChairID();
  WORD wViewChairID=SwitchViewChairID(pAddGold->wLastChairID);
  m_wCurrentUser = pAddGold->wCurrentUser;

  //��������
  m_lTurnBasicGold=pAddGold->lCurrentLessGold;

  m_bShowHand = pAddGold->bShowHand;
  if(m_bShowHand && m_bPlayStatus[pAddGold->wLastChairID])
  {
    m_GameClientView.SetUserShowHand(true);
    m_GameClientView.m_GoldView[wViewChairID*2].SetGoldIndex(2);
  }

  if(pAddGold->lLastAddGold > 0L)
  {
    m_GameClientView.SetUserGoldInfo(wViewChairID,false,pAddGold->lLastAddGold);
  }
  if((IsLookonMode()==false)&&(pAddGold->wCurrentUser==wMeChairID))
  {
    UpdateScoreControl();
  }

  //��������
  SetGameClock(pAddGold->wCurrentUser,IDI_GIVE_UP,30);

  if(m_bShowHand && m_bPlayStatus[pAddGold->wLastChairID])
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
  }
  else if(m_bPlayStatus[pAddGold->wLastChairID]==TRUE)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
  }

  return true;
}

//�û�����
bool CGameClientEngine::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_GiveUp))
  {
    return false;
  }
  CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

  //���ñ���
  m_bPlayStatus[pGiveUp->wUserChairID]=false;
  if(pGiveUp->wUserChairID==GetMeChairID())
  {
    SetGameStatus(GAME_STATUS_FREE);
  }

  //���ý���
  BYTE bCard[5]= {0,0,0,0,0};
  WORD wViewStation=SwitchViewChairID(pGiveUp->wUserChairID);
  WORD bCount=m_GameClientView.m_CardControl[wViewStation].GetCardCount();
  m_GameClientView.m_CardControl[wViewStation].AllowPositively(false);
  m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(false);
  m_GameClientView.m_CardControl[wViewStation].SetCardData(bCard,bCount);

  //��������
  if(m_lTurnMaxGold != pGiveUp->lTurnMaxGold)
  {
    //��������
    m_lTurnMaxGold = pGiveUp->lTurnMaxGold;

    //���������ע
    m_GameClientView.SetGoldTitleInfo(m_lTurnMaxGold,m_GameClientView.m_lBasicGold);

    if(!IsLookonMode() && m_wCurrentUser==GetMeChairID() && pGiveUp->wUserChairID != m_wCurrentUser)
    {
      UpdateScoreControl();
    }
  }

  //��������
  if(pGiveUp->wUserChairID==GetClockChairID())
  {
    KillGameClock(IDI_GIVE_UP);
  }
  PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

  return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_R_SendCard))
  {
    return false;
  }
  CMD_R_SendCard * pSendCard=(CMD_R_SendCard *)pBuffer;

  //���ñ���
  m_lGoldShow=0L;
  m_lTurnBasicGold=0L;
  m_lTurnMaxGold=pSendCard->lMaxGold;
  m_wCurrentUser = pSendCard->wCurrentUser;

  //���½���
  LONGLONG lUserTableGold=0L;
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��ȡλ��
    WORD wViewChairID=SwitchViewChairID(i);

    //���ý��
    lUserTableGold=m_GameClientView.m_GoldView[wViewChairID*2].GetGold();
    lUserTableGold+=m_GameClientView.m_GoldView[wViewChairID*2+1].GetGold();
    m_GameClientView.SetUserGoldInfo(wViewChairID,false,0L);
    m_GameClientView.SetUserGoldInfo(wViewChairID,true,lUserTableGold);
  }

  //�ɷ��˿�,���ϴ������ҿ�ʼ����
  WORD wLastMostUser = pSendCard->wStartChairId;
  ASSERT(wLastMostUser != INVALID_CHAIR);
  for(BYTE i=0; i<pSendCard->cbSendCardCount; i++)
  {
    for(WORD j=0; j<GAME_PLAYER; j++)
    {
      WORD wChairId = (wLastMostUser+j)%GAME_PLAYER;
      if(m_bPlayStatus[wChairId]==TRUE&&pSendCard->bUserCard[wChairId][i]!=0)
      {
        WORD wViewChairID=SwitchViewChairID(wChairId);
        m_GameClientView.DispatchUserCard(wViewChairID,pSendCard->bUserCard[wChairId][i]);
      }
    }
  }

  //��������
  if(!m_bShowHand)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
  }

  return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
  //Ч�����
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

  //��������
  if(!IsLookonMode())
  {
    if(pGameEnd->lGameGold[GetMeChairID()]>0L)
    {
      PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
    }
    else
    {
      PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
    }
  }
  else
  {
    PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));
  }

  //����״̬
  SetGameStatus(GAME_STATUS_FREE);

  KillGameClock(IDI_GIVE_UP);

  //��������
  CopyMemory(&m_GameEnd,pGameEnd,sizeof(m_GameEnd));

  if(!m_GameClientView.IsDispatchCard())
  {
    PerformGameEnd();
  }

  return true;
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
  //ɾ��ʱ��
  KillGameClock(IDI_START_GAME);

  //���ÿؼ�
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_GameClientView.m_GoldView[i*2].SetGold(0L);
    m_GameClientView.m_GoldView[i*2+1].SetGold(0L);
    m_GameClientView.m_GoldView[i*2].SetGoldIndex(1);
    m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
    m_GameClientView.m_CardControl[i].ShowFirstCard(false);
  }

  //���ÿؼ�
  m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
  HideScoreControl();

  //���ؿؼ�
  m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

  //���½���
  m_GameClientView.RefreshGameView();

  //������Ϣ
  SendUserReady(NULL,0);

  return 0;
}

//������ť
LRESULT CGameClientEngine::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
  //������Ϸ
  KillGameClock(IDI_GIVE_UP);
  //��ʾ��ť
  HideScoreControl();

  m_GameClientView.SetUserGoldInfo(MYSELF_VIEW_ID,false,m_lGoldShow);

  //������Ϣ
  SendSocketData(SUB_C_GIVE_UP);

  return 0;
}

//��ע��ť
LRESULT CGameClientEngine::OnFollow(WPARAM wParam, LPARAM lParam)
{

  //��ȡ���
  LONGLONG lGold=m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold();

  LONGLONG lShowHandScore = __min(m_lUserScore[GetMeChairID()],m_lShowHandScore);

  if(lGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold() < lShowHandScore)
  {
    lGold=__max(lGold,m_lTurnBasicGold);
  }

  //���ñ���
  if(lShowHandScore == lGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold())
  {
    if(!m_bShowHand)
    {
      m_GameClientView.SetUserShowHand(true);
    }
    m_bShowHand=true;
    m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].SetGoldIndex(2);
  }

  //��ʾ��ť
  HideScoreControl();

  m_GameClientView.SetUserGoldInfo(MYSELF_VIEW_ID,false,lGold);

  //ɾ����ʱ��
  KillGameClock(IDI_GIVE_UP);

  //��������
  CMD_C_AddGold AddPoint;
  AddPoint.lGold=lGold;
  SendSocketData(SUB_C_ADD_GOLD,&AddPoint,sizeof(AddPoint));

  return 0;
}

//�����ť
LRESULT CGameClientEngine::OnShowHand(WPARAM wParam, LPARAM lParam)
{
  LONGLONG lShowHandScore = __min(m_lUserScore[GetMeChairID()],m_lShowHandScore);
  LONGLONG lGold=lShowHandScore-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold();

  m_GameClientView.SetUserGoldInfo(MYSELF_VIEW_ID,false,lGold);
  OnFollow(0,0);

  m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].SetGoldIndex(2);

  return 0;
}

//��ע��Ϣ

LRESULT CGameClientEngine::OnAddGold(WPARAM wParam, LPARAM lParam)
{
  LONGLONG lGold = m_lTurnBasicGold+m_lBasicGold*(LONG)wParam;
  LONGLONG lShowHandScore = __min(m_lUserScore[GetMeChairID()],m_lShowHandScore);

  if(lGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold() < lShowHandScore)
  {
    lGold=__max(lGold,m_lTurnBasicGold);
  }

  //���ü�ע
  m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].SetGold(lGold);

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("CHOOSE_SCORE"));

  //
  OnFollow(0,0);

  return 0;
}

//�������
LRESULT CGameClientEngine::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
  if(m_bPlayStatus[GetMeChairID()] && GetGameStatus() == GAME_STATUS_FREE)
  {
    PerformGameEnd();
    return 0;
  }

  //��ע�ж�
  if((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
  {
    UpdateScoreControl();
  }


  //���ö�ʱ��
  SetGameClock(m_wCurrentUser,IDI_GIVE_UP,30);

  return 0;
}

//����
LRESULT CGameClientEngine::OnAdminControl(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_AMDIN_COMMAND,(CMD_C_AdminReq*)wParam,sizeof(CMD_C_AdminReq));
  return true;
}

//����
bool CGameClientEngine::OnSubAdminControl(const void * pBuffer, WORD wDataSize)
{
  ASSERT(wDataSize==sizeof(CMD_S_CommandResult));
  if(wDataSize!=sizeof(CMD_S_CommandResult))
  {
    return false;
  }

  m_GameClientView.m_pClientControlDlg->ReqResult(pBuffer);
  return true;
}

//����
LRESULT CGameClientEngine::OnAdminControlEx(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_AMDIN_COMMAND_EX,(CMD_C_AdminReqEx*)wParam,sizeof(CMD_C_AdminReqEx));
  return true;
}

//����
bool CGameClientEngine::OnSubAdminControlEx(const void * pBuffer, WORD wDataSize)
{
  ASSERT(wDataSize==sizeof(CMD_S_CommandResultEx));
  if(wDataSize!=sizeof(CMD_S_CommandResultEx))
  {
    return false;
  }

  m_GameClientView.m_pClientControlDlgEx->ReqResult(pBuffer);
  return true;
}

//
void CGameClientEngine::UpdateScoreControl()
{
  ActiveGameFrame();

  //��ʾ��ť
  m_GameClientView.ShowScoreControl(true);

  WORD wMeChairID = GetMeChairID();

  //��������
  m_lGoldShow=m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold();

  //��������
  WORD wCardCount=m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardData(NULL,0);
  LONGLONG lLeaveScore=m_lTurnMaxGold-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold()-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold();

  //��ֹ��ť
  m_GameClientView.m_btShowHand.EnableWindow((wCardCount>=3&&(lLeaveScore>0||m_bShowHand))?TRUE:FALSE);
  lLeaveScore=m_lTurnBasicGold-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2].GetGold();
  LONGLONG lShowHandScore = __min(m_lUserScore[wMeChairID],m_lShowHandScore);
  if(!m_bShowHand &&
     (m_lTurnBasicGold == 0 || lLeaveScore>0L&&lShowHandScore>m_lTurnBasicGold+m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold())
    )
  {
    m_GameClientView.m_btFollow.EnableWindow(TRUE);
  }
  else
  {
    m_GameClientView.m_btFollow.EnableWindow(FALSE);
  }

  //��ע��ť
  lLeaveScore=m_lTurnMaxGold-m_lTurnBasicGold-m_GameClientView.m_GoldView[MYSELF_VIEW_ID*2+1].GetGold();
  m_GameClientView.m_btAddTimes1.EnableWindow((lLeaveScore>=m_lBasicGold)?TRUE:FALSE);
  m_GameClientView.m_btAddTimes2.EnableWindow((lLeaveScore>=2*m_lBasicGold)?TRUE:FALSE);
  m_GameClientView.m_btAddTimes3.EnableWindow((lLeaveScore>=3*m_lBasicGold)?TRUE:FALSE);

  return;
}

//
void CGameClientEngine::HideScoreControl()
{
  m_GameClientView.ShowScoreControl(false);
}

//
void CGameClientEngine::PerformGameEnd()
{
  CMD_S_GameEnd *pGameEnd = &m_GameEnd;

  //���û���
  CString strTemp ,strEnd = _T("\n");
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(pGameEnd->lGameGold[i]>0)
    {
      strTemp.Format(_T("%s: %+I64d\n"),m_szName[i],pGameEnd->lGameGold[i]);
    }
    else if(m_szName[i][0] != 0)
    {
      strTemp.Format(_T("%s: %I64d\n"),m_szName[i],pGameEnd->lGameGold[i]);
    }
    else
    {
      strTemp.Empty();
    }
    strEnd += strTemp;
  }
  //��Ϣ����
  if(m_pIStringMessage != NULL)
  {
    m_pIStringMessage->InsertSystemString((LPCTSTR)strEnd);
  }

  //��Ϸ��������
  BYTE bCardData[5];
  m_GameClientView.m_ScoreView.ResetScore();
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //������Ϣ
    if(pGameEnd->lGameGold[i]!=0L)
    {
      if(m_szName[i][0]==0)
      {
        m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("���뿪"),pGameEnd->lGameGold[i]);
      }
      else
      {
        m_GameClientView.m_ScoreView.SetGameScore(i,m_szName[i],pGameEnd->lGameGold[i]);
      }
      if(pGameEnd->bUserCard[i]!=0)
      {
        WORD wViewStation=SwitchViewChairID(i);
        WORD wCardCount=m_GameClientView.m_CardControl[wViewStation].GetCardData(bCardData,CountArray(bCardData));
        bCardData[0]=pGameEnd->bUserCard[i];
        m_GameClientView.m_CardControl[wViewStation].SetCardData(bCardData,wCardCount);
        m_GameClientView.m_CardControl[wViewStation].ShowFirstCard(true);
      }
    }
  }
  m_GameClientView.m_ScoreView.SetTax(pGameEnd->lTax);
  m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

  //���ÿؼ�
  m_GameClientView.SetUserShowHand(false);
  HideScoreControl();

  //��Ϸ����
  m_lTurnMaxGold=0L;
  m_lTurnBasicGold=0L;
  memset(m_bPlayStatus,0,sizeof(m_bPlayStatus));
  m_wCurrentUser = INVALID_CHAIR;
  m_lShowHandScore = 0L;

  //��������
  m_lGoldShow=0L;
  m_bShowHand=false;
  memset(m_szName,0,sizeof(m_szName));

  //���ý���
  if(IsLookonMode()==false)
  {
    m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
    m_GameClientView.m_btStart.SetFocus();
    SetGameClock(GetMeChairID(),IDI_START_GAME,30);
  }
}

//////////////////////////////////////////////////////////////////////////
