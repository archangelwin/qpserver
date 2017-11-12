#include "StdAfx.h"

#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_START_GAME        200                 //��ʼ��ʱ��
#define IDI_USER_OPERATE      201                 //��ע��ʱ��

//ʱ���ʶ
#define TIME_START_GAME       30                  //��ʼ��ʱ��
#define TIME_USER_OPERATE     30                  //������ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
  ON_MESSAGE(IDM_START,OnStart)
  ON_MESSAGE(IDM_ADD_SCORE,OnAddScore)
  ON_MESSAGE(IDM_GET_CARD,OnGetCard)
  ON_MESSAGE(IDM_STOP_CARD,OnStopCard)
  ON_MESSAGE(IDM_DOUBLE_CARD,OnDoubleCard)
  ON_MESSAGE(IDM_SPLIT_CARD,OnSplitCard)
  ON_MESSAGE(IDM_INSURE_CARD,OnInsureCard)
  ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
  ON_MESSAGE(IDM_ADMIN_UPDATE_STORAGE, OnStorage)
  ON_MESSAGE(IDM_ADMIN_MODIFY_STORAGE, OnStorageInfo)
  ON_MESSAGE(IDM_REQUEST_QUERY_USER, OnRequestQueryUser)
  ON_MESSAGE(IDM_USER_CONTROL, OnUserControl)
  ON_MESSAGE(IDM_REQUEST_UPDATE_ROOMINFO, OnRequestUpdateRoomInfo)
  ON_MESSAGE(IDM_CLEAR_CURRENT_QUERYUSER, OnClearCurrentQueryUser)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
  //���ñ���
  m_dwCardHSpace=16;//DEFAULT_PELS;

  m_wBankerUser = INVALID_CHAIR;

  ZeroMemory(m_bStopCard,sizeof(m_bStopCard));
  ZeroMemory(m_bInsureCard,sizeof(m_bInsureCard));
  ZeroMemory(m_bDoubleCard,sizeof(m_bDoubleCard));
  ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

  m_bTurnBanker = FALSE;
  m_bSpliting = FALSE;
  m_wSplitingUser = INVALID_CHAIR;

  //��ע��Ϣ
  m_lMaxScore=0L;
  m_lCellScore=0L;

  //״̬����
  ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
  ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

  return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��������
bool CGameClientEngine::OnInitGameEngine()
{
  //��������
  HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
  m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

  for(int i=0; i<GAME_PLAYER; i++)
  {
    m_GameClientView.m_CardControl[i].SetCardSpace(m_dwCardHSpace);
  }
  return true;
}

//���ú���
bool CGameClientEngine::OnResetGameEngine()
{
  m_wBankerUser = INVALID_CHAIR;

  ZeroMemory(m_bStopCard,sizeof(m_bStopCard));
  ZeroMemory(m_bInsureCard,sizeof(m_bInsureCard));
  ZeroMemory(m_bDoubleCard,sizeof(m_bDoubleCard));
  ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

  m_bTurnBanker = FALSE;
  m_bSpliting = FALSE;
  m_wSplitingUser = INVALID_CHAIR;

  //��ע��Ϣ
  m_lMaxScore=0L;
  m_lCellScore=0L;

  //״̬����
  ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
  ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
  return true;
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
    case IDI_START_GAME:    //��ʼ��ʱ��
    {
      //��ֹ�ж�
      if(nElapse==0)
      {

        AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
        return false;
      }

      //����֪ͨ
      if(nElapse<=5)
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("START_WARN"));
      }

      return true;
    }
    case IDI_USER_OPERATE:  //��ע��ʱ��
    {
      //��ȡλ��
      WORD wViewChairID=SwitchViewChairID(wChairID);

      //��ֹ�ж�
      if(nElapse==0)
      {
        if((IsLookonMode()==false)&&(wViewChairID==2))
        {
          if(m_cbCardCount[0] > 0)
          {
            OnStopCard(0,0);
          }
          else
          {
            OnAddScore(1,1);
          }
        }
        return false;
      }

      //����֪ͨ
      if((nElapse<=3)&&(wViewChairID==2)&&(IsLookonMode()==false))
      {
        PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
        return true;
      }

      return true;
    }
  }

  return false;
}

//�Թ���Ϣ
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
  return true;
}

