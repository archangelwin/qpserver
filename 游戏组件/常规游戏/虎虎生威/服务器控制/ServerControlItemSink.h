#pragma once
#include "../��Ϸ������/ServerControl.h"


class CServerControlItemSink : public IServerControl
{
//���Ʊ���
protected:
  BYTE              m_cbControlArea;            //��������
  BYTE              m_cbControlTimes;           //���ƴ���
public:
  CServerControlItemSink(void);
  virtual ~CServerControlItemSink(void);

public:
  //����������
  virtual bool  RecvControlReq(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);
  //ִ�п���
  virtual bool  ExcuteControl(BYTE *cbCard);
};
