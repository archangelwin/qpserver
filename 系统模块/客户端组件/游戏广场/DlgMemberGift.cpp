#include "Stdafx.h"
#include "Resource.h"
#include "DlgMemberGift.h"
#include "GlobalUnits.h"

//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonTakeControlName = TEXT("ButtonTake");
const TCHAR* const szPageLayoutGiftControlName = TEXT("PageLayoutGift");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgMemberGift::CDlgMemberGift() : CFGuiDialog(IDD_DLG_POPUP)
{
  m_bMember = false;
  ZeroMemory(m_GiftSub,sizeof(m_GiftSub));
  m_wGiftSubCount = 0;
}

//��������
CDlgMemberGift::~CDlgMemberGift()
{
}

//��������
BOOL CDlgMemberGift::OnInitDialog()
{
  __super::OnInitDialog();

  //�޸���ʽ
  ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

  //���ñ���
  SetWindowText(TEXT("��Ա���"));

  //���д���
  CenterWindow(this);


  return TRUE;
}

//ȷ������
VOID CDlgMemberGift::OnOK()
{

  return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgMemberGift::OnCancel()
{
  return __super::OnCancel();
}

//��Ϣ����
void CDlgMemberGift::Notify(TNotifyUI &  msg)
{
  //��ȡ����
  CControlUI * pControlUI = msg.pSender;

  //����¼�
  if(lstrcmp(msg.sType, TEXT("click")) == 0)
  {
    //ȡǰ��λ
    if(lstrcmp(pControlUI->GetName(), szButtonCloseControlName)==0)
    {
      return OnCancel();
    }
    else if(lstrcmp(pControlUI->GetName(), szButtonTakeControlName)==0)
    {
      return OnOK();
    }
  }

  return;
}

LPCTSTR CDlgMemberGift::GetSkinFile()
{
  return TEXT("DlgMemberGift.xml");
}

VOID CDlgMemberGift::SetMember(bool bMember)
{
  m_bMember =bMember;
}

void CDlgMemberGift::InitControlUI()
{
  UpdateUI();

}

VOID CDlgMemberGift::UpdateUI()
{
  CControlUI * pContorl1 = (CControlUI*)GetControlByName(szButtonTakeControlName);
  pContorl1->SetEnabled(m_bMember);

  int nGiftcount = (int)m_wGiftSubCount;


  const ConfigProperty *pPropConfig = CGlobalUnits::GetInstance()->GetPropertyConfigInfo();

  CPageLayoutUI * pPageLayoutUI = static_cast<CPageLayoutUI *>(GetControlByName(szPageLayoutGiftControlName));
  if(pPageLayoutUI)
  {
    for(int i =0; i<pPropConfig->cbPropertyCount; i++)
    {
      for(int j=0; j<nGiftcount; j++)
      {
        if(pPropConfig->PropertyInfo[i].wIndex == m_GiftSub[j].wPropertyID)
        {
          //����UI
          CDialogBuilder builder;
          CVerticalLayoutUI* pVerticalLayoutUI =NULL;
          pVerticalLayoutUI = static_cast<CVerticalLayoutUI *>(builder.Create(_T("DlgMemberGiftItem.xml"), (UINT)0, NULL, &m_PaintManager));
          pPageLayoutUI->Add(pVerticalLayoutUI);

          //����ͼ��
          CLabelUI* pPropertyImage = (CLabelUI*)pVerticalLayoutUI->FindControl(TEXT("ItemPropIOC"));
          if(pPropertyImage != NULL)
          {
            CStdString  csImageKind;
            csImageKind.Format(TEXT("DLG_BOX_SHOP\\GMAE_PROPERTY_%d.png"),pPropConfig->PropertyInfo[i].wIndex);
            pPropertyImage->SetBkImage(csImageKind);
          }

          //��������
          CNumberUI* pPropertyNum = (CNumberUI*)pVerticalLayoutUI->FindControl(TEXT("ItemPropNum"));
          if(pPropertyNum != NULL)
          {
            pPropertyNum->SetNumber(m_GiftSub[j].wPropertyCount);
          }
          //��������
          CLabelUI* pPropertyName = (CLabelUI*)pVerticalLayoutUI->FindControl(TEXT("ItemPropName"));
          if(pPropertyName != NULL)
          {
            pPropertyName->SetText(pPropConfig->PropertyInfo[i].szName);
          }
        }
      }
    }
  }

  CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
  tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
  CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();
  int nMemberOrder = pGlobalUserData->cbMemberOrder;
  if(nMemberOrder == 0)
  {
    nMemberOrder = 1;
  }
  const tagMemberParameterNew*  pMemberPara = pUserItemElement->GetUserMemberParameter(nMemberOrder);
  if(pPageLayoutUI)
  {
    //����UI
    CDialogBuilder builder;
    CVerticalLayoutUI* pVerticalLayoutUI =NULL;
    pVerticalLayoutUI = static_cast<CVerticalLayoutUI *>(builder.Create(_T("DlgMemberGiftItem.xml"), (UINT)0, NULL, &m_PaintManager));
    pPageLayoutUI->Add(pVerticalLayoutUI);

    //����ͼ��
    CLabelUI* pPropertyImage = (CLabelUI*)pVerticalLayoutUI->FindControl(TEXT("ItemPropIOC"));
    if(pPropertyImage != NULL)
    {
      CStdString  csImageKind;
      csImageKind.Format(TEXT("DLG_BOX_CHECKINNEW\\LABEL_MONEY.png"));
      pPropertyImage->SetBkImage(csImageKind);
    }

    //��������
    CNumberUI* pPropertyNum = (CNumberUI*)pVerticalLayoutUI->FindControl(TEXT("ItemPropNum"));
    if(pPropertyNum != NULL)
    {
      pPropertyNum->SetVisible(false);
    }

    TCHAR szText[128]= {0};
    _sntprintf(szText, CountArray(szText), TEXT("%d"), pMemberPara->dwMemberDayPresent);

    //��������
    CLabelUI* pPropertyName = (CLabelUI*)pVerticalLayoutUI->FindControl(TEXT("ItemPropName"));
    if(pPropertyName != NULL)
    {
      pPropertyName->SetText(szText);
    }
  }

}


VOID CDlgMemberGift::SetData(const  tagGiftPropertyInfo *pGiftSub,DWORD nCount)
{
  m_wGiftSubCount = nCount;
  memcpy(m_GiftSub, pGiftSub, sizeof(tagGiftPropertyInfo)*nCount);
}

//////////////////////////////////////////////////////////////////////////////////