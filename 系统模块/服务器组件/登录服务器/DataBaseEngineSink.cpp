#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseEngineSink::CDataBaseEngineSink()
{
  //�������
  m_pInitParameter=NULL;
  m_pIDataBaseEngineEvent=NULL;

  return;
}

//��������
CDataBaseEngineSink::~CDataBaseEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
  return NULL;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
  //��������
  if((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
  {
    ASSERT(FALSE);
    return false;
  }

  //��������
  if((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false))
  {
    ASSERT(FALSE);
    return false;
  }

  //��������
  if((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false))
  {
    ASSERT(FALSE);
    return false;
  }

  try
  {
    //������Ϣ
    tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
    tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
    tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

    //��������
    m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
                                          pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass);
    m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
                                          pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass);
    m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
                                          pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass);

    //��������
    m_AccountsDBModule->OpenConnection();
    m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

    //��������
    m_TreasureDBModule->OpenConnection();
    m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

    //��������
    m_PlatformDBModule->OpenConnection();
    m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
    CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

    return false;
  }

  return true;
}

//ֹͣ�¼�
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
  //���ö���
  m_AccountsDBAide.SetDataBase(NULL);
  m_TreasureDBAide.SetDataBase(NULL);
  m_PlatformDBAide.SetDataBase(NULL);

  //�ر�����
  if(m_AccountsDBModule.GetInterface()!=NULL)
  {
    m_AccountsDBModule->CloseConnection();
    m_AccountsDBModule.CloseInstance();
  }

  //�ر�����
  if(m_TreasureDBModule.GetInterface()!=NULL)
  {
    m_TreasureDBModule->CloseConnection();
    m_TreasureDBModule.CloseInstance();
  }

  //�ر�����
  if(m_PlatformDBModule.GetInterface()!=NULL)
  {
    m_PlatformDBModule->CloseConnection();
    m_PlatformDBModule.CloseInstance();
  }

  //�������
  m_pInitParameter=NULL;
  m_pIDataBaseEngineEvent=NULL;

  return true;
}

