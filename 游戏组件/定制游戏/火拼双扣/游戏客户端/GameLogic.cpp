#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//��������
const BYTE cbIndexCount=5;

//�˿�����
const BYTE  CGameLogic::m_cbCardData[FULL_COUNT]=
{
  //0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D, //���� A - K
  //0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D, //÷�� A - K
  //0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D, //���� A - K
  //0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D, //���� A - K
  //0x4E,0x4F,
  //0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D, //���� A - K
  //0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D, //÷�� A - K
  //0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D, //���� A - K
  //0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D, //���� A - K
  //0x4E,0x4F,
  0x01,0x11,0x21,0x31,0x01,0x11,0x21,0x31,0x02,0x12,0x22,0x22,0x32,0x02,0x12,0x32,
  0x03,0x13,0x23,0x33,0x03,0x13,0x23,0x33,0x04,0x14,0x24,0x24,0x34,0x04,0x14,0x34,
  0x05,0x15,0x25,0x35,0x05,0x15,0x25,0x35,0x06,0x4E,0x16,0x26,0x26,0x36,0x06,0x16,
  0x36,0x07,0x17,0x27,0x37,0x07,0x17,0x27,0x37,0x08,0x18,0x28,0x28,0x38,0x08,0x18,
  0x38,0x4E,0x09,0x19,0x29,0x39,0x09,0x19,0x29,0x39,0x0A,0x1A,0x2A,0x2A,0x4F,0x3A,
  0x0A,0x1A,0x3A,0x0B,0x1B,0x2B,0x3B,0x0B,0x1B,0x2B,0x3B,0x0C,0x1C,0x2C,0x2C,0x3C,
  0x0C,0x1C,0x3C,0x0D,0x0D,0x1D,0x1D,0x2D,0x2D,0x3D,0x3D,0x4F,
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}



//�鿴�Ƿ����� �����ڼ����ӣ�ͬ���ţ����öԣ���ͬ����,�������Ƶ�ը����˳�ӵ�����
bool CGameLogic::IsStructureLink(tagAnalyseResult & AnalyseResult,int cbSameCardNumber,int cbLinkLong)
{
  //����Ƶ�����(��ȥ��С����)���������������
  if(AnalyseResult.m_cbCardStyleNumber>cbLinkLong)
  {
    return false;
  }

  if(AnalyseResult.m_cbCardStyleNumber==0)
  {
    AnalyseResult.m_MinCardValue=GetCardLogicValue(0x41);
  }
  else
  {
    int nOutIndex=AnalyseResult.m_cbCardStyleNumber-1;
    int nInIndex=0;
    int  nDifference=0;
    AnalyseResult.m_MinCardValue=GetCardLogicValue(AnalyseResult.m_CardStyle[nOutIndex].m_cbCard);
    BYTE nBeforeCard=GetCardLogicValue(AnalyseResult.m_CardStyle[nOutIndex].m_cbCard);

    if(GetCardLogicValue(AnalyseResult.m_CardStyle[0].m_cbCard)!=15)  //�鿴2�Ƿ�����������
    {
      for(; nOutIndex>=0; nOutIndex--) //����Ԥ�ڳ��Ƚ��в���
      {
        //��ʼ�����������в��Ʋ���
        nDifference=cbSameCardNumber-AnalyseResult.m_CardStyle[nOutIndex].m_cbCounter;
        if(nDifference<0)   //���ѡ����ͬ�����ֽ�ƶ���������򷵻ش���
        {
          return false;
        }
        else
        {
          if(nDifference<=AnalyseResult.m_cbJockerNumber)
          {
            AnalyseResult.m_CardStyle[nOutIndex].m_cbCounter+=nDifference;
            AnalyseResult.m_cbJockerNumber-=nDifference;
          }
          else
          {
            return false;
          }
        }
        //���Ʋ�������



        if(nOutIndex!=AnalyseResult.m_cbCardStyleNumber-1)
        {
          int nResult=GetCardLogicValue(AnalyseResult.m_CardStyle[nOutIndex].m_cbCard)-nBeforeCard;
          if(nResult==1) //�ж��Ƿ�����
          {
            nBeforeCard=GetCardLogicValue(AnalyseResult.m_CardStyle[nOutIndex].m_cbCard);
          }
          else           //�����������������Ʋ���
          {
            if((nResult-1)*cbSameCardNumber<=AnalyseResult.m_cbJockerNumber)
            {
              AnalyseResult.m_cbJockerNumber-=(nResult-1)*cbSameCardNumber;
              nBeforeCard=GetCardLogicValue(AnalyseResult.m_CardStyle[nOutIndex].m_cbCard);
            }
            else
            {
              return false;
            }
          }
        }
      }


      //����������ҷ��ֻ��б��ƣ�������ص�����ϵĶ��׻��β
      if(AnalyseResult.m_cbJockerNumber>0)
      {
        if(AnalyseResult.m_cbJockerNumber%cbSameCardNumber==0)
        {
          int k=0;
          for(k=0; k<AnalyseResult.m_cbJockerNumber/cbSameCardNumber; k++)
          {
            if((GetCardLogicValue(AnalyseResult.m_CardStyle[0].m_cbCard)+k)<GetCardValue(0x31))
            {
              continue;
            }
            else
            {
              AnalyseResult.m_MinCardValue-=1;
            }
          }
        }
        else
        {
          return false;
        }
      }
    }
    else
    {
      return false;
    }
  }
  return true;
}

BYTE CGameLogic::GetNoSameCard(const BYTE cbCardData[], int cbCardCount,int&nCardCount,int&nKingCount)
{

}

BYTE CGameLogic::GetCardType(const BYTE cbCardData[], int cbCardCount,tagAnalyseResult & AnalyseResult)
{

  tagAnalyseResult bkupAnalyse;
  AnalysebCardData(cbCardData,cbCardCount,bkupAnalyse);
  memcpy(&AnalyseResult,&bkupAnalyse,sizeof(tagAnalyseResult));

  switch(cbCardCount)
  {
    case 0: //����
    {
      return CT_ERROR;
    }
    case 1: //����
    {
      AnalyseResult.m_MinCardValue=GetCardLogicValue(cbCardData[0]);
      return CT_SINGLE;
    }
    case 2: //����
    {
      if(IsStructureLink(AnalyseResult,2,1))
      {
        return CT_DOUBLE;
      }
      return CT_ERROR;
    }
    case 3: //����
    {
      if(IsStructureLink(AnalyseResult,3,1))
      {
        if(GetCardLogicValue(AnalyseResult.m_CardStyle[0].m_cbCard)==14)
        {
          return CT_BOMB;
        }
        else
        {
          return CT_THREE;
        }
      }
      return CT_ERROR;
    }
  }

  //�����˿�
  memcpy(&AnalyseResult,&bkupAnalyse,sizeof(tagAnalyseResult));
  if((cbCardCount==4||cbCardCount==5)&&IsBomb(AnalyseResult,cbCardCount))
  {
    return CT_BOMB;
  }

  //��������
  memcpy(&AnalyseResult,&bkupAnalyse,sizeof(tagAnalyseResult));
  if(cbCardCount>=5&&IsStructureLink(AnalyseResult,1,cbCardCount))
  {
    return CT_SINGLE_LINK;
  }

  //��������
  memcpy(&AnalyseResult,&bkupAnalyse,sizeof(tagAnalyseResult));
  if(cbCardCount%2==0&&cbCardCount>=4&&IsStructureLink(AnalyseResult,2,cbCardCount/2))
  {
    return CT_DOUBLE_LINK;
  }

  //��������
  memcpy(&AnalyseResult,&bkupAnalyse,sizeof(tagAnalyseResult));
  if(cbCardCount%3==0&&cbCardCount>=6&&IsStructureLink(AnalyseResult,3,cbCardCount/3))
  {
    return CT_THREE_LINK;
  }

  //��ը����
  memcpy(&AnalyseResult,&bkupAnalyse,sizeof(tagAnalyseResult));
  if(cbCardCount%3==0&&cbCardCount>=6&&IsStructureLink(AnalyseResult,3,cbCardCount/3))
  {
    return CT_THREE_LINK;
  }


  //��������
  memcpy(&AnalyseResult,&bkupAnalyse,sizeof(tagAnalyseResult));
  if(cbCardCount==5&&ThreeDoubleLink(AnalyseResult))
  {
    return CT_THREE_DOUBLE_LINK;
  }

  return CT_ERROR;

}


//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount,BYTE &cbStarLevel)
{
  //������ʼ��
  cbStarLevel=0;

  //������
  switch(cbCardCount)
  {
    case 0: //����
    {
      return CT_ERROR;
    }
    case 1: //����
    {
      return CT_SINGLE;
    }
  }

  //��ը����
  if(cbCardCount>=12)
  {
    //��������
    BYTE cbCardIndex=0;
    BYTE cbBlockCount=0;
    BYTE cbBombCardData[MAX_COUNT/4];
    BYTE cbSmallCount=0;
    BYTE cbMaxCount=0;
    ZeroMemory(cbBombCardData,sizeof(cbBombCardData));

    //�˿˷���
    do
    {
      //��������
      BYTE cbSameCount=1;
      BYTE cbCardValue=GetCardLogicValue(cbCardData[cbCardIndex]);

      //ͬ������
      for(BYTE i=cbCardIndex+1; i<cbCardCount; i++)
      {
        if(GetCardLogicValue(cbCardData[i])==cbCardValue)
        {
          cbSameCount++;
        }
        else
        {
          break;
        }
      }

      //�����ж�
      if(cbSameCount>=4)
      {
        cbBombCardData[cbBlockCount]=cbCardData[cbCardIndex];
        cbBlockCount++;
        cbCardIndex+=cbSameCount;
        if(cbSmallCount==0)
        {
          cbSmallCount=cbSameCount;
        }
        else
        {
          cbSmallCount=min(cbSameCount,cbSmallCount);
        }
        if(cbMaxCount==0)
        {
          cbMaxCount=cbSameCount;
        }
        else
        {
          cbMaxCount=max(cbMaxCount,cbSameCount);
        }

      }
      else
      {
        break;
      }

    }
    while(cbCardIndex<cbCardCount);

    //����ж�
    if((cbBlockCount>=3)&&(cbCardIndex==cbCardCount)&&(cbCardCount%cbBlockCount==0)&&IsStructureLink(cbCardData,cbCardCount,cbCardCount/cbBlockCount,true)&&(cbSmallCount==cbMaxCount))
    {
      cbStarLevel=cbBlockCount+cbCardCount/cbBlockCount;
      return CT_BOMB_LINK;
    }
    if((cbBlockCount>=3)&&(cbCardIndex==cbCardCount)&&(IsStructureLink(cbBombCardData,cbBlockCount,1,true)))
    {
      cbStarLevel=cbBlockCount+cbSmallCount;
      return CT_BOMB_LINK;
    }
  }

  //�����˿�
  tagAnalyseResult AnalyseResult;
  AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);
  //3��ը��
  if((cbCardCount==3)&&(cbCardData[0]==0x4E)&&(cbCardData[2]==0x4E))
  {
    cbStarLevel= 6;
    return CT_BOMB_3W;
  }

  //ͬ���ж�
  if((cbCardCount==3)&&(AnalyseResult.cbBlockCount[2]==1))
  {
    return CT_THREE;  //3��ͬ��
  }
  if((cbCardCount==2)&&(AnalyseResult.cbBlockCount[1]==1))
  {
    return CT_DOUBLE;  //��������
  }


  //����ը��
  if((cbCardCount==4)&&(cbCardData[0]==0x4E)&&(cbCardData[3]==0x4E)&&(AnalyseResult.cbBlockCount[3]==1))
  {
    cbStarLevel= 7;//4��ը��7�Ǽ� ���
    return CT_BOMB_TW;
  }
  //ը������
  if((cbCardCount>=4)&&(cbCardCount<=10)&&(AnalyseResult.cbBlockCount[cbCardCount-1]==1))
  {
    cbStarLevel= cbCardCount;
    return CT_BOMB;
  }

  //��������
  if((cbCardCount>=6)&&((AnalyseResult.cbBlockCount[1]*2)==cbCardCount))
  {
    BYTE cbDoubleCount=AnalyseResult.cbBlockCount[1]*2;
    if(IsStructureLink(AnalyseResult.cbCardData[1],cbDoubleCount,2)==true)
    {
      return CT_DOUBLE_LINK;
    }
  }

  //��������
  if((cbCardCount>=9)&&((AnalyseResult.cbBlockCount[2]*3)==cbCardCount))
  {
    BYTE cbThreeCount=AnalyseResult.cbBlockCount[2]*3;
    if(IsStructureLink(AnalyseResult.cbCardData[2],cbThreeCount,3)==true)
    {
      return CT_THREE_LINK;
    }
  }

  //˳������
  if((cbCardCount>=5)&&AnalyseResult.cbBlockCount[0]==cbCardCount)
  {
    if(IsStructureLink(AnalyseResult.cbCardData[0],cbCardCount,1)==true)
    {
      return CT_SHUNZI;
    }
  }

  return CT_ERROR;
}



