#ifndef DLG_BUY_PROPERTY_HEAD_FILE
#define DLG_BUY_PROPERTY_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CGamePropertyItem;

//////////////////////////////////////////////////////////////////////////////////

//���߹���
class CDlgBuyProperty : public CSkinDialog
{
  //��Ԫ����
  friend class CGamePropertyItem;

  //��������
protected:
  LONGLONG                        m_lCanBuyCount;                     //�����Ŀ

  //�ӿڱ���
protected:
  CGamePropertyItem *       m_pGamePropertyItem;        //���߶���
  IGamePropertySink *       m_pIGamePropertySink;       //�ص��ӿ�

  //��̬����
protected:
  static CDlgBuyProperty *    m_pDlgBuyProperty;          //����ָ��

  //��������
public:
  //���캯��
  CDlgBuyProperty();
  //��������
  virtual ~CDlgBuyProperty();

  //���ú���
protected:
  //��Ϣ����
  virtual BOOL PreTranslateMessage(MSG * pMsg);
  //��������
  virtual BOOL OnInitDialog();
  //ȷ������
  virtual VOID OnOK();
  //ȡ����Ϣ
  virtual VOID OnCancel();

  //���غ���
public:
  //��ʼ�ؼ�
  virtual VOID InitControlUI();
  //��Ϣ����
  virtual VOID Notify(TNotifyUI &  msg);
  //Ƥ������
  virtual LPCTSTR GetSkinFile()
  {
    return TEXT("DlgBuyProperty.xml");
  }

  //�滭����
protected:
  //�滭��Ϣ
  virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

  //���ܺ���
protected:
  //����״̬
  VOID UpdateCtrlStatus();

  //��̬����
protected:
  //��ʾ����
  static VOID ShowBuyPropertyWnd(CGamePropertyItem * pGamePropertyItem, IGamePropertySink * pIGamePropertySink);
  //ִ������
  static VOID OnEventPerformCommand(WORD wCommandID,VOID * pCmdData);

  //��Ϣ����
protected:
  //�ı��¼�
  VOID OnEnChangePropertyCount();

  DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif