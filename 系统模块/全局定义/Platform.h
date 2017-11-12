#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//�����ļ�
#include "Macro.h"
#include "Define.h"

//�ṹ�ļ�
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//ģ���ļ�
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//����汾
#define VERSION_FRAME       PROCESS_VERSION(7,0,1)        //��ܰ汾
#define VERSION_PLAZA       PROCESS_VERSION(7,0,1)        //�����汾
#define VERSION_MOBILE_ANDROID    PROCESS_VERSION(7,0,1)        //�ֻ��汾
#define VERSION_MOBILE_IOS      PROCESS_VERSION(7,0,1)        //�ֻ��汾

//�汾����
#define VERSION_EFFICACY      0                 //Ч��汾
#define VERSION_FRAME_SDK     INTERFACE_VERSION(7,1)        //��ܰ汾

//////////////////////////////////////////////////////////////////////////////////
//�����汾

#ifndef _DEBUG

//ƽ̨����
const TCHAR szProduct[]=TEXT("Game96��Ϸ����");             //��Ʒ����
const TCHAR szPlazaClass[]=TEXT("NewQP98GamePlaza");            //�㳡����
const TCHAR szProductKey[]=TEXT("NewQP98GamePlatform");         //��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://");          //��¼��ַ
const TCHAR szLogonServer[]=TEXT("192.168.199.104");//TEXT("ry.foxuc.net");           //��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://");       //ƽ̨��վ
const TCHAR szValidateKey[]=TEXT("RYSyncLoginKey");           //��֤��Կ
const TCHAR szValidateLink[]=TEXT("SyncLogin.aspx?userid=%d&time=%d&signature=%s&url=/"); //��֤��ַ

#else

//////////////////////////////////////////////////////////////////////////////////
//�ڲ�汾

//ƽ̨����
const TCHAR szProduct[]=TEXT("Wuly��Ϸ����");             //��Ʒ����
const TCHAR szPlazaClass[]=TEXT("NewQP98GamePlaza");            //�㳡����
const TCHAR szProductKey[]=TEXT("NewQP98GamePlatform");         //��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://192.168.31.183");          //��¼��ַ
const TCHAR szLogonServer[]=TEXT("192.168.31.128");//TEXT("ry.foxuc.net");           //��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://192.168.31.183");       //ƽ̨��վ
const TCHAR szValidateKey[]=TEXT("RYSyncLoginKey");           //��֤��Կ
const TCHAR szValidateLink[]=TEXT("SyncLogin.aspx?userid=%d&time=%d&signature=%s&url=/"); //��֤��ַ

#endif

//////////////////////////////////////////////////////////////////////////////////

//���ݿ���
const TCHAR szPlatformDB[]=TEXT("RYPlatformDB");            //ƽ̨���ݿ�
const TCHAR szAccountsDB[]=TEXT("RYAccountsDB");            //�û����ݿ�
const TCHAR szTreasureDB[]=TEXT("RYTreasureDB");            //�Ƹ����ݿ�
const TCHAR szGameMatchDB[]=TEXT("RYGameMatchDB");            //�������ݿ�
const TCHAR szExerciseDB[]=TEXT("RYEducateDB");             //��ϰ���ݿ�

//////////////////////////////////////////////////////////////////////////////////

//��Ȩ��Ϣ
const TCHAR szCompilation[]=TEXT("BC4BFD9D-F5EB-45E5-8BC7-0CAE74A3B167");

//////////////////////////////////////////////////////////////////////////////////

#endif