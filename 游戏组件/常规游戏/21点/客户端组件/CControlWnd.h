#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CControlWnd : public CWnd
{
  //��������
protected:
  LONGLONG            m_lScoreMax;            //�����Ŀ
  LONGLONG            m_lScoreLess;           //������Ŀ

  //״̬����
public:
  WORD              m_wItemCount;           //������Ŀ
  LONGLONG            m_lScoreCurrent;          //��ǰ��Ŀ
  WORD              m_wCurrentItem;           //��ǰ����

  //�����ߴ�
protected:
  INT               m_ControlHead;            //ͷ�����
  INT               m_ControlTail;            //β�����

  //�ؼ�λ��
protected:
  CSize             m_SizeItem;             //����ߴ�
  CSize             m_SizeControl;            //�ؼ���С
  CPoint              m_BenchmarkPos;           //��׼λ��

  //�ؼ�����
protected:
  CSkinButton           m_btMaxScore;           //������
  CSkinButton           m_btLessScore;            //��С����
  CSkinButton           m_btSendScore;            //�ų�����

  //��Դ����
protected:
  CBitImage           m_ImageControlL;          //������Դ
  CBitImage           m_ImageControlM;          //������Դ
  CBitImage           m_ImageControlR;          //������Դ
  CBitImage           m_ImageArrow;           //�����ͷ
  CBitImage           m_ImageNumber;            //����

protected:
  CGameFrameViewGDI         *pGameClientView;
  //��������
public:
  //���캯��
  CControlWnd();
  //��������
  virtual ~CControlWnd();

  //���ܺ���
public:
  //����λ��
  VOID SetBenchmarkPos(INT nXPos, INT nYPos);
  //��ȡ��Ŀ
  LONGLONG GetScoreCurrent()
  {
    return m_lScoreCurrent;
  }
  //������Ϣ
  VOID SetScoreControl(LONGLONG lScoreMax, LONGLONG lScoreLess, LONGLONG lScoreCurrent);

  VOID SetGameClientHandler(CGameFrameViewGDI *pClientView)
  {
    pGameClientView = pClientView;
  }

  //�ڲ�����
protected:
  //��������
  VOID PlayGoldSound();
  //ת������
  WORD SwitchToIndex(CPoint MousePoint);

  //��ť��Ϣ
protected:
  //���ť
  VOID OnBnClickedScoreMax();
  //��С��ť
  VOID OnBnClickedScoreLess();
  //�ų���ť
  VOID OnBnClickedSendScore();

  //��Ϣӳ��
protected:
  //�滭��Ϣ
  VOID OnPaint();
  //λ����Ϣ
  VOID OnSize(UINT nType, INT cx, INT cy);
  //������Ϣ
  INT OnCreate(LPCREATESTRUCT lpCreateStruct);
  //�����Ϣ
  VOID OnLButtonUp(UINT nFlags, CPoint Point);
  //�����Ϣ
  VOID OnRButtonUp(UINT nFlags, CPoint Point);
  //�����Ϣ
  BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif