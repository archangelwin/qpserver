#include "StdAfx.h"
#include "Afxinet.h"
#include "UDPNetworkEngine.h"


//���캯��
CUDPNetworkEngine::CUDPNetworkEngine()
{
}

//��������
CUDPNetworkEngine::~CUDPNetworkEngine()
{
}

//�ӿڲ�ѯ
VOID * CUDPNetworkEngine::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(IUDPNetworkEngine,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(IUDPNetworkEngine,Guid,dwQueryVer);
  return NULL;
}

//��������
bool  CUDPNetworkEngine::StartService()
{
  return true;
}
//ֹͣ����
bool  CUDPNetworkEngine::ConcludeService()
{
  return true;
}

//���ö˿�
WORD CUDPNetworkEngine::GetServicePort()
{
  return 0;
}
//��ǰ�˿�
WORD CUDPNetworkEngine::GetCurrentPort()
{
  return 0;
}


//���ýӿ�
bool CUDPNetworkEngine::SetUDPNetworkEngineEvent(IUnknownEx * pIUnknownEx)
{
  return true;
}
//���ò���
bool CUDPNetworkEngine::SetServiceParameter(WORD wServicePort, WORD wMaxConnect)
{
  return true;
}

//���ͺ���
bool CUDPNetworkEngine::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID)
{
  return true;
}

//���ͺ���
bool CUDPNetworkEngine::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
  return true;
}

//�ر�����
bool CUDPNetworkEngine::CloseSocket(DWORD dwSocketID)
{
  return true;
}

//���ùر�
bool CUDPNetworkEngine::ShutDownSocket(DWORD dwSocketID)
{
  return true;
}

//////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(UDPNetworkEngine);

//////////////////////////////////////////////////////////////////////////
