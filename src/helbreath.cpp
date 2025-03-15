//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "game.h"
#include <algorithm>
#include <iostream>
#include <fmt/format.h>
#include <SFML/Window/ContextSettings.hpp>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include "sprite.h"
#include "sprite_id.h"
#include "char_info.h"
#include "mouse_interface.h"
#include "action_id.h"
#include "item.h"
#include "map_data.h"
#include "lan_eng.h"
#include "msg.h"
#include "str_tok.h"

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

void CGame::on_input_event(sf::Event event)
{
    //TODO: fix
    int64_t dwTime = m_dwCurTime;
    int iTotalMsg = 0;

    if (m_bInputStatus && event.type == sf::Event::TextEntered)
    {
        //rethink this
//         if (
//             event.key.code != Keyboard::Backspace
//             && event.key.code != Keyboard::Enter
//             && event.key.code != Keyboard::Escape
//             && event.key.code != Keyboard::Tab
// //             && event.key.code != Keyboard::LShift
// //             && event.key.code != Keyboard::RShift
// //             && event.key.code != Keyboard::LSystem
// //             && event.key.code != Keyboard::RSystem
// //             && event.key.code != Keyboard::Menu
// //             && event.key.code != Keyboard::LControl
// //             && event.key.code != Keyboard::RControl
// //             && event.key.code != Keyboard::LAlt
// //             && event.key.code != Keyboard::RAlt
//             )
        if (event.text.unicode == 8)
        {

        }
        else if (
            event.text.unicode != 9
            && event.text.unicode != 4
            && event.text.unicode != 13
            && event.text.unicode != 27
            )
        {
            std::string s{ (char)(((int32_t)event.text.unicode) & 0xff) };

            uint32_t len = (uint32_t)strlen(m_pInputBuffer);
            if (len >= m_inputMaxLen - 1) return;
            m_pInputBuffer[len] = s[0];
            m_pInputBuffer[len + 1] = 0;
            return;
        }
    }

    switch (event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch (event.key.code)
            {
                case Keyboard::Backspace:
                    if (m_pInputBuffer)
                        if (int len = (int)strlen(m_pInputBuffer))
                            m_pInputBuffer[len - 1] = 0;
                    break;

                case Keyboard::LShift:
                    m_bShiftPressed = true;
                    break;
                case Keyboard::LControl:
                    m_bCtrlPressed = true;
                    break;
                case Keyboard::LAlt:
                    m_altPressed = true;
                    break;

//                 case Keyboard::F11:
//                     scale_mouse_rendering = !scale_mouse_rendering;
//                     if (scale_mouse_rendering)
//                     {
//                         for (int i = 0; i < m_pSprite[DEF_SPRID_MOUSECURSOR]->m_iTotalFrame; ++i)
//                             m_pSprite[DEF_SPRID_MOUSECURSOR]->sprite_[i].setScale(static_cast<float>(screenwidth) / screenwidth_v, static_cast<float>(screenheight) / screenheight_v);
//                     }
//                     else
//                         for (int i = 0; i < m_pSprite[DEF_SPRID_MOUSECURSOR]->m_iTotalFrame; ++i)
//                             m_pSprite[DEF_SPRID_MOUSECURSOR]->sprite_[i].setScale(1, 1);
// 
//                     break;

                case Keyboard::A:
                    if (m_bCtrlPressed && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        if (m_bForceAttack)
                        {
                            m_bForceAttack = false;
                            AddEventList(DEF_MSG_FORCEATTACK_OFF, 10);
                        }
                        else
                        {
                            m_bForceAttack = true;
                            AddEventList(DEF_MSG_FORCEATTACK_ON, 10);
                        }
                    }
                    break;
                case Keyboard::D:
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        m_cDetailLevel++;
                        if (m_cDetailLevel > 2) m_cDetailLevel = 0;
                        switch (m_cDetailLevel)
                        {
                            case 0:
                                AddEventList(NOTIFY_MSG_DETAIL_LEVEL_LOW, 10);
                                break;
                            case 1:
                                AddEventList(NOTIFY_MSG_DETAIL_LEVEL_MEDIUM, 10);
                                break;
                            case 2:
                                AddEventList(NOTIFY_MSG_DETAIL_LEVEL_HIGH, 10);
                                break;
                        }
                    }
                    break;
                case Keyboard::M:
                    if (m_cGameMode == DEF_GAMEMODE_ONMAINGAME)
                    {
                        if (m_bCtrlPressed)
                        {
                            if (m_bIsDialogEnabled[9] == true) DisableDialogBox(9);
                            else EnableDialogBox(9, 0, 0, 0, 0);
                        }
                    }
                    break;
                case Keyboard::R:
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        if (m_bRunningMode)
                        {
                            m_bRunningMode = false;
                            AddEventList(NOTIFY_MSG_CONVERT_WALKING_MODE, 10);
                        }
                        else
                        {
                            m_bRunningMode = true;
                            AddEventList(NOTIFY_MSG_CONVERT_RUNNING_MODE, 10);
                        }
                    }
                    break;
                case Keyboard::S:
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        if (m_bMusicStat == true)
                        {
                            // Music Off
                            m_bMusicStat = false;
                            if (m_bSoundFlag)
                                m_pBGM.stop();
                            AddEventList(NOTIFY_MSG_MUSIC_OFF, 10);
                            break;
                        }
                        else if (m_bSoundStat == true)
                        {
                            m_pESound[38].stop();
                            m_bSoundStat = false;
                            AddEventList(NOTIFY_MSG_SOUND_OFF, 10);
                            break;
                        }
                        else
                        {
                            if (m_bSoundFlag)
                            {
                                m_bMusicStat = true;
                                AddEventList(NOTIFY_MSG_MUSIC_ON, 10);
                            }
                            if (m_bSoundFlag)
                            {
                                m_bSoundStat = true;
                                AddEventList(NOTIFY_MSG_SOUND_ON, 10);
                            }
                            StartBGM();
                        }
                    }
                    break;
                case Keyboard::T:
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100];
                        short sX, sY;
                        short msX = m_stMCursor.sX, msY = m_stMCursor.sY, msZ = m_stMCursor.sZ;
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        memset(tempid, 0, sizeof(tempid));
                        if (m_bIsDialogEnabled[10] == true && (msX >= sX + 20) && (msX <= sX + 360) && (msY >= sY + 35) && (msY <= sY + 139))
                        {
                            CStrTok * pStrTok;
                            char * token, cBuff[64];
                            char   seps[] = ":";
                            int i = (139 - msY + sY) / 13;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] == 0) return;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message[0] == ' ') i++;
                            strcpy(cBuff, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message.c_str());
                            pStrTok = new CStrTok(cBuff, seps);
                            token = pStrTok->pGet();
                            format_to_local(tempid, "/to {}", token);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                            delete pStrTok;
                        }
                        else if (_tmp_sOwnerType < 7 && (strlen(_tmp_cName) > 0) && (m_iIlusionOwnerH == 0) && ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
                        {
                            format_to_local(tempid, "/to {}", _tmp_cName);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                        else
                        {
                            EndInputString();
                            format_to_local(m_cChatMsg, "/to ");
                            StartInputString(10, get_virtual_height() - 66, sizeof(m_cChatMsg), m_cChatMsg);
                        }
                    }
                    break;
                case Keyboard::Add:
                    if (m_bInputStatus == false) m_bZoomMap = true;
                    break;
                case Keyboard::Subtract:
                    if (m_bInputStatus == false) m_bZoomMap = false;
                    break;
                case Keyboard::F2:
                    UseShortCut(1);
                    break;
                case Keyboard::F3:
                    UseShortCut(2);
                    break;
                case Keyboard::Insert:
                    if (m_iHP <= 0) return;
                    if (m_bItemUsingStatus == true)
                    {
                        AddEventList(USE_RED_POTION1, 10);
                        return;
                    }
                    if (m_bIsDialogEnabled[27] == true)
                    {
                        AddEventList(USE_RED_POTION2, 10);
                        return;
                    }//Change Pot Hack
                    for (int i = 0; i < DEF_MAXITEMS; i++)
                        if ((m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
                            (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 1))
                        {

                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);

                            m_bIsItemDisabled[i] = true;
                            m_bItemUsingStatus = true;

                            return;
                        }
                    for (int i = 0; i < DEF_MAXITEMS; i++)
                        if ((m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
                            (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 2))
                        {

                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);

                            m_bIsItemDisabled[i] = true;
                            m_bItemUsingStatus = true;

                            return;
                        }
                    break;
                case Keyboard::Delete:
                    if (m_iHP <= 0) return;
                    if (m_bItemUsingStatus == true)
                    {
                        AddEventList(USE_BLUE_POTION1, 10);
                        return;
                    }
                    if (m_bIsDialogEnabled[27] == true)
                    {
                        AddEventList(USE_BLUE_POTION2, 10);
                        return;
                    }
                    for (int i = 0; i < DEF_MAXITEMS; i++)
                        if ((m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
                            (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 3))
                        {

                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);

                            m_bIsItemDisabled[i] = true;
                            m_bItemUsingStatus = true;

                            return;
                        }
                    for (int i = 0; i < DEF_MAXITEMS; i++)
                        if ((m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
                            (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 4))
                        {

                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);

                            m_bIsItemDisabled[i] = true;
                            m_bItemUsingStatus = true;

                            return;
                        }
                    break;
                case Keyboard::PageUp:
                    if (m_iHP <= 0) return;
                    if (m_bItemUsingStatus == true)
                    {
                        AddEventList(USE_RED_POTION1, 10);
                        return;
                    }
                    if (m_bIsDialogEnabled[27] == true)
                    {
                        AddEventList(USE_RED_POTION2, 10);
                        return;
                    }//Change Pot Hack
                    for (int i = 0; i < DEF_MAXITEMS; i++)
                        if ((m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
                            (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 5))
                        {

                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);

                            m_bIsItemDisabled[i] = true;
                            m_bItemUsingStatus = true;

                            return;
                        }
                    for (int i = 0; i < DEF_MAXITEMS; i++)
                        if ((m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
                            (m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 6))
                        {

                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);

                            m_bIsItemDisabled[i] = true;
                            m_bItemUsingStatus = true;

                            return;
                        }
                    break;
                case Keyboard::End:
                    if (((m_bIsDialogEnabled[7] == true) && (m_stDialogBoxInfo[7].cMode == 1) && (iGetTopDialogBoxIndex() == 7)) ||
                        ((m_bIsDialogEnabled[17] == true) && (m_stDialogBoxInfo[17].cMode == 1) && (iGetTopDialogBoxIndex() == 17)))
                    {
                    }
                    else if ((!m_bInputStatus)/* && (m_cBackupChatMsg[0] != '!') && (m_cBackupChatMsg[0] != '~') && (m_cBackupChatMsg[0] != '^') &&
                              (m_cBackupChatMsg[0] != '@')*/)
                    {//Change Allow Any Chat to be recovered from Buffer from END key

                        memset(m_cChatMsg, 0, sizeof(m_cChatMsg));
                        strcpy(m_cChatMsg, m_cBackupChatMsg);
                        StartInputString(10, get_virtual_height() - 66, sizeof(m_cChatMsg), m_cChatMsg);
                    }
                    break;
                case Keyboard::F4:
                    if (m_cGameMode != DEF_GAMEMODE_ONMAINGAME) return;
                    UseMagic(m_sMagicShortCut);
                    break;
                case Keyboard::F5:
#ifdef DEF_SHORTCUT
                    UseShortCut(3);
#else
                    if (m_bIsDialogEnabled[1] == false)
                        EnableDialogBox(1, 0, 0, 0);
                    else DisableDialogBox(1);
#endif
                    break;
                case Keyboard::F6:
#ifdef DEF_SHORTCUT
                    UseShortCut(4);
#else
                    if (m_bIsDialogEnabled[2] == false)
                        EnableDialogBox(2, 0, 0, 0);
                    else DisableDialogBox(2);
#endif
                    break;
                case Keyboard::F7:
                    //                     m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                    //                     bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_MAGIC, 0, ((m_sViewPointX + msX + 16)/32), ((m_sViewPointY + msY + 16)/32), m_iPointCommandType, 0);

#ifdef DEF_SHORTCUT
                    if (m_bIsDialogEnabled[1] == false)
                        EnableDialogBox(1, 0, 0, 0);
                    else DisableDialogBox(1);
#else
                    if (m_bIsDialogEnabled[3] == false)
                        EnableDialogBox(3, 0, 0, 0);
                    else DisableDialogBox(3);
#endif
                    break;
                case Keyboard::F8:
                    //                     m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                    //                     bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_MAGIC, 0, ((m_sViewPointX + msX + 16)/32), ((m_sViewPointY + msY + 16)/32), m_iPointCommandType, 0);

#ifdef DEF_SHORTCUT
                    if (m_bIsDialogEnabled[2] == false)
                        EnableDialogBox(2, 0, 0, 0);
                    else DisableDialogBox(2);
#else
                    if (m_bIsDialogEnabled[15] == false)
                        EnableDialogBox(15, 0, 0, 0);
                    else DisableDialogBox(15);
#endif
                    break;
                case Keyboard::F9:
                    //                     m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                    //                     bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_MAGIC, 0, ((m_sViewPointX + msX + 16)/32), ((m_sViewPointY + msY + 16)/32), m_iPointCommandType, 0);

#ifdef DEF_SHORTCUT
                    if (m_bIsDialogEnabled[3] == false)
                        EnableDialogBox(3, 0, 0, 0);
                    else DisableDialogBox(3);
#else
                    if (m_bIsDialogEnabled[10] == false)
                        EnableDialogBox(10, 0, 0, 0);
                    else DisableDialogBox(10);
#endif
                    break;
                case Keyboard::F11:
#ifdef DEF_SHORTCUT
                    if (m_bCtrlPressed) m_bDialogTrans = !m_bDialogTrans;
                    else
                    {
                        if (m_bIsDialogEnabled[10] == false)
                            EnableDialogBox(10, 0, 0, 0);
                        else DisableDialogBox(10);
                    }
#else
                    m_bDialogTrans = !m_bDialogTrans;
#endif
                    break;
                case Keyboard::F12:
                    if (m_bInputStatus) return;
                    if (m_bCtrlPressed)
                        CreateScreenShot();
                    else
                    {
                        if (m_bIsDialogEnabled[19] == false)
                            EnableDialogBox(19, 0, 0, 0);
                        else DisableDialogBox(19);
                    }
                    break;
                case Keyboard::F1:
#ifdef DEF_SHORTCUT
                    UseShortCut(0);
#else
                    if (m_bIsDialogEnabled[35] == false)
                        EnableDialogBox(35, 0, 0, 0);
                    else
                    {
                        DisableDialogBox(35);
                        DisableDialogBox(18);
                    }
#endif
                    break;
                case Keyboard::Up:
                    if ((m_cGameMode == DEF_GAMEMODE_ONMAINGAME) && (m_bIsObserverMode))
                        bSendCommand(MSGID_REQUEST_PANNING, 0, 1, 0, 0, 0, 0);
                    else
                        m_cArrowPressed = 1;
                    break;
                    for (int x = DEF_MAXWHISPERMSG - 1; x >= 0; x--)
                    {
                        if (m_pWhisperMsg[x] != 0)
                        {
                            iTotalMsg = x;
                            break;
                        }
                    }
                    m_cWhisperIndex++;
                    if (m_cWhisperIndex > iTotalMsg) m_cWhisperIndex = 0;
                    if (m_cWhisperIndex < 0) m_cWhisperIndex = iTotalMsg;
                    if (m_pWhisperMsg[m_cWhisperIndex] != 0)
                    {
                        EndInputString();
                        format_to_local(m_cChatMsg, "/to {}", m_pWhisperMsg[m_cWhisperIndex]->message);
                        StartInputString(10, get_virtual_height() - 66, sizeof(m_cChatMsg), m_cChatMsg);
                    }
                    break;
                case Keyboard::Right:
                    if ((m_cGameMode == DEF_GAMEMODE_ONMAINGAME) && (m_bIsObserverMode))
                        bSendCommand(MSGID_REQUEST_PANNING, 0, 3, 0, 0, 0, 0);
                    else
                        m_cArrowPressed = 2;
                    break;
                case Keyboard::Down:
                    if ((m_cGameMode == DEF_GAMEMODE_ONMAINGAME) && (m_bIsObserverMode))
                        bSendCommand(MSGID_REQUEST_PANNING, 0, 5, 0, 0, 0, 0);
                    else
                        m_cArrowPressed = 3;
                    break;
                case Keyboard::Left:
                    if ((m_cGameMode == DEF_GAMEMODE_ONMAINGAME) && (m_bIsObserverMode))
                        bSendCommand(MSGID_REQUEST_PANNING, 0, 7, 0, 0, 0, 0);
                    else
                        m_cArrowPressed = 4;
                    break;
                case Keyboard::Tab:
                    if (m_bShiftPressed)
                    {
                        m_cCurFocus--;
                        if (m_cCurFocus < 1) m_cCurFocus = m_cMaxFocus;
                    }
                    else
                    {
                        m_cCurFocus++;
                        if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                    }

                    if (m_cGameMode == DEF_GAMEMODE_ONMAINGAME)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
                    }
                    break;
                case Keyboard::Return:
                    m_bEnterPressed = true;

                    if (event.key.alt)
                    {
                        // todo: recreate surfaces to resize to recenter area
                        fullscreen = !fullscreen;
                        create_renderer();
                        captured = true;
                        window.setMouseCursorGrabbed(clipmousegame);
                        window.setMouseCursorVisible(false);
                    }
                    else
                    {
                        m_bEnterPressed = true;
                    }
                    break;
                case Keyboard::Home:
                    if (m_cGameMode == DEF_GAMEMODE_ONMAINGAME)
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TOGGLESAFEATTACKMODE, 0, 0, 0, 0, 0);
                    break;
                case Keyboard::Escape:
                    // todo: 
