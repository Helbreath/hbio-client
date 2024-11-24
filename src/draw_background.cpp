//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include <fmt/core.h>
#include <fmt/format.h>
#include "Game.h"
#include "lan_eng.h"
#include "map_data.h"
#include "sprite_id.h"
#include "sprite.h"
#include "tile_spr.h"

extern char G_cSpriteAlphaDegree;

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];


extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;
extern int _tmp_iStatus;
extern char _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int64_t _tmp_owner_time, _tmp_start_time;
extern int64_t _tmp_max_frames, _tmp_frame_time;
extern int _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
extern uint16_t _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern uint16_t wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int iFocusStatus;
extern int iFocusApprColor;

#define DEF_HEIGHT 15
#define DEF_AMOUNT 10
#define DEF_TIME   100
void CGame::DrawBackground(short sDivX, short sModX, short sDivY, short sModY)
{
    int indexX, indexY, ix, iy;
    static double dradd = 1.0, dreffect = 0.0, dradd2 = 8.0, dradd3 = 1.0;
    float drupdate = 0.0;
    static int64_t drtime = m_dwCurTime;
    static bool bParsed = false;
    bool bReverse = false;
    static bool bReverse2 = false;
    short sSpr, sSprFrame;

    if (sDivX < 0 || sDivY < 0) return;

    int16_t mouse_x = ((sDivX) * 32 + sModX + m_stMCursor.sX - 17) / 32 + 1;
    int16_t mouse_y = ((sDivY) * 32 + sModY + m_stMCursor.sY - 17) / 32 + 1;

    // drupdate += dradd;
    // if (drupdate > 30)dradd = -1;
    // if (drupdate < 1)dradd = 1;
     //dreffect+= drupdate;

    //if ((m_bIsRedrawPDBGS == true) || (m_iPDBGSdivX != sDivX) || (m_iPDBGSdivY != sDivY)) {
        //m_bIsRedrawPDBGS = false;
    m_iPDBGSdivX = sDivX;
    m_iPDBGSdivY = sDivY;
    indexY = sDivY;
    bool test = false;
    for (iy = -sModY; iy < get_virtual_height() + 100; iy += 32)
    {
        if (indexY < 0 || indexY >= m_pMapData->m_sMapSizeY) { indexY++; continue; }
        indexX = sDivX;
        for (ix = -sModX; ix < get_virtual_width() + 100; ix += 32)
        {
            if (indexX < 0 || indexX >= m_pMapData->m_sMapSizeX) { indexX++; continue; }
            test = false;
            sSpr = m_pMapData->m_tile[indexX][indexY].m_sTileSprite;
            sSprFrame = m_pMapData->m_tile[indexX][indexY].m_sTileSpriteFrame;

            if (m_pTileSpr[sSpr])
            {
                // todo: current tile hover highlight
                if (highlight_tile && indexX == mouse_x && indexY == mouse_y)
                    m_pTileSpr[sSpr]->draw_sprite_no_color_key(ix - 16 + sModX, iy - 16 + sModY, sSprFrame, m_dwCurTime, Color(255, 180, 180));
                else
                    m_pTileSpr[sSpr]->draw_sprite_no_color_key(ix - 16 + sModX, iy - 16 + sModY, sSprFrame, m_dwCurTime);
            }

            // todo: this is the casting range indicator - finish it
//             for (int i = 0; i < 50; i++)
//             {
//                 if ((indexX == m_coordlist[i].x) && indexY == m_coordlist[i].y)
//                 {
//                     m_pTileSpr[sSpr]->put_sprite_colorDst(m_DDraw.m_lpPDBGS, ix - 16 + sModX, iy - 16 + sModY, sSprFrame, 0, 0, 15, m_dwCurTime);
//                     test = true;
//                     break;
//                 }
//             }
//             if (test == false)
//                 if ((((indexX >= m_sPlayerX - 1) && (indexX <= m_sPlayerX + 1)) && ((indexY >= m_sPlayerY - 1) && (indexY <= m_sPlayerY + 1)))
//                     || ((indexX == (m_sViewPointX + m_sMX + 16) / 32) && (indexY == (m_sViewPointY + m_sMY + 16) / 32)))
//                     m_pTileSpr[sSpr]->put_sprite_colorDst(m_DDraw.m_lpPDBGS, ix - 16 + sModX, iy - 16 + sModY, sSprFrame, 0, 0, 15, m_dwCurTime);
//                 else
//                     m_pTileSpr[sSpr]->put_sprite_fastNoColorKeyDst(m_DDraw.m_lpPDBGS, ix - 16 + sModX /*+ dreffect%100*//*(ix%30) + (drupdate%30)*//* + (indexY%150)*/, iy - 16 + sModY, sSprFrame, m_dwCurTime);

            indexX++;
        }
        indexY++;
    }

    if (m_showGrid)
    {
        _text.setOutlineThickness(0);
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

                if (show_tile_details)
                {
                    text = fmt::format("{}\n{}\n{},{}", indexX, indexY, abs(m_sPlayerX - indexX), abs(m_sPlayerY - indexY));

                    _text.setCharacterSize(8);
                    _text.setOutlineThickness(0.5);
                    _text.setOutlineColor(sf::Color::Black);
                    _text.setFillColor(sf::Color::White);
                    _text.setString(text);
                    _text.setPosition(float(ix - 16 + sModX), float(iy + 16 + sModY));
                    bg.draw(_text);
                }
                indexX++;
            }
            indexY++;
        }
    }

