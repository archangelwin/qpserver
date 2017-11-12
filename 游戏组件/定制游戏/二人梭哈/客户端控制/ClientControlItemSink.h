#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"


// CClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
  DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
  BYTE m_cbWinArea;       //Ӯ������
  BYTE m_cbExcuteTimes;     //ִ�д���

public:
  CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
  virtual ~CClientControlItemSinkDlg();

// �Ի�������
  enum { IDD = IDD_CLIENT_CONTROL };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

  DECLARE_MESSAGE_MAP()

public:
  //���¿���
  virtual void __cdecl OnAllowControl(bool bEnable);
  //������
  virtual bool __cdecl ReqResult(const void * pBuffer);
  virtual BOOL OnInitDialog();
  afx_msg void  OnExcute();

protected:
  virtual void OnCancel();

public:
//  afx_msg void OnTimer(UINT nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CClientControlItemSinkDlgEx : public IClientControlDlg
{
  DECLARE_DYNAMIC(CClientControlItemSinkDlgEx)

protected:
  DWORD       m_wTableID;       //����
  LONGLONG      m_lStorage;       //���

public:
  CClientControlItemSinkDlgEx(CWnd* pParent = NULL);   // ��׼���캯��
  virtual ~CClientControlItemSinkDlgEx();

// �Ի�������
  enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

  DECLARE_MESSAGE_MAP()

public:
  //���¿���
  virtual void __cdecl OnAllowControl(bool bEnable);
  //������
  virtual bool __cdecl ReqResult(const void * pBuffer);
  virtual BOOL OnInitDialog();
  afx_msg void  OnUpdateStorage();
  afx_msg void OnSetStorage();
  afx_msg void OnClearAllStorage();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

};

////////////////////////////////////////////////////////////////////////////////////////////////