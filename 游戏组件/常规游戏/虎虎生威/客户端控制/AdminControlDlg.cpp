// AdminControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "AdminControlDlg.h"
#include ".\admincontroldlg.h"


// CAdminControlDlg �Ի���

IMPLEMENT_DYNAMIC(CAdminControlDlg, IClientControlDlg)
CAdminControlDlg::CAdminControlDlg(CWnd* pParent /*=NULL*/)
  : IClientControlDlg(CAdminControlDlg::IDD, pParent)
{
}

CAdminControlDlg::~CAdminControlDlg()
{
}

void CAdminControlDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_USERINF_LIST, m_SkinListCtrl);
  DDX_Control(pDX, IDC_LIST_ALL_BET, m_ListTotalBet);
  DDX_Control(pDX, IDC_SEARCH_ID, m_EditSerach);
}


BEGIN_MESSAGE_MAP(CAdminControlDlg, IClientControlDlg)
  ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
  ON_BN_CLICKED(IDC_BUTTON_SYN, OnBnClickedButtonSyn)
  ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
  ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
  ON_BN_CLICKED(IDC_FRESHSTO,OnFreshStorage)
  ON_WM_CTLCOLOR()
  ON_BN_CLICKED(IDC_BT_DEDUCT2, OnBnClickedBtDeduct2)
  ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
  ON_EN_KILLFOCUS(IDC_SEARCH_ID, OnEnKillfocusSearchId)
  ON_EN_SETFOCUS(IDC_SEARCH_ID, OnEnSetfocusSearchId)
END_MESSAGE_MAP()


// CAdminControlDlg ��Ϣ�������

BOOL CAdminControlDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  SetWindowText(TEXT("���ƹ�����"));
  //������Ϣ
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));
  SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƾ�����"));
  SetDlgItemText(IDC_STATIC_AREA, TEXT("������ƣ�"));
  SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵����"));
  SetDlgItemText(IDC_STATIC_TEXT,TEXT("������Ӯ���Ʊ���Ϸ�����Ʋ������ȣ�"));
  SetDlgItemText(IDC_RADIO_BEN_40,TEXT("��"));
  SetDlgItemText(IDC_RADIO_BAO_30,TEXT("��"));
  SetDlgItemText(IDC_RADIO_AO_20,TEXT("����"));
  SetDlgItemText(IDC_RADIO_DA_10,TEXT("����"));
  SetDlgItemText(IDC_RADIO_BEN_5,TEXT("С��"));
  SetDlgItemText(IDC_RADIO_BAO_5,TEXT("С��"));
  SetDlgItemText(IDC_RADIO_AO_5,TEXT("С��"));
  SetDlgItemText(IDC_RADIO_DA_5,TEXT("С��"));
  SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
  SetDlgItemText(IDC_BUTTON_SYN,TEXT("����"));
  SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
  SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));
  m_lStorageStart=0;
  m_lStorageDeduct=0;

  ZeroMemory(m_AreaTotalBet,sizeof(m_AreaTotalBet));
  ZeroMemory(m_lUserTotalScore,sizeof(m_lUserTotalScore));
  //�б�
  m_SkinListCtrl.ModifyStyle(0,LVS_REPORT);

  DWORD dwStyle = GetWindowLong(m_SkinListCtrl.m_hWnd, GWL_STYLE);
  SetWindowLong(m_SkinListCtrl.m_hWnd, GWL_STYLE, dwStyle|LVS_REPORT);

  DWORD dwStyles = m_SkinListCtrl.GetExStyle();
  dwStyles &= ~LVS_EX_CHECKBOXES;

  m_SkinListCtrl.SetExtendedStyle(dwStyles|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
  m_SkinListCtrl.GetHeaderCtrl()->EnableWindow(FALSE);

  m_SkinListCtrl.InsertColumn(0,TEXT(""),LVCFMT_CENTER,0);
  m_SkinListCtrl.InsertColumn(1,TEXT("���"),LVCFMT_CENTER,38);
  m_SkinListCtrl.InsertColumn(2,TEXT("����ǳ�"),LVCFMT_CENTER,80);
  m_SkinListCtrl.InsertColumn(3,TEXT("���ID"),LVCFMT_CENTER,70);
  m_SkinListCtrl.InsertColumn(4,TEXT("��ҽ��"),LVCFMT_CENTER,70);
  m_SkinListCtrl.InsertColumn(5,TEXT("����Ӯ"),LVCFMT_CENTER,70);
  m_SkinListCtrl.InsertColumn(6,TEXT("��"),LVCFMT_CENTER,50);
  m_SkinListCtrl.InsertColumn(7,TEXT("��"),LVCFMT_CENTER,50);
  m_SkinListCtrl.InsertColumn(8,TEXT("����"),LVCFMT_CENTER,50);
  m_SkinListCtrl.InsertColumn(9,TEXT("����"),LVCFMT_CENTER,50);
  m_SkinListCtrl.InsertColumn(10,TEXT("С��"),LVCFMT_CENTER,50);
  m_SkinListCtrl.InsertColumn(11,TEXT("С��"),LVCFMT_CENTER,50);
  m_SkinListCtrl.InsertColumn(12,TEXT("С��"),LVCFMT_CENTER,50);
  m_SkinListCtrl.InsertColumn(13,TEXT("С��"),LVCFMT_CENTER,50);

  m_ListTotalBet.SetExtendedStyle(m_ListTotalBet.GetExtendedStyle()|LVS_EX_GRIDLINES);
  m_ListTotalBet.InsertColumn(0,TEXT(""),LVCFMT_CENTER,0);
  m_ListTotalBet.InsertColumn(1,TEXT(""),LVCFMT_CENTER,328);//38+80+70+70+70
  m_ListTotalBet.InsertColumn(2,TEXT(""),LVCFMT_CENTER,50);
  m_ListTotalBet.InsertColumn(3,TEXT(""),LVCFMT_CENTER,50);
  m_ListTotalBet.InsertColumn(4,TEXT(""),LVCFMT_CENTER,50);
  m_ListTotalBet.InsertColumn(5,TEXT(""),LVCFMT_CENTER,50);
  m_ListTotalBet.InsertColumn(6,TEXT(""),LVCFMT_CENTER,50);
  m_ListTotalBet.InsertColumn(7,TEXT(""),LVCFMT_CENTER,50);
  m_ListTotalBet.InsertColumn(8,TEXT(""),LVCFMT_CENTER,50);
  m_ListTotalBet.InsertColumn(9,TEXT(""),LVCFMT_CENTER,50);
  m_ListTotalBet.InsertItem(0, TEXT(""));
  m_ListTotalBet.SetItemText(0,1,TEXT("��ʵ�����עͳ��"));
  for(BYTE cbSubItem=2; cbSubItem <=9; cbSubItem++)
  {
    m_ListTotalBet.SetItemText(0, cbSubItem, TEXT("0"));
  }

  m_EditSerach.SetWindowText(TEXT("������������ID"));

  return TRUE;
}

