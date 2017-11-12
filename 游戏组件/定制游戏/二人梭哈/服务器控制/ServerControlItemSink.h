#pragma once
#include "../��Ϸ������/ServerControl.h"
#include "GameLogic.h"
//��ֵ����
#define LOGIC_MASK_COLOR      0xF0                //��ɫ����
#define LOGIC_MASK_VALUE      0x0F                //��ֵ����


class CServerControlItemSink : public IServerControl
{
  //���Ʊ���
protected:
  BYTE              m_cbWinSideControl;         //������Ӯ
  BYTE              m_cbExcuteTimes;          //ִ�д���
  int               m_nSendCardCount;         //���ʹ���

  CGameLogic            m_GameLogic;              //��Ϸ�߼�
public:
  CServerControlItemSink(void);
  virtual ~CServerControlItemSink(void);

public:
  //����������
  virtual bool __cdecl ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);

  //��Ҫ����
  virtual bool __cdecl NeedControl();

  //�������
  virtual bool __cdecl MeetControl(tagControlInfo ControlInfo);

  //��ɿ���
  virtual bool __cdecl CompleteControl();

  //���ؿ�������
  virtual bool __cdecl ControlResult(BYTE cbControlCardData[GAME_PLAYER][MAX_COUNT]);
  //����������
  virtual bool __cdecl ShowCheatCard(BYTE cbHandCardData[GAME_PLAYER][5], ITableFrame * pITableFrame);

  //��ȡ����Ϣ
  CString GetCradInfo(BYTE cbCardData);
};
