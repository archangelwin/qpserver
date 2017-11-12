#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define ST_ORDER          0                 //��С����
#define ST_COUNT          1                 //��Ŀ����
#define ST_VALUE          2                 //��ֵ����

//��ֵ����
#define MASK_COLOR          0xF0                //��ɫ����
#define MASK_VALUE          0x0F                //��ֵ����

//�˿�����
#define CT_ERROR          0                 //��������
#define CT_SINGLE         1                 //��������
#define CT_DOUBLE         2                 //��������
#define CT_THREE          3                 //��������
#define CT_SINGLE_LINK        4                 //��������
#define CT_DOUBLE_LINK        5                 //���ö���
#define CT_THREE_LINK       6                 //���ö���
#define CT_BOMB           7                 //ը������
#define CT_BOMB_3W          8                 //3��ը��
#define CT_BOMB_TW          9                 //����ը��
#define CT_BOMB_LINK        10                  //��ը����

/////////////////////////////////////

static CString GetFileDialogPath()
{
  CString strFileDlgPath;
  TCHAR szModuleDirectory[MAX_PATH];  //ģ��Ŀ¼
  GetModuleFileName(AfxGetInstanceHandle(),szModuleDirectory,sizeof(szModuleDirectory));
  int nModuleLen=lstrlen(szModuleDirectory);
  int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE")) + 1;
  if(nModuleLen<=nProcessLen)
  {
    return TEXT("");
  }
  szModuleDirectory[nModuleLen-nProcessLen]=0;
  strFileDlgPath = szModuleDirectory;
  return strFileDlgPath;
}