//     //}
//     //Change Druncn City Effect 0x0104
//     RECT rcRect;
//     //SetRect(&rcRect, sModX, sModY, 640+sModX, 480+sModY); // our fictitious sprite bitmap is 
//     //m_DDraw.m_lpBackB4->BltFast( 0, 0, m_DDraw.m_lpPDBGS, &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
//     //dradd = 1.0;
//     if (m_dwCurTime - drtime > DEF_TIME)
//     {
//         //if (dreffect > 200.0) dradd2 = -1.0;
//         //if (dreffect < 1.0) dradd2 = 1.0;
//         dreffect += 1.0;
//         drtime = m_dwCurTime;
//         bParsed = false;
// 
//         if (dradd2 >= DEF_AMOUNT) bReverse2 = true;
//         else if (dradd2 <= 0) bReverse2 = false;
//         if (!bReverse2) dradd2++;
//         else if (bReverse2) dradd2--;
//         //dradd2 += (sModY - 10);
//     }
//     /*int dre1 = ((int)dreffect)%10;
//     if (dre1 == 0 && dradd3 == 1 && bParsed == false)
//     {
//         dradd3 = 2;
//         bParsed = true;
//     }
//     else if (dre1 == 0 && dradd3 == 2 && bParsed == false)
//     {
//         dradd3 = 1;
//         bParsed = true;
//     }
//     if (dradd3 == 1)
//     {
//         drupdate = ((int)dreffect%10);
//     }
//     else if (dradd3 == 2)
//     {
//         drupdate = 10-((int)dreffect%10);
//     }*/
//     dradd = 0.0;
//     drupdate = (DEF_AMOUNT - dradd2);
//     bReverse = bReverse2;
//     for (int i = 0; i < get_virtual_height(); i += DEF_HEIGHT)
//     {
//         //dreffect++;
//         //if (dreffect > 2) dreffect = 0;
//         //if (drupdate > 9.0) dradd = -1.0;
//         //if (drupdate < 1.0) dradd = 1.0;
//         //if (drupdate > 9.0) drupdate = -1.0;
//         //drupdate += dradd;
// 
//         /*if (!bReverse)
//         {
//             dradd++;
//             drupdate = (int)dreffect%10 - dradd + dradd2;
//         }
//         else if (bReverse)
//         {
//             dradd--;
//             drupdate = (int)dreffect%10 - dradd + dradd2;
//         }
//         if (dradd == 9) bReverse = true;
//         else if (dradd == 0) bReverse = false;*/
// 
//         //if (drupdate < 0) drupdate = 0;
// 
//         // todo: smooth out the effect
//         if ((m_cDetailLevel != 0) && (memcmp(m_cMapName, "druncncity", 10) == 0))
//         {
//             if (drupdate == 0)
//                 SetRect(&rcRect, sModX + (int)drupdate + 1, sModY + i, get_virtual_width() + sModX + (int)drupdate + 1, sModY + i + DEF_HEIGHT);
//             else if (drupdate == DEF_AMOUNT)
//                 SetRect(&rcRect, sModX + (int)drupdate - 1, sModY + i, get_virtual_width() + sModX + (int)drupdate - 1, sModY + i + DEF_HEIGHT);
//             else
//                 SetRect(&rcRect, sModX + (int)drupdate, sModY + i, get_virtual_width() + sModX + (int)drupdate, sModY + i + DEF_HEIGHT);
//         }
//         else
//         {
//             SetRect(&rcRect, sModX, sModY + i, get_virtual_width() + sModX, sModY + i + DEF_HEIGHT);
//         }
// 
//         //SetRect(&rcRect, sModX+(int)drupdate, sModY+i, 640+sModX+(int)drupdate, sModY+i+12);
//         m_DDraw.m_lpBackB4->BltFast(0, 0 + i, m_DDraw.m_lpPDBGS, &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
//         //format_to_local(G_cTxt, "drupdate: {}, dradd2: {}, sModX: {}, sModY: {}", (int)drupdate, (int)dradd2, sModX, sModY);
//         //put_string(0, 0+i, G_cTxt, Color::White);
//         if (drupdate >= DEF_AMOUNT) bReverse = true;
//         else if (drupdate <= 0) bReverse = false;
//         if (bReverse)
//             drupdate--;
//         else if (!bReverse)
//             drupdate++;
//     }

    if (m_bIsCrusadeMode)
    {
        if (m_iConstructLocX != -1) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, m_iConstructLocX * 32 - m_sViewPointX, m_iConstructLocY * 32 - m_sViewPointY, 41);
        if (m_iTeleportLocX != -1) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, m_iTeleportLocX * 32 - m_sViewPointX, m_iTeleportLocY * 32 - m_sViewPointY, 42);
    }
}

