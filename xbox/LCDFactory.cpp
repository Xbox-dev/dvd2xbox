
//#include "stdafx.h"
#include "lcdfactory.h"
#include "../dvd2xbox/d2xsettings.h"
#include "../lib/smartxx/smartxxLCD.h"
#include "../lib/libXenium/XeniumLCD.h"


ILCD* g_lcd=NULL;
CLCDFactory::CLCDFactory(void)
{
}

CLCDFactory::~CLCDFactory(void)
{
}

ILCD* CLCDFactory::Create()
{
  switch (g_d2xSettings.m_iLCDModChip)
  {
    case MODCHIP_XENIUM:
      return new CXeniumLCD();
    break;
    
    case MODCHIP_SMARTXX:
      return new CSmartXXLCD();
    break;

  }
  return new CSmartXXLCD();
}
