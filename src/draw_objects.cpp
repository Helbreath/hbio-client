//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <fmt/format.h>
#include "sprite.h"
#include "sprite_id.h"
#include "CharInfo.h"
#include "MouseInterface.h"
#include "action_id.h"
#include "Item.h"
#include "MapData.h"
#include "lan_eng.h"
#include "dynamic_object_id.h"
#include "Tile.h"
#include "TileSpr.h"

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

void CGame::DrawObjects(short sPivotX, short sPivotY, short sDivX, short sDivY, short sModX, short sModY, short msX, short msY)
{
    int ix{}, iy{}, indexX{}, indexY{}, dX{}, dY{}, iDvalue{};
    char cItemColor{};
    bool bIsPlayerDrawed = false;
    bool bContact = false;
    bool bRet = false;
    short sItemSprite{}, sItemSpriteFrame{}, sObjSpr{}, sObjSprFrame{}, sDynamicObject{}, sDynamicObjectFrame{};
    static uint32_t dwMCAnimTime = G_dwGlobalTime;
    static short sMCAnimFrame = 1;

    if (sDivY < 0 || sDivX < 0) return;
    m_sMCX = 0;
    m_sMCY = 0;
    memset(m_cMCName, 0, sizeof(m_cMCName));

    //dwTime = G_dwGlobalTime;
    uint32_t dwTime = m_dwCurTime;
    m_stMCursor.sCursorFrame = 0;

    uint16_t xdiff = ((get_virtual_width() / 32) / 2);
    uint16_t ydiff = ((get_virtual_height() / 32) / 2);

    indexY = sDivY - ydiff;
    for (iy = -sModY - ydiff * 32; iy <= get_virtual_height() + (ydiff * 32) * 2; iy += 32)
    {
        indexX = sDivX - xdiff;
        for (ix = -sModX - xdiff * 32; ix <= get_virtual_width() + (xdiff * 32) * 2; ix += 32)
        {
            sDynamicObject = 0;
            bRet = false;

            if ((ix >= -sModX) && (ix <= get_virtual_width() + 16) && (iy >= -sModY) && (iy <= get_virtual_height() + 32 + 16))
            {
                _tmp_wObjectID = _tmp_sOwnerType = _tmp_sAppr1 = _tmp_sAppr2 = _tmp_sAppr3 = _tmp_sAppr4 = _tmp_iStatus = 0;
                _tmp_cDir = _tmp_cFrame = 0;
                _tmp_iEffectType = _tmp_iEffectFrame = _tmp_iChatIndex = 0;

                memset(_tmp_cName, 0, sizeof(_tmp_cName));

                if ((indexX < 0) || (indexX > MAPDATASIZEX) ||
                    (indexY < 0) || (indexY > MAPDATASIZEY))
                {

                    sItemSprite = 0;
                    sItemSpriteFrame = 0;
                    bRet = false;
                    cItemColor = 0;
                }
                else
                {
                    _tmp_dX = dX = indexX; // v2.171 2002-6-14
                    _tmp_dY = dY = indexY; // v2.171 2002-6-14
                    _tmp_wObjectID = m_pMapData->m_pData[dX][dY].m_wDeadObjectID;
                    _tmp_sOwnerType = m_pMapData->m_pData[dX][dY].m_sDeadOwnerType;
                    _tmp_cDir = m_pMapData->m_pData[dX][dY].m_cDeadDir;
                    _tmp_sAppr1 = m_pMapData->m_pData[dX][dY].m_sDeadAppr1;
                    _tmp_sAppr2 = m_pMapData->m_pData[dX][dY].m_sDeadAppr2;
                    _tmp_sAppr3 = m_pMapData->m_pData[dX][dY].m_sDeadAppr3;
                    _tmp_sAppr4 = m_pMapData->m_pData[dX][dY].m_sDeadAppr4;
                    _tmp_iApprColor = m_pMapData->m_pData[dX][dY].m_iDeadApprColor; // v1.4
                    _tmp_cFrame = m_pMapData->m_pData[dX][dY].m_cDeadOwnerFrame;
                    _tmp_iChatIndex = m_pMapData->m_pData[dX][dY].m_iDeadChatMsg;
                    _tmp_iStatus = m_pMapData->m_pData[dX][dY].m_iDeadStatus;

                    strcpy(_tmp_cName, m_pMapData->m_pData[dX][dY].m_cDeadOwnerName);

                    sItemSprite = m_pMapData->m_pData[dX][dY].m_sItemSprite;
                    sItemSpriteFrame = m_pMapData->m_pData[dX][dY].m_sItemSpriteFrame;
                    cItemColor = m_pMapData->m_pData[dX][dY].m_cItemColor;

                    sDynamicObject = m_pMapData->m_pData[dX][dY].m_sDynamicObjectType;
                    sDynamicObjectFrame = (short)m_pMapData->m_pData[dX][dY].m_cDynamicObjectFrame;
                    cDynamicObjectData1 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData1;
                    cDynamicObjectData2 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData2;
                    cDynamicObjectData3 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData3;
                    cDynamicObjectData4 = m_pMapData->m_pData[dX][dY].m_cDynamicObjectData4;

                    bRet = true;
                }
                // +++

                if ((bRet == true) && (sItemSprite != 0))
                {
                    if (cItemColor == 0)
                        m_pSprite[DEF_SPRID_ITEMGROUND_PIVOTPOINT + sItemSprite]->put_sprite_fast(ix, iy, sItemSpriteFrame, dwTime);
                    else
                    {
                        switch (sItemSprite)
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 15: m_pSprite[DEF_SPRID_ITEMGROUND_PIVOTPOINT + sItemSprite]->put_sprite_color(ix, iy, sItemSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime); break;
                            default: m_pSprite[DEF_SPRID_ITEMGROUND_PIVOTPOINT + sItemSprite]->put_sprite_color(ix, iy, sItemSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime); break;
                        }
                    }

                    if ((ix - 13 < msX) && (ix + 13 > msX) && (iy - 13 < msY) && (iy + 13 > msY))
                    {
                        if ((dwTime - dwMCAnimTime) > 200)
                        {
                            dwMCAnimTime = dwTime;
                            if (sMCAnimFrame == 1)
                                sMCAnimFrame = 2;
                            else sMCAnimFrame = 1;
                        }
                        m_stMCursor.sCursorFrame = sMCAnimFrame;
                    }
                }

                if ((bRet == true) && (_tmp_wObjectID != 0))
                {
                    bContact = DrawObject_OnDead(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                }
                if ((bContact == true) && (msY <= get_virtual_height() - 49)) // height of icon panel
                {
                    m_sMCX = indexX;
                    m_sMCY = indexY;
                    sFocusX = ix;
                    sFocusY = iy;
                    sFocusOwnerType = _tmp_sOwnerType;
                    cFocusAction = DEF_OBJECTDEAD;
                    wFocusObjectID = _tmp_wObjectID;
                    cFocusFrame = _tmp_cFrame;
                    cFocusDir = _tmp_cDir;
                    sFocusAppr1 = _tmp_sAppr1;
                    sFocusAppr2 = _tmp_sAppr2;
                    sFocusAppr3 = _tmp_sAppr3;
                    sFocusAppr4 = _tmp_sAppr4;
                    iFocusApprColor = _tmp_iApprColor; // v1.4
                    iFocusStatus = _tmp_iStatus;
                    memset(cFocusName, 0, sizeof(cFocusName));
                    strcpy(cFocusName, _tmp_cName);
                    memset(m_cMCName, 0, sizeof(m_cMCName));
                    strcpy(m_cMCName, _tmp_cName);

                    sFocus_dX = _tmp_dX; // v2.171
                    sFocus_dY = _tmp_dY; // v2.171

                    bContact = false;
                }

                _tmp_wObjectID = _tmp_sOwnerType = _tmp_sAppr1 = _tmp_sAppr2 = _tmp_sAppr3 = _tmp_sAppr4 = _tmp_iStatus = 0;
                _tmp_cFrame = _tmp_cDir = 0;
                _tmp_iEffectType = _tmp_iEffectFrame = _tmp_iApprColor = _tmp_iChatIndex = 0;
                memset(_tmp_cName, 0, sizeof(_tmp_cName));
                if ((indexX < 0) || (indexX > MAPDATASIZEX) ||
                    (indexY < 0) || (indexY > MAPDATASIZEY))
                {

                    sItemSprite = 0;
                    bRet = false;
                }
                else
                {
                    _tmp_dX = dX = indexX;
                    _tmp_dY = dY = indexY;
                    _tmp_wObjectID = m_pMapData->m_pData[dX][dY].m_wObjectID;
                    _tmp_sOwnerType = m_pMapData->m_pData[dX][dY].m_sOwnerType;
                    _tmp_cAction = m_pMapData->m_pData[dX][dY].m_cOwnerAction;
                    _tmp_iStatus = m_pMapData->m_pData[dX][dY].m_iStatus;
                    _tmp_cDir = m_pMapData->m_pData[dX][dY].m_cDir;
                    _tmp_sAppr1 = m_pMapData->m_pData[dX][dY].m_sAppr1;
                    _tmp_sAppr2 = m_pMapData->m_pData[dX][dY].m_sAppr2;
                    _tmp_sAppr3 = m_pMapData->m_pData[dX][dY].m_sAppr3;
                    _tmp_sAppr4 = m_pMapData->m_pData[dX][dY].m_sAppr4;
                    _tmp_iApprColor = m_pMapData->m_pData[dX][dY].m_iApprColor; // v1.4
                    _tmp_cFrame = m_pMapData->m_pData[dX][dY].m_cOwnerFrame;
                    _tmp_iChatIndex = m_pMapData->m_pData[dX][dY].m_iChatMsg;
                    _tmp_iEffectType = m_pMapData->m_pData[dX][dY].m_iEffectType;
                    _tmp_iEffectFrame = m_pMapData->m_pData[dX][dY].m_iEffectFrame;

                    strcpy(_tmp_cName, m_pMapData->m_pData[dX][dY].m_cOwnerName);
                    bRet = true;

                    if (m_iIlusionOwnerH != 0)
                    {
                        if ((strcmp(_tmp_cName, m_cPlayerName) != 0) && (_tmp_sOwnerType < 10))
                        {
                            _tmp_sOwnerType = m_cIlusionOwnerType;
                            _tmp_iStatus = m_iStatus_IE;
                            _tmp_sAppr1 = m_sAppr1_IE;
                            _tmp_sAppr2 = m_sAppr2_IE;
                            _tmp_sAppr3 = m_sAppr3_IE;
                            _tmp_sAppr4 = m_sAppr4_IE;
                            _tmp_iApprColor = m_iApprColor_IE;
                        }
                    }
                }

                if ((bRet == true) && (strlen(_tmp_cName) > 0))
                {
                    _tmp_dx = 0;
                    _tmp_dy = 0;

                    switch (_tmp_cAction)
                    {
                        case DEF_OBJECTSTOP:
                            bContact = DrawObject_OnStop(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;

                        case DEF_OBJECTMOVE:
                            bContact = DrawObject_OnMove(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;

                        case DEF_OBJECTDAMAGEMOVE:
                            bContact = DrawObject_OnDamageMove(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;

                        case DEF_OBJECTRUN:
                            bContact = DrawObject_OnRun(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;

                        case DEF_OBJECTATTACK:
                            bContact = DrawObject_OnAttack(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;

                        case DEF_OBJECTATTACKMOVE:
                            bContact = DrawObject_OnAttackMove(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;

                        case DEF_OBJECTMAGIC:
                            bContact = DrawObject_OnMagic(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;

                        case DEF_OBJECTGETITEM:
                            bContact = DrawObject_OnGetItem(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;

                        case DEF_OBJECTDAMAGE:
                            bContact = DrawObject_OnDamage(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;

                        case DEF_OBJECTDYING:
                            bContact = DrawObject_OnDying(indexX, indexY, ix, iy, false, dwTime, msX, msY);
                            break;
                    }
                    if ((bContact == true) && (msY <= get_virtual_height() - 49))
                    {
                        m_sMCX = indexX;
                        m_sMCY = indexY;
                        sFocusX = ix;
                        sFocusY = iy;
                        wFocusObjectID = _tmp_wObjectID;
                        sFocusOwnerType = _tmp_sOwnerType;
                        cFocusAction = _tmp_cAction;
                        cFocusFrame = _tmp_cFrame;
                        cFocusDir = _tmp_cDir;
                        sFocusAppr1 = _tmp_sAppr1;
                        sFocusAppr2 = _tmp_sAppr2;
                        sFocusAppr3 = _tmp_sAppr3;
                        sFocusAppr4 = _tmp_sAppr4;
                        iFocusApprColor = _tmp_iApprColor;
                        iFocusStatus = _tmp_iStatus;
                        memset(cFocusName, 0, sizeof(cFocusName));
                        strcpy(cFocusName, _tmp_cName);
                        memset(m_cMCName, 0, sizeof(m_cMCName));
                        strcpy(m_cMCName, _tmp_cName);
                        sFocus_dX = _tmp_dX;
                        sFocus_dY = _tmp_dY;
                        bContact = false;
                    }

                    if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0)
                    {
                        if (m_bIsObserverMode == false)
                        {
                            m_sViewDstX = (indexX * 32) - ((get_virtual_width() / 2));
                            m_sViewDstY = (indexY * 32) - ((get_virtual_height() / 2) - 16);
                        }
                        SetRect(&m_rcPlayerRect, m_rcBodyRect.left, m_rcBodyRect.top, m_rcBodyRect.right, m_rcBodyRect.bottom);

                        bIsPlayerDrawed = true;
                    }
                }
            }

            if ((indexX >= 0) && (indexX < MAPDATASIZEX) &&
                (indexY >= 0) && (indexY < MAPDATASIZEY))
            {
                sObjSpr = m_pMapData->m_tile[indexX][indexY].m_sObjectSprite;
                sObjSprFrame = m_pMapData->m_tile[indexX][indexY].m_sObjectSpriteFrame;
            }

            if (sObjSpr != 0)
            {
                if ((sObjSpr < 100) || (sObjSpr >= 200))
                {
                    switch (sObjSpr)
                    {
                        case 200:
                        case 223:
                            m_pTileSpr[sObjSpr]->put_shadow_sprite(ix - 16, iy - 16, sObjSprFrame, dwTime);
                            break;

                        case 224:
                            switch (sObjSprFrame)
                            {
                                case 24:
                                case 34:
                                case 35:
                                case 36:
                                case 37:
                                case 38:
                                    break;

                                default:
                                    m_pTileSpr[sObjSpr]->put_shadow_sprite(ix - 16, iy - 16, sObjSprFrame, dwTime);
                                    break;
                            }
                    }
                    if (m_cDetailLevel == 0)
                    {
                        if ((sObjSpr != 6) && (sObjSpr != 9))
                            m_pTileSpr[sObjSpr]->put_sprite_fast(ix - 16, iy - 16, sObjSprFrame, dwTime);
                    }
                    else
                    {
                        m_pTileSpr[sObjSpr]->put_sprite_fast(ix - 16, iy - 16, sObjSprFrame, dwTime);
                    }

                    switch (sObjSpr)
                    {
                        case 223:
                            if (sObjSprFrame == 4)
                            {
                                if (G_cSpriteAlphaDegree == 2)
                                {
                                    int iDvalue1 = -1 * (rand() % 5);
                                    int iDvalue2 = -1 * (rand() % 5);
                                    int iDvalue3 = -1 * (rand() % 5);
                                    m_pEffectSpr[0]->put_trans_sprite_color(ix + 2 - 17, iy - 147 - 15, 1, iDvalue1, iDvalue1, iDvalue1, dwTime);
                                    m_pEffectSpr[0]->put_trans_sprite_color(ix + 16 - 17, iy - 96 - 15, 1, iDvalue2, iDvalue2, iDvalue2, dwTime);
                                    m_pEffectSpr[0]->put_trans_sprite_color(ix - 19 - 17, iy - 126 - 15, 1, iDvalue3, iDvalue3, iDvalue3, dwTime);
                                }
                            }
                            break;

                        case 370:
                            if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 9) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix - 16 + 30, m_sViewPointY + iy - 16 - 334, 0, 0, 0, 0);
                            if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 11) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix - 16 + 17, m_sViewPointY + iy - 16 - 300, 0, 0, 0, 0);
                            break;

                        case 374:
                            if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 2) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix - 7, m_sViewPointY + iy - 122, 0, 0, 0, 0);
                            if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 6) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix - 14, m_sViewPointY + iy - 321, 0, 0, 0, 0);
                            if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 7) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix + 7, m_sViewPointY + iy - 356, 0, 0, 0, 0);
                            break;

                        case 376:
                            if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 12) && (G_cSpriteAlphaDegree == 2))
                            {
                                bAddNewEffect(65, m_sViewPointX + ix - 16, m_sViewPointY + iy - 346, 0, 0, 0, 0);
                                bAddNewEffect(65, m_sViewPointX + ix + 11, m_sViewPointY + iy - 308, 0, 0, 0, 0);
                            }
                            break;

                        case 378:
                            if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 11) && (G_cSpriteAlphaDegree == 2)) bAddNewEffect(65, m_sViewPointX + ix, m_sViewPointY + iy - 91, 0, 0, 0, 0);
                            break;

                        case 382:
                            if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 9) && (G_cSpriteAlphaDegree == 2))
                            {
                                bAddNewEffect(65, m_sViewPointX + ix + 73, m_sViewPointY + iy - 264, 0, 0, 0, 0);
                                bAddNewEffect(65, m_sViewPointX + ix + 23, m_sViewPointY + iy - 228, 0, 0, 0, 0);
                            }
                            break;

                        case 429:
                            if (((dwTime - m_dwEnvEffectTime) > 400) && (sObjSprFrame == 2)) bAddNewEffect(65, m_sViewPointX + ix - 15, m_sViewPointY + iy - (get_virtual_height() / 2), 0, 0, 0, 0);
                            break;
                    }
                }
                else
                {
                    m_pTileSpr[sObjSpr]->get_sprite_rect(ix - 16, iy - 16, sObjSprFrame);
                    if (m_cDetailLevel == 0)
                    {
                        if (sObjSpr < 100 + 11) m_pTileSpr[100 + 4]->put_sprite_fast(ix - 16, iy - 16, sObjSprFrame, dwTime);
                        else if (sObjSpr < 100 + 23) m_pTileSpr[100 + 9]->put_sprite_fast(ix - 16, iy - 16, sObjSprFrame, dwTime);
                        else if (sObjSpr < 100 + 32) m_pTileSpr[100 + 23]->put_sprite_fast(ix - 16, iy - 16, sObjSprFrame, dwTime);
                        else m_pTileSpr[100 + 32]->put_sprite_fast(ix - 16, iy - 16, sObjSprFrame, dwTime);
                    }
                    else
                    {
                        if ((bIsPlayerDrawed == true) && (m_pTileSpr[sObjSpr]->m_rcBound.top <= m_rcPlayerRect.top) && (m_pTileSpr[sObjSpr]->m_rcBound.bottom >= m_rcPlayerRect.bottom) &&
                            (m_cDetailLevel >= 2) && (m_pTileSpr[sObjSpr]->m_rcBound.left <= m_rcPlayerRect.left) && (m_pTileSpr[sObjSpr]->m_rcBound.right >= m_rcPlayerRect.right))
                        {
                            m_pTileSpr[sObjSpr + 50]->put_fade_sprite(ix, iy, sObjSprFrame, dwTime);
                            m_pTileSpr[sObjSpr]->put_trans_sprite2(ix - 16, iy - 16, sObjSprFrame, dwTime);
                        }
                        else
                        {
                            m_pTileSpr[sObjSpr + 50]->put_sprite_fast(ix, iy, sObjSprFrame, dwTime);
                            m_pTileSpr[sObjSpr]->put_sprite_fast(ix - 16, iy - 16, sObjSprFrame, dwTime);
                        }

#ifdef DEF_XMAS
                        static int ix1[100];
                        static int iy2[100];
                        static int iXmasTreeBulbDelay = 76;
                        int idelay = 75;

                        if (G_cSpriteAlphaDegree == 2)
                        {
                            if (iXmasTreeBulbDelay < 0 || iXmasTreeBulbDelay > idelay + 1) iXmasTreeBulbDelay = 0;

                            if (iXmasTreeBulbDelay > idelay)
                            {
                                for (int i = 0; i < 100; i++)
                                {
                                    ix1[i] = 1 * (rand() % 400) - 200;
                                    iy2[i] = -1 * (rand() % 300);
                                }
                                iXmasTreeBulbDelay = 0;
                            }
                            else iXmasTreeBulbDelay++;
                            for (int j = 0; j < 100; j++)
                            {
                                if (m_pTileSpr[sObjSpr]->check_collison(ix - 16, iy - 16, sObjSprFrame, ix + ix1[j], iy + iy2[j]))
                                {
                                    m_pEffectSpr[66 + (j % 6)]->put_trans_sprite(ix + ix1[j], iy + iy2[j], (iXmasTreeBulbDelay >> 2), dwTime);
                                }
                            }

                        }
#endif
                    }
                }
            }
            if ((bRet == true) && (sDynamicObject != 0))
            {
                switch (sDynamicObject)
                {
                    case DEF_DYNAMICOBJECT_PCLOUD_BEGIN:
                        if (sDynamicObjectFrame >= 0)
                            m_pEffectSpr[23]->put_trans_sprite50_no_color_key(ix + (rand() % 2), iy + (rand() % 2), sDynamicObjectFrame, dwTime);
                        break;

                    case DEF_DYNAMICOBJECT_PCLOUD_LOOP:
                        m_pEffectSpr[23]->put_trans_sprite50_no_color_key(ix + (rand() % 2), iy + (rand() % 2), sDynamicObjectFrame + 8, dwTime);
                        break;

                    case DEF_DYNAMICOBJECT_PCLOUD_END:
                        m_pEffectSpr[23]->put_trans_sprite50_no_color_key(ix + (rand() % 2), iy + (rand() % 2), sDynamicObjectFrame + 16, dwTime);
                        break;

                    case DEF_DYNAMICOBJECT_ICESTORM:
                        iDvalue = (rand() % 5) * (-1);
                        m_pEffectSpr[0]->put_trans_sprite_color(ix, iy, 1, iDvalue, iDvalue, iDvalue, dwTime);
                        m_pEffectSpr[13]->put_trans_sprite70_no_color_key(ix, iy, sDynamicObjectFrame, dwTime);
                        break;

                    case DEF_DYNAMICOBJECT_FIRE:
                    case DEF_DYNAMICOBJECT_FIRE3://Change firebow
                        //iDvalue = (rand() % 5)*(-1);
                        //m_pEffectSpr[0]->put_trans_sprite_color(ix, iy, 1, iDvalue, iDvalue, iDvalue, dwTime);
                        switch (rand() % 3)
                        {
                            case 0: m_pEffectSpr[0]->put_trans_sprite25_no_color_key(ix, iy, 1, dwTime); break;
                            case 1: m_pEffectSpr[0]->put_trans_sprite50_no_color_key(ix, iy, 1, dwTime); break;
                            case 2: m_pEffectSpr[0]->put_trans_sprite70_no_color_key(ix, iy, 1, dwTime); break;
                        }
                        m_pEffectSpr[9]->put_trans_sprite70_no_color_key(ix, iy, sDynamicObjectFrame / 3, dwTime);
                        break;

                    case DEF_DYNAMICOBJECT_FIRE2:
                        switch (rand() % 3)
                        {
                            case 0: m_pEffectSpr[0]->put_trans_sprite25_no_color_key(ix, iy, 1, dwTime); break;
                            case 1: m_pEffectSpr[0]->put_trans_sprite50_no_color_key(ix, iy, 1, dwTime); break;
                            case 2: m_pEffectSpr[0]->put_trans_sprite70_no_color_key(ix, iy, 1, dwTime); break;
                        }
                        break;

                    case DEF_DYNAMICOBJECT_FISH:
                    {
                        char cTmpDOdir, cTmpDOframe;
                        cTmpDOdir = m_Misc.cCalcDirection(cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData1 + cDynamicObjectData3, cDynamicObjectData2 + cDynamicObjectData4);
                        cTmpDOframe = ((cTmpDOdir - 1) * 4) + (rand() % 4);
                        m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 0]->put_trans_sprite2(ix + cDynamicObjectData1, iy + cDynamicObjectData2, cTmpDOframe, dwTime);
                    }
                    break;

                    case DEF_DYNAMICOBJECT_MINERAL1:
                        m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->put_shadow_sprite(ix, iy, 0, dwTime);
                        m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->put_sprite_fast(ix, iy, 0, dwTime);
                        if ((m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.top != -1) &&
                            (m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.top < msY) &&
                            (m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.bottom > msY) &&
                            (m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.left < msX) &&
                            (m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.right > msX))
                        {
                            m_sMCX = indexX;
                            m_sMCY = indexY;
                            iFocusStatus = 0;
                            memset(cFocusName, 0, sizeof(cFocusName));
                            memset(m_cMCName, 0, sizeof(m_cMCName));
                        }
                        break;

                    case DEF_DYNAMICOBJECT_MINERAL2:
                        m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->put_shadow_sprite(ix, iy, 1, dwTime);
                        m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->put_sprite_fast(ix, iy, 1, dwTime);
                        if ((m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.top != -1) &&
                            (m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.top < msY) &&
                            (m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.bottom > msY) &&
                            (m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.left < msX) &&
                            (m_pSprite[DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT + 1]->m_rcBound.right > msX))
                        {
                            m_sMCX = indexX;
                            m_sMCY = indexY;
                            iFocusStatus = 0;
                            memset(cFocusName, 0, sizeof(cFocusName));
                            memset(m_cMCName, 0, sizeof(m_cMCName));
                        }
                        break;

                    case DEF_DYNAMICOBJECT_SPIKE:
                        m_pEffectSpr[17]->put_trans_sprite70_no_color_key(ix, iy, sDynamicObjectFrame, dwTime);
                        break;
                }
            }
            indexX++;
        }
        indexY++;
    }
    if ((dwTime - m_dwEnvEffectTime) > 400) m_dwEnvEffectTime = dwTime;


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DEF_HACKCLIENT
    if ((m_bShiftPressed) || (m_stConfigList.bShowNames))
    {
        indexY = sDivY + sPivotY - 7;
        for (iy = -sModY - 224; iy <= /*427*/get_virtual_height() + 352; iy += 32)
        {
            indexX = sDivX + sPivotX - 4;
            for (ix = -sModX - 128; ix <= get_virtual_width() + 128; ix += 32)
            {
                // v1.4311
                sDynamicObject = 0;
                bRet = false;

                //if ((ix >= -sModX) && (ix <= get_virtual_width()+16) && (iy >= -sModY) && (iy <= 427+32+16)) {
                if ((ix >= -sModX) && (ix <= get_virtual_width() + 16) && (iy >= -sModY) && (iy <= get_virtual_height() + 32 + 16))
                {

                    _tmp_wObjectID = _tmp_sOwnerType = _tmp_sAppr1 = _tmp_sAppr2 = _tmp_sAppr3 = _tmp_sAppr4 = _tmp_iStatus = 0;
                    _tmp_cDir = _tmp_cFrame = 0;
                    _tmp_iEffectType = _tmp_iEffectFrame = _tmp_iChatIndex = 0;

                    memset(_tmp_cName, 0, sizeof(_tmp_cName));

                    _tmp_wObjectID = _tmp_sOwnerType = _tmp_sAppr1 = _tmp_sAppr2 = _tmp_sAppr3 = _tmp_sAppr4 = _tmp_iStatus = 0;
                    _tmp_cFrame = _tmp_cDir = 0;
                    _tmp_iEffectType = _tmp_iEffectFrame = _tmp_iApprColor = _tmp_iChatIndex = 0;
                    memset(_tmp_cName, 0, sizeof(_tmp_cName));
                    /*				if ((indexX < m_pMapData->m_sPivotX) || (indexX > m_pMapData->m_sPivotX + MAPDATASIZEX) ||
                    (indexY < m_pMapData->m_sPivotY) || (indexY > m_pMapData->m_sPivotY + MAPDATASIZEY)) {

                    sItemSprite = 0;
                    bRet = false;
                    }
                    else {*/
                    _tmp_dX = dX = indexX - m_pMapData->m_sPivotX;
                    _tmp_dY = dY = indexY - m_pMapData->m_sPivotY;
                    _tmp_wObjectID = m_pMapData->m_pData[dX][dY].m_wObjectID;
                    _tmp_sOwnerType = m_pMapData->m_pData[dX][dY].m_sOwnerType;
                    _tmp_cAction = m_pMapData->m_pData[dX][dY].m_cOwnerAction;
                    _tmp_iStatus = m_pMapData->m_pData[dX][dY].m_iStatus;
                    _tmp_cDir = m_pMapData->m_pData[dX][dY].m_cDir;
                    _tmp_sAppr1 = m_pMapData->m_pData[dX][dY].m_sAppr1;
                    _tmp_sAppr2 = m_pMapData->m_pData[dX][dY].m_sAppr2;
                    _tmp_sAppr3 = m_pMapData->m_pData[dX][dY].m_sAppr3;
                    _tmp_sAppr4 = m_pMapData->m_pData[dX][dY].m_sAppr4;
                    _tmp_iApprColor = m_pMapData->m_pData[dX][dY].m_iApprColor;
                    _tmp_cFrame = m_pMapData->m_pData[dX][dY].m_cOwnerFrame;
                    _tmp_iChatIndex = m_pMapData->m_pData[dX][dY].m_iChatMsg;
                    _tmp_iEffectType = m_pMapData->m_pData[dX][dY].m_iEffectType;
                    _tmp_iEffectFrame = m_pMapData->m_pData[dX][dY].m_iEffectFrame;

                    strcpy(_tmp_cName, m_pMapData->m_pData[dX][dY].m_cOwnerName);
                    bRet = true;

                    if (m_iIlusionOwnerH != 0)
                    {
                        if ((strcmp(_tmp_cName, m_cPlayerName) != 0) && (_tmp_sOwnerType < 10))
                        {
                            _tmp_sOwnerType = m_cIlusionOwnerType;
                            _tmp_iStatus = m_iStatus_IE;
                            _tmp_sAppr1 = m_sAppr1_IE;
                            _tmp_sAppr2 = m_sAppr2_IE;
                            _tmp_sAppr3 = m_sAppr3_IE;
                            _tmp_sAppr4 = m_sAppr4_IE;
                            _tmp_iApprColor = m_iApprColor_IE;
                        }
                    }
                    //				}

                    if ((strlen(_tmp_cName) > 0))
                    {
                        _tmp_dx = ix;
                        _tmp_dy = iy;

                        switch (_tmp_cAction)
                        {
                            case DEF_OBJECTSTOP:
                                bContact = DrawObject_OnStop(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;

                            case DEF_OBJECTMOVE:
                                bContact = DrawObject_OnMove(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;

                            case DEF_OBJECTDAMAGEMOVE:
                                bContact = DrawObject_OnDamageMove(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;

                            case DEF_OBJECTRUN:
                                bContact = DrawObject_OnRun(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;

                            case DEF_OBJECTATTACK:
                                bContact = DrawObject_OnAttack(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;

                            case DEF_OBJECTATTACKMOVE:
                                bContact = DrawObject_OnAttackMove(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;

                            case DEF_OBJECTMAGIC:
                                bContact = DrawObject_OnMagic(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;

                            case DEF_OBJECTGETITEM:
                                bContact = DrawObject_OnGetItem(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;

                            case DEF_OBJECTDAMAGE:
                                bContact = DrawObject_OnDamage(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;

                            case DEF_OBJECTDYING:
                                bContact = DrawObject_OnDying(indexX, indexY, ix, iy, true, dwTime, msX, msY);
                                break;
                        }
                    }
                }
                indexX++;
            }
            indexY++;
        }
    }
#endif

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (m_sMCX != 0)
    {

        if (_iGetFOE(iFocusStatus) < 0) m_stMCursor.sCursorFrame = 3;
        else m_stMCursor.sCursorFrame = 6;

        _tmp_wObjectID = wFocusObjectID;
        _tmp_sOwnerType = sFocusOwnerType;
        _tmp_cAction = cFocusAction;
        _tmp_cFrame = cFocusFrame;
        _tmp_cDir = cFocusDir;
        _tmp_sAppr1 = sFocusAppr1;
        _tmp_sAppr2 = sFocusAppr2;
        _tmp_sAppr3 = sFocusAppr3;
        _tmp_sAppr4 = sFocusAppr4;
        _tmp_iApprColor = iFocusApprColor;
        _tmp_iStatus = iFocusStatus;
        strcpy(_tmp_cName, cFocusName);
        _tmp_dX = sFocus_dX;
        _tmp_dY = sFocus_dY;

        if ((_tmp_cAction != DEF_OBJECTDEAD) && (_tmp_cFrame < 0)) return;
        switch (_tmp_cAction)
        {
            case DEF_OBJECTSTOP:
                DrawObject_OnStop(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;
            case DEF_OBJECTMOVE:
                switch (_tmp_sOwnerType)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:

                    case 28:
                    case 29: // Orge.
                    case 30: // Liche
                    case 31: // Orge
                    case 32:
                    case 33:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    case 58:
                    case 59:
                    case 60:
                    case 61:
                    case 62:
                    case 63:
                    case 65:
                    case 66:
                        break;

                    default:
                        _tmp_cFrame = _tmp_cFrame * 2;
                        break;
                }

                DrawObject_OnMove(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;

            case DEF_OBJECTDAMAGEMOVE:
                DrawObject_OnDamageMove(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;

            case DEF_OBJECTRUN:
                DrawObject_OnRun(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;

            case DEF_OBJECTATTACK:
                DrawObject_OnAttack(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;

            case DEF_OBJECTATTACKMOVE:
                DrawObject_OnAttackMove(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;

            case DEF_OBJECTMAGIC:
                DrawObject_OnMagic(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;

            case DEF_OBJECTDAMAGE:
                DrawObject_OnDamage(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;

            case DEF_OBJECTDYING:
                DrawObject_OnDying(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;

            case DEF_OBJECTDEAD:
                DrawObject_OnDead(m_sMCX, m_sMCY, sFocusX, sFocusY, true, dwTime, msX, msY);
                break;
        }
    }
    if (m_bIsGetPointingMode == true)
    {
        if ((m_iPointCommandType >= 100) && (m_iPointCommandType < 200))
        {

            if (m_bCommandAvailable == true)
            {
                if (m_sMCX != 0)
                {
                    if (_iGetFOE(iFocusStatus) < 0)
                        m_stMCursor.sCursorFrame = 5;
                    else m_stMCursor.sCursorFrame = 4;
                }
                else m_stMCursor.sCursorFrame = 4;
            }
            else m_stMCursor.sCursorFrame = 8;
        }
        else if ((m_iPointCommandType >= 0) && (m_iPointCommandType < 50))
        {
            m_stMCursor.sCursorFrame = 10;
        }
    }
}

