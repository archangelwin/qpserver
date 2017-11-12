// DlgPlayVoice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgPlayVoice.h"
#include "Windows.h"
#include ".\dlgplayvoice.h"

// CDlgPlayVoice �Ի���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgPlayVoice, CSkinDialogEx)
  ON_CBN_SELCHANGE(IDC_COMBO1,OnSelectChange)
  ON_NOTIFY(NM_DBLCLK, IDC_LIST_VOICE, OnNMDblclkListVoice)
  ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////


//���캯��
CDlgPlayVoice::CDlgPlayVoice() : CSkinDialogEx(IDD_DLG_PLAYVOICE)
{
  m_nCurSelect=0;
  m_bTimerRun=false;
  return;
}

//��������
CDlgPlayVoice::~CDlgPlayVoice()
{
  CDirectSound*pDirectSound=NULL;
  while(m_DSObjectList.GetSize()>0)
  {
    pDirectSound=m_DSObjectList.RemoveHead();
    pDirectSound->Stop();
    delete pDirectSound;
  }
}

bool CDlgPlayVoice::GetWndState()
{
  return m_bIsShow;
}

bool CDlgPlayVoice::InitVoiceList()
{
  m_PlayList[0][0]="�����";
  m_PlayList[0][1]="�������";
  m_PlayList[0][2]="ûը������";
  m_PlayList[0][3]="����";
  m_PlayList[0][4]="ȫ������";
  m_PlayList[0][5]="ȫ��";
  m_PlayList[0][6]="ȫɢ";
  m_PlayList[0][7]="û�Բ���ù";
  m_PlayList[0][8]="�Զ�ըɢ��";
  m_PlayList[0][9]="����Ҫʲô�ư���������";
  m_PlayList[0][10]="û��˼û��˼";
  m_PlayList[0][11]="˯���ˣ��㶯������";
  m_PlayList[0][12]="������ô�����ô�డ";
  m_PlayList[0][13]="ը����ը����Ұ�";
  m_PlayList[0][14]="����ը�ι�ȥ��";
  m_PlayList[0][15]="����������ը����";
  m_PlayList[0][16]="��ͷùͷ������";
  m_PlayList[0][17]="����򣬿��컯����Ӯ��";
  m_PlayList[0][18]="��ը��ը��ը������ը";
  m_PlayList[0][19]="���˴�����";
  m_PlayList[0][20]="����ץ����";
  m_PlayList[0][21]="����";
  m_PlayList[0][22]="���ģ��Ұ��㿴��";
  m_PlayList[0][23]="����,��������,������";
  m_PlayList[0][24]="��������ù";
  m_PlayList[0][25]="���㿨ô��";
  m_PlayList[0][26]="������ô�����ô�డ";
  m_PlayList[0][27]="���ƶ���ĳ����˶���������";
  m_PlayList[0][28]="����ô�ˣ���������";
  m_PlayList[0][29]="����һ";
  m_PlayList[0][30]="��û��˼��";

  m_PlayList[1][0]="���ϴ�����";
  m_PlayList[1][1]="����ը�ι�ȥ";
  m_PlayList[1][2]="����2";
  m_PlayList[1][3]="��ը��ը��ը������ը";
  m_PlayList[1][4]="�������1";
  m_PlayList[1][5]="��ͷùͷ������2";
  m_PlayList[1][6]="����򣬿��컯����Ӯ��";
  m_PlayList[1][7]="�ع���ǰ��󵶰�1";
  m_PlayList[1][8]="û�Բ���ù1";
  m_PlayList[1][9]="û�Բ���ù2";
  m_PlayList[1][10]="ûζ����ûζ��1";
  m_PlayList[1][11]="ûը��1";
  m_PlayList[1][12]="�Զ���ըɢ��1";
  m_PlayList[1][13]="�������ը����1";
  m_PlayList[1][14]="����";
  m_PlayList[1][15]="ȫ������2";
  m_PlayList[1][16]="ȫ��1";
  m_PlayList[1][17]="ȫɢ1";
  m_PlayList[1][18]="�����ʰ��㣬������2";
  m_PlayList[1][19]="��Ҫ��ʲô�ƣ�����1";
  m_PlayList[1][20]="�ưͳԻ����пི����1";
  m_PlayList[1][21]="ը����ը����ؼҰ�";
  m_PlayList[1][22]="վ�����￴���ֱ�����������";

  m_PlayList[2][0]="������שͷ����";
  m_PlayList[2][1]="�����";
  m_PlayList[2][2]="������";
  m_PlayList[2][3]="���������";
  m_PlayList[2][4]="�Լ�û��";
  m_PlayList[2][5]="�����Ұ�������";
  m_PlayList[2][6]="������ʹ��";
  m_PlayList[2][7]="û�Բ���ù";
  m_PlayList[2][8]="ûը����";
  m_PlayList[2][9]="����ʲô����";
  m_PlayList[2][10]="����";
  m_PlayList[2][11]="ȫ��";
  m_PlayList[2][12]="ȫɢ";
  m_PlayList[2][13]="�����򣬰��ѳ�";
  m_PlayList[2][14]="����һ";
  m_PlayList[2][15]="˫��˫";
  m_PlayList[2][16]="͵��";
  m_PlayList[2][17]="���ƿ�͸";
  m_PlayList[2][18]="���п��ǽ�������";
  m_PlayList[2][19]="ը����ը���ؼҰ�";
  m_PlayList[2][20]="ը�ĸ���˹����һ��";
  m_PlayList[2][21]="����û�о���";
  m_PlayList[2][22]="�Լ�����ˣ���Թ";

  m_nVoiceNo[0]=31;
  m_nVoiceNo[1]=23;
  m_nVoiceNo[2]=23;

  SetTimer(1,3000,0);
  int index=0;
  CString str;
  for(; index<m_nVoiceNo[m_nCurSelect]; index++)
  {
    str.Format("%d.%s",index+1,m_PlayList[m_nCurSelect][index]);
    m_ListCtrl.InsertItem(index,"1");
    m_ListCtrl.SetItemText(index,0,str);
  }
  ((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(m_nCurSelect);
  return true;
}



bool CDlgPlayVoice::ShowWindow(bool bFlags)
{
  bool bReturn=m_bIsShow;
  m_bIsShow=bFlags;
  int nCmdShow=SW_SHOW;
  __super::ShowWindow(bFlags?SW_SHOW:SW_HIDE);
  return bReturn;
}

//�ؼ���
void CDlgPlayVoice::DoDataExchange(CDataExchange * pDX)
{
  __super::DoDataExchange(pDX);
  DDX_Control(pDX, IDOK, m_btOK);
  DDX_Control(pDX,IDC_LIST_VOICE,m_ListCtrl);
}

//��ʼ������
BOOL CDlgPlayVoice::OnInitDialog()
{
  __super::OnInitDialog();

  //���ñ���
  SetWindowText(TEXT("��Ϸ����"));

  DWORD   dwStyle   =   m_ListCtrl.GetExtendedStyle();
  dwStyle=dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
  m_ListCtrl.ModifyStyle(WS_HSCROLL | WS_VSCROLL,0,0);

  m_ListCtrl.SetExtendedStyle(dwStyle);   //������չ���
  CRect ListRect;
  m_ListCtrl.SetTextBkColor(RGB(248,249,251));
  m_ListCtrl.SetTextColor(RGB(87,100,110));

  m_ListCtrl.GetClientRect(&ListRect);
  m_ListCtrl.InsertColumn(0,"�����б�",LVCFMT_CENTER,ListRect.Width()-17);
  return TRUE;
}

//ȷ����Ϣ
void CDlgPlayVoice::OnOK()
{
  static bool flags=true;
  if(flags)
  {
    //  SetTimer(1,1000,0);
    flags=false;
    m_bTimerRun=true;
  }

  POSITION   pos   =  m_ListCtrl.GetFirstSelectedItemPosition();
  if(pos   !=   NULL)
  {
    int nItem   =   m_ListCtrl.GetNextSelectedItem(pos);
    int nSel    = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
    AfxGetMainWnd()->PostMessage(WM_PLAYVOICE,nItem,nSel);
  }
}

void CDlgPlayVoice::OnCancel()
{
  __super::ShowWindow(SW_HIDE);
  m_bIsShow=false;
}

void CDlgPlayVoice::OnSelectChange()
{
  int k=((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();

  if(k!=m_nCurSelect)
  {
    int index=0;
    CString str;
    m_ListCtrl.DeleteAllItems();
    for(; index<m_nVoiceNo[k]; index++)
    {
      str.Format("%d.%s",index+1,m_PlayList[k][index]);
      m_ListCtrl.InsertItem(index,"1");
      m_ListCtrl.SetItemText(index,0,str);
    }
    m_nCurSelect=k;
    ((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(k);
  }

}


void CDlgPlayVoice::OnNMDblclkListVoice(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  OnOK();
  *pResult = 0;
}

void CDlgPlayVoice::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
  if(pWndOther!=this)
  {
    ShowWindow(SW_HIDE);
    m_bIsShow=false;
  }
  CSkinDialogEx::OnActivate(nState, pWndOther, bMinimized);
  // TODO: �ڴ˴������Ϣ����������
}

void CDlgPlayVoice::PlayVoice(CString&str)
{
  CDirectSound*pDirectSound=new CDirectSound();
  if(pDirectSound->Create(str,this))
  {
    m_DSObjectList.AddTail(pDirectSound);
    pDirectSound->Play();
  }
  else
  {
    delete pDirectSound;
    AfxMessageBox("��Ƶ�ļ�����ʧ��!");
  }
}


void CDlgPlayVoice::OnTimer(UINT nIDEvent)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

  CDirectSound*pDirectSound=NULL;
  while(m_DSObjectList.GetSize()>0)
  {
    pDirectSound=m_DSObjectList.GetHead();
    if(pDirectSound->IsEnd())
    {
      pDirectSound=m_DSObjectList.RemoveHead();
      pDirectSound->Stop();
      delete pDirectSound;
    }
    else
    {
      break;
    }
  }

  CSkinDialogEx::OnTimer(nIDEvent);
}


