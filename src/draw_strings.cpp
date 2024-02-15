//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <algorithm>
#include <iostream>
#include <fmt/format.h>

#if DEF_LANGUAGE == 1
#include "lan_tai.h"
#elif DEF_LANGUAGE == 2
#include "lan_chi.h"
#elif DEF_LANGUAGE == 3
#include "lan_kor.h"
#elif DEF_LANGUAGE == 4
#include "lan_eng.h"
#elif DEF_LANGUAGE == 5
#include "lan_jap.h"
#endif

extern char G_cSpriteAlphaDegree;

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];


extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;//, _tmp_sStatus;
extern int _tmp_sStatus;
extern char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
extern WORD  _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern WORD  wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int sFocusStatus;
extern int   iFocusApprColor;

static char __cSpace[] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 7, 8, 8, 9, 10, 9, 7, 8, 8, 8, 8, 8, 8, 8,
                          15, 16, 12, 17, 14, 15, 14, 16, 10, 13, 19, 10, 17, 17, 15, 14, 15, 16, 13, 17, 16, 16, 20, 17, 16, 14,
                          8, 8, 8, 8, 8, 8, 8, 6, 7, 8, 7, 7, 7, 7, 4, 7, 7, 4, 11, 7, 8, 8, 7, 8, 6, 5, 8, 9, 14, 8, 9, 8, 8, 8, 8, 8,
                          8, 8, 8, 8, 8, 8, 8 };
void CGame::put_string_sprite_font(int iX, int iY, std::string_view pStr, short sR, short sG, short sB)
{
    int iXpos;
    uint32_t iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[100];

    ZeroMemory(cTmpStr, sizeof(cTmpStr));
    strcpy(cTmpStr, pStr.data());
    iXpos = iX;
    for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
    {
        if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122))
        {
            m_pSprite[DEF_SPRID_INTERFACE_FONT1]->put_sprite_color(iXpos + 1, iY, cTmpStr[iCnt] - 33, sR + 11, sG + 7, sB + 6, dwTime);
            if ((sR == 0) && (sG == 0) && (sB == 0))
                m_pSprite[DEF_SPRID_INTERFACE_FONT1]->put_sprite_fast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
            else
                m_pSprite[DEF_SPRID_INTERFACE_FONT1]->put_sprite_color(iXpos, iY, cTmpStr[iCnt] - 33, sR, sG, sB, dwTime);
            iXpos += __cSpace[cTmpStr[iCnt] - 33];
        }
        else
            iXpos += 5;
    }
}

void CGame::put_string_sprite_font2(int iX, int iY, std::string_view pStr, short sR, short sG, short sB)
{
    int iXpos;
    uint32_t iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[200];

    ZeroMemory(cTmpStr, sizeof(cTmpStr));
    strcpy(cTmpStr, pStr.data());

    iXpos = iX;
    for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
    {
        if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122))
        {
            m_pSprite[DEF_SPRID_INTERFACE_FONT1]->put_sprite_fast(iXpos + 1, iY, cTmpStr[iCnt] - 33, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_FONT1]->put_sprite_fast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 33, dwTime);
            if ((sR == 0) && (sG == 0) && (sB == 0))
                m_pSprite[DEF_SPRID_INTERFACE_FONT1]->put_sprite_fast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
            else
                m_pSprite[DEF_SPRID_INTERFACE_FONT1]->put_sprite_color(iXpos, iY, cTmpStr[iCnt] - 33, sR, sG, sB, dwTime);
            iXpos += __cSpace[cTmpStr[iCnt] - 33];
        }
        else
            iXpos += 5;
    }
}

