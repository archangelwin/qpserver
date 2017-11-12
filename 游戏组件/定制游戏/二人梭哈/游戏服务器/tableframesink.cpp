#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

#define TIME_PASS_CARD        50000                 //������ʱ

#define MAX_SHOW_HAND_SCORE     0x7FFFFFFFFFFFFFFF            //����������

//////////////////////////////////////////////////////////////////////////


//��̬����
const WORD      CTableFrameSink::m_wPlayerCount=GAME_PLAYER;      //��Ϸ����
const BYTE      CTableFrameSink::m_GameStartMode=START_MODE_ALL_READY;  //��ʼģʽ

LONGLONG *      CTableFrameSink::m_plStockScore = NULL;

//////////////////////////////////////////////////////////////////////////
void Debug(char *text,...)
{
  static DWORD num=0;
  char buf[1024];
  FILE *fp=NULL;
  va_list ap;
  va_start(ap,text);
  vsprintf(buf,text,ap);
  va_end(ap);
  if(num == 0)
  {
    fp=fopen("����������.log","w");
  }
  else
  {
    fp=fopen("����������.log","a");
  }
  if(fp == NULL)
  {
    return ;
  }
  num++;
  SYSTEMTIME time;
  GetLocalTime(&time);
  fprintf(fp, "%d:%s - %d/%d/%d %d:%d:%d \n", num, buf, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
  fclose(fp);
}
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
  //��Ϸ����
  m_wOperaCount=0;
  m_bSendCardCount=0;
  m_wCurrentUser=INVALID_CHAIR;
  memset(m_cbCardCount,0,sizeof(m_cbCardCount));
  memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
  m_cbGameEndReason = GER_NORMAL;
  m_bGameEnd = false;
  m_bShowHand = false;
  m_bAndroidLose=false;

  //�����Ϣ
  m_lMaxGold=0;
  m_lTurnMaxGold = 0;
  m_lTurnBasicGold=0;
  m_lBasicGold = 0;
  memset(m_lTableGold,0,sizeof(m_lTableGold));
  ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
  ZeroMemory(m_lLostScore,sizeof(m_lLostScore));
  ZeroMemory(m_lPlayBeginGold, sizeof(m_lPlayBeginGold));

  //��������
  memset(m_cbPlayStatus,0,sizeof(m_cbPlayStatus));

  //�������
  m_pITableFrame=NULL;
  m_pGameServiceOption=NULL;
  m_pGameCustomRule=NULL;

  //���Ʊ���
  m_wTableCount = 0;
  m_wMeTableID = 0;

  m_lStorageDeduct = 0;
  m_lStockLimit = 0;
  m_lStorageMin = 0;
  m_lStorageMax = 0;
  m_nStorageMul = 0;

  //�������
  m_hControlInst = NULL;
  m_pServerContro = NULL;
  m_hControlInst = LoadLibrary(TEXT("ShowHandANExServerControl.dll"));
  if(m_hControlInst)
  {
    typedef void * (*CREATE)();
    CREATE ServerControl = (CREATE)GetProcAddress(m_hControlInst,"CreateServerControl");
    if(ServerControl)
    {
      m_pServerContro = static_cast<IServerControl*>(ServerControl());
    }
  }

  return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
  if(m_pServerContro)
  {
    delete m_pServerContro;
    m_pServerContro = NULL;
  }

  if(m_hControlInst)
  {
    FreeLibrary(m_hControlInst);
    m_hControlInst = NULL;
  }

  if(NULL != m_plStockScore)
  {
    delete[] m_plStockScore;
    m_plStockScore = NULL;
  }
}

//�ͷŶ���
VOID CTableFrameSink::Release()
{
  if(m_pServerContro)
  {
    delete m_pServerContro;
    m_pServerContro = NULL;
  }

  if(m_hControlInst)
  {
    FreeLibrary(m_hControlInst);
    m_hControlInst = NULL;
  }

  delete this;
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
  QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
  return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
  try
  {
    //��ѯ�ӿ�
    ASSERT(pIUnknownEx!=NULL);
    m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
    if(m_pITableFrame==NULL)
    {
      return false;
    }

    //��ȡ����
    m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
    m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
    ASSERT(m_pGameServiceOption!=NULL);

    //��ʼ���������
    m_wTableCount = m_pGameServiceOption->wTableCount;
    m_wMeTableID = m_pITableFrame->GetTableID();

    if(NULL == m_plStockScore)
    {
      m_plStockScore = new LONGLONG[m_wTableCount];
      ZeroMemory(m_plStockScore, sizeof(LONGLONG)*m_wTableCount);
    }

    //��ȡ����
    TCHAR szFileName[MAX_PATH]=TEXT("");
    GetCurrentDirectory(sizeof(szFileName),szFileName);
    _sntprintf(szFileName,sizeof(szFileName),TEXT("%s\\ShowHandExServer.ini"),szFileName);

    //��ʼ�洢��Ŀ
    TCHAR szServerName[MAX_PATH]=TEXT("");
    _sntprintf(szServerName,sizeof(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);
    m_plStockScore[m_wMeTableID] = GetPrivateProfileInt(szServerName,TEXT("StorageStart"),0,szFileName);
    m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),100000,szFileName);
    m_lStorageDeduct=GetPrivateProfileInt(szServerName,TEXT("StorageDeduct"),0,szFileName);
    m_lStorageMax = GetPrivateProfileInt(szServerName, TEXT("StorageMax"), 50000000, szFileName);
    m_nStorageMul = GetPrivateProfileInt(szServerName, TEXT("StorageMul"), 50, szFileName);
    m_lStockLimit=0;

    if(m_plStockScore[m_wMeTableID]>=m_lStockLimit)
    {
      m_lStockLimit=0;
    }
    if(m_lStorageMin==0)
    {
      m_lStorageMin=100;
    }
    if(m_lStorageDeduct < 0 || m_lStorageDeduct > 1000)
    {
      m_lStorageDeduct = 1;
    }
    if(m_nStorageMul < 0 || m_nStorageMul > 100)
    {
      m_nStorageMul = 50;
    }

    //�Զ�����
    ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
    m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

    m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

    m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
  }
  catch(...)
  {
    AfxMessageBox(L"���ӳ�ʼ���쳣��");
  }
  return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
  //״̬����
  m_wOperaCount=0;
  m_bSendCardCount=0;
  m_wCurrentUser=INVALID_CHAIR;
  memset(m_cbPlayStatus,0,sizeof(m_cbPlayStatus));
  memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
  memset(m_cbCardCount,0,sizeof(m_cbCardCount));
  m_cbGameEndReason = GER_NORMAL;
  m_bGameEnd = false;
  m_bShowHand = false;
  m_bAndroidLose=false;

  //��ұ���
  m_lMaxGold=0L;
  m_lTurnMaxGold = 0;
  m_lTurnBasicGold=0L;
  m_lBasicGold = 0;
  memset(m_lTableGold,0,sizeof(m_lTableGold));
  ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
  ZeroMemory(m_lLostScore,sizeof(m_lLostScore));

  return;
}

