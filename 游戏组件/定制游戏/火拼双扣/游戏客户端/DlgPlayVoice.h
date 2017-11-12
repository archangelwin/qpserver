#pragma once

#include "Stdafx.h"
#include "resource.h"
#include "DirectSound.h"
#include "afxtempl.h"
#include <vector>
using namespace std;

#define WM_PLAYVOICE WM_USER+1000

// CDlgPlayVoice �Ի���
class CDlgPlayVoice : public CSkinDialogEx
{
  //��������
public:
  //�ؼ�����
public:
  CSkinButton           m_btOK;               //ȷ����ť
  //��������
public:
  //���캯��
  CDlgPlayVoice();
  //��������
  bool ShowWindow(bool bFlags);
  //��ȡ״̬
  bool GetWndState();
  //
  bool InitVoiceList();

  //
  void PlayVoice(CString&str);
  //��������
  virtual ~CDlgPlayVoice();
protected:
  CList<CDirectSound*,CDirectSound*> m_DSObjectList;
  bool m_bIsShow;
  CListCtrl m_ListCtrl;
  int     m_nVoiceNo[3];
  int     m_nCurSelect;
  bool    m_bTimerRun;
  //���غ���
public:
  CString m_PlayList[3][31];

protected:
  //�ؼ���
  virtual void DoDataExchange(CDataExchange * pDX);
  //��ʼ������
  virtual BOOL OnInitDialog();
  //ȷ����Ϣ
  virtual void OnOK();
  //
  virtual void OnCancel();
public:
  DECLARE_MESSAGE_MAP()

  //����ѡ��
  afx_msg void OnSelectChange();
  //ѡ������
  afx_msg void OnNMDblclkListVoice(NMHDR *pNMHDR, LRESULT *pResult);
  //���弤��
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
  afx_msg void OnTimer(UINT nIDEvent);
};

