#ifndef _TEXTLCD_H
#define _TEXTLCD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <unistd.h>
#include "property.h"
#include "9302hw.h"

using namespace std;

// Properties
#define TL_OBJECT           1
#define TL_PROP_BACKLIGHT   PROP_ID(TL_OBJECT, 0)
#define TL_PROP_HEIGHT      PROP_ID(TL_OBJECT, 1)
#define TL_PROP_WIDTH       PROP_ID(TL_OBJECT, 2)

#define TL_DELAY()          Delay(20)
#define TL_DELAYL()         Delay(2000)

class CTextLcd : public IProperty
   {
   public:

      static const unsigned int NUM_ROWS;
      static const unsigned int NUM_COLUMNS;

      CTextLcd();
      ~CTextLcd();

      void Clear();
      void ClearLine(unsigned int lineNumber);
      void MoveCursor(const unsigned int row, const unsigned int col);
      int printf(const char *format, ...);
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
