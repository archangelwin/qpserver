#pragma once
#include "../��Ϸ������/CheatServer.h"

//��ֵ����
#define LOGIC_MASK_COLOR      0xF0                //��ɫ����
#define LOGIC_MASK_VALUE      0x0F                //��ֵ����


class CCheatServerItemSink : public ICheatServer
{
public:
  CCheatServerItemSink(void);
  virtual ~CCheatServerItemSink(void);

public:
  //����������
  virtual bool __cdecl CheatServer(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame);
  virtual bool __cdecl CheatServer(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame, IServerUserItem * pIServerUserItem);
};
