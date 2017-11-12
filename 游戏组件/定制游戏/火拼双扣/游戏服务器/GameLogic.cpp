#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//��������
const BYTE cbIndexCount=5;
/*
//�˿�����
const BYTE  CGameLogic::m_cbCardData[FULL_COUNT]=
{
       0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,     //���� A - K
     0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,     //÷�� A - K
     0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,     //���� A - K
  0x31,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,  //���� A - K
};
*/

//�˿�����

//����λ��ʾ���ͼ���,����λ��ʾ�õȼ������ڲ��Ƿ��пɱ��ԣ��ַ���ȱ�ʾ�пɱ���
const BYTE  CGameLogic::m_cbCardGrade[11]= {0x00,0x11,0x12,0x13,0x14,0x15,0x16,0x20,0x20,0x20,0x20};

const BYTE  CGameLogic::m_cbCardData[FULL_COUNT]=
{
  0x01,0x11,0x21,0x31,
  0x02,0x12,0x22,0x32,
  0x03,0x13,0x23,0x33,
  0x04,0x14,0x24,0x34,
  0x05,0x15,0x25,0x35,
  0x06,0x16,0x26,0x36,
  0x07,0x17,0x27,0x37,
  0x08,0x18,0x28,0x38,
  0x09,0x19,0x29,0x39,
  0x0A,0x1A,0x2A,0x3A,
  0x0B,0x1B,0x2B,0x3B,
  0x0C,0x1C,0x2C,0x3C,
  0x0D,0x1D,0x2D,0x3D,0x4E,0x4F, //��С��
  0x01,0x11,0x21,0x31,
  0x02,0x12,0x22,0x32,
  0x03,0x13,0x23,0x33,
  0x04,0x14,0x24,0x34,
  0x05,0x15,0x25,0x35,
  0x06,0x16,0x26,0x36,
  0x07,0x17,0x27,0x37,
  0x08,0x18,0x28,0x38,
  0x09,0x19,0x29,0x39,
  0x0A,0x1A,0x2A,0x3A,
  0x0B,0x1B,0x2B,0x3B,
  0x0C,0x1C,0x2C,0x3C,
  0x0D,0x1D,0x2D,0x3D,0x4E,0x4F //��С��
};

//ը��Ȩֵת���� ��8λ��ʾ����ը��,��8λ��ʾ������ը������
const BYTE  CGameLogic::m_cbTransTable[9][9]=
{
  0x00,0x03,0x04,0x35,0x46,0x57,0x60,0x00,0x00,
  0x00,0x00,0x02,0x03,0x34,0x45,0x50,0x00,0x00,
  0x00,0x00,0x00,0x02,0x03,0x34,0x40,0x50,0x00,
  0x00,0x00,0x00,0x00,0x02,0x03,0x30,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x30,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x30,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};



BYTE CGameLogic::m_cbLowCardData[13]= {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D};
BYTE CGameLogic::m_cbCardColor[8]= {0x00,0x10,0x20,0x30,0x00,0x10,0x20,0x30};
BYTE CGameLogic::m_cbKing[4]= {0x4E,0x4E,0x4F,0x4F};


//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
  m_cbArrayIndex=0;
  m_cbKingCount=0;
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount,BYTE cbSortType)
{
  //��Ŀ����
  if(cbCardCount==0)
  {
    return;
  }

  //ת����ֵ
  BYTE cbSortValue[MAX_COUNT];
  for(BYTE i=0; i<cbCardCount; i++)
  {
    cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
  }

  //�������
  bool bSorted=true;
  BYTE cbSwitchData=0,cbLast=cbCardCount-1;
  do
  {
    bSorted=true;
    for(BYTE i=0; i<cbLast; i++)
    {
      if((cbSortValue[i]<cbSortValue[i+1])||
         ((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
      {
        //���ñ�־
        bSorted=false;

        //�˿�����
        cbSwitchData=cbCardData[i];
        cbCardData[i]=cbCardData[i+1];
        cbCardData[i+1]=cbSwitchData;

        //����Ȩλ
        cbSwitchData=cbSortValue[i];
        cbSortValue[i]=cbSortValue[i+1];
        cbSortValue[i+1]=cbSwitchData;
      }
    }
    cbLast--;
  }
  while(bSorted==false);


  //��Ŀ����
  if(cbSortType==ST_COUNT)
  {
    //��������
    BYTE cbCardIndex=0;

    //�����˿�
    tagAnalyseInitResult AnalyseResult;
    AnalyseInitCardData(&cbCardData[cbCardIndex],cbCardCount-cbCardIndex,AnalyseResult);

    //��ȡ�˿�
    for(BYTE i=0; i<CountArray(AnalyseResult.cbBlockCount); i++)
    {
      BYTE cbNewIndex=CountArray(AnalyseResult.cbBlockCount)-i-1;

      //if(AnalyseResult.cbKingCount==cbNewIndex+1)
      //{
      //  BYTE cbKingData[2]={0x4F,0x4F};
      //  CopyMemory(&cbCardData[cbCardIndex],cbKingData,AnalyseResult.cbKingCount*sizeof(BYTE));

      //  //��������
      //  cbCardIndex+=AnalyseResult.cbKingCount*sizeof(BYTE);
      //}

      //if(AnalyseResult.cbSubKingCount==cbNewIndex+1)
      //{
      //  BYTE cbSubKing[2]={0x4E,0x4E};
      //  CopyMemory(&cbCardData[cbCardIndex],cbSubKing,AnalyseResult.cbSubKingCount*sizeof(BYTE));

      //  //��������
      //  cbCardIndex+=AnalyseResult.cbSubKingCount*sizeof(BYTE);
      //}


      //�����˿�
      BYTE cbIndex=CountArray(AnalyseResult.cbBlockCount)-i-1;
      CopyMemory(&cbCardData[cbCardIndex],AnalyseResult.cbCardData[cbIndex],AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE));

      //��������
      cbCardIndex+=AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE);
    }
  }

  return;
}

BYTE CGameLogic::RandCardList(BYTE cbCardList[],BYTE cbBufferCount,BYTE cbMainCard,WORD cbOutPut[])
{
  //����׼��
  BYTE cbCardData[sizeof(BYTE)*FULL_COUNT];
  CopyMemory(cbCardData,cbCardList,sizeof(cbCardData));

  //�����˿�
  BYTE cbRandCount=0,cbPosition=0;
  BYTE cbOutCount=0;
  do
  {
    cbPosition=rand()%(cbBufferCount-cbRandCount);
    //��¼���
    if(cbCardData[cbPosition]==cbMainCard)
    {
      cbOutPut[cbOutCount++]=cbRandCount;
    }

    cbCardList[cbRandCount++]=cbCardData[cbPosition];
    cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
  }
  while(cbRandCount<cbBufferCount);
  return cbOutCount;
}


//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
  BYTE cbPosition=0;
  BYTE cbRandCount=0;
  BYTE cbTempRandCount=0;
  //����׼��
  BYTE cbCardData[CountArray(m_cbCardData)];
  CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

  do
  {
    cbTempRandCount=0;
    cbPosition=rand()%(cbBufferCount-cbRandCount);
    cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
    cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
    cbTempRandCount++;
    while((cbPosition<cbBufferCount-cbRandCount) && (cbTempRandCount<4))
    {
      cbPosition++;
      cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
      cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
      cbTempRandCount++;
    }

  }
  while(cbRandCount<cbBufferCount);


  //�������
  BYTE cbTemp=0;
  BYTE cbArray[4]= {0,1,2,3};
  memcpy(cbCardData,cbCardBuffer,sizeof(cbCardData));
  for(int nIndex=0; nIndex<4; nIndex++)
  {
    cbTemp=rand()%(4-nIndex);
    memcpy(cbCardBuffer+nIndex*MAX_COUNT,cbCardData+cbArray[cbTemp]*MAX_COUNT,sizeof(BYTE)*MAX_COUNT);
    cbArray[cbTemp]=cbArray[3-nIndex];
  }
  return;
}


//��ȡ�˿�
VOID CGameLogic::GetRandCardList(BYTE cbInitCardList[], BYTE cbInitCardCount,BYTE cbCardBuffer[], BYTE cbBufferCount)
{
  if(cbBufferCount>cbInitCardCount)
  {
    return;
  }

  //����׼��
  //BYTE cbCardData[FULL_COUNT];
  //ZeroMemory(cbCardData,sizeof(cbCardData));
  //CopyMemory(cbCardData,cbInitCardList,cbInitCardCount*sizeof(cbInitCardList[0]));


  CopyMemory(cbCardBuffer,cbInitCardList,cbBufferCount*sizeof(cbInitCardList[0]));



  ////�����˿�
  //BYTE cbRandCount=0,cbPosition=0;
  //do
  //{
  //  cbPosition=rand()%(cbBufferCount-cbRandCount);
  //  cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
  //  cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
  //} while (cbRandCount<cbBufferCount);

  return;
}


//��������
void CGameLogic::RandArray(INT nArrayOne[], INT nCountOne, INT nArrayTwo[], INT nCountTwo)
{
  ASSERT(nCountOne == nCountTwo);
  if(nCountTwo != nCountOne)
  {
    return;
  }

  DWORD wTick = GetTickCount();
  for(int i = 1; i < nCountOne; ++i)
  {
    int nTempIndex = (rand()+wTick)%nCountOne;

    int nTempValueOne = nArrayOne[i];
    nArrayOne[i] = nArrayOne[nTempIndex];
    nArrayOne[nTempIndex] = nTempValueOne;

    int nTempValueTwo = nArrayTwo[i];
    nArrayTwo[i] = nArrayTwo[nTempIndex];
    nArrayTwo[nTempIndex] = nTempValueTwo;
  }
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
  //��������
  ASSERT(cbRemoveCount<=cbCardCount);

  //�������
  BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
  if(cbCardCount>CountArray(cbTempCardData))
  {
    return false;
  }
  CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

  //�����˿�
  for(BYTE i=0; i<cbRemoveCount; i++)
  {
    for(BYTE j=0; j<cbCardCount; j++)
    {
      if(cbRemoveCard[i]==cbTempCardData[j])
      {
        cbDeleteCount++;
        cbTempCardData[j]=0;
        break;
      }
    }
  }
  if(cbDeleteCount!=cbRemoveCount)
  {
    NcaTextOut(TEXT("ɾ���˿����ݳ���"));
    return false;
  }

  ////�����˿�
  //BYTE cbCardPos=0;
  //for (BYTE i=0;i<cbCardCount;i++)
  //{
  //  if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
  //}

  return true;
}


//ɾ���˿�
bool CGameLogic::RemoveCardEx(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
  //��������
  ASSERT(cbRemoveCount<=cbCardCount);

  //�������
  BYTE cbDeleteCount=0,cbTempCardData[FULL_COUNT];
  ZeroMemory(cbTempCardData,sizeof(cbTempCardData));

  CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

  //�����˿�
  for(BYTE i=0; i<cbRemoveCount; i++)
  {
    for(BYTE j=0; j<cbCardCount; j++)
    {
      if(cbRemoveCard[i]==cbTempCardData[j])
      {
        cbDeleteCount++;
        cbTempCardData[j]=0;
        break;
      }
    }
  }
  if(cbDeleteCount!=cbRemoveCount)
  {
    return false;
  }

  ZeroMemory(cbCardData,sizeof(cbCardData));

  //�����˿�
  BYTE cbCardPos=0;
  for(BYTE i=0; i<cbCardCount; i++)
  {
    if(cbTempCardData[i]!=0)
    {
      cbCardData[cbCardPos++]=cbTempCardData[i];
    }
  }

  return true;
}

BYTE CGameLogic::TransForLogicValue(BYTE cbCardData)
{
  if(cbCardData>=14)
  {
    cbCardData-=13;
  }

  //�����ɫ
  switch(rand()%4)
  {
    case 0:
      cbCardData=cbCardData|0x00;
      break;
    case 1:
      cbCardData=cbCardData|0x10;
      break;
    case 2:
      cbCardData=cbCardData|0x20;
      break;
    case 3:
      cbCardData=cbCardData|0x30;
      break;
  }
  return cbCardData;
}

BYTE CGameLogic::GetMaxBombGrade(BYTE *cbBomblist)
{

  BYTE cbResultBombList[BOMB_TYPE_COUNT];
  ZeroMemory(cbResultBombList,BOMB_TYPE_COUNT);
  memcpy(cbResultBombList,cbBomblist,BOMB_TYPE_COUNT);

  TransBombGrade(cbResultBombList);

  BYTE cbMaxBombCrade=0;
  BYTE cbIndex=0;
  for(cbIndex=0; cbIndex<BOMB_TYPE_COUNT; cbIndex++)
  {
    if(cbResultBombList[cbIndex]>0)
    {
      cbMaxBombCrade=cbIndex;
    }
  }
  return cbMaxBombCrade;
}

//Ԥ����
bool CGameLogic::PreDealBombList(BYTE cbBombList[])
{

  if(cbBombList[1]<=1&&cbBombList[2]<=1&&cbBombList[3]<=1)
  {
    if(cbBombList[0]+cbBombList[1]+cbBombList[2]+cbBombList[3]>=6)
    {
      cbBombList[0]=cbBombList[0]+cbBombList[1]+cbBombList[2]+cbBombList[3];
      cbBombList[1]=0;
      cbBombList[2]=0;
      cbBombList[3]=0;
      return true;
    }
  }
  //��6��
  if(cbBombList[2]>=1&&cbBombList[0]+cbBombList[1]+cbBombList[2]<cbBombList[2]+4)
  {
    return true;
  }
  //��7��
  if(cbBombList[3]>=1&&cbBombList[0]+cbBombList[1]+cbBombList[2]+cbBombList[3]<cbBombList[3]+5)
  {
    return true;
  }




  BYTE cbResultBombList[BOMB_TYPE_COUNT];
  ZeroMemory(cbResultBombList,BOMB_TYPE_COUNT);
  memcpy(cbResultBombList,cbBombList,BOMB_TYPE_COUNT);

  int nVIndex=0;
  int nHIndex=0;
  //Ԥ����
  for(nHIndex=0; nHIndex<BOMB_TYPE_COUNT-1; nHIndex++)
  {

    int nMaxXiang=0;
    BYTE cbInitBombList[BOMB_TYPE_COUNT];
    BYTE cbNewBombList[BOMB_TYPE_COUNT];
    ZeroMemory(cbInitBombList,BOMB_TYPE_COUNT);
    ZeroMemory(cbNewBombList,BOMB_TYPE_COUNT);
    memcpy(cbInitBombList,cbBombList,BOMB_TYPE_COUNT);
    memcpy(cbNewBombList,cbBombList,BOMB_TYPE_COUNT);

    for(int j=1; j<4; j++)
    {
      if(nHIndex+j>=BOMB_TYPE_COUNT)
      {
        break;
      }

      if(cbBombList[nHIndex]>=3&&cbBombList[nHIndex+j]>=1)
      {
        nMaxXiang=GetMaxBombGrade(cbInitBombList,nHIndex+j);
        for(int i=0; i<cbBombList[nHIndex+j]; i++)
        {
          cbNewBombList[nHIndex]++;
          cbNewBombList[nHIndex+j]--;

          if(GetMaxBombGrade(cbNewBombList,nHIndex+j+1)>nMaxXiang)
          {
            memcpy(cbResultBombList,cbNewBombList,BOMB_TYPE_COUNT);
          }

        }

        memcpy(cbBombList,cbInitBombList,BOMB_TYPE_COUNT);
      }

    }
  }

  memcpy(cbBombList,cbResultBombList,BOMB_TYPE_COUNT);

  //�ж�ת���Ƿ���ȷ
  {
    int allCount=0;
    for(int i=0; i<BOMB_TYPE_COUNT; i++)
    {
      if(cbBombList[i]*(4+i)>MAX_COUNT)
      {
        return false;
      }
      allCount+=cbBombList[i]*(4+i);
    }

    if(allCount>MAX_COUNT)
    {
      return false;
    }
  }

  return true;
}

bool CGameLogic::TransBombGrade(BYTE cbBombList[])
{

  NcaTextOut(TEXT("ת��ǰ:"));
  CString strInfo=TEXT("");
  for(int i=0; i<BOMB_TYPE_COUNT; i++)
  {
    CString strOne=TEXT("");
    strOne.Format(TEXT("%d��:%x�� "),i+4,cbBombList[i]);
    strInfo+=strOne;
  }
  NcaTextOut(strInfo);

  int nVIndex=0;
  int nHIndex=0;

  BYTE cbResultBombList[BOMB_TYPE_COUNT];
  ZeroMemory(cbResultBombList,BOMB_TYPE_COUNT);
  memcpy(cbResultBombList,cbBombList,BOMB_TYPE_COUNT);

  if(PreDealBombList(cbResultBombList))
  {
    memcpy(cbBombList,cbResultBombList,BOMB_TYPE_COUNT);
  }

  for(nHIndex=0; nHIndex<BOMB_TYPE_COUNT; nHIndex++)
  {
    for(nVIndex=BOMB_TYPE_COUNT-1; nVIndex>=nHIndex; nVIndex--)
    {
      BYTE cbTemp=m_cbTransTable[nHIndex][nVIndex];
      if(cbTemp>0&&(cbTemp&0x0F)>0)
      {
        if(cbBombList[nHIndex]>=(cbTemp&0x0F))
        {
          cbBombList[nHIndex]-=(cbTemp&0x0F);
          cbBombList[nVIndex]+=1;
        }
      }
    }
  }


  NcaTextOut(TEXT("ת����:"));
  strInfo=TEXT("");
  for(int i=0; i<BOMB_TYPE_COUNT; i++)
  {
    CString strOne=TEXT("");
    strOne.Format(TEXT("%d��:%x�� "),i+4,cbBombList[i]);
    strInfo+=strOne;
  }
  NcaTextOut(strInfo);

  return true;
}

//�õ���ϵ������
BYTE CGameLogic::GetMaxBombGrade(BYTE cbBombList[],BYTE cbListCount)
{
  int nVIndex=0;
  int nHIndex=0;

  int nMaxXiang=0;
  for(nHIndex=0; nHIndex<cbListCount; nHIndex++)
  {
    for(nVIndex=BOMB_TYPE_COUNT-1; nVIndex>=nHIndex; nVIndex--)
    {
      BYTE cbTemp=m_cbTransTable[nHIndex][nVIndex];
      if(cbTemp>0&&(cbTemp&0x0F)>0)
      {
        if(cbBombList[nHIndex]>=(cbTemp&0x0F))
        {

          cbBombList[nHIndex]-=(cbTemp&0x0F);
          cbBombList[nVIndex]+=1;

          if(nMaxXiang<nVIndex)
          {
            nMaxXiang=nVIndex;
          }
        }
      }
    }
  }

  return nMaxXiang;

}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
  //�˿�����
  BYTE cbReturn=0;
  BYTE m_cbCardColor=GetCardColor(cbCardData);
  BYTE cbCardValue=GetCardValue(cbCardData);

  if(m_cbCardColor==0x40)
  {
    cbReturn=cbCardValue+2;
  }
  else
  {
    cbReturn=(cbCardValue<=2)?(cbCardValue+13):cbCardValue;
  }
  //ת����ֵ
  return cbReturn;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValueEx(BYTE cbCardData)
{
  //�˿�����
  BYTE cbReturn=0;
  BYTE m_cbCardColor=GetCardColor(cbCardData);
  BYTE cbCardValue=GetCardValue(cbCardData);

  if(m_cbCardColor==0x40)
  {
    cbReturn=cbCardValue+2;
  }
  else
  {
    cbReturn=(cbCardValue<=1)?(cbCardValue+13):cbCardValue;
  }
  //ת����ֵ
  return cbReturn;
}

//�Ա������˿�
bool CGameLogic::CompareHandCard(const BYTE cbFirstHandCardData[], BYTE cbFirstHandCardCount, const BYTE cbNextHandCardData[],BYTE cbNextHandCardCount,BYTE bBombList[BOMB_TYPE_COUNT])
{
  //�����˿�
  tagAnalyseInitResult AnalyseFirstResult,AnalyseNextResult;
  ZeroMemory(&AnalyseFirstResult,sizeof(AnalyseFirstResult));
  ZeroMemory(&AnalyseNextResult,sizeof(AnalyseNextResult));

  //��������
  int nFirstUserXiang = 0;
  int nNextUserXiang = 0;


  BYTE bSubKingCount = 0;
  BYTE bKingCount = 0;

  //��ը��ѯ
  BYTE bFistBombList[BOMB_TYPE_COUNT] = {0};
  BYTE bNextBombList[BOMB_TYPE_COUNT] = {0};

  tagAnalyseResult tempAnalyseResult;
  tagAnalyseInitResult LinkBombAnalyseResult;

  BYTE bTempFirstHandCardCount = cbFirstHandCardCount;
  BYTE bTempNextHandCardCount = cbNextHandCardCount;
  BYTE bTempFirstHandCardData[MAX_COUNT],bTempNextHandCardData[MAX_COUNT];

  ZeroMemory(bTempFirstHandCardData,sizeof(bTempFirstHandCardData));
  ZeroMemory(bTempNextHandCardData,sizeof(bTempNextHandCardData));
  CopyMemory(bTempFirstHandCardData,cbFirstHandCardData,sizeof(bTempFirstHandCardData));
  CopyMemory(bTempNextHandCardData,cbNextHandCardData,sizeof(bTempNextHandCardData));



  //�׼�����
  for(BYTE i=8; i>=4; i--)
  {
    //��ը����
    ZeroMemory(&tempAnalyseResult,sizeof(tempAnalyseResult));
    ZeroMemory(&LinkBombAnalyseResult,sizeof(LinkBombAnalyseResult));
    if(SearchSameLinkCard(bTempFirstHandCardData,bTempFirstHandCardCount,i,3,LinkBombAnalyseResult))
    {
      AnalysebCardData(LinkBombAnalyseResult.cbCardData[i-1],LinkBombAnalyseResult.cbBlockCount[i-1],tempAnalyseResult);

      //����ը��
      IsBombCard(tempAnalyseResult);
      bFistBombList[tempAnalyseResult.m_cbBombGrade-1]++;

      //ɾ���˿�
      if(RemoveCardEx(LinkBombAnalyseResult.cbCardData[i-1],LinkBombAnalyseResult.cbBlockCount[i-1],bTempFirstHandCardData,bTempFirstHandCardCount))
      {
        bTempFirstHandCardCount -= LinkBombAnalyseResult.cbBlockCount[i-1];
      }
    }
  }

  //��ը����
  AnalyseInitCardData(bTempFirstHandCardData,bTempFirstHandCardCount,AnalyseFirstResult);


  bSubKingCount = 0;
  bKingCount = 0;
  for(BYTE i=0; i<bTempFirstHandCardCount; i++)
  {
    if(bTempFirstHandCardData[i] == 0x4E)
    {
      bSubKingCount++;
    }

    if(bTempFirstHandCardData[i] == 0x4F)
    {
      bKingCount++;
    }
  }

  //����ը����
  for(BYTE i=1; i<=8; i++)
  {
    ZeroMemory(&tempAnalyseResult,sizeof(tempAnalyseResult));
    if(bKingCount + bSubKingCount + i> 5)
    {

      //3��6��
      if(bKingCount + bSubKingCount ==3)
      {
        //���Ƽ�����С������
        if(bKingCount + bSubKingCount + i < 6)
        {
          continue;
        }
      }

      //4��7��
      if(bKingCount + bSubKingCount ==4)
      {
        //���Ƽ�����С������
        if(bKingCount + bSubKingCount + i < 7)
        {
          continue;
        }
      }

      BYTE bCardData[MAX_COUNT];
      BYTE bCountIndex = i-1;
      CopyMemory(bCardData,AnalyseFirstResult.cbCardData[i-1],i);

      if(bKingCount>0)
      {
        for(BYTE k=0; k<bKingCount; k++)
        {
          bCardData[bCountIndex++] = 0x4F;
        }
      }

      if(bSubKingCount>0)
      {
        for(BYTE k=0; k<bSubKingCount; k++)
        {
          bCardData[bCountIndex++] = 0x4E;
        }
      }

      AnalysebCardData(bCardData,bCountIndex,tempAnalyseResult);

      //����ը��
      IsBombCard(tempAnalyseResult);

      if(tempAnalyseResult.m_cbBombGrade>0)
      {
        bFistBombList[tempAnalyseResult.m_cbBombGrade-1]++;

        if(RemoveCardEx(AnalyseFirstResult.cbCardData[i-1],AnalyseFirstResult.cbBlockCount[i-1],bTempFirstHandCardData,bTempFirstHandCardCount))
        {
          bTempFirstHandCardCount -= AnalyseFirstResult.cbBlockCount[i-1];
        }

        break;
      }
    }

    //����ը
    if(bKingCount + bSubKingCount==3)
    {
      bFistBombList[6-1]++;
    }
    if(bKingCount + bSubKingCount==4)
    {
      bFistBombList[7-1]++;
    }
  }

  //��ը����
  for(BYTE i=4; i<8; i++)
  {
    if(AnalyseFirstResult.cbBlockCount[i]>0)
    {
      bFistBombList[i]+=AnalyseFirstResult.cbBlockCount[i];
    }
  }

  //�¼�����
  for(BYTE i=8; i>=4; i--)
  {
    ZeroMemory(&tempAnalyseResult,sizeof(tempAnalyseResult));
    ZeroMemory(&LinkBombAnalyseResult,sizeof(LinkBombAnalyseResult));
    if(SearchSameLinkCard(bTempNextHandCardData,cbNextHandCardCount,i,3,LinkBombAnalyseResult))
    {
      AnalysebCardData(LinkBombAnalyseResult.cbCardData[i-1],LinkBombAnalyseResult.cbBlockCount[i-1],tempAnalyseResult);

      //����ը��
      IsBombCard(tempAnalyseResult);
      bNextBombList[tempAnalyseResult.m_cbBombGrade-1]++;

      //ɾ���˿�
      if(RemoveCardEx(LinkBombAnalyseResult.cbCardData[i-1],LinkBombAnalyseResult.cbBlockCount[i-1],bTempNextHandCardData,bTempNextHandCardCount))
      {
        bTempNextHandCardCount -= LinkBombAnalyseResult.cbBlockCount[i-1];
      }
    }
  }

  //��ըͳ��
  AnalyseInitCardData(bTempNextHandCardData,bTempNextHandCardCount,AnalyseNextResult);

  bSubKingCount = 0;
  bKingCount = 0;
  for(BYTE i=0; i<bTempNextHandCardCount; i++)
  {
    if(bTempNextHandCardData[i] == 0x4E)
    {
      bSubKingCount++;
    }

    if(bTempNextHandCardData[i] == 0x4F)
    {
      bKingCount++;
    }
  }

  //����ը����
  for(BYTE i=1; i<=8; i++)
  {
    ZeroMemory(&tempAnalyseResult,sizeof(tempAnalyseResult));
    if(bKingCount + bSubKingCount + i> 5)
    {

      //3��6��
      if(bKingCount + bSubKingCount ==3)
      {
        //���Ƽ�����С������
        if(bKingCount + bSubKingCount + i < 6)
        {
          continue;
        }
      }

      //4��7��
      if(bKingCount + bSubKingCount ==4)
      {
        //���Ƽ�����С������
        if(bKingCount + bSubKingCount + i < 7)
        {
          continue;
        }
      }

      BYTE bCardData[MAX_COUNT];
      BYTE bCountIndex = i-1;
      CopyMemory(bCardData,AnalyseNextResult.cbCardData[i-1],i);

      if(bKingCount>0)
      {
        for(BYTE k=0; k<bKingCount; k++)
        {
          bCardData[bCountIndex++] = 0x4F;
        }
      }

      if(bSubKingCount>0)
      {
        for(BYTE k=0; k<bSubKingCount; k++)
        {
          bCardData[bCountIndex++] = 0x4E;
        }
      }

      AnalysebCardData(bCardData,bCountIndex,tempAnalyseResult);

      //����ը��
      IsBombCard(tempAnalyseResult);
      if(tempAnalyseResult.m_cbBombGrade>0)
      {
        bNextBombList[tempAnalyseResult.m_cbBombGrade-1]++;
        if(RemoveCardEx(AnalyseNextResult.cbCardData[i-1],AnalyseNextResult.cbBlockCount[i-1],bTempNextHandCardData,bTempNextHandCardCount))
        {
          bTempNextHandCardCount -= AnalyseNextResult.cbBlockCount[i-1];
        }

        break;
      }
    }

    //����ը
    if(bKingCount + bSubKingCount==3)
    {
      bNextBombList[6-1]++;
    }
    if(bKingCount + bSubKingCount==4)
    {
      bNextBombList[7-1]++;
    }
  }

  for(BYTE i=4; i<8; i++)
  {
    if(AnalyseNextResult.cbBlockCount[i]>0)
    {
      bNextBombList[i]+=AnalyseNextResult.cbBlockCount[i];
    }
  }

  //6�����²��㹱�׷֣����Դ�6����
  for(BYTE i=5; i<8; i++)
  {
    nFirstUserXiang +=bFistBombList[i];
    nNextUserXiang +=bNextBombList[i];
  }

  //����ը���б�
  for(BYTE i=0; i<BOMB_TYPE_COUNT; i++)
  {
    bBombList[i] = bNextBombList[i];
  }

  //���׷ִ����¼�
  if(nFirstUserXiang>nNextUserXiang)
  {
    return true;
  }
  else if(nFirstUserXiang==nNextUserXiang)
  {
    //����Ƚ�
    if(bFistBombList[4]>bNextBombList[4])
    {
      return true;
    }
    //����Ƚ�
    if(bFistBombList[3]>bNextBombList[3])
    {
      return true;
    }
  }

  return false;
}

