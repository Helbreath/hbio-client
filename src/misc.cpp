//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "misc.h"
#include "global_def.h"

char CMisc::cGetNextMoveDir(short sX, short sY, short dX, short dY, bool bMIM)
{
    short absX, absY;
    char  cRet = 0;

    if (bMIM)
    {
        absX = dX - sX;
        absY = dY - sY;
    }
    else
    {
        absX = sX - dX;
        absY = sY - dY;
    }

    if ((absX == 0) && (absY == 0)) cRet = 0;

    if (absX == 0)
    {
        if (absY > 0) cRet = 1;
        if (absY < 0) cRet = 5;
    }
    if (absY == 0)
    {
        if (absX > 0) cRet = 7;
        if (absX < 0) cRet = 3;
    }
    if ((absX > 0) && (absY > 0)) cRet = 8;
    if ((absX < 0) && (absY > 0)) cRet = 2;
    if ((absX > 0) && (absY < 0)) cRet = 6;
    if ((absX < 0) && (absY < 0)) cRet = 4;

    return cRet;
}

void CMisc::GetPoint(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError, int iCount)
{
    int dx, dy, x_inc, y_inc, error, index;
    int iResultX, iResultY, iCnt = 0;


    if ((x0 == x1) && (y0 == y1))
    {
        *pX = x0;
        *pY = y0;
        return;
    }

    error = *pError;

    iResultX = x0;
    iResultY = y0;

    dx = x1 - x0;
    dy = y1 - y0;

    if (dx >= 0) x_inc = 1;
    else
    {
        x_inc = -1;
        dx = -dx;
    }
    if (dy >= 0) y_inc = 1;
    else
    {
        y_inc = -1;
        dy = -dy;
    }

    if (dx > dy)
    {
        for (index = 0; index <= dx; index++)
        {
            error += dy;
            if (error > dx)
            {
                error -= dx;
                iResultY += y_inc;
            }
            iResultX += x_inc;
            iCnt++;
            if (iCnt >= iCount) break;
        }
    }
    else
    {
        for (index = 0; index <= dy; index++)
        {
            error += dx;
            if (error > dy)
            {
                error -= dy;
                iResultX += x_inc;
            }
            iResultY += y_inc;
            iCnt++;
            if (iCnt >= iCount) break;
        }
    }

    *pX = iResultX;
    *pY = iResultY;
    *pError = error;
}


void CMisc::GetDirPoint(char cDir, int * pX, int * pY)
{
    switch (cDir)
    {
        case 1:	*pY--; break;
        case 2:	*pY--; *pX++;	break;
        case 3:	*pX++; break;
        case 4:	*pX++; *pY++;	break;
        case 5:	*pY++; break;
        case 6:	*pX--; *pY++;	break;
        case 7:	*pX--; break;
        case 8:	*pX--; *pY--;	break;
    }
}

bool CMisc::bCheckValidString(const char * str)
{
    int len = (int)strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] == ' ') return false;
    }
    return true;
}

bool CMisc::bCheckIMEString(const char * str)
{
    int len = (int)strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] < 0) return false;
    }
    return true;
}

void CMisc::ReplaceString(char * pStr, char cFrom, char cTo)
{
    int len = (int)strlen(pStr);
    for (int i = 0; i < len; i++)
    {
        if (pStr[i] == cFrom) pStr[i] = cTo;
    }
}

char CMisc::cCalcDirection(short sX, short sY, short dX, short dY)
{
    double dTmp1, dTmp2, dTmp3;
    // short x,y;
    // double r;

    if ((sX == dX) && (sY == dY)) return 1;
    if ((sX == dX) && (sY != dY))
    {
        if (sY > dY) return 1;
        else return 5;
    }

    if ((sX != dX) && (sY == dY))
    {
        if (sX > dX) return 7;
        else return 3;
    }

    dTmp1 = (double)(dX - sX);
    dTmp2 = (double)(dY - sY);
    dTmp3 = dTmp1 / dTmp2;

    if (dTmp3 < -3)
    {
        if (sX > dX) return 7;
        else return 3;
    }

    if (dTmp3 > 3)
    {
        if (sX > dX) return 7;
        else return 3;
    }

    if ((dTmp3 > -0.3333f) && (dTmp3 <= 0.3333f))
    {
        if (sY > dY) return 1;
        else return 5;
    }

    if ((dTmp3 > 0.3333f) && (dTmp3 <= 3.0f))
    {
        if (sX > dX) return 8;
        else return 4;
    }

    if ((dTmp3 >= -0.3333f) && (dTmp3 < 3.0f))
    {
        if (sX > dX) return 7;
        else return 3;
    }

    if ((dTmp3 >= -3.0f) && (dTmp3 < -0.3333f))
    {
        if (sX > dX) return 6;
        else return 2;
    }

    return 1;
}

