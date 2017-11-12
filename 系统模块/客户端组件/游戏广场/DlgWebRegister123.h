#ifndef DLG_WEB_REGISTER_HEAD_FILE
#define DLG_WEB_REGISTER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "UiWebBrowserEx.h"

//////////////////////////////////////////////////////////////////////////////////
//ѡ��״̬ QQLogonWeb
enum enum_Logon_Type
{
  en_Account_Logon,
  en_QQ_Logon,
  en_Register_Logon
};

//�ʺŵ�¼
struct stLogonAccounts
{
  //ϵͳ��Ϣ
  DWORD             dwClientAddr;           //�ͻ���IP

  //��¼��Ϣ
  TCHAR             szPassword[LEN_MD5];        //��¼����
  TCHAR             szAccounts[LEN_ACCOUNTS];     //��¼�ʺ�
};
//////////////////////////////////////////////////////////////////////////////////////
//ע�ᴰ��
class CDlgWebRegister : public CFGuiDialog ,public IDispWebEvent
{
public:
  enum_Logon_Type       m_enLogonType;

  stLogonAccounts       m_stLogonAccounts;      //��½��Ϣ


  TCHAR           m_szWebRegisterLink[MAX_PATH];
  TCHAR           m_szQQLogonLink[MAX_PATH];
private:
  WNDPROC           m_OldWndProc;
  bool            m_bSubclassed;
  //��������
public:
  //���캯��
  CDlgWebRegister();
  //��������
  virtual ~CDlgWebRegister();
  //���غ���
protected:
  //��Ϣ����
  virtual BOOL PreTranslateMessage(MSG * pMsg);
  //���ú���
  virtual BOOL OnInitDialog();
  //ȡ������
  virtual VOID OnCancel();
  //���ܺ���
public:
  //QQ��½
  void SetQQlogon(bool bCreat, CString strWebBrowser);
  //QQ��½
  void SetRegisterlogon(bool bCreat, CString strWebBrowser);
  WORD ConstructWebRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags);

  //���غ���
public:
  //Ƥ������
  virtual LPCTSTR GetSkinFile();
  //��ʼ�ؼ�
  virtual void InitControlUI();
  //��Ϣ����
  virtual void Notify(TNotifyUI &  msg);

  virtual VOID OnEventWebRight(CString WebRight) {}
  virtual bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
  //������Ϣ
  virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
  //QQ��½
  virtual VOID OnEventQQLogon(CString strAccounts, CString strPassword);

  //��½����
  virtual VOID OnEventAccountLogon(CString strAccounts, CString strPassword);
private:
  bool Subclass();
  bool UnSubclass();
  static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  virtual CControlUI* CreateControl(LPCTSTR pstrClass);
  afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCS);
  afx_msg void OnNcDestroy();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif