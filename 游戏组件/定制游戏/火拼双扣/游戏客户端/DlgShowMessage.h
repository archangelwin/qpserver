#pragma once

#include "Stdafx.h"
#include "resource.h"
// CDlgShowMessage �Ի���

class CDlgShowMessage : public CWnd
{

public:
  CDlgShowMessage(CWnd* pParent = NULL);   // ��׼���캯��
  //����λͼ
  bool LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans);
  //����λͼ
  bool LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans);
  //�����ק
  void IsMouseDrag(bool bFlags);
  //��ʾ����
  void ShowWindowEx(bool bFlags,int nTimer,CString&str);
  //�����ַ�
  void SetPrintStr(CString&str);
  //��ȡ���
  int GetWidth();
  //��ȡ�߶�
  int GetHight();
  //����λ��
  bool SetWndPos(CPoint&point);
  //��ȡλ��
  bool GetWndPos(CPoint&point);
  //����λ��
  void SetTextRect(CRect&rect);
  //��������
  void DrawTextString(CDC * pDC, CString&szString, COLORREF crText, COLORREF crFrame);

  //��������
  virtual ~CDlgShowMessage();
public:
  CRgn              m_DialogRgn;            //��������
  CSkinImage            m_ImageBack;            //����λͼ
  bool                            m_bMouseDrag;
  bool                            m_bIsShow;
  CString                         m_szStr;
  int                             m_nXBGImage;
  int                             m_nYBGImage;
  CRect                           m_TextRect;
  CPoint                          m_WndPos;
  //�ڲ�����
protected:
  //��������
  bool CreateControlRgn(COLORREF crTrans);

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg UINT OnNcHitTest(CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnTimer(UINT nIDEvent);
};