void CGame::put_string_sprite_font3(int iX, int iY, std::string_view pStr, short sR, short sG, short sB, bool bTrans, int iType)
{
    int iXpos, iAdd;
    uint32_t iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[128];

    ZeroMemory(cTmpStr, sizeof(cTmpStr));
    strcpy(cTmpStr, pStr.data());

    if (iType != -1)
    {
        iAdd = 95 * iType;
        iXpos = iX;
        for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
        {
            if ((cTmpStr[iCnt] >= 32) && (cTmpStr[iCnt] <= 126))
            {

                if (bTrans == false)
                {
                    m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->put_sprite_fast(iXpos, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->put_sprite_fast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    if ((sR == 0) && (sG == 0) && (sB == 0))
                        m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->put_sprite_fast(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    else
                        m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->put_sprite_color(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);
                }
                else
                    m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->put_trans_sprite_color(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);

                iXpos += (m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->brush[cTmpStr[iCnt] - 32 + iAdd].szx);
            }
            else
                iXpos += 5;
        }
    }
    else
    {
        iAdd = 0;
        iXpos = iX;
        for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
        {
            if ((cTmpStr[iCnt] >= 32) && (cTmpStr[iCnt] <= 126))
            {

                if (bTrans == false)
                {
                    m_pSprite[DEF_SPRID_INTERFACE_FONT2]->put_sprite_fast(iXpos, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    m_pSprite[DEF_SPRID_INTERFACE_FONT2]->put_sprite_fast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    if ((sR == 0) && (sG == 0) && (sB == 0))
                        m_pSprite[DEF_SPRID_INTERFACE_FONT2]->put_sprite_fast(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, dwTime);
                    else
                        m_pSprite[DEF_SPRID_INTERFACE_FONT2]->put_sprite_color(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);
                }
                else
                    m_pSprite[DEF_SPRID_INTERFACE_FONT2]->put_trans_sprite_color(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);

                iXpos += (m_pSprite[DEF_SPRID_INTERFACE_FONT2]->brush[cTmpStr[iCnt] - 32 + iAdd].szx);
            }
            else
                iXpos += 5;
        }
    }
}

static char __cSpace2[] = { 6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 6 }; //{8,6,9,8,8,9,8,8,8,8};
void CGame::put_string_sprite_number(int iX, int iY, char * pStr, short sR, short sG, short sB)
{
    int iXpos;
    unsigned char iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[200];
    ZeroMemory(cTmpStr, sizeof(cTmpStr));
    strcpy(cTmpStr, pStr);
    iXpos = iX;
    for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
    {
        if ((cTmpStr[iCnt] >= 0x30) && (cTmpStr[iCnt] <= 0x39))
        {
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_trans_sprite(iXpos + 2, iY, cTmpStr[iCnt] - 0x30 + 6, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_trans_sprite(iXpos + 1, iY + 1, cTmpStr[iCnt] - 0x30 + 6, dwTime);
            if ((sR == 0) && (sG == 0) && (sB == 0))
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_trans_sprite(iXpos, iY, cTmpStr[iCnt] - 0x30 + 6, dwTime);
            else
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_trans_sprite_color(iXpos, iY, cTmpStr[iCnt] - 0x30 + 6, sR, sG, sB, dwTime);
            iXpos += __cSpace2[cTmpStr[iCnt] - 0x30];
        }
    }
}

void CGame::put_string(int iX, int iY, std::string pString, Color color, bool bHide, char cBGtype)
{
    int i;
    if (pString.length() == 0)
        return;
    if (bHide == false)
    {
        switch (cBGtype)
        {
        case 0:
            put_font_string("default", iX + 1, iY, pString, color);
            break;
        case 1:
            put_font_string("default", iX, iY + 1, pString, Color(5, 5, 5));
            put_font_string("default", iX + 1, iY + 1, pString, Color(5, 5, 5));
            put_font_string("default", iX + 1, iY, pString, Color(5, 5, 5));
            break;
        }
        put_font_string("default", iX, iY, pString, color);
    }
    else
    {
        for (i = 0; i < pString.length(); ++i)
            if (pString[i] != 0)
                pString[i] = '*';

        switch (cBGtype)
        {
        case 0:
            put_font_string("default", iX + 1, iY, pString, color);
            break;
        case 1:
            put_font_string("default", iX, iY + 1, pString, Color(5, 5, 5));
            put_font_string("default", iX + 1, iY + 1, pString, Color(5, 5, 5));
            put_font_string("default", iX + 1, iY, pString, Color(5, 5, 5));
            break;
        }
        put_font_string("default", iX, iY, pString, color);
    }
}
void CGame::put_chat_string(int iX, int iY, std::string pString, Color color)
{
    put_font_string("default", iX, iY, pString, color); //TODO: make 'chat' font?
}
void CGame::put_chat_window_string(int iX, int iY, std::string pString, Color color)
{
    try
    {
        chat_window_text.setString(pString);
        chat_window_text.setFillColor(color);
        chat_window_text.setPosition((float)iX, (float)iY);
        draw(chat_window_text);
    }
    catch (const std::out_of_range &)
    {
        //error
        //__asm int 3;
        __debugbreak();
    }
}
void CGame::put_font_string_size(std::string fontname, int iX, int iY, std::string text, Color color, int size)
{
    try
    {
        _text.setFont(_font.at(fontname));
        _text.setString(text);
        _text.setFillColor(color);
        _text.setPosition((float)iX, (float)iY);
        _text.setCharacterSize(size);
        draw(_text);
    }
    catch (const std::out_of_range &)
    {
        //error
        //__asm int 3;
        __debugbreak();
    }
}

void CGame::put_font_string(std::string fontname, int iX, int iY, std::string text, Color color)
{
    try
    {
        _text.setFont(_font.at(fontname));
        _text.setString(text);
        _text.setFillColor(color);
        _text.setPosition((float)iX, (float)iY);
        _text.setCharacterSize(12);
        draw(_text);
    }
    catch (const std::out_of_range &)
    {
        //error
        //__asm int 3;
        __debugbreak();
    }
}

void CGame::put_aligned_string(int iX1, int iX2, int iY, std::string text, Color color)
{
    try
    {
        _text.setFont(_font.at("default"));
        _text.setString(text);
        _text.setFillColor(color);
        sf::FloatRect bounds = _text.getLocalBounds();
        // todo - properly make an aligned string function
        //_text.setPosition((float)((float(iX2) - iX1) / 2 + bounds.width), (float)iY);
        _text.setPosition((float)iX1, (float)iY);
        _text.setCharacterSize(12);

        draw(_text);
    }
    catch (const std::out_of_range &)
    {
        //error
        //__asm int 3;
        __debugbreak();
    }
}

void CGame::put_overhead_string(int x, int y, std::string text, Color color, int multiplier, bool transparency, int size)
{
    overhead_text.setString(text);
    overhead_text.setFillColor(Color(color.r * multiplier, color.g * multiplier, color.b * multiplier, transparency ? 200 : 255));
    overhead_text.setPosition((float)x, (float)y);
    overhead_text.setCharacterSize(size);
    draw(overhead_text);
}

void CGame::put_under_entity_string(int x, int y, std::string text, Color color, int size)
{
    under_text.setString(text);
    under_text.setFillColor(color);
    under_text.setPosition((float)x, (float)y);
    under_text.setCharacterSize(size);
    draw(under_text);
}