//��Ϸ��Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
  switch(wSubCmdID)
  {
    case SUB_S_GAME_START:  //��Ϸ��ʼ
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      //��Ϣ����
      return OnSubGameStart(pData,wDataSize);
    }
    case SUB_S_ADD_SCORE: //��ע
    {
      return OnSubAddScore(pData,wDataSize);
    }
    case SUB_S_STOP_CARD: //ͣ��
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      //��Ϣ����
      return OnSubStopCard(pData,wDataSize);
    }
    case SUB_S_DOUBLE_SCORE:    //�ӱ�
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      //��Ϣ����
      return OnSubDoubleCard(pData,wDataSize);
    }
    case SUB_S_SPLIT_CARD:  //����
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      //��Ϣ����
      return OnSubSplitCard(pData,wDataSize);
    }
    case SUB_S_INSURE:    //����
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      //��Ϣ����
      return OnSubInsureCard(pData,wDataSize);
    }
    case SUB_S_GET_CARD:  //Ҫ��
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      //��Ϣ����
      return OnSubGetCard(pData,wDataSize);
    }
    case SUB_S_SEND_CARD: //������Ϣ
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      //��Ϣ����
      return OnSubSendCard(pData,wDataSize);
    }
    case SUB_S_GAME_END:  //��Ϸ����
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      //��Ϣ����
      return OnSubGameEnd(pData,wDataSize);
    }
    case SUB_S_CHEAT_CARD:
    {
      //��������
      m_GameClientView.FinishDispatchCard();

      return OnSubGMCard(pData,wDataSize);
    }
    case SUB_S_ADMIN_STORAGE_INFO:  //����ͻ�����Ϣ
    {
      ASSERT(wDataSize == sizeof(CMD_S_ADMIN_STORAGE_INFO));
      if(wDataSize != sizeof(CMD_S_ADMIN_STORAGE_INFO))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_ADMIN_STORAGE_INFO *pStorage = (CMD_S_ADMIN_STORAGE_INFO *)pData;
      m_GameClientView.m_pClientControlDlg->SetStorageInfo(pStorage);
      return true;
    }
    case SUB_S_REQUEST_QUERY_RESULT:
    {
      ASSERT(wDataSize == sizeof(CMD_S_RequestQueryResult));
      if(wDataSize != sizeof(CMD_S_RequestQueryResult))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_RequestQueryResult *pQueryResult = (CMD_S_RequestQueryResult *)pData;
      m_GameClientView.m_pClientControlDlg->RequestQueryResult(pQueryResult);

      return true;
    }
    case SUB_S_USER_CONTROL:
    {
      ASSERT(wDataSize == sizeof(CMD_S_UserControl));
      if(wDataSize != sizeof(CMD_S_UserControl))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_UserControl *pUserControl = (CMD_S_UserControl *)pData;
      m_GameClientView.m_pClientControlDlg->RoomUserControlResult(pUserControl);

      return true;
    }
    case SUB_S_USER_CONTROL_COMPLETE:
    {
      ASSERT(wDataSize == sizeof(CMD_S_UserControlComplete));
      if(wDataSize != sizeof(CMD_S_UserControlComplete))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_UserControlComplete *pUserControlComplete = (CMD_S_UserControlComplete *)pData;
      m_GameClientView.m_pClientControlDlg->UserControlComplete(pUserControlComplete);

      return true;
    }
    case SUB_S_OPERATION_RECORD:
    {
      ASSERT(wDataSize == sizeof(CMD_S_Operation_Record));
      if(wDataSize != sizeof(CMD_S_Operation_Record))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_Operation_Record *pOperation_Record = (CMD_S_Operation_Record *)pData;
      m_GameClientView.m_pClientControlDlg->UpdateOperationRecord(pOperation_Record);

      return true;
    }
    case SUB_S_REQUEST_UDPATE_ROOMINFO_RESULT:
    {
      ASSERT(wDataSize == sizeof(CMD_S_RequestUpdateRoomInfo_Result));
      if(wDataSize != sizeof(CMD_S_RequestUpdateRoomInfo_Result))
      {
        return false;
      }
      //�ж�Ȩ��
      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) == false
         ||NULL == m_GameClientView.m_hInst)
      {
        return true;
      }

      CMD_S_RequestUpdateRoomInfo_Result *pRoomInfo_Result = (CMD_S_RequestUpdateRoomInfo_Result *)pData;
      m_GameClientView.m_pClientControlDlg->UpdateRoomInfoResult(pRoomInfo_Result);

      return true;
    }
  }
  ASSERT(FALSE);
  return false;
}

//������Ϣ
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
  switch(cbGameStatus)
  {
    case GAME_SCENE_FREE:   //����״̬
    {
      //Ч������
      if(wDataSize!=sizeof(CMD_S_StatusFree))
      {
        return false;
      }
      CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

      //���ÿؼ�
      if(IsLookonMode()==false&&GetMeUserItem()->GetUserStatus()!=US_READY)
      {
        m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
        m_GameClientView.m_btStart.SetFocus();
        //����ʱ��
        SetGameClock(GetMeChairID(),IDI_START_GAME,30);
      }
      else
      {
        m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
      }

      if(IsLookonMode()==false)
      {
        //������
        m_GameClientView.m_CardControl[4].SetPositively(true);
        m_GameClientView.m_CardControl[5].SetPositively(true);
      }

      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst)
      {
        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

        //���ø��¶�ʱ��
        m_GameClientView.m_pClientControlDlg->SetUpdateIDI();

        //���÷�����
        m_GameClientView.m_pClientControlDlg->SetRoomStorage(pStatusFree->lRoomStorageStart, pStatusFree->lRoomStorageCurrent);
      }

      return true;
    }
    case GAME_SCENE_ADD_SCORE:  //��Ϸ״̬
    {
      //Ч������
      if(wDataSize != sizeof(CMD_S_StatusAddScore))
      {
        return false;
      }
      CMD_S_StatusAddScore *pAddScoreStatus = (CMD_S_StatusAddScore *)pData;

      //���ñ���
      m_lCellScore = pAddScoreStatus->lCellScore;
      m_lMaxScore = pAddScoreStatus->lMaxScore;
      m_wBankerUser = pAddScoreStatus->wBankerUser;

      SetGameStatus(GAME_SCENE_ADD_SCORE);

      //���õȴ�״̬
      m_GameClientView.SetWaitStatus(1);

      //���õ�Ԫ����
      m_GameClientView.SetCellScore(m_lCellScore);

      m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        WORD wViewId = SwitchViewChairID(i);
        //���ó���
        m_GameClientView.m_PlayerJeton[wViewId].SetScore(pAddScoreStatus->lTableScore[i]);
      }

      //������
      if(!IsLookonMode())
      {
        m_GameClientView.m_CardControl[4].SetPositively(true);
        m_GameClientView.m_CardControl[5].SetPositively(true);
      }

      IClientUserItem *pUserItem=NULL;
      tagUserInfo * pUserData=NULL;
      //�ʺ�����
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        pUserItem=GetTableUserItem(i);
        if(pUserItem == NULL)
        {
          continue;
        }
        pUserData=pUserItem->GetUserInfo();
        if(pUserData != NULL)
        {
          m_cbPlayStatus[i] = TRUE;
        }
        if(pUserData!=NULL)
        {
          lstrcpyn(m_szAccounts[i],pUserData->szNickName,CountArray(m_szAccounts[i]));
        }
      }

      //���²����ؼ�
      if(!IsLookonMode() && pAddScoreStatus->lTableScore[GetMeChairID()] == 0L && m_wBankerUser != GetMeChairID())
      {
        m_GameClientView.m_GoldControl.SetScoreControl(m_lMaxScore/m_lCellScore,1,1);
        SetGameClock(GetMeChairID(),IDI_USER_OPERATE,TIME_USER_OPERATE);
      }

      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst)
      {
        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

        //���ø��¶�ʱ��
        m_GameClientView.m_pClientControlDlg->SetUpdateIDI();

        //���÷�����
        m_GameClientView.m_pClientControlDlg->SetRoomStorage(pAddScoreStatus->lRoomStorageStart, pAddScoreStatus->lRoomStorageCurrent);
      }

      return true;
    }
    case GAME_SCENE_GET_CARD:
    {
      //Ч������
      if(wDataSize != sizeof(CMD_S_StatusGetCard))
      {
        return false;
      }
      CMD_S_StatusGetCard *pGetCardStatus = (CMD_S_StatusGetCard *)pData;

      WORD wMeChairId = GetMeChairID();

      //���ñ���
      m_lCellScore = pGetCardStatus->lCellScore;
      m_wBankerUser = pGetCardStatus->wBankerUser;
      CopyMemory(m_bStopCard,pGetCardStatus->bStopCard,sizeof(pGetCardStatus->bStopCard));
      m_bInsureCard[0] = pGetCardStatus->bInsureCard[wMeChairId*2];
      m_bInsureCard[1] = pGetCardStatus->bInsureCard[wMeChairId*2+1];
      m_bDoubleCard[0] = pGetCardStatus->bDoubleCard[wMeChairId*2];
      m_bDoubleCard[1] = pGetCardStatus->bDoubleCard[wMeChairId*2+1];
      m_cbCardCount[0] = pGetCardStatus->cbCardCount[wMeChairId*2];
      m_cbCardCount[1] = pGetCardStatus->cbCardCount[wMeChairId*2+1];
      CopyMemory(m_cbHandCardData[0],pGetCardStatus->cbHandCardData[wMeChairId*2],sizeof(m_cbHandCardData[0]));
      CopyMemory(m_cbHandCardData[1],pGetCardStatus->cbHandCardData[wMeChairId*2+1],sizeof(m_cbHandCardData[0]));

      SetGameStatus(GAME_SCENE_GET_CARD);
      IClientUserItem *pUserItem = NULL ;
      tagUserInfo * pUserData = NULL;
      //�ʺ�����
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        pUserItem = GetTableUserItem(i);
        if(pUserItem == NULL)
        {
          continue;
        }
        pUserData=pUserItem->GetUserInfo();
        if(pUserData != NULL)
        {
          m_cbPlayStatus[i] = TRUE;
        }
        if(pUserData!=NULL)
        {
          lstrcpyn(m_szAccounts[i],pUserData->szNickName,CountArray(m_szAccounts[i]));
        }
      }

      //���õȴ�״̬
      bool bOperCardComplete = true;
      for(WORD i = 0; i < GAME_PLAYER; i ++)
      {
        if(!m_cbPlayStatus[i] || i == m_wBankerUser)
        {
          continue;
        }
        if(!m_bStopCard[i*2] || (pGetCardStatus->cbCardCount[i*2+1]>0&&!m_bStopCard[i*2+1]))
        {
          bOperCardComplete = false;
          break;
        }
      }
      if(bOperCardComplete)
      {
        m_bTurnBanker = TRUE;
      }
      m_GameClientView.SetWaitStatus(bOperCardComplete?3:2);

      //���õ�Ԫ����
      m_GameClientView.SetCellScore(m_lCellScore);

      m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

      //�����˿˿ؼ�
      if(!IsLookonMode())
      {
        m_GameClientView.m_CardControl[2*2].SetPositively(true);
        m_GameClientView.m_CardControl[2*2+1].SetPositively(true);
      }

      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        if(!m_cbPlayStatus[i])
        {
          continue;
        }
        WORD wViewId = SwitchViewChairID(i);
        //�����˿�
        m_GameClientView.m_CardControl[wViewId*2].SetCardData(pGetCardStatus->cbHandCardData[i*2],pGetCardStatus->cbCardCount[i*2]);
        m_GameClientView.m_CardControl[wViewId*2+1].SetCardData(pGetCardStatus->cbHandCardData[i*2+1],pGetCardStatus->cbCardCount[i*2+1]);
        //���ó���
        m_GameClientView.m_PlayerJeton[wViewId].SetScore(pGetCardStatus->lTableScore[i]);
        //��������ֵ
        BYTE cbCardType = 0;
        if(i == wMeChairId && !IsLookonMode())
        {
          cbCardType = m_GameLogic.GetCardType(pGetCardStatus->cbHandCardData[i*2],pGetCardStatus->cbCardCount[i*2],m_cbCardCount[1]>0);
        }
        else
        {
          cbCardType = m_GameLogic.GetCardType(&pGetCardStatus->cbHandCardData[i*2][1],pGetCardStatus->cbCardCount[i*2]-1,true);
        }
        m_GameClientView.SetUserCardType(wViewId,0,cbCardType);
        if(pGetCardStatus->cbCardCount[i*2+1] > 0)
        {
          if(i == wMeChairId && !IsLookonMode())
          {
            cbCardType = m_GameLogic.GetCardType(pGetCardStatus->cbHandCardData[i*2+1],pGetCardStatus->cbCardCount[i*2+1],true);
          }
          else
          {
            cbCardType = m_GameLogic.GetCardType(&pGetCardStatus->cbHandCardData[i*2+1][1],pGetCardStatus->cbCardCount[i*2+1]-1,true);
          }
          m_GameClientView.SetUserCardType(wViewId,0,cbCardType);
        }
        //���üӱ�,����,ͣ��
        if(pGetCardStatus->bDoubleCard[i*2])
        {
          m_GameClientView.SetUserDoubleCard(wViewId);
        }
        if(pGetCardStatus->bInsureCard[i*2])
        {
          m_GameClientView.SetUserInsureCard(wViewId,0);
        }
        if(pGetCardStatus->bStopCard[i*2])
        {
          m_GameClientView.SetUserStopCard(wViewId);
        }
        if(pGetCardStatus->bDoubleCard[i*2+1])
        {
          m_GameClientView.SetUserDoubleCard(wViewId);
        }
        if(pGetCardStatus->bInsureCard[i*2+1])
        {
          m_GameClientView.SetUserInsureCard(wViewId,1);
        }
        if(pGetCardStatus->bStopCard[i*2+1])
        {
          m_GameClientView.SetUserStopCard(wViewId);
        }
      }

      //���²����ؼ�
      if(!IsLookonMode() && ((bOperCardComplete&&wMeChairId==m_wBankerUser) ||
                             (!m_bStopCard[wMeChairId*2] || (m_cbCardCount[1]>0&&!m_bStopCard[wMeChairId*2+1])))
        )
      {
        UpdateScoreControl();
        SetGameClock(GetMeChairID(),IDI_USER_OPERATE,TIME_USER_OPERATE);
      }

      if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight) && m_GameClientView.m_hInst)
      {
        m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

        //���ø��¶�ʱ��
        m_GameClientView.m_pClientControlDlg->SetUpdateIDI();

        //���÷�����
        m_GameClientView.m_pClientControlDlg->SetRoomStorage(pGetCardStatus->lRoomStorageStart, pGetCardStatus->lRoomStorageCurrent);
      }

      return true;
    }
  }

  return false;
}

