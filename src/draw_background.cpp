//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include <fmt/core.h>
#include <fmt/format.h>
#include "Game.h"
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
#include "MapData.h"
#include "SpriteID.h"
#include "sprite.h"
#include "TileSpr.h"

extern char G_cSpriteAlphaDegree;

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];


extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;//, _tmp_sStatus;
extern int _tmp_sStatus;
extern char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
extern uint16_t  _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern uint16_t  wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int sFocusStatus;
extern int   iFocusApprColor;

void CGame::DrawBackground(short sDivX, short sModX, short sDivY, short sModY)
{
    int16_t indexX, indexY, ix, iy;
    short sSpr, sSprFrame;
    //if (sDivX < 0 || sDivY < 0) return;
    int16_t mouse_x = ((sDivX) * 32 + sModX + m_stMCursor.sX - 17) / 32 + 1;
    int16_t mouse_y = ((sDivY) * 32 + sModY + m_stMCursor.sY - 17) / 32 + 1;

    //if ((m_bIsRedrawPDBGS == true) || (m_iPDBGSdivX != sDivX) || (m_iPDBGSdivY != sDivY))
    {
        //m_bIsRedrawPDBGS = false;
        m_iPDBGSdivX = sDivX;
        m_iPDBGSdivY = sDivY;
        indexY = sDivY;
        for (iy = -sModY; iy < get_virtual_height() + 100; iy += 32)
        {
            if (indexY < 0 || indexY >= m_pMapData->m_sMapSizeY) { indexY++; continue; }
            indexX = sDivX;
            for (ix = -sModX; ix < get_virtual_width() + 100; ix += 32)
            {
                if (indexX < 0 || indexX >= m_pMapData->m_sMapSizeX) { indexX++; continue; }
                sSpr = m_pMapData->m_tile[indexX][indexY].m_sTileSprite;
                sSprFrame = m_pMapData->m_tile[indexX][indexY].m_sTileSpriteFrame;
                if (m_pTileSpr[sSpr])
                {
                    if (highlight_tile && indexX == mouse_x && indexY == mouse_y)
                        m_pTileSpr[sSpr]->draw_sprite_no_color_key(ix - 16 + sModX, iy - 16 + sModY, sSprFrame, m_dwCurTime, Color(255, 180, 180));
                    else
                        m_pTileSpr[sSpr]->draw_sprite_no_color_key(ix - 16 + sModX, iy - 16 + sModY, sSprFrame, m_dwCurTime);
                }
                indexX++;
            }
            indexY++;
        }
        if (m_showGrid)
        {
            _text.setFont(_font.at("test"));
            _text.setFillColor(Color(255, 127, 127, 255));
            _text.setCharacterSize(14);
            _text.setStyle(sf::Text::Regular);
            std::string text;

            // todo: optimize this to only draw entire lines across the screen instead of 4 per tile
            indexY = sDivY + 1;
            for (iy = -sModY; iy < get_virtual_height() + 32; iy += 32)
            {
                indexX = sDivX;
                for (ix = -sModX; ix < get_virtual_width() + 32; ix += 32)
                {
                    {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(float(ix - 16 + sModX), float(iy - 16 + sModY)), Color(0, 0, 0, 255)),
                            sf::Vertex(sf::Vector2f(float(ix - 16 + sModX), float(iy + 16 + sModY)), Color(0, 0, 0, 255)) };
                        bg.draw(line, 2, sf::Lines);
                    }
                    {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(float(ix - 16 + sModX), float(iy - 16 + sModY)), Color(0, 0, 0, 255)),
                            sf::Vertex(sf::Vector2f(float(ix + 16 + sModX), float(iy - 16 + sModY)), Color(0, 0, 0, 255)) };
                        bg.draw(line, 2, sf::Lines);
                    }

                    //                     int id = 0;
                    //                     if (m_pMapData->m_pData[dX][dY].owner) id = m_pMapData->m_pData[dX][dY].owner->id;
                                        //text = fmt::format("({},{})\n({},{})", (m_sViewPointX + ix) / 32, (m_sViewPointY + iy) / 32, dX, dY);
                    text = fmt::format("{}\n{}\n{},{}", indexX, indexY, abs(m_sPlayerX - indexX), abs(m_sPlayerY - indexY));
                    //text = fmt::format("({},{})", m_pMapData->m_pData[dX][dY].m_wObjectID, id);

                    _text.setCharacterSize(8);
                    _text.setOutlineThickness(0.5);
                    _text.setOutlineColor(sf::Color::Black);
                    _text.setFillColor(sf::Color::White);
                    _text.setString(text);
                    _text.setPosition(float(ix - 16 + sModX), float(iy + 16 + sModY));
                    bg.draw(_text);
                    indexX++;
                }
                indexY++;
            }
        }
    }


    if (m_bIsCrusadeMode)
    {
        if (m_iConstructLocX != -1) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, m_iConstructLocX * 32 - m_sViewPointX, m_iConstructLocY * 32 - m_sViewPointY, 41);
        if (m_iTeleportLocX != -1) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, m_iTeleportLocX * 32 - m_sViewPointX, m_iTeleportLocY * 32 - m_sViewPointY, 42);
    }
}
