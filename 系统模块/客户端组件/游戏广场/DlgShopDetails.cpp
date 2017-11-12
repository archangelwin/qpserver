#include "Stdafx.h"
#include "Resource.h"
#include "DlgShopDetails.h"
#include "PlatformEvent.h"
#include "DlgInformation.h"
//////////////////////////////////////////////////////////////////////////////////
//��ť�ؼ�
const TCHAR* const szButtonDecrease = TEXT("ButtonDecrease");
const TCHAR* const szButtonIncrease = TEXT("ButtonIncrease");
const TCHAR* const szButtonBuyUse = TEXT("ButtonBuyUse");
const TCHAR* const szButtonBuyIntoBag = TEXT("ButtonBuyIntoBag");
//const TCHAR* const sz = TEXT("ButtonBuyUse");
//const TCHAR* const sz = TEXT("ButtonBuyUse");

//������Ϣ
const TCHAR* const szLabelPrice = TEXT("LabelPrice");

const TCHAR* const szLabelVIPPrice = TEXT("LabelVIPPrice");
const TCHAR* const szLabelBuyScore = TEXT("LabelBuyScore");
const TCHAR* const szLabelInsureScore = TEXT("LabelInsureScore");
const TCHAR* const szLabelItemShopIOC = TEXT("LabelItemShopIOC");
const TCHAR* const szLableItemShopName = TEXT("LableItemShopName");
const TCHAR* const szNumberBuy = TEXT("NumberBuy");

//VIP�ؼ�
const TCHAR* const szContainerVipAwardControlName = TEXT("ContainerVip");
const TCHAR* const szLabelVipLevelControlName = TEXT("LabelVipLevel");
const TCHAR* const szLabelAwardMemberControlName = TEXT("LableShopVip");

const TCHAR* const szRadioButtonGold = TEXT("RadioButtonGold");
const TCHAR* const szRadioButtonUserMadel = TEXT("RadioButtonUserMadel");
const TCHAR* const szRadioButtonCash = TEXT("RadioButtonCash");
const TCHAR* const szRadioButtonLoveLiness = TEXT("RadioButtonLoveLiness");

const TCHAR* const szNumberOfTypeScore = TEXT("NumberOfTypeScore");
const TCHAR* const szLabelBuyTypeName = TEXT("LabelBuyTypeName");

//���ֿؼ�
const TCHAR* const szTextUseTipControlName = TEXT("TextlUseTip");

//�����ؼ�
const TCHAR* const szHoriLayoutDiscountControlName = TEXT("HoriLayoutDiscount");
const TCHAR* const szHoriLayoutVIPPriceControlName = TEXT("HoriLayoutVIPPrice");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgShopDetails::CDlgShopDetails() : CFGuiDialog(IDD_DLG_POPUP)
{
  //��������
  m_MissionManager.InsertMissionItem(this);

  m_ptagPropertyInfo = NULL;
  m_dwBuyNumber = 1;
  m_cbConsumeType = INVALID_BYTE;

  m_pIGamePropertyAction = NULL;
  m_eBuyMode = BUY_INTO_BAG;
}

//��������
CDlgShopDetails::~CDlgShopDetails()
{
}

//��������
BOOL CDlgShopDetails::OnInitDialog()
{
  __super::OnInitDialog();

  //�޸���ʽ
  ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

  //���ñ���
  SetWindowText(TEXT("�̳�"));

  //���д���
  CenterWindow(this);

  InitUI();

  UpdateUI();

  return TRUE;
}

//ȷ������
VOID CDlgShopDetails::OnOK()
{

  return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgShopDetails::OnCancel()
{
  return __super::OnCancel();
}

//��Ϣ����
void CDlgShopDetails::Notify(TNotifyUI &  msg)
{
  //��ȡ����
  CControlUI * pControlUI = msg.pSender;

  //����¼�
  if(lstrcmp(msg.sType, TEXT("click")) == 0)
  {
    //�رհ�ť
    if(lstrcmp(pControlUI->GetName(), TEXT("ButtonClose"))==0)
    {
      return OnCancel();
    }

    //
    if(lstrcmp(pControlUI->GetName(), szRadioButtonGold)==0
       ||lstrcmp(pControlUI->GetName(), szRadioButtonUserMadel)==0
       ||lstrcmp(pControlUI->GetName(), szRadioButtonCash)==0
       ||lstrcmp(pControlUI->GetName(), szRadioButtonLoveLiness)==0)
    {
      UpdateUI();
      //  return OnCancel();
    }

    if(pControlUI->GetName() == szButtonDecrease)
    {
      CNumberUI* pNumberBuy = static_cast<CNumberUI *>(GetControlByName(szNumberBuy));
      if(pNumberBuy != NULL)
      {
        m_dwBuyNumber = (WORD)(pNumberBuy->GetNumber());
      }

      if(m_dwBuyNumber == 1)
      {
        CButtonUI * pButtonUI = static_cast<CButtonUI *>(GetControlByName(szButtonDecrease));
        if(pButtonUI != NULL)
        {
          pButtonUI->SetEnabled(false);
        }
      }
      return;
    }

    if(pControlUI->GetName() == szButtonIncrease)
    {
      CNumberUI* pNumberBuy = static_cast<CNumberUI *>(GetControlByName(szNumberBuy));
      if(pNumberBuy != NULL)
      {
        m_dwBuyNumber = (WORD)(pNumberBuy->GetNumber());
      }

      CButtonUI * pButtonUI = static_cast<CButtonUI *>(GetControlByName(szButtonDecrease));
      if(pButtonUI != NULL)
      {
        pButtonUI->SetEnabled(true);
      }

      UpdateUI();
      return;
    }

    if(pControlUI->GetName() == szButtonBuyUse || pControlUI->GetName() == szButtonBuyIntoBag)
    {
      m_eBuyMode = BUY_INTO_BAG;
      if(pControlUI->GetName() == szButtonBuyUse)
      {
        m_eBuyMode = BUY_IMMEDIATELY_USE;
      }
      tagBuyGamePropertyInfo BuyGamePropertyInfo;
      BuyGamePropertyInfo.dwBuyNumber = m_dwBuyNumber;
      BuyGamePropertyInfo.cbScoreType = m_cbConsumeType;
      BuyGamePropertyInfo.wPropertyID = m_ptagPropertyInfo->wIndex;
      BuyGamePropertyInfo.cbUse = m_eBuyMode;

      m_pIGamePropertyAction->PerformBuyGameProperty(&BuyGamePropertyInfo);
      return;
    }
  }

  return;
}

LPCTSTR CDlgShopDetails::GetSkinFile()
{
  return TEXT("DlgShopDetails.xml");
}

VOID CDlgShopDetails::InitData()
{

}

VOID CDlgShopDetails::LoadDataFromMemory()
{

}

VOID CDlgShopDetails::LoadDataFromUI()
{
  return;
}

VOID CDlgShopDetails::UpdateData()
{

}

VOID CDlgShopDetails::InitUI()
{
  //��ȡ����
  CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
  tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

  //�������
  CRadioButtonUI *pRadioButtonUIGold, *pRadioButtonUIUserMedal, *pRadioButtonUICash, *pRadioButtonUILoveLiness;

  //��ȡ�ؼ�
  pRadioButtonUIGold = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonGold));
  pRadioButtonUIUserMedal = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonUserMadel));
  pRadioButtonUICash = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonCash));
  pRadioButtonUILoveLiness = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonLoveLiness));

  //���ð�ť
  if(m_ptagPropertyInfo->lPropertyGold == 0 && pRadioButtonUIGold != NULL)
  {
    pRadioButtonUIGold->SetVisible(false);
  }
  if(m_ptagPropertyInfo->lPropertyUserMedal == 0 && pRadioButtonUIUserMedal != NULL)
  {
    pRadioButtonUIUserMedal->SetVisible(false);
  }
  if(m_ptagPropertyInfo->dPropertyCash == 0 && pRadioButtonUICash != NULL)
  {
    pRadioButtonUICash->SetVisible(false);
  }
  if(m_ptagPropertyInfo->lPropertyLoveLiness == 0 && pRadioButtonUILoveLiness != NULL)
  {
    pRadioButtonUILoveLiness->SetVisible(false);
  }

  if(m_cbConsumeType==0)
  {
    if(m_ptagPropertyInfo->lPropertyGold != 0 && pRadioButtonUIGold != NULL && pRadioButtonUIGold->IsEnabled() == true)
    {
      pRadioButtonUIGold->SetCheck(true);
    }
    else if(m_ptagPropertyInfo->lPropertyUserMedal != 0 && pRadioButtonUIUserMedal != NULL && pRadioButtonUIUserMedal->IsEnabled() == true)
    {
      pRadioButtonUIUserMedal->SetCheck(true);
    }
    else if(m_ptagPropertyInfo->dPropertyCash != 0 && pRadioButtonUICash != NULL && pRadioButtonUICash->IsEnabled() == true)
    {
      pRadioButtonUICash->SetCheck(true);
    }
    else if(m_ptagPropertyInfo->lPropertyLoveLiness != 0 && pRadioButtonUILoveLiness != NULL && pRadioButtonUILoveLiness->IsEnabled() == true)
    {
      pRadioButtonUILoveLiness->SetCheck(true);
    }

  }
  else
  {
    if(pRadioButtonUIGold)
    {
      pRadioButtonUIGold->SetCheck(m_cbConsumeType == CONSUME_TYPE_GOLD);
    }
    if(pRadioButtonUIUserMedal)
    {
      pRadioButtonUIUserMedal->SetCheck(m_cbConsumeType == CONSUME_TYPE_USEER_MADEL);
    }
    if(pRadioButtonUICash)
    {
      pRadioButtonUICash->SetCheck(m_cbConsumeType == CONSUME_TYPE_CASH);
    }
    if(pRadioButtonUILoveLiness)
    {
      pRadioButtonUILoveLiness->SetCheck(m_cbConsumeType == CONSUME_TYPE_LOVELINESS);
    }
  }


  //�ۿ���ʾ
  CHorizontalLayoutUI* pHoriLayoutDiscount = static_cast<CHorizontalLayoutUI*>(GetControlByName(szHoriLayoutDiscountControlName));
  if(pHoriLayoutDiscount)
  {
    pHoriLayoutDiscount->SetVisible(pGlobalUserData->cbMemberOrder>0);
  }

  //�ۺ�۸�
  CHorizontalLayoutUI* HoriLayoutVIPPrice = static_cast<CHorizontalLayoutUI*>(GetControlByName(szHoriLayoutVIPPriceControlName));
  if(HoriLayoutVIPPrice)
  {
    HoriLayoutVIPPrice->SetVisible(pGlobalUserData->cbMemberOrder>0);
  }

  //�����ı�
  TCHAR szText[128];
  _sntprintf(szText, CountArray(szText), TEXT("%I64d��Ϸ��"), m_ptagPropertyInfo->lPropertyGold);
  pRadioButtonUIGold->SetText(szText);
  _sntprintf(szText, CountArray(szText), TEXT("%I64dԪ��"), m_ptagPropertyInfo->lPropertyUserMedal);
  pRadioButtonUIUserMedal->SetText(szText);
  _sntprintf(szText, CountArray(szText), TEXT("%0.2f��Ϸ��"), m_ptagPropertyInfo->dPropertyCash);
  pRadioButtonUICash->SetText(szText);
  _sntprintf(szText, CountArray(szText), TEXT("%I64d����ֵ"), m_ptagPropertyInfo->lPropertyLoveLiness);
  pRadioButtonUILoveLiness->SetText(szText);

  //����ͼ��
  CLabelUI *pLabel = static_cast<CLabelUI *>(GetControlByName(szLabelItemShopIOC));
  if(pLabel != NULL)
  {
    CStdString  csImageKind;
    csImageKind.Format(TEXT("DLG_BOX_SHOP\\GMAE_PROPERTY_%d.png"),m_ptagPropertyInfo->wIndex);
    if(m_PaintManager.GetImageEx(csImageKind,NULL)==NULL)
    {
      csImageKind.Empty();
    }

    if(!csImageKind.IsEmpty())
    {
      pLabel->SetBkImage(csImageKind);
    }
  }

  //��������
  pLabel = static_cast<CLabelUI *>(GetControlByName(szLableItemShopName));
  if(pLabel != NULL)
  {
    pLabel->SetText(m_ptagPropertyInfo->szName);
  }


  TCHAR szTaskRewardTotal[64]=TEXT("");
  if(pGlobalUserData->cbMemberOrder > 0)
  {

    //��Ա����
    const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
    if(pMemberParameter != NULL)
    {
      _sntprintf(szTaskRewardTotal,CountArray(szTaskRewardTotal),TEXT("%d%%�ۿ�"),pMemberParameter->dwMemberShop);
    }

    CControlUI * pContainerVip = GetControlByName(szContainerVipAwardControlName);
    if(pContainerVip!=NULL)
    {
      //��Ա�ȼ�
      for(int i=0; i<6; i++)
      {
        CStdString strLevel;
        strLevel.SmallFormat(TEXT("%s%d"),szLabelVipLevelControlName,i);
        CControlUI * pLabelVipUI = pContainerVip->FindControl(strLevel);
        if(pLabelVipUI)
        {
          if(i==pGlobalUserData->cbMemberOrder)
          {
            pLabelVipUI->SetVisible(true);
          }
          else
          {
            pLabelVipUI->SetVisible(false);
          }
        }
      }

      CControlUI * pControl4 = pContainerVip->FindControl(szLabelAwardMemberControlName);
      if(pControl4 !=NULL)
      {
        pControl4->SetVisible(true);
        pControl4->SetText(szTaskRewardTotal);
      }
    }

  }
  else
  {
    CControlUI * pContainerVip = GetControlByName(szContainerVipAwardControlName);
    if(pContainerVip !=NULL)
    {
      pContainerVip->SetVisible(false);
    }
  }

  ////VIP�ۿ�
  //pLabel = static_cast<CLabelUI *>(GetControlByName(szLableShopVip));
  //if(pGlobalUserData->cbMemberOrder > 0)
  //{
  //  _sntprintf(szText, CountArray(szText), TEXT("VIP %0.2f��"), (double)m_ptagPropertyInfo->wDiscount/10.00);
  //  if(pLabel != NULL) pLabel->SetText(szText);
  //}
  //else
  //{
  //  if(pLabel != NULL) pLabel->SetText(TEXT(""));
  //}

  //���ð�ť
  CButtonUI * pButtonUI = static_cast<CButtonUI *>(GetControlByName(szButtonDecrease));
  if(pButtonUI != NULL)
  {
    pButtonUI->SetEnabled(false);
  }

  //��ť����
  bool bEnbale = IsImmediatelyUseProp(m_ptagPropertyInfo);
  CButtonUI* pUseImmediately = static_cast<CButtonUI*>(GetControlByName(szButtonBuyUse));
  if(pUseImmediately != NULL)
  {
    pUseImmediately->SetEnabled(bEnbale);
  }

  //����˵��
  CTextUI* pLabelUseTip = static_cast<CTextUI*>(GetControlByName(szTextUseTipControlName));
  if(pLabelUseTip != NULL)
  {
    if(_tcslen(m_ptagPropertyInfo->szRegulationsInfo) == 0)
    {
      pLabelUseTip->SetText(TEXT("����˵��"));
    }
    else
    {
      pLabelUseTip->SetText(m_ptagPropertyInfo->szRegulationsInfo);
    }
  }
  return;
}

