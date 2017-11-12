#include "StdAfx.h"
#include "GlobalWebLink.h"
#include "GlobalUserInfo.h"
#include "OptionParameter.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����
CGlobalWebLink * CGlobalWebLink::m_pGlobalWebLink=NULL;

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalWebLink::CGlobalWebLink()
{
  //���ñ���
  if(m_pGlobalWebLink==NULL)
  {
    m_pGlobalWebLink=this;
  }

  //���ñ���
  m_szSpreadLink[0]=0;
  m_szUpdateLink[0]=0;
  m_szPlatformLink[0]=0;
  m_szValidateLink[0]=0;
  m_szPlatformLinkMax[0]=0;
  m_szPlatformLinkMin[0]=0;
  m_szQQLogonLink[0]=0;
  m_szHelpLink[0]=0;
  m_szLogonLink[0]=0;
  m_szFindPasswordLink[0]=0;
  m_szPayLink[0]=0;
  m_szTranRecordLink[0]=0;
  m_szPayRecordLink[0]=0;
  m_szWebRegisterLink[0]=0;
  m_szrLink1[0]=0;
  m_szrLink2[0]=0;
  m_szrLink3[0]=0;

  m_dwSessionVaule=17298;
  m_dwSessionVaule2=17298;
  m_dwUserID = 0;

}

//��������
CGlobalWebLink::~CGlobalWebLink()
{
  //���ñ���
  if(m_pGlobalWebLink==this)
  {
    m_pGlobalWebLink=NULL;
  }
}

//��֤����
LPCTSTR CGlobalWebLink::GetValidateLink()
{
  //��ȡ����
  CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
  tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

  //��������
  DWORD dwUserID = pGlobalUserData?pGlobalUserData->dwUserID:0L;
  DWORD dwTickCount = GetTickCount()-pGlobalUserData->dwLogonTickCount;

  //����ǩ��
  TCHAR szSignature[MAX_PATH]=TEXT("");
  _sntprintf(szSignature,CountArray(szSignature),TEXT("%d%s%d%s"),dwUserID,pGlobalUserData->szDynamicPass,dwTickCount,szValidateKey);

  //����ǩ��
  TCHAR szMD5Signature[LEN_MD5] = TEXT("");
  CWHEncrypt::MD5Encrypt(szSignature,szMD5Signature);

  //��������
  _sntprintf(m_szValidateLink,CountArray(m_szValidateLink),szValidateLink,dwUserID,dwTickCount,szMD5Signature);

  return m_szValidateLink;
}

//��ȡ��ֵ
UINT CGlobalWebLink::ReadFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nDefault)
{
  //��ȡĿ¼
  TCHAR szDirectory[MAX_PATH]=TEXT("");
  CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

  //����·��
  TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
  _sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\PlatformLink.INI"),szDirectory);

  //��ȡ����
  UINT nValue=GetPrivateProfileInt(pszKeyName,pszItemName,nDefault,szServerInfoPath);

  return nValue;
}

//��ȡ����
VOID CGlobalWebLink::ReadFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, TCHAR szResult[], WORD wMaxCount)
{
  //��ȡĿ¼
  TCHAR szDirectory[MAX_PATH]=TEXT("");
  CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

  //����·��
  TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
  _sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\PlatformLink.INI"),szDirectory);

  //��ȡ����
  TCHAR szResultEncrypt[MAX_ENCRYPT_LEN];
  GetPrivateProfileString(pszKeyName,pszItemName,TEXT(""),szResultEncrypt,CountArray(szResultEncrypt),szServerInfoPath);

  //��������
  szResult[0]=0;
  CWHEncrypt::XorCrevasse(szResultEncrypt,szResult,wMaxCount);

  return;
}

//��ȡ����
VOID CGlobalWebLink::LoadPaltformLink()
{
  //ƽ̨����
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("PlatformLink"),m_szPlatformLink,MAX_PATH);
  if(m_szPlatformLink[0]==0)
  {
    lstrcpyn(m_szPlatformLink,szPlatformLink,CountArray(m_szPlatformLink));
  }

  //�ƹ�����
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("SpreadLink"),m_szSpreadLink,MAX_PATH);
  if(m_szSpreadLink[0]==0)
  {
    lstrcpyn(m_szSpreadLink,m_szPlatformLink,CountArray(m_szSpreadLink));
  }

  //��������
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("UpdateLink"),m_szUpdateLink,MAX_PATH);
  if(m_szUpdateLink[0]==0)
  {
    lstrcpyn(m_szUpdateLink,m_szPlatformLink,CountArray(m_szUpdateLink));
  }
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("PlatformLinkMax"),m_szPlatformLinkMax,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("PlatformLinkMin"),m_szPlatformLinkMin,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("QQLogonLink"),m_szQQLogonLink,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("HelpLink"),m_szHelpLink,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("LogonLink"),m_szLogonLink,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("FindPasswordLink"),m_szFindPasswordLink,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("PayLink"),m_szPayLink,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("TranRecordLink"),m_szTranRecordLink,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("PayRecordLink"),m_szPayRecordLink,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("WebRegisterLink"),m_szWebRegisterLink,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("Link1"),m_szrLink1,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("Link2"),m_szrLink2,MAX_PATH);
  ReadFileString(TEXT("PLATFORMLINK"),TEXT("Link3"),m_szrLink3,MAX_PATH);

  return;
}
//��ȡȫ����
bool CGlobalWebLink::MakeLinkURL(OUT TCHAR szURL[],IN BYTE cbCode)
{
  //�����������
  TCHAR szResPassword[128];
  TCHAR szMD5Password[LEN_MD5];
  ZeroMemory(szMD5Password,sizeof(szMD5Password));
  ZeroMemory(szResPassword,sizeof(szResPassword));
  //_sntprintf(szResPassword,sizeof(szResPassword),TEXT("%sCbY5G48PcdqL8oG0"),m_GlobalUserData.szPassword);
  _sntprintf(szResPassword,sizeof(szResPassword),TEXT("%ld Kp,9023*Js-d6@2[i]"),m_dwSessionVaule);
  CWHEncrypt::MD5Encrypt(szResPassword,szMD5Password);

  TCHAR szURLLongonLink[MAX_PATH];
  ZeroMemory(szURLLongonLink,sizeof(szURLLongonLink));

  if(cbCode==6)
  {
    _sntprintf(szURLLongonLink,sizeof(szURLLongonLink),TEXT("%s"),m_szPayRecordLink);
  }
  else
  {
    _sntprintf(szURLLongonLink,sizeof(szURLLongonLink),TEXT("%s"),m_szTranRecordLink);
  }

  //�������ӵ�ַ
  _sntprintf(szURL,sizeof(TCHAR)*MAX_PATH,TEXT("%s/WebRule.aspx?u=%d&k=%s&c=%s"),m_szPlatformLink,m_dwUserID,szMD5Password,szURLLongonLink);

  return true;
}
void CGlobalWebLink::SetSessionVaule(DWORD dwSessionVaule, DWORD dwSessionVaule2)
{
  m_dwSessionVaule=dwSessionVaule;
  m_dwSessionVaule2=dwSessionVaule2;
}
void CGlobalWebLink::SetUserID(DWORD dwUserID)
{
  m_dwUserID=dwUserID;
}
DWORD CGlobalWebLink::GetSessionKey(TCHAR szSessionKey[LEN_MD5])
{
  DWORD dwSessionVaule=0;
  CWHRegKey RegUserInfo;
  if(RegUserInfo.OpenRegKey(REG_USER_INFO,false))
  {
    dwSessionVaule=RegUserInfo.GetValue(TEXT("GUIDKey"),0L);
    if(dwSessionVaule==0)
    {
      dwSessionVaule=17298+rand()%1000000+(rand()%100)*2;
    }

    dwSessionVaule=dwSessionVaule+2;
    RegUserInfo.WriteValue(TEXT("GUIDKey"),dwSessionVaule);
  }
  if(dwSessionVaule==0)
  {
    dwSessionVaule=17298;
  }

  m_dwSessionVaule=dwSessionVaule;

  TCHAR szResPassword[128];
  ZeroMemory(szSessionKey,sizeof(szSessionKey));
  ZeroMemory(szResPassword,sizeof(szResPassword));
  _sntprintf(szResPassword,sizeof(szResPassword),TEXT("%ld Kp,9023*Js-d6@2[i]"),dwSessionVaule);
  CWHEncrypt::MD5Encrypt(szResPassword,szSessionKey);

  return dwSessionVaule;
}
//////////////////////////////////////////////////////////////////////////////////
