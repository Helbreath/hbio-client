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


void CGame::DrawDialogBox_15AgeMsg(short msX, short msY)
{
    short sX, sY;

    if (m_iFeedBackCardIndex == -1) return;

    sX = m_stDialogBoxInfo[5].sX;
    sY = m_stDialogBoxInfo[5].sY;

    
#ifdef DEF_FEEDBACKCARD
    switch (m_iFeedBackCardIndex)
    {
        case 1:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_FEEDBACK1, sX, sY, 0);
            break;
        case 2:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_FEEDBACK2, sX, sY, 0);
            break;
        case 3:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_FEEDBACK3, sX, sY, 0);
            break;
        case 4:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_FEEDBACK4, sX, sY, 0);
            break;
        case 5:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_FEEDBACK5, sX, sY, 0);
            break;
        case 6:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_FEEDBACK6, sX, sY, 0);
            break;
        case 7:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_FEEDBACK7, sX, sY, 0);
            break;

    }

#else
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX, sY, 2);

    put_string(sX + 30, sY + 26, "2003³â 1¿ù 16ÀÏ ÀÌÈÄºÎÅÍ´Â '¿µ»ó¹° ½ÉÀÇ µî±Þ", Color(200, 200, 45));
    put_string(sX + 30, sY + 43, "À§¿øÈ¸'ÀÇ ½ÉÀÇ ÆÇÁ¤¿¡ µû¶ó ¸¸ 15¼¼ ¹Ì¸¸ÀÇ", Color(200, 200, 45));
    put_string(sX + 30, sY + 60, "ÇÃ·¹ÀÌ¾î´Â °ÔÀÓ¿¡ Á¢¼ÓÇÒ ¼ö ¾ø½À´Ï´Ù. ¹®ÀÇ»çÇ×", Color(200, 200, 45));
    put_string(sX + 30, sY + 78, "Àº Çïºê·¹½º È¨ÆäÀÌÁö(http://www.helbreath", Color(200, 200, 45));
    put_string(sX + 30, sY + 96, ".com) Á¢¼ÓÇÏ½Ã°Å³ª Çïºê·¹½º °í°´Áö¿øÆÀÀ¸·Î", Color(200, 200, 45));
    put_string(sX + 30, sY + 114, "¿¬¶ôÇØ ÁÖ½Ã±â ¹Ù¶ø´Ï´Ù.", Color(200, 200, 45));

    if ((msX >= sX + 122) && (msX <= sX + 125 + DEF_BTNSZX) && (msY >= sY + 127) && (msY <= sY + 127 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 122, sY + 127, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 122, sY + 127, 0);
#endif
}

void CGame::DrawDialogBox_WarningMsg(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[6].sX;
    sY = m_stDialogBoxInfo[6].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX, sY, 2);

    put_string2(sX + 63, sY + 35, DEF_MSG_WARNING1, 200, 200, 25);

#if DEF_LANGUAGE == 5	
    put_string(sX + 59, sY + 57, DEF_MSG_WARNING2, Color(220, 130, 45));
    put_string(sX + 59, sY + 74, DEF_MSG_WARNING3, Color(220, 130, 45));
    put_string(sX + 59, sY + 92, DEF_MSG_WARNING4, Color(220, 130, 45));
    put_string(sX + 59, sY + 110, DEF_MSG_WARNING5, Color(220, 130, 45));

#else
    put_string(sX + 30, sY + 57, DEF_MSG_WARNING2, Color(220, 130, 45));
    put_string(sX + 30, sY + 74, DEF_MSG_WARNING3, Color(220, 130, 45));
    put_string(sX + 30, sY + 92, DEF_MSG_WARNING4, Color(220, 130, 45));
    put_string(sX + 30, sY + 110, DEF_MSG_WARNING5, Color(220, 130, 45));
#endif

    if ((msX >= sX + 122) && (msX <= sX + 125 + DEF_BTNSZX) && (msY >= sY + 127) && (msY <= sY + 127 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 122, sY + 127, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 122, sY + 127, 0);

}

void CGame::DrawDialogBox_ItemDrop(short msX, short msY)
{
    short sX, sY;
    char cTxt[120]{}, cStr1[64]{}, cStr2[64]{}, cStr3[64]{};

    sX = m_stDialogBoxInfo[4].sX;
    sY = m_stDialogBoxInfo[4].sY;

    //DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 5);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    GetItemName(m_pItemList[m_stDialogBoxInfo[4].sView]->m_cName, m_pItemList[m_stDialogBoxInfo[4].sView]->m_dwAttribute, cStr1, cStr2, cStr3, 64);

    if (strlen(m_stDialogBoxInfo[4].cStr) == 0)
        format_to_local(cTxt, "{}", cStr1);

    
    if (m_bIsSpecial) 
    {
        put_string(sX + 35, sY + 20, cTxt, Color(0, 255, 50));
        put_string(sX + 36, sY + 20, cTxt, Color(0, 255, 50));
    }

    else 
    {
        put_string(sX + 35, sY + 20, cTxt, Color(4, 0, 50));
        put_string(sX + 36, sY + 20, cTxt, Color(4, 0, 50));
    }

    put_string(sX + 35, sY + 36, DRAW_DIALOGBOX_ITEM_DROP1, Color(4, 0, 50));
    put_string(sX + 36, sY + 36, DRAW_DIALOGBOX_ITEM_DROP1, Color(4, 0, 50));
    

    if (m_bItemDrop)
    {
        if ((msX >= sX + 35) && (msX <= sX + 240) && (msY >= sY + 80) && (msY <= sY + 90))
        {
            put_string(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP2, Color(255, 255, 255));
            put_string(sX + 36, sY + 80, DRAW_DIALOGBOX_ITEM_DROP2, Color(255, 255, 255));
            
        }
        else
        {
            put_string(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP2, Color(4, 0, 50));
            put_string(sX + 36, sY + 80, DRAW_DIALOGBOX_ITEM_DROP2, Color(4, 0, 50));
            
        }
    }
    else
    {
        if ((msX >= sX + 35) && (msX <= sX + 240) && (msY >= sY + 80) && (msY <= sY + 90))
        {
            put_string(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP3, Color(255, 255, 255));
            put_string(sX + 36, sY + 80, DRAW_DIALOGBOX_ITEM_DROP3, Color(255, 255, 255));
            
        }
        else
        {
            put_string(sX + 35, sY + 80, DRAW_DIALOGBOX_ITEM_DROP3, Color(4, 0, 50));
            put_string(sX + 36, sY + 80, DRAW_DIALOGBOX_ITEM_DROP3, Color(4, 0, 50));
            
        }
    }

    if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 19);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 18);

    if ((msX >= sX + 170) && (msX <= sX + 170 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 3);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 2);
}

void CGame::DrawDialogBox_NpcTalk(short msX, short msY, char cLB)
{
    short sX, sY;
    int i, iTotalLines, iPointerLoc;
    double d1, d2, d3;


    sX = m_stDialogBoxInfo[21].sX;
    sY = m_stDialogBoxInfo[21].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);

    switch (m_stDialogBoxInfo[21].cMode)
    {
        case 0: 
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 1: // Accept / Decline
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 33);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 32);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 41);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 40);
            break;

        case 2: // Next
            if ((msX >= sX + 190) && (msX <= sX + 278) && (msY >= sY + 296) && (msY <= sY + 316))
                put_string_sprite_font(sX + 190, sY + 270, "Next", 6, 6, 20);
            else put_string_sprite_font(sX + 190, sY + 270, "Next", 0, 0, 7);
            break;
    }

    for (i = 0; i < 17; i++)
        if ((i < DEF_TEXTDLGMAXLINES) && (m_pMsgTextList2[i + m_stDialogBoxInfo[21].sView] != NULL))
        {
            put_aligned_string(sX, sX + m_stDialogBoxInfo[21].sSizeX, sY + 57 + i * 15, m_pMsgTextList2[i + m_stDialogBoxInfo[21].sView]->message, 45, 25, 25);
        }

    iTotalLines = 0;
    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
        if (m_pMsgTextList2[i] != NULL) iTotalLines++;

    
    if (iTotalLines > 17)
    {
        d1 = (double)m_stDialogBoxInfo[21].sView;
        d2 = (double)(iTotalLines - 17);
        d3 = (274.0f * d1) / d2;
        iPointerLoc = (int)d3;
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
    }
    else iPointerLoc = 0;

    
    if (cLB != 0 && iTotalLines > 17)
    {
        if ((iGetTopDialogBoxIndex() == 21))
        {
            if ((msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320))
            {
                d1 = (double)(msY - (sY + 40));
                d2 = (double)(iTotalLines - 17);
                d3 = (d1 * d2) / 274.0f;
                iPointerLoc = (int)d3;

                if (iPointerLoc > iTotalLines) iPointerLoc = iTotalLines;
                m_stDialogBoxInfo[21].sView = iPointerLoc;
            }
        }
    }
    else m_stDialogBoxInfo[21].bIsScrollSelected = FALSE;
}

void CGame::DrawDialogBox_Chat(short msX, short msY, short msZ, char cLB)
{
    short sX, sY;
    int i, iPointerLoc;
    double d1, d2, d3;

    sX = m_stDialogBoxInfo[10].sX;
    sY = m_stDialogBoxInfo[10].sY;
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 4, FALSE, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 22, FALSE, m_bDialogTrans);

    if (msZ != 0 && (iGetTopDialogBoxIndex() == 10))
    {
        m_stDialogBoxInfo[10].sView = m_stDialogBoxInfo[10].sView + msZ / 30;
        m_stMCursor.sZ = 0;
    }
    if (m_stDialogBoxInfo[10].sView < 0) m_stDialogBoxInfo[10].sView = 0;
    if (m_stDialogBoxInfo[10].sView > DEF_MAXCHATSCROLLMSGS - 8) m_stDialogBoxInfo[10].sView = DEF_MAXCHATSCROLLMSGS - 8;

    d1 = (double)m_stDialogBoxInfo[10].sView;
    d2 = (double)(105);
    d3 = (d1 * d2) / (DEF_MAXCHATSCROLLMSGS - 8);
    iPointerLoc = (int)d3;
    iPointerLoc = 105 - iPointerLoc;
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 346, sY + 33 + iPointerLoc, 7);

    for (i = 0; i < 8; i++)
        if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] != NULL)
        {
            switch (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_dwTime)
            {
                case 0:  put_chat_window_string(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, Color(230, 230, 230)); break;
                case 1:  put_chat_window_string(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, Color(130, 200, 130)); break;
                case 2:  put_chat_window_string(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, Color(255, 130, 130)); break;
                case 3:  put_chat_window_string(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, Color(130, 130, 255)); break;
                case 4:  put_chat_window_string(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, Color(230, 230, 130)); break;
                case 10: put_chat_window_string(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, Color(180, 255, 180)); break;
                case 20: put_chat_window_string(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, Color(150, 150, 170)); break;
            }
        }

    if ((cLB != 0) && (iGetTopDialogBoxIndex() == 10))
    {
        
        if ((msX >= sX + 336) && (msX <= sX + 361) && (msY >= sY + 28) && (msY <= sY + 140))
        {
            d1 = (double)(msY - (sY + 28));
            d2 = ((DEF_MAXCHATSCROLLMSGS - 8) * d1) / 105.0f;
            m_stDialogBoxInfo[10].sView = DEF_MAXCHATSCROLLMSGS - 8 - (int)d2;
        }

        if ((msX >= sX + 336) && (msX <= sX + 361) && (msY > sY + 18) && (msY < sY + 28))
            m_stDialogBoxInfo[10].sView = DEF_MAXCHATSCROLLMSGS - 8;

        if ((msX >= sX + 336) && (msX <= sX + 361) && (msY > sY + 140) && (msY < sY + 163))
            m_stDialogBoxInfo[10].sView = 0;
    }
    else m_stDialogBoxInfo[10].bIsScrollSelected = FALSE;
}

void CGame::DrawDialogBox_Inventory(int msX, int msY)
{
    int i;
    short sX, sY;
    DWORD dwTime = m_dwCurTime;
    char cItemColor;

    sX = m_stDialogBoxInfo[2].sX;
    sY = m_stDialogBoxInfo[2].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_INVENTORY, sX, sY, 0);

    for (i = 0; i < DEF_MAXITEMS; i++)
        if ((m_cItemOrder[i] != -1) && (m_pItemList[m_cItemOrder[i]] != NULL))
        {

            if (((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_ITEM) &&
                (m_stMCursor.sSelectedObjectID == m_cItemOrder[i])) ||
                (m_bIsItemEquipped[m_cItemOrder[i]] == TRUE))
            {
            }
            else
            {
                cItemColor = m_pItemList[m_cItemOrder[i]]->m_cItemColor;
                if (m_bIsItemDisabled[m_cItemOrder[i]] == TRUE)
                {
                    if (cItemColor == 0)
                        m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->put_trans_sprite2(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                            sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame, dwTime);
                    else
                    {

                        if ((m_pItemList[m_cItemOrder[i]]->m_cEquipPos == DEF_EQUIPPOS_LHAND) ||
                            (m_pItemList[m_cItemOrder[i]]->m_cEquipPos == DEF_EQUIPPOS_RHAND) ||
                            (m_pItemList[m_cItemOrder[i]]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
                        {
                            
                            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->put_trans_sprite_color(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame,
                                m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0],
                                dwTime);
                        }
                        else
                        {
                            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->put_trans_sprite_color(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame,
                                m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0],
                                dwTime);
                        }
                    }
                }
                else
                {
                    if (cItemColor == 0)
                        m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->put_sprite_fast(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                            sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame, dwTime);
                    else
                    {
                        if ((m_pItemList[m_cItemOrder[i]]->m_cEquipPos == DEF_EQUIPPOS_LHAND) ||
                            (m_pItemList[m_cItemOrder[i]]->m_cEquipPos == DEF_EQUIPPOS_RHAND) ||
                            (m_pItemList[m_cItemOrder[i]]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
                        {
                            
                            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->put_sprite_color(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame,
                                m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0],
                                dwTime);
                        }
                        else
                        {
                            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_cItemOrder[i]]->m_sSprite]->put_sprite_color(sX + 32 + m_pItemList[m_cItemOrder[i]]->m_sX,
                                sY + 44 + m_pItemList[m_cItemOrder[i]]->m_sY, m_pItemList[m_cItemOrder[i]]->m_sSpriteFrame,
                                m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0],
                                dwTime);
                        }
                    }
                }

                
                if ((m_pItemList[m_cItemOrder[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                    (m_pItemList[m_cItemOrder[i]]->m_cItemType == DEF_ITEMTYPE_ARROW))
                {

#ifdef _DEBUG
                    DisplayGold((int)m_pItemList[m_cItemOrder[i]]->m_dwCount);
#else
                    format_to_local(G_cTxt, "{}", (int)m_pItemList[m_cItemOrder[i]]->m_dwCount);
#endif

                    put_string_sprite_number(sX + 29 + m_pItemList[m_cItemOrder[i]]->m_sX + 10, sY + 41 + m_pItemList[m_cItemOrder[i]]->m_sY + 10, G_cTxt, 200, 200, 200);
                }
            }
        }

    if ((msX >= sX + 23) && (msX <= sX + 76) && (msY >= sY + 172) && (msY <= sY + 184))
    {
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_INVENTORY, sX + 23, sY + 172, 1);
    }

    if ((msX >= sX + 140) && (msX <= sX + 212) && (msY >= sY + 172) && (msY <= sY + 184))
    {
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_INVENTORY, sX + 140, sY + 172, 2);
    }
}

void CGame::DrawDialogBox_Map()
{
    short sX, sY;
    DWORD dwTime = m_dwCurTime;
    double dV1, dV2, dV3;
    int    tX, tY, szX, szY, dX, dY;

    sX = m_stDialogBoxInfo[22].sX;
    sY = m_stDialogBoxInfo[22].sY;

    szX = 0;
    szY = 0;

    switch (m_stDialogBoxInfo[22].sV1)
    {
        case 1:
            switch (m_stDialogBoxInfo[22].sV2)
            {
                case 0: // aresden
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS1]->put_trans_sprite2(sX, sY, 0, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS1]->put_sprite_fast(sX, sY, 0, dwTime);
                    dX = 19;
                    dY = 20;
                    szX = 260;
                    szY = 260;
                    break;

                case 1: // elvine
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS1]->put_trans_sprite2(sX, sY, 1, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS1]->put_sprite_fast(sX, sY, 1, dwTime);
                    dX = 20;
                    dY = 18;
                    szX = 260;
                    szY = 260;
                    break;

                case 2: // middleland
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS2]->put_trans_sprite2(sX, sY, 0, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS2]->put_sprite_fast(sX, sY, 0, dwTime);
                    dX = 11;
                    dY = 31;
                    szX = 280;
                    szY = 253;
                    break;

                case 3: // default
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS2]->put_trans_sprite2(sX, sY, 1, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS2]->put_sprite_fast(sX, sY, 1, dwTime);
                    dX = 52;
                    dY = 42;
                    szX = 200;
                    szY = 200;
                    break;

                case 4: 
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS3]->put_trans_sprite2(sX, sY, 0, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS3]->put_sprite_fast(sX, sY, 0, dwTime);
                    dX = 40;
                    dY = 40;
                    szX = 220;
                    szY = 220;
                    break;

                case 5: 
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS3]->put_trans_sprite2(sX, sY, 1, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS3]->put_sprite_fast(sX, sY, 1, dwTime);
                    dX = 40;
                    dY = 40;
                    szX = 220;
                    szY = 220;
                    break;

                case 6: 
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS4]->put_trans_sprite2(sX, sY, 0, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS4]->put_sprite_fast(sX, sY, 0, dwTime);
                    dX = 40;
                    dY = 40;
                    szX = 220;
                    szY = 220;
                    break;

                case 7: 
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS4]->put_trans_sprite2(sX, sY, 1, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS4]->put_sprite_fast(sX, sY, 1, dwTime);
                    dX = 40;
                    dY = 40;
                    szX = 220;
                    szY = 220;
                    break;
                case 8: 
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS5]->put_trans_sprite2(sX, sY, 0, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS5]->put_sprite_fast(sX, sY, 0, dwTime);
                    dX = 40;
                    dY = 32;
                    szX = 220;
                    szY = 220;
                    break;

                case 9: 
                    if (m_bDialogTrans)
                        m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS5]->put_trans_sprite2(sX, sY, 1, dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_NEWMAPS5]->put_sprite_fast(sX, sY, 1, dwTime);
                    dX = 40;
                    dY = 38;
                    szX = 220;
                    szY = 220;
                    break;

            }

            
            dV1 = (double)m_pMapData->m_sMapSizeX;
            dV2 = (double)m_sPlayerX;
            dV3 = (dV2 * (double)szX) / dV1;
            tX = (int)dV3 + dX;

            dV1 = (double)m_pMapData->m_sMapSizeY;
            if (dV1 == 752) dV1 = 680;
            dV2 = (double)m_sPlayerY;
            dV3 = (dV2 * (double)szY) / dV1;
            tY = (int)dV3 + dY;


            //m_pSprite[DEF_SPRID_INTERFACE_MAPS1]->PutSpriteFast(sX +tX, sY +tY, 4, dwTime);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX + tX, sY + tY, 43);
            format_to_local(G_cTxt, "{},{}", m_sPlayerX, m_sPlayerY);
            put_string_sprite_font3(sX + 10 + tX - 5, sY + 10 + tY - 6, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, FALSE, 2);
            break;
    }
}

/*
void CGame::DrawDialogBox_Bank(short msX, short msY, short msZ, char cLB)
{
 short sX, sY, szX;
 int  i, iTotalLines, iPointerLoc;
 double d1, d2, d3;
 char cItemColor;

    sX = m_stDialogBoxInfo[14].sX;
    sY = m_stDialogBoxInfo[14].sY;
    szX = m_stDialogBoxInfo[14].sSizeX - 5;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 21);
    char cStr1[64], cStr2[64], cStr3[64];
    ZeroMemory( cStr1, sizeof(cStr1) );
    ZeroMemory( cStr2, sizeof(cStr2) );
    ZeroMemory( cStr3, sizeof(cStr3) );

    switch (m_stDialogBoxInfo[14].cMode) {
    case -1:
        PutString(sX + 30 + 15, sY + 70, DRAW_DIALOGBOX_BANK1, Color(0,0,0));
        PutString(sX + 30 + 15, sY + 85, DRAW_DIALOGBOX_BANK2, Color(0,0,0));
        break;

    case 0:
        for (i = 0; i < m_stDialogBoxInfo[14].sV1; i++)
        if ((m_pBankList[i + m_stDialogBoxInfo[14].sView] != NULL) && ((i + m_stDialogBoxInfo[14].sView) < DEF_MAXBANKITEMS)) {
            GetItemName(m_pBankList[i + m_stDialogBoxInfo[14].sView], cStr1, cStr2, cStr3);
            if ((msX > sX + 30) && (msX < sX + 210) && (msY >= sY + 50 + i*15) && (msY <= sY + 64 + i*15))
            {
                m_stMCursor.sCursorFrame = -1;
                if( (strlen(cStr2)==0) && (strlen(cStr3)==0) ) PutAlignedString(sX + 24, sX +240, sY + 50 + i*15, cStr1, 255,255,255);
                else
                {
                    ZeroMemory( G_cTxt, sizeof(G_cTxt) );
                    if( (strlen(cStr1)+strlen(cStr2)+strlen(cStr3)) < 36 )
                    {
                        if( (strlen(cStr2)>0) && (strlen(cStr3)>0) ) format_to_local( G_cTxt, "{}({}, {})", cStr1, cStr2, cStr3 );
                        else format_to_local( G_cTxt, "{}({}{})", cStr1, cStr2, cStr3 );
                        PutAlignedString(sX, sX + szX, sY + 50 + i*15, G_cTxt, 255,255,255);
                    }
                    else
                    {
                        if( (strlen(cStr2)>0) && (strlen(cStr3)>0) ) format_to_local( G_cTxt, "({}, {})", cStr2, cStr3 );
                        else format_to_local( G_cTxt, "({}{})", cStr2, cStr3 );
                        PutAlignedString(sX, sX + szX, sY + 50 + i*15, cStr1, 255,255,255);
                        PutAlignedString(sX, sX + szX, sY + 50 + i*15 + 15, G_cTxt, 200,200,200);
                        i++;
                    }
                }
                cItemColor = m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cItemColor;
                if( cItemColor == 0 )
                {
                     m_pSprite[DEF_SPRID_ITEMGROUND_PIVOTPOINT + m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSprite]->PutSpriteFast(msX, msY, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSpriteFrame, m_dwCurTime);
                }
                else
                {
                    if ((m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos == DEF_EQUIPPOS_LHAND) ||
                        (m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos == DEF_EQUIPPOS_RHAND) ||
                        (m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND)) {
                        
                        m_pSprite[DEF_SPRID_ITEMGROUND_PIVOTPOINT + m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSprite]->PutSpriteColor(msX, msY, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSpriteFrame,
                                                                            m_wWR[cItemColor] -m_wR[0], m_wWG[cItemColor] -m_wG[0], m_wWB[cItemColor] -m_wB[0], m_dwCurTime);
                    }
                    else m_pSprite[DEF_SPRID_ITEMGROUND_PIVOTPOINT + m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSprite]->PutSpriteColor(msX, msY, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSpriteFrame,
                                                                            m_wR[cItemColor] -m_wR[0], m_wG[cItemColor] -m_wG[0], m_wB[cItemColor] -m_wB[0], m_dwCurTime);
                }
            }
            else
            {
                if( (strlen(cStr2)==0) && (strlen(cStr3)==0) ) PutAlignedString(sX + 24, sX +240, sY + 50 + i*15, cStr1);
                else
                {
                    ZeroMemory( G_cTxt, sizeof(G_cTxt) );
                    if( (strlen(cStr1)+strlen(cStr2)+strlen(cStr3)) < 36 )
                    {
                        if( (strlen(cStr2)>0) && (strlen(cStr3)>0) ) format_to_local( G_cTxt, "{}({}, {})", cStr1, cStr2, cStr3 );
                        else format_to_local( G_cTxt, "{}({}{})", cStr1, cStr2, cStr3 );
                        PutAlignedString(sX, sX + szX, sY + 50 + i*15, G_cTxt);
                    }
                    else PutAlignedString(sX, sX + szX, sY + 50 + i*15, cStr1);
                }
            }
        }

        iTotalLines = 0;
        for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pBankList[i] != NULL) iTotalLines++;

        
        if (iTotalLines > m_stDialogBoxInfo[14].sV1) {
            d1 = (double)m_stDialogBoxInfo[14].sView;
            d2 = (double)(iTotalLines-m_stDialogBoxInfo[14].sV1);
            d3 = (274.0f * d1)/d2;
            iPointerLoc = (int)d3;
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX+242, sY + iPointerLoc + 35, 7);
        }
        else iPointerLoc = 0;

        
        if (cLB != 0 && (iGetTopDialogBoxIndex() == 14) && iTotalLines > m_stDialogBoxInfo[14].sV1) {
            if ((msX >= sX + 230) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320)) {
                d1 = (double)(msY -(sY+35));
                d2 = (double)(iTotalLines-m_stDialogBoxInfo[14].sV1);
                d3 = (d1 * d2)/274.0f;
                m_stDialogBoxInfo[14].sView = (int)(d3+0.5);
            }
            else if ((msX >= sX + 230) && (msX <= sX + 260) && (msY > sY + 10) && (msY < sY + 40)) m_stDialogBoxInfo[14].sView = 0;
        }
        else m_stDialogBoxInfo[14].bIsScrollSelected = FALSE;
        if( iGetTopDialogBoxIndex() == 14 && msZ != 0 )
        {
            if( iTotalLines > 50 ) m_stDialogBoxInfo[14].sView = m_stDialogBoxInfo[14].sView - msZ/30;
            else {
                if( msZ > 0 ) m_stDialogBoxInfo[14].sView--;
                if( msZ < 0 ) m_stDialogBoxInfo[14].sView++;
            }
            m_stMCursor.sZ = 0;
        }
        if( iTotalLines > m_stDialogBoxInfo[14].sV1 && m_stDialogBoxInfo[14].sView > iTotalLines-m_stDialogBoxInfo[14].sV1 ) m_stDialogBoxInfo[14].sView = iTotalLines-m_stDialogBoxInfo[14].sV1;
        if( iTotalLines <= m_stDialogBoxInfo[14].sV1 ) m_stDialogBoxInfo[14].sView = 0;
        if( m_stDialogBoxInfo[14].sView < 0 ) m_stDialogBoxInfo[14].sView = 0;
        PutAlignedString(sX, sX+szX, sY + 265, DRAW_DIALOGBOX_BANK3);
        PutAlignedString(sX, sX+szX, sY + 280, DRAW_DIALOGBOX_BANK4);
        PutAlignedString(sX, sX+szX, sY + 295, DRAW_DIALOGBOX_BANK5);
        break;
    }
}
*/

void CGame::DrawDialogBox_Bank(short msX, short msY, short msZ, char cLB)
{
    short sX, sY, szX;
    int  i, iTotalLines, iPointerLoc, iLoc;
    double d1, d2, d3;
    char cItemColor, cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
    BOOL bFlag = FALSE;

    sX = m_stDialogBoxInfo[14].sX;
    sY = m_stDialogBoxInfo[14].sY;
    szX = m_stDialogBoxInfo[14].sSizeX - 5;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 21);
    ZeroMemory(cStr1, sizeof(cStr1));
    ZeroMemory(cStr2, sizeof(cStr2));
    ZeroMemory(cStr3, sizeof(cStr3));
    iLoc = 45;

    switch (m_stDialogBoxInfo[14].cMode)
    {
        case -1:
            put_string(sX + 30 + 15, sY + 70, DRAW_DIALOGBOX_BANK1, Color(0, 0, 0));
            put_string(sX + 30 + 15, sY + 85, DRAW_DIALOGBOX_BANK2, Color(0, 0, 0));
            break;

        case 0:
            for (i = 0; i < m_stDialogBoxInfo[14].sV1; i++)
                if ((m_pBankList[i + m_stDialogBoxInfo[14].sView] != NULL) && ((i + m_stDialogBoxInfo[14].sView) < DEF_MAXBANKITEMS))
                {
                    GetItemName(m_pBankList[i + m_stDialogBoxInfo[14].sView], cStr1, cStr2, cStr3, 64);
                    if ((msX > sX + 30) && (msX < sX + 210) && (msY >= sY + 110 + i * 15) && (msY <= sY + 124 + i * 15))
                    {
                        bFlag = TRUE;

                        put_aligned_string(sX, sX + szX, sY + 110 + i * 15, cStr1, 255, 255, 255);
                        if (m_bIsSpecial)
                            put_aligned_string(sX + 70, sX + szX, sY + iLoc, cStr1, 0, 255, 50);
                        else
                            put_aligned_string(sX + 70, sX + szX, sY + iLoc, cStr1, 255, 255, 255);

                        if (strlen(cStr2) > 0)
                        {
                            iLoc += 15;
                            put_aligned_string(sX + 70, sX + szX, sY + iLoc, cStr2, 150, 150, 150);
                        }
                        if (strlen(cStr3) > 0)
                        {
                            iLoc += 15;
                            put_aligned_string(sX + 70, sX + szX, sY + iLoc, cStr3, 150, 150, 150);
                        }
                        if (m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sLevelLimit != 0 && m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_dwAttribute & 0x00000001)
                        {
                            iLoc += 15;
                            format_to_local(G_cTxt, "{}: {}", DRAW_DIALOGBOX_SHOP24, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sLevelLimit);
                            put_aligned_string(sX + 70, sX + szX, sY + iLoc, G_cTxt, 150, 150, 150);
                        }

#if DEF_LANGUAGE == 3
                        if (!strcmp(m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cName, "½ºÅæ¿Àºê»õÅ©¸®ÆÄÀÌ½º"))
                        {
                            iLoc += 15;
                            format_to_local(G_cTxt, "³²Àº ¼ö¸í: {}", m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_wCurLifeSpan);
                            put_aligned_string(sX + 70, sX + szX, sY + iLoc, G_cTxt, 150, 150, 150);
                        }
#endif

                        if ((m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos != DEF_EQUIPPOS_NONE) && (m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_wWeight >= 1100))
                        {
                            iLoc += 15;
                            
                            
                            int		_wWeight = 0;
                            if (m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_wWeight % 100)				_wWeight = 1;
                            format_to_local(G_cTxt, DRAW_DIALOGBOX_SHOP15, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_wWeight / 100 + _wWeight);

                            put_aligned_string(sX + 70, sX + szX, sY + iLoc, G_cTxt, 150, 150, 150);
                        }

                        cItemColor = m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cItemColor;
                        if (cItemColor == 0)
                        {
                            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSprite]->put_sprite_fast(sX + 60, sY + 68, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSpriteFrame, m_dwCurTime);
                        }
                        else
                        {
                            if ((m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos == DEF_EQUIPPOS_LHAND) ||
                                (m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos == DEF_EQUIPPOS_RHAND) ||
                                (m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
                            {
                                
                                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSprite]->put_sprite_color(sX + 60, sY + 68, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSpriteFrame,
                                    m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
                            }
                            else m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSprite]->put_sprite_color(sX + 60, sY + 68, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSpriteFrame,
                                m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
                        }
                    }
                    else
                    {
                        if (m_bIsSpecial)
                            put_aligned_string(sX, sX + szX, sY + 110 + i * 15, cStr1, 0, 255, 50);
                        else
                            put_aligned_string(sX, sX + szX, sY + 110 + i * 15, cStr1, 0, 0, 0);
                    }
                }

            iTotalLines = 0;
            for (i = 0; i < DEF_MAXBANKITEMS; i++)
                if (m_pBankList[i] != NULL) iTotalLines++;

            
            if (iTotalLines > m_stDialogBoxInfo[14].sV1)
            {
                d1 = (double)m_stDialogBoxInfo[14].sView;
                d2 = (double)(iTotalLines - m_stDialogBoxInfo[14].sV1);
                d3 = (274.0f * d1) / d2;
                iPointerLoc = (int)d3;
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242, sY + iPointerLoc + 35, 7);
            }
            else iPointerLoc = 0;

            
            if (cLB != 0 && (iGetTopDialogBoxIndex() == 14) && iTotalLines > m_stDialogBoxInfo[14].sV1)
            {
                if ((msX >= sX + 230) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320))
                {
                    d1 = (double)(msY - (sY + 35));
                    d2 = (double)(iTotalLines - m_stDialogBoxInfo[14].sV1);
                    d3 = (d1 * d2) / 274.0f;
                    m_stDialogBoxInfo[14].sView = (int)(d3 + 0.5);
                }
                else if ((msX >= sX + 230) && (msX <= sX + 260) && (msY > sY + 10) && (msY < sY + 40)) m_stDialogBoxInfo[14].sView = 0;
            }
            else m_stDialogBoxInfo[14].bIsScrollSelected = FALSE;
            if (iGetTopDialogBoxIndex() == 14 && msZ != 0)
            {
                if (iTotalLines > 50) m_stDialogBoxInfo[14].sView = m_stDialogBoxInfo[14].sView - msZ / 30;
                else
                {
                    if (msZ > 0) m_stDialogBoxInfo[14].sView--;
                    if (msZ < 0) m_stDialogBoxInfo[14].sView++;
                }
                m_stMCursor.sZ = 0;
            }
            if (iTotalLines > m_stDialogBoxInfo[14].sV1 && m_stDialogBoxInfo[14].sView > iTotalLines - m_stDialogBoxInfo[14].sV1) m_stDialogBoxInfo[14].sView = iTotalLines - m_stDialogBoxInfo[14].sV1;
            if (iTotalLines <= m_stDialogBoxInfo[14].sV1) m_stDialogBoxInfo[14].sView = 0;
            if (m_stDialogBoxInfo[14].sView < 0) m_stDialogBoxInfo[14].sView = 0;
            if (bFlag == FALSE)
            {
                put_aligned_string(sX, sX + szX, sY + 45, DRAW_DIALOGBOX_BANK3);
                put_aligned_string(sX, sX + szX, sY + 60, DRAW_DIALOGBOX_BANK4);
                put_aligned_string(sX, sX + szX, sY + 75, DRAW_DIALOGBOX_BANK5);
            }
            break;
    }
}

