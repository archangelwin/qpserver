#ifndef GLOBAL_WEBLINK_HEAD_FILE
#define GLOBAL_WEBLINK_HEAD_FILE

#pragma once

#include "PlatformDataHead.h"

//////////////////////////////////////////////////////////////////////////////////

//ȫ�ַ�����
class PLATFORM_DATA_CLASS CGlobalWebLink
{
  //��������
protected:
  TCHAR             m_szSpreadLink[MAX_PATH];         //�ƹ�����
  TCHAR             m_szUpdateLink[MAX_PATH];         //��������
  TCHAR             m_szPlatformLink[MAX_PATH];         //ƽ̨����
  TCHAR             m_szValidateLink[MAX_PATH];         //��֤����
  TCHAR             m_szPlatformLinkMax[MAX_PATH];          //ƽ̨����
  TCHAR             m_szPlatformLinkMin[MAX_PATH];          //ƽ̨����
  TCHAR             m_szQQLogonLink[MAX_PATH];
  TCHAR             m_szHelpLink[MAX_PATH];
  TCHAR             m_szLogonLink[MAX_PATH];
  TCHAR             m_szFindPasswordLink[MAX_PATH];
  TCHAR             m_szPayLink[MAX_PATH];
  TCHAR             m_szTranRecordLink[MAX_PATH];
  TCHAR             m_szPayRecordLink[MAX_PATH];
  TCHAR             m_szWebRegisterLink[MAX_PATH];
  TCHAR             m_szrLink1[MAX_PATH];
  TCHAR             m_szrLink2[MAX_PATH];
  TCHAR             m_szrLink3[MAX_PATH];

  DWORD             m_dwSessionVaule;
  DWORD             m_dwSessionVaule2;
  DWORD             m_dwUserID;

  //��̬����
protected:
  static CGlobalWebLink *     m_pGlobalWebLink;             //ȫ������

  //��������
public:
  //���캯��
  CGlobalWebLink();
  //��������
  virtual ~CGlobalWebLink();

  //���ܺ���
public:
  //��ȡ����
  VOID LoadPaltformLink();

  //������Ϣ
public:
  //��֤����
  LPCTSTR GetValidateLink();
  //�ƹ�����
  LPCTSTR GetSpreadLink()
  {
    return m_szSpreadLink;
  }
  //��������
  LPCTSTR GetUpdateLink()
  {
    return m_szUpdateLink;
  }
  //ƽ̨����
  LPCTSTR GetPlatformLink()
  {
    return m_szPlatformLink;
  }
  LPCTSTR GetPlatformLinkMax()
  {
    return m_szPlatformLinkMax;
  }
  LPCTSTR GetPlatformLinkMin()
  {
    return m_szPlatformLinkMin;
  }
  LPCTSTR GetQQLogonLink()
  {
    return m_szQQLogonLink;
  }
  LPCTSTR GetHelpLink()
  {
    return m_szHelpLink;
  }
  LPCTSTR GetLogonLink()
  {
    return m_szLogonLink;
  }
  LPCTSTR GetFindPasswordLink()
  {
    return m_szFindPasswordLink;
  }
  LPCTSTR GetPayLink()
  {
    return m_szPayLink;
  }
  LPCTSTR GetTranRecordLink()
  {
    return m_szTranRecordLink;
  }
  LPCTSTR GetPayRecordLink()
  {
    return m_szPayRecordLink;
  }
  LPCTSTR GetWebRegisterLink()
  {
    return m_szWebRegisterLink;
  }
  LPCTSTR GetLink1()
  {
    return m_szrLink1;
  }
  LPCTSTR GetLink2()
  {
    return m_szrLink2;
  }
  LPCTSTR GetLink3()
  {
    return m_szrLink3;
  }
  //��ȡȫ����
  bool MakeLinkURL(OUT TCHAR szURL[],IN BYTE cbCode);
  void SetSessionVaule(DWORD dwSessionVaule, DWORD dwSessionVaule2);
  void SetUserID(DWORD dwUserID);
  DWORD GetSessionKey(TCHAR szSessionKey[LEN_MD5]);

  //��ȡ����
protected:
  //��ȡ��ֵ
  UINT ReadFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nDefault);
  //��ȡ����
  VOID ReadFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, TCHAR szResult[], WORD wMaxCount);

  //��̬����
public:
  //��ȡ����
  static CGlobalWebLink * GetInstance()
  {
    return m_pGlobalWebLink;
  }
};

//////////////////////////////////////////////////////////////////////////////////

#endif