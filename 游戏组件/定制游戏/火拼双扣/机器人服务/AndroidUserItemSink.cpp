#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS         4                 //����ʱ��
#define TIME_DISPATCH       5                 //����ʱ��

#ifdef _DEBUG

//��Ϸʱ��
#define TIME_OUT_CARD       1                 //����ʱ��
#define TIME_START_GAME       2                 //��ʼʱ��
#define TIME_THROW_CARD       5                 //����ʱ��
#define TIME_INCEP_CARD       5                 //�ع���ʱ��

#else
//��Ϸʱ��
#define TIME_OUT_CARD       4                 //����ʱ��
#define TIME_START_GAME       6                 //��ʼʱ��
#define TIME_THROW_CARD       5                 //����ʱ��
#define TIME_INCEP_CARD       5                 //�ع���ʱ��

#endif

//��Ϸʱ��
#define IDI_START_GAME        100                 //��ʼʱ��
#define IDI_OUT_CARD        101                 //����ʱ��
#define IDI_PASS_CARD       102                 //passʱ��



//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
  //��Ϸ����
  m_wBankerUser=INVALID_CHAIR;

  m_wOutCardUser = INVALID_CHAIR ;
  m_wCurrentUser=INVALID_CHAIR;


  //�˿˱���
  m_cbTurnCardCount=0;

  ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

  //�����˿�
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
  ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));


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
VOID *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
  return NULL;
}

//��ʼ�ӿ�
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
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
bool  CAndroidUserItemSink::RepositionSink()
{
  //��Ϸ����
  m_wBankerUser=INVALID_CHAIR;


  //�˿˱���
  m_cbTurnCardCount=0;
  ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

  //�����˿�
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
  ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

  return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
  switch(nTimerID)
  {
    case IDI_START_GAME:  //��ʼ��Ϸ
    {
      m_pIAndroidUserItem->SendUserReady(NULL,0);
      return true;
    }
    case IDI_PASS_CARD:   //pass
    {
      m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
      return true;
    }
    case IDI_OUT_CARD:    //�û�����
    {
      //���ƴ���
      if(m_wCurrentUser == m_pIAndroidUserItem->GetChairID())
      {
        //ɾ��ʱ��
        m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);

        //�������
        tagOutCardResult OutCardResult;
        ZeroMemory(&OutCardResult,sizeof(OutCardResult));

        ////���ƴ���
        //if (m_cbTurnCardCount==0)
        //{
        //  //�������
        //  tagOutCardResult OutCardResult;
        //  m_GameLogic.SearchOutCard(m_cbHandCardData[m_pIAndroidUserItem->GetChairID()],m_cbHandCardCount[m_pIAndroidUserItem->GetChairID()],m_cbTurnCardData,m_cbTurnCardCount,OutCardResult);

        //  //���ý���
        //  if (OutCardResult.cbCardCount>0)
        //  {
        //
        //  }
        //  else
        //  {
        //    m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
        //    return true;
        //  }
        //}
        //else if (m_cbTurnCardCount>0)
        //{
        //  //if (rand()%2==0)
        //  //{
        //  //  m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
        //  //  return true;
        //  //}
        //
        //}

        //�������
        bool bValid=false;
        WORD wPlayerID = (m_pIAndroidUserItem->GetChairID()+1)%GAME_PLAYER;
        m_GameLogic.SortCardList(m_cbHandCardData[m_pIAndroidUserItem->GetChairID()],m_cbHandCardCount[m_pIAndroidUserItem->GetChairID()],ST_ORDER);


        bValid = m_GameLogic.AiSearchOutCard(m_cbHandCardData[m_pIAndroidUserItem->GetChairID()],m_cbHandCardCount[m_pIAndroidUserItem->GetChairID()],
                                             m_cbTurnCardData,m_cbTurnCardCount,m_cbHandCardData[wPlayerID],m_cbHandCardCount[wPlayerID],OutCardResult);

        if(!bValid)
        {
          m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
          return true;
        }
        else
        {
          if(VerdictOutCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount))
          {
            if(OutCardResult.cbCardCount>0)
            {
              //��ȡ�˿�
              BYTE cbCardData[MAX_COUNT];
              BYTE cbCardCount=OutCardResult.cbCardCount;
              CopyMemory(cbCardData,OutCardResult.cbResultCard,sizeof(cbCardData));

              //�����˿�
              m_GameLogic.SortCardList(cbCardData,cbCardCount,ST_ORDER);

              if(m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData[m_pIAndroidUserItem->GetChairID()],m_cbHandCardCount[m_pIAndroidUserItem->GetChairID()])==false)
              {
                ASSERT(false);
                m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
                return true;
              }
              //��������
              CMD_C_OutCard OutCard;
              OutCard.cbCardCount=cbCardCount;
              CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount);
              m_pIAndroidUserItem->SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)*sizeof(BYTE));
              return true;
            }

          }
          else
          {
            m_pIAndroidUserItem->SendSocketData(SUB_C_PASS_CARD);
            return true;
          }
        }

      }

      return true;
    }
  }

  return false;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
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
      return true;
    }
    case SUB_S_ON_READY:    //�û�׼��
    {
      return true;
    }
    case SUB_S_CHEAT_CARD:    //����ָ��
    {
      return true;
    }
  }

  //�������
  ASSERT(FALSE);

  return true;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
  return true;
}