//�Ա��˿�
bool CGameLogic::CompareCard(tagAnalyseResult& CurOutCard,tagAnalyseResult& PerOutCard)
{
  //���Ҷ�Ӧ��Ч����
  bool bReturn=false;
  if(FindValidCard(CurOutCard,PerOutCard))
  {
    BYTE  cbPerType=PerOutCard.m_cbCardType;
    BYTE  cbCurType=CurOutCard.m_cbCardType;
    BYTE  cbPerMinCard=GetCardLogicValue(PerOutCard.m_CardStyle[PerOutCard.m_cbMinIndex].m_cbCard);
    BYTE  cbCurMinCard=GetCardLogicValue(CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCard);

    if((m_cbCardGrade[cbPerType]&0xF0)<(m_cbCardGrade[cbCurType]&0xF0))
    {
      bReturn=true;
    }
    else
    {
      if((m_cbCardGrade[cbPerType]&0xF0)==(m_cbCardGrade[cbCurType]&0xF0))
      {
        switch(cbPerType)
        {
          case CT_SINGLE:       //��������
          case CT_DOUBLE:       //��������
          case CT_THREE:        //��������
          case CT_SINGLE_LINK:    //��������
          case CT_DOUBLE_LINK:    //���ö���
          case CT_THREE_LINK:     //��������
          {
            //��Ŀ�ж�
            if(PerOutCard.m_cbCardCount==CurOutCard.m_cbCardCount)
            {
              bReturn=(cbCurMinCard>cbPerMinCard)?true:false;
            }
            break;
          }
          case CT_BOMB:
          case CT_BOMB_3W:
          case CT_BOMB_TW:
          case CT_BOMB_LINK:
          {

            if(CurOutCard.m_cbBombGrade!=PerOutCard.m_cbBombGrade)
            {
              bReturn=(CurOutCard.m_cbBombGrade>PerOutCard.m_cbBombGrade)?true:false;
            }
            else
            {
              //Ȩֵ��������жϵ����������
              if(cbPerMinCard!=cbCurMinCard)
              {
                bReturn=(cbCurMinCard>cbPerMinCard)?true:false;

                return bReturn;
              }


              if(CurOutCard.m_cbBombGrade==PerOutCard.m_cbBombGrade)
              {
                if(CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCount!=PerOutCard.m_CardStyle[PerOutCard.m_cbMinIndex].m_cbCount)
                {
                  bReturn=(CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCount>PerOutCard.m_CardStyle[PerOutCard.m_cbMinIndex].m_cbCount)?true:false;
                }
                else
                {
                  if(cbPerType==CT_BOMB_LINK)
                  {
                    bReturn=(GetCardLogicValueEx(CurOutCard.m_CardStyle[CurOutCard.m_cbMaxIndex].m_cbCard)>GetCardLogicValueEx(PerOutCard.m_CardStyle[PerOutCard.m_cbMaxIndex].m_cbCard))?true:false;
                  }
                  else
                  {
                    if(cbPerType==CT_BOMB)
                    {
                      if(CurOutCard.m_nJockerCount==0&&PerOutCard.m_nJockerCount>0)
                      {
                        return true;
                      }
                      if(CurOutCard.m_nJockerCount>0&&PerOutCard.m_nJockerCount==0)
                      {
                        return false;
                      }

                    }
                    bReturn=(GetCardLogicValue(CurOutCard.m_CardStyle[CurOutCard.m_cbMaxIndex].m_cbCard)>GetCardLogicValue(PerOutCard.m_CardStyle[PerOutCard.m_cbMaxIndex].m_cbCard))?true:false;
                  }
                }
              }

            }
            break;
          }
        }
      }
    }
  }

  return bReturn;
}

//��������
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,BYTE cbPlayerCount, tagOutCardResult & OutCardResult)
{
  if(cbTurnCardCount>0)
  {
    //���ý��
    ZeroMemory(&OutCardResult,sizeof(OutCardResult));

    //��ȡ����
    tagAnalyseResult AnalyseResult,TempResult;
    memset(&AnalyseResult,0,sizeof(AnalyseResult));
    memset(&TempResult,0,sizeof(TempResult));
    AnalysebCardData(cbTurnCardData,cbTurnCardCount,AnalyseResult);
    FindValidCard(AnalyseResult,TempResult);

    tagAnalyseInitResult AnalyseInitResult;
    ZeroMemory(&AnalyseInitResult,sizeof(AnalyseInitResult));
    AnalyseInitCardData(cbHandCardData,cbHandCardCount,AnalyseInitResult);

    //��������
    BYTE bSubKingCount = 0;
    BYTE bKingCount = 0;
    for(BYTE i=0; i<cbHandCardCount; i++)
    {
      if(cbHandCardData[i] == 0x4E)
      {
        bSubKingCount++;
      }

      if(cbHandCardData[i] == 0x4F)
      {
        bKingCount++;
      }
    }

    //��ըѹ��
    BYTE cbAllKinCount=bSubKingCount +bKingCount;

    //��ȡ�˿�
    //��ͨը������ͬ�������ҿ���ѹס����
    for(BYTE i=0; i<CountArray(AnalyseInitResult.cbBlockCount); i++)
    {
      if(AnalyseInitResult.cbBlockCount[i]>0)
      {
        for(BYTE j=AnalyseInitResult.cbBlockCount[i]-1; j>=0; j--)
        {
          if(j<0 || j==0xFF)
          {
            break;
          }

          //��Ҫ���Ƴ�ը�����ø��๱�׷�
          if(AnalyseInitResult.cbCardData[i][j*(i+1)] == 0x4E || AnalyseInitResult.cbCardData[i][j*(i+1)] == 0x4F)
          {
            continue;
          }

          //�Լҳ�����ֻʣһ����,������ը���ȳ�ը����
          if(cbPlayerCount == 1)
          {
            if(i==0)
            {
              for(BYTE n=0; n<CountArray(AnalyseInitResult.cbBlockCount); n++)
              {
                if(AnalyseInitResult.cbBlockCount[n] >0)
                {
                  if(bKingCount+bSubKingCount+n+1 >= 4)
                  {
                    continue;
                  }
                }
              }
            }
          }

          //�����˿�
          BYTE cbNewCardData[MAX_COUNT];
          ZeroMemory(cbNewCardData,sizeof(cbNewCardData));
          BYTE cbNewCardCount=i+1;
          BYTE cbIndex=j*(i+1);

          for(BYTE n=0; n<cbNewCardCount; n++)
          {
            cbNewCardData[n] = AnalyseInitResult.cbCardData[i][cbIndex+n];
          }

          //��������һ���
          if(cbAllKinCount>0)
          {
            bool bMostCard = false;

            //��������
            for(BYTE n=i+1; n<CountArray(AnalyseInitResult.cbBlockCount); n++)
            {
              if(AnalyseInitResult.cbBlockCount[n]>0)
              {
                bMostCard = true;
              }
            }

            //��������
            for(BYTE n=cbIndex-1; n>=0; n--)
            {
              if(n<0 || n==0xFF)
              {
                break;
              }
              if(GetCardLogicValue(AnalyseInitResult.cbCardData[i][n])>GetCardLogicValue(AnalyseInitResult.cbCardData[i][cbIndex]))
              {
                bMostCard = true;
              }
            }

            //û�и����ƣ��������һ��
            if(!bMostCard)
            {
              //��Ӵ���
              for(BYTE k = 0; k<bKingCount; k++)
              {
                cbNewCardData[cbNewCardCount++] = 0x4F;
              }

              //���С��
              for(BYTE k = 0; k<bSubKingCount; k++)
              {
                cbNewCardData[cbNewCardCount++] = 0x4E;
              }
            }
          }

          //����ÿ��ѹ������ը����С�ƿ�ѡ��pass
          if(cbTurnCardCount<=2 && cbPlayerCount>MAX_COUNT/2)
          {
            //����ĵ���+���ƻ�ʣ3������
            int nSmallCardCount = AnalyseInitResult.cbBlockCount[0] + AnalyseInitResult.cbBlockCount[1];
            if(cbNewCardCount>=4 && nSmallCardCount>=3)
            {
              return false;
            }
          }

          //��ȡ����
          tagAnalyseResult AnalyseNewResult;
          ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
          AnalysebCardData(cbNewCardData,cbNewCardCount,AnalyseNewResult);

          if(CompareCard(AnalyseNewResult,AnalyseResult))
          {
            OutCardResult.cbCardCount=cbNewCardCount;
            CopyMemory(OutCardResult.cbResultCard,cbNewCardData,sizeof(BYTE)*cbNewCardCount);
            return true;
          }
        }


        //��2�Ե��ƣ�����
        if(cbTurnCardCount<=2 && (i+1>=cbTurnCardCount))
        {
          //���ţ�3������2
          for(int m=1; m<3; m++)
          {
            for(int j=0; j<AnalyseInitResult.cbBlockCount[m]; j++)
            {
              if((GetCardValue(AnalyseInitResult.cbCardData[m][j]) == 0x02) && (m+1>cbTurnCardCount))
              {
                //�����˿�
                BYTE cbNewCardData[MAX_COUNT];
                ZeroMemory(cbNewCardData,sizeof(cbNewCardData));

                for(BYTE n=0; n<cbTurnCardCount; n++)
                {
                  cbNewCardData[n] = AnalyseInitResult.cbCardData[m][j+n];
                }

                //��ȡ����
                tagAnalyseResult AnalyseNewResult;
                ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
                AnalysebCardData(cbNewCardData,cbTurnCardCount,AnalyseNewResult);

                if(CompareCard(AnalyseNewResult,AnalyseResult))
                {
                  OutCardResult.cbCardCount=cbTurnCardCount;
                  CopyMemory(OutCardResult.cbResultCard,cbNewCardData,sizeof(BYTE)*cbTurnCardCount);
                  return true;
                }
              }
            }
          }
        }
      }
    }

    //����ը����
    for(BYTE i=0; i<CountArray(AnalyseInitResult.cbBlockCount); i++)
    {
      for(BYTE j=0; j<AnalyseInitResult.cbBlockCount[i]; j++)
      {
        if(AnalyseInitResult.cbBlockCount[i]>0 && cbAllKinCount + i> 3)
        {
          BYTE cbCardData[12] = {0};
          BYTE cbCardIndex = 0;

          //3��6��
          if(cbAllKinCount ==3)
          {
            //���Ƽ�����С������
            if(bSubKingCount +bKingCount + i < 6)
            {
              continue;
            }
          }

          //4��7��
          if(cbAllKinCount ==4)
          {
            //���Ƽ�����С������
            if(bKingCount + bSubKingCount+ i < 7)
            {
              continue;
            }
          }

          //�����˿�
          BYTE cbNewCardCount=i+1;
          BYTE cbIndex=j*(i+1);

          for(BYTE n=0; n<cbNewCardCount; n++)
          {
            cbCardData[n] = AnalyseInitResult.cbCardData[i][cbIndex+n];
          }

          cbCardIndex += cbNewCardCount;

          //��Ӵ���
          for(BYTE k = 0; k<bKingCount; k++)
          {
            cbCardData[cbCardIndex++] = 0x4F;
          }

          //���С��
          for(BYTE k = 0; k<bSubKingCount; k++)
          {
            cbCardData[cbCardIndex++] = 0x4E;
          }

          //��ȡ����
          tagAnalyseResult AnalyseNewResult;
          ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
          AnalysebCardData(cbCardData,cbNewCardCount+cbAllKinCount,AnalyseNewResult);

          if(CompareCard(AnalyseNewResult,AnalyseResult))
          {
            OutCardResult.cbCardCount=cbNewCardCount+cbAllKinCount;
            CopyMemory(OutCardResult.cbResultCard,cbCardData,sizeof(BYTE)*(cbNewCardCount+cbAllKinCount));
            return true;
          }
        }
      }
    }

    //����ը
    if(cbAllKinCount >=3)
    {
      BYTE cbCardData[4] = {0};
      BYTE cbCardIndex = 0;

      for(BYTE k = 0; k< bSubKingCount; k++)
      {
        cbCardData[cbCardIndex] = 0x4E;
        cbCardIndex++;
      }

      for(BYTE k = 0; k< bKingCount; k++)
      {
        cbCardData[cbCardIndex] = 0x4F;
        cbCardIndex++;
      }

      //��ȡ����
      tagAnalyseResult AnalyseNewResult;
      ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
      AnalysebCardData(cbCardData,cbAllKinCount,AnalyseNewResult);

      if(CompareCard(AnalyseNewResult,AnalyseResult))
      {
        OutCardResult.cbCardCount=cbAllKinCount;
        CopyMemory(OutCardResult.cbResultCard,cbCardData,sizeof(BYTE)*cbAllKinCount);
        return true;
      }
    }


    //��ըѹ��
    tagAnalyseInitResult AnalyseTempResult;
    ZeroMemory(&AnalyseTempResult,sizeof(AnalyseTempResult));

    BYTE bTempCardData[MAX_COUNT];
    CopyMemory(bTempCardData,cbHandCardData,sizeof(bTempCardData));

    for(BYTE i=4; i<9; i++)
    {
      BYTE bSearchCardCount  = i;
      BYTE bSearchLineCount  = 3;

      if(SearchSameLinkCard(bTempCardData,cbHandCardCount,bSearchCardCount,bSearchLineCount,AnalyseTempResult))
      {
        //��ȡ����
        tagAnalyseResult AnalyseNewResult;
        ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
        AnalysebCardData(AnalyseTempResult.cbCardData[i-1],AnalyseTempResult.cbBlockCount[i-1],AnalyseNewResult);

        if(CompareCard(AnalyseNewResult,AnalyseResult))
        {
          OutCardResult.cbCardCount=AnalyseTempResult.cbBlockCount[bSearchCardCount-1];
          CopyMemory(OutCardResult.cbResultCard,AnalyseTempResult.cbCardData[i-1],sizeof(BYTE)*AnalyseTempResult.cbBlockCount[i-1]);
          return true;
        }
      }
    }

    //�������Բ����ٲ��Ƴ�
    ZeroMemory(&AnalyseInitResult,sizeof(AnalyseInitResult));
    AnalyseSameCardData((BYTE*)cbHandCardData,cbHandCardCount,cbTurnCardCount,AnalyseInitResult);

    //��ͨը������ͬ�������ҿ���ѹס����
    for(BYTE i=0; i<CountArray(AnalyseInitResult.cbBlockCount); i++)
    {
      if(i>=0&&AnalyseInitResult.cbBlockCount[i]>0)
      {
        for(BYTE j=AnalyseInitResult.cbBlockCount[i]-1; j>=0; j--)
        {
          if(j<0 || j==0xFF)
          {
            break;
          }

          //�����˿�
          BYTE cbNewCardData[MAX_COUNT];
          ZeroMemory(cbNewCardData,sizeof(cbNewCardData));
          BYTE cbNewCardCount=i+1;
          BYTE cbIndex=j*(i+1);

          for(BYTE n=0; n<cbNewCardCount; n++)
          {
            cbNewCardData[n] = AnalyseInitResult.cbCardData[i][cbIndex+n];
          }

          //��ȡ����
          tagAnalyseResult AnalyseNewResult;
          ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
          AnalysebCardData(cbNewCardData,cbNewCardCount,AnalyseNewResult);

          if(CompareCard(AnalyseNewResult,AnalyseResult))
          {
            OutCardResult.cbCardCount=cbNewCardCount;
            CopyMemory(OutCardResult.cbResultCard,cbNewCardData,sizeof(BYTE)*cbNewCardCount);
            return true;
          }
        }
      }
    }

    return false;
  }
  else if(cbTurnCardCount==0)     //�׳�
  {
    tagAnalyseInitResult AnalyseInitResult;
    ZeroMemory(&AnalyseInitResult,sizeof(AnalyseInitResult));
    AnalyseInitCardData(cbHandCardData,cbHandCardCount,AnalyseInitResult);

    BYTE bSubKingCount = 0;
    BYTE bKingCount = 0;
    for(BYTE i=0; i<cbHandCardCount; i++)
    {
      if(cbHandCardData[i] == 0x4E)
      {
        bSubKingCount++;
      }

      if(cbHandCardData[i] == 0x4F)
      {
        bKingCount++;
      }
    }

    //��ȡ�˿�
    for(BYTE i=0; i<CountArray(AnalyseInitResult.cbBlockCount); i++)
    {
      if(i>=0&&AnalyseInitResult.cbBlockCount[i]>0)
      {
        for(BYTE j=0; j<AnalyseInitResult.cbBlockCount[i]; j++)
        {
          //����������С��
          if(bSubKingCount+bKingCount != cbHandCardCount)
          {
            if(AnalyseInitResult.cbCardData[i][j*(i+1)] == 0x4E || AnalyseInitResult.cbCardData[i][j*(i+1)] == 0x4F)
            {
              continue;
            }
          }

          BYTE bSingelCard=0,bDoubleCard=0;
          if(bSubKingCount==1)
          {
            bSingelCard++;
          }
          if(bKingCount==1)
          {
            bSingelCard++;
          }
          if(bSubKingCount==2)
          {
            bDoubleCard++;
          }
          if(bKingCount==2)
          {
            bDoubleCard++;
          }

          if(cbPlayerCount == 3)
          {
            //ֻ�ȶԵ���˫����
            if(i == 0 && AnalyseInitResult.cbBlockCount[1]-bDoubleCard >=AnalyseInitResult.cbBlockCount[i]-bSingelCard)
            {
              continue;
            }
          }
          else if(cbPlayerCount == 2)
          {
            //ֻ�ȶԵ�����������
            if(i == 0 && AnalyseInitResult.cbBlockCount[2] >=AnalyseInitResult.cbBlockCount[i]-bSingelCard)
            {
              continue;
            }
          }
          else if(cbPlayerCount == 1)
          {
            //˫�������ĸ��������ȳ��ĸ�
            if(i == 1 && (AnalyseInitResult.cbBlockCount[2] >=AnalyseInitResult.cbBlockCount[i]-bDoubleCard))
            {
              continue;
            }
          }
          else
          {
            //����˫�������ĸ��������ȳ��ĸ�
            if(i == 0 && (AnalyseInitResult.cbBlockCount[2] >=AnalyseInitResult.cbBlockCount[i]-bSingelCard ||
                          AnalyseInitResult.cbBlockCount[1]-bDoubleCard >AnalyseInitResult.cbBlockCount[i]-bSingelCard))
            {
              continue;
            }

            if(i == 1 && (AnalyseInitResult.cbBlockCount[2] >=AnalyseInitResult.cbBlockCount[i]-bDoubleCard))
            {
              continue;
            }
          }


          //�Լ�ʣ��С��3�ţ������ﻹ�б������ƣ��Լ����ȳ���ͬ������
          if(cbPlayerCount <= 3)
          {
            BYTE bStartIndex = cbPlayerCount;
            bool bHaveBigCard = false;
            for(BYTE n=bStartIndex; n<CountArray(AnalyseInitResult.cbBlockCount); n++)
            {
              if(AnalyseInitResult.cbBlockCount[n]>0)
              {
                bHaveBigCard =true;
              }
            }
            if(bHaveBigCard && (i==cbPlayerCount-1))
            {
              continue;
            }
          }

          //�����˿�
          BYTE cbNewCardData[MAX_COUNT];
          ZeroMemory(cbNewCardData,sizeof(cbNewCardData));
          BYTE cbNewCardCount=i+1;
          BYTE cbIndex= cbNewCardCount *(AnalyseInitResult.cbBlockCount[i] - 1);

          //�Լ�ֻ��1,2,3��,���ҳ���ͬ�ŵĻ����Ӵ��С
          if(cbPlayerCount <= 3 && i==(cbPlayerCount-1))
          {
            cbIndex=j*(i+1);
          }

          //�����Ƿ�����
          int nLinkCount = 1;
          BYTE bCardValue = GetCardLogicValue(AnalyseInitResult.cbCardData[i][cbIndex]);
          for(int n = 1; n<13; n++)
          {
            BYTE bLinkIndex = cbNewCardCount *(AnalyseInitResult.cbBlockCount[i] - 1 - n);

            if(bLinkIndex>26)
            {
              break;
            }

            //2������
            if(GetCardValue(AnalyseInitResult.cbCardData[i][bLinkIndex]) == 0x02)
            {
              break;
            }

            if(GetCardLogicValue(AnalyseInitResult.cbCardData[i][bLinkIndex]) == bCardValue+n)
            {
              nLinkCount ++;
            }
          }

          //�����˿�
          for(BYTE n=0; n<cbNewCardCount; n++)
          {
            cbNewCardData[n] = AnalyseInitResult.cbCardData[i][cbIndex+n];
          }

          //����+����˳��
          if((i>0 && nLinkCount>2) ||(i==0 && nLinkCount>4))
          {
            ZeroMemory(cbNewCardData,cbNewCardCount);

            BYTE bCardCount=0;
            for(BYTE m=0; m<nLinkCount; m++)
            {
              BYTE bLinkIndex = cbNewCardCount *(AnalyseInitResult.cbBlockCount[i] - 1 - m);

              for(BYTE n=0; n<cbNewCardCount; n++)
              {
                cbNewCardData[bCardCount] = AnalyseInitResult.cbCardData[i][bLinkIndex+n];
                bCardCount++;
              }
            }

            cbNewCardCount = bCardCount;
          }

          //������ֻʣ��С��+������һ���Գ���
          if(bKingCount + bSubKingCount + (i+1) == cbHandCardCount)
          {
            ZeroMemory(cbNewCardData,cbNewCardCount);
            CopyMemory(cbNewCardData,cbHandCardData,cbHandCardCount);

            cbNewCardCount += bKingCount + bSubKingCount;
          }

          //��ȡ����
          tagAnalyseResult AnalyseNewResult;
          ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
          AnalysebCardData(cbNewCardData,cbNewCardCount,AnalyseNewResult);

          OutCardResult.cbCardCount=cbNewCardCount;
          CopyMemory(OutCardResult.cbResultCard,cbNewCardData,sizeof(BYTE)*cbNewCardCount);
          return true;
        }
      }
    }
    return false;
  }

  return false;

}

//������ʣ���ֿ��Գ���
bool CGameLogic::AnalyseTakeOverCount(const BYTE cbCardData[], BYTE cbCardCount,tagOutCountResult & OutCountResult)
{
  if(cbCardCount<=0)
  {
    return true;
  }

  BYTE bCardData[MAX_COUNT];
  CopyMemory(bCardData,cbCardData,cbCardCount);

  //��������
  SortCardList(bCardData,cbCardCount,ST_ORDER);

  tagAnalyseInitResult AnalyseInitResult;
  ZeroMemory(&AnalyseInitResult,sizeof(AnalyseInitResult));
  ZeroMemory(&OutCountResult,sizeof(OutCountResult));
  AnalyseInitCardData(bCardData,cbCardCount,AnalyseInitResult);


  for(int i=0; i<CountArray(AnalyseInitResult.cbBlockCount); i++)
  {
    if(i<3)
    {
      for(BYTE j=0; j<AnalyseInitResult.cbBlockCount[i]; j++)
      {
        //�����˿�
        BYTE cbNewCardData[MAX_COUNT];
        ZeroMemory(cbNewCardData,sizeof(cbNewCardData));
        BYTE cbNewCardCount=i+1;
        BYTE cbIndex= cbNewCardCount * j;

        //��С������
        if(AnalyseInitResult.cbCardData[i][cbIndex] == 0x4E || AnalyseInitResult.cbCardData[i][cbIndex] == 0x4F)
        {
          continue;
        }

        //�����Ƿ�����
        int nLinkCount = 1;
        BYTE bCardValue = GetCardLogicValue(AnalyseInitResult.cbCardData[i][cbIndex]);
        for(int n = 1; n<13; n++)
        {
          BYTE bLinkIndex = cbNewCardCount *(j + n);

          if(bLinkIndex>26)
          {
            break;
          }

          //2������
          if(GetCardValue(AnalyseInitResult.cbCardData[i][bLinkIndex]) == 0x02)
          {
            break;
          }
          if(GetCardValue(AnalyseInitResult.cbCardData[i][cbNewCardCount *j]) == 0x02)
          {
            break;
          }

          if(GetCardLogicValue(AnalyseInitResult.cbCardData[i][bLinkIndex]) == bCardValue-n)
          {
            nLinkCount ++;
          }
        }

        //����+����˳��
        if((i>0 && nLinkCount>2) ||(i==0 && nLinkCount>4))
        {
          BYTE bCardCount=0;
          for(BYTE m=0; m<nLinkCount; m++)
          {
            BYTE bLinkIndex = cbNewCardCount *(j + m);

            for(BYTE n=0; n<cbNewCardCount; n++)
            {
              bCardCount++;
            }
          }

          OutCountResult.cbOutOverCount++;

          //������Ʋ�������
          j=j+bCardCount;

          cbNewCardCount = bCardCount;
        }
        else
        {
          OutCountResult.cbOutOverCount++;
        }

      }

    }
    else if(i>=3)
    {
      OutCountResult.cbBombCount += AnalyseInitResult.cbBlockCount[i];
    }
  }


  return true;
}


//�����˳���
bool CGameLogic::AiSearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount,
                                 const BYTE cbPlayerCardData[],BYTE cbPlayerCount,tagOutCardResult & OutCardResult)
{
  if(cbTurnCardCount>0)
  {
    //���ý��
    ZeroMemory(&OutCardResult,sizeof(OutCardResult));

    //��ȡ����
    tagAnalyseResult AnalyseResult,TempResult;
    memset(&AnalyseResult,0,sizeof(AnalyseResult));
    memset(&TempResult,0,sizeof(TempResult));
    AnalysebCardData(cbTurnCardData,cbTurnCardCount,AnalyseResult);
    FindValidCard(AnalyseResult,TempResult);

    tagAnalyseInitResult AnalyseInitResult;
    ZeroMemory(&AnalyseInitResult,sizeof(AnalyseInitResult));
    AnalyseInitCardData(cbHandCardData,cbHandCardCount,AnalyseInitResult);

    //��������
    BYTE bSubKingCount = 0;
    BYTE bKingCount = 0;
    for(BYTE i=0; i<cbHandCardCount; i++)
    {
      if(cbHandCardData[i] == 0x4E)
      {
        bSubKingCount++;
      }

      if(cbHandCardData[i] == 0x4F)
      {
        bKingCount++;
      }
    }

    //��ըѹ��
    BYTE cbAllKinCount=bSubKingCount +bKingCount;

    //��ȡ�˿�
    //��ͨը������ͬ�������ҿ���ѹס����
    for(BYTE i=0; i<CountArray(AnalyseInitResult.cbBlockCount); i++)
    {
      if(AnalyseInitResult.cbBlockCount[i]>0)
      {
        for(BYTE j=AnalyseInitResult.cbBlockCount[i]-1; j>=0; j--)
        {
          if(j<0 || j==0xFF)
          {
            break;
          }

          //��Ҫ���Ƴ�ը�����ø��๱�׷�
          if(AnalyseInitResult.cbCardData[i][j*(i+1)] == 0x4E || AnalyseInitResult.cbCardData[i][j*(i+1)] == 0x4F)
          {
            continue;
          }

          //�Լҳ�����ֻʣһ����,������ը���ȳ�ը����
          if(cbPlayerCount == 1)
          {
            if(i==0)
            {
              for(BYTE n=0; n<CountArray(AnalyseInitResult.cbBlockCount); n++)
              {
                if(AnalyseInitResult.cbBlockCount[n] >0)
                {
                  if(bKingCount+bSubKingCount+n+1 >= 4)
                  {
                    continue;
                  }
                }
              }
            }
          }

          //�����˿�
          BYTE cbNewCardData[MAX_COUNT];
          ZeroMemory(cbNewCardData,sizeof(cbNewCardData));
          BYTE cbNewCardCount=i+1;
          BYTE cbIndex=j*(i+1);

          for(BYTE n=0; n<cbNewCardCount; n++)
          {
            cbNewCardData[n] = AnalyseInitResult.cbCardData[i][cbIndex+n];
          }

          //��������һ���
          if(cbAllKinCount>0)
          {
            bool bMostCard = false;

            //��������
            for(BYTE n=i+1; n<CountArray(AnalyseInitResult.cbBlockCount); n++)
            {
              if(AnalyseInitResult.cbBlockCount[n]>0)
              {
                bMostCard = true;
              }
            }

            //��������
            for(BYTE n=cbIndex-1; n>=0; n--)
            {
              if(n<0 || n==0xFF)
              {
                break;
              }
              if(GetCardLogicValue(AnalyseInitResult.cbCardData[i][n])>GetCardLogicValue(AnalyseInitResult.cbCardData[i][cbIndex]))
              {
                bMostCard = true;
              }
            }

            //û�и����ƣ��������һ��
            if(!bMostCard)
            {
              //��Ӵ���
              for(BYTE k = 0; k<bKingCount; k++)
              {
                cbNewCardData[cbNewCardCount++] = 0x4F;
              }

              //���С��
              for(BYTE k = 0; k<bSubKingCount; k++)
              {
                cbNewCardData[cbNewCardCount++] = 0x4E;
              }
            }
          }

          //�����Լҳ�ը�����м��ֿɳ��꣬��ը��n>n+2(ʣ������)ʱ����������ըһ��ը��
          tagOutCountResult tagPlayerOutCount,tagAiOutCount;
          AnalyseTakeOverCount(cbPlayerCardData,cbPlayerCount,tagPlayerOutCount);
          AnalyseTakeOverCount(cbHandCardData,cbHandCardCount,tagAiOutCount);

          if(tagPlayerOutCount.cbBombCount+2<=tagPlayerOutCount.cbOutOverCount && !IsBombCard(AnalyseResult))
          {
            bool bPass = tagAiOutCount.cbOutOverCount>tagAiOutCount.cbBombCount?true:false;
            if(cbNewCardCount>=4 && bPass)
            {
              return false;
            }
          }

          ////����ÿ��ѹ������ը����С�ƿ�ѡ��pass
          //if (cbTurnCardCount<=2 && cbPlayerCount>MAX_COUNT/2)
          //{
          //  //����ĵ���+���ƻ�ʣ3������
          //  int nSmallCardCount = AnalyseInitResult.cbBlockCount[0] + AnalyseInitResult.cbBlockCount[1];
          //  if (cbNewCardCount>=4 && nSmallCardCount>=3) return false;
          //}

          //��ȡ����
          tagAnalyseResult AnalyseNewResult;
          ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
          AnalysebCardData(cbNewCardData,cbNewCardCount,AnalyseNewResult);

          if(CompareCard(AnalyseNewResult,AnalyseResult))
          {
            OutCardResult.cbCardCount=cbNewCardCount;
            CopyMemory(OutCardResult.cbResultCard,cbNewCardData,sizeof(BYTE)*cbNewCardCount);
            return true;
          }
        }


        //��2�Ե��ƣ�����
        if(cbTurnCardCount<=2 && (i+1>=cbTurnCardCount))
        {
          //���ţ�3������2
          for(int m=1; m<3; m++)
          {
            for(int j=0; j<AnalyseInitResult.cbBlockCount[m]; j++)
            {
              if((GetCardValue(AnalyseInitResult.cbCardData[m][j]) == 0x02) && (m+1>cbTurnCardCount))
              {
                //�����˿�
                BYTE cbNewCardData[MAX_COUNT];
                ZeroMemory(cbNewCardData,sizeof(cbNewCardData));

                for(BYTE n=0; n<cbTurnCardCount; n++)
                {
                  cbNewCardData[n] = AnalyseInitResult.cbCardData[m][j+n];
                }

                //��ȡ����
                tagAnalyseResult AnalyseNewResult;
                ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
                AnalysebCardData(cbNewCardData,cbTurnCardCount,AnalyseNewResult);

                if(CompareCard(AnalyseNewResult,AnalyseResult))
                {
                  OutCardResult.cbCardCount=cbTurnCardCount;
                  CopyMemory(OutCardResult.cbResultCard,cbNewCardData,sizeof(BYTE)*cbTurnCardCount);
                  return true;
                }
              }
            }
          }
        }
      }
    }

    //����ը����
    for(BYTE i=0; i<CountArray(AnalyseInitResult.cbBlockCount); i++)
    {
      for(BYTE j=0; j<AnalyseInitResult.cbBlockCount[i]; j++)
      {
        if(AnalyseInitResult.cbBlockCount[i]>0 && cbAllKinCount + i> 3)
        {
          BYTE cbCardData[12] = {0};
          BYTE cbCardIndex = 0;

          //3��6��
          if(cbAllKinCount ==3)
          {
            //���Ƽ�����С������
            if(bSubKingCount +bKingCount + i < 6)
            {
              continue;
            }
          }

          //4��7��
          if(cbAllKinCount ==4)
          {
            //���Ƽ�����С������
            if(bKingCount + bSubKingCount+ i < 7)
            {
              continue;
            }
          }

          //�����˿�
          BYTE cbNewCardCount=i+1;
          BYTE cbIndex=j*(i+1);

          for(BYTE n=0; n<cbNewCardCount; n++)
          {
            cbCardData[n] = AnalyseInitResult.cbCardData[i][cbIndex+n];
          }

          cbCardIndex += cbNewCardCount;

          //��Ӵ���
          for(BYTE k = 0; k<bKingCount; k++)
          {
            cbCardData[cbCardIndex++] = 0x4F;
          }

          //���С��
          for(BYTE k = 0; k<bSubKingCount; k++)
          {
            cbCardData[cbCardIndex++] = 0x4E;
          }

          //��ȡ����
          tagAnalyseResult AnalyseNewResult;
          ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
          AnalysebCardData(cbCardData,cbNewCardCount+cbAllKinCount,AnalyseNewResult);

          if(CompareCard(AnalyseNewResult,AnalyseResult))
          {
            OutCardResult.cbCardCount=cbNewCardCount+cbAllKinCount;
            CopyMemory(OutCardResult.cbResultCard,cbCardData,sizeof(BYTE)*(cbNewCardCount+cbAllKinCount));
            return true;
          }
        }
      }
    }

    //����ը
    if(cbAllKinCount >=3)
    {
      BYTE cbCardData[4] = {0};
      BYTE cbCardIndex = 0;

      for(BYTE k = 0; k< bSubKingCount; k++)
      {
        cbCardData[cbCardIndex] = 0x4E;
        cbCardIndex++;
      }

      for(BYTE k = 0; k< bKingCount; k++)
      {
        cbCardData[cbCardIndex] = 0x4F;
        cbCardIndex++;
      }

      //��ȡ����
      tagAnalyseResult AnalyseNewResult;
      ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
      AnalysebCardData(cbCardData,cbAllKinCount,AnalyseNewResult);

      if(CompareCard(AnalyseNewResult,AnalyseResult))
      {
        OutCardResult.cbCardCount=cbAllKinCount;
        CopyMemory(OutCardResult.cbResultCard,cbCardData,sizeof(BYTE)*cbAllKinCount);
        return true;
      }
    }


    //��ըѹ��
    tagAnalyseInitResult AnalyseTempResult;
    ZeroMemory(&AnalyseTempResult,sizeof(AnalyseTempResult));

    BYTE bTempCardData[MAX_COUNT];
    CopyMemory(bTempCardData,cbHandCardData,sizeof(bTempCardData));

    for(BYTE i=4; i<9; i++)
    {
      BYTE bSearchCardCount  = i;
      BYTE bSearchLineCount  = 3;

      if(SearchSameLinkCard(bTempCardData,cbHandCardCount,bSearchCardCount,bSearchLineCount,AnalyseTempResult))
      {
        //��ȡ����
        tagAnalyseResult AnalyseNewResult;
        ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
        AnalysebCardData(AnalyseTempResult.cbCardData[i-1],AnalyseTempResult.cbBlockCount[i-1],AnalyseNewResult);

        if(CompareCard(AnalyseNewResult,AnalyseResult))
        {
          OutCardResult.cbCardCount=AnalyseTempResult.cbBlockCount[bSearchCardCount-1];
          CopyMemory(OutCardResult.cbResultCard,AnalyseTempResult.cbCardData[i-1],sizeof(BYTE)*AnalyseTempResult.cbBlockCount[i-1]);
          return true;
        }
      }
    }

    //�������Բ����ٲ��Ƴ�
    ZeroMemory(&AnalyseInitResult,sizeof(AnalyseInitResult));
    AnalyseSameCardData((BYTE*)cbHandCardData,cbHandCardCount,cbTurnCardCount,AnalyseInitResult);

    //��ͨը������ͬ�������ҿ���ѹס����
    for(BYTE i=0; i<CountArray(AnalyseInitResult.cbBlockCount); i++)
    {
      if(i>=0&&AnalyseInitResult.cbBlockCount[i]>0)
      {
        for(BYTE j=AnalyseInitResult.cbBlockCount[i]-1; j>=0; j--)
        {
          if(j<0 || j==0xFF)
          {
            break;
          }

          //�����˿�
          BYTE cbNewCardData[MAX_COUNT];
          ZeroMemory(cbNewCardData,sizeof(cbNewCardData));
          BYTE cbNewCardCount=i+1;
          BYTE cbIndex=j*(i+1);

          for(BYTE n=0; n<cbNewCardCount; n++)
          {
            cbNewCardData[n] = AnalyseInitResult.cbCardData[i][cbIndex+n];
          }

          //��ȡ����
          tagAnalyseResult AnalyseNewResult;
          ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
          AnalysebCardData(cbNewCardData,cbNewCardCount,AnalyseNewResult);

          if(CompareCard(AnalyseNewResult,AnalyseResult))
          {
            OutCardResult.cbCardCount=cbNewCardCount;
            CopyMemory(OutCardResult.cbResultCard,cbNewCardData,sizeof(BYTE)*cbNewCardCount);
            return true;
          }
        }
      }
    }

    return false;
  }
  else if(cbTurnCardCount==0)     //�׳�
  {
    tagAnalyseInitResult AnalyseInitResult;
    ZeroMemory(&AnalyseInitResult,sizeof(AnalyseInitResult));
    AnalyseInitCardData(cbHandCardData,cbHandCardCount,AnalyseInitResult);

    BYTE bSubKingCount = 0;
    BYTE bKingCount = 0;
    for(BYTE i=0; i<cbHandCardCount; i++)
    {
      if(cbHandCardData[i] == 0x4E)
      {
        bSubKingCount++;
      }

      if(cbHandCardData[i] == 0x4F)
      {
        bKingCount++;
      }
    }

    //��ȡ�˿�
    for(BYTE i=0; i<CountArray(AnalyseInitResult.cbBlockCount); i++)
    {
      if(i>=0&&AnalyseInitResult.cbBlockCount[i]>0)
      {
        for(BYTE j=0; j<AnalyseInitResult.cbBlockCount[i]; j++)
        {
          //����������С��
          if(bSubKingCount+bKingCount != cbHandCardCount)
          {
            if(AnalyseInitResult.cbCardData[i][j*(i+1)] == 0x4E || AnalyseInitResult.cbCardData[i][j*(i+1)] == 0x4F)
            {
              continue;
            }
          }

          BYTE bSingelCard=0,bDoubleCard=0;
          if(bSubKingCount==1)
          {
            bSingelCard++;
          }
          if(bKingCount==1)
          {
            bSingelCard++;
          }
          if(bSubKingCount==2)
          {
            bDoubleCard++;
          }
          if(bKingCount==2)
          {
            bDoubleCard++;
          }

          //һ��2 ��һ�� �Է�����4�ţ��ȳ�2 �ڳ���
          bool bContinue = true;
          if((cbPlayerCount<4) && (cbHandCardCount == 3) && (GetCardValue(AnalyseInitResult.cbCardData[i][j*(i+1)]) == 0x02)
             && (AnalyseInitResult.cbBlockCount[1] == 1) && (AnalyseInitResult.cbBlockCount[0]==1))
          {
            bContinue = false;
          }

          //������ͬ��������
          if(cbPlayerCount == 3)
          {
            //ֻ�ȶԵ���˫����
            if(bContinue && i == 0 && AnalyseInitResult.cbBlockCount[1]-bDoubleCard >=AnalyseInitResult.cbBlockCount[i]-bSingelCard)
            {
              continue;
            }
          }
          else if(cbPlayerCount == 2)
          {
            //ֻ�ȶԵ�����������
            if(i == 0 && AnalyseInitResult.cbBlockCount[2] >=AnalyseInitResult.cbBlockCount[i]-bSingelCard)
            {
              continue;
            }
          }
          else if(cbPlayerCount == 1)
          {
            //˫�������ĸ��������ȳ��ĸ�
            if(i == 1 && (AnalyseInitResult.cbBlockCount[2] >=AnalyseInitResult.cbBlockCount[i]-bDoubleCard))
            {
              continue;
            }
          }
          else
          {
            //����˫�������ĸ��������ȳ��ĸ�
            if(bContinue && i == 0 && (AnalyseInitResult.cbBlockCount[2] >=AnalyseInitResult.cbBlockCount[i]-bSingelCard ||
                                       AnalyseInitResult.cbBlockCount[1]-bDoubleCard >AnalyseInitResult.cbBlockCount[i]-bSingelCard))
            {
              continue;
            }

            if(i == 1 && (AnalyseInitResult.cbBlockCount[2] >=AnalyseInitResult.cbBlockCount[i]-bDoubleCard))
            {
              continue;
            }
          }


          //�Լ�ʣ��С��3�ţ������ﻹ�б������ƣ��Լ����ȳ���ͬ������
          if(cbPlayerCount <= 3)
          {
            BYTE bStartIndex = cbPlayerCount;
            bool bHaveBigCard = false;
            for(BYTE n=bStartIndex; n<CountArray(AnalyseInitResult.cbBlockCount); n++)
            {
              if(AnalyseInitResult.cbBlockCount[n]>0)
              {
                bHaveBigCard =true;
              }
            }
            if(bHaveBigCard && (i==cbPlayerCount-1))
            {
              continue;
            }
          }

          //�����˿�
          BYTE cbNewCardData[MAX_COUNT];
          ZeroMemory(cbNewCardData,sizeof(cbNewCardData));
          BYTE cbNewCardCount=i+1;
          BYTE cbIndex= cbNewCardCount *(AnalyseInitResult.cbBlockCount[i] - 1);

          //�Լ�ֻ��1,2,3��,���ҳ���ͬ�ŵĻ����Ӵ��С
          if(cbPlayerCount <= 3 && i==(cbPlayerCount-1))
          {
            cbIndex=j*(i+1);
          }

          //�����Ƿ�����
          int nLinkCount = 1;
          BYTE bCardValue = GetCardLogicValue(AnalyseInitResult.cbCardData[i][cbIndex]);
          for(int n = 1; n<13; n++)
          {
            BYTE bLinkIndex = cbNewCardCount *(AnalyseInitResult.cbBlockCount[i] - 1 - n);

            if(bLinkIndex>26)
            {
              break;
            }

            //2������
            if(GetCardValue(AnalyseInitResult.cbCardData[i][bLinkIndex]) == 0x02)
            {
              break;
            }

            if(GetCardLogicValue(AnalyseInitResult.cbCardData[i][bLinkIndex]) == bCardValue+n)
            {
              nLinkCount ++;
            }
          }

          //�����˿�
          for(BYTE n=0; n<cbNewCardCount; n++)
          {
            cbNewCardData[n] = AnalyseInitResult.cbCardData[i][cbIndex+n];
          }

          //����+����˳��
          if((i>0 && nLinkCount>2) ||(i==0 && nLinkCount>4))
          {
            ZeroMemory(cbNewCardData,cbNewCardCount);

            BYTE bCardCount=0;
            for(BYTE m=0; m<nLinkCount; m++)
            {
              BYTE bLinkIndex = cbNewCardCount *(AnalyseInitResult.cbBlockCount[i] - 1 - m);

              for(BYTE n=0; n<cbNewCardCount; n++)
              {
                cbNewCardData[bCardCount] = AnalyseInitResult.cbCardData[i][bLinkIndex+n];
                bCardCount++;
              }
            }

            cbNewCardCount = bCardCount;
          }

          //������ֻʣ��С��+������һ���Գ���
          if(bKingCount + bSubKingCount + (i+1) == cbHandCardCount)
          {
            ZeroMemory(cbNewCardData,cbNewCardCount);
            CopyMemory(cbNewCardData,cbHandCardData,cbHandCardCount);

            cbNewCardCount += bKingCount + bSubKingCount;
          }

          //��ȡ����
          tagAnalyseResult AnalyseNewResult;
          ZeroMemory(&AnalyseNewResult,sizeof(AnalyseNewResult));
          AnalysebCardData(cbNewCardData,cbNewCardCount,AnalyseNewResult);

          OutCardResult.cbCardCount=cbNewCardCount;
          CopyMemory(OutCardResult.cbResultCard,cbNewCardData,sizeof(BYTE)*cbNewCardCount);
          return true;
        }
      }
    }
    return false;
  }

  return false;

}


bool CGameLogic::FindValidCard(tagAnalyseResult & CurOutCard,tagAnalyseResult&PerOutCard)
{
  tagAnalyseResult analyseCard;
  memcpy(&analyseCard,&CurOutCard,sizeof(tagAnalyseResult));

  BYTE cbPerCardType=PerOutCard.m_cbCardType;
  bool bReturn=false;
  for(BYTE cbIndex=cbPerCardType; cbIndex<11; cbIndex++)
  {
    //�Ƿ��ǲο����ͻ��߸߽�����
    if(cbIndex==cbPerCardType||(m_cbCardGrade[cbPerCardType]&0xF0)<(m_cbCardGrade[cbIndex]&0xF0))
    {
      memcpy(&CurOutCard,&analyseCard,sizeof(tagAnalyseResult));
      switch(cbIndex)
      {
        case CT_SINGLE:
        {
          if(CurOutCard.m_cbCardCount==1)
          {
            bReturn=true;
            CurOutCard.m_cbCardType=cbIndex;
          }
          break;
        }
        case CT_DOUBLE:
        {
          if(CurOutCard.m_cbCardCount==2)
          {
            if(CurOutCard.m_cbCardCount==CurOutCard.m_nJockerCount)
            {
              bReturn=true;
              CurOutCard.m_cbCardType=cbIndex;
            }
            else
            {
              if(CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCount+CurOutCard.m_nJockerCount==2)
              {
                bReturn=true;
                CurOutCard.m_cbCardType=cbIndex;
              }
            }
          }
          break;
        }
        case CT_THREE:
        {
          if(CurOutCard.m_cbCardCount==3&&CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCount+CurOutCard.m_nJockerCount==3&&CurOutCard.m_nJockerCount<=2)
          {
            bReturn=true;
            CurOutCard.m_cbCardType=cbIndex;
          }
          break;
        }
        case CT_SINGLE_LINK:
        {
          if(bReturn=FindLinkCard(CurOutCard,1,5,true,false))
          {
            CurOutCard.m_cbCardType=cbIndex;
          }
          break;
        }
        case CT_DOUBLE_LINK:
        {
          if(bReturn=FindLinkCard(CurOutCard,2,3,true,false))
          {
            CurOutCard.m_cbCardType=cbIndex;
          }
          break;
        }
        case CT_THREE_LINK:
        {
          if(!IsBombCard(CurOutCard) && (bReturn=FindLinkCard(CurOutCard,3,3,true,false)))
          {
            CurOutCard.m_cbCardType=cbIndex;
          }
          break;
        }

        case CT_BOMB:
        case CT_BOMB_3W:
        case CT_BOMB_TW:
        case CT_BOMB_LINK:
        {
          bReturn=IsBombCard(CurOutCard);
          break;
        }
      }
      if(bReturn)
      {
        break;
      }
    }
  }

  return bReturn;
}


#define  LEFT_BORDER  0
#define  RIGHT_BORDER 12
#define  ELEMT_COUNT  13

