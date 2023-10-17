//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <SFML\Graphics\Text.hpp>
#include <cstring>

enum { CODE_ENG, CODE_HAN1, CODE_HAN2 };

class CMisc
{
public:

    int iGetTextLengthLoc(sf::Text & text, char * pStr, int iLength);
    int  _iGetFileCheckSum(char * pFn);
    BOOL _iConvertFileXor(char * pFn, char * pDestFn, char cKey);
    BOOL bCheckValidName(const char * pStr);
    BOOL bCheckIMEString(const char * str);
    BOOL bDecode(char cKey, char * pStr);
    BOOL bEncode(char cKey, char * pStr);
    void ColorTransfer(char cPixelFormat, COLORREF fcolor, WORD * wR, WORD * wG, WORD * wB);
    char cCalcDirection(short sX, short sY, short dX, short dY);
    void ReplaceString(char * pStr, char cFrom, char cTo);
    BOOL bCheckValidString(const char * str);
    void GetDirPoint(char cDir, int * pX, int * pY);
    void GetPoint(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError, int iCount);
    char cGetNextMoveDir(short sX, short sY, short dX, short dY);
    BOOL bIsValidEmail(char * pStr);
    CMisc();
    ~CMisc();
};
