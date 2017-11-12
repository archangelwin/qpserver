#include "StdAfx.h"
#include "GamePlaza.h"
#include "DlgWebRegister.h"
#include "MissionLogon.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////

//��Ļλ��
#define LAYERED_SIZE        5                 //�ֲ��С

//////////////////////////////////////////////////////////////////////////////////
#define LAST_VERIFY_NONE      0
#define LAST_VERIFY_LOCAL     1
#define LAST_VERIFY_RESULT_FAIL   2
#define LAST_VERIFY_RESULT_SUCESS 3

#define PERFORM_VERIFY_ACCOUNTS   0
#define PERFORM_VERIFY_NICKNAME   1
#define PERFORM_VERIFY_PASSWORD   2
#define PERFORM_VERIFY_TWICE    3


//////////////////////////////////////////////////////////////////////////////////
//��Ӧ�ؼ�
const TCHAR * szButtonCloseControlName3 = TEXT("ButtonClose");

//////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDlgWebRegister, CFGuiDialog)
  ON_WM_NCCREATE()
  ON_WM_NCDESTROY()
  //ON_WM_CREATE()


END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgWebRegister::CDlgWebRegister() : CFGuiDialog(IDD_DLG_WEB_REGISTER)
{
  m_enLogonType = en_Account_Logon;

  memset(&m_stLogonAccounts,0, sizeof(m_stLogonAccounts));
  memset(m_szWebRegisterLink,0, sizeof(m_szWebRegisterLink));
  memset(m_szQQLogonLink,0, sizeof(m_szQQLogonLink));


  return;
}

//��������
CDlgWebRegister::~CDlgWebRegister()
{
  DestroyWindow();
}

BOOL CDlgWebRegister::PreTranslateMessage(MSG * pMsg)
{
  //��������
  if((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
  {
    return TRUE;
  }

  return CFGuiDialog::PreTranslateMessage(pMsg);
}
int CDlgWebRegister::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if(__super::OnCreate(lpCreateStruct)==-1)
  {
    return -1;
  }

  //ModifyStyle(WS_CAPTION,0,0);
  //ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);
  //ModifyStyle(0, WS_MINIMIZEBOX);

  // ��ʼ��UI������
  m_PaintManager.Init(m_hWnd);
  return 1;
}
//���ú���
BOOL CDlgWebRegister::OnInitDialog()
{
  CFGuiDialog::OnInitDialog();

  //���ñ���
  SetWindowText(TEXT("�û�ע��"));

  //����·��
  TCHAR szDirectory[MAX_PATH]=TEXT("");
  CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

  //�����ļ�
  TCHAR szFileName[MAX_PATH]=TEXT("");
  _snwprintf(szFileName,sizeof(szFileName),TEXT("%s\\LobbyUrl.ini"),szDirectory);

  //��ȡ��ַ
  GetPrivateProfileString(TEXT("URL"), TEXT("WebRegister"), TEXT("http://www.7298.com/lobby/Register.aspx"), m_szWebRegisterLink, sizeof(TCHAR)*MAX_PATH, szFileName);
  GetPrivateProfileString(TEXT("URL"), TEXT("QQLogon"), TEXT("http://www.7298.com/lobby/QQLogin.aspx"), m_szQQLogonLink, sizeof(TCHAR)*MAX_PATH, szFileName);

  return TRUE;
}

//ȡ������
VOID CDlgWebRegister::OnCancel()
{
  //�رմ���
  EndDialog(IDCANCEL);

  //��ʾ��¼
  ASSERT(CMissionLogon::GetInstance()!=NULL);
  if(CMissionLogon::GetInstance()!=NULL)
  {
    CMissionLogon::GetInstance()->ShowLogon();
  }

  DestroyWindow();

  return;
}

//��Ϣ����
void CDlgWebRegister::Notify(TNotifyUI &  msg)
{
  //��ȡ����
  CControlUI * pControlUI = msg.pSender;

  //����¼�
  if(lstrcmp(msg.sType, TEXT("click")) == 0)
  {
    if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName3)==0)
    {
      return OnCancel();
    }

  }

}

