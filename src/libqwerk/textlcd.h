#ifndef _TEXTLCD_H
#define _TEXTLCD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <unistd.h>
#include "singleton.h"
#include "property.h"
#include "9302hw.h"

using namespace std;

// Properties
#define TL_OBJECT           1
#define TL_PROP_BACKLIGHT   PROP_ID(TL_OBJECT, 0)
#define TL_PROP_HEIGHT      PROP_ID(TL_OBJECT, 1)
#define TL_PROP_WIDTH       PROP_ID(TL_OBJECT, 2)

#define TL_CHAR_UP          0x00
#define TL_CHAR_DOWN        0xf5
#define TL_CHAR_LEFT        0xf7
#define TL_CHAR_RIGHT       0xf6
#define TL_CHAR_LOWBATT     0x01
#define TL_CHAR_COPYRIGHT   0xcf 
#define TL_CHAR_WSA0        0x02 // wireless strength (just antenna)
#define TL_CHAR_WSA1        0x03 // wireless strength (antenna with 1 bar)
#define TL_CHAR_WSB0        0x20 // wireless strength (2nd char, 0 bars)
#define TL_CHAR_WSB1        0x04 // wireless strength (2nd char, 1 bar)
#define TL_CHAR_WSB2        0x05 // wireless strength (2nd char, 2 bars)
#define TL_CHAR_WSB3        0x06 // wireless strength (2nd char, 2 bars)

#define TL_DELAY()          Delay(20)
#define TL_DELAYL()         Delay(2000)

class CTextLcd : public IProperty
{
public:
  SINGLETON(CTextLcd);

  static const unsigned int NUM_ROWS;
  static const unsigned int NUM_COLUMNS;
          
  void Clear();
  void ClearLine(unsigned int lineNumber);
  void MoveCursor(const unsigned int row, const unsigned int col);
  int printf(const char *format, ...);
  void SetCharacter(const char character);
  void SetCharacter(const unsigned int row, const unsigned int col, const char character);
  void SetLine(const unsigned int lineNumber, const string& text, const bool willClearLineFirst = true);
  void SetText(const string& text, const bool willClearFirst = true);
  void SetBacklight(const bool isOn);

  // Save(CContext *context);
  // Restore(CContext context);

  // IProperty
  virtual int GetProperty(int property, long *value);
  virtual int SetProperty(int property, long value);

private:
  CTextLcd();
  virtual ~CTextLcd();

  static const string BLANK_LINE;

  void Init();
  void DefineChars();
  void PutNibble(unsigned char c);
  void PutByte(unsigned char c);
  void Delay(unsigned int us);

  const bool IsValidRow(const unsigned int row) const
  {
    return (row < CTextLcd::NUM_ROWS);
  }

  const bool IsValidColumn(const unsigned int col) const
  {
    return (col < CTextLcd::NUM_COLUMNS);
  }

  const bool IsValidPosition(const unsigned int row, const unsigned int col) const
  {
    return IsValidRow(row) && IsValidColumn(col);
  }

  C9302Hardware *m_p9302hw;
};

#endif
