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
#include "item.h"
#include "map_data.h"
#include "lan_eng.h"
#include "msg.h"
#include "magic.h"
#include "skill.h"
#include "build_item.h"

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


bool CGame::_bCheckDlgBoxClick(short msX, short msY)
{
    int i;
    char         cDlgID;

    m_stMCursor.sZ = 0;
    for (i = 0; i < 101; i++)
        if (m_cDialogBoxOrder[100 - i] != 0)
        {
            cDlgID = m_cDialogBoxOrder[100 - i];
            if ((m_stDialogBoxInfo[cDlgID].sX < msX) && ((m_stDialogBoxInfo[cDlgID].sX + m_stDialogBoxInfo[cDlgID].sSizeX) > msX) &&
                (m_stDialogBoxInfo[cDlgID].sY < msY) && ((m_stDialogBoxInfo[cDlgID].sY + m_stDialogBoxInfo[cDlgID].sSizeY) > msY))
            {
                switch (cDlgID)
                {
                    case 1:
                        DlgBoxClick_Character(msX, msY);
                        break;
                    case 2:
                        DlgBoxClick_Inventory(msX, msY);
                        break;
                    case 3:
                        DlgBoxClick_Magic(msX, msY);
                        break;
                    case 4:
                        DlgBoxClick_ItemDrop(msX, msY);
                        break;
                    case 5:
                        DlgBoxClick_15AgeMsg(msX, msY);
                        break;
                    case 6:
                        DlgBoxClick_WarningMsg(msX, msY);
                        break;
                    case 7:
                        DlgBoxClick_GuildMenu(msX, msY);
                        break;
                    case 8:
                        DlgBoxClick_GuildOp(msX, msY);
                        break;
                    case 9:
                        break;
                    case 11:
                        DlgBoxClick_Shop(msX, msY);
                        break;
                    case 12:
                        DlgBoxClick_LevelUpSettings(msX, msY);
                        break;
                    case 13:
                        DlgBoxClick_CityhallMenu(msX, msY);
                        break;
                    case 14:
                        DlgBoxClick_Bank(msX, msY);
                        break;
                    case 15:
                        DlgBoxClick_Skill(msX, msY);
                        break;
                    case 16:
                        DlgBoxClick_MagicShop(msX, msY);
                        break;
                    case 18:
                        DlgBoxClick_Text(msX, msY);
                        break;
                    case 19:
                        DlgBoxClick_SysMenu(msX, msY);
                        break;
                    case 20:
                        DlgBoxClick_NpcActionQuery(msX, msY);
                        break;
                    case 21:
                        DlgBoxClick_NpcTalk(msX, msY);
                        break;
                    case 23:
                        DlgBoxClick_ItemSellorRepair(msX, msY);
                        break;
                    case 24:
                        DlgBoxClick_Fish(msX, msY);
                        break;
                    case 25:
                        DlgBoxClick_ShutDownMsg(msX, msY);
                        break;
                    case 26:
                        DlgBoxClick_SkillDlg(msX, msY);
                        break;
                    case 27:
                        DlgBoxClick_Exchange(msX, msY);
                        break;
                    case 28:
                        DlgBoxClick_Quest(msX, msY);
                        break;
                    case 30:
                        DlgBoxClick_IconPannel(msX, msY);
                        break;
                    case 31:
                        DlgBoxClick_SellList(msX, msY);
                        break;
                    case 32:
                        DlgBoxClick_Party(msX, msY);
                        break;
                    case 33:
                        DlgBoxClick_CrusadeJob(msX, msY);
                        break;
                    case 34:
                        DlgBoxClick_ItemUpgrade(msX, msY);
                        break;
                    case 35:
                        DlgBoxClick_Help(msX, msY);
                        break;

                    case 36:
                        DlgBoxClick_Commander(msX, msY);
                        break;

                    case 37:
                        DlgBoxClick_Constructor(msX, msY);
                        break;

                    case 38:
                        DlgBoxClick_Soldier(msX, msY);
                        break;

                    case 40:
                        DlgBoxClick_Slates(msX, msY);
                        break;
                    case 41:
                        DlgBoxClick_ConfirmExchange(msX, msY);
                        break;
                    case 42:
                        DlgBoxClick_ChangeStatsMajestic(msX, msY);
                        break;
                    case 50:
                        DlgBoxClick_Resurrect(msX, msY);
                        break;
                    case 51:
                        DlgBoxClick_CMDHallMenu(msX, msY);
                        break;


                        //Dialog
                    case 70:
                        DlgBoxClick_FriendsList(msX, msY);
                        break;
                        //Change Admin Dialog
#ifdef DEF_ADMINCLIENT
                    case 71:
                        DlgBoxClick_AdminControl(msX, msY);
                        break;
                    case 72:
                        DlgBoxClick_UserList(msX, msY);
                        break;
#endif
                    case 73:
                        DlgBoxClick_Config(msX, msY);
                        break;
                }

                return true;
            }
        }

    return false;
}

bool CGame::_bCheckDlgBoxDoubleClick(short msX, short msY)
{
    int i;
    char cDlgID;
    //if (m_iHP <= 0) return false;

    for (i = 0; i < 101; i++)
        if (m_cDialogBoxOrder[100 - i] != 0)
        {
            cDlgID = m_cDialogBoxOrder[100 - i];
            if ((m_stDialogBoxInfo[cDlgID].sX < msX) && ((m_stDialogBoxInfo[cDlgID].sX + m_stDialogBoxInfo[cDlgID].sSizeX) > msX) &&
                (m_stDialogBoxInfo[cDlgID].sY < msY) && ((m_stDialogBoxInfo[cDlgID].sY + m_stDialogBoxInfo[cDlgID].sSizeY) > msY))
            {
                switch (cDlgID)
                {
                    case 1:
                        DlbBoxDoubleClick_Character(msX, msY);
                        break;
                    case 2:
                        DlbBoxDoubleClick_Inventory(msX, msY);
                        break;
                    case 9:
                        DlbBoxDoubleClick_GuideMap(msX, msY);
                        break;
                }
                return true;
            }
        }
    return false;
}


bool CGame::bDlgBoxPress_Inventory(short msX, short msY)
{
    int i;
    char  cItemID;
    short sX, sY, x1, x2, y1, y2;

#ifdef _DEBUG
    AddEventList("Press Inventory", 10);
#endif

    if (m_bIsDialogEnabled[2] == false) return false;
    if (m_bIsDialogEnabled[17] == true) return false;
    if (m_bIsDialogEnabled[4] == true) return false;

    sX = m_stDialogBoxInfo[2].sX;
    sY = m_stDialogBoxInfo[2].sY;

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_cItemOrder[DEF_MAXITEMS - 1 - i] != -1)
        {
            cItemID = m_cItemOrder[DEF_MAXITEMS - 1 - i];

            if (m_pItemList[cItemID] != 0)
            {

                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + 32 + m_pItemList[cItemID]->m_sX,
                    sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame);
                x1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
                y1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
                x2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
                y2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

                if ((m_bIsItemDisabled[cItemID] == false) && (m_bIsItemEquipped[cItemID] == false) && (msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
                {

                    if (m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->check_collison(sX + 32 + m_pItemList[cItemID]->m_sX, sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame, msX, msY) == true)
                    {
                        _SetItemOrder(0, cItemID);

                        if ((m_bIsGetPointingMode == true) && (m_iPointCommandType < 100) && (m_iPointCommandType >= 0) &&
                            (m_pItemList[m_iPointCommandType] != 0) &&
                            (m_pItemList[m_iPointCommandType]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST) &&
                            (m_iPointCommandType != cItemID))
                        {

                            PointCommandHandler(0, 0, cItemID);
                            //m_bCommandAvailable  = false;
                            m_bIsGetPointingMode = false;
                            //ClearCoords();
                        }
                        else
                        {
                            m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                            m_stMCursor.sSelectedObjectID = cItemID;
                            m_stMCursor.sDistX = msX - x1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                            m_stMCursor.sDistY = msY - y1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;
                        }
                        return true;
                    }
                }
            }
        }

    return false;
}


bool CGame::_bCheckDraggingItemRelease(short msX, short msY)
{
    int i;
    char         cDlgID;

    for (i = 0; i < 101; i++)
        if (m_cDialogBoxOrder[100 - i] != 0)
        {
            cDlgID = m_cDialogBoxOrder[100 - i];
            if ((m_stDialogBoxInfo[cDlgID].sX < msX) && ((m_stDialogBoxInfo[cDlgID].sX + m_stDialogBoxInfo[cDlgID].sSizeX) > msX) &&
                (m_stDialogBoxInfo[cDlgID].sY < msY) && ((m_stDialogBoxInfo[cDlgID].sY + m_stDialogBoxInfo[cDlgID].sSizeY) > msY))
            {
                EnableDialogBox(cDlgID, 0, 0, 0);

                switch (cDlgID)
                {
                    case 1:
                        bItemDrop_Character();
                        break;

                    case 2:
                        bItemDrop_Inventory(msX, msY);
                        break;

                    case 14:
                        bItemDrop_Bank(msX, msY);
                        break;

                    case 26:
                        bItemDrop_SkillDialog();
                        break;

                    case 27:
                        bItemDrop_ExchangeDialog(msX, msY);
                        break;

                    case 30:
                        bItemDrop_IconPannel(msX, msY);
                        break;

                    case 31:
                        bItemDrop_SellList(msX, msY);
                        break;

                    case 34:
                        bItemDrop_ItemUpgrade();
                        break;

                    case 40:
                        bItemDrop_Slates();
                        break;
                }

                return true;
            }
        }
    bItemDrop_ExternalScreen((char)m_stMCursor.sSelectedObjectID, msX, msY);

    return false;
}

void CGame::bItemDrop_ExternalScreen(char cItemID, short msX, short msY)
{

    char  cName[21];
    short sType, tX, tY;
    int iStatus;

    if (bCheckItemOperationEnabled(cItemID) == false) return;
    if ((m_sMCX != 0) && (m_sMCY != 0) && (abs(m_sPlayerX - m_sMCX) <= 8) && (abs(m_sPlayerY - m_sMCY) <= 8))
    {

        memset(cName, 0, sizeof(cName));
        m_pMapData->bGetOwner(m_sMCX, m_sMCY, cName, &sType, &iStatus, &m_wCommObjectID); // v1.4

        if (memcmp(m_cPlayerName, cName, 10) == 0)
        {

        }
        else
        {
            if (((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
                (m_pItemList[cItemID]->m_dwCount > 1))
            {
                m_stDialogBoxInfo[17].sX = msX - 140;
                m_stDialogBoxInfo[17].sY = msY - 70;
                if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;

                m_stDialogBoxInfo[17].sV1 = m_sMCX;
                m_stDialogBoxInfo[17].sV2 = m_sMCY;
                m_stDialogBoxInfo[17].sV3 = sType;
                // v1.4
                m_stDialogBoxInfo[17].sV4 = m_wCommObjectID;

                memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
                if (sType < 10)
                    memcpy(m_stDialogBoxInfo[17].cStr, cName, 10);
                else
                {
                    GetNpcName(sType, m_stDialogBoxInfo[17].cStr);
                }
                EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
            }
            else
            {
                switch (sType)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        EnableDialogBox(20, 1, cItemID, sType);
                        m_stDialogBoxInfo[20].sV3 = 1;
                        m_stDialogBoxInfo[20].sV4 = m_wCommObjectID;			// v1.4
                        m_stDialogBoxInfo[20].sV5 = m_sMCX;
                        m_stDialogBoxInfo[20].sV6 = m_sMCY;

                        tX = msX - 117;
                        tY = msY - 50;
                        if (tX < 0) tX = 0;
                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                        if (tY < 0) tY = 0;
                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                        m_stDialogBoxInfo[20].sX = tX;
                        m_stDialogBoxInfo[20].sY = tY;

                        memset(m_stDialogBoxInfo[20].cStr, 0, sizeof(m_stDialogBoxInfo[20].cStr));
                        strcpy(m_stDialogBoxInfo[20].cStr, cName);
                        //bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, cItemID, 1, m_sMCX, m_sMCY, m_pItemList[cItemID]->m_cName); //v1.4
                        break;

                    case 20:
                        EnableDialogBox(20, 3, cItemID, sType);
                        m_stDialogBoxInfo[20].sV3 = 1;
                        m_stDialogBoxInfo[20].sV4 = m_wCommObjectID; // v1.4
                        m_stDialogBoxInfo[20].sV5 = m_sMCX;
                        m_stDialogBoxInfo[20].sV6 = m_sMCY;

                        tX = msX - 117;
                        tY = msY - 50;
                        if (tX < 0) tX = 0;
                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                        if (tY < 0) tY = 0;
                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                        m_stDialogBoxInfo[20].sX = tX;
                        m_stDialogBoxInfo[20].sY = tY;

                        memset(m_stDialogBoxInfo[20].cStr, 0, sizeof(m_stDialogBoxInfo[20].cStr));
                        GetNpcName(sType, m_stDialogBoxInfo[20].cStr);
                        //bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, cItemID, 1, m_sMCX, m_sMCY, m_pItemList[cItemID]->m_cName); 
                        break;

                    case 15:

                    case 24:

                        EnableDialogBox(20, 2, cItemID, sType);
                        m_stDialogBoxInfo[20].sV3 = 1;
                        m_stDialogBoxInfo[20].sV4 = m_wCommObjectID; // v1.4
                        m_stDialogBoxInfo[20].sV5 = m_sMCX;
                        m_stDialogBoxInfo[20].sV6 = m_sMCY;

                        tX = msX - 117;
                        tY = msY - 50;
                        if (tX < 0) tX = 0;
                        if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                        if (tY < 0) tY = 0;
                        if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                        m_stDialogBoxInfo[20].sX = tX;
                        m_stDialogBoxInfo[20].sY = tY;

                        memset(m_stDialogBoxInfo[20].cStr, 0, sizeof(m_stDialogBoxInfo[20].cStr));
                        GetNpcName(sType, m_stDialogBoxInfo[20].cStr);
                        break;

                    default:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, cItemID, 1, m_sMCX, m_sMCY, m_pItemList[cItemID]->m_cName);
                        break;
                }
                //m_bIsItemDisabled[cItemID] = true;
            }
            m_bIsItemDisabled[cItemID] = true;
        }
    }
    else
    {
        if (((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
            (m_pItemList[cItemID]->m_dwCount > 1))
        {
            m_stDialogBoxInfo[17].sX = msX - 140;
            m_stDialogBoxInfo[17].sY = msY - 70;
            if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;

            m_stDialogBoxInfo[17].sV1 = 0;
            m_stDialogBoxInfo[17].sV2 = 0;

            m_stDialogBoxInfo[17].sV3 = 0; // v1.4
            m_stDialogBoxInfo[17].sV4 = 0;
            memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
            EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
        }
        else
        {

            if (_ItemDropHistory(m_pItemList[cItemID]->m_cName))
            {
                m_stDialogBoxInfo[4].sX = msX - 140;
                m_stDialogBoxInfo[4].sY = msY - 70;

                if (m_stDialogBoxInfo[4].sY < 0)
                    m_stDialogBoxInfo[4].sY = 0;
                m_stDialogBoxInfo[4].sV1 = 0;
                m_stDialogBoxInfo[4].sV2 = 0;

                m_stDialogBoxInfo[4].sV3 = 1; // v1.4
                m_stDialogBoxInfo[4].sV4 = 0;
                m_stDialogBoxInfo[4].sV5 = cItemID;

                memset(m_stDialogBoxInfo[4].cStr, 0, sizeof(m_stDialogBoxInfo[4].cStr));
                EnableDialogBox(4, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
            }
            else
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_ITEMDROP, 0, cItemID, 1, 0, m_pItemList[cItemID]->m_cName);
            }
        }
        m_bIsItemDisabled[cItemID] = true;
    }
}


void CGame::DlgBoxClick_GuildMenu(short msX, short msY)
{
    short sX, sY;
    char cTemp[21];
    int iAdjX, iAdjY;
    sX = m_stDialogBoxInfo[7].sX;
    sY = m_stDialogBoxInfo[7].sY;

    iAdjX = -13;
    iAdjY = 30;

    switch (m_stDialogBoxInfo[7].cMode)
    {
        case 0:
            if ((msX > sX + iAdjX + 80) && (msX < sX + iAdjX + 210) && (msY > sY + iAdjY + 63) && (msY < sY + iAdjY + 78))
            {
                if (m_iGuildRank != -1) return;
                if (m_iCharisma < 20) return;
                if (m_iLevel < 20) return;
                if (m_bIsCrusadeMode) return;
                EndInputString();
                StartInputString(sX + 75, sY + 140, 21, m_cGuildName);
                m_stDialogBoxInfo[7].cMode = 1;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 72) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 82) && (msY < sY + iAdjY + 99))
            {
                if (m_iGuildRank != 0) return;
                if (m_bIsCrusadeMode) return;
                m_stDialogBoxInfo[7].cMode = 5;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 61) && (msX < sX + iAdjX + 226) && (msY > sY + iAdjY + 103) && (msY < sY + iAdjY + 120))
            {
                m_stDialogBoxInfo[7].cMode = 9;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 60) && (msX < sX + iAdjX + 227) && (msY > sY + iAdjY + 123) && (msY < sY + iAdjY + 139))
            {
                m_stDialogBoxInfo[7].cMode = 11;
                PlaySound('E', 14, 5);
            }
            if (m_iFightzoneNumber < 0) break;
            if ((msX > sX + iAdjX + 72) && (msX < sX + iAdjX + 228) && (msY > sY + iAdjY + 143) && (msY < sY + iAdjY + 169))
            {
                if (m_iGuildRank != 0) return;

                if (m_iFightzoneNumber == 0)	m_stDialogBoxInfo[7].cMode = 13;
                else  m_stDialogBoxInfo[7].cMode = 19;

                PlaySound('E', 14, 5);
            }

            break;
        case 1:

            if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                if (strcmp(m_cGuildName, "NONE") == 0) return;
                if (strlen(m_cGuildName) == 0) return;

                bSendCommand(MSGID_REQUEST_CREATENEWGUILD, DEF_MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 2;
                EndInputString();
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[7].cMode = 0;
                EndInputString();
                PlaySound('E', 14, 5);
            }
            break;

        case 3:
        case 4:
        case 7:
        case 8:
        case 10:
        case 12:
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[7].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 9:
            if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                memset(cTemp, 0, sizeof(cTemp));
#ifdef DEF_ENGLISHITEM
                strcpy(cTemp, "GuildAdmissionTicket");
#else
                strcpy(cTemp, "±æµå°¡ÀÔ½ÅÃ»¼­");
#endif
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_PURCHASEITEM, 0, 1, 0, 0, cTemp);
                m_stDialogBoxInfo[7].cMode = 0;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[7].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 11:
            if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                memset(cTemp, 0, sizeof(cTemp));
#ifdef DEF_ENGLISHITEM
                strcpy(cTemp, "GuildSecessionTicket");
#else
                strcpy(cTemp, "±æµåÅ»Åð½ÅÃ»¼­");
#endif
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_PURCHASEITEM, 0, 1, 0, 0, cTemp);
                m_stDialogBoxInfo[7].cMode = 0;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[7].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 5:
            if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                bSendCommand(MSGID_REQUEST_DISBANDGUILD, DEF_MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 6;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[7].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;
            // bool bSendCommand(uint32_t dwMsgID, uint16_t wCommand, char cDir, int iV1, int iV2, int iV3, char * pString, int iV4 = 0); // v1.4

        case 13:
            if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 168) && (msY < sY + iAdjY + 185))
            {
                bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 1, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 18;
                m_iFightzoneNumberTemp = 1;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 168) && (msY < sY + iAdjY + 185))
            {
                bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 2, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 18;
                m_iFightzoneNumberTemp = 2;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 188) && (msY < sY + iAdjY + 205))
            {
                bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 3, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 18;
                m_iFightzoneNumberTemp = 3;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 188) && (msY < sY + iAdjY + 205))
            {
                bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 4, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 18;
                m_iFightzoneNumberTemp = 4;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 208) && (msY < sY + iAdjY + 225))
            {
                bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 5, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 18;
                m_iFightzoneNumberTemp = 5;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 208) && (msY < sY + iAdjY + 225))
            {
                bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 6, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 18;
                m_iFightzoneNumberTemp = 6;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 228) && (msY < sY + iAdjY + 245))
            {
                bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 7, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 18;
                m_iFightzoneNumberTemp = 7;
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 228) && (msY < sY + iAdjY + 245))
            {
                bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, 0, 0, 8, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 18;
                m_iFightzoneNumberTemp = 8;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[7].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        case 21:
        case 22:
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[7].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;
    }
}


void CGame::DlgBoxClick_GuildOp(short msX, short msY)
{
    short sX, sY;
    char cName[12], cName20[24];

    memset(cName, 0, sizeof(cName));
    memset(cName20, 0, sizeof(cName20));
    sX = m_stDialogBoxInfo[8].sX;
    sY = m_stDialogBoxInfo[8].sY;

    switch (m_stGuildOpList[0].cOpMode)
    {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                _ShiftGuildOperationList();
                if (m_stGuildOpList[0].cOpMode == 0) DisableDialogBox(8);
            }
            return;
    }

    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        PlaySound('E', 14, 5);

        switch (m_stGuildOpList[0].cOpMode)
        {
            case 1:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_JOINGUILDAPPROVE, 0, 0, 0, 0, cName20);
                break;

            case 2:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_DISMISSGUILDAPPROVE, 0, 0, 0, 0, cName20);
                break;
        }

        _ShiftGuildOperationList();
        if (m_stGuildOpList[0].cOpMode == 0) DisableDialogBox(8);
    }

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        PlaySound('E', 14, 5);

        switch (m_stGuildOpList[0].cOpMode)
        {
            case 1:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_JOINGUILDREJECT, 0, 0, 0, 0, cName20);
                break;

            case 2:
                strcpy(cName20, m_stGuildOpList[0].cName);
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_DISMISSGUILDREJECT, 0, 0, 0, 0, cName20);
                break;
        }

        _ShiftGuildOperationList();
        if (m_stGuildOpList[0].cOpMode == 0) DisableDialogBox(8);
    }
}

void CGame::bItemDrop_IconPannel(short msX, short msY)
{

    short sX, sY, sItemIndex;

    sX = m_stDialogBoxInfo[30].sX;
    sY = m_stDialogBoxInfo[30].sY;

    sItemIndex = m_stMCursor.sSelectedObjectID;
    if (m_bIsItemDisabled[sItemIndex] == true) return;
    if (m_cCommand < 0) return;
    if ((453 < msX) && (486 > msX) && (440 < msY) && (475 > msY))
    {
        bItemDrop_Inventory(m_stDialogBoxInfo[2].sX + (rand() % 148), m_stDialogBoxInfo[2].sY + (rand() % 55));
        return;
    }

    if ((425 < msX) && (448 > msX) && (440 < msY) && (475 > msY))
    {
        bItemDrop_Character();
        return;
    }
}

int CGame::_iCheckDlgBoxFocus(short msX, short msY, char cButtonSide)
{
    int i;
    char         cDlgID;
    short        sX, sY;
    uint32_t		  dwTime = m_dwCurTime;

    if (cButtonSide == 1)
    {

        for (i = 0; i < 101; i++)
            if (m_cDialogBoxOrder[100 - i] != 0)
            {
                cDlgID = m_cDialogBoxOrder[100 - i];
                if ((m_stDialogBoxInfo[cDlgID].sX <= msX) && ((m_stDialogBoxInfo[cDlgID].sX + m_stDialogBoxInfo[cDlgID].sSizeX) >= msX) &&
                    (m_stDialogBoxInfo[cDlgID].sY <= msY) && ((m_stDialogBoxInfo[cDlgID].sY + m_stDialogBoxInfo[cDlgID].sSizeY) >= msY))
                {
                    EnableDialogBox(cDlgID, 0, 0, 0);

                    m_stMCursor.sPrevX = msX;
                    m_stMCursor.sPrevY = msY;
                    m_stMCursor.sDistX = msX - m_stDialogBoxInfo[cDlgID].sX;
                    m_stMCursor.sDistY = msY - m_stDialogBoxInfo[cDlgID].sY;
                    switch (cDlgID)
                    {
                        case 1:
                            if (bDlgBoxPress_Character(msX, msY) == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            break;

                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 7:
                        case 8:
                        case 9:

                        case 12:
                        case 13:
                        case 16:
                        case 17:
                        case 20:
                        case 22:
                        case 23:
                        case 24:
                        case 25:
                        case 28:
                        case 29:
                        case 30:
                        case 31:
                        case 32:
                        case 33:
                        case 34:
                        case 35:
                        case 36:
                        case 37:
                        case 38:
                        case 67:
                        case 68:
                        case 69:
                            m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                            m_stMCursor.sSelectedObjectID = cDlgID;
                            break;

                        case 2:
                            if (bDlgBoxPress_Inventory(msX, msY) == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            break;
                            //Dialog
                        case 70://Friends List
                            sX = m_stDialogBoxInfo[70].sX;
                            sY = m_stDialogBoxInfo[70].sY;
                            if ((msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320))
                            {
                                m_stDialogBoxInfo[70].bIsScrollSelected = true;
                                return -1;
                            }

                            if (m_stDialogBoxInfo[70].bIsScrollSelected == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 71://Admin Panel
                            sX = m_stDialogBoxInfo[71].sX;
                            sY = m_stDialogBoxInfo[71].sY;
                            if ((msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320))
                            {
                                m_stDialogBoxInfo[71].bIsScrollSelected = true;
                                return -1;
                            }

                            if (m_stDialogBoxInfo[71].bIsScrollSelected == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                            //Admin control user list
                        case 72:
                            sX = m_stDialogBoxInfo[72].sX;
                            sY = m_stDialogBoxInfo[72].sY;
                            if ((m_stDialogBoxInfo[72].cMode == 0) && (msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 20) && (msY <= sY + 330))
                            {
                                m_stDialogBoxInfo[72].bIsScrollSelected = true;
                                return -1;
                            }

                            if ((m_stDialogBoxInfo[72].bIsScrollSelected == false))
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 73://Change //Configs Dialog 0x0109
                            sX = m_stDialogBoxInfo[73].sX;
                            sY = m_stDialogBoxInfo[73].sY;
                            if ((m_stDialogBoxInfo[73].cMode == 0) && (msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 20) && (msY <= sY + 330))
                            {
                                m_stDialogBoxInfo[73].bIsScrollSelected = true;
                                return -1;
                            }

                            if ((m_stDialogBoxInfo[73].bIsScrollSelected == false))
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 10:
                            sX = m_stDialogBoxInfo[10].sX;
                            sY = m_stDialogBoxInfo[10].sY;
                            if ((msX >= sX + 340) && (msX <= sX + 360) && (msY >= sY + 22) && (msY <= sY + 138))
                            {
                                m_stDialogBoxInfo[10].bIsScrollSelected = true;
                                return -1;
                            }

                            if (m_stDialogBoxInfo[10].bIsScrollSelected == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 11:
                            sX = m_stDialogBoxInfo[11].sX;
                            sY = m_stDialogBoxInfo[11].sY;
                            if ((m_stDialogBoxInfo[11].cMode == 0) && (msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 20) && (msY <= sY + 330))
                            {
                                m_stDialogBoxInfo[11].bIsScrollSelected = true;
                                return -1;
                            }

                            if ((m_stDialogBoxInfo[11].bIsScrollSelected == false))
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 14:
                            sX = m_stDialogBoxInfo[14].sX;
                            sY = m_stDialogBoxInfo[14].sY;
                            if ((msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320))
                            {
                                m_stDialogBoxInfo[14].bIsScrollSelected = true;
                                return -1;
                            }

                            if (m_stDialogBoxInfo[14].bIsScrollSelected == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 15:
                            sX = m_stDialogBoxInfo[15].sX;
                            sY = m_stDialogBoxInfo[15].sY;
                            if ((msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320))
                            {
                                m_stDialogBoxInfo[15].bIsScrollSelected = true;
                                return -1;
                            }

                            if (m_stDialogBoxInfo[15].bIsScrollSelected == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 18:
                            sX = m_stDialogBoxInfo[18].sX;
                            sY = m_stDialogBoxInfo[18].sY;
                            if ((msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320))
                            {
                                m_stDialogBoxInfo[18].bIsScrollSelected = true;
                                return -1;
                            }

                            if (m_stDialogBoxInfo[18].bIsScrollSelected == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 19:
                            sX = m_stDialogBoxInfo[19].sX;
                            sY = m_stDialogBoxInfo[19].sY;
                            if ((msX >= sX + 126) && (msX <= sX + 238) && (msY >= sY + 122) && (msY <= sY + 138))
                            {
                                m_stDialogBoxInfo[19].bIsScrollSelected = true;
                                return -1;
                            }
                            if ((msX >= sX + 126) && (msX <= sX + 238) && (msY >= sY + 139) && (msY <= sY + 155))
                            {
                                m_stDialogBoxInfo[19].bIsScrollSelected = true;
                                return -1;
                            }

                            if (m_stDialogBoxInfo[19].bIsScrollSelected == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 21:
                            sX = m_stDialogBoxInfo[21].sX;
                            sY = m_stDialogBoxInfo[21].sY;
                            if ((msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320))
                            {
                                m_stDialogBoxInfo[21].bIsScrollSelected = true;
                                return -1;
                            }

                            if (m_stDialogBoxInfo[21].bIsScrollSelected == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            else return -1;
                            break;

                        case 26:
                            if (bDlgBoxPress_SkillDlg(msX, msY) == false)
                            {
                                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                                m_stMCursor.sSelectedObjectID = cDlgID;
                            }
                            break;

                        case 27:
                            m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                            m_stMCursor.sSelectedObjectID = cDlgID;
                            break;
                    }

                    return 1;
                }
            }

        return 0;
    }
    else
        if (cButtonSide == 2)
        {
            if ((dwTime - m_dwDialogCloseTime) < 300) return 0;
            for (i = 0; i < 101; i++)
                if (m_cDialogBoxOrder[100 - i] != 0)
                {
                    cDlgID = m_cDialogBoxOrder[100 - i];
                    if ((m_stDialogBoxInfo[cDlgID].sX < msX) && ((m_stDialogBoxInfo[cDlgID].sX + m_stDialogBoxInfo[cDlgID].sSizeX) > msX) &&
                        (m_stDialogBoxInfo[cDlgID].sY < msY) && ((m_stDialogBoxInfo[cDlgID].sY + m_stDialogBoxInfo[cDlgID].sSizeY) > msY))
                    {
                        if ((cDlgID != 5) && (cDlgID != 6) && (cDlgID != 8) && (cDlgID != 12) && ((cDlgID != 23) || (m_stDialogBoxInfo[23].cMode < 3)) && (cDlgID != 24) && (cDlgID != 27) && (cDlgID != 34) &&
                            (cDlgID != 33) && !((cDlgID == 32) && ((m_stDialogBoxInfo[cDlgID].cMode == 1) || (m_stDialogBoxInfo[cDlgID].cMode == 3))))
                            DisableDialogBox(cDlgID);

                        m_dwDialogCloseTime = dwTime;
                        return 1;
                    }
                }
        }
    return 0;
}

void CGame::DlgBoxClick_IconPannel(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[30].sX;
    sY = m_stDialogBoxInfo[30].sY;

    if ((msX > sX + 322) && (msX < sX + 355) && (sY + 8 < msY) && (sY + 51 > msY))
    {

        if (m_bIsCrusadeMode == false) return;
        switch (m_iCrusadeDuty)
        {
            case 1: // Fighter
                EnableDialogBox(38, 0, 0, 0);
                break;

            case 2: // Constructor
                EnableDialogBox(37, 0, 0, 0);
                break;

            case 3: // Commander
                EnableDialogBox(36, 0, 0, 0);
                break;

            default: break;
        }
        PlaySound('E', 14, 5);
    }

    if ((sX + 362 < msX) && (sX + 404 > msX) && (sY + 8 < msY) && (sY + 51 > msY))
    {
        // Combat Mode Toggle
        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
        PlaySound('E', 14, 5);
    }

    // Character
    if ((sX + 413 <= msX) && (sX + 446 >= msX) && (sY + 8 < msY) && (sY + 51 > msY))
    {
        if (m_bIsDialogEnabled[1] == true)
            DisableDialogBox(1);
        else EnableDialogBox(1, 0, 0, 0);
        PlaySound('E', 14, 5);
    }

    // Inventory
    if ((sX + 453 <= msX) && (sX + 486 >= msX) && (sY + 8 < msY) && (sY + 51 > msY))
    {
        if (m_bIsDialogEnabled[2] == true)
            DisableDialogBox(2);
        else EnableDialogBox(2, 0, 0, 0);
        PlaySound('E', 14, 5);
    }

    // Magic
    if ((sX + 490 <= msX) && (sX + 522 >= msX) && (sY + 8 < msY) && (sY + 51 > msY))
    {
        if (m_bIsDialogEnabled[3] == true)
            DisableDialogBox(3);
        else EnableDialogBox(3, 0, 0, 0);
        PlaySound('E', 14, 5);
    }

    // Skill
    if ((sX + 526 <= msX) && (sX + 552 >= msX) && (sY + 8 < msY) && (sY + 51 > msY))
    {
        if (m_bIsDialogEnabled[15] == true)
            DisableDialogBox(15);
        else EnableDialogBox(15, 0, 0, 0);
        PlaySound('E', 14, 5);
    }

    // Chat
    if ((sX + 556 <= msX) && (sX + 587 >= msX) && (sY + 8 < msY) && (sY + 51 > msY))
    {
        if (m_bIsDialogEnabled[10] == true)
            DisableDialogBox(10);
        else EnableDialogBox(10, 0, 0, 0);
        PlaySound('E', 14, 5);
    }

    // System Menu
    if ((sX + 589 <= msX) && (sX + 621 >= msX) && (sY + 8 < msY) && (sY + 51 > msY))
    {
        if (m_bIsDialogEnabled[19] == true)
            DisableDialogBox(19);
        else EnableDialogBox(19, 0, 0, 0);
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_Party(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[32].sX;
    sY = m_stDialogBoxInfo[32].sY;

    switch (m_stDialogBoxInfo[32].cMode)
    {
        case 0:
            if (m_iPartyStatus == 0)
            {
                if ((msX > sX + 80) && (msX < sX + 195) && (msY > sY + 80) && (msY < sY + 100))
                {
                    m_stDialogBoxInfo[32].cMode = 2;
                    m_bIsGetPointingMode = true;
                    m_iPointCommandType = 200;
                    PlaySound('E', 14, 5);
                }
            }

            if (m_iPartyStatus != 0)
            {
                if ((msX > sX + 80) && (msX < sX + 195) && (msY > sY + 100) && (msY < sY + 120))
                {
                    m_stDialogBoxInfo[32].cMode = 11;
                    PlaySound('E', 14, 5);
                }
            }

            if (m_iPartyStatus != 0)
            {
                if ((msX > sX + 80) && (msX < sX + 195) && (msY > sY + 120) && (msY < sY + 140))
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_JOINPARTY, 0, 2, 0, 0, m_cMCName);
                    m_stDialogBoxInfo[32].cMode = 4;
                    PlaySound('E', 14, 5);
                }
            }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY)) DisableDialogBox(32);
            break;

        case 1:
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY, 0, 1, 0, 0, m_stDialogBoxInfo[32].cStr);
                DisableDialogBox(32);
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY, 0, 0, 0, 0, m_stDialogBoxInfo[32].cStr);
                DisableDialogBox(32);
                PlaySound('E', 14, 5);
            }
            break;

        case 2:
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[32].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 3:
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[32].cMode = 0;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY, 0, 2, 0, 0, m_stDialogBoxInfo[32].cStr);
                DisableDialogBox(32);
                PlaySound('E', 14, 5);
            }
            break;

        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[32].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 11:
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_JOINPARTY, 0, 0, 0, 0, m_cMCName);
                m_stDialogBoxInfo[32].cMode = 5;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[32].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;
    }
}


void CGame::DlgBoxClick_CrusadeJob(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[33].sX;
    sY = m_stDialogBoxInfo[33].sY;

    switch (m_stDialogBoxInfo[33].cMode)
    {
        case 1:
            if (m_bCitizen == false)
            {
                DisableDialogBox(33);
                PlaySound('E', 14, 5);
            }
            else if (m_bAresden == true)
            {

                if ((m_iGuildRank == 0) || (m_iGuildRank == 1))
                {
                    if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, 0, 3, 0, 0, 0);
                        DisableDialogBox(33);
                        PlaySound('E', 14, 5);
                    }
                }
                else
                {
                    if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                    {

                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, 0, 1, 0, 0, 0);
                        DisableDialogBox(33);
                    }
                    if (m_iGuildRank != -1)
                    {
                        if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 175) && (msY < sY + 190))
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, 0, 2, 0, 0, 0);
                            DisableDialogBox(33);
                            PlaySound('E', 14, 5);
                        }
                    }
                }
            }
            else if (m_bAresden == false)
            {
                if ((m_iGuildRank == 0) || (m_iGuildRank == 1))
                {
                    if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, 0, 3, 0, 0, 0);
                        DisableDialogBox(33);
                        PlaySound('E', 14, 5);
                    }
                }
                else
                {
                    if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                    {

                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, 0, 1, 0, 0, 0);
                        DisableDialogBox(33);
                        PlaySound('E', 14, 5);
                    }
                    if (m_iGuildRank != -1)
                    {
                        if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 175) && (msY < sY + 190))
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, 0, 2, 0, 0, 0);
                            DisableDialogBox(33);
                            PlaySound('E', 14, 5);
                        }
                    }
                }
            }

            if ((msX > sX + 210) && (msX < sX + 260) && (msY >= sY + 296) && (msY <= sY + 316))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 813, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 2:
            if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 160) && (msY < sY + 175))
            {
                switch (m_iCrusadeDuty)
                {
                    case 1: EnableDialogBox(18, 803, 0, 0); break;
                    case 2: EnableDialogBox(18, 805, 0, 0); break;
                    case 3: EnableDialogBox(18, 808, 0, 0); break;
                }
            }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                DisableDialogBox(33);
                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DlgBoxClick_Commander(int msX, int msY)
{
    short sX, sY, tX, tY;
    double d1, d2, d3;

    if (m_bIsCrusadeMode == false) return;

    sX = m_stDialogBoxInfo[36].sX;
    sY = m_stDialogBoxInfo[36].sY;

    switch (m_stDialogBoxInfo[36].cMode)
    {
        case 0:
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {

                m_stDialogBoxInfo[36].cMode = 1;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                if (m_iTeleportLocX == -1)
                {
                    SetTopMsg(m_pGameMsgList[15]->message, 5);
                }
                else if (strcmp(m_cMapName, m_cTeleportMapName) == 0)
                {

                    SetTopMsg(m_pGameMsgList[16]->message, 5);
                }
                else
                {
                    m_stDialogBoxInfo[36].cMode = 2;
                    PlaySound('E', 14, 5);
                }
            }
            if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 46 + 100) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_stDialogBoxInfo[36].cMode = 3;
                m_stDialogBoxInfo[36].sV1 = 0;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 46 + 150) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_stDialogBoxInfo[36].cMode = 4;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 808, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 1:
            if ((msX >= sX + 15) && (msX <= sX + 15 + 280) && (msY >= sY + 60) && (msY <= sY + 60 + 253))
            {
                d1 = (double)(msX - (sX + 15));
                d2 = (double)(752.0f);
                d3 = (d2 * d1) / 280.0f;
                tX = (int)d3;

                d1 = (double)(msY - (sY + 60));
                d2 = (double)(680.0f);
                d3 = (d2 * d1) / 253.0f;
                tY = (int)d3;

                if (tX < 30) tX = 30;
                if (tY < 30) tY = 30;
                if (tX > 722) tX = 722;
                if (tY > 650) tY = 650;

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SETGUILDTELEPORTLOC, 0, tX, tY, 0, "middleland");
                m_stDialogBoxInfo[36].cMode = 0;
                PlaySound('E', 14, 5);

                _RequestMapStatus("middleland", 1);
            }

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {

                m_stDialogBoxInfo[36].cMode = 0;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 809, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 2:
            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GUILDTELEPORT, 0, 0, 0, 0, 0);
                DisableDialogBox(36);
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {

                m_stDialogBoxInfo[36].cMode = 0;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 810, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 3:
            if (m_bAresden == true)
            {

                if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {
                    if (m_iConstructionPoint >= 3000)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 47, 1, m_stDialogBoxInfo[36].sV1, 0);
                        PlaySound('E', 14, 5);
                        DisableDialogBox(36);
                    }
                }
                if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {
                    if (m_iConstructionPoint >= 2000)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 46, 1, m_stDialogBoxInfo[36].sV1, 0);
                        PlaySound('E', 14, 5);
                        DisableDialogBox(36);
                    }
                }
                if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {

                    if (m_iConstructionPoint >= 1000)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 43, 1, m_stDialogBoxInfo[36].sV1, 0);
                        PlaySound('E', 14, 5);
                        DisableDialogBox(36);
                    }
                }
                if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {
                    if (m_iConstructionPoint >= 1500)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 51, 1, m_stDialogBoxInfo[36].sV1, 0);
                        PlaySound('E', 14, 5);
                        DisableDialogBox(36);
                    }
                }
            }
            else if (m_bAresden == false)
            {
                if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {
                    if (m_iConstructionPoint >= 3000)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 45, 1, m_stDialogBoxInfo[36].sV1, 0);
                        PlaySound('E', 14, 5);
                        DisableDialogBox(36);
                    }
                }
                if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {
                    if (m_iConstructionPoint >= 2000)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 44, 1, m_stDialogBoxInfo[36].sV1, 0);
                        PlaySound('E', 14, 5);
                        DisableDialogBox(36);
                    }
                }
                if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {

                    if (m_iConstructionPoint >= 1000)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 43, 1, m_stDialogBoxInfo[36].sV1, 0);
                        PlaySound('E', 14, 5);
                        DisableDialogBox(36);
                    }
                }
                if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {
                    if (m_iConstructionPoint >= 1500)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 51, 1, m_stDialogBoxInfo[36].sV1, 0);
                        PlaySound('E', 14, 5);
                        DisableDialogBox(36);
                    }
                }
            }

            if ((msX >= sX + 20) && (msX <= sX + 380) && (msY > sY + 140) && (msY < sY + 160))
            {
                m_stDialogBoxInfo[36].sV1 = 0;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 20) && (msX <= sX + 380) && (msY > sY + 160) && (msY < sY + 175))
            {
                m_stDialogBoxInfo[36].sV1 = 1;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {

                m_stDialogBoxInfo[36].cMode = 0;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 811, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 4:
            if ((msX >= sX + 15) && (msX <= sX + 15 + 280) && (msY >= sY + 60) && (msY <= sY + 60 + 253))
            {
                d1 = (double)(msX - (sX + 15));
                d2 = (double)(752.0f);
                d3 = (d2 * d1) / 280.0f;
                tX = (int)d3;

                d1 = (double)(msY - (sY + 60));
                d2 = (double)(680.0f);
                d3 = (d2 * d1) / 253.0f;
                tY = (int)d3;

                if (tX < 30) tX = 30;
                if (tY < 30) tY = 30;
                if (tX > 722) tX = 722;
                if (tY > 650) tY = 650;

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC, 0, tX, tY, 0, "middleland");
                m_stDialogBoxInfo[36].cMode = 0;
                PlaySound('E', 14, 5);

                _RequestMapStatus("middleland", 1);
            }

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {

                m_stDialogBoxInfo[36].cMode = 0;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 812, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;
    }
}