LPCTSTR CDlgWebRegister::GetSkinFile()
{
  return TEXT("DlgWebRegister.xml");
}

//��ʼ�ؼ�
void CDlgWebRegister::InitControlUI()
{

}


//����QQ��½
void CDlgWebRegister::SetQQlogon(bool bCreat, CString strWebBrowser)
{
  //��½��Ϸ
  CTabLayoutUI* pTabLayout = (CTabLayoutUI*)m_PaintManager.FindControl(_T("LogonRegister"));
  pTabLayout->SelectItem(2);

  m_enLogonType = en_QQ_Logon;

  CUiWebBrowserEx * pActiveXUI = (CUiWebBrowserEx*)m_PaintManager.FindControl(strWebBrowser);

  if(pActiveXUI)
  {
    if(bCreat/* && !m_bLogonCreat*/)
    {
      static CWebBrowserEventHandlerEx wbehEx1;
      pActiveXUI->SetWebBrowserEventHandler(&wbehEx1);

      pActiveXUI->NavigateUrl(TEXT(""));
      pActiveXUI->Navigate2(TEXT(""));
      pActiveXUI->Navigate2(m_szQQLogonLink);
    }
  }
}


void CDlgWebRegister::SetRegisterlogon(bool bCreat, CString strWebBrowser)
{
  //CDialogBuilder builder;
  // CControlUI* pRoot = builder.Create(TEXT("Resource//DlgWebRegister.xml"), 0, this, &m_PaintManager, NULL);
  //m_PaintManager.AttachDialog(pRoot);
  //m_PaintManager.AddNotifier(this);


  //��½��Ϸ
  CTabLayoutUI* pTabLayout = (CTabLayoutUI*)m_PaintManager.FindControl(_T("LogonRegister"));
  pTabLayout->SelectItem(1);
  m_enLogonType = en_Register_Logon;

  CUiWebBrowserEx * pActiveXUI = (CUiWebBrowserEx*)m_PaintManager.FindControl(strWebBrowser);

  if(pActiveXUI)
  {
    if(bCreat/* && !m_bRegtistCreat*/)
    {

      pActiveXUI->NavigateUrl(TEXT(""));
      pActiveXUI->Navigate2(m_szWebRegisterLink);
    }
  }
}


VOID CDlgWebRegister::OnEventQQLogon(CString strAccounts, CString strPassword)
{
  _sntprintf(m_stLogonAccounts.szAccounts,CountArray(m_stLogonAccounts.szAccounts),TEXT("%s"),strAccounts.GetBuffer());
  _sntprintf(m_stLogonAccounts.szPassword,CountArray(m_stLogonAccounts.szPassword),TEXT("%s"),strPassword.GetBuffer());

  CString strm;
  strm.Format(TEXT("OnEventQQLogon %s,%s"),m_stLogonAccounts.szAccounts,m_stLogonAccounts.szPassword);
  AfxMessageBox(strm,0,0);

  //���ش���
  ShowWindow(SW_HIDE);

  //ִ��ע��
  CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
  if(pMissionLogon!=NULL)
  {
    pMissionLogon->PerformLogonMission(FALSE);
  }
}

