//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "game.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <fmt/format.h>
#include "sprite.h"
#include "sprite_id.h"
#include "char_info.h"
#include "mouse_interface.h"
#include "action_id.h"
#include "Item.h"
#include "map_data.h"
#include "lan_eng.h"
#include "msg.h"
#include "tile.h"
#include "tile_spr.h"
#include "magic.h"

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

void CGame::CommandProcessor(short msX, short msY, short indexX, short indexY, char cLB, char cRB)
{
    char cDir, absX, absY, cName[12];
    short sX, sY, sObjectType, tX, tY;
    int iObjectStatus;
    int iRet;
    int64_t dwTime = m_dwCurTime;
    uint16_t wType = 0;
    int i;//, iFOE;
    char cTxt[120];

    char pDstName[21];
    short sDstOwnerType;
    int iDstOwnerStatus;
    bool bGORet;
    m_ssX = msX;
    m_ssY = msY;
    if ((m_bIsObserverCommanded == false) && (m_bIsObserverMode == true))
    {
        if ((msX == 0) && (msY == 0) && (m_sViewDstX > 32 * 21) && (m_sViewDstY > 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 8, 0, 0, 0, 0);
        else if ((msX == get_virtual_width()) && (msY == 0) && (m_sViewDstX < 32 * m_pMapData->m_sMapSizeX - 32 * 21) && (m_sViewDstY > 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 2, 0, 0, 0, 0);
        else if ((msX == get_virtual_width()) && (msY == get_virtual_height()) && (m_sViewDstX < 32 * m_pMapData->m_sMapSizeX - 32 * 21) && (m_sViewDstY < 32 * m_pMapData->m_sMapSizeY - 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 4, 0, 0, 0, 0);
        else if ((msX == 0) && (msY == get_virtual_height())) bSendCommand(MSGID_REQUEST_PANNING, 0, 6, 0, 0, 0, 0);
        else if ((msX == 0) && (m_sViewDstX > 32 * 21)) bSendCommand(MSGID_REQUEST_PANNING, 0, 7, 0, 0, 0, 0);
        else if ((msX == get_virtual_width()) && (m_sViewDstX < 32 * m_pMapData->m_sMapSizeX - 32 * 21)) bSendCommand(MSGID_REQUEST_PANNING, 0, 3, 0, 0, 0, 0);
        else if ((msY == 0) && (m_sViewDstY > 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 1, 0, 0, 0, 0);
        else if ((msY == get_virtual_height()) && (m_sViewDstY < 32 * m_pMapData->m_sMapSizeY - 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, 0, 5, 0, 0, 0, 0);
        else return;

        m_bIsObserverCommanded = true;
        m_cArrowPressed = 0;
        return;
    }
    if (m_bIsObserverMode == true) return;
    if (m_altPressed)
        m_bSuperAttackMode = true;
    else m_bSuperAttackMode = false;
    switch (m_stMCursor.cPrevStatus)
    {
        case DEF_CURSORSTATUS_NULL:
            if (cLB != 0)
            {
                iRet = _iCheckDlgBoxFocus(msX, msY, 1);
                if (iRet == 1)
                {
                    m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_SELECTED;
                    return;
                }
                else if (iRet == 0)
                {
                    m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_PRESSED;
                    if ((msX > get_virtual_width() - 80) && (msX < get_virtual_width() - 20) && (msY > get_virtual_height() - 90) && (msY < get_virtual_height() - 75) && (m_iLU_Point > 0)) //tofix
                    {
                        if (m_bIsDialogEnabled[12] != true)
                        {
                            EnableDialogBox(12, 0, 0, 0);
                            PlaySound('E', 14, 5);
                        }
                        m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                        return;
                    }
                }
                else if (iRet == -1)
                {
                    return;
                }
            }
            else
                if (cRB != 0)
                {
                    iRet = _iCheckDlgBoxFocus(msX, msY, 2);
                    if (iRet == 1) return;
                }
            break;
        case DEF_CURSORSTATUS_PRESSED:
            if (cLB == 0)
            {
                m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
            }
            break;
        case DEF_CURSORSTATUS_SELECTED:
            if (cLB == 0)
            {

                if (((dwTime - m_stMCursor.dwSelectClickTime) < DEF_DOUBLECLICKTIME) &&
                    (msX == m_stMCursor.sClickX) && (msY == m_stMCursor.sClickY))
                {
                    // Double Click 
                    m_stMCursor.dwSelectClickTime = m_stMCursor.dwSelectClickTime;
                    _bCheckDlgBoxDoubleClick(msX, msY);
                }
                else
                {
                    // Click 
                    _bCheckDlgBoxClick(msX, msY);
                    m_stMCursor.sClickX = msX;
                    m_stMCursor.sClickY = msY;
                }

                m_stMCursor.dwSelectClickTime = dwTime;
                m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                if (m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_ITEM)
                {
                    _bCheckDraggingItemRelease(msX, msY);

                    m_stMCursor.cSelectedObjectType = 0;
                    m_stMCursor.sSelectedObjectID = 0;
                }
                return;
            }
            if (cLB != 0)
            {
                if ((m_pMapData->bIsTeleportLoc(m_sPlayerX, m_sPlayerY) == true) && (m_cCommandCount == 0)) goto CP_SKIPMOUSEBUTTONSTATUS;

                if ((m_stMCursor.sPrevX != msX) || (m_stMCursor.sPrevY != msY))
                {
                    m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_DRAGGING;
                    m_stMCursor.sPrevX = msX;
                    m_stMCursor.sPrevY = msY;

                    //Change movable icon panel
                    if ((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX) &&
                        (((m_stMCursor.sSelectedObjectID == 30) && (m_stDialogBoxInfo[30].sV14 == true)) || (m_stMCursor.sSelectedObjectID == 29)))
                    {
                        m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                    }

                    if ((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX) &&
                        (m_stMCursor.sSelectedObjectID == 7) && (m_stDialogBoxInfo[7].cMode == 1))
                    {
                        EndInputString();
                        m_stDialogBoxInfo[7].cMode = 20;
                    }

                    //Change Admin Panel
                    if ((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX) &&
                        (m_stMCursor.sSelectedObjectID == 71) && (m_stDialogBoxInfo[71].sV1 == 1))
                    {
                        EndInputString();
                        m_stDialogBoxInfo[71].sV1 = 20;
                    }


                    if ((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX) &&
                        (m_stMCursor.sSelectedObjectID == 17) && (m_stDialogBoxInfo[17].cMode == 1))
                    {
                        EndInputString();
                        m_stDialogBoxInfo[17].cMode = 20;
                    }
                    return;
                }
                if ((m_cCommand == DEF_OBJECTMOVE) || (m_cCommand == DEF_OBJECTRUN)) goto MOTION_COMMAND_PROCESS;
                return;
            }
            break;
        case DEF_CURSORSTATUS_DRAGGING:
            if (cLB != 0)
            {

                if ((m_pMapData->bIsTeleportLoc(m_sPlayerX, m_sPlayerY) == true) && (m_cCommandCount == 0)) goto CP_SKIPMOUSEBUTTONSTATUS;

                if (m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX)
                {
                    m_stDialogBoxInfo[m_stMCursor.sSelectedObjectID].sX = msX - m_stMCursor.sDistX;
                    m_stDialogBoxInfo[m_stMCursor.sSelectedObjectID].sY = msY - m_stMCursor.sDistY;
                }
                m_stMCursor.sPrevX = msX;
                m_stMCursor.sPrevY = msY;

                if ((m_cCommand == DEF_OBJECTMOVE) || (m_cCommand == DEF_OBJECTRUN)) goto MOTION_COMMAND_PROCESS;
                return;
            }
            if (cLB == 0)
            {
                switch (m_stMCursor.cSelectedObjectType)
                {
                    case DEF_SELECTEDOBJTYPE_DLGBOX:
                        if ((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX) &&
                            (m_stMCursor.sSelectedObjectID == 7) && (m_stDialogBoxInfo[7].cMode == 20))
                        {
                            sX = m_stDialogBoxInfo[7].sX;
                            sY = m_stDialogBoxInfo[7].sY;
                            StartInputString(sX + 75, sY + 140, 21, m_cGuildName);
                            m_stDialogBoxInfo[7].cMode = 1;
                        }

#ifdef DEF_ADMINCLIENT
                        if ((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX) &&
                            (m_stMCursor.sSelectedObjectID == 71) && (m_stDialogBoxInfo[71].sV1 == 20))
                        {
                            //Change Admin Panel
                            sX = m_stDialogBoxInfo[71].sX;
                            sY = m_stDialogBoxInfo[71].sY;
                            StartInputString(sX + 105, sY + 65, 10, m_cRefreshRate);
                            m_bInputStatus = false;
                            m_stDialogBoxInfo[7].sV1 = 1;
                        }
#endif

                        if ((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX) &&
                            (m_stMCursor.sSelectedObjectID == 17) && (m_stDialogBoxInfo[17].cMode == 20))
                        {
                            sX = m_stDialogBoxInfo[17].sX;
                            sY = m_stDialogBoxInfo[17].sY;
                            StartInputString(sX + 40, sY + 57, 11, m_cAmountString);
                            m_stDialogBoxInfo[17].cMode = 1;
                        }

                        if (m_stMCursor.sSelectedObjectID == 9)
                        {
                            if (msX < get_virtual_width() / 2) m_stDialogBoxInfo[9].sX = 0;
                            else m_stDialogBoxInfo[9].sX = get_virtual_width() - m_stDialogBoxInfo[9].sSizeX;
                            if (msY < (get_virtual_height() - 53) / 2) m_stDialogBoxInfo[9].sY = 0;
                            else m_stDialogBoxInfo[9].sY = (get_virtual_height()) - m_stDialogBoxInfo[9].sSizeY - 53;
                        }

                        m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                        m_stMCursor.cSelectedObjectType = 0;
                        m_stMCursor.sSelectedObjectID = 0;
                        break;

                    case DEF_SELECTEDOBJTYPE_ITEM:
                        _bCheckDraggingItemRelease(msX, msY);
                        m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                        m_stMCursor.cSelectedObjectType = 0;
                        m_stMCursor.sSelectedObjectID = 0;
                        break;

                    default:
                        m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                        m_stMCursor.cSelectedObjectType = 0;
                        m_stMCursor.sSelectedObjectID = 0;
                        break;
                }
                return;
            }
            break;
    }

    CP_SKIPMOUSEBUTTONSTATUS:;
    if (m_bCommandAvailable == false) return;
#ifdef DEF_HACKCLIENT
    if ((dwTime - m_dwCommandTime) < 0)
    {
        return;
    }
#else
    if ((dwTime - m_dwCommandTime) < 100)
    {
        return;
    }
#endif

    if (m_iHP <= 0) return;
    if (m_sDamageMove != 0)
    {
#ifndef DEF_HACKCLIENT
        m_cCommand = DEF_OBJECTDAMAGEMOVE;
        goto MOTION_COMMAND_PROCESS;
#endif
    }
    if ((m_pMapData->bIsTeleportLoc(m_sPlayerX, m_sPlayerY) == true) && (m_cCommandCount == 0))
        RequestTeleportAndWaitData();
    if (cLB != 0)
    {
        if (m_bIsGetPointingMode == true)
        {
            if ((m_sMCX != 0) || (m_sMCY != 0))
                PointCommandHandler(m_sMCX, m_sMCY);
            else PointCommandHandler(indexX, indexY);

#ifdef DEF_HACKCLIENT
            if (m_stConfigList.bFastCast == true)
            {
                m_bCommandAvailable = true;
                m_bIsGetPointingMode = true;
            }
            else
            {
#endif
                next_command = dwTime + 300;
                m_bCommandAvailable = false;
                m_bIsGetPointingMode = false;
                ClearCoords();
#ifdef DEF_HACKCLIENT
            }
#endif
            m_dwCommandTime =
#ifdef DEF_HACKCLIENT
                m_stConfigList.dwFastCast =
#endif
                m_dwCurTime;
            return;
        }

        m_pMapData->bGetOwner(m_sMCX, m_sMCY - 1, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID);
        //m_pMapData->m_pData[dX][dY].m_sItemSprite
        if (memcmp(m_cMCName, m_cPlayerName, 10) == 0 && (sObjectType <= 6 || m_pMapData->m_pData[m_sPlayerX][m_sPlayerY].m_sItemSprite != 0))
        {
            //if (memcmp(m_cMCName, m_cPlayerName, 10) == 0 && ( sObjectType <= 6 || m_pMapData->m_pData[15][15].m_sItemSprite != 0 )) {
            if ((m_sPlayerType >= 1) && (m_sPlayerType <= 6)/* && ((m_sPlayerAppr2 & 0xF000) == 0)*/)
            {
                m_cCommand = DEF_OBJECTGETITEM;
                m_sCommX = m_sPlayerX;
                m_sCommY = m_sPlayerY;
            }
        }
        else
        {
            if (memcmp(m_cMCName, m_cPlayerName, 10) == 0) m_sMCY -= 1;
            if ((m_sMCX != 0) && (m_sMCY != 0))
            {
                if (m_bCtrlPressed == true)
                {

                    m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID);
#ifndef DEF_HACKCLIENT
                    if ((iObjectStatus & 0x10) != 0) return;
#endif
                    if ((sObjectType == 15) || (sObjectType == 20) || (sObjectType == 24)) return;

                    set_mouse_cursor(mouse_cursor::Sword);

                    absX = abs(m_sPlayerX - m_sMCX);
                    absY = abs(m_sPlayerY - m_sMCY);

                    //Change 3.51 monsters having 3x3 area - allows you to attack/dash them properly
                    //0x0101
                    if ((sObjectType == 66) || (sObjectType == 73) || (sObjectType == 81))
                    {
                        absX -= 1;
                        if (absX < 0)
                            absX = 0;
                        absY -= 1;
                        if (absY < 0)
                            absY = 0;
                    }

                    if ((absX <= 1) && (absY <= 1))
                    {
                        wType = _iGetAttackType();
                        m_cCommand = DEF_OBJECTATTACK;
                        m_sCommX = m_sMCX;
                        m_sCommY = m_sMCY;
                    }
                    else
                    {
                        switch (_iGetWeaponSkillType())
                        {
                            case 6:
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                                wType = _iGetAttackType();
                                break;

                            case 5:
                            case 7:
                                if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0)))
                                {
                                    if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                    {

                                        if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
                                        {
                                            m_cCommand = DEF_OBJECTATTACKMOVE;
                                            wType = _iGetAttackType();
                                        }
                                        else
                                        {
                                            m_cCommand = DEF_OBJECTRUN;
                                            GetPlayerTurn();
                                        }
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                    }
                                    else
                                    {
                                        m_cCommand = DEF_OBJECTMOVE;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        GetPlayerTurn();
                                    }
                                }
                                else
                                {
                                    if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                        (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                        m_cCommand = DEF_OBJECTRUN;
                                    else m_cCommand = DEF_OBJECTMOVE;
                                    m_sCommX = m_sMCX;
                                    m_sCommY = m_sMCY;
                                    GetPlayerTurn();
                                }
                                break;

                            case 8:
                                if ((absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                {
                                    m_cCommand = DEF_OBJECTATTACK;
                                    m_sCommX = m_sMCX;
                                    m_sCommY = m_sMCY;
                                    wType = _iGetAttackType();
                                }
                                else if ((absX <= 5) && (absY <= 5) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
                                    && (_iGetAttackType() == 30))  // Crit with Storm Bringer
                                {
                                    wType = _iGetAttackType();
                                    m_cCommand = DEF_OBJECTATTACK;
                                    m_sCommX = m_sMCX;
                                    m_sCommY = m_sMCY;
                                }
                                else if ((absX <= 3) && (absY <= 3) && (_iGetAttackType() == 5))  // Normal hit with Storm Bringer
                                {
                                    wType = _iGetAttackType();
                                    m_cCommand = DEF_OBJECTATTACK;
                                    m_sCommX = m_sMCX;
                                    m_sCommY = m_sMCY;
                                }
                                else
                                {
                                    // No Storm Bringer dash
                                    if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0))
                                        && (_iGetAttackType() != 5))
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                        {

                                            if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
                                            {
                                                m_cCommand = DEF_OBJECTATTACKMOVE;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                m_cCommand = DEF_OBJECTRUN;
                                                GetPlayerTurn();
                                            }
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                        }
                                        else
                                        {
                                            m_cCommand = DEF_OBJECTMOVE;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            GetPlayerTurn();
                                        }
                                    }
                                    else
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                            (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                            m_cCommand = DEF_OBJECTRUN;
                                        else m_cCommand = DEF_OBJECTMOVE;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        GetPlayerTurn();
                                    }
                                }
                                break;

                            case 9:
                                if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                {
                                    m_cCommand = DEF_OBJECTATTACK;
                                    m_sCommX = m_sMCX;
                                    m_sCommY = m_sMCY;
                                    wType = _iGetAttackType();
                                }
                                else
                                {
                                    if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0)))
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                        {

                                            if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
                                            {
                                                m_cCommand = DEF_OBJECTATTACKMOVE;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                m_cCommand = DEF_OBJECTRUN;
                                                GetPlayerTurn();
                                            }
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                        }
                                        else
                                        {
                                            m_cCommand = DEF_OBJECTMOVE;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            GetPlayerTurn();
                                        }
                                    }
                                    else
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                            (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                            m_cCommand = DEF_OBJECTRUN;
                                        else m_cCommand = DEF_OBJECTMOVE;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        GetPlayerTurn();
                                    }
                                }
                                break;

                            case 10:
                                if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                {
                                    m_cCommand = DEF_OBJECTATTACK;
                                    m_sCommX = m_sMCX;
                                    m_sCommY = m_sMCY;
                                    wType = _iGetAttackType();
                                }
                                else
                                {
                                    if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0)))
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                        {

                                            if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
                                            {
                                                m_cCommand = DEF_OBJECTATTACKMOVE;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                m_cCommand = DEF_OBJECTRUN;
                                                GetPlayerTurn();
                                            }
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                        }
                                        else
                                        {
                                            m_cCommand = DEF_OBJECTMOVE;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            GetPlayerTurn();
                                        }
                                    }
                                    else
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                            (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                            m_cCommand = DEF_OBJECTRUN;
                                        else m_cCommand = DEF_OBJECTMOVE;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        GetPlayerTurn();
                                    }
                                }
                                break;
                            case 14:
                                if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                {
                                    m_cCommand = DEF_OBJECTATTACK;
                                    m_sCommX = m_sMCX;
                                    m_sCommY = m_sMCY;
                                    wType = _iGetAttackType();
                                }
                                else
                                {
                                    if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0)))
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                        {

                                            if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
                                            {
                                                m_cCommand = DEF_OBJECTATTACKMOVE;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                m_cCommand = DEF_OBJECTRUN;
                                                GetPlayerTurn();
                                            }
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                        }
                                        else
                                        {
                                            m_cCommand = DEF_OBJECTMOVE;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            GetPlayerTurn();
                                        }
                                    }
                                    else
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                            (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                            m_cCommand = DEF_OBJECTRUN;
                                        else m_cCommand = DEF_OBJECTMOVE;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        GetPlayerTurn();
                                    }
                                }
                                break;
                            case 21:
                                if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                {
                                    m_cCommand = DEF_OBJECTATTACK;
                                    m_sCommX = m_sMCX;
                                    m_sCommY = m_sMCY;
                                    wType = _iGetAttackType();
                                }
                                else
                                {
                                    if (((absX == 2) && (absY == 2)) || ((absX == 0) && (absY == 2)) || ((absX == 2) && (absY == 0)))
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                        {

                                            if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
                                            {
                                                m_cCommand = DEF_OBJECTATTACKMOVE;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                m_cCommand = DEF_OBJECTRUN;
                                                GetPlayerTurn();
                                            }
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                        }
                                        else
                                        {
                                            m_cCommand = DEF_OBJECTMOVE;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            GetPlayerTurn();
                                        }
                                    }
                                    else
                                    {
                                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                            (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                            m_cCommand = DEF_OBJECTRUN;
                                        else m_cCommand = DEF_OBJECTMOVE;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        GetPlayerTurn();
                                    }
                                }
                                break;
                        }
                    }
                }
                else
                {
                    m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID);

                    if (sObjectType >= 10 || ((sObjectType >= 1) && (sObjectType <= 6)))
                    {
                        switch (sObjectType)
                        {
                            case 15:
                                switch (cName[0])
                                {
                                    case '1':
                                        EnableDialogBox(20, 5, 11, 1);
                                        tX = msX - 117;
                                        tY = msY - 50;
                                        if (tX < 0) tX = 0;
                                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                        if (tY < 0) tY = 0;
                                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                        m_stDialogBoxInfo[20].sX = tX;
                                        m_stDialogBoxInfo[20].sY = tY;
                                        m_stDialogBoxInfo[20].sV3 = 15;
                                        break;
                                }
                                break;

                            case 19:
                                switch (cName[0])
                                {
                                    case '1':
                                        EnableDialogBox(20, 0, 16, 0);
                                        tX = msX - 117;
                                        tY = msY - 50;
                                        if (tX < 0) tX = 0;
                                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                        if (tY < 0) tY = 0;
                                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                        m_stDialogBoxInfo[20].sX = tX;
                                        m_stDialogBoxInfo[20].sY = tY;
                                        m_stDialogBoxInfo[20].sV3 = 19;
                                        break;
                                }
                                break;

                            case 20:
                                switch (cName[0])
                                {
                                    case '1':
                                        EnableDialogBox(20, 0, 14, 0);
                                        tX = msX - 117;
                                        tY = msY - 50;
                                        if (tX < 0) tX = 0;
                                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                        if (tY < 0) tY = 0;
                                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                        m_stDialogBoxInfo[20].sX = tX;
                                        m_stDialogBoxInfo[20].sY = tY;
                                        m_stDialogBoxInfo[20].sV3 = 20;

                                        m_stDialogBoxInfo[39].sV3 = 20;
                                        m_stDialogBoxInfo[39].sV4 = m_wCommObjectID;
                                        m_stDialogBoxInfo[39].sV5 = m_sMCX;
                                        m_stDialogBoxInfo[39].sV6 = m_sMCY;

                                        break;
                                }
                                break;

                            case 24:

                                switch (cName[0])
                                {
                                    case '1':
                                        EnableDialogBox(20, 5, 11, 2);
                                        tX = msX - 117;
                                        tY = msY - 50;
                                        if (tX < 0) tX = 0;
                                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                        if (tY < 0) tY = 0;
                                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                        m_stDialogBoxInfo[20].sX = tX;
                                        m_stDialogBoxInfo[20].sY = tY;
                                        m_stDialogBoxInfo[20].sV3 = 24;

                                        m_stDialogBoxInfo[39].sV3 = 24;
                                        m_stDialogBoxInfo[39].sV4 = m_wCommObjectID;
                                        m_stDialogBoxInfo[39].sV5 = m_sMCX;
                                        m_stDialogBoxInfo[39].sV6 = m_sMCY;
                                        break;
                                }
                                break;

                            case 25:
                                switch (cName[0])
                                {
                                    case '1':
                                        EnableDialogBox(20, 0, 13, 0);
                                        tX = msX - 117;
                                        tY = msY - 50;
                                        if (tX < 0) tX = 0;
                                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                        if (tY < 0) tY = 0;
                                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                        m_stDialogBoxInfo[20].sX = tX;
                                        m_stDialogBoxInfo[20].sY = tY;
                                        m_stDialogBoxInfo[20].sV3 = 25;
                                        break;
                                }
                                break;

                            case 26:
                                switch (cName[0])
                                {
                                    case '1':
                                        EnableDialogBox(20, 0, 7, 0);
                                        tX = msX - 117;
                                        tY = msY - 50;
                                        if (tX < 0) tX = 0;
                                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                        if (tY < 0) tY = 0;
                                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                        m_stDialogBoxInfo[20].sX = tX;
                                        m_stDialogBoxInfo[20].sY = tY;
                                        m_stDialogBoxInfo[20].sV3 = 26;
                                        break;
                                }
                                break;

                            case 21:

                                if ((_iGetFOE(iObjectStatus) == 1) && (!m_bIsCombatMode))
                                {
                                    EnableDialogBox(20, 4, 0, 0);
                                    tX = msX - 117;
                                    tY = msY - 50;
                                    if (tX < 0) tX = 0;
                                    if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                    if (tY < 0) tY = 0;
                                    if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                    m_stDialogBoxInfo[20].sX = tX;
                                    m_stDialogBoxInfo[20].sY = tY;
                                    m_stDialogBoxInfo[20].sV3 = 21;
                                }
                                break;
                            case 67:
                            case 68:
                            case 69:
                                if (!m_bIsCombatMode)
                                {
                                    EnableDialogBox(20, 4, 0, 0);
                                    tX = msX - 117;
                                    tY = msY - 50;
                                    if (tX < 0) tX = 0;
                                    if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                    if (tY < 0) tY = 0;
                                    if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                    m_stDialogBoxInfo[20].sX = tX;
                                    m_stDialogBoxInfo[20].sY = tY;
                                    m_stDialogBoxInfo[20].sV3 = sObjectType;
                                }
                                break;

                            case 32:
                                // Unicorn 
                                if (!m_bIsCombatMode)
                                {
                                    EnableDialogBox(20, 4, 0, 0);
                                    tX = msX - 117;
                                    tY = msY - 50;
                                    if (tX < 0) tX = 0;
                                    if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                    if (tY < 0) tY = 0;
                                    if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                    m_stDialogBoxInfo[20].sX = tX;
                                    m_stDialogBoxInfo[20].sY = tY;
                                    m_stDialogBoxInfo[20].sV3 = 32;
                                }
                                break;


                            case 90: //Change Gail
                                switch (cName[0])
                                {
                                    case '1':
                                        EnableDialogBox(20, 6, 0, 0);
                                        tX = msX - 117;
                                        tY = msY - 50;
                                        if (tX < 0) tX = 0;
                                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                        if (tY < 0) tY = 0;
                                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                        m_stDialogBoxInfo[20].sX = tX;
                                        m_stDialogBoxInfo[20].sY = tY;
                                        m_stDialogBoxInfo[20].sV3 = 90;
                                        break;
                                }
                                break;

                            default:
                                if (_iGetFOE(iObjectStatus) >= 0) break;
                                if ((sObjectType >= 1) && (sObjectType <= 6) && (m_bForceAttack == false)) break;
                                absX = abs(m_sPlayerX - m_sMCX);
                                absY = abs(m_sPlayerY - m_sMCY);

                                //Change 3.51 monsters having 3x3 area - allows you to attack/dash them properly
                                //0x0101
                                if ((sObjectType == 66) || (sObjectType == 73) || (sObjectType == 81))
                                {
                                    absX -= 1;
                                    if (absX < 0)
                                        absX = 0;
                                    absY -= 1;
                                    if (absY < 0)
                                        absY = 0;
                                }

                                if ((absX <= 1) && (absY <= 1))
                                {
                                    wType = _iGetAttackType();
                                    m_cCommand = DEF_OBJECTATTACK;
                                    m_sCommX = m_sMCX;
                                    m_sCommY = m_sMCY;
                                }
                                else
                                {
                                    switch (_iGetWeaponSkillType())
                                    {
                                        case 6:
                                            m_cCommand = DEF_OBJECTATTACK;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            wType = _iGetAttackType();
                                            break;

                                        case 5:
                                        case 7:
                                            if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                                (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                                m_cCommand = DEF_OBJECTRUN;
                                            else m_cCommand = DEF_OBJECTMOVE;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            GetPlayerTurn();
                                            break;

                                        case 8:
                                            if ((absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (_iGetAttackType() != 30))
                                            {
                                                if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                                    m_cCommand = DEF_OBJECTATTACKMOVE;
                                                else m_cCommand = DEF_OBJECTATTACK;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                wType = _iGetAttackType();
                                            }
                                            else if ((absX <= 5) && (absY <= 5) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (_iGetAttackType() == 30)) //Change
                                            {
                                                if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                                    m_cCommand = DEF_OBJECTATTACKMOVE;
                                                else m_cCommand = DEF_OBJECTATTACK;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                wType = _iGetAttackType();
                                            }
                                            else if ((absX <= 3) && (absY <= 3) && (_iGetAttackType() == 5)) // Stormbringer Normal attack
                                            {
                                                m_cCommand = DEF_OBJECTATTACK;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) && (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                                    m_cCommand = DEF_OBJECTRUN;
                                                else m_cCommand = DEF_OBJECTMOVE;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                GetPlayerTurn();
                                            }
                                            break;

                                        case 9:
                                            if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                            {
                                                if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                                    m_cCommand = DEF_OBJECTATTACKMOVE;
                                                else m_cCommand = DEF_OBJECTATTACK;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                                    (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                                    m_cCommand = DEF_OBJECTRUN;
                                                else m_cCommand = DEF_OBJECTMOVE;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                GetPlayerTurn();
                                            }
                                            break;


                                        case 10:
                                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                            {
                                                if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                                    m_cCommand = DEF_OBJECTATTACKMOVE;
                                                else m_cCommand = DEF_OBJECTATTACK;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                                    (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                                    m_cCommand = DEF_OBJECTRUN;
                                                else m_cCommand = DEF_OBJECTMOVE;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                GetPlayerTurn();
                                            }
                                            break;
                                        case 14:
                                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                            {
                                                if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                                    m_cCommand = DEF_OBJECTATTACKMOVE;
                                                else m_cCommand = DEF_OBJECTATTACK;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                                    (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                                    m_cCommand = DEF_OBJECTRUN;
                                                else m_cCommand = DEF_OBJECTMOVE;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                GetPlayerTurn();
                                            }
                                            break;
                                        case 21:
                                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                            {
                                                if ((absX <= 1) && (absY <= 1) && (m_bShiftPressed || m_bRunningMode) && (m_iSP > 0))
                                                    m_cCommand = DEF_OBJECTATTACKMOVE;
                                                else m_cCommand = DEF_OBJECTATTACK;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                wType = _iGetAttackType();
                                            }
                                            else
                                            {
                                                if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                                                    (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                                                    m_cCommand = DEF_OBJECTRUN;
                                                else m_cCommand = DEF_OBJECTMOVE;
                                                m_sCommX = m_sMCX;
                                                m_sCommY = m_sMCY;
                                                GetPlayerTurn();
                                            }
                                            break;
                                    }
                                }
                                break;
                        }
                    }
                    else
                    {
                        if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                            (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                            m_cCommand = DEF_OBJECTRUN;
                        else m_cCommand = DEF_OBJECTMOVE;
                        m_sCommX = m_sMCX;
                        m_sCommY = m_sMCY;
                        GetPlayerTurn();
                    }
                }
            }
            else
            {
                if ((m_bShiftPressed || m_bRunningMode) && (m_iSP > 0) &&
                    (m_sPlayerType >= 1) && (m_sPlayerType <= 6))
                    m_cCommand = DEF_OBJECTRUN;
                else m_cCommand = DEF_OBJECTMOVE;
                m_sCommX = indexX;
                m_sCommY = indexY;
                GetPlayerTurn();
            }
        }
    }
    else if (cRB != 0)
    {
        m_cCommand = DEF_OBJECTSTOP;

        if (m_bIsGetPointingMode == true)
        {
            m_bIsGetPointingMode = false;
            ClearCoords();
            AddEventList(COMMAND_PROCESSOR1, 10);
        }

        if (next_command > dwTime) return;
        if (m_bCommandAvailable == false) return;
        //if (m_cCommandCount >= 6) return;

        if ((m_sMCX != 0) && (m_sMCY != 0))
        {
            absX = abs(m_sPlayerX - m_sMCX);
            absY = abs(m_sPlayerY - m_sMCY);
            if (absX == 0 && absY == 0) return;

            if (m_bCtrlPressed == true)
            {
                m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID);
#ifndef DEF_HACKCLIENT
                if ((iObjectStatus & 0x10) != 0) return;
#endif
                if ((sObjectType == 15) || (sObjectType == 20) || (sObjectType == 24)) return;

                if ((sObjectType == 66) || (sObjectType == 73) || (sObjectType == 81))
                {
                    absX -= 1;
                    if (absX < 0)
                        absX = 0;
                    absY -= 1;
                    if (absY < 0)
                        absY = 0;
                }

                if ((absX <= 1) && (absY <= 1))
                {
                    wType = _iGetAttackType();
                    m_cCommand = DEF_OBJECTATTACK;
                    m_sCommX = m_sMCX;
                    m_sCommY = m_sMCY;
                }
                else
                {
                    switch (_iGetWeaponSkillType())
                    {
                        case 6:
                            m_cCommand = DEF_OBJECTATTACK;
                            m_sCommX = m_sMCX;
                            m_sCommY = m_sMCY;
                            wType = _iGetAttackType();
                            break;

                        case 5:
                        case 7:
                            break;

                        case 8:
                            if ((absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
                                && (_iGetAttackType() != 30))
                            {
                                wType = _iGetAttackType();
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                            }
                            else if ((absX <= 5) && (absY <= 5) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
                                && (_iGetAttackType() == 30))
                            {
                                wType = _iGetAttackType();
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                            }
                            else if ((absX <= 3) && (absY <= 3)
                                && (_iGetAttackType() == 5))
                            {
                                wType = _iGetAttackType();
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                            }
                            break;

                        case 9:
                            if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                            {
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                                wType = _iGetAttackType();
                            }
                            break;

                        case 10:
                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                            {
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                                wType = _iGetAttackType();
                            }
                            break;

                        case 14:
                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                            {
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                                wType = _iGetAttackType();
                            }
                            break;
                        case 21:
                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                            {
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                                wType = _iGetAttackType();
                            }
                            break;
                    }
                }
            }
            else
            {

                absX = abs(m_sPlayerX - m_sMCX);
                absY = abs(m_sPlayerY - m_sMCY);

                m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &iObjectStatus, &m_wCommObjectID);

                if ((sObjectType == 66) || (sObjectType == 73) || (sObjectType == 81))
                {
                    absX -= 1;
                    if (absX < 0)
                        absX = 0;
                    absY -= 1;
                    if (absY < 0)
                        absY = 0;
                }

                if (sObjectType >= 10 || ((sObjectType >= 1) && (sObjectType <= 6)))
                {
                    switch (sObjectType)
                    {
                        case 15:
                        case 19:
                        case 20:
                        case 24:
                        case 25:
                        case 26:
                            break;

                        default:
                            if (_iGetFOE(iObjectStatus) >= 0) break;
                            if ((sObjectType >= 1) && (sObjectType <= 6) && (m_bForceAttack == false)) break;
                            if ((absX <= 1) && (absY <= 1))
                            {
                                wType = _iGetAttackType();
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                            }
                            else
                            {
                                switch (_iGetWeaponSkillType())
                                {
                                    case 6:
                                        m_cCommand = DEF_OBJECTATTACK;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        wType = _iGetAttackType();
                                        break;

                                    case 5:
                                    case 7:
                                        break;

                                    case 8:
                                        if ((absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
                                            && (_iGetAttackType() != 30))
                                        {
                                            wType = _iGetAttackType();
                                            m_cCommand = DEF_OBJECTATTACK;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                        }
                                        else if ((absX <= 5) && (absY <= 5) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true)
                                            && (_iGetAttackType() == 30))
                                        {
                                            wType = _iGetAttackType();
                                            m_cCommand = DEF_OBJECTATTACK;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                        }
                                        else if ((absX <= 3) && (absY <= 3)
                                            && (_iGetAttackType() == 5))
                                        {
                                            wType = _iGetAttackType();
                                            m_cCommand = DEF_OBJECTATTACK;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                        }
                                        break;

                                    case 9:
                                        if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                        {
                                            m_cCommand = DEF_OBJECTATTACK;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            wType = _iGetAttackType();
                                        }
                                        break;

                                    case 10:
                                        if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                        {
                                            m_cCommand = DEF_OBJECTATTACK;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            wType = _iGetAttackType();
                                        }
                                        break;
                                    case 14:
                                        if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                        {
                                            m_cCommand = DEF_OBJECTATTACK;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            wType = _iGetAttackType();
                                        }
                                        break;
                                    case 21:
                                        if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true))
                                        {
                                            m_cCommand = DEF_OBJECTATTACK;
                                            m_sCommX = m_sMCX;
                                            m_sCommY = m_sMCY;
                                            wType = _iGetAttackType();
                                        }
                                        break;

                                }
                            }
                            break;
                    }
                }
            }
        }
        else
        {
            cDir = m_Misc.cGetNextMoveDir(m_sPlayerX, m_sPlayerY, indexX, indexY, m_bIllusionMVT);

            //remove
//			sprintf(G_cTxt, "New DIR: {}", cDir);
//			AddEventList(G_cTxt, 10);

            if (m_iHP <= 0) return;
            if (cDir == 0) return;
            if (m_cPlayerDir == cDir) return;
            ClearSkillUsingStatus();

            m_cPlayerDir = cDir;
            bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTSTOP, m_cPlayerDir, 0, 0, 0, 0);

            m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                m_iPlayerStatus, m_cPlayerName,
                m_cCommand, 0, 0, 0, 0,
                10);
            m_bCommandAvailable = false;
            next_command = dwTime += 100;
            m_dwCommandTime = m_dwCurTime;

            if (game_configs.old_camera == false)
            {
                m_sViewPointX = m_sViewStartX = m_sViewDstX = (m_sPlayerX * 32) - (get_virtual_width() / 2);
                m_sViewPointY = m_sViewStartY = m_sViewDstY = (m_sPlayerY * 32) - ((get_virtual_height() / 2) - 16);
            }
            return;
        }
    }

    MOTION_COMMAND_PROCESS:;

    if (m_cCommand != DEF_OBJECTSTOP)
    {
        if (m_iHP <= 0) return;
        //if (m_cCommandCount == 5) AddEventList(COMMAND_PROCESSOR2, 10, false);
        if (m_bCommandAvailable == false) return;
        //if (m_cCommandCount >= 6) return;

        if ((m_sPlayerType >= 0) && (m_sPlayerType > 6))
        {
            switch (m_cCommand)
            {
                case DEF_OBJECTRUN:
                case DEF_OBJECTMAGIC:
                case DEF_OBJECTGETITEM:
                    m_cCommand = DEF_OBJECTSTOP;
                    break;
            }
        }
        ClearSkillUsingStatus();

#ifndef DEF_HACKCLIENT
        if (m_sDamageMove != 0)
        {
            camera_reset = true;
            m_cCommand = DEF_OBJECTDAMAGEMOVE;
            m_sCommX = m_sPlayerX;
            m_sCommY = m_sPlayerY;

            // todo: keep illusion movement flyback backwards? (target would fly towards you, the attacker)
            if (m_bIllusionMVT == true)
            {
                switch (m_sDamageMove)
                {
                    case 1: m_sCommY--; break;
                    case 2: m_sCommX++; m_sCommY--; break;
                    case 3: m_sCommX++; break;
                    case 4: m_sCommX++; m_sCommY++; break;
                    case 5: m_sCommY++; break;
                    case 6: m_sCommX--; m_sCommY++; break;
                    case 7: m_sCommX--; break;
                    case 8: m_sCommX--; m_sCommY--; break;
                }
            }
            else
            {
                switch (m_sDamageMove)
                {
                    case 1: m_sCommY++; break;
                    case 2: m_sCommX--; m_sCommY++; break;
                    case 3: m_sCommX--; break;
                    case 4: m_sCommX--; m_sCommY--; break;
                    case 5: m_sCommY--; break;
                    case 6: m_sCommX++; m_sCommY--; break;
                    case 7: m_sCommX++; break;
                    case 8: m_sCommX++; m_sCommY++; break;
                }
            }
            for (i = 1; i < DEF_MAXCHATMSGS; i++)
                if (m_pChatMsgList[i] == nullptr)
                {
                    memset(cTxt, 0, sizeof(cTxt));
                    if (m_sDamageMoveAmount > 0)
                        format_to_local(cTxt, "-{}Pts", m_sDamageMoveAmount); //pts
                    else strcpy(cTxt, "Critical!");

                    int iFontType;
                    if ((m_sDamageMoveAmount >= 0) && (m_sDamageMoveAmount < 12))		iFontType = 21;
                    else if ((m_sDamageMoveAmount >= 12) && (m_sDamageMoveAmount < 40)) iFontType = 22;
                    else if ((m_sDamageMoveAmount >= 40) || (m_sDamageMoveAmount < 0))	iFontType = 23;

                    m_pChatMsgList[i] = new CMsg(iFontType, cTxt, m_dwCurTime);
                    m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;

                    m_stNPCList[m_sPlayerObjectID].m_iHP -= m_sDamageMoveAmount;//Change HP Bar

                    if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == false)
                    {
                        delete m_pChatMsgList[i];
                        m_pChatMsgList[i] = nullptr;
                    }
                    break;
                }
            m_sDamageMove = 0;
        }
