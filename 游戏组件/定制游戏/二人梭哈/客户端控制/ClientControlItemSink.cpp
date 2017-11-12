// ClientControlItemSink.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ClientControlItemSink.h"

////////////////////////////////////////////////////////////////////////////////////////////
// CClientControlItemSinkDlg �Ի���
IMPLEMENT_DYNAMIC(CClientControlItemSinkDlg, IClientControlDlg)

CClientControlItemSinkDlg::CClientControlItemSinkDlg(CWnd* pParent /*=NULL*/)
  : IClientControlDlg(CClientControlItemSinkDlg::IDD, pParent)
{
  m_cbWinArea=0;        //Ӯ������
  m_cbExcuteTimes=0;      //ִ�д���
}

CClientControlItemSinkDlg::~CClientControlItemSinkDlg()
{
}

void CClientControlItemSinkDlg::DoDataExchange(CDataExchange* pDX)
{
  IClientControlDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientControlItemSinkDlg, IClientControlDlg)
  ON_BN_CLICKED(IDC_BUTTON_OK, OnExcute)
  ON_BN_CLICKED(IDC_BUTTON_CANCEL,OnCancel)
END_MESSAGE_MAP()

// CClientControlItemSinkDlg ��Ϣ�������

//��ʼ��
BOOL CClientControlItemSinkDlg::OnInitDialog()
{
  IClientControlDlg::OnInitDialog();

  SetDlgItemText(IDC_STATIC_AREA, TEXT("��Ӯ���ƣ�"));
  SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵��������ѡ�е��û���"));
  SetDlgItemText(IDC_STATIC_TEXT,TEXT("�û���Ӯ���Ʊ���Ϸ�����Ʋ�������!"));
  SetDlgItemText(IDC_CHECK_LONG,TEXT("�Լ�"));
  SetDlgItemText(IDC_CHECK_HE,TEXT("����"));
  SetDlgItemText(IDC_CHECK_HU,TEXT("2"));
  SetDlgItemText(IDC_CHECK_SMALL,TEXT("3"));

  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));
  ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(0);


  SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
  SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));

  return TRUE;
}

void CClientControlItemSinkDlg::OnExcute()
{
  CMD_C_AdminReq adminReq;
  BYTE cbIndex=0;
  int nCheckID=255;
  for(cbIndex=0; cbIndex<GAME_PLAYER; cbIndex++)
  {
    if(((CButton*)GetDlgItem(IDC_CHECK_LONG+cbIndex))->GetCheck())
    {
      nCheckID=cbIndex;
    }
  }

  int nSelect=0;

  if(nSelect>=0 && nCheckID != 255)
  {

    CGameFrameEngine *pFrameEngine = CGameFrameEngine::GetInstance();

    adminReq.cbReqType=RQ_SET_WIN_AREA;
    adminReq.cbExtendData[0]=pFrameEngine->SwitchViewChairID((BYTE)nCheckID+1);
    adminReq.cbExtendData[1]=pFrameEngine->SwitchViewChairID((BYTE)nCheckID);
    adminReq.cbExcuteTimes = GetDlgItemInt(IDC_COMBO_TIMES);
    m_cbWinArea=adminReq.cbExtendData[0];
    m_cbExcuteTimes = adminReq.cbExcuteTimes;

    CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
  }
  else
  {
    AfxMessageBox(TEXT("��ѡ���ܿش����Լ��ܿ�����!"));
    //OnRefresh();
  }
  // TODO: �ڴ���ӿؼ�֪ͨ����������
}


bool CClientControlItemSinkDlg::ReqResult(const void * pBuffer)
{

  const CMD_S_CommandResult*pResult=(CMD_S_CommandResult*)pBuffer;
  CString str;
  switch(pResult->cbAckType)
  {

    case ACK_SET_WIN_AREA:
    {
      if(pResult->cbResult==CR_ACCEPT)
      {
        str="��Ӯ���������Ѿ����ܣ�";
      }
      else
      {
        str.Format(TEXT("����ʧ��!"));
        m_cbExcuteTimes=0;
        m_cbWinArea=0;
      }
      break;
    }
    case ACK_RESET_CONTROL:
    {
      if(pResult->cbResult==CR_ACCEPT)
      {
        str="���������ѽ���!";
        for(int nIndex=0; nIndex<11; nIndex++)
        {
          ((CButton*)GetDlgItem(IDC_CHECK_LONG+nIndex))->SetCheck(0);
        }
        m_cbWinArea=0;
        m_cbExcuteTimes=0;
      }
      break;
    }
    case ACK_PRINT_SYN:
    {
      if(pResult->cbResult==CR_ACCEPT)
      {
        str="������ͬ�������ѽ���!";
        for(int nIndex=0; nIndex<11; nIndex++)
        {
          ((CButton*)GetDlgItem(IDC_CHECK_LONG+nIndex))->SetCheck(0);
        }

        m_cbWinArea=pResult->cbExtendData[0];
        m_cbExcuteTimes=pResult->cbExtendData[1];
        if(m_cbWinArea>0&&m_cbWinArea<=11)
        {
          ((CButton*)GetDlgItem(IDC_CHECK_LONG+m_cbWinArea-1))->SetCheck(1);
        }

        //SetTimer(10,2000,0);
      }
      else
      {
        str.Format(TEXT("����ʧ��!"));
      }
      break;
    }

    default:
      break;
  }

  SetWindowText(str);
  return true;
}


void CClientControlItemSinkDlg::OnCancel()
{
  // TODO: �ڴ����ר�ô����/����û���
  ShowWindow(SW_HIDE);
  //  CDialog::OnCancel();
}

void __cdecl CClientControlItemSinkDlg::OnAllowControl(bool bEnable)
{
  GetDlgItem(IDC_BUTTON_OK)->EnableWindow(bEnable);
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// CClientControlItemSinkDlg �Ի���
IMPLEMENT_DYNAMIC(CClientControlItemSinkDlgEx, IClientControlDlg)

CClientControlItemSinkDlgEx::CClientControlItemSinkDlgEx(CWnd* pParent /*=NULL*/)
  : IClientControlDlg(CClientControlItemSinkDlgEx::IDD, pParent)
{
  m_wTableID = 0;
  m_lStorage = 0;
}

CClientControlItemSinkDlgEx::~CClientControlItemSinkDlgEx()
{
}

void CClientControlItemSinkDlgEx::DoDataExchange(CDataExchange* pDX)
{
  IClientControlDlg::DoDataExchange(pDX);

  DDX_Text(pDX,IDC_EDIT_TABLE_ID,m_wTableID);
  DDX_Text(pDX,IDC_EDIT_STORAGE,m_lStorage);

  //DDV_MinMaxLongLong(pDX,m_wTableID, 1, USHRT_MAX);
  //DDV_MinMaxLongLong(pDX,m_lStorage, LLONG_MIN, LLONG_MAX);
}

BEGIN_MESSAGE_MAP(CClientControlItemSinkDlgEx, IClientControlDlg)
  ON_BN_CLICKED(IDC_BUTTON_UPDATE_STORAGE, OnUpdateStorage)
  ON_BN_CLICKED(IDC_BUTTON_SET_STORAGE,OnSetStorage)
  ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALLSTORAGE,OnClearAllStorage)
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//��ʼ��
BOOL CClientControlItemSinkDlgEx::OnInitDialog()
{
  IClientControlDlg::OnInitDialog();

  SetDlgItemText(IDC_STATIC_STORAGE,TEXT(""));

  return TRUE;
}


void CClientControlItemSinkDlgEx::OnUpdateStorage()
{
  //�������ֵ�ǿ�
  TCHAR szData[256] = {0};
  GetDlgItemText(IDC_EDIT_TABLE_ID, szData, sizeof(szData));
  if(0 == szData[0])
  {
    if(IDOK  == AfxMessageBox(TEXT("�������ѯ���ţ�")))
    {
      return;
    }
  }

  //��ȡ����
  UpdateData(TRUE); //�ӿؼ���ȡ���ݵ�����

  if(m_wTableID < 1 || m_wTableID > USHRT_MAX)
  {
    CString str;
    str.Format(TEXT("[����]���뷶ΧΪ1-%u"),USHRT_MAX);
    if(IDOK  == AfxMessageBox(str))
    {
      return;
    }
  }

  //������Ϣ
  CGameFrameEngine *pFrameEngine = CGameFrameEngine::GetInstance();

  CMD_C_AdminReqEx adminReq;
  ZeroMemory(&adminReq, sizeof(adminReq));

  adminReq.cbReqType=RQ_UPDATE_STORAGE;
  adminReq.wTableID = m_wTableID-1;

  CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN_EX,(WPARAM)&adminReq,0);

  return;
}

void CClientControlItemSinkDlgEx::OnSetStorage()
{
  //�������ֵ�ǿ�
  TCHAR szData[256] = {0};
  GetDlgItemText(IDC_EDIT_TABLE_ID, szData, sizeof(szData));
  if(0 == szData[0])
  {
    if(IDOK  == AfxMessageBox(TEXT("�������������ţ�")))
    {
      return;
    }
  }
  GetDlgItemText(IDC_EDIT_STORAGE, szData, sizeof(szData));
  if(0 == szData[0])
  {
    if(IDOK  == AfxMessageBox(TEXT("���������ÿ��ֵ��")))
    {
      return;
    }
  }

  //��ȡ����
  UpdateData(TRUE); //�ӿؼ���ȡ���ݵ�����

  if(m_wTableID < 1 || m_wTableID > USHRT_MAX)
  {
    CString str;
    str.Format(TEXT("[����]���뷶ΧΪ1-%u"),USHRT_MAX);
    if(IDOK  == AfxMessageBox(str))
    {
      return;
    }
  }

  if(m_lStorage < 0 || m_lStorage > LLONG_MAX)
  {
    CString str;
    str.Format(TEXT("[���]���뷶ΧΪ%I64d-%I64d"), 0, LLONG_MAX);
    if(IDOK  == AfxMessageBox(str))
    {
      return;
    }
  }

  //������Ϣ
  CGameFrameEngine *pFrameEngine = CGameFrameEngine::GetInstance();

  CMD_C_AdminReqEx adminReq;
  ZeroMemory(&adminReq, sizeof(adminReq));

  adminReq.cbReqType=RQ_SET_STORAGE;
  adminReq.wTableID = m_wTableID-1;
  adminReq.lStorage = m_lStorage;

  CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN_EX,(WPARAM)&adminReq,0);

  return;
}

void CClientControlItemSinkDlgEx::OnClearAllStorage()
{
  CMD_C_AdminReqEx adminReq;
  ZeroMemory(&adminReq, sizeof(adminReq));

  adminReq.cbReqType=RQ_CLEAR_ALLSTORAGE;
  CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN_EX,(WPARAM)&adminReq,0);
}

bool CClientControlItemSinkDlgEx::ReqResult(const void * pBuffer)
{

  const CMD_S_CommandResultEx*pResult=(CMD_S_CommandResultEx*)pBuffer;
  CString str;
  switch(pResult->cbAckType)
  {
    case ACK_UPDATE_STORAGE:
    {
      if(pResult->cbResult==CR_ACCEPT)
      {
        m_wTableID = pResult->wTableID+1;
        m_lStorage = pResult->lStorage;

        //����ֵд��ؼ�
        UpdateData(FALSE);

        str.Format(TEXT("����ѯ�ɹ���\r\n�ڡ�%u�������ֵ��%I64d"), m_wTableID, m_lStorage);
      }
      else
      {
        str.Format(TEXT("����ѯʧ�ܣ�"));
      }
      break;
    }
    case ACK_SET_STORAGE:
    {
      if(pResult->cbResult==CR_ACCEPT)
      {
        m_wTableID = pResult->wTableID+1;
        m_lStorage = pResult->lStorage;

        //����ֵд��ؼ�
        UpdateData(FALSE);

        str.Format(TEXT("������óɹ���\r\n�ڡ�%u�������ֵ��%I64d"), m_wTableID, m_lStorage);

      }
      else
      {
        str.Format(TEXT("�������ʧ�ܣ�"));
      }
      break;
    }
    case ACK_CLEAR_ALLSTORAGE:
    {
      if(pResult->cbResult==CR_ACCEPT)
      {
        str = TEXT("����������ӿ��ɹ���");
      }
      break;
    }

    default:
      break;
  }

  SetDlgItemText(IDC_STATIC_STORAGE, str);

  return true;
}

void __cdecl CClientControlItemSinkDlgEx::OnAllowControl(bool bEnable)
{
  return;
}

HBRUSH CClientControlItemSinkDlgEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = IClientControlDlg::OnCtlColor(pDC, pWnd, nCtlColor);

  if(pWnd->GetDlgCtrlID() == IDC_STATIC_STORAGE)
  {
    pDC->SetTextColor(RGB(255,0,0)); //������ɫ
    //pDC->SetBkColor(RGB(0, 0, 255)); //���屳��ɫ
  }

  return hbr;
}

/////////////////////////////////////////////////////////////////////////////////////////////