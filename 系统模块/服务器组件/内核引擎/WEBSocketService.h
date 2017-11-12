#ifndef WEB_SOCKET_SERVICE_HEAD_FILE
#define WEB_SOCKET_SERVICE_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"

//�����¼�
class CWEBSocketEvent : public IWEBSocketEvent
{
  //��������
public:
  //���캯��
  CWEBSocketEvent() {};
  //��������
  virtual ~CWEBSocketEvent() {};

  //״̬�ӿ�
public:
  //������Ϣ
  virtual bool OnEventWEBSocketLink(WORD wServiceID, WORD wRequestID, INT nErrorCode)
  {
    return true;
  };
  //�ر���Ϣ
  virtual bool OnEventWEBSocketShut(WORD wServiceID, WORD wRequestID, BYTE cbShutReason)
  {
    return true;
  };

  //���ݽӿ�
public:
  //���ݰ���
  virtual bool OnEventWEBSocketMain(WORD wServiceID, WORD wRequestID, VOID * pcbMailData, WORD wStreamSize)
  {
    return true;
  };
  //���ݰ�ͷ
  virtual bool OnEventWEBSocketHead(WORD wServiceID, WORD wRequestID, VOID * pcbHeadData, WORD wHeadSize, INT nStatusCode)
  {
    return true;
  };
};

//WEB����
class CWEBSocketService : public IWEBSocketService
{

//��������
public:
  //���캯��
  CWEBSocketService();
  //��������
  virtual ~CWEBSocketService();

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

//���ýӿ�
public:
  //���ú���
  virtual bool SetServiceID(WORD wServiceID);
  //���ýӿ�
  virtual bool SetWEBSocketEvent(IUnknownEx * pIUnknownEx);

//���ܽӿ�
public:
  //�ر�����
  virtual bool CloseSocket(BYTE cbShutReason);
  //���Ӳ���
  virtual bool ConnectServer(LPCTSTR pszURL, WORD wPort);
  //��������
  virtual bool SendRequestData(VOID * pData, WORD wDataSize);
};
#endif