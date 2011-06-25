/*
 *  Copyright 2009 Utkin Dmitry
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
 
/* 
 *  This file is part of the WSF Staff project.
 *  Please, visit http://code.google.com/p/staff for more information.
 */

#include <stdio.h>
#include <iomanip>
#include <iostream>
#include "console.h"

#ifdef __MINGW32__
#ifndef COMMON_LVB_REVERSE_VIDEO
#define COMMON_LVB_REVERSE_VIDEO   0x4000
#endif
#ifndef COMMON_LVB_UNDERSCORE
#define COMMON_LVB_UNDERSCORE      0x8000
#endif
#endif

namespace rise
{
#ifdef WIN32
  class CConsAttr
  {
  private:
    HANDLE m_hStd;
  
  public:
    CConsAttr()
    {
      m_hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    ~CConsAttr()
    {
      CloseHandle(m_hStd);
    }

    void SetTitle(const CString& sTitle)
    {
      SetConsoleTitle(sTitle.c_str());
    }

    COStream& SetAttribute(COStream& rStream, WORD nAttr)
    {
      if (rStream != tStdOut) 
        return rStream;
      rStream << std::flush;
      SetConsoleTextAttribute(m_hStd, nAttr);
      return rStream;
    }

    WORD GetAttribute()
    {
      CONSOLE_SCREEN_BUFFER_INFO tConsoleScreenBufferInfo;
      if(GetConsoleScreenBufferInfo(m_hStd, &tConsoleScreenBufferInfo))
        return tConsoleScreenBufferInfo.wAttributes;
      else
        return 0xffffu;
    }

    void SetCursorPos(short shX, short shY)
    {
      COORD stCoord = {shX, shY};
      SetConsoleCursorPosition(m_hStd, stCoord);
    }

    bool GetCursorPos(short& shX, short& shY)
    {
      CONSOLE_SCREEN_BUFFER_INFO tConsoleScreenBufferInfo;
      if(!GetConsoleScreenBufferInfo(m_hStd, &tConsoleScreenBufferInfo))
        return false;

      shX = tConsoleScreenBufferInfo.dwCursorPosition.X;
      shY = tConsoleScreenBufferInfo.dwCursorPosition.X;
      return true;
    }

    bool MoveCursor(short shDeltaX, short shDeltaY)
    {
      short shCurrX = 0;
      short shCurrY = 0;
      if(!GetCursorPos(shCurrX, shCurrY))
        return false;

      SetCursorPos(shCurrX + shDeltaX, shCurrY + shDeltaY);
      return true;
    }

  } tConsAttr;
#endif

  COStream& ColorDefault(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    return (rStream << "\033[0m");
#endif
  }

  COStream& ColorBright(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, FOREGROUND_INTENSITY | tConsAttr.GetAttribute());
#else
    return (rStream << "\033[1m");
#endif
  }

  COStream& ColorDim(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() & ~ (FOREGROUND_INTENSITY & BACKGROUND_INTENSITY) );
#else
    return (rStream << "\033[2m");
#endif
  }

  COStream& ColorBrightDefault(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() & ~ (FOREGROUND_INTENSITY & BACKGROUND_INTENSITY) );
#else
    return (rStream << "\033[22m");
#endif
  }

  COStream& ColorFlashOn(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, BACKGROUND_INTENSITY | tConsAttr.GetAttribute());
#else
    return (rStream << "\033[5m");
#endif
  }

  COStream& ColorFlashOff(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() & ~ BACKGROUND_INTENSITY );
#else
    return (rStream << "\033[25m");
#endif
  }

  COStream& ColorInverseOn(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() | COMMON_LVB_REVERSE_VIDEO);
#else
    return (rStream << "\033[7m");
#endif
  }

  COStream& ColorInverseOff(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() & ~ COMMON_LVB_REVERSE_VIDEO);
#else
    return (rStream << "\033[27m");
#endif
  }

  COStream& ColorUnderlineOn(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() | COMMON_LVB_UNDERSCORE);
#else
    return (rStream << "\033[4m");
#endif
  }

  COStream& ColorUnderlineOff(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() & ~ COMMON_LVB_UNDERSCORE);
#else
    return (rStream << "\033[24m");
#endif
  }

  COStream& ColorInkBlack(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() & ~ (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE));
#else
    return (rStream << "\033[30m");
#endif
  }

  COStream& ColorInkRed(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ (FOREGROUND_GREEN | FOREGROUND_BLUE)) | FOREGROUND_RED);
#else
    return (rStream << "\033[31m");
