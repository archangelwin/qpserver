#pragma once

//��Ϸ���ƻ���
class IServerControl
{
public:
  IServerControl(void) {};
  virtual ~IServerControl(void) {};

public:
  //����������
  virtual bool  RecvControlReq(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame) = NULL;
  //ִ�п���
  virtual bool  ExcuteControl(BYTE *cbCard)=NULL;
};
