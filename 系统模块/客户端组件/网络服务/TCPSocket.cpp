#include "Stdafx.h"
#include "TCPSocket.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define WM_SOCKET_NOTIFY      (WM_USER+100)           //������Ϣ

//ʱ�䶨��
#define IDI_DETEST_SOCKET     100                 //������

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTCPSocket, CWnd)
  ON_WM_TIMER()
  ON_MESSAGE(WM_SOCKET_NOTIFY,OnSocketNotify)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CTCPSocket::CTCPSocket()
{
  //��������
  m_wSocketID=0;
  m_cbSocketStatus=SOCKET_STATUS_IDLE;

  //�ں˱���
  m_pITCPSocketSink=NULL;
  m_hSocket=INVALID_SOCKET;

  //������Ϣ
  m_wProxyType=PROXY_NONE;
  ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

  //���ձ���
  m_wRecvSize=0;
  ZeroMemory(m_cbRecvBuf,sizeof(m_cbRecvBuf));

  //�������
  m_dwBufferData=0L;
  m_dwBufferSize=0L;
  m_bNeedBuffer=false;
  m_pcbDataBuffer=NULL;

  //��������
  m_cbSendRound=0;
  m_cbRecvRound=0;
  m_dwSendXorKey=0;
  m_dwRecvXorKey=0;

  //��������
  m_dwSendTickCount=0;
  m_dwRecvTickCount=0;
  m_dwSendPacketCount=0;
  m_dwRecvPacketCount=0;

  return;
}

//��������
CTCPSocket::~CTCPSocket()
{
  //�ر�����
  if(m_hSocket!=INVALID_SOCKET)
  {
    closesocket(m_hSocket);
    m_hSocket=INVALID_SOCKET;
  }

  //ɾ������
  SafeDeleteArray(m_pcbDataBuffer);

  return;
}

//�ӿڲ�ѯ
VOID *  CTCPSocket::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
  QUERYINTERFACE(ITCPSocket,Guid,dwQueryVer);
  QUERYINTERFACE_IUNKNOWNEX(ITCPSocket,Guid,dwQueryVer);
  return NULL;
}

//���ýӿ�
bool  CTCPSocket::SetTCPSocketSink(IUnknownEx * pIUnknownEx)
{
  //���ýӿ�
  if(pIUnknownEx!=NULL)
  {
    //��ѯ�ӿ�
    ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPSocketSink)!=NULL);
    m_pITCPSocketSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPSocketSink);

    //�ɹ��ж�
    if(m_pITCPSocketSink==NULL)
    {
      return false;
    }
  }
  else
  {
    m_pITCPSocketSink=NULL;
  }

  return true;
}


//�������
BYTE  CTCPSocket::ProxyServerTesting()
{
  //Ч�����
  ASSERT(m_hSocket==INVALID_SOCKET);
  ASSERT(m_wProxyType!=PROXY_NONE);
  ASSERT(m_cbSocketStatus==SOCKET_STATUS_IDLE);

  //Ч��״̬
  if(m_hSocket!=INVALID_SOCKET)
  {
    return CONNECT_EXCEPTION;
  }
  if(m_wProxyType==PROXY_NONE)
  {
    return CONNECT_EXCEPTION;
  }
  if(m_cbSocketStatus!=SOCKET_STATUS_IDLE)
  {
    return CONNECT_EXCEPTION;
  }

  //���� SOCKET
  m_hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(m_hSocket==INVALID_SOCKET)
  {
    return CONNECT_EXCEPTION;
  }

  //���Ӵ���
  BYTE bResult=ConnectProxyServer();

  //�ر�����
  closesocket(m_hSocket);
  m_hSocket=INVALID_SOCKET;

  return bResult;
}

//���ô���
bool  CTCPSocket::SetProxyServerInfo(BYTE cbProxyType, const tagProxyServer & ProxyServer)
{
  switch(cbProxyType)
  {
    case PROXY_NONE:  //û�д���
    {
      //���ñ���
      m_wProxyType=cbProxyType;
      ZeroMemory(&m_ProxyServer,sizeof(m_ProxyServer));

      return true;
    }
    case PROXY_HTTP:  //HTTP ����
    case PROXY_SOCKS4:  //SOKKS4 ����
    case PROXY_SOCKS5:  //SOKKS5 ����
    {
      //���ñ���
      m_wProxyType=cbProxyType;
      m_ProxyServer=ProxyServer;

      return true;
    }
    default:
    {
      ASSERT(FALSE);
      return false;
    }
  }

  return false;
}