#endif
  }

  COStream& ColorInkGreen(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ (FOREGROUND_RED | FOREGROUND_BLUE)) | FOREGROUND_GREEN);
#else
    return (rStream << "\033[32m");
#endif
  }

  COStream& ColorInkBrown(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ FOREGROUND_BLUE) | FOREGROUND_RED | FOREGROUND_GREEN);
#else
    return (rStream << "\033[33m");
#endif
  }

  COStream& ColorInkBlue(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ (FOREGROUND_RED | FOREGROUND_GREEN)) | FOREGROUND_BLUE);
#else
    return (rStream << "\033[34m");
#endif
  }

  COStream& ColorInkMagenta(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ FOREGROUND_GREEN) | FOREGROUND_RED | FOREGROUND_BLUE);
#else
    return (rStream << "\033[35m");
#endif
  }

  COStream& ColorInkCyan(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ FOREGROUND_RED) | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    return (rStream << "\033[36m");
#endif
  }

  COStream& ColorInkGrey(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    return (rStream << "\033[37m");
#endif
  }


  COStream& ColorPaperBlack(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() & ~ (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE));
#else
    return (rStream << "\033[40m");
#endif
  }

  COStream& ColorPaperRed(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ (BACKGROUND_GREEN | BACKGROUND_BLUE)) | BACKGROUND_RED);
#else
    return (rStream << "\033[41m");
#endif
  }

  COStream& ColorPaperGreen(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ (BACKGROUND_RED | BACKGROUND_BLUE)) | BACKGROUND_GREEN);
#else
    return (rStream << "\033[42m");
#endif
  }

  COStream& ColorPaperBrown(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ BACKGROUND_BLUE) | BACKGROUND_RED | BACKGROUND_GREEN);
#else
    return (rStream << "\033[43m");
#endif
  }

  COStream& ColorPaperBlue(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ (BACKGROUND_RED | BACKGROUND_GREEN)) | BACKGROUND_BLUE);
#else
    return (rStream << "\033[44m");
#endif
  }

  COStream& ColorPaperMagenta(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ BACKGROUND_GREEN) | BACKGROUND_RED | BACKGROUND_BLUE);
#else
    return (rStream << "\033[45m");
#endif
  }

  COStream& ColorPaperCyan(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, (tConsAttr.GetAttribute() & ~ BACKGROUND_RED) | BACKGROUND_GREEN | BACKGROUND_BLUE);
#else
    return (rStream << "\033[46m");
