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
  ON_BN_CLICKED(IDC_BUTTON_RESET_EX, OnBnClickedButtonReset)
  ON_BN_CLICKED(IDC_BUTTON_SYN_EX, OnBnClickedButtonSyn)
  ON_BN_CLICKED(IDC_BUTTON_OK_EX, OnBnClickedButtonOk)
  ON_BN_CLICKED(IDC_BUTTON_CANCEL_EX, OnBnClickedButtonCancel)
  ON_BN_CLICKED(IDC_CHECK_ZHUANG_EX, OnBnClickedCheckZhuang)
  ON_BN_CLICKED(IDC_CHECK_DONG_EX, OnBnClickedCheckDong)
  ON_BN_CLICKED(IDC_CHECK_NAN_EX, OnBnClickedCheckNan)
  ON_BN_CLICKED(IDC_CHECK_XI_EX, OnBnClickedCheckXi)
  ON_BN_CLICKED(IDC_CHECK_BEI_EX, OnBnClickedCheckBei)
END_MESSAGE_MAP()

// CClientControlItemSinkDlg ��Ϣ�������

//��ʼ��
BOOL CClientControlItemSinkDlg::OnInitDialog()
{
  IClientControlDlg::OnInitDialog();

  //������Ϣ
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES_EX))->AddString(TEXT("1"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES_EX))->AddString(TEXT("2"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES_EX))->AddString(TEXT("3"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES_EX))->AddString(TEXT("4"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES_EX))->AddString(TEXT("5"));
  SetDlgItemText(IDC_STATIC_TIMES_EX,TEXT("���ƾ�����"));
  SetDlgItemText(IDC_STATIC_AREA_EX, TEXT("������ƣ�"));
  SetDlgItemText(IDC_STATIC_NOTIC_EX,TEXT("����˵����"));
  SetDlgItemText(IDC_STATIC_TEXT_EX,TEXT("������Ӯ���Ʊ���Ϸ�����Ʋ������ȣ�"));
  SetDlgItemText(IDC_CHECK_ZHUANG_EX,TEXT("ׯ"));
  SetDlgItemText(IDC_CHECK_DONG_EX,TEXT("��"));
  SetDlgItemText(IDC_CHECK_NAN_EX,TEXT("��"));
  SetDlgItemText(IDC_CHECK_XI_EX,TEXT("��"));
  SetDlgItemText(IDC_CHECK_BEI_EX,TEXT("��"));
  SetDlgItemText(IDC_BUTTON_RESET_EX,TEXT("ȡ������"));
  SetDlgItemText(IDC_BUTTON_SYN_EX,TEXT("������Ϣ"));
  SetDlgItemText(IDC_BUTTON_OK_EX,TEXT("ִ��"));
  SetDlgItemText(IDC_BUTTON_CANCEL_EX,TEXT("ȡ��"));

  return TRUE;
}

//������ɫ
HBRUSH CClientControlItemSinkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = IClientControlDlg::OnCtlColor(pDC, pWnd, nCtlColor);

  if(pWnd->GetDlgCtrlID()==IDC_STATIC_INFO_EX)
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
  AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);

}

//���ֿ���
void CClientControlItemSinkDlg::OnBnClickedButtonSyn()
{
  CMD_C_ControlApplication ControlApplication;
  ZeroMemory(&ControlApplication, sizeof(ControlApplication));
  ControlApplication.cbControlAppType = C_CA_UPDATE;
  AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//��������
void CClientControlItemSinkDlg::OnBnClickedButtonOk()
{
  CMD_C_ControlApplication ControlApplication;
  ZeroMemory(&ControlApplication, sizeof(ControlApplication));

  BYTE cbSelectedIndex[MAX_INDEX] = {FALSE};
  bool bSeting = false;
  if(((CButton*)GetDlgItem(IDC_CHECK_ZHUANG_EX))->GetCheck())
  {
    bSeting = true;
    cbSelectedIndex[BANKER_INDEX] = TRUE;
  }
  if(((CButton*)GetDlgItem(IDC_CHECK_DONG_EX))->GetCheck())
  {
    bSeting = true;
    cbSelectedIndex[SHUN_MEN_INDEX] = TRUE;
  }
  if(((CButton*)GetDlgItem(IDC_CHECK_NAN_EX))->GetCheck())
  {
    bSeting = true;
    cbSelectedIndex[DUI_MEN_INDEX] = TRUE;
  }
  if(((CButton*)GetDlgItem(IDC_CHECK_XI_EX))->GetCheck())
  {
    bSeting = true;
    cbSelectedIndex[DAO_MEN_INDEX] = TRUE;
  }
  if(((CButton*)GetDlgItem(IDC_CHECK_BEI_EX))->GetCheck())
  {
    bSeting = true;
    cbSelectedIndex[HUAN_MEN_INDEX] = TRUE;
  }

  int nSelectTimes = ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES_EX))->GetCurSel();

  if(bSeting && nSelectTimes >= 0 && nSelectTimes != 0xffffffff)
  {
    ControlApplication.cbControlAppType = C_CA_SET;
    memcpy(ControlApplication.cbControlArea, cbSelectedIndex, sizeof(ControlApplication.cbControlArea));
    ControlApplication.cbControlTimes = static_cast<BYTE>(nSelectTimes + 1);
    AfxGetMainWnd()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
  }
  else
  {
    SetDlgItemText(IDC_STATIC_INFO_EX,TEXT("��ѡ���ܿش����Լ��ܿ�����"));
  }
}

