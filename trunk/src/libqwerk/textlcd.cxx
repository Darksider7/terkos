#include <unistd.h>
#include <stdio.h>
#include "textlcd.h"

CTextLcd::CTextLcd()
{
  m_p9302hw = C9302Hardware::GetObject();
  Init();
}

CTextLcd::~CTextLcd()
{
  C9302Hardware::ReleaseObject();
}
 
void CTextLcd::Clear()
{
  // RS low
  *m_p9302hw->PortGDataDR() |= 0x02;
  TL_DELAY();
  
  PutByte(1);
}

void CTextLcd::MoveCursor(int x, int y)
{
  // RS low
  *m_p9302hw->PortGDataDR() |= 0x02;
  TL_DELAY();

  PutByte(0x80 | (x&0x0f) + (y ? 0x40 : 0)); 
}

int CTextLcd::printf(char *format, ...)
{
  int i;
  char buf[256];
  va_list args;

  va_start (args, format);
  vsprintf (buf, format, args);
  va_end (args);

  // RS High
  *m_p9302hw->PortGDataDR() &= ~0x02;
  TL_DELAY();

  for (i=0; buf[i]; i++)
    PutByte(buf[i]);
}

// RS eedat portg1
// R/W egpio10 portb2
// E egpio11 portb3
// D4 egpio12 portb4
// D5 egpio13 portb5
// D6 egpio14 portb6
// D7 egpio15 portb7


void CTextLcd::Init()
{
  // R/W and E low
  *m_p9302hw->PortBData() &= ~0xfc;
  *m_p9302hw->PortBDataDR() |= 0x0c;

  // RS low
  *m_p9302hw->PortGData() &= ~0x02;
  *m_p9302hw->PortGDataDR() |= 0x02;

#if 0
  PutNibble(0x02);
  PutNibble(0x02);
  PutNibble(0x08);
  PutByte(0x0c);
  PutByte(0x01);
  PutByte(0x03);
#endif

  Clear();
}

void CTextLcd::DefineChars()
{
}

  void CTextLcd::PutNibble(unsigned char c)
{
  *m_p9302hw->PortBDataDR() = (~(c << 4)&0xf0) | (*m_p9302hw->PortBDataDR() & 0x0f);
  TL_DELAY();
  *m_p9302hw->PortBDataDR() &= ~0x08;
  TL_DELAY();
  *m_p9302hw->PortBDataDR() |= 0x08;
  TL_DELAY();
  
  *m_p9302hw->PortBDataDR() &= ~0xf0;
}

void CTextLcd::PutByte(unsigned char c)
{
  PutNibble(c>>4);
  PutNibble(c);
  TL_DELAYL();
}

void CTextLcd::Delay(unsigned int us)
{
  volatile unsigned int i;
  
  us *= 10;
  for (i=0; i<us; i++);
}

int CTextLcd::GetProperty(int property, long *value)
{
  if (value==NULL)
    return PROP_ERROR;

  switch(property)
    {
    case TL_PROP_BACKLIGHT:
      *value = *m_p9302hw->m_uart1.Uint(0x100) & 0x0001 ? 0 : 1;
      break;

    case TL_PROP_HEIGHT:
      *value = TL_HEIGHT;
      break;

    case TL_PROP_WIDTH:
      *value = TL_WIDTH;
      break;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;    
}

int CTextLcd::SetProperty(int property, long value)
{
  switch (property)
    {
    case TL_PROP_BACKLIGHT:
      if (value)
	*m_p9302hw->m_uart1.Uint(0x100) &= ~0x0001;
      else
	*m_p9302hw->m_uart1.Uint(0x100) |= 0x0001;
      break;

    case TL_PROP_HEIGHT:
      return PROP_ERROR_READ_ONLY;

    case TL_PROP_WIDTH:
      return PROP_ERROR_READ_ONLY;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}