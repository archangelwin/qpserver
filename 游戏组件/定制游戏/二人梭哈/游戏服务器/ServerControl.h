#pragma once

//��Ϸ���ƻ���
class IServerControl
{
public:
  IServerControl(void) {};
  virtual ~IServerControl(void) {};

public:
  //����������
  virtual bool __cdecl ShowCheatCard(BYTE cbHandCardData[GAME_PLAYER][5], ITableFrame * pITableFrame) = NULL;
  //����������
  virtual bool __cdecl ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame) = NULL;

  //��Ҫ����
  virtual bool __cdecl NeedControl() = NULL;

  //�������
  virtual bool __cdecl MeetControl(tagControlInfo ControlInfo) = NULL;

  //��ɿ���
  virtual bool __cdecl CompleteControl() = NULL;

  //���ؿ�������
  virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER][MAX_COUNT]) = NULL;
};
