#ifndef GAME_SERVER_MANAGER_HEAD_FILE
#define GAME_SERVER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"
//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#define VER_IAndroidService INTERFACE_VERSION(1,1)
static const GUID IID_IAndroidService= {0xf1c76760,0x4236,0x40ec,0xa5,0x58,0xfb,0x64,0xee,0x78,0xe2,0xd6};
#else
#define VER_IAndroidService INTERFACE_VERSION(1,1)
static const GUID IID_IAndroidService= {0xcf6e68b2,0x8455,0x4022,0x95,0xcc,0xad,0x4c,0xd1,0x3d,0x67,0xfe};
#endif

//�����˽ӿ�
interface IAndroidService :
public IUnknownEx
{
  //���ܽӿ�
public:
  //����������
  virtual VOID* __cdecl CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)=NULL;
};

DECLARE_MODULE_DYNAMIC(AndroidService);

//��Ϸ������������
//��Ϸ������������
class CGameServiceManager : public IGameServiceManager, public IGameServiceCustomRule
{
  //�ؼ�����
protected:
  CDlgCustomRule            m_DlgCustomRule;        //�Զ�����

  //��������
protected:
  tagGameServiceAttrib        m_GameServiceAttrib;      //��������

  //�������
  CGameServiceManagerHelper       m_AndroidServiceHelper;     //�����˷���
  HINSTANCE             m_hDllInstance;         //DLL ���
  //��������
public:
  //���캯��
  CGameServiceManager(void);
  //��������
  virtual ~CGameServiceManager(void);

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID Release()
  {
    return;
  }
  //�ӿڲ�ѯ
  virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);
  //�����ӿ�
public:
  //��������
  virtual VOID * CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer);
  //��������
  virtual VOID * CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer);
  //��������
  virtual VOID * CreateGameDataBaseEngineSink(REFGUID Guid, DWORD dwQueryVer);

  //�����ӿ�
public:
  //�������
  virtual bool GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib);
  //��������
  virtual bool RectifyParameter(tagGameServiceOption & GameServiceOption);

  //���ýӿ�
public:
  //Ĭ������
  virtual bool DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize);
  //��ȡ����
  virtual bool SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize);
  //��������
  virtual HWND CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize);
};

//////////////////////////////////////////////////////////////////////////

#endif