//                     clipmousegame = !clipmousegame;
//                     window.setMouseCursorGrabbed(clipmousegame);
//                     captured = true;

                    m_bEscPressed = true;
                    if (m_cGameMode == DEF_GAMEMODE_ONMAINGAME)
                    {
                        if ((m_bIsObserverMode == true) && (m_bShiftPressed))
                        {
                            // Log Out
                            if (m_cLogOutCount == -1) m_cLogOutCount = 1;
                            DisableDialogBox(19);
                            PlaySound('E', 14, 5);
                        }
                        else if (m_cLogOutCount != -1)
                        {
                            if (m_bForceDisconn == false)
                            {
                                m_cLogOutCount = -1;
                                AddEventList(DLGBOX_CLICK_SYSMENU2, 10);
                            }
                        }
                        if (m_bIsGetPointingMode == true)
                        {
                            m_bIsGetPointingMode = false;
                            ClearCoords();
                            AddEventList(COMMAND_PROCESSOR1, 10);
                        }
                        m_bIsF1HelpWindowEnabled = false;
                    }
                    break;
                case Keyboard::PageDown:
                    if (m_cGameMode != DEF_GAMEMODE_ONMAINGAME) return;
                    if (m_bInputStatus) return;

                    if (m_bIsSpecialAbilityEnabled == true)
                    {
                        if (m_iSpecialAbilityType != 0)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_ACTIVATESPECABLTY, 0, 0, 0, 0, 0);
                            m_bIsSpecialAbilityEnabled = false;
                        }
                        else AddEventList(ON_KEY_UP26, 10);
                    }
                    else
                    {
                        if (m_iSpecialAbilityType == 0) AddEventList(ON_KEY_UP26, 10);
                        else
                        {
                            if ((m_sPlayerAppr4 & 0x00F0) != 0)
                            {
                                AddEventList(ON_KEY_UP28, 10);
                                return;
                            }

                            int64_t i = (dwTime - m_dwSpecialAbilitySettingTime) / 1000;
                            i = m_iSpecialAbilityTimeLeftSec - i;
                            if (i < 0) i = 0;

                            memset(G_cTxt, 0, sizeof(G_cTxt));
                            if (i < 60)
                            {
                                switch (m_iSpecialAbilityType)
                                {
                                    case 1: format_to_local(G_cTxt, ON_KEY_UP29, i); break;
                                    case 2: format_to_local(G_cTxt, ON_KEY_UP30, i); break;
                                    case 3: format_to_local(G_cTxt, ON_KEY_UP31, i); break;
                                    case 4: format_to_local(G_cTxt, ON_KEY_UP32, i); break;
                                    case 5: format_to_local(G_cTxt, ON_KEY_UP33, i); break;
                                    case 50:format_to_local(G_cTxt, ON_KEY_UP34, i); break;
                                    case 51:format_to_local(G_cTxt, ON_KEY_UP35, i); break;
                                    case 52:format_to_local(G_cTxt, ON_KEY_UP36, i); break;
                                }
                            }
                            else
                            {
                                switch (m_iSpecialAbilityType)
                                {
                                    case 1: format_to_local(G_cTxt, ON_KEY_UP37, i / 60); break;
                                    case 2: format_to_local(G_cTxt, ON_KEY_UP38, i / 60); break;
                                    case 3: format_to_local(G_cTxt, ON_KEY_UP39, i / 60); break;
                                    case 4: format_to_local(G_cTxt, ON_KEY_UP40, i / 60); break;
                                    case 5: format_to_local(G_cTxt, ON_KEY_UP41, i / 60); break;
                                    case 50:format_to_local(G_cTxt, ON_KEY_UP42, i / 60); break;
                                    case 51:format_to_local(G_cTxt, ON_KEY_UP43, i / 60); break;
                                    case 52:format_to_local(G_cTxt, ON_KEY_UP44, i / 60); break;
                                }
                            }
                            AddEventList(G_cTxt, 10);
                        }
                    }
                    break;