//Ҫ��
bool CGameClientEngine::OnSubGetCard(const void *pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize != sizeof(CMD_S_GetCard))
  {
    return false;
  }
  CMD_S_GetCard *pGetCard = (CMD_S_GetCard *)pBuffer;

  //�����Լ�Ҫ��
  WORD wMeChairId = GetMeChairID();
  if(wMeChairId == pGetCard->wGetCardUser)
  {
    WORD wIndex = m_bStopCard[wMeChairId*2]?1:0;
    m_cbHandCardData[wIndex][m_cbCardCount[wIndex]++] = pGetCard->cbCardData;
  }

  //����
  BYTE cbCardData = pGetCard->cbCardData;
  WORD wDispatchId = SwitchViewChairID(pGetCard->wGetCardUser)*2;
  if(m_bStopCard[pGetCard->wGetCardUser*2])
  {
    wDispatchId++;
  }

  BYTE cbCard[MAX_COUNT],cbCardCount;
  cbCardCount = (BYTE)m_GameClientView.m_CardControl[wDispatchId].GetCardData(cbCard,MAX_COUNT);

  m_GameClientView.DispatchUserCard(wDispatchId,cbCardData);

  //��������ֵ
  cbCard[cbCardCount++] = pGetCard->cbCardData;
  BYTE cbCardType = 0;
  if(pGetCard->wGetCardUser == wMeChairId && !IsLookonMode())
  {
    cbCardType = m_GameLogic.GetCardType(cbCard,cbCardCount,m_cbCardCount[1]>0);
  }
  else
  {
    cbCardType = m_GameLogic.GetCardType(&cbCard[1],cbCardCount-1,true);
  }
  m_GameClientView.SetUserCardType(SwitchViewChairID(pGetCard->wGetCardUser),m_bStopCard[pGetCard->wGetCardUser*2]?1:0,cbCardType);

  return true;
}

//������Ϣ
bool CGameClientEngine::OnSubInsureCard(const void *pBuffer, WORD wDataSize)
{
  //Ч��
  if(wDataSize != sizeof(CMD_S_Insure))
  {
    return false;
  }
  CMD_S_Insure *pInsureCard = (CMD_S_Insure *)pBuffer;

  //���ñ���
  WORD wMeChairId = GetMeChairID();
  if(wMeChairId == pInsureCard->wInsureUser)
  {
    if(!m_bStopCard[wMeChairId*2])
    {
      m_bInsureCard[0] = TRUE;
    }
    else
    {
      m_bInsureCard[1] = TRUE;
    }
  }

  //���ó���
  WORD wViewId = SwitchViewChairID(pInsureCard->wInsureUser);
  m_GameClientView.m_PlayerJeton[wViewId].AddScore(pInsureCard->lInsureScore);

  //��������
  if(IsLookonMode() || GetMeChairID() != pInsureCard->wInsureUser)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
  }

  //���ý���
  m_GameClientView.SetUserInsureCard(wViewId,m_bStopCard[pInsureCard->wInsureUser*2]?1:0);

  //���²����ؼ�
  if(GetMeChairID() == pInsureCard->wInsureUser && !IsLookonMode())
  {
    UpdateScoreControl();
    //���ö�ʱ��
    SetGameClock(GetMeChairID(),IDI_USER_OPERATE,TIME_USER_OPERATE);
  }

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSplitCard(const void *pBuffer, WORD wDataSize)
{
  //Ч��
  if(wDataSize != sizeof(CMD_S_SplitCard))
  {
    return false;
  }
  CMD_S_SplitCard *pSplitCard = (CMD_S_SplitCard *)pBuffer;

  //���ó���
  WORD wViewId = SwitchViewChairID(pSplitCard->wSplitUser);
  m_GameClientView.m_PlayerJeton[wViewId].AddScore(pSplitCard->lAddScore);

  //��������
  if(IsLookonMode() || GetMeChairID() != pSplitCard->wSplitUser)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
  }

  //�����Լ�����
  WORD wMeChairId = GetMeChairID();
  if(pSplitCard->wSplitUser == wMeChairId)
  {
    ASSERT(m_cbCardCount[0] == 2 && m_cbCardCount[1] == 0);
    m_cbHandCardData[1][1] = m_cbHandCardData[0][1];
    m_cbHandCardData[0][0] = pSplitCard->cbCardData[0];
    m_cbHandCardData[1][0] = pSplitCard->cbCardData[1];
    m_cbCardCount[0] = m_cbCardCount[1] = 2;
  }

  //�������ڷ���
  m_bSpliting = TRUE;
  m_wSplitingUser = pSplitCard->wSplitUser;

  //�����˿�
  BYTE cbCardData[MAX_COUNT],cbCardCount;
  cbCardCount = (BYTE)m_GameClientView.m_CardControl[wViewId*2].GetCardData(cbCardData,MAX_COUNT);
  m_GameClientView.m_CardControl[wViewId*2].SetCardData(&cbCardData[1],1);
  m_GameClientView.m_CardControl[wViewId*2+1].SetCardData(&cbCardData[1],1);
  m_GameClientView.m_CardControl[wViewId*2].SetDisplayHead(true);
  m_GameClientView.m_CardControl[wViewId*2+1].SetDisplayHead(true);
  if(wMeChairId == pSplitCard->wSplitUser)
  {
    m_GameClientView.m_CardControl[wViewId*2].SetPositively(false);
    m_GameClientView.m_CardControl[wViewId*2+1].SetPositively(false);
  }
  //����
  m_GameClientView.DispatchUserCard(wViewId*2,cbCardData[0]);
  m_GameClientView.DispatchUserCard(wViewId*2+1,cbCardData[0]);

  //��������ֵ
  if(wViewId == 2 && !IsLookonMode())
  {
    BYTE cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[0],2,true);
    m_GameClientView.SetUserCardType(wViewId,0,cbCardType);
    cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[1],2,true);
    m_GameClientView.SetUserCardType(wViewId,1,cbCardType);
  }
  else
  {
    BYTE cbCardType = m_GameLogic.GetCardType(&cbCardData[1],1,true);
    m_GameClientView.SetUserCardType(wViewId,1,cbCardType);
  }

  //������±���,��ڶ�����Ҳ���ű���
  if(pSplitCard->bInsured)
  {
    m_GameClientView.SetUserInsureCard(wViewId,1);
  }

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  return true;
}

//�ӱ���Ϣ
bool CGameClientEngine::OnSubDoubleCard(const void *pBuffer, WORD wDataSize)
{
  //Ч��
  if(wDataSize != sizeof(CMD_S_DoubleScore))
  {
    return false;
  }
  CMD_S_DoubleScore *pDoubleScore = (CMD_S_DoubleScore *)pBuffer;

  //���ó���
  WORD wViewId = SwitchViewChairID(pDoubleScore->wDoubleScoreUser);
  m_GameClientView.m_PlayerJeton[wViewId].AddScore(pDoubleScore->lAddScore);

  //��������
  if(IsLookonMode() || GetMeChairID() != pDoubleScore->wDoubleScoreUser)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
  }

  //�����Լ��ӱ�
  WORD wMeChairId = GetMeChairID();
  if(wMeChairId == pDoubleScore->wDoubleScoreUser)
  {
    WORD wIndex = m_bStopCard[wMeChairId*2]?1:0;
    m_cbHandCardData[wIndex][m_cbCardCount[wIndex]++] = pDoubleScore->cbCardData;
  }

  //���üӱ�
  if(wMeChairId == pDoubleScore->wDoubleScoreUser)
  {
    if(!m_bDoubleCard[0])
    {
      m_bDoubleCard[0] = TRUE;
    }
    else
    {
      m_bDoubleCard[1] = TRUE;
    }
  }
  m_GameClientView.SetUserDoubleCard(wViewId);

  //�����˿�
  WORD wOperIndex = 0;
  if(m_bStopCard[pDoubleScore->wDoubleScoreUser*2])
  {
    wOperIndex++;
  }
  ASSERT(!m_bStopCard[pDoubleScore->wDoubleScoreUser*2+wOperIndex]);
  BYTE cbCardData[MAX_COUNT],cbCardCount;
  cbCardCount = (BYTE)m_GameClientView.m_CardControl[wViewId*2+wOperIndex].GetCardData(cbCardData,MAX_COUNT);
  cbCardData[cbCardCount++] = pDoubleScore->cbCardData;
  m_GameClientView.DispatchUserCard(wViewId*2+wOperIndex,cbCardData[cbCardCount-1]);

  //��������ֵ
  BYTE cbCardType = 0;
  if(wViewId == 2 && !IsLookonMode())
  {
    cbCardType = m_GameLogic.GetCardType(cbCardData,cbCardCount,wOperIndex>0);
  }
  else
  {
    cbCardType = m_GameLogic.GetCardType(&cbCardData[1],cbCardCount-1,true);
  }
  m_GameClientView.SetUserCardType(wViewId,wOperIndex,cbCardType);

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  return true;
}

//��ע��Ϣ
bool CGameClientEngine::OnSubAddScore(const void *pBuffer, WORD wDataSize)
{
  //Ч��
  if(wDataSize != sizeof(CMD_S_AddScore))
  {
    return false;
  }
  CMD_S_AddScore *pAddScore = (CMD_S_AddScore *)pBuffer;

  //���ó���
  WORD wViewId = SwitchViewChairID(pAddScore->wAddScoreUser);
  m_GameClientView.m_PlayerJeton[wViewId].SetScore(pAddScore->lAddScore);

  //��������
  if(IsLookonMode() || GetMeChairID() != pAddScore->wAddScoreUser)
  {
    PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
  }

  m_GameClientView.InvalidGameView(0,0,0,0);

  return true;
}

//ͣ����Ϣ
bool CGameClientEngine::OnSubStopCard(const void *pBuffer, WORD wDataSize)
{
  //Ч��
  if(wDataSize != sizeof(CMD_S_StopCard))
  {
    return false;
  }
  CMD_S_StopCard *pStopCard = (CMD_S_StopCard *)pBuffer;

  //���ز����ؼ�
  if(pStopCard->wStopCardUser == GetMeChairID())
  {
    HideScoreControl();
  }

  //����ͣ�Ʊ���
  WORD wOperIndex = pStopCard->wStopCardUser*2;
  if(m_bStopCard[wOperIndex])
  {
    m_bStopCard[++wOperIndex] = TRUE;
  }
  else
  {
    m_bStopCard[wOperIndex] = TRUE;
  }

  //���ý���
  WORD wViewId = SwitchViewChairID(pStopCard->wStopCardUser);
  m_GameClientView.SetUserStopCard(wViewId);

  //�Ƿ���������ȫͣ
  WORD wMeChairId = GetMeChairID();
  if(!IsLookonMode() && wMeChairId == pStopCard->wStopCardUser)
  {
    if((wOperIndex%2 == 0) && m_cbCardCount[1] > 0)
    {
      //���²����ؼ�
      UpdateScoreControl();
      //���ö�ʱ��
      SetGameClock(wMeChairId,IDI_USER_OPERATE,TIME_USER_OPERATE);
    }
  }

  //�Ƿ��ֵ�ׯ�Ҳ���
  if(pStopCard->bTurnBanker)
  {
    m_bTurnBanker = TRUE;
    m_GameClientView.SetWaitStatus(3);
    if(wMeChairId == m_wBankerUser)
    {
      ActiveGameFrame();
      UpdateScoreControl();
      SetGameClock(wMeChairId,IDI_USER_OPERATE,TIME_USER_OPERATE);
    }
  }

  //���½���
  m_GameClientView.InvalidGameView(0,0,0,0);

  return true;
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

  //����״̬
  SetGameStatus(GAME_SCENE_ADD_SCORE);

  //��ע����
  m_lMaxScore=pGameStart->lMaxScore;
  m_lCellScore=pGameStart->lCellScore;
  m_wBankerUser = pGameStart->wBankerUser;

  //���ñ���
  LONGLONG lTableScore=0L;
  IClientUserItem *pUserItem=NULL;
  tagUserInfo * pUserData =NULL;
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��ȡ�û�
    pUserItem=GetTableUserItem(i);
    if(pUserItem == NULL)
    {
      continue;
    }
    pUserData = pUserItem->GetUserInfo();
    if(pUserData==NULL)
    {
      continue;
    }

    //��Ϸ��Ϣ
    m_cbPlayStatus[i]=TRUE;

    //�û�����
    lstrcpyn(m_szAccounts[i],pUserData->szNickName,CountArray(m_szAccounts[i]));
  }

  //���������ע
  if(!IsLookonMode() && GetMeChairID() != m_wBankerUser)
  {
    ASSERT(GetMeUserItem()!= NULL);
    const tagUserInfo *pUserData = GetMeUserItem()->GetUserInfo();
    ASSERT(pUserData != NULL);
    m_lMaxScore = __min(m_lMaxScore,pUserData->lScore/3L);
  }

  //���ý���
  m_GameClientView.SetWaitStatus(1);
  m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
  m_GameClientView.SetCellScore(m_lCellScore);
  if(IsLookonMode())
  {
    m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
    m_GameClientView.SetUserStopCard(INVALID_CHAIR);
    m_GameClientView.SetUserDoubleCard(INVALID_CHAIR);
    m_GameClientView.SetUserInsureCard(INVALID_CHAIR,0);
    for(WORD i = 0; i < GAME_PLAYER; i++)
    {
      //�����˿�
      m_GameClientView.m_CardControl[i*2].SetDisplayHead(false);
      m_GameClientView.m_CardControl[i*2+1].SetDisplayHead(false);
      m_GameClientView.m_CardControl[i*2].SetCardData(NULL,0);
      m_GameClientView.m_CardControl[i*2+1].SetCardData(NULL,0);
    }
  }

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

  //������
  if(IsLookonMode()==false&&m_wBankerUser!=GetMeChairID())
  {
    ActiveGameFrame();
    m_GameClientView.m_GoldControl.SetScoreControl(m_lMaxScore/m_lCellScore,1,1);
    SetGameClock(GetMeChairID(),IDI_USER_OPERATE,TIME_USER_OPERATE);
  }

  return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_SendCard))
  {
    return false;
  }
  CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

  //������Ϸ״̬
  SetGameStatus(GAME_SCENE_ADD_SCORE);

  //���ñ���
  WORD wMeChairId = GetMeChairID();
  m_cbCardCount[0] = 2;
  CopyMemory(m_cbHandCardData[0],pSendCard->cbHandCardData[wMeChairId],sizeof(BYTE)*2);

  //���ý���
  m_GameClientView.SetWaitStatus(2);

  //����
  for(BYTE j = 0; j < CountArray(pSendCard->cbHandCardData[0]); j++)
  {
    WORD wId = m_wBankerUser;
    for(BYTE i = 0; i < GAME_PLAYER; i++)
    {
      wId = (wId+1)%GAME_PLAYER;
      if(!m_cbPlayStatus[wId])
      {
        continue;
      }
      m_GameClientView.DispatchUserCard(SwitchViewChairID(wId)*2,pSendCard->cbHandCardData[wId][j]);
    }
  }

  //��������ֵ
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(!m_cbPlayStatus[i])
    {
      continue;
    }
    BYTE cbCardType = 0;
    if(i == GetMeChairID() && !IsLookonMode())
    {
      cbCardType = m_GameLogic.GetCardType(pSendCard->cbHandCardData[i],2,false);
    }
    else
    {
      cbCardType = m_GameLogic.GetCardType(&pSendCard->cbHandCardData[i][1],1,false);
    }
    m_GameClientView.SetUserCardType(SwitchViewChairID(i),0,cbCardType);
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

  //״̬����
  HideScoreControl();
  SetGameStatus(GAME_SCENE_FREE);
  KillGameClock(IDI_USER_OPERATE);

  //��������
  if(IsLookonMode()==false)
  {
    if(pGameEnd->lGameScore[GetMeChairID()]>0L)
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

  //��������
  m_GameClientView.m_ScoreView.ResetScore();
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //���ó���
    m_GameClientView.m_PlayerJeton[i].SetScore(0L);
    //������Ϣ
    if(pGameEnd->lGameScore[i]!=0L||m_szAccounts[i][0]!=0)
    {
      if(m_szAccounts[i][0]==0)
      {
        m_GameClientView.m_ScoreView.SetGameScore(i,TEXT("���뿪"),pGameEnd->lGameScore[i]);
      }
      else
      {
        m_GameClientView.m_ScoreView.SetGameScore(i,m_szAccounts[i],pGameEnd->lGameScore[i]);
      }
      //��˰
      m_GameClientView.m_ScoreView.SetGameTax(i,pGameEnd->lGameTax[i]);
      if(pGameEnd->cbCardData[i*2]!=0)
      {
        //��ȡ�˿�
        BYTE cbCardData[MAX_COUNT];
        WORD wViewChairID=SwitchViewChairID(i);
        BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID*2].GetCardData(cbCardData,CountArray(cbCardData));
        //�����˿�
        cbCardData[0]=pGameEnd->cbCardData[i*2];
        m_GameClientView.m_CardControl[wViewChairID*2].SetDisplayHead(true);
        m_GameClientView.m_CardControl[wViewChairID*2].SetCardData(cbCardData,cbCardCount);
        BYTE cbCardType = m_GameLogic.GetCardType(cbCardData,cbCardCount,
                          m_GameClientView.m_CardControl[wViewChairID*2+1].GetCardCount()>0);
        m_GameClientView.SetUserCardType(wViewChairID,0,cbCardType);

        cbCardCount = (BYTE)m_GameClientView.m_CardControl[wViewChairID*2+1].GetCardData(cbCardData,CountArray(cbCardData));
        if(cbCardCount > 0)
        {
          //�����˿�
          cbCardData[0]=pGameEnd->cbCardData[i*2+1];
          m_GameClientView.m_CardControl[wViewChairID*2+1].SetDisplayHead(true);
          m_GameClientView.m_CardControl[wViewChairID*2+1].SetCardData(cbCardData,cbCardCount);
          BYTE cbCardType = m_GameLogic.GetCardType(cbCardData,cbCardCount,true);
          m_GameClientView.SetUserCardType(wViewChairID,1,cbCardType);
        }
      }
    }
  }
  m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

  //���û���

  //�ɼ���ʾ�ڼ�ʱ����Ի���
  TCHAR szBuffer[256]=TEXT("");
  _sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ֽ���,�ɼ�ͳ��:"));
  m_pIStringMessage->InsertNormalString(szBuffer);
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if(pGameEnd->lGameScore[i]==0)
    {
      continue;
    }

    //�ɼ����
    //IClientUserItem * pUserData=GetTableUserItem(i);
    _sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n%s��%+I64d"),m_szAccounts[i],pGameEnd->lGameScore[i]);
    m_pIStringMessage->InsertNormalString(szBuffer);
  }
  m_pIStringMessage->InsertNormalString(TEXT("\n"));

  //ׯ��
  m_wBankerUser = INVALID_CHAIR;

  ZeroMemory(m_bStopCard,sizeof(m_bStopCard));
  ZeroMemory(m_bInsureCard,sizeof(m_bInsureCard));
  ZeroMemory(m_bDoubleCard,sizeof(m_bDoubleCard));
  ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

  m_bTurnBanker = FALSE;
  m_bSpliting = FALSE;
  m_wSplitingUser = INVALID_CHAIR;

  //��ע��Ϣ
  m_lMaxScore=0L;
  m_lCellScore=0L;

  //״̬����
  ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
  ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

  //��ʼ��ť
  if(IsLookonMode()==false)
  {
    m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
    SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
  }

  return true;
}

bool CGameClientEngine::OnSubGMCard(const void * pData, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_CheatCard));
  if(wDataSize!=sizeof(CMD_S_CheatCard))
  {
    return false;
  }

  //��������
  CMD_S_CheatCard * pCheatCard=(CMD_S_CheatCard *)pData;

  for(WORD i = 0; i < GAME_PLAYER ; i++)
  {
    //��ʾ�˿�
    if(i==GetMeChairID() || m_cbPlayStatus[i] == FALSE)
    {
      continue;
    }

    //�����˿�
    m_GameClientView.m_CardControl[SwitchViewChairID(i)*2].SetCardData(pCheatCard->cbCardData[i*2],2);
    m_GameClientView.m_CardControl[SwitchViewChairID(i)*2].SetDisplayHead(true);
  }

  for(WORD j = 0; j < GAME_PLAYER ; j++)
  {
    if(i==GetMeChairID() || m_cbPlayStatus[i] == FALSE)
    {
      continue;
    }
    if(pCheatCard->cbCardData[j*2+1][0]!=0)
    {
      m_GameClientView.m_CardControl[SwitchViewChairID(j)*2+1].SetCardData(pCheatCard->cbCardData[j*2+1],2);
      m_GameClientView.m_CardControl[SwitchViewChairID(j)*2+1].SetDisplayHead(true);
    }
  }

  m_GameClientView.InvalidGameView(0,0,0,0);
  return true;
}

//���ؿ���
void CGameClientEngine::HideScoreControl()
{
  m_GameClientView.m_GoldControl.ShowWindow(SW_HIDE);
  //���ư�ť
  m_GameClientView.m_btGetCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btStopCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btDoubleCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btSplitCard.ShowWindow(SW_HIDE);
  m_GameClientView.m_btInsureCard.ShowWindow(SW_HIDE);
  return;
}

//���¿���
BOOL CGameClientEngine::UpdateScoreControl()
{
  //Ч��
  if(IsLookonMode())
  {
    return FALSE;
  }

  WORD wMeChairId = GetMeChairID();
  if((!m_bTurnBanker && wMeChairId==m_wBankerUser) || (m_bTurnBanker && wMeChairId!=m_wBankerUser))
  {
    return FALSE;
  }

  if(m_bStopCard[wMeChairId*2] && m_bStopCard[wMeChairId*2+1])
  {
    return FALSE;
  }

  if(m_bDoubleCard[0] && (m_cbCardCount[1]==0||m_bDoubleCard[1]))
  {
    return FALSE;
  }

  WORD wIndex = m_bStopCard[wMeChairId*2]?1:0;
  if(m_cbCardCount[wIndex] == 0)
  {
    return FALSE;
  }
  BYTE cbCardType = m_GameLogic.GetCardType(&m_cbHandCardData[wIndex][1],m_cbCardCount[wIndex]-1,true);
  if(cbCardType == CT_BAOPAI || cbCardType == CT_BJ-1)
  {
    m_GameClientView.m_btStopCard.ShowWindow(SW_HIDE);
    return FALSE;
  }
  if(m_GameClientView.m_btGetCard.IsWindowVisible())
  {
    return FALSE;
  }

  m_GameClientView.m_btStopCard.ShowWindow(SW_SHOW);
  m_GameClientView.m_btStopCard.EnableWindow();

  cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[wIndex],m_cbCardCount[wIndex],false);
  if(cbCardType > CT_BAOPAI && cbCardType < CT_BJ-1 || cbCardType > CT_BJ)
  {
    m_GameClientView.m_btGetCard.ShowWindow(SW_SHOW);
    m_GameClientView.m_btGetCard.EnableWindow();
  }

  if(wMeChairId != m_wBankerUser)
  {
    if(m_cbCardCount[wIndex]==2)
    {
      m_GameClientView.m_btDoubleCard.ShowWindow(SW_SHOW);
      m_GameClientView.m_btDoubleCard.EnableWindow();
    }
    if(m_cbCardCount[1] == 0 && m_cbCardCount[0] == 2 &&
       m_GameLogic.GetCardValue(m_cbHandCardData[0][1]) == m_GameLogic.GetCardValue(m_cbHandCardData[0][0]))
    {
      m_GameClientView.m_btSplitCard.ShowWindow(SW_SHOW);
      m_GameClientView.m_btSplitCard.EnableWindow();
    }
    if(m_cbCardCount[wIndex]==2 && !m_bInsureCard[wIndex])
    {
      BYTE cbCardData[MAX_COUNT],cbCardCount;
      cbCardCount = (BYTE)m_GameClientView.m_CardControl[SwitchViewChairID(m_wBankerUser)*2].GetCardData(cbCardData,MAX_COUNT);
      if(cbCardCount == 2 && m_GameLogic.GetCardValue(cbCardData[1])==1)
      {
        m_GameClientView.m_btInsureCard.ShowWindow(SW_SHOW);
        m_GameClientView.m_btInsureCard.EnableWindow(TRUE);
      }
    }
  }

  return TRUE;
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
  //ɾ����ʱ��
  KillGameClock(IDI_START_GAME);

  //���ý���
  m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
  m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

  //���ý���
  m_GameClientView.SetWaitStatus(0);
  m_GameClientView.SetBankerUser(INVALID_CHAIR);
  m_GameClientView.SetUserStopCard(INVALID_CHAIR);
  m_GameClientView.SetUserDoubleCard(INVALID_CHAIR);
  m_GameClientView.SetUserInsureCard(INVALID_CHAIR,0);
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    //�����˿�
    m_GameClientView.m_CardControl[i*2].SetDisplayHead(false);
    m_GameClientView.m_CardControl[i*2+1].SetDisplayHead(false);
    m_GameClientView.m_CardControl[i*2].SetCardData(NULL,0);
    m_GameClientView.m_CardControl[i*2+1].SetCardData(NULL,0);
  }

  //������Ϣ
  SendUserReady(NULL,0);

  return 0;
}