//ʱ���¼�
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
  return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
  return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  bool bSucceed = false;
  CString strDataBaseEngineRequest=TEXT("");
  switch(wRequestID)
  {
    case DBR_GP_LOGON_GAMEID:     //��ʶ��¼
    {
      bSucceed =  OnRequestLogonGameID(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOGON_GAMEID");
      break;
    }
    case DBR_GP_LOGON_ACCOUNTS:     //�ʺŵ�¼
    {
      bSucceed =  OnRequestLogonAccounts(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOGON_ACCOUNTS");
      break;
    }
    case DBR_GP_REGISTER_ACCOUNTS:    //ע���ʺ�
    {
      bSucceed =  OnRequestRegisterAccounts(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_REGISTER_ACCOUNTS");
      break;
    }
    case DBR_GP_LOAD_PLATFORM_PARAMETER: //ƽ̨����
    {
      bSucceed =  OnRequestPlatformParameter(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOAD_PLATFORM_PARAMETER");
      break;
    }
    case DBR_GP_VERIFY_INDIVIDUAL:    //��֤����
    {
      bSucceed =  OnRequestVerifyIndividual(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_VERIFY_INDIVIDUAL");
      break;
    }
    case DBR_GP_LOGON_VISITOR:      //�ο͵�½
    {
      bSucceed =  OnRequestLogonVisitor(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOGON_VISITOR");
      break;
    }
    case DBR_GP_MODIFY_MACHINE:     //�޸Ļ���
    {
      bSucceed =  OnRequestModifyMachine(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MODIFY_MACHINE");
      break;
    }
    case DBR_GP_MODIFY_LOGON_PASS:    //�޸�����
    {
      bSucceed =  OnRequestModifyLogonPass(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MODIFY_LOGON_PASS");
      break;
    }
    case DBR_GP_MODIFY_INSURE_PASS:   //�޸�����
    {
      bSucceed =  OnRequestModifyInsurePass(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MODIFY_INSURE_PASS");
      break;
    }
    case DBR_GP_MODIFY_UNDER_WRITE:   //�޸�ǩ��
    {
      bSucceed =  OnRequestModifyUnderWrite(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MODIFY_UNDER_WRITE");
      break;
    }
    case DBR_GP_MODIFY_REAL_AUTH:   //ʵ����֤
    {
      bSucceed =  OnRequestModifyRealAuth(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MODIFY_REAL_AUTH");
      break;
    }
    case DBR_GP_MODIFY_SYSTEM_FACE:   //�޸�ͷ��
    {
      bSucceed =  OnRequestModifySystemFace(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MODIFY_SYSTEM_FACE");
      break;
    }
    case DBR_GP_MODIFY_CUSTOM_FACE:   //�޸�ͷ��
    {
      bSucceed =  OnRequestModifyCustomFace(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MODIFY_CUSTOM_FACE");
      break;
    }
    case DBR_GP_MODIFY_INDIVIDUAL:    //�޸�����
    {
      bSucceed =  OnRequestModifyIndividual(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MODIFY_INDIVIDUAL");
      break;
    }
    case DBR_GP_USER_ENABLE_INSURE:   //��ͨ����
    {
      bSucceed =  OnRequestUserEnableInsure(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_USER_ENABLE_INSURE");
      break;
    }
    case DBR_GP_USER_SAVE_SCORE:    //������Ϸ��
    {
      bSucceed =  OnRequestUserSaveScore(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_USER_SAVE_SCORE");
      break;
    }
    case DBR_GP_USER_TAKE_SCORE:    //��ȡ��Ϸ��
    {
      bSucceed =  OnRequestUserTakeScore(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_USER_TAKE_SCORE");
      break;
    }
    case DBR_GP_USER_TRANSFER_SCORE:  //ת����Ϸ��
    {
      bSucceed =  OnRequestUserTransferScore(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_USER_TRANSFER_SCORE");
      break;
    }
    case DBR_GP_QUERY_INDIVIDUAL:   //��ѯ����
    {
      bSucceed =  OnRequestQueryIndividual(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_QUERY_INDIVIDUAL");
      break;
    }
    case DBR_GP_QUERY_INSURE_INFO:    //��ѯ����
    {
      bSucceed =  OnRequestQueryInsureInfo(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_QUERY_INSURE_INFO");
      break;
    }
    case DBR_GP_QUERY_USER_INFO:    //��ѯ�û�
    {
      bSucceed =  OnRequestQueryTransferUserInfo(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_QUERY_USER_INFO");
      break;
    }
    case DBR_GP_LOAD_GAME_LIST:     //�����б�
    {
      bSucceed =  OnRequestLoadGameList(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOAD_GAME_LIST");
      break;
    }
    case DBR_GP_ONLINE_COUNT_INFO:    //������Ϣ
    {
      bSucceed =  OnRequestOnLineCountInfo(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_ONLINE_COUNT_INFO");
      break;
    }
    case DBR_GP_LOAD_CHECKIN_REWARD:  //ǩ������
    {
      bSucceed =  OnRequestCheckInReward(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOAD_CHECKIN_REWARD");
      break;
    }
    case DBR_GP_LOAD_TASK_LIST:     //��������
    {
      bSucceed =  OnRequestLoadTaskList(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOAD_TASK_LIST");
      break;
    }
    case DBR_GP_LOAD_BASEENSURE:    //���صͱ�
    {
      bSucceed =  OnRequestLoadBaseEnsure(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOAD_BASEENSURE");
      break;
    }
    case DBR_GP_QUERY_SPREAD_INFO:    //�ƹ���Ϣ
    {
      bSucceed =  OnRequestQuerySpreadInfo(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_QUERY_SPREAD_INFO");
      break;
    }
    case DBR_GP_LOAD_REAL_AUTH:     //������֤
    {
      bSucceed =  OnRequestLoadRealAuth(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOAD_REAL_AUTH");
      break;
    }
    case DBR_GP_GET_PARAMETER:      //��ȡ����
    {
      bSucceed =  OnRequestGetParameter(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_GET_PARAMETER");
      break;
    }
    case DBR_GP_ADD_PARAMETER:      //��Ӳ���
    {
      bSucceed =  OnRequestAddParameter(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_ADD_PARAMETER");
      break;
    }
    case DBR_GP_MODIFY_PARAMETER:   //�޸Ĳ���
    {
      bSucceed =  OnRequestModifyParameter(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MODIFY_PARAMETER");
      break;
    }
    case DBR_GP_DELETE_PARAMETER:   //ɾ������
    {
      bSucceed =  OnRequestDeleteParameter(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_DELETE_PARAMETER");
      break;
    }
    case DBR_GP_CHECKIN_QUERY_INFO:   //��ѯǩ��
    {
      bSucceed =  OnRequestCheckInQueryInfo(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_CHECKIN_QUERY_INFO");
      break;
    }
    case DBR_GP_CHECKIN_DONE:     //ִ��ǩ��
    {
      bSucceed =  OnRequestCheckInDone(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_CHECKIN_DONE");
      break;
    }
    case DBR_GP_TASK_QUERY_INFO:    //��ѯ����
    {
      bSucceed =  OnRequestTaskQueryInfo(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_TASK_QUERY_INFO");
      break;
    }
    case DBR_GP_TASK_GIVEUP:      //��������
    {
      bSucceed =  OnRequestTaskGiveUp(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_TASK_GIVEUP");
      break;
    }
    case DBR_GP_TASK_TAKE:        //��ȡ����
    {
      bSucceed =  OnRequestTaskTake(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_TASK_TAKE");
      break;
    }
    case DBR_GP_TASK_REWARD:      //��ȡ����
    {
      bSucceed =  OnRequestTaskReward(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_TASK_REWARD");
      break;
    }
    case DBR_GP_BASEENSURE_TAKE:    //��ȡ�ͱ�
    {
      bSucceed =  OnRequestTakeBaseEnsure(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_BASEENSURE_TAKE");
      break;
    }
    case DBR_GP_GROWLEVEL_QUERY_IFNO: //��ѯ�ȼ�
    {
      bSucceed =  OnRequestQueryGrowLevelParameter(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_GROWLEVEL_QUERY_IFNO");
      break;
    }
    case DBR_GP_LOAD_MEMBER_PARAMETER:  //��Ա����
    {
      bSucceed =  OnRequestMemberLoadParameter(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOAD_MEMBER_PARAMETER");
      break;
    }
    case DBR_GP_MEMBER_QUERY_INFO:  //��Ա��ѯ
    {
      bSucceed =  OnRequestMemberQueryInfo(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MEMBER_QUERY_INFO");
      break;
    }
    case DBR_GP_MEMBER_DAY_PRESENT: //��Ա����
    {
      bSucceed =  OnRequestMemberDayPresent(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MEMBER_DAY_PRESENT");
      break;
    }
    case DBR_GP_MEMBER_DAY_GIFT:  //��Ա����
    {
      bSucceed =  OnRequestMemberDayGift(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_MEMBER_DAY_GIFT");
      break;
    }
    case DBR_GP_LOAD_GROWLEVEL_CONFIG:  //�ȼ�����
    {
      bSucceed =  OnRequestLoadGrowLevelConfig(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOAD_GROWLEVEL_CONFIG");
      break;
    }
    case DBR_GP_LOAD_GAME_PROPERTY: //���ص���
    {
      bSucceed =  OnRequestLoadGameProperty(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOAD_GAME_PROPERTY");
      break;
    }
    case DBR_GP_PROPERTY_BUY:     //�������
    {
      bSucceed =  OnRequestBuyGameProperty(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_PROPERTY_BUY");
      break;
    }
    case DBR_GP_PROPERTY_USE:     //ʹ�õ���
    {
      bSucceed =  OnRequestUseProperty(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_PROPERTY_USE");
      break;
    }
    case DBR_GP_USER_PROPERTY_BUFF:   //�������Buff
    {
      bSucceed =  OnRequestPropertyBuff(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_USER_PROPERTY_BUFF");
      break;
    }
    case DBR_GP_QUERY_BACKPACK:      //��ѯ����
    {
      bSucceed =  OnRequestUserBackpackProperty(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_QUERY_BACKPACK");
      break;
    }
    case DBR_GP_PROPERTY_PRESENT:   //���͵���
    {
      bSucceed =  OnRequestPropertyPresent(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_PROPERTY_PRESENT");
      break;
    }
    case DBR_GP_QUERY_SEND_PRESENT:   //��ѯ����
    {
      bSucceed =  OnRequestQuerySendPresent(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_QUERY_SEND_PRESENT");
      break;
    }
    case DBR_GP_GET_SEND_PRESENT:   //��ȡ����
    {
      bSucceed =  OnRequestGetSendPresent(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_GET_SEND_PRESENT");
      break;
    }
    case DBR_GP_PURCHASE_MEMBER:    //�����Ա
    {
      bSucceed =  OnRequestPurchaseMember(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_PURCHASE_MEMBER");
      break;
    }
    case DBR_GP_EXCHANGE_SCORE_INGOT: //�һ���Ϸ��
    {
      bSucceed =  OnRequestExchangeScoreByIngot(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_EXCHANGE_SCORE_INGOT");
      break;
    }
    case DBR_GP_EXCHANGE_SCORE_BEANS: //�һ���Ϸ��
    {
      bSucceed =  OnRequestExchangeScoreByBeans(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_EXCHANGE_SCORE_BEANS");
      break;
    }
    case DBR_MB_LOGON_GAMEID:     //��ʶ��¼
    {
      bSucceed =  OnMobileLogonGameID(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_MB_LOGON_GAMEID");
      break;
    }
    case DBR_MB_LOGON_ACCOUNTS:     //�ʺŵ�¼
    {
      bSucceed =  OnMobileLogonAccounts(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_MB_LOGON_ACCOUNTS");
      break;
    }
    case DBR_MB_REGISTER_ACCOUNTS:    //ע���ʺ�
    {
      bSucceed =  OnMobileRegisterAccounts(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_MB_REGISTER_ACCOUNTS");
      break;
    }
    case DBR_MB_LOGON_OTHERPLATFORM:  //������¼
    {
      bSucceed =  OnMobileLogonOtherPlatform(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_MB_LOGON_OTHERPLATFORM");
      break;
    }
    case DBR_MB_LOGON_VISITOR:      //�ο͵�½
    {
      bSucceed =  OnMobileLogonVisitor(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_MB_LOGON_VISITOR");
      break;
    }
    case DBR_GP_LOTTERY_CONFIG_REQ:   //��������
    {
      bSucceed =  OnRequestLotteryConfigReq(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOTTERY_CONFIG_REQ");
      break;
    }
    case DBR_GP_LOTTERY_START:      //�齱��ʼ
    {
      bSucceed =  OnRequestLotteryStart(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_LOTTERY_START");
      break;
    }
    case DBR_GP_QUERY_USER_GAME_DATA: //��Ϸ����
    {
      bSucceed =  OnRequestQueryUserGameData(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_QUERY_USER_GAME_DATA");
      break;
    }
    case DBR_GP_ACCOUNT_BIND:
    {
      bSucceed =  OnRequestAccountBind(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_ACCOUNT_BIND");
      break;
    }
    case DBR_GP_ACCOUNT_BIND_EXISTS:
    {
      bSucceed =  OnRequestAccountBindExists(dwContextID,pData,wDataSize);
      strDataBaseEngineRequest=TEXT("DBR_GP_ACCOUNT_BIND_EXISTS");
      break;
    }
  }

  if(!bSucceed)
  {
    CString str2;
    str2.Format(TEXT("OnDataBaseEngineRequest %d,,%d"),wRequestID,bSucceed);


    CTraceService::TraceString(str2,TraceLevel_Exception);
    CTraceService::TraceString(strDataBaseEngineRequest,TraceLevel_Exception);
  }
  return bSucceed;
}

//I D ��¼
bool CDataBaseEngineSink::OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_LogonGameID));
    if(wDataSize!=sizeof(DBR_GP_LogonGameID))
    {
      return false;
    }

    //ִ�в�ѯ
    DBR_GP_LogonGameID * pLogonGameID=(DBR_GP_LogonGameID *)pData;

    //��������
    tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
    m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonGameID->cbNeeValidateMBCard);
    m_AccountsDBAide.AddParameter(TEXT("@strSessionKey"),pLogonGameID->szSessionKey);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,false);
    CTraceService::TraceString(TEXT("GSP_GP_EfficacyGameID"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_LogonAccounts));
    if(wDataSize!=sizeof(DBR_GP_LogonAccounts))
    {
      return false;
    }

    //������
    DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;

    //ִ���ж�
    tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //��������
    LONG lResultCode=DB_SUCCESS;

    //�������
    TCHAR szDescribeString[128]=TEXT("");

    //���Ȩ��
    if(pLogonAccounts->dwCheckUserRight!=0)
    {
      //�������
      m_AccountsDBAide.ResetParameter();
      m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
      m_AccountsDBAide.AddParameter(TEXT("@dwcheckRight"),pLogonAccounts->dwCheckUserRight);
      m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

      //ִ�й���
      lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CheckUserRight"),false);
    }

    //��֤�ʺ�
    if(lResultCode==DB_SUCCESS)
    {
      //�������
      m_AccountsDBAide.ResetParameter();
      m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
      m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
      m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
      m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
      m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);
      m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pLogonAccounts->szPassPortID);
      //m_AccountsDBAide.AddParameter(TEXT("@strSessionKey"),pLogonAccounts->szSessionKey);
      m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

      //ִ�в�ѯ
      lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);
    }

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnLogonDisposeResult(dwContextID,lResultCode, CW2CT(DBVarValue.bstrVal),pLogonAccounts->dwCheckUserRight,false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,false);
    CTraceService::TraceString(TEXT("GSP_GP_EfficacyAccounts or GSP_GP_CheckUserRight"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//ע�ᴦ��
bool CDataBaseEngineSink::OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
    if(wDataSize!=sizeof(DBR_GP_RegisterAccounts))
    {
      return false;
    }

    //������
    DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

    //ִ���ж�
    tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
    m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
    m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
    m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
    m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
    m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
    m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
    m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
    m_AccountsDBAide.AddParameter(TEXT("@wAgentID"),pRegisterAccounts->wAgentID);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,false);
    CTraceService::TraceString(TEXT("GSP_GP_RegisterAccounts"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��֤����
bool CDataBaseEngineSink::OnRequestVerifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_VerifyIndividual));
    if(wDataSize!=sizeof(DBR_GP_VerifyIndividual))
    {
      return false;
    }

    //������
    DBR_GP_VerifyIndividual * pVerifyIndividual=(DBR_GP_VerifyIndividual *)pData;

    //ִ���ж�
    tagBindParameter * pBindParameter=(tagBindParameter *)pVerifyIndividual->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@wVerifyMask"),pVerifyIndividual->wVerifyMask);
    m_AccountsDBAide.AddParameter(TEXT("@strVerifyContent"),pVerifyIndividual->szVerifyContent);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_VerifyIndividual"),false);

    //�������
    DBO_GP_VerifyIndividualResult VerifyIndividualResult;
    VerifyIndividualResult.szErrorMsg[0]=0;
    VerifyIndividualResult.wVerifyMask=pVerifyIndividual->wVerifyMask;
    VerifyIndividualResult.bVerifyPassage=lResultCode==DB_SUCCESS;

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    lstrcpyn(VerifyIndividualResult.szErrorMsg,CW2CT(DBVarValue.bstrVal),CountArray(VerifyIndividualResult.szErrorMsg));

    //���ͽ��
    WORD wDataSize=CountStringBuffer(VerifyIndividualResult.szErrorMsg);
    WORD wHeadSize=sizeof(VerifyIndividualResult)-sizeof(VerifyIndividualResult.szErrorMsg);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VERIFY_RESULT,dwContextID,&VerifyIndividualResult,wHeadSize+wDataSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    DBR_GP_VerifyIndividual * pVerifyIndividual=(DBR_GP_VerifyIndividual *)pData;

    //�������
    DBO_GP_VerifyIndividualResult VerifyIndividualResult;
    VerifyIndividualResult.bVerifyPassage=false;
    VerifyIndividualResult.wVerifyMask=pVerifyIndividual->wVerifyMask;
    lstrcpyn(VerifyIndividualResult.szErrorMsg,TEXT("���ݿ�����쳣,��֤ʧ��!"),CountArray(VerifyIndividualResult.szErrorMsg));

    //���ͽ��
    WORD wDataSize=CountStringBuffer(VerifyIndividualResult.szErrorMsg);
    WORD wHeadSize=sizeof(VerifyIndividualResult)-sizeof(VerifyIndividualResult.szErrorMsg);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VERIFY_RESULT,dwContextID,&VerifyIndividualResult,wHeadSize+wDataSize);

    CTraceService::TraceString(TEXT("GSP_GP_VerifyIndividual"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�ο͵�¼
bool CDataBaseEngineSink::OnRequestLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_LogonVisitor));
    if(wDataSize!=sizeof(DBR_GP_LogonVisitor))
    {
      return false;
    }

    //������
    DBR_GP_LogonVisitor * pVisitorLogon=(DBR_GP_LogonVisitor *)pData;

    //ִ���ж�
    tagBindParameter * pBindParameter=(tagBindParameter *)pVisitorLogon->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pVisitorLogon->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@cbPlatformID"),pVisitorLogon->cbPlatformID);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pVisitorLogon->szMachineID);
    m_AccountsDBAide.AddParameter(TEXT("@strSessionKey"),pVisitorLogon->szSessionKey);
    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyLogonVisitor"),true);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,false);

    CTraceService::TraceString(TEXT("GSP_GP_EfficacyLogonVisitor"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//I D ��¼
bool CDataBaseEngineSink::OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_MB_LogonGameID));
    if(wDataSize!=sizeof(DBR_MB_LogonGameID))
    {
      return false;
    }

    //ִ�в�ѯ
    DBR_MB_LogonGameID * pLogonGameID=(DBR_MB_LogonGameID *)pData;

    //��������
    tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
    m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonGameID->szMobilePhone);
    m_AccountsDBAide.AddParameter(TEXT("@strSessionKey"),pLogonGameID->szSessionKey);
    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyGameID"),true);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

    CTraceService::TraceString(TEXT("GSP_MB_EfficacyGameID"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_MB_LogonAccounts));
    if(wDataSize!=sizeof(DBR_MB_LogonAccounts))
    {
      return false;
    }

    //������
    DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

    //ִ���ж�
    tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
    m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);
    m_AccountsDBAide.AddParameter(TEXT("@strSessionKey"),pLogonAccounts->szSessionKey);
    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyAccounts"),true);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

    CTraceService::TraceString(TEXT("GSP_MB_EfficacyAccounts"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//������¼
bool CDataBaseEngineSink::OnMobileLogonOtherPlatform(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_MB_LogonOtherPlatform));
    if(wDataSize!=sizeof(DBR_MB_LogonOtherPlatform))
    {
      return false;
    }

    //������
    DBR_MB_LogonOtherPlatform * pLogonOtherPlatform=(DBR_MB_LogonOtherPlatform *)pData;

    //ִ���ж�
    tagBindParameter * pBindParameter=(tagBindParameter *)pLogonOtherPlatform->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pLogonOtherPlatform->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@cbPlatformID"),pLogonOtherPlatform->cbPlatformID);
    m_AccountsDBAide.AddParameter(TEXT("@strUserUin"),pLogonOtherPlatform->szUserUin);
    m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pLogonOtherPlatform->szNickName);
    m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pLogonOtherPlatform->szCompellation);
    m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pLogonOtherPlatform->cbGender);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonOtherPlatform->szMachineID);
    m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonOtherPlatform->szMobilePhone);
    m_AccountsDBAide.AddParameter(TEXT("@strSessionKey"),pLogonOtherPlatform->szSessionKey);
    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyOtherPlatform"),true);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

    CTraceService::TraceString(TEXT("GSP_MB_EfficacyOtherPlatform"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�ο͵�¼
bool CDataBaseEngineSink::OnMobileLogonVisitor(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_MB_LogonVisitor));
    if(wDataSize!=sizeof(DBR_MB_LogonVisitor))
    {
      return false;
    }

    //������
    DBR_MB_LogonVisitor * pVisitorLogon=(DBR_MB_LogonVisitor *)pData;

    //ִ���ж�
    tagBindParameter * pBindParameter=(tagBindParameter *)pVisitorLogon->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pVisitorLogon->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@cbPlatformID"),pVisitorLogon->cbPlatformID);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pVisitorLogon->szMachineID);
    m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pVisitorLogon->szMobilePhone);
    m_AccountsDBAide.AddParameter(TEXT("@strSessionKey"),pVisitorLogon->szSessionKey);
    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyLogonVisitor"),true);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������ϢF
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

    CTraceService::TraceString(TEXT("GSP_MB_EfficacyLogonVisitor"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�ʺ�ע��
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_MB_RegisterAccounts));
    if(wDataSize!=sizeof(DBR_MB_RegisterAccounts))
    {
      return false;
    }

    //������
    DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

    //ִ���ж�
    tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
    if(pBindParameter->dwSocketID!=dwContextID)
    {
      return true;
    }

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
    m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
    m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
    m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
    m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
    m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
    m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
    m_AccountsDBAide.AddParameter(TEXT("@cbDeviceType"),pRegisterAccounts->cbDeviceType);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_RegisterAccounts"),true);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0,true);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),0,true);

    CTraceService::TraceString(TEXT("GSP_MB_RegisterAccounts"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�޸Ļ���
bool CDataBaseEngineSink::OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ModifyMachine));
    if(wDataSize!=sizeof(DBR_GP_ModifyMachine))
    {
      return false;
    }

    //������
    DBR_GP_ModifyMachine * pModifyMachine=(DBR_GP_ModifyMachine *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    //�󶨲���
    if(pModifyMachine->cbBind==TRUE)
    {
      m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachine"),false);
    }
    else
    {
      m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UnMoorMachine"),false);
    }

    //�������
    if(m_AccountsDBAide.GetReturnValue()==DB_SUCCESS)
    {
      //��������
      DBO_GP_OperateSuccess OperateSuccess;
      ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

      //��ȡ��Ϣ
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OperateSuccess.lResultCode=m_AccountsDBModule->GetReturnValue();
      lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

      //���ͽ��
      WORD wStringSize=CountStringBuffer(OperateSuccess.szDescribeString);
      WORD wSendSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString)+wStringSize;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wSendSize);
    }
    else
    {
      //��������
      DBO_GP_OperateFailure OperateFailure;
      ZeroMemory(&OperateFailure,sizeof(OperateFailure));

      //��ȡ��Ϣ
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OperateFailure.lResultCode=m_AccountsDBModule->GetReturnValue();
      lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

      //���ͽ��
      WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
      WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_MoorMachine or GSP_GP_UnMoorMachine"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ModifySystemFace));
    if(wDataSize!=sizeof(DBR_GP_ModifySystemFace))
    {
      return false;
    }

    //������
    DBR_GP_ModifySystemFace * pModifySystemFace=(DBR_GP_ModifySystemFace *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pModifySystemFace->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifySystemFace->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifySystemFace->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pModifySystemFace->wFaceID);

    //������Ϣ
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifySystemFace->szMachineID);

    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    //�������
    if(m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SystemFaceInsert"),true)==DB_SUCCESS)
    {
      //��������
      DBO_GP_UserFaceInfo UserFaceInfo;
      ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

      //��ȡ��Ϣ
      UserFaceInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));

      //���ͽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
    }
    else
    {
      //��ȡ����
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
    }
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_SystemFaceInsert"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ModifyCustomFace));
    if(wDataSize!=sizeof(DBR_GP_ModifyCustomFace))
    {
      return false;
    }

    //������
    DBR_GP_ModifyCustomFace * pModifyCustomFace=(DBR_GP_ModifyCustomFace *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pModifyCustomFace->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //��������
    SAFEARRAYBOUND SafeArrayBound;
    ZeroMemory(&SafeArrayBound,sizeof(SafeArrayBound));

    //���ñ���
    SafeArrayBound.lLbound=0L;
    SafeArrayBound.cElements=sizeof(pModifyCustomFace->dwCustomFace);

    //��������
    BYTE * pcbCustomFace=(BYTE *)pModifyCustomFace->dwCustomFace;
    SAFEARRAY * pSafeArray=SafeArrayCreate(VT_UI1,1,&SafeArrayBound);

    //��������
    for(LONG nIndex=0; nIndex<sizeof(pModifyCustomFace->dwCustomFace); nIndex++)
    {
      SafeArrayPutElement(pSafeArray,&nIndex,pcbCustomFace+nIndex);
    }

    //���ñ���
    VARIANT VarChunk;
    VarChunk.parray=pSafeArray;
    VarChunk.vt=VT_ARRAY|VT_UI1;

    //��ȡ����
    ASSERT(m_AccountsDBModule.GetInterface()!=NULL);
    IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyCustomFace->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyCustomFace->szPassword);

    //ͷ����Ϣ
    pIDataBase->AddParameter(TEXT("@dwCustomFace"),adLongVarBinary,adParamInput,sizeof(pModifyCustomFace->dwCustomFace),CDBVarValue(VarChunk));

    //������Ϣ
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyCustomFace->szMachineID);

    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    //�ͷ���Դ
    SafeArrayDestroy(pSafeArray);

    //�������
    if(m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CustomFaceInsert"),true)==DB_SUCCESS)
    {
      //��������
      DBO_GP_UserFaceInfo UserFaceInfo;
      ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

      //��ȡ��Ϣ
      UserFaceInfo.wFaceID=INVALID_WORD;
      UserFaceInfo.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));

      //���ͽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
    }
    else
    {
      //��ȡ����
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
    }
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_CustomFaceInsert"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ModifyLogonPass));
    if(wDataSize!=sizeof(DBR_GP_ModifyLogonPass))
    {
      return false;
    }

    //������
    DBR_GP_ModifyLogonPass * pModifyLogonPass=(DBR_GP_ModifyLogonPass *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pModifyLogonPass->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyLogonPass->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyLogonPass->szScrPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyLogonPass->szDesPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyLogonPassword"),false);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_ModifyLogonPassword"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
    if(wDataSize!=sizeof(DBR_GP_ModifyInsurePass))
    {
      return false;
    }

    //������
    DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    //�������
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_ModifyInsurePassword"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�޸�ǩ��
bool CDataBaseEngineSink::OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ModifyUnderWrite));
    if(wDataSize!=sizeof(DBR_GP_ModifyUnderWrite))
    {
      return false;
    }

    //������
    DBR_GP_ModifyUnderWrite * pModifyUnderWrite=(DBR_GP_ModifyUnderWrite *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pModifyUnderWrite->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyUnderWrite->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyUnderWrite->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyUnderWrite->szUnderWrite);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUnderWrite"),false);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_ModifyUnderWrite"),TraceLevel_Exception);
    return false;
  }

  return true;
}

bool CDataBaseEngineSink::OnRequestModifyRealAuth(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_RealAuth));
    if(wDataSize!=sizeof(DBR_GP_RealAuth))
    {
      return false;
    }

    //������
    DBR_GP_RealAuth * pDataPara=(DBR_GP_RealAuth *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pDataPara->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pDataPara->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pDataPara->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pDataPara->szCompellation);
    m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pDataPara->szPassPortID);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RealAuth"),true);

    if(lResultCode == 0)
    {
      //�������
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
      OnIndividualDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal));
    }
    else
    {
      //�������
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
      OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_RealAuth"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ModifyIndividual));
    if(wDataSize!=sizeof(DBR_GP_ModifyIndividual))
    {
      return false;
    }

    //������
    DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pModifyIndividual->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyIndividual->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pModifyIndividual->cbGender);
    m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pModifyIndividual->szNickName);
    m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyIndividual->szUnderWrite);
    m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pModifyIndividual->szCompellation);
    m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pModifyIndividual->szPassPortID);
    m_AccountsDBAide.AddParameter(TEXT("@strQQ"),pModifyIndividual->szQQ);
    m_AccountsDBAide.AddParameter(TEXT("@strEMail"),pModifyIndividual->szEMail);
    m_AccountsDBAide.AddParameter(TEXT("@strSeatPhone"),pModifyIndividual->szSeatPhone);
    m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pModifyIndividual->szMobilePhone);
    m_AccountsDBAide.AddParameter(TEXT("@strDwellingPlace"),pModifyIndividual->szDwellingPlace);
    m_AccountsDBAide.AddParameter(TEXT("@strUserNote"),pModifyIndividual->szUserNote);
    m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pModifyIndividual->szSpreader);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUserIndividual"),true);

    //�������
    CDBVarValue DBVarValue;
    m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_ModifyUserIndividual"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��ͨ����
bool CDataBaseEngineSink::OnRequestUserEnableInsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_UserEnableInsure));
    if(wDataSize!=sizeof(DBR_GP_UserEnableInsure))
    {
      return false;
    }

    //������
    DBR_GP_UserEnableInsure * pUserEnableInsure=(DBR_GP_UserEnableInsure *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pUserEnableInsure->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserEnableInsure->dwUserID);
    m_TreasureDBAide.AddParameter(TEXT("@strLogonPass"),pUserEnableInsure->szLogonPass);
    m_TreasureDBAide.AddParameter(TEXT("@strInsurePass"),pUserEnableInsure->szInsurePass);
    m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserEnableInsure->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserEnableInsure"),true);

    //�������
    CDBVarValue DBVarValue;
    m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

    //�������
    DBO_GP_UserInsureEnableResult UserEnableInsureResult;
    ZeroMemory(&UserEnableInsureResult,sizeof(UserEnableInsureResult));

    //���ñ���
    UserEnableInsureResult.cbInsureEnabled=(lResultCode==DB_SUCCESS)?TRUE:FALSE;
    lstrcpyn(UserEnableInsureResult.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(UserEnableInsureResult.szDescribeString));

    //���ͽ��
    WORD wDataSize=CountStringBuffer(UserEnableInsureResult.szDescribeString);
    WORD wHeadSize=sizeof(UserEnableInsureResult)-sizeof(UserEnableInsureResult.szDescribeString);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_ENABLE_RESULT,dwContextID,&UserEnableInsureResult,wHeadSize+wDataSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    DBO_GP_UserInsureEnableResult UserEnableInsureResult;
    ZeroMemory(&UserEnableInsureResult,sizeof(UserEnableInsureResult));

    //���ñ���
    UserEnableInsureResult.cbInsureEnabled=FALSE;
    lstrcpyn(UserEnableInsureResult.szDescribeString,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(UserEnableInsureResult.szDescribeString));

    //���ͽ��
    WORD wDataSize=CountStringBuffer(UserEnableInsureResult.szDescribeString);
    WORD wHeadSize=sizeof(UserEnableInsureResult)-sizeof(UserEnableInsureResult.szDescribeString);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_ENABLE_RESULT,dwContextID,&UserEnableInsureResult,wHeadSize+wDataSize);

    CTraceService::TraceString(TEXT("GSP_GR_UserEnableInsure"),TraceLevel_Exception);
    return false;
  }
}

//������Ϸ��
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_UserSaveScore));
    if(wDataSize!=sizeof(DBR_GP_UserSaveScore))
    {
      return false;
    }

    //������
    DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
    m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
    m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
    m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
    m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserSaveScore"),true);

    //�������
    CDBVarValue DBVarValue;
    m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GR_UserSaveScore"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��ȡ��Ϸ��
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_UserTakeScore));
    if(wDataSize!=sizeof(DBR_GP_UserTakeScore))
    {
      return false;
    }

    //������
    DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
    m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
    m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
    m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
    m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
    m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTakeScore"),true);

    //�������
    CDBVarValue DBVarValue;
    m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GR_UserTakeScore"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//ת����Ϸ��
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_UserTransferScore));
    if(wDataSize!=sizeof(DBR_GP_UserTransferScore))
    {
      return false;
    }

    //������
    DBR_GP_UserTransferScore * pUserTransferScore=(DBR_GP_UserTransferScore *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
    m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransferScore->lTransferScore);
    m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTransferScore->szPassword);
    m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pUserTransferScore->szAccounts);
    m_TreasureDBAide.AddParameter(TEXT("@strTransRemark"),pUserTransferScore->szTransRemark);
    m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
    m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
    m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

    //�������
    CDBVarValue DBVarValue;
    m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //������
    OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GR_UserTransferScore"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��������
bool CDataBaseEngineSink::OnRequestLotteryConfigReq(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_LotteryConfigReq));
    if(wDataSize!=sizeof(DBR_GP_LotteryConfigReq))
    {
      return false;
    }

    //������
    DBR_GP_LotteryConfigReq * pLotteryConfigReq=(DBR_GP_LotteryConfigReq *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pLotteryConfigReq->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@wKindID"),pLotteryConfigReq->wKindID);


    //�齱����
    LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadLotteryConfig"),true);
    ASSERT(lResultCode==DB_SUCCESS);
    if(lResultCode==DB_SUCCESS)
    {
      //������
      DBO_GP_LotteryConfig LotteryConfig;
      ZeroMemory(&LotteryConfig,sizeof(LotteryConfig));

      //��������
      WORD wItemCount=0;
      while(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        //��ȡ����
        LotteryConfig.LotteryItem[wItemCount].cbItemIndex = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemIndex"));
        LotteryConfig.LotteryItem[wItemCount].cbItemType = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemType"));
        LotteryConfig.LotteryItem[wItemCount].lItemQuota = m_PlatformDBAide.GetValue_WORD(TEXT("ItemQuota"));
        wItemCount++;

        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }

      LotteryConfig.wLotteryCount = wItemCount;

      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_CONFIG,dwContextID,&LotteryConfig,sizeof(LotteryConfig));
    }
    else
    {
      CTraceService::TraceString(TEXT("�齱���ü���ʧ�ܣ�"),TraceLevel_Exception);
    }

    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@wKindID"),pLotteryConfigReq->wKindID);
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pLotteryConfigReq->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strLogonPass"),pLotteryConfigReq->szLogonPass);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //�齱��Ϣ
    lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LotteryUserInfo"),true);
    if(lResultCode==DB_SUCCESS)
    {
      //������
      DBO_GP_LotteryUserInfo LotteryUserInfo;
      ZeroMemory(&LotteryUserInfo,sizeof(LotteryUserInfo));

      LotteryUserInfo.wKindID = pLotteryConfigReq->wKindID;
      LotteryUserInfo.dwUserID = pLotteryConfigReq->dwUserID;
      LotteryUserInfo.cbFreeCount = m_PlatformDBAide.GetValue_BYTE(TEXT("FreeCount"));
      LotteryUserInfo.cbAlreadyCount = m_PlatformDBAide.GetValue_BYTE(TEXT("AlreadyCount"));
      LotteryUserInfo.lChargeFee = m_PlatformDBAide.GetValue_LONGLONG(TEXT("ChargeFee"));

      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_USER_INFO,dwContextID,&LotteryUserInfo,sizeof(LotteryUserInfo));
    }
    else
    {
      //��������
      DBO_GP_OperateFailure OperateFailure;
      ZeroMemory(&OperateFailure,sizeof(OperateFailure));

      //��ȡ��Ϣ
      CDBVarValue DBVarValue;
      m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OperateFailure.lResultCode=m_PlatformDBModule->GetReturnValue();
      lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

      //���ͽ��
      WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
      WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_LotteryUserInfo"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�齱��ʼ
bool CDataBaseEngineSink::OnRequestLotteryStart(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_LotteryStart));
    if(wDataSize!=sizeof(DBR_GP_LotteryStart))
    {
      return false;
    }

    //������
    DBR_GP_LotteryStart * pLotteryStart=(DBR_GP_LotteryStart *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pLotteryStart->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@wKindID"),pLotteryStart->wKindID);
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pLotteryStart->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strLogonPass"),pLotteryStart->szLogonPass);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pLotteryStart->szMachineID);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LotteryStart"),true);

    if(lResultCode==DB_SUCCESS)
    {
      //������
      DBO_GP_LotteryResult LotteryResult;
      ZeroMemory(&LotteryResult,sizeof(LotteryResult));

      LotteryResult.wKindID = pLotteryStart->wKindID;
      LotteryResult.dwUserID = pLotteryStart->dwUserID;
      LotteryResult.bWined = (m_PlatformDBAide.GetValue_BYTE(TEXT("Wined"))==TRUE);
      LotteryResult.lUserScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("UserScore"));
      LotteryResult.dUserBeans = m_PlatformDBAide.GetValue_DOUBLE(TEXT("UserBeans"));
      LotteryResult.LotteryItem.cbItemIndex = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemIndex"));
      LotteryResult.LotteryItem.cbItemType = m_PlatformDBAide.GetValue_BYTE(TEXT("ItemType"));
      LotteryResult.LotteryItem.lItemQuota = m_PlatformDBAide.GetValue_LONGLONG(TEXT("ItemQuota"));

      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_RESULT,dwContextID,&LotteryResult,sizeof(LotteryResult));
    }
    else
    {
      //��������
      DBO_GP_OperateFailure OperateFailure;
      ZeroMemory(&OperateFailure,sizeof(OperateFailure));

      //��ȡ��Ϣ
      CDBVarValue DBVarValue;
      m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OperateFailure.lResultCode=m_PlatformDBModule->GetReturnValue();
      lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

      //���ͽ��
      WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
      WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_LotteryStart"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��Ϸ����
bool CDataBaseEngineSink::OnRequestQueryUserGameData(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_QueryUserGameData));
    if(wDataSize!=sizeof(DBR_GP_QueryUserGameData))
    {
      return false;
    }

    //������
    DBR_GP_QueryUserGameData * pQueryUserGameData=(DBR_GP_QueryUserGameData *)pData;

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@wKindID"),pQueryUserGameData->wKindID);
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryUserGameData->dwUserID);
    m_TreasureDBAide.AddParameter(TEXT("@strDynamicPass"),pQueryUserGameData->szDynamicPass);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�в�ѯ
    LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserGameData"),true);

    if(lResultCode==DB_SUCCESS)
    {
      //������
      DBO_GP_QueryUserGameData QueryUserGameData;
      ZeroMemory(&QueryUserGameData,sizeof(QueryUserGameData));

      QueryUserGameData.wKindID=pQueryUserGameData->wKindID;
      QueryUserGameData.dwUserID=pQueryUserGameData->dwUserID;
      m_TreasureDBAide.GetValue_String(TEXT("UserGameData"),QueryUserGameData.szUserGameData,CountArray(QueryUserGameData.szUserGameData));

      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_USER_GAME_DATA,dwContextID,&QueryUserGameData,sizeof(QueryUserGameData));
    }
    else
    {
      //��������
      DBO_GP_OperateFailure OperateFailure;
      ZeroMemory(&OperateFailure,sizeof(OperateFailure));

      //��ȡ��Ϣ
      CDBVarValue DBVarValue;
      m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OperateFailure.lResultCode=m_TreasureDBModule->GetReturnValue();
      lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

      //���ͽ��
      WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
      WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GR_QueryUserGameData"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�ʺŰ�
bool CDataBaseEngineSink::OnRequestAccountBind(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_AccountBind));
    if(wDataSize!=sizeof(DBR_GP_AccountBind))
    {
      return false;
    }

    //������
    DBR_GP_AccountBind * pModifyMachine=(DBR_GP_AccountBind *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

    m_AccountsDBAide.AddParameter(TEXT("@strBindAccounts"),pModifyMachine->szBindNewAccounts);
    m_AccountsDBAide.AddParameter(TEXT("@strBindPassword"),pModifyMachine->szBindNewPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strBindSpreader"),pModifyMachine->szBindNewSpreader);


    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AccountBind"),false);

    //�������
    if(m_AccountsDBAide.GetReturnValue()==DB_SUCCESS)
    {
      //��������
      DBO_GP_OperateSuccess OperateSuccess;
      ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

      //��ȡ��Ϣ
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OperateSuccess.lResultCode=m_AccountsDBModule->GetReturnValue();
      lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

      //���ͽ��
      WORD wStringSize=CountStringBuffer(OperateSuccess.szDescribeString);
      WORD wSendSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString)+wStringSize;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wSendSize);
    }
    else
    {
      //��������
      DBO_GP_OperateFailure OperateFailure;
      ZeroMemory(&OperateFailure,sizeof(OperateFailure));

      //��ȡ��Ϣ
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OperateFailure.lResultCode=m_AccountsDBModule->GetReturnValue();
      lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

      //���ͽ��
      WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
      WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_AccountBind"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�ʺŰ�
bool CDataBaseEngineSink::OnRequestAccountBindExists(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_AccountBind_Exists));
    if(wDataSize!=sizeof(DBR_GP_AccountBind_Exists))
    {
      return false;
    }

    //������
    DBR_GP_AccountBind_Exists * pModifyMachine=(DBR_GP_AccountBind_Exists *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

    m_AccountsDBAide.AddParameter(TEXT("@strBindAccounts"),pModifyMachine->szBindExistsAccounts);
    m_AccountsDBAide.AddParameter(TEXT("@strBindPassword"),pModifyMachine->szBindExistsPassword);

    //�������
    WCHAR szDescribe[128]=L"";
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

    m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AccountBindExists"),false);

    //�������
    if(m_AccountsDBAide.GetReturnValue()==DB_SUCCESS)
    {
      //��������
      DBO_GP_OperateSuccess OperateSuccess;
      ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

      //��ȡ��Ϣ
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OperateSuccess.lResultCode=m_AccountsDBModule->GetReturnValue();
      lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

      //���ͽ��
      WORD wStringSize=CountStringBuffer(OperateSuccess.szDescribeString);
      WORD wSendSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString)+wStringSize;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wSendSize);
    }
    else
    {
      //��������
      DBO_GP_OperateFailure OperateFailure;
      ZeroMemory(&OperateFailure,sizeof(OperateFailure));

      //��ȡ��Ϣ
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //�������
      OperateFailure.lResultCode=m_AccountsDBModule->GetReturnValue();
      lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

      //���ͽ��
      WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
      WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_AccountBindExists"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��ȡ����
bool CDataBaseEngineSink::OnRequestGetParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_GetParameter));
    if(wDataSize!=sizeof(DBR_GP_GetParameter))
    {
      return false;
    }

    //������
    DBR_GP_GetParameter * pGetParameter=(DBR_GP_GetParameter *)pData;

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@wServerID"),pGetParameter->wServerID);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidGetParameter"),true);

    //ִ�н��
    OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_GET_PARAMETER,pGetParameter->wServerID);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("���ݿ��쳣,���Ժ����ԣ�"),false,false);

    CTraceService::TraceString(TEXT("SUB_GP_GET_PARAMETER"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��Ӳ���
bool CDataBaseEngineSink::OnRequestAddParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_AddParameter));
    if(wDataSize!=sizeof(DBR_GP_AddParameter))
    {
      return false;
    }

    //������
    DBR_GP_AddParameter * pAddParameter=(DBR_GP_AddParameter *)pData;

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@wServerID"),pAddParameter->wServerID);
    m_AccountsDBAide.AddParameter(TEXT("@dwServiceMode"),pAddParameter->AndroidParameter.dwServiceMode);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCount"),pAddParameter->AndroidParameter.dwAndroidCount);
    m_AccountsDBAide.AddParameter(TEXT("@dwEnterTime"),pAddParameter->AndroidParameter.dwEnterTime);
    m_AccountsDBAide.AddParameter(TEXT("@dwLeaveTime"),pAddParameter->AndroidParameter.dwLeaveTime);
    m_AccountsDBAide.AddParameter(TEXT("@dwEnterMinInterval"),pAddParameter->AndroidParameter.dwEnterMinInterval);
    m_AccountsDBAide.AddParameter(TEXT("@dwEnterMaxInterval"),pAddParameter->AndroidParameter.dwEnterMaxInterval);
    m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMinInterval"),pAddParameter->AndroidParameter.dwLeaveMinInterval);
    m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMaxInterval"),pAddParameter->AndroidParameter.dwLeaveMaxInterval);
    m_AccountsDBAide.AddParameter(TEXT("@lTakeMinScore"),pAddParameter->AndroidParameter.lTakeMinScore);
    m_AccountsDBAide.AddParameter(TEXT("@lTakeMaxScore"),pAddParameter->AndroidParameter.lTakeMaxScore);
    m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMinInnings"),pAddParameter->AndroidParameter.dwSwitchMinInnings);
    m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMaxInnings"),pAddParameter->AndroidParameter.dwSwitchMaxInnings);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember0"),pAddParameter->AndroidParameter.AndroidCountMember0);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember1"),pAddParameter->AndroidParameter.AndroidCountMember1);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember2"),pAddParameter->AndroidParameter.AndroidCountMember2);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember3"),pAddParameter->AndroidParameter.AndroidCountMember3);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember4"),pAddParameter->AndroidParameter.AndroidCountMember4);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember5"),pAddParameter->AndroidParameter.AndroidCountMember5);
    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidAddParameter"),true);

    //ִ�н��
    OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_ADD_PARAMETER,pAddParameter->wServerID);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("���ݿ��쳣,���Ժ����ԣ�"),false,false);

    CTraceService::TraceString(TEXT("GSP_GP_AndroidAddParameter"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�޸Ĳ���
bool CDataBaseEngineSink::OnRequestModifyParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ModifyParameter));
    if(wDataSize!=sizeof(DBR_GP_ModifyParameter))
    {
      return false;
    }

    //������
    DBR_GP_ModifyParameter * pModifyParameter=(DBR_GP_ModifyParameter *)pData;

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwDatchID"),pModifyParameter->AndroidParameter.dwBatchID);
    m_AccountsDBAide.AddParameter(TEXT("@dwServiceMode"),pModifyParameter->AndroidParameter.dwServiceMode);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCount"),pModifyParameter->AndroidParameter.dwAndroidCount);
    m_AccountsDBAide.AddParameter(TEXT("@dwEnterTime"),pModifyParameter->AndroidParameter.dwEnterTime);
    m_AccountsDBAide.AddParameter(TEXT("@dwLeaveTime"),pModifyParameter->AndroidParameter.dwLeaveTime);
    m_AccountsDBAide.AddParameter(TEXT("@dwEnterMinInterval"),pModifyParameter->AndroidParameter.dwEnterMinInterval);
    m_AccountsDBAide.AddParameter(TEXT("@dwEnterMaxInterval"),pModifyParameter->AndroidParameter.dwEnterMaxInterval);
    m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMinInterval"),pModifyParameter->AndroidParameter.dwLeaveMinInterval);
    m_AccountsDBAide.AddParameter(TEXT("@dwLeaveMaxInterval"),pModifyParameter->AndroidParameter.dwLeaveMaxInterval);
    m_AccountsDBAide.AddParameter(TEXT("@lTakeMinScore"),pModifyParameter->AndroidParameter.lTakeMinScore);
    m_AccountsDBAide.AddParameter(TEXT("@lTakeMaxScore"),pModifyParameter->AndroidParameter.lTakeMaxScore);
    m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMinInnings"),pModifyParameter->AndroidParameter.dwSwitchMinInnings);
    m_AccountsDBAide.AddParameter(TEXT("@dwSwitchMaxInnings"),pModifyParameter->AndroidParameter.dwSwitchMaxInnings);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember0"),pModifyParameter->AndroidParameter.AndroidCountMember0);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember1"),pModifyParameter->AndroidParameter.AndroidCountMember1);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember2"),pModifyParameter->AndroidParameter.AndroidCountMember2);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember3"),pModifyParameter->AndroidParameter.AndroidCountMember3);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember4"),pModifyParameter->AndroidParameter.AndroidCountMember4);
    m_AccountsDBAide.AddParameter(TEXT("@dwAndroidCountMember5"),pModifyParameter->AndroidParameter.AndroidCountMember5);
    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidModifyParameter"),true);

    //ִ�н��
    OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_MODIFY_PARAMETER,pModifyParameter->wServerID);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("���ݿ��쳣,���Ժ����ԣ�"),false,false);

    CTraceService::TraceString(TEXT("GSP_GP_AndroidModifyParameter"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//ɾ������
bool CDataBaseEngineSink::OnRequestDeleteParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_DeleteParameter));
    if(wDataSize!=sizeof(DBR_GP_DeleteParameter))
    {
      return false;
    }

    //������
    DBR_GP_DeleteParameter * pDeleteParameter=(DBR_GP_DeleteParameter *)pData;

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwBatchID"),pDeleteParameter->dwBatchID);

    //ִ�в�ѯ
    LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_AndroidDeleteParameter"),true);

    //ִ�н��
    OnAndroidDisposeResult(dwContextID,lResultCode,SUB_GP_DELETE_PARAMETER,pDeleteParameter->wServerID);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //������Ϣ
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("���ݿ��쳣,���Ժ����ԣ�"),false,false);

    CTraceService::TraceString(TEXT("GSP_GP_AndroidDeleteParameter"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_QueryIndividual));
    if(wDataSize!=sizeof(DBR_GP_QueryIndividual))
    {
      return false;
    }

    //������
    DBR_GP_QueryIndividual * pQueryIndividual=(DBR_GP_QueryIndividual *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pQueryIndividual->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_AccountsDBAide.ResetParameter();
    m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryIndividual->dwUserID);
    m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pQueryIndividual->szPassword);
    m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //�������
    if(m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserIndividual"),true)==DB_SUCCESS)
    {
      //��������
      DBO_GP_UserIndividual UserIndividual;
      ZeroMemory(&UserIndividual,sizeof(UserIndividual));

      //�û���Ϣ
      UserIndividual.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
      m_AccountsDBAide.GetValue_String(TEXT("UserNote"),UserIndividual.szUserNote,CountArray(UserIndividual.szUserNote));
      m_AccountsDBAide.GetValue_String(TEXT("Compellation"),UserIndividual.szCompellation,CountArray(UserIndividual.szCompellation));
      m_AccountsDBAide.GetValue_String(TEXT("PassPortID"),UserIndividual.szPassPortID,CountArray(UserIndividual.szPassPortID));

      //�绰����
      m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),UserIndividual.szSeatPhone,CountArray(UserIndividual.szSeatPhone));
      m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),UserIndividual.szMobilePhone,CountArray(UserIndividual.szMobilePhone));

      //��ϵ����
      m_AccountsDBAide.GetValue_String(TEXT("QQ"),UserIndividual.szQQ,CountArray(UserIndividual.szQQ));
      m_AccountsDBAide.GetValue_String(TEXT("EMail"),UserIndividual.szEMail,CountArray(UserIndividual.szEMail));
      m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),UserIndividual.szDwellingPlace,CountArray(UserIndividual.szDwellingPlace));

      //�ƹ���Ϣ
      m_AccountsDBAide.GetValue_String(TEXT("Spreader"),UserIndividual.szSpreader,CountArray(UserIndividual.szSpreader));

      //���ͽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INDIVIDUAL,dwContextID,&UserIndividual,sizeof(UserIndividual));
    }
    else
    {
      //��ȡ����
      CDBVarValue DBVarValue;
      m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //������
      OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GP_QueryUserIndividual"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
    if(wDataSize!=sizeof(DBR_GP_QueryInsureInfo))
    {
      return false;
    }

    //������
    DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
    m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pQueryInsureInfo->szPassword);
    m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //�������
    if(m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserInsureInfo"),true)==DB_SUCCESS)
    {
      //��������
      DBO_GP_UserInsureInfo UserInsureInfo;
      ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

      //������Ϣ
      UserInsureInfo.cbEnjoinTransfer=m_TreasureDBAide.GetValue_BYTE(TEXT("EnjoinTransfer"));
      UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
      UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
      UserInsureInfo.wRevenueTransferMember=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransferMember"));
      UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
      UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
      UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
      UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
      UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

      //���ͽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
    }
    else
    {
      //��ȡ����
      CDBVarValue DBVarValue;
      m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //������
      OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GR_QueryUserInsureInfo"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��ѯ�û�
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  //Ч�����
  ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureUserInfo));
  if(wDataSize!=sizeof(DBR_GP_QueryInsureUserInfo))
  {
    return false;
  }

  //������
  DBR_GP_QueryInsureUserInfo * pQueryTransferUserInfo=(DBR_GP_QueryInsureUserInfo *)pData;

  try
  {
    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pQueryTransferUserInfo->cbByNickName);
    m_TreasureDBAide.AddParameter(TEXT("@strAccounts"),pQueryTransferUserInfo->szAccounts);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //�������
    if(m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTransferUserInfo"),true)==DB_SUCCESS)
    {
      //��������
      DBO_GP_UserTransferUserInfo TransferUserInfo;
      ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

      //������Ϣ
      TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
      m_TreasureDBAide.GetValue_String(TEXT("Accounts"), TransferUserInfo.szAccounts, CountArray(TransferUserInfo.szAccounts));

      //���ͽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
    }
    else
    {
      //��ȡ����
      CDBVarValue DBVarValue;
      m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //������
      OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //�������
    OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

    CTraceService::TraceString(TEXT("GSP_GR_QueryTransferUserInfo"),TraceLevel_Exception);
    return false;
  }
}

