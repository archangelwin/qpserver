#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CWnd
{
  //��������
protected:
  LONGLONG            m_lGameTax[GAME_PLAYER];      //��Ϸ˰��
  LONGLONG            m_lScore[GAME_PLAYER];        //��Ϸ�÷�
  TCHAR             m_szUserName[GAME_PLAYER][LEN_ACCOUNTS];//�û�����

  //��Դ����
protected:
  CBitImage           m_ImageBack;            //����ͼ��
  CBitImage           m_ImageWinLose;           //ʤ����־

  //��������
public:
  //���캯��
  CScoreView();
  //��������
  virtual ~CScoreView();

  //���ܺ���
public:
  //���û���
  void ResetScore();
  //����˰��
  void SetGameTax(WORD wChairID,LONGLONG lGameTax);
  //���û���
  void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore);

  //��Ϣ����
protected:
  //�ػ�����
  afx_msg void OnPaint();
  //������Ϣ
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //�����Ϣ
  afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

  afx_msg void OnMove(int x, int y);
  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif