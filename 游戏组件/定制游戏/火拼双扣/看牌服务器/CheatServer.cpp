// ServerControl.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>
#include "CheatServerItemSink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE CheatServerDLL = { NULL, NULL };

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  // ���ʹ�� lpReserved���뽫���Ƴ�
  UNREFERENCED_PARAMETER(lpReserved);

  if(dwReason == DLL_PROCESS_ATTACH)
  {
    TRACE0("ServerControl.DLL ���ڳ�ʼ����\n");
    if(!AfxInitExtensionModule(CheatServerDLL, hInstance))
    {
      return 0;
    }
    new CDynLinkLibrary(CheatServerDLL);

  }
  else if(dwReason == DLL_PROCESS_DETACH)
  {
    TRACE0("ServerControl.DLL ������ֹ��\n");
    AfxTermExtensionModule(CheatServerDLL);
  }
  return 1;   // ȷ��
}

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateCheatServerControl()
{
  //��������
  CCheatServerItemSink * pServerControl = NULL;
  try
  {
    pServerControl = new CCheatServerItemSink();
    if(pServerControl == NULL)
    {
      throw TEXT("����ʧ��");
    }

    return pServerControl;
  }
  catch(...) {}

  //�������
  SafeDelete(pServerControl);
  return NULL;
}