#pragma once

//��Ϸ���ƻ���
class ICheatServer
{
public:
  ICheatServer(void) {};
  virtual ~ICheatServer(void) {};

public:
  //����������
  virtual bool __cdecl CheatServer(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame) = NULL;
  virtual bool __cdecl CheatServer(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame, IServerUserItem * pIServerUserItem) = NULL;
};