void CGame::DlgBoxClick_Constructor(int msX, int msY)
{
    short sX, sY;

    if (m_bIsCrusadeMode == false) return;

    sX = m_stDialogBoxInfo[37].sX;
    sY = m_stDialogBoxInfo[37].sY;

    switch (m_stDialogBoxInfo[37].cMode)
    {
        case 0:
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                if (m_iConstructLocX == -1)
                {
                    SetTopMsg(m_pGameMsgList[14]->message, 5);
                }
                else
                {
                    m_stDialogBoxInfo[37].cMode = 1;
                    PlaySound('E', 14, 5);
                }
            }
            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                if (m_iTeleportLocX == -1)
                {
                    SetTopMsg(m_pGameMsgList[15]->message, 5);
                }
                else if (strcmp(m_cMapName, m_cTeleportMapName) == 0)
                {

                    SetTopMsg(m_pGameMsgList[16]->message, 5);
                }
                else
                {
                    m_stDialogBoxInfo[37].cMode = 2;
                    PlaySound('E', 14, 5);
                }
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 805, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 1:
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 38, 1, m_stDialogBoxInfo[36].sV1, 0);
                PlaySound('E', 14, 5);
                DisableDialogBox(37);
            }
            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 39, 1, m_stDialogBoxInfo[36].sV1, 0);
                PlaySound('E', 14, 5);
                DisableDialogBox(37);
            }
            if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 36, 1, m_stDialogBoxInfo[36].sV1, 0);
                PlaySound('E', 14, 5);
                DisableDialogBox(37);
            }
            if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, 0, 37, 1, m_stDialogBoxInfo[36].sV1, 0);
                PlaySound('E', 14, 5);
                DisableDialogBox(37);
            }

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                // Back
                m_stDialogBoxInfo[37].cMode = 0;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 806, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 2:
            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                // Teleport Confirm
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GUILDTELEPORT, 0, 0, 0, 0, 0);
                DisableDialogBox(37);
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                // Back
                m_stDialogBoxInfo[37].cMode = 0;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 807, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;
    }
}


void CGame::DlgBoxClick_Soldier(int msX, int msY)
{
    short sX, sY;

    if (m_bIsCrusadeMode == false) return;

    sX = m_stDialogBoxInfo[38].sX;
    sY = m_stDialogBoxInfo[38].sY;

    switch (m_stDialogBoxInfo[38].cMode)
    {
        case 0:
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                if (m_iTeleportLocX == -1)
                {
                    SetTopMsg(m_pGameMsgList[15]->message, 5);
                }
                else if (strcmp(m_cMapName, m_cTeleportMapName) == 0)
                {

                    SetTopMsg(m_pGameMsgList[16]->message, 5);
                }
                else
                {
                    m_stDialogBoxInfo[38].cMode = 1;
                    PlaySound('E', 14, 5);
                }
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 803, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 1:
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                // Teleport Confirm
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GUILDTELEPORT, 0, 0, 0, 0, 0);
                DisableDialogBox(38);
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                // Back
                m_stDialogBoxInfo[38].cMode = 0;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                DisableDialogBox(18);
                EnableDialogBox(18, 804, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DlgBoxClick_NpcTalk(int msX, int msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[21].sX;
    sY = m_stDialogBoxInfo[21].sY;

    switch (m_stDialogBoxInfo[21].cMode)
    {
        case 0:
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                DisableDialogBox(21);
                PlaySound('E', 14, 5);
            }
            break;

        case 1: // Accept / Decline
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Accept
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_QUESTACCEPTED, 0, 0, 0, 0, 0);
                DisableDialogBox(21);
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Cancel
                DisableDialogBox(21);
                PlaySound('E', 14, 5);
            }
            break;

        case 2:
            // Next
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                DisableDialogBox(21);
                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DlgBoxClick_ItemUpgrade(int msX, int msY)
{
    short sX, sY;
    int i, iSoX, iSoM;

    sX = m_stDialogBoxInfo[34].sX;
    sY = m_stDialogBoxInfo[34].sY;

    switch (m_stDialogBoxInfo[34].cMode)
    {
        case 1:
            if ((m_stDialogBoxInfo[34].sV1 != -1) && (msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Upgrade
                int iValue = (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_dwAttribute & 0xF0000000) >> 28;
                iValue = iValue * (iValue + 6) / 8 + 2;
                if (m_iGizonItemUpgradeLeft < iValue) break;
                PlaySound('E', 14, 5);
                PlaySound('E', 44, 0);
                m_stDialogBoxInfo[34].cMode = 2;
                m_stDialogBoxInfo[34].dwV1 = m_dwCurTime;
            }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Cancel 
                PlaySound('E', 14, 5);
                DisableDialogBox(34);
            }
            break;

        case 3:
        case 4:
        case 7:
        case 8:
        case 9:
        case 10:

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // OK
                PlaySound('E', 14, 5);
                DisableDialogBox(34);
            }
            break;
        case 5:
            if ((msX > sX + 24) && (msX < sX + 248) && (msY > sY + 100) && (msY < sY + 115))
            {
                PlaySound('E', 14, 5);
                iSoX = iSoM = 0;
                for (i = 0; i < DEF_MAXITEMS; i++)
                    if (m_pItemList[i] != 0)
                    {
                        if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 128)) iSoX++;
                        if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 129)) iSoM++;
                    }

                if ((iSoX > 0) || (iSoM > 0))
                {
                    m_stDialogBoxInfo[34].cMode = 6;
                    m_stDialogBoxInfo[34].sV2 = iSoX;
                    m_stDialogBoxInfo[34].sV3 = iSoM;
                }
                else AddEventList(DRAW_DIALOGBOX_ITEMUPGRADE30, 10);
            }

            if ((msX > sX + 24) && (msX < sX + 248) && (msY > sY + 120) && (msY < sY + 135))
            {
                m_stDialogBoxInfo[34].cMode = 1;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Cancel 
                PlaySound('E', 14, 5);
                DisableDialogBox(34);
            }
            break;

        case 6:
            if ((m_stDialogBoxInfo[34].sV1 != -1) && (msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Upgrade
                PlaySound('E', 14, 5);
                PlaySound('E', 44, 0);
                m_stDialogBoxInfo[34].cMode = 2;
                m_stDialogBoxInfo[34].dwV1 = m_dwCurTime;
            }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Cancel 
                PlaySound('E', 14, 5);
                DisableDialogBox(34);
            }
            break;
    }
}


void CGame::DlgBoxClick_SellList(short msX, short msY)
{
    int i, x;
    short sX, sY;

    sX = m_stDialogBoxInfo[31].sX;
    sY = m_stDialogBoxInfo[31].sY;

    for (i = 0; i < DEF_MAXSELLLIST; i++)
        if ((msX > sX + 25) && (msX < sX + 250) && (msY >= sY + 55 + i * 15) && (msY <= sY + 55 + 14 + i * 15))
        {
            if (m_pItemList[m_stSellItemList[i].iIndex] != 0)
            {
                m_bIsItemDisabled[m_stSellItemList[i].iIndex] = false;
                m_stSellItemList[i].iIndex = -1;

                PlaySound('E', 14, 5);
                for (x = 0; x < DEF_MAXSELLLIST - 1; x++)
                    if (m_stSellItemList[x].iIndex == -1)
                    {
                        m_stSellItemList[x].iIndex = m_stSellItemList[x + 1].iIndex;
                        m_stSellItemList[x].iAmount = m_stSellItemList[x + 1].iAmount;

                        m_stSellItemList[x + 1].iIndex = -1;
                        m_stSellItemList[x + 1].iAmount = 0;
                    }
            }
            return;
        }

    if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        // Sell
        bSendCommand(MSGID_REQUEST_SELLITEMLIST, 0, 0, 0, 0, 0, 0);
        PlaySound('E', 14, 5);
        DisableDialogBox(31);
    }

    if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        // Cancel 
        PlaySound('E', 14, 5);
        DisableDialogBox(31);
    }
}
/*
#ifdef _DEBUG
void CGame::DlgBoxClick_LevelUpSettings(short msX, short msY)
{
 short sX, sY;

    sX = m_stDialogBoxInfo[12].sX;
    sY = m_stDialogBoxInfo[12].sY;

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 177 - 50) && (msY < sY + 187 - 50)) {
        if (m_iLU_Point > 0) {
            m_iLU_Point--;
            m_cLU_Str++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 197 - 50) && (msY < sY + 206 - 50)) {
        if (m_iLU_Point > 0) {
            m_iLU_Point--;
            m_cLU_Vit++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 216 - 50) && (msY < sY + 224 - 50)) {
        if (m_iLU_Point > 0) {
            m_iLU_Point--;
            m_cLU_Dex++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 235 - 50) && (msY < sY + 242 - 50)) {
        if (m_iLU_Point > 0) {
            m_iLU_Point--;
            m_cLU_Int++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 255 - 50) && (msY < sY + 262 - 50)) {
        if (m_iLU_Point > 0) {
            m_iLU_Point--;
            m_cLU_Mag++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 274 - 50) && (msY < sY + 280 - 50)) {
        if (m_iLU_Point > 0) {
            m_iLU_Point--;
            m_cLU_Char++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 177 - 50) && (msY < sY + 187 - 50)) {
        if (m_cLU_Str > 0) {
            m_cLU_Str--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 197 - 50) && (msY < sY + 206 - 50)) {
        if (m_cLU_Vit > 0) {
            m_cLU_Vit--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 216 - 50) && (msY < sY + 224 - 50)) {
        if (m_cLU_Dex > 0) {
            m_cLU_Dex--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 235 - 50) && (msY < sY + 242 - 50)) {
        if (m_cLU_Int > 0) {
            m_cLU_Int--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 255 - 50) && (msY < sY + 262 - 50)) {
        if (m_cLU_Mag > 0) {
            m_cLU_Mag--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 274 - 50) && (msY < sY + 280 - 50)) {
        if (m_cLU_Char > 0) {
            m_cLU_Char--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY)) {
        if (m_iLU_Point == 0) {
            bSendCommand(MSGID_LEVELUPSETTINGS, 0, 0, 0, 0, 0, 0);
            DisableDialogBox(12);
        }
        PlaySound('E', 14, 5);
    }
}

#else
*/
void CGame::DlgBoxClick_LevelUpSettings(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[12].sX;
    sY = m_stDialogBoxInfo[12].sY;
    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 127) && (msY <= sY + 133) && (m_iStr <= m_iStatLimit) && (m_iLU_Point > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_iLU_Point >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point -= 5;
                m_cLU_Str += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_iLU_Point > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point--;
                m_cLU_Str++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 146) && (msY <= sY + 152) && (m_iVit <= m_iStatLimit) && (m_iLU_Point > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_iLU_Point >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point -= 5;
                m_cLU_Vit += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_iLU_Point > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point--;
                m_cLU_Vit++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 165) && (msY <= sY + 171) && (m_iDex <= m_iStatLimit) && (m_iLU_Point > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_iLU_Point >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point -= 5;
                m_cLU_Dex += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_iLU_Point > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point--;
                m_cLU_Dex++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 184) && (msY <= sY + 190) && (m_iInt <= m_iStatLimit) && (m_iLU_Point > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_iLU_Point >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point -= 5;
                m_cLU_Int += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_iLU_Point > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point--;
                m_cLU_Int++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 203) && (msY <= sY + 209) && (m_iMag <= m_iStatLimit) && (m_iLU_Point > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_iLU_Point >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point -= 5;
                m_cLU_Mag += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_iLU_Point > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point--;
                m_cLU_Mag++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 222) && (msY <= sY + 228) && (m_iCharisma <= m_iStatLimit) && (m_iLU_Point > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_iLU_Point >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point -= 5;
                m_cLU_Char += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_iLU_Point > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_iLU_Point--;
                m_cLU_Char++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 127) && (msY <= sY + 133) && (m_cLU_Str > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_cLU_Str >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Str -= 5;
                m_iLU_Point += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_cLU_Str > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Str--;
                m_iLU_Point++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 146) && (msY <= sY + 152) && (m_cLU_Vit > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_cLU_Vit >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Vit -= 5;
                m_iLU_Point += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_cLU_Vit > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Vit--;
                m_iLU_Point++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 165) && (msY <= sY + 171) && (m_cLU_Dex > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_cLU_Dex >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Dex -= 5;
                m_iLU_Point += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_cLU_Dex > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Dex--;
                m_iLU_Point++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 184) && (msY <= sY + 190) && (m_cLU_Int > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_cLU_Int >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Int -= 5;
                m_iLU_Point += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_cLU_Int > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Int--;
                m_iLU_Point++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 203) && (msY <= sY + 209) && (m_cLU_Mag > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_cLU_Mag >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Mag -= 5;
                m_iLU_Point += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_cLU_Mag > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Mag--;
                m_iLU_Point++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 222) && (msY <= sY + 228) && (m_cLU_Char > 0))
    {
        if (m_bCtrlPressed == true)
        {
            if ((m_cLU_Char >= 5) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Char -= 5;
                m_iLU_Point += 5;
            }
            PlaySound('E', 14, 5);
        }
        else
        {
            if ((m_cLU_Char > 0) && (m_bIsDialogEnabled[42] == false))
            {
                m_cLU_Char--;
                m_iLU_Point++;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        //		if (m_stDialogBoxInfo[12].sV1 != m_iLU_Point)
        bSendCommand(MSGID_LEVELUPSETTINGS, 0, 0, 0, 0, 0, 0);
        //m_cLU_Str = m_cLU_Vit = m_cLU_Dex = m_cLU_Int = m_cLU_Mag = m_cLU_Char = 0;
        DisableDialogBox(12);
        PlaySound('E', 14, 5);
    }
    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {	// Change stats with Majestic
        if ((m_iGizonItemUpgradeLeft > 0) && (m_iLU_Point <= 0)
            && (m_cLU_Str == 0) && (m_cLU_Vit == 0) && (m_cLU_Dex == 0) && (m_cLU_Int == 0) && (m_cLU_Mag == 0) && (m_cLU_Char == 0))
        {
            DisableDialogBox(12);
            EnableDialogBox(42, 0, 0, 0);
            PlaySound('E', 14, 5);
        }
    }
}
//#endif

bool CGame::bDlgBoxPress_Character(short msX, short msY)
{
    int i;
    short sX, sY, sSprH, sFrame;
    char cEquipPoiStatus[DEF_MAXITEMEQUIPPOS]{};

    if (m_bIsDialogEnabled[17] == true) return false;

    sX = m_stDialogBoxInfo[1].sX;
    sY = m_stDialogBoxInfo[1].sY;
    for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) cEquipPoiStatus[i] = -1;
    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))	cEquipPoiStatus[m_pItemList[i]->m_cEquipPos] = i;
    }

    if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
    {
        if (cEquipPoiStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 72, sY + 135, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 32, sY + 193, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_RFINGER];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 35, sY + 120, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 57, sY + 186, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 57, sY + 186, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 90, sY + 170, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_FULLBODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BOOTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 41, sY + 137, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BACK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
    }
    else if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
    {
        if (cEquipPoiStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 72, sY + 139, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 32, sY + 193, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_RFINGER];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 35, sY + 120, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 60, sY + 191, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 60, sY + 191, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 84, sY + 175, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_FULLBODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if ((cEquipPoiStatus[DEF_EQUIPPOS_BOOTS] != -1))
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BOOTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 45, sY + 143, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BACK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return true;
            }
        }
    }
    return false;
}