void CGame::DrawDialogBox_FeedBackCard(short msX, short msY)
{

}

void CGame::DrawDialogBox_Character(short msX, short msY)
{
    short sX, sY, sSprH, sFrame;
    int i, iR, iG, iB, iSkirtDraw = 0;
    char cTxt2[120], cEquipPosStatus[DEF_MAXITEMEQUIPPOS]{};
    char  cItemColor, cCollison;

    sX = m_stDialogBoxInfo[1].sX;
    sY = m_stDialogBoxInfo[1].sY;
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 0, FALSE, m_bDialogTrans);

    ZeroMemory(G_cTxt, sizeof(G_cTxt));
    strcpy(G_cTxt, m_cPlayerName);
    strcat(G_cTxt, " : ");

    if (m_iPKCount > 0)
    {
        ZeroMemory(cTxt2, sizeof(cTxt2));
        format_to_local(cTxt2, DRAW_DIALOGBOX_CHARACTER1, m_iPKCount);
        strcat(G_cTxt, cTxt2);
    }

    ZeroMemory(cTxt2, sizeof(cTxt2));
    format_to_local(cTxt2, DRAW_DIALOGBOX_CHARACTER2, m_iContribution);
    strcat(G_cTxt, cTxt2);

    put_aligned_string(sX + 24, sX + 252, sY + 52, G_cTxt, 45, 20, 20);

    ZeroMemory(G_cTxt, sizeof(G_cTxt));

    if (m_bCitizen == FALSE) strcpy(G_cTxt, DRAW_DIALOGBOX_CHARACTER7);
    else
    {

        //#if DEF_LANGUAGE > 2		// Korea 2.19
        if (m_bHunter)
        {
            if (m_bAresden)
                strcat(G_cTxt, DEF_MSG_ARECIVIL);
            else strcat(G_cTxt, DEF_MSG_ELVCIVIL);
        }
        else
        {
            if (m_bAresden)
                strcat(G_cTxt, DEF_MSG_ARESOLDIER);
            else strcat(G_cTxt, DEF_MSG_ELVSOLDIER);
        }
        //#else
        //		if (m_bAresden)
        
        
        //#endif

        if (m_iGuildRank >= 0)
        {
            strcat(G_cTxt, "(");
            strcat(G_cTxt, m_cGuildName);
            if (m_iGuildRank == 0) strcat(G_cTxt, DEF_MSG_GUILDMASTER1);
            else strcat(G_cTxt, DEF_MSG_GUILDSMAN1);
        }
    }

    
    put_aligned_string(sX, sX + 275, sY + 69, G_cTxt, 45, 25, 25);

    int iTemp;
    // Level
    format_to_local(G_cTxt, "{}", m_iLevel);
    put_aligned_string(sX + 180, sX + 250, sY + 106, G_cTxt, 45, 25, 25);
    // Exp
    format_to_local(G_cTxt, "{}", m_iExp);
    put_aligned_string(sX + 180, sX + 250, sY + 125, G_cTxt, 45, 25, 25);
    // Next.Exp
    format_to_local(G_cTxt, "{}", iGetLevelExp(m_iLevel + 1));
    put_aligned_string(sX + 180, sX + 250, sY + 142, G_cTxt, 45, 25, 25);
    // Hp
    iTemp = m_iHP;
    format_to_local(G_cTxt, "{}/{}", iTemp, m_iVit * 3 + m_iLevel * 2 + m_iStr / 2);
    put_aligned_string(sX + 180, sX + 250, sY + 173, G_cTxt, 45, 25, 25);
    // Mp
    iTemp = m_iMP;
    format_to_local(G_cTxt, "{}/{}", iTemp, m_iMag * 2 + m_iLevel * 2 + m_iInt / 2);
    put_aligned_string(sX + 180, sX + 250, sY + 191, G_cTxt, 45, 25, 25);
    // Sp
    iTemp = m_iSP;
    format_to_local(G_cTxt, "{}/{}", iTemp, m_iStr * 2 + m_iLevel * 2);
    put_aligned_string(sX + 180, sX + 250, sY + 208, G_cTxt, 45, 25, 25);
    // Max.Load
    format_to_local(G_cTxt, "{}/{}", (_iCalcTotalWeight() / 100), (m_iStr * 5 + m_iLevel * 5));
    put_aligned_string(sX + 180, sX + 250, sY + 240, G_cTxt, 45, 25, 25);
    // Enemy Kills
    format_to_local(G_cTxt, "{}", m_iEnemyKillCount);
    put_aligned_string(sX + 180, sX + 250, sY + 257, G_cTxt, 45, 25, 25);
    // Str
    format_to_local(G_cTxt, "{}", m_iStr);
    put_aligned_string(sX + 48, sX + 82, sY + 285, G_cTxt, 45, 25, 25);
    // Vit
    format_to_local(G_cTxt, "{}", m_iVit);
    put_aligned_string(sX + 218, sX + 251, sY + 285, G_cTxt, 45, 25, 25);
    // Dex
    format_to_local(G_cTxt, "{}", m_iDex);
    put_aligned_string(sX + 48, sX + 82, sY + 302, G_cTxt, 45, 25, 25);
    // Int
    format_to_local(G_cTxt, "{}", m_iInt);
    put_aligned_string(sX + 135, sX + 167, sY + 285, G_cTxt, 45, 25, 25);
    // Mag
    format_to_local(G_cTxt, "{}", m_iMag);
    put_aligned_string(sX + 135, sX + 167, sY + 302, G_cTxt, 45, 25, 25);
    // Chr
    format_to_local(G_cTxt, "{}", m_iCharisma);
    put_aligned_string(sX + 218, sX + 251, sY + 302, G_cTxt, 45, 25, 25);
    
    for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
        cEquipPosStatus[i] = -1;

    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if ((m_pItemList[i] != NULL) && (m_bIsItemEquipped[i] == TRUE))	cEquipPosStatus[m_pItemList[i]->m_cEquipPos] = i;
    }
    
    if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
    {
        cCollison = -1;
        
        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 0]->put_sprite_fast(sX + 171, sY + 290, m_sPlayerType - 1, m_dwCurTime);

        if (cEquipPosStatus[DEF_EQUIPPOS_HEAD] == -1)
        {
            
            _GetHairColorColor(((m_sPlayerAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 18]->put_sprite_color(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x0F00) >> 8, iR, iG, iB, m_dwCurTime);
        }

        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 19]->put_sprite_fast(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x000F), m_dwCurTime);

        
        if (cEquipPosStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_BACK]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 41, sY + 137, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 41, sY + 137, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 41, sY + 137, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 41, sY + 137, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 41, sY + 137, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_BACK;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_PANTS]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_PANTS;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_ARMS]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_ARMS;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_BOOTS;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_BODY]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_BODY;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_FULLBODY;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_LHAND]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 90, sY + 170, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 90, sY + 170, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 90, sY + 170, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 90, sY + 170, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 90, sY + 170, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_LHAND;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_RHAND]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 57, sY + 186, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 57, sY + 186, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 57, sY + 186, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 57, sY + 186, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 57, sY + 186, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_RHAND;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 57, sY + 186, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 57, sY + 186, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 57, sY + 186, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 57, sY + 186, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 57, sY + 186, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_TWOHAND;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_NECK]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 35, sY + 120, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 35, sY + 120, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 35, sY + 120, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 35, sY + 120, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 35, sY + 120, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_NECK;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 32, sY + 193, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 32, sY + 193, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 32, sY + 193, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 32, sY + 193, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 32, sY + 193, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_RFINGER;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_HEAD]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_fast(sX + 72, sY + 135, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_sprite_color(sX + 72, sY + 135, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite2(sX + 72, sY + 135, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite_color(sX + 72, sY + 135, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->check_collison(sX + 72, sY + 135, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_HEAD;
        }
        if (cCollison != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[cCollison]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[cCollison]]->m_sSpriteFrame;
            if (cCollison == DEF_EQUIPPOS_HEAD)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 72, sY + 135, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_RFINGER)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 32, sY + 193, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_NECK)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 35, sY + 120, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_TWOHAND)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 57, sY + 186, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_RHAND)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 57, sY + 186, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_LHAND)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 90, sY + 170, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_BODY)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_FULLBODY)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_BOOTS)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_ARMS)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_PANTS)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_BACK)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->put_trans_sprite(sX + 41, sY + 137, sFrame, m_dwCurTime);
        }
       
    }
    else if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
    {
        cCollison = -1;
        
        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 40]->put_sprite_fast(sX + 171, sY + 290, m_sPlayerType - 4, m_dwCurTime);

        if (cEquipPosStatus[DEF_EQUIPPOS_HEAD] == -1)
        {
            _GetHairColorColor(((m_sPlayerAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 18 + 40]->put_sprite_color(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x0F00) >> 8, iR, iG, iB, m_dwCurTime);
        }

        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 19 + 40]->put_sprite_fast(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x000F), m_dwCurTime);

        
        if ((cEquipPosStatus[DEF_EQUIPPOS_PANTS] != -1))
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if ((sSprH == 12) && (sFrame == 0)) iSkirtDraw = 1;
        }

        

        if (cEquipPosStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_BACK]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 45, sY + 143, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 45, sY + 143, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 45, sY + 143, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 45, sY + 143, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 45, sY + 143, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_BACK;
        }

        if ((cEquipPosStatus[DEF_EQUIPPOS_BOOTS] != -1) && (iSkirtDraw == 1))
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_BOOTS;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_PANTS]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_PANTS;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_ARMS]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_ARMS;
        }

        if ((cEquipPosStatus[DEF_EQUIPPOS_BOOTS] != -1) && (iSkirtDraw == 0))
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_BOOTS;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_BODY]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_BODY;
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 171, sY + 290, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 171, sY + 290, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 171, sY + 290, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_FULLBODY;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_LHAND]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 84, sY + 175, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 84, sY + 175, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 84, sY + 175, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 84, sY + 175, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 84, sY + 175, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_LHAND;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_RHAND]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 60, sY + 191, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 60, sY + 191, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 60, sY + 191, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 60, sY + 191, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 60, sY + 191, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_RHAND;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 60, sY + 191, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 60, sY + 191, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 60, sY + 191, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 60, sY + 191, sFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 60, sY + 191, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_TWOHAND;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_NECK]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 35, sY + 120, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 35, sY + 120, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 35, sY + 120, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 35, sY + 120, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 35, sY + 120, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_NECK;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 32, sY + 193, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 32, sY + 193, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 32, sY + 193, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 32, sY + 193, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 32, sY + 193, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_RFINGER;
        }

        if (cEquipPosStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPosStatus[DEF_EQUIPPOS_HEAD]] == FALSE)
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_fast(sX + 72, sY + 139, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_sprite_color(sX + 72, sY + 139, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            else
            {
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite2(sX + 72, sY + 139, sFrame, m_dwCurTime);
                else m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite_color(sX + 72, sY + 139, sFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], m_dwCurTime);
            }
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->check_collison(sX + 72, sY + 139, sFrame, msX, msY))
                cCollison = DEF_EQUIPPOS_HEAD;
        }
        if (cCollison != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[cCollison]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[cCollison]]->m_sSpriteFrame;
            if (cCollison == DEF_EQUIPPOS_HEAD)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 72, sY + 139, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_RFINGER)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 32, sY + 193, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_NECK)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 35, sY + 120, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_TWOHAND)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 60, sY + 191, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_RHAND)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 60, sY + 191, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_LHAND)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 84, sY + 175, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_BODY)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_FULLBODY)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_BOOTS)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_ARMS)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_PANTS)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 171, sY + 290, sFrame, m_dwCurTime);
            else if (cCollison == DEF_EQUIPPOS_BACK)
                m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->put_trans_sprite(sX + 45, sY + 143, sFrame, m_dwCurTime);
        }
    }

    // v2.05
    if ((msX >= sX + 15) && (msX <= sX + 15 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 15, sY + 340, 5, FALSE, m_bDialogTrans);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 15, sY + 340, 4, FALSE, m_bDialogTrans);


    if ((msX >= sX + 98) && (msX <= sX + 98 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 98, sY + 340, 45, FALSE, m_bDialogTrans);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 98, sY + 340, 44, FALSE, m_bDialogTrans);


    if ((msX >= sX + 180) && (msX <= sX + 180 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 180, sY + 340, 11, FALSE, m_bDialogTrans);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 180, sY + 340, 10, FALSE, m_bDialogTrans);
}

#ifdef _DEBUG
void CGame::DrawDialogBox_LevelUpSetting(short msX, short msY)
{
    short sX, sY, szX;
    DWORD dwTime = m_dwCurTime;
    char cTxt[120];
    sX = m_stDialogBoxInfo[12].sX;
    sY = m_stDialogBoxInfo[12].sY;
    szX = m_stDialogBoxInfo[12].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX - 1, sY + 100, 4);

    put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_LEVELUP_SETTING1);
    put_aligned_string(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_LEVELUP_SETTING2);

    format_to_local(cTxt, "X:{}  Y:{}, sx:{}, sy:{}", msX, msY, sX, sY);
    put_string(sX + 20, sY + 80, cTxt, Color(0, 0, 0));

    format_to_local(cTxt, DRAW_DIALOGBOX_LEVELUP_SETTING3, m_iLU_Point);
    put_string(sX + 40,  sY + 85, cTxt, Color(0,0,0));
    
    format_to_local(cTxt, "{}", m_iLU_Point);
    put_string(sX + 105, sY + 103, cTxt, Color(0, 0, 0));

    put_string(sX + 20, sY + 127, DRAW_DIALOGBOX_LEVELUP_SETTING4, Color(5, 5, 5));//"Strength"
    format_to_local(cTxt, "{}", m_cLU_Str);
    put_string(sX + 158, sY + 127, cTxt, Color(25, 35, 25));
    format_to_local(cTxt, "{}", m_iStr);
    put_string(sX + 105, sY + 127, cTxt, Color(25, 35, 25));

    put_string(sX + 20, sY + 147, DRAW_DIALOGBOX_LEVELUP_SETTING5, Color(5, 5, 5));//"Vitality"
    format_to_local(cTxt, "{}", m_cLU_Vit);
    put_string(sX + 158, sY + 147, cTxt, Color(25, 35, 25));
    format_to_local(cTxt, "{}", m_iVit);
    put_string(sX + 105, sY + 147, cTxt, Color(25, 35, 25));

    put_string(sX + 20, sY + 165, DRAW_DIALOGBOX_LEVELUP_SETTING6, Color(5, 5, 5));//"Dexterity"
    format_to_local(cTxt, "{}", m_cLU_Dex);
    put_string(sX + 158, sY + 165, cTxt, Color(25, 35, 25));
    format_to_local(cTxt, "{}", m_iDex);
    put_string(sX + 105, sY + 165, cTxt, Color(25, 35, 25));

    put_string(sX + 20, sY + 185, DRAW_DIALOGBOX_LEVELUP_SETTING7, Color(5, 5, 5));//"Intelligence"
    format_to_local(cTxt, "{}", m_cLU_Int);
    put_string(sX + 158, sY + 185, cTxt, Color(25, 35, 25));
    format_to_local(cTxt, "{}", m_iInt);
    put_string(sX + 105, sY + 185, cTxt, Color(25, 35, 25));

    put_string(sX + 20, sY + 203, DRAW_DIALOGBOX_LEVELUP_SETTING8, Color(5, 5, 5));//"Magic"
    format_to_local(cTxt, "{}", m_cLU_Mag);
    put_string(sX + 158, sY + 203, cTxt, Color(25, 35, 25));
    format_to_local(cTxt, "{}", m_iMag);
    put_string(sX + 105, sY + 203, cTxt, Color(25, 35, 25));

    put_string(sX + 20, sY + 223, DRAW_DIALOGBOX_LEVELUP_SETTING9, Color(5, 5, 5));//"Charisma"
    format_to_local(cTxt, "{}", m_cLU_Char);
    put_string(sX + 158, sY + 223, cTxt, Color(25, 35, 25));
    format_to_local(cTxt, "{}", m_iCharisma);
    put_string(sX + 105, sY + 223, cTxt, Color(25, 35, 25));

    
    
    
    

    if (_iCheckLUS() == 0)
    {
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
        else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
    }

    switch (_iCheckLUS())
    {
        case 0:	break;
        case 1: put_aligned_string(sX, sX + szX, sY + 295, DRAW_DIALOGBOX_LEVELUP_SETTING14, 65, 0, 0);
            break;
        case 2: put_aligned_string(sX, sX + szX, sY + 295, DRAW_DIALOGBOX_LEVELUP_SETTING15, 65, 0, 0);
            put_aligned_string(sX, sX + szX, sY + 310, DRAW_DIALOGBOX_LEVELUP_SETTING16, 65, 0, 0);
            put_aligned_string(sX, sX + szX, sY + 325, DRAW_DIALOGBOX_LEVELUP_SETTING17, 65, 0, 0);
            break;
    }
}

#else

void CGame::DrawDialogBox_LevelUpSetting(short msX, short msY)
{
    short sX, sY, szX;
    DWORD dwTime = m_dwCurTime;
    char cTxt[120];
    sX = m_stDialogBoxInfo[12].sX;
    sY = m_stDialogBoxInfo[12].sY;
    szX = m_stDialogBoxInfo[12].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 2);

    put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_LEVELUP_SETTING1);
    put_aligned_string(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_LEVELUP_SETTING2);

    format_to_local(cTxt, DRAW_DIALOGBOX_LEVELUP_SETTING3, m_iLU_Point);
    put_string(sX + 40, sY + 85, cTxt, Color(0, 0, 0));

    put_string(sX + 44, sY + 110, DRAW_DIALOGBOX_LEVELUP_SETTING4, Color(5, 5, 5));//"Strength"
    put_string(sX + 139, sY + 112, "_____", Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_cLU_Str);
    put_string(sX + 139, sY + 110, cTxt, Color(25, 35, 25));
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 190, sY + 115, 19, dwTime);
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 205, sY + 115, 20, dwTime);

    put_string(sX + 44, sY + 125, DRAW_DIALOGBOX_LEVELUP_SETTING5, Color(5, 5, 5));//"Vitality"
    put_string(sX + 139, sY + 127, "_____", Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_cLU_Vit);
    put_string(sX + 139, sY + 125, cTxt, Color(25, 35, 25));
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 190, sY + 130, 19, dwTime);
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 205, sY + 130, 20, dwTime);

    put_string(sX + 44, sY + 140, DRAW_DIALOGBOX_LEVELUP_SETTING6, Color(5, 5, 5));//"Dexterity"
    put_string(sX + 139, sY + 142, "_____", Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_cLU_Dex);
    put_string(sX + 139, sY + 140, cTxt, Color(25, 35, 25));
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 190, sY + 145, 19, dwTime);
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 205, sY + 145, 20, dwTime);

    put_string(sX + 44, sY + 155, DRAW_DIALOGBOX_LEVELUP_SETTING7, Color(5, 5, 5));//"Intelligence"
    put_string(sX + 139, sY + 157, "_____", Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_cLU_Int);
    put_string(sX + 139, sY + 155, cTxt, Color(25, 35, 25));
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 190, sY + 160, 19, dwTime);
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 205, sY + 160, 20, dwTime);

    put_string(sX + 44, sY + 170, DRAW_DIALOGBOX_LEVELUP_SETTING8, Color(5, 5, 5));//"Magic"
    put_string(sX + 139, sY + 172, "_____", Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_cLU_Mag);
    put_string(sX + 139, sY + 170, cTxt, Color(25, 35, 25));
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 190, sY + 175, 19, dwTime);
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 205, sY + 175, 20, dwTime);

    put_string(sX + 44, sY + 185, DRAW_DIALOGBOX_LEVELUP_SETTING9, Color(5, 5, 5));//"Charisma"
    put_string(sX + 139, sY + 187, "_____", Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_cLU_Char);
    put_string(sX + 139, sY + 185, cTxt, Color(25, 35, 25));
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 190, sY + 190, 19, dwTime);
    m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 205, sY + 190, 20, dwTime);

    put_aligned_string(sX, sX + szX, sY + 220, DRAW_DIALOGBOX_LEVELUP_SETTING10);
    put_aligned_string(sX, sX + szX, sY + 235, DRAW_DIALOGBOX_LEVELUP_SETTING11);
    put_aligned_string(sX, sX + szX, sY + 250, DRAW_DIALOGBOX_LEVELUP_SETTING12);
    put_aligned_string(sX, sX + szX, sY + 265, DRAW_DIALOGBOX_LEVELUP_SETTING13);

    if (_iCheckLUS() == 0)
    {
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
        else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
    }

    switch (_iCheckLUS())
    {
        case 0:	break;
        case 1: put_aligned_string(sX, sX + szX, sY + 295, DRAW_DIALOGBOX_LEVELUP_SETTING14, 65, 0, 0);
            break;
        case 2: put_aligned_string(sX, sX + szX, sY + 295, DRAW_DIALOGBOX_LEVELUP_SETTING15, 65, 0, 0);
            put_aligned_string(sX, sX + szX, sY + 310, DRAW_DIALOGBOX_LEVELUP_SETTING16, 65, 0, 0);
            put_aligned_string(sX, sX + szX, sY + 325, DRAW_DIALOGBOX_LEVELUP_SETTING17, 65, 0, 0);
            break;
    }
}
#endif

void CGame::DrawDialogBox_CityHallMenu(short msX, short msY)
{
    short sX, sY, szX;
    char cTxt[120];

    sX = m_stDialogBoxInfo[13].sX;
    sY = m_stDialogBoxInfo[13].sY;
    szX = m_stDialogBoxInfo[13].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 18);//CityHall Menu Text

    switch (m_stDialogBoxInfo[13].cMode)
    {
        case 0:
            if (m_bCitizen == FALSE)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
                    put_aligned_string(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_CITYHALL_MENU1, 255, 255, 255);	
                else put_aligned_string(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_CITYHALL_MENU1, 4, 0, 50);		
            }
            else put_aligned_string(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_CITYHALL_MENU1, 65, 65, 65);			

            if (m_iRewardGold > 0)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
                    put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU4, 255, 255, 255);	
                else put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU4, 4, 0, 50);		
            }
            else put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU4, 65, 65, 65);			

            if (m_iEnemyKillCount >= 300)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
                    put_aligned_string(sX, sX + szX, sY + 120, DRAW_DIALOGBOX_CITYHALL_MENU8, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 120, DRAW_DIALOGBOX_CITYHALL_MENU8, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 120, DRAW_DIALOGBOX_CITYHALL_MENU8, 65, 65, 65);

            if (m_stQuest.sQuestType != NULL)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 145) && (msY < sY + 170))
                    put_aligned_string(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_CITYHALL_MENU11, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_CITYHALL_MENU11, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_CITYHALL_MENU11, 65, 65, 65);

            if ((m_bIsCrusadeMode == FALSE) && m_bCitizen && (m_iPKCount == 0))
            {
                if (m_bHunter == TRUE)
                { 
                    if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 170) && (msY < sY + 195))
                        put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU56, 255, 255, 255);
                    else
                        put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU56, 4, 0, 50);
                }
                else if (m_iLevel < 100) 
                {
                    if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 170) && (msY < sY + 195))
                        put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU56, 255, 255, 255);
                    else
                        put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU56, 4, 0, 50);
                }
                else	
                    put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU56, 65, 65, 65);
            }
            else put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU56, 65, 65, 65);

            if ((m_bIsCrusadeMode == FALSE) && m_bCitizen && (m_iPKCount == 0))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 195) && (msY < sY + 220))
                    put_aligned_string(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU69, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU69, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU69, 65, 65, 65);

            if (m_bIsCrusadeMode && m_bCitizen)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 220) && (msY < sY + 220))
                    put_aligned_string(sX, sX + szX, sY + 220, DRAW_DIALOGBOX_CITYHALL_MENU14, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 220, DRAW_DIALOGBOX_CITYHALL_MENU14, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 220, DRAW_DIALOGBOX_CITYHALL_MENU14, 65, 65, 65);

            put_aligned_string(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU17);

            break;

        case 1:
            put_aligned_string(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU18, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU19, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU20, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU21, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU22, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU23, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU24, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 200, DRAW_DIALOGBOX_CITYHALL_MENU25, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU26, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 230, DRAW_DIALOGBOX_CITYHALL_MENU27, 55, 25, 25);

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 2:
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU28, 55, 25, 25);
            break;

        case 3:
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU29, 55, 25, 25);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 4:
            put_aligned_string(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU30, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 100, DRAW_DIALOGBOX_CITYHALL_MENU31, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 115, DRAW_DIALOGBOX_CITYHALL_MENU32, 55, 25, 25);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 5:
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU33, 55, 25, 25);
            format_to_local(cTxt, DRAW_DIALOGBOX_CITYHALL_MENU34, m_iRewardGold);
            put_aligned_string(sX, sX + szX, sY + 140, cTxt, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU35, 55, 25, 25);

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 7:
            put_aligned_string(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU46, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU47, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU48, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU49, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU50, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU51, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU52, 55, 25, 25);

            put_aligned_string(sX, sX + szX, sY + 205, DRAW_DIALOGBOX_CITYHALL_MENU53, 55, 25, 25);

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 8:
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU54, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU55, 55, 25, 25);

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 9:
            if (m_bHunter)
            {
                put_aligned_string(sX, sX + szX, sY + 53, DRAW_DIALOGBOX_CITYHALL_MENU57, 200, 200, 25);
            }
            else
            {
                put_aligned_string(sX, sX + szX, sY + 53, DRAW_DIALOGBOX_CITYHALL_MENU58, 200, 200, 25);
            }
            put_aligned_string(sX, sX + szX, sY + 78, DRAW_DIALOGBOX_CITYHALL_MENU59, 55, 25, 25);

            put_string(sX + 35, sY + 108, DRAW_DIALOGBOX_CITYHALL_MENU60, Color(220, 130, 45));
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU61, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU62, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU63, 55, 25, 25);
            put_string(sX + 35, sY + 177, DRAW_DIALOGBOX_CITYHALL_MENU64, Color(220, 130, 45));
            put_aligned_string(sX, sX + szX, sY + 194, DRAW_DIALOGBOX_CITYHALL_MENU65, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 209, DRAW_DIALOGBOX_CITYHALL_MENU66, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 224, DRAW_DIALOGBOX_CITYHALL_MENU67, 55, 25, 25);

            put_aligned_string(sX, sX + szX, sY + 252, DRAW_DIALOGBOX_CITYHALL_MENU68, 55, 25, 25);
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 10:
            if (m_iTeleportMapCount > 0)
            {
                put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_CITYHALL_MENU69, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU70, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU71, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU72, 55, 25, 25);

                put_string2(sX + 21, sY + 200, DRAW_DIALOGBOX_CITYHALL_MENU72_1, 55, 25, 25);

                for (int i = 0; i < m_iTeleportMapCount; i++)
                {
                    ZeroMemory(cTxt, sizeof(cTxt));
                    GetOfficialMapName(m_stTeleportList[i].mapname, cTxt);
                    format_to_local(G_cTxt, DRAW_DIALOGBOX_CITYHALL_MENU77, cTxt, m_stTeleportList[i].iCost);
                    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 120 + i * 15) && (msY <= sY + 134 + i * 15))
                        put_aligned_string(sX, sX + szX, sY + 130 + i * 15, G_cTxt, 255, 255, 255);
                    else put_aligned_string(sX, sX + szX, sY + 130 + i * 15, G_cTxt, 55, 25, 25);
                }
            }
            else if (m_iTeleportMapCount == -1)
            {
                put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU73, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_CITYHALL_MENU74, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_CITYHALL_MENU75, 55, 25, 25);
            }
            else
            {
                put_aligned_string(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_CITYHALL_MENU76, 55, 25, 25);
            }
            break;
    }
}

