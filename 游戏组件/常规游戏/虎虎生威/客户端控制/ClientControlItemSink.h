#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"


// CClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CClientControlItemSinkDlg:public CDialog
{
  DECLARE_DYNAMIC(CClientControlItemSinkDlg)

public:
  CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
  virtual ~CClientControlItemSinkDlg();

// �Ի�������
  enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

  DECLARE_MESSAGE_MAP()

public:
  //���¿���
  void UpdateControl(CMD_S_ControlReturns* pControlReturns);

protected:
  //��Ϣ
  void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);

public:
  //��ʼ��
  virtual BOOL OnInitDialog();
  //������ɫ
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  //ȡ������
  afx_msg void OnBnClickedButtonReset();
  //���ֿ���
  afx_msg void OnBnClickedButtonSyn();
  //��������
  afx_msg void OnBnClickedButtonOk();
  //ȡ���ر�
  afx_msg void OnBnClickedButtonCancel();
};