//���Ӳ���
BYTE  CTCPSocket::Connect(DWORD dwServerIP, WORD wPort)
{
  //Ч�����
  ASSERT(dwServerIP!=INADDR_NONE);
  ASSERT(m_hSocket==INVALID_SOCKET);
  ASSERT(m_cbSocketStatus==SOCKET_STATUS_IDLE);

  //���ò���
  m_wRecvSize=0;
  m_cbSendRound=0;
  m_cbRecvRound=0;
  m_dwSendXorKey=0x12345678;
  m_dwRecvXorKey=0x12345678;
  m_dwSendTickCount=GetTickCount()/1000L;
  m_dwRecvTickCount=GetTickCount()/1000L;

  try
  {
    //���� SOCKET
    m_hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(m_hSocket==INVALID_SOCKET)
    {
      throw CONNECT_EXCEPTION;
    }

    //��������
    if(m_hWnd==NULL)
    {
      CRect rcSocketWnd;
      Create(NULL,NULL,WS_CHILD,rcSocketWnd,GetDesktopWindow(),100);
    }

    //��������
    SOCKADDR_IN SocketAddr;
    ZeroMemory(&SocketAddr,sizeof(SocketAddr));

    //���ñ���
    SocketAddr.sin_family=AF_INET;
    SocketAddr.sin_port=htons(wPort);
    SocketAddr.sin_addr.S_un.S_addr=dwServerIP;

    //��������
    if(m_wProxyType!=PROXY_NONE)
    {
      //���Ӵ���
      BYTE bResult=ConnectProxyServer();
      if(bResult!=CONNECT_SUCCESS)
      {
        throw bResult;
      }

      //��������
      switch(m_wProxyType)
      {
        case PROXY_HTTP:  //HTTP ����
        {
          //��������
          char cbDataBuffer[512];
          BYTE * pcbServerIP=(BYTE *)&dwServerIP;
          _snprintf(cbDataBuffer,CountArray(cbDataBuffer),"CONNECT %d.%d.%d.%d:%d HTTP/1.0\r\nUser-Agent: WHPlatform/0.1\r\n\r\n",
                    pcbServerIP[0],pcbServerIP[1],pcbServerIP[2],pcbServerIP[3],wPort);

          //��������
          INT nRequestLen=(INT)strlen(cbDataBuffer);
          if(send(m_hSocket,cbDataBuffer,nRequestLen,0)!=nRequestLen)
          {
            throw CONNECT_PROXY_FAILURE;
          }

          //������Ӧ
          INT nRecvCount=recv(m_hSocket,cbDataBuffer,sizeof(cbDataBuffer),0);
          if(nRecvCount>0)
          {
            cbDataBuffer[nRecvCount]=0;
          }

          //����ж�
          bool bSuccess=false;
          for(INT i=0; i<nRecvCount; i++)
          {
            if((cbDataBuffer[i]==' ')&&(cbDataBuffer[i+1]=='2')&&(cbDataBuffer[i+2]=='0')&&(cbDataBuffer[i+3]=='0'))
            {
              bSuccess=true;
              break;
            }
          }

          //����ж�
          if(bSuccess==false)
          {
            throw CONNECT_PROXY_FAILURE;
          }

          break;
        }
        case PROXY_SOCKS5:  //Socks5 ����
        {
          //��������
          INT nOff=4;
          char cbDataBuffer[256]= {5,1,0,1};

          //���ӵ�ַ
          CopyMemory(cbDataBuffer+nOff,&SocketAddr.sin_addr,sizeof(SocketAddr.sin_addr));
          nOff+=sizeof(SocketAddr.sin_addr);

          //���Ӷ˿�
          *(WORD*)(cbDataBuffer+nOff)=SocketAddr.sin_port;
          nOff+=sizeof(WORD);

          //��������
          if(send(m_hSocket,cbDataBuffer,nOff,0)!=nOff)
          {
            throw CONNECT_PROXY_FAILURE;
          }

          //������Ӧ
          INT nRecvCount=recv(m_hSocket,cbDataBuffer,4,0);
          if(nRecvCount!=4)
          {
            throw CONNECT_PROXY_FAILURE;
          }
          if(cbDataBuffer[0]!=5)
          {
            throw CONNECT_PROXY_FAILURE;
          }

          //ʧ���ж�
          if(cbDataBuffer[1]==0)
          {
            return CONNECT_PROXY_FAILURE;
          }

          //��������
          INT nLeftDataLen=(cbDataBuffer[3]==1)?6:cbDataBuffer[4]+2;
          if(recv(m_hSocket,cbDataBuffer,nLeftDataLen,0)!=nLeftDataLen)
          {
            throw CONNECT_PROXY_FAILURE;
          }

          break;
        }
        case PROXY_SOCKS4:  //Socks4 ����
        {
          //��������
          INT nOff=2;
          char cbDataBuffer[256]= {4,1};

          //���Ӷ˿�
          *(WORD*)(cbDataBuffer+nOff)=SocketAddr.sin_port;
          nOff+=sizeof(WORD);

          //���ӵ�ַ
          CopyMemory(cbDataBuffer+nOff,&SocketAddr.sin_addr,sizeof(SocketAddr.sin_addr));
          nOff+=sizeof(SocketAddr.sin_addr);

          //�����ֶ�
          cbDataBuffer[nOff++]=0;

          //��������
          INT nResult=send(m_hSocket,cbDataBuffer,nOff,0);
          if(nResult!=nOff)
          {
            throw CONNECT_PROXY_FAILURE;
          }

          //������Ӧ
          nResult=recv(m_hSocket,cbDataBuffer,8,0);
          if(nResult!=8)
          {
            throw CONNECT_PROXY_FAILURE;
          }
          if(cbDataBuffer[0]!=0)
          {
            throw CONNECT_PROXY_FAILURE;
          }

          //ʧ���ж�
          if(cbDataBuffer[1]!=90)
          {
            return CONNECT_PROXY_FAILURE;
          }

          break;
        }
        default:    //Ĭ�ϴ���
        {
          ASSERT(FALSE);
          throw CONNECT_PROXY_FAILURE;
        }
      }

      //���ӳɹ�
      WSASetLastError(0);

      //���ñ���
      m_cbSocketStatus=SOCKET_STATUS_CONNECT;

      //������Ϣ
      PostMessage(WM_SOCKET_NOTIFY,0,MAKELONG(FD_CONNECT,0));

      //�󶨴���
      WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY,FD_READ|FD_CONNECT|FD_CLOSE|FD_WRITE);
    }
    else
    {
      //�󶨴���
      WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY,FD_READ|FD_CONNECT|FD_CLOSE|FD_WRITE);

      //��������
      INT nErrorCode=connect(m_hSocket,(SOCKADDR *)&SocketAddr,sizeof(SocketAddr));
      if((nErrorCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSAEWOULDBLOCK))
      {
        throw CONNECT_EXCEPTION;
      }

      //���ñ���
      m_cbSocketStatus=SOCKET_STATUS_WAIT;
    }

    return CONNECT_SUCCESS;
  }
  catch(...)
  {
    //�ر�����
    CloseSocket(SHUT_REASON_INSIDE);
  }

  return CONNECT_EXCEPTION;
}

//���Ӳ���
BYTE CTCPSocket::Connect(LPCTSTR pszServerIP, WORD wPort)
{
  //Ч������
  ASSERT((wPort!=0)&&(pszServerIP!=NULL));
  if((pszServerIP==NULL)||(wPort==0))
  {
    return false;
  }

  //���Ӳ���
  return Connect(TranslateAddr(pszServerIP),wPort);
}

//���ͺ���
WORD  CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
  //Ч��״̬
  if(m_hSocket==INVALID_SOCKET)
  {
    return 0L;
  }
  if(m_cbSocketStatus!=SOCKET_STATUS_CONNECT)
  {
    return 0L;
  }

  //��������
  BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
  TCP_Head * pHead=(TCP_Head *)cbDataBuffer;

  //���ñ���
  pHead->CommandInfo.wSubCmdID=wSubCmdID;
  pHead->CommandInfo.wMainCmdID=wMainCmdID;

  //��������
// WORD wSendSize=EncryptBuffer(cbDataBuffer,sizeof(TCP_Head),sizeof(cbDataBuffer));
  MappedBuffer(cbDataBuffer, sizeof(TCP_Head));
  //��������
  return SendDataBuffer(cbDataBuffer, sizeof(TCP_Head));
}

//���ͺ���
WORD  CTCPSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
  //Ч��״̬
  if(m_hSocket==INVALID_SOCKET)
  {
    return false;
  }
  if(m_cbSocketStatus!=SOCKET_STATUS_CONNECT)
  {
    return false;
  }

  //Ч���С
  ASSERT(wDataSize<=SOCKET_TCP_PACKET);
  if(wDataSize>SOCKET_TCP_PACKET)
  {
    return false;
  }

  //��������
  BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
  TCP_Head * pHead=(TCP_Head *)cbDataBuffer;

  //���ñ���
  pHead->CommandInfo.wMainCmdID=wMainCmdID;
  pHead->CommandInfo.wSubCmdID=wSubCmdID;

  //��չ����
  if(wDataSize>0)
  {
    ASSERT(pData!=NULL);
    CopyMemory(pHead+1,pData,wDataSize);
  }

  //��������
  //WORD wSendSize=EncryptBuffer(cbDataBuffer,sizeof(TCP_Head)+wDataSize,sizeof(cbDataBuffer));
  MappedBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize);
  //��������
  return SendDataBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize);
}

//�ر�����
VOID  CTCPSocket::CloseSocket()
{
  //�ر�����
  CloseSocket(SHUT_REASON_NORMAL);

  return;
}

//�ر�����
VOID CTCPSocket::CloseSocket(BYTE cbShutReason)
{
  //���ñ���
  m_wRecvSize=0;
  m_cbSendRound=0;
  m_cbRecvRound=0;
  m_dwSendXorKey=0;
  m_dwRecvXorKey=0;

  //��������
  m_dwSendTickCount=0;
  m_dwRecvTickCount=0;
  m_dwSendPacketCount=0;
  m_dwRecvPacketCount=0;

  //�������
  m_dwBufferData=0L;
  m_dwBufferSize=0L;
  m_bNeedBuffer=false;
  SafeDeleteArray(m_pcbDataBuffer);

  //״̬����
  m_cbSocketStatus=SOCKET_STATUS_IDLE;

  //�ر��ж�
  if(m_hSocket!=INVALID_SOCKET)
  {
    //ɾ��ʱ��
    KillTimer(IDI_DETEST_SOCKET);

    //ע���¼�
    WSAAsyncSelect(m_hSocket,m_hWnd,WM_SOCKET_NOTIFY,0);

    //�ر�����
    closesocket(m_hSocket);
    m_hSocket=INVALID_SOCKET;

    //�¼�֪ͨ
    if(cbShutReason!=SHUT_REASON_INSIDE)
    {
      ASSERT(m_pITCPSocketSink!=NULL);
      if(m_pITCPSocketSink!=NULL)
      {
        m_pITCPSocketSink->OnEventTCPSocketShut(m_wSocketID,cbShutReason);
      }
    }
  }

  return;
}

//��������
VOID CTCPSocket::AmortizeBuffer(VOID * pData, WORD wDataSize)
{
  //���뻺��
  if((m_dwBufferData+wDataSize)>m_dwBufferSize)
  {
    //��������
    LPBYTE pcbDataBuffer=NULL;
    LPBYTE pcbDeleteBuffer=m_pcbDataBuffer;

    //�����С
    DWORD dwNeedSize=m_dwBufferData+wDataSize;
    DWORD dwApplySize=__max(dwNeedSize,m_dwBufferSize*2L);

    //���뻺��
    try
    {
      pcbDataBuffer=new BYTE [dwApplySize];
    }
    catch(...) { }

    //ʧ���ж�
    if(pcbDataBuffer==NULL)
    {
      CloseSocket(SHUT_REASON_EXCEPTION);
      return;
    }

    //���ñ���
    m_dwBufferSize=dwApplySize;
    m_pcbDataBuffer=pcbDataBuffer;
    CopyMemory(m_pcbDataBuffer,pcbDeleteBuffer,m_dwBufferData);

    //ɾ������
    SafeDeleteArray(pcbDeleteBuffer);
  }

  //���ñ���
  m_bNeedBuffer=true;
  m_dwBufferData+=wDataSize;
  CopyMemory(m_pcbDataBuffer+m_dwBufferData-wDataSize,pData,wDataSize);

  return;
}

//��������
WORD CTCPSocket::SendDataBuffer(VOID * pBuffer, WORD wSendSize)
{
  //��������
  WORD wTotalCount=0;

  //���ñ���
  m_dwSendTickCount=GetTickCount()/1000L;

  //��������
  while((m_bNeedBuffer==false)&&(wTotalCount<wSendSize))
  {
    //��������
    INT nSendCount=send(m_hSocket,(char *)pBuffer+wTotalCount,wSendSize-wTotalCount,0);

    //�����ж�
    if(nSendCount==SOCKET_ERROR)
    {
      //�����ж�
      if(WSAGetLastError()==WSAEWOULDBLOCK)
      {
        AmortizeBuffer((LPBYTE)pBuffer+wTotalCount,wSendSize-wTotalCount);
        return wSendSize;
      }

      //�ر�����
      CloseSocket(SHUT_REASON_EXCEPTION);

      return 0L;
    }
    else
    {
      //���ñ���
      wTotalCount+=nSendCount;
    }
  }

  //��������
  if(wTotalCount>wSendSize)
  {
    AmortizeBuffer((LPBYTE)pBuffer+wTotalCount,wSendSize-wTotalCount);
  }

  return wSendSize;
}

//���ӳ��
WORD CTCPSocket::SeedRandMap(WORD wSeed)
{
  DWORD dwHold=wSeed;
  return (WORD)((dwHold=dwHold*241103L+2533101L)>>16);
}

//ӳ�䷢������
BYTE CTCPSocket::MapSendByte(BYTE cbData)
{
  BYTE cbMap=g_SendByteMap[(BYTE)(cbData+m_cbSendRound)];
  m_cbSendRound+=3;
  return cbMap;
}

//ӳ���������
BYTE CTCPSocket::MapRecvByte(BYTE cbData)
{
  BYTE cbMap=g_RecvByteMap[cbData]-m_cbRecvRound;
  m_cbRecvRound+=3;
  return cbMap;
}

