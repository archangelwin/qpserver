#include "StdAfx.h"
#include "TCPSocket.h"
#include "NetworkManager.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CWHNetwork::CWHNetwork()
{
}

//��������
CWHNetwork::~CWHNetwork()
{
}

//�ӿڲ�ѯ
VOID *  CWHNetwork::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(IWHNetwork,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(IWHNetwork,Guid,dwQueryVer);
  return NULL;
}

//ö�ٴ���
bool  CWHNetwork::EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyDescribe)
{
  //��������
  BYTE bProxyType=(BYTE)wIndex;

  //��Ϣ���
  switch(bProxyType)
  {
    case PROXY_NONE:    //��ʹ�ô���
    {
      ProxyDescribe.cbProxyType=bProxyType;
      lstrcpyn(ProxyDescribe.szDescribe,TEXT("��ʹ�ô���"),CountArray(ProxyDescribe.szDescribe));
      return true;
    }
    case PROXY_HTTP:    //HTTP ����
    {
      ProxyDescribe.cbProxyType=bProxyType;
      lstrcpyn(ProxyDescribe.szDescribe,TEXT("HTTP ����"),CountArray(ProxyDescribe.szDescribe));
      return true;
    }
    case PROXY_SOCKS4:    //SOCKS 4 ����
    {
      ProxyDescribe.cbProxyType=bProxyType;
      lstrcpyn(ProxyDescribe.szDescribe,TEXT("SOCKS 4 ����"),CountArray(ProxyDescribe.szDescribe));
      return true;
    }
    case PROXY_SOCKS5:    //SOCKS 5 ����
    {
      ProxyDescribe.cbProxyType=bProxyType;
      lstrcpyn(ProxyDescribe.szDescribe,TEXT("SOCKS 5 ����"),CountArray(ProxyDescribe.szDescribe));
      return true;
    }
  }

  return false;
}

//�������
BYTE  CWHNetwork::ProxyServerTesting(BYTE cbProxyType, const tagProxyServer & ProxyServer)
{
  //Ч�����
  ASSERT(cbProxyType!=PROXY_NONE);
  if(cbProxyType==PROXY_NONE)
  {
    return CONNECT_EXCEPTION;
  }

  //��������
  CTCPSocket TCPSocket;
  if(TCPSocket.SetProxyServerInfo(cbProxyType,ProxyServer)==false)
  {
    return CONNECT_EXCEPTION;
  }

  //�������
  return TCPSocket.ProxyServerTesting();
}

//////////////////////////////////////////////////////////////////////////

//�������
CWHNetwork g_NetworkManager;

//�����������
extern "C" __declspec(dllexport) VOID *  CreateWHNetwork(const GUID & Guid, DWORD dwInterfaceVer)
{
  return g_NetworkManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