#ifdef DEF_ADMINCLIENT
                case 87: //'W'
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100], cLB, cRB;
                        short sX, sY, msX, msY, msZ;
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        memset(tempid, 0, sizeof(tempid));
                        m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                        if (m_bIsDialogEnabled[10] == true && (msX >= sX + 20) && (msX <= sX + 360) && (msY >= sY + 35) && (msY <= sY + 139))
                        {
                            CStrTok * pStrTok;
                            char * token, cBuff[64];
                            char   seps[] = ":";
                            int i = (139 - msY + sY) / 13;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] == 0) return;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg[0] == ' ') i++;
                            strcpy(cBuff, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg);
                            pStrTok = new CStrTok(cBuff, seps);
                            token = pStrTok->pGet();
                            format_to_local(tempid, "/summonplayer {}", token);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                            delete pStrTok;
                        }
                        else if (m_bIsDialogEnabled[72] == true && (m_cHighlightedChar[0] != 0))
                        {
                            format_to_local(tempid, "/summonplayer {}", m_cHighlightedChar);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                        else if (_tmp_sOwnerType < 7 && (strlen(_tmp_cName) > 0) && (m_iIlusionOwnerH == 0) && ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
                        {
                            format_to_local(tempid, "/summonplayer {}", _tmp_cName);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                    }
                    break;

                case 76: //'L'
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100], cLB, cRB;
                        short sX, sY, msX, msY, msZ;
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        memset(tempid, 0, sizeof(tempid));
                        m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                        if (m_bIsDialogEnabled[10] == true && (msX >= sX + 20) && (msX <= sX + 360) && (msY >= sY + 35) && (msY <= sY + 139))
                        {
                            CStrTok * pStrTok;
                            char * token, cBuff[64];
                            char   seps[] = ":";
                            int i = (139 - msY + sY) / 13;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] == 0) return;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg[0] == ' ') i++;
                            strcpy(cBuff, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg);
                            pStrTok = new CStrTok(cBuff, seps);
                            token = pStrTok->pGet();
                            format_to_local(tempid, "/closeconn {}", token);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                            delete pStrTok;
                        }
                        else if (m_bIsDialogEnabled[72] == true && (m_cHighlightedChar[0] != 0))
                        {
                            format_to_local(tempid, "/closeconn {}", m_cHighlightedChar);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                        else if (_tmp_sOwnerType < 7 && (strlen(_tmp_cName) > 0) && (m_iIlusionOwnerH == 0) && ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
                        {
                            format_to_local(tempid, "/closeconn {}", _tmp_cName);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                    }
                    break;
                case 66: //'B'
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100], cLB, cRB;
                        short sX, sY, msX, msY, msZ;
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        memset(tempid, 0, sizeof(tempid));
                        m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                        if (m_bIsDialogEnabled[10] == true && (msX >= sX + 20) && (msX <= sX + 360) && (msY >= sY + 35) && (msY <= sY + 139))
                        {
                            CStrTok * pStrTok;
                            char * token, cBuff[64];
                            char   seps[] = ":";
                            int i = (139 - msY + sY) / 13;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] == 0) return;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg[0] == ' ') i++;
                            strcpy(cBuff, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg);
                            pStrTok = new CStrTok(cBuff, seps);
                            token = pStrTok->pGet();
                            format_to_local(tempid, "/rape {}", token);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                            delete pStrTok;
                        }
                        else if (_tmp_sOwnerType < 7 && (strlen(_tmp_cName) > 0) && (m_iIlusionOwnerH == 0) && ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
                        {
                            format_to_local(tempid, "/rape {}", _tmp_cName);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                    }
                    break;
                case 75: //'K'
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100], cLB, cRB;
                        short sX, sY, msX, msY, msZ;
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        memset(tempid, 0, sizeof(tempid));
                        m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                        if (m_bIsDialogEnabled[10] == true && (msX >= sX + 20) && (msX <= sX + 360) && (msY >= sY + 35) && (msY <= sY + 139))
                        {
                            CStrTok * pStrTok;
                            char * token, cBuff[64];
                            char   seps[] = ":";
                            int i = (139 - msY + sY) / 13;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] == 0) return;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg[0] == ' ') i++;
                            strcpy(cBuff, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg);
                            pStrTok = new CStrTok(cBuff, seps);
                            token = pStrTok->pGet();
                            format_to_local(tempid, "/revive {} -1 5000", token);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                            delete pStrTok;
                        }
                        else if (m_bIsDialogEnabled[72] == true && (m_cHighlightedChar[0] != 0))
                        {
                            format_to_local(tempid, "/revive {} -1 5000", m_cHighlightedChar);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                        else if (_tmp_sOwnerType < 7 && (strlen(_tmp_cName) > 0) && (m_iIlusionOwnerH == 0) && ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
                        {
                            format_to_local(tempid, "/revive {} -1 5000", _tmp_cName);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                    }
                    break;
#endif

#ifdef DEF_HACKCLIENT
                case 70:
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        format_to_local(G_cTxt, "/setobservermode ");
                        bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, G_cTxt);
                    }
                    break;

                case 81://'Q'
                    if ((m_bCtrlPressed == true) && (m_cGameMode == DEF_GAMEMODE_ONMAINGAME))
                    {
                        memset(m_cChatMsg, 0, sizeof(m_cChatMsg));
                        strcpy(m_cChatMsg, "/enableadmincreateitem 1954 ");
                        //			strcpy(m_cChatMsg, "/enableadmincreateitem loco123 ");
                        StartInputString(10, get_virtual_height() - 66, sizeof(m_cChatMsg), m_cChatMsg);
                        //ClearInputString();
                    }
                    break;
                    //#endif

                case 86://'V'
                    if ((m_bCtrlPressed == true) && (m_cGameMode == DEF_GAMEMODE_ONMAINGAME))
                        m_cCommand = 7;
                    break;
                    //case 49://'1'
                    //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) )
                    //		m_cCommand = 1;
                    //	break;
                    //case 50://'2'
                    //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) )
                    //		m_cCommand = 2;
                    //	break;
                    //case 51://'3'
                    //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) )
                    //		m_cCommand = 3;
                    //	break;
                    //case 52://'4'
                    //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) )
                    //		m_cCommand = 4;
                    //	break;
                    //case 53://'5'
                    //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) )
                    //		m_cCommand = 5;
                    //	break;
                    //case 54://'6'
                    //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) )
                    //		m_cCommand = 6;
                    //	break;
                    //case 55://'7'
                    //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) )
                    //		m_cCommand = 7;
                    //	break;
                    //case 56://'8'
                    //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) )
                    //		m_cCommand = 8;
                    //	break;
                    //case 57://'9'
                    //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) )
                    //		m_cCommand = 9;
                    //	break;

                case 88://'X'//Change Hack Spell
                    if ((m_bCtrlPressed == true) && (m_cGameMode == DEF_GAMEMODE_ONMAINGAME))
                    {
                        memset(m_cChatMsg, 0, sizeof(m_cChatMsg));
                        strcpy(m_cChatMsg, "/enableadmincommand 1954 ");
                        //			strcpy(m_cChatMsg, "/enableadmincommand loco123 ");
                        StartInputString(10, get_virtual_height() - 66, sizeof(m_cChatMsg), m_cChatMsg);
                        //ClearInputString();
                    }
                    break;

                    //Change
                //case 72://'H'
                //	if( ( m_bCtrlPressed == true ) && ( m_cGameMode == DEF_GAMEMODE_ONMAINGAME ) ) {
                //		memset(m_cChatMsg, 0, sizeof(m_cChatMsg) );
                //		strcpy(m_cChatMsg, "!www.helbreathx.net 2.20 client sources build");
                //		StartInputString(10, get_virtual_height()-66, sizeof(m_cChatMsg), m_cChatMsg); 
                //		//ClearInputString();
                //	}
                //	break;

                case 69: //'E'
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100], cLB, cRB;
                        bSendCommand(MSGID_REQUEST_INITDATA, 0, 0, 0, 0, 0, 0);
                    }
                    break;

                case 80: //'P'
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100], cLB, cRB;
                        short sX, sY, msX, msY, msZ;
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        memset(tempid, 0, sizeof(tempid));
                        m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                        if (m_bIsDialogEnabled[10] == true && (msX >= sX + 20) && (msX <= sX + 360) && (msY >= sY + 35) && (msY <= sY + 139))
                        {
                            CStrTok * pStrTok;
                            char * token, cBuff[64];
                            char   seps[] = ":";
                            int i = (139 - msY + sY) / 13;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] == 0) return;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg[0] == ' ') i++;
                            strcpy(cBuff, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg);
                            pStrTok = new CStrTok(cBuff, seps);
                            token = pStrTok->pGet();
                            format_to_local(tempid, "/send {} fightzone4", token);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                            delete pStrTok;
                        }
                        else if (_tmp_sOwnerType < 7 && (strlen(_tmp_cName) > 0) && (m_iIlusionOwnerH == 0) && ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
                        {
                            format_to_local(tempid, "/send {} fightzone4", _tmp_cName);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                    }
                    break;

                case 79: //'O'
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100], cLB, cRB;
                        short sX, sY, msX, msY, msZ;
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        memset(tempid, 0, sizeof(tempid));
                        m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                        if (m_bIsDialogEnabled[10] == true && (msX >= sX + 20) && (msX <= sX + 360) && (msY >= sY + 35) && (msY <= sY + 139))
                        {
                            CStrTok * pStrTok;
                            char * token, cBuff[64];
                            char   seps[] = ":";
                            int i = (139 - msY + sY) / 13;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] == 0) return;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg[0] == ' ') i++;
                            strcpy(cBuff, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg);
                            pStrTok = new CStrTok(cBuff, seps);
                            token = pStrTok->pGet();
                            format_to_local(tempid, "/send {} fightzone3", token);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                            delete pStrTok;
                        }
                        else if (_tmp_sOwnerType < 7 && (strlen(_tmp_cName) > 0) && (m_iIlusionOwnerH == 0) && ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
                        {
                            format_to_local(tempid, "/send {} fightzone3", _tmp_cName);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                    }
                    break;

                case 73: //'I'
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100], cLB, cRB;
                        short sX, sY, msX, msY, msZ;
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        memset(tempid, 0, sizeof(tempid));
                        m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                        if (m_bIsDialogEnabled[10] == true && (msX >= sX + 20) && (msX <= sX + 360) && (msY >= sY + 35) && (msY <= sY + 139))
                        {
                            CStrTok * pStrTok;
                            char * token, cBuff[64];
                            char   seps[] = ":";
                            int i = (139 - msY + sY) / 13;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] == 0) return;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg[0] == ' ') i++;
                            strcpy(cBuff, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg);
                            pStrTok = new CStrTok(cBuff, seps);
                            token = pStrTok->pGet();
                            format_to_local(tempid, "/send {} fightzone2", token);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                            delete pStrTok;
                        }
                        else if (_tmp_sOwnerType < 7 && (strlen(_tmp_cName) > 0) && (m_iIlusionOwnerH == 0) && ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
                        {
                            format_to_local(tempid, "/send {} fightzone2", _tmp_cName);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                    }
                    break;

                case 85: //'U'
                    if (m_bCtrlPressed == true && m_cGameMode == DEF_GAMEMODE_ONMAINGAME && (!m_bInputStatus))
                    {
                        char tempid[100], cLB, cRB;
                        short sX, sY, msX, msY, msZ;
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        memset(tempid, 0, sizeof(tempid));
                        m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
                        if (m_bIsDialogEnabled[10] == true && (msX >= sX + 20) && (msX <= sX + 360) && (msY >= sY + 35) && (msY <= sY + 139))
                        {
                            CStrTok * pStrTok;
                            char * token, cBuff[64];
                            char   seps[] = ":";
                            int i = (139 - msY + sY) / 13;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] == 0) return;
                            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg[0] == ' ') i++;
                            strcpy(cBuff, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_pMsg);
                            pStrTok = new CStrTok(cBuff, seps);
                            token = pStrTok->pGet();
                            format_to_local(tempid, "/send {} fightzone1", token);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                            delete pStrTok;
                        }
                        else if (_tmp_sOwnerType < 7 && (strlen(_tmp_cName) > 0) && (m_iIlusionOwnerH == 0) && ((m_bIsCrusadeMode == false) || _iGetFOE(_tmp_iStatus) >= 0))
                        {
                            format_to_local(tempid, "/send {} fightzone1", _tmp_cName);
                            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, tempid);
                        }
                    }
                    break;

                case 72:
                    if (m_bInputStatus) return;
                    if (m_bCtrlPressed == true) EnableDialogBox(35, 0, 0, 0);//Help Request
                    break;
#endif

                default:
                    if (m_cGameMode == DEF_GAMEMODE_ONMAINGAME)
                    {
                        if (m_bCtrlPressed)
                        {
                            switch (event.key.code)
                            {
                                case Keyboard::Num0: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 9; break; // 0
                                case Keyboard::Num1: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 0; break; // 1
                                case Keyboard::Num2: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 1; break; // 2 
                                case Keyboard::Num3: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 2; break; // 3
                                case Keyboard::Num4: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 3; break; // 4 
                                case Keyboard::Num5: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 4; break; // 5
                                case Keyboard::Num6: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 5; break; // 6
                                case Keyboard::Num7: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 6; break; // 7
                                case Keyboard::Num8: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 7; break; // 8
                                case Keyboard::Num9: EnableDialogBox(3, 0, 0, 0); m_stDialogBoxInfo[3].sView = 8; break; // 9
                            }//Change was uncommented
                        }
                        else if ((m_bInputStatus == false) && (m_altPressed == false))
                        {
                            if (!m_bIsObserverMode)
                            {
                                StartInputString(10, get_virtual_height() - 66, sizeof(m_cChatMsg), m_cChatMsg);
                                ClearInputString();
                            }
                        }
                    }
                    break;
            }
            break;
        }
        case sf::Event::KeyReleased:
        {
            if (event.key.code == Keyboard::Backspace)
            {
            }
            if (event.key.code == Keyboard::Tab)
            {
            }

            switch (event.key.code)
            {
                case Keyboard::LShift:
                    m_bShiftPressed = false;
                    break;
                case Keyboard::LControl:
                    m_bCtrlPressed = false;
                    break;
                case Keyboard::LAlt:
                    m_altPressed = false;
                    break;
            }
            break;
        }
        case sf::Event::Resized:
            break;
        case sf::Event::LostFocus:
            window.setFramerateLimit(frame_limit_bg); //set to var
            break;
        case sf::Event::GainedFocus:
            if (m_cGameMode != DEF_GAMEMODE_ONLOADING)
                window.setFramerateLimit(frame_limit);
            else
                window.setFramerateLimit(0);
            break;
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                if (event.mouseWheelScroll.delta > 0.f)
                    m_stMCursor.sZ++;
                else if (event.mouseWheelScroll.delta < 0.f)
                    m_stMCursor.sZ--;
            }
            else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
            {
                if (event.mouseWheelScroll.delta > 0.f)
                    m_stMCursor.sZ2++;
                else if (event.mouseWheelScroll.delta < 0.f)
                    m_stMCursor.sZ2--;
            }
            else
            {
            }
            break;
        case sf::Event::MouseButtonPressed:
        {
            if (wasinactive)
            {
                wasinactive = false;
            }

            //             for (auto & rect : dialogs)
            //             {
            //                 if (rect.contains(m_stMCursor.sX, m_stMCursor.sY))
            //                 {
            //                     break;
            //                 }
            //             }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                m_stMCursor.RB = true;
            }
            else if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_stMCursor.LB = true;
            }
            else if (event.mouseButton.button == sf::Mouse::Middle)
            {
                m_stMCursor.MB = true;
            }
            break;
        }
        case sf::Event::MouseButtonReleased:
        {
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                m_stMCursor.RB = false;
            }
            else if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_stMCursor.LB = false;
            }
            else if (event.mouseButton.button == sf::Mouse::Middle)
            {
                m_stMCursor.MB = false;
            }
            break;
        }
        case sf::Event::MouseMoved:
        {
            float diffx = static_cast<float>(screenwidth_v) / screenwidth;
            float diffy = static_cast<float>(screenheight_v) / screenheight;
            uint16_t x = uint16_t(event.mouseMove.x * diffx);
            uint16_t y = uint16_t(event.mouseMove.y * diffy);

            // if admin client
            if (m_stMCursor.LB && m_bCtrlPressed)
            {
                // pan event
                pan_x += event.mouseMove.x - m_stMCursor.sX;
                pan_y += event.mouseMove.y - m_stMCursor.sY;
            }

            m_stMCursor.sX = x;
            m_stMCursor.sY = y;

            //std::cout << fmt::format("{:#4}, {:#4} || {:#4}, {:#4}\n", event.mouseMove.x, event.mouseMove.y, x, y);
            break;
        }
    }
}

