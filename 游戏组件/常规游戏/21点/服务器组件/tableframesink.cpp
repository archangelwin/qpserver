#include "StdAfx.h"
#include "TableFrameSink.h"
#include "DlgCustomRule.h"
#include <conio.h>
#include <locale>

//////////////////////////////////////////////////////////////////////////////////

//���������Ϣ
CMap<DWORD, DWORD, ROOMUSERINFO, ROOMUSERINFO> g_MapRoomUserInfo; //���USERIDӳ�������Ϣ
//�����û�����
CList<ROOMUSERCONTROL, ROOMUSERCONTROL&> g_ListRoomUserControl;   //�����û���������
//�������Ƽ�¼
CList<CString, CString&> g_ListOperationRecord;           //�������Ƽ�¼

ROOMUSERINFO  g_CurrentQueryUserInfo;               //��ǰ��ѯ�û���Ϣ

//ȫ�ֱ���
LONGLONG            g_lRoomStorageStart = 0LL;                //������ʼ���
LONGLONG            g_lRoomStorageCurrent = 0LL;              //����Ӯ��
LONGLONG            g_lStorageDeductRoom = 0LL;               //�ؿ۱���
LONGLONG            g_lStorageMax1Room = 0LL;               //���ⶥ
LONGLONG            g_lStorageMul1Room = 0LL;               //ϵͳ��Ǯ����
LONGLONG            g_lStorageMax2Room = 0LL;               //���ⶥ
LONGLONG            g_lStorageMul2Room = 0LL;               //ϵͳ��Ǯ����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
  //��Ϸ����
  m_wBankerUser = INVALID_CHAIR;
  ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
  ZeroMemory(m_lInsureScore,sizeof(m_lInsureScore));
  ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
  ZeroMemory(m_bStopCard,sizeof(m_bStopCard));
  ZeroMemory(m_bDoubleCard,sizeof(m_bDoubleCard));
  ZeroMemory(m_bInsureCard,sizeof(m_bInsureCard));
  ZeroMemory(m_lMaxUserScore,sizeof(m_lMaxUserScore));
  ZeroMemory(m_lTableScoreAll,sizeof(m_lTableScoreAll));



  //�˿˱���
  ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
  m_cbRepertoryCount = 0;
  ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

  //��ע����
  m_lMaxScore = 0L;
  m_lCellScore = 0L;

  //�������
  g_ListRoomUserControl.RemoveAll();
  g_ListOperationRecord.RemoveAll();
  ZeroMemory(&g_CurrentQueryUserInfo, sizeof(g_CurrentQueryUserInfo));

  //�������
  m_hControlInst = NULL;
  m_pServerControl = NULL;
  m_hControlInst = LoadLibrary(TEXT("BlackJackServerControl.dll"));
  if(m_hControlInst)
  {
    typedef void * (*CREATE)();
    CREATE ServerControl = (CREATE)GetProcAddress(m_hControlInst,"CreateServerControl");
    if(ServerControl)
    {
      m_pServerControl = static_cast<IServerControl*>(ServerControl());
    }
  }

  return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
  if(m_pServerControl)
  {
    delete m_pServerControl;
    m_pServerControl = NULL;
  }

  if(m_hControlInst)
  {
    FreeLibrary(m_hControlInst);
    m_hControlInst = NULL;
  }
}

