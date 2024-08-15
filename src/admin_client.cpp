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

#ifdef DEF_ADMINCLIENT
void CGame::DlgBoxClick_AdminControl(int msX, int msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[71].sX;
    sY = m_stDialogBoxInfo[71].sY;

    if ((msX > sX + 110) && (msX < sX + 140) && (msY > sY + 65) && (msY < sY + 80))
    {
        EndInputString();
        StartInputString(sX + 105, sY + 50 + 15 * 1, 10, m_cRefreshRate);
        m_stDialogBoxInfo[71].sV1 = 1;
        PlaySound('E', 14, 5);
    }
    if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
    {//Char list
        PlaySound('E', 14, 5);
        EnableDialogBox(72, 1, 0, 0);
    }
    if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
    {//Map list
        PlaySound('E', 14, 5);
        EnableDialogBox(72, 2, 0, 0);
    }
    if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 4) && (msY < sY + 50 + 15 * 5))
    {//IP list
        PlaySound('E', 14, 5);
        EnableDialogBox(72, 3, 0, 0);
    }
    if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 5) && (msY < sY + 50 + 15 * 6))
    {//Ban list
        PlaySound('E', 14, 5);
        EnableDialogBox(72, 4, 0, 0);
    }
    if ((msX >= sX + 160) && (msX <= sX + 160 + 60) && (msY > sY + 50 + 15 * 1) && (msY < sY + 50 + 15 * 2))
    {
        PlaySound('E', 14, 5);
        if (bAdminListUpdate == true)
            bAdminListUpdate = false;
        else
            bAdminListUpdate = true;
    }
    if ((msX >= sX + 90) && (msX <= sX + 90 + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        PlaySound('E', 14, 5);
        DisableDialogBox(71);
    }
}
void CGame::DrawDialogBox_AdminControl(int msX, int msY)
{
    short sX, sY, szX;

    sX = m_stDialogBoxInfo[71].sX;
    sY = m_stDialogBoxInfo[71].sY;
    szX = m_stDialogBoxInfo[71].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);

    put_string(sX + 30, sY + 5 + 15 * 2, "Helbreath Xtreme Administration Panel", Color::White);
    put_aligned_string(sX, sX + szX, sY + 5 + 15 * 3, "Server: Helbreath Xtreme", 32, 7, 2);

    //if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY)) 
    //	DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1); 
    //else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0); 


    if ((msX >= sX + 90) && (msX <= sX + 90 + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 90, sY + DEF_BTNPOSY, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 90, sY + DEF_BTNPOSY, 0);

    put_string(sX + 25, sY + 50 + 15 * 1, "Refresh Rate: _____", Color(25, 35, 25));

    put_string(sX + 105, sY + 50 + 15 * 1, m_cRefreshRate, GREEN);

    if (bAdminListUpdate == true)
        put_string(sX + 160, sY + 50 + 15 * 1, "Updating Lists", GREEN);
    else
        put_string(sX + 150, sY + 50 + 15 * 1, "->Begin Updating<-", Color::Red);

    //Character list viewing + idle time
    if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
        put_string(sX + 20, sY + 50 + 15 * 2, "Character List", Color::White);
    else put_string(sX + 20, sY + 50 + 15 * 2, "Character List", Color(0xAD, 0xD8, 0xE6));

    //Map list + who is on what at where
    if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
        put_string(sX + 20, sY + 50 + 15 * 3, "Map List", Color::White);
    else put_string(sX + 20, sY + 50 + 15 * 3, "Map List", Color(0xAD, 0xD8, 0xE6));

    //IP List - <colored red=ares, blue=elv, green=gms, purple=travs>Charname<t>level<t>mapname<t>pkpts<t>ip
    if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 4) && (msY < sY + 50 + 15 * 5))
        put_string(sX + 20, sY + 50 + 15 * 4, "IP List", Color::White);
    else put_string(sX + 20, sY + 50 + 15 * 4, "IP List", Color(0xAD, 0xD8, 0xE6));

    //Ban list viewing/editing controls
    if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 5) && (msY < sY + 50 + 15 * 6))
        put_string(sX + 20, sY + 50 + 15 * 5, "Ban List", Color::White);
    else put_string(sX + 20, sY + 50 + 15 * 5, "Ban List", Color(0xAD, 0xD8, 0xE6));
}
void CGame::DrawDialogBox_UserList(short msX, short msY, short msZ, char cLB)
{
    short sX, sY;
    uint32_t dwTime = m_dwCurTime;
    int  i, iTemp;
    char cTemp[255], cStr2[255], cStr3[255];
    int  iCost, iDiscountCost, iDiscountRatio, iMsgLoc;
    double dTmp1, dTmp2, dTmp3;
    int  iTotalLines, iPointerLoc;
    bool bFlag = true;
    double d1, d2, d3;
    bool bTest = false;

    iMsgLoc = 0;

    memset(m_cHighlightedChar, 0, sizeof(m_cHighlightedChar));

    sX = m_stDialogBoxInfo[72].sX;
    sY = m_stDialogBoxInfo[72].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    //DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

    switch (m_stDialogBoxInfo[72].cMode)
    {
        case 0:

            switch (m_stDialogBoxInfo[72].sV1)
            {
                case 1:
                {
                    if (m_iTotalCharList > 16)
                    {
                        d1 = (double)m_stDialogBoxInfo[72].sView;
                        d2 = (double)(m_iTotalCharList - 16);
                        d3 = (274.0f * d1) / d2;
                        iPointerLoc = (int)(d3);
                        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242, sY + iPointerLoc + 35, 7);
                    }
                    else iPointerLoc = 0;

                    if (cLB != 0 && m_iTotalCharList > 16)
                    {
                        if ((iGetTopDialogBoxIndex() == 72))
                        {
                            if ((msX >= sX + 235) && (msX <= sX + 260) && (msY >= sY + 10) && (msY <= sY + 330))
                            {
                                d1 = (double)(msY - (sY + 35));
                                d2 = (double)(m_iTotalCharList - 16);
                                d3 = (d1 * d2) / 274.0f;
                                m_stDialogBoxInfo[72].sView = (int)(d3 + 0.5);
                            }
                        }
                    }
                    else m_stDialogBoxInfo[72].bIsScrollSelected = false;
                    if (iGetTopDialogBoxIndex() == 72 && msZ != 0)
                    {
                        m_stDialogBoxInfo[72].sView = m_stDialogBoxInfo[72].sView - msZ / 60;
                        m_stMCursor.sZ = 0;
                    }
                    if (m_iTotalCharList > 16 && m_stDialogBoxInfo[72].sView > m_iTotalCharList - 16) m_stDialogBoxInfo[72].sView = m_iTotalCharList - 16;
                    if (m_stDialogBoxInfo[72].sView < 0 || m_iTotalCharList < 16) m_stDialogBoxInfo[72].sView = 0;

                    put_aligned_string(sX + 20, sX + 80, sY + 45, "Name", 80, 0, 80);
                    put_aligned_string(sX + 90, sX + 160, sY + 45, "Mapname", 80, 0, 80);
                    put_aligned_string(sX + 160, sX + 180, sY + 45, "Coords", 80, 0, 80);
                    put_aligned_string(sX + 200, sX + 230, sY + 45, "Level:PKs", 80, 0, 80);

                    for (i = 0; i < 16; i++)
                        if ((i + m_stDialogBoxInfo[72].sView) < DEF_MAXCHARLIST)
                        {
                            if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 15 + 65) && (msY <= sY + i * 15 + 79))
                            {
                                if (m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName[0] != 0)
                                {
                                    if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown + 3, "hunter", 6) == 0)
                                        put_string(sX + 10, sY + i * 15 + 65, "c", Color::White);
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::White);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::White);
                                    format_to_local(G_cTxt, "({},{})", m_stCharList[i + m_stDialogBoxInfo[72].sView].iX, m_stCharList[i + m_stDialogBoxInfo[72].sView].iY);
                                    put_string(sX + 150, sY + i * 15 + 65, G_cTxt, Color::White);
                                    format_to_local(G_cTxt, "{}:{}", m_stCharList[i + m_stDialogBoxInfo[72].sView].iLevel, m_stCharList[i + m_stDialogBoxInfo[72].sView].iPKs);
                                    put_string(sX + 200, sY + i * 15 + 65, G_cTxt, Color::White);
                                    bTest = true;
                                    memcpy(m_cHighlightedChar, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, 11);
                                }
                            }
                            else
                            {
                                if (m_stCharList[i + m_stDialogBoxInfo[72].sView].bIsGM > 0)
                                {
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, GREEN);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, GREEN);
                                    format_to_local(G_cTxt, "({},{})", m_stCharList[i + m_stDialogBoxInfo[72].sView].iX, m_stCharList[i + m_stDialogBoxInfo[72].sView].iY);
                                    put_string(sX + 150, sY + i * 15 + 65, G_cTxt, GREEN);
                                    format_to_local(G_cTxt, "{}:{}", m_stCharList[i + m_stDialogBoxInfo[72].sView].iLevel, m_stCharList[i + m_stDialogBoxInfo[72].sView].iPKs);
                                    put_string(sX + 200, sY + i * 15 + 65, G_cTxt, GREEN);
                                }
                                else if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "elvine", 6) == 0)
                                {
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::Blue);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Blue);
                                    format_to_local(G_cTxt, "({},{})", m_stCharList[i + m_stDialogBoxInfo[72].sView].iX, m_stCharList[i + m_stDialogBoxInfo[72].sView].iY);
                                    put_string(sX + 150, sY + i * 15 + 65, G_cTxt, Color::Blue);
                                    format_to_local(G_cTxt, "{}:{}", m_stCharList[i + m_stDialogBoxInfo[72].sView].iLevel, m_stCharList[i + m_stDialogBoxInfo[72].sView].iPKs);
                                    put_string(sX + 200, sY + i * 15 + 65, G_cTxt, Color::Blue);
                                }
                                else if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "elvhunter", 9) == 0)
                                {

                                    put_string(sX + 10, sY + i * 15 + 65, "c", Color::Blue);
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::Blue);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Blue);
                                    format_to_local(G_cTxt, "({},{})", m_stCharList[i + m_stDialogBoxInfo[72].sView].iX, m_stCharList[i + m_stDialogBoxInfo[72].sView].iY);
                                    put_string(sX + 150, sY + i * 15 + 65, G_cTxt, Color::Blue);
                                    format_to_local(G_cTxt, "{}:{}", m_stCharList[i + m_stDialogBoxInfo[72].sView].iLevel, m_stCharList[i + m_stDialogBoxInfo[72].sView].iPKs);
                                    put_string(sX + 200, sY + i * 15 + 65, G_cTxt, Color::Blue);
                                }
                                else if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "arehunter", 9) == 0)
                                {
                                    put_string(sX + 10, sY + i * 15 + 65, "c", Color::Red);
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::Red);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Red);
                                    format_to_local(G_cTxt, "({},{})", m_stCharList[i + m_stDialogBoxInfo[72].sView].iX, m_stCharList[i + m_stDialogBoxInfo[72].sView].iY);
                                    put_string(sX + 150, sY + i * 15 + 65, G_cTxt, Color::Red);
                                    format_to_local(G_cTxt, "{}:{}", m_stCharList[i + m_stDialogBoxInfo[72].sView].iLevel, m_stCharList[i + m_stDialogBoxInfo[72].sView].iPKs);
                                    put_string(sX + 200, sY + i * 15 + 65, G_cTxt, Color::Red);
                                }
                                else if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "aresden", 7) == 0)
                                {
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::Red);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Red);
                                    format_to_local(G_cTxt, "({},{})", m_stCharList[i + m_stDialogBoxInfo[72].sView].iX, m_stCharList[i + m_stDialogBoxInfo[72].sView].iY);
                                    put_string(sX + 150, sY + i * 15 + 65, G_cTxt, Color::Red);
                                    format_to_local(G_cTxt, "{}:{}", m_stCharList[i + m_stDialogBoxInfo[72].sView].iLevel, m_stCharList[i + m_stDialogBoxInfo[72].sView].iPKs);
                                    put_string(sX + 200, sY + i * 15 + 65, G_cTxt, Color::Red);
                                }
                                else if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "NONE", 4) == 0)
                                {
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, 0x800080);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, 0x800080);
                                    format_to_local(G_cTxt, "({},{})", m_stCharList[i + m_stDialogBoxInfo[72].sView].iX, m_stCharList[i + m_stDialogBoxInfo[72].sView].iY);
                                    put_string(sX + 150, sY + i * 15 + 65, G_cTxt, 0x800080);
                                    format_to_local(G_cTxt, "{}:{}", m_stCharList[i + m_stDialogBoxInfo[72].sView].iLevel, m_stCharList[i + m_stDialogBoxInfo[72].sView].iPKs);
                                    put_string(sX + 200, sY + i * 15 + 65, G_cTxt, 0x800080);
                                }
                            }
                        }
                    if ((bTest = false) && (strlen(m_cHighlightedChar) > 0))
                        memset(m_cHighlightedChar, 0, sizeof(m_cHighlightedChar));
                }
                break;

                case 2:
                {
                    bTest = false;
                    if (m_iTotalMapList > 16)
                    {
                        d1 = (double)m_stDialogBoxInfo[72].sView;
                        d2 = (double)(m_iTotalMapList - 16);
                        d3 = (274.0f * d1) / d2;
                        iPointerLoc = (int)(d3);
                        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242, sY + iPointerLoc + 35, 7);
                    }
                    else iPointerLoc = 0;

                    if (cLB != 0 && m_iTotalMapList > 16)
                    {
                        if ((iGetTopDialogBoxIndex() == 72))
                        {
                            if ((msX >= sX + 235) && (msX <= sX + 260) && (msY >= sY + 10) && (msY <= sY + 330))
                            {
                                d1 = (double)(msY - (sY + 35));
                                d2 = (double)(m_iTotalMapList - 16);
                                d3 = (d1 * d2) / 274.0f;
                                m_stDialogBoxInfo[72].sView = (int)(d3 + 0.5);
                            }
                        }
                    }
                    else m_stDialogBoxInfo[72].bIsScrollSelected = false;
                    if (iGetTopDialogBoxIndex() == 72 && msZ != 0)
                    {
                        m_stDialogBoxInfo[72].sView = m_stDialogBoxInfo[72].sView - msZ / 60;
                        m_stMCursor.sZ = 0;
                    }
                    if (m_iTotalMapList > 16 && m_stDialogBoxInfo[72].sView > m_iTotalMapList - 16) m_stDialogBoxInfo[72].sView = m_iTotalMapList - 16;
                    if (m_stDialogBoxInfo[72].sView < 0 || m_iTotalMapList < 16) m_stDialogBoxInfo[72].sView = 0;

                    put_aligned_string(sX + 20, sX + 80, sY + 45, "Mapname", 80, 0, 80);
                    put_aligned_string(sX + 70, sX + 110, sY + 45, "Clients", 80, 0, 80);
                    put_aligned_string(sX + 110, sX + 160, sY + 45, "Mobs", 80, 0, 80);
                    put_aligned_string(sX + 160, sX + 200, sY + 45, "Items", 80, 0, 80);

                    for (i = 0; i < 16; i++)
                        if ((i + m_stDialogBoxInfo[72].sView) < DEF_MAXCHARLIST)
                        {
                            if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 15 + 65) && (msY <= sY + i * 15 + 79))
                            {
                                if (m_stMapList[i + m_stDialogBoxInfo[72].sView].cMapName[0] != 0)
                                {
                                    put_string(sX + 20, sY + i * 15 + 65, m_stMapList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::White);
                                    format_to_local(G_cTxt, "{}", m_stMapList[i + m_stDialogBoxInfo[72].sView].iClients);
                                    put_string(sX + 100, sY + i * 15 + 65, G_cTxt, Color::White);
                                    format_to_local(G_cTxt, "{}", m_stMapList[i + m_stDialogBoxInfo[72].sView].iMobs);
                                    put_string(sX + 130, sY + i * 15 + 65, G_cTxt, Color::White);
                                    format_to_local(G_cTxt, "{}", m_stMapList[i + m_stDialogBoxInfo[72].sView].iItems);
                                    put_string(sX + 180, sY + i * 15 + 65, G_cTxt, Color::White);
                                    memcpy(m_cHighlightedMap, m_stMapList[i + m_stDialogBoxInfo[72].sView].cMapName, 11);
                                    bTest = true;
                                }
                            }
                            else
                            {
                                if (m_stMapList[i + m_stDialogBoxInfo[72].sView].cMapName[0] != 0)
                                {
                                    put_string(sX + 20, sY + i * 15 + 65, m_stMapList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Red);
                                    format_to_local(G_cTxt, "{}", m_stMapList[i + m_stDialogBoxInfo[72].sView].iClients);
                                    put_string(sX + 100, sY + i * 15 + 65, G_cTxt, Color::Red);
                                    format_to_local(G_cTxt, "{}", m_stMapList[i + m_stDialogBoxInfo[72].sView].iMobs);
                                    put_string(sX + 130, sY + i * 15 + 65, G_cTxt, Color::Red);
                                    format_to_local(G_cTxt, "{}", m_stMapList[i + m_stDialogBoxInfo[72].sView].iItems);
                                    put_string(sX + 180, sY + i * 15 + 65, G_cTxt, Color::Red);
                                }
                            }
                        }
                    if ((bTest = false) && (strlen(m_cHighlightedMap) > 0))
                        memset(m_cHighlightedMap, 0, sizeof(m_cHighlightedMap));
                }
                break;

                case 3:
                {
                    if (m_iTotalCharList > 16)
                    {
                        d1 = (double)m_stDialogBoxInfo[72].sView;
                        d2 = (double)(m_iTotalCharList - 16);
                        d3 = (274.0f * d1) / d2;
                        iPointerLoc = (int)(d3);
                        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
                        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242, sY + iPointerLoc + 35, 7);
                    }
                    else iPointerLoc = 0;

                    if (cLB != 0 && m_iTotalCharList > 16)
                    {
                        if ((iGetTopDialogBoxIndex() == 72))
                        {
                            if ((msX >= sX + 235) && (msX <= sX + 260) && (msY >= sY + 10) && (msY <= sY + 330))
                            {
                                d1 = (double)(msY - (sY + 35));
                                d2 = (double)(m_iTotalCharList - 16);
                                d3 = (d1 * d2) / 274.0f;
                                m_stDialogBoxInfo[72].sView = (int)(d3 + 0.5);
                            }
                        }
                    }
                    else m_stDialogBoxInfo[72].bIsScrollSelected = false;
                    if (iGetTopDialogBoxIndex() == 72 && msZ != 0)
                    {
                        m_stDialogBoxInfo[72].sView = m_stDialogBoxInfo[72].sView - msZ / 60;
                        m_stMCursor.sZ = 0;
                    }
                    if (m_iTotalCharList > 16 && m_stDialogBoxInfo[72].sView > m_iTotalCharList - 16) m_stDialogBoxInfo[72].sView = m_iTotalCharList - 16;
                    if (m_stDialogBoxInfo[72].sView < 0 || m_iTotalCharList < 16) m_stDialogBoxInfo[72].sView = 0;

                    put_aligned_string(sX + 20, sX + 80, sY + 45, "Name", 80, 0, 80);
                    put_aligned_string(sX + 90, sX + 160, sY + 45, "Mapname", 80, 0, 80);
                    put_aligned_string(sX + 160, sX + 180, sY + 45, "IP", 80, 0, 80);

                    for (i = 0; i < 16; i++)
                        if ((i + m_stDialogBoxInfo[72].sView) < DEF_MAXCHARLIST)
                        {
                            if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 15 + 65) && (msY <= sY + i * 15 + 79))
                            {
                                if (m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown[0] != 0)
                                {
                                    if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown + 3, "hunter", 6) == 0)
                                        put_string(sX + 10, sY + i * 15 + 65, "c", Color::White);
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::White);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::White);
                                    put_string(sX + 150, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cAddress, Color::White);
                                    bTest = true;
                                }
                                memcpy(m_cHighlightedChar, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, 11);
                            }
                            else
                            {
                                if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "elvine", 6) == 0)
                                {
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::Blue);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Blue);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Blue);
                                    put_string(sX + 150, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cAddress, Color::Blue);
                                }
                                else if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "aresden", 7) == 0)
                                {
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::Red);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Red);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Red);
                                    put_string(sX + 150, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cAddress, Color::Red);
                                }
                                else if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "arehunter", 9) == 0)
                                {
                                    put_string(sX + 10, sY + i * 15 + 65, "c", Color::Red);
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::Red);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Red);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Red);
                                    put_string(sX + 150, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cAddress, Color::Red);
                                }
                                else if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "elvhunter", 9) == 0)
                                {
                                    put_string(sX + 10, sY + i * 15 + 65, "c", Color::Blue);
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, Color::Blue);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Blue);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, Color::Blue);
                                    put_string(sX + 150, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cAddress, Color::Blue);
                                }
                                else if (memcmp(m_stCharList[i + m_stDialogBoxInfo[72].sView].cTown, "NONE", 4) == 0)
                                {
                                    put_string(sX + 20, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cCharName, 0x800080);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, 0x800080);
                                    put_string(sX + 100, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cMapName, 0x800080);
                                    put_string(sX + 150, sY + i * 15 + 65, m_stCharList[i + m_stDialogBoxInfo[72].sView].cAddress, 0x800080);
                                }
                            }
                        }
                    if ((bTest = false) && (strlen(m_cHighlightedChar) > 0))
                        memset(m_cHighlightedChar, 0, sizeof(m_cHighlightedChar));
                }
                break;

                case 4:
                {
                }
                break;
            }
            break;

        default:
            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sSprite]->put_sprite_fast(sX + 62 + 30 - 35, sY + 84 + 30 - 10,
                m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sSpriteFrame, dwTime);

            memset(cTemp, 0, sizeof(cTemp));
            GetItemName(m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1], cTemp, cStr2, cStr3);

            put_aligned_string(sX + 25, sX + 240, sY + 50, cTemp, 255, 255, 255);
            put_aligned_string(sX + 26, sX + 241, sY + 50, cTemp, 255, 255, 255);

            strcpy(cTemp, DRAW_DIALOGBOX_SHOP3);//"°¡  °Ý"
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
            //		    register int iThrow, iRange, iTotalDmg;
            //            register int i, iRet;
            //			iThrow = m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue1;	
            //			iRange = m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue2;	
            //			srand( (unsigned)time( 0 ));
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

            //			put_string(sX + 95 + 30 -35, sY + 108 + 30 -10, cTemp, Color(40,10,10));
            //			put_string(sX + 96 + 30 -35, sY + 108 + 30 -10, cTemp, Color(40,10,10));
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
                    if (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cSpeed == 0)   format_to_local(cTemp, ": 0(10~10)");
                    else format_to_local(cTemp, ": {}({} ~ {})", m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cSpeed, iTemp, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cSpeed * 13);

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

                    bFlag = true;
                    switch (m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue4)
                    {
                        case 10:
                            bFlag = false;
                            format_to_local(cTemp, DRAW_DIALOGBOX_SHOP15, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5);

                            if (m_iStr >= m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sItemEffectValue5)
                            {
                                put_string(sX + 95 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                put_string(sX + 96 + 30 - 35, sY + 125 + 45 - 10, cTemp, Color(45, 25, 25));
                                bFlag = true;
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
                                bFlag = true;
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
                                bFlag = true;
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
                                bFlag = true;
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
                                bFlag = true;
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
                                bFlag = true;
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
                        bFlag = false;
                    }
                    else if (bFlag == false)
                    {
                        strcpy(cTemp, DRAW_DIALOGBOX_SHOP21);
                        put_aligned_string(sX + 25, sX + 240, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                        put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                        iMsgLoc++;
                    }

                    if (strstr(m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cName, "(M)") != 0)
                    {

                        if (m_sPlayerType > 3)
                        {
                            strcpy(cTemp, DRAW_DIALOGBOX_SHOP22);
                            put_aligned_string(sX + 25, sX + 240, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                            put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                            iMsgLoc++;
                            bFlag = false;
                        }
                    }
                    else if (strstr(m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cName, "(W)") != 0)
                    {

                        if (m_sPlayerType <= 3)
                        {
                            strcpy(cTemp, DRAW_DIALOGBOX_SHOP23);
                            put_aligned_string(sX + 25, sX + 240, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                            put_aligned_string(sX + 25 + 1, sX + 240 + 1, sY + 254 - 10 + 14 + iMsgLoc * 15, cTemp, 195, 25, 25);
                            iMsgLoc++;
                            bFlag = false;
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

                if ((bFlag == true) && (m_iLevel < m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sLevelLimit))
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
                memset(cTemp, 0, sizeof(cTemp));
                _itoa(m_stDialogBoxInfo[11].sV3, cTemp, 10);
                cTemp[1] = 0;
                put_string(sX - 35 + 186, sY - 10 + 237, cTemp, Color(40, 10, 10));
                put_string(sX - 35 + 187, sY - 10 + 237, cTemp, Color(40, 10, 10));
                memset(cTemp, 0, sizeof(cTemp));
                _itoa(m_stDialogBoxInfo[11].sV3, cTemp, 10);
                put_string(sX - 35 + 200, sY - 10 + 237, (cTemp + 1), Color(40, 10, 10));
                put_string(sX - 35 + 201, sY - 10 + 237, (cTemp + 1), Color(40, 10, 10));
            }
            else
            {
                put_string(sX - 35 + 186, sY - 10 + 237, "0", Color(40, 10, 10));
                put_string(sX - 35 + 187, sY - 10 + 237, "0", Color(40, 10, 10));

                memset(cTemp, 0, sizeof(cTemp));
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
void CGame::DlgBoxClick_UserList(short msX, short msY)
{
    int i;
    short sX, sY;
    char cTemp[21];


    sX = m_stDialogBoxInfo[72].sX;
    sY = m_stDialogBoxInfo[72].sY;

    switch (m_stDialogBoxInfo[72].cMode)
    {
        case 0:
            switch (m_stDialogBoxInfo[72].sV1)
            {
                case 1:
                    for (i = 0; i < 16; i++)
                        if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 15 + 65) && (msY <= sY + i * 15 + 79))
                        {
                            //				if (_iGetTotalItemNum() >= 50) {
                            //					return;
                            //				}

                            PlaySound('E', 14, 5);
                            //if (m_pItemForSaleList[m_stDialogBoxInfo[72].sView + i] != 0) 
                            //	m_stDialogBoxInfo[72].cMode = m_stDialogBoxInfo[72].sView + i + 1;	
                            return;
                        }
                    break;
                case 2:
                    break;
            }

            break;

            //default:
            //	if ((msX >= sX + 145) && (msX <= sX + 162) && (msY >= sY + 209) && (msY <= sY + 230)) {

            //		m_stDialogBoxInfo[72].sV3 += 10;
            //		if (m_stDialogBoxInfo[72].sV3 >= (50 - _iGetTotalItemNum())) 
            //			m_stDialogBoxInfo[72].sV3 = (50 - _iGetTotalItemNum());
            //	}

            //	if ((msX >= sX + 145) && (msX <= sX + 162) && (msY >= sY + 234) && (msY <= sY + 251)) {

            //		m_stDialogBoxInfo[11].sV3 -= 10;
            //		if (m_stDialogBoxInfo[11].sV3 <= 1) 
            //			m_stDialogBoxInfo[11].sV3 = 1;
            //	}

            //	if ((msX >= sX + 163) && (msX <= sX + 180) && (msY >= sY + 209) && (msY <= sY + 230)) {

            //		m_stDialogBoxInfo[11].sV3++;
            //		if (m_stDialogBoxInfo[11].sV3 >= (50 - _iGetTotalItemNum())) 
            //			m_stDialogBoxInfo[11].sV3 = (50 - _iGetTotalItemNum());
            //	}

            //	if ((msX >= sX + 163) && (msX <= sX + 180) && (msY >= sY + 234) && (msY <= sY + 251)) {

            //		m_stDialogBoxInfo[11].sV3--;
            //		if (m_stDialogBoxInfo[11].sV3 <= 1) 
            //			m_stDialogBoxInfo[11].sV3 = 1;
            //	}

            //	if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY)) {
            //		if ((50 - _iGetTotalItemNum()) < m_stDialogBoxInfo[11].sV3) {
            //		}
            //		else {
            //			memset(cTemp, 0, sizeof(cTemp));
            //			strcpy(cTemp, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cName);
            //			bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_PURCHASEITEM, 0, m_stDialogBoxInfo[11].sV3, 0, 0, cTemp); 
            //		}
            //		m_stDialogBoxInfo[11].cMode = 0;
            //		m_stDialogBoxInfo[11].sV3   = 1;
            //		PlaySound('E', 14, 5);
            //	}

            //	if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY)) {
            //		m_stDialogBoxInfo[11].cMode = 0;
            //		m_stDialogBoxInfo[11].sV3   = 1;
            //		PlaySound('E', 14, 5);
            //	}
            //	break;
    }
}
#endif