//���Ӵ���
BYTE CTCPSocket::ConnectProxyServer()
{
  //Ч��״̬
  ASSERT(m_wProxyType!=PROXY_NONE);
  if(m_wProxyType==PROXY_NONE)
  {
    return CONNECT_EXCEPTION;
  }

  //��������
  SOCKADDR_IN SocketAddrProxy;
  ZeroMemory(&SocketAddrProxy,sizeof(SocketAddrProxy));

  //���ñ���
  SocketAddrProxy.sin_family=AF_INET;
  SocketAddrProxy.sin_port=htons(m_ProxyServer.wProxyPort);
  SocketAddrProxy.sin_addr.S_un.S_addr=TranslateAddr(m_ProxyServer.szProxyServer);

  //���Ӵ���
  INT nErrorCode=connect(m_hSocket,(sockaddr *)&SocketAddrProxy,sizeof(SocketAddrProxy));
  if(nErrorCode==SOCKET_ERROR)
  {
    return CONNECT_PROXY_FAILURE;
  }

  //��������
  if(m_wProxyType==PROXY_SOCKS5)
  {
    //��������
    INT nOff=2;
    char cbDataBuffer[256]= {5,1};

    //��������
    cbDataBuffer[nOff++]=(m_ProxyServer.szUserName[0]==0)?0:2;
    if(send(m_hSocket,cbDataBuffer,nOff,0)!=nOff)
    {
      return CONNECT_PROXY_FAILURE;
    }

    //��������
    if(recv(m_hSocket,cbDataBuffer,2,0)!=2)
    {
      return CONNECT_PROXY_FAILURE;
    }

    //���ݷ���
    if(cbDataBuffer[0]!=5)
    {
      return CONNECT_PROXY_FAILURE;
    }
    if((cbDataBuffer[1]!=0)&&(cbDataBuffer[1]!=2))
    {
      return CONNECT_PROXY_FAILURE;
    }

    //�����֤
    if(cbDataBuffer[1]==2)
    {
      //��������
      INT nStringLen=0;
      CT2CA strUserName(m_ProxyServer.szUserName);
      CT2CA strPassword(m_ProxyServer.szPassword);

      //��¼�ʺ�
      nOff=0;
      cbDataBuffer[nOff++]=1;
      nStringLen=lstrlenA(strUserName);
      cbDataBuffer[nOff++]=(char)nStringLen;
      CopyMemory(cbDataBuffer+nOff,(LPCSTR)strUserName,nStringLen);
      nOff+=nStringLen;

      //��¼����
      nStringLen=lstrlenA(strPassword);
      cbDataBuffer[nOff++]=(char)nStringLen;
      CopyMemory(cbDataBuffer+nOff,(LPCSTR)strPassword,nStringLen);
      nOff+=nStringLen;

      //��������
      if(send(m_hSocket,cbDataBuffer,nOff,0)!=nOff)
      {
        return CONNECT_PROXY_FAILURE;
      }

      //��������
      if(recv(m_hSocket,cbDataBuffer,2,0)!=2)
      {
        return CONNECT_PROXY_FAILURE;
      }

      //���ݷ���
      if((cbDataBuffer[0]!=1)||(cbDataBuffer[1]!=0))
      {
        return CONNECT_PROXY_USER_INVALID;
      }
    }
  }

  return CONNECT_SUCCESS;
}

//��ַ����
DWORD CTCPSocket::TranslateAddr(LPCTSTR pszServerAddr)
{
  //ת����ַ
  CT2CA ServerAddr(pszServerAddr);
  DWORD dwServerIP=inet_addr(ServerAddr);

  //��������
  if(dwServerIP==INADDR_NONE)
  {
    LPHOSTENT lpHost=gethostbyname(ServerAddr);
    if(lpHost==NULL)
    {
      return INADDR_NONE;
    }
    dwServerIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
  }

  return dwServerIP;
}

