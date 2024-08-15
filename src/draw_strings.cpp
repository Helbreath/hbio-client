//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <algorithm>
#include <iostream>
#include <fmt/format.h>
#include "sprite.h"
#include "sprite_id.h"
#include "lan_eng.h"

extern char G_cSpriteAlphaDegree;

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];


extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;//, _tmp_sStatus;
extern int _tmp_iStatus;
extern char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
extern uint16_t  _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern uint16_t  wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int iFocusStatus;
extern int   iFocusApprColor;

static char __cSpace[] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 7, 8, 8, 9, 10, 9, 7, 8, 8, 8, 8, 8, 8, 8,
                          15, 16, 12, 17, 14, 15, 14, 16, 10, 13, 19, 10, 17, 17, 15, 14, 15, 16, 13, 17, 16, 16, 20, 17, 16, 14,
                          8, 8, 8, 8, 8, 8, 8, 6, 7, 8, 7, 7, 7, 7, 4, 7, 7, 4, 11, 7, 8, 8, 7, 8, 6, 5, 8, 9, 14, 8, 9, 8, 8, 8, 8, 8,
                          8, 8, 8, 8, 8, 8, 8 };
void CGame::put_string_sprite_font(uint16_t iX, uint16_t iY, std::string_view pStr, uint8_t sR, uint8_t sG, uint8_t sB)
{
    int iXpos;
    uint32_t iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[100];

    memset(cTmpStr, 0, sizeof(cTmpStr));
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

void CGame::put_string_sprite_font2(uint16_t iX, uint16_t iY, std::string_view pStr, uint8_t sR, uint8_t sG, uint8_t sB)
{
    int iXpos;
    uint32_t iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[200];

    memset(cTmpStr, 0, sizeof(cTmpStr));
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

void CGame::put_string_sprite_font3(uint16_t iX, uint16_t iY, std::string_view pStr, uint8_t sR, uint8_t sG, uint8_t sB, bool bTrans, int iType)
{
    int iXpos, iAdd;
    uint32_t iCnt;
    uint64_t dwTime = G_dwGlobalTime;
    char cTmpStr[128];

    memset(cTmpStr, 0, sizeof(cTmpStr));
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
void CGame::put_string_sprite_number(uint16_t iX, uint16_t iY, std::string_view pStr, uint8_t sR, uint8_t sG, uint8_t sB)
{
    int iXpos;
    unsigned char iCnt;
    uint64_t dwTime = G_dwGlobalTime;

    iXpos = iX;
    for (iCnt = 0; iCnt < pStr.length(); iCnt++)
    {
        if ((pStr[iCnt] >= 0x30) && (pStr[iCnt] <= 0x39))
        {
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_trans_sprite(iXpos + 2, iY, pStr[iCnt] - 0x30 + 6, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_trans_sprite(iXpos + 1, iY + 1, pStr[iCnt] - 0x30 + 6, dwTime);
            if ((sR == 0) && (sG == 0) && (sB == 0))
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_trans_sprite(iXpos, iY, pStr[iCnt] - 0x30 + 6, dwTime);
            else
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_trans_sprite_color(iXpos, iY, pStr[iCnt] - 0x30 + 6, sR, sG, sB, dwTime);
            iXpos += __cSpace2[pStr[iCnt] - 0x30];
        }
    }
}

void CGame::render_item_details_box(uint16_t iX, uint16_t iY, std::vector<chat_msg> strings, Color background_color)
{
    if (strings.size() == 0)
        return;

    item_box.clear(background_color);

    uint16_t width = 0;
    uint16_t height = 5;

    for (int i = 0; i < strings.size(); i++)
    {
        _text.setOutlineThickness(0);
        _text.setFont(*arya_font);
        _text.setString(strings[i].msg);
        _text.setFillColor(strings[i].color);
        _text.setPosition(5.f, (float)height);
        _text.setCharacterSize(strings[i].size);
        item_box.draw(_text);

        sf::FloatRect bounds = _text.getLocalBounds();
        if (bounds.width + 5.f > width)
            width = bounds.width + 5.f;
        height += strings[i].size * 1.3;
    }

    width += 10;
    height += 5;

    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(1.f, 1.f), Color(200, 200, 200)),
        sf::Vertex(sf::Vector2f(1.f, float(height - 1)), Color(200, 200, 200)),
        sf::Vertex(sf::Vector2f(float(width - 1), float(height - 1)), Color(200, 200, 200)),
        sf::Vertex(sf::Vector2f(float(width - 1), 1.f), Color(200, 200, 200)),
        sf::Vertex(sf::Vector2f(1.f, 1.f), Color(200, 200, 200)),
    };
    item_box.draw(line, 5, sf::LineStrip);

    item_box.display();
    sf::Sprite sprite(item_box.getTexture());
    sprite.setPosition((float)iX, (float)iY);
    sprite.setTextureRect({ 0, 0, width, height });
    draw(sprite);
}

void CGame::put_string(uint16_t iX, uint16_t iY, std::string pString, Color color, bool bHide, char cBGtype)
{
    if (pString.length() == 0)
        return;
    if (bHide == false)
    {
        switch (cBGtype)
        {
            case 0:
                put_font_string(default_font, iX + 1, iY, pString, color);
                break;
            case 1:
                put_font_string(default_font, iX, iY + 1, pString, Color(5, 5, 5));
                put_font_string(default_font, iX + 1, iY + 1, pString, Color(5, 5, 5));
                put_font_string(default_font, iX + 1, iY, pString, Color(5, 5, 5));
                break;
        }
        put_font_string(default_font, iX, iY, pString, color);
    }
    else
    {
        for (int i = 0; i < pString.length(); ++i)
            if (pString[i] != 0)
                pString[i] = '*';

        switch (cBGtype)
        {
            case 0:
                put_font_string(default_font, iX + 1, iY, pString, color);
                break;
            case 1:
                put_font_string(default_font, iX, iY + 1, pString, Color(5, 5, 5));
                put_font_string(default_font, iX + 1, iY + 1, pString, Color(5, 5, 5));
                put_font_string(default_font, iX + 1, iY, pString, Color(5, 5, 5));
                break;
        }
        put_font_string(default_font, iX, iY, pString, color);
    }
}
void CGame::put_chat_string(uint16_t iX, uint16_t iY, std::string pString, Color color)
{
    put_font_string(default_font, iX, iY, pString, color); //TODO: make 'chat' font?
}
void CGame::put_chat_window_string(uint16_t iX, uint16_t iY, std::string pString, Color color)
{
    chat_window_text.setString(pString);
    chat_window_text.setFillColor(color);
    chat_window_text.setPosition((float)iX, (float)iY);
    draw(chat_window_text);
}
void CGame::put_font_string_size(sf::Font * fontname, uint16_t iX, uint16_t iY, std::string text, Color color, int size, int outline_size, Color outline_color)
{
    _text.setFont(*fontname);
    _text.setString(text);
    _text.setFillColor(color);
    if (outline_size > 0)
    {
        _text.setOutlineColor(outline_color);
        _text.setOutlineThickness(outline_size);
    }
    _text.setPosition((float)iX, (float)iY);
    _text.setCharacterSize(size);
    draw(_text);
}

void CGame::put_font_string(sf::Font * fontname, uint16_t iX, uint16_t iY, std::string text, Color color)
{
    _text.setOutlineThickness(0);
    _text.setFont(*fontname);
    _text.setString(text);
    _text.setFillColor(color);
    _text.setPosition((float)iX, (float)iY);
    _text.setCharacterSize(12);
    draw(_text);
}

void CGame::put_aligned_string(uint16_t iX1, uint16_t iX2, uint16_t iY, std::string text, Color color, int font_size)
{
    _text.setOutlineThickness(0);
    _text.setFont(*default_font);
    _text.setString(text);
    _text.setFillColor(color);
    _text.setCharacterSize(font_size);
    sf::FloatRect bounds = _text.getLocalBounds();
    // convert to int first to avoid sub-pixel blurring
    _text.setPosition(float(int(iX1 + (iX2 - iX1 - bounds.width) / 2)), iY);

    draw(_text);
}

void CGame::put_overhead_string(uint16_t x, uint16_t y, std::string text, Color color, int multiplier, bool transparency, int size)
{
    overhead_text.setString(text);
    overhead_text.setFillColor(Color(color.r * multiplier, color.g * multiplier, color.b * multiplier, transparency ? 200 : 255));
    overhead_text.setPosition((float)x, (float)y);
    overhead_text.setCharacterSize(size);
    draw(overhead_text);
}

void CGame::put_under_entity_string(uint16_t x, uint16_t y, std::string text, Color color, uint8_t size)
{
    under_text.setString(text);
    under_text.setFillColor(color);
    under_text.setPosition((float)x, (float)y);
    under_text.setCharacterSize(size);
    draw(under_text);
}

void CGame::put_string_sprite_font4(uint16_t iX, uint16_t iY, std::string_view pStr, uint8_t sR, uint8_t sG, uint8_t sB)
{
    int iXpos, iAdd;
    uint32_t iCnt;
    uint32_t dwTime = G_dwGlobalTime;

    iAdd = 95 * 2;
    iXpos = iX;
    for (iCnt = 0; iCnt < pStr.length(); iCnt++)
    {
        if ((pStr[iCnt] >= 32) && (pStr[iCnt] <= 126))
        {
            m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->put_sprite_fast(iXpos, iY + 1, pStr[iCnt] - 32 + iAdd, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->put_sprite_fast(iXpos + 1, iY + 1, pStr[iCnt] - 32 + iAdd, dwTime);
            if ((sR == 0) && (sG == 0) && (sB == 0))
                m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->put_sprite_fast(iXpos, iY, pStr[iCnt] - 32 + iAdd, dwTime);
            else m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->put_sprite_color(iXpos, iY, pStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);
            iXpos += (m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2]->brush[pStr[iCnt] - 32 + iAdd].szx);
        }
        else iXpos += 5;
    }
}
