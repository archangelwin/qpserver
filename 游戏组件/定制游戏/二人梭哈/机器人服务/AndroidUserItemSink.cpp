#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS         2                 //����ʱ��

//��Ϸʱ��
#define TIME_USER_CALL_BANKER   2                 //��ׯ��ʱ��
#define TIME_USER_START_GAME    3                 //��ʼ��ʱ��
#define TIME_USER_ADD_SCORE     3                 //��ע��ʱ��
#define TIME_USER_OPEN_CARD     12                  //̯�ƶ�ʱ��

//��Ϸʱ��
#define IDI_START_GAME      (100)     //��ʼʱ��
#define IDI_USER_ADD_SCORE    (102)     //��עʱ��
#define IDI_OPEN_CARD     (103)     //����ʱ��
#define IDI_GIVE_UP       (104)     //����
#define IDI_SHOW_HAND     (105)     //���
#define IDI_FOLLOW        (106)     //����

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
  m_lTurnMaxScore = 0;
  m_lBasicGold=0;
  m_lShowHandGold=0;
  m_lCurrentLessGold=0;
  ZeroMemory(m_HandCardCount,sizeof(m_HandCardCount));
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));


  //�ӿڱ���
  m_pIAndroidUserItem=NULL;
  srand((unsigned)time(NULL));

  return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
  return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
  //��ѯ�ӿ�
  m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
  if(m_pIAndroidUserItem==NULL)
  {
    return false;
  }

  return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
  m_lTurnMaxScore = 0;
  m_lBasicGold=0;
  m_lShowHandGold=0;
  m_lCurrentLessGold=0;
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
  switch(nTimerID)
  {
    case IDI_START_GAME:    //��ʼ��ʱ��
    {
      //����׼��
      m_pIAndroidUserItem->SendUserReady(NULL,0);
      //BankOperate();
      return true;
    }
    case IDI_USER_ADD_SCORE:  //��ע��ʱ
    {
      //����ע����
      LONGLONG lUserMaxScore[4]= {0,m_lBasicGold,m_lBasicGold*2,m_lBasicGold*3};
      //������Ϣ
      CMD_C_AddGold AddScore;
      AddScore.lGold=lUserMaxScore[rand()%4];
      m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_GOLD,&AddScore,sizeof(AddScore));

      return true;
    }
    case IDI_SHOW_HAND:
    {
      //������Ϣ
      CMD_C_AddGold AddScore;
      AddScore.lGold=m_lShowHandGold;
      m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_GOLD,&AddScore,sizeof(AddScore));
      return true;
    }
    case IDI_GIVE_UP:
    {
      m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP);
      return true;
    }
    case IDI_FOLLOW:
    {
      //������Ϣ
      CMD_C_AddGold AddScore;
      AddScore.lGold=m_lCurrentLessGold;
      m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_GOLD,&AddScore,sizeof(AddScore));

      return true;
    }
  }

  return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
  switch(wSubCmdID)
  {
    case SUB_S_GAME_START:  //��Ϸ��ʼ
    {
      //��Ϣ����
      return OnSubGameStart(pData,wDataSize);
    }
    case SUB_S_GIVE_UP: //�û�����
    {
      //��Ϣ����
      return OnSubGiveUp(pData,wDataSize);
    }
    case SUB_S_SEND_CARD: //������Ϣ
    {
      //��Ϣ����
      return OnSubSendCard(pData,wDataSize);
    }
    case SUB_S_GAME_END:  //��Ϸ����
    {
      //��Ϣ����
      return OnSubGameEnd(pData,wDataSize);
    }
    case SUB_S_ALL_CARD:
    {
      return true;
    }
    case SUB_S_ADD_GOLD:
    {
      return OnSubAddGold(pData,wDataSize);
    }
  }

  //�������
  ASSERT(FALSE);

  return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
  return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
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

      //��Ϣ����
      CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

      memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

      ReadConfigInformation();

      BankOperate();

      //��ʼʱ��
      UINT nElapse=rand()%(5)+1;
      m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

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

      memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

      ReadConfigInformation();

      //BankOperate();

      //���ñ���
      m_lTurnMaxScore=pStatusPlay->lTurnMaxGold;
      //WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();

      for(BYTE i=0; i<GAME_PLAYER; i++)
      {
        CopyMemory(m_HandCardData[i],pStatusPlay->bTableCardArray[i],MAX_COUNT);
        m_HandCardCount[i] = pStatusPlay->bTableCardCount[i];
      }


      return true;
    }
  }

  ASSERT(FALSE);

  return false;
}

//�û�����
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û��뿪
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û�����
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û�״̬
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û���λ
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
    return false;
  }
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

  //���ñ���
  m_lTurnMaxScore=pGameStart->lTurnMaxGold;
  m_lBasicGold=pGameStart->lBasicGold;

  //�������
  ZeroMemory(m_HandCardCount,sizeof(m_HandCardCount));
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

  WORD wMeChairID = m_pIAndroidUserItem->GetChairID();
  for(BYTE i=0; i<GAME_PLAYER; i++)
  {
    m_HandCardCount[i] = 2;

    if(wMeChairID == i)
    {
      m_HandCardData[i][0]=pGameStart->bFundusCard;
    }

    m_HandCardData[i][1]=pGameStart->bCardData[i];
  }

  //���ó���
  if(pGameStart->wCurrentUser==m_pIAndroidUserItem->GetChairID())
  {
    //��עʱ��
    UINT nElapse=rand()%(4)+TIME_LESS;
    m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
    return true;
  }

  return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddGold(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_AddGold))
  {
    return false;
  }
  CMD_S_AddGold * pAddScore=(CMD_S_AddGold *)pBuffer;
  m_lShowHandGold=pAddScore->lShowHandGold;
  m_lCurrentLessGold=pAddScore->lCurrentLessGold;


  WORD wMeChairID = m_pIAndroidUserItem->GetChairID();
  if(pAddScore->wCurrentUser==m_pIAndroidUserItem->GetChairID()/*&&!pAddScore->bShowHand*//*&&m_lCurrentLessGold!=0*/)
  {
    //�������
    bool bShowHand = false;
    if(m_GameLogic.GetCardGenre(m_HandCardData[wMeChairID],m_HandCardCount[wMeChairID])>=CT_TWO_DOUBLE)
    {
      bShowHand = true;
    }

    if(m_GameLogic.GetCardGenre(m_HandCardData[wMeChairID],m_HandCardCount[wMeChairID])==CT_ONE_DOUBLE &&
       m_HandCardCount[wMeChairID] == 3)
    {
      BYTE bCardCount[15]= {0};
      for(BYTE i=0; i<3; i++)
      {
        BYTE bCardValue = m_GameLogic.GetCardLogicValue(m_HandCardData[wMeChairID][i]);
        bCardCount[bCardValue] ++;
      }

      BYTE bDoubleCardValue = 0;
      for(BYTE i=0; i<15; i++)
      {
        if(bCardCount[i] == 2)
        {
          bDoubleCardValue = i;
        }
      }

      if(bDoubleCardValue>=12)
      {
        for(BYTE i=0; i<GAME_PLAYER; i++)
        {
          if(wMeChairID == i)
          {
            continue;
          }

          if(m_GameLogic.GetCardLogicValue(m_HandCardData[i][1]) == m_GameLogic.GetCardLogicValue(m_HandCardData[i][2]) &&
             m_GameLogic.GetCardLogicValue(m_HandCardData[i][1]) > 12)
          {
            bShowHand = false;
          }
          else
          {
            bShowHand = true;
          }
        }
      }
      else
      {
        bShowHand = false;
      }
    }

    if(bShowHand&&pAddScore->bCanShowHand/*&&(pAddScore->bAndroidLose == false)*/)
    {
      //��עʱ��
      UINT nElapse=rand()%5+TIME_LESS;
      m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_HAND,nElapse);

      return true;
    }

    if(pAddScore->bShowHand && pAddScore->bNoGiveUp)
    {
      if(m_HandCardCount[wMeChairID] > 3)
      {
        //��עʱ��
        UINT nElapse=rand()%(5)+TIME_LESS;
        m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_HAND,nElapse);

        return true;
      }
      else
      {
        for(BYTE i=0; i<GAME_PLAYER; i++)
        {
          if(wMeChairID == i)
          {
            continue;
          }

          if(!(m_GameLogic.GetCardLogicValue(m_HandCardData[i][1]) == m_GameLogic.GetCardLogicValue(m_HandCardData[i][2]) &&
               m_GameLogic.GetCardLogicValue(m_HandCardData[i][1]) > 12))
          {
            //��עʱ��
            UINT nElapse=rand()%(5)+TIME_LESS;
            m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_HAND,nElapse);

            return true;
          }
        }
      }
      return true;
    }

    if(m_lCurrentLessGold==0)
    {
      //��עʱ��
      UINT nElapse=rand()%(4)+TIME_LESS;
      m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
      return true;
    }
    else
    {
      if(pAddScore->bNoGiveUp && !pAddScore->bShowHand)
      {
        //��עʱ��
        UINT nElapse=rand()%(4)+TIME_LESS;
        m_pIAndroidUserItem->SetGameTimer(IDI_FOLLOW,nElapse);

      }
      else
      {
        //��עʱ��
        UINT nElapse=rand()%5+TIME_LESS;
        m_pIAndroidUserItem->SetGameTimer(IDI_GIVE_UP,nElapse);
      }
    }

    return true;
  }



  if(/*pAddScore->bNoGiveUp==false&&*/pAddScore->wCurrentUser==m_pIAndroidUserItem->GetChairID())
  {
    //��עʱ��
    UINT nElapse=rand()%5+TIME_LESS;
    m_pIAndroidUserItem->SetGameTimer(IDI_GIVE_UP,nElapse);
    return true;
  }
  //if (pAddScore->bShowHand&&pAddScore->wCurrentUser==m_pIAndroidUserItem->GetChairID()&&pAddScore->bNoGiveUp)
  //{
  //  //��עʱ��
  //  UINT nElapse=rand()%(5)+TIME_LESS;
  //  m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_HAND,nElapse);
  //  return true;
  //}

  return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{

  //Ч������
  if(wDataSize!=sizeof(CMD_R_SendCard))
  {
    return false;
  }
  CMD_R_SendCard * pSendCard=(CMD_R_SendCard *)pBuffer;

  //��������
  WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();

  //�����˿�
  for(WORD i=0; i<pSendCard->cbSendCardCount; i++)
  {
    if(m_HandCardCount[wMeChiarID]>MAX_COUNT)
    {
      break;
    }

    m_HandCardData[wMeChiarID][m_HandCardCount[wMeChiarID]++] =pSendCard->bUserCard[wMeChiarID][i];
  }

  if(pSendCard->wCurrentUser==wMeChiarID)
  {
    //�������
    bool bShowHand = false;
    if(m_GameLogic.GetCardGenre(m_HandCardData[wMeChiarID],m_HandCardCount[wMeChiarID])>=CT_TWO_DOUBLE)
    {
      bShowHand = true;
    }

    if(m_GameLogic.GetCardGenre(m_HandCardData[wMeChiarID],m_HandCardCount[wMeChiarID])==CT_ONE_DOUBLE &&
       m_HandCardCount[wMeChiarID] == 3)
    {
      BYTE bCardCount[15]= {0};
      for(BYTE i=0; i<3; i++)
      {
        BYTE bCardValue = m_GameLogic.GetCardLogicValue(m_HandCardData[wMeChiarID][i]);
        bCardCount[bCardValue] ++;
      }

      BYTE bDoubleCardValue = 0;
      for(BYTE i=0; i<15; i++)
      {
        if(bCardCount[i] == 2)
        {
          bDoubleCardValue = i;
        }
      }

      if(bDoubleCardValue>=12)
      {
        for(BYTE i=0; i<GAME_PLAYER; i++)
        {
          if(wMeChiarID == i)
          {
            continue;
          }

          if(m_GameLogic.GetCardLogicValue(m_HandCardData[i][1]) == m_GameLogic.GetCardLogicValue(m_HandCardData[i][2]) &&
             m_GameLogic.GetCardLogicValue(m_HandCardData[i][1]) > 12)
          {
            bShowHand = false;
          }
          else
          {
            bShowHand = true;
          }
        }
      }
      else
      {
        bShowHand = false;
      }
    }


    if(bShowHand && pSendCard->bCanShowHand/* &&(pSendCard->bAndroidLose == false)*/)
    {
      m_lShowHandGold=pSendCard->lShowHandGold;
      //��עʱ��
      UINT nElapse=rand()%(4)+TIME_LESS;
      m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_HAND,nElapse);

      return true;
    }

    if(pSendCard->bNoGiveUp)
    {
      //��עʱ��
      UINT nElapse=rand()%(4)+TIME_LESS;
      m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);

    }
    else
    {
      //��עʱ��
      UINT nElapse=rand()%(4)+TIME_LESS;
      m_pIAndroidUserItem->SetGameTimer(IDI_GIVE_UP,nElapse);
    }

    return true;
  }

  return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
  //Ч�����
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

  //ɾ����ʱ��
  m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
  m_pIAndroidUserItem->KillGameTimer(IDI_GIVE_UP);
  m_pIAndroidUserItem->KillGameTimer(IDI_SHOW_HAND);
  m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);
  m_pIAndroidUserItem->KillGameTimer(IDI_FOLLOW);

  m_lTurnMaxScore = 0;
  m_lBasicGold=0;
  m_lShowHandGold=0;
  m_lCurrentLessGold=0;

  //��ʼʱ��
  UINT nElapse=rand()%(5)+TIME_LESS;
  m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

  //�������
  m_lTurnMaxScore = 0;
  ZeroMemory(m_HandCardData,sizeof(m_HandCardData));


  return true;
}