//��������
LONGLONG CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
  return 0L;
}

//���ٻ���
LONGLONG CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
  return 0L;
}


//��ʼģʽ
BYTE  CTableFrameSink::GetGameStartMode()
{
  return m_GameStartMode;
}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
  ASSERT(wChairID<m_wPlayerCount);
  return m_cbPlayStatus[wChairID]?true:false;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
  //״̬����
  m_wOperaCount=0;
  m_bSendCardCount=0;
  memset(m_cbPlayStatus,0,sizeof(m_cbPlayStatus));
  memset(m_cbCardCount,0,sizeof(m_cbCardCount));

  //���
  if(m_plStockScore[m_wMeTableID]>0)
  {
    m_plStockScore[m_wMeTableID]=m_plStockScore[m_wMeTableID]-m_plStockScore[m_wMeTableID]*m_lStorageDeduct/1000;
  }

  /*CString cs;
  cs.Format(TEXT("��ǰ��棺%I64d"),m_lStockScore);
  CTraceService::TraceString(cs,TraceLevel_Exception);*/
  Debug("��%d���Ŀ��Ϊ��%I64d",m_pITableFrame->GetTableID()+1,m_plStockScore[m_wMeTableID]);

  //��ұ���
  m_lMaxGold=0L;
  m_lTurnBasicGold=0L;
  memset(m_lTableGold,0,sizeof(m_lTableGold));

  //����״̬
  m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);

  //��������
  tagUserScore * pUserScore=NULL;
  LONGLONG lScore=0;
  IServerUserItem * pIServerUserItem=NULL;

  //������ע
  bool bCalculateGold=false;
  LONGLONG lMaxShowHandScore = m_pGameServiceOption->lRestrictScore!=0L?m_pGameServiceOption->lRestrictScore:MAX_SHOW_HAND_SCORE;
  LONGLONG lServiceScore = m_pGameServiceOption->lServiceScore;
  for(WORD i=0; i<m_wPlayerCount; i++)
  {
    pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
    if(pIServerUserItem!=NULL)
    {
      m_cbPlayStatus[i]=true;
      lScore=pIServerUserItem->GetUserScore();
      m_lPlayBeginGold[i]=pIServerUserItem->GetUserScore();
      m_lUserScore[i] = __min(lScore,lMaxShowHandScore);
      if(bCalculateGold==false)
      {
        bCalculateGold=true;
        m_lMaxGold=m_lUserScore[i];
      }
      else
      {
        m_lMaxGold=__min(m_lMaxGold,m_lUserScore[i]);
      }
    }
  }
  m_lTurnMaxGold = m_lMaxGold/2;
  m_lBasicGold=__max(m_lMaxGold/100L,m_pGameServiceOption->lCellScore);

  for(WORD i = 0; i < m_wPlayerCount; i++)
  {
    if(m_cbPlayStatus[i])
    {
      m_lTableGold[i*2+1]=m_lBasicGold;
    }
  }

  //�ַ��˿�
  m_bSendCardCount=2;
  m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));

  if(0)
  {
    m_cbHandCardData[0][0]=0x1a;
    m_cbHandCardData[0][1]=0x3b;
    m_cbHandCardData[0][2]=0x39;
    m_cbHandCardData[0][3]=0x2c;
    m_cbHandCardData[0][4]=0x08;

    m_cbHandCardData[1][0]=0x09;
    m_cbHandCardData[1][1]=0x21;
    m_cbHandCardData[1][2]=0x1b;
    m_cbHandCardData[1][3]=0x28;
    m_cbHandCardData[1][4]=0x0c;

    /*m_cbHandCardData[1][0]=0x09;
    m_cbHandCardData[1][1]=0x1c;
    m_cbHandCardData[1][2]=0x2b;
    m_cbHandCardData[1][3]=0x1d;
    m_cbHandCardData[1][4]=0x11;*/
  }

  //�˿˷���
  AnalyseCard();

  //����
  bool bControl = false;
  BYTE bMaxCount=0;
  if(m_pServerContro != NULL && m_pServerContro->NeedControl())  //�ڴ˴��������
  {
    if(m_pServerContro->ControlResult(m_cbHandCardData))
    {
      bControl=true;
      m_pServerContro->CompleteControl();

    }
  }

  //��������ͻ�����
  if(m_pServerContro)
  {
    //������Ϣ
    BYTE cbHandCardData[GAME_PLAYER][5];
    CopyMemory(cbHandCardData, m_cbHandCardData, sizeof(cbHandCardData));

    //�����˿�
    for(WORD i= 0; i < GAME_PLAYER; i++)
    {
      m_GameLogic.SortCardList(cbHandCardData[i], 5);
    }

    //��Ϣ����
    m_pServerContro->ShowCheatCard(cbHandCardData, m_pITableFrame);
  }

  for(WORD i=0; i<m_wPlayerCount; i++)
  {
    if(m_cbPlayStatus[i]==TRUE)
    {
      m_cbCardCount[i]=m_bSendCardCount;
    }
  }

  //�����û�
  m_wCurrentUser=EstimateWinner(1,1);

  //���Ϳ�ʼ
  CMD_S_GameStart GameStart;
  memset(&GameStart,0,sizeof(GameStart));
  GameStart.wCurrentUser=m_wCurrentUser;
  GameStart.lTurnMaxGold=m_lTurnMaxGold;
  GameStart.lTurnBasicGold=m_lTurnBasicGold;
  GameStart.lBasicGold = m_lBasicGold;
  GameStart.lShowHandScore = lMaxShowHandScore;
  GameStart.lServiceScore=lServiceScore;
  for(WORD i=0; i<m_wPlayerCount; i++)
  {
    if(m_cbPlayStatus[i])
    {
      GameStart.bCardData[i]=m_cbHandCardData[i][1];
    }
  }
  for(WORD i=0; i<m_wPlayerCount; i++)
  {
    if(m_cbPlayStatus[i]==TRUE)
    {
      GameStart.bFundusCard=m_cbHandCardData[i][0];
      m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
      GameStart.bFundusCard=0;
      m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
    }
  }

  return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  switch(cbReason)
  {
    case GER_DISMISS:   //��Ϸ��ɢ
    {
      //Ч�����
      ASSERT(pIServerUserItem!=NULL);
      ASSERT(wChairID<m_wPlayerCount);

      //��������
      CMD_S_GameEnd GameEnd;
      memset(&GameEnd,0,sizeof(GameEnd));

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

      //������Ϸ
      m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

      return true;
    }
    case GER_NORMAL:    //�������
    case GER_NO_PLAYER:   //û�����
    {
      //�������
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //ͳ����Ϣ
      WORD wWinerUser=EstimateWinner(0,4);
      ASSERT(wWinerUser != INVALID_CHAIR);
      LONGLONG lMaxWinScore = m_lTableGold[wWinerUser*2]+m_lTableGold[wWinerUser*2+1];
      for(WORD i=0; i<m_wPlayerCount; i++)
      {
        //������Ϣ
        if((cbReason==GER_NO_PLAYER)||(m_cbPlayStatus[i]==FALSE))
        {
          GameEnd.bUserCard[i]=0;
        }
        else
        {
          GameEnd.bUserCard[i]=m_cbHandCardData[i][0];
        }

        //������Ϣ
        if(i!=wWinerUser)
        {
          if(m_cbPlayStatus[i])
          {
            LONGLONG lScore = __min(m_lTableGold[i*2]+m_lTableGold[i*2+1],m_cbPlayStatus[i]?lMaxWinScore:m_lMaxGold);

            //��ֹ����
            IServerUserItem* pDEXIServerUserItem = m_pITableFrame->GetTableUserItem(i);
            if(pDEXIServerUserItem != NULL)
            {
              if(lScore > pDEXIServerUserItem->GetUserScore())
              {
                lScore = pDEXIServerUserItem->GetUserScore();
              }
            }
            else
            {
              if(lScore > m_lPlayBeginGold[i])
              {
                lScore = m_lPlayBeginGold[i];
              }
            }

            GameEnd.lGameGold[i] = -lScore;
            GameEnd.lGameGold[wWinerUser] += lScore;
          }
          else
          {
            //��ֹ����
            IServerUserItem* pDEXIServerUserItem = m_pITableFrame->GetTableUserItem(i);
            if(pDEXIServerUserItem != NULL)
            {
              if((-m_lLostScore[i]) > pDEXIServerUserItem->GetUserScore())
              {
                m_lLostScore[i] = -pDEXIServerUserItem->GetUserScore();
              }
            }
            else
            {
              if((-m_lLostScore[i]) > m_lPlayBeginGold[i])
              {
                m_lLostScore[i] = -m_lPlayBeginGold[i];
              }
            }

            GameEnd.lGameGold[i] = m_lLostScore[i];
            GameEnd.lGameGold[wWinerUser] += -m_lLostScore[i];
          }
        }
      }

      tagScoreInfo ScoreInfoArray[GAME_PLAYER];
      ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        //ʤ������
        ScoreInfoArray[i].cbType=(GameEnd.lGameGold[i]>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
        ScoreInfoArray[i].lScore=GameEnd.lGameGold[i];
        //����˰��
        if(m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
        {
          //��˰����
          WORD cbRevenue=m_pGameServiceOption->wRevenueRatio;

          if(GameEnd.lGameGold[i]>=100L)
          {
            //����˰��
            if(GameEnd.lGameGold[i] > 0)
            {
              ScoreInfoArray[i].lRevenue = m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
            }

            //�ɼ�����
            GameEnd.lTax+=ScoreInfoArray[i].lRevenue;
            GameEnd.lGameGold[i]=GameEnd.lGameGold[i]-ScoreInfoArray[i].lRevenue;
            ScoreInfoArray[i].lScore=GameEnd.lGameGold[i];
          }
        }
      }
      //д�����
      m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

      //���ͳ��
      for(WORD i=0; i<m_wPlayerCount; i++)
      {
        //��ȡ�û�
        IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
        if(pIServerUserIte==NULL)
        {
          continue;
        }

        //����ۼ�
        if(!pIServerUserIte->IsAndroidUser())
        {
          m_plStockScore[m_wMeTableID]-=GameEnd.lGameGold[i];
        }

      }

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

      //������Ϸ
      m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

      return true;
    }
    case GER_NETWORK_ERROR:
    case GER_USER_LEAVE:    //�û�ǿ��
    {
      //Ч�����
      ASSERT(pIServerUserItem!=NULL);
      ASSERT(wChairID<m_wPlayerCount);

      //ǿ�˴���
      if(m_cbPlayStatus[wChairID])
      {
        OnUserGiveUp(wChairID,true);
      }

      return true;
    }
  }

  return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:    //����״̬
    {
      //��������
      CMD_S_StatusFree StatusFree;
      memset(&StatusFree,0,sizeof(StatusFree));
      StatusFree.dwBasicGold=0;
      //��������
      CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

      //���ͳ���
      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
    }
    case GAME_STATUS_PLAY:  //��Ϸ״̬
    {
      //��������
      CMD_S_StatusPlay StatusPlay;
      memset(&StatusPlay,0,sizeof(StatusPlay));
      StatusPlay.wCurrentUser=m_wCurrentUser;
      StatusPlay.lTurnBasicGold=m_lTurnBasicGold;
      StatusPlay.lBasicGold=m_lBasicGold;
      CopyMemory(StatusPlay.bPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));
      CopyMemory(StatusPlay.lTableGold,m_lTableGold,sizeof(m_lTableGold));
      StatusPlay.lShowHandScore = m_pGameServiceOption->lRestrictScore!=0L?m_pGameServiceOption->lRestrictScore:MAX_SHOW_HAND_SCORE;
      StatusPlay.bShowHand = m_bShowHand?TRUE:FALSE;
      //��������
      CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

      //������ע
      StatusPlay.lTurnMaxGold=m_lTurnMaxGold;

      //�����˿�
      for(WORD i=0; i<m_wPlayerCount; i++)
      {
        if(m_cbPlayStatus[i]==TRUE)
        {
          ASSERT(m_pITableFrame->GetTableUserItem(i)!=NULL);
          StatusPlay.bTableCardCount[i]=m_cbCardCount[i];
          if(i==wChairID&&pIServerUserItem->GetUserStatus()!=US_LOOKON)
          {
            StatusPlay.bTableCardArray[i][0]=m_cbHandCardData[i][0];
          }
          CopyMemory(&StatusPlay.bTableCardArray[i][1],&m_cbHandCardData[i][1],(m_cbCardCount[i]-1)*sizeof(BYTE));
        }
      }

      //���ͳ���
      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
    }
  }

  //Ч����
  ASSERT(FALSE);

  return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
  return false;
}

//�����¼�
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
  return false;
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  return true;
}


//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  switch(wSubCmdID)
  {
    case SUB_C_ADD_GOLD:  //�û���ע
    {
      //Ч������
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }
      if(m_cbPlayStatus[pUserData->wChairID]==FALSE)
      {
        return true;
      }

      //��Ϣ����
      CMD_C_AddGold * pAddGold=(CMD_C_AddGold *)pData;
      OnUserAddGold(pUserData->wChairID,pAddGold->lGold,false);

      return true;
    }
    case SUB_C_GIVE_UP:   //�û�����
    {
      //Ч������
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }
      if(m_cbPlayStatus[pUserData->wChairID]==FALSE)
      {
        return true;
      }

      //��Ϣ����
      OnUserGiveUp(pUserData->wChairID);

      return true;
    }
    case SUB_C_AMDIN_COMMAND:
    {
      ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
      if(wDataSize!=sizeof(CMD_C_AdminReq))
      {
        return false;
      }

      ASSERT(pIServerUserItem->GetUserStatus()!=US_PLAYING);
      if(pIServerUserItem->GetUserStatus()==US_PLAYING)
      {
        return true;
      }

      if(m_pServerContro == NULL)
      {
        return true;
      }

      return m_pServerContro->ServerControl(wSubCmdID, pData, wDataSize, pIServerUserItem, m_pITableFrame);
    }
    case SUB_C_AMDIN_COMMAND_EX:
    {
      ASSERT(wDataSize==sizeof(CMD_C_AdminReqEx));
      if(wDataSize!=sizeof(CMD_C_AdminReqEx))
      {
        return false;
      }

      //��������й���ԱȨ�� �򷵻ش���
      if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
      {
        return false;
      }

      CMD_C_AdminReqEx* AdminReq = (CMD_C_AdminReqEx*)pData;

      switch(AdminReq->cbReqType)
      {
        case RQ_UPDATE_STORAGE:
        {
          if(AdminReq->wTableID < m_wTableCount)
          {
            CMD_S_CommandResultEx cResult;
            ZeroMemory(&cResult, sizeof(cResult));

            cResult.cbAckType=ACK_UPDATE_STORAGE;
            cResult.cbResult=CR_ACCEPT;
            cResult.wTableID = AdminReq->wTableID;
            cResult.lStorage = m_plStockScore[AdminReq->wTableID];
            m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND_EX,&cResult,sizeof(CMD_S_CommandResultEx));
          }
          else
          {
            CMD_S_CommandResultEx cResult;
            ZeroMemory(&cResult, sizeof(cResult));

            cResult.cbAckType=ACK_UPDATE_STORAGE;
            cResult.cbResult=CR_REFUSAL;
            m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND_EX,&cResult,sizeof(CMD_S_CommandResultEx));
          }
          break;
        }
        case RQ_SET_STORAGE:
        {
          if(AdminReq->wTableID < m_wTableCount)
          {
            //���ÿ��
            m_plStockScore[AdminReq->wTableID] = AdminReq->lStorage;

            CMD_S_CommandResultEx cResult;
            ZeroMemory(&cResult, sizeof(cResult));

            cResult.cbAckType=ACK_SET_STORAGE;
            cResult.cbResult=CR_ACCEPT;
            cResult.wTableID = AdminReq->wTableID;
            cResult.lStorage = m_plStockScore[AdminReq->wTableID];
            m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND_EX,&cResult,sizeof(CMD_S_CommandResultEx));
          }
          else
          {
            CMD_S_CommandResultEx cResult;
            ZeroMemory(&cResult, sizeof(cResult));

            cResult.cbAckType=ACK_SET_STORAGE;
            cResult.cbResult=CR_REFUSAL;
            m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND_EX,&cResult,sizeof(CMD_S_CommandResultEx));
          }
          break;
        }
        case RQ_CLEAR_ALLSTORAGE:
        {
          //����������ӿ��
          for(WORD i=0; i<m_wTableCount; i++)
          {
            m_plStockScore[i] = 0;
          }

          CMD_S_CommandResultEx cResult;
          ZeroMemory(&cResult, sizeof(cResult));

          cResult.cbAckType=ACK_CLEAR_ALLSTORAGE;
          cResult.cbResult=CR_ACCEPT;
          m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_AMDIN_COMMAND_EX,&cResult,sizeof(CMD_S_CommandResultEx));

          break;
        }

      }

      return true;

    }
  }

  return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  return false;
}