void CGame::DrawDialogBox_Exchange(short msX, short msY)
{
    short sX, sY, szX;
    DWORD dwTime = m_dwCurTime;
    char cItemColor, cTxt[120]{}, cTxt2[128]{};
    char cNameStr[120]{}, cSubStr1[120]{}, cSubStr2[120]{};
    int iLoc;

    sX = m_stDialogBoxInfo[27].sX;
    sY = m_stDialogBoxInfo[27].sY;
    szX = m_stDialogBoxInfo[27].sSizeX;

    //DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 1);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_NEWEXCHANGE, sX, sY, 0);

    //	DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 9);

    switch (m_stDialogBoxInfo[27].cMode)
    {
        case 1:
            if (m_stDialogBoxInfo[27].sV1 != -1)
            {
                
                cItemColor = m_stDialogBoxInfo[27].sV4;
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV1]->put_sprite_fast(sX + 130, sY + 100, m_stDialogBoxInfo[27].sV2, dwTime);
                else
                {
                    switch (m_stDialogBoxInfo[27].sV1)
                    {
                        case 1:
                        case 2:
                        case 3:  m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV1]->put_sprite_color(sX + 130, sY + 100, m_stDialogBoxInfo[27].sV2,
                            m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime); break;

                        default: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV1]->put_sprite_color(sX + 130, sY + 100, m_stDialogBoxInfo[27].sV2,
                            m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime); break;
                    }
                }

               
                GetItemName(m_stDialogBoxInfo[27].cStr, m_stDialogBoxInfo[27].dwV1, cNameStr, cSubStr1, cSubStr2, 120);

                put_aligned_string(sX + 80, sX + 180, sY + 42, m_cPlayerName, 35, 55, 35);

                format_to_local(cTxt, "{}", cNameStr);

                if (m_bIsSpecial)
                {
                    put_aligned_string(sX + 15, sX + 250, sY + 145, cTxt, 0, 255, 50);
                    put_aligned_string(sX + 16, sX + 251, sY + 145, cTxt, 0, 255, 50);
                }
                else
                {
                    put_aligned_string(sX + 15, sX + 250, sY + 145, cTxt, 35, 35, 35);
                    put_aligned_string(sX + 16, sX + 251, sY + 145, cTxt, 35, 35, 35);
                }

                iLoc = 0;
                if (strlen(cSubStr1) != 0)
                {
                    put_aligned_string(sX + 16, sX + 250, sY + 160 + iLoc, cSubStr1, 0, 0, 0);
                    iLoc += 15;
                }
                if (strlen(cSubStr2) != 0)
                {
                    put_aligned_string(sX + 16, sX + 250, sY + 160 + iLoc, cSubStr2, 0, 0, 0);
                    iLoc += 15;
                }

                if (m_stDialogBoxInfo[27].sV3 != 1)
                {
                    
                    if (m_stDialogBoxInfo[27].sV3 > 1)
                    {
                        DisplayGold(m_stDialogBoxInfo[27].sV3);
                        strcpy(cTxt2, G_cTxt);
                    }
                    else
                        format_to_local(cTxt2, DRAW_DIALOGBOX_EXCHANGE2, m_stDialogBoxInfo[27].sV3);
                    

                    put_aligned_string(sX + 7, sX + 250, sY + 160 + iLoc, cTxt2, 35, 35, 35);
                    iLoc += 15;
                }

                if (m_stDialogBoxInfo[27].sV9 != -1)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE4, m_stDialogBoxInfo[27].sV11);
                    
                    put_aligned_string(sX + 7, sX + 250, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }

                if (iLoc < 45)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE3, m_stDialogBoxInfo[27].sV9, m_stDialogBoxInfo[27].sV10);
                    
                    put_aligned_string(sX + 7, sX + 250, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }
            }

            if (m_stDialogBoxInfo[27].sV5 != -1)
            {
                cItemColor = m_stDialogBoxInfo[27].sV8;
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV5]->put_sprite_fast(sX + 400, sY + 100,
                        m_stDialogBoxInfo[27].sV6, dwTime);
                else
                {
                    switch (m_stDialogBoxInfo[27].sV5)
                    {
                        case 1:
                        case 2:
                        case 3:  m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV5]->put_sprite_color(sX + 400, sY + 100, m_stDialogBoxInfo[27].sV6,
                            m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime); break;

                        default: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV5]->put_sprite_color(sX + 400, sY + 100, m_stDialogBoxInfo[27].sV6,
                            m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime); break;
                    }
                }

                put_aligned_string(sX + 250, sX + 540, sY + 42, m_stDialogBoxInfo[27].cStr4, 35, 55, 35);
               
                GetItemName(m_stDialogBoxInfo[27].cStr3, m_stDialogBoxInfo[27].dwV2, cNameStr, cSubStr1, cSubStr2, 120);

                format_to_local(cTxt, "{}", cNameStr);

                if (m_bIsSpecial)
                {
                    put_aligned_string(sX + 270, sX + 520, sY + 145, cTxt, 0, 255, 50);
                    put_aligned_string(sX + 271, sX + 521, sY + 145, cTxt, 0, 255, 50);
                }
                else
                {
                    put_aligned_string(sX + 270, sX + 520, sY + 145, cTxt, 35, 35, 35);
                    put_aligned_string(sX + 271, sX + 521, sY + 145, cTxt, 35, 35, 35);
                }

                iLoc = 0;
                if (strlen(cSubStr1) != 0)
                {
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cSubStr1, 0, 0, 0);
                    iLoc += 15;
                }

                if (strlen(cSubStr2) != 0)
                {
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cSubStr2, 0, 0, 0);
                    iLoc += 15;
                }

                if (m_stDialogBoxInfo[27].sV7 != 1)
                {

                    
                    if (m_stDialogBoxInfo[27].sV7 > 1)
                    {
                        DisplayGold(m_stDialogBoxInfo[27].sV7);
                        strcpy(cTxt2, G_cTxt);
                    }
                    else
                        format_to_local(cTxt2, DRAW_DIALOGBOX_EXCHANGE2, m_stDialogBoxInfo[27].sV7);
                    
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cTxt2, 35, 35, 35);
                    iLoc += 15;
                }

                if (m_stDialogBoxInfo[27].sV14 != -1)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE4, m_stDialogBoxInfo[27].sV14);
                    
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }

                if (iLoc < 45)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE3, m_stDialogBoxInfo[27].sV12, m_stDialogBoxInfo[27].sV13);
                    
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }
            }

            
            if ((m_stDialogBoxInfo[27].sV1 != -1) && (m_stDialogBoxInfo[27].sV5 == -1))
            {
                put_aligned_string(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE9, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE10, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE11, 55, 25, 25);
            }
            else if ((m_stDialogBoxInfo[27].sV1 == -1) && (m_stDialogBoxInfo[27].sV5 != -1))
            {
                put_aligned_string(sX, sX + szX, sY + 205 + 10, DRAW_DIALOGBOX_EXCHANGE12, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 220 + 10, DRAW_DIALOGBOX_EXCHANGE13, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE14, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE15, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE16, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 280 + 10, DRAW_DIALOGBOX_EXCHANGE17, 55, 25, 25);
            }
            else if ((m_stDialogBoxInfo[27].sV1 != -1) && (m_stDialogBoxInfo[27].sV5 != -1))
            {
                put_aligned_string(sX, sX + szX, sY + 205 + 10, DRAW_DIALOGBOX_EXCHANGE18, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 220 + 10, DRAW_DIALOGBOX_EXCHANGE19, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE20, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE21, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE22, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + 280 + 10, DRAW_DIALOGBOX_EXCHANGE23, 55, 25, 25);
            }

            if ((m_stDialogBoxInfo[27].sV1 != -1) && (m_stDialogBoxInfo[27].sV5 != -1))
            {
                if ((msX >= sX + 200) && (msX <= sX + 200 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY))
                    put_string_sprite_font(sX + 220, sY + 310, "Exchange", 6, 6, 20);
                else put_string_sprite_font(sX + 220, sY + 310, "Exchange", 0, 0, 7);
            }
            else put_string_sprite_font(sX + 220, sY + 310, "Exchange", 15, 15, 15);

            if ((msX >= sX + 450) && (msX <= sX + 450 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY))
                put_string_sprite_font(sX + 450, sY + 310, "Cancel", 6, 6, 20);
            else put_string_sprite_font(sX + 450, sY + 310, "Cancel", 0, 0, 7);
            break;

        case 2:
            if (m_stDialogBoxInfo[27].sV1 != -1)
            {
                
                cItemColor = m_stDialogBoxInfo[27].sV4;
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV1]->put_sprite_fast(sX + 130, sY + 100,
                        m_stDialogBoxInfo[27].sV2, dwTime);
                else
                {
                    switch (m_stDialogBoxInfo[27].sV1)
                    {
                        case 1:
                        case 2:
                        case 3:  m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV1]->put_sprite_color(sX + 130, sY + 100, m_stDialogBoxInfo[27].sV2,
                            m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime); break;

                        default: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV1]->put_sprite_color(sX + 130, sY + 100, m_stDialogBoxInfo[27].sV2,
                            m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime); break;
                    }
                }
               
                GetItemName(m_stDialogBoxInfo[27].cStr, m_stDialogBoxInfo[27].dwV1, cNameStr, cSubStr1, cSubStr2, 120);
                put_aligned_string(sX + 80, sX + 180, sY + 42, m_cPlayerName, 35, 55, 35);

                format_to_local(cTxt, "{}", cNameStr);

                if (m_bIsSpecial)
                {
                    put_aligned_string(sX + 15, sX + 250, sY + 145, cTxt, 0, 255, 50);
                    put_aligned_string(sX + 16, sX + 251, sY + 145, cTxt, 0, 255, 50);
                }
                else
                {
                    put_aligned_string(sX + 15, sX + 250, sY + 145, cTxt, 35, 35, 35);
                    put_aligned_string(sX + 16, sX + 251, sY + 145, cTxt, 35, 35, 35);
                }

                iLoc = 0;
                if (strlen(cSubStr1) != 0)
                {
                    put_aligned_string(sX + 16, sX + 250, sY + 160 + iLoc, cSubStr1, 0, 0, 0);
                    iLoc += 15;
                }

                if (strlen(cSubStr2) != 0)
                {
                    put_aligned_string(sX + 16, sX + 250, sY + 160 + iLoc, cSubStr2, 0, 0, 0);
                    iLoc += 15;
                }

                if (m_stDialogBoxInfo[27].sV3 != 1)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE2, m_stDialogBoxInfo[27].sV3);
                    
                    put_aligned_string(sX + 7, sX + 250, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }

                if (m_stDialogBoxInfo[27].sV9 != -1)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE4, m_stDialogBoxInfo[27].sV11);
                    
                    put_aligned_string(sX + 7, sX + 250, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }

                if (iLoc < 45)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE3, m_stDialogBoxInfo[27].sV9, m_stDialogBoxInfo[27].sV10);
                    
                    put_aligned_string(sX + 7, sX + 250, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }
            }

            if (m_stDialogBoxInfo[27].sV5 != -1)
            {
                cItemColor = m_stDialogBoxInfo[27].sV8;
                if (cItemColor == 0)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV5]->put_sprite_fast(sX + 400, sY + 100,
                        m_stDialogBoxInfo[27].sV6, dwTime);
                else
                {
                    switch (m_stDialogBoxInfo[27].sV5)
                    {
                        case 1:
                        case 2:
                        case 3:  m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV5]->put_sprite_color(sX + 400, sY + 100, m_stDialogBoxInfo[27].sV6,
                            m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime); break;

                        default: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[27].sV5]->put_sprite_color(sX + 400, sY + 100, m_stDialogBoxInfo[27].sV6,
                            m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime); break;
                    }
                }
               
                GetItemName(m_stDialogBoxInfo[27].cStr3, m_stDialogBoxInfo[27].dwV2, cNameStr, cSubStr1, cSubStr2, 120);

                put_aligned_string(sX + 250, sX + 540, sY + 42, m_stDialogBoxInfo[27].cStr4, 35, 55, 35);
                format_to_local(cTxt, "{}", cNameStr);

                if (m_bIsSpecial)
                {
                    put_aligned_string(sX + 270, sX + 520, sY + 145, cTxt, 0, 255, 50);
                    put_aligned_string(sX + 271, sX + 521, sY + 145, cTxt, 0, 255, 50);
                }
                else
                {
                    put_aligned_string(sX + 270, sX + 520, sY + 145, cTxt, 35, 35, 35);
                    put_aligned_string(sX + 271, sX + 521, sY + 145, cTxt, 35, 35, 35);
                }

                iLoc = 0;
                if (strlen(cSubStr1) != 0)
                {
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cSubStr1, 0, 0, 0);
                    iLoc += 15;
                }

                if (strlen(cSubStr2) != 0)
                {
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cSubStr2, 0, 0, 0);
                    iLoc += 15;
                }

                if (m_stDialogBoxInfo[27].sV7 != 1)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE2, m_stDialogBoxInfo[27].sV7);
                    
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }

                if (m_stDialogBoxInfo[27].sV14 != -1)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE4, m_stDialogBoxInfo[27].sV14);
                    
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }

                if (iLoc < 45)
                {
                    format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE3, m_stDialogBoxInfo[27].sV12, m_stDialogBoxInfo[27].sV13);
                    
                    put_aligned_string(sX + 270, sX + 520, sY + 160 + iLoc, cTxt, 35, 35, 35);
                    iLoc += 15;
                }
            }

            put_aligned_string(sX, sX + szX, sY + 205 + 10, DRAW_DIALOGBOX_EXCHANGE33, 55, 25, 25);
            
            put_aligned_string(sX, sX + szX, sY + 220 + 10, DRAW_DIALOGBOX_EXCHANGE34, 55, 25, 25);
            
            put_aligned_string(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE35, 55, 25, 25);
            
            put_aligned_string(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE36, 55, 25, 25);
            
            put_aligned_string(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE37, 55, 25, 25);
            
            put_aligned_string(sX, sX + szX, sY + 280 + 10, DRAW_DIALOGBOX_EXCHANGE38, 55, 25, 25);
            

            if ((msX >= sX + 450) && (msX <= sX + 450 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY))
                put_string_sprite_font(sX + 450, sY + 310, "Cancel", 6, 6, 20);
            else put_string_sprite_font(sX + 450, sY + 310, "Cancel", 0, 0, 7);
            break;
    }
}

void CGame::DrawDialogBox_Fishing(short msX, short msY)
{

    short sX, sY;
    DWORD dwTime = m_dwCurTime;
    char  cTxt[120]{};

    sX = m_stDialogBoxInfo[24].sX;
    sY = m_stDialogBoxInfo[24].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
    GetItemName(m_stDialogBoxInfo[24].cStr, NULL, cStr1, cStr2, cStr3, 64);

    switch (m_stDialogBoxInfo[24].cMode)
    {
        case 0:
            
            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[24].sV3]->put_sprite_fast(sX + 18 + 35, sY + 18 + 17, m_stDialogBoxInfo[24].sV4, dwTime);
            
            format_to_local(cTxt, "{}", cStr1);
            put_string(sX + 98, sY + 14, cTxt, Color(255, 255, 255));

            format_to_local(cTxt, DRAW_DIALOGBOX_FISHING1, m_stDialogBoxInfo[24].sV2);
            
            put_string(sX + 98, sY + 28, cTxt, Color(0, 0, 0));

            put_string(sX + 97, sY + 43, DRAW_DIALOGBOX_FISHING2, Color(0, 0, 0));
            

            format_to_local(cTxt, "{} %", m_stDialogBoxInfo[24].sV1);
            put_string_sprite_font(sX + 157, sY + 40, cTxt, 10, 0, 0);
#if DEF_LANGUAGE == 1  
            if ((msX >= sX + 160) && (msX <= sX + 253) && (msY >= sY + 70) && (msY <= sY + 90))
                put_string(sX + 160, sY + 70, "²{¦b´N¸Õ¸Õ¡T", Color(78, 64, 249));
            else put_string(sX + 160, sY + 70, "²{¦b´N¸Õ¸Õ¡T", Color(26, 11, 216));
#else
            if ((msX >= sX + 160) && (msX <= sX + 253) && (msY >= sY + 70) && (msY <= sY + 90))
                put_string_sprite_font(sX + 160, sY + 70, "Try Now!", 6, 6, 20);
            else put_string_sprite_font(sX + 160, sY + 70, "Try Now!", 0, 0, 7);
#endif
            break;
    }

}

void CGame::DrawDialogBox_GuildMenu(short msX, short msY)
{
    short sX, sY, szX;
    int iAdjX, iAdjY;

    sX = m_stDialogBoxInfo[7].sX;
    sY = m_stDialogBoxInfo[7].sY;
    szX = m_stDialogBoxInfo[7].sSizeX;

    iAdjX = -13;
    iAdjY = 30;
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 19);

    switch (m_stDialogBoxInfo[7].cMode)
    {
        case 0:
            if ((m_iGuildRank == -1) && (m_iCharisma >= 20) && (m_iLevel >= 20))
            {
                if ((msX > sX + iAdjX + 80) && (msX < sX + iAdjX + 210) && (msY > sY + iAdjY + 63) && (msY < sY + iAdjY + 78))
                    put_aligned_string(sX, sX + szX, sY + iAdjY + 65, DRAW_DIALOGBOX_GUILDMENU1, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + iAdjY + 65, DRAW_DIALOGBOX_GUILDMENU1, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + iAdjY + 65, DRAW_DIALOGBOX_GUILDMENU1, 65, 65, 65);

            if (m_iGuildRank == 0)
            {
                if ((msX > sX + iAdjX + 72) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 82) && (msY < sY + iAdjY + 99))
                    put_aligned_string(sX, sX + szX, sY + iAdjY + 85, DRAW_DIALOGBOX_GUILDMENU4, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + iAdjY + 85, DRAW_DIALOGBOX_GUILDMENU4, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + iAdjY + 85, DRAW_DIALOGBOX_GUILDMENU4, 65, 65, 65);

            if ((msX > sX + iAdjX + 61) && (msX < sX + iAdjX + 226) && (msY > sY + iAdjY + 103) && (msY < sY + iAdjY + 120))
                put_aligned_string(sX, sX + szX, sY + iAdjY + 105, DRAW_DIALOGBOX_GUILDMENU7, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + iAdjY + 105, DRAW_DIALOGBOX_GUILDMENU7, 4, 0, 50);

            if ((msX > sX + iAdjX + 60) && (msX < sX + iAdjX + 227) && (msY > sY + iAdjY + 123) && (msY < sY + iAdjY + 139))
                put_aligned_string(sX, sX + szX, sY + iAdjY + 125, DRAW_DIALOGBOX_GUILDMENU9, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + iAdjY + 125, DRAW_DIALOGBOX_GUILDMENU9, 4, 0, 50);

            
            if (m_iGuildRank == 0 && m_iFightzoneNumber == 0)
            {
                
                if ((msX > sX + iAdjX + 72) && (msX < sX + iAdjX + 228) && (msY > sY + iAdjY + 143) && (msY < sY + iAdjY + 169))
                    put_aligned_string(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU11, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU11, 4, 0, 50);

            }
            else if (m_iGuildRank == 0 && m_iFightzoneNumber > 0)
            {
                
                if ((msX > sX + iAdjX + 72) && (msX < sX + iAdjX + 216) && (msY > sY + iAdjY + 143) && (msY < sY + iAdjY + 169))
                    put_aligned_string(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU13, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU13, 4, 0, 50);

            }
            else if (m_iFightzoneNumber < 0)
            {
                
                put_aligned_string(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU13, 65, 65, 65);
            }
            else put_aligned_string(sX, sX + szX, sY + iAdjY + 145, DRAW_DIALOGBOX_GUILDMENU11, 65, 65, 65);

            put_aligned_string(sX, sX + szX, sY + iAdjY + 205, DRAW_DIALOGBOX_GUILDMENU17);
            break;

        case 1:
            put_aligned_string(sX + 24, sX + 239, sY + 125, DRAW_DIALOGBOX_GUILDMENU18, 55, 25, 25);
            put_string(sX + 75, sY + 144, "____________________", Color(25, 35, 25));

#if DEF_LANGUAGE == 2	//¾ð¾î:China
            put_string(sX + 24, sY + 176, DRAW_DIALOGBOX_GUILDMENU82, Color(55, 25, 25));
            put_string(sX + 24, sY + 192, DRAW_DIALOGBOX_GUILDMENU83, Color(55, 25, 25));
#endif


            if (iGetTopDialogBoxIndex() != 7)
                put_string(sX + 75, sY + 140, m_cGuildName, Color(255, 255, 255));

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
            {
                if ((strcmp(m_cGuildName, "NONE") == 0) || (strlen(m_cGuildName) == 0))
                {
                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 24);//Create Gray Button
                }
                else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 25);//Create Highlight Button
            }
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 24);//Create Gray Button

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);//Red Cancel Button
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);//Gray Cancel Button
            break;

        case 2:
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU19, 55, 25, 25);
            break;
        case 3:
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_GUILDMENU20, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 140, m_cGuildName, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 144, "____________________", 25, 35, 25);
            put_aligned_string(sX, sX + szX, sY + 160, DRAW_DIALOGBOX_GUILDMENU21, 55, 25, 25);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
        case 4:
            put_aligned_string(sX, sX + szX, sY + 135, DRAW_DIALOGBOX_GUILDMENU22, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_GUILDMENU23, 55, 25, 25);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
        case 5:
            put_aligned_string(sX, sX + szX, sY + 90, DRAW_DIALOGBOX_GUILDMENU24);
            put_aligned_string(sX, sX + szX, sY + 105, m_cGuildName, 35, 35, 35);
            put_aligned_string(sX, sX + szX, sY + 109, "____________________", 0, 0, 0);
            put_aligned_string(sX, sX + szX, sY + 130, DRAW_DIALOGBOX_GUILDMENU25);
            put_aligned_string(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_GUILDMENU26);
            put_aligned_string(sX, sX + szX, sY + 160, DRAW_DIALOGBOX_GUILDMENU27);
            put_aligned_string(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_GUILDMENU28, 55, 25, 25);

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;
        case 6:
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU29, 55, 25, 25);
            break;
        case 7:
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU30, 55, 25, 25);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
        case 8:
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU31, 55, 25, 25);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
        case 9:
            put_aligned_string(sX, sX + szX, sY + iAdjY + 60, DRAW_DIALOGBOX_GUILDMENU32);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 75, DRAW_DIALOGBOX_GUILDMENU33);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 90, DRAW_DIALOGBOX_GUILDMENU34);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 105, DRAW_DIALOGBOX_GUILDMENU35);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 130, DRAW_DIALOGBOX_GUILDMENU36);
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 31);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 30);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;
        case 10:
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU37, 55, 25, 25);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
        case 11:
            put_aligned_string(sX, sX + szX, sY + iAdjY + 60, DRAW_DIALOGBOX_GUILDMENU38);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 75, DRAW_DIALOGBOX_GUILDMENU39);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 90, DRAW_DIALOGBOX_GUILDMENU40);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 105, DRAW_DIALOGBOX_GUILDMENU41);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 130, DRAW_DIALOGBOX_GUILDMENU42);
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 31);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 30);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;
        case 12:
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU43, 55, 25, 25);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

            
        case 13:
            put_aligned_string(sX, sX + szX, sY + iAdjY + 40, DRAW_DIALOGBOX_GUILDMENU44);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 55, DRAW_DIALOGBOX_GUILDMENU45);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 70, DRAW_DIALOGBOX_GUILDMENU46);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 85, DRAW_DIALOGBOX_GUILDMENU47);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 100, DRAW_DIALOGBOX_GUILDMENU48);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 115, DRAW_DIALOGBOX_GUILDMENU49);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 130, DRAW_DIALOGBOX_GUILDMENU50);

            if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 168) && (msY < sY + iAdjY + 185))
                put_string(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 170, DRAW_DIALOGBOX_GUILDMENU51, Color(255, 255, 255));
            else put_string(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 170, DRAW_DIALOGBOX_GUILDMENU51, Color(4, 0, 50));

            if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 168) && (msY < sY + iAdjY + 185))
                put_string(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 170, DRAW_DIALOGBOX_GUILDMENU53, Color(255, 255, 255));
            else put_string(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 170, DRAW_DIALOGBOX_GUILDMENU53, Color(4, 0, 50));

            if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 188) && (msY < sY + iAdjY + 205))
                put_string(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 190, DRAW_DIALOGBOX_GUILDMENU55, Color(255, 255, 255));
            else put_string(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 190, DRAW_DIALOGBOX_GUILDMENU55, Color(4, 0, 50));

            if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 188) && (msY < sY + iAdjY + 205))
                put_string(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 190, DRAW_DIALOGBOX_GUILDMENU57, Color(255, 255, 255));
            else put_string(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 190, DRAW_DIALOGBOX_GUILDMENU57, Color(4, 0, 50));

            if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 208) && (msY < sY + iAdjY + 225))
                put_string(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 210, DRAW_DIALOGBOX_GUILDMENU59, Color(255, 255, 255));
            else put_string(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 210, DRAW_DIALOGBOX_GUILDMENU59, Color(4, 0, 50));

            if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 208) && (msY < sY + iAdjY + 225))
                put_string(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 210, DRAW_DIALOGBOX_GUILDMENU61, Color(255, 255, 255));
            else put_string(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 210, DRAW_DIALOGBOX_GUILDMENU61, Color(4, 0, 50));

            if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 228) && (msY < sY + iAdjY + 245))
                put_string(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 230, DRAW_DIALOGBOX_GUILDMENU63, Color(255, 255, 255));
            else put_string(sX + iAdjX + 65 + 25 - 23, sY + iAdjY + 230, DRAW_DIALOGBOX_GUILDMENU63, Color(4, 0, 50));

            if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 228) && (msY < sY + iAdjY + 245))
                put_string(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 230, DRAW_DIALOGBOX_GUILDMENU65, Color(255, 255, 255));
            else put_string(sX + iAdjX + 150 + 25 - 23, sY + iAdjY + 230, DRAW_DIALOGBOX_GUILDMENU65, Color(4, 0, 50));

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;
            
        case 14:
            
            put_aligned_string(sX, sX + szX, sY + 130, DRAW_DIALOGBOX_GUILDMENU66, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_GUILDMENU67, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 160, DRAW_DIALOGBOX_GUILDMENU68, 55, 25, 25);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
            
        case 15:
            
            put_aligned_string(sX, sX + szX, sY + 135, DRAW_DIALOGBOX_GUILDMENU69, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_GUILDMENU70, 55, 25, 25);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
            
        case 16:
            
            put_aligned_string(sX, sX + szX, sY + 135, DRAW_DIALOGBOX_GUILDMENU71, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_GUILDMENU72, 55, 25, 25);
            
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
            
        case 17:
            
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU73, 55, 25, 25);

            
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 18:
            
            
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU74, 55, 25, 25);
            break;

        case 19:
            
            
            if (m_iFightzoneNumber > 0)
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET, NULL, NULL, NULL, NULL, NULL);
            m_stDialogBoxInfo[7].cMode = 0;
            break;

        case 20:
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_GUILDMENU75, 55, 25, 25);
            put_string(sX + 75, sY + 144, "____________________", Color(25, 35, 25));
            put_string(sX + 75, sY + 140, m_cGuildName, Color(255, 255, 255), false, 2);
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 25);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 24);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;

        case 21:
            
            put_aligned_string(sX, sX + szX, sY + iAdjY + 95, DRAW_DIALOGBOX_GUILDMENU76, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 110, DRAW_DIALOGBOX_GUILDMENU77, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 135, DRAW_DIALOGBOX_GUILDMENU78, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 150, DRAW_DIALOGBOX_GUILDMENU79, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + iAdjY + 165, DRAW_DIALOGBOX_GUILDMENU80, 55, 25, 25);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 22:
            
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_GUILDMENU81, 55, 25, 25);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
    }
}


void CGame::DrawDialogBox_GuildOperation(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[8].sX;
    sY = m_stDialogBoxInfo[8].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 19);

    switch (m_stGuildOpList[0].cOpMode)
    {
        case 1:
            
            put_aligned_string(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION1);
            
            put_aligned_string(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            put_aligned_string(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            put_aligned_string(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION2);
            
            put_aligned_string(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION3);
            
            put_aligned_string(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_GUILD_OPERATION4);
            

            put_aligned_string(sX + 24, sX + 248, sY + 160, DRAW_DIALOGBOX_GUILD_OPERATION5, 55, 25, 25);
            

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 33);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 32);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 35);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 34);
            break;

        case 2:
            
            put_aligned_string(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION6);
            
            put_aligned_string(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            put_aligned_string(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            put_aligned_string(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION7);
            
            put_aligned_string(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION8);
            
            put_aligned_string(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_GUILD_OPERATION9);
            

            put_aligned_string(sX + 24, sX + 248, sY + 160, DRAW_DIALOGBOX_GUILD_OPERATION10, 55, 25, 25);
            

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 33);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 32);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 35);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 34);
            break;

        case 3:
            
            put_aligned_string(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION11);
            
            put_aligned_string(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            put_aligned_string(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            put_aligned_string(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION12);
            
            put_aligned_string(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION13);
            

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 4:
            
            put_aligned_string(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION14);
            
            put_aligned_string(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            put_aligned_string(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            put_aligned_string(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION15);
            
            put_aligned_string(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION16);
            

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 5:
            
            put_aligned_string(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION17);
            
            put_aligned_string(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            put_aligned_string(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            put_aligned_string(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION18);
            
            put_aligned_string(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION19);
            
            put_aligned_string(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_GUILD_OPERATION20);
            

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 6:
            
            put_aligned_string(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION21);
            
            put_aligned_string(sX + 24, sX + 248, sY + 65, m_stGuildOpList[0].cName, 35, 35, 35);
            put_aligned_string(sX + 24, sX + 248, sY + 69, "____________________", 0, 0, 0);
            put_aligned_string(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION22);
            
            put_aligned_string(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION23);
            

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 7:
            
            put_aligned_string(sX + 24, sX + 248, sY + 50, DRAW_DIALOGBOX_GUILD_OPERATION24);
            
    //PutString(sX + 60, sY + 65, m_stGuildOpList[0].cName, Color(35,35,35));
    //PutString(sX + 60, sY + 69, "____________________", Color(0,0,0));
            put_aligned_string(sX + 24, sX + 248, sY + 90, DRAW_DIALOGBOX_GUILD_OPERATION25);
            
            put_aligned_string(sX + 24, sX + 248, sY + 105, DRAW_DIALOGBOX_GUILD_OPERATION26);
            
            put_aligned_string(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_GUILD_OPERATION27);
            

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
    }
}

int  _tmp_iMCProb[] = { 0, 300, 250, 200, 150, 100, 80, 70, 60, 50, 40 };
int  _tmp_iMLevelPenalty[] = { 0,   5,   5,   8,   8,   10, 14, 28, 32, 36, 40 };

void CGame::DrawDialogBox_Magic(short msX, short msY, short msZ)
{
    short sX, sY, sMagicCircle, sLevelMagic;
    int  iCPivot, i, iYloc, iResult, iManaCost;
    char cTxt[120], cMana[10];
    DWORD dwTime = m_dwCurTime;
    double dV1, dV2, dV3, dV4;

    sX = m_stDialogBoxInfo[3].sX;
    sY = m_stDialogBoxInfo[3].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME1, sX, sY, 1, FALSE, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 7, FALSE, m_bDialogTrans);

    if (iGetTopDialogBoxIndex() == 3 && msZ != 0)
    {
        if (msZ > 0) m_stDialogBoxInfo[3].sView--;
        if (msZ < 0) m_stDialogBoxInfo[3].sView++;
        m_stMCursor.sZ = 0;
    }
    if (m_stDialogBoxInfo[3].sView < 0) m_stDialogBoxInfo[3].sView = 9;
    if (m_stDialogBoxInfo[3].sView > 9) m_stDialogBoxInfo[3].sView = 0;

    //Circle
    ZeroMemory(cTxt, sizeof(cTxt));
    switch (m_stDialogBoxInfo[3].sView)
    {
        case 0: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC1);  break;//"Circle One"
        case 1: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC2);  break;//"Circle Two"
        case 2: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC3);  break;//"Circle Three"
        case 3: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC4);  break;//"Circle Four"
        case 4: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC5);  break;//"Circle Five"
        case 5: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC6);  break;//"Circle Six"
        case 6: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC7);  break;//"Circle Seven"
        case 7: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC8);  break;//"Circle Eight"
        case 8: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC9);  break;//"Circle Nine"   
        case 9: strcpy(cTxt, DRAW_DIALOGBOX_MAGIC10); break;//"Circle Ten"
    }
    put_aligned_string(sX + 3, sX + 256, sY + 50, cTxt);
    put_aligned_string(sX + 4, sX + 257, sY + 50, cTxt);

    
    iCPivot = m_stDialogBoxInfo[3].sView * 10;
    iYloc = 0;

    for (i = 0; i < 9; i++)
    {
        if ((m_cMagicMastery[iCPivot + i] != NULL) && (m_pMagicCfgList[iCPivot + i] != NULL))
        {
            
            format_to_local(cTxt, "{}", m_pMagicCfgList[iCPivot + i]->m_cName);

            m_Misc.ReplaceString(cTxt, '-', ' ');
            iManaCost = iGetManaCost(iCPivot + i);
            if (iManaCost > m_iMP)
            {
                if (m_Misc.bCheckIMEString(cTxt) == FALSE)
                {
                    put_string(sX + 30, sY + 73 + iYloc, cTxt, Color(41, 16, 41));
                    put_string(sX + 31, sY + 73 + iYloc, cTxt, Color(41, 16, 41));
                }
                else put_string_sprite_font(sX + 30, sY + 70 + iYloc, cTxt, 5, 5, 5);
                format_to_local(cMana, "{:3}", iManaCost);
                put_string_sprite_font(sX + 206, sY + 70 + iYloc, cMana, 5, 5, 5);
            }
            else
                if ((msX >= sX + 30) && (msX <= sX + 240) && (msY >= sY + 70 + iYloc) && (msY <= sY + 70 + 14 + iYloc))
                {
                    if (m_Misc.bCheckIMEString(cTxt) == FALSE)
                    {
                        put_string(sX + 30, sY + 73 + iYloc, cTxt, Color(255, 255, 255));
                        put_string(sX + 31, sY + 73 + iYloc, cTxt, Color(255, 255, 255));
                    }
                    else put_string_sprite_font(sX + 30, sY + 70 + iYloc, cTxt, 250, 250, 250);
                    format_to_local(cMana, "{:3}", iManaCost);
                    put_string_sprite_font(sX + 206, sY + 70 + iYloc, cMana, 250, 250, 250);
                }
                else
                {
                    if (m_Misc.bCheckIMEString(cTxt) == FALSE)
                    {
                        put_string(sX + 30, sY + 73 + iYloc, cTxt, Color(8, 0, 66));
                        put_string(sX + 31, sY + 73 + iYloc, cTxt, Color(8, 0, 66));
                    }
                    else put_string_sprite_font(sX + 30, sY + 70 + iYloc, cTxt, 1, 1, 8);
                    format_to_local(cMana, "{:3}", iManaCost);
                    put_string_sprite_font(sX + 206, sY + 70 + iYloc, cMana, 1, 1, 8);
                }

            iYloc += 18;
        }

    }

    if (iYloc == 0)
    {
        
        put_aligned_string(sX + 3, sX + 256, sY + 100, DRAW_DIALOGBOX_MAGIC11);
        put_aligned_string(sX + 3, sX + 256, sY + 115, DRAW_DIALOGBOX_MAGIC12);
        put_aligned_string(sX + 3, sX + 256, sY + 130, DRAW_DIALOGBOX_MAGIC13);
        put_aligned_string(sX + 3, sX + 256, sY + 145, DRAW_DIALOGBOX_MAGIC14);
        put_aligned_string(sX + 3, sX + 256, sY + 160, DRAW_DIALOGBOX_MAGIC15);
    }

   
    m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 30, sY + 250, 19, dwTime);

    switch (m_stDialogBoxInfo[3].sView)
    {
        case 0: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 30, sY + 250, 20, dwTime); break;
        case 1: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 43, sY + 250, 21, dwTime); break;
        case 2: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 61, sY + 250, 22, dwTime); break;
        case 3: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 86, sY + 250, 23, dwTime); break;
        case 4: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 106, sY + 250, 24, dwTime); break;
        case 5: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 121, sY + 250, 25, dwTime); break;
        case 6: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 142, sY + 250, 26, dwTime); break;
        case 7: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 169, sY + 250, 27, dwTime); break;
        case 8: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 202, sY + 250, 28, dwTime); break;
        case 9: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 222, sY + 250, 29, dwTime); break;
    }

    
    sMagicCircle = m_stDialogBoxInfo[3].sView + 1;
    if (m_cSkillMastery[4] == 0)
        dV1 = 1.0f;
    else dV1 = (double)m_cSkillMastery[4];

    dV2 = (double)(dV1 / 100.0f);
    dV3 = (double)_tmp_iMCProb[sMagicCircle];

    dV1 = dV2 * dV3;
    iResult = (int)dV1;

    
    if (m_iInt > 50) iResult += (m_iInt - 50) / 2;

    
    sLevelMagic = (m_iLevel / 10);
    if (sMagicCircle != sLevelMagic)
    {
        if (sMagicCircle > sLevelMagic)
        {
            
            dV1 = (double)(m_iLevel - sLevelMagic * 10);
            dV2 = (double)abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle];
            dV3 = (double)abs(sMagicCircle - sLevelMagic) * 10;
            dV4 = (dV1 / dV3) * dV2;

            iResult -= abs(abs(sMagicCircle - sLevelMagic) * _tmp_iMLevelPenalty[sMagicCircle] - (int)dV4);
        }
        else
        {
            
            iResult += 5 * abs(sMagicCircle - sLevelMagic);
        }
    }

    
    switch (m_cWhetherStatus)
    {
        case 0: break;
        case 1: iResult = iResult - (iResult / 24); break; 
        case 2:	iResult = iResult - (iResult / 12); break;
        case 3: iResult = iResult - (iResult / 5);  break;
    }
    
    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if (m_pItemList[i] == NULL) continue;
        if (m_bIsItemEquipped[i] == TRUE)
        {
            if (((m_pItemList[i]->m_dwAttribute & 0x00F00000) >> 20) == 10)
            {
                dV1 = (double)iResult;
                dV2 = (double)(((m_pItemList[i]->m_dwAttribute & 0x000F0000) >> 16) * 3);
                dV3 = dV1 + dV2;
                iResult = (int)dV3;
                break;
            }
        }
    }

    
    if (iResult > 100) iResult = 100;
    if (m_iSP < 1) iResult = iResult * 9 / 10;
    if (iResult < 1) iResult = 1;

    ZeroMemory(cTxt, sizeof(cTxt));
    format_to_local(cTxt, DRAW_DIALOGBOX_MAGIC16, iResult);
    put_aligned_string(sX, sX + 256, sY + 267, cTxt);
    put_aligned_string(sX + 1, sX + 257, sY + 267, cTxt);

   
    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 285) && (msY <= sY + 285 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + 285, 49, FALSE, m_bDialogTrans);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + 285, 48, FALSE, m_bDialogTrans);
}

void CGame::DrawDialogBox_MagicShop(short msX, short msY, short msZ)
{
    short sX, sY;
    DWORD dwTime = m_dwCurTime;
    int  i;

    int  iCPivot, iYloc;
    char cTxt[120], cMana[10];

    sX = m_stDialogBoxInfo[16].sX;
    sY = m_stDialogBoxInfo[16].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX, sY, 1);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 14);

    if (iGetTopDialogBoxIndex() == 16 && msZ != 0)
    {
        if (msZ > 0) m_stDialogBoxInfo[16].sView--;
        if (msZ < 0) m_stDialogBoxInfo[16].sView++;
        m_stMCursor.sZ = 0;
    }
    if (m_stDialogBoxInfo[16].sView < 0) m_stDialogBoxInfo[16].sView = 9;
    if (m_stDialogBoxInfo[16].sView > 9) m_stDialogBoxInfo[16].sView = 0;

    put_string(sX - 20 + 60 - 17, sY - 35 + 90, DRAW_DIALOGBOX_MAGICSHOP11, Color(45, 25, 25));//"Spell Name"
    put_string(sX - 20 + 232 - 20, sY - 35 + 90, DRAW_DIALOGBOX_MAGICSHOP12, Color(45, 25, 25));//"Req.Int"
    put_string(sX - 20 + 270, sY - 35 + 90, DRAW_DIALOGBOX_MAGICSHOP13, Color(45, 25, 25));//"Cost"

    
    iCPivot = m_stDialogBoxInfo[16].sView * 10;

    iYloc = 0;
    for (i = 0; i < 9; i++)
    {
        if ((m_pMagicCfgList[iCPivot + i] != NULL) && (m_pMagicCfgList[iCPivot + i]->m_bIsVisible))
        {
            
            format_to_local(cTxt, "{}", m_pMagicCfgList[iCPivot + i]->m_cName);

            m_Misc.ReplaceString(cTxt, '-', ' ');
            if (m_cMagicMastery[iCPivot + i] != 0)
            {
                if (m_Misc.bCheckIMEString(cTxt) == FALSE)
                {
                    put_string(sX + 24, sY + 73 + iYloc, cTxt, Color(41, 16, 41));
                    put_string(sX + 25, sY + 73 + iYloc, cTxt, Color(41, 16, 41));
                }
                else put_string_sprite_font(sX + 24, sY + 70 + iYloc, cTxt, 5, 5, 5);
                format_to_local(cMana, "{:3}", m_pMagicCfgList[iCPivot + i]->m_sValue2);
                put_string_sprite_font(sX + 200, sY + 70 + iYloc, cMana, 5, 5, 5);
                format_to_local(cMana, "{:3}", m_pMagicCfgList[iCPivot + i]->m_sValue3);
                put_string_sprite_font(sX + 241, sY + 70 + iYloc, cMana, 5, 5, 5);
            }
            else
                if ((msX >= sX + 24) && (msX <= sX + 24 + 135) && (msY >= sY + 70 + iYloc) && (msY <= sY + 70 + 14 + iYloc))
                {
                    if (m_Misc.bCheckIMEString(cTxt) == FALSE)
                    {
                        put_string(sX + 24, sY + 73 + iYloc, cTxt, Color(255, 255, 255));
                        put_string(sX + 25, sY + 73 + iYloc, cTxt, Color(255, 255, 255));
                    }
                    else put_string_sprite_font(sX - 20 + 44, sY + 70 + iYloc, cTxt, 250, 250, 250);
                    format_to_local(cMana, "{:3}", m_pMagicCfgList[iCPivot + i]->m_sValue2);
                    put_string_sprite_font(sX - 20 + 220, sY + 70 + iYloc, cMana, 250, 250, 250);
                    format_to_local(cMana, "{:3}", m_pMagicCfgList[iCPivot + i]->m_sValue3);
                    put_string_sprite_font(sX - 20 + 261, sY + 70 + iYloc, cMana, 250, 250, 250);
                }
                else
                {
                    if (m_Misc.bCheckIMEString(cTxt) == FALSE)
                    {
                        put_string(sX + 24, sY + 73 + iYloc, cTxt, Color(8, 0, 66));
                        put_string(sX + 25, sY + 73 + iYloc, cTxt, Color(8, 0, 66));
                    }
                    else put_string_sprite_font(sX - 20 + 44, sY + 70 + iYloc, cTxt, 1, 1, 8);
                    format_to_local(cMana, "{:3}", m_pMagicCfgList[iCPivot + i]->m_sValue2);
                    put_string_sprite_font(sX - 20 + 220, sY + 70 + iYloc, cMana, 1, 1, 8);
                    format_to_local(cMana, "{:3}", m_pMagicCfgList[iCPivot + i]->m_sValue3);
                    put_string_sprite_font(sX - 20 + 261, sY + 70 + iYloc, cMana, 1, 1, 8);
                }
            iYloc += 18;
        }
    }

    m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX + 55, sY + 250, 19, dwTime);

    switch (m_stDialogBoxInfo[16].sView)
    {
        case 0: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 44 + 31, sY + 250, 20, dwTime); break;
        case 1: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 57 + 31, sY + 250, 21, dwTime); break;
        case 2: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 75 + 31, sY + 250, 22, dwTime); break;
        case 3: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 100 + 31, sY + 250, 23, dwTime); break;
        case 4: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 120 + 31, sY + 250, 24, dwTime); break;
        case 5: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 135 + 31, sY + 250, 25, dwTime); break;
        case 6: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 156 + 31, sY + 250, 26, dwTime); break;
        case 7: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 183 + 31, sY + 250, 27, dwTime); break;
        case 8: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 216 + 31, sY + 250, 28, dwTime); break;
        case 9: m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS]->put_sprite_fast(sX - 20 + 236 + 31, sY + 250, 29, dwTime); break;
    }

    put_aligned_string(sX, sX + m_stDialogBoxInfo[16].sSizeX, sY + 275, DRAW_DIALOGBOX_MAGICSHOP14, 45, 25, 25);
}

void CGame::DrawDialogBox_GuideMap(short msX, short msY, char cLB)
{
    if (m_cMapIndex < 0) return;
    short sX, sY, shX, shY, szX, szY;
    sX = m_stDialogBoxInfo[9].sX;
    sY = m_stDialogBoxInfo[9].sY;
    szX = m_stDialogBoxInfo[9].sSizeX;
    szY = m_stDialogBoxInfo[9].sSizeY;
    if (sX < 20) sX = 0;
    if (sY < 20) sY = 0;
    if (sX > 640 - 128 - 20) sX = 640 - 128;
    if (sY > 427 - 128 - 20) sY = 427 - 128;
    for (shX = -2; shX < 130; shX++)
    {
        put_pixel(sX + shX, sY - 2, 50, 50, 50);
        put_pixel(sX + shX, sY - 1, 50, 50, 50);
        put_pixel(sX + shX, sY + 128, 50, 50, 50);
        put_pixel(sX + shX, sY + 129, 50, 50, 50);
    }
    for (shY = -2; shY < 130; shY++)
    {
        put_pixel(sX - 2, sY + shY, 50, 50, 50);
        put_pixel(sX - 1, sY + shY, 50, 50, 50);
        put_pixel(sX + 128, sY + shY, 50, 50, 50);
        put_pixel(sX + 129, sY + shY, 50, 50, 50);
    }
    if (m_bZoomMap)
    {
        shX = m_sPlayerX - 64;
        shY = m_sPlayerY - 64;
        if (shX < 0) shX = 0;
        if (shY < 0) shY = 0;
        if (shX > m_pMapData->m_sMapSizeX - 128) shX = m_pMapData->m_sMapSizeX - 128;
        if (shY > m_pMapData->m_sMapSizeY - 128) shY = m_pMapData->m_sMapSizeY - 128;
        if (m_bDialogTrans) m_pSprite[DEF_SPRID_INTERFACE_GUIDEMAP + m_cMapIndex + 1]->put_shift_trans_sprite2(sX, sY, shX, shY, 0, m_dwCurTime);
        else m_pSprite[DEF_SPRID_INTERFACE_GUIDEMAP + m_cMapIndex + 1]->put_shift_sprite_fast(sX, sY, shX, shY, 0, m_dwCurTime);
        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX - shX + m_sPlayerX, sY - shY + m_sPlayerY, 37, m_dwCurTime); 
        
        if ((m_dwCurTime - m_dwMonsterEventTime) < 30000)
        {
            if ((m_dwCurTime % 500) < 370)
            {
                if (m_sEventX >= shX && m_sEventX <= shX + 128 && m_sEventY >= shY && m_sEventY <= shY + 128)
                    m_pSprite[DEF_SPRID_INTERFACE_MONSTER]->put_sprite_fast(sX + m_sEventX - shX, sY + m_sEventY - shY, m_sMonsterID, m_dwCurTime);
            }
        }
        else
        {
            m_dwMonsterEventTime = 0;
            m_sMonsterID = 0;
        }
    }
    else
    {
        if (m_bDialogTrans) m_pSprite[DEF_SPRID_INTERFACE_GUIDEMAP]->put_trans_sprite2(sX, sY, m_cMapIndex, m_dwCurTime);
        else m_pSprite[DEF_SPRID_INTERFACE_GUIDEMAP]->put_sprite_fast_no_color_key(sX, sY, m_cMapIndex, m_dwCurTime);
        shX = (m_sPlayerX * 128) / (m_pMapData->m_sMapSizeX);
        shY = (m_sPlayerY * 128) / (m_pMapData->m_sMapSizeX);
        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + shX, sY + shY, 37, m_dwCurTime); 

        if ((G_dwGlobalTime - m_dwMonsterEventTime) < 30000)
        {
            if ((m_dwCurTime % 500) < 370)
            {
                shX = (m_sEventX * 128) / (m_pMapData->m_sMapSizeX);
                shY = (m_sEventY * 128) / (m_pMapData->m_sMapSizeX);
                m_pSprite[DEF_SPRID_INTERFACE_MONSTER]->put_sprite_fast(sX + shX, sY + shY, m_sMonsterID, m_dwCurTime);
            }
        }
        else
        {
            m_dwMonsterEventTime = 0;
            m_sMonsterID = 0;
        }
    }

    if (cLB != 0) return;
    if (msX >= sX && msX < sX + szY && msY >= sY && msY < sY + szY)
    {
        if (sY > 213) shY = sY - 17;
        else shY = sY + szY + 4;
        if (m_bZoomMap) put_string(sX, shY, DEF_MSG_GUIDEMAP_MIN, Color(200, 200, 120));
        else put_string(sX, shY, DEF_MSG_GUIDEMAP_MAX, Color(200, 200, 120));

        if (m_bZoomMap)
        {
            shX = m_sPlayerX - 64;
            shY = m_sPlayerY - 64;
            if (shX < 0) shX = 0;
            if (shY < 0) shY = 0;
            if (shX > m_pMapData->m_sMapSizeX - 128) shX = m_pMapData->m_sMapSizeX - 128;
            if (shY > m_pMapData->m_sMapSizeY - 128) shY = m_pMapData->m_sMapSizeY - 128;
            shX += msX - sX;
            shY += msY - sY;
        }
        else
        {
            shX = (msX - sX) * m_pMapData->m_sMapSizeX / 128;
            shY = (msY - sY) * m_pMapData->m_sMapSizeX / 128;
        }
        std::string output = std::format("({}, {}) ", shX, shY);
        if (m_cMapIndex == 11)
        {
            if (shX > 165 && shX < 225 && shY > 136 && shY < 175) output += DEF_MSG_MAPNAME_CITYHALL;
            else if (shX > 47 && shX < 76 && shY > 134 && shY < 165) output += DEF_MSG_MAPNAME_MAGICTOWER;
            else if (shX > 99 && shX < 124 && shY > 204 && shY < 227) output += DEF_MSG_MAPNAME_DUNGEON;
            else if (shX > 125 && shX < 157 && shY > 185 && shY < 218) output += DEF_MSG_MAPNAME_WAREHOUSE;
            else if (shX > 147 && shX < 184 && shY >  65 && shY < 97) output += DEF_MSG_MAPNAME_BARRACK;
            else if (shX > 155 && shX < 185 && shY > 110 && shY < 136) output += DEF_MSG_MAPNAME_GUILDHALL;
            else if (shX > 166 && shX < 193 && shY > 185 && shY < 211) output += DEF_MSG_MAPNAME_SHOP;
            else if (shX > 175 && shX < 202 && shY >  97 && shY < 120) output += DEF_MSG_MAPNAME_DUNGEON;
            else if (shX > 201 && shX < 229 && shY > 220 && shY < 245) output += DEF_MSG_MAPNAME_BLACKSMITH;
            else if (shX > 205 && shX < 249 && shY >  98 && shY < 140) output += DEF_MSG_MAPNAME_CATH;
            else if (shX > 262 && shX < 290 && shY > 148 && shY < 178) output += DEF_MSG_MAPNAME_WAREHOUSE;
        }
        else if (m_cMapIndex == 3) 
        {
            if (shX > 77 && shX < 114 && shY >  81 && shY < 114) output += DEF_MSG_MAPNAME_MAGICTOWER;
            else if (shX > 88 && shX < 120 && shY > 151 && shY < 183) output += DEF_MSG_MAPNAME_GUILDHALL;
            else if (shX > 126 && shX < 171 && shY >  97 && shY < 141) output += DEF_MSG_MAPNAME_CATH;
            else if (shX > 157 && shX < 194 && shY > 150 && shY < 190) output += DEF_MSG_MAPNAME_CITYHALL;
            else if (shX > 171 && shX < 207 && shY >  76 && shY < 107) output += DEF_MSG_MAPNAME_BARRACK;
            else if (shX > 207 && shX < 231 && shY >  99 && shY < 124) output += DEF_MSG_MAPNAME_DUNGEON;
            else if (shX > 247 && shX < 277 && shY > 139 && shY < 170) output += DEF_MSG_MAPNAME_WAREHOUSE;
            else if (shX > 237 && shX < 270 && shY > 225 && shY < 258) output += DEF_MSG_MAPNAME_WAREHOUSE;
            else if (shX > 258 && shX < 287 && shY > 109 && shY < 137) output += DEF_MSG_MAPNAME_SHOP;
            else if (shX > 302 && shX < 333 && shY > 147 && shY < 175) output += DEF_MSG_MAPNAME_BLACKSMITH;
            else if (shX > 301 && shX < 330 && shY > 239 && shY < 265) output += DEF_MSG_MAPNAME_DUNGEON;
        }
        else if (m_cMapIndex == 5) 
        {
            if (shX > 62 && shX < 78 && shY >  178 && shY < 192) output += DEF_MSG_MAPNAME_WAREHOUSE;
            else if (shX > 82 && shX < 95 && shY > 163 && shY < 174) output += DEF_MSG_MAPNAME_SHOP;
            else if (shX > 107 && shX < 122 && shY > 177 && shY < 189) output += DEF_MSG_MAPNAME_BLACKSMITH;
        }
        else if (m_cMapIndex == 6) 
        {
            if (shX > 35 && shX < 48 && shY >  70 && shY < 85) output += DEF_MSG_MAPNAME_WAREHOUSE;
            else if (shX > 55 && shX < 73 && shY > 77 && shY < 90) output += DEF_MSG_MAPNAME_BLACKSMITH;
            else if (shX > 53 && shX < 66 && shY > 53 && shY < 65) output += DEF_MSG_MAPNAME_SHOP;
        }
        put_string(msX - 10, msY - 13, output, Color(200, 200, 120));
    }
}

void CGame::DrawDialogBoxs(short msX, short msY, short msZ, char cLB)
{
    int i;

    
    if (m_bIsObserverMode == TRUE) return;
    m_stMCursor.sZ = 0;

    for (i = 0; i < 41; i++)
        if (m_cDialogBoxOrder[i] != NULL)
        {
            
            switch (m_cDialogBoxOrder[i])
            {
                case 1:
                    DrawDialogBox_Character(msX, msY); 
                    break;
                case 2:
                    DrawDialogBox_Inventory(msX, msY); 
                    break;
                case 3:
                    DrawDialogBox_Magic(msX, msY, msZ); 
                    break;
                case 4:
                    DrawDialogBox_ItemDrop(msX, msY); 
                    break;
                case 5:
                    DrawDialogBox_15AgeMsg(msX, msY); 
                    break;
                case 6:
                    DrawDialogBox_WarningMsg(msX, msY); 
                    break;
                case 7:
                    DrawDialogBox_GuildMenu(msX, msY);
                    break;
                case 8:
                    DrawDialogBox_GuildOperation(msX, msY);
                    break;
                case 9:
                    DrawDialogBox_GuideMap(msX, msY, cLB);
                    break;
                case 10:
                    DrawDialogBox_Chat(msX, msY, msZ, cLB); 
                    break;
                case 11:
                    DrawDialogBox_Shop(msX, msY, msZ, cLB); 
                    break;
                case 12:
                    DrawDialogBox_LevelUpSetting(msX, msY); 
                    break;
                case 13:
                    DrawDialogBox_CityHallMenu(msX, msY);
                    break;
                case 14:
                    DrawDialogBox_Bank(msX, msY, msZ, cLB); 
                    break;
                case 15:
                    DrawDialogBox_Skill(msX, msY, msZ, cLB); 
                    break;
                case 16:
                    DrawDialogBox_MagicShop(msX, msY, msZ); 
                    break;
                case 17:
                    DrawDialogBox_QueryDropItemAmount();
                    break;
                case 18:
                    DrawDialogBox_Text(msX, msY, msZ, cLB); 
                    break;
                case 19:
                    DrawDialogBox_SysMenu(msX, msY, cLB); 
                    break;
                case 20:
                    DrawDialogBox_NpcActionQuery(msX, msY); 
                    break;
                case 21:
                    DrawDialogBox_NpcTalk(msX, msY, cLB); 
                    break;
                case 22:
                    DrawDialogBox_Map();
                    break;
                case 23:
                    DrawDialogBox_SellorRepairItem(msX, msY); 
                    break;
                case 24:
                    DrawDialogBox_Fishing(msX, msY);
                    break;
                case 25:
                    DrawDialogBox_ShutDownMsg(msX, msY); 
                    break;
                case 26:
                    DrawDialogBox_SkillDlg(msX, msY, msZ, cLB);
                    break;
                case 27:
                    DrawDialogBox_Exchange(msX, msY); 
                    break;
                case 28:
                    DrawDialogBox_Quest(msX, msY); 
                    break;
                case 29:
                    DrawDialogBox_GaugePannel(); 
                    break;
                case 30:
                    DrawDialogBox_IconPannel(msX, msY); 
                    break;
                case 31:
                    DrawDialogBox_SellList(msX, msY); 
                    break;
                case 32:
                    DrawDialogBox_Party(msX, msY); 
                    break;
                case 33:
                    DrawDialogBox_CrusadeJob(msX, msY); 
                    break;
                case 34:
                    DrawDialogBox_ItemUpgrade(msX, msY);
                    break;
                case 35:
                    DrawDialogBox_Help(msX, msY); 
                    break;
                case 36:
                    DrawDialogBox_Commander(msX, msY); 
                    break;
                case 37:
                    DrawDialogBox_Constructor(msX, msY); 
                    break;
                case 38:
                    DrawDialogBox_Soldier(msX, msY); 
                    break;
            }
        }

    if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
    {
        
        if (m_iSuperAttackLeft > 0)
        {
            if (GetAsyncKeyState(VK_MENU) >> 15)
                m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_trans_sprite(368, 439, 3, m_dwCurTime);

            format_to_local(G_cTxt, "{}", m_iSuperAttackLeft);
            put_string_sprite_font2(380, 454, G_cTxt, 220, 200, 200);
        }
    }
    else
    {
        if (m_iSuperAttackLeft > 0)
        {
            format_to_local(G_cTxt, "{}", m_iSuperAttackLeft);
            put_string_sprite_font2(380, 454, G_cTxt, 10, 10, 10);
        }
    }
}

void CGame::DrawDialogBox_IconPannel(short msX, short msY)
{
    short sX, sY;
    DWORD dwTime = m_dwCurTime;

    sX = m_stDialogBoxInfo[30].sX;
    sY = m_stDialogBoxInfo[30].sY;

    m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX, sY, 14, dwTime);

    if ((362 < msX) && (404 > msX) && (434 < msY) && (475 > msY))
    {
        m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(362, 434, 16, dwTime);
    }

    if (m_bIsCombatMode)
    {
        if (m_bIsSafeAttackMode)
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(368, 440, 4, dwTime);
        else m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(368, 440, 5, dwTime);
    }

    if ((m_bIsCrusadeMode) && (m_iCrusadeDuty != 0))
    {
        if (m_bAresden == TRUE)
        {
            if ((322 <= msX) && (355 >= msX) && (434 < msY) && (475 > msY))
                m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(322, 434, 1, dwTime);
            else m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(322, 434, 2, dwTime);
        }
        else if (m_bAresden == FALSE)
        {
            if ((322 <= msX) && (355 >= msX) && (434 < msY) && (475 > msY))
                m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(322, 434, 0, dwTime);
            else m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(322, 434, 15, dwTime);
        }
    }

    if ((msY > 436) && (msY < 478))
    {
        if ((msX > 410) && (msX < 447))
        {
#if DEF_LANGUAGE == 3		// Korea 2.19
            if (m_iLevel < 80) put_aligned_string(360, 497, 410, "Ä³¸¯ÅÍ ÀÎÆ÷(F5)", 200, 200, 120);
#endif
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(412, 434, 6, dwTime);// Character
        }
        if ((msX > 447) && (msX < 484))
        {
#if DEF_LANGUAGE == 3		// Korea 2.19
            if (m_iLevel < 80) put_aligned_string(397, 534, 410, "ÀÎº¥Åä¸®(F6)", 200, 200, 120);
#endif
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(449, 434, 7, dwTime);// Inventory
        }
        if ((msX > 484) && (msX < 521))
        {
#if DEF_LANGUAGE == 3		// Korea 2.19
            if (m_iLevel < 80) put_aligned_string(434, 571, 410, "¸¶¹ý¸®½ºÆ®(F7)", 200, 200, 120);
#endif
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(486, 434, 8, dwTime);// Magic
        }
        if ((msX > 521) && (msX < 558))
        {
#if DEF_LANGUAGE == 3		// Korea 2.19
            if (m_iLevel < 80) put_aligned_string(471, 608, 410, "½ºÅ³Á¤º¸(F8)", 200, 200, 120);
#endif
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(523, 434, 9, dwTime);// Skill
        }
        if ((msX > 558) && (msX < 595))
        {
#if DEF_LANGUAGE == 3		// Korea 2.19
            if (m_iLevel < 80) put_aligned_string(508, 645, 410, "Ã¤ÆÃÈ÷½ºÅä¸®(F9)", 200, 200, 120);
#endif
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(560, 434, 10, dwTime);// History
        }
        if ((msX > 595) && (msX < 631))
        {
#if DEF_LANGUAGE == 3		// Korea 2.19
            if (m_iLevel < 80) put_aligned_string(530, 640, 410, "½Ã½ºÅÛ ¸Þ´º(F12)", 200, 200, 120);
#endif
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(597, 434, 11, dwTime);// System Menu
        }
    }

    std::string output;

    if (((msX > 144) && (msX < 317) && (msY > 434) && (msY < 477)) || (m_bCtrlPressed))
    {
        

        
        int iLev = 0;
        int iCurExp = iGetLevelExp(m_iLevel);
        int iNextExp;

        if (m_iLevel > 139)
        {
            iLev = (m_iLevel - 139) * 3;
            iNextExp = iGetLevelExp(m_iLevel + iLev);
        }
        else
            iNextExp = iGetLevelExp(m_iLevel + 1);

        if (m_iExp < iNextExp)
        {
            iNextExp = iNextExp - iCurExp;
            if (m_iExp > iCurExp) iCurExp = m_iExp - iCurExp;
            else iCurExp = 0;
            short sPerc = 0;
            if (iCurExp > 200000) sPerc = short(((iCurExp >> 4) * 10000) / (iNextExp >> 4));
            else sPerc = (short)((iCurExp * 10000) / iNextExp);
            output = std::format(DEF_MSG_EXP"{}/{}({}.{:2}%)", iNextExp - iCurExp, iNextExp, sPerc / 100, sPerc % 100);
        }
        else
        {
            output = std::format(DEF_MSG_EXP"{} / {}", m_iExp, iNextExp);
        }
    }
    else output = std::format("{}({},{})", m_cMapMessage, m_sPlayerX, m_sPlayerY);
    put_aligned_string(140, 323, 456, output, 200, 200, 120);
}

void CGame::DrawDialogBox_GaugePannel()
{
    int iMaxPoint, iBarWidth, iTemp;

    iMaxPoint = m_iVit * 3 + m_iLevel * 2 + m_iStr / 2;
    iBarWidth = 101 - (m_iHP * 101) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 101) iBarWidth = 101;
    m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast_width(23, 437, 12, iBarWidth, m_dwCurTime);
    iTemp = m_iHP;
    format_to_local(G_cTxt, "{}", iTemp);
    if (m_bIsPoisoned)
    {
        put_string_sprite_number(85, 441, G_cTxt, m_wR[5] * 11, m_wG[5] * 11, m_wB[5] * 11);
        put_string_sprite_font3(35, 440, "Poisoned", m_wR[5] * 8, m_wG[5] * 8, m_wB[5] * 8, TRUE, 2);
    }
    else put_string_sprite_number(85, 441, G_cTxt, 200, 100, 100);


    iMaxPoint = m_iMag * 2 + m_iLevel * 2 + m_iInt / 2;
    iBarWidth = 101 - (m_iMP * 101) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 101) iBarWidth = 101;
    m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast_width(23, 459, 12, iBarWidth, m_dwCurTime);
    iTemp = m_iMP;
    format_to_local(G_cTxt, "{}", iTemp);
    put_string_sprite_number(85, 463, G_cTxt, 100, 100, 200);


    iMaxPoint = m_iStr * 2 + m_iLevel * 2;
    iBarWidth = 167 - (m_iSP * 167) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 167) iBarWidth = 167;
    m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast_width(147, 435, 13, iBarWidth, m_dwCurTime);
}

void CGame::DrawDialogBox_Text(short msX, short msY, short msZ, char cLB)
{
    short sX, sY;
    int i, iTotalLines, iPointerLoc;
    double d1, d2, d3;

    sX = m_stDialogBoxInfo[18].sX;
    sY = m_stDialogBoxInfo[18].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);

    iTotalLines = 0;
    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
        if (m_pMsgTextList[i] != NULL) iTotalLines++;

    if (iTotalLines > 17) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 1);
    if (iGetTopDialogBoxIndex() == 18 && msZ != 0)
    {
        m_stDialogBoxInfo[18].sView = m_stDialogBoxInfo[18].sView - msZ / 60;
        m_stMCursor.sZ = 0;
    }
    if (m_stDialogBoxInfo[18].sView < 0) m_stDialogBoxInfo[18].sView = 0;
    if (iTotalLines > 17 && m_stDialogBoxInfo[18].sView > iTotalLines - 17) m_stDialogBoxInfo[18].sView = iTotalLines - 17;

    
    if (iTotalLines > 17)
    {
        d1 = (double)m_stDialogBoxInfo[18].sView;
        d2 = (double)(iTotalLines - 17);
        d3 = (274.0f * d1) / d2;
        iPointerLoc = (int)(d3 + 0.5);
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 1);
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242, sY + 35 + iPointerLoc, 7);
    }
    else iPointerLoc = 0;

    for (i = 0; i < 17; i++)
        if (m_pMsgTextList[i + m_stDialogBoxInfo[18].sView] != NULL)
        {
            if (m_bDialogTrans == FALSE)
            {
                switch (m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->message[0])
                {
                    case '_': put_aligned_string(sX + 24, sX + 236, sY + 50 + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->message.c_str() + 1), 255, 255, 255); break;
                    case ';': put_aligned_string(sX + 24, sX + 236, sY + 50 + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->message.c_str() + 1), 4, 0, 50); break;
                    default: put_aligned_string(sX + 24, sX + 236, sY + 50 + i * 13, m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->message, 45, 25, 25); break;
                }
            }
            else put_aligned_string(sX + 24, sX + 236, sY + 50 + i * 13, m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->message, 0, 0, 0);
        }

    
    if (cLB != 0 && iTotalLines > 17)
    {
        if ((iGetTopDialogBoxIndex() == 18))
        {
            if ((msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 40) && (msY <= sY + 320))
            {
                d1 = (double)(msY - (sY + 35));
                d2 = (double)(iTotalLines - 17);
                d3 = (d1 * d2) / 274.0f;
                iPointerLoc = (int)d3;
                if (iPointerLoc > iTotalLines - 17) iPointerLoc = iTotalLines - 17;
                m_stDialogBoxInfo[18].sView = iPointerLoc;
            }
        }
    }
    else m_stDialogBoxInfo[18].bIsScrollSelected = FALSE;

    if ((msX > sX + DEF_RBTNPOSX) && (msX < sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
}

void CGame::DrawDialogBox_ShutDownMsg(short msX, short msY)
{
    short sX, sY, szX;

    sX = m_stDialogBoxInfo[25].sX;
    sY = m_stDialogBoxInfo[25].sY;
    szX = m_stDialogBoxInfo[25].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX, sY, 2);

    switch (m_stDialogBoxInfo[25].cMode)
    {
        case 1:
            ZeroMemory(G_cTxt, sizeof(G_cTxt));
            if (m_stDialogBoxInfo[25].sV1 != 0) format_to_local(G_cTxt, DRAW_DIALOGBOX_NOTICEMSG1, m_stDialogBoxInfo[25].sV1);
            else strcpy(G_cTxt, DRAW_DIALOGBOX_NOTICEMSG2);
            put_aligned_string(sX, sX + szX, sY + 31, G_cTxt, 100, 10, 10);
            put_aligned_string(sX, sX + szX, sY + 48, DRAW_DIALOGBOX_NOTICEMSG3);
            put_aligned_string(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_NOTICEMSG4);
            put_aligned_string(sX, sX + szX, sY + 82, DRAW_DIALOGBOX_NOTICEMSG5);
            put_aligned_string(sX, sX + szX, sY + 99, DRAW_DIALOGBOX_NOTICEMSG6);
            if ((msX >= sX + 210) && (msX <= sX + 210 + DEF_BTNSZX) && (msY > sY + 127) && (msY < sY + 127 + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 210, sY + 127, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 210, sY + 127, 0);
            break;

        case 2:
            put_aligned_string(sX, sX + szX, sY + 31, DRAW_DIALOGBOX_NOTICEMSG7, 100, 10, 10);
            put_aligned_string(sX, sX + szX, sY + 48, DRAW_DIALOGBOX_NOTICEMSG8);
            put_aligned_string(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_NOTICEMSG9);
            put_aligned_string(sX, sX + szX, sY + 82, DRAW_DIALOGBOX_NOTICEMSG10);
            put_aligned_string(sX, sX + szX, sY + 99, DRAW_DIALOGBOX_NOTICEMSG11);
            if ((msX >= sX + 210) && (msX <= sX + 210 + DEF_BTNSZX) && (msY > sY + 127) && (msY < sY + 127 + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 210, sY + 127, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 210, sY + 127, 0);
            break;
    }
}

void CGame::DrawDialogBox_NpcActionQuery(short msX, short msY)
{
    short sX, sY, szX;

    char cTxt[120]{}, cTxt2[120]{}, cStr1[64]{}, cStr2[64]{}, cStr3[64]{};

    sX = m_stDialogBoxInfo[20].sX;
    sY = m_stDialogBoxInfo[20].sY;
    szX = m_stDialogBoxInfo[20].sSizeX;

    
    

    switch (m_stDialogBoxInfo[20].cMode)
    {
        case 0:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 5); 
            switch (m_stDialogBoxInfo[20].sV3)
            {
                case 15:
                    put_string(sX + 33, sY + 23, NPC_NAME_SHOP_KEEPER, Color(45, 25, 25));
                    put_string(sX + 33 - 1, sY + 23 - 1, NPC_NAME_SHOP_KEEPER, Color(255, 255, 255));
                    break;
                case 19:
                    put_string(sX + 33, sY + 23, NPC_NAME_MAGICIAN, Color(45, 25, 25));
                    put_string(sX + 33 - 1, sY + 23 - 1, NPC_NAME_MAGICIAN, Color(255, 255, 255));
                    break;
                case 20:
                    put_string(sX + 33, sY + 23, NPC_NAME_WAREHOUSE_KEEPER, Color(45, 25, 25));
                    put_string(sX + 33 - 1, sY + 23 - 1, NPC_NAME_WAREHOUSE_KEEPER, Color(255, 255, 255));
                    break;
                case 24:
                    put_string(sX + 33, sY + 23, NPC_NAME_BLACKSMITH_KEEPER, Color(45, 25, 25));
                    put_string(sX + 33 - 1, sY + 23 - 1, NPC_NAME_BLACKSMITH_KEEPER, Color(255, 255, 255));
                    break;
                case 25:
                    put_string(sX + 33, sY + 23, NPC_NAME_CITYHALL_OFFICER, Color(45, 25, 25));
                    put_string(sX + 33 - 1, sY + 23 - 1, NPC_NAME_CITYHALL_OFFICER, Color(255, 255, 255));
                    break;
                case 26:
                    put_string(sX + 33, sY + 23, NPC_NAME_GUILDHALL_OFFICER, Color(45, 25, 25));
                    put_string(sX + 33 - 1, sY + 23 - 1, NPC_NAME_GUILDHALL_OFFICER, Color(255, 255, 255));
                    break;
            }

           
            if (m_stDialogBoxInfo[20].sV3 == 25)
            {
                if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
                {
                    put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY13, Color(255, 255, 255));
                    put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY13, Color(255, 255, 255));
                }
                else
                {
                    put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY13, Color(4, 0, 50));
                    put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY13, Color(4, 0, 50));
                }
            }
            else if (m_stDialogBoxInfo[20].sV3 == 20)
            {
                if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
                {
                    put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY17, Color(255, 255, 255));
                    put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY17, Color(255, 255, 255));
                }
                else
                {
                    put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY17, Color(4, 0, 50));
                    put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY17, Color(4, 0, 50));
                }
            }
            else
            {
                if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
                {
                    put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, Color(255, 255, 255));
                    put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, Color(255, 255, 255));
                }
                else
                {
                    put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, Color(4, 0, 50));
                    put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, Color(4, 0, 50));
                }
            }

            if (m_bIsDialogEnabled[21] == FALSE)
            {
                if ((msX > sX + 125) && (msX < sX + 180) && (msY > sY + 55) && (msY < sY + 70))
                {
                    put_string(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(255, 255, 255));
                    put_string(sX + 126, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(255, 255, 255));
                }
                else
                {
                    put_string(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(4, 0, 50));
                    put_string(sX + 126, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(4, 0, 50));
                }
            }
            break;

        case 1:
            
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 6); 
            GetItemName(m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_dwAttribute, cStr1, cStr2, cStr3, 64);
#if DEF_LANGUAGE == 4	//¾ð¾î:English
            format_to_local(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, m_stDialogBoxInfo[20].sV3, cStr1);
            format_to_local(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_stDialogBoxInfo[20].cStr);
#else
            format_to_local(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, cStr1, m_stDialogBoxInfo[20].sV3);
            format_to_local(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_stDialogBoxInfo[20].cStr);
#endif
            put_string(sX + 24, sY + 25, cTxt, Color(45, 25, 25));
            put_string(sX + 24, sY + 40, cTxt2, Color(45, 25, 25));

            if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
            {
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY30, Color(255, 255, 255));
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY30, Color(255, 255, 255));
            }
            else
            {
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY30, Color(4, 0, 50));
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY30, Color(4, 0, 50));
            }

            if ((msX > sX + 155) && (msX < sX + 210) && (msY > sY + 55) && (msY < sY + 70))
            {
                put_string(sX + 155, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY34, Color(255, 255, 255));
                put_string(sX + 156, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY34, Color(255, 255, 255));
            }
            else
            {
                put_string(sX + 155, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY34, Color(4, 0, 50));
                put_string(sX + 156, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY34, Color(4, 0, 50));
            }
            break;

        case 2:
            
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 5); 
            GetItemName(m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_dwAttribute, cStr1, cStr2, cStr3, 64);
#if DEF_LANGUAGE == 4	//¾ð¾î:English
            format_to_local(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, m_stDialogBoxInfo[20].sV3, cStr1);
            format_to_local(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_stDialogBoxInfo[20].cStr);
#else
            format_to_local(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, cStr1, m_stDialogBoxInfo[20].sV3);
            format_to_local(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_stDialogBoxInfo[20].cStr);
#endif
            put_string(sX + 24, sY + 20, cTxt, Color(45, 25, 25));
            put_string(sX + 24, sY + 35, cTxt2, Color(45, 25, 25));

            if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
            {
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, Color(255, 255, 255));
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, Color(255, 255, 255));
            }
            else
            {
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, Color(4, 0, 50));
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, Color(4, 0, 50));
            }

            if ((m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                (m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cItemType == DEF_ITEMTYPE_ARROW))
            {
                
            }
            else
            {
                if ((msX > sX + 125) && (msX < sX + 180) && (msY > sY + 55) && (msY < sY + 70))
                {
                    put_string(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY43, Color(255, 255, 255));
                    put_string(sX + 126, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY43, Color(255, 255, 255));
                }
                else
                {
                    put_string(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY43, Color(4, 0, 50));
                    put_string(sX + 126, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY43, Color(4, 0, 50));
                }
            }
            break;

        case 3:
            
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 6); 
            GetItemName(m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_dwAttribute, cStr1, cStr2, cStr3, 64);
#if DEF_LANGUAGE == 4	//¾ð¾î:English
            format_to_local(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, m_stDialogBoxInfo[20].sV3, cStr1);
            format_to_local(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_stDialogBoxInfo[20].cStr);
#else
            format_to_local(cTxt, DRAW_DIALOGBOX_NPCACTION_QUERY29, cStr1, m_stDialogBoxInfo[20].sV3);
            format_to_local(cTxt2, DRAW_DIALOGBOX_NPCACTION_QUERY29_1, m_stDialogBoxInfo[20].cStr);
#endif
            put_aligned_string(sX, sX + 240, sY + 20, cTxt, 45, 25, 25);
            put_aligned_string(sX, sX + 240, sY + 35, cTxt2, 45, 25, 25);

            if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
            {
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, Color(255, 255, 255));
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, Color(255, 255, 255));
            }
            else
            {
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, Color(4, 0, 50));
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, Color(4, 0, 50));
            }
            break;

        case 4:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 5); 
            switch (m_stDialogBoxInfo[20].sV3)
            {

                case 21:
                    put_string(sX + 35, sY + 25, NPC_NAME_GUARD, Color(45, 25, 25));
                    put_string(sX + 35 - 1, sY + 25 - 1, NPC_NAME_GUARD, Color(255, 255, 255));
                    break;
                case 32:
                    put_string(sX + 35, sY + 25, NPC_NAME_UNICORN, Color(45, 25, 25));
                    put_string(sX + 35 - 1, sY + 25 - 1, NPC_NAME_UNICORN, Color(255, 255, 255));
                    break;

                    
                case 67:
                    put_string(sX + 35, sY + 25, NPC_NAME_MCGAFFIN, Color(45, 25, 25));
                    put_string(sX + 35 - 1, sY + 25 - 1, NPC_NAME_MCGAFFIN, Color(255, 255, 255));
                    break;
                case 68:
                    put_string(sX + 35, sY + 25, NPC_NAME_PERRY, Color(45, 25, 25));
                    put_string(sX + 35 - 1, sY + 25 - 1, NPC_NAME_PERRY, Color(255, 255, 255));
                    break;
                case 69:
                    put_string(sX + 35, sY + 25, NPC_NAME_DEVLIN, Color(45, 25, 25));
                    put_string(sX + 35 - 1, sY + 25 - 1, NPC_NAME_DEVLIN, Color(255, 255, 255));
                    break;

            }

            if (m_bIsDialogEnabled[21] == FALSE)
            {
                if ((msX > sX + 125) && (msX < sX + 180) && (msY > sY + 55) && (msY < sY + 70))
                {
                    put_string(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(255, 255, 255));
                    put_string(sX + 126, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(255, 255, 255));
                }
                else
                {
                    put_string(sX + 125, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(4, 0, 50));
                    put_string(sX + 126, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(4, 0, 50));
                }
            }
            break;

        case 5:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 6); 
            switch (m_stDialogBoxInfo[20].sV3)
            {
                case 15:
                    put_string(sX + 33, sY + 23, NPC_NAME_SHOP_KEEPER, Color(45, 25, 25));
                    put_string(sX + 33 - 1, sY + 23 - 1, NPC_NAME_SHOP_KEEPER, Color(255, 255, 255));
                    break;
                case 24:
                    put_string(sX + 33, sY + 23, NPC_NAME_BLACKSMITH_KEEPER, Color(45, 25, 25));
                    put_string(sX + 33 - 1, sY + 23 - 1, NPC_NAME_BLACKSMITH_KEEPER, Color(255, 255, 255));
                    break;

            }

            if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
            {
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, Color(255, 255, 255));
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, Color(255, 255, 255));
            }
            else
            {
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, Color(4, 0, 50));
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY21, Color(4, 0, 50));
            }

            if ((msX > sX + 25 + 79) && (msX < sX + 80 + 75) && (msY > sY + 55) && (msY < sY + 70))
            {
                put_string(sX + 28 + 75, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, Color(255, 255, 255));
                put_string(sX + 29 + 75, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, Color(255, 255, 255));
            }
            else
            {
                put_string(sX + 28 + 75, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, Color(4, 0, 50));
                put_string(sX + 29 + 75, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY39, Color(4, 0, 50));
            }

            if (m_bIsDialogEnabled[21] == FALSE)
            {
                if ((msX > sX + 155) && (msX < sX + 210) && (msY > sY + 55) && (msY < sY + 70))
                {
                    put_string(sX + 155, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(255, 255, 255));
                    put_string(sX + 156, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(255, 255, 255));
                }
                else
                {
                    put_string(sX + 155, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(4, 0, 50));
                    put_string(sX + 156, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY25, Color(4, 0, 50));
                }
            }
            break;
    }
}

void CGame::DrawDialogBox_Party(short msX, short msY)
{
    short sX, sY, szX;
    int  i, iNth;

    

    sX = m_stDialogBoxInfo[32].sX;
    sY = m_stDialogBoxInfo[32].sY;
    szX = m_stDialogBoxInfo[32].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 3);

    switch (m_stDialogBoxInfo[32].cMode)
    {
        case 0:
            if (m_iPartyStatus == 0)
            {
                if ((msX > sX + 80) && (msX < sX + 195) && (msY > sY + 80) && (msY < sY + 100))
                    put_aligned_string(sX, sX + szX, sY + 85, DRAW_DIALOGBOX_PARTY1, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 85, DRAW_DIALOGBOX_PARTY1, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 85, DRAW_DIALOGBOX_PARTY1, 65, 65, 65);

            if (m_iPartyStatus != 0)
            {
                if ((msX > sX + 80) && (msX < sX + 195) && (msY > sY + 100) && (msY < sY + 120))
                    put_aligned_string(sX, sX + szX, sY + 105, DRAW_DIALOGBOX_PARTY4, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 105, DRAW_DIALOGBOX_PARTY4, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 105, DRAW_DIALOGBOX_PARTY4, 65, 65, 65);

#if DEF_LANGUAGE == 5
            if (m_iPartyStatus != 0)
            {
                if ((msX > sX + 80) && (msX < sX + 195) && (msY > sY + 120) && (msY < sY + 145))
                {
                    put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 255, 255, 255);
                    put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY8, 255, 255, 255);
                }
                else
                {
                    put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 4, 0, 50);
                    put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY8, 4, 0, 50);
                }
            }
            else
            {
                put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 65, 65, 65);
                put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY8, 65, 65, 65);
            }
#else
            if (m_iPartyStatus != 0)
            {
                if ((msX > sX + 80) && (msX < sX + 195) && (msY > sY + 120) && (msY < sY + 140))
                    put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 65, 65, 65);
#endif

            switch (m_iPartyStatus)
            {
                case 0:
                    put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY10);
                    put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_PARTY11);
                    put_aligned_string(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_PARTY12);
                    break;

                case 1:
                case 2:
                    put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY13);
                    put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_PARTY14);
                    put_aligned_string(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_PARTY15);
                    break;
            }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
            break;

        case 1: 
            format_to_local(G_cTxt, DRAW_DIALOGBOX_PARTY16, m_stDialogBoxInfo[32].cStr);
            
            put_aligned_string(sX, sX + szX, sY + 95, G_cTxt);

#if DEF_LANGUAGE == 5
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY17);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY17_1);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY18);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY19);
            put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_PARTY20);
#else
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY17);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY18);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY19);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY20);
#endif

            put_aligned_string(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_PARTY21);

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 2:
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY22);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY23);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY24);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY25);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;

        case 3:
            format_to_local(G_cTxt, DRAW_DIALOGBOX_PARTY26, m_stDialogBoxInfo[32].cStr);
            put_aligned_string(sX, sX + szX, sY + 95, G_cTxt);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY27);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY28);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY29);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY30);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;

        case 4:
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY31);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY32);

            iNth = 0;
            for (i = 0; i <= DEF_MAXPARTYMEMBERS; i++)
                if (strlen(m_stPartyMemberNameList[i].cName) != 0)
                {
                    format_to_local(G_cTxt, "{}", m_stPartyMemberNameList[i].cName);
                    put_aligned_string(sX + 17, sX + 270, sY + 140 + 15 * (iNth), G_cTxt);
                    iNth++;
                }

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 5:
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY33);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY34);
            break;

        case 6:
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY35);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 7:
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY36);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY37);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY38);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY39);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 8:
#if DEF_LANGUAGE == 5
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY40);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY41);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY42);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY43);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY44);
            put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_PARTY45);
            put_aligned_string(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_PARTY45_1);
#else
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY40);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY41);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY42);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY43);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY44);
            put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_PARTY45);
#endif

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 9:
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY46);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY47);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY48);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY49);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY50);
            put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_PARTY51);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 10:
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY52);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY53);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY54);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 11: 

            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY55);

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;
    }
}

void CGame::DrawDialogBox_QueryDropItemAmount()
{
    short sX, sY;
    char cTxt[120]{}, cStr1[64]{}, cStr2[64]{}, cStr3[64]{};

    sX = m_stDialogBoxInfo[17].sX;
    sY = m_stDialogBoxInfo[17].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 5);

    switch (m_stDialogBoxInfo[17].cMode)
    {
        case 1:
            GetItemName(m_pItemList[m_stDialogBoxInfo[17].sView]->m_cName, m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwAttribute, cStr1, cStr2, cStr3, 64);
            if (strlen(m_stDialogBoxInfo[17].cStr) == 0)
                format_to_local(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT1, cStr1);
            else format_to_local(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT2, cStr1, m_stDialogBoxInfo[17].cStr);

            if (m_stDialogBoxInfo[17].sV3 < 1000)
                put_string(sX + 30, sY + 20, cTxt, Color(55, 25, 25));

            put_string(sX + 30, sY + 35, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT3, Color(55, 25, 25));
            if (iGetTopDialogBoxIndex() != 17)
                put_string(sX + 40, sY + 57, m_cAmountString, Color(255, 255, 255), false, 2);
            format_to_local(cTxt, "__________ (0 ~ {})", m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwCount);
            put_string(sX + 38, sY + 62, cTxt, Color(25, 35, 25));
            break;

        case 20:
            GetItemName(m_pItemList[m_stDialogBoxInfo[17].sView]->m_cName, m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwAttribute, cStr1, cStr2, cStr3, 64);
            if (strlen(m_stDialogBoxInfo[17].cStr) == 0)
                format_to_local(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT1, cStr1);
            else format_to_local(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT2, cStr1, m_stDialogBoxInfo[17].cStr);

            if (m_stDialogBoxInfo[17].sV3 < 1000)
                put_string(sX + 30, sY + 20, cTxt, Color(55, 25, 25));

            put_string(sX + 30, sY + 35, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT3, Color(55, 25, 25));
            put_string(sX + 40, sY + 57, m_cAmountString, Color(255, 255, 255), FALSE, 2);
            format_to_local(cTxt, "__________ (0 ~ {})", m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwCount);
            put_string(sX + 38, sY + 62, cTxt, Color(25, 35, 25));
            break;
    }
}

void CGame::DrawDialogBox_Quest(int msX, int msY)
{
    short sX, sY, szX;
    char cTxt[120], cTemp[21];

    sX = m_stDialogBoxInfo[28].sX;
    sY = m_stDialogBoxInfo[28].sY;
    szX = m_stDialogBoxInfo[28].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 4);

    switch (m_stDialogBoxInfo[28].cMode)
    {
        case 1:
            switch (m_stQuest.sQuestType)
            {
                case NULL:
                    put_aligned_string(sX, sX + szX, sY + 50 + 115 - 30, DRAW_DIALOGBOX_QUEST1, 55, 25, 25);
                    
                    break;

                case 1:
                    
                    if (m_stQuest.bIsQuestCompleted == FALSE)
                        put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST2, 55, 25, 25);
                    
                    else put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST3, 55, 25, 25);
                    

                    ZeroMemory(cTemp, sizeof(cTemp));
                    switch (m_stQuest.sWho)
                    {
                        case 1:
                        case 2:
                        case 3: break;
                        case 4: strcpy(cTemp, NPC_NAME_CITYHALL_OFFICER); break;
                            
                        case 5:
                        case 6:
                        case 7: break;
                    }
                    ZeroMemory(cTxt, sizeof(cTxt));
                    format_to_local(cTxt, DRAW_DIALOGBOX_QUEST5, cTemp);
                    
                    put_aligned_string(sX, sX + szX, sY + 50 + 45, cTxt, 55, 25, 25);

                    ZeroMemory(cTemp, sizeof(cTemp));
                    GetNpcName(m_stQuest.sTargetType, cTemp);
                    ZeroMemory(cTxt, sizeof(cTxt));
                    format_to_local(cTxt, NPC_TALK_HANDLER16, m_stQuest.sTargetCount, cTemp);
                    put_aligned_string(sX, sX + szX, sY + 50 + 60, cTxt, 55, 25, 25);

                    ZeroMemory(cTxt, sizeof(cTxt));
                    if (memcmp(m_stQuest.cTargetName, "NONE", 4) == 0)
                    {
                        strcpy(cTxt, DRAW_DIALOGBOX_QUEST31);
                        put_aligned_string(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);
                    }
                    else
                    {
                        ZeroMemory(cTemp, sizeof(cTemp));
                        GetOfficialMapName(m_stQuest.cTargetName, cTemp);
                        format_to_local(cTxt, DRAW_DIALOGBOX_QUEST32, cTemp);
                        put_aligned_string(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);

                        if (m_stQuest.sX != 0)
                        {
                            ZeroMemory(cTxt, sizeof(cTxt));
                            format_to_local(cTxt, DRAW_DIALOGBOX_QUEST33, m_stQuest.sX, m_stQuest.sY, m_stQuest.sRange);
                            put_aligned_string(sX, sX + szX, sY + 50 + 90, cTxt, 55, 25, 25);
                        }
                    }

                    ZeroMemory(cTxt, sizeof(cTxt));
                    format_to_local(cTxt, DRAW_DIALOGBOX_QUEST34, m_stQuest.sContribution);
                    put_aligned_string(sX, sX + szX, sY + 50 + 105, cTxt, 55, 25, 25);
                    break;

                case 7: 
                    if (m_stQuest.bIsQuestCompleted == FALSE)
                        put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST26, 55, 25, 25);
                    else put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST27, 55, 25, 25);

                    ZeroMemory(cTemp, sizeof(cTemp));
                    switch (m_stQuest.sWho)
                    {
                        case 1:
                        case 2:
                        case 3: break;
                        case 4: strcpy(cTemp, NPC_NAME_CITYHALL_OFFICER); break;
                        case 5:
                        case 6:
                        case 7: break;
                    }
                    ZeroMemory(cTxt, sizeof(cTxt));
                    format_to_local(cTxt, DRAW_DIALOGBOX_QUEST29, cTemp);
                    put_aligned_string(sX, sX + szX, sY + 50 + 45, cTxt, 55, 25, 25);

                    put_aligned_string(sX, sX + szX, sY + 50 + 60, DRAW_DIALOGBOX_QUEST30, 55, 25, 25);

                    ZeroMemory(cTxt, sizeof(cTxt));
                    if (memcmp(m_stQuest.cTargetName, "NONE", 4) == 0)
                    {
                        strcpy(cTxt, DRAW_DIALOGBOX_QUEST31);
                        put_aligned_string(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);
                    }
                    else
                    {
                        ZeroMemory(cTemp, sizeof(cTemp));
                        GetOfficialMapName(m_stQuest.cTargetName, cTemp);
                        format_to_local(cTxt, DRAW_DIALOGBOX_QUEST32, cTemp);
                        put_aligned_string(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);

                        if (m_stQuest.sX != 0)
                        {
                            ZeroMemory(cTxt, sizeof(cTxt));
                            format_to_local(cTxt, DRAW_DIALOGBOX_QUEST33, m_stQuest.sX, m_stQuest.sY, m_stQuest.sRange);
                            put_aligned_string(sX, sX + szX, sY + 50 + 90, cTxt, 55, 25, 25);
                        }
                    }

                    ZeroMemory(cTxt, sizeof(cTxt));
                    format_to_local(cTxt, DRAW_DIALOGBOX_QUEST34, m_stQuest.sContribution);
                    put_aligned_string(sX, sX + szX, sY + 50 + 105, cTxt, 55, 25, 25);
                    break;
            }
            break;

        case 2:
            put_aligned_string(sX, sX + szX, sY + 50 + 115 - 30, DRAW_DIALOGBOX_QUEST35, 55, 25, 25);
            break;
    }

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
}

void CGame::DrawDialogBox_SellList(short msX, short msY)
{
    short sX, sY, szX;
    int  i, iItem;
    char cTemp[255], cStr1[64]{}, cStr2[64]{}, cStr3[64]{};

    sX = m_stDialogBoxInfo[31].sX;
    sY = m_stDialogBoxInfo[31].sY;
    szX = m_stDialogBoxInfo[31].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

    iItem = 0;
    for (i = 0; i < DEF_MAXSELLLIST; i++)
        if (m_stSellItemList[i].iIndex != -1)
        {
            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pItemList[m_stSellItemList[i].iIndex]->m_cName, m_pItemList[m_stSellItemList[i].iIndex]->m_dwAttribute, cStr1, cStr2, cStr3, 64);
            if (m_stSellItemList[i].iAmount > 1)
            {
#if DEF_LANGUAGE == 4	//¾ð¾î:English
                format_to_local(cTemp, DRAW_DIALOGBOX_SELL_LIST1, m_stSellItemList[i].iAmount, cStr1);
#else
                format_to_local(cTemp, DRAW_DIALOGBOX_SELL_LIST1, cStr1, m_stSellItemList[i].iAmount);
#endif
                if ((msX > sX + 25) && (msX < sX + 250) && (msY >= sY + 55 + i * 15) && (msY <= sY + 55 + 14 + i * 15))
                    put_aligned_string(sX, sX + szX, sY + 55 + i * 15, cTemp, 255, 255, 255);
                else
                {
                    if (m_bIsSpecial)
                        put_aligned_string(sX, sX + szX, sY + 55 + i * 15, cTemp, 0, 255, 50);
                    else
                        put_aligned_string(sX, sX + szX, sY + 55 + i * 15, cTemp, 45, 25, 25);
                }
            }
            else
            {
                if ((msX > sX + 25) && (msX < sX + 250) && (msY >= sY + 55 + i * 15) && (msY <= sY + 55 + 14 + i * 15))
                {
                    if ((strlen(cStr2) == 0) && (strlen(cStr3) == 0)) put_aligned_string(sX, sX + szX, sY + 55 + i * 15, cStr1, 255, 255, 255);
                    else
                    {
                        ZeroMemory(G_cTxt, sizeof(G_cTxt));
                        if ((strlen(cStr1) + strlen(cStr2) + strlen(cStr3)) < 36)
                        {
                            if ((strlen(cStr2) > 0) && (strlen(cStr3) > 0)) format_to_local(G_cTxt, "{}({}, {})", cStr1, cStr2, cStr3);
                            else format_to_local(G_cTxt, "{}({}{})", cStr1, cStr2, cStr3);
                            put_aligned_string(sX, sX + szX, sY + 55 + i * 15, G_cTxt, 255, 255, 255);

                        }
                        else
                        {
                            if ((strlen(cStr2) > 0) && (strlen(cStr3) > 0)) format_to_local(G_cTxt, "({}, {})", cStr2, cStr3);
                            else format_to_local(G_cTxt, "({}{})", cStr2, cStr3);
                            put_aligned_string(sX, sX + szX, sY + 55 + i * 15, cStr1, 255, 255, 255);
                            put_aligned_string(sX, sX + szX, sY + 55 + i * 15 + 15, G_cTxt, 200, 200, 200);
                            i++;
                        }
                    }
                }
                else
                {
                    if ((strlen(cStr2) == 0) && (strlen(cStr3) == 0))
                    {
                        
                        if (m_bIsSpecial)
                            put_aligned_string(sX, sX + szX, sY + 55 + i * 15, cStr1, 0, 255, 50);
                        else
                            put_aligned_string(sX, sX + szX, sY + 55 + i * 15, cStr1, 45, 25, 25);
                    }
                    else
                    {
                        ZeroMemory(G_cTxt, sizeof(G_cTxt));
                        if ((strlen(cStr1) + strlen(cStr2) + strlen(cStr3)) < 36)
                        {
                            if ((strlen(cStr2) > 0) && (strlen(cStr3) > 0)) format_to_local(G_cTxt, "{}({}, {})", cStr1, cStr2, cStr3);
                            else format_to_local(G_cTxt, "{}({}{})", cStr1, cStr2, cStr3);

                            if (m_bIsSpecial)
                                put_aligned_string(sX, sX + szX, sY + 55 + i * 15, G_cTxt, 0, 255, 50);
                            else
                                put_aligned_string(sX, sX + szX, sY + 55 + i * 15, G_cTxt, 45, 25, 25);

                        }
                        else
                        {
                            
                            if (m_bIsSpecial)
                                put_aligned_string(sX, sX + szX, sY + 55 + i * 15, cStr1, 0, 255, 50);
                            else
                                put_aligned_string(sX, sX + szX, sY + 55 + i * 15, cStr1, 45, 25, 25);
                        }
                    }
                }
            }
        }
        else iItem++;

    if (iItem == DEF_MAXSELLLIST)
    {
        put_aligned_string(sX, sX + szX, sY + 55 + 30 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST2);
        put_aligned_string(sX, sX + szX, sY + 55 + 45 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST3);
        put_aligned_string(sX, sX + szX, sY + 55 + 60 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST4);
        put_aligned_string(sX, sX + szX, sY + 55 + 75 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST5);
        put_aligned_string(sX, sX + szX, sY + 55 + 95 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST6);
        put_aligned_string(sX, sX + szX, sY + 55 + 110 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST7);
        put_aligned_string(sX, sX + szX, sY + 55 + 125 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST8);

#if DEF_LANGUAGE == 5
        put_aligned_string(sX, sX + szX, sY + 55 + 155 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST9);
        put_aligned_string(sX, sX + szX, sY + 55 + 170 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST10);
#else
        put_aligned_string(sX, sX + szX, sY + 55 + 155 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST9);
#endif
    }

    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY) && (iItem < DEF_MAXSELLLIST))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 39);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 38);

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
}