//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
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
    if(cbSortType==ST_VALUE)
    {
      cbSortValue[i]=GetCardValue(cbCardData[i]);
    }
    else
    {
      cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
    }
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
    tagAnalyseResult AnalyseResult;
    AnalysebCardData(&cbCardData[cbCardIndex],cbCardCount-cbCardIndex,AnalyseResult);

    //��ȡ�˿�
    for(BYTE i=0; i<CountArray(AnalyseResult.cbBlockCount); i++)
    {
      //�����˿�
      BYTE cbIndex=CountArray(AnalyseResult.cbBlockCount)-i-1;
      CopyMemory(&cbCardData[cbCardIndex],AnalyseResult.cbCardData[cbIndex],AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE));

      //��������
      cbCardIndex+=AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE);
    }
  }

  return;
}

//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount,bool bSecond)
{


  //����׼��
  BYTE cbCardData[CountArray(m_cbCardData)];
  CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

  //����Ŀ¼
  TCHAR m_szWorkDirectory[MAX_PATH];
  GetModuleFileName(AfxGetInstanceHandle(),m_szWorkDirectory,sizeof(m_szWorkDirectory));
  int nModuleLen=lstrlen(m_szWorkDirectory);
  TCHAR m_szDllName[MAX_PATH];
  CopyMemory(m_szDllName,AfxGetApp()->m_pszExeName,sizeof(TCHAR)*MAX_PATH);
  int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
  m_szWorkDirectory[nModuleLen-nProcessLen]=0;
  SetCurrentDirectory(m_szWorkDirectory);

  //��ȡ�˿�
  TCHAR szFile[MAX_PATH]=TEXT("");
  BYTE cbPosition=0;
  _sntprintf(szFile,CountArray(szFile),TEXT("%s\\ShuangKouQB.INI"),m_szWorkDirectory);
  cbPosition=GetPrivateProfileInt(TEXT("ShuangKouQB"),TEXT("DisCardCount"),4,szFile);
  if(cbPosition>8)
  {
    cbPosition=4;
  }

  //�����˿�
  BYTE cbRandCount=0,cbTempRandCount=0;
  if(bSecond)
  {
    do
    {
      cbTempRandCount=0;
      cbPosition=0;
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

  }
  else
  {
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
  }


  //CopyMemory(cbCardBuffer,m_cbCardData,sizeof(m_cbCardData));

  /*
  //����׼��
  BYTE cbCardData[CountArray(m_cbCardData)];
  CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

  //�����˿�
  BYTE cbRandCount=0,cbPosition=0;
  do
  {
    cbPosition=rand()%(cbBufferCount-cbRandCount);
    cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
    cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
  } while (cbRandCount<cbBufferCount);
  */

  return;
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
  ASSERT(cbDeleteCount==cbRemoveCount);
  if(cbDeleteCount!=cbRemoveCount)
  {
    return false;
  }

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

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
  //��ȡ����
  BYTE cbCardColor=GetCardColor(cbCardData);
  BYTE cbCardValue=GetCardValue(cbCardData);

  //��Ч�ж�
  if((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D))
  {
    return true;
  }
  if(cbCardData==0x4F||cbCardData==0x4E)
  {
    return true;
  }
  return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
  //�˿�����
  BYTE cbCardColor=GetCardColor(cbCardData);
  BYTE cbCardValue=GetCardValue(cbCardData);

  //�����˿�
  if(cbCardValue>=0x0E)
  {
    return cbCardValue+2;
  }

  //ת����ֵ
  return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{

  //���ñ���
  BYTE cbFirstStarLevel;
  BYTE cbNextStarLevel;

  //�����ж�
  BYTE cbNextType=GetCardType(cbNextCard,cbNextCount,cbNextStarLevel);
  BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount,cbFirstStarLevel);

  //ը������
  if((cbFirstType>=CT_BOMB)||(cbNextType>=CT_BOMB))
  {
    //ը������
    if(cbNextType<CT_BOMB)
    {
      return false;
    }
    if(cbFirstType<CT_BOMB)
    {
      return true;
    }

    //�Ǽ��Ƚ�
    if(cbFirstStarLevel != cbNextStarLevel)
    {
      return cbNextStarLevel>cbFirstStarLevel;
    }
    else
    {
      //45��Ƚ�
      if((cbFirstStarLevel<=5)&&(cbFirstStarLevel>=4))
      {
        //��������
        BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
        BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);
        return cbConsultNext>cbConsultFirst;
      }
      //6��Ƚ�
      if(cbFirstStarLevel==6)
      {
        //�׼�3��
        if(cbFirstCount==3)
        {
          return true;
        }
        //�¼�3��
        if(cbNextCount==3)
        {
          return false;
        }

        //��������
        BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
        BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);
        return cbConsultNext>cbConsultFirst;

      }
      //7��Ƚ�
      if(cbFirstStarLevel==7)
      {
        //�׼�����
        if(cbFirstCount==4) //������7��������
        {
          return false;
        }
        //�¼�����
        if(cbNextCount==4)
        {
          return true;
        }
        //ͬ��Ƚ�
        if((cbFirstType==CT_BOMB)&&(cbNextType==CT_BOMB_LINK))
        {
          return false;

        }
        //ͬ��Ƚ�
        if((cbFirstType==CT_BOMB_LINK)&&(cbNextType==CT_BOMB))
        {
          return true;

        }
        //ͬ��Ƚ�
        if((cbFirstType==CT_BOMB)&&(cbNextType)==CT_BOMB)
        {
          //��������
          BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
          BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);
          return cbConsultNext>cbConsultFirst;

        }
        //��ը�Ƚ�
        if((cbFirstType==CT_BOMB_LINK)&&(cbNextType==CT_BOMB_LINK))
        {

          //��������
          BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
          BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);

          //�˿˷���
          if(cbNextType==CT_BOMB_LINK)
          {
            //�˿�����
            for(BYTE i=0; i<cbNextCount; i++)
            {
              BYTE cbCardValue=GetCardLogicValue(cbNextCard[i]);
              if(cbCardValue<cbConsultNext)
              {
                cbConsultNext=cbCardValue;
              }
            }
          }

          //�˿˷���
          if(cbFirstType==CT_BOMB_LINK)
          {

            //�˿�����
            for(BYTE i=0; i<cbFirstCount; i++)
            {
              BYTE cbCardValue=GetCardLogicValue(cbFirstCard[i]);
              if(cbCardValue<cbConsultFirst)
              {
                cbConsultFirst=cbCardValue;
              }
            }
          }
          return cbConsultNext>cbConsultFirst;

        }

        ASSERT(FALSE);
        return false;
      }

      //8�Ǽ�
      if(cbFirstStarLevel>=8)
      {
        //ͬ��Ƚ�
        if((cbFirstType==CT_BOMB)&&(cbNextType==CT_BOMB_LINK))
        {
          return false;

        }
        //ͬ��Ƚ�
        if((cbFirstType==CT_BOMB_LINK)&&(cbNextType==CT_BOMB))
        {
          return true;

        }
        //ͬ��Ƚ�
        if((cbFirstType==CT_BOMB)&&(cbNextType)==CT_BOMB)
        {
          //��������
          BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
          BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);
          return cbConsultNext>cbConsultFirst;

        }
        if((cbFirstType==CT_BOMB_LINK)&&(cbNextType==CT_BOMB_LINK))
        {

          //��������
          BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
          BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);


          //������ͬ
          if(cbNextType==CT_BOMB_LINK)
          {

            //�˿�����
            for(BYTE i=0; i<cbNextCount; i++)
            {
              BYTE cbCardValue=GetCardValue(cbNextCard[i]);
              if(cbCardValue<cbConsultNext)
              {
                cbConsultNext=cbCardValue;
              }
            }
          }

          //�˿˷���
          if(cbFirstType==CT_BOMB_LINK)
          {

            //�˿�����
            for(BYTE i=0; i<cbFirstCount; i++)
            {
              BYTE cbCardValue=GetCardValue(cbFirstCard[i]);
              if(cbCardValue<cbConsultFirst)
              {
                cbConsultFirst=cbCardValue;
              }
            }
          }
          return cbConsultNext>cbConsultFirst;

        }

        ASSERT(FALSE);
        return false;
      }

    }
  }

  //��ͬ����
  if(cbFirstType!=cbNextType)
  {
    return false;
  }

  //��ͬ����
  switch(cbFirstType)
  {
    case CT_SINGLE:       //��������
    case CT_DOUBLE:       //��������
    case CT_THREE:        //��������
    case CT_SHUNZI:       //˳������
    {
      //��Ŀ�ж�
      if(cbFirstCount!=cbNextCount)
      {
        return false;
      }

      //��������
      BYTE cbConsultNext=GetCardLogicValue(cbNextCard[0]);
      BYTE cbConsultFirst=GetCardLogicValue(cbFirstCard[0]);

      return cbConsultNext>cbConsultFirst;
    }
    case CT_DOUBLE_LINK:    //��������
    case CT_THREE_LINK:     //��������
    {
      //��Ŀ�ж�
      if(cbFirstCount!=cbNextCount)
      {
        return false;
      }

      //��������
      BYTE cbConsultNext=GetCardValue(cbNextCard[0]);
      BYTE cbConsultFirst=GetCardValue(cbFirstCard[0]);

      //��ֵת��
      if(cbConsultNext!=2)
      {
        //���ñ���
        cbConsultNext=0;

        //�˿�����
        for(BYTE i=0; i<cbNextCount; i++)
        {
          BYTE cbLogicValue=GetCardLogicValue(cbNextCard[i]);
          if(cbLogicValue>cbConsultNext)
          {
            cbConsultNext=cbLogicValue;
          }
        }
      }

      //��ֵת��
      if(cbConsultFirst!=2)
      {
        //���ñ���
        cbConsultFirst=0;

        //�˿�����
        for(BYTE i=0; i<cbFirstCount; i++)
        {
          BYTE cbLogicValue=GetCardLogicValue(cbFirstCard[i]);
          if(cbLogicValue>cbConsultFirst)
          {
            cbConsultFirst=cbLogicValue;
          }
        }
      }

      return cbConsultNext>cbConsultFirst;
    }

  }

  //�������
  ASSERT(FALSE);

  return false;
}
//�Ա��˿�
bool CGameLogic::CompareCard(BYTE cbFirstCard, BYTE cbNextCard)
{
  //��ȡ��ֵ
  BYTE cbNextValue=GetCardLogicValue(cbNextCard);
  BYTE cbFirstValue=GetCardLogicValue(cbFirstCard);

  //�Ƚϴ�С
  return (cbNextValue>cbFirstValue);
}
//����˿�
bool CGameLogic::MagicCardData(BYTE cbCardData[], BYTE cbCardCount, BYTE cbResultCard[MAX_COUNT])
{
  //��������
  BYTE cbNormalCard[MAX_COUNT];
  BYTE cbMagicCardCount=0,cbNormalCardCount=0;

  //���׼��
  for(BYTE i=0; i<cbCardCount; i++)
  {
    if(cbCardData[i]==0x4F||cbCardData[i]==0x4E)
    {
      cbMagicCardCount++;
    }
    else
    {
      cbNormalCard[cbNormalCardCount++]=cbCardData[i];
    }
  }

  //ʧ���ж�
  if((cbMagicCardCount==0)||(cbNormalCardCount==0))
  {
    CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));
    return false;
  }

  //��ȡ��ֵ
  BYTE cbLogicValue=GetCardLogicValue(cbNormalCard[0]);

  //ͬ�Ʊ��
  for(BYTE i=1; i<cbNormalCardCount; i++)
  {
    if(GetCardLogicValue(cbNormalCard[i])!=cbLogicValue)
    {
      break;
    }
  }
  //�����ж�
  if(1 == cbNormalCardCount)
  {
    i=cbNormalCardCount;
  }
  if(i==cbNormalCardCount)
  {
    //���ý��
    for(BYTE j=0; j<cbMagicCardCount; j++)
    {
      cbResultCard[j]=cbNormalCard[0];
    }
    for(BYTE j=0; j<cbNormalCardCount; j++)
    {
      cbResultCard[j+cbMagicCardCount]=cbNormalCard[j];
    }
    return true;
  }

  //�˿˷���
  BYTE cbMaxSameCount=1,cbSameCount=1,cbBlock=0;
  for(BYTE i=1; i<cbNormalCardCount; i++)
  {
    //��ȡ�˿�
    BYTE cbCardValueTemp=GetCardLogicValue(cbNormalCard[i]);
    if(cbCardValueTemp==cbLogicValue)
    {
      cbSameCount++;
    }

    //�������
    if((cbCardValueTemp!=cbLogicValue)||(i==(cbNormalCardCount-1)))
    {
      cbMaxSameCount=__max(cbSameCount,cbMaxSameCount);
      cbBlock++;
    }

    //�ָ�����
    if((cbCardValueTemp!=cbLogicValue)&&(i!=(cbNormalCardCount-1)))
    {
      cbSameCount=1;
      cbLogicValue=cbCardValueTemp;
    }
  }

  //��ͬ�任
  if((cbCardCount>=12)&&(cbCardCount%cbBlock==0)&&(cbBlock>=3))
  {
    //�������
    BYTE cbMaxSameCount1=cbCardCount/cbBlock;

    //��ը�ж�
    if(cbMaxSameCount1>=4)
    {
      //�����ֲ�
      tagDistributing Distributing;
      AnalysebDistributing(cbNormalCard,cbNormalCardCount,Distributing);

      //��������
      BYTE cbFillCount=0;
      BYTE cbLeaveCount=cbNormalCardCount;
      BYTE cbUseableCount=cbMagicCardCount;

      //���״̬
      BYTE cbFillStatus[13];
      ZeroMemory(cbFillStatus,sizeof(cbFillStatus));

      //����˿�
      for(BYTE i=0; i<13; i++)
      {
        //��������
        BYTE cbIndex=i;

        //����ж�
        if((cbLeaveCount!=cbNormalCardCount)||(Distributing.cbDistributing[cbIndex][cbIndexCount]!=0))
        {
          //ʧ���ж�
          if((Distributing.cbDistributing[cbIndex][cbIndexCount]+cbUseableCount)<cbMaxSameCount1)
          {
            break;
          }

          //��������
          BYTE cbTurnFillCount=0;

          //�����˿�
          for(BYTE j=0; j<4; j++)
          {
            for(BYTE k=0; k<Distributing.cbDistributing[cbIndex][j]; k++)
            {
              cbLeaveCount--;
              cbTurnFillCount++;
              cbResultCard[cbFillCount++]=MakeCardData(cbIndex,j);
            }
          }

          //�����
          for(BYTE i=cbTurnFillCount; i<cbMaxSameCount1; i++)
          {
            cbUseableCount--;
            cbResultCard[cbFillCount++]=MakeCardData(cbIndex,0);
          }

          //���ñ���
          cbFillStatus[cbIndex]=cbMaxSameCount;
        }
      }
      //�ɹ��ж�
      if((cbUseableCount==0)&&(cbLeaveCount==0))
      {
        SortCardList(cbResultCard,cbCardCount,ST_ORDER);
        return true;
      }
    }
  }
  //��ͬ�任
  if((cbCardCount>=12)&&(cbCardCount%cbBlock==0)&&(cbBlock>=3))
  {
    //�������
    BYTE cbMaxSameCount1=cbCardCount/cbBlock;

    //��ը�ж�
    if(cbMaxSameCount1>=4)
    {
      //�����ֲ�
      tagDistributing Distributing;
      AnalyseRealDistributing(cbNormalCard,cbNormalCardCount,Distributing);

      //��������
      BYTE cbFillCount=0;
      BYTE cbLeaveCount=cbNormalCardCount;
      BYTE cbUseableCount=cbMagicCardCount;

      //���״̬
      BYTE cbFillStatus[13];
      ZeroMemory(cbFillStatus,sizeof(cbFillStatus));

      //����˿�
      for(BYTE i=0; i<13; i++)
      {
        //��������
        BYTE cbIndex=i;

        //����ж�
        if((cbLeaveCount!=cbNormalCardCount)||(Distributing.cbDistributing[cbIndex][cbIndexCount]!=0))
        {
          //ʧ���ж�
          if((Distributing.cbDistributing[cbIndex][cbIndexCount]+cbUseableCount)<cbMaxSameCount)
          {
            break;
          }

          //��������
          BYTE cbTurnFillCount=0;

          //�����˿�
          for(BYTE j=0; j<4; j++)
          {
            for(BYTE k=0; k<Distributing.cbDistributing[cbIndex][j]; k++)
            {
              cbLeaveCount--;
              cbTurnFillCount++;
              cbResultCard[cbFillCount++]=MakeRealCardData(cbIndex,j);
            }
          }

          //�����
          for(BYTE i=cbTurnFillCount; i<cbMaxSameCount1; i++)
          {
            cbUseableCount--;
            cbResultCard[cbFillCount++]=MakeRealCardData(cbIndex,0);
          }

          //���ñ���
          cbFillStatus[cbIndex]=cbMaxSameCount;
        }
      }
      //�ɹ��ж�
      if((cbUseableCount==0)&&(cbLeaveCount==0))
      {
        SortCardList(cbResultCard,cbCardCount,ST_ORDER);
        return true;
      }
    }
  }

  //���ȱ任
  if((cbMaxSameCount>=4)&&(cbBlock>=3)&&(cbCardCount>=12)) //3����ը
  {
    //�����ֲ�
    tagDistributing Distributing;
    AnalysebDistributing(cbNormalCard,cbNormalCardCount,Distributing);

    //������
    for(BYTE cbTimes=0; cbTimes<2; cbTimes++)
    {
      //��������
      BYTE cbFillCount=0;
      BYTE cbLeaveCount=cbNormalCardCount;
      BYTE cbUseableCount=cbMagicCardCount;

      //���״̬
      BYTE cbFillStatus[13];
      ZeroMemory(cbFillStatus,sizeof(cbFillStatus));

      //����˿�
      for(BYTE i=0; i<13; i++)
      {
        //��������
        BYTE cbIndex=(cbTimes==1)?(cbIndex=(13-i)%13):i;

        //����ж�
        if((cbLeaveCount!=cbNormalCardCount)||(Distributing.cbDistributing[cbIndex][cbIndexCount]!=0))
        {
          //����ը��
          if(Distributing.cbDistributing[cbIndex][cbIndexCount]>=4)
          {
            //��¼����
            cbFillStatus[cbIndex]=Distributing.cbDistributing[cbIndex][cbIndexCount];
          }

          //ʧ���ж�
          if((Distributing.cbDistributing[cbIndex][cbIndexCount]+cbUseableCount)<4)
          {
            break;
          }

          //��������
          BYTE cbTurnFillCount=0;

          //�����˿�
          for(BYTE j=0; j<4; j++)
          {
            for(BYTE k=0; k<Distributing.cbDistributing[cbIndex][j]; k++)
            {
              cbLeaveCount--;
              cbTurnFillCount++;
              cbResultCard[cbFillCount++]=MakeCardData(cbIndex,j);
            }
          }

          //�����
          for(BYTE i=cbTurnFillCount; i<4; i++)
          {
            cbUseableCount--;
            cbResultCard[cbFillCount++]=MakeCardData(cbIndex,0);
          }

          //���ñ���
          if(cbTurnFillCount<4)
          {
            cbFillStatus[cbIndex]=4;
          }
          else
          {
            cbFillStatus[cbIndex]=cbTurnFillCount;
          }
        }
      }

      //ʣ�����
      if((cbLeaveCount==0)&&(cbUseableCount>0))
      {
        //�����Ȳ�����С�����
        BYTE cbMinFillCount=0;
        BYTE cbMaxFillCount=0;
        BYTE cbMinIndex=0;//��С����
        for(BYTE i=0; i<13; i++)
        {
          if(cbFillStatus[i]==0)
          {
            continue;  //����û������
          }

          if(cbMinIndex==0 &&cbMinFillCount==0)
          {
            cbMinFillCount=cbFillStatus[i];
            cbMinIndex=i;
          }

          //��ʼѰ�� ȡ��
          if(cbFillStatus[i]<cbMinFillCount)
          {
            cbMinFillCount=cbFillStatus[i];
            cbMinIndex=i;
          }
          cbMaxFillCount=max(cbMaxFillCount,cbFillStatus[i]);
        }

        for(BYTE i=cbMinIndex; i<13; i++)
        {
          //��������
          BYTE cbIndex=(cbTimes==1)?(cbIndex=(13-i)%13):i;
          if(cbFillStatus[cbIndex]==0)
          {
            continue;  //����û������
          }


          //����˿�
          for(BYTE j=cbFillStatus[cbIndex]; j<cbMaxFillCount; j++)
          {
            cbUseableCount--;
            cbResultCard[cbFillCount++]=MakeCardData(cbIndex,0);
            //����ж�
            if(cbUseableCount==0)
            {
              break;
            }

          }

          //����ж�
          if(cbUseableCount==0)
          {
            break;
          }
        }
        //������������  ���ж������ �ǾʹӴ�С�����   �Ͼ�����ֻ��2��  һ�ξ͹��˰�
        if(cbUseableCount>0)
        {
          for(BYTE i=12; i>=0; i--)
          {
            //����˿�
            if(cbFillStatus[i]>0)
            {
              cbUseableCount--;
              cbResultCard[cbFillCount++]=MakeCardData(i,0);
              cbFillStatus[i]++;
            }

            //����ж�
            if(cbUseableCount==0)
            {
              break;
            }
          }
        }
      }



      //�ɹ��ж�
      if((cbUseableCount==0)&&(cbLeaveCount==0))
      {
        SortCardList(cbResultCard,cbCardCount,ST_ORDER);
        return true;
      }
    }
  }

  //���ȱ任
  if((cbMaxSameCount>=4)&&(cbBlock>=3)&&(cbCardCount>=12)) //3����ը
  {
    //�����ֲ�
    tagDistributing Distributing;
    AnalyseRealDistributing(cbNormalCard,cbNormalCardCount,Distributing);

    //������
    for(BYTE cbTimes=0; cbTimes<2; cbTimes++)
    {
      //��������
      BYTE cbFillCount=0;
      BYTE cbLeaveCount=cbNormalCardCount;
      BYTE cbUseableCount=cbMagicCardCount;

      //���״̬
      BYTE cbFillStatus[13];
      ZeroMemory(cbFillStatus,sizeof(cbFillStatus));

      //����˿�
      for(BYTE i=0; i<13; i++)
      {
        //��������
        BYTE cbIndex=(cbTimes==1)?(cbIndex=(13-i)%13):i;

        //����ж�
        if((cbLeaveCount!=cbNormalCardCount)||(Distributing.cbDistributing[cbIndex][cbIndexCount]!=0))
        {
          //����ը��
          if(Distributing.cbDistributing[cbIndex][cbIndexCount]>=4)
          {
            //��¼����
            cbFillStatus[cbIndex]=Distributing.cbDistributing[cbIndex][cbIndexCount];
          }

          //ʧ���ж�
          if((Distributing.cbDistributing[cbIndex][cbIndexCount]+cbUseableCount)<4)
          {
            break;
          }

          //��������
          BYTE cbTurnFillCount=0;

          //�����˿�
          for(BYTE j=0; j<4; j++)
          {
            for(BYTE k=0; k<Distributing.cbDistributing[cbIndex][j]; k++)
            {
              cbLeaveCount--;
              cbTurnFillCount++;
              cbResultCard[cbFillCount++]=MakeRealCardData(cbIndex,j);
            }
          }

          //�����
          for(BYTE i=cbTurnFillCount; i<4; i++)
          {
            cbUseableCount--;
            cbResultCard[cbFillCount++]=MakeRealCardData(cbIndex,0);
          }

          //���ñ���
          if(cbTurnFillCount<4)
          {
            cbFillStatus[cbIndex]=4;
          }
          else
          {
            cbFillStatus[cbIndex]=cbTurnFillCount;
          }
        }
      }

      //ʣ�����
      if((cbLeaveCount==0)&&(cbUseableCount>0))
      {
        //�����Ȳ�����С�����
        BYTE cbMinFillCount=0;
        BYTE cbMaxFillCount=0;
        BYTE cbMinIndex=0;//��С����
        for(BYTE i=0; i<13; i++)
        {
          if(cbFillStatus[i]==0)
          {
            continue;  //����û������
          }

          if(cbMinIndex==0 &&cbMinFillCount==0)
          {
            cbMinFillCount=cbFillStatus[i];
            cbMinIndex=i;
          }

          //��ʼѰ�� ȡ��
          if(cbFillStatus[i]<cbMinFillCount)
          {
            cbMinFillCount=cbFillStatus[i];
            cbMinIndex=i;
          }
          cbMaxFillCount=max(cbMaxFillCount,cbFillStatus[i]);
        }

        for(BYTE i=cbMinIndex; i<13; i++)
        {
          //��������
          BYTE cbIndex=(cbTimes==1)?(cbIndex=(13-i)%13):i;
          if(cbFillStatus[cbIndex]==0)
          {
            continue;  //����û������
          }


          //����˿�
          for(BYTE j=cbFillStatus[cbIndex]; j<cbMaxFillCount; j++)
          {
            cbUseableCount--;
            cbResultCard[cbFillCount++]=MakeRealCardData(cbIndex,0);
            //����ж�
            if(cbUseableCount==0)
            {
              break;
            }

          }

          //����ж�
          if(cbUseableCount==0)
          {
            break;
          }
        }
        //������������  ���ж������ �ǾʹӴ�С�����   �Ͼ�����ֻ��2��  һ�ξ͹��˰�
        if(cbUseableCount>0)
        {
          for(BYTE i=12; i>=0; i--)
          {
            //����˿�
            if(cbFillStatus[i]>0)
            {
              cbUseableCount--;
              cbResultCard[cbFillCount++]=MakeRealCardData(i,0);
              cbFillStatus[i]++;
            }

            //����ж�
            if(cbUseableCount==0)
            {
              break;
            }
          }
        }
      }



      //�ɹ��ж�
      if((cbUseableCount==0)&&(cbLeaveCount==0))
      {
        SortCardList(cbResultCard,cbCardCount,ST_ORDER);
        return true;
      }
    }
  }
  //���Ʊ��
  if((cbMaxSameCount<=3)&&((cbCardCount%cbMaxSameCount)==0)&&(cbCardCount/cbMaxSameCount<=13))
  {
    //�����ֲ�
    tagDistributing Distributing;
    AnalysebDistributing(cbNormalCard,cbNormalCardCount,Distributing);

    //������
    for(BYTE cbTimes=0; cbTimes<2; cbTimes++)
    {
      //��������
      BYTE cbFillCount=0;
      BYTE cbLeaveCount=cbNormalCardCount;
      BYTE cbUseableCount=cbMagicCardCount;

      //���״̬
      BYTE cbFillStatus[13];
      ZeroMemory(cbFillStatus,sizeof(cbFillStatus));

      //����˿�
      for(BYTE i=0; i<12; i++)
      {
        //��������
        BYTE cbIndex=(cbTimes==1)?(cbIndex=(11-i)%12):i;

        //����ж�
        if((cbLeaveCount!=cbNormalCardCount)||(Distributing.cbDistributing[cbIndex][cbIndexCount]!=0))
        {
          //ʧ���ж�
          if((Distributing.cbDistributing[cbIndex][cbIndexCount]+cbUseableCount)<cbMaxSameCount)
          {
            break;
          }

          //��������
          BYTE cbTurnFillCount=0;

          //�����˿�
          for(BYTE j=0; j<4; j++)
          {
            for(BYTE k=0; k<Distributing.cbDistributing[cbIndex][j]; k++)
            {
              cbLeaveCount--;
              cbTurnFillCount++;
              cbResultCard[cbFillCount++]=MakeCardData(cbIndex,j);
            }
          }

          //�����
          for(BYTE i=cbTurnFillCount; i<cbMaxSameCount; i++)
          {
            cbUseableCount--;
            cbResultCard[cbFillCount++]=MakeCardData(cbIndex,0);
          }

          //���ñ���
          cbFillStatus[cbIndex]=cbMaxSameCount;
        }
      }

      //ʣ�����
      if((cbLeaveCount==0)&&(cbUseableCount>0))
      {
        for(BYTE i=0; i<12; i++)
        {
          //��������
          BYTE cbIndex=(cbTimes==0)?(cbIndex=(11-i)%12):i;

          //����˿�
          for(BYTE j=cbFillStatus[cbIndex]; j<cbMaxSameCount; j++)
          {
            cbUseableCount--;
            cbResultCard[cbFillCount++]=MakeCardData(cbIndex,0);
          }

          //����ж�
          if(cbUseableCount==0)
          {
            break;
          }
        }
      }

      //�ɹ��ж�
      if((cbUseableCount==0)&&(cbLeaveCount==0))
      {
        SortCardList(cbResultCard,cbCardCount,ST_ORDER);
        return true;
      }
    }
  }
  //�����˿�
  CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));

  return false;
}

