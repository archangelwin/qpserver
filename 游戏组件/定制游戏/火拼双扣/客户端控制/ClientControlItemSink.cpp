// ClientControlItemSink.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ClientControlItemSink.h"
#include ".\clientcontrolitemsink.h"


// CClientControlItemSinkDlg �Ի���

IMPLEMENT_DYNAMIC(CClientControlItemSinkDlg, IClientControlDlg)

CClientControlItemSinkDlg::CClientControlItemSinkDlg(CWnd* pParent /*=NULL*/)
  : IClientControlDlg(CClientControlItemSinkDlg::IDD, pParent)
{
}

CClientControlItemSinkDlg::~CClientControlItemSinkDlg()
{
}

void CClientControlItemSinkDlg::DoDataExchange(CDataExchange* pDX)
{
  IClientControlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientControlItemSinkDlg, IClientControlDlg)
  ON_WM_CTLCOLOR()
  ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
  ON_BN_CLICKED(IDC_BUTTON_SYN, OnBnClickedButtonSyn)
  ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
  ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
  ON_BN_CLICKED(IDC_RADIO_WIN, OnBnClickedRadioWin)
  ON_BN_CLICKED(IDC_RADIO_LOSE, OnBnClickedRadioLose)
END_MESSAGE_MAP()

// CClientControlItemSinkDlg ��Ϣ�������

//��ʼ��
BOOL CClientControlItemSinkDlg::OnInitDialog()
{
  IClientControlDlg::OnInitDialog();

  //������Ϣ
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));
  SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƾ�����"));

  //SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵����"));
  //SetDlgItemText(IDC_STATIC_TEXT,TEXT("������Ӯ���Ʊ���Ϸ�����Ʋ������ȣ�"));

  SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
  SetDlgItemText(IDC_BUTTON_SYN,TEXT("������Ϣ"));
  SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
  SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));

  return TRUE;
}

//������ɫ
HBRUSH CClientControlItemSinkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = IClientControlDlg::OnCtlColor(pDC, pWnd, nCtlColor);

  if(pWnd->GetDlgCtrlID()==IDC_STATIC_INFO)
  {
    pDC->SetTextColor(RGB(255,10,10));
  }
  return hbr;
}

//ȡ������
void CClientControlItemSinkDlg::OnBnClickedButtonReset()
{
  CMD_C_ControlApplication ControlApplication;
  ZeroMemory(&ControlApplication, sizeof(ControlApplication));
  ControlApplication.cbControlAppType = C_CA_CANCELS;
  ReSetAdminWnd();
  CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
  if(pGameFrameEngine!=NULL)
  {
    pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
  }

}

//���ֿ���
void CClientControlItemSinkDlg::OnBnClickedButtonSyn()
{
  CMD_C_ControlApplication ControlApplication;
  ZeroMemory(&ControlApplication, sizeof(ControlApplication));
  ControlApplication.cbControlAppType = C_CA_UPDATE;
  CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
  if(pGameFrameEngine!=NULL)
  {
    pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
  }
}

//��������
void CClientControlItemSinkDlg::OnBnClickedButtonOk()
{

  BYTE cbIndex=0;
  bool bFlags=false;
  m_cbControlStyle=0;
  m_cbExcuteTimes=0;

  //���ƶԼ�
  if(((CButton*)GetDlgItem(IDC_RADIO_WIN))->GetCheck())
  {
    m_cbControlStyle=CS_BANKER_WIN;
    bFlags=true;
  }
  else
  {
    if(((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck())
    {
      m_cbControlStyle=CS_BANKER_LOSE;
      bFlags=true;
    }
  }
  m_cbExcuteTimes=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel()+1;
  //��ȡִ�д���
  if(m_cbExcuteTimes<=0)
  {
    bFlags=false;
  }
  if(bFlags) //������Ч
  {
    CMD_C_ControlApplication ControlApplication;
    ZeroMemory(&ControlApplication, sizeof(ControlApplication));

    ControlApplication.cbControlAppType = C_CA_SET;
    ControlApplication.cbControlStyle = m_cbControlStyle;
    ControlApplication.cbControlTimes = static_cast<BYTE>(m_cbExcuteTimes);
    CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
    if(pGameFrameEngine!=NULL)
    {
      pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
    }

  }
  else
  {
    AfxMessageBox(TEXT("��ѡ���ܿش����Լ��ܿط�ʽ!"));
    return;
  }

}

//ȡ���ر�
void CClientControlItemSinkDlg::OnBnClickedButtonCancel()
{
  ShowWindow(SW_HIDE);
}

//���¿���
void  CClientControlItemSinkDlg::UpdateControl(CMD_S_ControlReturns* pControlReturns)
{
  CString str=TEXT("");
  switch(pControlReturns->cbReturnsType)
  {
    case S_CR_FAILURE:
    {
      SetDlgItemText(IDC_STATIC_INFO,TEXT("����ʧ�ܣ�"));
      break;
    }
    case S_CR_UPDATE_SUCCES:
    {
      TCHAR zsText[256] = TEXT("");
      TCHAR zsTextTemp[256] = TEXT("");
      m_cbExcuteTimes=pControlReturns->cbControlTimes;
      m_cbControlStyle=pControlReturns->cbControlStyle;
      myprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
      SetDlgItemText(IDC_STATIC_INFO,zsText);
      ReSetAdminWnd();
      break;
    }
    case S_CR_SET_SUCCESS:
    {
      TCHAR zsText[256] = TEXT("");
      TCHAR zsTextTemp[256] = TEXT("");
      myprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
      SetDlgItemText(IDC_STATIC_INFO,zsText);


      switch(m_cbControlStyle)
      {
        case CS_BANKER_WIN:
        {
          str.Format(TEXT("�����ѽ��ܣ����Ʒ�ʽ���Լ�Ӯ��ִ�д�����%d"),m_cbExcuteTimes);
          break;
        }
        case CS_BANKER_LOSE:
        {
          str.Format(TEXT("�����ѽ��ܣ����Ʒ�ʽ���Լ��䣬ִ�д�����%d"),m_cbExcuteTimes);
          break;
        }
        default:
          break;
      }


      break;
    }
    case S_CR_CANCEL_SUCCESS:
    {
      SetDlgItemText(IDC_STATIC_INFO,TEXT("ȡ�����óɹ���"));
      break;
    }
  }

  SetWindowText(str);
}



//��Ϣ
void CClientControlItemSinkDlg::PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes)
{
  if(cbArea == 0xff)
  {
    myprintf(pText,cbCount,TEXT("��ʱ�޿��ơ�"));
    return;
  }
  TCHAR szDesc[32] = TEXT("");
  myprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��"), cbTimes);
  lstrcat(pText,szDesc);
}

void CClientControlItemSinkDlg::ReSetAdminWnd()
{

  if(m_cbControlStyle!=CS_BANKER_WIN&&m_cbControlStyle!=CS_BANKER_LOSE)
  {
    ((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(0);
    ((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(0);
  }
  else
  {
    ((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(m_cbControlStyle==CS_BANKER_WIN?1:0);
    ((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(m_cbControlStyle==CS_BANKER_LOSE?1:0);
  }

  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(m_cbExcuteTimes>0?m_cbExcuteTimes:-1);

}

bool CClientControlItemSinkDlg:: InitControl()
{
  return true;
}

void CClientControlItemSinkDlg::OnBnClickedRadioWin()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CClientControlItemSinkDlg::OnBnClickedRadioLose()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
}