void CGame::DrawDialogBox_SellorRepairItem(short msX, short msY)
{
    short sX, sY;
    DWORD dwTime = m_dwCurTime;
    char cItemID, cItemColor, cTxt[120], cTemp[120]{}, cStr2[120]{}, cStr3[120]{};

    sX = m_stDialogBoxInfo[23].sX;
    sY = m_stDialogBoxInfo[23].sY;

    switch (m_stDialogBoxInfo[23].cMode)
    {
        case 1:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

            
            cItemID = m_stDialogBoxInfo[23].sV1;

            cItemColor = m_pItemList[cItemID]->m_cItemColor;
            if (cItemColor == 0)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->put_sprite_fast(sX + 62 + 15, sY + 84 + 30,
                    m_pItemList[cItemID]->m_sSpriteFrame, dwTime);
            else
            {
                
                switch (m_pItemList[cItemID]->m_sSprite)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 15: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->put_sprite_color(sX + 62 + 15, sY + 84 + 30, m_pItemList[cItemID]->m_sSpriteFrame,
                        m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime); break;


                    default: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->put_sprite_color(sX + 62 + 15, sY + 84 + 30, m_pItemList[cItemID]->m_sSpriteFrame,
                        m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime); break;
                }
            }
            GetItemName(m_pItemList[cItemID]->m_cName, m_pItemList[cItemID]->m_dwAttribute, cTemp, cStr2, cStr3, 120);
            if (m_stDialogBoxInfo[23].sV4 == 1) strcpy(cTxt, cTemp);
#if DEF_LANGUAGE == 4	//¾ð¾î:English
            else format_to_local(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM1, m_stDialogBoxInfo[23].sV4, cTemp);
#else
            else format_to_local(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM1, cTemp, m_stDialogBoxInfo[23].sV4);
#endif

            if (m_bIsSpecial)
            {
                put_aligned_string(sX + 25, sX + 240, sY + 60, cTxt, 0, 255, 50);
                put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 60, cTxt, 0, 255, 50);
            }
            else
            {
                put_aligned_string(sX + 25, sX + 240, sY + 60, cTxt, 45, 25, 25);
                put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 60, cTxt, 45, 25, 25);
            }

            
            format_to_local(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM2, m_stDialogBoxInfo[23].sV2);
            
            put_string(sX + 95 + 15, sY + 53 + 60, cTxt, Color(45, 25, 25));

            
            format_to_local(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM3, m_stDialogBoxInfo[23].sV3);
            
            put_string(sX + 95 + 15, sY + 53 + 75, cTxt, Color(45, 25, 25));

            put_string(sX + 55, sY + 190, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM4, Color(45, 25, 25));
            

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 39);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 38);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;

        case 2:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 10);

            
            cItemID = m_stDialogBoxInfo[23].sV1;
            cItemColor = m_pItemList[cItemID]->m_cItemColor;
            if (cItemColor == 0)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->put_sprite_fast(sX + 62 + 15, sY + 84 + 30,
                    m_pItemList[cItemID]->m_sSpriteFrame, dwTime);
            else
            {
                
                switch (m_pItemList[cItemID]->m_sSprite)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 15: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->put_sprite_color(sX + 62 + 15, sY + 84 + 30, m_pItemList[cItemID]->m_sSpriteFrame,
                        m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime); break;


                    default: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->put_sprite_color(sX + 62 + 15, sY + 84 + 30, m_pItemList[cItemID]->m_sSpriteFrame,
                        m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime); break;
                }
            }
            GetItemName(m_pItemList[cItemID], cTemp, cStr2, cStr3, 120);

            format_to_local(cTxt, "{}", cTemp);

            //		PutAlignedString(sX + 25, sX + 240, sY + 60, cTxt, 45,25,25);
            //		PutAlignedString(sX + 25 +1, sX + 240 +1, sY + 60, cTxt, 45,25,25);
            if (m_bIsSpecial)
            {
                put_aligned_string(sX + 25, sX + 240, sY + 60, cTxt, 0, 255, 50);
                put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 60, cTxt, 0, 255, 50);
            }
            else
            {
                put_aligned_string(sX + 25, sX + 240, sY + 60, cTxt, 45, 25, 25);
                put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 60, cTxt, 45, 25, 25);
            }

            
            format_to_local(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM2, m_stDialogBoxInfo[23].sV2);
            put_string(sX + 95 + 15, sY + 53 + 60, cTxt, Color(45, 25, 25));
            
            format_to_local(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM6, m_stDialogBoxInfo[23].sV3);
            put_string(sX + 95 + 15, sY + 53 + 75, cTxt, Color(45, 25, 25));
            put_string(sX + 55, sY + 190, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM7, Color(45, 25, 25));

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 43);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 42);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;

        case 3:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

            put_string(sX + 55, sY + 100, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM8, Color(45, 25, 25));
            put_string(sX + 55, sY + 120, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM9, Color(45, 25, 25));
            put_string(sX + 55, sY + 135, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM10, Color(45, 25, 25));
            break;

        case 4:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 10);

            put_string(sX + 55, sY + 100, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM11, Color(45, 25, 25));
            put_string(sX + 55, sY + 120, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM9, Color(45, 25, 25));
            put_string(sX + 55, sY + 135, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM10, Color(45, 25, 25));
            break;
    }
}

void CGame::DrawDialogBox_Shop(short msX, short msY, short msZ, char cLB)
{
    short sX, sY;
    DWORD dwTime = m_dwCurTime;
    int  i, iTemp;
    char cTemp[255]{}, cStr2[255]{}, cStr3[255]{};
    int  iCost, iDiscountCost, iDiscountRatio, iMsgLoc;
    double dTmp1, dTmp2, dTmp3;
    int  iTotalLines, iPointerLoc;
    BOOL bFlag = TRUE;
    double d1, d2, d3;

    iMsgLoc = 0;

    sX = m_stDialogBoxInfo[11].sX;
    sY = m_stDialogBoxInfo[11].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

    switch (m_stDialogBoxInfo[11].cMode)
    {
        case 0:
            iTotalLines = 0;
            for (i = 0; i < DEF_MAXMENUITEMS; i++)
                if (m_pItemForSaleList[i] != NULL) iTotalLines++;

            
            if (iTotalLines > 13)
            {
                d1 = (double)m_stDialogBoxInfo[11].sView;
                d2 = (double)(iTotalLines - 13);
                d3 = (274.0f * d1) / d2;
                iPointerLoc = (int)(d3);
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242, sY + iPointerLoc + 35, 7);
            }
            else iPointerLoc = 0;

            
            if (cLB != 0 && iTotalLines > 13)
            {
                if ((iGetTopDialogBoxIndex() == 11))
                {
                    if ((msX >= sX + 235) && (msX <= sX + 260) && (msY >= sY + 10) && (msY <= sY + 330))
                    {
                        d1 = (double)(msY - (sY + 35));
                        d2 = (double)(iTotalLines - 13);
                        d3 = (d1 * d2) / 274.0f;
                        m_stDialogBoxInfo[11].sView = (int)(d3 + 0.5);
                    }
                }
            }
            else m_stDialogBoxInfo[11].bIsScrollSelected = FALSE;
            if (iGetTopDialogBoxIndex() == 11 && msZ != 0)
            {
                m_stDialogBoxInfo[11].sView = m_stDialogBoxInfo[11].sView - msZ / 60;
                m_stMCursor.sZ = 0;
            }
            if (iTotalLines > 13 && m_stDialogBoxInfo[11].sView > iTotalLines - 13) m_stDialogBoxInfo[11].sView = iTotalLines - 13;
            if (m_stDialogBoxInfo[11].sView < 0 || iTotalLines < 13) m_stDialogBoxInfo[11].sView = 0;
            put_aligned_string(sX + 22, sX + 165, sY + 45, DRAW_DIALOGBOX_SHOP1);
            put_aligned_string(sX + 23, sX + 166, sY + 45, DRAW_DIALOGBOX_SHOP1);
            put_aligned_string(sX + 153, sX + 250, sY + 45, DRAW_DIALOGBOX_SHOP3);
            put_aligned_string(sX + 154, sX + 251, sY + 45, DRAW_DIALOGBOX_SHOP3);

            for (i = 0; i < 13; i++)
                if (((i + m_stDialogBoxInfo[11].sView) < DEF_MAXMENUITEMS) && (m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView] != NULL))
                {
                    ZeroMemory(cTemp, sizeof(cTemp));
                    GetItemName(m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView], cTemp, cStr2, cStr3, 255);

                    if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 65) && (msY <= sY + i * 18 + 79))
                    {
                        put_aligned_string(sX + 10, sX + 190, sY + i * 18 + 65, cTemp, 255, 255, 255);
                    }
                    else put_aligned_string(sX + 10, sX + 190, sY + i * 18 + 65, cTemp, 4, 0, 50);
                }

            for (i = 0; i < 13; i++)
                if (((i + m_stDialogBoxInfo[11].sView) < DEF_MAXMENUITEMS) && (m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView] != NULL))
                {

                    //			iCost =(int)((float)(m_cDiscount) * 0.9f + 0.5f) ;
                                

                    iDiscountRatio = ((m_iCharisma - 10) / 4);
                    dTmp1 = (double)iDiscountRatio;
                    dTmp2 = dTmp1 / 100.0f;
                    dTmp1 = (double)m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView]->m_wPrice;
                    dTmp3 = dTmp1 * dTmp2;
                    iDiscountCost = (int)dTmp3;

                    iCost = (int)(m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView]->m_wPrice * ((100 + m_cDiscount) / 100.));
                    iCost = iCost - iDiscountCost;

                    
                    if (iCost < (m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView]->m_wPrice / 2))
                        iCost = (m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView]->m_wPrice / 2) - 1;

                    ZeroMemory(cTemp, sizeof(cTemp));
                    format_to_local(cTemp, "{:6}", iCost);
                    if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 65) && (msY <= sY + i * 18 + 79))
                        put_aligned_string(sX + 148, sX + 260, sY + i * 18 + 65, cTemp, 255, 255, 255);
                    else	put_aligned_string(sX + 148, sX + 260, sY + i * 18 + 65, cTemp, 4, 0, 50);
                }
            break;

        default:
            
            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sSprite]->put_sprite_fast(sX + 62 + 30 - 35, sY + 84 + 30 - 10,
                m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sSpriteFrame, dwTime);

            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1], cTemp, cStr2, cStr3, 255);

            put_aligned_string(sX + 25, sX + 240, sY + 50, cTemp, 255, 255, 255);
            put_aligned_string(sX + 26, sX + 241, sY + 50, cTemp, 255, 255, 255);

            strcpy(cTemp, DRAW_DIALOGBOX_SHOP3);
            put_string(sX + 95 + 30 - 35, sY + 78 + 30 - 10, cTemp, Color(40, 10, 10));
            put_string(sX + 96 + 30 - 35, sY + 78 + 30 - 10, cTemp, Color(40, 10, 10));
            strcpy(cTemp, DRAW_DIALOGBOX_SHOP6);
            
            put_string(sX + 95 + 30 - 35, sY + 93 + 30 - 10, cTemp, Color(40, 10, 10));
            put_string(sX + 96 + 30 - 35, sY + 93 + 30 - 10, cTemp, Color(40, 10, 10));


            
            iDiscountRatio = ((m_iCharisma - 10) / 4);
            dTmp1 = (double)iDiscountRatio;
            dTmp2 = dTmp1 / 100.0f;
            dTmp1 = (double)m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wPrice;
            dTmp3 = dTmp1 * dTmp2;
            iDiscountCost = (int)dTmp3;

            iCost = (int)(m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wPrice * ((100 + m_cDiscount) / 100.));
            iCost = iCost - iDiscountCost;

            
            if (iCost < (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wPrice / 2))
                iCost = (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wPrice / 2) - 1;

            format_to_local(cTemp, DRAW_DIALOGBOX_SHOP7, iCost);
            //": {} Gold"
            put_string(sX + 140, sY + 98, cTemp, Color(45, 25, 25));

            iTemp = m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wWeight / 100;
            format_to_local(cTemp, DRAW_DIALOGBOX_SHOP8, iTemp);
            //": {} Stone"
            put_string(sX + 140, sY + 113, cTemp, Color(45, 25, 25));

            //	#ifdef _DEBUG
            
            //		if ((m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cEquipPos == DEF_EQUIPPOS_RHAND) ||
            //			(m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND) )
            //		{
            //		    int iThrow, iRange, iTotalDmg;
            //          int i, iRet;
            //			iThrow = m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue1;	
            //			iRange = m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue2;	
            //			srand( (unsigned)time( NULL ));
            //			iRet = 0;
            //			iTotalDmg = 0;
            //			for (int j = 0 ; j < 10 ; j++)
            //			{
            //				for (i = 1; i <= iThrow; i++) 
            //				{
            //					iRet += (rand() % iRange) + 1;
            //				}
            //				iTotalDmg += iRet;
            //			} 
            
            //			PutString(sX + 95 + 30 -35, sY + 108 + 30 -10, cTemp, Color(40,10,10));
            //			PutString(sX + 96 + 30 -35, sY + 108 + 30 -10, cTemp, Color(40,10,10));
            //		}
            
            //	#endif

            switch (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cEquipPos)
            {
                case DEF_EQUIPPOS_RHAND:
                case DEF_EQUIPPOS_TWOHAND:
                    
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP9);
                    
                    put_string(sX + 40, sY + 145, cTemp, Color(45, 25, 25));
                    put_string(sX + 41, sY + 145, cTemp, Color(45, 25, 25));
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP10);
                    
                    put_string(sX + 30, sY + 175, cTemp, Color(45, 25, 25));
                    put_string(sX + 31, sY + 175, cTemp, Color(45, 25, 25));

                    if (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue3 != 0)
                    {
                        format_to_local(cTemp, ": {}D{}+{} (S-M)", m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue1,
                            m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue2,
                            m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue3);

                    }
                    else
                    {
                        format_to_local(cTemp, ": {}D{} (S-M)", m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue1,
                            m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue2);
                    }
                    put_string(sX + 140, sY + 145, cTemp, Color(45, 25, 25));

                    if (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue6 != 0)
                    {
                        format_to_local(cTemp, ": {}D{}+{} (L)", m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue4,
                            m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5,
                            m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue6);
                    }
                    else
                    {
                        format_to_local(cTemp, ": {}D{} (L)", m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue4,
                            m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5);
                    }
                    put_string(sX + 140, sY + 160, cTemp, Color(45, 25, 25));
                    
#if DEF_LANGUAGE == 5			
                    if (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cSpeed == 0)   format_to_local(cTemp, ": 0(10-10)");
                    else format_to_local(cTemp, ": {}({} - {})", m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cSpeed, iTemp, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cSpeed * 13);
#else
                    if (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cSpeed == 0)   format_to_local(cTemp, ": 0(10~10)");
                    else format_to_local(cTemp, ": {}({} ~ {})", m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cSpeed, iTemp, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cSpeed * 13);
#endif

                    put_string(sX + 140, sY + 175, cTemp, Color(45, 25, 25));

                    
                    if ((m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wWeight / 100) > m_iStr)
                    {
                        format_to_local(cTemp, DRAW_DIALOGBOX_SHOP11, (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wWeight / 100));
                        
                        put_aligned_string(sX + 25, sX + 240, sY + 258 + iMsgLoc * 15, cTemp, 195, 25, 25);
                        put_aligned_string(sX + 26, sX + 241, sY + 258 + iMsgLoc * 15, cTemp, 195, 25, 25);
                        iMsgLoc++;
                    }
                    break;

                case DEF_EQUIPPOS_LHAND:
                    
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP12);
                    put_string(sX + 90, sY + 145, cTemp, Color(45, 25, 25));
                    put_string(sX + 91, sY + 145, cTemp, Color(45, 25, 25));

                    format_to_local(cTemp, ": +{}%", m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue1);
                    put_string(sX + 140, sY + 145, cTemp, Color(45, 25, 25));

                    
                    if ((m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wWeight / 100) > m_iStr)
                    {
                        format_to_local(cTemp, DRAW_DIALOGBOX_SHOP11, (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wWeight / 100));
                        
                        put_aligned_string(sX + 25, sX + 240, sY + 258, cTemp, 195, 25, 25);
                        put_aligned_string(sX + 26, sX + 241, sY + 258, cTemp, 195, 25, 25);
                    }
                    break;

                case DEF_EQUIPPOS_HEAD:
                case DEF_EQUIPPOS_BODY:
                case DEF_EQUIPPOS_BOOTS:
                case DEF_EQUIPPOS_ARMS:
                case DEF_EQUIPPOS_PANTS:
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP12);
                    put_string(sX + 90, sY + 145, cTemp, Color(45, 25, 25));
                    put_string(sX + 91, sY + 145, cTemp, Color(45, 25, 25));

                    format_to_local(cTemp, ": +{}%", m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue1);
                    put_string(sX + 140, sY + 145, cTemp, Color(45, 25, 25));

                    bFlag = TRUE;
                    switch (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue4)
                    {
                        case 10:
                            bFlag = FALSE;
                            format_to_local(cTemp, DRAW_DIALOGBOX_SHOP15, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5);
                            
                            if (m_iStr >= m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5)
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                bFlag = TRUE;
                            }
                            else
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                            }
                            break;
                        case 11:
                            format_to_local(cTemp, DRAW_DIALOGBOX_SHOP16, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5);
                            
                            if (m_iDex >= m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5)
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                bFlag = TRUE;
                            }
                            else
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                            }
                            break;
                        case 12:
                            format_to_local(cTemp, DRAW_DIALOGBOX_SHOP17, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5);
                            
                            if (m_iVit >= m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5)
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                bFlag = TRUE;
                            }
                            else
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                            }
                            break;
                        case 13:
                            format_to_local(cTemp, DRAW_DIALOGBOX_SHOP18, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5);
                            
                            if (m_iInt >= m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5)
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                bFlag = TRUE;
                            }
                            else
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                            }
                            break;
                        case 14:
                            format_to_local(cTemp, DRAW_DIALOGBOX_SHOP19, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5);
                            
                            if (m_iMag >= m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5)
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                bFlag = TRUE;
                            }
                            else
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(55, 25, 25));
                            }
                            break;
                        case 15:
                            format_to_local(cTemp, DRAW_DIALOGBOX_SHOP20, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5);
                            
                            if (m_iCharisma >= m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5)
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                bFlag = TRUE;
                            }
                            else
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                            }
                            break;
                        default:
                            break;
                    }

                    
                    if ((m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wWeight / 100) > m_iStr)
                    {
                        format_to_local(cTemp, DRAW_DIALOGBOX_SHOP11, (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_wWeight / 100));
                        
                        put_aligned_string(sX + 25, sX + 240, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                        put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                        iMsgLoc++;
                        bFlag = FALSE;
                    }
                    else if (bFlag == FALSE)
                    {
                        strcpy(cTemp, DRAW_DIALOGBOX_SHOP21);
                        put_aligned_string(sX + 25, sX + 240, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                        put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                        iMsgLoc++;
                    }

                    if (strstr(m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cName, "(M)") != NULL)
                    {
                        
                        if (m_sPlayerType > 3)
                        {
                            strcpy(cTemp, DRAW_DIALOGBOX_SHOP22);
                            put_aligned_string(sX + 25, sX + 240, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                            put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                            iMsgLoc++;
                            bFlag = FALSE;
                        }
                    }
                    else if (strstr(m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cName, "(W)") != NULL)
                    {
                        
                        if (m_sPlayerType <= 3)
                        {
                            strcpy(cTemp, DRAW_DIALOGBOX_SHOP23);
                            
                            put_aligned_string(sX + 25, sX + 240, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                            put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                            iMsgLoc++;
                            bFlag = FALSE;
                        }
                    }
                    break;

                case DEF_EQUIPPOS_NONE:
                    break;
            }

            
            if (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sLevelLimit != 0)
            {
                strcpy(cTemp, DRAW_DIALOGBOX_SHOP24);
                
                put_string(sX + 75 - 35, sY + 170 + 30 - 10, cTemp, Color(45, 25, 25));
                put_string(sX + 76 - 35, sY + 170 + 30 - 10, cTemp, Color(45, 25, 25));

                format_to_local(cTemp, DRAW_DIALOGBOX_SHOP25, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sLevelLimit);
                
                put_string(sX + 145 + 30 - 35, sY + 170 + 30 - 10, cTemp, Color(45, 25, 25));

                if ((bFlag == TRUE) && (m_iLevel < m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sLevelLimit))
                {
                    strcpy(cTemp, DRAW_DIALOGBOX_SHOP26);
                    
                    put_aligned_string(sX + 25, sX + 240, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                    put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                    iMsgLoc++;
                }
            }


            m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 156, sY + 219, 19, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 170, sY + 219, 19, dwTime);

            put_string(sX + 127 - 35, sY + 237 - 10, DRAW_DIALOGBOX_SHOP27, Color(40, 10, 10));
            put_string(sX + 128 - 35, sY + 237 - 10, DRAW_DIALOGBOX_SHOP27, Color(40, 10, 10));
            if (iGetTopDialogBoxIndex() == 11 && msZ != 0)
            {
                m_stDialogBoxInfo[11].sV3 = m_stDialogBoxInfo[11].sV3 + msZ / 60;
                m_stMCursor.sZ = 0;
            }
            if (m_stDialogBoxInfo[11].sV3 > (50 - _iGetTotalItemNum())) m_stDialogBoxInfo[11].sV3 = (50 - _iGetTotalItemNum());
            if (m_stDialogBoxInfo[11].sV3 < 1) m_stDialogBoxInfo[11].sV3 = 1;

            if (m_stDialogBoxInfo[11].sV3 >= 10)
            {
                
                ZeroMemory(cTemp, sizeof(cTemp));
                _itoa(m_stDialogBoxInfo[11].sV3, cTemp, 10);
                cTemp[1] = NULL;
                put_string(sX - 35 + 186, sY - 10 + 237, cTemp, Color(40, 10, 10));
                put_string(sX - 35 + 187, sY - 10 + 237, cTemp, Color(40, 10, 10));

                
                ZeroMemory(cTemp, sizeof(cTemp));
                _itoa(m_stDialogBoxInfo[11].sV3, cTemp, 10);
                put_string(sX - 35 + 200, sY - 10 + 237, (cTemp + 1), Color(40, 10, 10));
                put_string(sX - 35 + 201, sY - 10 + 237, (cTemp + 1), Color(40, 10, 10));
            }
            else
            {
                put_string(sX - 35 + 186, sY - 10 + 237, "0", Color(40, 10, 10));
                put_string(sX - 35 + 187, sY - 10 + 237, "0", Color(40, 10, 10));

                ZeroMemory(cTemp, sizeof(cTemp));
                _itoa(m_stDialogBoxInfo[11].sV3, cTemp, 10);
                put_string(sX - 35 + 200, sY - 10 + 237, (cTemp), Color(40, 10, 10));
                put_string(sX - 35 + 201, sY - 10 + 237, (cTemp), Color(40, 10, 10));
            }

            m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 156, sY + 244, 20, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + 170, sY + 244, 20, dwTime);

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 31);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 30);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;
    }
}

void CGame::DrawDialogBox_Skill(short msX, short msY, short msZ, char cLB)
{
    short sX, sY;
    int  i, iTotalLines, iPointerLoc;
    char cTemp[255], cTemp2[255];
    double d1, d2, d3;

    sX = m_stDialogBoxInfo[15].sX;
    sY = m_stDialogBoxInfo[15].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0); // Normal Dialog
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 1); // Skill Dialog Title Bar

    switch (m_stDialogBoxInfo[15].cMode)
    {
        case 0:
            for (i = 0; i < 17; i++)
                if ((i < DEF_MAXSKILLTYPE) && (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView] != NULL))
                {
                    ZeroMemory(cTemp, sizeof(cTemp));
                    format_to_local(cTemp, "{}", m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_cName);

                    m_Misc.ReplaceString(cTemp, '-', ' ');

                    ZeroMemory(cTemp2, sizeof(cTemp2));
                    format_to_local(cTemp2, "{:3}%", m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_iLevel);

                    if ((msX >= sX + 25) && (msX <= sX + 166) && (msY >= sY + 45 + i * 15) && (msY <= sY + 59 + i * 15))
                    {
                        if ((m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_bIsUseable == TRUE) &&
                            (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_iLevel != 0))
                        {

                            put_string(sX + 30, sY + 45 + i * 15, cTemp, Color(255, 255, 255));
                            put_string(sX + 183, sY + 45 + i * 15, cTemp2, Color(255, 255, 255));
                        }
                        else
                        {
                            put_string(sX + 30, sY + 45 + i * 15, cTemp, Color(5, 5, 5));
                            put_string(sX + 183, sY + 45 + i * 15, cTemp2, Color(5, 5, 5));
                        }
                    }
                    else
                    {
                        if ((m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_bIsUseable == TRUE) &&
                            (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_iLevel != 0))
                        {
                            put_string(sX + 30, sY + 45 + i * 15, cTemp, Color(34, 30, 120));
                            put_string(sX + 183, sY + 45 + i * 15, cTemp2, Color(34, 30, 120));
                        }
                        else
                        {
                            put_string(sX + 30, sY + 45 + i * 15, cTemp, Color(5, 5, 5));
                            put_string(sX + 183, sY + 45 + i * 15, cTemp2, Color(5, 5, 5));
                        }
                    }

                    if (m_iDownSkillIndex == (i + m_stDialogBoxInfo[15].sView))
                        m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_trans_sprite_color(sX + 215, sY + 47 + i * 15, 21, 50, 50, 50, m_dwTime);
                    else m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + 215, sY + 47 + i * 15, 20, m_dwTime);
                }

            iTotalLines = 0;
            for (i = 0; i < DEF_MAXSKILLTYPE; i++)
                if (m_pSkillCfgList[i] != NULL) iTotalLines++;

            
            if (iTotalLines > 17)
            {
                d1 = (double)m_stDialogBoxInfo[15].sView;
                d2 = (double)(iTotalLines - 17);
                d3 = (274.0f * d1) / d2;
                iPointerLoc = (int)d3;
            }
            else iPointerLoc = 0;
            if (iTotalLines > 17)
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 1);
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242, sY + iPointerLoc + 35, 7);
            }

            
            if (cLB != 0 && iTotalLines > 17)
            {
                if ((iGetTopDialogBoxIndex() == 15))
                {
                    if ((msX >= sX + 240) && (msX <= sX + 260) && (msY >= sY + 30) && (msY <= sY + 320))
                    {
                        d1 = (double)(msY - (sY + 35));
                        d2 = (double)(iTotalLines - 17);
                        d3 = (d1 * d2) / 274.0f;
                        iPointerLoc = (int)(d3 + 0.5);

                        if (iPointerLoc > iTotalLines - 17) iPointerLoc = iTotalLines - 17;
                        m_stDialogBoxInfo[15].sView = iPointerLoc;
                    }
                }
            }
            else m_stDialogBoxInfo[15].bIsScrollSelected = FALSE;
            if (iGetTopDialogBoxIndex() == 15 && msZ != 0)
            {
                if (msZ > 0) m_stDialogBoxInfo[15].sView--;
                if (msZ < 0) m_stDialogBoxInfo[15].sView++;
                m_stMCursor.sZ = 0;
            }
            if (m_stDialogBoxInfo[15].sView < 0) m_stDialogBoxInfo[15].sView = 0;
            if (iTotalLines > 17 && m_stDialogBoxInfo[15].sView > iTotalLines - 17) m_stDialogBoxInfo[15].sView = iTotalLines - 17;
            break;
    }
}

void CGame::DrawDialogBox_SkillDlg(short msX, short msY, short msZ, char cLB)
{
    int i, iLoc, iAdjX, iAdjY;
    char cTemp[120], cTemp2[120];
    short sX, sY, szX;
    char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
    DWORD dwTime = m_dwCurTime;

    iAdjX = 5;
    iAdjY = 8;

    switch (m_stDialogBoxInfo[26].cMode)
    {
        case 1:
            
            if (m_stDialogBoxInfo[26].cStr[0] != 0)
            {
                sX = m_stDialogBoxInfo[26].sX + iAdjX + (m_stDialogBoxInfo[26].cStr[0] - (rand() % (m_stDialogBoxInfo[26].cStr[0] * 2)));
                sY = m_stDialogBoxInfo[26].sY + iAdjY + (m_stDialogBoxInfo[26].cStr[0] - (rand() % (m_stDialogBoxInfo[26].cStr[0] * 2)));
            }
            else
            {
                sX = m_stDialogBoxInfo[26].sX;
                sY = m_stDialogBoxInfo[26].sY;
            }

            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX, sY, 1, dwTime);

            if (m_stDialogBoxInfo[26].sV1 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV1]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV1]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV2 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV2]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV2]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV3 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV3]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV3]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV4 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV4]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV4]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV5 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV5]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV5]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV6 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV6]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV6]->m_sSpriteFrame, dwTime);

            if ((msX >= sX + iAdjX + 60) && (msX <= sX + iAdjX + 153) && (msY >= sY + iAdjY + 175) && (msY <= sY + iAdjY + 195))
#if DEF_LANGUAGE == 1 //Taiwan
                put_string(sX + iAdjX + 60, sY + iAdjY + 175, "²{¦b´N¸Õ¸Õ¡T", Color(78, 64, 249));
            else put_string(sX + iAdjX + 60, sY + iAdjY + 175, "²{¦b´N¸Õ¸Õ¡T", Color(26, 11, 216));
#else
                put_string_sprite_font(sX + iAdjX + 60, sY + iAdjY + 175, "Try Now!", 16, 16, 30);
            else put_string_sprite_font(sX + iAdjX + 60, sY + iAdjY + 175, "Try Now!", 6, 6, 20);
