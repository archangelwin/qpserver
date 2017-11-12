#ifndef NETWORK_MANAGER_HEAD_FILE
#define NETWORK_MANAGER_HEAD_FILE

#pragma once

#include "WHSocketHead.h"

//////////////////////////////////////////////////////////////////////////

//�������
class CWHNetwork : public IWHNetwork
{
  //��������
public:
  //���캯��
  CWHNetwork();
  //��������
  virtual ~CWHNetwork();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID  Release()
  {
    return;
  }
  //�ӿڲ�ѯ
  virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);

  //���ܲ�ѯ
public:
  //ö�ٴ���
  virtual bool  EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyDescribe);
  //�������
  virtual BYTE  ProxyServerTesting(BYTE cbProxyType, const tagProxyServer & ProxyServer);
};

//////////////////////////////////////////////////////////////////////////

#endif