//������ɫ
HBRUSH CAdminControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

  if(pWnd->GetDlgCtrlID()==IDC_STATIC_INFO  ||
     pWnd->GetDlgCtrlID()==IDC_STATIC_INFO2 ||
     pWnd->GetDlgCtrlID()==IDC_STATIC_SERACHINFO)
  {
    pDC->SetTextColor(RGB(255,10,10));
  }
  if(pWnd->GetDlgCtrlID() == IDC_SEARCH_ID)
  {
    CString str;
    m_EditSerach.GetWindowText(str);
    if(str.Compare(TEXT("������������ID")) == 0)
    {
      pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
    }
  }
  return hbr;
}

//���¿��
void CAdminControlDlg::OnFreshStorage()
{
  CString str;
  CMD_C_FreshStorage FreshStorage;
  m_lStorageStart=(SCORE)GetDlgItemInt(IDC_STORAGE);
  m_lStorageDeduct=(SCORE)GetDlgItemInt(IDC_DEDUCT);
  FreshStorage.lStorageStart=m_lStorageStart;
  FreshStorage.lStorageDeuct=m_lStorageDeduct;

  CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
  if(pGameFrameEngine!=NULL)
  {
    pGameFrameEngine->SendMessage(IDM_ADMIN_STORAGE,(WPARAM)&FreshStorage,0);
  }
  str=TEXT("����Ѹ��£�");

  SetDlgItemText(IDC_STATIC_INFO2,str);
}