#endif
            break;

        case 2:
            
            if (m_stDialogBoxInfo[26].cStr[0] != 0)
            {
                sX = m_stDialogBoxInfo[26].sX + iAdjX + (m_stDialogBoxInfo[26].cStr[0] - (rand() % (m_stDialogBoxInfo[26].cStr[0] * 2)));
                sY = m_stDialogBoxInfo[26].sY + iAdjY + (m_stDialogBoxInfo[26].cStr[0] - (rand() % (m_stDialogBoxInfo[26].cStr[0] * 2)));
            }
            else
            {
                sX = m_stDialogBoxInfo[26].sX;
                sY = m_stDialogBoxInfo[26].sY;
            }
            
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX, sY, 1, dwTime);

            if (m_stDialogBoxInfo[26].sV1 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV1]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV1]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV2 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV2]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV2]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV3 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV3]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 55 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV3]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV4 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV4]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV4]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV5 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV5]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + 45 * 1 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV5]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV6 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV6]->m_sSprite]->put_trans_sprite50(sX + iAdjX + 55 + 45 * 2 + (1 - (rand() % 3)), sY + iAdjY + 100 + (1 - (rand() % 3)),
                    m_pItemList[m_stDialogBoxInfo[26].sV6]->m_sSpriteFrame, dwTime);

            put_string_sprite_font(sX + iAdjX + 60, sY + iAdjY + 175, "Creating...", 20, 6, 6);


            if ((dwTime - m_stDialogBoxInfo[26].dwT1) > 1000)
            {
                m_stDialogBoxInfo[26].dwT1 = dwTime;
                m_stDialogBoxInfo[26].cStr[0]++;   
            }

            if (m_stDialogBoxInfo[26].cStr[0] >= 5)
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_CREATEPORTION, NULL, NULL, NULL, NULL, NULL);
                DisableDialogBox(26);
                PlaySound('E', 42, 0);
            }
            break;

        case 3:
            sX = m_stDialogBoxInfo[26].sX;
            sY = m_stDialogBoxInfo[26].sY;
            szX = m_stDialogBoxInfo[26].sSizeX;

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 0);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 8);

            
            put_string(sX + iAdjX + 44, sY + iAdjY + 38, "Name", Color(0, 0, 0));
            put_string(sX + iAdjX + 171, sY + iAdjY + 38, "Max.Skill", Color(0, 0, 0));

            iLoc = 0;
            for (i = 0; i < 13; i++)
                if (m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView] != NULL)
                {

                    ZeroMemory(cTemp, sizeof(cTemp));
                    GetItemName(m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView]->m_cName, NULL, cStr1, cStr2, cStr3, 64);
                    format_to_local(cTemp, "{}", cStr1);
                    ZeroMemory(cTemp2, sizeof(cTemp2));
                    format_to_local(cTemp2, "{}%", m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView]->m_iMaxSkill);

                    if ((msX >= sX + 30) && (msX <= sX + 180) && (msY >= sY + iAdjY + 55 + iLoc * 15) && (msY <= sY + iAdjY + 69 + iLoc * 15))
                    {
                        
                        put_string(sX + 30, sY + iAdjY + 55 + iLoc * 15, cTemp, Color(255, 255, 255));
                        put_string(sX + 190, sY + iAdjY + 55 + iLoc * 15, cTemp2, Color(255, 255, 255));
                    }
                    else
                    {
                        if (m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView]->m_bBuildEnabled == TRUE)
                        {
                            put_string(sX + 30, sY + iAdjY + 55 + iLoc * 15, cTemp, Color(34, 30, 120));
                            put_string(sX + 190, sY + iAdjY + 55 + iLoc * 15, cTemp2, Color(34, 30, 120));
                        }
                        else
                        {
                            put_string(sX + 30, sY + iAdjY + 55 + iLoc * 15, cTemp, Color(45, 25, 25));
                            put_string(sX + 190, sY + iAdjY + 55 + iLoc * 15, cTemp2, Color(45, 25, 25));
                        }
                    }

                    iLoc++;
                }

            
            if ((m_stDialogBoxInfo[26].sView >= 1) && (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView - 1] != NULL))
                m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + iAdjX + 225, sY + iAdjY + 210, 23, dwTime);
            else m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_trans_sprite_color(sX + iAdjX + 225, sY + iAdjY + 210, 23, 5, 5, 5, dwTime);

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView + 13] != NULL)
                m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + iAdjX + 225, sY + iAdjY + 230, 24, dwTime);
            else m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_trans_sprite_color(sX + iAdjX + 225, sY + iAdjY + 230, 24, 5, 5, 5, dwTime);

            if ((cLB != 0) && (iGetTopDialogBoxIndex() == 26))
            {
                if ((msX >= sX + iAdjX + 225) && (msX <= sX + iAdjX + 245) && (msY >= sY + iAdjY + 210) && (msY <= sY + iAdjY + 230))
                {
                    m_stDialogBoxInfo[26].sView--;
                }

                if ((msX >= sX + iAdjX + 225) && (msX <= sX + iAdjX + 245) && (msY >= sY + iAdjY + 230) && (msY <= sY + iAdjY + 250))
                {
                    if (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView + 13] != NULL)
                        m_stDialogBoxInfo[26].sView++;
                }
            }
            if ((msZ != 0) && (iGetTopDialogBoxIndex() == 26))
            {
                m_stDialogBoxInfo[26].sView = m_stDialogBoxInfo[26].sView - msZ / 60;
                m_stMCursor.sZ = 0;
            }
            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView + 12] == NULL)
            {
                while (1)
                {
                    m_stDialogBoxInfo[26].sView--;
                    if (m_stDialogBoxInfo[26].sView < 1) break;
                    if (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView + 12] != NULL) break;
                }
            }
            if (m_stDialogBoxInfo[26].sView < 0) m_stDialogBoxInfo[26].sView = 0;

            put_aligned_string(sX, sX + m_stDialogBoxInfo[26].sSizeX, sY + 265, DRAW_DIALOGBOX_SKILLDLG2, 55, 25, 25);
            put_aligned_string(sX, sX + m_stDialogBoxInfo[26].sSizeX, sY + 280, DRAW_DIALOGBOX_SKILLDLG3, 55, 25, 25);
            put_aligned_string(sX, sX + m_stDialogBoxInfo[26].sSizeX, sY + 295, DRAW_DIALOGBOX_SKILLDLG4, 55, 25, 25);
            put_aligned_string(sX, sX + m_stDialogBoxInfo[26].sSizeX, sY + 310, DRAW_DIALOGBOX_SKILLDLG5, 55, 25, 25);
            put_aligned_string(sX, sX + m_stDialogBoxInfo[26].sSizeX, sY + 340, DRAW_DIALOGBOX_SKILLDLG6, 55, 25, 25);
            break;

        case 4:
            sX = m_stDialogBoxInfo[26].sX;
            sY = m_stDialogBoxInfo[26].sY;
            szX = m_stDialogBoxInfo[26].sSizeX;
            iAdjX = -1;
            iAdjY = -7;
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 0);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 8);
            
            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iSprH]->put_sprite_fast(sX + iAdjX + 62 + 5, sY + iAdjY + 84 + 17,
                m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iSprFrame, dwTime);

            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cName, 0, cStr1, cStr2, cStr3, 64);
            format_to_local(cTemp, "{}", cStr1);
            put_string(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55, cTemp, Color(255, 255, 255));

            format_to_local(cTemp, DRAW_DIALOGBOX_SKILLDLG7, m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iSkillLimit, m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iMaxSkill);
            
            put_string(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55 + 2 * 15, cTemp, Color(45, 25, 25));
            put_string(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55 + 3 * 15 + 5, DRAW_DIALOGBOX_SKILLDLG8, Color(45, 25, 25));

            iLoc = 4;
            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[1] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName1, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[1] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[2] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName2, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[2] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[3] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName3, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[3] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[4] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName4, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[4] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[5] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName5, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[5] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[6] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName6, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[6] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bBuildEnabled == TRUE)
            {
                

               
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 45 * 1 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 45 * 2 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180, 2, dwTime);
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 45 * 1 + 13, sY + iAdjY + 100 + 180, 2, dwTime);
                m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 45 * 2 + 13, sY + iAdjY + 100 + 180, 2, dwTime);

                if (m_stDialogBoxInfo[26].sV1 != -1)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_stDialogBoxInfo[26].sV1]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180,
                        m_pItemList[m_stDialogBoxInfo[26].sV1]->m_sSpriteFrame, dwTime);

                if (m_stDialogBoxInfo[26].sV2 != -1)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_stDialogBoxInfo[26].sV2]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 55 + 180,
                        m_pItemList[m_stDialogBoxInfo[26].sV2]->m_sSpriteFrame, dwTime);

                if (m_stDialogBoxInfo[26].sV3 != -1)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_stDialogBoxInfo[26].sV3]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 55 + 180,
                        m_pItemList[m_stDialogBoxInfo[26].sV3]->m_sSpriteFrame, dwTime);

                if (m_stDialogBoxInfo[26].sV4 != -1)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_stDialogBoxInfo[26].sV4]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180,
                        m_pItemList[m_stDialogBoxInfo[26].sV4]->m_sSpriteFrame, dwTime);

                if (m_stDialogBoxInfo[26].sV5 != -1)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_stDialogBoxInfo[26].sV5]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 100 + 180,
                        m_pItemList[m_stDialogBoxInfo[26].sV5]->m_sSpriteFrame, dwTime);

                if (m_stDialogBoxInfo[26].sV6 != -1)
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                    m_pItemList[m_stDialogBoxInfo[26].sV6]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 100 + 180,
                        m_pItemList[m_stDialogBoxInfo[26].sV6]->m_sSpriteFrame, dwTime);

               
                put_aligned_string(sX, sX + szX, sY + iAdjY + 230 + 75, DRAW_DIALOGBOX_SKILLDLG15, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + iAdjY + 245 + 75, DRAW_DIALOGBOX_SKILLDLG16, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + iAdjY + 260 + 75, DRAW_DIALOGBOX_SKILLDLG17, 55, 25, 25);

                if ((msX >= sX + iAdjX + 32) && (msX <= sX + iAdjX + 95) && (msY >= sY + iAdjY + 353) && (msY <= sY + iAdjY + 372))
                    put_string_sprite_font(sX + iAdjX + 25, sY + iAdjY + 330 + 23, "Back", 6, 6, 20);
                else put_string_sprite_font(sX + iAdjX + 25, sY + iAdjY + 330 + 23, "Back", 0, 0, 7);


                if ((msX >= sX + iAdjX + 160) && (msX <= sX + iAdjX + 255) && (msY >= sY + iAdjY + 353) && (msY <= sY + iAdjY + 372))
                {
                    if (m_stDialogBoxInfo[26].cStr[4] == 1)
                        put_string_sprite_font(sX + iAdjX + 153, sY + iAdjY + 330 + 23, "Manufacture", 6, 6, 20);
                    else put_string_sprite_font(sX + iAdjX + 153, sY + iAdjY + 330 + 23, "Manufacture", 10, 10, 10);
                }
                else
                {
                    if (m_stDialogBoxInfo[26].cStr[4] == 1)
                        put_string_sprite_font(sX + iAdjX + 153, sY + iAdjY + 330 + 23, "Manufacture", 0, 0, 7);
                    else put_string_sprite_font(sX + iAdjX + 153, sY + iAdjY + 330 + 23, "Manufacture", 10, 10, 10);
                }
            }
            else
            {
                put_aligned_string(sX, sX + szX, sY + iAdjY + 200 + 75, DRAW_DIALOGBOX_SKILLDLG18, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + iAdjY + 215 + 75, DRAW_DIALOGBOX_SKILLDLG19, 55, 25, 25);
                put_aligned_string(sX, sX + szX, sY + iAdjY + 230 + 75, DRAW_DIALOGBOX_SKILLDLG20, 55, 25, 25);
                if ((msX >= sX + iAdjX + 32) && (msX <= sX + iAdjX + 95) && (msY >= sY + iAdjY + 353) && (msY <= sY + iAdjY + 372))
                    put_string_sprite_font(sX + iAdjX + 25, sY + iAdjY + 330 + 23, "Back", 6, 6, 20);
                else put_string_sprite_font(sX + iAdjX + 25, sY + iAdjY + 330 + 23, "Back", 0, 0, 7);
            }
            break;

        case 5:
            sX = m_stDialogBoxInfo[26].sX;
            sY = m_stDialogBoxInfo[26].sY;
            iAdjX = -1;
            iAdjY = -7;

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 0);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 8);

            
            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iSprH]->put_sprite_fast(sX + iAdjX + 62 + 5, sY + iAdjY + 84 + 17,
                m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iSprFrame, dwTime);

            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cName, 0, cStr1, cStr2, cStr3, 64);
            format_to_local(cTemp, "{}", cStr1);
            put_string(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55, cTemp, Color(255, 255, 255));

            format_to_local(cTemp, DRAW_DIALOGBOX_SKILLDLG7, m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iSkillLimit, m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iMaxSkill);
            put_string(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55 + 2 * 15, cTemp, Color(45, 25, 25));
            put_string(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55 + 3 * 15 + 5, DRAW_DIALOGBOX_SKILLDLG8, Color(45, 25, 25));

            iLoc = 4;
            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[1] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName1, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[1] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[2] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName2, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[2] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[3] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName3, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[3] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[4] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName4, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[4] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[5] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName5, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[5] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[6] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName6, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[6] == TRUE)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

           
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 45 * 1 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 45 * 2 + 13, sY + iAdjY + 55 + 180, 2, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180, 2, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 45 * 1 + 13, sY + iAdjY + 100 + 180, 2, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE]->put_sprite_fast(sX + iAdjX + 55 + 30 + 45 * 2 + 13, sY + iAdjY + 100 + 180, 2, dwTime);

            if (m_stDialogBoxInfo[26].sV1 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV1]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180,
                    m_pItemList[m_stDialogBoxInfo[26].sV1]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV2 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV2]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 55 + 180,
                    m_pItemList[m_stDialogBoxInfo[26].sV2]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV3 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV3]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 55 + 180,
                    m_pItemList[m_stDialogBoxInfo[26].sV3]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV4 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV4]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180,
                    m_pItemList[m_stDialogBoxInfo[26].sV4]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV5 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV5]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 100 + 180,
                    m_pItemList[m_stDialogBoxInfo[26].sV5]->m_sSpriteFrame, dwTime);

            if (m_stDialogBoxInfo[26].sV6 != -1)
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT +
                m_pItemList[m_stDialogBoxInfo[26].sV6]->m_sSprite]->put_sprite_fast(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 100 + 180,
                    m_pItemList[m_stDialogBoxInfo[26].sV6]->m_sSpriteFrame, dwTime);

           

            put_string(sX + iAdjX + 33, sY + iAdjY + 230 + 75, DRAW_DIALOGBOX_SKILLDLG29, Color(55, 25, 25));
            put_string(sX + iAdjX + 33, sY + iAdjY + 245 + 75, DRAW_DIALOGBOX_SKILLDLG30, Color(55, 25, 25));

            if ((dwTime - m_stDialogBoxInfo[26].dwT1) > 1000)
            {
                m_stDialogBoxInfo[26].dwT1 = dwTime;
                m_stDialogBoxInfo[26].cStr[1]++;   
                if (m_stDialogBoxInfo[26].cStr[1] >= 7) m_stDialogBoxInfo[26].cStr[1] = 7;
            }

            if (m_stDialogBoxInfo[26].cStr[1] == 4)
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_BUILDITEM, NULL, NULL, NULL, NULL, m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cName);
                m_stDialogBoxInfo[26].cStr[1]++;
            }
            break;

        case 6:
            sX = m_stDialogBoxInfo[26].sX;
            sY = m_stDialogBoxInfo[26].sY;
            iAdjX = -1;
            iAdjY = -7;

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 0);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 8);

            
            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iSprH]->put_sprite_fast(sX + iAdjX + 62 + 5, sY + iAdjY + 84 + 17,
                m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iSprFrame, dwTime);

            ZeroMemory(cTemp, sizeof(cTemp));
            GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cName, 0, cStr1, cStr2, cStr3, 64);

            format_to_local(cTemp, "{}", cStr1);
            put_string(sX + iAdjX + 44 + 10 + 60, sY + iAdjY + 55, cTemp, Color(255, 255, 255));

            if (m_stDialogBoxInfo[26].cStr[2] == 1)
            {
                
                put_string(sX + iAdjX + 33 + 11, sY + iAdjY + 200 - 45, DRAW_DIALOGBOX_SKILLDLG31, Color(55, 25, 25));

                if (m_stDialogBoxInfo[26].sV1 == DEF_ITEMTYPE_MATERIAL)
                {
                    format_to_local(G_cTxt, DRAW_DIALOGBOX_SKILLDLG32, m_stDialogBoxInfo[26].cStr[3]);
                    put_string(sX + iAdjX + 33 + 11, sY + iAdjY + 215 - 45, G_cTxt, Color(55, 25, 25));
                }
                else
                {
                    format_to_local(G_cTxt, DRAW_DIALOGBOX_SKILLDLG33, (int)m_stDialogBoxInfo[26].cStr[3] + 100);
                    put_string(sX + iAdjX + 33, sY + iAdjY + 215 - 45, G_cTxt, Color(55, 25, 25));
                }
            }
            else
            {
                
                put_string(sX + iAdjX + 33 + 11, sY + iAdjY + 200, DRAW_DIALOGBOX_SKILLDLG34, Color(55, 25, 25));
            }

            if ((msX >= sX + iAdjX + 32) && (msX <= sX + iAdjX + 95) && (msY >= sY + iAdjY + 353) && (msY <= sY + iAdjY + 372))
                put_string_sprite_font(sX + iAdjX + 35, sY + iAdjY + 330 + 23, "Back", 6, 6, 20);
            else put_string_sprite_font(sX + iAdjX + 35, sY + iAdjY + 330 + 23, "Back", 0, 0, 7);
            break;
    }
}

void CGame::DrawDialogBox_SysMenu(short msX, short msY, char cLB)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[19].sX;
    sY = m_stDialogBoxInfo[19].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME1, sX, sY, 0);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 6);

    put_string(sX + 23, sY + 63, DRAW_DIALOGBOX_SYSMENU_DETAILLEVEL, Color(45, 25, 25));
    put_string(sX + 24, sY + 63, DRAW_DIALOGBOX_SYSMENU_DETAILLEVEL, Color(45, 25, 25));

    if (m_cDetailLevel == 0)
        put_string(sX + 121, sY + 63, DRAW_DIALOGBOX_SYSMENU_LOW, Color(255, 255, 255));
    else put_string(sX + 121, sY + 63, DRAW_DIALOGBOX_SYSMENU_LOW, Color(45, 25, 25));
    if (m_cDetailLevel == 1)
        put_string(sX + 153, sY + 63, DRAW_DIALOGBOX_SYSMENU_NORMAL, Color(255, 255, 255));
    else put_string(sX + 153, sY + 63, DRAW_DIALOGBOX_SYSMENU_NORMAL, Color(45, 25, 25));
    if (m_cDetailLevel == 2)
        put_string(sX + 205, sY + 63, DRAW_DIALOGBOX_SYSMENU_HIGH, Color(255, 255, 255));
    else put_string(sX + 205, sY + 63, DRAW_DIALOGBOX_SYSMENU_HIGH, Color(45, 25, 25));

    put_string(sX + 23, sY + 84, DRAW_DIALOGBOX_SYSMENU_SOUND, Color(45, 25, 25));
    put_string(sX + 24, sY + 84, DRAW_DIALOGBOX_SYSMENU_SOUND, Color(45, 25, 25));
    if (m_bSoundFlag)
    {
        if (m_bSoundStat) put_string(sX + 85, sY + 85, DRAW_DIALOGBOX_SYSMENU_ON, Color(255, 255, 255));
        else put_string(sX + 83, sY + 85, DRAW_DIALOGBOX_SYSMENU_OFF, Color(200, 200, 200));
    }
    else put_string(sX + 68, sY + 85, DRAW_DIALOGBOX_SYSMENU_DISABLED, Color(100, 100, 100));

    put_string(sX + 123, sY + 84, DRAW_DIALOGBOX_SYSMENU_MUSIC, Color(45, 25, 25));
    put_string(sX + 124, sY + 84, DRAW_DIALOGBOX_SYSMENU_MUSIC, Color(45, 25, 25));
    if (m_bSoundFlag)
    {
        if (m_bMusicStat) put_string(sX + 180, sY + 85, DRAW_DIALOGBOX_SYSMENU_ON, Color(255, 255, 255));
        else put_string(sX + 178, sY + 85, DRAW_DIALOGBOX_SYSMENU_OFF, Color(200, 200, 200));
    }
    else put_string(sX + 163, sY + 85, DRAW_DIALOGBOX_SYSMENU_DISABLED, Color(100, 100, 100));

    
    put_string(sX + 23, sY + 106, DRAW_DIALOGBOX_SYSMENU_WHISPER, Color(45, 25, 25));
    put_string(sX + 24, sY + 106, DRAW_DIALOGBOX_SYSMENU_WHISPER, Color(45, 25, 25));
    if (m_bWhisper) put_string(sX + 85, sY + 106, DRAW_DIALOGBOX_SYSMENU_ON, Color(255, 255, 255));
    else put_string(sX + 82, sY + 106, DRAW_DIALOGBOX_SYSMENU_OFF, Color(200, 200, 200));

    put_string(sX + 123, sY + 106, DRAW_DIALOGBOX_SYSMENU_SHOUT, Color(45, 25, 25));
    put_string(sX + 124, sY + 106, DRAW_DIALOGBOX_SYSMENU_SHOUT, Color(45, 25, 25));
    if (m_bShout) put_string(sX + 180, sY + 106, DRAW_DIALOGBOX_SYSMENU_ON, Color(255, 255, 255));
    else put_string(sX + 177, sY + 106, DRAW_DIALOGBOX_SYSMENU_OFF, Color(200, 200, 200));

    put_string(sX + 23, sY + 124, DRAW_DIALOGBOX_SYSMENU_SOUNDVOLUME, Color(45, 25, 25));
    put_string(sX + 24, sY + 124, DRAW_DIALOGBOX_SYSMENU_SOUNDVOLUME, Color(45, 25, 25));
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 130 + m_cSoundVolume, sY + 129, 8);

    put_string(sX + 23, sY + 141, DRAW_DIALOGBOX_SYSMENU_MUSICVOLUME, Color(45, 25, 25));
    put_string(sX + 24, sY + 141, DRAW_DIALOGBOX_SYSMENU_MUSICVOLUME, Color(45, 25, 25));
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 130 + m_cMusicVolume, sY + 145, 8);

    put_string(sX + 23, sY + 158, DRAW_DIALOGBOX_SYSMENU_TRANSPARENCY, Color(45, 25, 25));
    put_string(sX + 24, sY + 158, DRAW_DIALOGBOX_SYSMENU_TRANSPARENCY, Color(45, 25, 25));
    if (m_bDialogTrans) put_string(sX + 208, sY + 158, DRAW_DIALOGBOX_SYSMENU_ON, Color(255, 255, 255));
    else put_string(sX + 207, sY + 158, DRAW_DIALOGBOX_SYSMENU_OFF, Color(200, 200, 200));

    put_string(sX + 23, sY + 180, DRAW_DIALOGBOX_SYSMENU_GUIDEMAP, Color(45, 25, 25));
    put_string(sX + 24, sY + 180, DRAW_DIALOGBOX_SYSMENU_GUIDEMAP, Color(45, 25, 25));
    if (m_bIsDialogEnabled[9]) put_string(sX + 208, sY + 180, DRAW_DIALOGBOX_SYSMENU_ON, Color(255, 255, 255));
    else put_string(sX + 207, sY + 180, DRAW_DIALOGBOX_SYSMENU_OFF, Color(200, 200, 200));

   
    SYSTEMTIME SysTime;
    GetLocalTime(&SysTime);
    ZeroMemory(G_cTxt, sizeof(G_cTxt));
    format_to_local(G_cTxt, "{}:{}:{}:{}:{}", SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
    put_string(sX + 23, sY + 204, G_cTxt, Color(45, 25, 25));
    put_string(sX + 24, sY + 204, G_cTxt, Color(45, 25, 25));

   
    put_string(sX + 23, sY + 46, MSG_WORLDNAME1, Color(45, 25, 25));
    put_string(sX + 24, sY + 46, MSG_WORLDNAME1, Color(45, 25, 25));


    if ((cLB != 0) && (iGetTopDialogBoxIndex() == 19))
    {
        //  (msX >= sX + 120) && (msX <= sX + 242) && (msY >= sY +122) && (msY <= sY +138)
        if ((msX >= sX + 127) && (msX <= sX + 238) && (msY >= sY + 122) && (msY <= sY + 138))
        {
            m_cSoundVolume = msX - (sX + 127);
            if (m_cSoundVolume > 100) m_cSoundVolume = 100;
            if (m_cSoundVolume < 0) m_cSoundVolume = 0;
        }
        if ((msX >= sX + 127) && (msX <= sX + 238) && (msY >= sY + 139) && (msY <= sY + 155))
        {
            m_cMusicVolume = msX - (sX + 127);
            if (m_cMusicVolume > 100) m_cMusicVolume = 100;
            if (m_cMusicVolume < 0) m_cMusicVolume = 0;
            if (m_bSoundFlag)
            {
                //todo: sound volume control
                int iVol;
                iVol = (m_cMusicVolume - 100) * 20;
                if (iVol > 0) iVol = 0;
                if (iVol < -10000) iVol = -10000;

                StartBGM();
            }
        }
    }
    else m_stDialogBoxInfo[19].bIsScrollSelected = FALSE;

    // Log-Out
    if (m_cLogOutCount == -1)
    {
        if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + 225) && (msY <= sY + 225 + DEF_BTNSZY))
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + 225, 9);
        else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + 225, 8);
    }
    else
    { //Continue
        if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + 225) && (msY <= sY + 225 + DEF_BTNSZY))
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + 225, 7);
        else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + 225, 6);
    }
    // Restart
    if ((m_iHP <= 0) && (m_cRestartCount == -1))
    {
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 225) && (msY <= sY + 225 + DEF_BTNSZY))
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + 225, 37);
        else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + 225, 36);
    }
}

void CGame::DrawDialogBox_CrusadeJob(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[33].sX;
    sY = m_stDialogBoxInfo[33].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);

    switch (m_stDialogBoxInfo[33].cMode)
    {
        case 1:
            put_aligned_string(sX + 24, sX + 246, sY + 45 + 20, DRAWDIALOGBOX_CRUSADEJOB1);
            put_aligned_string(sX + 24, sX + 246, sY + 60 + 20, DRAWDIALOGBOX_CRUSADEJOB2);
            put_aligned_string(sX + 24, sX + 246, sY + 75 + 20, DRAWDIALOGBOX_CRUSADEJOB3);
            put_aligned_string(sX + 24, sX + 246, sY + 90 + 20, DRAWDIALOGBOX_CRUSADEJOB4);

            if (m_bCitizen == TRUE)
            {
                if (m_bAresden)
                {
                    
                    if (m_iGuildRank == 0)
                    {
                        
                        if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                            put_aligned_string(sX + 24, sX + 246, sY + 150, DRAWDIALOGBOX_CRUSADEJOB5, 255, 255, 255);
                        else put_aligned_string(sX + 24, sX + 246, sY + 150, DRAWDIALOGBOX_CRUSADEJOB5, 4, 0, 50);
                    }
                    else
                    {
                        
                        if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                            put_aligned_string(sX + 24, sX + 246, sY + 150, DRAWDIALOGBOX_CRUSADEJOB7, 255, 255, 255);
                        else put_aligned_string(sX + 24, sX + 246, sY + 150, DRAWDIALOGBOX_CRUSADEJOB7, 4, 0, 50);

                        
                        if (m_iGuildRank != -1)
                        {
                            if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 175) && (msY < sY + 190))
                                put_aligned_string(sX + 24, sX + 246, sY + 175, DRAWDIALOGBOX_CRUSADEJOB9, 255, 255, 255);
                            else put_aligned_string(sX + 24, sX + 246, sY + 175, DRAWDIALOGBOX_CRUSADEJOB9, 4, 0, 50);
                        }
                    }
                }
                else if (m_bAresden == FALSE)
                {
                    
                    if (m_iGuildRank == 0)
                    {
                        
                        if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                            put_aligned_string(sX + 24, sX + 246, sY + 150, DRAWDIALOGBOX_CRUSADEJOB5, 255, 255, 255);
                        else put_aligned_string(sX + 24, sX + 246, sY + 150, DRAWDIALOGBOX_CRUSADEJOB5, 4, 0, 50);
                    }
                    else
                    {
                        
                        if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                            put_aligned_string(sX + 24, sX + 246, sY + 150, DRAWDIALOGBOX_CRUSADEJOB7, 255, 255, 255);
                        else put_aligned_string(sX + 24, sX + 246, sY + 150, DRAWDIALOGBOX_CRUSADEJOB7, 4, 0, 50);

                        
                        if (m_iGuildRank != -1)
                        {
                            if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 175) && (msY < sY + 190))
                                put_aligned_string(sX + 24, sX + 246, sY + 175, DRAWDIALOGBOX_CRUSADEJOB9, 255, 255, 255);
                            else put_aligned_string(sX + 24, sX + 246, sY + 175, DRAWDIALOGBOX_CRUSADEJOB9, 4, 0, 50);
                        }
                    }
                }
            }

            put_aligned_string(sX + 24, sX + 246, sY + 290 - 40, DRAWDIALOGBOX_CRUSADEJOB10);
            put_aligned_string(sX + 24, sX + 246, sY + 305 - 40, DRAWDIALOGBOX_CRUSADEJOB17);

            if ((msX > sX + 210) && (msX < sX + 260) && (msY >= sY + 296) && (msY <= sY + 316))
                put_string_sprite_font(sX + 50 + 160, sY + 296, "Help", 6, 6, 20);
            else put_string_sprite_font(sX + 50 + 160, sY + 296, "Help", 0, 0, 7);
            break;

        case 2:
            put_aligned_string(sX + 24, sX + 246, sY + 90 + 20, DRAWDIALOGBOX_CRUSADEJOB18);
            
            switch (m_iCrusadeDuty)
            {
                case 1: put_aligned_string(sX + 24, sX + 246, sY + 125, DRAWDIALOGBOX_CRUSADEJOB19); break;
                case 2: put_aligned_string(sX + 24, sX + 246, sY + 125, DRAWDIALOGBOX_CRUSADEJOB20); break;
                case 3: put_aligned_string(sX + 24, sX + 246, sY + 125, DRAWDIALOGBOX_CRUSADEJOB21); break;
            }

            put_aligned_string(sX + 24, sX + 246, sY + 145, DRAWDIALOGBOX_CRUSADEJOB22);
            if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 160) && (msY < sY + 175))
                put_aligned_string(sX + 24, sX + 246, sY + 160, DRAWDIALOGBOX_CRUSADEJOB23, 255, 255, 255);
            else put_aligned_string(sX + 24, sX + 246, sY + 160, DRAWDIALOGBOX_CRUSADEJOB23, 4, 0, 50);

            put_aligned_string(sX + 24, sX + 246, sY + 175, DRAWDIALOGBOX_CRUSADEJOB25);
            put_aligned_string(sX + 24, sX + 246, sY + 190, DRAWDIALOGBOX_CRUSADEJOB26);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
    }
}