//����
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
  //Ч������
  if(wDataSize!=sizeof(CMD_S_GiveUp))
  {
    return false;
  }
  CMD_S_GiveUp * pGameStart=(CMD_S_GiveUp *)pBuffer;

  return true;
}

//���в���
void CAndroidUserItemSink::BankOperate()
{
  //��������
  IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
  LONGLONG lRobotScore = pUserItem->GetUserScore();
  {
    //�жϴ�ȡ
    if(lRobotScore > m_lRobotScoreRange[1])
    {


      LONGLONG lSaveScore=0L;

      lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
      if(lSaveScore > lRobotScore)
      {
        lSaveScore = lRobotScore;
      }

      if(lSaveScore > 0)
      {
        m_pIAndroidUserItem->PerformSaveScore(lSaveScore);
      }

      LONGLONG lRobotNewScore = pUserItem->GetUserScore();

    }
    else if(lRobotScore < m_lRobotScoreRange[0])
    {
      BYTE RandCount[10]= {10,20,30,40,50,60,70,80,90,100};
      int Index=rand()%10;
      SCORE lScore=(m_lRobotBankGetScoreBanker*RandCount[Index]/100)+m_lRobotBankGetScore;
      if(lScore > 0)
      {
        m_pIAndroidUserItem->PerformTakeScore(lScore);
      }

      LONGLONG lRobotNewScore = pUserItem->GetUserScore();

    }
  }
}

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation()
{
  //�����ļ���
  TCHAR szPath[MAX_PATH] = TEXT("");
  TCHAR szConfigFileName[MAX_PATH] = TEXT("");
  TCHAR OutBuf[255] = TEXT("");
  GetCurrentDirectory(sizeof(szPath), szPath);
  myprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\ShowHandExServer.ini"), szPath);

  //��������
  ZeroMemory(OutBuf, sizeof(OutBuf));
  GetPrivateProfileString(m_szRoomName, TEXT("RobotScoreMin"), _T("10000"), OutBuf, 255, szConfigFileName);
  myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[0]);

  ZeroMemory(OutBuf, sizeof(OutBuf));
  GetPrivateProfileString(m_szRoomName, TEXT("RobotScoreMax"), _T("1000000"), OutBuf, 255, szConfigFileName);
  myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[1]);

  if(m_lRobotScoreRange[1] < m_lRobotScoreRange[0])
  {
    m_lRobotScoreRange[1] = m_lRobotScoreRange[0];
  }

  //�������

  ZeroMemory(OutBuf, sizeof(OutBuf));
  GetPrivateProfileString(m_szRoomName, TEXT("RobotBankGet"), _T("20000000"), OutBuf, 255, szConfigFileName);
  myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScore);

  //������� (ׯ��)
  ZeroMemory(OutBuf, sizeof(OutBuf));
  GetPrivateProfileString(m_szRoomName, TEXT("RobotBankGetBanker"), _T("30000000"), OutBuf, 255, szConfigFileName);
  myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScoreBanker);

  //����
  m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

  if(m_nRobotBankStorageMul<0||m_nRobotBankStorageMul>100)
  {
    m_nRobotBankStorageMul =20;
  }
}


//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