//ȡ������
void CAdminControlDlg::OnBnClickedButtonReset()
{
  CMD_C_ControlApplication ControlApplication;
  ZeroMemory(&ControlApplication, sizeof(ControlApplication));
  ControlApplication.cbControlAppType = C_CA_CANCELS;
  CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
  if(pGameFrameEngine!=NULL)
  {
    pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
  }
}

//���ֿ���
void CAdminControlDlg::OnBnClickedButtonSyn()
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
void CAdminControlDlg::OnBnClickedButtonOk()
{
  CMD_C_ControlApplication ControlApplication;
  ZeroMemory(&ControlApplication, sizeof(ControlApplication));

  BYTE cbSelectedIndex = 0xff;
  if(((CButton*)GetDlgItem(IDC_RADIO_BEN_40))->GetCheck())
  {
    cbSelectedIndex = 0;
  }
  else if(((CButton*)GetDlgItem(IDC_RADIO_BAO_30))->GetCheck())
  {
    cbSelectedIndex = 1;
  }
  else if(((CButton*)GetDlgItem(IDC_RADIO_AO_20))->GetCheck())
  {
    cbSelectedIndex = 2;
  }
  else if(((CButton*)GetDlgItem(IDC_RADIO_DA_10))->GetCheck())
  {
    cbSelectedIndex = 3;
  }
  else if(((CButton*)GetDlgItem(IDC_RADIO_BEN_5))->GetCheck())
  {
    cbSelectedIndex = 4;
  }
  else if(((CButton*)GetDlgItem(IDC_RADIO_BAO_5))->GetCheck())
  {
    cbSelectedIndex = 5;
  }
  else if(((CButton*)GetDlgItem(IDC_RADIO_AO_5))->GetCheck())
  {
    cbSelectedIndex = 6;
  }
  else if(((CButton*)GetDlgItem(IDC_RADIO_DA_5))->GetCheck())
  {
    cbSelectedIndex = 7;
  }

  int nSelectTimes = ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();

  if(cbSelectedIndex != 0xff && nSelectTimes >= 0 && nSelectTimes != 0xffffffff)
  {
    ControlApplication.cbControlAppType = C_CA_SET;
    ControlApplication.cbControlArea = cbSelectedIndex;
    ControlApplication.cbControlTimes = static_cast<BYTE>(nSelectTimes + 1);
    CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
    if(pGameFrameEngine!=NULL)
    {
      pGameFrameEngine->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
    }
  }
  else
  {
    SetDlgItemText(IDC_STATIC_INFO,TEXT("��ѡ���ܿش����Լ��ܿ�����"));
  }

}

//ȡ���ر�
void CAdminControlDlg::OnBnClickedButtonCancel()
{
  ShowWindow(SW_HIDE);
}


//���¿���
void CAdminControlDlg::UpdateControl(CMD_S_ControlReturns* pControlReturns)
{
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
      PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
      myprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
      SetDlgItemText(IDC_STATIC_INFO,zsText);
      break;
    }
    case S_CR_SET_SUCCESS:
    {
      TCHAR zsText[256] = TEXT("");
      TCHAR zsTextTemp[256] = TEXT("");
      PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
      myprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
      SetDlgItemText(IDC_STATIC_INFO,zsText);
      break;
    }
    case S_CR_CANCEL_SUCCESS:
    {
      SetDlgItemText(IDC_STATIC_INFO,TEXT("ȡ�����óɹ���"));
      break;
    }
  }
}

//��Ϣ
void CAdminControlDlg::PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes)
{
  if(cbArea == 0xff)
  {
    myprintf(pText,cbCount,TEXT("��ʱ�޿��ơ�"));
    return;
  }
  TCHAR szDesc[32] = TEXT("");
  myprintf(pText,cbCount,TEXT("ʤ������"));
  if(cbArea == 0)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 1)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 2)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("����"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 3)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("����"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 4)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("С��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 5)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("С��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 6)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("С��"));
    lstrcat(pText,szDesc);
  }
  else if(cbArea == 7)
  {
    _sntprintf(szDesc,CountArray(szDesc),TEXT("С��"));
    lstrcat(pText,szDesc);
  }
  myprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��"), cbTimes);
  lstrcat(pText,szDesc);
}



void CAdminControlDlg::OnBnClickedBtDeduct2()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CString strBuf;
  CMD_C_StorageInfo StorageInfo;
  ZeroMemory(&StorageInfo,sizeof(StorageInfo));
  StorageInfo.lMaxStorage[0] = GetDlgItemInt(IDC_MAXSTORAGE1);
  StorageInfo.lMaxStorage[1] = GetDlgItemInt(IDC_MAXSTORAGE2);
  StorageInfo.wStorageMul[0] = GetDlgItemInt(IDC_StorageMul1);
  StorageInfo.wStorageMul[1] = GetDlgItemInt(IDC_StorageMul2);

  if(StorageInfo.lMaxStorage[0]<=0 || StorageInfo.lMaxStorage[0]>=StorageInfo.lMaxStorage[1] ||
     StorageInfo.wStorageMul[0]<=0 || StorageInfo.wStorageMul[0]>100 ||
     StorageInfo.wStorageMul[1]<=0 || StorageInfo.wStorageMul[1]>100)
  {
    strBuf += TEXT("�޸���������ִ��ʧ�ܣ�");
  }
  else
  {
    CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
    if(pGameFrameEngine!=NULL)
    {
      pGameFrameEngine->SendMessage(IDM_ADMIN_STORAGEINFO,(WPARAM)&StorageInfo,0);
    }

    strBuf += TEXT("ִ�гɹ�!");
  }
  SetDlgItemText(IDC_STATIC_INFO2,strBuf);
}

void CAdminControlDlg::OnBnClickedButton2()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  int numLine = 0;
  CString strlist;
  GetDlgItem(IDC_SEARCH_ID)->GetWindowText(strlist);
  if(strlist.Compare(TEXT("������������ID")) == 0)
  {
    SetDlgItemText(IDC_STATIC_SERACHINFO,TEXT("������������ID"));
    return;
  }
  numLine = m_SkinListCtrl.GetItemCount();
  int nItem = -1;
  for(int i = 0; i< numLine; i++)
  {
    if(strlist.Compare(m_SkinListCtrl.GetItemText(i,3)) == 0)
    {
      nItem=i;
      break;
    }

  }
  ////��������
  if(nItem == -1)
  {
    SetDlgItemText(IDC_STATIC_SERACHINFO,TEXT("����ҵ���Ҳ�����Ϸ�л��߸����û����ע!"));
  }
  else if(nItem == 0)
  {
    //m_SkinListCtrl.SetItemColor(nItem,RGB(255,0,0),RGB(255,255,255));
    m_SkinListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
    m_SkinListCtrl.SetSelectionMark(0);
    m_SkinListCtrl.SetFocus();
  }
  else
  {
    //�ȱ����һ������
    CString strBuf[13];
    for(int i=0; i<13; i++)
    {
      strBuf[i]=m_SkinListCtrl.GetItemText(0,i+1);
    }

    for(int j=0; j<13; j++)
    {
      m_SkinListCtrl.SetItemText(0,j+1,m_SkinListCtrl.GetItemText(nItem,j+1));
      m_SkinListCtrl.SetItemText(nItem,j+1,strBuf[j]);
    }
    //m_SkinListCtrl.SetItemColor(0,RGB(255,0,0),RGB(255,255,255));
    m_SkinListCtrl.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
    m_SkinListCtrl.SetSelectionMark(0);
    m_SkinListCtrl.SetFocus();
  }
}

void CAdminControlDlg::OnEnKillfocusSearchId()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CString str;
  m_EditSerach.GetWindowText(str);

  if(str.IsEmpty())
  {
    m_EditSerach.SetWindowText(TEXT("������������ID"));
  }
}

void CAdminControlDlg::OnEnSetfocusSearchId()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CString str;
  m_EditSerach.GetWindowText(str);
  if(str.Compare(TEXT("������������ID")) == 0)
  {
    m_EditSerach.SetWindowText(TEXT(""));
  }
}

