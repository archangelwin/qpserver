#pragma once
#include "../��Ϸ������/ServerControl.h"

class CServerControlItemSink : public IServerControl
{
  //���Ʋ���
protected:

  BYTE              m_cbControlTimes;           //���ƴ���
  BYTE              m_cbControlStyle;           //���Ʒ�ʽ

public:
  CServerControlItemSink(void);
  virtual ~CServerControlItemSink(void);

public:
  //����������
  virtual bool  ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);

  //��Ҫ����
  virtual bool  NeedControl();

  //��ɿ���
  virtual bool  CompleteControl();

  //���ؿ�������
  virtual void  ReturnControlType(int & ControlType);


};