void CGame::create_surfaces()
{
    visible.create(screenwidth_v, screenheight_v);
    bg.create(screenwidth_v + 300, screenheight_v + 300);
    dynamic_bg.create(screenwidth_v + 300, screenheight_v + 300);
    dialog.create(screenwidth_v + 300, screenheight_v + 300);
    item_box.create(screenwidth_v + 300, screenheight_v + 300);
    map_text.create(screenwidth_v + 300, screenheight_v + 300);
    //bg.create(32768, 32768);
    //charselect.create(256, 256);
}

void CGame::create_fonts()
{
    sf::Font s;
    s.loadFromFile(workingdirectory + "fonts/Arya-Regular.ttf");
    _font.insert(std::pair<std::string, sf::Font>("arya", s));

    s.loadFromFile(workingdirectory + "fonts/OpenSans-Regular.ttf");
    _font.insert(std::pair<std::string, sf::Font>("default", s));

    s.loadFromFile(workingdirectory + "fonts/PoetsenOne-Regular.ttf");
    _font.insert(std::pair<std::string, sf::Font>("test", s));

    _text.setFont(_font.at("arya"));
    arya_font = &_font.at("arya");
    default_font = &_font.at("default");
    test_font = &_font.at("test");

    chat_window_text.setFont(*arya_font);
    chat_window_text.setCharacterSize(12);

    overhead_text.setFont(*test_font);
    overhead_text.setCharacterSize(12);
    overhead_text.setOutlineColor(sf::Color::Black);
    overhead_text.setOutlineThickness(2);

    under_text.setFont(*arya_font);
    under_text.setCharacterSize(12);
    under_text.setOutlineColor(sf::Color::Black);
    under_text.setOutlineThickness(1);
}

void CGame::create_window()
{
    if (window.isOpen()) window.close();

    format_to_local(winName, "Helbreath Xtreme {}.{}.{} Renderer: {}", DEF_UPPERVERSION, DEF_LOWERVERSION, DEF_PATCHVERSION, _renderer);
    sf::ContextSettings context;
    context.antialiasingLevel = antialiasing;
    context.majorVersion = 2;
    context.minorVersion = 0;

    if (fullscreen)
    {
        screenwidth_windowed = screenwidth;
        screenheight_windowed = screenheight;
        auto mode = sf::VideoMode::getDesktopMode();
        screenwidth = mode.width;
        screenheight = mode.height;
        window.create(sf::VideoMode(screenwidth, screenheight), winName, (fullscreen ? sf::Style::Fullscreen : (sf::Style::Close)), context);
    }
    else
    {
        screenwidth = screenwidth_windowed;
        screenheight = screenheight_windowed;
        window.create(sf::VideoMode(screenwidth, screenheight), winName, (fullscreen ? sf::Style::Fullscreen : (sf::Style::Close)), context);
    }

    handle = window.getSystemHandle();

    window.setVerticalSyncEnabled(vsync);

    frame_limit = 0;
}

bool CGame::create_renderer()
{
    create_window();
    create_surfaces();
    create_fonts();
    create_load_list();

    std::string vertexShaderSource = R"(void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
})";
    std::string fragmentShaderSource = R"(uniform sampler2D texture;