//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  return true;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID,bool bLeave)
{
  //״̬�ж�
  ASSERT(m_cbPlayStatus[wChairID]);
  if(!m_cbPlayStatus[wChairID])
  {
    return false;
  }

  //Ч��
  if(m_bGameEnd)
  {
    return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
  }

  //��������
  m_cbPlayStatus[wChairID]=FALSE;
  m_cbCardCount[wChairID]=0;

  //��������
  LONGLONG lScore=0;
  LONGLONG lRevenue=0;
  BYTE      ScoreKind;

  //���ñ���
  ScoreKind=bLeave?SCORE_TYPE_FLEE:SCORE_TYPE_LOSE;
  lScore=m_lTableGold[wChairID*2]+m_lTableGold[wChairID*2+1];
  lScore = -__min(m_lMaxGold,lScore);

  m_lLostScore[wChairID] = lScore;

  //���»�ȡ�����ע
  if(m_lUserScore[wChairID] == m_lMaxGold)
  {
    for(WORD i = 0; i < GAME_PLAYER; i ++)
    {
      if(!m_cbPlayStatus[i])
      {
        continue;
      }
      m_lMaxGold = m_lUserScore[i];
      break;
    }
    for(; i < GAME_PLAYER; i++)
    {
      if(m_cbPlayStatus[i])
      {
        m_lMaxGold = __min(m_lMaxGold,m_lUserScore[i]);
      }
    }
  }
  m_lTurnMaxGold = m_bSendCardCount>=3?m_lMaxGold:m_lMaxGold/2L;

  //���ͷ�����Ϣ
  CMD_S_GiveUp GiveUp;
  GiveUp.wUserChairID=wChairID;
  GiveUp.lTurnMaxGold = m_lTurnMaxGold;
  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

  ////�޸Ļ���
  //tagScoreInfo ScoreInfoArray[GAME_PLAYER];
  //ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

  //ScoreInfoArray[wChairID].cbType=ScoreKind;
  //ScoreInfoArray[wChairID].lScore=m_lLostScore[wChairID];

  ////�û�д��
  //m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

  //�жϽ���
  WORD wPlayerCount=0;
  for(WORD i=0; i<m_wPlayerCount; i++)
  {
    if(m_cbPlayStatus[i]==TRUE)
    {
      wPlayerCount++;
    }
  }
  if(wPlayerCount>=2)
  {
    if(m_wCurrentUser==wChairID)
    {
      OnUserAddGold(wChairID,0L,true);
    }
  }
  else
  {
    OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);
  }

  return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddGold(WORD wChairID, LONGLONG lGold, bool bCancel)
{
  //Ч��
  if(m_bGameEnd)
  {
    return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
  }

  LONGLONG TempGold=0;

  //��������
  if(bCancel==false)
  {
    //Ч������
    ASSERT(m_wCurrentUser==wChairID);
    if(m_wCurrentUser!=wChairID)
    {
      return false;
    }

    /*ASSERT( !m_bShowHand && lGold >= m_lTurnBasicGold && lGold <= m_lTurnMaxGold
      || lGold+m_lTableGold[wChairID*2+1]==m_lUserScore[wChairID] );*/
    if((m_bShowHand || lGold < m_lTurnBasicGold || lGold > m_lTurnMaxGold))
    {
      if(lGold+m_lTableGold[wChairID*2+1]!=m_lUserScore[wChairID])
      {
        IServerUserItem * pIServerAndroid = m_pITableFrame->GetTableUserItem(wChairID);
        if(pIServerAndroid->IsAndroidUser())
        {
          lGold = m_lUserScore[wChairID]-m_lTableGold[wChairID*2+1];
        }
        else
        {
          return false;
        }
      }
    }

    m_lTableGold[wChairID*2]=lGold;

    if(!m_bShowHand && m_lTableGold[wChairID*2]+m_lTableGold[wChairID*2+1] == m_lUserScore[wChairID])
    {
      m_bShowHand = true;
    }
  }

  //��������
  bool bFinishTurn=false;
  WORD wNextUser=INVALID_CHAIR;
  for(WORD i=0; i<m_wPlayerCount-1; i++)
  {
    m_wOperaCount++;
    wNextUser=(m_wCurrentUser+i+1)%m_wPlayerCount;
    if(m_cbPlayStatus[wNextUser]==TRUE)
    {
      break;
    }
  }

  //�ж���ע
  if(m_wOperaCount>=m_wPlayerCount)
  {
    LONGLONG lTableGold=0L;
    for(WORD i=0; i<m_wPlayerCount; i++)
    {
      if(m_cbPlayStatus[i]==TRUE)
      {
        lTableGold=m_lTableGold[i*2];
        break;
      }
    }
    while(i<m_wPlayerCount)
    {
      if(m_cbPlayStatus[i]==TRUE)
      {
        if(!m_bShowHand&&m_lTableGold[i*2]!=lTableGold ||
           m_bShowHand&&m_lTableGold[i*2]+m_lTableGold[i*2+1]<m_lUserScore[i])
        {
          break;
        }
      }
      i++;
    }
    if(i==m_wPlayerCount)
    {
      bFinishTurn=true;
    }
  }

  bool bGiveUp=true;
  bGiveUp=AnalyseCardEX();

  //һ���ж�
  if(bFinishTurn==true)
  {
    //��������
    CMD_S_AddGold GoldResult;
    GoldResult.lCurrentLessGold=0L;
    GoldResult.wLastChairID=wChairID;
    GoldResult.wCurrentUser=INVALID_CHAIR;
    GoldResult.lLastAddGold=m_lTableGold[wChairID*2];
    GoldResult.bShowHand = m_bShowHand;
    GoldResult.bNoGiveUp=bGiveUp;
    GoldResult.lShowHandGold=TempGold;
    GoldResult.bAndroidLose=m_bAndroidLose;
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&GoldResult,sizeof(GoldResult));

    //�ۼƽ��
    for(WORD i=0; i<m_wPlayerCount; i++)
    {
      m_lTableGold[i*2+1]+=m_lTableGold[i*2];
      m_lTableGold[i*2]=0L;
    }

    //��ʼһ��
    if(m_bSendCardCount<5)
    {
      //���ñ���
      m_wOperaCount=0;
      m_lTurnBasicGold=0L;
      WORD wLastMostUser = EstimateWinner(1,m_bSendCardCount-1);

      BYTE bySourceCount = m_bSendCardCount;
      if(m_bShowHand)
      {
        m_bSendCardCount = MAX_COUNT;
      }
      else
      {
        m_bSendCardCount++;
      }

      //�����˿�
      CMD_R_SendCard SendCard;
      ZeroMemory(&SendCard,sizeof(SendCard));
      for(WORD i=0; i<m_wPlayerCount; i++)
      {
        if(m_cbPlayStatus[i]==FALSE)
        {
          continue;
        }

        m_cbCardCount[i]=m_bSendCardCount;

        //�ɷ��˿�
        for(BYTE j=0; j<(m_bSendCardCount-bySourceCount); j++)
        {
          ASSERT(j<CountArray(SendCard.bUserCard[i]));
          SendCard.bUserCard[i][j]=m_cbHandCardData[i][bySourceCount+j];
        }
      }

      m_wCurrentUser=EstimateWinner(1,m_bSendCardCount-1);
      m_lTurnMaxGold = m_bSendCardCount>=3?m_lMaxGold:m_lMaxGold/2L;
      bool bShowHand=m_bSendCardCount>=3?true:false;
      //������Ϣ
      SendCard.wCurrentUser=(m_bShowHand?INVALID_CHAIR:m_wCurrentUser);
      SendCard.wStartChairId = wLastMostUser;
      SendCard.cbSendCardCount = m_bSendCardCount-bySourceCount;
      SendCard.bAndroidLose=m_bAndroidLose;
      SendCard.lMaxGold=m_lTurnMaxGold;
      SendCard.bNoGiveUp=bGiveUp;
      SendCard.lShowHandGold=m_lUserScore[m_wCurrentUser]-m_lTableGold[m_wCurrentUser*2+1];
      SendCard.bCanShowHand=bShowHand;
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

      //������Ϸ
      if(m_bSendCardCount == 5 && m_lMaxGold <= m_lTableGold[m_wCurrentUser*2+1])
      {
        m_bGameEnd = true;
        m_cbGameEndReason = GER_NORMAL;
        OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
        return true;
      }
    }
    else
    {
      m_bGameEnd = true;
      m_cbGameEndReason = GER_NORMAL;
      OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
      return true;
    }
  }
  else
  {
    //���ñ���
    m_wCurrentUser=wNextUser;
    m_lTurnBasicGold=__max(m_lTableGold[wChairID*2],m_lTurnBasicGold);

    //��������
    CMD_S_AddGold AddGold;
    AddGold.wLastChairID=wChairID;
    AddGold.wCurrentUser=m_wCurrentUser;
    AddGold.lCurrentLessGold=m_lTurnBasicGold;
    AddGold.lLastAddGold=m_lTableGold[wChairID*2];
    AddGold.bShowHand = m_bShowHand;
    AddGold.bAndroidLose=m_bAndroidLose;
    AddGold.bNoGiveUp=bGiveUp;
    AddGold.bCanShowHand=m_bSendCardCount>=3?true:false;
    //if (m_bShowHand)
    {
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
        if(pIServerUserItem->IsAndroidUser())
        {
          TempGold=pIServerUserItem->GetUserScore()-m_lTableGold[i*2+1];
          AddGold.lShowHandGold=TempGold;
        }
      }
    }
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_GOLD,&AddGold,sizeof(AddGold));
  }
  return true;
}

