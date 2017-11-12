#ifndef UI_WEBBROWSER_MENU_EX
#define UI_WEBBROWSER_MENU_EX

#pragma once
#include "stdafx.h"

#define _FN_QQ_LOGON           (_T("fnQQLogon"))
#define _FN_ACCOUNT_LOGON        (_T("fnAccountLogon"))

#define DISP_QQ_LOGON           300
#define DISP_ACCOUNT_LOGON          400

//����ӿ�
interface IDispWebEvent
{
  //��½����
  virtual VOID OnEventQQLogon(CString strAccounts, CString strPassword)=NULL;
  //��½����
  virtual VOID OnEventAccountLogon(CString strAccounts, CString strPassword)=NULL;
  //ҳ����Ϣ
  virtual VOID OnEventWebRight(CString WebRight)=NULL;
};



class CUiWebBrowserEx : public CWebBrowserUI
{
  //����ӿ�
public:
  IDispWebEvent*        m_pIDispWebEvent;

public:
  CUiWebBrowserEx(void);
  ~CUiWebBrowserEx(void);

  //���ýӿ�
public:
  void SetIDispWebEvent(IDispWebEvent* pIDispWebEvent);
  void SendNotifyEx();


  //DIPS
public:
  //������3D�߿�
  virtual STDMETHODIMP GetExternal(IDispatch** ppDispatch);

  //ҳ�����
  virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid,  LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);

  virtual STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);
};


class CWebBrowserEventHandlerEx :
  public CWebBrowserEventHandler
{
public:
  CWebBrowserEventHandlerEx(void);
  ~CWebBrowserEventHandlerEx(void);
  //��Ҫ���������������
  virtual HRESULT STDMETHODCALLTYPE GetHostInfo(
    /* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
};


#endif