uniform float time;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    float red = sin(time) * 0.5 + 0.5;
    float green = sin(time + 2.09439) * 0.5 + 0.5;
    float blue = sin(time + 4.18879) * 0.5 + 0.5;

    vec4 rainbowColor = vec4(red, green, blue, 1.0);

    // multiply it by the color
    gl_FragColor = pixel * rainbowColor;
})";
    shader.loadFromMemory(vertexShaderSource, sf::Shader::Vertex);
    shader.loadFromMemory(fragmentShaderSource, sf::Shader::Fragment);

    init_dialogs();

    modx = get_virtual_width() / 2 - 640 / 2;
    mody = get_virtual_height() / 2 - 480 / 2;

    return true;
}

int CGame::send_message(char * cData, std::size_t dwSize, char cKey)
{
    return write(cData, dwSize);
}

std::string CGame::get_game_mode_str()
{
    for (auto & mode : game_mode_map)
    {
        if (mode.first == m_cGameMode)
            return mode.second;
    }
    return "unknown";
}

std::string CGame::get_game_mode(int _gamemode)
{
    for (auto & mode : game_mode_map)
    {
        if (mode.first == _gamemode)
            return mode.second;
    }
    return "unknown";
}

int16_t CGame::get_game_mode(std::string _gamemode)
{
    for (auto & mode : game_mode_map)
    {
        if (mode.second == _gamemode)
            return mode.first;
    }
    return DEF_GAMEMODE_NULL;
}

void CGame::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color)
{ //TODO: replace all instances
    draw_line(x0, y0, x1, y1, color.r, color.g, color.b, color.a);
}

void CGame::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, int iR, int iG, int iB, int iA)
{
    if ((x0 == x1) && (y0 == y1))
        return;

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x0, y0), Color(iR, iG, iB, iA)),
        sf::Vertex(sf::Vector2f(x1, y1), Color(iR, iG, iB, iA))
    };
    visible.draw(line, 2, sf::Lines);
}

void CGame::update_mouse_state(uint16_t & x, uint16_t & y, uint16_t & z, char & left_button, char & right_button)
{
    x = m_stMCursor.sX;
    y = m_stMCursor.sY;
    z = m_stMCursor.sZ;
    left_button = m_stMCursor.LB;
    right_button = m_stMCursor.RB;
}

void CGame::update_mouse_state(uint16_t & x, uint16_t & y, uint16_t & z, bool & left_button, bool & right_button, bool & middle_button)
{
    x = m_stMCursor.sX;
    y = m_stMCursor.sY;
    z = m_stMCursor.sZ;
    left_button = m_stMCursor.LB;
    right_button = m_stMCursor.RB;
    middle_button = m_stMCursor.MB;
}

void CGame::draw_shadow_box(uint16_t sX, uint16_t sY, uint16_t dX, uint16_t dY, Color color)
{
    if ((sX == dX) && (sY == dY))
        return;

    sf::RectangleShape rectangle(sf::Vector2f(float(dX - sX), float(dY - sY)));
    rectangle.setPosition((float)sX, (float)sY);
    rectangle.setFillColor(color);
    visible.draw(rectangle);
}

void CGame::put_pixel(uint16_t sX, uint16_t sY, Color color)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f((float)sX, (float)sY), color)
    };
    visible.draw(line, 1, sf::Points);
}

void CGame::change_display_mode()
{
    fullscreen = !fullscreen;
    create_renderer();
}

void CGame::set_mouse_cursor(mouse_cursor cursor)
{
    m_stMCursor.sCursorFrame = static_cast<int>(cursor);
}

void CGame::render_mouse(uint16_t mx, uint16_t my)
{
    if (m_bIsObserverMode == true)
    {
        draw_line(mx - 5, my, mx + 5, my, Color(255, 0, 0));
        draw_line(mx, my, mx, my + 5, Color(255, 0, 0));

        draw_line(mx - 5, my - 1, mx + 5, my - 1, Color(255, 0, 0, 127));
        draw_line(mx - 1, my - 5, mx - 1, my + 5, Color(255, 0, 0, 127));

        draw_line(mx - 5, my + 1, mx + 5, my + 1, Color(255, 0, 0, 127));
        draw_line(mx + 1, my - 5, mx + 1, my + 5, Color(255, 0, 0, 127));
    }
    else
    {
        static sf::Clock clock;
        float time = clock.getElapsedTime().asSeconds();
        sprite * spr = m_pSprite[DEF_SPRID_MOUSECURSOR];
        if (spr->m_bIsSurfaceEmpty) spr->open_sprite_();
        spr->sprite_[m_stMCursor.sCursorFrame].setPosition(float(mx + spr->brush[m_stMCursor.sCursorFrame].pvx), float(my + spr->brush[m_stMCursor.sCursorFrame].pvy));

        if (m_stMCursor.use_shader)
        {
            shader.setUniform("texture", sf::Shader::CurrentTexture);
            shader.setUniform("time", time);
            window.draw(spr->sprite_[m_stMCursor.sCursorFrame], &shader);
        }
        else
            window.draw(spr->sprite_[m_stMCursor.sCursorFrame]);
    }
}

char CGame::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, bool bMoveCheck, bool bMIM)
{
    char  cDir, cTmpDir;
    //int   aX, aY, aX2, aY2, dX, dY;
    int   aX, aY, dX, dY;
    int   i;

    if ((sX == dstX) && (sY == dstY)) return 0;

    dX = sX;
    dY = sY;

    cDir = m_Misc.cGetNextMoveDir(dX, dY, dstX, dstY, bMIM);

    if (m_cPlayerTurn == 0)
        for (i = cDir; i <= cDir + 2; i++)
        {
            cTmpDir = i;
            if (cTmpDir > 8) cTmpDir -= 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (((dX + aX) == m_iPrevMoveX) && ((dY + aY) == m_iPrevMoveY) && (m_bIsPrevMoveBlocked == true) && (bMoveCheck == true))
            {
                m_bIsPrevMoveBlocked = false;
            }
            else if (m_pMapData->bGetIsLocateable(dX + aX, dY + aY) == true)
            {
                if (m_pMapData->bIsTeleportLoc(dX + aX, dY + aY) == true)
                {
                    if (_bCheckMoveable(dX + aX, dY + aY) == true) return cTmpDir;
                    else
                    {
                        SetTopMsg(DEF_MSG_GETNEXTMOVEDIR, 5);
                    }
                }
                else return cTmpDir;
            }
        }

    if (m_cPlayerTurn == 1)
        for (i = cDir; i >= cDir - 2; i--)
        {
            cTmpDir = i;
            if (cTmpDir < 1) cTmpDir += 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (((dX + aX) == m_iPrevMoveX) && ((dY + aY) == m_iPrevMoveY) && (m_bIsPrevMoveBlocked == true) && (bMoveCheck == true))
            {
                m_bIsPrevMoveBlocked = false;
            }
            else if (m_pMapData->bGetIsLocateable(dX + aX, dY + aY) == true)
            {
                if (m_pMapData->bIsTeleportLoc(dX + aX, dY + aY) == true)
                {
                    if (_bCheckMoveable(dX + aX, dY + aY) == true) return cTmpDir;
                    else
                    {
                        SetTopMsg(DEF_MSG_GETNEXTMOVEDIR, 5);
                    }
                }
                else return cTmpDir;
            }
        }
    return 0;
}
