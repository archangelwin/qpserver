#pragma once

#define IDM_ADMIN_COMMDN WM_USER+1000

//��Ϸ���ƻ���
class IClientControlDlg : public CDialog
{
public:
  IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent) {}
  virtual ~IClientControlDlg(void) {}

  //���¿���
  virtual void __cdecl OnAllowControl(bool bEnable) = NULL;
  //������
  virtual bool __cdecl ReqResult(const void * pBuffer)=NULL;
};
