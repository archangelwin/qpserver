#pragma once

//��Ϸ���ƻ���
class IClientControlDlg : public CDialog
{
public:
  IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent) {}
  virtual ~IClientControlDlg(void) {}

public:
  //���¿���
  virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns) = NULL;
  //��ʼ�ؼ�
  virtual bool  InitControl()=NULL;
};
