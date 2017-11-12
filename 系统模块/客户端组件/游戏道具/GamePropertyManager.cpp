#include "StdAfx.h"
#include "GamePropertyManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CGamePropertyManager * CGamePropertyManager::m_pGamePropertyManager=NULL;//����ָ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGamePropertyManager::CGamePropertyManager()
{
  //���ñ���
  ASSERT(m_pGamePropertyManager==NULL);
  if(m_pGamePropertyManager==NULL)
  {
    m_pGamePropertyManager=this;
  }

  return;
}

//��������
CGamePropertyManager::~CGamePropertyManager()
{
  //���ñ���
  ASSERT(m_pGamePropertyManager==this);
  if(m_pGamePropertyManager==this)
  {
    m_pGamePropertyManager=NULL;
  }

  //�ͷŶ���
  if(m_MapPropertyItem.GetCount()>0)
  {
    //��������
    POSITION Position=m_MapPropertyItem.GetStartPosition();

    //�ͷŶ���
    WORD wKey=0;
    CGamePropertyItem * pGamePropertyItem=NULL;
    do
    {
      m_MapPropertyItem.GetNextAssoc(Position,wKey,pGamePropertyItem);
      if(pGamePropertyItem!=NULL)
      {
        SafeDelete(pGamePropertyItem);
      }

    }
    while(Position!=NULL);
  }

  //�Ƴ�Ԫ��
  m_MapPropertyItem.RemoveAll();

  return;
}

//��ȡ����
CGamePropertyItem * CGamePropertyManager::GetPropertyItem(WORD wIndex)
{
  //���ҵ���
  CGamePropertyItem * pGamePropertyItem=NULL;
  m_MapPropertyItem.Lookup(wIndex,pGamePropertyItem);

  return pGamePropertyItem;
}

//��������
CGamePropertyItem * CGamePropertyManager::CreatePropertyItem(tagPropertyInfo & PropertyInfo)
{
  //�����ִ�
  CGamePropertyItem * pGamePropertyItem=NULL;
  m_MapPropertyItem.Lookup(PropertyInfo.wIndex,pGamePropertyItem);

  //�ִ��ж�
  if(pGamePropertyItem!=NULL)
  {
    //���õ���
    pGamePropertyItem->m_PropertyInfo=PropertyInfo;

    //���ȹ���
    if(PropertyInfo.wIndex==PROPERTY_ID_TRUMPET || PropertyInfo.wIndex==PROPERTY_ID_TYPHON)
    {
      return NULL;
    }
    else
    {
      return pGamePropertyItem;
    }
  }

  //��������
  switch(PropertyInfo.wIndex)
  {
    case PROPERTY_ID_CAR:     //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyCar;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_EGG:     //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyEgg;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_CLAP:      //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyClap;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_KISS:      //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyKiss;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_BEER:      //ơ������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyBeer;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_CAKE:      //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyCake;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_RING:      //�������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyRing;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_BEAT:      //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyBeat;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_BOMB:      //ը������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyBomb;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_SMOKE:     //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertySmoke;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_VILLA:     //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyVilla;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_BRICK:     //שͷ����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyBrick;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_FLOWER:    //�ʻ�����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyFlower;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_TWO_CARD:    //˫������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyTwoScoreCard;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_FOUR_CARD:    //�ı�����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyFourScoreCard;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_SCORE_CLEAR:    //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyScoreClear;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_ESCAPE_CLEAR:    //��������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyEscapeClear;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_TRUMPET:     //С����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyTrumpet;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_TYPHON:      //������
    {
      try
      {
        pGamePropertyItem=new CGamePropertyTyphon;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_GUARDKICK_CARD:   //���߿�
    {
      try
      {
        pGamePropertyItem=new CGamePropertyGuardKickCard;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_POSSESS:     //�����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyPossess;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }

      break;
    }
    case PROPERTY_ID_DOUBLE_1HOUR:  //1Сʱ˫����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyDouble1Hour;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_DOUBLE_3HOUR:   //3Сʱ˫����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyDouble3Hour;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_DOUBLE_1WEEK:   //1��˫����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyDouble1Week;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_DOUBLE_1DAY:  //1��˫����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyDouble1Day;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_AMULET_1HOUR:  //1Сʱ����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyAmulet1Hour;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_AMULET_3HOUR:   //3Сʱ����
    {
      try
      {
        pGamePropertyItem=new CGamePropertyAmulet3Hour;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_AMULET_1WEEK:   //1�ܻ���
    {
      try
      {
        pGamePropertyItem=new CGamePropertyAmulet1Week;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_AMULET_1DAY:  //1�커��
    {
      try
      {
        pGamePropertyItem=new CGamePropertyAmulet1Day;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_GUARDKICK_1HOUR: //1Сʱ���߿�
    {
      try
      {
        pGamePropertyItem=new CGamePropertyAmulet1Hour;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_GUARDKICK_3HOUR:  //3Сʱ���߿�
    {
      try
      {
        pGamePropertyItem=new CGamePropertyAmulet3Hour;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_GUARDKICK_1WEEK:  //1�ܷ��߿�
    {
      try
      {
        pGamePropertyItem=new CGamePropertyAmulet1Week;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_GUARDKICK_1DAY:   //1����߿�
    {
      try
      {
        pGamePropertyItem=new CGamePropertyAmulet1Day;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_CRYSTAL:     //ˮ��
    {
      try
      {
        pGamePropertyItem=new CGamePropertyCrystal;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_AGATE:     //���
    {
      try
      {
        pGamePropertyItem=new CGamePropertyAgate;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_BOULDER:     //��ʯ
    {
      try
      {
        pGamePropertyItem=new CGamePropertyBoulder;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_GEM:     //��ʯ
    {
      try
      {
        pGamePropertyItem=new CGamePropertyGem;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    case PROPERTY_ID_DIAMOND:     //��ʯ
    {
      try
      {
        pGamePropertyItem=new CGamePropertyDiamond;
      }
      catch(...)
      {
        ASSERT(FALSE);
      }
      break;
    }
    //case PROPERTY_ID_BLUERING_CARD:   //�����Ա��
    //  {
    //    try
    //    {
    //      pGamePropertyItem=new CGamePropertyBlueRingCard;
    //    }
    //    catch (...)
    //    {
    //      ASSERT(FALSE);
    //    }

    //    break;
    //  }
    //case PROPERTY_ID_YELLOWRING_CARD: //�����Ա��
    //  {
    //    try
    //    {
    //      pGamePropertyItem=new CGamePropertyYellowRingCard;
    //    }
    //    catch (...)
    //    {
    //      ASSERT(FALSE);
    //    }

    //    break;
    //  }
    //case PROPERTY_ID_WHITERING_CARD:  //�����Ա��
    //  {
    //    try
    //    {
    //      pGamePropertyItem=new CGamePropertyWhiteRingCard;
    //    }
    //    catch (...)
    //    {
    //      ASSERT(FALSE);
    //    }

    //    break;
    //  }
    //case PROPERTY_ID_REDRING_CARD:    //�����Ա��
    //  {
    //    try
    //    {
    //      pGamePropertyItem=new CGamePropertyRedRingCard;
    //    }
    //    catch (...)
    //    {
    //      ASSERT(FALSE);
    //    }

    //    break;
    //  }
    //case PROPERTY_ID_VIPROOM_CARD:    //VIP����
    //  {
    //    try
    //    {
    //      pGamePropertyItem=new CGamePropertyVipRoomCard;
    //    }
    //    catch (...)
    //    {
    //      ASSERT(FALSE);
    //    }

    //    break;
    //  }
  }

  //�����ж�
  if(pGamePropertyItem!=NULL)
  {
    //���õ���
    pGamePropertyItem->m_PropertyInfo=PropertyInfo;
    m_MapPropertyItem[PropertyInfo.wIndex]=pGamePropertyItem;

    //���ȹ���
    if(PropertyInfo.wIndex==PROPERTY_ID_TRUMPET || PropertyInfo.wIndex==PROPERTY_ID_TYPHON)
    {
      return NULL;
    }
    else
    {
      return pGamePropertyItem;
    }
  }

  //�������
  //ASSERT(FALSE);

  return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