//ͣ�ư�ť
LRESULT CGameClientEngine::OnStopCard(WPARAM wParam, LPARAM lParam)
{
  //���ؿؼ�
  HideScoreControl();

  //ɾ����ʱ��
  KillGameClock(IDI_USER_OPERATE);

  //��������
  SendSocketData(SUB_C_STOP_CARD);

  return 0;
}

//�ӱ���ť
LRESULT CGameClientEngine::OnDoubleCard(WPARAM wParam, LPARAM lParam)
{
  //���ؿؼ�
  HideScoreControl();

  //ɾ����ʱ��
  KillGameClock(IDI_USER_OPERATE);

  //��������
  SendSocketData(SUB_C_DOUBLE_SCORE);

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

  return 0;
}

//���հ�ť
LRESULT CGameClientEngine::OnInsureCard(WPARAM wParam, LPARAM lParam)
{
  //���ؿؼ�
  HideScoreControl();

  //ɾ����ʱ��
  KillGameClock(IDI_USER_OPERATE);

  //��������
  SendSocketData(SUB_C_INSURE);

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

  return 0;
}

//���ư�ť
LRESULT CGameClientEngine::OnSplitCard(WPARAM wParam, LPARAM lParam)
{
  //���ؿؼ�
  HideScoreControl();

  //ɾ����ʱ��
  KillGameClock(IDI_USER_OPERATE);

  //��������
  SendSocketData(SUB_C_SPLIT_CARD);

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

  return 0;
}

//Ҫ�ư�ť
LRESULT CGameClientEngine::OnGetCard(WPARAM wParam, LPARAM lParam)
{
  //���ؿؼ�
  HideScoreControl();

  //ɾ����ʱ��
  KillGameClock(IDI_USER_OPERATE);

  //��������
  SendSocketData(SUB_C_GET_CARD);

  return 0;
}

//��ע��ť
LRESULT CGameClientEngine::OnAddScore(WPARAM wParam, LPARAM lParam)
{
  //���ؿؼ�
  HideScoreControl();

  //ɾ����ʱ��
  KillGameClock(IDI_USER_OPERATE);

  LONGLONG pTempValue = (LONGLONG)lParam;
  LONGLONG lValue = pTempValue;

  //��������
  CMD_C_AddScore AddScore;
  AddScore.lScore = lValue*m_lCellScore;
  SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

  //��������
  PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

  return 0;
}

//�������
LRESULT CGameClientEngine::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
  //���ƴ���
  if(m_bSpliting)
  {
    m_bSpliting = FALSE;
    WORD wViewId = SwitchViewChairID(m_wSplitingUser)*2;
    m_GameClientView.m_CardControl[wViewId].SetDisplayHead(false);
    m_GameClientView.m_CardControl[wViewId+1].SetDisplayHead(false);
    if(m_wSplitingUser == GetMeChairID())
    {
      m_GameClientView.m_CardControl[wViewId].SetCardData(m_cbHandCardData[0],m_cbCardCount[0]);
      m_GameClientView.m_CardControl[wViewId+1].SetCardData(m_cbHandCardData[1],m_cbCardCount[1]);
      if(!IsLookonMode())
      {
        m_GameClientView.m_CardControl[wViewId].SetPositively(true);
        m_GameClientView.m_CardControl[wViewId+1].SetPositively(true);
      }
    }
    else
    {
      BYTE cbCardData[2];
      m_GameClientView.m_CardControl[wViewId].GetCardData(cbCardData,2);
      cbCardData[1] = cbCardData[0];
      cbCardData[0] = 0;
      m_GameClientView.m_CardControl[wViewId].SetCardData(cbCardData,2);
      m_GameClientView.m_CardControl[wViewId+1].SetCardData(cbCardData,2);
    }
    m_wSplitingUser = INVALID_CHAIR;
  }

  //����״̬
  if(IsLookonMode()==false)
  {
    //��������
    ActiveGameFrame();

    WORD wMeChairId = GetMeChairID();
    //ׯ��һ�����ƻ�BJ��21�㣬���Զ�ͣ��
    if(wMeChairId == m_wBankerUser && m_bTurnBanker)
    {
      BYTE cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[0],m_cbCardCount[0],false);
      if(cbCardType == CT_BAOPAI || cbCardType == CT_BJ || cbCardType == CT_BJ-1)
      {
        OnStopCard(0,0);
        return 0;
      }
    }

    //���¿��ƣ�����ʱ��
    if(UpdateScoreControl())
    {
      SetGameClock(wMeChairId,IDI_USER_OPERATE,TIME_USER_OPERATE);
    }
    ////����ʱ��
    //if( (!m_bDoubleCard[0] || (m_cbCardCount[1]>0&&!m_bDoubleCard[1])) && m_GameClientView.m_btStopCard.IsWindowVisible() &&
    //  ( (m_bTurnBanker&&wMeChairId==m_wBankerUser) || (!m_bTurnBanker&&wMeChairId!=m_wBankerUser) ) )
    //  SetGameTimer(wMeChairId,IDI_USER_OPERATE,TIME_USER_OPERATE);
  }

  return 0;
}

LRESULT CGameClientEngine::OnStorage(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_STORAGE, (CMD_C_UpdateStorage*)wParam, sizeof(CMD_C_UpdateStorage));
  return true;
}

LRESULT CGameClientEngine::OnStorageInfo(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_STORAGEMAXMUL, (CMD_C_ModifyStorage*)wParam, sizeof(CMD_C_ModifyStorage));
  return true;
}

LRESULT CGameClientEngine::OnRequestQueryUser(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_REQUEST_QUERY_USER, (CMD_C_RequestQuery_User*)wParam, sizeof(CMD_C_RequestQuery_User));
  return true;
}

//�û�����
LRESULT CGameClientEngine::OnUserControl(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_USER_CONTROL, (CMD_C_UserControl*)wParam, sizeof(CMD_C_UserControl));
  return true;
}

//������·�����Ϣ
LRESULT CGameClientEngine::OnRequestUpdateRoomInfo(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_REQUEST_UDPATE_ROOMINFO);
  return true;
}

//��յ�ǰ��ѯ�û�
LRESULT CGameClientEngine::OnClearCurrentQueryUser(WPARAM wParam, LPARAM lParam)
{
  SendSocketData(SUB_C_CLEAR_CURRENT_QUERYUSER);
  return true;
}

//////////////////////////////////////////////////////////////////////////////////
