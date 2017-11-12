// DlgShowMessage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgShowMessage.h"
#include ".\dlgshowmessage.h"


// CDlgShowMessage �Ի���

CDlgShowMessage::CDlgShowMessage(CWnd* pParent /*=NULL*/)
{
  m_bMouseDrag=false;
  m_bIsShow=false;
  m_szStr=_T("");
}

CDlgShowMessage::~CDlgShowMessage()
{
}


BEGIN_MESSAGE_MAP(CDlgShowMessage, CWnd)
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
  ON_WM_LBUTTONDOWN()
  ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgShowMessage ��Ϣ�������

//����λͼ
bool CDlgShowMessage::LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans)
{
  m_ImageBack.SetLoadInfo(uBitmapID,hInstance);
  return CreateControlRgn(crTrans);
}

//����λͼ
bool CDlgShowMessage::LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans)
{
  m_ImageBack.SetLoadInfo(pszFileName);
  return CreateControlRgn(crTrans);
}

void CDlgShowMessage::IsMouseDrag(bool bFlags)
{
  m_bMouseDrag=bFlags;
}
//��������
bool CDlgShowMessage::CreateControlRgn(COLORREF crTrans)
{
  CImageHandle ImageHandle(&m_ImageBack);
  m_nYBGImage=m_ImageBack.GetHeight();
  m_nXBGImage=m_ImageBack.GetWidth();

  if(m_ImageBack.CreateImageRegion(m_DialogRgn,crTrans))
  {
    SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE);
    SetWindowRgn(HRGN(m_DialogRgn.m_hObject),TRUE);
    return true;
  }
  return false;
}

void CDlgShowMessage::OnPaint()
{
  CPaintDC dc(this); // device context for painting
  CRect ClientRect;
  GetClientRect(&ClientRect);
  CImageHandle ImageHandle(&m_ImageBack);
  if(m_ImageBack.IsNull()==false)
  {
    m_ImageBack.BitBlt(dc,0,0);
  }
  dc.SetBkMode(TRANSPARENT);
  DrawTextString(&dc,m_szStr,RGB(255,51,0),RGB(234,234,234));

  // TODO: �ڴ˴������Ϣ����������
  // ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CDlgShowMessage::OnEraseBkgnd(CDC* pDC)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
  Invalidate(FALSE);
  UpdateWindow();
  return TRUE;
}


void CDlgShowMessage::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

  CWnd::OnLButtonDown(nFlags, point);
  if(m_bMouseDrag)
  {
    PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
  }
}

void CDlgShowMessage::ShowWindowEx(bool bFlags,int nTimer,CString&str)
{
  if(m_bIsShow)
  {
    KillTimer(1);
    ShowWindow(SW_HIDE);
  }
  SetTimer(1,nTimer,NULL);
  m_szStr=str;
  m_bIsShow=true;
  UpdateWindow();
  ShowWindow(SW_SHOW);
}

void CDlgShowMessage::SetPrintStr(CString&str)
{
  m_szStr=str;
}


void CDlgShowMessage::SetTextRect(CRect&rect)
{
  m_TextRect=rect;
}

void CDlgShowMessage::DrawTextString(CDC * pDC, CString&szString, COLORREF crText, COLORREF crFrame)
{
  //��������
  int nXExcursion[8]= {1,1,1,0,-1,-1,-1,0};
  int nYExcursion[8]= {-1,0,1,1,1,0,-1,-1};

  CSize fontSize=pDC->GetTextExtent(szString);

  int y=m_TextRect.top+(m_TextRect.Height()-fontSize.cy)/2;
  int x=m_TextRect.left+(m_TextRect.Width()-fontSize.cx)/2;

  if(fontSize.cx>m_TextRect.Width())
  {
    //�滭�߿�
    pDC->SetTextColor(crFrame);
    for(int i=0; i<CountArray(nXExcursion); i++)
    {
      pDC->TextOut(10+nXExcursion[i],y+nYExcursion[i],szString,szString.GetLength());
    }

    //�滭����
    pDC->SetTextColor(crText);
    pDC->TextOut(10,y,szString,szString.GetLength());
  }
  else
  {
    pDC->SetTextColor(crFrame);
    for(int i=0; i<CountArray(nXExcursion); i++)
    {
      pDC->TextOut(x+nXExcursion[i],y+nYExcursion[i],szString,szString.GetLength());
    }
    //�滭����
    pDC->SetTextColor(crText);
    pDC->TextOut(x,y,szString,szString.GetLength());
  }
  return;
}

//��ȡ���
int CDlgShowMessage::GetWidth()
{
  return m_nXBGImage;
}
//��ȡ�߶�
int CDlgShowMessage::GetHight()
{
  return m_nYBGImage;
}

//����λ��
bool CDlgShowMessage::SetWndPos(CPoint&point)
{
  if(m_hWnd)
  {
    if(point==m_WndPos)
    {
      return true;
    }
    else
    {
      m_WndPos.SetPoint(point.x,point.y);
      CRect rect;
      GetParent()->GetWindowRect(&rect);
      SetWindowPos(&wndTop,rect.left+m_WndPos.x,rect.top+m_WndPos.y,0,0,SWP_NOSIZE);
    }

  }
  return true;
}


//��ȡλ��
bool CDlgShowMessage::GetWndPos(CPoint&point)
{
  m_WndPos.SetPoint(point.x,point.y);
  return true;
}


void CDlgShowMessage::OnTimer(UINT nIDEvent)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
  ShowWindow(SW_HIDE);
  KillTimer(1);
  m_bIsShow=false;
  CWnd::OnTimer(nIDEvent);
}