VOID CTableFrameSink::Release()
{
  if(m_pServerControl)
  {
    delete m_pServerControl;
    m_pServerControl = NULL;
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

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
  //��ѯ�ӿ�
  m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

  //�����ж�
  if(m_pITableFrame==NULL)
  {
    CTraceService::TraceString(TEXT("��Ϸ���� CTableFrameSink ��ѯ ITableFrame �ӿ�ʧ��"),TraceLevel_Exception);
    return false;
  }

  //��ʼģʽ
  m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

  //��ȡ����
  m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

  //��ȡ����
  ReadConfigInformation();

  return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
  //��Ϸ����
  ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
  ZeroMemory(m_lInsureScore,sizeof(m_lInsureScore));
  ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
  ZeroMemory(m_bStopCard,sizeof(m_bStopCard));
  ZeroMemory(m_bDoubleCard,sizeof(m_bDoubleCard));
  ZeroMemory(m_bInsureCard,sizeof(m_bInsureCard));
  ZeroMemory(m_lMaxUserScore,sizeof(m_lMaxUserScore));
  ZeroMemory(m_lTableScoreAll,sizeof(m_lTableScoreAll));

  //�˿˱���
  ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
  m_cbRepertoryCount = 0;
  ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

  //��ע����
  m_lMaxScore = 0L;
  m_lCellScore = 0L;

  return;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
  //���·����û���Ϣ
  UpdateRoomUserInfo(pIServerUserItem, USER_OFFLINE);

  return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
  //����״̬
  m_pITableFrame->SetGameStatus(GAME_SCENE_ADD_SCORE);
  //��ʼ������
  ZeroMemory(m_bStopCard,sizeof(m_bStopCard));
  //���˥��
  if(g_lRoomStorageCurrent > 0)
  {
    g_lRoomStorageCurrent = g_lRoomStorageCurrent - g_lRoomStorageCurrent*g_lStorageDeductRoom/1000;
  }

  //���·����û���Ϣ
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��ȡ�û�
    IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
    if(pIServerUserItem != NULL)
    {
      UpdateRoomUserInfo(pIServerUserItem, USER_SITDOWN);
    }

  }

  //���Ϳ��
  SendStorage();

  //��������
  CMD_S_GameStart GameStart;
  ZeroMemory(&GameStart,sizeof(GameStart));
  //�û�״̬
  ASSERT(m_wBankerUser != INVALID_CHAIR);
  SCORE lMinScore = -1;

  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(i);
    if(pServerUserItem)
    {
      //�������״̬
      m_cbPlayStatus[i] = TRUE;

      if(lMinScore == -1)
      {
        lMinScore = pServerUserItem->GetUserScore();
      }
      else if(pServerUserItem->GetUserScore() < lMinScore)
      {
        lMinScore = pServerUserItem->GetUserScore();
      }
      m_lMaxUserScore[i]=pServerUserItem->GetUserScore();
    }
  }
  m_lCellScore =__max(lMinScore/600L,1);
  m_lMaxScore = m_lCellScore*50;

  GameStart.lCellScore = m_lCellScore;
  GameStart.lMaxScore = m_lMaxScore;
  GameStart.wBankerUser = m_wBankerUser;

  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i]==TRUE)
    {
      GameStart.lMaxScore = __min(m_lMaxUserScore[i],m_lMaxScore);
      m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
    }
  }

  return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
  switch(cbReason)
  {
    case GER_NORMAL:    //�������
    case GER_NO_PLAYER:   //û�����
    {
      //�������
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //���
      if(AnalyseCard(false))
      {
        for(WORD i=0; i<GAME_PLAYER; i++)
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
              for(int j = 0; j < 2; j++)
              {
                BYTE cbOperateIndex = i * 2 + j;
                if(m_cbHandCardData[cbOperateIndex][0] != 0)
                {
                  bool bWin = AnalyseCardEx();;
                  BYTE cbGetCardData=0;
                  BYTE cbRepertoryCount = m_cbRepertoryCount;
                  BYTE cbHandCardData0 = m_cbHandCardData[cbOperateIndex][0];

                  while(bWin && cbRepertoryCount > 0)
                  {
                    //�����˿�
                    cbGetCardData = m_cbRepertoryCard[--cbRepertoryCount];
                    m_cbHandCardData[cbOperateIndex][0] = cbGetCardData;
                    bWin = AnalyseCardEx();
                    if(!bWin)
                    {
                      //SendInfo(TEXT("�������Ƶ��ƣ�"));
                      m_cbRepertoryCard[cbRepertoryCount] = cbHandCardData0;
                      break;
                    }
                  }

                  if(bWin)    //û���ʵ�
                  {
                    m_cbHandCardData[cbOperateIndex][0] = cbHandCardData0;
                  }
                }
              }
            }
          }
        }
      }

      //��������
      LONGLONG lGameScore[GAME_PLAYER];
      ZeroMemory(lGameScore,sizeof(lGameScore));
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        if(!m_cbPlayStatus[i])
        {
          lGameScore[m_wBankerUser] += m_lTableScore[i*2]+m_lTableScore[i*2+1]+m_lInsureScore[i*2]+m_lInsureScore[i*2+1];
          continue;
        }
        //����������
        if(cbReason != GER_NO_PLAYER)
        {
          for(BYTE j = 0; j < 2; j++)
          {
            if(m_cbCardCount[i*2+j] < 2)
            {
              continue;
            }
            //�������û�б���
            if(m_GameLogic.GetCardType(&m_cbHandCardData[i*2+j][1],m_cbCardCount[i*2+j]-1,true) > CT_BAOPAI)
            {
              GameEnd.cbCardData[i*2+j] = m_cbHandCardData[i*2+j][0];
            }
          }
        }
      }

      //����
      BYTE cbBankerCardType = m_GameLogic.GetCardType(m_cbHandCardData[m_wBankerUser*2],m_cbCardCount[m_wBankerUser*2],false);
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        if(i == m_wBankerUser || !m_cbPlayStatus[i])
        {
          continue;
        }
        for(BYTE j = 0; j < 2; j++)
        {
          if(m_cbCardCount[i*2+j] == 0)
          {
            continue;
          }
          BYTE cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[i*2+j],m_cbCardCount[i*2+j],m_cbCardCount[i*2+1]>0);
          //ׯӮ
          if(cbCardType == CT_BAOPAI || cbCardType < cbBankerCardType)
          {
            lGameScore[m_wBankerUser] += m_lTableScore[i*2+j];
            lGameScore[i] -= m_lTableScore[i*2+j];
            //����м����˱���,��ׯ��BJ
            if(m_lInsureScore[i*2+j] > 0L && cbBankerCardType == CT_BJ)
            {
              lGameScore[m_wBankerUser] -= m_lInsureScore[i*2+j];
              lGameScore[i] += m_lInsureScore[i*2+j];
            }
            else
            {
              lGameScore[m_wBankerUser] += m_lInsureScore[i*2+j];
              lGameScore[i] -= m_lInsureScore[i*2+j];
            }
          }
          //�м�Ӯ
          else if(cbCardType > cbBankerCardType)
          {
            lGameScore[m_wBankerUser] -= m_lTableScore[i*2+j];
            lGameScore[i] += m_lTableScore[i*2+j];

            //����м����˱���
            if(m_lInsureScore[i*2+j] > 0L)
            {
              if(cbBankerCardType != CT_BJ)
              {
                lGameScore[m_wBankerUser] += m_lInsureScore[i*2+j];
                lGameScore[i] -= m_lInsureScore[i*2+j];
              }
              else
              {
                lGameScore[m_wBankerUser] -= m_lInsureScore[i*2+j];
                lGameScore[i] += m_lInsureScore[i*2+j];
              }
            }
          }
          else
          {
            //ׯ��,�мҶ�BJ,���м����˱���,�м�Ӯ�ñ��շ֣�ׯ������Ӧ��
            if(CT_BJ == cbBankerCardType && cbBankerCardType == cbCardType && m_lInsureScore[i*2+j] > 0L)
            {
              lGameScore[m_wBankerUser] -= m_lInsureScore[i*2+j];
              lGameScore[i] += m_lInsureScore[i*2+j];
            }
          }
        }
      }

      //���ֱ���
      tagScoreInfo ScoreInfoArray[GAME_PLAYER];
      ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

      //��˰

      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        if(!m_cbPlayStatus[i])
        {
          continue;
        }

        if(lGameScore[i] > 0L)
        {
          ScoreInfoArray[i].cbType = SCORE_TYPE_WIN;
        }
        else if(lGameScore[i] < 0L)
        {
          ScoreInfoArray[i].cbType = SCORE_TYPE_LOSE;
        }
        else
        {
          ScoreInfoArray[i].cbType = SCORE_TYPE_DRAW;
        }
        //��ֹ����
        if(lGameScore[i] < 0L)
        {
          ASSERT(m_pITableFrame->GetTableUserItem(i));
          ASSERT(m_pITableFrame->GetTableUserItem(i)->GetUserScore()>= -lGameScore[i]);
          if(m_pITableFrame->GetTableUserItem(i)->GetUserScore() < -lGameScore[i])
          {
            lGameScore[i] = -m_pITableFrame->GetTableUserItem(i)->GetUserScore();
          }
        }
        if(GAME_GENRE_SCORE != m_pGameServiceOption->wServerType && lGameScore[i] > 0L)
        {
          ScoreInfoArray[i].lRevenue = lGameScore[i]*m_pGameServiceOption->wRevenueRatio/1000L;
          lGameScore[i] -= ScoreInfoArray[i].lRevenue;
        }
        GameEnd.lGameTax[i] = ScoreInfoArray[i].lRevenue;
        GameEnd.lGameScore[i] = lGameScore[i];

        ScoreInfoArray[i].lScore = lGameScore[i];

        if(m_pITableFrame->GetTableUserItem(i)->IsAndroidUser())
        {
          g_lRoomStorageCurrent += lGameScore[i];
        }
      }

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      //д��
      m_pITableFrame->WriteTableScore(ScoreInfoArray,GAME_PLAYER);
      //������ׯ
      m_wBankerUser = (m_wBankerUser+1)%GAME_PLAYER;
      while(!m_cbPlayStatus[m_wBankerUser])
      {
        m_wBankerUser = (m_wBankerUser+1)%GAME_PLAYER;
      }

      //������Ϸ
      m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

      //���·����û���Ϣ
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        //��ȡ�û�
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);

        if(!pIServerUserItem)
        {
          continue;
        }

        UpdateRoomUserInfo(pIServerUserItem, USER_SITDOWN);
      }

      //���Ϳ��
      CString strInfo;
      strInfo.Format(TEXT("��ǰ��棺%I64d"), g_lRoomStorageCurrent);
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
        if(!pIServerUserItem)
        {
          continue;
        }
        if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) && !pIServerUserItem->IsAndroidUser())
        {
          m_pITableFrame->SendGameMessage(pIServerUserItem, strInfo, SMT_CHAT);

          CMD_S_ADMIN_STORAGE_INFO StorageInfo;
          ZeroMemory(&StorageInfo, sizeof(StorageInfo));
          StorageInfo.lRoomStorageStart = g_lRoomStorageStart;
          StorageInfo.lRoomStorageCurrent = g_lRoomStorageCurrent;
          StorageInfo.lRoomStorageDeduct = g_lStorageDeductRoom;
          StorageInfo.lMaxRoomStorage[0] = g_lStorageMax1Room;
          StorageInfo.lMaxRoomStorage[1] = g_lStorageMax2Room;
          StorageInfo.wRoomStorageMul[0] = (WORD)g_lStorageMul1Room;
          StorageInfo.wRoomStorageMul[1] = (WORD)g_lStorageMul2Room;
          m_pITableFrame->SendTableData(i, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
          m_pITableFrame->SendLookonData(i, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
        }
      }

      return true;
    }
    case GER_USER_LEAVE:    //�û�ǿ��
    case GER_NETWORK_ERROR:     //�����ж�
    {
      //Ч�����
      ASSERT(pIServerUserItem!=NULL);
      ASSERT(wChairID<GAME_PLAYER);

      //���ׯ��ǿ��
      if(wChairID == m_wBankerUser)
      {
        LONGLONG lGameScore[GAME_PLAYER];
        ZeroMemory(lGameScore,sizeof(lGameScore));

        for(WORD i = 0; i < GAME_PLAYER; i++)
        {
          if(i == m_wBankerUser || !m_cbPlayStatus[i])
          {
            continue;
          }

          lGameScore[i] += m_lTableScore[i*2] + m_lTableScore[i*2+1];
          lGameScore[i] += m_lInsureScore[i*2] + m_lInsureScore[i*2+1];

          lGameScore[m_wBankerUser] -= lGameScore[i];
        }

        CMD_S_GameEnd GameEnd;
        ZeroMemory(&GameEnd,sizeof(GameEnd));

        //���ֱ���
        tagScoreInfo ScoreInfoArray[GAME_PLAYER];
        ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

        //��˰
        LONGLONG lTax[GAME_PLAYER];
        ZeroMemory(lTax,sizeof(lTax));
        for(WORD i = 0; i < GAME_PLAYER; i++)
        {
          if(!m_cbPlayStatus[i])
          {
            continue;
          }

          if(GAME_GENRE_SCORE != m_pGameServiceOption->wServerType &&
             lGameScore[i] > 0L)
          {
            lTax[i] =lGameScore[i]*m_pGameServiceOption->wRevenueRatio/1000L;
            lGameScore[i] -= lTax[i];
          }

          if(lGameScore[i] > 0L)
          {
            ScoreInfoArray[i].cbType = SCORE_TYPE_WIN;
          }
          else if(lGameScore[i] < 0L)
          {
            ScoreInfoArray[i].cbType = SCORE_TYPE_LOSE;
          }
          else
          {
            ScoreInfoArray[i].cbType = SCORE_TYPE_DRAW;
          }
          GameEnd.lGameTax[i] = lTax[i];
          GameEnd.lGameScore[i] = lGameScore[i];
        }

        //��ֹ����
        for(WORD i = 0; i < GAME_PLAYER; i++)
        {
          if(m_cbPlayStatus[i]==FALSE)
          {
            continue;
          }

          if(GameEnd.lGameScore[i] < 0L)
          {
            ASSERT(m_pITableFrame->GetTableUserItem(i)->GetUserScore() >= -GameEnd.lGameScore[i]);
            if(m_pITableFrame->GetTableUserItem(i)->GetUserScore() < -GameEnd.lGameScore[i])
            {
              GameEnd.lGameScore[i] = -m_pITableFrame->GetTableUserItem(i)->GetUserScore();
            }
          }
          ScoreInfoArray[i].lRevenue =GameEnd.lGameTax[i];
          ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];
        }

        //������Ϣ
        m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
        m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

        //д��
        m_pITableFrame->WriteTableScore(ScoreInfoArray,GAME_PLAYER);

        //������Ϸ
        m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
      }
      //����
      else
      {
        LONGLONG lGameScore = -(m_lTableScore[wChairID*2]+m_lTableScore[wChairID*2+1]+m_lInsureScore[wChairID*2]+m_lInsureScore[wChairID*2+1]);
        //��ֹ����
        ASSERT(-lGameScore <= m_pITableFrame->GetTableUserItem(wChairID)->GetUserScore());
        if(-lGameScore > m_pITableFrame->GetTableUserItem(wChairID)->GetUserScore())
        {
          lGameScore = -m_pITableFrame->GetTableUserItem(wChairID)->GetUserScore();
        }
        tagScoreInfo ScoreInfo;
        ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
        ScoreInfo.lScore=lGameScore;
        ScoreInfo.cbType=SCORE_TYPE_FLEE;
        m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);

        //����״̬
        m_cbPlayStatus[wChairID] = FALSE;

        //���û���м�
        BYTE cbPlayCount = 0;
        for(WORD i = 0; i < GAME_PLAYER; i++)
        {
          if(m_cbPlayStatus[i])
          {
            cbPlayCount++;
          }
        }

        if(cbPlayCount < 2)
        {
          return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);
        }
      }

      //���·����û���Ϣ
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        //��ȡ�û�
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);

        if(!pIServerUserItem)
        {
          continue;
        }

        UpdateRoomUserInfo(pIServerUserItem, USER_SITDOWN);
      }

      //���Ϳ��
      CString strInfo;
      strInfo.Format(TEXT("��ǰ��棺%I64d"), g_lRoomStorageCurrent);
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
        if(!pIServerUserItem)
        {
          continue;
        }
        if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) && !pIServerUserItem->IsAndroidUser())
        {
          m_pITableFrame->SendGameMessage(pIServerUserItem, strInfo, SMT_CHAT);

          CMD_S_ADMIN_STORAGE_INFO StorageInfo;
          ZeroMemory(&StorageInfo, sizeof(StorageInfo));
          StorageInfo.lRoomStorageStart = g_lRoomStorageStart;
          StorageInfo.lRoomStorageCurrent = g_lRoomStorageCurrent;
          StorageInfo.lRoomStorageDeduct = g_lStorageDeductRoom;
          StorageInfo.lMaxRoomStorage[0] = g_lStorageMax1Room;
          StorageInfo.lMaxRoomStorage[1] = g_lStorageMax2Room;
          StorageInfo.wRoomStorageMul[0] = (WORD)g_lStorageMul1Room;
          StorageInfo.wRoomStorageMul[1] = (WORD)g_lStorageMul2Room;
          m_pITableFrame->SendTableData(i, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
          m_pITableFrame->SendLookonData(i, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
        }
      }

      return true;
    }
    case GER_DISMISS:
    {
      //�������
      CMD_S_GameEnd GameEnd;
      ZeroMemory(&GameEnd,sizeof(GameEnd));

      //������Ϣ
      m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
      m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

      //������Ϸ
      m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

      //���·����û���Ϣ
      for(WORD i=0; i<GAME_PLAYER; i++)
      {
        //��ȡ�û�
        IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);

        if(!pIServerUserItem)
        {
          continue;
        }

        UpdateRoomUserInfo(pIServerUserItem, USER_STANDUP);
      }

      return true;
    }
  }

  ASSERT(FALSE);
  return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
  switch(cbGameStatus)
  {
    case GAME_SCENE_FREE:   //����״̬
    {
      //��������
      CMD_S_StatusFree StatusFree;
      ZeroMemory(&StatusFree,sizeof(StatusFree));
      StatusFree.lRoomStorageStart = g_lRoomStorageStart;
      StatusFree.lRoomStorageCurrent = g_lRoomStorageCurrent;

      //��ȡ�Զ�������
      tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
      ASSERT(pCustomRule);
      tagCustomAndroid CustomAndroid;
      ZeroMemory(&CustomAndroid, sizeof(CustomAndroid));
      CustomAndroid.lRobotBankGet = pCustomRule->lRobotBankGet;
      CustomAndroid.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
      CustomAndroid.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
      CustomAndroid.lRobotScoreMax = pCustomRule->lRobotScoreMax;
      CustomAndroid.lRobotScoreMin = pCustomRule->lRobotScoreMin;
      CopyMemory(&StatusFree.CustomAndroid, &CustomAndroid, sizeof(CustomAndroid));

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) && !pIServerUserItem->IsAndroidUser())
      {
        CMD_S_ADMIN_STORAGE_INFO StorageInfo;
        ZeroMemory(&StorageInfo, sizeof(StorageInfo));
        StorageInfo.lRoomStorageStart = g_lRoomStorageStart;
        StorageInfo.lRoomStorageCurrent = g_lRoomStorageCurrent;
        StorageInfo.lRoomStorageDeduct = g_lStorageDeductRoom;
        StorageInfo.lMaxRoomStorage[0] = g_lStorageMax1Room;
        StorageInfo.lMaxRoomStorage[1] = g_lStorageMax2Room;
        StorageInfo.wRoomStorageMul[0] = (WORD)g_lStorageMul1Room;
        StorageInfo.wRoomStorageMul[1] = (WORD)g_lStorageMul2Room;

        m_pITableFrame->SendTableData(wChairID, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
        m_pITableFrame->SendLookonData(wChairID, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
      }

      //���ͳ���
      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
    }
    case GAME_SCENE_ADD_SCORE:
    {
      CMD_S_StatusAddScore StatusScore;

      //��ע����
      StatusScore.lCellScore = m_lCellScore;
      StatusScore.lMaxScore = m_lMaxScore;

      //ׯ��
      StatusScore.wBankerUser = m_wBankerUser;

      //������ע
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        StatusScore.lTableScore[i] = m_lTableScore[i*2]+m_lTableScore[i*2+1]+m_lInsureScore[i*2]+m_lInsureScore[i*2+1];
      }
      StatusScore.lRoomStorageStart = g_lRoomStorageStart;
      StatusScore.lRoomStorageCurrent = g_lRoomStorageCurrent;

      //��ȡ�Զ�������
      tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
      ASSERT(pCustomRule);
      tagCustomAndroid CustomAndroid;
      ZeroMemory(&CustomAndroid, sizeof(CustomAndroid));
      CustomAndroid.lRobotBankGet = pCustomRule->lRobotBankGet;
      CustomAndroid.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
      CustomAndroid.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
      CustomAndroid.lRobotScoreMax = pCustomRule->lRobotScoreMax;
      CustomAndroid.lRobotScoreMin = pCustomRule->lRobotScoreMin;
      CopyMemory(&StatusScore.CustomAndroid, &CustomAndroid, sizeof(CustomAndroid));

      //���·����û���Ϣ
      UpdateRoomUserInfo(pIServerUserItem, USER_RECONNECT);

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) && !pIServerUserItem->IsAndroidUser())
      {
        CMD_S_ADMIN_STORAGE_INFO StorageInfo;
        ZeroMemory(&StorageInfo, sizeof(StorageInfo));
        StorageInfo.lRoomStorageStart = g_lRoomStorageStart;
        StorageInfo.lRoomStorageCurrent = g_lRoomStorageCurrent;
        StorageInfo.lRoomStorageDeduct = g_lStorageDeductRoom;
        StorageInfo.lMaxRoomStorage[0] = g_lStorageMax1Room;
        StorageInfo.lMaxRoomStorage[1] = g_lStorageMax2Room;
        StorageInfo.wRoomStorageMul[0] = (WORD)g_lStorageMul1Room;
        StorageInfo.wRoomStorageMul[1] = (WORD)g_lStorageMul2Room;
        m_pITableFrame->SendTableData(wChairID, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
        m_pITableFrame->SendLookonData(wChairID, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
      }

      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
    }
    case GAME_SCENE_GET_CARD: //��Ϸ״̬
    {
      CMD_S_StatusGetCard StatusGetCard;

      //��ע����
      StatusGetCard.lCellScore = m_lCellScore;

      //ׯ��
      StatusGetCard.wBankerUser = m_wBankerUser;

      //������ע
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        StatusGetCard.lTableScore[i] = m_lTableScore[i*2]+m_lTableScore[i*2+1]+m_lInsureScore[i*2]+m_lInsureScore[i*2+1];
      }

      //�˿˱���
      CopyMemory(StatusGetCard.cbCardCount,m_cbCardCount,sizeof(StatusGetCard.cbCardCount));
      CopyMemory(StatusGetCard.cbHandCardData,m_cbHandCardData,sizeof(StatusGetCard.cbHandCardData));

      //�����˿�
      for(WORD i = 0; i < GAME_PLAYER; i++)
      {
        if(i == wChairID)
        {
          continue;
        }

        StatusGetCard.cbHandCardData[i*2][0] = 0;
        StatusGetCard.cbHandCardData[i*2+1][0] = 0;
      }

      CopyMemory(StatusGetCard.bStopCard,m_bStopCard,sizeof(StatusGetCard.bStopCard));
      CopyMemory(StatusGetCard.bDoubleCard,m_bDoubleCard,sizeof(StatusGetCard.bDoubleCard));
      CopyMemory(StatusGetCard.bInsureCard,m_bInsureCard,sizeof(StatusGetCard.bInsureCard));

      StatusGetCard.lRoomStorageStart = g_lRoomStorageStart;
      StatusGetCard.lRoomStorageCurrent = g_lRoomStorageCurrent;

      //��ȡ�Զ�������
      tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
      ASSERT(pCustomRule);
      tagCustomAndroid CustomAndroid;
      ZeroMemory(&CustomAndroid, sizeof(CustomAndroid));
      CustomAndroid.lRobotBankGet = pCustomRule->lRobotBankGet;
      CustomAndroid.lRobotBankGetBanker = pCustomRule->lRobotBankGetBanker;
      CustomAndroid.lRobotBankStoMul = pCustomRule->lRobotBankStoMul;
      CustomAndroid.lRobotScoreMax = pCustomRule->lRobotScoreMax;
      CustomAndroid.lRobotScoreMin = pCustomRule->lRobotScoreMin;
      CopyMemory(&StatusGetCard.CustomAndroid, &CustomAndroid, sizeof(CustomAndroid));

      //���·����û���Ϣ
      UpdateRoomUserInfo(pIServerUserItem, USER_RECONNECT);

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) && !pIServerUserItem->IsAndroidUser())
      {
        CMD_S_ADMIN_STORAGE_INFO StorageInfo;
        ZeroMemory(&StorageInfo, sizeof(StorageInfo));
        StorageInfo.lRoomStorageStart = g_lRoomStorageStart;
        StorageInfo.lRoomStorageCurrent = g_lRoomStorageCurrent;
        StorageInfo.lRoomStorageDeduct = g_lStorageDeductRoom;
        StorageInfo.lMaxRoomStorage[0] = g_lStorageMax1Room;
        StorageInfo.lMaxRoomStorage[1] = g_lStorageMax2Room;
        StorageInfo.wRoomStorageMul[0] = (WORD)g_lStorageMul1Room;
        StorageInfo.wRoomStorageMul[1] = (WORD)g_lStorageMul2Room;
        m_pITableFrame->SendTableData(wChairID, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
        m_pITableFrame->SendLookonData(wChairID, SUB_S_ADMIN_STORAGE_INFO, &StorageInfo, sizeof(StorageInfo));
      }

      //���ͳ���
      return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusGetCard,sizeof(StatusGetCard));;
    }
  }

  //Ч�����
  ASSERT(FALSE);

  return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
  return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  switch(wSubCmdID)
  {
    case SUB_C_ADD_SCORE:     //�û���ע
    {
      //Ч������
      ASSERT(wDataSize==sizeof(CMD_C_AddScore));
      if(wDataSize!=sizeof(CMD_C_AddScore))
      {
        return false;
      }

      //��������
      CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //״̬�ж�
      ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
      if(m_cbPlayStatus[pUserData->wChairID]==FALSE)
      {
        return false;
      }

      ASSERT(m_pITableFrame->GetGameStatus() == GAME_SCENE_ADD_SCORE);
      if(m_pITableFrame->GetGameStatus() != GAME_SCENE_ADD_SCORE)
      {
        return false;
      }

      //��Ϣ����
      return OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
    }
    case SUB_C_GET_CARD:
    {
      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //״̬�ж�
      ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
      if(m_cbPlayStatus[pUserData->wChairID]==FALSE)
      {
        return false;
      }

      ASSERT(m_pITableFrame->GetGameStatus() == GAME_SCENE_GET_CARD);
      if(m_pITableFrame->GetGameStatus() != GAME_SCENE_GET_CARD)
      {
        return false;
      }

      return OnUserGetCard(pUserData->wChairID);
    }
    case SUB_C_STOP_CARD:       //�û�����
    {
      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //״̬�ж�
      ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
      if(m_cbPlayStatus[pUserData->wChairID]==FALSE)
      {
        return false;
      }

      ASSERT(m_pITableFrame->GetGameStatus() == GAME_SCENE_GET_CARD);
      if(m_pITableFrame->GetGameStatus() != GAME_SCENE_GET_CARD)
      {
        return false;
      }

      //��Ϣ����
      return OnUserStopCard(pUserData->wChairID);
    }
    case SUB_C_DOUBLE_SCORE:
    {
      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //״̬�ж�
      ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
      if(m_cbPlayStatus[pUserData->wChairID]==FALSE)
      {
        return false;
      }

      ASSERT(m_pITableFrame->GetGameStatus() == GAME_SCENE_GET_CARD);
      if(m_pITableFrame->GetGameStatus() != GAME_SCENE_GET_CARD)
      {
        return false;
      }

      return OnUserDoubleScore(pUserData->wChairID);
    }
    case SUB_C_INSURE:
    {
      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //״̬�ж�
      ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
      if(m_cbPlayStatus[pUserData->wChairID]==FALSE)
      {
        return false;
      }

      ASSERT(m_pITableFrame->GetGameStatus() == GAME_SCENE_GET_CARD);
      if(m_pITableFrame->GetGameStatus() != GAME_SCENE_GET_CARD)
      {
        return false;
      }

      return OnUserInsure(pUserData->wChairID);
    }
    case SUB_C_SPLIT_CARD:
    {
      //�û�Ч��
      tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
      if(pUserData->cbUserStatus!=US_PLAYING)
      {
        return true;
      }

      //״̬�ж�
      ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
      if(m_cbPlayStatus[pUserData->wChairID]==FALSE)
      {
        return false;
      }

      ASSERT(m_pITableFrame->GetGameStatus() == GAME_SCENE_GET_CARD);
      if(m_pITableFrame->GetGameStatus() != GAME_SCENE_GET_CARD)
      {
        return false;
      }

      return OnUserSplitCard(pUserData->wChairID);
    }
    case SUB_C_STORAGE:
    {
      ASSERT(wDataSize==sizeof(CMD_C_UpdateStorage));
      if(wDataSize!=sizeof(CMD_C_UpdateStorage))
      {
        return false;
      }

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
      {
        return false;
      }

      CMD_C_UpdateStorage *pUpdateStorage=(CMD_C_UpdateStorage *)pData;
      g_lRoomStorageCurrent = pUpdateStorage->lRoomStorageCurrent;
      g_lStorageDeductRoom = pUpdateStorage->lRoomStorageDeduct;

      //20��������¼
      if(g_ListOperationRecord.GetSize() == MAX_OPERATION_RECORD)
      {
        g_ListOperationRecord.RemoveHead();
      }

      CString strOperationRecord;
      CTime time = CTime::GetCurrentTime();
      strOperationRecord.Format(TEXT("����ʱ��: %d/%d/%d-%d:%d:%d, �����˻�[%s],�޸ĵ�ǰ���Ϊ %I64d,˥��ֵΪ %I64d"),
                                time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), pIServerUserItem->GetNickName(),
                                g_lRoomStorageCurrent, g_lStorageDeductRoom);

      g_ListOperationRecord.AddTail(strOperationRecord);

      //д����־
      strOperationRecord += TEXT("\n");
      WriteInfo(strOperationRecord);

      //��������
      CMD_S_Operation_Record OperationRecord;
      ZeroMemory(&OperationRecord, sizeof(OperationRecord));
      POSITION posListRecord = g_ListOperationRecord.GetHeadPosition();
      WORD wIndex = 0;//�����±�
      while(posListRecord)
      {
        CString strRecord = g_ListOperationRecord.GetNext(posListRecord);

        CopyMemory(OperationRecord.szRecord[wIndex], strRecord, sizeof(OperationRecord.szRecord[wIndex]));
        wIndex++;
      }

      ASSERT(wIndex <= MAX_OPERATION_RECORD);

      //��������
      m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));
      m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));

      return true;
    }
    case SUB_C_STORAGEMAXMUL:
    {
      ASSERT(wDataSize==sizeof(CMD_C_ModifyStorage));
      if(wDataSize!=sizeof(CMD_C_ModifyStorage))
      {
        return false;
      }

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
      {
        return false;
      }

      CMD_C_ModifyStorage *pModifyStorage = (CMD_C_ModifyStorage *)pData;
      g_lStorageMax1Room = pModifyStorage->lMaxRoomStorage[0];
      g_lStorageMax2Room = pModifyStorage->lMaxRoomStorage[1];
      g_lStorageMul1Room = (SCORE)(pModifyStorage->wRoomStorageMul[0]);
      g_lStorageMul2Room = (SCORE)(pModifyStorage->wRoomStorageMul[1]);

      //20��������¼
      if(g_ListOperationRecord.GetSize() == MAX_OPERATION_RECORD)
      {
        g_ListOperationRecord.RemoveHead();
      }

      CString strOperationRecord;
      CTime time = CTime::GetCurrentTime();
      strOperationRecord.Format(TEXT("����ʱ��: %d/%d/%d-%d:%d:%d,�����˻�[%s], �޸Ŀ������ֵ1Ϊ %I64d,Ӯ�ָ���1Ϊ %I64d,����ֵ2Ϊ %I64d,Ӯ�ָ���2Ϊ %I64d"),
                                time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), pIServerUserItem->GetNickName(), g_lStorageMax1Room, g_lStorageMul1Room, g_lStorageMax2Room, g_lStorageMul2Room);

      g_ListOperationRecord.AddTail(strOperationRecord);

      //д����־
      strOperationRecord += TEXT("\n");
      WriteInfo(strOperationRecord);

      //��������
      CMD_S_Operation_Record OperationRecord;
      ZeroMemory(&OperationRecord, sizeof(OperationRecord));
      POSITION posListRecord = g_ListOperationRecord.GetHeadPosition();
      WORD wIndex = 0;//�����±�
      while(posListRecord)
      {
        CString strRecord = g_ListOperationRecord.GetNext(posListRecord);

        CopyMemory(OperationRecord.szRecord[wIndex], strRecord, sizeof(OperationRecord.szRecord[wIndex]));
        wIndex++;
      }

      ASSERT(wIndex <= MAX_OPERATION_RECORD);

      //��������
      m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));
      m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));

      return true;
    }
    case SUB_C_REQUEST_QUERY_USER:
    {
      ASSERT(wDataSize == sizeof(CMD_C_RequestQuery_User));
      if(wDataSize != sizeof(CMD_C_RequestQuery_User))
      {
        return false;
      }

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false || pIServerUserItem->IsAndroidUser())
      {
        return false;
      }

      CMD_C_RequestQuery_User *pQuery_User = (CMD_C_RequestQuery_User *)pData;

      //����ӳ��
      POSITION ptHead = g_MapRoomUserInfo.GetStartPosition();
      DWORD dwUserID = 0;
      ROOMUSERINFO userinfo;
      ZeroMemory(&userinfo, sizeof(userinfo));

      CMD_S_RequestQueryResult QueryResult;
      ZeroMemory(&QueryResult, sizeof(QueryResult));

      while(ptHead)
      {
        g_MapRoomUserInfo.GetNextAssoc(ptHead, dwUserID, userinfo);
        if(pQuery_User->dwGameID == userinfo.dwGameID || _tcscmp(pQuery_User->szNickName, userinfo.szNickName) == 0)
        {
          //�����û���Ϣ����
          QueryResult.bFind = true;
          CopyMemory(&(QueryResult.userinfo), &userinfo, sizeof(userinfo));

          ZeroMemory(&g_CurrentQueryUserInfo, sizeof(g_CurrentQueryUserInfo));
          CopyMemory(&(g_CurrentQueryUserInfo), &userinfo, sizeof(userinfo));
        }
      }

      //��������
      m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_REQUEST_QUERY_RESULT, &QueryResult, sizeof(QueryResult));
      m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_REQUEST_QUERY_RESULT, &QueryResult, sizeof(QueryResult));

      return true;
    }
    case SUB_C_USER_CONTROL:
    {
      ASSERT(wDataSize == sizeof(CMD_C_UserControl));
      if(wDataSize != sizeof(CMD_C_UserControl))
      {
        return false;
      }

      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false || pIServerUserItem->IsAndroidUser() == true)
      {
        return false;
      }

      CMD_C_UserControl *pUserControl = (CMD_C_UserControl *)pData;

      //����ӳ��
      POSITION ptMapHead = g_MapRoomUserInfo.GetStartPosition();
      DWORD dwUserID = 0;
      ROOMUSERINFO userinfo;
      ZeroMemory(&userinfo, sizeof(userinfo));

      //20��������¼
      if(g_ListOperationRecord.GetSize() == MAX_OPERATION_RECORD)
      {
        g_ListOperationRecord.RemoveHead();
      }

      //��������
      CMD_S_UserControl serverUserControl;
      ZeroMemory(&serverUserControl, sizeof(serverUserControl));

      //�������
      if(pUserControl->userControlInfo.bCancelControl == false)
      {
        ASSERT(pUserControl->userControlInfo.control_type == CONTINUE_WIN || pUserControl->userControlInfo.control_type == CONTINUE_LOST);

        while(ptMapHead)
        {
          g_MapRoomUserInfo.GetNextAssoc(ptMapHead, dwUserID, userinfo);
          if(pUserControl->dwGameID == userinfo.dwGameID || _tcscmp(pUserControl->szNickName, userinfo.szNickName) == 0)
          {
            //������Ʊ�ʶ
            bool bEnableControl = false;
            IsSatisfyControl(userinfo, bEnableControl);

            //�������
            if(bEnableControl)
            {
              ROOMUSERCONTROL roomusercontrol;
              ZeroMemory(&roomusercontrol, sizeof(roomusercontrol));
              CopyMemory(&(roomusercontrol.roomUserInfo), &userinfo, sizeof(userinfo));
              CopyMemory(&(roomusercontrol.userControl), &(pUserControl->userControlInfo), sizeof(roomusercontrol.userControl));


              //������������
              TravelControlList(roomusercontrol);

              //ѹ��������ѹ��ͬGAMEID��NICKNAME)
              g_ListRoomUserControl.AddHead(roomusercontrol);

              //��������
              serverUserControl.dwGameID = userinfo.dwGameID;
              CopyMemory(serverUserControl.szNickName, userinfo.szNickName, sizeof(userinfo.szNickName));
              serverUserControl.controlResult = CONTROL_SUCCEED;
              serverUserControl.controlType = pUserControl->userControlInfo.control_type;
              serverUserControl.cbControlCount = pUserControl->userControlInfo.cbControlCount;

              //������¼
              CString strOperationRecord;
              CString strControlType;
              GetControlTypeString(serverUserControl.controlType, strControlType);
              CTime time = CTime::GetCurrentTime();
              strOperationRecord.Format(TEXT("����ʱ��: %d/%d/%d-%d:%d:%d, �����˻�[%s], �������%s,%s,���ƾ���%d "),
                                        time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), pIServerUserItem->GetNickName(), serverUserControl.szNickName, strControlType, serverUserControl.cbControlCount);

              g_ListOperationRecord.AddTail(strOperationRecord);

              //д����־
              strOperationRecord += TEXT("\n");
              WriteInfo(strOperationRecord);
            }
            else  //������
            {
              //��������
              serverUserControl.dwGameID = userinfo.dwGameID;
              CopyMemory(serverUserControl.szNickName, userinfo.szNickName, sizeof(userinfo.szNickName));
              serverUserControl.controlResult = CONTROL_FAIL;
              serverUserControl.controlType = pUserControl->userControlInfo.control_type;
              serverUserControl.cbControlCount = 0;

              //������¼
              CString strOperationRecord;
              CString strControlType;
              GetControlTypeString(serverUserControl.controlType, strControlType);
              CTime time = CTime::GetCurrentTime();
              strOperationRecord.Format(TEXT("����ʱ��: %d/%d/%d-%d:%d:%d, �����˻�[%s], �������%s,%s,ʧ�ܣ�"),
                                        time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), pIServerUserItem->GetNickName(), serverUserControl.szNickName, strControlType);

              g_ListOperationRecord.AddTail(strOperationRecord);

              //д����־
              strOperationRecord += TEXT("\n");
              WriteInfo(strOperationRecord);
            }

            //��������
            m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_USER_CONTROL, &serverUserControl, sizeof(serverUserControl));
            m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_USER_CONTROL, &serverUserControl, sizeof(serverUserControl));

            CMD_S_Operation_Record OperationRecord;
            ZeroMemory(&OperationRecord, sizeof(OperationRecord));
            POSITION posListRecord = g_ListOperationRecord.GetHeadPosition();
            WORD wIndex = 0;//�����±�
            while(posListRecord)
            {
              CString strRecord = g_ListOperationRecord.GetNext(posListRecord);

              CopyMemory(OperationRecord.szRecord[wIndex], strRecord, sizeof(OperationRecord.szRecord[wIndex]));
              wIndex++;
            }

            ASSERT(wIndex <= MAX_OPERATION_RECORD);

            //��������
            m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));
            m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));

            return true;
          }
        }

        ASSERT(FALSE);
        return false;
      }
      else  //ȡ������
      {
        ASSERT(pUserControl->userControlInfo.control_type == CONTINUE_CANCEL);

        POSITION ptListHead = g_ListRoomUserControl.GetHeadPosition();
        POSITION ptTemp;
        ROOMUSERCONTROL roomusercontrol;
        ZeroMemory(&roomusercontrol, sizeof(roomusercontrol));

        //��������
        while(ptListHead)
        {
          ptTemp = ptListHead;
          roomusercontrol = g_ListRoomUserControl.GetNext(ptListHead);
          if(pUserControl->dwGameID == roomusercontrol.roomUserInfo.dwGameID || _tcscmp(pUserControl->szNickName, roomusercontrol.roomUserInfo.szNickName) == 0)
          {
            //��������
            serverUserControl.dwGameID = roomusercontrol.roomUserInfo.dwGameID;
            CopyMemory(serverUserControl.szNickName, roomusercontrol.roomUserInfo.szNickName, sizeof(roomusercontrol.roomUserInfo.szNickName));
            serverUserControl.controlResult = CONTROL_CANCEL_SUCCEED;
            serverUserControl.controlType = pUserControl->userControlInfo.control_type;
            serverUserControl.cbControlCount = 0;

            //�Ƴ�Ԫ��
            g_ListRoomUserControl.RemoveAt(ptTemp);

            //��������
            m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_USER_CONTROL, &serverUserControl, sizeof(serverUserControl));
            m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_USER_CONTROL, &serverUserControl, sizeof(serverUserControl));

            //������¼
            CString strOperationRecord;
            CTime time = CTime::GetCurrentTime();
            strOperationRecord.Format(TEXT("����ʱ��: %d/%d/%d-%d:%d:%d, �����˻�[%s], ȡ�������%s�Ŀ��ƣ�"),
                                      time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), pIServerUserItem->GetNickName(), serverUserControl.szNickName);

            g_ListOperationRecord.AddTail(strOperationRecord);

            //д����־
            strOperationRecord += TEXT("\n");
            WriteInfo(strOperationRecord);

            CMD_S_Operation_Record OperationRecord;
            ZeroMemory(&OperationRecord, sizeof(OperationRecord));
            POSITION posListRecord = g_ListOperationRecord.GetHeadPosition();
            WORD wIndex = 0;//�����±�
            while(posListRecord)
            {
              CString strRecord = g_ListOperationRecord.GetNext(posListRecord);

              CopyMemory(OperationRecord.szRecord[wIndex], strRecord, sizeof(OperationRecord.szRecord[wIndex]));
              wIndex++;
            }

            ASSERT(wIndex <= MAX_OPERATION_RECORD);

            //��������
            m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));
            m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));

            return true;
          }
        }

        //��������
        serverUserControl.dwGameID = pUserControl->dwGameID;
        CopyMemory(serverUserControl.szNickName, pUserControl->szNickName, sizeof(serverUserControl.szNickName));
        serverUserControl.controlResult = CONTROL_CANCEL_INVALID;
        serverUserControl.controlType = pUserControl->userControlInfo.control_type;
        serverUserControl.cbControlCount = 0;

        //��������
        m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_USER_CONTROL, &serverUserControl, sizeof(serverUserControl));
        m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_USER_CONTROL, &serverUserControl, sizeof(serverUserControl));

        //������¼
        CString strOperationRecord;
        CTime time = CTime::GetCurrentTime();
        strOperationRecord.Format(TEXT("����ʱ��: %d/%d/%d-%d:%d:%d, �����˻�[%s], ȡ�������%s�Ŀ��ƣ�������Ч��"),
                                  time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), pIServerUserItem->GetNickName(), serverUserControl.szNickName);

        g_ListOperationRecord.AddTail(strOperationRecord);

        //д����־
        strOperationRecord += TEXT("\n");
        WriteInfo(strOperationRecord);

        CMD_S_Operation_Record OperationRecord;
        ZeroMemory(&OperationRecord, sizeof(OperationRecord));
        POSITION posListRecord = g_ListOperationRecord.GetHeadPosition();
        WORD wIndex = 0;//�����±�
        while(posListRecord)
        {
          CString strRecord = g_ListOperationRecord.GetNext(posListRecord);

          CopyMemory(OperationRecord.szRecord[wIndex], strRecord, sizeof(OperationRecord.szRecord[wIndex]));
          wIndex++;
        }

        ASSERT(wIndex <= MAX_OPERATION_RECORD);

        //��������
        m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));
        m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_OPERATION_RECORD, &OperationRecord, sizeof(OperationRecord));

      }

      return true;
    }
    case SUB_C_REQUEST_UDPATE_ROOMINFO:
    {
      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false || pIServerUserItem->IsAndroidUser() == true)
      {
        return false;
      }

      CMD_S_RequestUpdateRoomInfo_Result RoomInfo_Result;
      ZeroMemory(&RoomInfo_Result, sizeof(RoomInfo_Result));

      RoomInfo_Result.lRoomStorageCurrent = g_lRoomStorageCurrent;


      DWORD dwKeyGameID = g_CurrentQueryUserInfo.dwGameID;
      TCHAR szKeyNickName[LEN_NICKNAME];
      ZeroMemory(szKeyNickName, sizeof(szKeyNickName));
      CopyMemory(szKeyNickName, g_CurrentQueryUserInfo.szNickName, sizeof(szKeyNickName));

      //����ӳ��
      POSITION ptHead = g_MapRoomUserInfo.GetStartPosition();
      DWORD dwUserID = 0;
      ROOMUSERINFO userinfo;
      ZeroMemory(&userinfo, sizeof(userinfo));

      while(ptHead)
      {
        g_MapRoomUserInfo.GetNextAssoc(ptHead, dwUserID, userinfo);
        if(dwKeyGameID == userinfo.dwGameID && _tcscmp(szKeyNickName, userinfo.szNickName) == 0)
        {
          //�����û���Ϣ����
          CopyMemory(&(RoomInfo_Result.currentqueryuserinfo), &userinfo, sizeof(userinfo));

          ZeroMemory(&g_CurrentQueryUserInfo, sizeof(g_CurrentQueryUserInfo));
          CopyMemory(&(g_CurrentQueryUserInfo), &userinfo, sizeof(userinfo));
        }
      }

      //
      //��������
      POSITION ptListHead = g_ListRoomUserControl.GetHeadPosition();
      POSITION ptTemp;
      ROOMUSERCONTROL roomusercontrol;
      ZeroMemory(&roomusercontrol, sizeof(roomusercontrol));

      //��������
      while(ptListHead)
      {
        ptTemp = ptListHead;
        roomusercontrol = g_ListRoomUserControl.GetNext(ptListHead);

        //Ѱ�����
        if((dwKeyGameID == roomusercontrol.roomUserInfo.dwGameID) &&
           _tcscmp(szKeyNickName, roomusercontrol.roomUserInfo.szNickName) == 0)
        {
          RoomInfo_Result.bExistControl = true;
          CopyMemory(&(RoomInfo_Result.currentusercontrol), &(roomusercontrol.userControl), sizeof(roomusercontrol.userControl));
          break;
        }
      }

      //��������
      m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(), SUB_S_REQUEST_UDPATE_ROOMINFO_RESULT, &RoomInfo_Result, sizeof(RoomInfo_Result));
      m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(), SUB_S_REQUEST_UDPATE_ROOMINFO_RESULT, &RoomInfo_Result, sizeof(RoomInfo_Result));

      return true;
    }
    case SUB_C_CLEAR_CURRENT_QUERYUSER:
    {
      //Ȩ���ж�
      if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false || pIServerUserItem->IsAndroidUser() == true)
      {
        return false;
      }

      ZeroMemory(&g_CurrentQueryUserInfo, sizeof(g_CurrentQueryUserInfo));

      return true;
    }
  }

  return false;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
  return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  if(!bLookonUser && m_wBankerUser == INVALID_CHAIR)
  {
    m_wBankerUser = pIServerUserItem->GetChairID();
  }

  //���·����û���Ϣ
  UpdateRoomUserInfo(pIServerUserItem, USER_SITDOWN);

  //����ͬ���û�����
  UpdateUserControl(pIServerUserItem);

  return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
  if(!bLookonUser)
  {
    if(wChairID == m_wBankerUser)
    {
      m_wBankerUser = INVALID_CHAIR;
      for(WORD i = (wChairID+1)%GAME_PLAYER; i != wChairID; i = (i+1)%GAME_PLAYER)
      {
        if(m_pITableFrame->GetTableUserItem(i))
        {
          m_wBankerUser = i;
          break;
        }
      }
    }
  }

  //���·����û���Ϣ
  UpdateRoomUserInfo(pIServerUserItem, USER_STANDUP);

  return true;
}

