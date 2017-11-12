#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView
{
  //��������
protected:
  LONGLONG            m_lGameTax;             //��Ϸ˰��
  LONGLONG            m_lScore[GAME_PLAYER];        //��Ϸ�÷�

  int                             m_nMutile;              //����
  int                             m_nBaseScore;           //������
  LONGLONG            m_lGivingScore[GAME_PLAYER];    //���׷�



  TCHAR             m_szUserName[GAME_PLAYER][LEN_ACCOUNTS];//�û�����
  CString                         m_strUserName[GAME_PLAYER];     //�û�����

  //��Դ����
protected:
  CPngImage           m_ImageBack;            //����ͼ��
  CPngImage           m_PngWinLose;           //�����Ϣ

  //��������
public:
  //���캯��
  CScoreView(void);
  //��������
  virtual ~CScoreView(void);

  //���ܺ���
public:
  //���û���
  void ResetScore();
  //����˰��
  void SetGameTax(LONG lGameTax);
  //���û���
  void SetBaseScore(int nMultle,int nBaseScore);
  //���û���
  void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore,LONGLONG lGivingScore);
  //�滭��ͼ
  VOID DrawScoreView(CDC * pDC,int xBegin,int yBegin,CWnd * Pwnd);
  //�滭�ַ�
  void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,int nWidth, int nWeight,UINT nFormat,CWnd * Pwnd);


};

//////////////////////////////////////////////////////////////////////////

#endif