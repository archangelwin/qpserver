#ifndef GOLD_VIEW_HEAD_FILE
#define GOLD_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CGoldView
{
  //��������
public:
  LONGLONG              m_lGold;            //������Ŀ
  INT                 m_nGoldIndex;         //
  CDFontEx              m_DFontEx;              //����

  //��������
protected:
  static bool             m_bInit;            //��ʼ��־
  static CBitImage          m_ImageGold;          //����ͼƬ

  //��������
public:
  //���캯��
  CGoldView();
  //��������
  virtual ~CGoldView();

  //���ܺ���
public:
  //��������
  void OnCreate(CWnd* pWnd);
  //���ó���
  void SetGold(LONGLONG lGold);
  //��ȡ����
  LONGLONG GetGold()
  {
    return m_lGold;
  };
  //�滭����
  void DrawGoldView(CDC * pDC, int nXPos, int nYPos, COLORREF crColor);
  //���û滭��������
  VOID SetGoldIndex(INT nIndex);

  //�ڲ�����
private:
  //���Ա��ַ�
  LPCTSTR GetGlodString(LONGLONG lGold, TCHAR szString[]);
};

//////////////////////////////////////////////////////////////////////////

#endif