//���ؽ���
bool CDataBaseEngineSink::OnRequestCheckInReward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //�������
    m_PlatformDBAide.ResetParameter();

    //ִ������
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadCheckInReward"),true);

    //����ṹ
    DBO_GP_CheckInReward CheckInReward;
    ZeroMemory(&CheckInReward,sizeof(CheckInReward));

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //��������
      WORD wDayIndex=0;
      while(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        wDayIndex = m_PlatformDBAide.GetValue_WORD(TEXT("DayID"));
        if(wDayIndex<=LEN_WEEK)
        {
          CheckInReward.lRewardGold[wDayIndex-1] = m_PlatformDBAide.GetValue_LONGLONG(TEXT("RewardGold"));
        }

        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }

      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_REWARD,dwContextID,&CheckInReward,sizeof(CheckInReward));
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_LoadCheckInReward"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��ѯǩ��
bool CDataBaseEngineSink::OnRequestCheckInQueryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_CheckInQueryInfo));
    if(wDataSize!=sizeof(DBR_GP_CheckInQueryInfo))
    {
      return false;
    }

    //������
    DBR_GP_CheckInQueryInfo * pCheckInQueryInfo=(DBR_GP_CheckInQueryInfo *)pData;

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pCheckInQueryInfo->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pCheckInQueryInfo->szPassword);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //�������
    if(m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_CheckInQueryInfo"),true)==DB_SUCCESS)
    {
      //��������
      DBO_GP_CheckInInfo CheckInInfo;
      ZeroMemory(&CheckInInfo,sizeof(CheckInInfo));

      //������Ϣ
      CheckInInfo.wSeriesDate=m_PlatformDBAide.GetValue_WORD(TEXT("SeriesDate"));
      CheckInInfo.bTodayChecked = (m_PlatformDBAide.GetValue_BYTE(TEXT("TodayCheckIned"))==TRUE);

      //���ͽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_INFO,dwContextID,&CheckInInfo,sizeof(CheckInInfo));
    }
    else
    {
      //��ȡ����
      CDBVarValue DBVarValue;
      m_PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //����ṹ
      DBO_GP_CheckInResult CheckInResult;
      CheckInResult.bSuccessed=false;
      lstrcpyn(CheckInResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(CheckInResult.szNotifyContent));

      //���ͽ��
      WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_CheckInResult CheckInResult;
    CheckInResult.bSuccessed=false;
    lstrcpyn(CheckInResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(CheckInResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

    CTraceService::TraceString(TEXT("GSP_GR_CheckInQueryInfo"),TraceLevel_Exception);
    return false;
  }
}

//ִ��ǩ��
bool CDataBaseEngineSink::OnRequestCheckInDone(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_CheckInDone));
    if(wDataSize!=sizeof(DBR_GP_CheckInDone))
    {
      return false;
    }

    //������
    DBR_GP_CheckInDone * pCheckInDone=(DBR_GP_CheckInDone *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pCheckInDone->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pCheckInDone->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pCheckInDone->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pCheckInDone->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_CheckInDone"),true);

    //��������
    DBO_GP_CheckInResult CheckInResult;
    ZeroMemory(&CheckInResult,sizeof(CheckInResult));

    //��ȡ����
    if(lResultCode==DB_SUCCESS && m_PlatformDBModule->IsRecordsetEnd()==false)
    {
      CheckInResult.lScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Score"));
    }

    //��ȡ����
    CDBVarValue DBVarValue;
    m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

    //������Ϣ
    CheckInResult.bSuccessed=lResultCode==DB_SUCCESS;
    lstrcpyn(CheckInResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(CheckInResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_CheckInResult CheckInResult;
    CheckInResult.bSuccessed=false;
    lstrcpyn(CheckInResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(CheckInResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(CheckInResult)-sizeof(CheckInResult.szNotifyContent)+CountStringBuffer(CheckInResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHECKIN_RESULT,dwContextID,&CheckInResult,wSendSize);

    CTraceService::TraceString(TEXT("GSP_GR_CheckInDone"),TraceLevel_Exception);
    return false;
  }
}

//��������
bool CDataBaseEngineSink::OnRequestTaskGiveUp(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_TaskGiveUP));
    if(wDataSize!=sizeof(DBR_GP_TaskGiveUP))
    {
      return false;
    }

    //������
    DBR_GP_TaskGiveUP * pTake=(DBR_GP_TaskGiveUP *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pTake->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pTake->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@wTaskID"),pTake->wTaskID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pTake->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pTake->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_TaskGiveUp"),false);

    //��������
    DBO_GP_TaskResult TaskResult;
    ZeroMemory(&TaskResult,sizeof(TaskResult));

    //��ȡ����
    CDBVarValue DBVarValue;
    m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

    //������Ϣ
    TaskResult.wCommandID=SUB_GP_TASK_GIVEUP;
    TaskResult.bSuccessed=lResultCode==DB_SUCCESS;
    lstrcpyn(TaskResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(TaskResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_TaskResult TaskResult;
    TaskResult.bSuccessed=false;
    TaskResult.wCommandID=SUB_GP_TASK_TAKE;
    lstrcpyn(TaskResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(TaskResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

    CTraceService::TraceString(TEXT("GSP_GR_TaskGiveUp"),TraceLevel_Exception);
    return false;
  }
}


//��ȡ����
bool CDataBaseEngineSink::OnRequestTaskTake(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_TaskTake));
    if(wDataSize!=sizeof(DBR_GP_TaskTake))
    {
      return false;
    }

    //������
    DBR_GP_TaskTake * pTaskTake=(DBR_GP_TaskTake *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pTaskTake->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pTaskTake->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@wTaskID"),pTaskTake->wTaskID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pTaskTake->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pTaskTake->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_TaskTake"),false);

    //��������
    DBO_GP_TaskResult TaskResult;
    ZeroMemory(&TaskResult,sizeof(TaskResult));

    //��ȡ����
    CDBVarValue DBVarValue;
    m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

    //������Ϣ
    TaskResult.wCommandID=SUB_GP_TASK_TAKE;
    TaskResult.bSuccessed=lResultCode==DB_SUCCESS;
    lstrcpyn(TaskResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(TaskResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_TaskResult TaskResult;
    TaskResult.bSuccessed=false;
    TaskResult.wCommandID=SUB_GP_TASK_TAKE;
    lstrcpyn(TaskResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(TaskResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

    CTraceService::TraceString(TEXT("GSP_GR_TaskTake"),TraceLevel_Exception);
    return false;
  }
}

//��ȡ����
bool CDataBaseEngineSink::OnRequestTaskReward(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_TaskReward));
    if(wDataSize!=sizeof(DBR_GP_TaskReward))
    {
      return false;
    }

    //������
    DBR_GP_TaskReward * pTaskReward=(DBR_GP_TaskReward *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pTaskReward->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pTaskReward->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@wTaskID"),pTaskReward->wTaskID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pTaskReward->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pTaskReward->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_TaskReward"),true);

    //��������
    DBO_GP_TaskResult TaskResult;
    ZeroMemory(&TaskResult,sizeof(TaskResult));

    //��ȡ����
    CDBVarValue DBVarValue;
    m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

    //������Ϣ
    TaskResult.wCommandID=SUB_GP_TASK_REWARD;
    TaskResult.bSuccessed=lResultCode==DB_SUCCESS;
    lstrcpyn(TaskResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(TaskResult.szNotifyContent));

    //��ȡ����
    if(TaskResult.bSuccessed==true)
    {
      TaskResult.lCurrScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Score"));
      TaskResult.lCurrIngot= m_PlatformDBAide.GetValue_LONGLONG(TEXT("Ingot"));
    }

    //���ͽ��
    WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_TaskResult TaskResult;
    TaskResult.bSuccessed=false;
    TaskResult.wCommandID=SUB_GP_TASK_REWARD;
    lstrcpyn(TaskResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(TaskResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

    CTraceService::TraceString(TEXT("GSP_GR_TaskReward"),TraceLevel_Exception);
    return false;
  }
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestTaskQueryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_TaskQueryInfo));
    if(wDataSize!=sizeof(DBR_GP_TaskQueryInfo))
    {
      return false;
    }

    //������
    DBR_GP_TaskQueryInfo * pTaskQueryInfo=(DBR_GP_TaskQueryInfo *)pData;

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@wKindID"),0);
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pTaskQueryInfo->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pTaskQueryInfo->szPassword);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTaskInfo"),true);

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //��������
      DBO_GP_TaskInfo TaskInfo;
      tagTaskStatus * pTaskStatus=NULL;
      ZeroMemory(&TaskInfo,sizeof(TaskInfo));

      //��������
      while(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        //���ñ���
        pTaskStatus = &TaskInfo.TaskStatus[TaskInfo.wTaskCount++];

        //��ȡ����
        pTaskStatus->wTaskID = m_PlatformDBAide.GetValue_WORD(TEXT("TaskID"));
        pTaskStatus->cbTaskStatus = m_PlatformDBAide.GetValue_BYTE(TEXT("TaskStatus"));
        pTaskStatus->wTaskProgress = m_PlatformDBAide.GetValue_WORD(TEXT("Progress"));

        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }

      //���ͽ��
      WORD wSendDataSize = sizeof(TaskInfo)-sizeof(TaskInfo.TaskStatus);
      wSendDataSize += sizeof(TaskInfo.TaskStatus[0])*TaskInfo.wTaskCount;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_INFO,dwContextID,&TaskInfo,wSendDataSize);
    }
    else
    {
      //��������
      DBO_GP_TaskResult TaskResult;
      ZeroMemory(&TaskResult,sizeof(TaskResult));

      //��ȡ����
      CDBVarValue DBVarValue;
      m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

      //������Ϣ
      TaskResult.bSuccessed=false;
      TaskResult.wCommandID=SUB_GP_TASK_LOAD;
      lstrcpyn(TaskResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(TaskResult.szNotifyContent));

      //���ͽ��
      WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_TaskResult TaskResult;
    TaskResult.bSuccessed=false;
    TaskResult.wCommandID=SUB_GP_TASK_LOAD;
    lstrcpyn(TaskResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(TaskResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(TaskResult)-sizeof(TaskResult.szNotifyContent)+CountStringBuffer(TaskResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_RESULT,dwContextID,&TaskResult,wSendSize);

    CTraceService::TraceString(TEXT("GSP_GR_QueryTaskInfo"),TraceLevel_Exception);
    return false;
  }
}

//�ͱ�����
bool CDataBaseEngineSink::OnRequestLoadBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //�������
    m_PlatformDBAide.ResetParameter();

    //ִ������
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadBaseEnsure"),true);

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //����ṹ
      DBO_GP_BaseEnsureParameter BaseEnsureParameter;
      ZeroMemory(&BaseEnsureParameter,sizeof(BaseEnsureParameter));

      //��������
      if(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        BaseEnsureParameter.cbTakeTimes= m_PlatformDBAide.GetValue_BYTE(TEXT("TakeTimes"));
        BaseEnsureParameter.lScoreAmount = m_PlatformDBAide.GetValue_WORD(TEXT("ScoreAmount"));
        BaseEnsureParameter.lScoreCondition = m_PlatformDBAide.GetValue_WORD(TEXT("ScoreCondition"));

        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }

      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_PARAMETER,dwContextID,&BaseEnsureParameter,sizeof(BaseEnsureParameter));
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_LoadBaseEnsure"),TraceLevel_Exception);
    return false;
  }
}

