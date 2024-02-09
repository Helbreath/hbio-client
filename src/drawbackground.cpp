//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

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

void CGame::DrawBackground(short sDivX, short sModX, short sDivY, short sModY)
{
    int indexX, indexY, ix, iy;
    short sSpr, sSprFrame;
    if (sDivX < 0 || sDivY < 0) return;
    //if ((m_bIsRedrawPDBGS == TRUE) || (m_iPDBGSdivX != sDivX) || (m_iPDBGSdivY != sDivY))
    {
        //m_bIsRedrawPDBGS = FALSE;
        m_iPDBGSdivX = sDivX;
        m_iPDBGSdivY = sDivY;
        indexY = sDivY + m_pMapData->m_sPivotY;
        for (iy = -sModY; iy < GetVirtualHeight() + 48; iy += 32)
        {
            indexX = sDivX + m_pMapData->m_sPivotX;
            for (ix = -sModX; ix < GetVirtualWidth() + 48; ix += 32)
            {
                sSpr = m_pMapData->m_tile[indexX][indexY].m_sTileSprite;
                sSprFrame = m_pMapData->m_tile[indexX][indexY].m_sTileSpriteFrame;
                if (m_pTileSpr[sSpr])
                {
                    m_pTileSpr[sSpr]->DrawSpriteNCK(ix - 16 + sModX, iy - 16 + sModY, sSprFrame, m_dwCurTime);
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

            indexY = sDivY;
            for (iy = -sModY; iy < GetHeight() + 32; iy += 32)
            {
                indexX = sDivX;
                for (ix = -sModX; ix < GetWidth() + 32; ix += 32)
                {
                    {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f((float)ix - 16 + sModX, (float)iy - 16 + sModY), Color(0, 0, 0, 255)),
                            sf::Vertex(sf::Vector2f((float)ix - 16 + sModX, (float)iy + 16 + sModY), Color(0, 0, 0, 255)) };
                        bg.draw(line, 2, sf::Lines);
                    }
                    {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f((float)ix - 16 + sModX, (float)iy - 16 + sModY), Color(0, 0, 0, 255)),
                            sf::Vertex(sf::Vector2f((float)ix + 16 + sModX, (float)iy - 16 + sModY), Color(0, 0, 0, 255)) };
                        bg.draw(line, 2, sf::Lines);
                    }

//                     int id = 0;
//                     if (m_pMapData->m_pData[dX][dY].owner) id = m_pMapData->m_pData[dX][dY].owner->id;
                    //text = fmt::format("({},{})\n({},{})", (m_sViewPointX + ix) / 32, (m_sViewPointY + iy) / 32, dX, dY);
                    text = fmt::format("{}\n{}", indexX, indexY);
                    //text = fmt::format("({},{})", m_pMapData->m_pData[dX][dY].m_wObjectID, id);

                    _text.setString(text);
                    _text.setPosition((float)ix - 16 + sModX, (float)iy + 16 + sModY);
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