//�����¼�
bool CTableFrameSink::OnUserStopCard(WORD wChairID)
{
  //ȷ����������
  BYTE cbOperateIndex = wChairID*2;
  if(m_bStopCard[cbOperateIndex])
  {
    cbOperateIndex++;
  }

  //Ч��
  ASSERT(m_bStopCard[cbOperateIndex] == FALSE);
  if(m_bStopCard[cbOperateIndex])
  {
    return true;
  }

  //����ͣ��
  m_bStopCard[cbOperateIndex] = TRUE;

  //�������ׯ��ͣ��
  if(wChairID != m_wBankerUser)
  {
    CMD_S_StopCard StopCard;
    StopCard.wStopCardUser = wChairID;

    //����Ƿ���ȫ��ͣ��]
    bool bStopCardComplete = true;
    for(WORD i = 0; i < GAME_PLAYER; i++)
    {
      if(!m_cbPlayStatus[i] || i == m_wBankerUser)
      {
        continue;
      }
      if(!m_bStopCard[i*2] || (m_cbCardCount[i*2+1]>0 && !m_bStopCard[i*2+1]))
      {
        bStopCardComplete = false;
        break;
      }
    }
    //���ȫ��ͣ��
    if(bStopCardComplete)
    {
      StopCard.bTurnBanker = TRUE;
    }
    else
    {
      StopCard.bTurnBanker = FALSE;
    }

    //��������
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));
  }
  //����
  else
  {
    //��������
    CMD_S_StopCard StopCard;
    StopCard.wStopCardUser = wChairID;
    StopCard.bTurnBanker = FALSE;
    m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));
    m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STOP_CARD,&StopCard,sizeof(StopCard));

    //������Ϸ
    OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
  }

  return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore)
{
  //Ч��
  ASSERT(lScore > 0L && lScore <= m_lMaxScore &&(lScore<=m_lMaxUserScore[wChairID]));
  if(lScore <= 0L || lScore > m_lMaxScore ||lScore>m_lMaxUserScore[wChairID])
  {
    return false;
  }
  ASSERT(m_lTableScore[wChairID*2] == 0L);
  if(m_lTableScore[wChairID*2] != 0L)
  {
    return true;
  }

  //�����ע
  m_lTableScore[wChairID*2] = lScore;
  m_lTableScoreAll[wChairID]+=lScore;

  //��������
  CMD_S_AddScore AddScore;
  AddScore.wAddScoreUser = wChairID;
  AddScore.lAddScore = lScore;
  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));

  //�ж��Ƿ���ȫ����ע
  bool bAddScoreComplete = true;
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(!m_cbPlayStatus[i] || i == m_wBankerUser)
    {
      continue;
    }
    if(m_lTableScore[i*2] == 0L)
    {
      bAddScoreComplete = false;
      break;
    }
  }
  //����ȫ����ע
  if(bAddScoreComplete)
  {
    //������Ϸ״̬
    m_pITableFrame->SetGameStatus(GAME_SCENE_GET_CARD);

    //���·����û���Ϣ
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //��ȡ�û�
      IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
      if(pIServerUserItem != NULL)
      {
        UpdateRoomUserInfo(pIServerUserItem, USER_SITDOWN);
      }

    }

    //�����˿�
    m_GameLogic.RandCardList(m_cbRepertoryCard,FULL_COUNT);
    m_cbRepertoryCount = FULL_COUNT;

    CopyMemory(m_cbHandCardData[m_wBankerUser*2],&m_cbRepertoryCard[m_cbRepertoryCount-2],2*sizeof(BYTE));
    m_cbRepertoryCount -= 2;
    m_cbCardCount[m_wBankerUser*2] = 2;
    for(WORD i = (m_wBankerUser+1)%GAME_PLAYER; i != m_wBankerUser; i = (i+1)%GAME_PLAYER)
    {
      if(m_cbPlayStatus[i])
      {
        m_cbCardCount[i*2] = 2;
        CopyMemory(m_cbHandCardData[i*2],&m_cbRepertoryCard[m_cbRepertoryCount-2],sizeof(BYTE)*2);
        m_cbRepertoryCount -= 2;
      }
    }

    bool bWin = AnalyseCard(true);         //��ʱ�����⣬������

    //��������
    ROOMUSERCONTROL roomusercontrol;
    ZeroMemory(&roomusercontrol, sizeof(roomusercontrol));
    POSITION posKeyList;

    //����
    if(m_pServerControl != NULL && AnalyseRoomUserControl(roomusercontrol, posKeyList))
    {
      //У������
      ASSERT(roomusercontrol.roomUserInfo.wChairID != INVALID_CHAIR && roomusercontrol.userControl.cbControlCount != 0
             && roomusercontrol.userControl.control_type != CONTINUE_CANCEL);

      if(m_pServerControl->ControlResult(m_cbHandCardData, m_cbCardCount, roomusercontrol, m_pITableFrame, m_wBankerUser, m_cbPlayStatus))
      {
        //��ȡԪ��
        ROOMUSERCONTROL &tmproomusercontrol = g_ListRoomUserControl.GetAt(posKeyList);

        //У������
        ASSERT(roomusercontrol.userControl.cbControlCount == tmproomusercontrol.userControl.cbControlCount);

        //���ƾ���
        tmproomusercontrol.userControl.cbControlCount--;

        CMD_S_UserControlComplete UserControlComplete;
        ZeroMemory(&UserControlComplete, sizeof(UserControlComplete));
        UserControlComplete.dwGameID = roomusercontrol.roomUserInfo.dwGameID;
        CopyMemory(UserControlComplete.szNickName, roomusercontrol.roomUserInfo.szNickName, sizeof(UserControlComplete.szNickName));
        UserControlComplete.controlType = roomusercontrol.userControl.control_type;
        UserControlComplete.cbRemainControlCount = tmproomusercontrol.userControl.cbControlCount;

        for(WORD i=0; i<GAME_PLAYER; i++)
        {
          IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
          if(!pIServerUserItem)
          {
            continue;
          }
          if(pIServerUserItem->IsAndroidUser() == true || CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) == false)
          {
            continue;
          }

          //��������
          m_pITableFrame->SendTableData(i, SUB_S_USER_CONTROL_COMPLETE, &UserControlComplete, sizeof(UserControlComplete));
          m_pITableFrame->SendLookonData(i, SUB_S_USER_CONTROL_COMPLETE, &UserControlComplete, sizeof(UserControlComplete));

        }
      }
    }

    //��������
    CMD_S_SendCard SendCard;
    ZeroMemory(&SendCard,sizeof(SendCard));
    SendCard.bWin = bWin;

    for(WORD i = 0; i < GAME_PLAYER; i++)
    {
      SendCard.cbHandCardData[i][1] = m_cbHandCardData[i*2][1];
    }
    for(WORD i = 0; i < GAME_PLAYER; i++)
    {
      if(!m_cbPlayStatus[i])
      {
        continue;
      }

      SendCard.cbHandCardData[i][0] = m_cbHandCardData[i*2][0];

      m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
      m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
      SendCard.cbHandCardData[i][0] = 0;
    }

    if(m_pServerControl)

    {
      CMD_S_CheatCard CheatCard ;
      ZeroMemory(&CheatCard, sizeof(CheatCard));
      CopyMemory(CheatCard.cbCardData,m_cbHandCardData,sizeof(CheatCard.cbCardData));

      m_pServerControl->ServerControl(&CheatCard, m_pITableFrame);
    }
  }

  return true;
}