//�����б�
void CAdminControlDlg::UpdateClistCtrl(LONGLONG lAllPlayBet[GAME_PLAYER][AREA_COUNT],BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair, WORD wViewChairID,IClientUserItem* pUserData)
{
  int nListIndex = cbViewIndex-1;

  //�������
  LVFINDINFO lvFindInfo;
  ZeroMemory(&lvFindInfo, sizeof(lvFindInfo));
  lvFindInfo.flags =  LVFI_STRING | LVFI_PARAM;
  lvFindInfo.psz = (LPCTSTR)pUserData->GetNickName();

  //���ҵ�2���
  int numLine = 0;
  CString strlist=lvFindInfo.psz ;
  numLine = m_SkinListCtrl.GetItemCount();
  int nItem = -1;
  for(int i = 0; i< numLine; i++)
  {
    if(strlist.Compare(m_SkinListCtrl.GetItemText(i,2)) == 0)
    {
      nItem=i;
      break;
    }

  }
  ////��������
  //nItem = m_SkinListCtrl.FindItem( &lvFindInfo );
  if(nItem == -1)
  {

    int line=m_SkinListCtrl.GetItemCount();
    if(line>=2)
    {
      line-=1;
    }
    CString strNum;
    strNum.Format(L"%d",line+1);
    nItem = m_SkinListCtrl.InsertItem(line, TEXT("0"));//������
    m_SkinListCtrl.SetItemText(line,1,strNum);
  }

  CString strScore;
  int index=0;
  for(int i=0; i<8; i++)
  {
    strScore.Format(TEXT("%I64d"), lAllPlayBet[wViewChairID][i]);
    index=i+6;
    m_SkinListCtrl.SetItemText(nItem, index, strScore);   //���û���
  }


  strScore.Format(TEXT("%s"),pUserData->GetNickName());
  m_SkinListCtrl.SetItemText(nItem, 2, strScore);   //�����ǳ�

  strScore.Format(TEXT("%d"),pUserData->GetGameID());
  m_SkinListCtrl.SetItemText(nItem, 3, strScore);   //����ID

  strScore.Format(TEXT("%d"),pUserData->GetUserScore());
  m_SkinListCtrl.SetItemText(nItem, 4, strScore);   //���ý��

  strScore.Format(TEXT("%d"),m_lUserTotalScore[wChair]);
  m_SkinListCtrl.SetItemText(nItem,5,strScore);   //��������Ӯ

  //���������ܽ��
  m_AreaTotalBet[cbViewIndex-1]+=lScoreCount;
  for(int i=0; i<8; i++)
  {
    strScore.Format(TEXT("%I64d"), m_AreaTotalBet[i]);
    index=i+2;
    m_ListTotalBet.SetItemText(0, index, strScore);   //��������
  }
}
//����б�
void CAdminControlDlg::ClearClist()
{
  m_SkinListCtrl.DeleteAllItems();
  for(BYTE cbSubItem=2; cbSubItem <=9; cbSubItem++)
  {
    m_ListTotalBet.SetItemText(0, cbSubItem, TEXT("0"));
  }
  ZeroMemory(m_AreaTotalBet,sizeof(m_AreaTotalBet));
}
//���ÿ����Ϣ
void CAdminControlDlg::SetStorageInfo(CMD_S_ADMIN_STORAGE_INFO *pStorageInof)
{
  CString strBuf;
  //��ǰ���
  strBuf.Format(TEXT("%I64d"),pStorageInof->lCurrentStorage);
  GetDlgItem(IDC_STORAGE)->SetWindowText(strBuf);
  //��ǰ˥��
  strBuf.Format(TEXT("%I64d"),pStorageInof->lCurrentDeduct);
  GetDlgItem(IDC_DEDUCT)->SetWindowText(strBuf);
  //�������1
  strBuf.Format(TEXT("%I64d"),pStorageInof->lMaxStorage[0]);
  GetDlgItem(IDC_MAXSTORAGE1)->SetWindowText(strBuf);
  //�·ָ���
  strBuf.Format(TEXT("%d"),pStorageInof->wStorageMul[0]);
  GetDlgItem(IDC_StorageMul1)->SetWindowText(strBuf);
  //�������2
  strBuf.Format(TEXT("%I64d"),pStorageInof->lMaxStorage[1]);
  GetDlgItem(IDC_MAXSTORAGE2)->SetWindowText(strBuf);
  //�·ָ���
  strBuf.Format(TEXT("%d"),pStorageInof->wStorageMul[1]);
  GetDlgItem(IDC_StorageMul2)->SetWindowText(strBuf);

  CopyMemory(m_lUserTotalScore,pStorageInof->lUserTotalScore,sizeof(m_lUserTotalScore));
}
//������ʼ���
void CAdminControlDlg::SetStorageInfo(LONGLONG lStartStorage)
{
  CString str;
  str.Format(TEXT("%I64d"),lStartStorage);
  SetDlgItemText(IDC_STARTSTORAGE,str);
}