VOID CDlgShopDetails::UpdateUI()
{
  if(m_ptagPropertyInfo == NULL)
  {
    return;
  }

  //�������
  CLabelUI *pLablePrice, *pLabelVIPPrice, *pLabelBuyScore;
  CRadioButtonUI *pRadioButtonUIGold, *pRadioButtonUIUserMedal, *pRadioButtonUICash, *pRadioButtonUILoveLiness;

  TCHAR szText[128] = {0};

  //��ȡ�ؼ�
  pLablePrice = static_cast<CLabelUI *>(GetControlByName(szLabelPrice));
  pLabelVIPPrice = static_cast<CLabelUI *>(GetControlByName(szLabelVIPPrice));
  pLabelBuyScore = static_cast<CLabelUI *>(GetControlByName(szLabelBuyScore));
  CNumberUI* pNumberBuy = static_cast<CNumberUI *>(GetControlByName(szNumberBuy));

  pRadioButtonUIGold = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonGold));
  pRadioButtonUIUserMedal = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonUserMadel));
  pRadioButtonUICash = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonCash));
  pRadioButtonUILoveLiness = static_cast<CRadioButtonUI *>(GetControlByName(szRadioButtonLoveLiness));

  ASSERT(pLablePrice != NULL && pLabelVIPPrice != NULL && pLabelBuyScore != NULL && pNumberBuy != NULL);
  if(pLablePrice == NULL || pLabelVIPPrice == NULL || pLabelBuyScore == NULL || pNumberBuy == NULL)
  {
    return;
  }

  //��������
  //_sntprintf(szText, CountArray(szText), TEXT("%d"), m_dwBuyNumber);
  //pLableNum->SetText(szText);
  if(pNumberBuy != NULL)
  {
    pNumberBuy->SetMinNumber(1);
  }

  //��ȡ����
  CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
  tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

  //��Ա�ۿ�
  WORD wDiscount = 0;
  const tagMemberParameterNew* pMemberParameter = CUserItemElement::GetInstance()->GetUserMemberParameter(pGlobalUserData->cbMemberOrder);
  if(pMemberParameter != NULL)
  {
    wDiscount = (WORD)pMemberParameter->dwMemberShop;
  }
  //��ҹ���
  if(pRadioButtonUIGold != NULL && pRadioButtonUIGold->GetCheck() == true)
  {
    //����ԭ��
    _sntprintf(szText, CountArray(szText), TEXT("%I64d��Ϸ��/��"), m_ptagPropertyInfo->lPropertyGold);
    pLablePrice->SetText(szText);

    //��Ա�۸�
    _sntprintf(szText, CountArray(szText), TEXT("%I64d��Ϸ��/��"), m_ptagPropertyInfo->lPropertyGold * wDiscount / 100);
    pLabelVIPPrice->SetText(szText);

    //������
    LONGLONG lBuyScore;
    LONG BuyOneScore = 0;
    if(pGlobalUserData->cbMemberOrder == 0)
    {
      BuyOneScore = (LONG)(m_ptagPropertyInfo->lPropertyGold);
    }
    else
    {
      BuyOneScore = (LONG)(m_ptagPropertyInfo->lPropertyGold * ((double)wDiscount / 100.00));
    }
    lBuyScore = BuyOneScore * m_dwBuyNumber;
    _sntprintf(szText, CountArray(szText), TEXT("%I64d��Ϸ��"), lBuyScore);
    pLabelBuyScore->SetText(szText);

    m_cbConsumeType = CONSUME_TYPE_GOLD;

    CLabelUI* LabelBuyTypeName = static_cast<CLabelUI*>(GetControlByName(szLabelBuyTypeName));
    if(LabelBuyTypeName != NULL)
    {
      LabelBuyTypeName->SetText(TEXT("���д�"));
    }

    //���н��
    CNumberUI *pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberOfTypeScore));
    if(pNumberUI != NULL)
    {
      //��ȡ����
      CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
      tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

      float fUserInsure = (float)(pGlobalUserData->lUserInsure);
      pNumberUI->SetNumberDecimal(false);
      pNumberUI->SetNumber(fUserInsure);
    }



    if(pNumberBuy != NULL)
    {
      pNumberBuy->SetMaxNumber((int)(max(pGlobalUserData->lUserInsure/BuyOneScore, 1)));
    }

    CButtonUI* pButtonIncrease = static_cast<CButtonUI*>(GetControlByName(szButtonIncrease));
    if(pButtonIncrease != NULL)
    {
      pButtonIncrease->SetEnabled(pGlobalUserData->lUserInsure/BuyOneScore > 1);
    }
  }

  //Ԫ������
  if(pRadioButtonUIUserMedal != NULL && pRadioButtonUIUserMedal->GetCheck() == true)
  {
    //����ԭ��
    _sntprintf(szText, CountArray(szText), TEXT("%I64dԪ��/��"), m_ptagPropertyInfo->lPropertyUserMedal);
    pLablePrice->SetText(szText);

    //��Ա�۸�
    _sntprintf(szText, CountArray(szText), TEXT("%I64dԪ��/��"), m_ptagPropertyInfo->lPropertyUserMedal * wDiscount / 100);
    pLabelVIPPrice->SetText(szText);

    //������
    LONGLONG lBuyScore;
    LONG BuyOneScore = 0;
    if(pGlobalUserData->cbMemberOrder == 0)
    {
      BuyOneScore = (LONG)(m_ptagPropertyInfo->lPropertyUserMedal);
    }
    else
    {
      BuyOneScore = (LONG)(m_ptagPropertyInfo->lPropertyUserMedal * ((double)wDiscount / 100.00));
    }
    lBuyScore = BuyOneScore * m_dwBuyNumber;
    _sntprintf(szText, CountArray(szText), TEXT("%I64dԪ��"), lBuyScore);
    pLabelBuyScore->SetText(szText);

    m_cbConsumeType = CONSUME_TYPE_USEER_MADEL;


    CLabelUI* LabelBuyTypeName = static_cast<CLabelUI*>(GetControlByName(szLabelBuyTypeName));
    if(LabelBuyTypeName != NULL)
    {
      LabelBuyTypeName->SetText(TEXT("Ԫ    ����"));
    }

    CNumberUI *pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberOfTypeScore));
    if(pNumberUI != NULL)
    {
      float fUserIngot = (float)(pGlobalUserData->lUserIngot);
      pNumberUI->SetNumberDecimal(false);
      pNumberUI->SetNumber(fUserIngot);
    }

    if(pNumberBuy != NULL)
    {
      pNumberBuy->SetMaxNumber((int)(pGlobalUserData->lUserIngot/BuyOneScore));
    }

    CButtonUI* pButtonIncrease = static_cast<CButtonUI*>(GetControlByName(szButtonIncrease));
    if(pButtonIncrease != NULL)
    {
      pButtonIncrease->SetEnabled(pGlobalUserData->lUserIngot/BuyOneScore > 1);
    }
  }

  //��Ϸ������
  if(pRadioButtonUICash != NULL && pRadioButtonUICash->GetCheck() == true)
  {
    //����ԭ��
    _sntprintf(szText, CountArray(szText), TEXT("%0.2f��Ϸ��/��"), m_ptagPropertyInfo->dPropertyCash);
    pLablePrice->SetText(szText);

    //��Ա�۸�
    _sntprintf(szText, CountArray(szText), TEXT("%0.2f��Ϸ��/��"), m_ptagPropertyInfo->dPropertyCash * ((double)wDiscount / 100.00));
    pLabelVIPPrice->SetText(szText);

    //������
    double lBuyScore;
    LONG BuyOneScore = 0;
    if(pGlobalUserData->cbMemberOrder == 0)
    {
      BuyOneScore = (LONG)(m_ptagPropertyInfo->dPropertyCash);
    }
    else
    {
      BuyOneScore = (LONG)(m_ptagPropertyInfo->dPropertyCash * ((double)wDiscount / 100.00));
    }
    lBuyScore = BuyOneScore * m_dwBuyNumber;
    _sntprintf(szText, CountArray(szText), TEXT("%0.2f��Ϸ��"), lBuyScore);
    pLabelBuyScore->SetText(szText);

    m_cbConsumeType = CONSUME_TYPE_CASH;

    CLabelUI* LabelBuyTypeName = static_cast<CLabelUI*>(GetControlByName(szLabelBuyTypeName));
    if(LabelBuyTypeName != NULL)
    {
      LabelBuyTypeName->SetText(TEXT("  ��Ϸ����"));
    }

    CNumberUI *pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberOfTypeScore));
    if(pNumberUI != NULL)
    {
      float fUserBeans = (float)(pGlobalUserData->dUserBeans);
      pNumberUI->SetNumberDecimal(true);
      pNumberUI->SetNumber(fUserBeans);

    }

    if(pNumberBuy != NULL)
    {
      pNumberBuy->SetMaxNumber((int)(pGlobalUserData->dUserBeans/BuyOneScore));
    }

    CButtonUI* pButtonIncrease = static_cast<CButtonUI*>(GetControlByName(szButtonIncrease));
    if(pButtonIncrease != NULL)
    {
      pButtonIncrease->SetEnabled(pGlobalUserData->dUserBeans/BuyOneScore > 1);
    }
  }

  //����ֵ����
  if(pRadioButtonUILoveLiness != NULL && pRadioButtonUILoveLiness->GetCheck() == true)
  {
    //����ԭ��
    _sntprintf(szText, CountArray(szText), TEXT("%I64d����ֵ/��"), m_ptagPropertyInfo->lPropertyLoveLiness);
    pLablePrice->SetText(szText);

    //��Ա�۸�
    _sntprintf(szText, CountArray(szText), TEXT("%I64d����ֵ/��"), m_ptagPropertyInfo->lPropertyLoveLiness * wDiscount / 100);
    pLabelVIPPrice->SetText(szText);

    //������
    LONGLONG lBuyScore;
    LONG BuyOneScore = 0;
    if(pGlobalUserData->cbMemberOrder == 0)
    {
      BuyOneScore = (LONG)(m_ptagPropertyInfo->lPropertyLoveLiness);
    }
    else
    {
      BuyOneScore = (LONG)(m_ptagPropertyInfo->lPropertyLoveLiness * ((double)wDiscount / 100.00));
    }
    lBuyScore = BuyOneScore * m_dwBuyNumber;

    _sntprintf(szText, CountArray(szText), TEXT("%I64d����ֵ"), lBuyScore);
    pLabelBuyScore->SetText(szText);

    m_cbConsumeType = CONSUME_TYPE_LOVELINESS;

    CLabelUI* LabelBuyTypeName = static_cast<CLabelUI*>(GetControlByName(szLabelBuyTypeName));
    if(LabelBuyTypeName != NULL)
    {
      LabelBuyTypeName->SetText(TEXT("  ����ֵ��"));
    }

    CNumberUI *pNumberUI = static_cast<CNumberUI *>(GetControlByName(szNumberOfTypeScore));
    if(pNumberUI != NULL)
    {
      float fLoveLiness = (float)(pGlobalUserData->dwLoveLiness);
      pNumberUI->SetNumberDecimal(false);
      pNumberUI->SetNumber(fLoveLiness);
    }

    if(pNumberBuy != NULL)
    {
      pNumberBuy->SetMaxNumber(pGlobalUserData->dwLoveLiness/BuyOneScore);
    }

    CButtonUI* pButtonIncrease = static_cast<CButtonUI*>(GetControlByName(szButtonIncrease));
    if(pButtonIncrease != NULL)
    {
      pButtonIncrease->SetEnabled(pGlobalUserData->dwLoveLiness/BuyOneScore > 1);
    }
  }

  return;
}


