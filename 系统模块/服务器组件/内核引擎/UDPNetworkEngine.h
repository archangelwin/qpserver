#ifndef UDP_NETWORK_ENGINE_HEAD_FILE
#define UDP_NETWORK_ENGINE_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"

//��������
class CUDPNetworkEngine : public IUDPNetworkEngine
{


  //��������
public:
  //���캯��
  CUDPNetworkEngine();
  //��������
  virtual ~CUDPNetworkEngine();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID Release()
  {
    delete this;
  }
  //�ӿڲ�ѯ
  virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

  //���ýӿ�
public:
  //��������
  virtual bool  StartService();
  //ֹͣ����
  virtual bool  ConcludeService();

//��Ϣ�ӿ�
public:
  //���ö˿�
  virtual WORD GetServicePort();
  //��ǰ�˿�
  virtual WORD GetCurrentPort();

  //���ýӿ�
public:
  //���ýӿ�
  virtual bool SetUDPNetworkEngineEvent(IUnknownEx * pIUnknownEx);
  //���ò���
  virtual bool SetServiceParameter(WORD wServicePort, WORD wMaxConnect);

  //���ͽӿ�
public:
  //���ͺ���
  virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID);
  //���ͺ���
  virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

  //���ƽӿ�
public:
  //�ر�����
  virtual bool CloseSocket(DWORD dwSocketID);
  //���ùر�
  virtual bool ShutDownSocket(DWORD dwSocketID);
};
#endif