#endif
  }

  COStream& ColorPaperGrey(COStream& rStream)
  {
#ifdef WIN32
    return tConsAttr.SetAttribute(rStream, tConsAttr.GetAttribute() | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
#else
    return (rStream << "\033[47m");
#endif
  }

  COStream& _CursorUp( COStream& rStream, short nValue )
  {
#ifdef WIN32
    tConsAttr.MoveCursor(0, -nValue);
    return rStream;
#else
    return (rStream << "\033[" << nValue << "A");
#endif
  }

  CManip1<short> CursorUp( short nValue )
  {
    return CManip1<short>(_CursorUp, nValue);
  }

  COStream& _CursorDown( COStream& rStream, short nValue )
  {
#ifdef WIN32
    tConsAttr.MoveCursor(0, nValue);
    return rStream;
#else
    return (rStream << "\033[" << nValue << "B");
#endif
  }

  CManip1<short> CursorDown(short nValue)
  {
    return CManip1<short>(_CursorDown, nValue);
  }

  COStream& _CursorRight( COStream& rStream, short nValue )
  {
#ifdef WIN32
    tConsAttr.MoveCursor(0, nValue);
    return rStream;
#else
    return (rStream << "\033[" << nValue << "C");
#endif
  }

  CManip1<short> CursorRight( short nValue )
{
    return CManip1<short>(_CursorRight, nValue);
  }

  COStream& _CursorLeft( COStream& rStream, short nValue )
  {
#ifdef WIN32
    tConsAttr.MoveCursor(0, -nValue);
    return rStream;
#else
    return (rStream << "\033[" << nValue << "D");
#endif
  }

  CManip1<short> CursorLeft( short nValue )
{
    return CManip1<short>(_CursorLeft, nValue);
  }

  COStream& _CursorDownNl( COStream& rStream, short nValue )
  {
#ifdef WIN32
    short shCurrX = 0;
    short shCurrY = 0;
    if(tConsAttr.GetCursorPos(shCurrX, shCurrY))
      tConsAttr.SetCursorPos(0, shCurrY + static_cast<short>(nValue));

    return rStream;
#else
    return (rStream << "\033[" << nValue << "E");
#endif
  }

  CManip1<short> CursorDownNl( short nValue )
{
    return CManip1<short>(_CursorDownNl, nValue);
  }

  COStream& _CursorUpNl( COStream& rStream, short nValue )
  {
#ifdef WIN32
    short shCurrX = 0;
    short shCurrY = 0;
    if(tConsAttr.GetCursorPos(shCurrX, shCurrY))
      tConsAttr.SetCursorPos(0, shCurrY - nValue);

    return rStream;
#else
    return (rStream << "\033[" << nValue << "F");
#endif
  }

  CManip1<short> CursorUpNl( short nValue )
{
    return CManip1<short>(_CursorUpNl, nValue);
  }

  COStream& _CursorTab( COStream& rStream, short nValue )
  {
#ifdef WIN32
    short shCurrX = 0;
    short shCurrY = 0;
    if(tConsAttr.GetCursorPos(shCurrX, shCurrY))
      tConsAttr.SetCursorPos(nValue, shCurrY);

    return rStream;
#else
    return (rStream << "\033[" << nValue << "G");
#endif
  }

  CManip1<short> CursorTab( short nValue )
{
    return CManip1<short>(_CursorTab, nValue);
  }

  COStream& _CursorGoto( COStream& rStream, short shX, short shY )
  {
#ifdef WIN32
    tConsAttr.SetCursorPos(shX, shY);
    return rStream;
#else
    return (rStream << "\033[" << shX << "," << shY << "H");
#endif
  }

  CManip2<short> CursorGoto(short shX, short shY)
  {
    return CManip2<short>(_CursorGoto, shX, shY);
  }

  COStream& _PaletteSet(COStream& rStream, short nPaletteIndex, short nR, short nG, short nB)
  {
#ifdef WIN32
#ifdef _MSC_VER // warning: statement has no effect
    nPaletteIndex; nR; nG; nB;
#endif
    return rStream;
#else
    return (rStream << "\033]P" << std::setw(2) << std::setfill('0') 
        << std::hex << nPaletteIndex << nR << nG << nB);
#endif
  }

  CManip4<short> PaletteSet(short nPaletteIndex, short nR, short nG, short nB)
  {
    return CManip4<short>(_PaletteSet, nPaletteIndex, nR, nG, nB);
  }

  COStream& PaletteReset(COStream& rStream)
  {
#ifdef WIN32
    return rStream;
#else
    return (rStream << "\033]R");
#endif
  }

  COStream& _ConsoleSwitchTo(COStream& rStream, short nValue)
  {
#ifdef WIN32
#ifdef _MSC_VER // warning: statement has no effect
    nValue;
#endif
    return rStream;
#else
    return (rStream << "\033[12;" << nValue << "]");
#endif
  }

  CManip1<short> ConsoleSwitchTo(short nValue)
  {
    return CManip1<short>(_ConsoleSwitchTo, nValue);
  }

  COStream& _Console(COStream& rStream, CString nValue)
  {
#ifdef WIN32
    return rStream;
#else
    return (rStream << "\033[12;" << nValue << "]");
#endif
  }

  CManip1<CString> Console(const CString& nValue)
  {
    return CManip1<CString>(_Console, nValue);
  }


  COStream& _ConsoleSetWindowTextAndIcon(COStream& rStream, const CString& sText)
  {
#ifdef WIN32
    tConsAttr.SetTitle(sText);
    return rStream;
#else
    return (rStream << "\033]0;" + sText + "\007");
#endif
  }

  RISE_EXPORT CManip1<const CString&> ConsoleSetWindowTextAndIcon( const CString& sText )
  {
    return CManip1<const CString&>(_ConsoleSetWindowTextAndIcon, sText);
  }

  COStream& _ConsoleSetIcon(COStream& rStream, const CString& sText)
  {
#ifdef WIN32
#ifdef _MSC_VER // warning: statement has no effect
    sText;
#endif
    return rStream;
#else
    return (rStream << "\033]1;" + sText + "\007");
#endif
  }

  CManip1<const CString&> ConsoleSetIcon(const CString& sText)
  {
    return CManip1<const CString&>(_ConsoleSetIcon, sText);
  }

  COStream& _ConsoleSetWindowText(COStream& rStream, const CString& sText)
  {
#ifdef WIN32
    tConsAttr.SetTitle(sText);
    return rStream;
#else
    return (rStream << "\033]2;" + sText + "\007");
#endif
  }

  CManip1<const CString&> ConsoleSetWindowText(const CString& sText)
  {
    return CManip1<const CString&>(_ConsoleSetWindowText, sText);
  }

} // namespace rise
