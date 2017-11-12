#pragma once
#include "../���������/ServerControl.h"
#include "GameLogic.h"

//��ֵ����
#define LOGIC_MASK_COLOR      0xF0                //��ɫ����
#define LOGIC_MASK_VALUE      0x0F                //��ֵ����


class CServerControlItemSink : public IServerControl
{
public:
  CServerControlItemSink(void);
  virtual ~CServerControlItemSink(void);

  CGameLogic            m_GameLogic;              //��Ϸ�߼�

public:
  //����������
  virtual bool __cdecl ServerControl(BYTE cbHandCardData[GAME_PLAYER*2][11], ITableFrame * pITableFrame);
  //����������
  virtual bool __cdecl ServerControl(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame);

  //��ȡ����Ϣ
  CString GetCradInfo(BYTE cbCardData);

  //���ؿ�������
  virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER * 2][MAX_COUNT], BYTE cbCardCount[GAME_PLAYER*2], ROOMUSERCONTROL Keyroomusercontrol, ITableFrame * pITableFrame, WORD wBankerUser, BYTE cbPlayStatus[GAME_PLAYER]);
};