bool CMisc::bEncode(char cKey, char * pStr)
{
    int iLen = (int)strlen(pStr);
    for (int i = 0; i <= iLen - 1; i++)
    {
        pStr[i] += i;
        pStr[i] = pStr[i] ^ (cKey ^ (iLen - i));
    }
    return true;
}

bool CMisc::bDecode(char cKey, char * pStr)
{
    int iLen = (int)strlen(pStr);
    for (int i = 0; i <= iLen - 1; i++)
    {
        pStr[i] = pStr[i] ^ (cKey ^ (iLen - i));
        pStr[i] -= i;
    }
    return true;
}

bool CMisc::bCheckValidName(const char * pStr)
{
    int i, iLen;

    iLen = (int)strlen(pStr);
    for (i = 0; i < iLen; i++)
    {
        if (pStr[i] < 0)	return false;
#ifndef DEF_FUCK_USA

        if ((pStr[i] == ',') || (pStr[i] == '=') || (pStr[i] == ' ') || (pStr[i] == '\n') ||
            (pStr[i] == '\t') || (pStr[i] == '.') || (pStr[i] == '\\') || (pStr[i] == '/') ||
            (pStr[i] == ':') || (pStr[i] == '*') || (pStr[i] == '?') || (pStr[i] == '<') ||
            (pStr[i] == '>') || (pStr[i] == '|') || (pStr[i] == '"') || (pStr[i] == '`')) return false;


#else

        if ((pStr[i] == ',') || (pStr[i] == '=') || (pStr[i] == ' ') || (pStr[i] == '\n') ||
            (pStr[i] == '\t') || (pStr[i] == '.') || (pStr[i] == '\\') || (pStr[i] == '/') ||
            (pStr[i] == ':') || (pStr[i] == '*') || (pStr[i] == '?') || (pStr[i] == '<') ||
            (pStr[i] == '>') || (pStr[i] == '|') || (pStr[i] == '"') || (pStr[i] == '`') ||
            (pStr[i] == ';') || (pStr[i] == '=') || (pStr[i] == '@') || (pStr[i] == '[') ||
            (pStr[i] == ']') || (pStr[i] == '^') || (pStr[i] == '_') || (pStr[i] == '\'')) return false;

        if ((pStr[i] < '0') || (pStr[i] > 'z')) return false;

#endif

    }
    return true;
}

int CMisc::iGetTextLengthLoc(sf::Text & text, char * pStr, int iLength)
{
    int i;
    bool bFlag;
    int len = (int)strlen(pStr) - 1;

    std::string s{ pStr };

    text.setPosition(0.f, 0.f);

    int last_space = 0;
    int lines = 0;

    i = 0;
    bFlag = false;
    while (bFlag == false)
    {
        if (i > len) return 0;
        i++;
        if (pStr[i] == ' ') last_space = i;
        text.setString(s.substr(0, i));
        if (text.getGlobalBounds().width > iLength) bFlag = true;
    }
    return i - 1;
}

bool CMisc::bIsValidEmail(char * pStr)
{
    int len = (int)strlen(pStr);
    if (len < 7) return false;
    char cEmail[52];
    memset(cEmail, 0, sizeof(cEmail));
    memcpy(cEmail, pStr, len);
    bool bFlag = false;
    for (int i = 0; i < len; i++)
    {
        if (cEmail[i] == '@') bFlag = true;
    }
    if (bFlag == false) return false;
    bFlag = false;
    for (int i = 0; i < len; i++)
    {
        if (cEmail[i] == '.') bFlag = true;
    }
    if (bFlag == false) return false;
    return true;
}