//�ƶ�ʤ��
WORD CTableFrameSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
  //�����˿�
  BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
  CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

  //Ѱ�����
  for(WORD wWinner=0; wWinner<GAME_PLAYER; wWinner++)
  {
    if(m_cbPlayStatus[wWinner]==TRUE)
    {
      m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
      break;
    }
  }

  //�Ա����
  WORD wId = wWinner;
  for(WORD i=0; i<GAME_PLAYER-1; i++)
  {
    wId = (wId+1)%GAME_PLAYER;

    //�û�����
    if(m_cbPlayStatus[wId]==FALSE)
    {
      continue;
    }

    //�����˿�
    m_GameLogic.SortCardList(cbUserCardData[wId]+cbStartPos,cbConcludePos-cbStartPos+1);

    //�Ա��˿�
    if(m_GameLogic.CompareCard(cbUserCardData[wId]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true)
    {
      wWinner=wId;
    }
  }

  return wWinner;
}

//�˿˷���
bool CTableFrameSink::AnalyseCard()
{
  //��������
  WORD wAiCount = 0;
  WORD wPlayerCount = 0;
  WORD wAndroid=INVALID_CHAIR;
  WORD wPalyer=INVALID_CHAIR;
  WORD wMinScoreID=INVALID_CHAIR;
  for(WORD i=0; i<m_wPlayerCount; i++)
  {
    //��ȡ�û�
    IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
    if(pIServerUserItem!=NULL)
    {
      if(m_cbPlayStatus[i]==FALSE)
      {
        continue;
      }
      if(pIServerUserItem->IsAndroidUser())
      {
        wAiCount++ ;
        wAndroid=i;
      }
      else
      {
        wPalyer=i;
      }
      wPlayerCount++;

      if(wMinScoreID != INVALID_CHAIR)
      {
        if(pIServerUserItem->GetUserScore()< m_pITableFrame->GetTableUserItem((i+1)%m_wPlayerCount)->GetUserScore())
        {
          wMinScoreID = i;
        }
      }
      else
      {
        wMinScoreID = i;
      }
    }
  }

  //ȫ������
  if(wPlayerCount == wAiCount || wAiCount==0)
  {
    return false;
  }

  //�˿˱���
  BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
  CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

  //��������
  LONGLONG lAndroidScore=0;

  if(wPalyer!=INVALID_CHAIR||wAndroid!=INVALID_CHAIR)
  {
    //�Ա��˿�
    if(m_GameLogic.CompareCard(cbUserCardData[wPalyer],cbUserCardData[wAndroid],MAX_COUNT)==true)
    {
      lAndroidScore -= m_lTableGold[wPalyer*2+1]+m_lTableGold[wPalyer*2];

      //������������ͨѹע
      lAndroidScore = lAndroidScore*4;

      //����Ӯ��
      if(m_plStockScore[m_wMeTableID] - m_lStorageMin + lAndroidScore <= 0)
      {
        CopyMemory(m_cbHandCardData[wAndroid], cbUserCardData[wPalyer], sizeof(cbUserCardData[wPalyer]));
        CopyMemory(m_cbHandCardData[wPalyer], cbUserCardData[wAndroid], sizeof(cbUserCardData[wAndroid]));
        ZeroMemory(cbUserCardData, sizeof(cbUserCardData));

        //��Ӯ������Ӵ�С����ó�����֪������Ƶ����
        m_GameLogic.SortCardList(m_cbHandCardData[wAndroid],MAX_COUNT);

        m_bAndroidLose = false;
      }
      else
      {
        m_bAndroidLose = true;
      }
    }
    else
    {
      lAndroidScore -= m_lTableGold[wPalyer*2+1]+m_lTableGold[wPalyer*2];

      //������������ͨѹע
      lAndroidScore = lAndroidScore*4;

      //����Ӯ��
      if(m_plStockScore[m_wMeTableID] - m_lStorageMax + lAndroidScore > 0 && (rand()%100 <= m_nStorageMul))
      {
        CopyMemory(m_cbHandCardData[wAndroid], cbUserCardData[wPalyer], sizeof(cbUserCardData[wPalyer]));
        CopyMemory(m_cbHandCardData[wPalyer], cbUserCardData[wAndroid], sizeof(cbUserCardData[wAndroid]));
        ZeroMemory(cbUserCardData, sizeof(cbUserCardData));

        m_bAndroidLose = true;
      }
      else
      {
        m_bAndroidLose = false;
      }
    }
  }

  return true;

}