//�����˿�
VOID CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
  //���ý��
  ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

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
    BYTE cbCardColor=GetCardColor(cbCardData[i]);
    BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

    //�ֲ���Ϣ
    Distributing.cbCardCount++;
    Distributing.cbDistributing[cbLogicValue-3][cbIndexCount]++;
    Distributing.cbDistributing[cbLogicValue-3][cbCardColor>>4]++;
  }


  return;
}

//�����ֲ�
VOID CGameLogic::AnalyseRealDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
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
    BYTE cbCardColor=GetCardColor(cbCardData[i]);
    BYTE cbCardValue=GetCardValue(cbCardData[i]);

    //�ֲ���Ϣ
    Distributing.cbCardCount++;
    Distributing.cbDistributing[cbCardValue-1][cbIndexCount]++;
    Distributing.cbDistributing[cbCardValue-1][cbCardColor>>4]++;
  }
}

//��������
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
  //���ý��
  ZeroMemory(&OutCardResult,sizeof(OutCardResult));

  //�����˿�
  BYTE cbCardData[MAX_COUNT];
  BYTE cbCardCount=cbHandCardCount;
  CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);

  //�����˿�
  SortCardList(cbCardData,cbCardCount,ST_ORDER);

  //��ȡ����
  BYTE cbStarLevel;
  BYTE cbTurnOutType=GetCardType(cbTurnCardData,cbTurnCardCount,cbStarLevel);

  //���Ʒ���
  switch(cbTurnOutType)
  {
    case CT_ERROR:          //��������
    {
      //��ȡ��ֵ
      BYTE cbLogicValue=GetCardLogicValue(cbCardData[cbCardCount-1]);

      //�����ж�
      BYTE cbSameCount=1;
      for(BYTE i=1; i<cbCardCount; i++)
      {
        if(GetCardLogicValue(cbCardData[cbCardCount-i-1])==cbLogicValue)
        {
          cbSameCount++;
        }
        else
        {
          break;
        }
      }

      //��ɴ���
      if(cbSameCount>1)
      {
        OutCardResult.cbCardCount=cbSameCount;
        for(BYTE j=0; j<cbSameCount; j++)
        {
          OutCardResult.cbResultCard[j]=cbCardData[cbCardCount-1-j];
        }
        return true;
      }

      //���ƴ���
      OutCardResult.cbCardCount=1;
      OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-1];

      return true;
    }

  }
  return false;
}