VOID CDlgWebRegister::OnEventAccountLogon(CString strAccounts, CString strPassword)
{
  _sntprintf(m_stLogonAccounts.szAccounts,CountArray(m_stLogonAccounts.szAccounts),TEXT("%s"),strAccounts.GetBuffer());
  _sntprintf(m_stLogonAccounts.szPassword,CountArray(m_stLogonAccounts.szPassword),TEXT("%s"),strPassword.GetBuffer());

  CString strm;
  strm.Format(TEXT("OnEventAccountLogon %s,%s"),m_stLogonAccounts.szAccounts,m_stLogonAccounts.szPassword);
  AfxMessageBox(strm,0,0);

  //���ش���
  ShowWindow(SW_HIDE);

  //ִ��ע��
  CMissionLogon * pMissionLogon=CMissionLogon::GetInstance();
  if(pMissionLogon!=NULL)
  {
    pMissionLogon->PerformLogonMission(FALSE);
  }

}
//��������
WORD CDlgWebRegister::ConstructWebRegisterPacket(BYTE cbBuffer[], WORD wBufferSize, BYTE cbValidateFlags)
{
  //��������
  CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)cbBuffer;

  //ϵͳ��Ϣ
  pLogonAccounts->dwPlazaVersion=VERSION_PLAZA;

  //������ʶ
  CWHService::GetMachineIDEx(pLogonAccounts->szMachineID);
  //CWHService::GetMachineIDEx(pLogonAccounts->szMachineID2);
  //��¼��Ϣ
  lstrcpyn(pLogonAccounts->szPassword,m_stLogonAccounts.szPassword,CountArray(pLogonAccounts->szPassword));
  lstrcpyn(pLogonAccounts->szAccounts,m_stLogonAccounts.szAccounts,CountArray(pLogonAccounts->szAccounts));
  pLogonAccounts->cbValidateFlags=cbValidateFlags;

  CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();
  if(pGlobalWebLink != NULL)
  {
    pGlobalWebLink->GetSessionKey(pLogonAccounts->szSessionKey);
  }

  //��������
  CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
  tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
  lstrcpyn(pGlobalUserData->szPassword,pLogonAccounts->szPassword,CountArray(pGlobalUserData->szPassword));

  //�����С
  WORD wDataSize = sizeof(CMD_GP_LogonAccounts)-sizeof(pLogonAccounts->szPassPortID);
  wDataSize += CountStringBuffer(pLogonAccounts->szPassPortID);

  return wDataSize;
}
bool CDlgWebRegister::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
{
  if(uMsg == WM_MOUSEMOVE)
  {
    int n = 0;
  }
  if(uMsg == WM_KEYDOWN)
  {

  }


  return m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes);
}
LRESULT CDlgWebRegister::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  return CFGuiDialog::HandleMessage(uMsg,wParam,lParam);
}
CControlUI* CDlgWebRegister::CreateControl(LPCTSTR pstrClass)
{
  AfxMessageBox(pstrClass,0,0);
  if(_tcsicmp(pstrClass, _T("WebBrowserEx")) == 0)
  {
    CUiWebBrowserEx* pUI = new CUiWebBrowserEx();
    if(pUI)
    {
      pUI->SetIDispWebEvent(this);
    }
    return pUI;
  }
  return NULL;

}
BOOL CDlgWebRegister::OnNcCreate(LPCREATESTRUCT lpCS)
{
  Subclass();

  return CFGuiDialog::OnNcCreate(lpCS);
}

void CDlgWebRegister::OnNcDestroy()
{
  UnSubclass();

  CFGuiDialog::OnNcDestroy();
}
bool CDlgWebRegister::Subclass()
{
  ASSERT(::IsWindow(m_hWnd));
  ::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(this));
  m_OldWndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LPARAM)__WndProc);
  if(m_OldWndProc == NULL)
  {
    return false;
  }
  m_bSubclassed = true;

  return true;
}

bool CDlgWebRegister::UnSubclass()
{
  ASSERT(::IsWindow(m_hWnd));
  if(!m_bSubclassed)
  {
    return false;
  }
  ::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, 0);
  ::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LPARAM)m_OldWndProc);
  m_OldWndProc = ::DefWindowProc;
  m_bSubclassed = false;

  return true;
}

LRESULT CALLBACK CDlgWebRegister::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  CDlgWebRegister* pThis = reinterpret_cast<CDlgWebRegister*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

  if(pThis != NULL)
  {
    LRESULT lResult=TRUE;
    if(!pThis->MessageHandler(uMsg, wParam, lParam, lResult))
    {
      return ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
    }
    return lResult;
  }
  else
  {
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return 0L;
}
//////////////////////////////////////////////////////////////////////////////////