static void NcaTextOut(CString strInfo)
{
#ifdef _DEBUG


  CString strName = GetFileDialogPath()+  "\\ShuangKouHP.log";
  CTime time = CTime::GetCurrentTime() ;
  CString strTime ;
  strTime.Format(TEXT("%d:%d:%d") ,
                 time.GetHour() ,
                 time.GetMinute() ,
                 time.GetSecond()
                );
  CString strMsg;
  strMsg.Format(TEXT("%s,%s\r\n"),strTime,strInfo);

  CFile fLog;
  if(fLog.Open(strName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
  {

    fLog.SeekToEnd();
    int strLength=strMsg.GetLength();
#ifdef _UNICODE
    BYTE bom[2] = {0xff, 0xfe};
    fLog.Write(bom,sizeof(BYTE)*2);
    strLength*=2;
#endif
    fLog.Write((LPCTSTR)strMsg,strLength);
    fLog.Close();
  }


#endif


}


//���ƽ��
struct tagOutCardResult
{
  BYTE              cbCardCount;            //�˿���Ŀ
  BYTE              cbResultCard[MAX_COUNT];      //����˿�
};

//���ƽ��
struct tagOutCountResult
{
  BYTE              cbBombCount;            //ը����Ŀ
  BYTE              cbOutOverCount;           //��������
};

//�ֲ���Ϣ
struct tagDistributing
{
  BYTE              cbCardCount;            //�˿���Ŀ
  BYTE              cbDistributing[15][13];       //�ֲ���Ϣ
};

//////////////////////////////////////////////////////////////////////////



//��Ϸ�߼���
class CGameLogic
{
  //��������
public:
  static const BYTE       m_cbCardData[FULL_COUNT];             //�˿�����
  static const BYTE       m_cbCardGrade[TYPE_COUNT];              //���͵ȼ�
  static const BYTE       m_cbTransTable[BOMB_TYPE_COUNT][BOMB_TYPE_COUNT]; //ԾǨ�б�

  static BYTE                     m_cbLowCardData[13];
  static BYTE           m_cbCardColor[8];
  static BYTE           m_cbKing[4];
  BYTE                            m_cbArrayIndex;
  BYTE                            m_cbColorIndex;
  BYTE                            m_cbKingCount;
  //��������
public:
  //���캯��
  CGameLogic();
  //��������
  virtual ~CGameLogic();

  //���Ժ���
public:
  //��ȡ��ֵ
  BYTE GetCardValue(BYTE cbCardData)
  {
    return cbCardData&MASK_VALUE;
  }
  //��ȡ��ɫ
  BYTE GetCardColor(BYTE cbCardData)
  {
    return cbCardData&MASK_COLOR;
  }
  //���ƺ���
public:
  //�����˿�
  VOID SortCardList(BYTE cbCardData[], BYTE cbCardCount,BYTE cbSortType = ST_ORDER);
  //�����˿�
  VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //�����˿ˣ�����ǣ�
  BYTE RandCardList(BYTE cbCardList[],BYTE cbBufferCount,BYTE cbMainCard,WORD cbOutPut[]);
  //ɾ���˿�
  bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
  //ɾ���˿�
  bool RemoveCardEx(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);
  //��������
  void RandArray(INT nArrayOne[], INT nCountOne, INT nArrayTwo[], INT nCountTwo);
  //��ȡ�˿�
  VOID GetRandCardList(BYTE cbInitCardList[], BYTE cbInitCardCount,BYTE cbCardBuffer[], BYTE cbBufferCount);
  //��������
  VOID SetKingCount(BYTE cbKingCount)
  {
    m_cbKingCount=cbKingCount;
  };
  //�߼�����
public:
  //Ԥ����ը��
  bool PreDealBombList(BYTE cbBombList[]);
  //ը��ת��
  bool TransBombGrade(BYTE cbBombInfo[]);
  //�����߷���
  BYTE GetMaxBombGrade(BYTE *cbBomblist);
  //�õ���ϵ������
  BYTE GetMaxBombGrade(BYTE cbBombList[],BYTE cbListCount);
  //�߼���ֵ
  BYTE GetCardLogicValue(BYTE cbCardData);
  //�߼�ת����
  BYTE TransForLogicValue(BYTE cbCardData);
  //�߼�ת����
  BYTE GetCardLogicValueEx(BYTE cbCardData);
  //�Ա��˿�
  bool CompareCard(tagAnalyseResult& PerOutCard,tagAnalyseResult& CurOutCard);
  //��������
  bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,BYTE cbPlayerCount, tagOutCardResult & OutCardResult);
  //��������
  bool AiSearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,const BYTE cbPlayerCardData[],BYTE cbPlayerCount, tagOutCardResult & OutCardResult);
  //��ͬ����
  bool SearchSameLinkCard(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE bCardCount,BYTE bSearchLineCount,tagAnalyseInitResult &AnalyseResult);
  //�����˿�(�������������ı��ض�)
  VOID AnalyseSameCardData(BYTE cbCardData[], BYTE cbCardCount,BYTE bSearchCount, tagAnalyseInitResult & AnalyseResult);
  //�Ա������˿�
  bool CompareHandCard(const BYTE cbFirstHandCardData[], BYTE cbFirstHandCardCount, const BYTE cbNextHandCardData[],BYTE cbNextHandCardCount,BYTE bBombList[BOMB_TYPE_COUNT]);
  //�ڲ�����
public:
  //ը������
  bool IsBombCard(tagAnalyseResult& AnalyseResult);
  //�Ƿ�����
  bool IsLinkCardData(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //����Ŀ������
  bool FindValidCard(tagAnalyseResult& CurOutCard,tagAnalyseResult& cbPerOutCard);
  //��������
  bool FillCardCount(tagAnalyseResult&AnalyseResultEx,int nMinCount);
  //�������
  bool FillCardSeque(tagAnalyseResult&AnalyseResult,int cbMinCount,int cbMinLink,bool bAllowCycle);
  //�����˿�
  VOID AnalysebCardData(const BYTE cbCardData[],int cbCardCount,tagAnalyseResult& AnalyseResult);
  //�����˿�
  VOID AnalyseInitCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseInitResult & AnalyseResult);
  //��������
  bool FindLinkCard(tagAnalyseResult& AnalyseResult,BYTE cbMinCardCount,BYTE cbMinLinkLong,bool bLockCount,bool bAllowCycle);
  //�����˿�
  BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
  //�����ֲ�
  VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
  //����˳��
  bool SearchLinkCard(const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbFirstCardData,const BYTE cbLastCardData,tagOutCardResult &OutCardResult);
  //����ͬ��
  bool SearchSamePointCard(const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbCardData,tagOutCardResult &OutCardResult);
  //�Ƿ���ը��(û��������ͬ���� ��������С��)
  bool HasNotBombCard(BYTE cbHandCardData[],BYTE cbHandCardCount);
  //�Ƿ���nSameCount�����ϵ���
  bool HasNSameCountCard(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE nSameCount);
  //ʣ���ֿɳ���
  bool AnalyseTakeOverCount(const BYTE cbCardData[], BYTE cbCardCount,tagOutCountResult & OutCountResult);


  //�б�����
  VOID RandCardListData(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //�б�����������
  VOID RandCardListNoLink(BYTE cbCardBuffer[], BYTE cbBufferCount,BYTE cbNeedCount);
  //��ȡը������
  bool GetBombCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount,BYTE cbBombCount,BYTE cbSameCount,
                       BYTE cbMaxSameCount,BYTE cbMaxSameCountLimit,bool bLimitMaxBombCount=false);
  //��ȡ0ը����
  VOID GetZeroBombCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount);
  //��ȡ3ը��������
  VOID GetLessThreeBombCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount,BYTE cbBombCount);
  //��ȡ��������
  VOID GetSixXiangCardData(BYTE cbCardBuffer[], BYTE & cbBufferCount);
  //��ȡ��������
  VOID GetSevenXiangCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount);
  //��ȡ��������
  VOID GetEightXiangCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount);
  //��ȡ��������
  VOID GetNineXiangCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount);
  //��ȡʮ������
  VOID GetTenXiangCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount);
  //��ȡʮһ������
  VOID GetElevenXiangCardData(BYTE cbCardBuffer[], BYTE &  cbBufferCount);
  //��ȡʮ��������
  VOID GetTwelveXiangCardData(BYTE cbCardBuffer[], BYTE & cbBufferCount);
  //��ȡn��ը����
  VOID GetLinkBombCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount,BYTE cbCardCount,BYTE cbLinkCount);
  //�������
  VOID RandomSetKingCard(BYTE cbCardBuffer[], BYTE cbBufferCount);
  //����������
  VOID GetOtherCardData(BYTE cbCardBuffer[],BYTE  & cbBufferCount,BYTE cbSameCount);
  //��ȡ������
  VOID GetXiangCardDataByIndex(BYTE cbCardBuffer[], BYTE  & cbBufferCount,BYTE cbXiangIndex);

};

//////////////////////////////////////////////////////////////////////////

#endif