//ȡ���ر�
void CClientControlItemSinkDlg::OnBnClickedButtonCancel()
{
  ShowWindow(SW_HIDE);
}

//���¿���
void __cdecl CClientControlItemSinkDlg::UpdateControl(CMD_S_ControlReturns* pControlReturns)
{
  switch(pControlReturns->cbReturnsType)
  {
    case S_CR_FAILURE:
    {
      SetDlgItemText(IDC_STATIC_INFO_EX,TEXT("����ʧ�ܣ�"));
      break;
    }
    case S_CR_UPDATE_SUCCES:
    {
      TCHAR zsText[256] = TEXT("");
      TCHAR zsTextTemp[256] = TEXT("");
      PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
      _snprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
      SetDlgItemText(IDC_STATIC_INFO_EX,zsText);
      break;
    }
    case S_CR_SET_SUCCESS:
    {
      TCHAR zsText[256] = TEXT("");
      TCHAR zsTextTemp[256] = TEXT("");
      PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
      _snprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
      SetDlgItemText(IDC_STATIC_INFO_EX,zsText);
      break;
    }
    case S_CR_CANCEL_SUCCESS:
    {
      SetDlgItemText(IDC_STATIC_INFO_EX,TEXT("ȡ�����óɹ���"));
      break;
    }
  }
}

//��Ϣ
void CClientControlItemSinkDlg::PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea[MAX_INDEX], BYTE cbTimes)
{
  if(cbArea[BANKER_INDEX] != TRUE
     && cbArea[SHUN_MEN_INDEX] != TRUE
     && cbArea[DUI_MEN_INDEX] != TRUE
     && cbArea[DAO_MEN_INDEX] != TRUE
     && cbArea[HUAN_MEN_INDEX] != TRUE)
  {
    _snprintf(pText,cbCount,TEXT("��ʱ�޿��ơ�"));
    return;
  }
  TCHAR szDesc[32] = TEXT("");
  _snprintf(pText,cbCount,TEXT("ʤ������"));
  if(cbArea[BANKER_INDEX] == TRUE)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("ׯ��"));
    lstrcat(pText,szDesc);
  }
  if(cbArea[SHUN_MEN_INDEX] == TRUE)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("����"));
    lstrcat(pText,szDesc);
  }
  if(cbArea[DUI_MEN_INDEX] == TRUE)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("�ϣ�"));
    lstrcat(pText,szDesc);
  }
  if(cbArea[DAO_MEN_INDEX] == TRUE)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("����"));
    lstrcat(pText,szDesc);
  }
  if(cbArea[HUAN_MEN_INDEX] == TRUE)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("����"));
    lstrcat(pText,szDesc);
  }
  _snprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��"), cbTimes);
  lstrcat(pText,szDesc);
}

//ׯ
void CClientControlItemSinkDlg::OnBnClickedCheckZhuang()
{
  ((CButton*)GetDlgItem(IDC_CHECK_DONG_EX))->SetCheck(FALSE);
  ((CButton*)GetDlgItem(IDC_CHECK_NAN_EX))->SetCheck(FALSE);
  ((CButton*)GetDlgItem(IDC_CHECK_XI_EX))->SetCheck(FALSE);
  ((CButton*)GetDlgItem(IDC_CHECK_BEI_EX))->SetCheck(FALSE);
}

//��
void CClientControlItemSinkDlg::OnBnClickedCheckDong()
{
  ((CButton*)GetDlgItem(IDC_CHECK_ZHUANG_EX))->SetCheck(FALSE);
}

//��
void CClientControlItemSinkDlg::OnBnClickedCheckNan()
{
  ((CButton*)GetDlgItem(IDC_CHECK_ZHUANG_EX))->SetCheck(FALSE);
}

//��
void CClientControlItemSinkDlg::OnBnClickedCheckXi()
{
  ((CButton*)GetDlgItem(IDC_CHECK_ZHUANG_EX))->SetCheck(FALSE);
}

//��
void CClientControlItemSinkDlg::OnBnClickedCheckBei()
{
  ((CButton*)GetDlgItem(IDC_CHECK_ZHUANG_EX))->SetCheck(FALSE);
}
