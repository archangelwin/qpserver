#pragma once
#include "Resource.h"
#include "../��Ϸ�ͻ���/ClientControl.h"
#include "afxcmn.h"
#include "afxwin.h"

// CAdminControlDlg �Ի���
#define IDM_ADMIN_COMMDN WM_USER+207

#ifndef _UNICODE
#define myprintf  _snprintf
#define mystrcpy  strcpy
#define mystrlen  strlen
#define myscanf   _snscanf
#define myLPSTR   LPCSTR
#else
#define myprintf  swprintf
#define mystrcpy  wcscpy
#define mystrlen  wcslen
#define myscanf   _snwscanf
#define myLPSTR   LPWSTR
#endif

class AFX_EXT_CLASS CAdminControlDlg :public IClientControlDlg
{
  DECLARE_DYNAMIC(CAdminControlDlg)

public:
  CAdminControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
  virtual ~CAdminControlDlg();
  LONGLONG            m_lStorageStart;            //�����ֵ
  LONGLONG            m_lStorageDeduct;           //���˥��
  LONGLONG            m_AreaTotalBet[AREA_COUNT];       //������ע
  LONGLONG            m_lUserTotalScore[GAME_PLAYER];     //�������Ӯ
  CListCtrl           m_SkinListCtrl;
  CListCtrl           m_ListTotalBet;
  // �Ի�������
  enum { IDD = IDD_DIALOG_SYSTEM };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

  DECLARE_MESSAGE_MAP()

public:
  //���¿���
  virtual void __cdecl UpdateControl(CMD_S_ControlReturns* pControlReturns);
  //�����б�
  virtual void UpdateClistCtrl(LONGLONG lAllPlayBet[GAME_PLAYER][AREA_COUNT],BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair, WORD wViewChairID,IClientUserItem* pUserData);
  //����б�
  virtual void ClearClist();
  //���ÿ����Ϣ
  virtual void SetStorageInfo(CMD_S_ADMIN_STORAGE_INFO *pStorageInof);
  //������ʼ���
  virtual void SetStorageInfo(LONGLONG lStartStorage);

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
  afx_msg void  OnFreshStorage();

  afx_msg void OnBnClickedBtDeduct2();

  afx_msg void OnBnClickedButton2();
  CEdit m_EditSerach;
  afx_msg void OnEnKillfocusSearchId();
  afx_msg void OnEnSetfocusSearchId();
};