//��Ҽӱ�
bool CTableFrameSink::OnUserDoubleScore(WORD wChairID)
{

  //ȷ����������
  BYTE cbOperateIndex = wChairID*2;
  if(m_bStopCard[cbOperateIndex])
  {
    cbOperateIndex++;
  }

  //Ч��
  ASSERT(m_bStopCard[cbOperateIndex] == FALSE);
  if(m_bStopCard[cbOperateIndex])
  {
    return true;
  }

  ASSERT(m_cbCardCount[cbOperateIndex] == 2);
  if(m_cbCardCount[cbOperateIndex] != 2)
  {
    return false;
  }

  SCORE lScoreAll = m_lTableScoreAll[wChairID]+(m_lTableScore[cbOperateIndex] *2+m_lInsureScore[cbOperateIndex]*2);
  ASSERT(lScoreAll<=m_lMaxUserScore[wChairID]);
  if(lScoreAll>m_lMaxUserScore[wChairID])
  {
    return false;
  }

  //���üӱ���־
  m_bDoubleCard[cbOperateIndex] = TRUE;

  //�ӱ���ע�뱣�ս�
  m_lTableScore[cbOperateIndex] *= 2;
  m_lInsureScore[cbOperateIndex] *= 2;

  m_lTableScoreAll[wChairID]+=m_lTableScore[cbOperateIndex] ;
  m_lTableScoreAll[wChairID]+=m_lInsureScore[cbOperateIndex] ;

  //�����˿�
  BYTE cbGetCardData = m_cbRepertoryCard[--m_cbRepertoryCount];
  m_cbHandCardData[cbOperateIndex][m_cbCardCount[cbOperateIndex]++] = cbGetCardData;

  //��������
  CMD_S_DoubleScore DoubleScore;
  DoubleScore.cbCardData = cbGetCardData;
  DoubleScore.wDoubleScoreUser = wChairID;
  DoubleScore.lAddScore = m_lTableScore[cbOperateIndex]/2+m_lInsureScore[cbOperateIndex]/2;
  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_DOUBLE_SCORE,&DoubleScore,sizeof(DoubleScore));
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_DOUBLE_SCORE,&DoubleScore,sizeof(DoubleScore));

  //�Զ�ͣ��
  return OnUserStopCard(wChairID);
}

//���Ҫ��
bool CTableFrameSink::OnUserGetCard(WORD wChairID)
{
  //ȷ����������
  BYTE cbOperateIndex = wChairID*2;
  if(m_bStopCard[cbOperateIndex])
  {
    cbOperateIndex++;
  }

  //Ч��
  ASSERT(m_bStopCard[cbOperateIndex] == FALSE);
  if(m_bStopCard[cbOperateIndex])
  {
    return true;
  }

  //������Ҫ��תͣ��
  if(AnalyseGetCard(wChairID, cbOperateIndex))
  {
    //SendInfo(TEXT("����Ҫ��תͣ�ƣ�"));
    return true;
  }


  bool bWin = false;
  BYTE cbGetCardData=0;
  BYTE cbRepertoryCount = m_cbRepertoryCount;
  do
  {
    //�����˿�
    cbGetCardData = m_cbRepertoryCard[--cbRepertoryCount];
    m_cbHandCardData[cbOperateIndex][m_cbCardCount[cbOperateIndex]++] = cbGetCardData;
    bWin = AnalyseCardEx();
    m_cbCardCount[cbOperateIndex]--;
    if(!bWin)
    {
//      if (m_cbRepertoryCount - 1 > cbRepertoryCount)
//      {
//        SendInfo(TEXT("����Ҫ�ƣ�"));
//      }

      break;
    }

  }
  while(bWin && cbRepertoryCount > 0);


  if(!bWin && cbRepertoryCount != m_cbRepertoryCount - 1)     //�к��ʵ�
  {
    cbGetCardData = m_cbRepertoryCard[cbRepertoryCount];
    m_cbRepertoryCard[cbRepertoryCount] = m_cbRepertoryCard[m_cbRepertoryCount - 1];
    m_cbRepertoryCard[m_cbRepertoryCount - 1] = cbGetCardData;
  }

  cbGetCardData = m_cbRepertoryCard[--m_cbRepertoryCount];
  m_cbHandCardData[cbOperateIndex][m_cbCardCount[cbOperateIndex]++] = cbGetCardData;


  //��������
  CMD_S_GetCard GetCard;
  GetCard.cbCardData = cbGetCardData;
  GetCard.wGetCardUser = wChairID;
  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GET_CARD,&GetCard,sizeof(GetCard));
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GET_CARD,&GetCard,sizeof(GetCard));

  //�Ƿ����汬��
  BYTE cbCardType = m_GameLogic.GetCardType(&m_cbHandCardData[cbOperateIndex][1],m_cbCardCount[cbOperateIndex]-1,true);
  if(cbCardType == CT_BAOPAI || cbCardType == CT_BJ-1)
  {
    if(wChairID == m_wBankerUser)
    {
      return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
    }
    else
    {
      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

      if(pIServerUserItem->IsAndroidUser()==false)
      {
        return OnUserStopCard(wChairID);
      }
      else
      {
        return true;
      }
    }

  }

  return true;
}

