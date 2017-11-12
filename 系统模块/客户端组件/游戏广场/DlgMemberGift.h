#ifndef DLG_MEMBER_GIFT_HEAD_FILE
#define DLG_MEMBER_GIFT_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//��Ա����
class CDlgMemberGift : public CFGuiDialog
{
  //��������
public:
  //���캯��
  CDlgMemberGift();
  //��������
  virtual ~CDlgMemberGift();

  //���غ���
protected:
  //��������
  virtual BOOL OnInitDialog();
  //ȷ������
  virtual VOID OnOK();
  //ȡ����Ϣ
  virtual VOID OnCancel();

  //�ӿں���
public:
  //��ʼ�ؼ�
  virtual void InitControlUI();
  //��Ϣ����
  virtual void Notify(TNotifyUI &  msg);

public:
  //  ���û�Ա
  VOID SetMember(bool bMember);
  //  ��������
  VOID SetData(const tagGiftPropertyInfo *pGiftSub,DWORD nCount);

  //���غ���
public:
  //Ƥ������
  virtual LPCTSTR GetSkinFile();

public:
  VOID UpdateUI();
public:
  bool m_bMember;
  tagGiftPropertyInfo       m_GiftSub[50];            //��������
  DWORD             m_wGiftSubCount;          //��������
};

//////////////////////////////////////////////////////////////////////////////////

#endif