//��ȡ�ͱ�
bool CDataBaseEngineSink::OnRequestTakeBaseEnsure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_TakeBaseEnsure));
    if(wDataSize!=sizeof(DBR_GP_TakeBaseEnsure))
    {
      return false;
    }

    //��ȡ����
    DBR_GP_TakeBaseEnsure * pTakeBaseEnsure=(DBR_GP_TakeBaseEnsure *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pTakeBaseEnsure->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pTakeBaseEnsure->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pTakeBaseEnsure->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pTakeBaseEnsure->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_TakeBaseEnsure"),true);

    //��������
    DBO_GP_BaseEnsureResult BaseEnsureResult;
    ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

    //��ȡ����
    if(lResultCode==DB_SUCCESS && m_PlatformDBModule->IsRecordsetEnd()==false)
    {
      BaseEnsureResult.lGameScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Score"));
    }

    //��ȡ����
    CDBVarValue DBVarValue;
    m_PlatformDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);

    //������Ϣ
    BaseEnsureResult.bSuccessed=lResultCode==DB_SUCCESS;
    lstrcpyn(BaseEnsureResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(BaseEnsureResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
    wSendSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_BaseEnsureResult BaseEnsureResult;
    ZeroMemory(&BaseEnsureResult,sizeof(BaseEnsureResult));

    //���ñ���
    BaseEnsureResult.bSuccessed=false;
    lstrcpyn(BaseEnsureResult.szNotifyContent,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(BaseEnsureResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(BaseEnsureResult)-sizeof(BaseEnsureResult.szNotifyContent);
    wSendSize+=CountStringBuffer(BaseEnsureResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BASEENSURE_RESULT,dwContextID,&BaseEnsureResult,wSendSize);

    CTraceService::TraceString(TEXT("GSP_GP_TakeBaseEnsure"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�ƹ���Ϣ
bool CDataBaseEngineSink::OnRequestQuerySpreadInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //��ȡ����
    DBR_GP_QuerySpreadInfo * pLoadSpreadInfo = (DBR_GP_QuerySpreadInfo*)pData;

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pLoadSpreadInfo->dwUserID);

    //ִ������
    LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_LoadSpreadInfo"),true);

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //����ṹ
      DBO_GP_UserSpreadInfo UserSpreadInfo;
      ZeroMemory(&UserSpreadInfo,sizeof(UserSpreadInfo));

      //��������
      if(m_TreasureDBModule->IsRecordsetEnd()==false)
      {
        UserSpreadInfo.dwSpreadCount = m_TreasureDBAide.GetValue_DWORD(TEXT("SpreadCount"));
        UserSpreadInfo.lSpreadReward= m_TreasureDBAide.GetValue_LONGLONG(TEXT("SpreadReward"));

        //�ƶ���¼
        m_TreasureDBModule->MoveToNext();
      }

      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SPREAD_INFO,dwContextID,&UserSpreadInfo,sizeof(UserSpreadInfo));
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GR_LoadSpreadInfo"),TraceLevel_Exception);
    return false;
  }
}

bool CDataBaseEngineSink::OnRequestLoadRealAuth(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //�������
    m_PlatformDBAide.ResetParameter();

    //ִ������
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadRealAuth"),true);

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //����ṹ
      DBO_GP_RealAuthParameter CmdParameter;
      ZeroMemory(&CmdParameter,sizeof(CmdParameter));
      CmdParameter.dwAuthRealAward= m_PlatformDBAide.GetValue_INT(TEXT("AuthRealAward"));
      CmdParameter.dwAuthentDisable= m_PlatformDBAide.GetValue_INT(TEXT("AuthentDisable"));


      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_REAL_AUTH_PARAMETER,dwContextID,&CmdParameter,sizeof(CmdParameter));
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_LoadRealAuth"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�ȼ�����
bool CDataBaseEngineSink::OnRequestLoadGrowLevelConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //�������
    m_PlatformDBAide.ResetParameter();

    //ִ������
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadGrowLevelConfig"),true);

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //��������
      DBO_GP_GrowLevelConfig GrowLevelConfig;
      ZeroMemory(&GrowLevelConfig,sizeof(GrowLevelConfig));

      //���ñ���
      WORD wLevelCount=0;
      tagGrowLevelConfig * pGrowLevelConfig=NULL;

      //��������
      while(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        //����ж�
        if(GrowLevelConfig.wLevelCount>=CountArray(GrowLevelConfig.GrowLevelConfig))
        {
          break;
        }

        //���ñ���
        pGrowLevelConfig = &GrowLevelConfig.GrowLevelConfig[GrowLevelConfig.wLevelCount++];

        //��ȡ����
        pGrowLevelConfig->wLevelID= m_PlatformDBAide.GetValue_WORD(TEXT("LevelID"));
        pGrowLevelConfig->dwExperience= m_PlatformDBAide.GetValue_DWORD(TEXT("Experience"));

        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }

      //��������
      WORD wSendDataSize = sizeof(GrowLevelConfig)-sizeof(GrowLevelConfig.GrowLevelConfig);
      wSendDataSize += sizeof(tagGrowLevelConfig)*GrowLevelConfig.wLevelCount;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GROWLEVEL_CONFIG,dwContextID,&GrowLevelConfig,wSendDataSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GR_LoadGrowLevelConfig"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��ѯ�ȼ�
bool CDataBaseEngineSink::OnRequestQueryGrowLevelParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_GrowLevelQueryInfo));
    if(wDataSize!=sizeof(DBR_GP_GrowLevelQueryInfo))
    {
      return false;
    }

    //������
    DBR_GP_GrowLevelQueryInfo * pGrowLevelQueryInfo=(DBR_GP_GrowLevelQueryInfo *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pGrowLevelQueryInfo->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pGrowLevelQueryInfo->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pGrowLevelQueryInfo->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pGrowLevelQueryInfo->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strUpgradeDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_QueryGrowLevel"),true);

    //����ṹ
    DBO_GP_GrowLevelParameter GrowLevelParameter;
    ZeroMemory(&GrowLevelParameter,sizeof(GrowLevelParameter));

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //��������
      if(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        //��ȡ����
        GrowLevelParameter.wCurrLevelID = m_PlatformDBAide.GetValue_WORD(TEXT("CurrLevelID"));
        GrowLevelParameter.dwExperience = m_PlatformDBAide.GetValue_DWORD(TEXT("Experience"));
        GrowLevelParameter.dwUpgradeExperience = m_PlatformDBAide.GetValue_DWORD(TEXT("UpgradeExperience"));
        GrowLevelParameter.lUpgradeRewardGold = m_PlatformDBAide.GetValue_LONGLONG(TEXT("RewardGold"));
        GrowLevelParameter.lUpgradeRewardIngot = m_PlatformDBAide.GetValue_LONGLONG(TEXT("RewardMedal"));
      }

      //����ṹ
      DBO_GP_GrowLevelUpgrade GrowLevelUpgrade;
      ZeroMemory(&GrowLevelUpgrade,sizeof(GrowLevelUpgrade));

      //������ʾ
      CDBVarValue DBVarValue;
      m_PlatformDBModule->GetParameter(TEXT("@strUpGradeDescribe"),DBVarValue);
      lstrcpyn(GrowLevelUpgrade.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(GrowLevelUpgrade.szNotifyContent));
      if(GrowLevelUpgrade.szNotifyContent[0]!=0)
      {
        //��ȡ�Ƹ�
        GrowLevelUpgrade.lCurrScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Score"));
        GrowLevelUpgrade.lCurrIngot = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Ingot"));

        //������ʾ
        WORD wSendDataSize = sizeof(GrowLevelUpgrade)-sizeof(GrowLevelUpgrade.szNotifyContent);
        wSendDataSize += CountStringBuffer(GrowLevelUpgrade.szNotifyContent);
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GROWLEVEL_UPGRADE,dwContextID,&GrowLevelUpgrade,wSendDataSize);
      }
    }

    //���Ͳ���
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GROWLEVEL_PARAMETER,dwContextID,&GrowLevelParameter,sizeof(GrowLevelParameter));

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_GrowLevelParameter GrowLevelParameter;
    ZeroMemory(&GrowLevelParameter,sizeof(GrowLevelParameter));

    //���Ͳ���
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GROWLEVEL_PARAMETER,dwContextID,&GrowLevelParameter,sizeof(GrowLevelParameter));

    CTraceService::TraceString(TEXT("GSP_GP_QueryGrowLevel"),TraceLevel_Exception);
    return false;
  }
}

//���ص���
bool CDataBaseEngineSink::OnRequestLoadGameProperty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //��������
    WORD wPacketSize=0;
    BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

    //���ص���
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameProperty"),true);

    DBO_GP_GamePropertyItem  * pGamePropertyItem=NULL;
    while(m_PlatformDBModule->IsRecordsetEnd()==false)
    {
      //������Ϣ
      if((wPacketSize+sizeof(DBO_GP_GamePropertyItem))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
      {
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PROPERTY_ITEM,dwContextID,cbBuffer,wPacketSize);
        wPacketSize=0;
      }

      //��ȡ��Ϣ
      pGamePropertyItem=(DBO_GP_GamePropertyItem *)(cbBuffer+wPacketSize);
      pGamePropertyItem->wIndex=m_PlatformDBAide.GetValue_WORD(TEXT("ID"));
      m_PlatformDBAide.GetValue_String(TEXT("Name"),pGamePropertyItem->szName,CountArray(pGamePropertyItem->szName));
      pGamePropertyItem->wKind=m_PlatformDBAide.GetValue_INT(TEXT("Kind"));
      pGamePropertyItem->dPropertyCash=m_PlatformDBAide.GetValue_DOUBLE(TEXT("Cash"));
      pGamePropertyItem->lPropertyGold=m_PlatformDBAide.GetValue_LONGLONG(TEXT("Gold"));
      pGamePropertyItem->lPropertyUserMedal=m_PlatformDBAide.GetValue_LONGLONG(TEXT("UserMedal"));
      pGamePropertyItem->lPropertyLoveLiness=m_PlatformDBAide.GetValue_LONGLONG(TEXT("LoveLiness"));
      pGamePropertyItem->wUseArea=m_PlatformDBAide.GetValue_WORD(TEXT("UseArea"));
      pGamePropertyItem->wServiceArea=m_PlatformDBAide.GetValue_WORD(TEXT("ServiceArea"));
      pGamePropertyItem->cbSuportMobile=m_PlatformDBAide.GetValue_BYTE(TEXT("SuportMobile"));
      m_PlatformDBAide.GetValue_String(TEXT("RegulationsInfo"),pGamePropertyItem->szRegulationsInfo,CountArray(pGamePropertyItem->szRegulationsInfo));
      pGamePropertyItem->lSendLoveLiness=m_PlatformDBAide.GetValue_LONGLONG(TEXT("SendLoveLiness"));
      pGamePropertyItem->lRecvLoveLiness=m_PlatformDBAide.GetValue_LONGLONG(TEXT("RecvLoveLiness"));
      pGamePropertyItem->lUseResultsGold=m_PlatformDBAide.GetValue_LONGLONG(TEXT("UseResultsGold"));
      pGamePropertyItem->wUseResultsValidTime=m_PlatformDBAide.GetValue_INT(TEXT("UseResultsValidTime"));
      pGamePropertyItem->wUseResultsValidTimeScoreMultiple=m_PlatformDBAide.GetValue_INT(TEXT("UseResultsValidTimeScoreMultiple"));
      pGamePropertyItem->wUseResultsGiftPackage=m_PlatformDBAide.GetValue_INT(TEXT("UseResultsGiftPackage"));
      pGamePropertyItem->wRecommend=m_PlatformDBAide.GetValue_WORD(TEXT("Recommend"));

      //����λ��
      wPacketSize+=sizeof(DBO_GP_GamePropertyItem);

      //�ƶ���¼
      m_PlatformDBModule->MoveToNext();
    }

    if(wPacketSize>0)
    {
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PROPERTY_ITEM,dwContextID,cbBuffer,wPacketSize);
    }

    //��������
    DBO_GP_GamePropertyListResult GamePropertyResult;
    ZeroMemory(&GamePropertyResult,sizeof(GamePropertyResult));

    //���ñ���
    GamePropertyResult.cbSuccess=TRUE;

    //������Ϣ
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PROPERTY_LIST_RESULT,dwContextID,&GamePropertyResult,sizeof(GamePropertyResult));

  }
  catch(IDataBaseException * pIException)
  {
    //�������
    LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
    CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

    //��������
    DBO_GP_GamePropertyListResult GamePropertyResult;
    ZeroMemory(&GamePropertyResult,sizeof(GamePropertyResult));

    //���ñ���
    GamePropertyResult.cbSuccess=FALSE;

    //������Ϣ
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PROPERTY_LIST_RESULT,dwContextID,&GamePropertyResult,sizeof(GamePropertyResult));

    CTraceService::TraceString(TEXT("GSP_GP_LoadGameProperty"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�������
bool CDataBaseEngineSink::OnRequestBuyGameProperty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_PropertyBuy));
    if(wDataSize!=sizeof(DBR_GP_PropertyBuy))
    {
      return false;
    }

    //������
    DBR_GP_PropertyBuy * pPropertyBuy=(DBR_GP_PropertyBuy *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pPropertyBuy->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwSourceUserID"),pPropertyBuy->dwSourceUserID);
    m_PlatformDBAide.AddParameter(TEXT("@wPropertyID"),pPropertyBuy->wPropertyIndex);
    m_PlatformDBAide.AddParameter(TEXT("@wPropertyCount"),pPropertyBuy->wItemCount);
    m_PlatformDBAide.AddParameter(TEXT("@cbConsumeType"),pPropertyBuy->cbConsumeType);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);
    //ִ�в�ѯ
    LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_BuyProperty"),true);

    if(lResultCode == DB_SUCCESS)
    {
      //������
      DBO_GP_PropertyBuyResult PropertyBuyResult;
      ZeroMemory(&PropertyBuyResult,sizeof(DBO_GP_PropertyBuyResult));
      PropertyBuyResult.dwPropID = pPropertyBuy->wPropertyIndex;
      PropertyBuyResult.dwPropCount = pPropertyBuy->wItemCount;
      PropertyBuyResult.lInsureScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Gold"));
      PropertyBuyResult.lUserMedal = m_PlatformDBAide.GetValue_LONGLONG(TEXT("UserMedal"));
      PropertyBuyResult.lLoveLiness = m_PlatformDBAide.GetValue_LONGLONG(TEXT("LoveLiness"));
      PropertyBuyResult.dCash = m_PlatformDBAide.GetValue_DOUBLE(TEXT("Cash"));
      PropertyBuyResult.cbCurrMemberOrder = m_PlatformDBAide.GetValue_BYTE(TEXT("MemberOrder"));
      PropertyBuyResult.cbSuccessed = 1;

      //��ȡ��ʾ
      CDBVarValue DBVarValue;
      m_PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
      lstrcpyn(PropertyBuyResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(PropertyBuyResult.szNotifyContent));

      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PROPERTY_BUY,dwContextID,&PropertyBuyResult,sizeof(DBO_GP_PropertyBuyResult));
    }
    else
    {
      //������
      DBO_GP_PropertyBuyResult PropertyBuyResult;
      ZeroMemory(&PropertyBuyResult,sizeof(DBO_GP_PropertyBuyResult));
      PropertyBuyResult.dwPropID = pPropertyBuy->wPropertyIndex;
      PropertyBuyResult.dwPropCount = pPropertyBuy->wItemCount;

      //��ȡ��ʾ
      CDBVarValue DBVarValue;
      m_PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
      lstrcpyn(PropertyBuyResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(PropertyBuyResult.szNotifyContent));

      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PROPERTY_BUY,dwContextID,&PropertyBuyResult,sizeof(DBO_GP_PropertyBuyResult));
      CTraceService::TraceString(TEXT("GSP_GP_BuyProperty."),TraceLevel_Exception);
      CTraceService::TraceString(PropertyBuyResult.szNotifyContent,TraceLevel_Exception);
      return false;
    }
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_BuyProperty"),TraceLevel_Exception);
    return false;
  }
  return true;
}


bool CDataBaseEngineSink::OnRequestUseProperty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  LONG lResultCode=0;
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_PropertyUse));
    if(wDataSize!=sizeof(DBR_GP_PropertyUse))
    {
      return false;
    }

    //������
    DBR_GP_PropertyUse * pPropertyUse=(DBR_GP_PropertyUse *)pData;

    DBO_GP_PropertyUse DBOPropertyUseResult;
    ZeroMemory(&DBOPropertyUseResult, sizeof(DBOPropertyUseResult));
    DBOPropertyUseResult.dwPropID = pPropertyUse->dwPropID;
    DBOPropertyUseResult.dwUserID = pPropertyUse->dwUserID;
    DBOPropertyUseResult.dwRecvUserID = pPropertyUse->dwRecvUserID;
    DBOPropertyUseResult.wPropCount = pPropertyUse->wPropCount;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pPropertyUse->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pPropertyUse->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@dwRecvUserID"),pPropertyUse->dwRecvUserID);
    m_PlatformDBAide.AddParameter(TEXT("@dwPropID"),pPropertyUse->dwPropID);
    m_PlatformDBAide.AddParameter(TEXT("@dwPropCount"),pPropertyUse->wPropCount);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    //0 �ɹ� 1���߲����� 2ʹ�÷�Χ����
    lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_UseProp"),true);
    DBOPropertyUseResult.dwHandleCode = lResultCode;
    if(lResultCode == 0)
    {
      DBOPropertyUseResult.lRecvLoveLiness = m_PlatformDBAide.GetValue_INT(TEXT("RecvLoveLiness"));
      DBOPropertyUseResult.lSendLoveLiness = m_PlatformDBAide.GetValue_INT(TEXT("SendLoveLiness"));
      DBOPropertyUseResult.Score = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Score"));
      DBOPropertyUseResult.lUseResultsGold = m_PlatformDBAide.GetValue_INT(TEXT("UseResultsGold"));
      DBOPropertyUseResult.UseResultsValidTime = m_PlatformDBAide.GetValue_INT(TEXT("UseResultsValidTime"));
      DBOPropertyUseResult.dwScoreMultiple = m_PlatformDBAide.GetValue_INT(TEXT("UseResultsValidTimeScoreMultiple"));
      DBOPropertyUseResult.dwPropKind = m_PlatformDBAide.GetValue_INT(TEXT("Kind"));
      DBOPropertyUseResult.dwRemainderPropCount = m_PlatformDBAide.GetValue_INT(TEXT("RemainderCount"));
      DBOPropertyUseResult.cbMemberOrder = m_PlatformDBAide.GetValue_BYTE(TEXT("MemberOrder"));
      m_PlatformDBAide.GetValue_String(TEXT("Name"),DBOPropertyUseResult.szName,CountArray(DBOPropertyUseResult.szName));
      SYSTEMTIME st;
      m_PlatformDBAide.GetValue_SystemTime(TEXT("UseTime"), st);
      struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
      DBOPropertyUseResult.tUseTime = mktime(&gm);
    }
    else
    {
      CTraceService::TraceString(TEXT("GSP_GP_UseProp."),TraceLevel_Exception);
    }

    //��ȡ��ʾ
    CDBVarValue DBVarValue;
    m_PlatformDBAide.GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    lstrcpyn(DBOPropertyUseResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(DBOPropertyUseResult.szNotifyContent));

    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PROPERTY_USE,dwContextID,&DBOPropertyUseResult,sizeof(DBOPropertyUseResult));
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_UseProp"),TraceLevel_Exception);
    return false;
  }
  return (lResultCode==0);
}

bool CDataBaseEngineSink::OnRequestUserBackpackProperty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_QueryBackpack));
    if(wDataSize!=sizeof(DBR_GP_QueryBackpack))
    {
      return false;
    }

    //������
    DBR_GP_QueryBackpack * pQueryBackpack = (DBR_GP_QueryBackpack *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pQueryBackpack->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);


    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pQueryBackpack->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@dwKind"),pQueryBackpack->dwKindID);

    const DWORD dwDataHead = sizeof(DBO_GP_QueryBackpack)-sizeof(tagBackpackProperty);

    //���췵��
    BYTE cbDataBuffer[SOCKET_TCP_PACKET]= {0};
    DBO_GP_QueryBackpack* pQueryBackpackResult = (DBO_GP_QueryBackpack*)cbDataBuffer;

    //��ʼ������
    WORD dwDataBufferSize = dwDataHead;
    pQueryBackpackResult->dwUserID = pQueryBackpack->dwUserID;
    pQueryBackpackResult->dwCount = 0;
    pQueryBackpackResult->dwStatus = 0;


    //���ݶ���
    tagPropertyInfo PropertyInfo;
    ZeroMemory(&PropertyInfo,sizeof(PropertyInfo));
    int nPropertyCount = 0;

    //ִ�нű�
    LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserBackpack"),true);
    if(lResultCode == DB_SUCCESS)
    {
      int nCount = 0;
      while(true)
      {

        //�����ж�
        if(m_PlatformDBModule->IsRecordsetEnd()==true)
        {
          break;
        }

        //��ȡ����
        nPropertyCount = m_PlatformDBAide.GetValue_INT(TEXT("GoodsCount"));
        PropertyInfo.wIndex = m_PlatformDBAide.GetValue_INT(TEXT("GoodsID"));
        PropertyInfo.wKind = m_PlatformDBAide.GetValue_WORD(TEXT("Kind"));
        PropertyInfo.wUseArea = m_PlatformDBAide.GetValue_WORD(TEXT("UseArea"));
        PropertyInfo.wRecommend = m_PlatformDBAide.GetValue_INT(TEXT("Recommend"));
        PropertyInfo.lPropertyGold = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Gold"));
        PropertyInfo.dPropertyCash = m_PlatformDBAide.GetValue_DOUBLE(TEXT("Cash"));
        PropertyInfo.lPropertyUserMedal = m_PlatformDBAide.GetValue_LONGLONG(TEXT("UserMedal"));
        PropertyInfo.lPropertyLoveLiness = m_PlatformDBAide.GetValue_LONGLONG(TEXT("LoveLiness"));
        PropertyInfo.lSendLoveLiness = m_PlatformDBAide.GetValue_LONGLONG(TEXT("SendLoveLiness"));
        PropertyInfo.lRecvLoveLiness = m_PlatformDBAide.GetValue_LONGLONG(TEXT("RecvLoveLiness"));
        PropertyInfo.lUseResultsGold = m_PlatformDBAide.GetValue_LONGLONG(TEXT("UseResultsGold"));
        m_PlatformDBAide.GetValue_String(TEXT("Name"),PropertyInfo.szName,CountArray(PropertyInfo.szName));
        m_PlatformDBAide.GetValue_String(TEXT("RegulationsInfo"),PropertyInfo.szRegulationsInfo,CountArray(PropertyInfo.szRegulationsInfo));

        //��������
        memcpy(&pQueryBackpackResult->PropertyInfo[nCount++].Property, &PropertyInfo, sizeof(PropertyInfo));
        pQueryBackpackResult->PropertyInfo[nCount-1].nCount = nPropertyCount;
        dwDataBufferSize += sizeof(PropertyInfo);

        //�ж����ݰ���С
        if(dwDataBufferSize >= SOCKET_TCP_PACKET-sizeof(PropertyInfo))
        {
          //������ʾ
          TCHAR szDescribe[128]=TEXT("");
          _sntprintf(szDescribe,CountArray(szDescribe),TEXT("���͵�������:%d"),pQueryBackpackResult->dwCount);
          CTraceService::TraceString(szDescribe,TraceLevel_Exception);

          pQueryBackpackResult->dwStatus = 0;
          m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_BACKPACK,dwContextID,pQueryBackpackResult,dwDataBufferSize);
          nCount = 0;
          dwDataBufferSize = dwDataHead;
        }



        //������ʱ����
        ZeroMemory(&PropertyInfo,sizeof(PropertyInfo));
        nPropertyCount = 0;

        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }

      //������Ϣ
      pQueryBackpackResult->dwCount = nCount;
      pQueryBackpackResult->dwStatus = 1;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_BACKPACK,dwContextID,pQueryBackpackResult,dwDataBufferSize);
    }

  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_QueryUserBackpack"),TraceLevel_Exception);
    return false;
  }
  return true;
}

