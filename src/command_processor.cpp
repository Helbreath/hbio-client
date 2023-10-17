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



void CGame::CommandProcessor(short msX, short msY, short indexX, short indexY, char cLB, char cRB)
{
    char   cDir, absX, absY, cName[12];
    short  sX, sY, sObjectType, tX, tY;
    int sObjectStatus;
    int    iRet;
    DWORD  dwTime = unixtime();
    WORD   wType = 0;
    int i;//, iFOE;
    char   cTxt[120];

    char  pDstName[21];
    short sDstOwnerType;
    int sDstOwnerStatus;
    BOOL  bGORet;
    if ((m_bIsObserverCommanded == FALSE) && (m_bIsObserverMode == TRUE))
    {
        
        if ((msX == 0) && (msY == 0) && (m_sViewDstX > 32 * 21) && (m_sViewDstY > 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 8, NULL, NULL, NULL, NULL);
        else if ((msX == 640) && (msY == 0) && (m_sViewDstX < 32 * m_pMapData->m_sMapSizeX - 32 * 21) && (m_sViewDstY > 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 2, NULL, NULL, NULL, NULL);
        else if ((msX == 640) && (msY == 480) && (m_sViewDstX < 32 * m_pMapData->m_sMapSizeX - 32 * 21) && (m_sViewDstY < 32 * m_pMapData->m_sMapSizeY - 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 4, NULL, NULL, NULL, NULL);
        else if ((msX == 0) && (msY == 480)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 6, NULL, NULL, NULL, NULL);
        else if ((msX == 0) && (m_sViewDstX > 32 * 21)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 7, NULL, NULL, NULL, NULL);
        else if ((msX == 640) && (m_sViewDstX < 32 * m_pMapData->m_sMapSizeX - 32 * 21)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 3, NULL, NULL, NULL, NULL);
        else if ((msY == 0) && (m_sViewDstY > 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 1, NULL, NULL, NULL, NULL);
        else if ((msY == 480) && (m_sViewDstY < 32 * m_pMapData->m_sMapSizeY - 32 * 16)) bSendCommand(MSGID_REQUEST_PANNING, NULL, 5, NULL, NULL, NULL, NULL);
        else return;

        m_bIsObserverCommanded = TRUE;
        m_cArrowPressed = 0;
        return;
    }

    
    if (m_bIsObserverMode == TRUE) return;

    
    if (GetAsyncKeyState(VK_MENU) >> 15)
        m_bSuperAttackMode = TRUE;
    else m_bSuperAttackMode = FALSE;

    
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
                m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_PRESSED;
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

                m_stMCursor.cSelectedObjectType = NULL;
                m_stMCursor.sSelectedObjectID = NULL;
            }
            return;
        }
        if (cLB != 0)
        {
            
            if ((m_pMapData->bIsTeleportLoc(m_sPlayerX, m_sPlayerY) == TRUE) && (m_cCommandCount == 0)) goto CP_SKIPMOUSEBUTTONSTATUS;

            if ((m_stMCursor.sPrevX != msX) || (m_stMCursor.sPrevY != msY))
            {
                m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_DRAGGING;
                m_stMCursor.sPrevX = msX;
                m_stMCursor.sPrevY = msY;

                if ((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX) &&
                    ((m_stMCursor.sSelectedObjectID == 30) || (m_stMCursor.sSelectedObjectID == 29)))
                {
                    
                    m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                }

                
                
                if ((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_DLGBOX) &&
                    (m_stMCursor.sSelectedObjectID == 7) && (m_stDialogBoxInfo[7].cMode == 1))
                {
                    
                    EndInputString();
                    m_stDialogBoxInfo[7].cMode = 20;
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
            
            
            if ((m_pMapData->bIsTeleportLoc(m_sPlayerX, m_sPlayerY) == TRUE) && (m_cCommandCount == 0)) goto CP_SKIPMOUSEBUTTONSTATUS;

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
                    if (msX < 320) m_stDialogBoxInfo[9].sX = 0;
                    else m_stDialogBoxInfo[9].sX = 640 - m_stDialogBoxInfo[9].sSizeX;
                    if (msY < 213) m_stDialogBoxInfo[9].sY = 0;
                    else m_stDialogBoxInfo[9].sY = 427 - m_stDialogBoxInfo[9].sSizeY;
                }

                m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                m_stMCursor.cSelectedObjectType = NULL;
                m_stMCursor.sSelectedObjectID = NULL;
                break;

            case DEF_SELECTEDOBJTYPE_ITEM:
                
                _bCheckDraggingItemRelease(msX, msY);
                m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                m_stMCursor.cSelectedObjectType = NULL;
                m_stMCursor.sSelectedObjectID = NULL;
                break;

            default:
                m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
                m_stMCursor.cSelectedObjectType = NULL;
                m_stMCursor.sSelectedObjectID = NULL;
                break;
            }
            return;
        }
        break;
    }

    CP_SKIPMOUSEBUTTONSTATUS:;

    if (m_bCommandAvailable == false) return;
// 	if ((dwTime - m_dwCommandTime) < 300)
// 	{
//         socketmode(0);
//         close(1000, "speedhack");
//         m_bEscPressed = false;
//         PlaySound('E', 14, 5);
//         if (m_bSoundFlag)
//             m_pESound[38].stop();
//         if ((m_bSoundFlag) && (m_bMusicStat == true))
//         {
//             m_pBGM.stop();
//         }
//         isItemLoaded = false;
//         ChangeGameMode(GAMEMODE_ONMAINMENU);
//         return;
// 	}
// 

//     if (m_bCommandAvailable == FALSE) return;
//     if ((dwTime - m_dwCommandTime) < 300)
//     {

//         delete m_pGSock;
//         m_pGSock = NULL;
//         m_bEscPressed = FALSE;
// 
//         PlaySound('E', 14, 5);

//         if (m_bSoundFlag) m_pESound[38].stop();

//         if ((m_bSoundFlag) && (m_bMusicStat == TRUE))
//         {
//             if (m_pBGM != NULL) m_pBGM.stop();
//         }
// 

//         if (strlen(G_cCmdLineTokenA) != 0)
//             ChangeGameMode(DEF_GAMEMODE_ONQUIT);
//         else
//         {
//             ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
//         }
//         return;
//     }
    if (m_iHP <= 0) return;

    
    if (m_sDamageMove != 0)
    {
        m_cCommand = DEF_OBJECTDAMAGEMOVE;
        goto MOTION_COMMAND_PROCESS;
    }

    
    if ((m_pMapData->bIsTeleportLoc(m_sPlayerX, m_sPlayerY) == TRUE) && (m_cCommandCount == 0))
        RequestTeleportAndWaitData();

    
    if (cLB != 0)
    {

        
        if (m_bIsGetPointingMode == TRUE)
        {
            
            if ((m_sMCX != 0) || (m_sMCY != 0))
                PointCommandHandler(m_sMCX, m_sMCY);
            else PointCommandHandler(indexX, indexY);

            m_bCommandAvailable = FALSE;
            m_dwCommandTime = unixtime();
            m_bIsGetPointingMode = FALSE;
            return;
        }

        m_pMapData->bGetOwner(m_sMCX, m_sMCY - 1, cName, &sObjectType, &sObjectStatus, &m_wCommObjectID);
        //m_pMapData->m_pData[dX][dY].m_sItemSprite
        if (memcmp(m_cMCName, m_cPlayerName, 10) == 0 && (sObjectType <= 6 || m_pMapData->m_pData[m_sPlayerX - m_pMapData->m_sPivotX][m_sPlayerY - m_pMapData->m_sPivotY].m_sItemSprite != 0))
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
                
                if (m_bCtrlPressed == TRUE)
                { 

                    m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &sObjectStatus, &m_wCommObjectID);

                    
                    if ((sObjectStatus & 0x10) != 0) return;
                    if ((sObjectType == 15) || (sObjectType == 20) || (sObjectType == 24)) return;

                    m_stMCursor.sCursorFrame = 3;

                    absX = abs(m_sPlayerX - m_sMCX);
                    absY = abs(m_sPlayerY - m_sMCY);

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
                            if ((absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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

                        case 9: 
                            if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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
                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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
                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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
                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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
                    
                    m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &sObjectStatus, &m_wCommObjectID);

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
                                if ((tX + 235) > 639) tX = 639 - 235;
                                if (tY < 0) tY = 0;
                                if ((tY + 100) > 479) tY = 479 - 100;
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
                                EnableDialogBox(20, 0, 16, NULL);
                                tX = msX - 117;
                                tY = msY - 50;
                                if (tX < 0) tX = 0;
                                if ((tX + 235) > 639) tX = 639 - 235;
                                if (tY < 0) tY = 0;
                                if ((tY + 100) > 479) tY = 479 - 100;
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
                                EnableDialogBox(20, 0, 14, NULL);
                                tX = msX - 117;
                                tY = msY - 50;
                                if (tX < 0) tX = 0;
                                if ((tX + 235) > 639) tX = 639 - 235;
                                if (tY < 0) tY = 0;
                                if ((tY + 100) > 479) tY = 479 - 100;
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
                                if ((tX + 235) > 639) tX = 639 - 235;
                                if (tY < 0) tY = 0;
                                if ((tY + 100) > 479) tY = 479 - 100;
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
                                EnableDialogBox(20, 0, 13, NULL);
                                tX = msX - 117;
                                tY = msY - 50;
                                if (tX < 0) tX = 0;
                                if ((tX + 235) > 639) tX = 639 - 235;
                                if (tY < 0) tY = 0;
                                if ((tY + 100) > 479) tY = 479 - 100;
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
                                EnableDialogBox(20, 0, 7, NULL);
                                tX = msX - 117;
                                tY = msY - 50;
                                if (tX < 0) tX = 0;
                                if ((tX + 235) > 639) tX = 639 - 235;
                                if (tY < 0) tY = 0;
                                if ((tY + 100) > 479) tY = 479 - 100;
                                m_stDialogBoxInfo[20].sX = tX;
                                m_stDialogBoxInfo[20].sY = tY;
                                m_stDialogBoxInfo[20].sV3 = 26;
                                break;
                            }
                            break;

                        case 21:
                            
                            if ((_iGetFOE(sObjectStatus) == 1) && (!m_bIsCombatMode))
                            {
                                EnableDialogBox(20, 4, NULL, NULL);
                                tX = msX - 117;
                                tY = msY - 50;
                                if (tX < 0) tX = 0;
                                if ((tX + 235) > 639) tX = 639 - 235;
                                if (tY < 0) tY = 0;
                                if ((tY + 100) > 479) tY = 479 - 100;
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
                                EnableDialogBox(20, 4, NULL, NULL);
                                tX = msX - 117;
                                tY = msY - 50;
                                if (tX < 0) tX = 0;
                                if ((tX + 235) > 639) tX = 639 - 235;
                                if (tY < 0) tY = 0;
                                if ((tY + 100) > 479) tY = 479 - 100;
                                m_stDialogBoxInfo[20].sX = tX;
                                m_stDialogBoxInfo[20].sY = tY;
                                m_stDialogBoxInfo[20].sV3 = sObjectType;
                            }
                            break;

                        case 32:
                            // Unicorn 
                            if (!m_bIsCombatMode)
                            {
                                EnableDialogBox(20, 4, NULL, NULL);
                                tX = msX - 117;
                                tY = msY - 50;
                                if (tX < 0) tX = 0;
                                if ((tX + 235) > 639) tX = 639 - 235;
                                if (tY < 0) tY = 0;
                                if ((tY + 100) > 479) tY = 479 - 100;
                                m_stDialogBoxInfo[20].sX = tX;
                                m_stDialogBoxInfo[20].sY = tY;
                                m_stDialogBoxInfo[20].sV3 = 32;
                            }
                            break;

                        default:
                            
                            
                            if (_iGetFOE(sObjectStatus) >= 0) break;
                            if ((sObjectType >= 1) && (sObjectType <= 6) && (m_bForceAttack == FALSE)) break;
                            absX = abs(m_sPlayerX - m_sMCX);
                            absY = abs(m_sPlayerY - m_sMCY);

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
                                    if ((absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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

                                case 9: 
                                    if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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
                                    if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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
                                    if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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
                                    if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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
    else
        if (cRB != 0)
        {
            
            m_cCommand = DEF_OBJECTSTOP;

            if (m_bIsGetPointingMode == TRUE)
            {
                
                
                m_bIsGetPointingMode = FALSE;
                AddEventList(COMMAND_PROCESSOR1, 10);
            }

            if (m_bCommandAvailable == FALSE) return;
            if (m_cCommandCount >= 6) return;

            if ((m_sMCX != 0) && (m_sMCY != 0))
            {
                
                absX = abs(m_sPlayerX - m_sMCX);
                absY = abs(m_sPlayerY - m_sMCY);
                if (absX == 0 && absY == 0) return;

                if (m_bCtrlPressed == TRUE)
                {
                    
                    m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &sObjectStatus, &m_wCommObjectID);
                    
                    if ((sObjectStatus & 0x10) != 0) return;
                    if ((sObjectType == 15) || (sObjectType == 20) || (sObjectType == 24)) return;

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
                            if ((absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
                            {
                                
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                                wType = _iGetAttackType();
                            }
                            break;

                        case 9: 
                            if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
                            {
                                
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                                wType = _iGetAttackType();
                            }
                            break;

                        case 10: 
                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
                            {
                                
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                                wType = _iGetAttackType();
                            }
                            break;

                        case 14: 
                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
                            {
                                
                                m_cCommand = DEF_OBJECTATTACK;
                                m_sCommX = m_sMCX;
                                m_sCommY = m_sMCY;
                                wType = _iGetAttackType();
                            }
                            break;
                        case 21: 
                            if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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

                    m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sObjectType, &sObjectStatus, &m_wCommObjectID);

                    

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
                            
                            if (_iGetFOE(sObjectStatus) >= 0) break;
                            if ((sObjectType >= 1) && (sObjectType <= 6) && (m_bForceAttack == FALSE)) break;
                            
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
                                    if ((absX <= 3) && (absY <= 3) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
                                    {
                                        
                                        m_cCommand = DEF_OBJECTATTACK;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        wType = _iGetAttackType();
                                    }
                                    break;

                                case 9: 
                                    if ((absX <= 4) && (absY <= 4) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
                                    {
                                        
                                        m_cCommand = DEF_OBJECTATTACK;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        wType = _iGetAttackType();
                                    }
                                    break;

                                case 10: 
                                    if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
                                    {
                                        
                                        m_cCommand = DEF_OBJECTATTACK;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        wType = _iGetAttackType();
                                    }
                                    break;
                                case 14: 
                                    if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
                                    {
                                        
                                        m_cCommand = DEF_OBJECTATTACK;
                                        m_sCommX = m_sMCX;
                                        m_sCommY = m_sMCY;
                                        wType = _iGetAttackType();
                                    }
                                    break;
                                case 21: 
                                    if ((absX <= 2) && (absY <= 2) && (m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == TRUE))
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
                cDir = m_Misc.cGetNextMoveDir(m_sPlayerX, m_sPlayerY, indexX, indexY);

                
                if (m_iHP <= 0) return;
                if (cDir == 0) return;
                if (m_cPlayerDir == cDir) return;

                
                ClearSkillUsingStatus();

                m_cPlayerDir = cDir;
                bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTSTOP, m_cPlayerDir, NULL, NULL, NULL, NULL);

                m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                    m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                    m_sPlayerStatus, m_cPlayerName,
                    m_cCommand, NULL, NULL, NULL, 0,
                    10);
                m_bCommandAvailable = FALSE;
                m_dwCommandTime = unixtime();
                return;
            }
        }

    MOTION_COMMAND_PROCESS:;

    if (m_cCommand != DEF_OBJECTSTOP)
    {

        
        if (m_iHP <= 0) return;
        if (m_cCommandCount == 5) AddEventList(COMMAND_PROCESSOR2, 10, FALSE);
        if (m_bCommandAvailable == FALSE) return;
        if (m_cCommandCount >= 6) return;

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

        if (m_sDamageMove != 0)
        {
            m_cCommand = DEF_OBJECTDAMAGEMOVE;
            m_sCommX = m_sPlayerX;
            m_sCommY = m_sPlayerY;

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

            
            for (i = 1; i < DEF_MAXCHATMSGS; i++)
                if (m_pChatMsgList[i] == NULL)
                {
                    ZeroMemory(cTxt, sizeof(cTxt));
                    if (m_sDamageMoveAmount > 0)
                        format_to_local(cTxt, "-{}Pts", m_sDamageMoveAmount);
                    else strcpy(cTxt, "Critical!");

                    int iFontType;
                    if ((m_sDamageMoveAmount >= 0) && (m_sDamageMoveAmount < 12))		iFontType = 21;
                    else if ((m_sDamageMoveAmount >= 12) && (m_sDamageMoveAmount < 40)) iFontType = 22;
                    else if ((m_sDamageMoveAmount >= 40) || (m_sDamageMoveAmount < 0))	iFontType = 23;

                    m_pChatMsgList[i] = new CMsg(iFontType, cTxt, m_dwCurTime);
                    m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;

                    if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == FALSE)
                    {
                        delete m_pChatMsgList[i];
                        m_pChatMsgList[i] = NULL;
                    }
                    break;
                }
            m_sDamageMove = 0;
        }

        
        switch (m_cCommand)
        {
        case DEF_OBJECTRUN:
        case DEF_OBJECTMOVE:
        case DEF_OBJECTDAMAGEMOVE:

            if (m_bParalyze) return;
            bGORet = m_pMapData->bGetOwner(m_sCommX, m_sCommY, pDstName, &sDstOwnerType, &sDstOwnerStatus, &m_wCommObjectID);

            if ((m_sPlayerX == m_sCommX) && (m_sPlayerY == m_sCommY))
                m_cCommand = DEF_OBJECTSTOP;
            else if ((abs(m_sPlayerX - m_sCommX) <= 1) && (abs(m_sPlayerY - m_sCommY) <= 1) &&
                (bGORet == TRUE) && (sDstOwnerType != NULL))
                m_cCommand = DEF_OBJECTSTOP;
            
            else if ((abs(m_sPlayerX - m_sCommX) <= 2) && (abs(m_sPlayerY - m_sCommY) <= 2) &&
                (m_pMapData->m_tile[m_sCommX][m_sCommY].m_bIsMoveAllowed == FALSE))
                m_cCommand = DEF_OBJECTSTOP;
            else
            {
                
                if (m_cCommand == DEF_OBJECTMOVE)
                {
                    if (m_bRunningMode || m_bShiftPressed) m_cCommand = DEF_OBJECTRUN;
                }
                if (m_cCommand == DEF_OBJECTRUN)
                {
                    if ((m_bRunningMode == FALSE) && (m_bShiftPressed == FALSE)) m_cCommand = DEF_OBJECTMOVE;
                    if (m_iSP < 1) m_cCommand = DEF_OBJECTMOVE;
                }

                cDir = cGetNextMoveDir(m_sPlayerX, m_sPlayerY, m_sCommX, m_sCommY, TRUE);
                if (cDir != 0)
                {

                    m_cPlayerDir = cDir;
                    bSendCommand(MSGID_COMMAND_MOTION, m_cCommand, cDir, NULL, NULL, NULL, NULL);

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
                        m_sPlayerStatus, m_cPlayerName,
                        m_cCommand, NULL, NULL, NULL);
                    m_bCommandAvailable = FALSE;
                    m_dwCommandTime = unixtime();

                    m_iPrevMoveX = m_sPlayerX;
                    m_iPrevMoveY = m_sPlayerY;
                }
            }

            if (m_cCommand == DEF_OBJECTDAMAGEMOVE)
            {
                
                m_bIsGetPointingMode = FALSE;
                m_iPointCommandType = -1;
               
                m_stMCursor.sCursorFrame = 0;
                
                ClearSkillUsingStatus();

                m_cCommand = DEF_OBJECTSTOP;
            }
            break;

        case DEF_OBJECTATTACK:

            cDir = m_Misc.cGetNextMoveDir(m_sPlayerX, m_sPlayerY, m_sCommX, m_sCommY);
            if (cDir != 0)
            {

                if ((wType == 2) || (wType == 25))
                {
                    
                    if (_bCheckItemByType(DEF_ITEMTYPE_ARROW) == FALSE)
                        wType = 0;
                }

                
                if (wType >= 20)
                {
                    m_iSuperAttackLeft--;
                    if (m_iSuperAttackLeft < 0) m_iSuperAttackLeft = 0;
                }

                m_cPlayerDir = cDir;
                bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTATTACK, cDir, m_sCommX, m_sCommY, wType, NULL, m_wCommObjectID);

                m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                    m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                    m_sPlayerStatus, m_cPlayerName,
                    DEF_OBJECTATTACK,
                    m_sCommX - m_sPlayerX, m_sCommY - m_sPlayerY, wType);
                m_bCommandAvailable = FALSE;
                m_dwCommandTime = unixtime();
            }

            m_cCommand = DEF_OBJECTSTOP;
            break;

        case DEF_OBJECTATTACKMOVE:

            if (m_bParalyze) return;
            bGORet = m_pMapData->bGetOwner(m_sCommX, m_sCommY, pDstName, &sDstOwnerType, &sDstOwnerStatus, &m_wCommObjectID);

            if ((m_sPlayerX == m_sCommX) && (m_sPlayerY == m_sCommY))
                m_cCommand = DEF_OBJECTSTOP;
            else if ((abs(m_sPlayerX - m_sCommX) <= 1) && (abs(m_sPlayerY - m_sCommY) <= 1) &&
                (bGORet == TRUE) && (sDstOwnerType != NULL))
                m_cCommand = DEF_OBJECTSTOP;
            else
            {
                cDir = cGetNextMoveDir(m_sPlayerX, m_sPlayerY, m_sCommX, m_sCommY, TRUE);
                if (cDir != 0)
                {

                    m_cPlayerDir = cDir;
                    bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTATTACKMOVE, cDir, m_sCommX, m_sCommY, wType, NULL, m_wCommObjectID);

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
                        m_sPlayerStatus, m_cPlayerName,
                        m_cCommand, m_sCommX - m_sPlayerX, m_sCommY - m_sPlayerY, wType);
                    m_bCommandAvailable = FALSE;
                    m_dwCommandTime = unixtime();

                    m_iPrevMoveX = m_sPlayerX;
                    m_iPrevMoveY = m_sPlayerY;
                }
            }

            m_cCommand = DEF_OBJECTSTOP;
            break;

        case DEF_OBJECTGETITEM:
            bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTGETITEM, m_cPlayerDir, NULL, NULL, NULL, NULL);

            m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                m_sPlayerStatus, m_cPlayerName,
                DEF_OBJECTGETITEM, NULL, NULL, NULL);
            m_bCommandAvailable = FALSE;
            m_cCommand = DEF_OBJECTSTOP;
            break;

        case DEF_OBJECTMAGIC:
            bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTMAGIC, m_cPlayerDir, m_iCastingMagicType, NULL, NULL, NULL);

            m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                m_sPlayerStatus, m_cPlayerName,
                DEF_OBJECTMAGIC, m_iCastingMagicType, NULL, NULL);
            m_bCommandAvailable = FALSE;
            m_dwCommandTime = unixtime();
            m_bIsGetPointingMode = TRUE;
            m_cCommand = DEF_OBJECTSTOP;

            _RemoveChatMsgListByObjectID(m_sPlayerObjectID);

            for (i = 1; i < DEF_MAXCHATMSGS; i++)
                if (m_pChatMsgList[i] == NULL)
                {
                    ZeroMemory(cTxt, sizeof(cTxt));
                    format_to_local(cTxt, "{}!", m_pMagicCfgList[m_iCastingMagicType]->m_cName);
                    m_pChatMsgList[i] = new CMsg(41, cTxt, unixtime());
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