bool CGameLogic::FillCardCount(tagAnalyseResult&AnalyseResultEx,int nMinCount)
{
  //�������
  int nDifference=0;
  int nTempIndex=0;
  bool bFlags=true;

  tagAnalyseResult AnalyseResult;
  memcpy(&AnalyseResult,&AnalyseResultEx,sizeof(tagAnalyseResult));

  for(int nIndex=AnalyseResult.m_cbMinIndex; nIndex<=AnalyseResult.m_cbMaxIndex; nIndex++)
  {
    nTempIndex=nIndex%ELEMT_COUNT;
    nDifference=nMinCount-(int)AnalyseResult.m_CardStyle[nTempIndex].m_cbCount;
    if(nDifference<=AnalyseResult.m_nJockerCount)
    {
      if(nDifference>0)
      {
        if(AnalyseResult.m_CardStyle[nTempIndex].m_cbCount==0)
        {
          AnalyseResult.m_nUnEqualCard++;
        }

        AnalyseResult.m_CardStyle[nTempIndex].m_cbCount+=nDifference;
        AnalyseResult.m_nJockerCount-=nDifference;

        if(AnalyseResult.m_CardStyle[nTempIndex].m_cbCard==0)
        {
          AnalyseResult.m_CardStyle[nTempIndex].m_cbCard=TransForLogicValue((AnalyseResult.m_cbMinIndex+1+nIndex)%ELEMT_COUNT);
        }
      }
    }
    else
    {
      return false;
    }
  }


  //AnalyseResult.m_nMaxCount=nMinCount; ��֪������Ϊʲô���¸�ֵ ע�͵���
  memcpy(&AnalyseResultEx,&AnalyseResult,sizeof(tagAnalyseResult));

  // 2���ܳ����������棬��������ը
  int n2Count=AnalyseResultEx.m_CardStyle[12].m_cbCount;

  if(n2Count>0)
  {
    if(n2Count>=4)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  return true;
}




bool CGameLogic::FillCardSeque(tagAnalyseResult&AnalyseResult,int cbMinCount,int cbMinLink,bool bAllowCycle)
{
  //��������������������ͬ�����������Ƶ������򷵻�
  if(AnalyseResult.m_nUnEqualCard*cbMinCount>AnalyseResult.m_cbCardCount)
  {
    return false;
  }
  //��������
  BYTE cbMaxLong=AnalyseResult.m_nUnEqualCard+AnalyseResult.m_nJockerCount/cbMinCount;
  //���������󳤶�С����С���󳤶�
  if(cbMaxLong<cbMinLink)
  {
    return false;
  }

  //�����ѭ���޷���λ�ɹ�
  if((AnalyseResult.m_cbMinIndex+cbMaxLong)<AnalyseResult.m_cbMaxIndex)
  {
    if(bAllowCycle) //�Ƿ�������ѭ����λ
    {
      //�ҵ���ʼ��
      int nCoverCount=AnalyseResult.m_nUnEqualCard;
      BYTE cbStartIndex=(AnalyseResult.m_cbMinIndex+ELEMT_COUNT-cbMaxLong)%ELEMT_COUNT;
      for(BYTE cbIndex=cbStartIndex; cbIndex<cbStartIndex+ELEMT_COUNT; cbIndex++)
      {
        if(AnalyseResult.m_CardStyle[cbIndex%ELEMT_COUNT].m_cbCount>0)
        {
          //��������
          if(nCoverCount==AnalyseResult.m_nUnEqualCard)
          {
            AnalyseResult.m_cbMinIndex=cbIndex;
          }
          if((--nCoverCount)==0)
          {
            AnalyseResult.m_cbMaxIndex=cbIndex;
            break;
          }
        }
      }
    }
  }

  //������ֹ��
  BYTE cbCurLinkLong=AnalyseResult.m_cbMaxIndex-AnalyseResult.m_cbMinIndex+1;
  //��������
  if(cbCurLinkLong<cbMinLink)
  {
    for(BYTE cbIndex=0; cbIndex<cbMinLink-cbCurLinkLong; cbIndex++)
    {
      //���Ҳ���
      if(AnalyseResult.m_cbMaxIndex+1<RIGHT_BORDER)
      {
        AnalyseResult.m_cbMaxIndex++;
      }
      else //���󲹳�
      {
        AnalyseResult.m_cbMinIndex=(((AnalyseResult.m_cbMinIndex+ELEMT_COUNT)-1)%ELEMT_COUNT);
      }
    }
  }
  //����λ
  return FillCardCount(AnalyseResult,cbMinCount);
}

//�鿴�Ƿ����� �����ڼ����ӣ�ͬ���ţ����öԣ���ͬ����,�������Ƶ�ը����˳�ӵ�����
bool CGameLogic::FindLinkCard(tagAnalyseResult & AnalyseResult,BYTE cbMinCardCount,BYTE cbMinLinkLong,bool bLockCount,bool bAllowCycle)
{
  //�����ͬ����Ϊ��
  if(AnalyseResult.m_nUnEqualCard==0)
  {
    return false;
  }

  if(bLockCount)
  {
    if(AnalyseResult.m_nMaxCount>cbMinCardCount)
    {
      return false;
    }
  }

  //�Ƿ�������̳��Ⱥ�����������Ҫ��
  if(!FillCardSeque(AnalyseResult,cbMinCardCount,cbMinLinkLong,bAllowCycle))
  {
    return false;
  }

  //�����������ҷ��ֻ��б��ƣ�������ص�����ϵĶ��׻��β������������
  if(AnalyseResult.m_nJockerCount>0)
  {
    //������������
    if(!bLockCount)
    {
      FillCardCount(AnalyseResult,AnalyseResult.m_nMaxCount +1);
    }
    //���ӳ���
    if(AnalyseResult.m_nJockerCount>0)
    {
      BYTE cbAddLong=AnalyseResult.m_nJockerCount/AnalyseResult.m_nMaxCount;
      if(AnalyseResult.m_nJockerCount%(AnalyseResult.m_nMaxCount)==0)
      {
        return FillCardSeque(AnalyseResult,AnalyseResult.m_nMaxCount,AnalyseResult.m_nUnEqualCard+cbAddLong,bAllowCycle);
      }
      else
      {
        if(bLockCount)
        {
          return false;
        }
      }
    }
  }

  AnalyseResult.m_cbMaxIndex=AnalyseResult.m_cbMaxIndex%ELEMT_COUNT;
  AnalyseResult.m_cbMinIndex=AnalyseResult.m_cbMinIndex%ELEMT_COUNT;

  if(AnalyseResult.m_cbMinIndex>AnalyseResult.m_cbMaxIndex)
  {
    BYTE cbTemp=AnalyseResult.m_cbMinIndex;
    AnalyseResult.m_cbMinIndex=AnalyseResult.m_cbMaxIndex;
    AnalyseResult.m_cbMaxIndex=cbTemp;
  }
  return true;
}


bool  CGameLogic::IsBombCard(tagAnalyseResult & AnalyseResult)
{
  switch(AnalyseResult.m_cbCardCount)
  {
    case 0: //����
    case 1: //����
    case 2: //����
    {
      break;
    }
    case 3: //����
    {
      if(AnalyseResult.m_nJockerCount==AnalyseResult.m_cbCardCount)
      {
        AnalyseResult.m_cbCardType=CT_BOMB_3W;
        AnalyseResult.m_cbBombGrade=6;
        AnalyseResult.m_nUnEqualCard=1;
        AnalyseResult.m_cbMinIndex=0;
        AnalyseResult.m_cbMaxIndex=0;
        AnalyseResult.m_CardStyle[0].m_cbCard=0x4E;
        AnalyseResult.m_CardStyle[0].m_cbCount=3;
        return true;
      }
      break;
    }
    default:
    {
      //����ը
      if(AnalyseResult.m_nJockerCount==AnalyseResult.m_cbCardCount)
      {
        AnalyseResult.m_cbCardType=CT_BOMB_TW;
        AnalyseResult.m_cbBombGrade=7;
        AnalyseResult.m_nUnEqualCard=1;
        AnalyseResult.m_cbMinIndex=0;
        AnalyseResult.m_cbMaxIndex=0;
        AnalyseResult.m_CardStyle[0].m_cbCard=0x4E;
        AnalyseResult.m_CardStyle[0].m_cbCount=4;
        return true;
      }
      //��ը
      if(AnalyseResult.m_nUnEqualCard==1)
      {
        AnalyseResult.m_cbCardType=CT_BOMB;
        AnalyseResult.m_cbBombGrade=AnalyseResult.m_cbCardCount;
        AnalyseResult.m_CardStyle[AnalyseResult.m_cbMinIndex].m_cbCount+=AnalyseResult.m_nJockerCount;
        return true;
      }
      //��ը
      if(FindLinkCard(AnalyseResult,4,3,false,true))
      {
        AnalyseResult.m_cbCardType=CT_BOMB_LINK;

        int nMinCount=0,nResultCount=0,nAllCardType=0,nAllCardNumber=0,nNeedJockerCount = 0;
        for(int nIndex=0; nIndex<=12; nIndex++)
        {
          int nTempIndex=nIndex%ELEMT_COUNT;

          if(AnalyseResult.m_CardStyle[nTempIndex].m_cbCount>0)
          {
            if(nMinCount==0)
            {
              nMinCount=AnalyseResult.m_CardStyle[nTempIndex].m_cbCount;
            }

            nAllCardNumber+=AnalyseResult.m_CardStyle[nTempIndex].m_cbCount;
            nAllCardType++;

            if(AnalyseResult.m_CardStyle[nTempIndex].m_cbCount<nMinCount)
            {
              nMinCount = AnalyseResult.m_CardStyle[nTempIndex].m_cbCount;
            }

            //��Ҫ�����룬������������˶�����Сը,�����ƿ��Դ���
            nNeedJockerCount += AnalyseResult.m_nMaxCount - AnalyseResult.m_CardStyle[nTempIndex].m_cbCount;
          }
        }

        if(AnalyseResult.m_nJockerCount>0 && (nNeedJockerCount <= AnalyseResult.m_nJockerCount))
        {
          if(nAllCardType>0)
          {
            nResultCount = (nAllCardNumber+AnalyseResult.m_nJockerCount)/nAllCardType;
          }
        }
        else
        {
          //��������
          if(AnalyseResult.m_nJockerCount>0)
          {
            tagAnalyseResult tagResult;
            CopyMemory(&tagResult,&AnalyseResult,sizeof(tagResult));

            BYTE bJockerCount = AnalyseResult.m_nJockerCount;

            //�ҳ���С��������
            BYTE bMinIndex = 0,bMinCount = INVALID_BYTE;

            do
            {
              bMinIndex = 0;
              bMinCount = INVALID_BYTE;
              for(int nIndex=0; nIndex<=12; nIndex++)
              {
                int nTempIndex=nIndex%ELEMT_COUNT;

                if(tagResult.m_CardStyle[nTempIndex].m_cbCount>0)
                {
                  if(tagResult.m_CardStyle[nTempIndex].m_cbCount<bMinCount)
                  {
                    bMinCount = tagResult.m_CardStyle[nTempIndex].m_cbCount;
                    bMinIndex = nTempIndex;
                  }
                }
              }

              if(bMinCount != INVALID_BYTE)
              {
                tagResult.m_CardStyle[bMinIndex].m_cbCount++;
              }

              bJockerCount --;

            }
            while(bJockerCount>0);

            //ȫ����������ҳ���С��
            bMinIndex = 0;
            bMinCount = INVALID_BYTE;
            for(int nIndex=0; nIndex<=12; nIndex++)
            {
              int nTempIndex=nIndex%ELEMT_COUNT;

              if(tagResult.m_CardStyle[nTempIndex].m_cbCount>0)
              {
                if(tagResult.m_CardStyle[nTempIndex].m_cbCount<bMinCount)
                {
                  bMinCount = tagResult.m_CardStyle[nTempIndex].m_cbCount;
                  bMinIndex = nTempIndex;
                }
              }
            }

            nMinCount = bMinCount;
          }

          nResultCount=nMinCount;
        }

        BYTE cbTemp;
        //��ȡ��ը������
        for(int i=0; i<BOMB_TYPE_COUNT; i++)
        {
          //cbTemp=m_cbTransTable[AnalyseResult.m_nMaxCount-4][i];
          cbTemp=m_cbTransTable[nResultCount-4][i];
          if((cbTemp&0xF0)>0&&AnalyseResult.m_nUnEqualCard==((cbTemp&0xF0)>>4))
          {
            AnalyseResult.m_cbBombGrade=i+4;
            break;
          }
        }
        if(i==9)
        {
          return false;
        }
        return true;
      }
      break;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////
VOID CGameLogic::AnalysebCardData(const BYTE cbCardData[], int cbCardCount, tagAnalyseResult & analyseResultEx)
{
  //���ý��
  ZeroMemory(&analyseResultEx,sizeof(analyseResultEx));
  int nIndex=0;
  analyseResultEx.m_cbMinIndex=255;
  analyseResultEx.m_cbCardCount=cbCardCount;
  for(nIndex=0; nIndex<cbCardCount; nIndex++)
  {
    analyseResultEx.m_CardStyle[GetCardLogicValue(cbCardData[nIndex])-3].m_cbCard=cbCardData[nIndex];
    analyseResultEx.m_CardStyle[GetCardLogicValue(cbCardData[nIndex])-3].m_cbCount++;
    analyseResultEx.m_cbMinIndex=min(GetCardLogicValue(cbCardData[nIndex])-3,analyseResultEx.m_cbMinIndex);

    if(cbCardData[nIndex]&0x40)
    {
      analyseResultEx.m_nJockerCount++;
    }
    else
    {
      analyseResultEx.m_cbMaxIndex=max(GetCardLogicValue(cbCardData[nIndex])-3,analyseResultEx.m_cbMaxIndex);
    }
  }

  //���ֻ����
  if(analyseResultEx.m_nJockerCount==analyseResultEx.m_cbCardCount)
  {
    if(analyseResultEx.m_CardStyle[14].m_cbCount>0)
    {
      analyseResultEx.m_cbMaxIndex=14;
    }
    else
    {
      analyseResultEx.m_cbMaxIndex=13;
    }
  }
  else
  {
    //�Ƶ�����
    for(nIndex=analyseResultEx.m_cbMinIndex; nIndex<=analyseResultEx.m_cbMaxIndex; nIndex++)
    {
      if(analyseResultEx.m_CardStyle[nIndex].m_cbCount>0)
      {
        analyseResultEx.m_nUnEqualCard++;
      }
      analyseResultEx.m_nMaxCount=max(analyseResultEx.m_nMaxCount,analyseResultEx.m_CardStyle[nIndex].m_cbCount);
    }
  }
  return;
}


//�����˿�
VOID CGameLogic::AnalyseInitCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseInitResult & AnalyseResult)
{
  //���ý��
  ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

  //�˿˷���
  for(BYTE i=0; i<cbCardCount; i++)
  {
    //��������
    BYTE cbSameCount=1,cbCardValueTemp=0;
    BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

    if(cbCardData[i]==0x4E)
    {
      AnalyseResult.cbSubKingCount++;
      //continue;
    }
    if(cbCardData[i]==0x4F)
    {
      AnalyseResult.cbKingCount++;
      //continue;
    }

    //����ͬ��
    for(BYTE j=i+1; j<cbCardCount; j++)
    {
      //��ȡ�˿�
      if(GetCardLogicValue(cbCardData[j])!=cbLogicValue)
      {
        break;
      }

      //���ñ���
      cbSameCount++;
    }

    //���ý��
    BYTE cbIndex=AnalyseResult.cbBlockCount[cbSameCount-1]++;
    for(BYTE j=0; j<cbSameCount; j++)
    {
      AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];
    }

    //��������
    i+=cbSameCount-1;
  }

  return;
}


//�����˿�(�������������ı��ض�)
VOID CGameLogic::AnalyseSameCardData(BYTE cbCardData[], BYTE cbCardCount,BYTE bSearchCount, tagAnalyseInitResult & AnalyseResult)
{
  //���ý��
  ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

  //����
  SortCardList(cbCardData,cbCardCount);

  BYTE bLastLogicValue = 0;

  //�˿˷���
  for(BYTE i=0; i<cbCardCount; i++)
  {
    //��������
    BYTE cbSameCount=1,cbCardValueTemp=0;
    BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

    //ȥ���ظ�
    if(cbLogicValue == bLastLogicValue)
    {
      continue;
    }
    bLastLogicValue = cbLogicValue;

    if(cbCardData[i]==0x4E)
    {
      AnalyseResult.cbSubKingCount++;
      //continue;
    }
    if(cbCardData[i]==0x4F)
    {
      AnalyseResult.cbKingCount++;
      //continue;
    }

    //����ͬ��
    for(BYTE j=i+1; j<cbCardCount; j++)
    {
      //��ȡ�˿�
      if(GetCardLogicValue(cbCardData[j])!=cbLogicValue)
      {
        break;
      }

      //���ñ���
      cbSameCount++;
    }

    //ȫ���ض�
    cbSameCount = cbSameCount>bSearchCount?bSearchCount:cbSameCount;

    //���ý��
    BYTE cbIndex=AnalyseResult.cbBlockCount[cbSameCount-1]++;
    for(BYTE j=0; j<cbSameCount; j++)
    {
      AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];
    }


    //��������
    i+=cbSameCount-1;
  }
}

//�����˿�
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
  //�����˿�
  switch(cbValueIndex)
  {
    case 11:  //A �˿�
    {
      return (cbColorIndex<<4)|0x01;
    }
    case 12:  //2 �˿�
    {
      return (cbColorIndex<<4)|0x02;
    }
    case 13:  //С���˿�
    {
      return (cbColorIndex<<4)|0x01;
    }
    case 14:  //����˿�
    {
      return (cbColorIndex<<4)|0x02;
    }
    default:  //�����˿�
    {
      return (cbColorIndex<<4)|(cbValueIndex+3);
    }
  }

  return 0x00;
}