void CGame::DrawDialogBox_Commander(int msX, int msY)
{
    short sX, sY, szX, szY, MapSzX, MapSzY;
    DWORD dwTime = G_dwGlobalTime;
    double dV1, dV2, dV3;
    int i, tX, tY;

    sX = m_stDialogBoxInfo[36].sX;
    sY = m_stDialogBoxInfo[36].sY;
    szX = m_stDialogBoxInfo[36].sSizeX;

    if ((dwTime - m_dwCommanderCommandRequestedTime) > 1000 * 10)
    {
        
        _RequestMapStatus("middleland", 3);
        _RequestMapStatus("middleland", 1);
        m_dwCommanderCommandRequestedTime = dwTime;
    }

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 0, FALSE, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 15, FALSE, m_bDialogTrans);

    switch (m_stDialogBoxInfo[36].cMode)
    {
        case 0: 
            
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 322, 3, dwTime); 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 322, 1, dwTime); 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100, sY + 322, 2, dwTime); 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150, sY + 322, 30, dwTime); 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 
            put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER1);

            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 322, 17, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER2, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 322, 15, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER3, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 46 + 100) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100, sY + 322, 16, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER4, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 46 + 150) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150, sY + 322, 24, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER5, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER6, 255, 255, 255);
            }

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, FALSE, m_bDialogTrans);
            break;

        case 1: 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 20, dwTime); 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 
            put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER7);

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 19, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER8, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER9, 255, 255, 255);
            }

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, FALSE, m_bDialogTrans);

            if ((msX >= sX + 15) && (msX <= sX + 15 + 280) && (msY >= sY + 60) && (msY <= sY + 60 + 253))
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, msX, msY, 42, FALSE, TRUE);
            }
            break;

        case 2: 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 322, 1, dwTime); 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 20, dwTime); 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 

            put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER10);

            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 322, 15, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER11, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 19, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER12, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER13, 255, 255, 255);
            }

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, FALSE, m_bDialogTrans);
            break;

        case 3: 
            if ((m_bCitizen == TRUE) && (m_bAresden == TRUE))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 220, 6, dwTime); 
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 220, 5, dwTime); 
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100, sY + 220, 7, dwTime); 
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150, sY + 220, 35, dwTime); 
            }
            else if ((m_bCitizen == TRUE) && (m_bAresden == FALSE))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 220, 9, dwTime); 
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 220, 8, dwTime); 
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100, sY + 220, 7, dwTime); 
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150, sY + 220, 35, dwTime); 
            }

            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 20, dwTime); 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 

            put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER14);

            format_to_local(G_cTxt, "{} {}", DRAW_DIALOGBOX_COMMANDER15, m_iConstructionPoint);
            put_aligned_string(sX, sX + 323, sY + 190, G_cTxt);

            if ((m_bCitizen == TRUE) && (m_bAresden == TRUE))
            {
                
                if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {

                    if (m_iConstructionPoint >= 3000)
                    {
                        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 220, 11, dwTime); 
                    }
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER16, 255, 255, 255);
                    put_string2(msX + 20, msY + 50, DRAW_DIALOGBOX_COMMANDER17, 255, 255, 255);

                }
                else if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {

                    if (m_iConstructionPoint >= 2000)
                    {
                        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 220, 10, dwTime); 
                    }
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER18, 255, 255, 255);
                    put_string2(msX + 20, msY + 50, DRAW_DIALOGBOX_COMMANDER19, 255, 255, 255);

                }
                else if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {

                    if (m_iConstructionPoint >= 1000)
                    {
                        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100, sY + 220, 12, dwTime); 
                    }
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER20, 255, 255, 255);
                    put_string2(msX + 20, msY + 50, DRAW_DIALOGBOX_COMMANDER21, 255, 255, 255);
                }
                else if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {

                    if (m_iConstructionPoint >= 5000)
                    {
                        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150, sY + 220, 29, dwTime); 
                    }
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER22, 255, 255, 255);
                    put_string2(msX + 20, msY + 50, DRAW_DIALOGBOX_COMMANDER23, 255, 255, 255);
                }
                else if ((msX >= sX + 20) && (msX <= sX + 380) && (msY > sY + 140) && (msY < sY + 160))
                {
                    
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER24, 255, 255, 255);
                }
                else if ((msX >= sX + 20) && (msX <= sX + 380) && (msY > sY + 160) && (msY < sY + 175))
                {
                    
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER25, 255, 255, 255);
                }
                else if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
                {
                    
                    m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 19, dwTime); 
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER26, 255, 255, 255);

                }
                else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
                {
                    
                    m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER27, 255, 255, 255);
                }
            }
            else if ((m_bCitizen == TRUE) && (m_bAresden == FALSE))
            {
                
                if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {
                    

                    if (m_iConstructionPoint >= 3000)
                    {
                        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 220, 14, dwTime); 
                    }
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER28, 255, 255, 255);
                    put_string2(msX + 20, msY + 50, DRAW_DIALOGBOX_COMMANDER29, 255, 255, 255);
                }
                else if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {
                    

                    if (m_iConstructionPoint >= 2000)
                    {
                        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 220, 13, dwTime); 
                    }
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER30, 255, 255, 255);
                    put_string2(msX + 20, msY + 50, DRAW_DIALOGBOX_COMMANDER31, 255, 255, 255);
                }
                else if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {
                    

                    if (m_iConstructionPoint >= 1000)
                    {
                        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100, sY + 220, 12, dwTime); 
                    }
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER32, 255, 255, 255);
                    put_string2(msX + 20, msY + 50, DRAW_DIALOGBOX_COMMANDER33, 255, 255, 255);
                }
                else if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
                {

                    if (m_iConstructionPoint >= 5000)
                    {
                        m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150, sY + 220, 29, dwTime); 
                    }
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER34, 255, 255, 255);
                    put_string2(msX + 20, msY + 50, DRAW_DIALOGBOX_COMMANDER35, 255, 255, 255);
                }
                else if ((msX >= sX + 20) && (msX <= sX + 380) && (msY > sY + 140) && (msY < sY + 160))
                {
                    
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER36, 255, 255, 255);
                }
                else if ((msX >= sX + 20) && (msX <= sX + 380) && (msY > sY + 160) && (msY < sY + 175))
                {
                    
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER37, 255, 255, 255);
                }
                else if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
                {
                    
                    m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 19, dwTime); 
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER38, 255, 255, 255);

                }
                else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
                {
                    
                    m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
                    put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER39, 255, 255, 255);
                }
            }

            put_aligned_string(sX, sX + 323, sY + 80, DRAW_DIALOGBOX_COMMANDER40);
            put_aligned_string(sX, sX + 323, sY + 95, DRAW_DIALOGBOX_COMMANDER41);
            put_aligned_string(sX, sX + 323, sY + 110, DRAW_DIALOGBOX_COMMANDER42);

            switch (m_stDialogBoxInfo[36].sV1)
            {
                case 0:
                    
                    put_aligned_string(sX, sX + 323, sY + 140, DRAW_DIALOGBOX_COMMANDER43, 255, 255, 255);//"Guard Mode"
                    put_aligned_string(sX, sX + 323, sY + 160, DRAW_DIALOGBOX_COMMANDER44, 4, 0, 50);//"Hold Mode"
                    break;
                case 1:
                    
                    put_aligned_string(sX, sX + 323, sY + 140, DRAW_DIALOGBOX_COMMANDER43, 4, 0, 50);//"Guard Mode"
                    put_aligned_string(sX, sX + 323, sY + 160, DRAW_DIALOGBOX_COMMANDER44, 255, 255, 255);//"Hold Mode"
                    break;
            }
            break;

        case 4: 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 20, dwTime); 
            m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 

            put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_COMMANDER47);

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 19, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER48, 255, 255, 255);

            }
            else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_COMMANDER49, 255, 255, 255);
            }

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21);
            if ((msX >= sX + 15) && (msX <= sX + 15 + 280) && (msY >= sY + 60) && (msY <= sY + 60 + 253))
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, msX, msY, 41, FALSE, TRUE);
            }
            break;
    }

    
    switch (m_stDialogBoxInfo[36].cMode)
    {
        case 0:
        case 1:
        case 2:
        case 4:
            szX = NULL;
            szY = NULL;
            MapSzX = NULL;
            MapSzY = NULL;

            if (strcmp(m_cStatusMapName, "aresden") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "elvine") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "middleland") == 0)
            {
                szX = 280;
                szY = 253;
                MapSzX = 752;
                MapSzY = 680;
            }

            if (szX != NULL)
            {
                
                for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
                    if (m_stCrusadeStructureInfo[i].cType != NULL)
                    {
                        dV1 = (double)MapSzX;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sX;
                        dV3 = (dV2 * (double)szX) / dV1;
                        tX = (int)dV3;

                        dV1 = (double)MapSzY;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sY;
                        dV3 = (dV2 * (double)szY) / dV1;
                        tY = (int)dV3;

                        switch (m_stCrusadeStructureInfo[i].cType)
                        {
                            case 38:
                                if (m_stCrusadeStructureInfo[i].cSide == 1)
                                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 39, FALSE, TRUE);
                                else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 37, FALSE, TRUE);
                                break;

                            case 36:
                            case 37:
                            case 39:
                                if (m_stCrusadeStructureInfo[i].cSide == 1)
                                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 38, FALSE, TRUE);
                                else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 36, FALSE, TRUE);
                                break;
                            case 42:
                                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 40);
                                break;
                        }
                    }

                
                if (m_iTeleportLocX != -1)
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iTeleportLocX;
                    dV3 = (dV2 * (double)szX) / dV1;
                    tX = (int)dV3;

                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iTeleportLocY;
                    dV3 = (dV2 * (double)szY) / dV1;
                    tY = (int)dV3;

                    if ((m_stDialogBoxInfo[36].cMode == 1) && (tY >= 100) && (tY <= 724))
                    {
                        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, FALSE, TRUE);
                    }
                    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, FALSE, TRUE);
                }

                
                if ((m_stDialogBoxInfo[36].cMode != 2) && (m_iConstructLocX != -1))
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iConstructLocX;
                    dV3 = (dV2 * (double)szX) / dV1;
                    tX = (int)dV3;

                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iConstructLocY;
                    dV3 = (dV2 * (double)szY) / dV1;
                    tY = (int)dV3;

                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 41, FALSE, TRUE);
                }

                
                if (strcmp(m_cMapName, "middleland") == 0)
                {
                    dV1 = (double)m_pMapData->m_sMapSizeX;
                    dV2 = (double)m_sPlayerX;
                    dV3 = (dV2 * (double)szX) / dV1;
                    tX = (int)dV3;

                    dV1 = (double)m_pMapData->m_sMapSizeY;
                    if (dV1 == 752) dV1 = 680;
                    dV2 = (double)m_sPlayerY;
                    dV3 = (dV2 * (double)szY) / dV1;
                    tY = (int)dV3;

                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 43);
                }
            }
            break;
    }

    switch (m_stDialogBoxInfo[36].cMode)
    {
        case 0:
        case 1:
        case 4:
            
            if ((msX >= sX + 15) && (msX <= sX + 15 + 280) && (msY >= sY + 60) && (msY <= sY + 60 + 253))
            {
                dV1 = (double)(msX - (sX + 15));
                dV2 = (double)(752.0f); 
                dV3 = (dV2 * dV1) / 280.0f;
                tX = (int)dV3;

                dV1 = (double)(msY - (sY + 60));
                dV2 = (double)(680.0f); 
                dV3 = (dV2 * dV1) / 253.0f;
                tY = (int)dV3;

                if (tX < 30) tX = 30;
                if (tY < 30) tY = 30;
                if (tX > 722) tX = 722;
                if (tY > 650) tY = 650;

                format_to_local(G_cTxt, "{},{}", tX, tY);
                put_string_sprite_font3(msX + 10, msY - 10, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, FALSE, 2);
            }
            break;
    }
}

void CGame::DrawDialogBox_Constructor(int msX, int msY)
{
    short sX, sY, szX, szY, MapSzX, MapSzY;
    DWORD dwTime = G_dwGlobalTime;
    double dV1, dV2, dV3;
    int tX, tY;
    char cMapName[12];

    sX = m_stDialogBoxInfo[37].sX;
    sY = m_stDialogBoxInfo[37].sY;
    szX = m_stDialogBoxInfo[37].sSizeX;

    if ((dwTime - m_dwCommanderCommandRequestedTime) > 1000 * 10)
    {
        
        _RequestMapStatus("middleland", 1);
        m_dwCommanderCommandRequestedTime = dwTime;
    }

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 0, FALSE, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 16, FALSE, m_bDialogTrans);

    switch (m_stDialogBoxInfo[37].cMode)
    {
        case 0: 
            if (m_iConstructLocX != -1)
            {
                ZeroMemory(cMapName, sizeof(cMapName));
                GetOfficialMapName(m_cConstructMapName, cMapName);
                format_to_local(G_cTxt, DRAW_DIALOGBOX_CONSTRUCTOR1, cMapName, m_iConstructLocX, m_iConstructLocY);
                put_aligned_string(sX, sX + szX, sY + 40, G_cTxt);
            }
            else put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_CONSTRUCTOR2);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, FALSE, m_bDialogTrans);
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 322, 24, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 322, 30, dwTime); 

            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 322, 15, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 322, 1, dwTime); 

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 

            
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR3, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR4, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR5, 255, 255, 255);
            }
            break;

        case 1: 
            put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_CONSTRUCTOR6);
            put_aligned_string(sX, sX + 323, sY + 80, DRAW_DIALOGBOX_CONSTRUCTOR7);
            put_aligned_string(sX, sX + 323, sY + 95, DRAW_DIALOGBOX_CONSTRUCTOR8);
            put_aligned_string(sX, sX + 323, sY + 110, DRAW_DIALOGBOX_CONSTRUCTOR9);
            put_aligned_string(sX, sX + 323, sY + 125, DRAW_DIALOGBOX_CONSTRUCTOR10);
            put_aligned_string(sX, sX + 323, sY + 140, DRAW_DIALOGBOX_CONSTRUCTOR11);
            put_aligned_string(sX, sX + 323, sY + 155, DRAW_DIALOGBOX_CONSTRUCTOR12);

            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 220, 27, dwTime);		
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 220, 33, dwTime);		

            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 220, 28, dwTime);	
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 220, 34, dwTime);	

            if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100, sY + 220, 26, dwTime);	
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100, sY + 220, 32, dwTime);	

            if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150, sY + 220, 25, dwTime);	
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150, sY + 220, 31, dwTime);	

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 19, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 20, dwTime); 

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 

            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR13, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR14, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR15, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR16, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR17, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR18, 255, 255, 255);
            }
            break;

        case 2: 
            put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_CONSTRUCTOR19);

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, FALSE, m_bDialogTrans);

            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 322, 15, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 322, 1, dwTime); 

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 19, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100 + 74, sY + 322, 20, dwTime); 

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 

            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR20, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR21, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_CONSTRUCTOR22, 255, 255, 255);
            }
            break;
    }

    
    switch (m_stDialogBoxInfo[37].cMode)
    {
        case 0:
        case 2:
            szX = NULL;
            szY = NULL;
            MapSzX = NULL;
            MapSzY = NULL;

            if (strcmp(m_cStatusMapName, "aresden") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "elvine") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "middleland") == 0)
            {
                szX = 280;
                szY = 253;
                MapSzX = 752;
                MapSzY = 680;
            }

            if (szX != 0)
            {
                for (int i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
                    if (m_stCrusadeStructureInfo[i].cType == 42)
                    {
                        dV1 = (double)MapSzX;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sX;
                        dV3 = (dV2 * (double)szX) / dV1;
                        tX = (int)dV3;

                        dV1 = (double)MapSzY;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sY;
                        dV3 = (dV2 * (double)szY) / dV1;
                        tY = (int)dV3;

                        switch (m_stCrusadeStructureInfo[i].cType)
                        {
                            case 42:
                                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 40);
                                break;
                        }
                    }
            }

            
            if (m_iTeleportLocX != -1)
            {

                if (szX != NULL)
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iTeleportLocX;
                    dV3 = (dV2 * (double)szX) / dV1;
                    tX = (int)dV3;

                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iTeleportLocY;
                    dV3 = (dV2 * (double)szY) / dV1;
                    tY = (int)dV3;

                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, FALSE, TRUE);
                }
            }

            
            if ((m_stDialogBoxInfo[37].cMode != 2) && (m_iConstructLocX != -1))
            {

                if (szX != 0)
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iConstructLocX;
                    dV3 = (dV2 * (double)szX) / dV1;
                    tX = (int)dV3;

                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iConstructLocY;
                    dV3 = (dV2 * (double)szY) / dV1;
                    tY = (int)dV3;

                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 41, FALSE, TRUE);
                }
            }

            
            if (strcmp(m_cMapName, "middleland") == 0)
            {
                dV1 = (double)m_pMapData->m_sMapSizeX;
                dV2 = (double)m_sPlayerX;
                dV3 = (dV2 * (double)280.0f) / dV1;
                tX = (int)dV3;

                dV1 = (double)m_pMapData->m_sMapSizeY;
                if (dV1 == 752) dV1 = 680;
                dV2 = (double)m_sPlayerY;
                dV3 = (dV2 * (double)253.0f) / dV1;
                tY = (int)dV3;

                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 43);
            }

            if ((msX >= sX + 15) && (msX <= sX + 15 + 280) && (msY >= sY + 60) && (msY <= sY + 60 + 253))
            {
                dV1 = (double)(msX - (sX + 15));
                dV2 = (double)(752.0f); 
                dV3 = (dV2 * dV1) / 280.0f;
                tX = (int)dV3;

                dV1 = (double)(msY - (sY + 60));
                dV2 = (double)(680.0f); 
                dV3 = (dV2 * dV1) / 253.0f;
                tY = (int)dV3;

                if (tX < 30) tX = 30;
                if (tY < 30) tY = 30;
                if (tX > 722) tX = 722;
                if (tY > 650) tY = 650;

                format_to_local(G_cTxt, "{},{}", tX, tY);
                put_string_sprite_font3(msX + 10, msY - 10, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, FALSE, 2);
            }
            break;
    }
}

void CGame::DrawDialogBox_Soldier(int msX, int msY)
{
    short sX, sY, szX, szY, MapSzX, MapSzY;
    DWORD dwTime = G_dwGlobalTime;
    char cMapName[120];
    double dV1, dV2, dV3;
    int tX, tY;

    sX = m_stDialogBoxInfo[38].sX;
    sY = m_stDialogBoxInfo[38].sY;
    szX = m_stDialogBoxInfo[38].sSizeX;

    if ((dwTime - m_dwCommanderCommandRequestedTime) > 1000 * 10)
    {
        
        _RequestMapStatus("middleland", 1);
        m_dwCommanderCommandRequestedTime = dwTime;
    }

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 0, FALSE, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, FALSE, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 17, FALSE, m_bDialogTrans); // Crusade Soldier Menu Text

    switch (m_stDialogBoxInfo[38].cMode)
    {
        case 0: 
            

            if (m_iTeleportLocX != -1)
            {
                ZeroMemory(cMapName, sizeof(cMapName));
                GetOfficialMapName(m_cTeleportMapName, cMapName);
                format_to_local(G_cTxt, DRAW_DIALOGBOX_SOLDIER1, cMapName, m_iTeleportLocX, m_iTeleportLocY);
                put_aligned_string(sX, sX + szX, sY + 40, G_cTxt);
            }
            else put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_SOLDIER2);

            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 322, 15, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 322, 1, dwTime); 

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 

            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_SOLDIER3, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_SOLDIER4, 255, 255, 255);
            }
            break;

        case 1: 
            put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_SOLDIER5);
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 322, 15, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 322, 1, dwTime); 

            if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74 - 50, sY + 322, 19, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74 - 50, sY + 322, 20, dwTime); 

            if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 18, dwTime); 
            }
            else m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150 + 74, sY + 322, 4, dwTime); 

            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_SOLDIER6, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74 - 50) && (msX <= sX + 20 + 46 + 150 + 74 - 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_SOLDIER7, 255, 255, 255);
            }
            else if ((msX >= sX + 20 + 150 + 74) && (msX <= sX + 20 + 46 + 150 + 74) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
            {
                put_string2(msX + 20, msY + 35, DRAW_DIALOGBOX_SOLDIER8, 255, 255, 255);
            }
            break;
    }

    
    switch (m_stDialogBoxInfo[38].cMode)
    {
        case 0:
        case 1:
            szX = NULL;
            szY = NULL;
            MapSzX = NULL;
            MapSzY = NULL;

            
            
            if (strcmp(m_cStatusMapName, "aresden") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "elvine") == 0)
            {
                szX = 250;
                szY = 250;
            }
            else if (strcmp(m_cStatusMapName, "middleland") == 0)
            {
                szX = 280;
                szY = 253;
                MapSzX = 752;
                MapSzY = 680;
            }

            if (szX != 0)
            {
                for (int i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
                    if (m_stCrusadeStructureInfo[i].cType == 42)
                    {
                        dV1 = (double)MapSzX;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sX;
                        dV3 = (dV2 * (double)szX) / dV1;
                        tX = (int)dV3;

                        dV1 = (double)MapSzY;
                        dV2 = (double)m_stCrusadeStructureInfo[i].sY;
                        dV3 = (dV2 * (double)szY) / dV1;
                        tY = (int)dV3;

                        switch (m_stCrusadeStructureInfo[i].cType)
                        {
                            case 42:
                                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 40);
                                break;
                        }
                    }
            }

            
            if (m_iTeleportLocX != -1)
            {

                if (szX != NULL)
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iTeleportLocX;
                    dV3 = (dV2 * (double)szX) / dV1;
                    tX = (int)dV3;

                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iTeleportLocY;
                    dV3 = (dV2 * (double)szY) / dV1;
                    tY = (int)dV3;

                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, FALSE, TRUE);
                }
            }



            
            if (strcmp(m_cMapName, "middleland") == 0)
            {
                dV1 = (double)m_pMapData->m_sMapSizeX;
                dV2 = (double)m_sPlayerX;
                dV3 = (dV2 * (double)280.0f) / dV1;
                tX = (int)dV3;

                dV1 = (double)m_pMapData->m_sMapSizeY;
                if (dV1 == 752) dV1 = 680;
                dV2 = (double)m_sPlayerY;
                dV3 = (dV2 * (double)253.0f) / dV1;
                tY = (int)dV3;

                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 43);
            }

            if ((msX >= sX + 15) && (msX <= sX + 15 + 280) && (msY >= sY + 60) && (msY <= sY + 60 + 253))
            {
                dV1 = (double)(msX - (sX + 15));
                dV2 = (double)(752.0f); 
                dV3 = (dV2 * dV1) / 280.0f;
                tX = (int)dV3;

                dV1 = (double)(msY - (sY + 60));
                dV2 = (double)(680.0f); 
                dV3 = (dV2 * dV1) / 253.0f;
                tY = (int)dV3;

                if (tX < 30) tX = 30;
                if (tY < 30) tY = 30;
                if (tX > 722) tX = 722;
                if (tY > 650) tY = 650;

                format_to_local(G_cTxt, "{},{}", tX, tY);
                put_string_sprite_font3(msX + 10, msY - 10, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, FALSE, 2);
            }
            break;
    }
}

void CGame::DrawDialogBox_Help(int msX, int msY)
{
    short sX, sY, szX;

    sX = m_stDialogBoxInfo[35].sX;
    sY = m_stDialogBoxInfo[35].sY;
    szX = m_stDialogBoxInfo[35].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 0) && (msY < sY + 50 + 15 * 1))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 0, DRAW_DIALOGBOX_HELP1, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 0, DRAW_DIALOGBOX_HELP1, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 1) && (msY < sY + 50 + 15 * 2))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 1, DRAW_DIALOGBOX_HELP2, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 1, DRAW_DIALOGBOX_HELP2, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 2, DRAW_DIALOGBOX_HELP3, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 2, DRAW_DIALOGBOX_HELP3, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 3, DRAW_DIALOGBOX_HELP4, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 3, DRAW_DIALOGBOX_HELP4, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 4) && (msY < sY + 50 + 15 * 5))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 4, DRAW_DIALOGBOX_HELP5, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 4, DRAW_DIALOGBOX_HELP5, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 5) && (msY < sY + 50 + 15 * 6))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 5, DRAW_DIALOGBOX_HELP6, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 5, DRAW_DIALOGBOX_HELP6, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 6) && (msY < sY + 50 + 15 * 7))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 6, DRAW_DIALOGBOX_HELP7, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 6, DRAW_DIALOGBOX_HELP7, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 7) && (msY < sY + 50 + 15 * 8))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 7, DRAW_DIALOGBOX_HELP8, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 7, DRAW_DIALOGBOX_HELP8, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 8) && (msY < sY + 50 + 15 * 9))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 8, DRAW_DIALOGBOX_HELP9, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 8, DRAW_DIALOGBOX_HELP9, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 9) && (msY < sY + 50 + 15 * 10))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 9, DRAW_DIALOGBOX_HELP10, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 9, DRAW_DIALOGBOX_HELP10, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 10) && (msY < sY + 50 + 15 * 11))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 10, DRAW_DIALOGBOX_HELP11, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 10, DRAW_DIALOGBOX_HELP11, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 11) && (msY < sY + 50 + 15 * 12))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 11, DRAW_DIALOGBOX_HELP12, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 11, DRAW_DIALOGBOX_HELP12, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 12) && (msY < sY + 50 + 15 * 13))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 12, "F.A.Q.", 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 12, "F.A.Q.", 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 13) && (msY < sY + 50 + 15 * 14))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 13, DRAW_DIALOGBOX_HELP13, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 13, DRAW_DIALOGBOX_HELP13, 4, 0, 50);

#if DEF_LANGUAGE == 3
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 14) && (msY < sY + 50 + 15 * 15))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 14, DRAW_DIALOGBOX_HELP14, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 14, DRAW_DIALOGBOX_HELP14, 4, 0, 50);
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 15) && (msY < sY + 50 + 15 * 16))
        put_aligned_string(sX, sX + szX, sY + 50 + 15 * 15, DRAW_DIALOGBOX_HELP15, 255, 255, 255);
    else put_aligned_string(sX, sX + szX, sY + 50 + 15 * 15, DRAW_DIALOGBOX_HELP15, 4, 0, 50);
#endif



    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
}


void CGame::DrawDialogBox_ItemUpgrade(int msX, int msY)
{
    int i, sX, sY, iValue;
    char cItemColor, cStr1[120]{}, cStr2[120]{}, cStr3[120]{};
    DWORD dwTime = unixtime();

    sX = m_stDialogBoxInfo[34].sX;
    sY = m_stDialogBoxInfo[34].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 5); //Item Upgrade Text

    switch (m_stDialogBoxInfo[34].cMode)
    {
        case 1:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 30, DRAW_DIALOGBOX_ITEMUPGRADE1);
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 45, DRAW_DIALOGBOX_ITEMUPGRADE2);
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 60, DRAW_DIALOGBOX_ITEMUPGRADE3);
            format_to_local(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE11, m_iGizonItemUpgradeLeft);
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 80, G_cTxt);

            
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 46);

            
            if (m_stDialogBoxInfo[34].sV1 != -1)
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                iValue = (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_dwAttribute & 0xF0000000) >> 28;
                iValue = iValue * (iValue + 6) / 8 + 2;
                format_to_local(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE12, iValue);
                if (m_iGizonItemUpgradeLeft < iValue) put_aligned_string(sX + 24, sX + 248, sY + 55 + 95 + 282 - 117 - 170 - 30, G_cTxt, 195, 25, 25);
                else put_aligned_string(sX + 24, sX + 248, sY + 55 + 95 + 282 - 117 - 170 - 30, G_cTxt);
                i = m_stDialogBoxInfo[34].sV1;
                cItemColor = m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cItemColor;
                if ((m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_LHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_RHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
                {
                    
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }
                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_stDialogBoxInfo[34].sV1], cStr1, cStr2, cStr3, 120);
                put_aligned_string(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                put_aligned_string(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                put_aligned_string(sX + 24, sX + 248, sY + 260 + 20, cStr3);
                if (m_iGizonItemUpgradeLeft < iValue) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 46);
                else
                {
                    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 47);
                    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 46);
                }
            }
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 46);
            // Cancel
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;

        case 2:
            
            put_aligned_string(sX + 24, sX + 248, sY + 55 + 30 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE5);
            put_aligned_string(sX + 24, sX + 248, sY + 55 + 45 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE6);

            
            if (m_stDialogBoxInfo[34].sV1 != -1)
            {
                
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                i = m_stDialogBoxInfo[34].sV1;
                cItemColor = m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cItemColor;
                if ((m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_LHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_RHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
                {
                    
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame,
                        m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame,
                        m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }
                if ((rand() % 5) == 0) m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_trans_sprite25(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame, dwTime);
                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_stDialogBoxInfo[34].sV1], cStr1, cStr2, cStr3, 120);
                put_aligned_string(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                put_aligned_string(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                put_aligned_string(sX + 24, sX + 248, sY + 260 + 20, cStr3);
            }

            if (((dwTime - m_stDialogBoxInfo[34].dwV1) / 1000 > 4) && (m_stDialogBoxInfo[34].dwV1 != NULL))
            {
                m_stDialogBoxInfo[34].dwV1 = NULL;
                
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_UPGRADEITEM, NULL, m_stDialogBoxInfo[34].sV1, NULL, NULL, NULL);
            }
            break;

        case 3:
            
            put_aligned_string(sX + 24, sX + 248, sY + 55 + 30 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE7);
            
            put_aligned_string(sX + 24, sX + 248, sY + 55 + 45 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE8);
            
    
            if (m_stDialogBoxInfo[34].sV1 != -1)
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                i = m_stDialogBoxInfo[34].sV1;
                cItemColor = m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cItemColor;
                if ((m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_LHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_RHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
                {
                    
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame,
                        m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame,
                        m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }

                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_stDialogBoxInfo[34].sV1], cStr1, cStr2, cStr3, 120);
                put_aligned_string(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                put_aligned_string(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                put_aligned_string(sX + 24, sX + 248, sY + 260 + 20, cStr3);
            }

            // OK
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 4:
            
            put_aligned_string(sX + 24, sX + 248, sY + 55 + 30 + 282 - 117 - 170, DRAW_DIALOGBOX_ITEMUPGRADE9);
            if ((m_stDialogBoxInfo[34].sV1 != -1) && (m_pItemList[m_stDialogBoxInfo[34].sV1] == NULL))
            {
                
                PlaySound('E', 24, 0, 0);
                m_stDialogBoxInfo[34].cMode = 7;
                return;
            }

            
            if (m_stDialogBoxInfo[34].sV1 != -1)
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                i = m_stDialogBoxInfo[34].sV1;
                cItemColor = m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cItemColor;
                if ((m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_LHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_RHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
                {
                    
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame,
                        m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame,
                        m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }

                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_stDialogBoxInfo[34].sV1], cStr1, cStr2, cStr3, 120);
                put_aligned_string(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                put_aligned_string(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                put_aligned_string(sX + 24, sX + 248, sY + 260 + 20, cStr3);
            }

            // OK
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

            
        case 5: 
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 45, DRAW_DIALOGBOX_ITEMUPGRADE13);

            if ((msX > sX + 24) && (msX < sX + 248) && (msY > sY + 100) && (msY < sY + 115))
            {
                put_aligned_string(sX + 24, sX + 248, sY + 100, DRAW_DIALOGBOX_ITEMUPGRADE14, 255, 255, 255);

                put_aligned_string(sX + 24, sX + 248, sY + 150, DRAW_DIALOGBOX_ITEMUPGRADE16);
                put_aligned_string(sX + 24, sX + 248, sY + 165, DRAW_DIALOGBOX_ITEMUPGRADE17);
                put_aligned_string(sX + 24, sX + 248, sY + 180, DRAW_DIALOGBOX_ITEMUPGRADE18);
                put_aligned_string(sX + 24, sX + 248, sY + 195, DRAW_DIALOGBOX_ITEMUPGRADE19);
                put_aligned_string(sX + 24, sX + 248, sY + 210, DRAW_DIALOGBOX_ITEMUPGRADE20);
                put_aligned_string(sX + 24, sX + 248, sY + 225, DRAW_DIALOGBOX_ITEMUPGRADE21);
                put_aligned_string(sX + 24, sX + 248, sY + 255, DRAW_DIALOGBOX_ITEMUPGRADE26);
                put_aligned_string(sX + 24, sX + 248, sY + 270, DRAW_DIALOGBOX_ITEMUPGRADE27);
            }
            else put_aligned_string(sX + 24, sX + 248, sY + 100, DRAW_DIALOGBOX_ITEMUPGRADE14, 4, 0, 50);

            if ((msX > sX + 24) && (msX < sX + 248) && (msY > sY + 120) && (msY < sY + 135))
            {
                put_aligned_string(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_ITEMUPGRADE15, 255, 255, 255);

                put_aligned_string(sX + 24, sX + 248, sY + 150, DRAW_DIALOGBOX_ITEMUPGRADE22);
                put_aligned_string(sX + 24, sX + 248, sY + 165, DRAW_DIALOGBOX_ITEMUPGRADE23);
                put_aligned_string(sX + 24, sX + 248, sY + 180, DRAW_DIALOGBOX_ITEMUPGRADE24);
                put_aligned_string(sX + 24, sX + 248, sY + 195, DRAW_DIALOGBOX_ITEMUPGRADE25);
                put_aligned_string(sX + 24, sX + 248, sY + 225, DRAW_DIALOGBOX_ITEMUPGRADE28);
                put_aligned_string(sX + 24, sX + 248, sY + 240, DRAW_DIALOGBOX_ITEMUPGRADE29);
            }
            else put_aligned_string(sX + 24, sX + 248, sY + 120, DRAW_DIALOGBOX_ITEMUPGRADE15, 4, 0, 50);

            // Cancel
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;

        case 6:
            
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 30, DRAW_DIALOGBOX_ITEMUPGRADE31);
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 45, DRAW_DIALOGBOX_ITEMUPGRADE32);
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 60, DRAW_DIALOGBOX_ITEMUPGRADE33);
            if (m_stDialogBoxInfo[34].sV2 == 0)
            {
                put_aligned_string(sX + 24, sX + 248, sY + 20 + 80, DRAW_DIALOGBOX_ITEMUPGRADE41, 195, 25, 25);
            }
            else
            {
                format_to_local(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE34, m_stDialogBoxInfo[34].sV2);
                put_aligned_string(sX + 24, sX + 248, sY + 20 + 80, G_cTxt);
            }
            if (m_stDialogBoxInfo[34].sV3 == 0)
            {
                put_aligned_string(sX + 24, sX + 248, sY + 20 + 95, DRAW_DIALOGBOX_ITEMUPGRADE42, 195, 25, 25);
            }
            else
            {
                format_to_local(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE35, m_stDialogBoxInfo[34].sV3);
                put_aligned_string(sX + 24, sX + 248, sY + 20 + 95, G_cTxt);
            }

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 46);

            
            if (m_stDialogBoxInfo[34].sV1 != -1)
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME3, sX, sY, 3);
                i = m_stDialogBoxInfo[34].sV1;
                cItemColor = m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cItemColor;
                if ((m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_LHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_RHAND) || (m_pItemList[m_stDialogBoxInfo[34].sV1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
                {
                    
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame,
                        m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                }
                else
                {
                    m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSprite]->put_sprite_color(sX + 134, sY + 182, m_pItemList[m_stDialogBoxInfo[34].sV1]->m_sSpriteFrame,
                        m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                }

                ZeroMemory(cStr1, sizeof(cStr1));
                ZeroMemory(cStr2, sizeof(cStr2));
                ZeroMemory(cStr3, sizeof(cStr3));
                GetItemName(m_pItemList[m_stDialogBoxInfo[34].sV1], cStr1, cStr2, cStr3, 120);
                put_aligned_string(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                put_aligned_string(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                put_aligned_string(sX + 24, sX + 248, sY + 260 + 20, cStr3);

                if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 47);
                else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 46);
            }
            // Cancel
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 17);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 16);
            break;

        case 7: 
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 130, DRAW_DIALOGBOX_ITEMUPGRADE36);
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 145, DRAW_DIALOGBOX_ITEMUPGRADE37);
            // OK
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 8: 
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 130, DRAW_DIALOGBOX_ITEMUPGRADE38);

            // OK
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 9: 
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 130, DRAW_DIALOGBOX_ITEMUPGRADE39);

            // OK
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 10: 
            put_aligned_string(sX + 24, sX + 248, sY + 20 + 130, DRAW_DIALOGBOX_ITEMUPGRADE40);

            // OK
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;
    }
}
