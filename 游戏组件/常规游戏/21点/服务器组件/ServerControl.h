#pragma once

#define MAX_COUNT         11                  //�����Ŀ

//��Ϸ���ƻ���
class IServerControl
{
public:
  IServerControl(void) {};
  virtual ~IServerControl(void) {};

public:
  //����������
  virtual bool __cdecl ServerControl(BYTE cbHandCardData[GAME_PLAYER*2][11], ITableFrame * pITableFrame) = NULL;
  virtual bool __cdecl ServerControl(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame) = NULL;

  //���ؿ�������
  virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER * 2][MAX_COUNT], BYTE cbCardCount[GAME_PLAYER*2], ROOMUSERCONTROL Keyroomusercontrol, ITableFrame * pITableFrame, WORD wBankerUser, BYTE cbPlayStatus[GAME_PLAYER]) = NULL;
};