//�����¼�
bool CDlgShopDetails::OnEventMissionLink(INT nErrorCode)
{
  //���ӽ��
  if(nErrorCode!=0)
  {
    //��������
    if(m_MissionManager.AvtiveMissionItem(this,true)==true)
    {
      return true;
    }

    //������ʾ
    CDlgInformation Information(NULL);
    Information.ShowMessageBox(TEXT("���ӷ�������ʱ��������Ϣ��ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);

    return true;
  }

  //��ȡ����
  CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
  tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

  //�������
  CMD_GP_C_PropertyBuy PropertyBuy;
  PropertyBuy.wItemCount = m_dwBuyNumber;
  PropertyBuy.cbConsumeType = m_cbConsumeType;
  PropertyBuy.wPropertyIndex = m_ptagPropertyInfo->wIndex;
  PropertyBuy.dwUserID = pGlobalUserData->dwUserID;

  m_MissionManager.SendData(MDM_GP_USER_SERVICE, SUB_GP_PROPERTY_BUY, &PropertyBuy, sizeof(CMD_GP_C_PropertyBuy));

  return true;
}

//�ر��¼�
bool CDlgShopDetails::OnEventMissionShut(BYTE cbShutReason)
{
  //��ʾ��Ϣ
  if(cbShutReason!=SHUT_REASON_NORMAL)
  {
    //��������
    if(m_MissionManager.AvtiveMissionItem(this,true)==true)
    {
      return true;
    }

    //������ʾ
    CDlgInformation Information(NULL);
    Information.ShowMessageBox(TEXT("���ӷ�������ʱ��������Ϣ��ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
  }


  return true;
}

//��ȡ�¼�
bool CDlgShopDetails::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
  if(Command.wMainCmdID == MDM_GP_PROPERTY)
  {
    if(Command.wSubCmdID == SUB_GP_PROPERTY_BUY_RESULT)
    {
      //Ч�����
      ASSERT(wDataSize == sizeof(CMD_GP_S_PropertySuccess));
      if(wDataSize != sizeof(CMD_GP_S_PropertySuccess))
      {
        return false;
      }

      //��ֹ����
      m_MissionManager.ConcludeMissionItem(this,false);

      //�������
      CMD_GP_S_PropertySuccess * pPropertySuccess=(CMD_GP_S_PropertySuccess *)pData;

      if(pPropertySuccess->szNotifyContent[0] != 0)
      {
        //������ʾ
        CDlgInformation Information(this);
        Information.ShowMessageBox(pPropertySuccess->szNotifyContent,MB_OK|MB_ICONERROR,30);

        return true;
      }
      //��ȡ����
      CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
      tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

      pGlobalUserData->lUserInsure = pPropertySuccess->lInsureScore;
      pGlobalUserData->lUserIngot = pPropertySuccess->lUserMedal;
      pGlobalUserData->dwLoveLiness = (DWORD)(pPropertySuccess->lLoveLiness);
      pGlobalUserData->dUserBeans = pPropertySuccess->dCash;
      pGlobalUserData->cbMemberOrder = pPropertySuccess->cbCurrMemberOrder;

      //�����¼�
      CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
      if(pPlatformEvent!=NULL)
      {
        pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE, 0L);
      }

      //������ʾ
      CDlgInformation Information(this);
      Information.ShowMessageBox(TEXT("����ɹ���"),MB_OK|MB_ICONINFORMATION,30);
    }
  }
  return true;
}

//������Ϣ
void CDlgShopDetails::SetPropertyInfo(tagPropertyInfo *ptagPropertyInfo)
{
  m_ptagPropertyInfo = ptagPropertyInfo;
  return;
}

//
void CDlgShopDetails::SetShopMainSink(IGamePropertyAction* pIGamePropertyAction)
{
  m_pIGamePropertyAction = pIGamePropertyAction;
  return;
}

bool CDlgShopDetails::IsImmediatelyUseProp(tagPropertyInfo *ptagPropertyInfo)
{
  if(ptagPropertyInfo == NULL)
  {
    return true;
  }
  //���� �� ��С���� �����Թ�������ʹ��
  if(ptagPropertyInfo->wKind == PROP_KIND_GIFT ||
     ptagPropertyInfo->wKind == PROP_KIND_TRUMPET ||
     ptagPropertyInfo->wKind == PROP_KIND_TYPHON)
  {
    return false;
  }
  return true;
}

void CDlgShopDetails::SetDefaultBuy(int nValue)
{
  switch(nValue)
  {
    case 1:
    {
      m_cbConsumeType= CONSUME_TYPE_CASH;
      break;
    }
    case 2:
    {
      m_cbConsumeType= CONSUME_TYPE_GOLD;
      break;
    }
    case 3:
    {
      m_cbConsumeType= CONSUME_TYPE_LOVELINESS;
      break;
    }
    case 4:
    {
      m_cbConsumeType= CONSUME_TYPE_USEER_MADEL;
      break;
    }
    default:
      m_cbConsumeType = 0;
  }

}

//////////////////////////////////////////////////////////////////////////////////