bool CDataBaseEngineSink::OnRequestPropertyBuff(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    ASSERT(wDataSize == sizeof(DBR_GP_UserPropertyBuff));
    DBR_GP_UserPropertyBuff* PropertyBuffRequest = (DBR_GP_UserPropertyBuff*)pData;

    //��������
    DBO_GR_UserPropertyBuff GamePropertyBuff;
    ZeroMemory(&GamePropertyBuff,sizeof(GamePropertyBuff));
    GamePropertyBuff.dwUserID = PropertyBuffRequest->dwUserID;

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"), PropertyBuffRequest->dwUserID);

    //ִ�в�ѯ
    LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadUserGameBuff"),true);

    //��ȡ��Ϣ
    for(WORD i=0; i<CountArray(GamePropertyBuff.PropertyBuff); i++)
    {
      //�����ж�
      if(m_PlatformDBModule->IsRecordsetEnd()==true)
      {
        break;
      }

      //���Ч��
      ASSERT(GamePropertyBuff.cbBuffCount<CountArray(GamePropertyBuff.PropertyBuff));
      if(GamePropertyBuff.cbBuffCount>=CountArray(GamePropertyBuff.PropertyBuff))
      {
        break;
      }

      //��ȡ����
      GamePropertyBuff.cbBuffCount++;
      //GamePropertyBuff.PropertyBuff[i].dwPropID=m_PlatformDBAide.GetValue_WORD(TEXT("PropID"));
      GamePropertyBuff.PropertyBuff[i].dwKind=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
      GamePropertyBuff.PropertyBuff[i].dwScoreMultiple=m_PlatformDBAide.GetValue_WORD(TEXT("UseResultsValidTimeScoreMultiple"));
      GamePropertyBuff.PropertyBuff[i].UseResultsValidTime=m_PlatformDBAide.GetValue_DWORD(TEXT("UseResultsValidTime"));
      m_PlatformDBAide.GetValue_String(TEXT("Name"),GamePropertyBuff.PropertyBuff[i].szName,CountArray(GamePropertyBuff.PropertyBuff[i].szName));
      SYSTEMTIME st;
      m_PlatformDBAide.GetValue_SystemTime(TEXT("UseTime"), st);
      struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
      GamePropertyBuff.PropertyBuff[i].tUseTime = mktime(&gm);

      //�ƶ���¼
      m_PlatformDBModule->MoveToNext();
    }
    //������Ϣ
    WORD wHeadSize=sizeof(GamePropertyBuff)-sizeof(GamePropertyBuff.PropertyBuff);
    WORD wDataSize=GamePropertyBuff.cbBuffCount*sizeof(GamePropertyBuff.PropertyBuff[0]);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_PROPERTY_BUFF,dwContextID,&GamePropertyBuff,wHeadSize+wDataSize);
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //��������
    DBO_GR_UserPropertyBuff GamePropertyBuff;
    ZeroMemory(&GamePropertyBuff,sizeof(GamePropertyBuff));

    //�������
    GamePropertyBuff.cbBuffCount=0L;

    //������Ϣ
    WORD wHeadSize=sizeof(GamePropertyBuff)-sizeof(GamePropertyBuff.PropertyBuff);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_PROPERTY_BUFF,dwContextID,&GamePropertyBuff,wHeadSize);

    CTraceService::TraceString(TEXT("GSP_GP_LoadUserGameBuff"),TraceLevel_Exception);
    return false;
  }

  return true;
}

bool CDataBaseEngineSink::OnRequestPropertyPresent(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    ASSERT(wDataSize == sizeof(DBR_GP_PropertyPresent));
    DBR_GP_PropertyPresent* PropertyPresentRequest = (DBR_GP_PropertyPresent*)pData;

    //��������
    DBO_GP_PropertyPresent PropertyPresent= {0};
    PropertyPresent.dwUserID = PropertyPresentRequest->dwUserID;
    PropertyPresent.dwRecvGameID = PropertyPresentRequest->dwRecvGameID;
    PropertyPresent.dwPropID = PropertyPresentRequest->dwPropID;
    PropertyPresent.wPropCount = PropertyPresentRequest->wPropCount;
    PropertyPresent.wType = PropertyPresentRequest->wType; //0�ǳ�  1 GameID
    lstrcpyn(PropertyPresent.szRecvNickName,PropertyPresentRequest->szRecvNickName,CountArray(PropertyPresent.szRecvNickName));

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&PropertyPresentRequest->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);


    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"), PropertyPresentRequest->dwUserID);
    if(PropertyPresentRequest->wType == 0)     //�����ǳ�����
    {
      m_PlatformDBAide.AddParameter(TEXT("@strReceiverNickName"), PropertyPresentRequest->szRecvNickName);
    }
    else if(PropertyPresentRequest->wType == 1)    //���� GameID
    {
      m_PlatformDBAide.AddParameter(TEXT("@dwReceiverGameID"), PropertyPresentRequest->dwRecvGameID);
    }
    m_PlatformDBAide.AddParameter(TEXT("@dwPropID"), PropertyPresentRequest->dwPropID);
    m_PlatformDBAide.AddParameter(TEXT("@dwPropCount"), PropertyPresentRequest->wPropCount);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

    //�������
    TCHAR szDescribeString[64]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    LONG lResultCode = -1;
    if(PropertyPresentRequest->wType == 0)
    {
      lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_UserSendPresentByNickName"),true);
    }
    else if(PropertyPresentRequest->wType == 1)
    {
      lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_UserSendPresentByID"),true);
    }

    //������
    PropertyPresent.nHandleCode = lResultCode;

    //��ȡ����
    CDBVarValue DBVarValue;
    m_PlatformDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
    lstrcpyn(PropertyPresent.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(PropertyPresent.szNotifyContent));

    //������Ϣ
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PROPERTY_PRESENT,dwContextID,&PropertyPresent,sizeof(PropertyPresent));
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    DBR_GP_PropertyPresent* PropertyPresentRequest = (DBR_GP_PropertyPresent*)pData;
    //��������
    DBO_GP_PropertyPresent PropertyPresent= {0};
    PropertyPresent.dwUserID = PropertyPresentRequest->dwUserID;
    PropertyPresent.dwRecvGameID = PropertyPresentRequest->dwRecvGameID;
    PropertyPresent.dwPropID = PropertyPresentRequest->dwPropID;
    PropertyPresent.wPropCount = PropertyPresentRequest->wPropCount;
    PropertyPresent.wType = PropertyPresentRequest->wType; //0�ǳ�  1 GameID
    lstrcpyn(PropertyPresent.szRecvNickName,PropertyPresentRequest->szRecvNickName,CountArray(PropertyPresent.szRecvNickName));
    //�������
    PropertyPresent.nHandleCode = -1;
    lstrcpyn(PropertyPresent.szNotifyContent, TEXT("���͵���ʱ ���ݿ�����쳣"), CountArray(PropertyPresent.szNotifyContent));

    //������Ϣ
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PROPERTY_PRESENT,dwContextID,&PropertyPresent,sizeof(PropertyPresent));
    CTraceService::TraceString(TEXT("GSP_GP_UserSendPresentByNickName or GSP_GP_UserSendPresentByID"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQuerySendPresent(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    ASSERT(wDataSize == sizeof(DBR_GP_QuerySendPresent));
    DBR_GP_QuerySendPresent* pQuerySendPresent = (DBR_GP_QuerySendPresent*)pData;

    //��������
    DBO_GP_QuerySendPresent QuerySendPresenResult;
    ZeroMemory(&QuerySendPresenResult,sizeof(QuerySendPresenResult));

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pQuerySendPresent->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"), pQuerySendPresent->dwUserID);

    //ִ�в�ѯ
    LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_QuerySendPresent"),true);
    WORD wHeadSize=sizeof(QuerySendPresenResult)-sizeof(QuerySendPresenResult.Present);

    if(lResultCode == 0)
    {
      //��ȡ��Ϣ
      for(WORD i=0;; i++)
      {
        //�����ж�
        if(m_PlatformDBModule->IsRecordsetEnd()==true)
        {
          break;
        }

        //����СЧ��
        ASSERT(QuerySendPresenResult.wPresentCount<CountArray(QuerySendPresenResult.Present));
        if(QuerySendPresenResult.wPresentCount>=CountArray(QuerySendPresenResult.Present))
        {
          WORD wDataSize=QuerySendPresenResult.wPresentCount*sizeof(QuerySendPresenResult.Present[0]);
          m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_SEND_PRESENT,dwContextID,&QuerySendPresenResult,wHeadSize+wDataSize);
          i=0;
          QuerySendPresenResult.wPresentCount = 0;
        }

        //��ȡ����
        QuerySendPresenResult.wPresentCount++;
        QuerySendPresenResult.Present[i].dwUserID=m_PlatformDBAide.GetValue_WORD(TEXT("UserID"));
        QuerySendPresenResult.Present[i].dwRecvUserID=m_PlatformDBAide.GetValue_WORD(TEXT("ReceiverUserID"));
        QuerySendPresenResult.Present[i].dwPropID=m_PlatformDBAide.GetValue_WORD(TEXT("PropID"));
        QuerySendPresenResult.Present[i].wPropCount=m_PlatformDBAide.GetValue_WORD(TEXT("PropCount"));
        m_PlatformDBAide.GetValue_String(TEXT("Name"),QuerySendPresenResult.Present[i].szPropName,CountArray(QuerySendPresenResult.Present[i].szPropName));
        SYSTEMTIME st;
        m_PlatformDBAide.GetValue_SystemTime(TEXT("SendTime"), st);
        struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
        QuerySendPresenResult.Present[i].tSendTime = mktime(&gm);

        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }
    }
    //������Ϣ
    wDataSize=QuerySendPresenResult.wPresentCount*sizeof(QuerySendPresenResult.Present[0]);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_SEND_PRESENT,dwContextID,&QuerySendPresenResult,wHeadSize+wDataSize);
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //��������
    //��������
    DBO_GP_QuerySendPresent QuerySendPresenResult;
    ZeroMemory(&QuerySendPresenResult,sizeof(QuerySendPresenResult));

    //�������
    QuerySendPresenResult.wPresentCount=0;

    //������Ϣ
    WORD wHeadSize=sizeof(QuerySendPresenResult)-sizeof(QuerySendPresenResult.Present);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_SEND_PRESENT,dwContextID,&QuerySendPresenResult,wHeadSize);

    CTraceService::TraceString(TEXT("GSP_GP_QuerySendPresent"),TraceLevel_Exception);
    return false;
  }

  return true;
}


bool CDataBaseEngineSink::OnRequestGetSendPresent(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    ASSERT(wDataSize == sizeof(DBR_GP_GetSendPresent));
    DBR_GP_GetSendPresent* pGetSendPresent = (DBR_GP_GetSendPresent*)pData;

    //��������
    DBO_GP_GetSendPresent GetSendPresenResult;
    ZeroMemory(&GetSendPresenResult,sizeof(GetSendPresenResult));

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pGetSendPresent->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"), pGetSendPresent->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@szPassword"), pGetSendPresent->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"), szClientAddr);

    //ִ�в�ѯ
    LONG lResultCode=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_GetSendPresent"),true);
    WORD wHeadSize=sizeof(GetSendPresenResult)-sizeof(GetSendPresenResult.Present);

    if(lResultCode == 0)
    {
      //��ȡ��Ϣ
      for(WORD i=0;; i++)
      {
        //�����ж�
        if(m_PlatformDBModule->IsRecordsetEnd()==true)
        {
          break;
        }

        //����СЧ��
        ASSERT(GetSendPresenResult.wPresentCount<CountArray(GetSendPresenResult.Present));
        if(GetSendPresenResult.wPresentCount>=CountArray(GetSendPresenResult.Present))
        {
          WORD wDataSize=GetSendPresenResult.wPresentCount*sizeof(GetSendPresenResult.Present[0]);
          m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_SEND_PRESENT,dwContextID,&GetSendPresenResult,wHeadSize+wDataSize);
          i=0;
          GetSendPresenResult.wPresentCount = 0;
        }

        //��ȡ����
        GetSendPresenResult.wPresentCount++;
        GetSendPresenResult.Present[i].dwUserID=m_PlatformDBAide.GetValue_WORD(TEXT("UserID"));
        GetSendPresenResult.Present[i].dwRecvUserID=m_PlatformDBAide.GetValue_WORD(TEXT("ReceiverUserID"));
        GetSendPresenResult.Present[i].dwPropID=m_PlatformDBAide.GetValue_WORD(TEXT("PropID"));
        GetSendPresenResult.Present[i].wPropCount=m_PlatformDBAide.GetValue_WORD(TEXT("PropCount"));
        m_PlatformDBAide.GetValue_String(TEXT("Name"),GetSendPresenResult.Present[i].szPropName,CountArray(GetSendPresenResult.Present[i].szPropName));
        SYSTEMTIME st;
        m_PlatformDBAide.GetValue_SystemTime(TEXT("SendTime"), st);
        struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
        GetSendPresenResult.Present[i].tSendTime = mktime(&gm);

        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }
    }
    //������Ϣ
    wDataSize=GetSendPresenResult.wPresentCount*sizeof(GetSendPresenResult.Present[0]);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_SEND_PRESENT,dwContextID,&GetSendPresenResult,wHeadSize+wDataSize);
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //��������
    //��������
    DBO_GP_GetSendPresent GetSendPresenResult;
    ZeroMemory(&GetSendPresenResult,sizeof(GetSendPresenResult));

    //�������
    GetSendPresenResult.wPresentCount=0;

    //������Ϣ
    WORD wHeadSize=sizeof(GetSendPresenResult)-sizeof(GetSendPresenResult.Present);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_SEND_PRESENT,dwContextID,&GetSendPresenResult,wHeadSize);

    CTraceService::TraceString(TEXT("GSP_GP_GetSendPresent"),TraceLevel_Exception);
    return false;
  }
  return true;
}
//��Ա����
bool CDataBaseEngineSink::OnRequestMemberLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //�������
    m_PlatformDBAide.ResetParameter();

    //ִ������
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadMemberParameter"),true);

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //��������
      DBO_GP_MemberParameter MemberParameter;
      ZeroMemory(&MemberParameter,sizeof(MemberParameter));

      //���ñ���
      WORD wMemberCount=0;
      tagMemberParameterNew * pMemberParameter=NULL;

      //��������
      while(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        //����ж�
        if(MemberParameter.wMemberCount>=CountArray(MemberParameter.MemberParameter))
        {
          break;
        }

        //���ñ���
        pMemberParameter = &MemberParameter.MemberParameter[MemberParameter.wMemberCount++];

        //��ȡ����
        pMemberParameter->cbMemberOrder= m_PlatformDBAide.GetValue_BYTE(TEXT("MemberOrder"));
        m_PlatformDBAide.GetValue_String(TEXT("MemberName"),pMemberParameter->szMemberName,CountArray(pMemberParameter->szMemberName));
        pMemberParameter->dwMemberRight= m_PlatformDBAide.GetValue_DWORD(TEXT("UserRight"));
        pMemberParameter->dwMemberTask= m_PlatformDBAide.GetValue_DWORD(TEXT("TaskRate"));
        pMemberParameter->dwMemberShop= m_PlatformDBAide.GetValue_DWORD(TEXT("ShopRate"));
        pMemberParameter->dwMemberInsure= m_PlatformDBAide.GetValue_DWORD(TEXT("InsureRate"));
        pMemberParameter->dwMemberDayPresent= m_PlatformDBAide.GetValue_DWORD(TEXT("DayPresent"));
        pMemberParameter->dwMemberDayGiftID= m_PlatformDBAide.GetValue_DWORD(TEXT("DayGiftID"));
        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }

      //��������
      WORD wSendDataSize = sizeof(MemberParameter)-sizeof(MemberParameter.MemberParameter);
      wSendDataSize += sizeof(tagMemberParameterNew)*MemberParameter.wMemberCount;
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MEMBER_PARAMETER,dwContextID,&MemberParameter,wSendDataSize);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_LoadMemberParameter"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��Ա��ѯ
bool CDataBaseEngineSink::OnRequestMemberQueryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_MemberQueryInfo));
    if(wDataSize!=sizeof(DBR_GP_MemberQueryInfo))
    {
      return false;
    }

    //��ȡ����
    DBR_GP_MemberQueryInfo * pDBRMemberInfo=(DBR_GP_MemberQueryInfo *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pDBRMemberInfo->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pDBRMemberInfo->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pDBRMemberInfo->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pDBRMemberInfo->szMachineID);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_MemberQueryInfo"),true);

    //��������
    DBO_GP_MemberQueryInfoResult pDBOMemberInfoResult;
    ZeroMemory(&pDBOMemberInfoResult,sizeof(pDBOMemberInfoResult));

    //��ȡ����
    if(lResultCode==DB_SUCCESS && m_PlatformDBModule->IsRecordsetEnd()==false)
    {
      pDBOMemberInfoResult.bPresent = (m_PlatformDBAide.GetValue_BYTE(TEXT("Present"))==TRUE);
      pDBOMemberInfoResult.bGift = (m_PlatformDBAide.GetValue_BYTE(TEXT("Gift"))==TRUE);
    }

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pDBRMemberInfo->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pDBRMemberInfo->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pDBRMemberInfo->szMachineID);

    //ִ������
    lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_MemberGiftQuery"),true);

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //���ñ���
      WORD wMemberCount=0;
      tagGiftPropertyInfo * pMemberParameter=NULL;

      //��������
      while(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        //����ж�
        if(pDBOMemberInfoResult.GiftSubCount>=CountArray(pDBOMemberInfoResult.GiftSub))
        {
          break;
        }

        //���ñ���
        pMemberParameter = &pDBOMemberInfoResult.GiftSub[pDBOMemberInfoResult.GiftSubCount++];
        //��ȡ����
        pMemberParameter->wPropertyID= m_PlatformDBAide.GetValue_WORD(TEXT("ID"));
        pMemberParameter->wPropertyCount= m_PlatformDBAide.GetValue_WORD(TEXT("Count"));
        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }
    }

    //�����С
    WORD wSendDataSize = sizeof(pDBOMemberInfoResult)-sizeof(pDBOMemberInfoResult.GiftSub);
    wSendDataSize += sizeof(tagGiftPropertyInfo)*(WORD)(pDBOMemberInfoResult.GiftSubCount);

    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MEMBER_QUERY_INFO_RESULT,dwContextID,&pDBOMemberInfoResult,wSendDataSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_MemberGiftQuery"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��Ա����
bool CDataBaseEngineSink::OnRequestMemberDayPresent(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_MemberDayPresent));
    if(wDataSize!=sizeof(DBR_GP_MemberDayPresent))
    {
      return false;
    }

    //��ȡ����
    DBR_GP_MemberDayPresent * pDBRMemberInfo=(DBR_GP_MemberDayPresent *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pDBRMemberInfo->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pDBRMemberInfo->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pDBRMemberInfo->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pDBRMemberInfo->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_MemberDayPresent"),true);

    //��������
    DBO_GP_MemberDayPresentResult pDBOMemberInfoResult;
    ZeroMemory(&pDBOMemberInfoResult,sizeof(pDBOMemberInfoResult));

    //��ȡ����
    pDBOMemberInfoResult.bSuccessed= (lResultCode ==DB_SUCCESS);

    //��ȡ����
    if(lResultCode==DB_SUCCESS)
    {
      pDBOMemberInfoResult.lGameScore = m_PlatformDBAide.GetValue_LONGLONG(TEXT("Score"));
    }

    //��ȡ����
    CDBVarValue DBVarValue;
    m_PlatformDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);
    lstrcpyn(pDBOMemberInfoResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(pDBOMemberInfoResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(pDBOMemberInfoResult)-sizeof(pDBOMemberInfoResult.szNotifyContent);
    wSendSize+=CountStringBuffer(pDBOMemberInfoResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MEMBER_DAY_PRESENT_RESULT,dwContextID,&pDBOMemberInfoResult,wSendSize);
    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_MemberDayPresent"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��Ա���
bool CDataBaseEngineSink::OnRequestMemberDayGift(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_MemberDayGift));
    if(wDataSize!=sizeof(DBR_GP_MemberDayGift))
    {
      return false;
    }

    //��ȡ����
    DBR_GP_MemberDayGift * pDBRMemberInfo=(DBR_GP_MemberDayGift *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pDBRMemberInfo->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@dwUserID"),pDBRMemberInfo->dwUserID);
    m_PlatformDBAide.AddParameter(TEXT("@strPassword"),pDBRMemberInfo->szPassword);
    m_PlatformDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),pDBRMemberInfo->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_PlatformDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_MemberDayGift"),true);

    //��������
    DBO_GP_MemberDayGiftResult pDBOMemberInfoResult;
    ZeroMemory(&pDBOMemberInfoResult,sizeof(pDBOMemberInfoResult));

    //��ȡ����
    pDBOMemberInfoResult.bSuccessed= (lResultCode ==DB_SUCCESS);
    //��ȡ����
    CDBVarValue DBVarValue;
    m_PlatformDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);
    lstrcpyn(pDBOMemberInfoResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(pDBOMemberInfoResult.szNotifyContent));

    //���ͽ��
    WORD wSendSize=sizeof(pDBOMemberInfoResult)-sizeof(pDBOMemberInfoResult.szNotifyContent);
    wSendSize+=CountStringBuffer(pDBOMemberInfoResult.szNotifyContent);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MEMBER_DAY_GIFT_RESULT,dwContextID,&pDBOMemberInfoResult,wSendSize);
    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_MemberDayGift"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//�����Ա
bool CDataBaseEngineSink::OnRequestPurchaseMember(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_PurchaseMember));
    if(wDataSize!=sizeof(DBR_GP_PurchaseMember))
    {
      return false;
    }

    //������
    DBR_GP_PurchaseMember * pPurchaseMember=(DBR_GP_PurchaseMember *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pPurchaseMember->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pPurchaseMember->dwUserID);
    m_TreasureDBAide.AddParameter(TEXT("@cbMemberOrder"),pPurchaseMember->cbMemberOrder);
    m_TreasureDBAide.AddParameter(TEXT("@PurchaseTime"),pPurchaseMember->wPurchaseTime);
    m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pPurchaseMember->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_PurchaseMember"),true);

    //����ṹ
    DBO_GP_PurchaseResult PurchaseResult;
    ZeroMemory(&PurchaseResult,sizeof(PurchaseResult));

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //���ñ���
      PurchaseResult.bSuccessed=true;

      //��������
      if(m_TreasureDBModule->IsRecordsetEnd()==false)
      {
        //��ȡ����
        PurchaseResult.lCurrScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrScore"));
        PurchaseResult.dCurrBeans = m_TreasureDBAide.GetValue_DOUBLE(TEXT("CurrBeans"));
        PurchaseResult.cbMemberOrder = m_TreasureDBAide.GetValue_BYTE(TEXT("MemberOrder"));
      }
    }

    //��ȡ��ʾ
    CDBVarValue DBVarValue;
    m_TreasureDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);
    lstrcpyn(PurchaseResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(PurchaseResult.szNotifyContent));

    //�����С
    WORD wSendDataSize = sizeof(PurchaseResult)-sizeof(PurchaseResult.szNotifyContent);
    wSendDataSize += CountStringBuffer(PurchaseResult.szNotifyContent);

    //���Ͳ���
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PURCHASE_RESULT,dwContextID,&PurchaseResult,wSendDataSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_PurchaseResult PurchaseResult;
    ZeroMemory(&PurchaseResult,sizeof(PurchaseResult));

    //���ñ���
    lstrcpyn(PurchaseResult.szNotifyContent,TEXT("���ݿ��쳣�����Ժ����ԣ�"),CountArray(PurchaseResult.szNotifyContent));

    //�����С
    WORD wSendDataSize = sizeof(PurchaseResult)-sizeof(PurchaseResult.szNotifyContent);
    wSendDataSize += CountStringBuffer(PurchaseResult.szNotifyContent);

    //���Ͳ���
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PURCHASE_RESULT,dwContextID,&PurchaseResult,wSendDataSize);

    CTraceService::TraceString(TEXT("GSP_GR_PurchaseMember"),TraceLevel_Exception);
    return false;
  }
}

//�һ���Ϸ��
bool CDataBaseEngineSink::OnRequestExchangeScoreByIngot(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ExchangeScoreByIngot));
    if(wDataSize!=sizeof(DBR_GP_ExchangeScoreByIngot))
    {
      return false;
    }

    //������
    DBR_GP_ExchangeScoreByIngot * pExchangeScore=(DBR_GP_ExchangeScoreByIngot *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pExchangeScore->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pExchangeScore->dwUserID);
    m_TreasureDBAide.AddParameter(TEXT("@ExchangeIngot"),pExchangeScore->lExchangeIngot);
    m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pExchangeScore->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_ExchangeScoreByIngot"),true);

    //����ṹ
    DBO_GP_ExchangeResult ExchangeResult;
    ZeroMemory(&ExchangeResult,sizeof(ExchangeResult));

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //���ñ���
      ExchangeResult.bSuccessed=true;

      //��������
      if(m_TreasureDBModule->IsRecordsetEnd()==false)
      {
        //��ȡ����
        ExchangeResult.lCurrScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrScore"));
        ExchangeResult.lCurrIngot = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrIngot"));
        ExchangeResult.dCurrBeans = m_TreasureDBAide.GetValue_DOUBLE(TEXT("CurrBeans"));
      }
    }

    //��ʾ����
    CDBVarValue DBVarValue;
    m_TreasureDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);
    lstrcpyn(ExchangeResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(ExchangeResult.szNotifyContent));

    //�����С
    WORD wSendDataSize = sizeof(ExchangeResult)-sizeof(ExchangeResult.szNotifyContent);
    wSendDataSize += CountStringBuffer(ExchangeResult.szNotifyContent);

    //���Ͳ���
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_RESULT,dwContextID,&ExchangeResult,wSendDataSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_ExchangeResult ExchangeResult;
    ZeroMemory(&ExchangeResult,sizeof(ExchangeResult));

    //���ñ���
    lstrcpyn(ExchangeResult.szNotifyContent,TEXT("���ݿ��쳣�����Ժ����ԣ�"),CountArray(ExchangeResult.szNotifyContent));

    //�����С
    WORD wSendDataSize = sizeof(ExchangeResult)-sizeof(ExchangeResult.szNotifyContent);
    wSendDataSize += CountStringBuffer(ExchangeResult.szNotifyContent);

    //���Ͳ���
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_RESULT,dwContextID,&ExchangeResult,wSendDataSize);

    CTraceService::TraceString(TEXT("GSP_GR_ExchangeScoreByIngot"),TraceLevel_Exception);
    return false;
  }
}


//�һ���Ϸ��
bool CDataBaseEngineSink::OnRequestExchangeScoreByBeans(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //Ч�����
    ASSERT(wDataSize==sizeof(DBR_GP_ExchangeScoreByBeans));
    if(wDataSize!=sizeof(DBR_GP_ExchangeScoreByBeans))
    {
      return false;
    }

    //������
    DBR_GP_ExchangeScoreByBeans * pExchangeScore=(DBR_GP_ExchangeScoreByBeans *)pData;

    //ת����ַ
    TCHAR szClientAddr[16]=TEXT("");
    BYTE * pClientAddr=(BYTE *)&pExchangeScore->dwClientAddr;
    _sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

    //�������
    m_TreasureDBAide.ResetParameter();
    m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pExchangeScore->dwUserID);
    m_TreasureDBAide.AddParameter(TEXT("@ExchangeBeans"),pExchangeScore->dExchangeBeans);
    m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
    m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pExchangeScore->szMachineID);

    //�������
    TCHAR szDescribeString[128]=TEXT("");
    m_TreasureDBAide.AddParameterOutput(TEXT("@strNotifyContent"),szDescribeString,sizeof(szDescribeString),adParamOutput);

    //ִ�нű�
    LONG lResultCode = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_ExchangeScoreByBeans"),true);

    //����ṹ
    DBO_GP_ExchangeResult ExchangeResult;
    ZeroMemory(&ExchangeResult,sizeof(ExchangeResult));

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //���ñ���
      ExchangeResult.bSuccessed=true;

      //��������
      if(m_TreasureDBModule->IsRecordsetEnd()==false)
      {
        //��ȡ����
        ExchangeResult.lCurrScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrScore"));
        ExchangeResult.lCurrIngot = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrIngot"));
        ExchangeResult.dCurrBeans = m_TreasureDBAide.GetValue_DOUBLE(TEXT("CurrBeans"));
      }
    }

    //��ʾ����
    CDBVarValue DBVarValue;
    m_TreasureDBModule->GetParameter(TEXT("@strNotifyContent"),DBVarValue);
    lstrcpyn(ExchangeResult.szNotifyContent,CW2CT(DBVarValue.bstrVal),CountArray(ExchangeResult.szNotifyContent));

    //�����С
    WORD wSendDataSize = sizeof(ExchangeResult)-sizeof(ExchangeResult.szNotifyContent);
    wSendDataSize += CountStringBuffer(ExchangeResult.szNotifyContent);

    //���Ͳ���
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_RESULT,dwContextID,&ExchangeResult,wSendDataSize);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    //����ṹ
    DBO_GP_ExchangeResult ExchangeResult;
    ZeroMemory(&ExchangeResult,sizeof(ExchangeResult));

    //���ñ���
    lstrcpyn(ExchangeResult.szNotifyContent,TEXT("���ݿ��쳣�����Ժ����ԣ�"),CountArray(ExchangeResult.szNotifyContent));

    //�����С
    WORD wSendDataSize = sizeof(ExchangeResult)-sizeof(ExchangeResult.szNotifyContent);
    wSendDataSize += CountStringBuffer(ExchangeResult.szNotifyContent);

    //���Ͳ���
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_RESULT,dwContextID,&ExchangeResult,wSendDataSize);

    CTraceService::TraceString(TEXT("GSP_GR_ExchangeScoreByBeans"),TraceLevel_Exception);
    return false;
  }
}

//�����б�
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //��������
    WORD wPacketSize=0;
    BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

    //��������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

    //��������
    wPacketSize=0;
    DBO_GP_GameType * pGameType=NULL;
    while(m_PlatformDBModule->IsRecordsetEnd()==false)
    {
      //������Ϣ
      if((wPacketSize+sizeof(DBO_GP_GameType))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
      {
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
        wPacketSize=0;
      }

      //��ȡ��Ϣ
      pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
      pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
      pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
      pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
      m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));

      //����λ��
      wPacketSize+=sizeof(DBO_GP_GameType);

      //�ƶ���¼
      m_PlatformDBModule->MoveToNext();
    }
    if(wPacketSize>0)
    {
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
    }

    //��ȡ����
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

    //��������
    wPacketSize=0;
    DBO_GP_GameKind * pGameKind=NULL;
    while(m_PlatformDBModule->IsRecordsetEnd()==false)
    {
      //������Ϣ
      if((wPacketSize+sizeof(DBO_GP_GameKind))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
      {
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
        wPacketSize=0;
      }

      //��ȡ��Ϣ
      pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
      pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
      pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
      pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
      pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
      pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
      m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
      m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));
      pGameKind->wRecommend=m_PlatformDBAide.GetValue_WORD(TEXT("Recommend"));
      pGameKind->wGameFlag =m_PlatformDBAide.GetValue_WORD(TEXT("GameFlag"));

      //����λ��
      wPacketSize+=sizeof(DBO_GP_GameKind);

      //�ƶ���¼
      m_PlatformDBModule->MoveToNext();
    }
    if(wPacketSize>0)
    {
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
    }

    //��ȡ�ڵ�
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

    //���ͽڵ�
    wPacketSize=0;
    DBO_GP_GameNode * pGameNode=NULL;
    while(m_PlatformDBModule->IsRecordsetEnd()==false)
    {
      //������Ϣ
      if((wPacketSize+sizeof(DBO_GP_GameNode))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
      {
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
        wPacketSize=0;
      }

      //��ȡ��Ϣ
      pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
      pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
      pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
      pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
      pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
      m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));

      //����λ��
      wPacketSize+=sizeof(DBO_GP_GameNode);

      //�ƶ���¼
      m_PlatformDBModule->MoveToNext();
    }
    if(wPacketSize>0)
    {
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
    }

    //��ȡ����
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

    //���Ͷ���
    wPacketSize=0;
    DBO_GP_GamePage * pGamePage=NULL;
    while(m_PlatformDBModule->IsRecordsetEnd()==false)
    {
      //������Ϣ
      if((wPacketSize+sizeof(DBO_GP_GamePage))>(sizeof(cbBuffer)-sizeof(NTY_DataBaseEvent)))
      {
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
        wPacketSize=0;
      }

      //��ȡ��Ϣ
      pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
      pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
      pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
      pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
      pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
      pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
      m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));

      //����λ��
      wPacketSize+=sizeof(DBO_GP_GamePage);

      //�ƶ���¼
      m_PlatformDBModule->MoveToNext();
    }
    if(wPacketSize>0)
    {
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
    }

    //��������
    DBO_GP_GameListResult GameListResult;
    ZeroMemory(&GameListResult,sizeof(GameListResult));

    //���ñ���
    GameListResult.cbSuccess=TRUE;

    //������Ϣ
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
    CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

    //��������
    DBO_GP_GameListResult GameListResult;
    ZeroMemory(&GameListResult,sizeof(GameListResult));

    //���ñ���
    GameListResult.cbSuccess=FALSE;

    //������Ϣ
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

    CTraceService::TraceString(TEXT("GSP_GP_LoadGamePageItem"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//������Ϣ
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //��������
    DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
    WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

    //Ч������
    ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKindEx))));
    if((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKindEx))))
    {
      return false;
    }

    //������ʶ
    TCHAR szMachineID[LEN_MACHINE_ID];
    CWHService::GetMachineID(szMachineID);

    //������Ϣ
    TCHAR szOnLineCountKind[2048]=TEXT("");
    for(WORD i=0; i<pOnLineCountInfo->wKindCount; i++)
    {
      INT nLength=lstrlen(szOnLineCountKind);
      _sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
    }

    //������Ϣ
    TCHAR szAndroidCountKind[2048]=TEXT("");
    for(WORD j=0; j<pOnLineCountInfo->wKindCount; j++)
    {
      INT nLength=lstrlen(szAndroidCountKind);
      _sntprintf(&szAndroidCountKind[nLength],CountArray(szAndroidCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[j].wKindID,pOnLineCountInfo->OnLineCountKind[j].dwAndroidCount);
    }

    //�������
    m_PlatformDBAide.ResetParameter();
    m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
    m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
    m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
    m_PlatformDBAide.AddParameter(TEXT("@dwAndroidCountSum"),pOnLineCountInfo->dwAndroidCountSum);
    //m_PlatformDBAide.AddParameter(TEXT("@dwMBCountSum"),pOnLineCountInfo->dwAndroidCountSum);
    m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);
    m_PlatformDBAide.AddParameter(TEXT("@strAndroidCountKind"),szAndroidCountKind);

    //ִ������
    m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GP_OnLineCountInfo"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//ƽ̨����
bool CDataBaseEngineSink::OnRequestPlatformParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //�������
    m_PlatformDBAide.ResetParameter();

    //ִ������
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadPlatformParameter"),true);

    //����ṹ
    DBO_GP_PlatformParameter PlatformParameter;
    ZeroMemory(&PlatformParameter,sizeof(PlatformParameter));

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //��¼�ж�
      if(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        //��ȡ����
        PlatformParameter.dwExchangeRate = m_PlatformDBAide.GetValue_DWORD(TEXT("ExchangeRate"));
        PlatformParameter.dwPresentExchangeRate = m_PlatformDBAide.GetValue_DWORD(TEXT("PresentExchangeRate"));
        PlatformParameter.dwRateGold = m_PlatformDBAide.GetValue_DWORD(TEXT("RateGold"));
      }
    }

    //��������
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_PLATFORM_PARAMETER,dwContextID,&PlatformParameter,sizeof(PlatformParameter));

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GR_LoadPlatformParameter"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��������
bool CDataBaseEngineSink::OnRequestLoadTaskList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
  try
  {
    //�������
    m_PlatformDBAide.ResetParameter();

    //ִ������
    LONG lResultCode = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadTaskList"),true);

    //ִ�гɹ�
    if(lResultCode==DB_SUCCESS)
    {
      //��������
      tagTaskParameter TaskParameter;
      BYTE cbDataBuffer[SOCKET_TCP_PACKET-1024]= {0};
      DBO_GP_TaskListInfo * pTaskListInfo = (DBO_GP_TaskListInfo *)cbDataBuffer;
      LPBYTE pDataBuffer=cbDataBuffer+sizeof(DBO_GP_TaskListInfo);

      //���ñ���
      WORD wTaskCount=0;
      WORD wSendDataSize=sizeof(DBO_GP_TaskListInfo);

      //��������
      while(m_PlatformDBModule->IsRecordsetEnd()==false)
      {
        //��ȡ����
        TaskParameter.wTaskID = m_PlatformDBAide.GetValue_WORD(TEXT("TaskID"));
        TaskParameter.wTaskType = m_PlatformDBAide.GetValue_WORD(TEXT("TaskType"));
        TaskParameter.cbPlayerType = m_PlatformDBAide.GetValue_BYTE(TEXT("UserType"));
        TaskParameter.wKindID = m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
        TaskParameter.wTaskObject= m_PlatformDBAide.GetValue_WORD(TEXT("Innings"));
        TaskParameter.dwTimeLimit = m_PlatformDBAide.GetValue_DWORD(TEXT("TimeLimit"));
        TaskParameter.lStandardAwardGold = m_PlatformDBAide.GetValue_LONGLONG(TEXT("StandardAwardGold"));
        TaskParameter.lStandardAwardMedal = m_PlatformDBAide.GetValue_LONGLONG(TEXT("StandardAwardMedal"));
        TaskParameter.lMemberAwardGold = m_PlatformDBAide.GetValue_LONGLONG(TEXT("MemberAwardGold"));
        TaskParameter.lMemberAwardMedal = m_PlatformDBAide.GetValue_LONGLONG(TEXT("MemberAwardMedal"));

        //������Ϣ
        m_PlatformDBAide.GetValue_String(TEXT("TaskName"),TaskParameter.szTaskName,CountArray(TaskParameter.szTaskName));
        m_PlatformDBAide.GetValue_String(TEXT("TaskDescription"),TaskParameter.szTaskDescribe,CountArray(TaskParameter.szTaskDescribe));

        //�����ж�
        if(wSendDataSize+sizeof(tagTaskParameter)>sizeof(cbDataBuffer))
        {
          //���ñ���
          pTaskListInfo->wTaskCount = wTaskCount;

          //��������
          m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_LIST,dwContextID,pTaskListInfo,wSendDataSize);

          //���ñ���
          wTaskCount=0;
          wSendDataSize=sizeof(DBO_GP_TaskListInfo);
          pDataBuffer = cbDataBuffer+sizeof(DBO_GP_TaskListInfo);
        }

        //��������
        CopyMemory(pDataBuffer,&TaskParameter,sizeof(tagTaskParameter));

        //���ñ���
        wTaskCount++;
        wSendDataSize += sizeof(tagTaskParameter);
        pDataBuffer += sizeof(tagTaskParameter);

        //�ƶ���¼
        m_PlatformDBModule->MoveToNext();
      }

      //ʣ�෢��
      if(wTaskCount>0 && wSendDataSize>0)
      {
        //���ñ���
        pTaskListInfo->wTaskCount = wTaskCount;

        //��������
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_LIST,dwContextID,pTaskListInfo,wSendDataSize);
      }

      //����֪ͨ
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TASK_LIST_END,dwContextID,NULL,0);
    }

    return true;
  }
  catch(IDataBaseException * pIException)
  {
    //�������
    CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

    CTraceService::TraceString(TEXT("GSP_GR_LoadTaskList"),TraceLevel_Exception);
    return false;
  }

  return true;
}

//��¼�ɹ�
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString,DWORD dwCheckUserRight, bool bMobileClient)
{
  if(bMobileClient==false)
  {
    if(dwErrorCode==DB_SUCCESS)
    {
      //��������
      DBO_GP_LogonSuccess LogonSuccess;
      ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

      //��������
      LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
      LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
      LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
      LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
      LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
      LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
      m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
      m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
      m_AccountsDBAide.GetValue_String(TEXT("DynamicPass"),LogonSuccess.szDynamicPass,CountArray(LogonSuccess.szDynamicPass));

      //�û��ɼ�
      LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
      LogonSuccess.lUserIngot=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Ingot"));
      LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));
      LogonSuccess.dUserBeans= m_AccountsDBAide.GetValue_DOUBLE(TEXT("Beans"));

      //�û�����
      LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
      LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
      m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

      //��Ա����
      LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
      m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

      //��չ��Ϣ
      LogonSuccess.cbInsureEnabled=m_AccountsDBAide.GetValue_BYTE(TEXT("InsureEnabled"));
      LogonSuccess.cbIsAgent=m_AccountsDBAide.GetValue_BYTE(TEXT("IsAgent"));
      LogonSuccess.dwCheckUserRight=dwCheckUserRight;

	  if (pszErrorString)
	  {
		  //��ȡ��Ϣ
		  lstrcpyn(LogonSuccess.szDescribeString, pszErrorString, CountArray(LogonSuccess.szDescribeString));
	  }

      //////////////////////////////////////////////////////////////////////////
      try
      {
        //��������
        WORD wPacketSize=0;
        BYTE cbBuffer[SOCKET_TCP_PACKET];

        //��ȡ����
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),LogonSuccess.dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@dwDeviceID"),1);
        //�������
        TCHAR szDescribeString[128]=TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);
        m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_LoadAgentGameKindItem"),true);

        //��������
        wPacketSize=0;
        tagAgentGameKind * pGameKind=NULL;
        while(m_AccountsDBModule->IsRecordsetEnd()==false)
        {
          //��ȡ��Ϣ
          pGameKind=(tagAgentGameKind *)(cbBuffer+wPacketSize);
          pGameKind->wKindID=m_AccountsDBAide.GetValue_WORD(TEXT("KindID"));
          pGameKind->wSortID=m_AccountsDBAide.GetValue_WORD(TEXT("SortID"));

          //����λ��
          wPacketSize+=sizeof(tagAgentGameKind);

          //�ƶ���¼
          m_AccountsDBModule->MoveToNext();
        }
        if(wPacketSize>0)
        {
          m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_AGENT_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
        }
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      //////////////////////////////////////////////////////////////////////////

      //���ͽ��
      WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
      WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
    }
    else if(dwErrorCode == DB_NEEDMB)
    {
      //��¼�ɹ�
      DBO_GP_ValidateMBCard ValidateMBCard;
      ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

      //��ȡ����
      ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
    }
    else if(dwErrorCode == DB_PASSPORT)
    {
      //Ͷ�ݽ��
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VALIDATE_PASSPORT,dwContextID,NULL,0);
    }
    else
    {
      //��������
      DBO_GP_LogonFailure LogonFailure;
      ZeroMemory(&LogonFailure,sizeof(LogonFailure));

      //��������
      LogonFailure.lResultCode=dwErrorCode;
      lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

      //���ͽ��
      WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
      WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
    }
  }
  else
  {
    if(dwErrorCode==DB_SUCCESS)
    {
      //��������
      DBO_MB_LogonSuccess LogonSuccess;
      ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

      //��������
      LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
      LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
      LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
      LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
      LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
      LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
      LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
	  m_AccountsDBAide.GetValue_String(TEXT("Accounts"), LogonSuccess.szAccount, CountArray(LogonSuccess.szAccount));
      m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
      m_AccountsDBAide.GetValue_String(TEXT("DynamicPass"),LogonSuccess.szDynamicPass,CountArray(LogonSuccess.szDynamicPass));
      m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

      //�û��ɼ�
      LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
      LogonSuccess.lUserIngot=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Ingot"));
      LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));
      LogonSuccess.dUserBeans= m_AccountsDBAide.GetValue_DOUBLE(TEXT("Beans"));

      //��Ա����
      LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
      m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

      //��չ��Ϣ
      LogonSuccess.cbInsureEnabled=m_AccountsDBAide.GetValue_BYTE(TEXT("InsureEnabled"));
      LogonSuccess.cbIsAgent=m_AccountsDBAide.GetValue_BYTE(TEXT("IsAgent"));

      //��ȡ��Ϣ
      lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

      //////////////////////////////////////////////////////////////////////////
      try
      {
        //��������
        WORD wPacketSize=0;
        BYTE cbBuffer[SOCKET_TCP_PACKET];

        //��ȡ����
        m_AccountsDBAide.ResetParameter();
        m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),LogonSuccess.dwUserID);
        m_AccountsDBAide.AddParameter(TEXT("@dwDeviceID"),2);
        //�������
        TCHAR szDescribeString[128]=TEXT("");
        m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);
        m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_LoadAgentGameKindItem"),true);

        //��������
        wPacketSize=0;
        tagAgentGameKind * pGameKind=NULL;
        while(m_AccountsDBModule->IsRecordsetEnd()==false)
        {
          //��ȡ��Ϣ
          pGameKind=(tagAgentGameKind *)(cbBuffer+wPacketSize);
          pGameKind->wKindID=m_AccountsDBAide.GetValue_WORD(TEXT("KindID"));
          pGameKind->wSortID=m_AccountsDBAide.GetValue_WORD(TEXT("SortID"));

          //����λ��
          wPacketSize+=sizeof(tagAgentGameKind);

          //�ƶ���¼
          m_AccountsDBModule->MoveToNext();
        }
        if(wPacketSize>0)
        {
          m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_AGENT_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
        }
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      //////////////////////////////////////////////////////////////////////////

      //���ͽ��
      WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
      WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
    }
    else
    {
      //��������
      DBO_MB_LogonFailure LogonFailure;
      ZeroMemory(&LogonFailure,sizeof(LogonFailure));

      //��������
      LogonFailure.lResultCode=dwErrorCode;
      lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

      //���ͽ��
      WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
      WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
      m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
    }
  }

  return;
}

//���н��
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
  if(dwErrorCode==DB_SUCCESS)
  {
    //��������
    DBO_GP_UserInsureSuccess UserInsureSuccess;
    ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

    //�������
    UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
    UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
    UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceInsure"));
    UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_LONGLONG(TEXT("InsureRevenue"));
    UserInsureSuccess.lVariationScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationScore"));
    UserInsureSuccess.lVariationInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationInsure"));
    lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

    //���ͽ��
    WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
    WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
  }
  else
  {
    //��������
    DBO_GP_UserInsureFailure UserInsureFailure;
    ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

    //�������
    UserInsureFailure.lResultCode=dwErrorCode;
    lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

    //���ͽ��
    WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
    WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
  }

  return;
}

//�������
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bCloseSocket)
{
  if(dwErrorCode==DB_SUCCESS)
  {
    //��������
    DBO_GP_OperateSuccess OperateSuccess;
    ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

    //�������
    OperateSuccess.bCloseSocket=bCloseSocket;
    OperateSuccess.lResultCode=dwErrorCode;
    lstrcpyn(OperateSuccess.szDescribeString,pszErrorString,CountArray(OperateSuccess.szDescribeString));

    //���ͽ��
    WORD wDataSize=CountStringBuffer(OperateSuccess.szDescribeString);
    WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wHeadSize+wDataSize);
  }
  else
  {
    //��������
    DBO_GP_OperateFailure OperateFailure;
    ZeroMemory(&OperateFailure,sizeof(OperateFailure));

    //�������
    OperateFailure.bCloseSocket=bCloseSocket;
    OperateFailure.lResultCode=dwErrorCode;
    lstrcpyn(OperateFailure.szDescribeString,pszErrorString,CountArray(OperateFailure.szDescribeString));

    //���ͽ��
    WORD wDataSize=CountStringBuffer(OperateFailure.szDescribeString);
    WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
    m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wHeadSize+wDataSize);
  }

  return;
}

//�������
VOID CDataBaseEngineSink::OnAndroidDisposeResult(DWORD dwContextID,DWORD dwErrorCode,WORD wSubCommdID,WORD wServerID)
{
  //����ṹ
  DBO_GP_AndroidParameter AndroidParameter;
  ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));

  //���ñ���
  AndroidParameter.wSubCommdID=wSubCommdID;
  AndroidParameter.wServerID=wServerID;

  if(dwErrorCode==DB_SUCCESS)
  {
    //��������
    WORD wParameterCount=0;

    while(m_AccountsDBModule->IsRecordsetEnd()==false)
    {
      wParameterCount = AndroidParameter.wParameterCount;
      AndroidParameter.AndroidParameter[wParameterCount].dwBatchID = m_AccountsDBAide.GetValue_DWORD(TEXT("BatchID"));
      AndroidParameter.AndroidParameter[wParameterCount].dwAndroidCount = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCount"));
      AndroidParameter.AndroidParameter[wParameterCount].dwServiceMode = m_AccountsDBAide.GetValue_DWORD(TEXT("ServiceMode"));
      AndroidParameter.AndroidParameter[wParameterCount].dwEnterTime = m_AccountsDBAide.GetValue_DWORD(TEXT("EnterTime"));
      AndroidParameter.AndroidParameter[wParameterCount].dwLeaveTime = m_AccountsDBAide.GetValue_DWORD(TEXT("LeaveTime"));
      AndroidParameter.AndroidParameter[wParameterCount].dwEnterMinInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("EnterMinInterval"));
      AndroidParameter.AndroidParameter[wParameterCount].dwEnterMaxInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("EnterMaxInterval"));
      AndroidParameter.AndroidParameter[wParameterCount].dwLeaveMinInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("LeaveMinInterval"));
      AndroidParameter.AndroidParameter[wParameterCount].dwLeaveMaxInterval = m_AccountsDBAide.GetValue_DWORD(TEXT("LeaveMaxInterval"));
      AndroidParameter.AndroidParameter[wParameterCount].dwSwitchMinInnings = m_AccountsDBAide.GetValue_DWORD(TEXT("SwitchMinInnings"));
      AndroidParameter.AndroidParameter[wParameterCount].dwSwitchMaxInnings = m_AccountsDBAide.GetValue_DWORD(TEXT("SwitchMaxInnings"));
      AndroidParameter.AndroidParameter[wParameterCount].lTakeMinScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("TakeMinScore"));
      AndroidParameter.AndroidParameter[wParameterCount].lTakeMaxScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("TakeMaxScore"));
      AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember0 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember0"));
      AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember1 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember1"));
      AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember2 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember2"));
      AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember3 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember3"));
      AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember4 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember4"));
      AndroidParameter.AndroidParameter[wParameterCount].AndroidCountMember5 = m_AccountsDBAide.GetValue_DWORD(TEXT("AndroidCountMember5"));


      //���ñ���
      AndroidParameter.wParameterCount++;

      //����ж�
      if(AndroidParameter.wParameterCount>=CountArray(AndroidParameter.AndroidParameter))
      {
        //��������
        m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ANDROID_PARAMETER,dwContextID,&AndroidParameter,sizeof(AndroidParameter));

        //���ñ���
        ZeroMemory(&AndroidParameter,sizeof(AndroidParameter));
      }

      //�ƶ��α�
      m_AccountsDBModule->MoveToNext();
    }
  }

  //�����С
  WORD wSendDataSize = sizeof(AndroidParameter)-sizeof(AndroidParameter.AndroidParameter);
  wSendDataSize += AndroidParameter.wParameterCount*sizeof(tagAndroidParameter);

  //��������
  m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_ANDROID_PARAMETER,dwContextID,&AndroidParameter,wSendDataSize);
}

VOID CDataBaseEngineSink::OnIndividualDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString)
{
  //����ṹ
  DBO_GP_IndividualResult individual;
  ZeroMemory(&individual,sizeof(individual));

  //�������
  individual.bSuccessed = true;
  individual.lScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
  lstrcpyn(individual.szDescribeString,pszErrorString,CountArray(individual.szDescribeString));

  //���ͽ��
  WORD wDataSize=CountStringBuffer(individual.szDescribeString);
  WORD wHeadSize=sizeof(individual)-sizeof(individual.szDescribeString);
  m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_INDIVIDUAL_RESULT,dwContextID,&individual,wHeadSize+wDataSize);
}

//////////////////////////////////////////////////////////////////////////////////
