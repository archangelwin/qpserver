#ifndef UILAUNCHER_HEAD_FILE
#define UILAUNCHER_HEAD_FILE

#pragma once
#include "stdafx.h"
#include "UiWebBrowserEx.h"

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


#define VALIDATE_COUNT        4                 //��֤����
//����ȼ�
#define PASSWORD_LEVEL_0      0                 //û�еȼ�
#define PASSWORD_LEVEL_1      1                 //û�еȼ�
#define PASSWORD_LEVEL_2      2                 //û�еȼ�
#define PASSWORD_LEVEL_3      3                 //û�еȼ�

//�������
class CDlgWebRegister : public CDialog, public INotifyUI, public IDialogBuilderCallback ,public IDispWebEvent
{
  //��������
protected:

  //���涨��
public:
  CPaintManagerUI   m_PaintManager;
  UINT            m_uSizeStatus;
  bool            m_bSubclassed;
  WNDPROC           m_OldWndProc;

  CSkinButton           m_btCancel;             //�رհ�ť
  //
public:
  enum_Logon_Type       m_enLogonType;

  stLogonAccounts       m_stLogonAccounts;      //��½��Ϣ

  bool            m_bLogonCreat;
  bool            m_bRegtistCreat;
  bool            m_bWebCreat;


  TCHAR           m_szWebRegisterLink[MAX_PATH];
  TCHAR           m_szQQLogonLink[MAX_PATH];

  //��������
public:
  //���캯��
  CDlgWebRegister();
  //��������
  virtual ~CDlgWebRegister();

  //���غ���
public:
  virtual LPCTSTR GetResFile()
  {
    return NULL;
  }
  virtual LPCTSTR GetResType()
  {
    return NULL;
  }
  virtual void OnInit() { }
  virtual void OnSized(UINT uCmd) { }

  virtual CControlUI* CreateControl(LPCTSTR pstrClass);
  virtual bool HitControl(CControlUI* pControl);
  virtual LRESULT HitTestNCA(HWND hWnd, POINT ptMouse);

  //���غ���
protected:
  //�ؼ���
  virtual VOID DoDataExchange(CDataExchange * pDX);
  //ȡ������
  VOID OnCancel();
  //duilib�¼�
  virtual bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
  //duilib�¼�
  virtual void Notify(TNotifyUI& msg);
  //��������
  virtual void SetCustomAttribute(LPCTSTR pszName,LPCTSTR pszValue) {};

public:
  //��ť�¼�
  bool ButtonEvent(TNotifyUI& msg);

  //QQ��½
  void SetQQlogon(bool bCreat, CString strWebBrowser);
  //QQ��½
  void SetRegisterlogon(bool bCreat, CString strWebBrowser);

  virtual VOID OnEventWebRight(CString WebRight) {}

  //QQ��½
  virtual VOID OnEventQQLogon(CString strAccounts, CString strPassword);

  //��½����
  virtual VOID OnEventAccountLogon(CString strAccounts, CString strPassword);

  WORD ConstructWebRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags);

private:
  bool Subclass();
  bool UnSubclass();
  static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
  afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCS);
  afx_msg void OnNcDestroy();
  afx_msg void OnNcPaint(void);
  afx_msg BOOL OnNcActivate(BOOL bActive);
  afx_msg void OnNcCalcSize(BOOL bCal, NCCALCSIZE_PARAMS* lpNCP);
  afx_msg LRESULT OnNcHitTest(CPoint ptMouse);

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

  afx_msg void OnSize(UINT nType, int cx, int cy);


  DECLARE_MESSAGE_MAP()
};

#endif