void CGame::DlgBoxClick_CityhallMenu(short msX, short msY)
{
    short sX, sY;
    sX = m_stDialogBoxInfo[13].sX;
    sY = m_stDialogBoxInfo[13].sY;
    int iReqHeroItemID = 0;
    switch (m_stDialogBoxInfo[13].cMode)
    {
        case 0:
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
            {
                if (m_bCitizen == true) return;
                m_stDialogBoxInfo[13].cMode = 1; // citizenship request
                PlaySound('E', 14, 5);
            }
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
            {
                if (m_iRewardGold <= 0) return;
                m_stDialogBoxInfo[13].cMode = 5; // reward gold
                PlaySound('E', 14, 5);
            }
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
            {
                if (m_iEnemyKillCount < 100) return;//hero
                m_stDialogBoxInfo[13].cMode = 7;
                PlaySound('E', 14, 5);
            }
            //Change DK items from cityhall
            //0x0102
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 145) && (msY < sY + 170))
            {
                if (m_iLevel < 180) return;//dk items
                m_stDialogBoxInfo[13].cMode = 15;
                PlaySound('E', 14, 5);
            }
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 170) && (msY < sY + 195))
            {
                if (m_stQuest.sQuestType == 0) return;//cancel quest
                m_stDialogBoxInfo[13].cMode = 8;
                PlaySound('E', 14, 5);
            }
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 195) && (msY < sY + 220))
            {
                if (m_bIsCrusadeMode) return;//play mode change
                if (m_iPKCount != 0) return;
                if (m_bCitizen == false) return;
                if ((m_iLevel > 100) && (m_bHunter == false)) return;
                m_stDialogBoxInfo[13].cMode = 9;
                PlaySound('E', 14, 5);
            }
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 220) && (msY < sY + 245))
            {
                m_stDialogBoxInfo[13].cMode = 10;
                m_iTeleportMapCount = -1;
                bSendCommand(MSGID_REQUEST_TELEPORT_LIST, 0, 0, 0, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 245) && (msY < sY + 270))
            {
                if (m_bIsCrusadeMode == false) return;
                EnableDialogBox(33, 1, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 1:
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Yes Click
                bSendCommand(MSGID_REQUEST_CIVILRIGHT, DEF_MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
                m_stDialogBoxInfo[13].cMode = 2;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // No Click
                m_stDialogBoxInfo[13].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 3:	//
        case 4:	// OK°
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // No Click
                m_stDialogBoxInfo[13].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 5:
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Yes
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETREWARDMONEY, 0, 0, 0, 0, 0);
                m_stDialogBoxInfo[13].cMode = 0;
                PlaySound('E', 14, 5);

            }
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // No
                m_stDialogBoxInfo[13].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 7:
            // Hero's Cape
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 95) && (msY <= sY + 110))
            {
                if (m_bAresden == true) iReqHeroItemID = 400;
                else iReqHeroItemID = 401;
                memset(m_cTakeHeroItemName, 0, sizeof(m_cTakeHeroItemName));
                memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU47, strlen(DRAW_DIALOGBOX_CITYHALL_MENU47));
                m_stDialogBoxInfo[13].cMode = 11;
                m_stDialogBoxInfo[13].sV1 = iReqHeroItemID;
                PlaySound('E', 14, 5);
            }
            // Hero's Helm
            if ((m_iEnemyKillCount >= 150) && (m_iContribution >= 20))
                if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 125) && (msY <= sY + 140))
                {
                    if ((m_bAresden == true) && (m_cGender == 0)) iReqHeroItemID = 403;
                    if ((m_bAresden == true) && (m_cGender == 1)) iReqHeroItemID = 404;
                    if ((m_bAresden == false) && (m_cGender == 0)) iReqHeroItemID = 405;
                    if ((m_bAresden == false) && (m_cGender == 1)) iReqHeroItemID = 406;
                    memset(m_cTakeHeroItemName, 0, sizeof(m_cTakeHeroItemName));
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU48, strlen(DRAW_DIALOGBOX_CITYHALL_MENU48));
                    m_stDialogBoxInfo[13].cMode = 11;
                    m_stDialogBoxInfo[13].sV1 = iReqHeroItemID;
                    PlaySound('E', 14, 5);
                }
            // Hero's Cap
            if ((m_iEnemyKillCount >= 100) && (m_iContribution >= 20))
                if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 155) && (msY <= sY + 170))
                {
                    if ((m_bAresden == true) && (m_cGender == 0)) iReqHeroItemID = 407;
                    if ((m_bAresden == true) && (m_cGender == 1)) iReqHeroItemID = 408;
                    if ((m_bAresden == false) && (m_cGender == 0)) iReqHeroItemID = 409;
                    if ((m_bAresden == false) && (m_cGender == 1)) iReqHeroItemID = 410;
                    memset(m_cTakeHeroItemName, 0, sizeof(m_cTakeHeroItemName));
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU49, strlen(DRAW_DIALOGBOX_CITYHALL_MENU49));
                    m_stDialogBoxInfo[13].cMode = 11;
                    m_stDialogBoxInfo[13].sV1 = iReqHeroItemID;
                    PlaySound('E', 14, 5);
                }
            // Hero's Armor
            if ((m_iEnemyKillCount >= 300) && (m_iContribution >= 30))
                if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 185) && (msY <= sY + 200))
                {
                    if ((m_bAresden == true) && (m_cGender == 0)) iReqHeroItemID = 411;
                    if ((m_bAresden == true) && (m_cGender == 1)) iReqHeroItemID = 412;
                    if ((m_bAresden == false) && (m_cGender == 0)) iReqHeroItemID = 413;
                    if ((m_bAresden == false) && (m_cGender == 1)) iReqHeroItemID = 414;
                    memset(m_cTakeHeroItemName, 0, sizeof(m_cTakeHeroItemName));
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU50, strlen(DRAW_DIALOGBOX_CITYHALL_MENU50));
                    m_stDialogBoxInfo[13].cMode = 11;
                    m_stDialogBoxInfo[13].sV1 = iReqHeroItemID;
                    PlaySound('E', 14, 5);
                }
            // Hero's Robe
            if ((m_iEnemyKillCount >= 200) && (m_iContribution >= 20))
                if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 215) && (msY <= sY + 230))
                {
                    if ((m_bAresden == true) && (m_cGender == 0)) iReqHeroItemID = 415;
                    if ((m_bAresden == true) && (m_cGender == 1)) iReqHeroItemID = 416;
                    if ((m_bAresden == false) && (m_cGender == 0)) iReqHeroItemID = 417;
                    if ((m_bAresden == false) && (m_cGender == 1)) iReqHeroItemID = 418;
                    memset(m_cTakeHeroItemName, 0, sizeof(m_cTakeHeroItemName));
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU51, strlen(DRAW_DIALOGBOX_CITYHALL_MENU51));
                    m_stDialogBoxInfo[13].cMode = 11;
                    m_stDialogBoxInfo[13].sV1 = iReqHeroItemID;
                    PlaySound('E', 14, 5);
                }
            // Hero's Hauberk
            if ((m_iEnemyKillCount >= 100) && (m_iContribution >= 10))
                if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 245) && (msY <= sY + 260))
                {
                    if ((m_bAresden == true) && (m_cGender == 0)) iReqHeroItemID = 419;
                    if ((m_bAresden == true) && (m_cGender == 1)) iReqHeroItemID = 420;
                    if ((m_bAresden == false) && (m_cGender == 0)) iReqHeroItemID = 421;
                    if ((m_bAresden == false) && (m_cGender == 1)) iReqHeroItemID = 422;
                    memset(m_cTakeHeroItemName, 0, sizeof(m_cTakeHeroItemName));
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU52, strlen(DRAW_DIALOGBOX_CITYHALL_MENU52));
                    m_stDialogBoxInfo[13].cMode = 11;
                    m_stDialogBoxInfo[13].sV1 = iReqHeroItemID;
                    PlaySound('E', 14, 5);
                }
            // Hero's Leggings
            if ((m_iEnemyKillCount >= 150) && (m_iContribution >= 15))
                if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 275) && (msY <= sY + 290))
                {
                    if ((m_bAresden == true) && (m_cGender == 0)) iReqHeroItemID = 423;
                    if ((m_bAresden == true) && (m_cGender == 1)) iReqHeroItemID = 424;
                    if ((m_bAresden == false) && (m_cGender == 0)) iReqHeroItemID = 425;
                    if ((m_bAresden == false) && (m_cGender == 1)) iReqHeroItemID = 426;
                    memset(m_cTakeHeroItemName, 0, sizeof(m_cTakeHeroItemName));
                    memcpy(m_cTakeHeroItemName, DRAW_DIALOGBOX_CITYHALL_MENU53, strlen(DRAW_DIALOGBOX_CITYHALL_MENU53));
                    m_stDialogBoxInfo[13].cMode = 11;
                    m_stDialogBoxInfo[13].sV1 = iReqHeroItemID;
                    PlaySound('E', 14, 5);
                }
            break;

        case 8:
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Yes
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_CANCELQUEST, 0, 0, 0, 0, 0);
                m_stDialogBoxInfo[13].cMode = 0;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // No
                m_stDialogBoxInfo[13].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 9:
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // Yes
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_HUNTMODE, 0, 0, 0, 0, 0);
                m_stDialogBoxInfo[13].cMode = 0;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                // No
                m_stDialogBoxInfo[13].cMode = 0;
                PlaySound('E', 14, 5);
            }
            break;

        case 10:
            if (m_iTeleportMapCount > 0)
            {
                for (int i = 0; i < m_iTeleportMapCount; i++)
                {
                    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 130 + i * 15) && (msY <= sY + 144 + i * 15))
                    {
                        bSendCommand(MSGID_REQUEST_CHARGED_TELEPORT, 0, 0, m_stTeleportList[i].iIndex, 0, 0, 0);
                        DisableDialogBox(13);
                        return;
                    }
                }
            }
            break;

        case 11: // Fix Drawjer
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETHEROMANTLE, 0, m_stDialogBoxInfo[13].sV1, 0, 0, 0);
                m_stDialogBoxInfo[13].cMode = 0;
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[13].cMode = 7;
                PlaySound('E', 14, 5);
            }
            break;

            //Change DK items from cityhall
            //0x0102
        case 15:
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 95) && (msY <= sY + 110))
            {
                //Hauberk
                //Male
                if (m_cGender == 0) iReqHeroItemID = 706;
                //Female
                if (m_cGender == 1) iReqHeroItemID = 724;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 110) && (msY <= sY + 125))
            {
                //Full Helm
                if (m_cGender == 0) iReqHeroItemID = 707;
                if (m_cGender == 1) iReqHeroItemID = 725;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 125) && (msY <= sY + 140))
            {
                //Leggings
                if (m_cGender == 0) iReqHeroItemID = 708;
                if (m_cGender == 1) iReqHeroItemID = 726;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 140) && (msY <= sY + 155))
            {
                //Robe
                if (m_cGender == 0) iReqHeroItemID = 715;
                if (m_cGender == 1) iReqHeroItemID = 733;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 155) && (msY <= sY + 170))
            {
                //Scale Mail
                if (m_cGender == 0) iReqHeroItemID = 719;
                if (m_cGender == 1) iReqHeroItemID = 732;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 170) && (msY <= sY + 185))
            {
                //Chain Mail
                if (m_cGender == 0) iReqHeroItemID = 712;
                if (m_cGender == 1) iReqHeroItemID = 730;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 185) && (msY <= sY + 200))
            {
                //Plate Mail
                if (m_cGender == 0) iReqHeroItemID = 710;
                if (m_cGender == 1) iReqHeroItemID = 728;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 200) && (msY <= sY + 215))
            {
                //Rapier
                iReqHeroItemID = 717;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 215) && (msY <= sY + 230))
            {
                //Great Sword
                iReqHeroItemID = 718;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 230) && (msY <= sY + 245))
            {
                //Flameberge
                iReqHeroItemID = 709;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 245) && (msY <= sY + 260))
            {
                //Staff (MS25)
                iReqHeroItemID = 714;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETDARKITEM, 0, iReqHeroItemID, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DlgBoxClick_Text(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[18].sX;
    sY = m_stDialogBoxInfo[18].sY;

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        DisableDialogBox(18);
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_Inventory(short msX, short msY)
{
    int i, sX, sY;

    sX = m_stDialogBoxInfo[2].sX;
    sY = m_stDialogBoxInfo[2].sY;

    if ((msX >= sX + 23) && (msX <= sX + 76) && (msY >= sY + 172) && (msY <= sY + 184))
    {
        if (m_iGizonItemUpgradeLeft == 0) m_iGizonItemUpgradeLeft = 0;
        EnableDialogBox(34, 5, 0, 0);
        PlaySound('E', 14, 5);
    }

    if ((msX >= sX + 140) && (msX <= sX + 212) && (msY >= sY + 172) && (msY <= sY + 184))
    {

        if (m_cSkillMastery[13] == 0)
        {
            AddEventList(DLGBOXCLICK_INVENTORY1, 10);
            AddEventList(DLGBOXCLICK_INVENTORY2, 10);
        }
        else if (m_bSkillUsingStatus == true)
        {
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY5, 10);
            return;
        }
        else if (_bIsItemOnHand() == true)
        {
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);
            return;
        }
        else
        {
            //
            for (i = 0; i < DEF_MAXITEMS; i++)
                if ((m_pItemList[i] != 0) && (m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX) &&
                    (m_pItemList[i]->m_sSpriteFrame == 113) && (m_pItemList[i]->m_wCurLifeSpan > 0))
                {
                    EnableDialogBox(26, 3, 0, 0, 0);
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY12, 10);
                    PlaySound('E', 14, 5);
                    return;
                }
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY14, 10);
            //
        }
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_Character(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[1].sX;
    sY = m_stDialogBoxInfo[1].sY;

    if ((msX >= sX + 15) && (msX <= sX + 15 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
    {
        EnableDialogBox(28, 1, 0, 0);
        DisableDialogBox(1);
        PlaySound('E', 14, 5);
    }
    else if ((msX >= sX + 98) && (msX <= sX + 98 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
    {
        EnableDialogBox(32, 0, 0, 0);
        DisableDialogBox(1);
        PlaySound('E', 14, 5);
    }
    else if ((msX >= sX + 180) && (msX <= sX + 180 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
    {
        EnableDialogBox(12, 0, 0, 0);
        DisableDialogBox(1);
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_FeedBackCard(short msX, short msY)
{

}

void CGame::DlgBoxClick_MagicShop(short msX, short msY)
{
    int i, iCPivot, iYloc, iAdjX, iAdjY;
    short sX, sY;

    sX = m_stDialogBoxInfo[16].sX;
    sY = m_stDialogBoxInfo[16].sY;

    iAdjX = -20;
    iAdjY = -35;
    iCPivot = m_stDialogBoxInfo[16].sView * 10;

    iYloc = 0;
    for (i = 0; i < 9; i++)
    {
        if ((m_pMagicCfgList[iCPivot + i] != 0) && (m_pMagicCfgList[iCPivot + i]->m_bIsVisible))
        {
            if ((msX >= sX + iAdjX + 44) && (msX <= sX + iAdjX + 135 + 44) && (msY >= sY + iAdjY + 70 + iYloc + 35) && (msY <= sY + iAdjY + 70 + 14 + iYloc + 35))
            {
                if (m_cMagicMastery[iCPivot + i] == 0)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_STUDYMAGIC, 0, 0, 0, 0, m_pMagicCfgList[iCPivot + i]->m_cName);
                    PlaySound('E', 14, 5);
                }
                return;
            }
            iYloc += 18;
        }
    }

    if ((msX >= sX + iAdjX + 42 + 31) && (msX <= sX + iAdjX + 50 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 0;
    if ((msX >= sX + iAdjX + 55 + 31) && (msX <= sX + iAdjX + 68 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 1;
    if ((msX >= sX + iAdjX + 73 + 31) && (msX <= sX + iAdjX + 93 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 2;
    if ((msX >= sX + iAdjX + 98 + 31) && (msX <= sX + iAdjX + 113 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 3;
    if ((msX >= sX + iAdjX + 118 + 31) && (msX <= sX + iAdjX + 129 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 4;
    if ((msX >= sX + iAdjX + 133 + 31) && (msX <= sX + iAdjX + 150 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 5;
    if ((msX >= sX + iAdjX + 154 + 31) && (msX <= sX + iAdjX + 177 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 6;
    if ((msX >= sX + iAdjX + 181 + 31) && (msX <= sX + iAdjX + 210 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 7;
    if ((msX >= sX + iAdjX + 214 + 31) && (msX <= sX + iAdjX + 230 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 8;
    if ((msX >= sX + iAdjX + 234 + 31) && (msX <= sX + iAdjX + 245 + 31) && (msY >= sY + iAdjY + 248 + 35) && (msY <= sY + iAdjY + 260 + 35))
        m_stDialogBoxInfo[16].sView = 9;
}

void CGame::DlgBoxClick_15AgeMsg(short msX, short msY)
{
    short sX, sY;
    sX = m_stDialogBoxInfo[5].sX;
    sY = m_stDialogBoxInfo[5].sY;
#ifdef DEF_FEEDBACKCARD
    m_iFeedBackCardIndex = -1;
    DisableDialogBox(5);
#else

    if ((msX >= sX + 120) && (msX <= sX + 120 + DEF_BTNSZX) && (msY >= sY + 127) && (msY <= sY + 127 + DEF_BTNSZY))
        DisableDialogBox(5);
#endif
}


void CGame::DlgBoxClick_WarningMsg(short msX, short msY)
{
    short sX, sY;
    sX = m_stDialogBoxInfo[6].sX;
    sY = m_stDialogBoxInfo[6].sY;

    if ((msX >= sX + 120) && (msX <= sX + 120 + DEF_BTNSZX) && (msY >= sY + 127) && (msY <= sY + 127 + DEF_BTNSZY))
        DisableDialogBox(6);
}

void CGame::DlgBoxClick_ItemDrop(short msX, short msY)
{
    short sX, sY;
    if (m_cCommand < 0) return;

    sX = m_stDialogBoxInfo[4].sX;
    sY = m_stDialogBoxInfo[4].sY;

    if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
    {
        m_stDialogBoxInfo[4].cMode = 3;
        bSendCommand(MSGID_COMMAND_COMMON,
            DEF_COMMONTYPE_ITEMDROP,
            0,
            m_stDialogBoxInfo[4].sView,
            1,
            0,
            m_pItemList[m_stDialogBoxInfo[4].sView]->m_cName);
        //m_stDialogBoxInfo[40].sView ;
        DisableDialogBox(4);
    }

    else if ((msX >= sX + 170) && (msX <= sX + 170 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
    {

        for (int i = 0; i < DEF_MAXSELLLIST; i++)
            m_bIsItemDisabled[i] = false;

        DisableDialogBox(4);
    }
    else if ((msX >= sX + 35) && (msX <= sX + 240) && (msY >= sY + 80) && (msY <= sY + 90))
    {
        m_bItemDrop = !m_bItemDrop;
    }
}

void CGame::DlgBoxClick_ItemSellorRepair(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[23].sX;
    sY = m_stDialogBoxInfo[23].sY;

    switch (m_stDialogBoxInfo[23].cMode)
    {
        case 1:
            if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_SELLITEMCONFIRM, 0, m_stDialogBoxInfo[23].sV1, m_stDialogBoxInfo[23].sV4, m_stDialogBoxInfo[23].sV3, m_pItemList[m_stDialogBoxInfo[23].sV1]->m_cName); //v1.2
                m_stDialogBoxInfo[23].cMode = 3;
            }
            if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_bIsItemDisabled[m_stDialogBoxInfo[23].sV1] = false;
                DisableDialogBox(23);
            }
            break;

        case 2:
            if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM, 0, m_stDialogBoxInfo[23].sV1, 0, 0, m_pItemList[m_stDialogBoxInfo[23].sV1]->m_cName);
                m_stDialogBoxInfo[23].cMode = 4;
            }
            if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_bIsItemDisabled[m_stDialogBoxInfo[23].sV1] = false;
                DisableDialogBox(23);
            }
            break;
    }
}

void CGame::DlgBoxClick_ShutDownMsg(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[25].sX;
    sY = m_stDialogBoxInfo[25].sY;
    if ((msX >= sX + 210) && (msX <= sX + 210 + DEF_BTNSZX) && (msY > sY + 127) && (msY < sY + 127 + DEF_BTNSZY))
    {
        DisableDialogBox(25);
        PlaySound('E', 14, 5);
    }
}

bool CGame::bDlgBoxPress_SkillDlg(short msX, short msY)
{
    int i, iAdjX, iAdjY;
    char  cItemID;
    short sX, sY, x1, y1, x2, y2, sArray[10];

    sX = m_stDialogBoxInfo[26].sX;
    sY = m_stDialogBoxInfo[26].sY;

    iAdjX = 5;
    iAdjY = 10;

    switch (m_stDialogBoxInfo[26].cMode)
    {
        case 1:
            memset(sArray, 0, sizeof(sArray));
            sArray[1] = m_stDialogBoxInfo[26].sV1;
            sArray[2] = m_stDialogBoxInfo[26].sV2;
            sArray[3] = m_stDialogBoxInfo[26].sV3;
            sArray[4] = m_stDialogBoxInfo[26].sV4;
            sArray[5] = m_stDialogBoxInfo[26].sV5;
            sArray[6] = m_stDialogBoxInfo[26].sV6;

            for (i = 1; i <= 6; i++)
                if ((sArray[i] != -1) && (m_pItemList[sArray[i]] != 0))
                {
                    cItemID = (char)sArray[i];

                    switch (i)
                    {
                        case 1: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 2: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 45 * 1, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 3: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 45 * 2, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 4: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 5: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 45 * 1, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 6: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 45 * 2, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                    }
                    x1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
                    y1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
                    x2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
                    y2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

                    if ((msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
                    {
                        switch (i)
                        {
                            case 1: m_stDialogBoxInfo[26].sV1 = -1; break;
                            case 2: m_stDialogBoxInfo[26].sV2 = -1; break;
                            case 3: m_stDialogBoxInfo[26].sV3 = -1; break;
                            case 4: m_stDialogBoxInfo[26].sV4 = -1; break;
                            case 5: m_stDialogBoxInfo[26].sV5 = -1; break;
                            case 6: m_stDialogBoxInfo[26].sV6 = -1; break;
                        }
                        m_bIsItemDisabled[cItemID] = false;

                        m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                        m_stMCursor.sSelectedObjectID = cItemID;
                        m_stMCursor.sDistX = msX + iAdjX - x1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                        m_stMCursor.sDistY = msY + iAdjY - y1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;
                        return true;
                    }
                }
            break;

        case 4:
            memset(sArray, 0, sizeof(sArray));
            sArray[1] = m_stDialogBoxInfo[26].sV1;
            sArray[2] = m_stDialogBoxInfo[26].sV2;
            sArray[3] = m_stDialogBoxInfo[26].sV3;
            sArray[4] = m_stDialogBoxInfo[26].sV4;
            sArray[5] = m_stDialogBoxInfo[26].sV5;
            sArray[6] = m_stDialogBoxInfo[26].sV6;

            for (i = 1; i <= 6; i++)
                if ((sArray[i] != -1) && (m_pItemList[sArray[i]] != 0))
                {
                    cItemID = (char)sArray[i];

                    switch (i)
                    {
                        case 1: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 2: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 55 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 3: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 55 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 4: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 5: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 100 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                        case 6: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 100 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                    }
                    x1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
                    y1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
                    x2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
                    y2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

                    if ((msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
                    {
                        switch (i)
                        {
                            case 1: m_stDialogBoxInfo[26].sV1 = -1; break;
                            case 2: m_stDialogBoxInfo[26].sV2 = -1; break;
                            case 3: m_stDialogBoxInfo[26].sV3 = -1; break;
                            case 4: m_stDialogBoxInfo[26].sV4 = -1; break;
                            case 5: m_stDialogBoxInfo[26].sV5 = -1; break;
                            case 6: m_stDialogBoxInfo[26].sV6 = -1; break;
                        }
                        m_bIsItemDisabled[cItemID] = false;

                        m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                        m_stMCursor.sSelectedObjectID = cItemID;
                        m_stMCursor.sDistX = msX + iAdjX - x1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                        m_stMCursor.sDistY = msY + iAdjY - y1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;

                        m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                        return true;
                    }
                }
            break;
    }

    return false;
}

void CGame::bItemDrop_ExchangeDialog(short msX, short msY)
{
    char cItemID;
    if (m_cCommand < 0) return;
    if (m_stDialogBoxExchangeInfo[3].sV1 != -1) return;

    cItemID = (char)m_stMCursor.sSelectedObjectID;
    if (((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
        (m_pItemList[cItemID]->m_dwCount > 1))
    {
        m_stDialogBoxInfo[17].sX = msX - 140;
        m_stDialogBoxInfo[17].sY = msY - 70;
        if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;
        m_stDialogBoxInfo[17].sV1 = m_sPlayerX + 1;
        m_stDialogBoxInfo[17].sV2 = m_sPlayerY + 1;
        m_stDialogBoxInfo[17].sV3 = 1000;
        m_stDialogBoxInfo[17].sV4 = cItemID;
        if (m_stDialogBoxExchangeInfo[0].sV1 == -1)			m_stDialogBoxExchangeInfo[0].sItemID = cItemID;
        else if (m_stDialogBoxExchangeInfo[1].sV1 == -1)	m_stDialogBoxExchangeInfo[1].sItemID = cItemID;
        else if (m_stDialogBoxExchangeInfo[2].sV1 == -1)	m_stDialogBoxExchangeInfo[2].sItemID = cItemID;
        else if (m_stDialogBoxExchangeInfo[3].sV1 == -1)	m_stDialogBoxExchangeInfo[3].sItemID = cItemID;
        else return;
        memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
        EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
        return;
    }
    else
    {
        /*m_stDialogBoxInfo[27].sV1 = m_pItemList[cItemID]->m_sSprite;
        m_stDialogBoxInfo[27].sV2 = m_pItemList[cItemID]->m_sSpriteFrame;
        m_stDialogBoxInfo[27].sV3 = 1;
        m_stDialogBoxInfo[27].sV4 = m_pItemList[cItemID]->m_cItemColor;
        m_stDialogBoxInfo[27].sView = cItemID;*/
        if (m_stDialogBoxExchangeInfo[0].sV1 == -1)			m_stDialogBoxExchangeInfo[0].sItemID = cItemID;
        else if (m_stDialogBoxExchangeInfo[1].sV1 == -1)	m_stDialogBoxExchangeInfo[1].sItemID = cItemID;
        else if (m_stDialogBoxExchangeInfo[2].sV1 == -1)	m_stDialogBoxExchangeInfo[2].sItemID = cItemID;
        else if (m_stDialogBoxExchangeInfo[3].sV1 == -1)	m_stDialogBoxExchangeInfo[3].sItemID = cItemID;
        else return;
        m_bIsItemDisabled[cItemID] = true;
        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SETEXCHANGEITEM, 0, cItemID, 1, 0, 0);
        return;
    }
}
void CGame::DlgBoxClick_Exchange(short msX, short msY)
{
    short sX, sY;
    sX = m_stDialogBoxInfo[27].sX;
    sY = m_stDialogBoxInfo[27].sY;
    switch (m_stDialogBoxInfo[27].cMode)
    {
        case 1:
            if ((msX >= sX + 220) && (msX <= sX + 220 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY))
            {
                if ((m_stDialogBoxExchangeInfo[0].sV1 != -1) && (m_stDialogBoxExchangeInfo[4].sV1 != -1))
                {
                    PlaySound('E', 14, 5);
                    m_stDialogBoxInfo[27].cMode = 2;
                    EnableDialogBox(41, 0, 0, 0);
                    m_stDialogBoxInfo[41].cMode = 1;
                }
                return;
            }
            if ((msX >= sX + 450) && (msX <= sX + 450 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY)
                && (m_bIsDialogEnabled[41] == false))
            {
                DisableDialogBox(27);
                DisableDialogBox(22);
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_CANCELEXCHANGEITEM, 0, 0, 0, 0, 0);
                PlaySound('E', 14, 5);
                return;
            }
            break;

        case 2:
            /*	if ((msX >= sX + 450) && (msX <= sX + 450 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY))  // Cancel
                {	DisableDialogBox(27);
                    DisableDialogBox(22);
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_CANCELEXCHANGEITEM, 0, 0, 0, 0, 0);
                    PlaySound('E', 14, 5);
                    return;
                }*/
            break;
    }
}
void CGame::DlgBoxClick_ConfirmExchange(short msX, short msY)
{
    short sX, sY;
    sX = m_stDialogBoxInfo[41].sX;
    sY = m_stDialogBoxInfo[41].sY;

    switch (m_stDialogBoxInfo[41].cMode)
    {
        case 1:
            if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
            {
                if ((m_stDialogBoxExchangeInfo[0].sV1 != -1) && (m_stDialogBoxExchangeInfo[4].sV1 != -1))
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_CONFIRMEXCHANGEITEM, 0
                        , m_stDialogBoxExchangeInfo[0].sV1
                        , m_stDialogBoxExchangeInfo[0].sV3
                        , 0, 0);
                    PlaySound('E', 14, 5);
                    m_stDialogBoxInfo[27].cMode = 2;
                    m_stDialogBoxInfo[41].cMode = 2;
                }
                return;
            }
            if ((msX >= sX + 170) && (msX <= sX + 170 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
            {
                DisableDialogBox(41);
                DisableDialogBox(27);
                DisableDialogBox(22);
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_CANCELEXCHANGEITEM, 0, 0, 0, 0, 0);
                PlaySound('E', 14, 5);
                return;
            }
            break;
        case 2:
            break;
    }
}

void CGame::DlgBoxClick_Quest(int msX, int msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[28].sX;
    sY = m_stDialogBoxInfo[28].sY;

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        DisableDialogBox(28);
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_FriendsList(int msX, int msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[71].sX;
    sY = m_stDialogBoxInfo[71].sY;

    //if ((msX > sX + 110) && (msX < sX + 140) && (msY > sY + 65) && (msY < sY + 80)) {
    //	EndInputString(); 
    //	StartInputString(sX+105, sY + 50 +15*1, 10, m_cRefreshRate);
    //	m_stDialogBoxInfo[71].sV1 = 1; 
    //	PlaySound('E', 14, 5);
    //}
    //if ((msX >= sX+20) && (msX <= sX+248) && (msY >= sY +50+15*2) && (msY < sY +50+15*3)) 
    //{//Char list
    //	PlaySound('E', 14, 5);
    //	EnableDialogBox(72, 1, 0, 0);
    //}
    //if ((msX >= sX+20) && (msX <= sX+248) && (msY >= sY +50+15*3) && (msY < sY +50+15*4)) 
    //{//Map list
    //	PlaySound('E', 14, 5);
    //	EnableDialogBox(72, 2, 0, 0);
    //}
    //if ((msX >= sX+20) && (msX <= sX+248) && (msY >= sY +50+15*4) && (msY < sY +50+15*5)) 
    //{//IP list
    //	PlaySound('E', 14, 5);
    //	EnableDialogBox(72, 3, 0, 0);
    //}
    //if ((msX >= sX+20) && (msX <= sX+248) && (msY >= sY +50+15*5) && (msY < sY +50+15*6)) 
    //{//Ban list
    //	PlaySound('E', 14, 5);
    //	EnableDialogBox(72, 4, 0, 0);
    //}
    //if ((msX >= sX+160) && (msX <= sX+160 + 60) && (msY > sY + 50 +15*1) && (msY < sY + 50 +15*2))
    //{
    //	PlaySound('E', 14, 5);
    //	if (bAdminListUpdate == true)
    //		bAdminListUpdate = false;
    //	else
    //		bAdminListUpdate = true;
    //}
    if ((msX >= sX + 90) && (msX <= sX + 90 + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        PlaySound('E', 14, 5);
        DisableDialogBox(71);
    }
}
void CGame::DlgBoxClick_Config(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[73].sX;
    sY = m_stDialogBoxInfo[73].sY;

    //if ((msX > sX + 110) && (msX < sX + 140) && (msY > sY + 65) && (msY < sY + 80)) {
    //	EndInputString(); 
    //	StartInputString(sX+105, sY + 50 +15*1, 10, m_cRefreshRate);
    //	m_stDialogBoxInfo[71].sV1 = 1; 
    //	PlaySound('E', 14, 5);
    //}
    switch (m_stDialogBoxInfo[73].cMode)
    {
        default:
            if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
            {//Party config
                PlaySound('E', 14, 5);
                m_stDialogBoxInfo[73].cMode = 1;
            }
            if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
            {
                PlaySound('E', 14, 5);
                m_stDialogBoxInfo[73].cMode = 2;
            }
            if ((msX >= sX + 90) && (msX <= sX + 90 + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                PlaySound('E', 14, 5);
                DisableDialogBox(73);
            }
            break;
        case 1:
            if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
            {//Enabled/disable party list
                PlaySound('E', 14, 5);
                m_stPartyConfig.bEnabled = !m_stPartyConfig.bEnabled;
            }
            if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
            {//Party list type
                PlaySound('E', 14, 5);
                m_stPartyConfig.cType = !m_stPartyConfig.cType;
            }
            if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 4) && (msY < sY + 50 + 15 * 5))
            {//Percents
                PlaySound('E', 14, 5);
                m_stPartyConfig.bShowPercent = !m_stPartyConfig.bShowPercent;
            }

            if ((msX >= sX + 90) && (msX <= sX + 90 + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                PlaySound('E', 14, 5);
                m_stDialogBoxInfo[73].cMode = 0;
            }
            break;
    }
    //if ((msX >= sX+20) && (msX <= sX+248) && (msY >= sY +50+15*4) && (msY < sY +50+15*5)) 
    //{//IP list
    //	PlaySound('E', 14, 5);
    //	EnableDialogBox(72, 3, 0, 0);
    //}
    //if ((msX >= sX+20) && (msX <= sX+248) && (msY >= sY +50+15*5) && (msY < sY +50+15*6)) 
    //{//Ban list
    //	PlaySound('E', 14, 5);
    //	EnableDialogBox(72, 4, 0, 0);
    //}
    //if ((msX >= sX+160) && (msX <= sX+160 + 60) && (msY > sY + 50 +15*1) && (msY < sY + 50 +15*2))
    //{
    //	PlaySound('E', 14, 5);
    //	if (bAdminListUpdate == true)
    //		bAdminListUpdate = false;
    //	else
    //		bAdminListUpdate = true;
    //}
}
void CGame::DlgBoxClick_Help(int msX, int msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[35].sX;
    sY = m_stDialogBoxInfo[35].sY;


    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 0) && (msY < sY + 50 + 15 * 1))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 900, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 1) && (msY < sY + 50 + 15 * 2))
    {
        DisableDialogBox(18);

#ifdef DEF_HTMLCOMMOM
        WebDialog();
#else
        EnableDialogBox(18, 1000, 0, 0);
#endif
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 901, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 902, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 4) && (msY < sY + 50 + 15 * 5))
    {

        DisableDialogBox(18);
        EnableDialogBox(18, 903, 0, 0);
        m_bIsF1HelpWindowEnabled = true;
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 5) && (msY < sY + 50 + 15 * 6))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 904, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 6) && (msY < sY + 50 + 15 * 7))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 905, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 7) && (msY < sY + 50 + 15 * 8))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 906, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 8) && (msY < sY + 50 + 15 * 9))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 907, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 9) && (msY < sY + 50 + 15 * 10))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 908, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 10) && (msY < sY + 50 + 15 * 11))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 909, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 11) && (msY < sY + 50 + 15 * 12))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 910, 0, 0);
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 12) && (msY < sY + 50 + 15 * 13))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 911, 0, 0); // FAQ
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 13) && (msY < sY + 50 + 15 * 14))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 912, 0, 0);
    }
    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        PlaySound('E', 14, 5);
        DisableDialogBox(35);
    }
}

