#include "StdAfx.h"
#include "Afxinet.h"
#include "WEBSocketService.h"


//���캯��
CWEBSocketService::CWEBSocketService()
{
}

//��������
CWEBSocketService::~CWEBSocketService()
{
}

//�ӿڲ�ѯ
VOID * CWEBSocketService::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(IWEBSocketService,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(IWEBSocketService,Guid,dwQueryVer);
  return NULL;
}

//��������
bool CWEBSocketService::StartService()
{
  return true;
}
//ֹͣ����
bool CWEBSocketService::ConcludeService()
{
  return true;
}

//���ú���
bool CWEBSocketService::SetServiceID(WORD wServiceID)
{
  return true;
}

//���ýӿ�
bool CWEBSocketService::SetWEBSocketEvent(IUnknownEx * pIUnknownEx)
{
  return true;
}

//�ر�����
bool CWEBSocketService::CloseSocket(BYTE cbShutReason)
{
  return true;
}

//���Ӳ���
bool CWEBSocketService::ConnectServer(LPCTSTR pszURL, WORD wPort)
{
  return true;
}

//��������
bool CWEBSocketService::SendRequestData(VOID * pData, WORD wDataSize)
{
  return true;
}

//////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(WEBSocketService);

//////////////////////////////////////////////////////////////////////////