//������Ϣ
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
  switch(cbGameStatus)
  {
    case GAME_STATUS_FREE:  //����״̬
    {
      //Ч������
      ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
      if(wDataSize!=sizeof(CMD_S_StatusFree))
      {
        return false;
      }

      //��������
      CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
      IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

      //�������
      UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
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

      //���Ʊ���
      m_cbTurnCardCount=pStatusPlay->cbTurnCardCount;
      CopyMemory(m_cbTurnCardData,pStatusPlay->cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

      //�˿�����
      WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
      CopyMemory(m_cbHandCardCount,pStatusPlay->cbHandCardCount,sizeof(m_cbHandCardCount));
      CopyMemory(m_cbHandCardData,pStatusPlay->cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);

      //�������
      if(pStatusPlay->wCurrentUser==m_pIAndroidUserItem->GetChairID())
      {
        UINT nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
        m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
      }

      return true;
    }
  }

  //�������
  ASSERT(FALSE);

  return false;
}

//�û�����
VOID  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û��뿪
VOID  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û�����
VOID  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//�û�״̬
VOID  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
  return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pData, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_GameStart));
  if(wDataSize!=sizeof(CMD_S_GameStart))
  {
    return false;
  }

  //��Ϣ����
  CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

  m_wBankerUser=pGameStart->wCurrentUser;
  //��Ϸ����
  m_wCurrentUser=pGameStart->wCurrentUser;

  //���Ʊ���
  m_cbTurnCardCount=0;
  ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
  ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

  //�����˿�
  WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
  for(WORD i=0; i<GAME_PLAYER; i++)
  {
    m_cbHandCardCount[i]=CountArray(pGameStart->cbCardData);
  }
  CopyMemory(m_cbHandCardData[wMeChairID],pGameStart->cbCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);
  CopyMemory(m_cbHandCardData[(wMeChairID+1)%GAME_PLAYER],pGameStart->cbPlayerCardData,sizeof(BYTE)*m_cbHandCardCount[(wMeChairID+1)%GAME_PLAYER]);

  if(m_wCurrentUser==wMeChairID)
  {
    //��עʱ��
    UINT nElapse=rand()%(4)+TIME_DISPATCH;
    m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
  }

  return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOutCard(const void * pData, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_OutCard));
  if(wDataSize!=sizeof(CMD_S_OutCard))
  {
    return false;
  }
  //��������
  CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pData;

  //��������
  WORD wMeChairID=m_pIAndroidUserItem->GetChairID();

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



  //��������
  if(wMeChairID!=pOutCard->wOutCardUser)
  {
    m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
  }
  else
  {
    //ɾ���˿�
    BYTE cbSourceCount=m_cbHandCardCount[wMeChairID];
    m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
    m_GameLogic.RemoveCardEx(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData[m_pIAndroidUserItem->GetChairID()],cbSourceCount);
  }

  //�������
  if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
  {
    //������
    BYTE cbCardCount=m_cbHandCardCount[pOutCard->wCurrentUser];
    if(cbCardCount==1&&pOutCard->cbCardCount>=2)
    {
      //passʱ��
      UINT nElapse=rand()%(2)+2;
      m_pIAndroidUserItem->SetGameTimer(IDI_PASS_CARD,nElapse);
    }
    else
    {
      //����ʱ��
      UINT nElapse=rand()%(TIME_OUT_CARD)+TIME_DISPATCH;
      m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
    }

  }
  return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubPassCard(const void * pData, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_PassCard));
  if(wDataSize!=sizeof(CMD_S_PassCard))
  {
    return false;
  }
  //��������
  CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pData;

  //���ñ���
  m_wCurrentUser=pPassCard->wCurrentUser;

  //һ���ж�
  if(pPassCard->cbTurnOver==TRUE)
  {
    //���Ʊ���
    m_cbTurnCardCount=0;
    ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
    ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
  }

  //�������
  if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
  {
    //����ʱ��
    UINT nElapse=rand()%(4)+1;
    m_pIAndroidUserItem->SetGameTimer(IDI_OUT_CARD,nElapse);
  }

  return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pData, WORD wDataSize)
{
  //Ч������
  ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
  if(wDataSize!=sizeof(CMD_S_GameEnd))
  {
    return false;
  }

  //��������
  CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

  //���ñ���
  m_cbTurnCardCount=0;
  ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
  ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
  ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

  //ɾ��ʱ��
  m_pIAndroidUserItem->KillGameTimer(IDI_OUT_CARD);

  //��ʼ����
  UINT nElapse=rand()%TIME_START_GAME+2;
  m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

  return true;
}
//�����ж�
bool CAndroidUserItemSink::VerdictOutCard(BYTE cbOutCardData[], BYTE cbOutCardCount)
{

  //״̬�ж�
  if(m_wCurrentUser!=m_pIAndroidUserItem->GetChairID())
  {
    return false;
  }

  //�����ж�
  if(cbOutCardCount>0L)
  {
    memset(&m_OutCardInfo,0,sizeof(m_OutCardInfo));
    m_GameLogic.AnalysebCardData(cbOutCardData,cbOutCardCount,m_OutCardInfo);

    return m_GameLogic.CompareCard(m_OutCardInfo,m_LastOutCard);
  }
  return false;

}
//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
