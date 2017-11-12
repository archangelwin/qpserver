#include "StdAfx.h"
#include "servercontrolitemsink.h"


CServerControlItemSink::CServerControlItemSink(void)
{
  m_cbControlTimes = 0;
  m_cbControlStyle=0;
}

CServerControlItemSink::~CServerControlItemSink(void)
{

}

//����������
bool  CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{
  ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
  if(wDataSize!=sizeof(CMD_C_ControlApplication))
  {
    return false;
  }
  //��������й���ԱȨ�� �򷵻ش���
  if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
  {
    return false;
  }

  CMD_C_ControlApplication* pControlApplication = (CMD_C_ControlApplication*)pDataBuffer;

  switch(pControlApplication->cbControlAppType)
  {
    case C_CA_UPDATE: //����
    {
      CMD_S_ControlReturns ControlReturns;
      ZeroMemory(&ControlReturns,sizeof(ControlReturns));
      ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
      ControlReturns.cbControlTimes = m_cbControlTimes;
      ControlReturns.cbControlStyle=m_cbControlStyle;
      pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
    }
    return true;
    case C_CA_SET:    //����
    {

      switch(pControlApplication->cbControlStyle)
      {

        case CS_BANKER_LOSE:  //ׯ������
        {
          m_cbControlStyle=pControlApplication->cbControlStyle;
          m_cbControlTimes=pControlApplication->cbControlTimes;
          CMD_S_ControlReturns ControlReturns;
          ZeroMemory(&ControlReturns,sizeof(ControlReturns));
          ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
          ControlReturns.cbControlTimes = m_cbControlTimes;
          ControlReturns.cbControlStyle=m_cbControlStyle;
          pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));

          break;
        }
        case CS_BANKER_WIN:   //ׯ��Ӯ��
        {
          m_cbControlStyle=pControlApplication->cbControlStyle;
          m_cbControlTimes=pControlApplication->cbControlTimes;
          CMD_S_ControlReturns ControlReturns;
          ZeroMemory(&ControlReturns,sizeof(ControlReturns));
          ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
          ControlReturns.cbControlTimes = m_cbControlTimes;
          ControlReturns.cbControlStyle=m_cbControlStyle;
          pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
          break;
        }
        default:  //�ܾ�����
        {
          //����ʧ��
          CMD_S_ControlReturns ControlReturns;
          ZeroMemory(&ControlReturns,sizeof(ControlReturns));
          ControlReturns.cbReturnsType = S_CR_FAILURE;
          pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
          break;
        }

      }
      return true;
    }
    case C_CA_CANCELS:  //ȡ��
    {
      m_cbControlTimes = 0;
      CMD_S_ControlReturns ControlReturns;
      ZeroMemory(&ControlReturns,sizeof(ControlReturns));
      ControlReturns.cbReturnsType = S_CR_CANCEL_SUCCESS;
      pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
    }
    return true;
  }
  //����ʧ��
  CMD_S_ControlReturns ControlReturns;
  ZeroMemory(&ControlReturns,sizeof(ControlReturns));
  ControlReturns.cbReturnsType = S_CR_FAILURE;
  pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
  return true;
}

//��Ҫ����
bool  CServerControlItemSink::NeedControl()
{
  if(m_cbControlStyle > 0 && m_cbControlTimes > 0)
  {
    return true;
  }

  return false;

}

//��ɿ���
bool  CServerControlItemSink::CompleteControl()
{

  if(m_cbControlTimes > 0)
  {
    m_cbControlTimes--;
  }

  if(m_cbControlTimes == 0)
  {
    m_cbControlTimes = 0;
    m_cbControlStyle = 0;

  }
  return true;
}

void  CServerControlItemSink::ReturnControlType(int & ControlType)
{
  ControlType=m_cbControlStyle;
}