//��������
WORD CTCPSocket::CrevasseBuffer(BYTE cbDataBuffer[], WORD wDataSize)
{
  //Ч�����
  ASSERT(m_dwSendPacketCount>0);
  ASSERT(wDataSize>=sizeof(TCP_Head));
  ASSERT(((TCP_Head *)cbDataBuffer)->TCPInfo.wPacketSize==wDataSize);

  //��������
  WORD wSnapCount=0;
  if((wDataSize%sizeof(DWORD))!=0)
  {
    wSnapCount=sizeof(DWORD)-wDataSize%sizeof(DWORD);
    ZeroMemory(cbDataBuffer+wDataSize,wSnapCount);
  }

  //��������
  DWORD dwXorKey=m_dwRecvXorKey;
  DWORD * pdwXor=(DWORD *)(cbDataBuffer+sizeof(TCP_Info));
  WORD  * pwSeed=(WORD *)(cbDataBuffer+sizeof(TCP_Info));
  WORD wEncrypCount=(wDataSize+wSnapCount-sizeof(TCP_Info))/4;
  for(WORD i=0; i<wEncrypCount; i++)
  {
    if((i==(wEncrypCount-1))&&(wSnapCount>0))
    {
      BYTE * pcbKey=((BYTE *)&m_dwRecvXorKey)+sizeof(DWORD)-wSnapCount;
      CopyMemory(cbDataBuffer+wDataSize,pcbKey,wSnapCount);
    }
    dwXorKey=SeedRandMap(*pwSeed++);
    dwXorKey|=((DWORD)SeedRandMap(*pwSeed++))<<16;
    dwXorKey^=g_dwPacketKey;
    *pdwXor++^=m_dwRecvXorKey;
    m_dwRecvXorKey=dwXorKey;
  }

  //Ч���ֽ�
  TCP_Head * pHead=(TCP_Head *)cbDataBuffer;
  BYTE cbCheckCode=pHead->TCPInfo.cbCheckCode;
  for(INT i=sizeof(TCP_Info); i<wDataSize; i++)
  {
    cbDataBuffer[i]=MapRecvByte(cbDataBuffer[i]);
    cbCheckCode+=cbDataBuffer[i];
  }

  //�ر�����
  if(cbCheckCode!=0)
  {
    ASSERT(FALSE);
    return 0;
  }

  return wDataSize;
}

//��������
WORD CTCPSocket::EncryptBuffer(BYTE cbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
  //Ч�����
  ASSERT(wDataSize>=sizeof(TCP_Head));
  ASSERT(wBufferSize>=(wDataSize+2*sizeof(DWORD)));
  ASSERT(wDataSize<=(sizeof(TCP_Head)+SOCKET_TCP_PACKET));

  //��������
  WORD wEncryptSize=wDataSize-sizeof(TCP_Command),wSnapCount=0;
  if((wEncryptSize%sizeof(DWORD))!=0)
  {
    wSnapCount=sizeof(DWORD)-wEncryptSize%sizeof(DWORD);
    ZeroMemory(cbDataBuffer+sizeof(TCP_Info)+wEncryptSize,wSnapCount);
  }

  //ӳ���ֽ�
  BYTE cbCheckCode=0;
  for(WORD i=sizeof(TCP_Info); i<wDataSize; i++)
  {
    cbCheckCode+=cbDataBuffer[i];
    cbDataBuffer[i]=MapSendByte(cbDataBuffer[i]);
  }

  //��д��Ϣͷ
  TCP_Head * pHead=(TCP_Head *)cbDataBuffer;
  pHead->TCPInfo.cbDataKind=DK_MAPPED;
  pHead->TCPInfo.wPacketSize=wDataSize;
  pHead->TCPInfo.cbCheckCode=~cbCheckCode+1;

  //������Կ
  DWORD dwXorKey=m_dwSendXorKey;
  if(m_dwSendPacketCount==0)
  {
    //�������
    GUID Guid;
    CoCreateGuid(&Guid);
    dwXorKey=GetTickCount();
    dwXorKey^=Guid.Data1;
    dwXorKey^=Guid.Data2;
    dwXorKey^=Guid.Data3;
    dwXorKey^=*((DWORD *)Guid.Data4);

    //ӳ������
    dwXorKey=SeedRandMap((WORD)dwXorKey);
    dwXorKey|=((DWORD)SeedRandMap((WORD)(dwXorKey>>16)))<<16;
    dwXorKey^=g_dwPacketKey;
    m_dwSendXorKey=dwXorKey;
    m_dwRecvXorKey=dwXorKey;
  }

  //��������
  WORD * pwSeed=(WORD *)(cbDataBuffer+sizeof(TCP_Info));
  DWORD * pdwXor=(DWORD *)(cbDataBuffer+sizeof(TCP_Info));
  WORD wEncrypCount=(wEncryptSize+wSnapCount)/sizeof(DWORD);
  for(int i=0; i<wEncrypCount; i++)
  {
    *pdwXor++^=dwXorKey;
    dwXorKey=SeedRandMap(*pwSeed++);
    dwXorKey|=((DWORD)SeedRandMap(*pwSeed++))<<16;
    dwXorKey^=g_dwPacketKey;
  }

  //������Կ
  if(m_dwSendPacketCount==0)
  {
    MoveMemory(cbDataBuffer+sizeof(TCP_Head)+sizeof(DWORD),cbDataBuffer+sizeof(TCP_Head),wDataSize);
    *((DWORD *)(cbDataBuffer+sizeof(TCP_Head)))=m_dwSendXorKey;
    pHead->TCPInfo.wPacketSize+=sizeof(DWORD);
    wDataSize+=sizeof(DWORD);
  }

  //���ñ���
  m_dwSendPacketCount++;
  m_dwSendXorKey=dwXorKey;

  return wDataSize;
}

//ӳ������
BOOL CTCPSocket::MappedBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
  //��������
  BYTE *pcbBuffer = pcbDataBuffer;
  BYTE cbCheckCode = 0;

  //ӳ������
  for(WORD i = sizeof(TCP_Info); i<wDataSize; i++)
  {
    cbCheckCode += pcbBuffer[i];
    pcbBuffer[i] = g_SendByteMap[pcbBuffer[i]];
  }

  //��������
  TCP_Info *pInfo = (TCP_Info*)pcbDataBuffer;
  pInfo->cbCheckCode = ~cbCheckCode + 1;
  pInfo->wPacketSize = wDataSize;
  pInfo->cbDataKind = DK_MAPPED;
  m_dwSendPacketCount++;
  return true;
}

//��ӳ������
BOOL CTCPSocket::UnMappedBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
  //��������
  BYTE* pcbBuffer = pcbDataBuffer;
  TCP_Info* pInfo = (TCP_Info*)pcbDataBuffer;

  //ӳ��
  if((pInfo->cbDataKind & DK_MAPPED) != 0)
  {
    BYTE cbCheckCode = pInfo->cbCheckCode;

    for(WORD i = sizeof(TCP_Info); i<wDataSize; i++)
    {
      cbCheckCode += g_RecvByteMap[pcbBuffer[i]];
      pcbBuffer[i] = g_RecvByteMap[pcbBuffer[i]];
    }
    //Ч��
    if(cbCheckCode != 0)
    {
      return false;
    }
  }
  return true;
}

//�����ȡ
LRESULT CTCPSocket::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
  //״̬Ч��
  if(m_dwSendPacketCount==0L)
  {
    return 0;
  }

  //��ȡ����
  INT iRetCode=recv(m_hSocket,(char *)m_cbRecvBuf+m_wRecvSize,sizeof(m_cbRecvBuf)-m_wRecvSize,0);

  //�ر��ж�
  if(iRetCode==SOCKET_ERROR)
  {
    CloseSocket(SHUT_REASON_EXCEPTION);
    return 1L;
  }

  //���ñ���
  m_wRecvSize+=iRetCode;
  m_dwRecvTickCount=GetTickCount()/1000L;

  //��������
  TCP_Head * pHead=(TCP_Head *)m_cbRecvBuf;
  BYTE cbDataBuffer[SOCKET_TCP_PACKET+sizeof(TCP_Head)] = {0};

  //���ݴ���
  while((m_wRecvSize>=sizeof(TCP_Head))&&(m_hSocket!=INVALID_SOCKET))
  {
    //�汾�ж�
    if(pHead->TCPInfo.cbDataKind!=DK_MAPPED)
    {
      CloseSocket(SHUT_REASON_EXCEPTION);
      return 1L;
    }

    //�����ж�
    if(pHead->TCPInfo.wPacketSize>(SOCKET_TCP_PACKET+sizeof(TCP_Head)))
    {
      CloseSocket(SHUT_REASON_EXCEPTION);
      return 1L;
    }

    //�����ж�
    WORD wPacketSize=pHead->TCPInfo.wPacketSize;
    if(m_wRecvSize<pHead->TCPInfo.wPacketSize)
    {
      return 1;
    }

    //���ñ���
    m_dwRecvPacketCount++;
    m_wRecvSize-=wPacketSize;

    //��ȡ����
    CopyMemory(cbDataBuffer,m_cbRecvBuf,wPacketSize);
    MoveMemory(m_cbRecvBuf,m_cbRecvBuf+wPacketSize,m_wRecvSize);

    //��������
    // WORD wRealySize=CrevasseBuffer(cbDataBuffer,wPacketSize);
    WORD wRealySize = wPacketSize;
    if(!UnMappedBuffer(cbDataBuffer, wPacketSize))
    {
      CloseSocket(SHUT_REASON_EXCEPTION);
      return 1L;
    }

    //�ر��ж�
    if((wRealySize==0)&&(m_hSocket==INVALID_SOCKET))
    {
      CloseSocket(SHUT_REASON_EXCEPTION);
      return 1L;
    }

    //��������
    WORD wDataSize=wRealySize-sizeof(TCP_Head);
    VOID * pDataBuffer=cbDataBuffer+sizeof(TCP_Head);
    TCP_Command Command=((TCP_Head *)cbDataBuffer)->CommandInfo;

    //�ں�����
    if(Command.wMainCmdID==MDM_KN_COMMAND)
    {
      switch(Command.wSubCmdID)
      {
        case SUB_KN_DETECT_SOCKET:    //������
        {
          //��Ӧ����
          SendData(MDM_KN_COMMAND,SUB_KN_DETECT_SOCKET);

          break;
        }
        case SUB_KN_SHUT_DOWN_SOCKET: //�ж�����
        {
          //�ж�����
          CloseSocket(SHUT_REASON_NORMAL);

          break;
        }
      }
    }

    //��������
    if((Command.wMainCmdID!=MDM_KN_COMMAND)&&(m_pITCPSocketSink!=NULL))
    {
      try
      {
        //��������
        bool bSuccess=m_pITCPSocketSink->OnEventTCPSocketRead(m_wSocketID,Command,pDataBuffer,wDataSize);

        //�ر��ж�
        if(bSuccess==false)
        {
          CloseSocket(SHUT_REASON_EXCEPTION);
          return 1L;
        }
      }
      catch(...)
      {
        //�ر�����
        CloseSocket(SHUT_REASON_EXCEPTION);
        return 1L;
      }
    }
  }

  return 1L;
}

//���緢��
LRESULT CTCPSocket::OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam)
{
  //�����ж�
  if((m_bNeedBuffer==true)&&(m_dwBufferData>0L))
  {
    //��������
    DWORD dwTotalCount=0;
    DWORD dwPacketSize=4096;

    //���ñ���
    m_dwSendTickCount=GetTickCount()/1000L;

    //��������
    while(dwTotalCount<m_dwBufferData)
    {
      //��������
      WORD wSendSize=(WORD)__min(dwPacketSize,m_dwBufferData-dwTotalCount);
      INT nSendCount=send(m_hSocket,(char *)m_pcbDataBuffer+dwTotalCount,wSendSize,0);

      //�����ж�
      if(nSendCount==SOCKET_ERROR)
      {
        //�����ж�
        if(WSAGetLastError()==WSAEWOULDBLOCK)
        {
          //���ñ���
          m_bNeedBuffer=false;
          m_dwBufferData-=dwTotalCount;

          //�ƶ��ڴ�
          if(m_dwBufferData>0L)
          {
            m_bNeedBuffer=true;
            MoveMemory(m_pcbDataBuffer,m_pcbDataBuffer+dwTotalCount,m_dwBufferData);
          }

          return 1L;
        }

        //�ر�����
        CloseSocket(SHUT_REASON_EXCEPTION);

        return 1L;
      }

      //���ñ���
      dwTotalCount+=nSendCount;
    }

    //���ñ���
    m_dwBufferData=0L;
    m_bNeedBuffer=false;
  }

  return 1L;
}

//����ر�
LRESULT CTCPSocket::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
  //�ر�����
  CloseSocket(SHUT_REASON_REMOTE);

  return 1L;
}

//��������
LRESULT CTCPSocket::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
  //��ȡ����
  INT nErrorCode=WSAGETSELECTERROR(lParam);

  //�ж�״̬
  if(nErrorCode==0)
  {

#ifndef _DEBUG

    //����ʱ��
    SetTimer(IDI_DETEST_SOCKET,10000L,NULL);

#endif

    //����״̬
    m_cbSocketStatus=SOCKET_STATUS_CONNECT;
  }
  else
  {
    //�ر�����
    CloseSocket(SHUT_REASON_INSIDE);
  }

  //����֪ͨ
  ASSERT(m_pITCPSocketSink!=NULL);
  if(m_pITCPSocketSink!=NULL)
  {
    m_pITCPSocketSink->OnEventTCPSocketLink(m_wSocketID,nErrorCode);
  }

  return 1;
}

//ʱ����Ϣ
VOID CTCPSocket::OnTimer(UINT_PTR nIDEvent)
{
  //������
  if(nIDEvent==IDI_DETEST_SOCKET)
  {
    //��ֹ�ж�
    if(m_cbSocketStatus!=SOCKET_STATUS_CONNECT)
    {
      ASSERT(FALSE);
      KillTimer(IDI_DETEST_SOCKET);

      return;
    }

    //����ж�
    DWORD dwCompareTickCount=GetTickCount()/1000L-30L;
    bool bIntermit=((m_dwSendTickCount<dwCompareTickCount)&&(m_dwRecvTickCount<dwCompareTickCount));

    //��������
    if(bIntermit==true)
    {
      CloseSocket(SHUT_REASON_TIME_OUT);
    }

    return;
  }

  __super::OnTimer(nIDEvent);
}

//������Ϣ
LRESULT CTCPSocket::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
  switch(WSAGETSELECTEVENT(lParam))
  {
    case FD_READ:   //���ݶ�ȡ
    {
      return OnSocketNotifyRead(wParam,lParam);
    }
    case FD_WRITE:    //���緢��
    {
      return OnSocketNotifyWrite(wParam,lParam);
    }
    case FD_CLOSE:    //����ر�
    {
      return OnSocketNotifyClose(wParam,lParam);
    }
    case FD_CONNECT:  //��������
    {
      return OnSocketNotifyConnect(wParam,lParam);
    }
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(TCPSocket);

//////////////////////////////////////////////////////////////////////////