//�����ֲ�
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
  //���ñ���
  ZeroMemory(&Distributing,sizeof(Distributing));

  //���ñ���
  for(BYTE i=0; i<cbCardCount; i++)
  {
    if(cbCardData[i]==0)
    {
      continue;
    }

    //��ȡ����
    BYTE m_cbCardColor=GetCardColor(cbCardData[i]);
    BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

    //�ֲ���Ϣ
    Distributing.cbCardCount++;
    Distributing.cbDistributing[cbLogicValue-3][cbIndexCount]++;
    Distributing.cbDistributing[cbLogicValue-3][m_cbCardColor>>4]++;
  }

  //�˿˷���
  for(BYTE i=0; i<cbCardCount; i++)
  {
    //��������
    BYTE cbSameCount=1,cbCardValueTemp=0;
    BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

    //����ͬ��
    for(BYTE j=i+1; j<cbCardCount; j++)
    {
      //��ȡ�˿�
      if(GetCardLogicValue(cbCardData[j])!=cbLogicValue)
      {
        break;
      }

      //���ñ���
      cbSameCount++;
    }
    for(BYTE j=0; j<cbSameCount; j++)
    {
      Distributing.cbDistributing[cbLogicValue-3][cbIndexCount+1+j]=cbCardData[i+j];
    }

    //��������
    i+=cbSameCount-1;
  }


  return;
}

//����˳��
bool CGameLogic::SearchLinkCard(const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbFirstCardData,const BYTE cbLastCardData,tagOutCardResult &OutCardResult)
{

  //��ȡ��λ
  BYTE byFirstCardValue = GetCardLogicValue(cbFirstCardData);
  BYTE byLastCardValue = GetCardLogicValue(cbLastCardData);

  //��������
  if(byFirstCardValue > byLastCardValue)
  {
    BYTE byTemp = byFirstCardValue;
    byFirstCardValue = byLastCardValue;
    byLastCardValue = byTemp;
  }

  //�ж�˳��
  if(byFirstCardValue > 14 || byLastCardValue > 14 || byLastCardValue-byFirstCardValue<4)
  {
    return false;
  }

  //�������
  tagDistributing Distribute;
  BYTE byCardData[MAX_COUNT],byCardCount = cbHandCardCount;
  CopyMemory(byCardData,cbHandCardData,sizeof(BYTE)*byCardCount);
  ZeroMemory(&OutCardResult,sizeof(OutCardResult));

  //�����˿�
  AnalysebDistributing(byCardData,byCardCount,Distribute);

  //��������
  OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = cbFirstCardData;
  OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = cbLastCardData;

  //ѭ���ж�
  for(BYTE i = byFirstCardValue-2; i < byLastCardValue-3; i++)
  {
    if(Distribute.cbDistributing[i][cbIndexCount] > 0)
    {
      //��ȡ��ɫ
      BYTE cbHeadColor=0xFF;
      for(BYTE cbColorIndex=0; cbColorIndex<4; cbColorIndex++)
      {
        if(Distribute.cbDistributing[i][3-cbColorIndex]>0)
        {
          cbHeadColor=3-cbColorIndex;
          break;
        }
      }
      ASSERT(cbHeadColor!=0xFF);
      OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = MakeCardData(i,cbHeadColor);

    }
    else
    {
      break;
    }
  }
  if(i == byLastCardValue-3)
  {

    SortCardList(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
    return true;
  }

  //�������
  ZeroMemory(&OutCardResult,sizeof(OutCardResult));
  return false;
}

//����ͬ��
bool CGameLogic::SearchSamePointCard(const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbCardData,tagOutCardResult &OutCardResult)
{

  for(BYTE i=0; i<cbHandCardCount; i++)
  {
    if(GetCardLogicValue(cbHandCardData[i])==GetCardLogicValue(cbCardData))
    {
      OutCardResult.cbResultCard[OutCardResult.cbCardCount++] =cbHandCardData[i];
    }
  }

  return true;
}
//�Ƿ���ը��(û��������ͬ���� ��������С��)
bool CGameLogic::HasNotBombCard(BYTE cbHandCardData[],BYTE cbHandCardCount)
{
  SortCardList(cbHandCardData,cbHandCardCount);

  bool bHaveNotBomb=true;
  tagAnalyseInitResult AnalyseResult;
  ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));
  AnalyseInitCardData(cbHandCardData,cbHandCardCount,AnalyseResult);

  for(BYTE cbIndex=3; cbIndex<8; cbIndex++)
  {
    if(AnalyseResult.cbBlockCount[cbIndex]>=1)
    {
      bHaveNotBomb=false;
      break;
    }
  }
  return bHaveNotBomb;
}

//*********************************************
//���ܣ�  ��ȡը������
//������
//cbBombCount      ը������
//cbSameCount      ��ը
//cbMaxSameCount     ���ը
//cbMaxSameCountLimit    ���ը���Գ��ֵĴ���
//bLimitMaxBombCount     �������ը������
//**********************************************
bool CGameLogic::GetBombCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount,BYTE cbBombCount,BYTE cbSameCount,BYTE cbMaxSameCount,
                                 BYTE cbMaxSameCountLimit,bool bLimitMaxBombCount)
{

  if(cbBufferCount>=MAX_COUNT)
  {
    return false;
  }

  if(!bLimitMaxBombCount)
  {
    if(cbBufferCount+cbBombCount*cbSameCount+cbMaxSameCountLimit*(cbMaxSameCount-cbSameCount)>MAX_COUNT)
    {
      return false;
    }
  }

  bool bHasGetCard=false;

  if(cbMaxSameCountLimit>cbBombCount)
  {
    return false;
  }

  int MaxTimes=rand()%(cbMaxSameCountLimit+1);

  int CurBombCount=cbBombCount;
  if(bLimitMaxBombCount)
  {
    CurBombCount=rand()%(cbBombCount+1);

  }

  if(CurBombCount>0)
  {
    bHasGetCard=true;
  }

  for(int k=0; k<CurBombCount; k++)
  {
    m_cbColorIndex=0;
    for(int i=0; i<cbSameCount; i++)
    {
      cbCardBuffer[cbBufferCount++]=m_cbLowCardData[m_cbArrayIndex]|m_cbCardColor[m_cbColorIndex++];
    }

    if(MaxTimes>0)
    {
      for(int j=0; j<cbMaxSameCount-cbSameCount; j++)
      {
        cbCardBuffer[cbBufferCount++]=m_cbLowCardData[m_cbArrayIndex]|m_cbCardColor[m_cbColorIndex++];
      }
      MaxTimes--;

    }
    m_cbArrayIndex++;
  }

  return bHasGetCard;

}

//*****************************************
//���ܣ�  ��ȡ0ը����
//������  û���ĸ�ͷ�ģ���������С��
//*****************************************
VOID CGameLogic::GetZeroBombCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount)
{
  GetOtherCardData(cbCardBuffer,cbBufferCount,4);
  //RandomSetKingCard(cbCardBuffer,cbBufferCount);
}

//*****************************************
//���ܣ�  ��ȡС����ը����
//������  û���ĸ�ͷ�ģ���������С��
//*****************************************
VOID CGameLogic::GetLessThreeBombCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount,BYTE cbBombCount)
{

  GetBombCardData(cbCardBuffer,cbBufferCount,cbBombCount,4,5,1);
  GetOtherCardData(cbCardBuffer,cbBufferCount,4);

}

//��ȡ��������
VOID CGameLogic::GetSixXiangCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount)
{
  int selectPath=rand()%3;
  if(selectPath==0)//����ͷ
  {
    GetBombCardData(cbCardBuffer,cbBufferCount,1,6,6,0);
    //���1-3��ը ������һ����5��
    GetBombCardData(cbCardBuffer,cbBufferCount,3,4,5,1,true);
  }
  else if(selectPath==1)//��ը
  {
    //�ķ�ը ���������5��
    GetBombCardData(cbCardBuffer,cbBufferCount,4,4,5,2);
  }

  else//��������
  {
    GetBombCardData(cbCardBuffer,cbBufferCount,2,5,5,0);
    //�������ը ���������4��
    GetBombCardData(cbCardBuffer,cbBufferCount,2,4,4,0,true);
  }

  GetOtherCardData(cbCardBuffer,cbBufferCount,4);

}
//��ȡ��������
VOID CGameLogic::GetSevenXiangCardData(BYTE cbCardBuffer[], BYTE &  cbBufferCount)
{

  int selectPath=rand()%5;
  if(selectPath==0)//�߸�ͷ
  {

    NcaTextOut(TEXT("7��ͷ"));
    GetBombCardData(cbCardBuffer,cbBufferCount,1,7,7,0);
    //���һ��6��
    GetBombCardData(cbCardBuffer,cbBufferCount,1,6,6,0,true);
    //���һ��5��
    GetBombCardData(cbCardBuffer,cbBufferCount,1,5,5,0,true);
    //�������4��
    GetBombCardData(cbCardBuffer,cbBufferCount,2,4,4,0,true);
  }
  else if(selectPath==1)//��ը
  {
    NcaTextOut(TEXT("5��  4��ը"));
    //�帱ը
    GetBombCardData(cbCardBuffer,cbBufferCount,5,4,5,2);
  }
  else if(selectPath==2)//��������
  {
    NcaTextOut(TEXT("2��  6��"));
    //����6��
    GetBombCardData(cbCardBuffer,cbBufferCount,2,6,6,0);
    //���һ��5��
    GetBombCardData(cbCardBuffer,cbBufferCount,1,5,5,0,true);
    //�������4��
    GetBombCardData(cbCardBuffer,cbBufferCount,2,4,4,0,true);
  }
  else if(selectPath==3)//��������
  {
    NcaTextOut(TEXT("3��  5��"));
    //����5��
    GetBombCardData(cbCardBuffer,cbBufferCount,3,5,5,0);
    //�������4��
    GetBombCardData(cbCardBuffer,cbBufferCount,2,4,4,0,true);
  }
  else //��ը
  {
    NcaTextOut(TEXT("��ը��4�� 3��"));
    GetLinkBombCardData(cbCardBuffer,cbBufferCount,4,3);
    //���һ��6��
    GetBombCardData(cbCardBuffer,cbBufferCount,1,6,6,0,true);
    //���һ��5��
    GetBombCardData(cbCardBuffer,cbBufferCount,1,5,5,0,true);
  }
  GetOtherCardData(cbCardBuffer,cbBufferCount,4);

}

//��ȡ��������
VOID CGameLogic::GetEightXiangCardData(BYTE cbCardBuffer[], BYTE &  cbBufferCount)
{
  int selectPath=rand()%7;
  if(selectPath==0)//�˸�ͷ
  {
    NcaTextOut(TEXT("8��ͷ"));

    GetBombCardData(cbCardBuffer,cbBufferCount,1,8,8,0);
    int curBombCount=0;
    //���һ��6��
    if(GetBombCardData(cbCardBuffer,cbBufferCount,1,6,6,0,true))
    {
      curBombCount++;
    }
    if(GetBombCardData(cbCardBuffer,cbBufferCount,1,5,5,0,true))
    {
      curBombCount++;
    }
    //�������4��
    GetBombCardData(cbCardBuffer,cbBufferCount,2-curBombCount,4,4,0,true);

  }
  else if(selectPath==1)//��ը
  {
    NcaTextOut(TEXT("6�� 4��"));

    //����������5���ը
    GetBombCardData(cbCardBuffer,cbBufferCount,6,4,5,2);
  }

  //��������
  else if(selectPath==2)
  {
    NcaTextOut(TEXT("2�� 7��"));
    //����7��
    GetBombCardData(cbCardBuffer,cbBufferCount,2,7,7,0);
    //���һ��5��
    GetBombCardData(cbCardBuffer,cbBufferCount,1,5,5,0,true);
    //�������4��
    GetBombCardData(cbCardBuffer,cbBufferCount,2,4,4,0,true);
  }
  else if(selectPath==3)//3��������6��
  {
    NcaTextOut(TEXT("3�� 6��"));
    //����6��
    GetBombCardData(cbCardBuffer,cbBufferCount,3,6,6,0);
    //���һ��5��
    GetBombCardData(cbCardBuffer,cbBufferCount,1,4,5,1,true);

  }
  else if(selectPath==4)//4�� ��������5��
  {
    NcaTextOut(TEXT("4�� 5��"));

    //�ĸ�5��
    GetBombCardData(cbCardBuffer,cbBufferCount,4,5,5,0);
    //���һ��4��
    GetBombCardData(cbCardBuffer,cbBufferCount,1,4,4,0,true);
  }
  else if(selectPath==5)//��ը
  {
    NcaTextOut(TEXT("��ը��4�� 4��"));
    GetLinkBombCardData(cbCardBuffer,cbBufferCount,4,4);
    //�������4��
    GetBombCardData(cbCardBuffer,cbBufferCount,1,4,4,0,true);
  }
  else//��ը
  {
    NcaTextOut(TEXT("��ը��5�� 3��"));
    GetLinkBombCardData(cbCardBuffer,cbBufferCount,5,3);
    //�������4��
    GetBombCardData(cbCardBuffer,cbBufferCount,3,4,4,0,true);
  }

  GetOtherCardData(cbCardBuffer,cbBufferCount,4);
}

//��ȡ��������
VOID CGameLogic::GetNineXiangCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount)
{

  int selectPath=rand()%7;
  if(selectPath==0)//2��8��
  {

    NcaTextOut(TEXT("2�� 8��"));

    GetBombCardData(cbCardBuffer,cbBufferCount,2,8,8,0);
    int curBombCount=0;
    //���һ��6��
    if(GetBombCardData(cbCardBuffer,cbBufferCount,1,6,6,0,true))
    {
      curBombCount++;
    }
    if(GetBombCardData(cbCardBuffer,cbBufferCount,1,5,5,0,true))
    {
      curBombCount++;
    }
    //�������4��
    GetBombCardData(cbCardBuffer,cbBufferCount,2-curBombCount,4,4,0,true);
  }
  else if(selectPath==1)//3�� ������7��
  {
    NcaTextOut(TEXT("3�� 7��"));
    GetBombCardData(cbCardBuffer,cbBufferCount,3,7,7,0);
  }
  else if(selectPath==2)//4�� ��������6��
  {
    NcaTextOut(TEXT("4�� 6��"));
    GetBombCardData(cbCardBuffer,cbBufferCount,4,6,6,0);
  }
  else if(selectPath==3)//5�� ������5��
  {
    NcaTextOut(TEXT("5�� 5��"));
    GetBombCardData(cbCardBuffer,cbBufferCount,5,5,5,0);
  }
  else if(selectPath==4)//��ը
  {
    NcaTextOut(TEXT("��ը��4�� 5��"));

    GetLinkBombCardData(cbCardBuffer,cbBufferCount,4,5);
  }
  else if(selectPath==5)//��ը
  {
    NcaTextOut(TEXT("��ը��5�� 4��"));

    GetLinkBombCardData(cbCardBuffer,cbBufferCount,5,4);
    //��໹������һ�� 4-5��ը
    GetBombCardData(cbCardBuffer,cbBufferCount,1,4,5,1,true);
  }
  else//��ը
  {
    NcaTextOut(TEXT("��ը��6�� 3��"));
    GetLinkBombCardData(cbCardBuffer,cbBufferCount,6,3);
    //��໹������һ�� 4-8��ը
    if(!GetBombCardData(cbCardBuffer,cbBufferCount,1,4,5,1,true))
    {
      if(!GetBombCardData(cbCardBuffer,cbBufferCount,1,6,7,1,true))
      {
        //GetBombCardData(cbCardBuffer,cbBufferCount,1,8,8,0,true);
      }
    }
  }
  GetOtherCardData(cbCardBuffer,cbBufferCount,4);
}