//�˿˷���
bool CTableFrameSink::AnalyseCardEX()
{
  //��������
  WORD wAiCount = 0;
  WORD wPlayerCount = 0;
  WORD wAndroid=INVALID_CHAIR;
  WORD wPalyer=INVALID_CHAIR;
  for(WORD i=0; i<m_wPlayerCount; i++)
  {
    //��ȡ�û�
    IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
    if(pIServerUserItem!=NULL)
    {
      if(m_cbPlayStatus[i]==FALSE)
      {
        continue;
      }
      if(pIServerUserItem->IsAndroidUser())
      {
        wAiCount++ ;
        wAndroid=i;
      }
      else
      {
        wPalyer=i;
      }
      wPlayerCount++;
    }
  }

  //ȫ������
  if(wPlayerCount == wAiCount || wAiCount==0)
  {
    return false;
  }

  //�˿˱���
  BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
  CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));
  //��������
  LONGLONG lAndroidScore=0;

  BYTE cbTempCardData[GAME_PLAYER][MAX_COUNT-1];
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //ģ��ֻ�������淭��������
    for(int j=1; j<5; j++)
    {
      cbTempCardData[i][j-1]=m_cbHandCardData[i][j];
    }
  }

  if(wPalyer!=INVALID_CHAIR||wAndroid!=INVALID_CHAIR)
  {
    //��������
    if(m_GameLogic.CompareCard(cbUserCardData[wPalyer],cbUserCardData[wAndroid],MAX_COUNT)==true)
    {
      lAndroidScore-=m_lTableGold[wPalyer*2+1] + m_lTableGold[wPalyer*2];

      //�������䣬�Ҽ�����ע�Ŀ�治���ۣ�������
      if(m_plStockScore[m_wMeTableID]-m_lStorageMin+lAndroidScore<=0)
      {
        return false;
      }
      else
      {
        if(m_bSendCardCount==3)
        {
          //�������䣬����ʾ��������Ҳ�Ȳ�������
          if(m_GameLogic.CompareCard(cbTempCardData[wPalyer],cbTempCardData[wAndroid],2)==true)
          {
            BYTE bPlayerType = m_GameLogic.GetCardGenre(cbTempCardData[wPalyer],2);

            if(bPlayerType==CT_ONE_DOUBLE && m_GameLogic.GetCardLogicValue(cbTempCardData[wPalyer][0])>=0x0C)
            {
              return false;
            }

            return true;
          }
        }
        else if(m_bSendCardCount==4)
        {
          //�������䣬����ʾ��������Ҳ�Ȳ�������
          if(m_GameLogic.CompareCard(cbTempCardData[wPalyer],cbTempCardData[wAndroid],3)==true)
          {

            BYTE bPlayerType = m_GameLogic.GetCardGenre(cbTempCardData[wPalyer],3);

            //�����д��ڣ�qka���������ϵģ��������������
            if(bPlayerType>CT_SINGLE)
            {
              m_GameLogic.SortCardList(cbTempCardData[wPalyer],3);

              //С��Q�Ķ����ٸ�һ��
              if(bPlayerType == CT_ONE_DOUBLE && m_GameLogic.GetCardLogicValue(cbTempCardData[wPalyer][0])< 0x0C)
              {
                return true;
              }
              else
              {
                return false;
              }
            }
            else
            {
              return true;
            }

            return false;
          }
        }
        else if(m_bSendCardCount==5)
        {
          //���������ƣ�����ʾ��������Ҳ�Ȳ������ˣ���������
          if(m_GameLogic.CompareCard(cbTempCardData[wPalyer],cbTempCardData[wAndroid],4)==true)
          {
            return false;
          }
        }

        return true;
      }
    }
    else
    {
      lAndroidScore-=m_lTableGold[wPalyer*2+1] +m_lTableGold[wPalyer*2];
      bool bStorage = (m_plStockScore[m_wMeTableID]-m_lStorageMin+lAndroidScore<=0);

      if(m_bSendCardCount==3)
      {
        //�������䣬����ʾ��������Ҳ�Ȳ�������
        if(m_GameLogic.CompareCard(cbTempCardData[wPalyer],cbTempCardData[wAndroid],2)==true && !bStorage)
        {
          BYTE bPlayerType = m_GameLogic.GetCardGenre(cbTempCardData[wPalyer],2);

          if(bPlayerType==CT_ONE_DOUBLE && m_GameLogic.GetCardLogicValue(cbTempCardData[wPalyer][0])>=0x0C)
          {
            return false;
          }

          return true;
        }
      }
      else if(m_bSendCardCount==4)
      {
        //������Ӯ����湻�ۣ�����������ʾ�������ƱȲ������˵�����£�һ�������������
        if((m_GameLogic.CompareCard(cbTempCardData[wPalyer],cbTempCardData[wAndroid],3)==true) && !bStorage)
        {

          BYTE bPlayerType = m_GameLogic.GetCardGenre(cbTempCardData[wPalyer],3);

          //�����д��ڣ�qka���������ϵģ��������������
          if(bPlayerType>CT_SINGLE)
          {
            m_GameLogic.SortCardList(cbTempCardData[wPalyer],3);

            //С��Q�Ķ����ٸ�һ��
            if(bPlayerType == CT_ONE_DOUBLE && m_GameLogic.GetCardLogicValue(cbTempCardData[wPalyer][0])< 0x0C)
            {
              return true;
            }
            else
            {
              return false;
            }
          }
          else
          {
            return true;
          }

          return false;
        }
      }

      if(m_bSendCardCount==5)
      {
        //������Ӯ����湻�ۣ�����������ʾ�������ƱȲ������˵�����£�һ�������������
        if((m_GameLogic.CompareCard(cbTempCardData[wPalyer],cbTempCardData[wAndroid],4)==true)  && !bStorage)
        {
          BYTE bPlayerType = m_GameLogic.GetCardGenre(m_cbHandCardData[wPalyer],5);
          if(bPlayerType == CT_SINGLE)
          {
            if(rand()%100<70)
            {
              return false;
            }
          }

          return true;
        }
      }

      return true;
    }
  }

  if(m_plStockScore[m_wMeTableID]-m_lStorageMin+lAndroidScore<=0)
  {
    return false;
  }

  return true;

}
//////////////////////////////////////////////////////////////////////////