#endif

        switch (m_cCommand)
        {
            case DEF_OBJECTRUN:
            case DEF_OBJECTMOVE:
            case DEF_OBJECTDAMAGEMOVE:

                if (m_bParalyze) return;
                bGORet = m_pMapData->bGetOwner(m_sCommX, m_sCommY, pDstName, &sDstOwnerType, &iDstOwnerStatus, &m_wCommObjectID);

                if ((m_sPlayerX == m_sCommX) && (m_sPlayerY == m_sCommY))
                    m_cCommand = DEF_OBJECTSTOP;
                else if ((abs(m_sPlayerX - m_sCommX) <= 1) && (abs(m_sPlayerY - m_sCommY) <= 1) &&
                    (bGORet == true) && (sDstOwnerType != 0))
                    m_cCommand = DEF_OBJECTSTOP;
                else if ((abs(m_sPlayerX - m_sCommX) <= 2) && (abs(m_sPlayerY - m_sCommY) <= 2) &&
                    (m_pMapData->m_tile[m_sCommX][m_sCommY].m_bIsMoveAllowed == false))
                    m_cCommand = DEF_OBJECTSTOP;
                else
                {
                    if (m_cCommand == DEF_OBJECTMOVE)
                    {
                        if (m_bRunningMode || m_bShiftPressed) m_cCommand = DEF_OBJECTRUN;
                    }
                    if (m_cCommand == DEF_OBJECTRUN)
                    {
                        if ((m_bRunningMode == false) && (m_bShiftPressed == false)) m_cCommand = DEF_OBJECTMOVE;
                        if (m_iSP < 1) m_cCommand = DEF_OBJECTMOVE;
                    }

                    cDir = cGetNextMoveDir(m_sPlayerX, m_sPlayerY, m_sCommX, m_sCommY, m_bIllusionMVT);
                    if (cDir != 0)
                    {

                        m_cPlayerDir = cDir;
                        bSendCommand(MSGID_COMMAND_MOTION, m_cCommand, cDir, 0, 0, 0, 0);

                        if (m_cCommand != DEF_OBJECTSTOP)
                        {
                            m_sViewStartX = (m_sPlayerX * 32) - (get_virtual_width() / 2);
                            m_sViewStartY = (m_sPlayerY * 32) - ((get_virtual_height() / 2) - 16);
                        }

                        switch (cDir)
                        {
                            case 1:	m_sPlayerY--; break;
                            case 2:	m_sPlayerY--; m_sPlayerX++;	break;
                            case 3:	m_sPlayerX++; break;
                            case 4:	m_sPlayerX++; m_sPlayerY++;	break;
                            case 5:	m_sPlayerY++; break;
                            case 6:	m_sPlayerX--; m_sPlayerY++;	break;
                            case 7:	m_sPlayerX--; break;
                            case 8:	m_sPlayerX--; m_sPlayerY--;	break;
                        }

                        if (m_cCommand == DEF_OBJECTSTOP)
                        {
                            m_sViewStartX = (m_sPlayerX * 32) - (get_virtual_width() / 2);
                            m_sViewStartY = (m_sPlayerY * 32) - ((get_virtual_height() / 2) - 16);
                        }

                        m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                            m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                            m_iPlayerStatus, m_cPlayerName,
                            m_cCommand, 0, 0, 0);
                        m_bCommandAvailable = false;
                        next_command = dwTime + m_pMapData->m_stFrame[m_sPlayerType][m_cCommand].m_sFrameTime * (m_pMapData->m_stFrame[m_sPlayerType][m_cCommand].m_sMaxFrame + 1);
                        m_dwCommandTime = m_dwCurTime;

                        m_iPrevMoveX = m_sPlayerX;
                        m_iPrevMoveY = m_sPlayerY;
                    }
                }

                if (m_cCommand == DEF_OBJECTDAMAGEMOVE)
                {
#ifndef DEF_HACKCLIENT
                    m_bIsGetPointingMode = false;
                    ClearCoords();
                    m_iPointCommandType = -1;
                    set_mouse_cursor(mouse_cursor::PointingHand);
                    ClearSkillUsingStatus();

                    m_cCommand = DEF_OBJECTSTOP;
#endif
                }
                break;

            case DEF_OBJECTATTACK:
                cDir = m_Misc.cGetNextMoveDir(m_sPlayerX, m_sPlayerY, m_sCommX, m_sCommY, m_bIllusionMVT);

                if (cDir != 0)
                {
                    if ((wType == 2) || (wType == 25))
                    {
                        if (_bCheckItemByType(DEF_ITEMTYPE_ARROW) == false)
                            wType = 0;
                    }

                    if (wType >= 20)
                    {
                        m_iSuperAttackLeft--;
                        if (m_iSuperAttackLeft < 0) m_iSuperAttackLeft = 0;
                    }

                    m_cPlayerDir = cDir;
                    bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTATTACK, cDir, m_sCommX, m_sCommY, wType, 0, m_wCommObjectID);

                    m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                        m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                        m_iPlayerStatus, m_cPlayerName,
                        DEF_OBJECTATTACK,
                        m_sCommX - m_sPlayerX, m_sCommY - m_sPlayerY, wType);
                    m_bCommandAvailable = false;
                    next_command = dwTime + m_pMapData->m_stFrame[m_sPlayerType][m_cCommand].m_sFrameTime * (m_pMapData->m_stFrame[m_sPlayerType][m_cCommand].m_sMaxFrame + 1);
                    m_dwCommandTime = m_dwCurTime;
                }

                m_cCommand = DEF_OBJECTSTOP;
                break;

            case DEF_OBJECTATTACKMOVE:
                if (m_bParalyze) return;
                bGORet = m_pMapData->bGetOwner(m_sCommX, m_sCommY, pDstName, &sDstOwnerType, &iDstOwnerStatus, &m_wCommObjectID);

                if ((m_sPlayerX == m_sCommX) && (m_sPlayerY == m_sCommY))
                    m_cCommand = DEF_OBJECTSTOP;
                else if ((abs(m_sPlayerX - m_sCommX) <= 1) && (abs(m_sPlayerY - m_sCommY) <= 1) &&
                    (bGORet == true) && (sDstOwnerType != 0))
                    m_cCommand = DEF_OBJECTSTOP;
                else
                {
                    cDir = cGetNextMoveDir(m_sPlayerX, m_sPlayerY, m_sCommX, m_sCommY, true, m_bIllusionMVT);

                    if (cDir != 0)
                    {
                        m_cPlayerDir = cDir;
                        bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTATTACKMOVE, cDir, m_sCommX, m_sCommY, wType, 0, m_wCommObjectID);

                        m_sViewStartX = (m_sPlayerX * 32) - ((get_virtual_width() / 2));
                        m_sViewStartY = (m_sPlayerY * 32) - ((get_virtual_height() / 2) - 16);

                        switch (cDir)
                        {
                            case 1:	m_sPlayerY--; break;
                            case 2:	m_sPlayerY--; m_sPlayerX++;	break;
                            case 3:	m_sPlayerX++; break;
                            case 4:	m_sPlayerX++; m_sPlayerY++;	break;
                            case 5:	m_sPlayerY++; break;
                            case 6:	m_sPlayerX--; m_sPlayerY++;	break;
                            case 7:	m_sPlayerX--; break;
                            case 8:	m_sPlayerX--; m_sPlayerY--;	break;
                        }

                        m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                            m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                            m_iPlayerStatus, m_cPlayerName,
                            m_cCommand, m_sCommX - m_sPlayerX, m_sCommY - m_sPlayerY, wType);
                        m_bCommandAvailable = false;
                        next_command = dwTime + m_pMapData->m_stFrame[m_sPlayerType][m_cCommand].m_sFrameTime * (m_pMapData->m_stFrame[m_sPlayerType][m_cCommand].m_sMaxFrame + 1);
                        m_dwCommandTime = m_dwCurTime;

                        m_iPrevMoveX = m_sPlayerX;
                        m_iPrevMoveY = m_sPlayerY;
                    }
                }

                m_cCommand = DEF_OBJECTSTOP;
                dashing = true;
                break;

            case DEF_OBJECTGETITEM:
                bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTGETITEM, m_cPlayerDir, 0, 0, 0, 0);

                m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                    m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                    m_iPlayerStatus, m_cPlayerName,
                    DEF_OBJECTGETITEM, 0, 0, 0);
                m_bCommandAvailable = false;
                m_cCommand = DEF_OBJECTSTOP;
                break;

            case DEF_OBJECTMAGIC:
                bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTMAGIC, m_cPlayerDir, m_iCastingMagicType, 0, 0, 0);

                m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                    m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                    m_iPlayerStatus, m_cPlayerName,
                    DEF_OBJECTMAGIC, m_iCastingMagicType, 0, 0);