void CGame::DlbBoxDoubleClick_Character(short msX, short msY)
{
    char cEquipPoiStatus[DEF_MAXITEMEQUIPPOS]{}, cItemID = -1;
    short sX, sY, sSprH, sFrame;
    int i;
    if (m_bIsDialogEnabled[17] == true) return;
    sX = m_stDialogBoxInfo[1].sX;
    sY = m_stDialogBoxInfo[1].sY;

    for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
        cEquipPoiStatus[i] = -1;

    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))	cEquipPoiStatus[m_pItemList[i]->m_cEquipPos] = i;
    }

    if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
    {
        if (cEquipPoiStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 41, sY + 137, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_BACK];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_PANTS];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_ARMS];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_BOOTS];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_BODY];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 90, sY + 170, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_LHAND];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 57, sY + 186, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_RHAND];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 57, sY + 186, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 35, sY + 120, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_NECK];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 32, sY + 193, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_RFINGER];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 72, sY + 135, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_HEAD];
        }
    }
    else if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
    {
        if (cEquipPoiStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 45, sY + 143, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_BACK];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_BOOTS];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_PANTS];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_ARMS];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_BOOTS];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_BODY];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 84, sY + 175, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_LHAND];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 60, sY + 191, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_RHAND];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 60, sY + 191, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 35, sY + 120, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_NECK];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 32, sY + 193, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_RFINGER];
        }
        if (cEquipPoiStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 72, sY + 139, sFrame, msX, msY))
                cItemID = cEquipPoiStatus[DEF_EQUIPPOS_HEAD];
        }
    }

    if (cItemID == -1 || m_pItemList[cItemID] == 0) return;
    if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EAT) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW) || (m_pItemList[cItemID]->m_dwCount > 1)) return;
    if ((m_bIsDialogEnabled[11] == true) && (m_bIsDialogEnabled[23] == false) && (m_stDialogBoxInfo[39].sV3 == 24))
        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_REPAIRITEM, 0, cItemID, m_stDialogBoxInfo[39].sV3, 0, m_pItemList[cItemID]->m_cName, m_stDialogBoxInfo[39].sV4); // v1.4
    else
    {
        if (m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] == true)
        {
            char cStr1[64], cStr2[64], cStr3[64];
            GetItemName(m_pItemList[m_stMCursor.sSelectedObjectID], cStr1, cStr2, cStr3, 64);
            memset(G_cTxt, 0, sizeof(G_cTxt));
            format_to_local(G_cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
            AddEventList(G_cTxt, 10);

            PlaySound('E', 29, 0);
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_RELEASEITEM, 0, m_stMCursor.sSelectedObjectID, 0, 0, 0);
            m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] = false;
            m_sItemEquipmentStatus[m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos] = -1;
            m_stMCursor.cSelectedObjectType = 0;
            m_stMCursor.sSelectedObjectID = 0;
        }
    }
}

void CGame::DlbBoxDoubleClick_GuideMap(short msX, short msY)
{
    short si = m_stMCursor.sCursorFrame;
    if (si != 0) return;

    if (m_cMapIndex < 0) return;
    short sX, sY, shX, shY, szX, szY;
    sX = m_stDialogBoxInfo[9].sX;
    sY = m_stDialogBoxInfo[9].sY;
    szX = m_stDialogBoxInfo[9].sSizeX;
    szY = m_stDialogBoxInfo[9].sSizeY;
    if (sX < 20) sX = 0;
    if (sY < 20) sY = 0;
    if (sX > get_virtual_width() - 128 - 20) sX = get_virtual_width() - 128;
    if (sY > get_virtual_height() - 53 - 128 - 20) sY = get_virtual_height() - 53 - 128;
    if (m_bZoomMap)
    {
        shX = m_sPlayerX - 64;
        shY = m_sPlayerY - 64;
        if (shX < 0) shX = 0;
        if (shY < 0) shY = 0;
        if (shX > m_pMapData->m_sMapSizeX - 128) shX = m_pMapData->m_sMapSizeX - 128;
        if (shY > m_pMapData->m_sMapSizeY - 128) shY = m_pMapData->m_sMapSizeY - 128;
        shX = shX + msX - sX;
        shY = shY + msY - sY;
    }
    else
    {
        shX = (m_pMapData->m_sMapSizeX * (msX - sX)) / 128;
        shY = (m_pMapData->m_sMapSizeX * (msY - sY)) / 128;
    }

    if (shX < 30 || shY < 30) return;
    if (shX > m_pMapData->m_sMapSizeX - 30 || shY > m_pMapData->m_sMapSizeY - 30) return;

#ifdef DEF_HACKCLIENT
    format_to_local(G_cTxt, "/tp {} {} {}", m_cMapName, shX, shY);
    bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, G_cTxt);
#else
    if ((m_bRunningMode == true) && (m_iSP > 0))
        m_cCommand = DEF_OBJECTRUN;
    else m_cCommand = DEF_OBJECTMOVE;
    m_sCommX = shX;
    m_sCommY = shY;
    GetPlayerTurn();
#endif
}

void CGame::DlbBoxDoubleClick_Inventory(short msX, short msY)
{
    register int i;
    char  cItemID, cTxt[120];
    short sX, sY, x1, x2, y1, y2;
    char cStr1[64], cStr2[64], cStr3[64];
    if (m_iHP <= 0) return;
#ifndef DEF_HACKCLIENT	
    if (m_bItemUsingStatus == true)
    {//Change Item Use Hack (faster)
        AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY1, 10);
        return;
    }
#endif

    sX = m_stDialogBoxInfo[2].sX;
    sY = m_stDialogBoxInfo[2].sY;


    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if (m_cItemOrder[DEF_MAXITEMS - 1 - i] == -1) continue;
        cItemID = m_cItemOrder[DEF_MAXITEMS - 1 - i];
        if (m_pItemList[cItemID] == 0) continue;

        m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->get_sprite_rect(sX + 32 + m_pItemList[cItemID]->m_sX, sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame);
        x1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
        y1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
        x2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
        y2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

        if ((m_bIsItemDisabled[cItemID] == false) && (m_bIsItemEquipped[cItemID] == false) && (msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
        {
            _SetItemOrder(0, cItemID);
            GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3, 64);

            if (m_bIsDialogEnabled[11] && (m_bIsDialogEnabled[23] == false) && (m_bIsDialogEnabled[23] == false) && (m_stDialogBoxInfo[39].sV3 == 24))
            {
                if (m_pItemList[cItemID]->m_cEquipPos != DEF_EQUIPPOS_NONE)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_REPAIRITEM, 0, cItemID, m_stDialogBoxInfo[39].sV3, 0, m_pItemList[cItemID]->m_cName, m_stDialogBoxInfo[39].sV4); // v1.4
                    return;
                }
            }
            if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_PERM) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EAT))
            {
                if (bCheckItemOperationEnabled(cItemID) == false) return;
#ifndef DEF_HACKCLIENT
                if ((m_dwCurTime - m_dwDamagedTime) < 10000)
                {//Change Item use after damage hack

                    if ((m_pItemList[cItemID]->m_sSprite == 6) && (m_pItemList[cItemID]->m_sSpriteFrame == 9))
                    {
                        format_to_local(G_cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY3, cStr1);
                        AddEventList(G_cTxt, 10);
                        return;
                    }

                    // v2.04
                    if ((m_pItemList[cItemID]->m_sSprite == 6) && (m_pItemList[cItemID]->m_sSpriteFrame == 89))
                    {
                        format_to_local(G_cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY3, cStr1);
                        AddEventList(G_cTxt, 10);
                        return;
                    }
                }
#endif

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, cItemID, 0, 0, 0);

                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
                    (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EAT))
                {
                    m_bIsItemDisabled[cItemID] = true;
                    m_bItemUsingStatus = true;
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_SKILL)
            {
                if (_bIsItemOnHand() == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);
                    return;
                }

                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY5, 10);
                    return;
                }

                if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY6, 10);
                }
                else
                {
                    m_bIsGetPointingMode = true;
                    m_iPointCommandType = cItemID;
                    format_to_local(cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY7, cStr1);
                    AddEventList(cTxt, 10);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST)
            {
                if (_bIsItemOnHand() == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);
                    return;
                }

                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY13, 10);
                    return;
                }

                if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY6, 10);
                }
                else
                {
                    m_bIsGetPointingMode = true;
                    m_iPointCommandType = cItemID;
                    format_to_local(cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY8, cStr1);
                    AddEventList(cTxt, 10);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX)
            {
                if (_bIsItemOnHand() == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);
                    return;
                }

                if (m_bSkillUsingStatus == true)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY5, 10);
                    return;
                }

                if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY6, 10);
                }
                else
                {

                    switch (m_pItemList[cItemID]->m_sSpriteFrame)
                    {
                        case 55:
                            // ¿¬±Ý¼ú±×¸©
                            if (m_cSkillMastery[12] == 0)
                            {
                                AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY9, 10);
                            }
                            else
                            {
                                EnableDialogBox(26, 1, 0, 0, 0);
                                AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY10, 10);
                            }
                            break;

                        case 113:
                            if (m_cSkillMastery[13] == 0)
                            {
                                AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY11, 10);
                            }
                            else
                            {
                                EnableDialogBox(26, 3, 0, 0, 0);
                                AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY12, 10);
                            }
                            break;
                        case 151:
                        case 152:
                        case 153:
                        case 154:
                            EnableDialogBox(40, 1, 0, 0, 0);
                    }
                }
            }


            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EQUIP)
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = (short)cItemID;
                bItemDrop_Character();
                m_stMCursor.cSelectedObjectType = 0;
                m_stMCursor.sSelectedObjectID = 0;
            }
            return;
        }
    }
}

void CGame::DlgBoxClick_SysMenu(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[19].sX;
    sY = m_stDialogBoxInfo[19].sY;

    if ((msX >= sX + 120) && (msX <= sX + 150) && (msY >= sY + 63) && (msY <= sY + 74))
    {
        m_cDetailLevel = 0; // v1.41
        AddEventList(NOTIFY_MSG_DETAIL_LEVEL_LOW, 10);
        PlaySound('E', 14, 5);
    }

    if ((msX >= sX + 151) && (msX <= sX + 200) && (msY >= sY + 63) && (msY <= sY + 74))
    {
        m_cDetailLevel = 1;
        AddEventList(NOTIFY_MSG_DETAIL_LEVEL_MEDIUM, 10);
        PlaySound('E', 14, 5);
    }

    if ((msX >= sX + 201) && (msX <= sX + 234) && (msY >= sY + 63) && (msY <= sY + 74))
    {
        m_cDetailLevel = 2;
        AddEventList(NOTIFY_MSG_DETAIL_LEVEL_HIGH, 10);
        PlaySound('E', 14, 5);
    }

    if ((msX >= sX + 24) && (msX <= sX + 115) && (msY >= sY + 81) && (msY <= sY + 100))
    {
        if (m_bSoundFlag)
        {
            if (m_bSoundStat == true)
            {
                m_pESound[38].stop();
                m_bSoundStat = false;
                AddEventList(NOTIFY_MSG_SOUND_OFF, 10);
            }
            else
            {
                m_bSoundStat = true;
                AddEventList(NOTIFY_MSG_SOUND_ON, 10);
            }
        }
    }

    if ((msX >= sX + 116) && (msX <= sX + 202) && (msY >= sY + 81) && (msY <= sY + 100))
    {
        if (m_bSoundFlag)
        {
            if (m_bMusicStat == true)
            {
                // Music Off
                m_bMusicStat = false;
                AddEventList(NOTIFY_MSG_MUSIC_OFF, 10);
                if (m_bSoundFlag)
                {
                    m_pBGM.stop();
                }
            }
            else
            {
                // Music On
                if (m_bSoundFlag)
                {
                    m_bMusicStat = true;
                    AddEventList(NOTIFY_MSG_MUSIC_ON, 10);
                    StartBGM();
                }
            }
        }
    }


    if ((msX >= sX + 23) && (msX <= sX + 108) && (msY >= sY + 108) && (msY <= sY + 119))
    {
        if (m_bWhisper == true)
        {
            m_bWhisper = false;
            AddEventList(BCHECK_LOCAL_CHAT_COMMAND7, 10);
        }
        else
        {
            m_bWhisper = true;
            AddEventList(BCHECK_LOCAL_CHAT_COMMAND6, 10);
        }
    }

    if ((msX >= sX + 123) && (msX <= sX + 203) && (msY >= sY + 108) && (msY <= sY + 119))
    {
        if (m_bShout == true)
        {
            m_bShout = false;
            AddEventList(BCHECK_LOCAL_CHAT_COMMAND9, 10);
        }
        else
        {
            m_bShout = true;
            AddEventList(BCHECK_LOCAL_CHAT_COMMAND8, 10);
        }
    }


    //Transparency Change
    if ((msX >= sX + 28) && (msX <= sX + 235) && (msY >= sY + 156) && (msY <= sY + 171)) m_bDialogTrans = !m_bDialogTrans;

    //Guide Map Toggle
    if ((msX >= sX + 28) && (msX <= sX + 235) && (msY >= sY + 178) && (msY <= sY + 193))
    {
        if (m_bIsDialogEnabled[9]) DisableDialogBox(9);
        else EnableDialogBox(9, 0, 0, 0, 0);
    }

    if (m_bForceDisconn) return;
    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + 225) && (msY <= sY + 225 + DEF_BTNSZY))
    {
        if (m_cLogOutCount == -1)
        {
            //#ifdef _DEBUG //Change //
#ifdef DEF_HACKCLIENT
            m_cLogOutCount = 1;
#else
            m_cLogOutCount = 11;
#endif
            for (int i = 0; i < 50; i++)//Change bag fix?
            {
                if (m_pItemList[i] != 0)
                    bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, i, m_pItemList[i]->m_sX, m_pItemList[i]->m_sY, 0, 0);
            }
            //#else
            //			m_cLogOutCount = 11;
            //#endif
        }
        else
        {
            m_cLogOutCount = -1;
            AddEventList(DLGBOX_CLICK_SYSMENU2, 10);
            DisableDialogBox(19);
        }
        PlaySound('E', 14, 5);
    }

    if ((m_iHP <= 0) && (m_cRestartCount == -1))
    {
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 225) && (msY <= sY + 225 + DEF_BTNSZY))
        {
#ifdef DEF_HACKCLIENT
            m_cRestartCount = 0;
#else
            m_cRestartCount = 5;
#endif
            m_dwRestartCountTime = m_dwCurTime;
            DisableDialogBox(19);
            format_to_local(G_cTxt, DLGBOX_CLICK_SYSMENU1, m_cRestartCount);
            AddEventList(G_cTxt, 10);
            PlaySound('E', 14, 5);
        }
    }
}

void CGame::bItemDrop_Character()
{
    ItemEquipHandler((char)m_stMCursor.sSelectedObjectID);
}

void CGame::bItemDrop_Inventory(short msX, short msY)
{
    short sX, sY, dX, dY;
    char  cTxt[120];
    if (m_cCommand < 0) return;
    if (m_pItemList[m_stMCursor.sSelectedObjectID] == 0) return;

    if ((m_bSkillUsingStatus == true) && (m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] == true))
    {
        AddEventList(BITEMDROP_INVENTORY1, 10);
        return;
    }
    if (m_bIsItemDisabled[m_stMCursor.sSelectedObjectID] == true) return;

    sY = m_stDialogBoxInfo[2].sY;
    sX = m_stDialogBoxInfo[2].sX;

    dX = msX - sX - 32 - m_stMCursor.sDistX;
    dY = msY - sY - 44 - m_stMCursor.sDistY;

    // v1.4
    if (dY < -10) dY = -10;
    if (dX < 0)   dX = 0;
    if (dX > 170) dX = 170;
    if (dY > 95) dY = 95;

    m_pItemList[m_stMCursor.sSelectedObjectID]->m_sX = dX;
    m_pItemList[m_stMCursor.sSelectedObjectID]->m_sY = dY;

    short sTmpSpr, sTmpSprFrm;
    sTmpSpr = m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite;
    sTmpSprFrm = m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame;
    /*
        if( m_bShiftPressed && (sTmpSpr == 6) && (sTmpSprFrm > 0) && (sTmpSprFrm <= 6) && (sTmpSprFrm != 5) )
        {
            for( int i=0 ; i<DEF_MAXITEMS ; i++ )
            {
                if( m_pItemList[i] == 0 ) continue;
                if( (m_pItemList[i]->m_sSprite == sTmpSpr) && (m_pItemList[i]->m_sSpriteFrame == sTmpSprFrm) )
                {
                    m_pItemList[i]->m_sX = dX;
                    m_pItemList[i]->m_sY = dY;
                    bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, i, dX, dY, 0, 0);
                }
            }
        }
        else bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, m_stMCursor.sSelectedObjectID, dX, dY, 0, 0);
    */
    char cItemID;
    if (m_bShiftPressed)
    {
        for (int i = 0; i < DEF_MAXITEMS; i++)
        {
            if (m_cItemOrder[DEF_MAXITEMS - 1 - i] != -1) // White if
            {
                cItemID = m_cItemOrder[DEF_MAXITEMS - 1 - i];
                //Blue if
                if (m_pItemList[cItemID] != 0 && memcmp(m_pItemList[cItemID]->m_cName, m_pItemList[m_stMCursor.sSelectedObjectID]->m_cName, 20) == 0)
                {
                    m_pItemList[cItemID]->m_sX = dX;
                    m_pItemList[cItemID]->m_sY = dY;
                    bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, cItemID, dX, dY, 0, 0);
                } // Close Blue if
            } // Close White if
        } // Close for loop
    }
    else
        bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, (char)(m_stMCursor.sSelectedObjectID), dX, dY, 0, 0);

    if (m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] == true)
    {

        char cStr1[64], cStr2[64], cStr3[64];
        GetItemName(m_pItemList[m_stMCursor.sSelectedObjectID], cStr1, cStr2, cStr3, 64);

        format_to_local(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
        AddEventList(cTxt, 10);

        PlaySound('E', 29, 0);
        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_RELEASEITEM, 0, m_stMCursor.sSelectedObjectID, 0, 0, 0);
        m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] = false;
        m_sItemEquipmentStatus[m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos] = -1;
    }
}

void CGame::bItemDrop_SellList(short msX, short msY)
{
    int i;
    char cItemID;

    cItemID = (char)m_stMCursor.sSelectedObjectID;

    if (m_pItemList[cItemID] == 0) return;
    if (m_bIsItemDisabled[cItemID] == true) return;
    if (m_cCommand < 0) return;
    for (i = 0; i < DEF_MAXSELLLIST; i++)
        if (m_stSellItemList[i].iIndex == cItemID)
        {
            AddEventList(BITEMDROP_SELLLIST1, 10);
            return;
        }
    if (strcmp(m_pItemList[cItemID]->m_cName, "Gold") == 0)
    {
        AddEventList(BITEMDROP_SELLLIST2, 10);
        return;
    }
    if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
    {
        memset(G_cTxt, 0, sizeof(G_cTxt));
        char cStr1[64], cStr2[64], cStr3[64];
        GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3, 64);
        format_to_local(G_cTxt, NOTIFYMSG_CANNOT_SELL_ITEM2, cStr1);
        AddEventList(G_cTxt, 10);
        return;
    }
    if (((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
        (m_pItemList[cItemID]->m_dwCount > 1))
    {

        m_stDialogBoxInfo[17].sX = msX - 140;
        m_stDialogBoxInfo[17].sY = msY - 70;
        if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;

        m_stDialogBoxInfo[17].sV1 = m_sPlayerX + 1;
        m_stDialogBoxInfo[17].sV2 = m_sPlayerY + 1;
        m_stDialogBoxInfo[17].sV3 = 1001;
        m_stDialogBoxInfo[17].sV4 = cItemID;

        memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
        EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
        m_bIsItemDisabled[cItemID] = true;
    }
    else
    {
        for (i = 0; i < DEF_MAXSELLLIST; i++)
            if (m_stSellItemList[i].iIndex == -1)
            {
                m_stSellItemList[i].iIndex = cItemID;
                m_stSellItemList[i].iAmount = 1;
                m_bIsItemDisabled[cItemID] = true;
                return;
            }
        AddEventList(BITEMDROP_SELLLIST3, 10);
    }
}

void CGame::bItemDrop_ItemUpgrade()
{
    char cItemID;
    cItemID = (char)m_stMCursor.sSelectedObjectID;
    if (m_bIsItemDisabled[cItemID] == true) return;
    if (m_cCommand < 0) return;
    if (m_pItemList[cItemID]->m_cEquipPos == DEF_EQUIPPOS_NONE) return;

    switch (m_stDialogBoxInfo[34].cMode)
    {
        case 1:
            m_bIsItemDisabled[m_stDialogBoxInfo[34].sV1] = false;
            m_stDialogBoxInfo[34].sV1 = cItemID;
            m_bIsItemDisabled[cItemID] = true;
            PlaySound('E', 29, 0);
            break;

        case 6:
            m_bIsItemDisabled[m_stDialogBoxInfo[34].sV1] = false;
            m_stDialogBoxInfo[34].sV1 = cItemID;
            m_bIsItemDisabled[cItemID] = true;
            PlaySound('E', 29, 0);
            break;
    }
}

void CGame::bItemDrop_Bank(short msX, short msY)
{
    m_stDialogBoxInfo[39].sV1 = m_stMCursor.sSelectedObjectID;
    if (m_cCommand < 0) return;
    if (m_pItemList[m_stDialogBoxInfo[39].sV1] == 0) return;
    if (m_bIsItemDisabled[m_stDialogBoxInfo[39].sV1] == true) return;
    if (m_bIsDialogEnabled[17] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }
    if ((m_bIsDialogEnabled[20] == true) && ((m_stDialogBoxInfo[20].cMode == 1) || (m_stDialogBoxInfo[20].cMode == 2)))
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }
    if (m_bIsDialogEnabled[23] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    if (m_bIsDialogEnabled[4] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }


    if (((m_pItemList[m_stDialogBoxInfo[39].sV1]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[m_stDialogBoxInfo[39].sV1]->m_cItemType == DEF_ITEMTYPE_ARROW)) && (m_pItemList[m_stDialogBoxInfo[39].sV1]->m_dwCount > 1))
    {
        m_stDialogBoxInfo[17].sX = msX - 140;
        m_stDialogBoxInfo[17].sY = msY - 70;
        if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;

        m_stDialogBoxInfo[17].sV1 = m_sPlayerX + 1;
        m_stDialogBoxInfo[17].sV2 = m_sPlayerY + 1;
        m_stDialogBoxInfo[17].sV3 = 1002;
        m_stDialogBoxInfo[17].sV4 = m_stDialogBoxInfo[39].sV1;

        memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
        EnableDialogBox(17, m_stDialogBoxInfo[39].sV1, m_pItemList[m_stDialogBoxInfo[39].sV1]->m_dwCount, 0);
    }
    else
    {
        if (_iGetBankItemCount() >= (DEF_MAXBANKITEMS - 1)) AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
        else bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, m_stDialogBoxInfo[39].sV1, 1, m_stDialogBoxInfo[39].sV5, m_stDialogBoxInfo[39].sV6, m_pItemList[m_stDialogBoxInfo[39].sV1]->m_cName, m_stDialogBoxInfo[39].sV4); //v1.4
    }
}

void CGame::bItemDrop_SkillDialog()
{
    int iConsumeNum;
    char cItemID;
    if (m_cCommand < 0) return;

    // v1.4
    cItemID = (char)m_stMCursor.sSelectedObjectID;
    if (m_pItemList[cItemID] == 0) return;
    if (m_bIsItemDisabled[cItemID] == true) return;
    if (m_bIsDialogEnabled[17] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    if ((m_bIsDialogEnabled[20] == true) &&
        ((m_stDialogBoxInfo[20].cMode == 1) || (m_stDialogBoxInfo[20].cMode == 2)))
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    if (m_bIsDialogEnabled[23] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    switch (m_stDialogBoxInfo[26].cMode)
    {
        case 1:
            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME)
            {
                iConsumeNum = 0;
                if (m_stDialogBoxInfo[26].sV1 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV2 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV3 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV4 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV5 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV6 == cItemID) iConsumeNum++;
                if (iConsumeNum >= (int)(m_pItemList[cItemID]->m_dwCount)) return;
            }

            if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EAT) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_NONE))
            {

            }
            else return;
            if (m_stDialogBoxInfo[26].sV1 == -1)
            {
                m_stDialogBoxInfo[26].sV1 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV2 == -1)
            {
                m_stDialogBoxInfo[26].sV2 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV3 == -1)
            {
                m_stDialogBoxInfo[26].sV3 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV4 == -1)
            {
                m_stDialogBoxInfo[26].sV4 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV5 == -1)
            {
                m_stDialogBoxInfo[26].sV5 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV6 == -1)
            {
                m_stDialogBoxInfo[26].sV6 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            AddEventList(BITEMDROP_SKILLDIALOG4, 10);
            break;

        case 4:
            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME)
            {
                iConsumeNum = 0;
                if (m_stDialogBoxInfo[26].sV1 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV2 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV3 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV4 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV5 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV6 == cItemID) iConsumeNum++;
                if (iConsumeNum >= (int)(m_pItemList[cItemID]->m_dwCount)) return;
            }
            if (m_stDialogBoxInfo[26].sV1 == -1)
            {
                m_stDialogBoxInfo[26].sV1 = cItemID;
                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV2 == -1)
            {
                m_stDialogBoxInfo[26].sV2 = cItemID;
                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV3 == -1)
            {
                m_stDialogBoxInfo[26].sV3 = cItemID;
                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV4 == -1)
            {
                m_stDialogBoxInfo[26].sV4 = cItemID;
                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV5 == -1)
            {
                m_stDialogBoxInfo[26].sV5 = cItemID;
                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV6 == -1)
            {
                m_stDialogBoxInfo[26].sV6 = cItemID;
                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {
                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            AddEventList(BITEMDROP_SKILLDIALOG4, 10);
            break;

        default:
            break;
    }
}

void CGame::DlgBoxClick_Bank(short msX, short msY)
{
    int i;
    short sX, sY;


    sX = m_stDialogBoxInfo[14].sX;
    sY = m_stDialogBoxInfo[14].sY;

    switch (m_stDialogBoxInfo[14].cMode)
    {
        case -1:
            break;

        case 0:
            for (i = 0; i < m_stDialogBoxInfo[14].sV1; i++)
                if ((msX > sX + 30) && (msX < sX + 210) && (msY >= sY + 110 + i * 15) && (msY <= sY + 124 + i * 15))
                {
                    if ((m_pBankList[m_stDialogBoxInfo[14].sView + i] != 0) && ((m_stDialogBoxInfo[14].sView + i) < DEF_MAXBANKITEMS))
                    {
                        if (_iGetTotalItemNum() >= 50)
                        {
                            AddEventList(DLGBOX_CLICK_BANK1, 10);
                            return;
                        }
                        bSendCommand(MSGID_REQUEST_RETRIEVEITEM, 0, 0, (m_stDialogBoxInfo[14].sView + i), 0, 0, 0);
                        m_stDialogBoxInfo[14].cMode = -1;
                        PlaySound('E', 14, 5);
                    }
                    return;
                }

            break;
    }
}

void CGame::DlgBoxClick_Fish(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[24].sX;
    sY = m_stDialogBoxInfo[24].sY;

    switch (m_stDialogBoxInfo[24].cMode)
    {
        case 0:
            if ((msX >= sX + 160) && (msX <= sX + 253) && (msY >= sY + 70) && (msY <= sY + 90))
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETFISHTHISTIME, 0, 0, 0, 0, 0);
                AddEventList(DLGBOX_CLICK_FISH1, 10);
                DisableDialogBox(24);

                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DlgBoxClick_Magic(short msX, short msY)
{
    int i, iCPivot, iYloc;
    short sX, sY;

    sX = m_stDialogBoxInfo[3].sX;
    sY = m_stDialogBoxInfo[3].sY;
    iCPivot = m_stDialogBoxInfo[3].sView * 10;
    iYloc = 0;

    for (i = 0; i < 9; i++)
    {
        if (((m_cMagicMastery[iCPivot + i] != 0) && (m_pMagicCfgList[iCPivot + i] != 0))/* || (iCPivot == 10)*/)//Change Added circle 1 spells to always display
        {
            if ((msX >= sX + 30) && (msX <= sX + 240) && (msY >= sY + 70 + iYloc) && (msY <= sY + 70 + 18 + iYloc))
            {
                UseMagic(iCPivot + i);
                PlaySound('E', 14, 5);
                return;
            }
            iYloc += 18;
        }
    }

    if ((msX >= sX + 16) && (msX <= sX + 38) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 0;
    if ((msX >= sX + 39) && (msX <= sX + 56) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 1;
    if ((msX >= sX + 57) && (msX <= sX + 81) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 2;
    if ((msX >= sX + 82) && (msX <= sX + 101) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 3;
    if ((msX >= sX + 102) && (msX <= sX + 116) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 4;
    if ((msX >= sX + 117) && (msX <= sX + 137) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 5;
    if ((msX >= sX + 138) && (msX <= sX + 165) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 6;
    if ((msX >= sX + 166) && (msX <= sX + 197) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 7;
    if ((msX >= sX + 198) && (msX <= sX + 217) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 8;
    if ((msX >= sX + 218) && (msX <= sX + 239) && (msY >= sY + 240) && (msY <= sY + 268))
        m_stDialogBoxInfo[3].sView = 9;

    // v2.15
    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 285) && (msY <= sY + 285 + DEF_BTNSZY))
    {
        // ¿¬±Ý¼ú 
        if (m_cSkillMastery[12] == 0) AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY16, 10);
        else
        {
            for (i = 0; i < DEF_MAXITEMS; i++)
                if ((m_pItemList[i] != 0) && (m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX) &&
                    (m_pItemList[i]->m_sSpriteFrame == 55))
                {

                    EnableDialogBox(26, 1, 0, 0, 0);
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY10, 10);
                    PlaySound('E', 14, 5);
                    return;
                }
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY15, 10);
        }
        PlaySound('E', 14, 5);
    }
}

void CGame::DlgBoxClick_NpcActionQuery(short msX, short msY)
{
    short sX, sY;
    int   absX, absY;
    if (m_bIsDialogEnabled[27] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    sX = m_stDialogBoxInfo[20].sX;
    sY = m_stDialogBoxInfo[20].sY;

    switch (m_stDialogBoxInfo[20].cMode)
    {
        case 0:
            if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
            {
                EnableDialogBox(m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV2, 0, 0);
                DisableDialogBox(20);
            }

            if ((m_bIsDialogEnabled[21] == false) && (msX > sX + 125) && (msX < sX + 180) && (msY > sY + 55) && (msY < sY + 70))
            {
                switch (m_stDialogBoxInfo[20].sV1)
                {
                    case 7:
                        // Guild
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 1, 0, 0, 0);
                        AddEventList(TALKING_TO_GUILDHALL_OFFICER, 10);
                        break;

                    case 11:
                        // 
                        switch (m_stDialogBoxInfo[20].sV2)
                        {
                            case 1:

                                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 2, 0, 0, 0);
                                AddEventList(TALKING_TO_SHOP_KEEPER, 10);
                                break;
                            case 2:

                                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 3, 0, 0, 0);
                                AddEventList(TALKING_TO_BLACKSMITH_KEEPER, 10);
                                break;
                        }
                        break;

                    case 13:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 4, 0, 0, 0);
                        AddEventList(TALKING_TO_CITYHALL_OFFICER, 10);
                        break;

                    case 14:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 5, 0, 0, 0);
                        AddEventList(TALKING_TO_WAREHOUSE_KEEPER, 10);
                        break;

                    case 16:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 6, 0, 0, 0);
                        AddEventList(TALKING_TO_MAGICIAN, 10);
                        break;
                }
                DisableDialogBox(20);
            }
            break;

        case 1:
            if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
            {
                absX = abs(m_stDialogBoxInfo[20].sV5 - m_sPlayerX);
                absY = abs(m_stDialogBoxInfo[20].sV6 - m_sPlayerY);

                if ((absX <= 4) && (absY <= 4))
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV3, m_stDialogBoxInfo[20].sV5, m_stDialogBoxInfo[20].sV6, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4); //v1.4
                else AddEventList(DLGBOX_CLICK_NPCACTION_QUERY7, 10);
                DisableDialogBox(20);
            }
            else
                if ((msX > sX + 155) && (msX < sX + 210) && (msY > sY + 55) && (msY < sY + 70))
                {
                    absX = abs(m_stDialogBoxInfo[20].sV5 - m_sPlayerX);
                    absY = abs(m_stDialogBoxInfo[20].sV6 - m_sPlayerY);

                    if ((absX <= 4) && (absY <= 4))
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_EXCHANGEITEMTOCHAR, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV3, m_stDialogBoxInfo[20].sV5, m_stDialogBoxInfo[20].sV6, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4); //v1.4
                    else AddEventList(DLGBOX_CLICK_NPCACTION_QUERY8, 10);
                    DisableDialogBox(20);
                }
            break;

        case 2:
            if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_SELLITEM, 0, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV2, m_stDialogBoxInfo[20].sV3, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4); // v1.4
                DisableDialogBox(20);
            }
            else
                if ((msX > sX + 125) && (msX < sX + 180) && (msY > sY + 55) && (msY < sY + 70))
                {
                    if (m_stDialogBoxInfo[20].sV3 == 1)
                    {
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_REPAIRITEM, 0, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV2, 0, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4); // v1.4
                        DisableDialogBox(20);
                    }
                }
            break;

        case 3:
            if ((msX > sX + 25) && (msX < sX + 105) && (msY > sY + 55) && (msY < sY + 70))
            {
                absX = abs(m_stDialogBoxInfo[20].sV5 - m_sPlayerX);
                absY = abs(m_stDialogBoxInfo[20].sV6 - m_sPlayerY);
                if ((absX <= 8) && (absY <= 8))
                {
                    if (_iGetBankItemCount() >= (DEF_MAXBANKITEMS - 1))
                    {
                        AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
                    }
                    else bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV3, m_stDialogBoxInfo[20].sV5, m_stDialogBoxInfo[20].sV6, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4); //v1.4
                }
                else AddEventList(DLGBOX_CLICK_NPCACTION_QUERY7, 10);

                DisableDialogBox(20);
            }
            break;

        case 4:
            if ((m_bIsDialogEnabled[21] == false) && (msX > sX + 125) && (msX < sX + 180) && (msY > sY + 55) && (msY < sY + 70))
            {
                switch (m_stDialogBoxInfo[20].sV3)
                {
                    case 21:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 21, 0, 0, 0);
                        AddEventList(TALKING_TO_GUARD, 10);
                        break;

                    case 32:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 32, 0, 0, 0);
                        AddEventList(TALKING_TO_UNICORN, 10);
                        break;
                    case 67:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 67, 0, 0, 0);
                        AddEventList(TALKING_TO_MCGAFFIN, 10);
                        break;
                    case 68:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 68, 0, 0, 0);
                        AddEventList(TALKING_TO_PERRY, 10);
                        break;
                    case 69:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 69, 0, 0, 0);
                        AddEventList(TALKING_TO_DEVLIN, 10);
                        break;
                }
            }
            DisableDialogBox(20);
            break;


        case 5:
            if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
            {
                EnableDialogBox(m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV2, 0, 0);
                DisableDialogBox(20);
            }

            if ((msX > sX + 25 + 75) && (msX < sX + 80 + 75) && (msY > sY + 55) && (msY < sY + 70))
            {
                EnableDialogBox(31, 0, 0, 0);
                DisableDialogBox(20);
            }

            if ((m_bIsDialogEnabled[21] == false) && (msX > sX + 155) && (msX < sX + 210) && (msY > sY + 55) && (msY < sY + 70))
            {
                switch (m_stDialogBoxInfo[20].sV1)
                {
                    case 7:
                        // Guild
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 1, 0, 0, 0);
                        AddEventList(TALKING_TO_GUILDHALL_OFFICER, 10);
                        break;

                    case 11:
                        // 
                        switch (m_stDialogBoxInfo[20].sV2)
                        {
                            case 1:

                                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 2, 0, 0, 0);
                                AddEventList(TALKING_TO_SHOP_KEEPER, 10);
                                break;
                            case 2:

                                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 3, 0, 0, 0);
                                AddEventList(TALKING_TO_BLACKSMITH_KEEPER, 10);
                                break;
                        }
                        break;

                    case 13:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 4, 0, 0, 0);
                        AddEventList(TALKING_TO_CITYHALL_OFFICER, 10);
                        break;

                    case 14:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 5, 0, 0, 0);
                        AddEventList(TALKING_TO_WAREHOUSE_KEEPER, 10);
                        break;

                    case 16:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, 0, 6, 0, 0, 0);
                        AddEventList(TALKING_TO_MAGICIAN, 10);
                        break;
                }
                DisableDialogBox(20);
            }
            break;
        case 6:
            if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
            {
                EnableDialogBox(51, 0, 0, 0);
                DisableDialogBox(20);
            }
    }
}


void CGame::DlgBoxClick_Shop(short msX, short msY)
{
    int i;
    short sX, sY;
    char cTemp[21];


    sX = m_stDialogBoxInfo[11].sX;
    sY = m_stDialogBoxInfo[11].sY;

    switch (m_stDialogBoxInfo[11].cMode)
    {
        case 0:
            for (i = 0; i < 13; i++)
                if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 65) && (msY <= sY + i * 18 + 79))
                {
                    if (_iGetTotalItemNum() >= 50)
                    {
                        AddEventList(DLGBOX_CLICK_SHOP1, 10);
                        return;
                    }

                    PlaySound('E', 14, 5);
                    if (m_pItemForSaleList[m_stDialogBoxInfo[11].sView + i] != 0)
                        m_stDialogBoxInfo[11].cMode = m_stDialogBoxInfo[11].sView + i + 1;
                    return;
                }
            break;

        default:
            if ((msX >= sX + 145) && (msX <= sX + 162) && (msY >= sY + 209) && (msY <= sY + 230))
            {

                m_stDialogBoxInfo[11].sV3 += 10;
                if (m_stDialogBoxInfo[11].sV3 >= (50 - _iGetTotalItemNum()))
                    m_stDialogBoxInfo[11].sV3 = (50 - _iGetTotalItemNum());
            }

            if ((msX >= sX + 145) && (msX <= sX + 162) && (msY >= sY + 234) && (msY <= sY + 251))
            {

                m_stDialogBoxInfo[11].sV3 -= 10;
                if (m_stDialogBoxInfo[11].sV3 <= 1)
                    m_stDialogBoxInfo[11].sV3 = 1;
            }

            if ((msX >= sX + 163) && (msX <= sX + 180) && (msY >= sY + 209) && (msY <= sY + 230))
            {

                m_stDialogBoxInfo[11].sV3++;
                if (m_stDialogBoxInfo[11].sV3 >= (50 - _iGetTotalItemNum()))
                    m_stDialogBoxInfo[11].sV3 = (50 - _iGetTotalItemNum());
            }

            if ((msX >= sX + 163) && (msX <= sX + 180) && (msY >= sY + 234) && (msY <= sY + 251))
            {

                m_stDialogBoxInfo[11].sV3--;
                if (m_stDialogBoxInfo[11].sV3 <= 1)
                    m_stDialogBoxInfo[11].sV3 = 1;
            }

            if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                if ((50 - _iGetTotalItemNum()) < m_stDialogBoxInfo[11].sV3)
                {
                    AddEventList(DLGBOX_CLICK_SHOP1, 10);
                }
                else
                {
                    memset(cTemp, 0, sizeof(cTemp));
                    strcpy(cTemp, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cName);
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_PURCHASEITEM, 0, m_stDialogBoxInfo[11].sV3, 0, 0, cTemp);
                }
                m_stDialogBoxInfo[11].cMode = 0;
                m_stDialogBoxInfo[11].sV3 = 1;
                PlaySound('E', 14, 5);
            }

            if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                m_stDialogBoxInfo[11].cMode = 0;
                m_stDialogBoxInfo[11].sV3 = 1;
                PlaySound('E', 14, 5);
            }
            break;
    }
}

void CGame::DlgBoxClick_Skill(short msX, short msY)
{
    int i;
    short sX, sY;


    sX = m_stDialogBoxInfo[15].sX;
    sY = m_stDialogBoxInfo[15].sY;

    switch (m_stDialogBoxInfo[15].cMode)
    {
        case -1:
            break;

        case 0:
            for (i = 0; i < 17; i++)
                if ((i < DEF_MAXSKILLTYPE) && (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView] != 0))
                {

                    if ((msX >= sX + 44) && (msX <= sX + 135 + 44) && (msY >= sY + 45 + i * 15) && (msY <= sY + 59 + i * 15))
                    {
                        if ((m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_bIsUseable == true) &&
                            (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_iLevel != 0))
                        {
                            if (m_bSkillUsingStatus == true)
                            {
                                AddEventList(DLGBOX_CLICK_SKILL1, 10);
                                return;
                            }

                            if ((m_bCommandAvailable == false) || (m_iHP <= 0))
                            {
                                AddEventList(DLGBOX_CLICK_SKILL2, 10);
                                return;
                            }

                            if (m_bIsGetPointingMode == true)
                            {
                                return;
                            }
                            switch (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_cUseMethod)
                            {
                                case 0:
                                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USESKILL, 0, (i + m_stDialogBoxInfo[15].sView), 0, 0, 0);
                                    m_bSkillUsingStatus = true;
                                    DisableDialogBox(15);
                                    PlaySound('E', 14, 5);
                                    break;
                            }
                        }
                    }
                    else if ((msX >= sX + 215) && (msX <= sX + 240) && (msY >= sY + 45 + i * 15) && (msY <= sY + 59 + i * 15))
                    {

                        if (m_stDialogBoxInfo[15].bFlag == false)
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX, 0, i + m_stDialogBoxInfo[15].sView, 0, 0, 0);
                            PlaySound('E', 14, 5);
                            m_stDialogBoxInfo[15].bFlag = true;
                        }
                    }
                }
            break;
    }
}

void CGame::DlgBoxClick_SkillDlg(short msX, short msY)
{
    int i, iAdjX, iAdjY;
    short sX, sY;

    sX = m_stDialogBoxInfo[26].sX;
    sY = m_stDialogBoxInfo[26].sY;

    iAdjX = 5;
    iAdjY = 8;


    switch (m_stDialogBoxInfo[26].cMode)
    {
        case 1:
            if ((msX >= sX + iAdjX + 60) && (msX <= sX + iAdjX + 153) && (msY >= sY + iAdjY + 175) && (msY <= sY + iAdjY + 195))
            {
                m_stDialogBoxInfo[26].cMode = 2;
                m_stDialogBoxInfo[26].cStr[0] = 1;
                m_stDialogBoxInfo[26].dwT1 = m_dwCurTime;
                PlaySound('E', 14, 5);
                AddEventList(DLGBOX_CLICK_SKILLDLG1, 10);
                PlaySound('E', 41, 0);
            }
            break;

        case 3:
            for (i = 0; i < 13; i++)
                if (m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView] != 0)
                {
                    if ((msX >= sX + iAdjX + 44) && (msX <= sX + iAdjX + 135 + 44) && (msY >= sY + iAdjY + 55 + i * 15) && (msY <= sY + iAdjY + 55 + 14 + i * 15))
                    {
                        m_stDialogBoxInfo[26].cMode = 4;
                        m_stDialogBoxInfo[26].cStr[0] = i + m_stDialogBoxInfo[26].sView;
                        PlaySound('E', 14, 5);
                    }
                }
            break;

        case 4:
            iAdjX = -1;
            iAdjY = -7;
            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bBuildEnabled == true)
            {

                if ((msX >= sX + iAdjX + 32) && (msX <= sX + iAdjX + 95) && (msY >= sY + iAdjY + 353) && (msY <= sY + iAdjY + 372))
                {
                    // Back
                    m_stDialogBoxInfo[26].cMode = 3;
                    PlaySound('E', 14, 5);

                    if ((m_stDialogBoxInfo[26].sV1 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV1] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV1] = false;
                    if ((m_stDialogBoxInfo[26].sV2 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV2] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV2] = false;
                    if ((m_stDialogBoxInfo[26].sV3 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV3] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV3] = false;
                    if ((m_stDialogBoxInfo[26].sV4 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV4] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV4] = false;
                    if ((m_stDialogBoxInfo[26].sV5 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV5] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV5] = false;
                    if ((m_stDialogBoxInfo[26].sV6 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV6] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV6] = false;

                    m_stDialogBoxInfo[26].sV1 = -1;
                    m_stDialogBoxInfo[26].sV2 = -1;
                    m_stDialogBoxInfo[26].sV3 = -1;
                    m_stDialogBoxInfo[26].sV4 = -1;
                    m_stDialogBoxInfo[26].sV5 = -1;
                    m_stDialogBoxInfo[26].sV6 = -1;
                    m_stDialogBoxInfo[26].cStr[0] = 0;
                    m_stDialogBoxInfo[26].cStr[1] = 0;
                    m_stDialogBoxInfo[26].cStr[4] = 0;
                }

                if ((msX >= sX + iAdjX + 160) && (msX <= sX + iAdjX + 255) && (msY >= sY + iAdjY + 353) && (msY <= sY + iAdjY + 372))
                {
                    // Manufacture
                    if (m_stDialogBoxInfo[26].cStr[4] == 1)
                    {
                        m_stDialogBoxInfo[26].cMode = 5;
                        m_stDialogBoxInfo[26].cStr[1] = 0;
                        m_stDialogBoxInfo[26].dwT1 = m_dwCurTime;
                        PlaySound('E', 14, 5);
                        PlaySound('E', 44, 0);
                    }
                }
            }
            else
            {
                if ((msX >= sX + iAdjX + 32) && (msX <= sX + iAdjX + 95) && (msY >= sY + iAdjY + 353) && (msY <= sY + iAdjY + 372))
                {
                    // Back
                    m_stDialogBoxInfo[26].cMode = 3;
                    PlaySound('E', 14, 5);

                    if ((m_stDialogBoxInfo[26].sV1 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV1] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV1] = false;
                    if ((m_stDialogBoxInfo[26].sV2 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV2] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV2] = false;
                    if ((m_stDialogBoxInfo[26].sV3 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV3] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV3] = false;
                    if ((m_stDialogBoxInfo[26].sV4 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV4] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV4] = false;
                    if ((m_stDialogBoxInfo[26].sV5 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV5] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV5] = false;
                    if ((m_stDialogBoxInfo[26].sV6 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV6] != 0))
                        m_bIsItemDisabled[m_stDialogBoxInfo[26].sV6] = false;

                    m_stDialogBoxInfo[26].sV1 = -1;
                    m_stDialogBoxInfo[26].sV2 = -1;
                    m_stDialogBoxInfo[26].sV3 = -1;
                    m_stDialogBoxInfo[26].sV4 = -1;
                    m_stDialogBoxInfo[26].sV5 = -1;
                    m_stDialogBoxInfo[26].sV6 = -1;
                    m_stDialogBoxInfo[26].cStr[0] = 0;
                    m_stDialogBoxInfo[26].cStr[1] = 0;
                    m_stDialogBoxInfo[26].cStr[4] = 0;
                }
            }
            break;

        case 6:
            iAdjX = -1;
            iAdjY = -7;
            if ((msX >= sX + iAdjX + 32) && (msX <= sX + iAdjX + 95) && (msY >= sY + iAdjY + 353) && (msY <= sY + iAdjY + 372))
            {
                // Back
                m_stDialogBoxInfo[26].cMode = 3;
                PlaySound('E', 14, 5);

                if ((m_stDialogBoxInfo[26].sV1 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV1] != 0))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV1] = false;
                if ((m_stDialogBoxInfo[26].sV2 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV2] != 0))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV2] = false;
                if ((m_stDialogBoxInfo[26].sV3 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV3] != 0))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV3] = false;
                if ((m_stDialogBoxInfo[26].sV4 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV4] != 0))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV4] = false;
                if ((m_stDialogBoxInfo[26].sV5 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV5] != 0))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV5] = false;
                if ((m_stDialogBoxInfo[26].sV6 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV6] != 0))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV6] = false;

                m_stDialogBoxInfo[26].sV1 = -1;
                m_stDialogBoxInfo[26].sV2 = -1;
                m_stDialogBoxInfo[26].sV3 = -1;
                m_stDialogBoxInfo[26].sV4 = -1;
                m_stDialogBoxInfo[26].sV5 = -1;
                m_stDialogBoxInfo[26].sV6 = -1;
                m_stDialogBoxInfo[26].cStr[0] = 0;
                m_stDialogBoxInfo[26].cStr[1] = 0;
                m_stDialogBoxInfo[26].cStr[4] = 0;
            }
            break;
    }
}

void CGame::DlgBoxClick_Resurrect(short msX, short msY)
{
    short sX, sY;
    sX = m_stDialogBoxInfo[50].sX;
    sY = m_stDialogBoxInfo[50].sY;
    if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
    {   // yes
        bSendCommand(DEF_REQUEST_RESURRECTPLAYER_YES, 0, 0, 0, 0, 0, 0, 0);
        DisableDialogBox(50);
    }
    else if ((msX >= sX + 170) && (msX <= sX + 170 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
    {	// no
        bSendCommand(DEF_REQUEST_RESURRECTPLAYER_NO, 0, 0, 0, 0, 0, 0, 0);
        DisableDialogBox(50);
    }
}
void CGame::DlgBoxClick_CMDHallMenu(short msX, short msY)
{
    short sX, sY;
    sX = m_stDialogBoxInfo[51].sX;
    sY = m_stDialogBoxInfo[51].sY;
    switch (m_stDialogBoxInfo[51].cMode)
    {
        case 0:
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
            {
                m_stDialogBoxInfo[51].cMode = 1; // TP diag
                m_iTeleportMapCount = -1;
                bSendCommand(MSGID_REQUEST_HELDENIAN_TP_LIST, 0, 0, 0, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
            {
                m_stDialogBoxInfo[51].cMode = 2; // Soldier Diag
                PlaySound('E', 14, 5);
            }
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145)
                /*&& (m_iEnemyKillCount >= 3)*/)
            {
                m_stDialogBoxInfo[51].cMode = 3; // Flag Diag
                PlaySound('E', 14, 5);
            }
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145)
                /*&& (m_iEnemyKillCount >= 3)*/)
            {
                m_stDialogBoxInfo[51].cMode = 3; // Flag Diag
                PlaySound('E', 14, 5);
            }

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 145) && (msY < sY + 170)
                /*&& (m_iGizonItemUpgradeLeft >= 5)*/)
            {
                m_stDialogBoxInfo[51].cMode = 4; // Flag Diag
                PlaySound('E', 14, 5);
            }


        case 1: // TP now
            if (m_iTeleportMapCount > 0)
            {
                for (int i = 0; i < m_iTeleportMapCount; i++)
                {
                    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 130 + i * 15) && (msY <= sY + 144 + i * 15))
                    {
                        bSendCommand(MSGID_REQUEST_HELDENIAN_TP, 0, 0, m_stTeleportList[i].iIndex, 0, 0, 0);
                        DisableDialogBox(51);
                        return;
                    }
                }
            }
            break;

        case 2:
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY > sY + 70) && (msY < sY + 95)
                && (m_iConstructionPoint >= 2000) && (m_bIsCrusadeMode == false)) // Sorceress
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 875, 1, 2, 3, 4, "Gail", 5);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY > sY + 95) && (msY < sY + 120)
                && (m_iConstructionPoint >= 3000) && (m_bIsCrusadeMode == false)) // ATK
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 876, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY > sY + 120) && (msY < sY + 145)
                && (m_iConstructionPoint >= 1500) && (m_bIsCrusadeMode == false)) // Elf
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 877, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY > sY + 145) && (msY < sY + 170)
                && (m_iConstructionPoint >= 3000) && (m_bIsCrusadeMode == false)) // DSK
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 878, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY > sY + 170) && (msY < sY + 195)
                && (m_iConstructionPoint >= 4000) && (m_bIsCrusadeMode == false)) // HBT
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 879, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY > sY + 195) && (msY < sY + 220)
                && (m_iConstructionPoint >= 3000) && (m_bIsCrusadeMode == false)) // Barbarian
            {
                bSendCommand(MSGID_REQUEST_HELDENIAN_SCROLL, 880, 0, 0, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 3:
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 140) && (msY <= sY + 165)
                && (m_iEnemyKillCount >= 3))
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETOCCUPYFLAG, 0, 0, 0, 0, 0, 0);
                PlaySound('E', 14, 5);
            }
            break;

        case 4:
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 175) && (msY <= sY + 200)
                && (m_iGizonItemUpgradeLeft >= 5))
            {
                bSendCommand(DEF_REQUEST_ANGEL, 0, 0, 1, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 200) && (msY <= sY + 225)
                && (m_iGizonItemUpgradeLeft >= 5))
            {
                bSendCommand(DEF_REQUEST_ANGEL, 0, 0, 2, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 225) && (msY <= sY + 250)
                && (m_iGizonItemUpgradeLeft >= 5))
            {
                bSendCommand(DEF_REQUEST_ANGEL, 0, 0, 3, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 250) && (msY <= sY + 275)
                && (m_iGizonItemUpgradeLeft >= 5))
            {
                bSendCommand(DEF_REQUEST_ANGEL, 0, 0, 4, 0, 0, "Gail", 0);
                PlaySound('E', 14, 5);
            }

            break;
    }
}


void CGame::DlgBoxClick_ChangeStatsMajestic(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[42].sX;
    sY = m_stDialogBoxInfo[42].sY;

    if ((m_cStateChange1 != 0) || (m_cStateChange2 != 0) || (m_cStateChange3 != 0))
    {
        if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 127) && (msY <= sY + 133) && (m_cLU_Str < 0))
        {
            if (m_cStateChange1 == DEF_STR)
            {
                m_cStateChange1 = 0;
                m_cLU_Str += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange2 == DEF_STR)
            {
                m_cStateChange2 = 0;
                m_cLU_Str += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange3 == DEF_STR)
            {
                m_cStateChange3 = 0;
                m_cLU_Str += 1;
                m_iLU_Point -= 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 146) && (msY <= sY + 152) && (m_cLU_Vit < 0))
        {
            if (m_cStateChange1 == DEF_VIT)
            {
                m_cStateChange1 = 0;
                m_cLU_Vit += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange2 == DEF_VIT)
            {
                m_cStateChange2 = 0;
                m_cLU_Vit += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange3 == DEF_VIT)
            {
                m_cStateChange3 = 0;
                m_cLU_Vit += 1;
                m_iLU_Point -= 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 165) && (msY <= sY + 171) && (m_cLU_Dex < 0))
        {
            if (m_cStateChange1 == DEF_DEX)
            {
                m_cStateChange1 = 0;
                m_cLU_Dex += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange2 == DEF_DEX)
            {
                m_cStateChange2 = 0;
                m_cLU_Dex += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange3 == DEF_DEX)
            {
                m_cStateChange3 = 0;
                m_cLU_Dex += 1;
                m_iLU_Point -= 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 184) && (msY <= sY + 190) && (m_cLU_Int < 0))
        {
            if (m_cStateChange1 == DEF_INT)
            {
                m_cStateChange1 = 0;
                m_cLU_Int += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange2 == DEF_INT)
            {
                m_cStateChange2 = 0;
                m_cLU_Int += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange3 == DEF_INT)
            {
                m_cStateChange3 = 0;
                m_cLU_Int += 1;
                m_iLU_Point -= 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 203) && (msY <= sY + 209) && (m_cLU_Mag < 0))
        {
            if (m_cStateChange1 == DEF_MAG)
            {
                m_cStateChange1 = 0;
                m_cLU_Mag += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange2 == DEF_MAG)
            {
                m_cStateChange2 = 0;
                m_cLU_Mag += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange3 == DEF_MAG)
            {
                m_cStateChange3 = 0;
                m_cLU_Mag += 1;
                m_iLU_Point -= 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 222) && (msY <= sY + 228) && (m_cLU_Char < 0))
        {
            if (m_cStateChange1 == DEF_CHR)
            {
                m_cStateChange1 = 0;
                m_cLU_Char += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange2 == DEF_CHR)
            {
                m_cStateChange2 = 0;
                m_cLU_Char += 1;
                m_iLU_Point -= 1;
            }
            else if (m_cStateChange3 == DEF_CHR)
            {
                m_cStateChange3 = 0;
                m_cLU_Char += 1;
                m_iLU_Point -= 1;
            }
            PlaySound('E', 14, 5);
        }
    }

    if ((m_cStateChange1 == 0) || (m_cStateChange2 == 0) || (m_cStateChange3 == 0) && (m_iGizonItemUpgradeLeft > 0))
    {
        if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 127) && (msY <= sY + 133) && (m_iStr > 10))
        {
            if (m_cStateChange1 == 0)
            {
                m_cStateChange1 = DEF_STR;
                m_cLU_Str -= 1;
                m_iLU_Point += 1;
            }
            else if (m_cStateChange2 == 0)
            {
                m_cStateChange2 = DEF_STR;
                m_cLU_Str -= 1;
                m_iLU_Point += 1;
            }
            else
            {
                m_cStateChange3 = DEF_STR;
                m_cLU_Str -= 1;
                m_iLU_Point += 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 146) && (msY <= sY + 152) && (m_iVit > 10))
        {
            if (m_cStateChange1 == 0)
            {
                m_cStateChange1 = DEF_VIT;
                m_cLU_Vit -= 1;
                m_iLU_Point += 1;
            }
            else if (m_cStateChange2 == 0)
            {
                m_cStateChange2 = DEF_VIT;
                m_cLU_Vit -= 1;
                m_iLU_Point += 1;
            }
            else
            {
                m_cStateChange3 = DEF_VIT;
                m_cLU_Vit -= 1;
                m_iLU_Point += 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 165) && (msY <= sY + 171) && (m_iDex > 10))
        {
            if (m_cStateChange1 == 0)
            {
                m_cStateChange1 = DEF_DEX;
                m_cLU_Dex -= 1;
                m_iLU_Point += 1;
            }
            else if (m_cStateChange2 == 0)
            {
                m_cStateChange2 = DEF_DEX;
                m_cLU_Dex -= 1;
                m_iLU_Point += 1;
            }
            else
            {
                m_cStateChange3 = DEF_DEX;
                m_cLU_Dex -= 1;
                m_iLU_Point += 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 184) && (msY <= sY + 190) && (m_iInt > 10))
        {
            if (m_cStateChange1 == 0)
            {
                m_cStateChange1 = DEF_INT;
                m_cLU_Int -= 1;
                m_iLU_Point += 1;
            }
            else if (m_cStateChange2 == 0)
            {
                m_cStateChange2 = DEF_INT;
                m_cLU_Int -= 1;
                m_iLU_Point += 1;
            }
            else
            {
                m_cStateChange3 = DEF_INT;
                m_cLU_Int -= 1;
                m_iLU_Point += 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 203) && (msY <= sY + 209) && (m_iMag > 10))
        {
            if (m_cStateChange1 == 0)
            {
                m_cStateChange1 = DEF_MAG;
                m_cLU_Mag -= 1;
                m_iLU_Point += 1;
            }
            else if (m_cStateChange2 == 0)
            {
                m_cStateChange2 = DEF_MAG;
                m_cLU_Mag -= 1;
                m_iLU_Point += 1;
            }
            else
            {
                m_cStateChange3 = DEF_MAG;
                m_cLU_Mag -= 1;
                m_iLU_Point += 1;
            }
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 222) && (msY <= sY + 228) && (m_iCharisma > 10))
        {
            if (m_cStateChange1 == 0)
            {
                m_cStateChange1 = DEF_CHR;
                m_cLU_Char -= 1;
                m_iLU_Point += 1;
            }
            else if (m_cStateChange2 == 0)
            {
                m_cStateChange2 = DEF_CHR;
                m_cLU_Char -= 1;
                m_iLU_Point += 1;
            }
            else
            {
                m_cStateChange3 = DEF_CHR;
                m_cLU_Char -= 1;
                m_iLU_Point += 1;
            }
            PlaySound('E', 14, 5);
        }
    }
    else
    {
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            bSendCommand(MSGID_STATECHANGEPOINT, 0, 0, 0, 0, 0, 0);
            DisableDialogBox(42);
            PlaySound('E', 14, 5);
        }
    }
    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        DisableDialogBox(42);
        PlaySound('E', 14, 5);
    }
}
void CGame::DlgBoxClick_Slates(short msX, short msY)
{
    int iAdjX, iAdjY;
    short sX, sY;

    sX = m_stDialogBoxInfo[40].sX;
    sY = m_stDialogBoxInfo[40].sY;
    iAdjX = 5;
    iAdjY = 8;
    switch (m_stDialogBoxInfo[40].cMode)
    {
        case 1:
            if ((m_stDialogBoxInfo[40].sV1 != -1) && (m_stDialogBoxInfo[40].sV2 != -1) && (m_stDialogBoxInfo[40].sV3 != -1) && (m_stDialogBoxInfo[40].sV4 != -1))
            {
                if ((msX >= sX + 120) && (msX <= sX + 180) && (msY >= sY + 150) && (msY <= sY + 165))
                {
                    m_stDialogBoxInfo[40].cMode = 2;
                    PlaySound('E', 14, 5);
                }
            }
            break;
        default:break;
    }
}
void CGame::bItemDrop_Slates()
{
    char cItemID;
    if (m_cCommand < 0) return;
    cItemID = (char)m_stMCursor.sSelectedObjectID;
    if (m_pItemList[cItemID] == 0) return;
    if (m_bIsItemDisabled[cItemID] == true) return;
    if (m_bIsDialogEnabled[17] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    if ((m_bIsDialogEnabled[20] == true) &&
        ((m_stDialogBoxInfo[20].cMode == 1) || (m_stDialogBoxInfo[20].cMode == 2)))
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    if (m_bIsDialogEnabled[23] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    switch (m_stDialogBoxInfo[40].cMode)
    {
        case 1:
            if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX) && (m_pItemList[cItemID]->m_sSpriteFrame >= 151) && (m_pItemList[cItemID]->m_sSpriteFrame <= 154))
            {
                char cItemIDText[20]{};
                switch (m_pItemList[cItemID]->m_sSpriteFrame)
                {
                    case 151:
                        if (m_stDialogBoxInfo[40].sV1 == -1)
                        {
                            m_bIsItemDisabled[cItemID] = true;
                            m_stDialogBoxInfo[40].sV1 = cItemID;
                            //			format_to_local(cItemIDText, "Item ID : {}", cItemID);
                            AddEventList(cItemIDText, 10);
                        }
                        break;
                    case 152:
                        if (m_stDialogBoxInfo[40].sV2 == -1)
                        {
                            m_bIsItemDisabled[cItemID] = true;
                            m_stDialogBoxInfo[40].sV2 = cItemID;
                            //			format_to_local(cItemIDText, "Item ID : {}", cItemID);
                            AddEventList(cItemIDText, 10);
                        }
                        break;
                    case 153:
                        if (m_stDialogBoxInfo[40].sV3 == -1)
                        {
                            m_bIsItemDisabled[cItemID] = true;
                            m_stDialogBoxInfo[40].sV3 = cItemID;
                            //			format_to_local(cItemIDText, "Item ID : {}", cItemID);
                            AddEventList(cItemIDText, 10);
                        }
                        break;
                    case 154:
                        if (m_stDialogBoxInfo[40].sV4 == -1)
                        {
                            m_bIsItemDisabled[cItemID] = true;
                            m_stDialogBoxInfo[40].sV4 = cItemID;
                            //			format_to_local(cItemIDText, "Item ID : {}", cItemID);
                            AddEventList(cItemIDText, 10);
                        }
                        break;
                }
            }
            break;

        default:
            break;
    }
}