//��ȡʮ������
VOID CGameLogic::GetTenXiangCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount)
{
  int selectPath=rand()%100;
  //��ը
  int RandNum = rand()%4;
  if(RandNum==0)
  {
    NcaTextOut(TEXT("��ը��4�� 6��"));
    GetLinkBombCardData(cbCardBuffer,cbBufferCount,4,6);
  }
  else if(RandNum==1)
  {
    NcaTextOut(TEXT("��ը��5�� 5��"));
    GetLinkBombCardData(cbCardBuffer,cbBufferCount,5,5);
  }
  else if(RandNum==2)
  {
    NcaTextOut(TEXT("��ը��6�� 4��"));
    GetLinkBombCardData(cbCardBuffer,cbBufferCount,6,4);
  }
  else
  {
    NcaTextOut(TEXT("��ը��7�� 3��"));
    GetLinkBombCardData(cbCardBuffer,cbBufferCount,7,3);
  }

  GetOtherCardData(cbCardBuffer,cbBufferCount,4);

}

//��ȡʮһ������
VOID CGameLogic::GetElevenXiangCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount)
{

  GetOtherCardData(cbCardBuffer,cbBufferCount,8);

}

//��ȡʮ��������
VOID CGameLogic::GetTwelveXiangCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount)
{
  GetOtherCardData(cbCardBuffer,cbBufferCount,8);
}



//��ͬ����
bool CGameLogic::SearchSameLinkCard(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE bCardCount,BYTE bSearchLineCount,tagAnalyseInitResult &AnalyseResult)
{
  //����ṹ
  tagAnalyseInitResult TempAnalyseResult;
  ZeroMemory(&TempAnalyseResult,sizeof(TempAnalyseResult));

  //������Ŀ
  BYTE bSearchCount = bCardCount;
  AnalyseSameCardData(cbHandCardData,cbHandCardCount,bSearchCount,TempAnalyseResult);

  BYTE bBlockCount = TempAnalyseResult.cbBlockCount[bSearchCount-1];
  if(bBlockCount>0)
  {
    for(BYTE i=bBlockCount; i>=0; i--)
    {
      //�����߽�
      if(i>bBlockCount || i<0)
      {
        break;
      }

      BYTE bLinkCount = 0;
      BYTE bCardData[27] = {0};

      //�����һ����ͬ
      bCardData[bLinkCount] = TempAnalyseResult.cbCardData[bSearchCount-1][i*bSearchCount -1];
      for(BYTE m=i*bSearchCount-1; i>1 && m>=i*bSearchCount - bSearchCount; m--)
      {
        bCardData[bLinkCount++] =  TempAnalyseResult.cbCardData[bSearchCount-1][m];
      }

      //����������ӵ���ͬ
      BYTE bStepInc = 1;
      for(BYTE j=i*bSearchCount -bSearchCount-1; j>=0; j-=bSearchCount)
      {
        if(j<0 || j>i*bSearchCount -bSearchCount-1)
        {
          break;
        }

        //2����������
        if(GetCardLogicValue(TempAnalyseResult.cbCardData[bSearchCount-1][j])>13)
        {
          break;
        }

        if(GetCardLogicValue(TempAnalyseResult.cbCardData[bSearchCount-1][j]) == GetCardLogicValue(bCardData[0])+bStepInc)
        {
          if(j+1>=bSearchCount)
          {
            for(BYTE n=j; n>=j-bSearchCount+1; n--)
            {
              if(n<0 || n>j)
              {
                break;
              }
              bCardData[bLinkCount++] =  TempAnalyseResult.cbCardData[bSearchCount-1][n];
            }
          }

          bStepInc++;
        }
      }

      //����3������
      if(bLinkCount>=bSearchCount*bSearchLineCount-1)
      {
        AnalyseResult.cbBlockCount[bSearchCount-1] = bLinkCount;
        CopyMemory(AnalyseResult.cbCardData[bSearchCount-1],bCardData,sizeof(bCardData));
        return true;
      }
    }
  }

  return false;
}



//��ȡn��ը����
VOID CGameLogic::GetLinkBombCardData(BYTE cbCardBuffer[], BYTE  & cbBufferCount,BYTE cbCardCount,BYTE cbLinkCount)
{
  if(cbCardCount*cbLinkCount>MAX_COUNT||cbCardCount>7||cbLinkCount>6)
  {
    return;
  }
  BYTE LowCardData[13]= {0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x01,0x02};
  m_cbArrayIndex=0;
  m_cbColorIndex=0;
  int nStartNumber=rand()%(13-cbLinkCount+1);
  BYTE TempCardData[13];
  ZeroMemory(TempCardData,13);
  for(int i=0; i<cbLinkCount; i++)
  {
    TempCardData[i]=LowCardData[nStartNumber+i];
    LowCardData[nStartNumber+i]=0;
  }
  int index=cbLinkCount;
  for(int i=0; i<13; i++)
  {
    if(LowCardData[i]!=0)
    {
      TempCardData[index++]=LowCardData[i];
    }
  }

  //�����ĸ����ܺ�ǰ������
  bool bEnd=false;
  do
  {
    bEnd = true;
    RandCardListData(&TempCardData[cbLinkCount],13-cbLinkCount);

    for(int i=0; i<cbLinkCount; i++)
    {
      for(int j=0; j<4; j++)
      {
        int SubValue=TempCardData[i]-TempCardData[cbLinkCount+j];
        if(SubValue==1||SubValue==-1)
        {
          bEnd=false;
          break;
        }
      }

    }

  }
  while(!bEnd);

  memcpy(m_cbLowCardData,TempCardData,13);

  //��������
  //��� cbLinkCount-1�����Զ�һ��
  int nCanOverOne=cbLinkCount-1;
  if(cbLinkCount>=6)
  {
    nCanOverOne=cbLinkCount-4;
  }
  else if(cbLinkCount>=5)
  {
    nCanOverOne=cbLinkCount-3;
  }
  else if(cbLinkCount>=4)
  {
    nCanOverOne=cbLinkCount-2;
  }

  if(cbCardCount>=6)
  {
    nCanOverOne=0;
  }
  for(int i=0; i<cbLinkCount; i++)
  {
    if(nCanOverOne>=1)
    {
      GetBombCardData(cbCardBuffer,cbBufferCount,1,cbCardCount,cbCardCount+1,1);
      nCanOverOne--;
    }
    else
    {
      GetBombCardData(cbCardBuffer,cbBufferCount,1,cbCardCount,cbCardCount,0);
    }

  }
}

//�������
VOID CGameLogic::RandomSetKingCard(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
  if(m_cbKingCount>0)
  {
    RandCardListData(cbCardBuffer,cbBufferCount);
    int index=0,kingIndex=0;
    for(int i=0; i<m_cbKingCount; i++)
    {
      cbCardBuffer[index++]=m_cbKing[kingIndex++];
    }
  }
}

//����������
VOID CGameLogic::GetOtherCardData(BYTE cbCardBuffer[],BYTE  & cbBufferCount,BYTE cbSameCount)
{
  int nLeaveCardCount = MAX_COUNT - cbBufferCount;
  if(nLeaveCardCount<=0)
  {
    return;
  }
  //��ȡ�˿�
  BYTE cbAllCardData[FULL_COUNT],cbLeaveCardData[MAX_COUNT];

  do
  {
    ZeroMemory(cbAllCardData,sizeof(cbAllCardData));
    ZeroMemory(cbLeaveCardData,sizeof(cbLeaveCardData));
    RandCardList(cbAllCardData,FULL_COUNT);
    //ɾ���˿�
    RemoveCardEx(cbCardBuffer,cbBufferCount,cbAllCardData,FULL_COUNT);


    int index=0;
    for(int i=0; i<FULL_COUNT; i++)
    {
      if(index>=nLeaveCardCount)
      {
        break;
      }

      if(cbAllCardData[i]==0x4E||cbAllCardData[i]==0x4F)
      {
        continue;
      }

      if(cbAllCardData[i]!=0)
      {
        bool bFindSameValue=false;
        for(int j=0; j<cbBufferCount; j++)
        {
          if(GetCardValue(cbCardBuffer[j])==GetCardValue(cbAllCardData[i]))
          {
            bFindSameValue=true;
            break;
          }
        }
        if(!bFindSameValue)
        {
          cbLeaveCardData[index++]=cbAllCardData[i];
        }
      }
    }


  }
  while(HasNSameCountCard(cbLeaveCardData,nLeaveCardCount,cbSameCount));



#ifdef _DEBUG



  for(int i=0; i<nLeaveCardCount; i++)
  {
    int sameCount=1;
    for(int j=i+1; j<nLeaveCardCount; j++)
    {
      if(GetCardValue(cbLeaveCardData[j])==GetCardValue(cbLeaveCardData[i]))
      {
        sameCount++;
      }
    }

    if(sameCount>=cbSameCount)
    {

      CString strInfo=TEXT("");
      strInfo.Format(TEXT("��ȡ���������ݴ��� Ҫ�󲻴���%d��ʵ����%d"),cbSameCount,sameCount);
      NcaTextOut(strInfo);

    }

  }

#endif

  {
    RandomSetKingCard(cbLeaveCardData,nLeaveCardCount);
  }


  memcpy(&cbCardBuffer[cbBufferCount],cbLeaveCardData,nLeaveCardCount*sizeof(BYTE));

  cbBufferCount=MAX_COUNT;

}


//��ȡ������
VOID CGameLogic::GetXiangCardDataByIndex(BYTE cbCardBuffer[], BYTE  & cbBufferCount,BYTE cbXiangIndex)
{
  RandCardListData(m_cbCardColor,8);
  RandCardListData(m_cbKing,4);
  RandCardListNoLink(m_cbLowCardData,13,6);
  m_cbArrayIndex=0;
  m_cbColorIndex=0;
  switch(cbXiangIndex)
  {
    case 1:
    {
      GetZeroBombCardData(cbCardBuffer,cbBufferCount);
      break;
    }
    case 2:
    case 3:
    case 4:
    {
      GetLessThreeBombCardData(cbCardBuffer,cbBufferCount,cbXiangIndex-1);
      break;
    }
    case 5:
    {
      GetSixXiangCardData(cbCardBuffer,cbBufferCount);
      break;
    }
    case 6:
    {
      GetSevenXiangCardData(cbCardBuffer,cbBufferCount);
      break;
    }
    case 7:
    {
      GetEightXiangCardData(cbCardBuffer,cbBufferCount);
      break;
    }
    case 8:
    {
      GetNineXiangCardData(cbCardBuffer,cbBufferCount);
      break;
    }
    case 9:
    {
      GetTenXiangCardData(cbCardBuffer,cbBufferCount);
      break;
    }
    case 10:
    {
      GetElevenXiangCardData(cbCardBuffer,cbBufferCount);
      break;
    }
    case 11:
    {
      GetTwelveXiangCardData(cbCardBuffer,cbBufferCount);
      break;
    }
    case 12:
    {
      GetLinkBombCardData(cbCardBuffer,cbBufferCount,5,3);
      break;
    }
    case 13://������
    {
      BYTE cbCard[27]= {0x04,0x14,0x24,
                        0x02,0x12,0x22,
                        0x03,0x13,0x23,0x33,0x03,
                        0x05,0x15,0x25,0x35,
                        0x06,0x16,0x37,0x27,0x37,0x17,
                        0x0C,0x1C,0x1C,0x2C,
                        0x4E,0x4F
                       };

      memcpy(cbCardBuffer,cbCard,sizeof(cbCard));
      cbBufferCount = 27;
      break;
    }
  }
  if(cbBufferCount<MAX_COUNT)
  {
    GetOtherCardData(cbCardBuffer,cbBufferCount,8);
  }
//#ifdef _DEBUG
  CString strInfo=TEXT("");
  if(cbXiangIndex<5)
  {
    strInfo.Format(TEXT("����%dը���� "),cbXiangIndex-1);
  }
  else
  {
    strInfo.Format(TEXT("����%d������ "),cbXiangIndex+1);
  }

  NcaTextOut(strInfo);

  strInfo=TEXT("");
  for(int i=0; i<cbBufferCount; i++)
  {
    CString strOne=TEXT("");
    strOne.Format(TEXT(" %x "),cbCardBuffer[i]);
    strInfo+=strOne;
  }
  NcaTextOut(strInfo);

//#endif
}

//�б�����
VOID CGameLogic::RandCardListData(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
  for(int i=0; i<=1000; i++)
  {
    int q=rand()%cbBufferCount;
    int j=rand()%cbBufferCount;
    if(q!=j)
    {
      BYTE cbTemp=cbCardBuffer[q];
      cbCardBuffer[q]=cbCardBuffer[j];
      cbCardBuffer[j]=cbTemp;
    }
  }
}
//�Ƿ�����(�ϸ�Ҫ������һ��������)
bool CGameLogic::IsLinkCardData(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
  //��ȡ����
  for(int i=0; i<cbBufferCount; i++)
  {
    for(int j=i+1; j<cbBufferCount; j++)
    {
      if(GetCardLogicValue(cbCardBuffer[j])-GetCardLogicValue(cbCardBuffer[i])==1||
         GetCardLogicValue(cbCardBuffer[j])-GetCardLogicValue(cbCardBuffer[i])==-1)
      {
        return true;
      }
    }

  }
  return false;

}
//�б�����������
VOID CGameLogic::RandCardListNoLink(BYTE cbCardBuffer[], BYTE cbBufferCount,BYTE cbNeedCount)
{

  if(cbBufferCount>13||cbNeedCount>cbBufferCount)
  {
    return;
  }

  BYTE TempCardData[13];
  ZeroMemory(TempCardData,sizeof(TempCardData));
  CopyMemory(TempCardData,cbCardBuffer,sizeof(BYTE)*cbBufferCount);

  bool bEnd=false;

  do
  {
    RandCardListData(TempCardData,cbBufferCount);
    if(!IsLinkCardData(TempCardData,cbNeedCount))
    {
      bEnd = true;
    }

  }
  while(!bEnd);
  CopyMemory(cbCardBuffer,TempCardData,sizeof(BYTE)*cbBufferCount);

}

//�Ƿ���nSameCount�����ϵ���
bool CGameLogic::HasNSameCountCard(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE nSameCount)
{
  if(nSameCount>8)
  {
    return false;
  }

  for(int i=0; i<cbHandCardCount; i++)
  {
    int sameCount=1;
    for(int j=i+1; j<cbHandCardCount; j++)
    {
      if(GetCardValue(cbHandCardData[j])==GetCardValue(cbHandCardData[i]))
      {
        sameCount++;
      }
    }

    if(sameCount>=nSameCount)
    {
      return true;
    }
  }
  return false;
}