#ifdef DEF_HACKCLIENT
                if (m_stConfigList.bFastCast == true)
                    m_bCommandAvailable = true;//Change false
                else
#endif
                    m_bCommandAvailable = false;
                next_command = dwTime + m_pMapData->m_stFrame[m_sPlayerType][m_cCommand].m_sFrameTime * (m_pMapData->m_stFrame[m_sPlayerType][m_cCommand].m_sMaxFrame + 1);
                m_dwCommandTime = m_dwCurTime;
                m_bIsGetPointingMode = true;
                m_cCommand = DEF_OBJECTSTOP;
                _RemoveChatMsgListByObjectID(m_sPlayerObjectID);

                for (i = 1; i < DEF_MAXCHATMSGS; i++)
                    if (m_pChatMsgList[i] == nullptr)
                    {
                        memset(cTxt, 0, sizeof(cTxt));
                        if (m_iCastingMagicType < 0 || m_iCastingMagicType > 100 || m_pMagicCfgList[m_iCastingMagicType] == nullptr)
                            format_to_local(cTxt, "Invalid Spell!");//Change Invalid Spell
                        else
                            format_to_local(cTxt, "{}!", m_pMagicCfgList[m_iCastingMagicType]->m_cName);
                        m_pChatMsgList[i] = new CMsg(41, cTxt, m_dwCurTime);
                        m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;

                        m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i);
                        return;
                    }
                break;

            default:
                break;
        }
    }
}