//����˳��
bool CGameLogic::SearchLinkCard(const BYTE cbHandCardData[],BYTE cbHandCardCount,const BYTE cbFirstCardData,const BYTE cbLastCardData,tagOutCardResult &OutCardResult)
{
  ASSERT(IsValidCard(cbFirstCardData) && IsValidCard(cbLastCardData));

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

  //���׼��
  BYTE cbMagicCardCount=0;
  for(BYTE i=0; i<byCardCount; i++)
  {
    if(byCardData[i]==0x4F)
    {
      cbMagicCardCount++;
    }
  }

  //��������
  OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = cbFirstCardData;
  OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = cbLastCardData;
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
      //�����滻
      if(cbMagicCardCount>0)
      {
        OutCardResult.cbResultCard[OutCardResult.cbCardCount++] = 0x4F;

        //���Ƶݼ�
        cbMagicCardCount--;
      }
      else
      {
        break;
      }
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
      return (4<<4)|0x0E;
    }
    case 14:  //����˿�
    {
      return (4<<4)|0x0F;
    }
    default:  //�����˿�
    {
      return (cbColorIndex<<4)|(cbValueIndex+3);
    }
  }

  return 0x00;
}

//�����˿�
BYTE CGameLogic::MakeRealCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
  //�����˿�
  switch(cbValueIndex)
  {

    case 13:  //С���˿�
    {
      return (4<<4)|0x0E;
    }
    case 14:  //����˿�
    {
      return (4<<4)|0x0F;
    }

    default:  //�����˿�
    {
      return (cbColorIndex<<4)|(cbValueIndex+1);
    }
  }

  return 0x00;
}


//�Ƿ�����
bool CGameLogic::IsStructureLink(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCellCount,bool bSpecial)
{
  //��Ŀ�ж�
  ASSERT((cbCardCount%cbCellCount)==0);
  if((cbCardCount%cbCellCount)!=0)
  {
    return false;
  }

  //��������
  BYTE cbBlockCount=cbCardCount/cbCellCount;
  BYTE cbFirstValue=GetCardLogicValue(cbCardData[0]);

  if(bSpecial)
  {
    //���⴦��  2����ը��
    if(cbFirstValue==15)
    {
      for(BYTE i=1; i<cbBlockCount; i++)
      {
        //��ȡ��ֵ
        BYTE cbLogicValue=GetCardLogicValue(cbCardData[i*cbCellCount]);

        //�����ж�
        if((cbFirstValue!=(cbLogicValue+i))&&(cbFirstValue!=(cbLogicValue+i+(15-cbBlockCount-2))))
        {
          return false;
        }
      }

      return true;
    }
  }

  //���ƹ���
  if(cbFirstValue>14)
  {
    return false;
  }

  //��������
  for(BYTE i=1; i<cbBlockCount; i++)
  {
    if(cbFirstValue!=(GetCardLogicValue(cbCardData[i*cbCellCount])+i))
    {
      return false;
    }
  }

  return true;
}

