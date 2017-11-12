#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��



//////////////////////////////////////////////////////////////////////////

//�˿˽ṹ
//struct tagCardItem
//{
//  bool              bShoot;               //�����־
//  BYTE              cbCardData;             //�˿�����
//};

//////////////////////////////////////////////////////////////////////////
struct tagShootCard
{
  BYTE cbCardData;
  DWORD dwCardIndex;
};

//�˿˿ؼ�
class CCardControl
{
  //״̬����
protected:
  bool              m_bSmallMode;           //С��ģʽ
  bool              m_bHorizontal;            //��ʾ����
  bool              m_bPositively;            //��Ӧ��־
  bool              m_bDisplayItem;           //��ʾ��־
  bool              m_bShowCardControl;         //��ʾ�˿�

  //�˿�����
protected:
  WORD              m_wCardCount;           //�˿���Ŀ
  tagCardItem           m_CardItemArray[MAX_CARD_COUNT];  //�˿�����

  //ѡ�����
protected:
  WORD              m_wEndIndex;            //��������
  WORD              m_wStartIndex;            //��ʼ����

  //�������
protected:
  UINT              m_nXDistance;           //������
  UINT              m_nYDistance;           //������
  UINT              m_nShootDistance;         //����߶�

  //λ�ñ���
protected:
  CPoint              m_BenchmarkPos;           //��׼λ��
  enXCollocateMode        m_XCollocateMode;         //��ʾģʽ
  enYCollocateMode        m_YCollocateMode;         //��ʾģʽ

  //��Դ����
protected:
  CSize             m_CardSize;             //�˿˴�С
  //CBitImage           m_ImageCard;            //ͼƬ��Դ
  CPngImage           m_PngCard;              //ͼƬ��Դ
  //��������
public:
  //���캯��
  CCardControl();
  //��������
  virtual ~CCardControl();

  //�˿˿���
public:
  //�����˿�
  bool SetCardData(WORD wCardCount);
  //����ģʽ
  bool SetCardMode(bool bSmallMode);
  //�߼�����
  BYTE GetCardLogicValue(BYTE cbCardData);
  //�����˿�
  bool SetCardData(const BYTE cbCardData[], WORD wCardCount);
  //�����˿�
  bool SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount);

  //�����˿�
public:
  //�����˿�
  void ShootAllCard(bool bShoot);
  //�����˿�
  bool SetShootIndex(WORD wStartIndex, WORD wEndIndex);
  //�����˿�
  bool SetSelectIndex(WORD wStartIndex, WORD wEndIndex);
  //�����˿�
  bool SetShootCard(const BYTE cbCardData[], WORD wCardCount);
  //��������
  bool ContinueShootCard(const BYTE cbCardData[], WORD wCardCount);

  //�˿˿���
public:
  //ɾ���˿�
  bool RemoveShootItem();
  //�ƶ��˿�
  bool MoveCardItem(WORD wTargerItem);
  //�����˿�
  bool SwitchCardItem(WORD wSourceItem, WORD wTargerItem);

  //��ȡ�˿�
public:
  //��ȡ�˿�
  tagCardItem * GetCardFromIndex(WORD wIndex);
  //��ȡ�˿�
  tagCardItem * GetCardFromPoint(CPoint & MousePoint);

  //��ȡ�˿�
public:
  //�˿���Ŀ
  WORD GetCardCount()
  {
    return m_wCardCount;
  }
  //��ȡ�˿�
  WORD GetCardData(BYTE cbCardData[], WORD wBufferCount);
  //��ȡ�˿�
  WORD GetShootCard(BYTE cbCardData[], WORD wBufferCount);
  //��ȡ�˿�
  WORD GetShootCard(tagShootCard cbCardData[], WORD&wBufferCount);
  //��ȡ�˿�
  WORD GetCardData(tagCardItem CardItemArray[], WORD wBufferCount);

  //״̬��ѯ
public:
  //��ȡ��С
  CSize GetCardSize()
  {
    return m_CardSize;
  }
  //��ѯ����
  bool GetDirection()
  {
    return m_bHorizontal;
  }
  //��ѯ��Ӧ
  bool GetPositively()
  {
    return m_bPositively;
  }
  //��ѯ��ʾ
  bool GetDisplayItem()
  {
    return m_bDisplayItem;
  }

  //״̬����
public:
  //���÷���
  VOID SetDirection(bool bHorizontal)
  {
    m_bHorizontal=bHorizontal;
  }
  //������Ӧ
  VOID SetPositively(bool bPositively)
  {
    m_bPositively=bPositively;
  }
  //������ʾ
  VOID SetDisplayFlag(bool bDisplayItem)
  {
    m_bDisplayItem=bDisplayItem;
  }
  //���þ���
  VOID SetCardSpace(UINT nXDistance, UINT nYDistance, UINT nShootDistance);
  //��ʾ�˿�
  void ShowCardControl(bool bShow)
  {
    m_bShowCardControl=bShow;
  }
  //��ʾ�˿�
  void ShowWindow(int nShow)
  {
    ShowCardControl(nShow==SW_HIDE?false:true);
  }

  //�ؼ�����
public:
  //��ȡ����
  VOID GetCenterPoint(CPoint & CenterPoint);
  //��׼λ��
  VOID SetBenchmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
  //��׼λ��
  VOID SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

  //�¼�����
public:
  //�滭�˿�
  VOID DrawCardControl(CDC * pDC);
  //�����Ϣ
  bool OnEventSetCursor(CPoint Point);

  //�ڲ�����
public:
  //��ȡ��С
  VOID GetControlSize(CSize & ControlSize);
  //��ȡԭ��
  VOID GetOriginPoint(CPoint & OriginPoint);
  //�����л�
  WORD SwitchCardPoint(CPoint & MousePoint);
};

//////////////////////////////////////////////////////////////////////////

#endif