//����±���
bool CTableFrameSink::OnUserInsure(WORD wChairID)
{


  //ȷ����������
  BYTE cbOperateIndex = wChairID*2;
  if(m_bStopCard[cbOperateIndex])
  {
    cbOperateIndex++;
  }

  SCORE lScoreAll = m_lTableScoreAll[wChairID]+m_lTableScore[cbOperateIndex]/2;
  ASSERT(lScoreAll<=m_lMaxUserScore[wChairID]);
  if(lScoreAll>m_lMaxUserScore[wChairID])
  {
    return false;
  }

  //Ч��
  ASSERT(m_cbCardCount[m_wBankerUser*2] == 2 && m_GameLogic.GetCardValue(m_cbHandCardData[m_wBankerUser*2][1]) == 1);
  if(m_cbCardCount[m_wBankerUser*2] != 2 || m_GameLogic.GetCardValue(m_cbHandCardData[m_wBankerUser*2][1]) != 1)
  {
    return false;
  }

  ASSERT(m_bStopCard[cbOperateIndex] == FALSE);
  if(m_bStopCard[cbOperateIndex])
  {
    return true;
  }
  ASSERT(m_lInsureScore[cbOperateIndex] == 0L);
  if(m_lInsureScore[cbOperateIndex] != 0L)
  {
    return true;
  }

  //���ñ��ձ�־
  m_bInsureCard[cbOperateIndex] = TRUE;

  //���ñ��ս�
  m_lInsureScore[cbOperateIndex] = m_lTableScore[cbOperateIndex]/2;

  m_lTableScoreAll[wChairID]+=m_lInsureScore[cbOperateIndex];

  //��������
  CMD_S_Insure UserInsure;
  UserInsure.lInsureScore = m_lInsureScore[cbOperateIndex];
  UserInsure.wInsureUser = wChairID;
  m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_INSURE,&UserInsure,sizeof(UserInsure));
  m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_INSURE,&UserInsure,sizeof(UserInsure));

  return true;
}

//��ҷ���
bool CTableFrameSink::OnUserSplitCard(WORD wChairID)
{
  //ȷ����������
  BYTE cbOperateIndex = wChairID*2;

  //Ч��
  ASSERT(wChairID != m_wBankerUser);
  if(wChairID == m_wBankerUser)
  {
    return false;
  }

  //Ч��
  ASSERT(m_bStopCard[cbOperateIndex] == FALSE);
  if(m_bStopCard[cbOperateIndex])
  {
    return true;
  }

  ASSERT(m_cbCardCount[cbOperateIndex+1] == 0);
  if(m_cbCardCount[cbOperateIndex+1] != 0)
  {
    return false;
  }

  ASSERT(m_cbCardCount[cbOperateIndex] == 2 && m_GameLogic.GetCardValue(m_cbHandCardData[cbOperateIndex][0]) ==
         m_GameLogic.GetCardValue(m_cbHandCardData[cbOperateIndex][1]));
  if(m_cbCardCount[cbOperateIndex] != 2 || m_GameLogic.GetCardValue(m_cbHandCardData[cbOperateIndex][0]) !=
     m_GameLogic.GetCardValue(m_cbHandCardData[cbOperateIndex][1]))
  {
    return false;
  }

  SCORE lScoreAll = m_lTableScoreAll[wChairID]+m_lTableScore[cbOperateIndex]+m_lInsureScore[cbOperateIndex];
  ASSERT(lScoreAll<=m_lMaxUserScore[wChairID]);
  if(lScoreAll>m_lMaxUserScore[wChairID])
  {
    return false;
  }

  //�ӱ�ע���뱣�ս�
  m_lTableScore[cbOperateIndex+1] = m_lTableScore[cbOperateIndex];
  m_lInsureScore[cbOperateIndex+1] = m_lInsureScore[cbOperateIndex];

  m_lTableScoreAll[wChairID]+=m_lTableScore[cbOperateIndex];
  m_lTableScoreAll[wChairID]+=m_lInsureScore[cbOperateIndex];

  //���ñ���
  if(m_bInsureCard[cbOperateIndex])
  {
    m_bInsureCard[cbOperateIndex+1] = TRUE;
  }

  //�����˿�
  m_cbHandCardData[cbOperateIndex+1][1] = m_cbHandCardData[cbOperateIndex][1];
  m_cbHandCardData[cbOperateIndex][0] = m_cbRepertoryCard[--m_cbRepertoryCount];
  m_cbHandCardData[cbOperateIndex+1][0] = m_cbRepertoryCard[--m_cbRepertoryCount];
  m_cbCardCount[cbOperateIndex+1] = 2;

  //��������
  CMD_S_SplitCard SplitCard;
  SplitCard.wSplitUser = wChairID;
  SplitCard.lAddScore = m_lTableScore[cbOperateIndex]+m_lInsureScore[cbOperateIndex];
  SplitCard.bInsured = m_lInsureScore[cbOperateIndex]>0L?TRUE:FALSE;
  for(WORD i = 0; i < GAME_PLAYER; i++)
  {
    if(!m_cbPlayStatus[i])
    {
      continue;
    }
    if(i == wChairID)
    {
      SplitCard.cbCardData[0] = m_cbHandCardData[cbOperateIndex][0];
      SplitCard.cbCardData[1] = m_cbHandCardData[cbOperateIndex+1][0];
    }
    else
    {
      ZeroMemory(SplitCard.cbCardData,sizeof(SplitCard.cbCardData));
    }
    m_pITableFrame->SendTableData(i,SUB_S_SPLIT_CARD,&SplitCard,sizeof(SplitCard));
    m_pITableFrame->SendLookonData(i,SUB_S_SPLIT_CARD,&SplitCard,sizeof(SplitCard));
  }
  if(m_pServerControl)
  {
    CMD_S_CheatCard CheatCard ;
    ZeroMemory(&CheatCard, sizeof(CheatCard));
    CopyMemory(CheatCard.cbCardData,m_cbHandCardData,sizeof(CheatCard.cbCardData));

    m_pServerControl->ServerControl(&CheatCard, m_pITableFrame);
  }

  return true;
}

//�˿˷���
bool CTableFrameSink::AnalyseCard(bool bContron)
{
  //��������
  bool bIsAiBanker = false;
  WORD wAiCount = 0;
  WORD wPlayerCount = 0;
  for(WORD i=0; i<GAME_PLAYER; i++)
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
        if(!bIsAiBanker && i==m_wBankerUser)
        {
          bIsAiBanker = true;
        }
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
  BYTE cbUserCardData[GAME_PLAYER*2][MAX_COUNT];
  CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

  //ţţ����
  BYTE bUserOxData[GAME_PLAYER*2];
  ZeroMemory(bUserOxData,sizeof(bUserOxData));
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i]==FALSE)
    {
      continue;
    }
    bUserOxData[i*2] = m_GameLogic.GetCardType(cbUserCardData[i*2],m_cbCardCount[i*2],false);
  }

  //��������
  LONGLONG lAndroidScore=0;

  //����ׯ��
  if(bIsAiBanker)
  {
    //�Ա��˿�
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //�û�����
      if((i==m_wBankerUser)||(m_cbPlayStatus[i]==FALSE))
      {
        continue;
      }

      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

      //��������
      if((pIServerUserItem!=NULL)&&(pIServerUserItem->IsAndroidUser()))
      {
        continue;
      }

      //ׯ����
      if(bUserOxData[i*2] > bUserOxData[m_wBankerUser*2])
      {
        lAndroidScore-=m_lTableScore[i*2];
      }
      else//ׯ��Ӯ
      {
        lAndroidScore+=m_lTableScore[i*2];
      }
    }
  }
  else//�û�ׯ��
  {
    //�Ա��˿�
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

      //�û�����
      if((i==m_wBankerUser)||(pIServerUserItem==NULL)||!(pIServerUserItem->IsAndroidUser()))
      {
        continue;
      }

      //ׯ����
      if(bUserOxData[i*2] > bUserOxData[m_wBankerUser*2])
      {
        lAndroidScore+=m_lTableScore[i*2];
      }
      else
      {
        lAndroidScore-=m_lTableScore[i*2];
      }
    }
  }

  if((g_lRoomStorageCurrent+lAndroidScore) > 0)
  {
    return false;
  }

  if(bContron)
  {
    //��������
    WORD wWinUser=m_wBankerUser;

    //��������
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //�û�����
      if(m_cbPlayStatus[i]==FALSE || i == m_wBankerUser)
      {
        continue;
      }

      if(bIsAiBanker)
      {
        if(bUserOxData[i*2] > bUserOxData[wWinUser*2])
        {
          wWinUser=i;
          continue;
        }
      }
      else
      {
        if(bUserOxData[i*2] < bUserOxData[wWinUser*2])
        {
          wWinUser=i;
          continue;
        }
      }
    }

    //��������
    BYTE cbTempData[MAX_COUNT];
    CopyMemory(cbTempData,m_cbHandCardData[m_wBankerUser*2],MAX_COUNT);
    CopyMemory(m_cbHandCardData[m_wBankerUser*2],m_cbHandCardData[wWinUser*2],MAX_COUNT);
    CopyMemory(m_cbHandCardData[wWinUser*2],cbTempData,MAX_COUNT);

    //SendInfo(TEXT("���Ʒ��ƣ�"));

    //�Ա��˿�
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

      //�û�����
      if(pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
      {
        if(m_GameLogic.GetCardLogicValue(m_cbHandCardData[i * 2][1]) < m_GameLogic.GetCardLogicValue(m_cbHandCardData[i * 2][0]))
        {
          BYTE cbTemp = m_cbHandCardData[i * 2][1];
          m_cbHandCardData[i * 2][1] = m_cbHandCardData[i * 2][0];
          m_cbHandCardData[i * 2][0] = cbTemp;
        }
      }
    }
  }


  return true;
}


//�˿˷���
bool CTableFrameSink::AnalyseCardEx()
{
  //��������
  bool bIsAiBanker = false;
  WORD wAiCount = 0;
  WORD wPlayerCount = 0;
  for(WORD i=0; i<GAME_PLAYER; i++)
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
        if(!bIsAiBanker && i==m_wBankerUser)
        {
          bIsAiBanker = true;
        }
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
  BYTE cbUserCardData[GAME_PLAYER*2][MAX_COUNT];
  CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

  //ţţ����
  BYTE bUserOxData[GAME_PLAYER*2];
  ZeroMemory(bUserOxData,sizeof(bUserOxData));
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    if(m_cbPlayStatus[i]==FALSE)
    {
      continue;
    }
    for(BYTE j = 0; j < 2; j++)
    {
      if(m_cbCardCount[i*2+j] < 2)
      {
        continue;
      }
      bUserOxData[i*2 + j] = m_GameLogic.GetCardType(cbUserCardData[i*2 + j],m_cbCardCount[i*2 + j],false);
    }
  }

  //��������
  LONGLONG lAndroidScore=0;

  //����ׯ��
  if(bIsAiBanker)
  {
    //�Ա��˿�
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //�û�����
      if((i==m_wBankerUser)||(m_cbPlayStatus[i]==FALSE))
      {
        continue;
      }

      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

      //��������
      if((pIServerUserItem!=NULL)&&(pIServerUserItem->IsAndroidUser()))
      {
        continue;
      }

      for(BYTE j = 0; j < 2; j++)
      {
        if(m_cbCardCount[i*2+j] < 2)
        {
          continue;
        }

        if(bUserOxData[i*2+j] > bUserOxData[m_wBankerUser*2]) //�м���
        {
          lAndroidScore-=m_lTableScore[i*2+j];
        }
        else                        //ׯ��Ӯ
        {
          lAndroidScore+=m_lTableScore[i*2+j];
        }
      }
    }
  }
  else//�û�ׯ��
  {
    //�Ա��˿�
    for(WORD i=0; i<GAME_PLAYER; i++)
    {
      //��ȡ�û�
      IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

      //�û�����
      if((i==m_wBankerUser)||(pIServerUserItem==NULL)||!(pIServerUserItem->IsAndroidUser()))
      {
        continue;
      }


      for(BYTE j = 0; j < 2; j++)
      {
        if(m_cbCardCount[i*2+j] < 2)
        {
          continue;
        }
        if(bUserOxData[i*2+j] > bUserOxData[m_wBankerUser*2])         //ׯ����
        {
          lAndroidScore+=m_lTableScore[i*2+j];
        }
        else
        {
          lAndroidScore-=m_lTableScore[i*2+j];
        }
      }
    }
  }

  if(g_lRoomStorageCurrent + lAndroidScore > 0)
  {
    return false;
  }

  if((g_lRoomStorageCurrent > g_lStorageMax2Room && rand()%100 < g_lStorageMul2Room)
     || (g_lRoomStorageCurrent > g_lStorageMax1Room && rand()%100 < g_lStorageMul1Room))
  {
    if(lAndroidScore < 0 && g_lRoomStorageCurrent + lAndroidScore > 0)
    {
      return false;
    }
  }

  return true;
}

//�˿˷���
bool CTableFrameSink::AnalyseGetCard(WORD wChairID, BYTE cbOperateIndex)
{
  //���ǻ����˲�����
  IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
  if(pIServerUserItem!=NULL && !pIServerUserItem->IsAndroidUser())
  {
    return false;
  }

  //���ǻ����˲�����
  pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wBankerUser);
  if(pIServerUserItem!=NULL && pIServerUserItem->IsAndroidUser() && m_wBankerUser != wChairID)
  {
    return false;
  }

  //��������
  bool bIsAiBanker = false;
  WORD wAiCount = 0;
  WORD wPlayerCount = 0;
  for(WORD i=0; i<GAME_PLAYER; i++)
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
        if(!bIsAiBanker && i==m_wBankerUser)
        {
          bIsAiBanker = true;
        }
      }
      wPlayerCount++;
    }
  }

  //ȫ������
  if(wPlayerCount == wAiCount || wAiCount==0)
  {
    return false;
  }

  //������ׯ��
  if(bIsAiBanker)
  {
    //ׯ��Ҫ��
    if(!AnalyseCardEx())
    {
      bool bSplitCard = m_cbCardCount[cbOperateIndex] > 0 ? true : false;
      BYTE cbCardType = m_GameLogic.GetCardType(m_cbHandCardData[cbOperateIndex],m_cbCardCount[cbOperateIndex],bSplitCard);
      if(cbCardType >= 15)
      {
        OnUserStopCard(wChairID);
        return true;
      }
    }
  }
  else      //����ׯ��
  {

  }




  return false;
}

//���Ϳ��
void CTableFrameSink::SendStorage(WORD wChairID/* = INVALID_CHAIR*/)
{
  //֪ͨ��Ϣ
  TCHAR szMessage[128]=TEXT("");
  _sntprintf(szMessage,CountArray(szMessage),TEXT("��ǰ��棺%I64d"), g_lRoomStorageCurrent);

  //�û�״̬
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��ȡ�û�
    IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);
    if(pIServerUser != NULL)
    {
      if(CUserRight::IsGameCheatUser(pIServerUser->GetUserRight()))
      {
        m_pITableFrame->SendGameMessage(pIServerUser,szMessage,SMT_CHAT);
      }
    }
  }

  int nLookonCount = 0;
  IServerUserItem* pLookonUserItem = m_pITableFrame->EnumLookonUserItem(nLookonCount);
  while(pLookonUserItem)
  {
    if(CUserRight::IsGameCheatUser(pLookonUserItem->GetUserRight()))
    {
      m_pITableFrame->SendGameMessage(pLookonUserItem,szMessage,SMT_CHAT);
    }

    nLookonCount++;
    pLookonUserItem = m_pITableFrame->EnumLookonUserItem(nLookonCount);
  }
}

//������Ϣ
void CTableFrameSink::SendInfo(CString str, WORD wChairID)
{
  //֪ͨ��Ϣ
  TCHAR szMessage[128]=TEXT("");
  _sntprintf(szMessage,CountArray(szMessage),TEXT("%s"), str);

  //�û�״̬
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    //��ȡ�û�
    IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);
    if(pIServerUser != NULL)
    {
      if(CUserRight::IsGameCheatUser(pIServerUser->GetUserRight()))
      {
        m_pITableFrame->SendGameMessage(pIServerUser,szMessage,SMT_CHAT);
      }
    }
  }

  int nLookonCount = 0;
  IServerUserItem* pLookonUserItem = m_pITableFrame->EnumLookonUserItem(nLookonCount);
  while(pLookonUserItem)
  {
    if(CUserRight::IsGameCheatUser(pLookonUserItem->GetUserRight()))
    {
      m_pITableFrame->SendGameMessage(pLookonUserItem,szMessage,SMT_CHAT);
    }

    nLookonCount++;
    pLookonUserItem = m_pITableFrame->EnumLookonUserItem(nLookonCount);
  }
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation()
{
  //��ȡ�Զ�������
  tagCustomRule *pCustomRule = (tagCustomRule *)m_pGameServiceOption->cbCustomRule;
  ASSERT(pCustomRule);

  g_lRoomStorageStart = pCustomRule->lRoomStorageStart;
  g_lRoomStorageCurrent = pCustomRule->lRoomStorageStart;
  g_lStorageDeductRoom = pCustomRule->lRoomStorageDeduct;
  g_lStorageMax1Room = pCustomRule->lRoomStorageMax1;
  g_lStorageMul1Room = pCustomRule->lRoomStorageMul1;
  g_lStorageMax2Room = pCustomRule->lRoomStorageMax2;
  g_lStorageMul2Room = pCustomRule->lRoomStorageMul2;

  if(g_lStorageDeductRoom < 0 || g_lStorageDeductRoom > 1000)
  {
    g_lStorageDeductRoom = 0;
  }
  if(g_lStorageDeductRoom > 1000)
  {
    g_lStorageDeductRoom = 1000;
  }
  if(g_lStorageMul1Room < 0 || g_lStorageMul1Room > 100)
  {
    g_lStorageMul1Room = 50;
  }
  if(g_lStorageMul2Room < 0 || g_lStorageMul2Room > 100)
  {
    g_lStorageMul2Room = 80;
  }
}

//���·����û���Ϣ
void CTableFrameSink::UpdateRoomUserInfo(IServerUserItem *pIServerUserItem, USERACTION userAction)
{
  //��������
  ROOMUSERINFO roomUserInfo;
  ZeroMemory(&roomUserInfo, sizeof(roomUserInfo));

  roomUserInfo.dwGameID = pIServerUserItem->GetGameID();
  CopyMemory(&(roomUserInfo.szNickName), pIServerUserItem->GetNickName(), sizeof(roomUserInfo.szNickName));
  roomUserInfo.cbUserStatus = pIServerUserItem->GetUserStatus();
  roomUserInfo.cbGameStatus = m_pITableFrame->GetGameStatus();

  roomUserInfo.bAndroid = pIServerUserItem->IsAndroidUser();

  //�û����º�����
  if(userAction == USER_SITDOWN || userAction == USER_RECONNECT)
  {
    roomUserInfo.wChairID = pIServerUserItem->GetChairID();
    roomUserInfo.wTableID = pIServerUserItem->GetTableID() + 1;
  }
  else if(userAction == USER_STANDUP || userAction == USER_OFFLINE)
  {
    roomUserInfo.wChairID = INVALID_CHAIR;
    roomUserInfo.wTableID = INVALID_TABLE;
  }

  g_MapRoomUserInfo.SetAt(pIServerUserItem->GetUserID(), roomUserInfo);
}

//����ͬ���û�����
void CTableFrameSink::UpdateUserControl(IServerUserItem *pIServerUserItem)
{
  //��������
  POSITION ptListHead;
  POSITION ptTemp;
  ROOMUSERCONTROL roomusercontrol;

  //��ʼ��
  ptListHead = g_ListRoomUserControl.GetHeadPosition();
  ZeroMemory(&roomusercontrol, sizeof(roomusercontrol));

  //��������
  while(ptListHead)
  {
    ptTemp = ptListHead;
    roomusercontrol = g_ListRoomUserControl.GetNext(ptListHead);

    //Ѱ���Ѵ��ڿ������
    if((pIServerUserItem->GetGameID() == roomusercontrol.roomUserInfo.dwGameID) &&
       _tcscmp(pIServerUserItem->GetNickName(), roomusercontrol.roomUserInfo.szNickName) == 0)
    {
      //��ȡԪ��
      ROOMUSERCONTROL &tmproomusercontrol = g_ListRoomUserControl.GetAt(ptTemp);

      //�������
      tmproomusercontrol.roomUserInfo.wChairID = pIServerUserItem->GetChairID();
      tmproomusercontrol.roomUserInfo.wTableID = m_pITableFrame->GetTableID() + 1;

      return;
    }
  }
}

//�����û�����
void CTableFrameSink::TravelControlList(ROOMUSERCONTROL keyroomusercontrol)
{
  //��������
  POSITION ptListHead;
  POSITION ptTemp;
  ROOMUSERCONTROL roomusercontrol;

  //��ʼ��
  ptListHead = g_ListRoomUserControl.GetHeadPosition();
  ZeroMemory(&roomusercontrol, sizeof(roomusercontrol));

  //��������
  while(ptListHead)
  {
    ptTemp = ptListHead;
    roomusercontrol = g_ListRoomUserControl.GetNext(ptListHead);

    //Ѱ���Ѵ��ڿ����������һ�������л�����
    if((keyroomusercontrol.roomUserInfo.dwGameID == roomusercontrol.roomUserInfo.dwGameID) &&
       _tcscmp(keyroomusercontrol.roomUserInfo.szNickName, roomusercontrol.roomUserInfo.szNickName) == 0)
    {
      g_ListRoomUserControl.RemoveAt(ptTemp);
    }
  }
}

//�Ƿ������������
void CTableFrameSink::IsSatisfyControl(ROOMUSERINFO &userInfo, bool &bEnableControl)
{
  if(userInfo.wChairID == INVALID_CHAIR || userInfo.wTableID == INVALID_TABLE)
  {
    bEnableControl = FALSE;
    return;
  }

  if(userInfo.cbUserStatus == US_SIT || userInfo.cbUserStatus == US_READY || userInfo.cbUserStatus == US_PLAYING)
  {
    if(userInfo.cbGameStatus == GAME_STATUS_FREE)
    {
      bEnableControl = TRUE;
      return;
    }
    else
    {
      bEnableControl = FALSE;
      return;
    }
  }
  else
  {
    bEnableControl = FALSE;
    return;
  }
}

//���������û�����
bool CTableFrameSink::AnalyseRoomUserControl(ROOMUSERCONTROL &Keyroomusercontrol, POSITION &ptList)
{
  //��������
  POSITION ptListHead;
  POSITION ptTemp;
  ROOMUSERCONTROL roomusercontrol;

  //��������
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
    if(!pIServerUserItem)
    {
      continue;
    }

    //��ʼ��
    ptListHead = g_ListRoomUserControl.GetHeadPosition();
    ZeroMemory(&roomusercontrol, sizeof(roomusercontrol));

    //��������
    while(ptListHead)
    {
      ptTemp = ptListHead;
      roomusercontrol = g_ListRoomUserControl.GetNext(ptListHead);

      //Ѱ�����
      if((pIServerUserItem->GetGameID() == roomusercontrol.roomUserInfo.dwGameID) &&
         _tcscmp(pIServerUserItem->GetNickName(), roomusercontrol.roomUserInfo.szNickName) == 0)
      {
        //��տ��ƾ���Ϊ0��Ԫ��
        if(roomusercontrol.userControl.cbControlCount == 0)
        {
          g_ListRoomUserControl.RemoveAt(ptTemp);
          break;
        }

        if(roomusercontrol.userControl.control_type == CONTINUE_CANCEL)
        {
          g_ListRoomUserControl.RemoveAt(ptTemp);
          break;
        }

        //��������
        CopyMemory(&Keyroomusercontrol, &roomusercontrol, sizeof(roomusercontrol));
        ptList = ptTemp;

        return true;
      }

    }

  }

  return false;
}

void CTableFrameSink::GetControlTypeString(CONTROL_TYPE &controlType, CString &controlTypestr)
{
  switch(controlType)
  {
    case CONTINUE_WIN:
    {
      controlTypestr = TEXT("��������Ϊ��Ӯ");
      break;
    }
    case CONTINUE_LOST:
    {
      controlTypestr = TEXT("��������Ϊ����");
      break;
    }
    case CONTINUE_CANCEL:
    {
      controlTypestr = TEXT("��������Ϊȡ������");
      break;
    }
  }
}

//д��־�ļ�
void CTableFrameSink::WriteInfo(LPCTSTR pszString)
{
  //������������
  char* old_locale = _strdup(setlocale(LC_CTYPE,NULL));
  setlocale(LC_CTYPE, "chs");

  CStdioFile myFile;
  CString strFileName = TEXT("21�����LOG.txt");
  BOOL bOpen = myFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
  if(bOpen)
  {
    myFile.SeekToEnd();
    myFile.WriteString(pszString);
    myFile.Flush();
    myFile.Close();
  }

  //��ԭ�����趨
  setlocale(LC_CTYPE, old_locale);
  free(old_locale);
}
//////////////////////////////////////////////////////////////////////////////////
