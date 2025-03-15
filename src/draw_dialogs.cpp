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
#include "magic.h"
#include "skill.h"
#include "msg.h"
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

void CGame::DrawNewDialogBox(char cType, int sX, int sY, int iFrame, bool bIsNoColorKey, bool bIsTrans)
{
    int64_t dwTime = G_dwGlobalTime;

    if (m_pSprite[cType] == 0) return;
    if (bIsNoColorKey == false)
    {
        if (bIsTrans == true)
            m_pSprite[cType]->put_trans_sprite2(sX, sY, iFrame, dwTime);
        else m_pSprite[cType]->put_sprite_fast(sX, sY, iFrame, dwTime);
    }
    else m_pSprite[cType]->put_sprite_fast_no_color_key(sX, sY, iFrame, dwTime);
}

void CGame::DrawDialogBox_GuideMap(short msX, short msY, char cLB)
{
    int m_iMaxMapIndex = DEF_SPRID_INTERFACE_GUIDEMAP + m_cMapIndex + 1;
    int m_iMinMapIndex = DEF_SPRID_INTERFACE_GUIDEMAP;
    int m_iMinMapSquare = m_cMapIndex;

    //telescope.pak:  frames 1..31 <=> DEF_SPRID_INTERFACE_GUIDEMAP +1...+31 <=> m_cMapIndex 0..30
    //telescope1.pak: frames 1..3  <=> DEF_SPRID_INTERFACE_GUIDEMAP +36..+38 <=> m_cMapIndex 35..37 et +
    if (m_cMapIndex < 0) return;
    if (m_cMapIndex >= 35)
    {
        m_iMaxMapIndex = DEF_SPRID_INTERFACE_GUIDEMAP + m_cMapIndex + 1;
        m_iMinMapIndex = DEF_SPRID_INTERFACE_GUIDEMAP + 35;
        m_iMinMapSquare = m_cMapIndex - 35;
    }
    short sX{}, sY{}, shX{}, shY{}, szX{}, szY{};

    sX = m_stDialogBoxInfo[9].sX;
    sY = m_stDialogBoxInfo[9].sY;
    szX = m_stDialogBoxInfo[9].sSizeX;
    szY = m_stDialogBoxInfo[9].sSizeY;

    if (sX < 20) m_stDialogBoxInfo[9].sX = sX = 0;
    if (sY < 20) m_stDialogBoxInfo[9].sY = sY = 0;
    if (sX > get_virtual_width() - 128 - 20) m_stDialogBoxInfo[9].sX = sX = get_virtual_width() - 128;
    if (sY > get_virtual_height() - 53 - 128 - 20) m_stDialogBoxInfo[9].sY = sY = get_virtual_height() - 53 - 128;

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
        if (m_bDialogTrans) m_pSprite[m_iMaxMapIndex]->put_shift_trans_sprite2(sX, sY, shX, shY, 0, m_dwCurTime);
        else m_pSprite[m_iMaxMapIndex]->put_shift_sprite_fast(sX, sY, shX, shY, 0, m_dwCurTime);
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
        for (int i = 0; i < DEF_MAXPARTYMEMBERS; i++)
        {
            if (m_stPartyMemberNameList[i].x >= shX && m_stPartyMemberNameList[i].x <= shX + 128 && m_stPartyMemberNameList[i].y >= shY && m_stPartyMemberNameList[i].y <= shY + 128)
            {
                if (strcmp(m_stPartyMemberNameList[i].cName, m_cPlayerName) != 0)
                    for (char x = -1; x < 2; x++)
                        for (char c = -1; c < 2; c++)
                            put_pixel(sX + m_stPartyMemberNameList[i].x - shX + x, sY + m_stPartyMemberNameList[i].y - shY + c, 255, 255, 0);
            }
        }
    }
    else
    {
        if (m_bDialogTrans) m_pSprite[m_iMinMapIndex]->put_trans_sprite2(sX, sY, m_iMinMapSquare, m_dwCurTime);
        else m_pSprite[m_iMinMapIndex]->put_sprite_fast_no_color_key(sX, sY, m_iMinMapSquare, m_dwCurTime);
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
        for (int i = 0; i < DEF_MAXPARTYMEMBERS; i++)
        {
            if (strcmp(m_stPartyMemberNameList[i].cName, m_cPlayerName) != 0)
                for (int x = -1; x < 2; x++)
                    for (int c = -1; c < 2; c++)
                        put_pixel(sX + ((m_stPartyMemberNameList[i].x * 128) / (m_pMapData->m_sMapSizeX)) + x, sY + ((m_stPartyMemberNameList[i].y * 128) / (m_pMapData->m_sMapSizeX)) + c, 255, 255, 0);
        }
    }

    if (cLB != 0) return;
    if (msX >= sX && msX < sX + szY && msY >= sY && msY < sY + szY)
    {
        if (sY > 213) shY = sY - 17;
        else shY = sY + szY + 4;
        if (m_bZoomMap) put_string(sX, shY, DEF_MSG_GUIDEMAP_MIN, Color(200, 200, 120));//"(-)
        else put_string(sX, shY, DEF_MSG_GUIDEMAP_MAX, Color(200, 200, 120));//"(+)

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
        format_to_local(G_cTxt, "{}, {}", shX, shY);
        if (m_cMapIndex == 11) // Aresden
        {
            if (shX > 46 && shX < 66 && shY > 107 && shY < 127) strcpy(G_cTxt, DEF_MSG_MAPNAME_MAGICTOWER);
            else if (shX > 103 && shX < 123 && shY > 86 && shY < 116)  strcpy(G_cTxt, DEF_MSG_MAPNAME_GUILDHALL);
            else if (shX > 176 && shX < 196 && shY >  62 && shY < 82)  strcpy(G_cTxt, DEF_MSG_MAPNAME_CATH);
            else if (shX > 135 && shX < 155 && shY > 113 && shY < 133) strcpy(G_cTxt, DEF_MSG_MAPNAME_CITYHALL);
            else if (shX > 97 && shX < 117 && shY > 175 && shY < 195) strcpy(G_cTxt, DEF_MSG_MAPNAME_WAREHOUSE);
            else if (shX > 223 && shX < 243 && shY > 124 && shY < 144) strcpy(G_cTxt, DEF_MSG_MAPNAME_WAREHOUSE);
            else if (shX > 118 && shX < 138 && shY > 157 && shY < 177) strcpy(G_cTxt, DEF_MSG_MAPNAME_SHOP);
            else if (shX > 148 && shX < 178 && shY > 188 && shY < 208) strcpy(G_cTxt, DEF_MSG_MAPNAME_BLACKSMITH);
            else if (shX > 69 && shX < 89 && shY > 199 && shY < 219) strcpy(G_cTxt, DEF_MSG_MAPNAME_DUNGEON);
            else if (shX > 21 && shX < 41 && shY > 266 && shY < 286) strcpy(G_cTxt, DEF_MSG_MAPNAME_HUNT);
            else if (shX > 20 && shX < 40 && shY > 13 && shY < 33)  strcpy(G_cTxt, DEF_MSG_MAPNAME_ML);
            else if (shX > 246 && shX < 266 && shY > 16 && shY < 36)  strcpy(G_cTxt, DEF_MSG_MAPNAME_ML);
            else if (shX > 265 && shX < 285 && shY > 195 && shY < 215) strcpy(G_cTxt, DEF_MSG_MAPNAME_FARM);
            else if (shX > 88 && shX < 108 && shY > 150 && shY < 170) strcpy(G_cTxt, DEF_MSG_MAPNAME_CMDHALL);
        }
        else if (m_cMapIndex == 3) // Elvine
        {
            if (shX > 170 && shX < 190 && shY >  65 && shY < 85)      strcpy(G_cTxt, DEF_MSG_MAPNAME_MAGICTOWER);
            else if (shX > 67 && shX < 87 && shY > 130 && shY < 150)  strcpy(G_cTxt, DEF_MSG_MAPNAME_GUILDHALL);
            else if (shX > 121 && shX < 141 && shY >  66 && shY < 86)  strcpy(G_cTxt, DEF_MSG_MAPNAME_CATH);
            else if (shX > 135 && shX < 155 && shY > 117 && shY < 137) strcpy(G_cTxt, DEF_MSG_MAPNAME_CITYHALL);
            else if (shX > 190 && shX < 213 && shY > 118 && shY < 138) strcpy(G_cTxt, DEF_MSG_MAPNAME_WAREHOUSE);
            else if (shX > 73 && shX < 103 && shY > 165 && shY < 185) strcpy(G_cTxt, DEF_MSG_MAPNAME_WAREHOUSE);
            else if (shX > 217 && shX < 237 && shY > 142 && shY < 162) strcpy(G_cTxt, DEF_MSG_MAPNAME_SHOP);
            else if (shX > 216 && shX < 256 && shY > 99 && shY < 119) strcpy(G_cTxt, DEF_MSG_MAPNAME_BLACKSMITH);
            else if (shX > 251 && shX < 271 && shY >  73 && shY < 93)  strcpy(G_cTxt, DEF_MSG_MAPNAME_DUNGEON);
            else if (shX > 212 && shX < 232 && shY > 13 && shY < 33)  strcpy(G_cTxt, DEF_MSG_MAPNAME_HUNT);
            else if (shX > 16 && shX < 36 && shY > 262 && shY < 282) strcpy(G_cTxt, DEF_MSG_MAPNAME_ML);
            else if (shX > 244 && shX < 264 && shY > 248 && shY < 268) strcpy(G_cTxt, DEF_MSG_MAPNAME_ML);
            else if (shX > 264 && shX < 284 && shY > 177 && shY < 207) strcpy(G_cTxt, DEF_MSG_MAPNAME_FARM);
            else if (shX > 207 && shX < 227 && shY > 79 && shY < 99)  strcpy(G_cTxt, DEF_MSG_MAPNAME_CMDHALL);
        }
        else if (m_cMapIndex == 5) // Elvine Farm
        {
            if (shX > 62 && shX < 82 && shY >  187 && shY < 207) strcpy(G_cTxt, DEF_MSG_MAPNAME_WAREHOUSE);
            else if (shX > 81 && shX < 101 && shY > 169 && shY < 189) strcpy(G_cTxt, DEF_MSG_MAPNAME_SHOP);
            else if (shX > 101 && shX < 131 && shY > 180 && shY < 200) strcpy(G_cTxt, DEF_MSG_MAPNAME_BLACKSMITH);
            else if (shX > 130 && shX < 150 && shY > 195 && shY < 215) strcpy(G_cTxt, DEF_MSG_MAPNAME_DUNGEON);
            else if (shX > 86 && shX < 106 && shY > 139 && shY < 159) strcpy(G_cTxt, DEF_MSG_MAPNAME_BARRACK);
        }
        else if (m_cMapIndex == 6) // Areden's Farm
        {
            if (shX > 30 && shX < 50 && shY >  80 && shY < 100) strcpy(G_cTxt, DEF_MSG_MAPNAME_WAREHOUSE);
            else if (shX > 55 && shX < 85 && shY > 80 && shY < 100)  strcpy(G_cTxt, DEF_MSG_MAPNAME_BLACKSMITH);
            else if (shX > 52 && shX < 72 && shY > 80 && shY < 100)  strcpy(G_cTxt, DEF_MSG_MAPNAME_SHOP);
            else if (shX > 70 && shX < 90 && shY > 60 && shY < 80)   strcpy(G_cTxt, DEF_MSG_MAPNAME_DUNGEON);
            else if (shX > 45 && shX < 65 && shY > 123 && shY < 143) strcpy(G_cTxt, DEF_MSG_MAPNAME_BARRACK);
        }
        for (int i = 0; i < DEF_MAXPARTYMEMBERS; i++)
        {
            if ((m_stPartyMemberNameList[i].x > 0) && (m_stPartyMemberNameList[i].y > 0))
                if (shX > m_stPartyMemberNameList[i].x - (m_bZoomMap ? 5 : 15) && shX < m_stPartyMemberNameList[i].x + (m_bZoomMap ? 5 : 15) && shY >  m_stPartyMemberNameList[i].y - (m_bZoomMap ? 5 : 15) && shY < m_stPartyMemberNameList[i].y + (m_bZoomMap ? 5 : 15)) strcpy(G_cTxt, m_stPartyMemberNameList[i].cName);
        }
        put_string(msX - 10, msY - 13, G_cTxt, Color(200, 200, 120));
    }
}

void CGame::DrawDialogBoxs(short msX, short msY, short msZ, char cLB)
{
    if (m_bIsObserverMode == true) return;
    m_stMCursor.sZ = 0;
    for (int i = 0; i < 101; i++)
        if (m_cDialogBoxOrder[i] != 0)
        {
            switch (m_cDialogBoxOrder[i])
            {
                case 1:
                    //DrawDialogBox_Character(msX, msY); //@@@
                    DrawDialogBox_Character(msX, msY);
                    break;
                case 2:
                    DrawDialogBox_Inventory(msX, msY); //@@@
                    break;
                case 3:
                    DrawDialogBox_Magic(msX, msY, msZ); //@@@
                    break;
                case 4:
                    DrawDialogBox_ItemDrop(msX, msY); //@@@
                    break;
                case 5:
                    DrawDialogBox_15AgeMsg(msX, msY); //@@@
                    break;
                case 6:
                    DrawDialogBox_WarningMsg(msX, msY); //@@@
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
                    DrawDialogBox_Chat(msX, msY, msZ, cLB); //@@@
                    break;
                case 11:
                    DrawDialogBox_Shop(msX, msY, msZ, cLB); //@@@
                    break;
                case 12:
                    DrawDialogBox_LevelUpSetting(msX, msY); //@@@
                    break;
                case 13:
                    DrawDialogBox_CityHallMenu(msX, msY);
                    break;
                case 14:
                    DrawDialogBox_Bank(msX, msY, msZ, cLB); //@@@
                    break;
                case 15:
                    DrawDialogBox_Skill(msX, msY, msZ, cLB); //@@@
                    break;
                case 16:
                    DrawDialogBox_MagicShop(msX, msY, msZ); //@@@
                    break;
                case 17:
                    DrawDialogBox_QueryDropItemAmount();
                    break;
                case 18:
                    DrawDialogBox_Text(msX, msY, msZ, cLB); //@@@
                    break;
                case 19:
                    DrawDialogBox_SysMenu(msX, msY, cLB); //@@@
                    break;
                case 20:
                    DrawDialogBox_NpcActionQuery(msX, msY); //@@@
                    break;
                case 21:
                    DrawDialogBox_NpcTalk(msX, msY, cLB); //@@@
                    break;
                case 22:
                    DrawDialogBox_Map();
                    break;
                case 23:
                    DrawDialogBox_SellorRepairItem(msX, msY); //@@@
                    break;
                case 24:
                    DrawDialogBox_Fishing(msX, msY);
                    break;
                case 25:
                    DrawDialogBox_ShutDownMsg(msX, msY); //@@@
                    break;
                case 26: // Manuf
                    DrawDialogBox_SkillDlg(msX, msY, msZ, cLB);
                    break;
                case 27:
                    DrawDialogBox_Exchange(msX, msY); //@@@
                    break;
                case 28:
                    DrawDialogBox_Quest(msX, msY); //@@@
                    break;
                case 29:
                    DrawDialogBox_GaugePannel(); //@@@
                    break;
                case 30:
                    DrawDialogBox_IconPannel(msX, msY); //@@@
                    break;
                case 31:
                    DrawDialogBox_SellList(msX, msY); //@@@
                    break;
                case 32:
                    DrawDialogBox_Party(msX, msY); //@@@
                    break;
                case 33:
                    DrawDialogBox_CrusadeJob(msX, msY); //@@@
                    break;
                case 34:
                    DrawDialogBox_ItemUpgrade(msX, msY);
                    break;
                case 35:
                    DrawDialogBox_Help(msX, msY); //@@@
                    break;
                case 36:
                    DrawDialogBox_Commander(msX, msY); //@@@
                    break;
                case 37:
                    DrawDialogBox_Constructor(msX, msY); //@@@
                    break;
                case 38:
                    DrawDialogBox_Soldier(msX, msY); //@@@
                    break;
                case 40:
                    DrawDialogBox_Slates(msX, msY, msZ, cLB);
                    break;
                case 41:
                    DrawDialogBox_ConfirmExchange(msX, msY);
                    break;
                case 42:
                    DrawDialogBox_ChangeStatsMajestic(msX, msY);
                    break;
                case 50:
                    DrawDialogBox_Resurrect(msX, msY);
                    break;
                case 51: // Gail
                    DrawDialogBox_CMDHallMenu(msX, msY);
                    break;

                case 70:
                    DrawDialogBox_FriendsList(msX, msY, msZ, cLB);
                    break;
                    //Change Admin Dialog
                    //Dialog
#ifdef DEF_ADMINCLIENT
                case 71:
                    DrawDialogBox_AdminControl(msX, msY);
                    break;
                case 72:
                    DrawDialogBox_UserList(msX, msY, msZ, cLB); //@@@
                    break;
#endif
                case 73:
                    DrawDialogBox_Config(msX, msY, msZ, cLB);
                    break;
            }
        }
    if (m_cSkillMastery[_iGetWeaponSkillType()] == 100)
    {
        if (m_iSuperAttackLeft > 0)
        {
            if (GetAsyncKeyState(VK_MENU) >> 15)
                m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_trans_sprite(m_stDialogBoxInfo[30].sX + 368, m_stDialogBoxInfo[30].sY + 12, 3, m_dwCurTime);
            format_to_local(G_cTxt, "{}", m_iSuperAttackLeft);
            put_string_sprite_font2(m_stDialogBoxInfo[30].sX + 380, m_stDialogBoxInfo[30].sY + 27, G_cTxt, 220, 200, 200);
        }
    }
    else
    {
        if (m_iSuperAttackLeft > 0)
        {
            format_to_local(G_cTxt, "{}", m_iSuperAttackLeft);
            put_string_sprite_font2(m_stDialogBoxInfo[30].sX + 380, m_stDialogBoxInfo[30].sY + 27, G_cTxt, 10, 10, 10);
        }
    }
}

void CGame::DrawDialogBox_IconPannel(short msX, short msY)
{
    short sX{}, sY{};
    int64_t dwTime = m_dwCurTime;

    sX = m_stDialogBoxInfo[30].sX;
    sY = m_stDialogBoxInfo[30].sY;

    if (sX < 0) m_stDialogBoxInfo[30].sX = 0;
    if (sY < 0) m_stDialogBoxInfo[30].sY = 0;
    if (sX + m_stDialogBoxInfo[30].sSizeX > get_virtual_width()) m_stDialogBoxInfo[30].sX = get_virtual_width() - m_stDialogBoxInfo[30].sSizeX;
    if (sY + m_stDialogBoxInfo[30].sSizeY > get_virtual_height()) m_stDialogBoxInfo[30].sY = get_virtual_height() - m_stDialogBoxInfo[30].sSizeY;

    m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX, sY, 14, dwTime);

    if ((sX + 362 < msX) && (sX + 404 > msX) && (sY + 7 < msY) && (sY + 48 > msY))
    {
        m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 362, sY + 7, 16, dwTime);
    }

    if (m_bIsCombatMode)
    {
        if (m_bIsSafeAttackMode)
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 368, sY + 13, 4, dwTime);
        else m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 368, sY + 13, 5, dwTime);
    }

    if ((m_bIsCrusadeMode) && (m_iCrusadeDuty != 0))
    {
        if (m_bAresden == true)
        {
            if ((sX + 322 <= msX) && (sX + 355 >= msX) && (sY + 7 < msY) && (sY + 48 > msY))
                m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 322, sY + 7, 1, dwTime);
            else m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 322, sY + 7, 2, dwTime);
        }
        else if (m_bAresden == false)
        {
            if ((sX + 322 <= msX) && (sX + 355 >= msX) && (sY + 7 < msY) && (sY + 48 > msY))
                m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 322, sY + 7, 0, dwTime);
            else m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 322, sY + 7, 15, dwTime);
        }
    }

    if ((msY > sY + 9) && (msY < sY + 51))
    {
        if ((msX > sX + 410) && (msX < sX + 447))
        {
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 412, sY + 7, 6, dwTime);// Character
        }
        if ((msX > sX + 447) && (msX < sX + 484))
        {
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 449, sY + 7, 7, dwTime);// Inventory
        }
        if ((msX > sX + 484) && (msX < sX + 521))
        {
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 486, sY + 7, 8, dwTime);// Magic
        }
        if ((msX > sX + 521) && (msX < sX + 558))
        {
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 523, sY + 7, 9, dwTime);// Skill
        }
        if ((msX > sX + 558) && (msX < sX + 595))
        {
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 560, sY + 7, 10, dwTime);// History
        }
        if ((msX > sX + 595) && (msX < sX + 631))
        {
            m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast(sX + 597, sY + 7, 11, dwTime);// System Menu
        }
    }

    if (((msX > sX + 144) && (msX < sX + 317) && (msY > sY + 7) && (msY < sY + 50)) || (m_bCtrlPressed))
    {
        int iLev = 0;
        int iCurExp = iGetLevelExp(m_iLevel);
        int iNextExp;

#if !defined(DEF_JAPAN_FOR_TERRA) || !defined(DEF_FUCK_USA)
        if (m_iLevel > 139)
        {
            iLev = (m_iLevel - 139) * 3;
            iNextExp = iGetLevelExp(m_iLevel + iLev);
        }
        else
            iNextExp = iGetLevelExp(m_iLevel + 1);
#else
        iNextExp = iGetLevelExp(m_iLevel + 1);
#endif

        if (m_iExp < iNextExp)
        {
            iNextExp = iNextExp - iCurExp;
            if (m_iExp > iCurExp) iCurExp = m_iExp - iCurExp;
            else iCurExp = 0;
            short sPerc = 0;
            if (iCurExp > 200000) sPerc = short(((iCurExp >> 4) * 10000) / (iNextExp >> 4));
            else sPerc = (short)((iCurExp * 10000) / iNextExp);
            format_to_local(G_cTxt, DEF_MSG_EXP"{}/{}({}.{:2}%)", iNextExp - iCurExp, iNextExp, sPerc / 100, sPerc % 100);
        }
        else
        {
            format_to_local(G_cTxt, DEF_MSG_EXP"{} / {}", m_iExp, iNextExp);
        }
    }
    else format_to_local(G_cTxt, "{}({},{})", m_cMapMessage, m_sPlayerX, m_sPlayerY);
    put_aligned_string(sX + 140, sX + 323, sY + 27, G_cTxt, 200, 200, 120);
}

void CGame::DrawDialogBox_GaugePannel()
{
    int iMaxPoint{}, iBarWidth{}, iTemp{};
    short sX{}, sY{};
    int64_t dwTime = m_dwCurTime;

    sX = m_stDialogBoxInfo[30].sX;
    sY = m_stDialogBoxInfo[30].sY;

    iMaxPoint = m_iVit * 3 + m_iLevel * 2 + m_iStr / 2;
    iBarWidth = 101 - (m_iHP * 101) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 101) iBarWidth = 101;
    m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast_width(23, get_virtual_height() - 43, 12, iBarWidth, m_dwCurTime);
    iTemp = m_iHP;
    format_to_local(G_cTxt, "{}", iTemp);
    if (m_bIsPoisoned)
    {
        put_string_sprite_number(sX + 85, sY + 6, G_cTxt, m_wR[5] * 11, m_wG[5] * 11, m_wB[5] * 11);
        put_string_sprite_font3(sX + 35, sY + 13, "Poisoned", m_wR[5] * 8, m_wG[5] * 8, m_wB[5] * 8, true, 2);
    }
    else put_string_sprite_number(sX + 85, sY + 14, G_cTxt, 200, 100, 100);


    iMaxPoint = m_iMag * 2 + m_iLevel * 2 + m_iInt / 2;
    iBarWidth = 101 - (m_iMP * 101) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 101) iBarWidth = 101;
    m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast_width(sX + 23, sY + 32, 12, iBarWidth, m_dwCurTime);
    iTemp = m_iMP;
    format_to_local(G_cTxt, "{}", iTemp);
    put_string_sprite_number(sX + 85, sY + 36, G_cTxt, 100, 100, 200);


    iMaxPoint = m_iStr * 2 + m_iLevel * 2;
    iBarWidth = 167 - (m_iSP * 167) / iMaxPoint;
    if (iBarWidth < 0) iBarWidth = 0;
    if (iBarWidth > 167) iBarWidth = 167;
    m_pSprite[DEF_SPRID_INTERFACE_ND_ICONPANNEL]->put_sprite_fast_width(sX + 147, sY + 8, 13, iBarWidth, m_dwCurTime);
}

void CGame::DrawDialogBox_Text(short msX, short msY, short msZ, char cLB)
{
    short sX{}, sY{};
    int i{}, iTotalLines{}, iPointerLoc{};
    double d1{}, d2{}, d3{};

    sX = m_stDialogBoxInfo[18].sX;
    sY = m_stDialogBoxInfo[18].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);

    iTotalLines = 0;
    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
        if (m_pMsgTextList[i] != 0) iTotalLines++;

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
        if (m_pMsgTextList[i + m_stDialogBoxInfo[18].sView] != 0)
        {
            if (m_bDialogTrans == false)
            { // v2.173 
                switch (m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->message[0])
                {
//                     case '_': put_aligned_string(sX + 24, sX + 236, sY + 50 + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->m_pMsg + 1), 255, 255, 255); break;
//                     case ';': put_aligned_string(sX + 24, sX + 236, sY + 50 + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->m_pMsg + 1), 4, 0, 50); break;
                    default: put_aligned_string(sX + 24, sX + 236, sY + 50 + i * 13, m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->message, 45, 25, 25); break;
                }
            }
            else put_aligned_string(sX + 24, sX + 236, sY + 50 + i * 13, m_pMsgTextList[i + m_stDialogBoxInfo[18].sView]->message, 0, 0, 0);
        }
    if (cLB != 0 && iTotalLines > 17)
    {
        if (iGetTopDialogBoxIndex() == 18)
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
    else m_stDialogBoxInfo[18].bIsScrollSelected = false;

    if ((msX > sX + DEF_RBTNPOSX) && (msX < sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
}

void CGame::DrawDialogBox_15AgeMsg(short msX, short msY)
{
}

void CGame::DrawDialogBox_WarningMsg(short msX, short msY)
{
    short sX{}, sY{};

    sX = m_stDialogBoxInfo[6].sX;
    sY = m_stDialogBoxInfo[6].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX, sY, 2);

    put_string2(sX + 63, sY + 35, DEF_MSG_WARNING1, 200, 200, 25);
    put_string(sX + 30, sY + 57, DEF_MSG_WARNING2, Color(220, 130, 45));
    put_string(sX + 30, sY + 74, DEF_MSG_WARNING3, Color(220, 130, 45));
    put_string(sX + 30, sY + 92, DEF_MSG_WARNING4, Color(220, 130, 45));
    put_string(sX + 30, sY + 110, DEF_MSG_WARNING5, Color(220, 130, 45));
    if ((msX >= sX + 122) && (msX <= sX + 125 + DEF_BTNSZX) && (msY >= sY + 127) && (msY <= sY + 127 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 122, sY + 127, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 122, sY + 127, 0);

}

void CGame::DrawDialogBox_ItemDrop(short msX, short msY)
{
    short sX{}, sY{};
    char cTxt[120]{}, cStr1[64]{}, cStr2[64]{}, cStr3[64]{};

    sX = m_stDialogBoxInfo[4].sX;
    sY = m_stDialogBoxInfo[4].sY;

    //DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 5);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    GetItemName(m_pItemList[m_stDialogBoxInfo[4].sView]->m_cName, m_pItemList[m_stDialogBoxInfo[4].sView]->m_dwAttribute, cStr1, cStr2, cStr3, 64);

    if (strlen(m_stDialogBoxInfo[4].cStr) == 0) // v1.4
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
    short sX{}, sY{};
    int i{}, iTotalLines{}, iPointerLoc{};
    double d1{}, d2{}, d3{};


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
        if ((i < DEF_TEXTDLGMAXLINES) && (m_pMsgTextList2[i + m_stDialogBoxInfo[21].sView] != 0))
        {
            put_aligned_string(sX, sX + m_stDialogBoxInfo[21].sSizeX, sY + 57 + i * 15, m_pMsgTextList2[i + m_stDialogBoxInfo[21].sView]->message, 45, 25, 25);
        }

    iTotalLines = 0;
    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
        if (m_pMsgTextList2[i] != 0) iTotalLines++;
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
    else m_stDialogBoxInfo[21].bIsScrollSelected = false;
}

void CGame::DrawDialogBox_Chat(short msX, short msY, short msZ, char cLB)
{
    short sX{}, sY{};
    int i{}, iPointerLoc{};
    double d1{}, d2{}, d3{};

    sX = m_stDialogBoxInfo[10].sX;
    sY = m_stDialogBoxInfo[10].sY;
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 4, false, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 22, false, m_bDialogTrans);

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
        if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView] != 0)
        {
            if (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_iGM > 0)
            {
                switch (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_dwTime)
                {
                    case 0:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 0, 230, 230); break;
                    case 1:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 130, 200, 130); break;
                    case 2:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 255, 130, 130); break;
                    case 3:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 130, 130, 255); break;
                    case 4:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 230, 230, 130); break;
                    case 10: put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 180, 255, 180); break;
                    case 20: put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 150, 150, 170); break;
                }
            }
            else
            {
                switch (m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->m_dwTime)
                {
                    case 0:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 230, 230, 230); break;
                    case 1:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 130, 200, 130); break;
                    case 2:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 255, 130, 130); break;
                    case 3:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 130, 130, 255); break;
                    case 4:  put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 230, 230, 130); break;
                    case 10: put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 180, 255, 180); break;
                    case 20: put_string2(sX + 25, sY + 127 - i * 13, m_pChatScrollList[i + m_stDialogBoxInfo[10].sView]->message, 150, 150, 170); break;
                }
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
    else m_stDialogBoxInfo[10].bIsScrollSelected = false;
}

void CGame::DrawDialogBox_Inventory(int msX, int msY)
{
    int i{};
    short sX{}, sY{};
    int64_t dwTime = m_dwCurTime;
    char cItemColor{};

    sX = m_stDialogBoxInfo[2].sX;
    sY = m_stDialogBoxInfo[2].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_INVENTORY, sX, sY, 0);

    for (i = 0; i < DEF_MAXITEMS; i++)
        if ((m_cItemOrder[i] != -1) && (m_pItemList[m_cItemOrder[i]] != 0))
        {

            if (((m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_ITEM) &&
                (m_stMCursor.sSelectedObjectID == m_cItemOrder[i])) ||
                (m_bIsItemEquipped[m_cItemOrder[i]] == true))
            {
            }
            else
            {
                cItemColor = m_pItemList[m_cItemOrder[i]]->m_cItemColor; // v1.4
                if (m_bIsItemDisabled[m_cItemOrder[i]] == true)
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

                    //		#ifdef _DEBUG
                    DisplayGold((int)m_pItemList[m_cItemOrder[i]]->m_dwCount);
                    //		#else
                    //				format_to_local(G_cTxt,"{}", (int)m_pItemList[m_cItemOrder[i]]->m_dwCount);
                    //		#endif

                    put_string2(sX + 29 + m_pItemList[m_cItemOrder[i]]->m_sX + 10, sY + 41 + m_pItemList[m_cItemOrder[i]]->m_sY + 10, G_cTxt, 200, 200, 200);
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

void CGame::DrawDialogBox_FriendsList(short msX, short msY, short msZ, char cLB)
{
    short sX{}, sY{}, szX{};
    int64_t dwTime = m_dwCurTime;
    int i{};//, iTemp;
    int iMsgLoc{};
    int /*iTotalLines,*/ iPointerLoc{};
    double d1{}, d2{}, d3{};
    bool bFlag = true;
    bool bTest = false;

    iMsgLoc = 0;

    sX = m_stDialogBoxInfo[70].sX;
    sY = m_stDialogBoxInfo[70].sY;
    szX = m_stDialogBoxInfo[70].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);

    put_aligned_string(sX, sX + szX, sY + 13, "Helbreath Xtreme", 255, 255, 255);
    put_aligned_string(sX, sX + szX, sY + 33, "Friends list (type \"/add name\" to add)", 60, 60, 200);

    if (m_iTotalFriends > 16)
    {
        d1 = (double)m_stDialogBoxInfo[70].sView;
        d2 = (double)(m_iTotalFriends - 16);
        d3 = (274.0f * d1) / d2;
        iPointerLoc = (int)(d3);
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 3);
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242, sY + iPointerLoc + 35, 7);
    }
    else iPointerLoc = 0;

    if (cLB != 0 && m_iTotalFriends > 16)
    {
        if ((iGetTopDialogBoxIndex() == 70))
        {
            if ((msX >= sX + 235) && (msX <= sX + 260) && (msY >= sY + 10) && (msY <= sY + 330))
            {
                d1 = (double)(msY - (sY + 35));
                d2 = (double)(m_iTotalFriends - 16);
                d3 = (d1 * d2) / 274.0f;
                m_stDialogBoxInfo[70].sView = (int)(d3 + 0.5);
            }
        }
    }
    else m_stDialogBoxInfo[70].bIsScrollSelected = false;
    if (iGetTopDialogBoxIndex() == 70 && msZ != 0)
    {
        m_stDialogBoxInfo[70].sView = m_stDialogBoxInfo[70].sView - msZ / 60;
        m_stMCursor.sZ = 0;
    }
    if (m_iTotalFriends > 16 && m_stDialogBoxInfo[70].sView > m_iTotalFriends - 16) m_stDialogBoxInfo[70].sView = m_iTotalFriends - 16;
    if (m_stDialogBoxInfo[70].sView < 0 || m_iTotalFriends < 16) m_stDialogBoxInfo[70].sView = 0;

    put_aligned_string(sX + 20, sX + 80, sY + 45, "Name", 80, 0, 80);
    put_aligned_string(sX + 90, sX + 160, sY + 45, "Mapname", 80, 0, 80);
    put_aligned_string(sX + 180, sX + 200, sY + 45, "Level", 80, 0, 80);
    put_aligned_string(sX + 220, sX + 230, sY + 45, "PKs", 80, 0, 80);

    for (i = 0; i < 16; i++)
        if ((i + m_stDialogBoxInfo[70].sView) < 50)
        {
            //			if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i*15 + 65) && (msY <= sY + i*15 + 79)) {
            if (m_stFriendsList[i + m_stDialogBoxInfo[70].sView].bIsOnline == true)
            {
                put_string(sX + 20, sY + i * 15 + 65, m_stFriendsList[i + m_stDialogBoxInfo[70].sView].cCharName, Color::White);
                put_string(sX + 100, sY + i * 15 + 65, m_stFriendsList[i + m_stDialogBoxInfo[70].sView].cMapName, Color::White);
                format_to_local(G_cTxt, "{}", m_stFriendsList[i + m_stDialogBoxInfo[70].sView].iLevel);
                switch (m_stFriendsList[m_iTotalFriends].cSide)
                {
                    case 0:
                        put_string(sX + 180, sY + i * 15 + 65, G_cTxt, Color(0x80, 0, 0x80));
                        break;
                    case 1:
                        put_string(sX + 180, sY + i * 15 + 65, G_cTxt, Color::Red);
                        break;
                    case 2:
                        put_string(sX + 180, sY + i * 15 + 65, G_cTxt, Color::Blue);
                        break;
                }
                format_to_local(G_cTxt, "{}", m_stFriendsList[i + m_stDialogBoxInfo[70].sView].iPKs);
                if (m_stFriendsList[i + m_stDialogBoxInfo[70].sView].iPKs > 0)
                    put_string(sX + 220, sY + i * 15 + 65, G_cTxt, Color::Red);
                else
                    put_string(sX + 220, sY + i * 15 + 65, G_cTxt, Color::White);
            }
            else
            {
                if (m_stFriendsList[i + m_stDialogBoxInfo[70].sView].cCharName[0] != 0)
                {
                    put_string(sX + 20, sY + i * 15 + 65, m_stFriendsList[i + m_stDialogBoxInfo[70].sView].cCharName, Color::Red);
                    put_string(sX + 100, sY + i * 15 + 65, "Offline", Color::Red);
                    format_to_local(G_cTxt, "0");
                    put_string(sX + 220, sY + i * 15 + 65, G_cTxt, Color::Red);
                    format_to_local(G_cTxt, "0");
                    put_string(sX + 220, sY + i * 15 + 65, G_cTxt, Color::Red);
                }
            }
            //memcpy(m_cHighlightedChar, m_stFriendsList[i + m_stDialogBoxInfo[70].sView].cCharName, 11);
//			}
//			else
//			{ 
//			}
        }
    //		if ((bTest = false) && (strlen(m_cHighlightedChar) > 0))
    //			memset(m_cHighlightedChar, 0, sizeof(m_cHighlightedChar));
}
void CGame::DrawDialogBox_Config(short msX, short msY, short msZ, char cLB)
{
    short sX{}, sY{}, szX{};
    int64_t dwTime = m_dwCurTime;
    int i{};//, iTemp;
    int iMsgLoc{};
    int /*iTotalLines,*/ iPointerLoc{};
    double d1{}, d2{}, d3{};
    bool bFlag = true;
    bool bTest = false;

    iMsgLoc = 0;

    sX = m_stDialogBoxInfo[73].sX;
    sY = m_stDialogBoxInfo[73].sY;
    szX = m_stDialogBoxInfo[73].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);

    put_aligned_string(sX, sX + szX, sY + 13, MSG_WORLDNAME1, 255, 128, 0);
    put_aligned_string(sX, sX + szX, sY + 33, "Config menu", 60, 60, 200);

    switch (m_stDialogBoxInfo[73].cMode)
    {
        default:
            if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
                put_string(sX + 20, sY + 50 + 15 * 2, "Party Config", Color::Blue);
            else put_string(sX + 20, sY + 50 + 15 * 2, "Party Config", Color(0xAD, 0xD8, 0xE6));

            if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
                put_string(sX + 20, sY + 50 + 15 * 3, "Other Config?", Color::Blue);
            else put_string(sX + 20, sY + 50 + 15 * 3, "Other Config?", Color(0xAD, 0xD8, 0xE6));

            if ((msX >= sX + 90) && (msX <= sX + 90 + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 90, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 90, sY + DEF_BTNPOSY, 0);
            break;
        case 1:
            if (m_stPartyConfig.bEnabled)
            {
                if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
                    put_string(sX + 20, sY + 50 + 15 * 2, "Party List (enabled)", Color::Green);//Green
                else put_string(sX + 20, sY + 50 + 15 * 2, "Party List (enabled)", Color(0x44, 0xAA, 0x44));
            }
            else
            {
                if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
                    put_string(sX + 20, sY + 50 + 15 * 2, "Party List (disabled)", Color::Red);//Red
                else put_string(sX + 20, sY + 50 + 15 * 2, "Party List (disabled)", Color(0x22, 0x22, 0xAA));
            }
            if (m_stPartyConfig.cType == 0)
            {
                if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
                    put_string(sX + 20, sY + 50 + 15 * 3, "Style (type 1)", Color::White);
                else put_string(sX + 20, sY + 50 + 15 * 3, "Style (type 1)", Color::Black);
            }
            else
            {
                if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
                    put_string(sX + 20, sY + 50 + 15 * 3, "Style (type 2)", Color::White);
                else put_string(sX + 20, sY + 50 + 15 * 3, "Style (type 2)", Color::Black);
            }
            if (m_stPartyConfig.bShowPercent)
            {
                if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 4) && (msY < sY + 50 + 15 * 5))
                    put_string(sX + 20, sY + 50 + 15 * 4, "Percents (enabled)", Color::Green);//Green
                else put_string(sX + 20, sY + 50 + 15 * 4, "Percents (enabled)", Color(0x44, 0xAA, 0x44));
            }
            else
            {
                if ((msX >= sX + 20) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 4) && (msY < sY + 50 + 15 * 5))
                    put_string(sX + 20, sY + 50 + 15 * 4, "Percents (disabled)", Color(0x00, 0x00, 0xDD));//Red
                else put_string(sX + 20, sY + 50 + 15 * 4, "Percents (disabled)", Color(0x22, 0x22, 0xAA));
            }
            //if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY)) 
            //	DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 21); 
            //else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 20); 

            //if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY)) 
            //	DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1); 
            //else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0); 
            if ((msX >= sX + 90) && (msX <= sX + 90 + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 90, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 90, sY + DEF_BTNPOSY, 0);

            break;
    }

}

void CGame::DrawDialogBox_Bank(short msX, short msY, short msZ, char cLB)
{
    short sX{}, sY{}, szX{};
    int i{}, iTotalLines{}, iPointerLoc{}, iLoc{};
    double d1{}, d2{}, d3{};
    char cItemColor{}, cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
    bool bFlag = false;

    sX = m_stDialogBoxInfo[14].sX;
    sY = m_stDialogBoxInfo[14].sY;
    szX = m_stDialogBoxInfo[14].sSizeX - 5;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 21);
    iLoc = 45;

    switch (m_stDialogBoxInfo[14].cMode)
    {
        case -1:
            put_string(sX + 30 + 15, sY + 70, DRAW_DIALOGBOX_BANK1, Color(0, 0, 0));
            put_string(sX + 30 + 15, sY + 85, DRAW_DIALOGBOX_BANK2, Color(0, 0, 0));
            break;

        case 0:
            for (i = 0; i < m_stDialogBoxInfo[14].sV1; i++)
                if ((m_pBankList[i + m_stDialogBoxInfo[14].sView] != 0) && ((i + m_stDialogBoxInfo[14].sView) < DEF_MAXBANKITEMS))
                {
                    GetItemName(m_pBankList[i + m_stDialogBoxInfo[14].sView], cStr1, cStr2, cStr3, 64);
                    if ((msX > sX + 30) && (msX < sX + 210) && (msY >= sY + 110 + i * 15) && (msY <= sY + 124 + i * 15))
                    {
                        bFlag = true;

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
                if (m_pBankList[i] != 0) iTotalLines++;
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
            else m_stDialogBoxInfo[14].bIsScrollSelected = false;
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
            if (bFlag == false)
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
    short sX{}, sY{}, sSprH{}, sFrame{};
    int i{}, iR{}, iG{}, iB{}, iSkirtDraw = 0;
    char cTxt2[120]{}, cEquipPoiStatus[DEF_MAXITEMEQUIPPOS]{};
    char cItemColor{}, cCollison{};

    sX = m_stDialogBoxInfo[1].sX;
    sY = m_stDialogBoxInfo[1].sY;
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 0, false, m_bDialogTrans);

    memset(G_cTxt, 0, sizeof(G_cTxt));
    strcpy(G_cTxt, m_cPlayerName);
    strcat(G_cTxt, " : ");

    if (m_iPKCount > 0)
    {
        memset(cTxt2, 0, sizeof(cTxt2));
        format_to_local(cTxt2, DRAW_DIALOGBOX_CHARACTER1, m_iPKCount);
        strcat(G_cTxt, cTxt2);
    }

    memset(cTxt2, 0, sizeof(cTxt2));
    format_to_local(cTxt2, DRAW_DIALOGBOX_CHARACTER2, m_iContribution, m_iRating);
    strcat(G_cTxt, cTxt2);

    put_aligned_string(sX + 24, sX + 252, sY + 52, G_cTxt, 45, 20, 20);

    memset(G_cTxt, 0, sizeof(G_cTxt));

    if (m_bCitizen == false) strcpy(G_cTxt, DRAW_DIALOGBOX_CHARACTER7);
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
            if (m_iGuildRank == 1) strcat(G_cTxt, DEF_MSG_GUILDCOMASTER1);
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
        cEquipPoiStatus[i] = -1;

    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))	cEquipPoiStatus[m_pItemList[i]->m_cEquipPos] = i;
    }

    if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
    {
        cCollison = -1;

        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 0]->put_sprite_fast(sX + 171, sY + 290, m_sPlayerType - 1, m_dwCurTime);

        if (cEquipPoiStatus[DEF_EQUIPPOS_HEAD] == -1)
        {
            _GetHairColorRGB(((m_sPlayerAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 18]->put_sprite_color(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x0F00) >> 8, iR, iG, iB, m_dwCurTime);
        }

        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 19]->put_sprite_fast(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x000F), m_dwCurTime);
        if (cEquipPoiStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_BACK]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_BODY]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_NECK]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]] == false)
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
            sSprH = m_pItemList[cEquipPoiStatus[cCollison]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[cCollison]]->m_sSpriteFrame;
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
        //
    }
    else if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
    {
        cCollison = -1;
        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 40]->put_sprite_fast(sX + 171, sY + 290, m_sPlayerType - 4, m_dwCurTime);

        if (cEquipPoiStatus[DEF_EQUIPPOS_HEAD] == -1)
        {
            _GetHairColorRGB(((m_sPlayerAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 18 + 40]->put_sprite_color(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x0F00) >> 8, iR, iG, iB, m_dwCurTime);
        }

        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 19 + 40]->put_sprite_fast(sX + 171, sY + 290, (m_sPlayerAppr1 & 0x000F), m_dwCurTime);
        if ((cEquipPoiStatus[DEF_EQUIPPOS_PANTS] != -1))
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if ((sSprH == 12) && (sFrame == 0)) iSkirtDraw = 1;
        }

        if (cEquipPoiStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BACK]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_BACK]] == false)
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

        if ((cEquipPoiStatus[DEF_EQUIPPOS_BOOTS] != -1) && (iSkirtDraw == 1))
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_PANTS]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_ARMS]] == false)
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

        if ((cEquipPoiStatus[DEF_EQUIPPOS_BOOTS] != -1) && (iSkirtDraw == 0))
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_BOOTS]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_BODY]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_BODY]] == false)
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
        if (cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_FULLBODY]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_LHAND]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_RHAND]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_TWOHAND]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_NECK]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_NECK]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_RFINGER]] == false)
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

        if (cEquipPoiStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            cItemColor = m_pItemList[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]]->m_cItemColor;

            if (m_bIsItemDisabled[cEquipPoiStatus[DEF_EQUIPPOS_HEAD]] == false)
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
            sSprH = m_pItemList[cEquipPoiStatus[cCollison]]->m_sSprite;
            sFrame = m_pItemList[cEquipPoiStatus[cCollison]]->m_sSpriteFrame;
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
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 15, sY + 340, 5, false, m_bDialogTrans);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 15, sY + 340, 4, false, m_bDialogTrans);


    if ((msX >= sX + 98) && (msX <= sX + 98 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 98, sY + 340, 45, false, m_bDialogTrans);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 98, sY + 340, 44, false, m_bDialogTrans);


    if ((msX >= sX + 180) && (msX <= sX + 180 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 180, sY + 340, 11, false, m_bDialogTrans);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 180, sY + 340, 10, false, m_bDialogTrans);
}

void CGame::DrawDialogBox_LevelUpSetting(short msX, short msY)
{
    short sX{}, sY{}, szX{};
    int64_t dwTime = m_dwCurTime;
    char cTxt[120]{};
    int iStats{};
    sX = m_stDialogBoxInfo[12].sX;
    sY = m_stDialogBoxInfo[12].sY;
    szX = m_stDialogBoxInfo[12].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX + 16, sY + 100, 4);

    put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_LEVELUP_SETTING1, Color::Black);
    put_aligned_string(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_LEVELUP_SETTING2, Color::Black);

    // Points Left - Display in green if > 0
    format_to_local(cTxt, DRAW_DIALOGBOX_LEVELUP_SETTING3, m_iLU_Point);
    put_string(sX + 15, sY + 103, cTxt, Color::Black);
    //put_string(sX + 20,  sY + 85, DRAW_DIALOGBOX_LEVELUP_SETTING3, Color(0,0,0));
    //format_to_local(cTxt, "{}", m_iLU_Point);
    //put_string(sX + 73,  sY + 102, cTxt, Color(0,0,0));

    // Strength
    put_string(sX + 24, sY + 125, DRAW_DIALOGBOX_LEVELUP_SETTING4, Color::Black);
    format_to_local(cTxt, "{}", m_iStr);
    put_string(sX + 109, sY + 125, cTxt, Color::Black);
    iStats = m_iStr + m_cLU_Str;
    format_to_local(cTxt, "{}", iStats);
    if (iStats != m_iStr)
    {
        put_string(sX + 162, sY + 125, cTxt, Color::Green);
    }
    else
    {
        put_string(sX + 162, sY + 125, cTxt, Color::Black);
    }
    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 127) && (msY <= sY + 139) && (m_iStr < m_iStatLimit))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 195, sY + 127, 5, dwTime);
    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 127) && (msY <= sY + 139) && (m_cLU_Str > 0))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 210, sY + 127, 6, dwTime);

    // Vitality
    put_string(sX + 24, sY + 144, DRAW_DIALOGBOX_LEVELUP_SETTING5, Color::Black);
    format_to_local(cTxt, "{}", m_iVit);
    put_string(sX + 109, sY + 144, cTxt, Color::Black);
    iStats = m_iVit + m_cLU_Vit;
    format_to_local(cTxt, "{}", iStats);
    if (iStats != m_iVit)
    {
        put_string(sX + 162, sY + 144, cTxt, Color::Green);
    }
    else
    {
        put_string(sX + 162, sY + 144, cTxt, Color::Black);
    }
    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 146) && (msY <= sY + 158) && (m_iVit < m_iStatLimit))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 195, sY + 146, 5, dwTime);
    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 146) && (msY <= sY + 158) && (m_cLU_Vit > 0))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 210, sY + 146, 6, dwTime);

    // Dexterity
    put_string(sX + 24, sY + 163, DRAW_DIALOGBOX_LEVELUP_SETTING6, Color::Black);
    format_to_local(cTxt, "{}", m_iDex);
    put_string(sX + 109, sY + 163, cTxt, Color::Black);
    iStats = m_iDex + m_cLU_Dex;
    format_to_local(cTxt, "{}", iStats);
    if (iStats != m_iDex)
    {
        put_string(sX + 162, sY + 163, cTxt, Color::Green);
    }
    else
    {
        put_string(sX + 162, sY + 163, cTxt, Color::Black);
    }
    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 165) && (msY <= sY + 177) && (m_iDex < m_iStatLimit))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 195, sY + 165, 5, dwTime);
    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 165) && (msY <= sY + 177) && (m_cLU_Dex > 0))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 210, sY + 165, 6, dwTime);

    // Intelligence
    put_string(sX + 24, sY + 182, DRAW_DIALOGBOX_LEVELUP_SETTING7, Color::Black);
    format_to_local(cTxt, "{}", m_iInt);
    put_string(sX + 109, sY + 182, cTxt, Color::Black);
    iStats = m_iInt + m_cLU_Int;
    format_to_local(cTxt, "{}", iStats);
    if (iStats != m_iInt)
    {
        put_string(sX + 162, sY + 182, cTxt, Color::Green);
    }
    else
    {
        put_string(sX + 162, sY + 182, cTxt, Color::Black);
    }
    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 184) && (msY <= sY + 196) && (m_iInt < m_iStatLimit))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 195, sY + 184, 5, dwTime);
    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 184) && (msY <= sY + 196) && (m_cLU_Int > 0))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 210, sY + 184, 6, dwTime);

    // Magic
    put_string(sX + 24, sY + 201, DRAW_DIALOGBOX_LEVELUP_SETTING8, Color::Black);
    format_to_local(cTxt, "{}", m_iMag);
    put_string(sX + 109, sY + 201, cTxt, Color::Black);
    iStats = m_iMag + m_cLU_Mag;
    format_to_local(cTxt, "{}", iStats);
    if (iStats != m_iMag)
    {
        put_string(sX + 162, sY + 201, cTxt, Color::Green);
    }
    else
    {
        put_string(sX + 162, sY + 201, cTxt, Color::Black);
    }
    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 203) && (msY <= sY + 205) && (m_iMag < m_iStatLimit))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 195, sY + 203, 5, dwTime);
    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 203) && (msY <= sY + 205) && (m_cLU_Mag > 0))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 210, sY + 203, 6, dwTime);

    // Charisma
    put_string(sX + 24, sY + 220, DRAW_DIALOGBOX_LEVELUP_SETTING9, Color::Black);
    format_to_local(cTxt, "{}", m_iCharisma);
    put_string(sX + 109, sY + 220, cTxt, Color::Black);
    iStats = m_iCharisma + m_cLU_Char;
    format_to_local(cTxt, "{}", iStats);
    if (iStats != m_iCharisma)
    {
        put_string(sX + 162, sY + 220, cTxt, Color::Green);
    }
    else
    {
        put_string(sX + 162, sY + 220, cTxt, Color::Black);
    }
    if ((msX >= sX + 195) && (msX <= sX + 205) && (msY >= sY + 222) && (msY <= sY + 224) && (m_iCharisma < m_iStatLimit))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 195, sY + 222, 5, dwTime);
    if ((msX >= sX + 210) && (msX <= sX + 220) && (msY >= sY + 222) && (msY <= sY + 224) && (m_cLU_Char > 0))
        m_pSprite[DEF_SPRID_INTERFACE_ND_GAME4]->put_sprite_fast(sX + 210, sY + 222, 6, dwTime);

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
    if ((m_cLU_Str == 0) && (m_cLU_Vit == 0) && (m_cLU_Dex == 0) && (m_cLU_Int == 0) && (m_cLU_Mag == 0) && (m_cLU_Char == 0))
    {
        if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            if (m_iLU_Point <= 0) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 11);
        }
        else
        {
            if (m_iLU_Point <= 0) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 10);
        }
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
    memset( cStr1, 0, sizeof(cStr1) );
    memset( cStr2, 0, sizeof(cStr2) );
    memset( cStr3, 0, sizeof(cStr3) );

    switch (m_stDialogBoxInfo[14].cMode) {
    case -1:
        put_string(sX + 30 + 15, sY + 70, DRAW_DIALOGBOX_BANK1, Color(0,0,0));
        put_string(sX + 30 + 15, sY + 85, DRAW_DIALOGBOX_BANK2, Color(0,0,0));
        break;

    case 0:
        for (i = 0; i < m_stDialogBoxInfo[14].sV1; i++)
        if ((m_pBankList[i + m_stDialogBoxInfo[14].sView] != 0) && ((i + m_stDialogBoxInfo[14].sView) < DEF_MAXBANKITEMS)) {
            GetItemName(m_pBankList[i + m_stDialogBoxInfo[14].sView], cStr1, cStr2, cStr3, 64);
            if ((msX > sX + 30) && (msX < sX + 210) && (msY >= sY + 50 + i*15) && (msY <= sY + 64 + i*15))
            {
                m_stMCursor.sCursorFrame = -1;
                if( (strlen(cStr2)==0) && (strlen(cStr3)==0) ) put_aligned_string(sX + 24, sX +240, sY + 50 + i*15, cStr1, 255,255,255);
                else
                {
                    memset( G_cTxt, 0, sizeof(G_cTxt) );
                    if( (strlen(cStr1)+strlen(cStr2)+strlen(cStr3)) < 36 )
                    {
                        if( (strlen(cStr2)>0) && (strlen(cStr3)>0) ) format_to_local( G_cTxt, "{}({}, {})", cStr1, cStr2, cStr3 );
                        else format_to_local( G_cTxt, "{}({}{})", cStr1, cStr2, cStr3 );
                        put_aligned_string(sX, sX + szX, sY + 50 + i*15, G_cTxt, 255,255,255);
                    }
                    else
                    {
                        if( (strlen(cStr2)>0) && (strlen(cStr3)>0) ) format_to_local( G_cTxt, "({}, {})", cStr2, cStr3 );
                        else format_to_local( G_cTxt, "({}{})", cStr2, cStr3 );
                        put_aligned_string(sX, sX + szX, sY + 50 + i*15, cStr1, 255,255,255);
                        put_aligned_string(sX, sX + szX, sY + 50 + i*15 + 15, G_cTxt, 200,200,200);
                        i++;
                    }
                }
                cItemColor = m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cItemColor;
                if( cItemColor == 0 )
                {
                     m_pSprite[DEF_SPRID_ITEMGROUND_PIVOTPOINT + m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSprite]->put_sprite_fast(msX, msY, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSpriteFrame, m_dwCurTime);
                }
                else
                {
                    if ((m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos == DEF_EQUIPPOS_LHAND) ||
                        (m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos == DEF_EQUIPPOS_RHAND) ||
                        (m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND)) {
                        m_pSprite[DEF_SPRID_ITEMGROUND_PIVOTPOINT + m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSprite]->put_sprite_color(msX, msY, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSpriteFrame,
                                                                            m_wWR[cItemColor] -m_wR[0], m_wWG[cItemColor] -m_wG[0], m_wWB[cItemColor] -m_wB[0], m_dwCurTime);
                    }
                    else m_pSprite[DEF_SPRID_ITEMGROUND_PIVOTPOINT + m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSprite]->put_sprite_color(msX, msY, m_pBankList[i + m_stDialogBoxInfo[14].sView]->m_sSpriteFrame,
                                                                            m_wR[cItemColor] -m_wR[0], m_wG[cItemColor] -m_wG[0], m_wB[cItemColor] -m_wB[0], m_dwCurTime);
                }
            }
            else
            {
                if( (strlen(cStr2)==0) && (strlen(cStr3)==0) ) put_aligned_string(sX + 24, sX +240, sY + 50 + i*15, cStr1);
                else
                {
                    memset( G_cTxt, 0, sizeof(G_cTxt) );
                    if( (strlen(cStr1)+strlen(cStr2)+strlen(cStr3)) < 36 )
                    {
                        if( (strlen(cStr2)>0) && (strlen(cStr3)>0) ) format_to_local( G_cTxt, "{}({}, {})", cStr1, cStr2, cStr3 );
                        else format_to_local( G_cTxt, "{}({}{})", cStr1, cStr2, cStr3 );
                        put_aligned_string(sX, sX + szX, sY + 50 + i*15, G_cTxt);
                    }
                    else put_aligned_string(sX, sX + szX, sY + 50 + i*15, cStr1);
                }
            }
        }

        iTotalLines = 0;
        for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pBankList[i] != 0) iTotalLines++;
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
        else m_stDialogBoxInfo[14].bIsScrollSelected = false;
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
        put_aligned_string(sX, sX+szX, sY + 265, DRAW_DIALOGBOX_BANK3);
        put_aligned_string(sX, sX+szX, sY + 280, DRAW_DIALOGBOX_BANK4);
        put_aligned_string(sX, sX+szX, sY + 295, DRAW_DIALOGBOX_BANK5);
        break;
    }
}
*/



void CGame::DrawDialogBox_CityHallMenu(short msX, short msY)
{
    short sX{}, sY{}, szX{};
    char cTxt[120]{};

    sX = m_stDialogBoxInfo[13].sX;
    sY = m_stDialogBoxInfo[13].sY;
    szX = m_stDialogBoxInfo[13].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 18);//CityHall Menu Text

    switch (m_stDialogBoxInfo[13].cMode)
    {
        case 0:
            // citizenship req
            if (m_bCitizen == false)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
                    put_aligned_string(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_CITYHALL_MENU1, 255, 255, 255);	//"
                else put_aligned_string(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_CITYHALL_MENU1, 4, 0, 50);		//"
            }
            else
                put_aligned_string(sX, sX + szX, sY + 70, DRAW_DIALOGBOX_CITYHALL_MENU1, 65, 65, 65);			//"

            if (m_iRewardGold > 0)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
                    put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU4, 255, 255, 255);	//"
                else put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU4, 4, 0, 50);		//"
            }
            else
                put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU4, 65, 65, 65);			//"

            if ((m_iEnemyKillCount >= 100) && (m_iContribution >= 10))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
                    put_aligned_string(sX, sX + szX, sY + 120, DRAW_DIALOGBOX_CITYHALL_MENU8, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 120, DRAW_DIALOGBOX_CITYHALL_MENU8, 4, 0, 50);
            }
            else
                put_aligned_string(sX, sX + szX, sY + 120, DRAW_DIALOGBOX_CITYHALL_MENU8, 65, 65, 65);

            //Change DK items from cityhall
            //0x0102
            if (m_iLevel >= 180)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 145) && (msY < sY + 170))
                    put_aligned_string(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_CITYHALL_MENU9, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_CITYHALL_MENU9, 4, 0, 50);
            }
            else
                put_aligned_string(sX, sX + szX, sY + 145, DRAW_DIALOGBOX_CITYHALL_MENU9, 65, 65, 65);

            // Cancel quest
            if (m_stQuest.sQuestType != 0)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 170) && (msY < sY + 195))
                    put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU11, 255, 255, 255);//"
                else put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU11, 4, 0, 50);//"
            }
            else
                put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU11, 65, 65, 65);//"

            // change playmode
            if ((m_bIsCrusadeMode == false) && m_bCitizen && (m_iPKCount == 0))
            {
                if (m_bHunter == true)
                {
                    if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 195) && (msY < sY + 220))
                        put_aligned_string(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU56, 255, 255, 255);
                    else
                        put_aligned_string(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU56, 4, 0, 50);
                }
                else if (m_iLevel < 100)
                {
                    if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 195) && (msY < sY + 220))
                        put_aligned_string(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU56, 255, 255, 255);
                    else
                        put_aligned_string(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU56, 4, 0, 50);
                }
                else // Disable...
                    put_aligned_string(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU56, 65, 65, 65);
            }
            else
                put_aligned_string(sX, sX + szX, sY + 195, DRAW_DIALOGBOX_CITYHALL_MENU56, 65, 65, 65);

            // Teleport menu
            if ((m_bIsCrusadeMode == false) && m_bCitizen && (m_iPKCount == 0))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 220) && (msY < sY + 245))
                    put_aligned_string(sX, sX + szX, sY + 220, DRAW_DIALOGBOX_CITYHALL_MENU69, 255, 255, 255);//"Teleporting to dungeon level 2."
                else
                    put_aligned_string(sX, sX + szX, sY + 220, DRAW_DIALOGBOX_CITYHALL_MENU69, 4, 0, 50);
            }
            else
                put_aligned_string(sX, sX + szX, sY + 220, DRAW_DIALOGBOX_CITYHALL_MENU69, 65, 65, 65);

            //Change crusade role
            if (m_bIsCrusadeMode && m_bCitizen)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 245) && (msY < sY + 270))
                    put_aligned_string(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU14, 255, 255, 255);//"Change the crusade assignment."
                else
                    put_aligned_string(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU14, 4, 0, 50);//"
            }
            else
                put_aligned_string(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU14, 65, 65, 65);//"

            put_aligned_string(sX, sX + szX, sY + 270, DRAW_DIALOGBOX_CITYHALL_MENU17);//"Select an Item you want."

            break;

        case 1: // become citizen warning
            put_aligned_string(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU18, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU19, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU20, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU21, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU22, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU23, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU24, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 200, DRAW_DIALOGBOX_CITYHALL_MENU25, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU26, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 230, DRAW_DIALOGBOX_CITYHALL_MENU27, 55, 25, 25);//"

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 2: // Offering Citizenship.
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU28, 55, 25, 25);//"
            break;

        case 3: //Congratulations!! You acquired Citizenship"
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU29, 55, 25, 25);//"

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 4: //Failed on acquiring citizenship!"
            put_aligned_string(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU30, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 100, DRAW_DIALOGBOX_CITYHALL_MENU31, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 115, DRAW_DIALOGBOX_CITYHALL_MENU32, 55, 25, 25);//"

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            break;

        case 5: //The prize gold for your"
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU33, 55, 25, 25);//"
            format_to_local(cTxt, DRAW_DIALOGBOX_CITYHALL_MENU34, m_iRewardGold);//"
            put_aligned_string(sX, sX + szX, sY + 140, cTxt, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU35, 55, 25, 25);//"

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 7:// 3.51 Cityhall Menu - Request Hero's Items
            put_aligned_string(sX, sX + szX, sY + 60, DRAW_DIALOGBOX_CITYHALL_MENU46, 255, 255, 255);// Here are the Hero's Item aivable :
            // Hero's Cape (EK 300)
            if (m_iEnemyKillCount >= 300)
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 110))
                    put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU47, 255, 255, 255);// On mouse over Mode
                else put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU47, 4, 0, 50);// Normal Mode
            }
            else put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU47, 65, 65, 65);// Disabled Mode
            // Hero's Helm (EK 150 - Contrib 20)
            if ((m_iEnemyKillCount >= 150) && (m_iContribution >= 20))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 125) && (msY < sY + 140))
                    put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU48, 255, 255, 255);// On mouse over Mode
                else put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU48, 4, 0, 50);// Normal Mode
            }
            else put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU48, 65, 65, 65);// Disabled Mode
            // Hero's Cap (EK 100 - Contrib 20)
            if ((m_iEnemyKillCount >= 100) && (m_iContribution >= 20))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 155) && (msY < sY + 170))
                    put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU49, 255, 255, 255);// On mouse over Mode
                else put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU49, 4, 0, 50);// Normal Mode
            }
            else put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU49, 65, 65, 65);// Disabled Mode
            // Hero's Armor (EK 300 - Contrib 30)
            if ((m_iEnemyKillCount >= 300) && (m_iContribution >= 30))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 185) && (msY < sY + 200))
                    put_aligned_string(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_CITYHALL_MENU50, 255, 255, 255);// On mouse over Mode
                else put_aligned_string(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_CITYHALL_MENU50, 4, 0, 50);// Normal Mode
            }
            else put_aligned_string(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_CITYHALL_MENU50, 65, 65, 65);// Disabled Mode
            // Hero's Robe (EK 200 - Contrib 20)
            if ((m_iEnemyKillCount >= 200) && (m_iContribution >= 20))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 215) && (msY < sY + 230))
                    put_aligned_string(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU51, 255, 255, 255);// On mouse over Mode
                else put_aligned_string(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU51, 4, 0, 50);// Normal Mode
            }
            else put_aligned_string(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU51, 65, 65, 65);// Disabled Mode
            // Hero's Hauberk (EK 100 - Contrib 10)
            if ((m_iEnemyKillCount >= 100) && (m_iContribution >= 10))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 245) && (msY < sY + 260))
                    put_aligned_string(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU52, 255, 255, 255);// On mouse over Mode
                else put_aligned_string(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU52, 4, 0, 50);// Normal Mode
            }
            else put_aligned_string(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU52, 65, 65, 65);// Disabled Mode
            // Hero's Leggings (EK 150 - Contrib 15)
            if ((m_iEnemyKillCount >= 150) && (m_iContribution >= 15))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 275) && (msY < sY + 290))
                    put_aligned_string(sX, sX + szX, sY + 275, DRAW_DIALOGBOX_CITYHALL_MENU53, 255, 255, 255);// On mouse over Mode
                else put_aligned_string(sX, sX + szX, sY + 275, DRAW_DIALOGBOX_CITYHALL_MENU53, 4, 0, 50);// Normal Mode
            }
            else put_aligned_string(sX, sX + szX, sY + 275, DRAW_DIALOGBOX_CITYHALL_MENU53, 65, 65, 65);// Disabled Mode
            break;

        case 8: // cancel current quest?
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU54, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU55, 55, 25, 25);//"

            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 9: // You are civilian/ combatant now
            if (m_bHunter)
            {
                put_aligned_string(sX, sX + szX, sY + 53, DRAW_DIALOGBOX_CITYHALL_MENU57, 200, 200, 25);//"
            }
            else
            {
                put_aligned_string(sX, sX + szX, sY + 53, DRAW_DIALOGBOX_CITYHALL_MENU58, 200, 200, 25);//"
            }
            put_aligned_string(sX, sX + szX, sY + 78, DRAW_DIALOGBOX_CITYHALL_MENU59, 55, 25, 25);//"

            put_string(sX + 35, sY + 108, DRAW_DIALOGBOX_CITYHALL_MENU60, Color(220, 130, 45));//"
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU61, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU62, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU63, 55, 25, 25);//"
            put_string(sX + 35, sY + 177, DRAW_DIALOGBOX_CITYHALL_MENU64, Color(220, 130, 45));//"
            put_aligned_string(sX, sX + szX, sY + 194, DRAW_DIALOGBOX_CITYHALL_MENU65, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 209, DRAW_DIALOGBOX_CITYHALL_MENU66, 55, 25, 25);//"
            put_aligned_string(sX, sX + szX, sY + 224, DRAW_DIALOGBOX_CITYHALL_MENU67, 55, 25, 25);//"

            put_aligned_string(sX, sX + szX, sY + 252, DRAW_DIALOGBOX_CITYHALL_MENU68, 55, 25, 25);//"
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

        case 10: // TP 2nd screen
            if (m_iTeleportMapCount > 0)
            {
                put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_CITYHALL_MENU69, 55, 25, 25);//"Teleporting to dungeon level 2."
                put_aligned_string(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU70, 55, 25, 25);//"5000Gold is required"
                put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU71, 55, 25, 25);//"to teleport to dungeon level 2."
                put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU72, 55, 25, 25);//"Would you like to teleport?"
                put_string2(sX + 35, sY + 250, DRAW_DIALOGBOX_CITYHALL_MENU72_1, 55, 25, 25);//"Civilians cannot go some area."
                for (int i = 0; i < m_iTeleportMapCount; i++)
                {
                    memset(cTxt, 0, sizeof(cTxt));
                    GetOfficialMapName(m_stTeleportList[i].mapname, cTxt);
                    format_to_local(G_cTxt, DRAW_DIALOGBOX_CITYHALL_MENU77, cTxt, m_stTeleportList[i].iCost);
                    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 130 + i * 15) && (msY <= sY + 144 + i * 15))
                        put_aligned_string(sX, sX + szX, sY + 130 + i * 15, G_cTxt, 255, 255, 255);
                    else put_aligned_string(sX, sX + szX, sY + 130 + i * 15, G_cTxt, 0, 250, 0);
                }
            }
            else if (m_iTeleportMapCount == -1)
            {
                put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU73, 55, 25, 25);//"Now it's searching for possible area"
                put_aligned_string(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_CITYHALL_MENU74, 55, 25, 25);//"to teleport."
                put_aligned_string(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_CITYHALL_MENU75, 55, 25, 25);//"Please wait for a moment."
            }
            else
            {
                put_aligned_string(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_CITYHALL_MENU76, 55, 25, 25);//"There is no area that you can teleport."
            }
            break;

        case 11:
            put_aligned_string(sX, sX + szX - 1, sY + 125, m_cTakeHeroItemName, 55, 25, 25);
            put_aligned_string(sX + 1, sX + szX, sY + 125, m_cTakeHeroItemName, 55, 25, 25);
            put_aligned_string(sX, sX + szX, sY + 260, DRAW_DIALOGBOX_CITYHALL_MENU46A, 55, 25, 25);
            if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX)
                && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 18);
            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX)
                && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 2);
            break;

            //Change DK items from city hall
            //0x0102
        case 15:
            put_aligned_string(sX, sX + szX, sY + 60, DRAW_DIALOGBOX_CITYHALL_MENU80, 255, 255, 255);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 110))
                put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU81, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU81, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 110) && (msY < sY + 125))
                put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU82, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU82, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 125) && (msY < sY + 140))
                put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU83, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU83, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 140) && (msY < sY + 155))
                put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU84, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_CITYHALL_MENU84, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 155) && (msY < sY + 170))
                put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU85, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_CITYHALL_MENU85, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 170) && (msY < sY + 185))
                put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU86, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_CITYHALL_MENU86, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 185) && (msY < sY + 200))
                put_aligned_string(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_CITYHALL_MENU87, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 185, DRAW_DIALOGBOX_CITYHALL_MENU87, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 200) && (msY < sY + 215))
                put_aligned_string(sX, sX + szX, sY + 200, DRAW_DIALOGBOX_CITYHALL_MENU88, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 200, DRAW_DIALOGBOX_CITYHALL_MENU88, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 215) && (msY < sY + 230))
                put_aligned_string(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU89, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 215, DRAW_DIALOGBOX_CITYHALL_MENU89, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 230) && (msY < sY + 245))
                put_aligned_string(sX, sX + szX, sY + 230, DRAW_DIALOGBOX_CITYHALL_MENU90, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 230, DRAW_DIALOGBOX_CITYHALL_MENU90, 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 245) && (msY < sY + 260))
                put_aligned_string(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU91, 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 245, DRAW_DIALOGBOX_CITYHALL_MENU91, 4, 0, 50);
            break;
    }
}
void CGame::DrawDialogBox_ConfirmExchange(short msX, short msY)
{
    short sX{}, sY{};
    sX = m_stDialogBoxInfo[41].sX;
    sY = m_stDialogBoxInfo[41].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME1, sX, sY, 2);
    switch (m_stDialogBoxInfo[41].cMode)
    {
        case 1: // Question
            put_string(sX + 35, sY + 30, "Do you really want to exchange?", Color(4, 0, 50));
            put_string(sX + 36, sY + 30, "Do you really want to exchange?", Color(4, 0, 50));

            if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 19);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 18);

            if ((msX >= sX + 170) && (msX <= sX + 170 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 3);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 2);
            break;

        case 2: // Waiting for response
            put_string(sX + 45, sY + 36, "Waiting for response...", Color(4, 0, 50));
            put_string(sX + 46, sY + 36, "Waiting for response...", Color(4, 0, 50));
            break;
    }
}

void CGame::DrawDialogBox_Exchange(short msX, short msY)
{
    short sX{}, sY{}, szX{}, sXadd{};
    int64_t dwTime = m_dwCurTime;
    char cItemColor{}, cTxt[120]{}, cTxt2[128]{};
    char cNameStr[120]{}, cSubStr1[120]{}, cSubStr2[120]{};
    int iLoc{}, i{};

    sX = m_stDialogBoxInfo[27].sX;
    sY = m_stDialogBoxInfo[27].sY;
    szX = m_stDialogBoxInfo[27].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_NEWEXCHANGE, sX, sY, 0);
    switch (m_stDialogBoxInfo[27].cMode)
    {
        case 1:
            put_aligned_string(sX + 80, sX + 180, sY + 38, m_cPlayerName, 35, 55, 35);
            if (m_stDialogBoxExchangeInfo[4].sV1 != -1)
                put_aligned_string(sX + 250, sX + 540, sY + 38, m_stDialogBoxExchangeInfo[4].cStr2, 35, 55, 35);
            for (i = 0; i < 8; i++)
            {
                sXadd = (58 * i) + 48; if (i > 3) sXadd += 20;
                if (m_stDialogBoxExchangeInfo[i].sV1 != -1)
                {
                    cItemColor = m_stDialogBoxExchangeInfo[i].sV4;
                    if (cItemColor == 0)
                    {
                        m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->put_sprite_fast(sX + sXadd, sY + 130, m_stDialogBoxExchangeInfo[i].sV2, dwTime);
                    }
                    else
                    {
                        switch (m_stDialogBoxExchangeInfo[i].sV1)
                        {
                            case 1: //  Swds
                            case 2: //  Bows
                            case 3: //  Shields
                            case 15: // Axes hammers
                            case 17: // Wands
                                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->put_sprite_color(sX + sXadd, sY + 130
                                    , m_stDialogBoxExchangeInfo[i].sV2, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                                break;
                            default: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->put_sprite_color(sX + sXadd, sY + 130
                                , m_stDialogBoxExchangeInfo[i].sV2, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                                break;
                        }
                    }
                    GetItemName(m_stDialogBoxExchangeInfo[i].cStr1, m_stDialogBoxExchangeInfo[i].dwV1, cNameStr, cSubStr1, cSubStr2, 120);
                    // If pointer over item then show this item data
                    if ((msX >= sX + sXadd - 6) && (msX <= sX + sXadd + 42)
                        && (msY >= sY + 61) && (msY <= sY + 200))
                    {
                        format_to_local(cTxt, "{}", cNameStr);
                        if (m_bIsSpecial)
                        {
                            put_aligned_string(sX + 15, sX + 155, sY + 215, cTxt, 0, 255, 50);
                            put_aligned_string(sX + 16, sX + 156, sY + 215, cTxt, 0, 255, 50);
                        }
                        else
                        {
                            put_aligned_string(sX + 15, sX + 155, sY + 215, cTxt, 35, 35, 35);
                            put_aligned_string(sX + 16, sX + 156, sY + 215, cTxt, 35, 35, 35);
                        }
                        iLoc = 0;
                        if (strlen(cSubStr1) != 0)
                        {
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cSubStr1, 0, 0, 0);
                            iLoc += 15;
                        }
                        if (strlen(cSubStr2) != 0)
                        {
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cSubStr2, 0, 0, 0);
                            iLoc += 15;
                        }
                        if (m_stDialogBoxExchangeInfo[i].sV3 != 1)
                        {
                            if (m_stDialogBoxExchangeInfo[i].sV3 > 1)
                            {
                                DisplayGold(m_stDialogBoxExchangeInfo[i].sV3);
                                strcpy(cTxt2, G_cTxt);
                            }
                            else format_to_local(cTxt2, DRAW_DIALOGBOX_EXCHANGE2, m_stDialogBoxExchangeInfo[i].sV3);
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cTxt2, 35, 35, 35);
                            iLoc += 15;
                        }
                        if (m_stDialogBoxExchangeInfo[i].sV5 != -1)
                        {
                            if (m_stDialogBoxExchangeInfo[i].sV1 == 22)
                            {
                                if ((m_stDialogBoxExchangeInfo[i].sV2 > 5) && (m_stDialogBoxExchangeInfo[i].sV2 < 10))
                                {
                                    format_to_local(cTxt, GET_ITEM_NAME2, (m_stDialogBoxExchangeInfo[i].sV7 - 100));
                                }
                            }
                            else if (m_stDialogBoxExchangeInfo[i].sV1 == 6)
                            {
                                format_to_local(cTxt, GET_ITEM_NAME1, (m_stDialogBoxExchangeInfo[i].sV7 - 100));
                            }
                            else
                            {
                                format_to_local(cTxt, GET_ITEM_NAME2, m_stDialogBoxExchangeInfo[i].sV7);
                            }
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cTxt, 35, 35, 35);
                            iLoc += 15;
                        }
                        if (iLoc < 45) // Endurance
                        {
                            format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE3, m_stDialogBoxExchangeInfo[i].sV5, m_stDialogBoxExchangeInfo[i].sV6);
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cTxt, 35, 35, 35);
                            iLoc += 15;
                        }
                    }
                }
            }
            if ((m_stDialogBoxExchangeInfo[0].sV1 != -1) && (m_stDialogBoxExchangeInfo[4].sV1 == -1))
            {
                put_aligned_string(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE9, 55, 25, 25); // Please wait until other player decides
                put_aligned_string(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE10, 55, 25, 25);// to exchange. If you want to cancel the
                put_aligned_string(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE11, 55, 25, 25);// exchange press the CANCEL button now.
                put_string_sprite_font(sX + 220, sY + 310, "Exchange", 15, 15, 15);
            }
            else if ((m_stDialogBoxExchangeInfo[0].sV1 == -1) && (m_stDialogBoxExchangeInfo[4].sV1 != -1))
            {
                put_aligned_string(sX, sX + szX, sY + 205 + 10, DRAW_DIALOGBOX_EXCHANGE12, 55, 25, 25);// Other player offered an item exchange
                put_aligned_string(sX, sX + szX, sY + 220 + 10, DRAW_DIALOGBOX_EXCHANGE13, 55, 25, 25);// Select an item which you want to exc-
                put_aligned_string(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE14, 55, 25, 25);// hange with above item, drag it to the
                put_aligned_string(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE15, 55, 25, 25);// blank and press the EXCHANGE button.
                put_aligned_string(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE16, 55, 25, 25);// And you can also reject an offer by
                put_aligned_string(sX, sX + szX, sY + 280 + 10, DRAW_DIALOGBOX_EXCHANGE17, 55, 25, 25);// pressing the CANCEL button.
                put_string_sprite_font(sX + 220, sY + 310, "Exchange", 15, 15, 15);
            }
            else if ((m_stDialogBoxExchangeInfo[0].sV1 != -1) && (m_stDialogBoxExchangeInfo[4].sV1 != -1))
            {
                put_aligned_string(sX, sX + szX, sY + 205 + 10, DRAW_DIALOGBOX_EXCHANGE18, 55, 25, 25);// The preparation for item exchange
                put_aligned_string(sX, sX + szX, sY + 220 + 10, DRAW_DIALOGBOX_EXCHANGE19, 55, 25, 25);// has been finished. Press the EXCHANGE
                put_aligned_string(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE20, 55, 25, 25);// button to exchange as above. Press the
                put_aligned_string(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE21, 55, 25, 25);// CANCEL button to cancel. Occasionally
                put_aligned_string(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE22, 55, 25, 25);// when you press the EXCHANGE button, you
                put_aligned_string(sX, sX + szX, sY + 280 + 10, DRAW_DIALOGBOX_EXCHANGE23, 55, 25, 25);// will not be able to cancel the exchange.
                if ((msX >= sX + 200) && (msX <= sX + 200 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY))
                    put_string_sprite_font(sX + 220, sY + 310, "Exchange", 6, 6, 20);
                else put_string_sprite_font(sX + 220, sY + 310, "Exchange", 0, 0, 7);
            }
            if ((msX >= sX + 450) && (msX <= sX + 450 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY)
                && (m_bIsDialogEnabled[41] == false))
                put_string_sprite_font(sX + 450, sY + 310, "Cancel", 6, 6, 20);
            else put_string_sprite_font(sX + 450, sY + 310, "Cancel", 0, 0, 7);
            break;

        case 2:
            put_aligned_string(sX + 80, sX + 180, sY + 38, m_cPlayerName, 35, 55, 35);
            if (m_stDialogBoxExchangeInfo[4].sV1 != -1)
                put_aligned_string(sX + 250, sX + 540, sY + 38, m_stDialogBoxExchangeInfo[4].cStr2, 35, 55, 35);
            for (i = 0; i < 8; i++)
            {
                sXadd = (58 * i) + 48; if (i > 3) sXadd += 20;
                if (m_stDialogBoxExchangeInfo[i].sV1 != -1)
                {
                    cItemColor = m_stDialogBoxExchangeInfo[i].sV4;
                    if (cItemColor == 0)
                    {
                        m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->put_sprite_fast(sX + sXadd, sY + 130, m_stDialogBoxExchangeInfo[i].sV2, dwTime);
                    }
                    else
                    {
                        switch (m_stDialogBoxExchangeInfo[i].sV1)
                        {
                            case 1: // Swds
                            case 2: // Bows
                            case 3: // Shields
                            case 15: // Axes hammers
                            case 17: // Wands
                                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->put_sprite_color(sX + sXadd, sY + 130,
                                    m_stDialogBoxExchangeInfo[i].sV2, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
                                break;
                            default: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxExchangeInfo[i].sV1]->put_sprite_color(sX + sXadd, sY + 130,
                                m_stDialogBoxExchangeInfo[i].sV2, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
                                break;
                        }
                    }
                    GetItemName(m_stDialogBoxExchangeInfo[i].cStr1, m_stDialogBoxExchangeInfo[i].dwV1, cNameStr, cSubStr1, cSubStr2, 120);
                    if ((msX >= sX + sXadd - 6) && (msX <= sX + sXadd + 42)
                        && (msY >= sY + 61) && (msY <= sY + 200))
                    {
                        format_to_local(cTxt, "{}", cNameStr);
                        if (m_bIsSpecial)
                        {
                            put_aligned_string(sX + 15, sX + 155, sY + 215, cTxt, 0, 255, 50);
                            put_aligned_string(sX + 16, sX + 156, sY + 215, cTxt, 0, 255, 50);
                        }
                        else
                        {
                            put_aligned_string(sX + 15, sX + 155, sY + 215, cTxt, 35, 35, 35);
                            put_aligned_string(sX + 16, sX + 156, sY + 215, cTxt, 35, 35, 35);
                        }
                        iLoc = 0;
                        if (strlen(cSubStr1) != 0)
                        {
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cSubStr1, 0, 0, 0);
                            iLoc += 15;
                        }
                        if (strlen(cSubStr2) != 0)
                        {
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cSubStr2, 0, 0, 0);
                            iLoc += 15;
                        }
                        if (m_stDialogBoxExchangeInfo[i].sV3 != 1)
                        {
                            if (m_stDialogBoxExchangeInfo[i].sV3 > 1)
                            {
                                DisplayGold(m_stDialogBoxExchangeInfo[i].sV3);
                                strcpy(cTxt2, G_cTxt);
                            }
                            else format_to_local(cTxt2, DRAW_DIALOGBOX_EXCHANGE2, m_stDialogBoxExchangeInfo[i].sV3);
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cTxt2, 35, 35, 35);
                            iLoc += 15;
                        }
                        if (m_stDialogBoxExchangeInfo[i].sV5 != -1) // completion
                        {	// Crafting Magins completion fix
                            if (m_stDialogBoxExchangeInfo[i].sV1 == 22)
                            {
                                if ((m_stDialogBoxExchangeInfo[i].sV2 > 5) && (m_stDialogBoxExchangeInfo[i].sV2 < 10))
                                {
                                    format_to_local(cTxt, GET_ITEM_NAME2, (m_stDialogBoxExchangeInfo[i].sV7 - 100)); //Completion - 100
                                }
                            }
                            else if (m_stDialogBoxExchangeInfo[i].sV1 == 6)
                            {
                                format_to_local(cTxt, GET_ITEM_NAME1, (m_stDialogBoxExchangeInfo[i].sV7 - 100)); //Purity
                            }
                            else
                            {
                                format_to_local(cTxt, GET_ITEM_NAME2, m_stDialogBoxExchangeInfo[i].sV7); //Completion
                            }
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cTxt, 35, 35, 35);
                            iLoc += 15;
                        }
                        if (iLoc < 45) // Endurance
                        {
                            format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE3, m_stDialogBoxExchangeInfo[i].sV5, m_stDialogBoxExchangeInfo[i].sV6);
                            put_aligned_string(sX + 16, sX + 155, sY + 235 + iLoc, cTxt, 35, 35, 35);
                            iLoc += 15;
                        }
                    }
                }
            }
            format_to_local(cTxt, DRAW_DIALOGBOX_EXCHANGE33, m_stDialogBoxExchangeInfo[4].cStr2);
            put_aligned_string(sX, sX + szX, sY + 205 + 10, cTxt, 55, 25, 25);                     // Please wait until {} agrees to
            put_aligned_string(sX, sX + szX, sY + 220 + 10, DRAW_DIALOGBOX_EXCHANGE34, 55, 25, 25);// exchange. The exchange can't be achieved
            put_aligned_string(sX, sX + szX, sY + 235 + 10, DRAW_DIALOGBOX_EXCHANGE35, 55, 25, 25);// unless both people agree.
            put_aligned_string(sX, sX + szX, sY + 250 + 10, DRAW_DIALOGBOX_EXCHANGE36, 55, 25, 25);//  If other player does not decide to exchange
            put_aligned_string(sX, sX + szX, sY + 265 + 10, DRAW_DIALOGBOX_EXCHANGE37, 55, 25, 25);// you can cancel the exchange by pressing the
            put_aligned_string(sX, sX + szX, sY + 280 + 10, DRAW_DIALOGBOX_EXCHANGE38, 55, 25, 25);// CANCEL button. But if other player already
            put_aligned_string(sX, sX + szX, sY + 295 + 10, DRAW_DIALOGBOX_EXCHANGE39, 55, 25, 25);// decided to exchange, you can't cancel anymore

            /*	if ( (msX >= sX + 450) && (msX <= sX + 450 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY) )
            put_string_sprite_font(sX + 450, sY + 310, "Cancel", 6,6,20);
            else put_string_sprite_font(sX + 450, sY + 310, "Cancel", 0,0,7);*/
            break;
    }
}
void CGame::DrawDialogBox_Fishing(short msX, short msY)
{
    short sX{}, sY{};
    int64_t dwTime = m_dwCurTime;
    char cTxt[120]{};

    sX = m_stDialogBoxInfo[24].sX;
    sY = m_stDialogBoxInfo[24].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    char cStr1[64], cStr2[64], cStr3[64];
    GetItemName(m_stDialogBoxInfo[24].cStr, 0, cStr1, cStr2, cStr3, 64);

    switch (m_stDialogBoxInfo[24].cMode)
    {
        case 0:
            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_stDialogBoxInfo[24].sV3]->put_sprite_fast(sX + 18 + 35, sY + 18 + 17, m_stDialogBoxInfo[24].sV4, dwTime);
            format_to_local(cTxt, "{}", cStr1);
            put_string(sX + 98, sY + 14, cTxt, Color(255, 255, 255));

            format_to_local(cTxt, DRAW_DIALOGBOX_FISHING1, m_stDialogBoxInfo[24].sV2);

            put_string(sX + 98, sY + 28, cTxt, Color(0, 0, 0));

            put_string(sX + 97, sY + 43, DRAW_DIALOGBOX_FISHING2, Color(0, 0, 0));

            format_to_local(cTxt, "{} %%", m_stDialogBoxInfo[24].sV1);
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
    short sX{}, sY{}, szX{};
    int iAdjX{}, iAdjY{};

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

#if DEF_LANGUAGE == 2
            put_string(sX + 24, sY + 176, DRAW_DIALOGBOX_GUILDMENU82, Color(55, 25, 25));
            put_string(sX + 24, sY + 192, DRAW_DIALOGBOX_GUILDMENU83, Color(55, 25, 25));
#endif


            if (iGetTopDialogBoxIndex() != 7)
                put_string(sX + 75, sY + 140, m_cGuildName, Color(255, 255, 255), false, 2);

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
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETOCCUPYFIGHTZONETICKET, 0, 0, 0, 0, 0);
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
    short sX{}, sY{};

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
//put_string(sX + 60, sY + 65, m_stGuildOpList[0].cName, Color(35,35,35));
//put_string(sX + 60, sY + 69, "____________________", Color(0,0,0));
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
    short sX{}, sY{}, sMagicCircle{}, sLevelMagic{};
    int iCPivot{}, i{}, iYloc{}, iResult{}, iManaCost{};
    char cTxt[120]{}, cMana[10]{};
    int64_t dwTime = m_dwCurTime;
    double dV1{}, dV2{}, dV3{}, dV4{};

    sX = m_stDialogBoxInfo[3].sX;
    sY = m_stDialogBoxInfo[3].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME1, sX, sY, 1, false, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 7, false, m_bDialogTrans);

    if (iGetTopDialogBoxIndex() == 3 && msZ != 0)
    {
        if (msZ > 0) m_stDialogBoxInfo[3].sView--;
        if (msZ < 0) m_stDialogBoxInfo[3].sView++;
        m_stMCursor.sZ = 0;
    }
    if (m_stDialogBoxInfo[3].sView < 0) m_stDialogBoxInfo[3].sView = 9;
    if (m_stDialogBoxInfo[3].sView > 9) m_stDialogBoxInfo[3].sView = 0;

    //Circle
    memset(cTxt, 0, sizeof(cTxt));
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
        //if (((m_cMagicMastery[iCPivot + i] != 0) && (m_pMagicCfgList[iCPivot + i] != 0)) || (iCPivot == 0)) {//Change Added circle 1 spells to always display
        if (((m_cMagicMastery[iCPivot + i] != 0) && (m_pMagicCfgList[iCPivot + i] != 0)))
        {
            if (m_pMagicCfgList[iCPivot + i] == 0)//Change Invalid Spell
                format_to_local(cTxt, "Invalid Spell!");
            else
                format_to_local(cTxt, "{}", m_pMagicCfgList[iCPivot + i]->m_cName);

            m_Misc.ReplaceString(cTxt, '-', ' ');
            iManaCost = iGetManaCost(iCPivot + i);
            if (iManaCost > m_iMP)
            {
                if (m_Misc.bCheckIMEString(cTxt) == false)
                {
                    put_string(sX + 30, sY + 73 + iYloc, cTxt, Color(41, 16, 41));
                    put_string(sX + 31, sY + 73 + iYloc, cTxt, Color(41, 16, 41));
                }
                else put_string_sprite_font(sX + 30, sY + 70 + iYloc, cTxt, 5, 5, 5);
                format_to_local(cMana, "{}", iManaCost);
                put_string_sprite_font(sX + 206, sY + 70 + iYloc, cMana, 5, 5, 5);
            }
            else
                if ((msX >= sX + 30) && (msX <= sX + 240) && (msY >= sY + 70 + iYloc) && (msY <= sY + 70 + 14 + iYloc))
                {
                    if (m_Misc.bCheckIMEString(cTxt) == false)
                    {
                        put_string(sX + 30, sY + 73 + iYloc, cTxt, Color(255, 255, 255));
                        put_string(sX + 31, sY + 73 + iYloc, cTxt, Color(255, 255, 255));
                    }
                    else put_string_sprite_font(sX + 30, sY + 70 + iYloc, cTxt, 250, 250, 250);
                    format_to_local(cMana, "{}", iManaCost);
                    put_string_sprite_font(sX + 206, sY + 70 + iYloc, cMana, 250, 250, 250);
                }
                else
                {
                    if (m_Misc.bCheckIMEString(cTxt) == false)
                    {
                        put_string(sX + 30, sY + 73 + iYloc, cTxt, Color(8, 0, 66));
                        put_string(sX + 31, sY + 73 + iYloc, cTxt, Color(8, 0, 66));
                    }
                    else put_string_sprite_font(sX + 30, sY + 70 + iYloc, cTxt, 1, 1, 8);
                    format_to_local(cMana, "{}", iManaCost);
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

    // 
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
        if (m_pItemList[i] == 0) continue;
        if (m_bIsItemEquipped[i] == true)
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

    memset(cTxt, 0, sizeof(cTxt));
    format_to_local(cTxt, DRAW_DIALOGBOX_MAGIC16, iResult);
    put_aligned_string(sX, sX + 256, sY + 267, cTxt);
    put_aligned_string(sX + 1, sX + 257, sY + 267, cTxt);

    // v2.15
    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 285) && (msY <= sY + 285 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + 285, 49, false, m_bDialogTrans);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + 285, 48, false, m_bDialogTrans);
}

void CGame::DrawDialogBox_MagicShop(short msX, short msY, short msZ)
{
    short sX{}, sY{};
    int64_t dwTime = m_dwCurTime;
    int i{};
    int iCPivot{}, iYloc{};
    char cTxt[120]{}, cMana[10]{};

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
        if ((m_pMagicCfgList[iCPivot + i] != 0) && (m_pMagicCfgList[iCPivot + i]->m_bIsVisible))
        {
            format_to_local(cTxt, "{}", m_pMagicCfgList[iCPivot + i]->m_cName);

            m_Misc.ReplaceString(cTxt, '-', ' ');
            if (m_cMagicMastery[iCPivot + i] != 0)
            {
                if (m_Misc.bCheckIMEString(cTxt) == false)
                {
                    put_string(sX + 24, sY + 73 + iYloc, cTxt, Color(41, 16, 41));
                    put_string(sX + 25, sY + 73 + iYloc, cTxt, Color(41, 16, 41));
                }
                else put_string_sprite_font(sX + 24, sY + 70 + iYloc, cTxt, 5, 5, 5);
                format_to_local(cMana, "{}", m_pMagicCfgList[iCPivot + i]->m_sValue2);
                put_string_sprite_font(sX + 200, sY + 70 + iYloc, cMana, 5, 5, 5);
                format_to_local(cMana, "{}", m_pMagicCfgList[iCPivot + i]->m_sValue3);
                put_string_sprite_font(sX + 241, sY + 70 + iYloc, cMana, 5, 5, 5);
            }
            else
                if ((msX >= sX + 24) && (msX <= sX + 24 + 135) && (msY >= sY + 70 + iYloc) && (msY <= sY + 70 + 14 + iYloc))
                {
                    if (m_Misc.bCheckIMEString(cTxt) == false)
                    {
                        put_string(sX + 24, sY + 73 + iYloc, cTxt, Color(255, 255, 255));
                        put_string(sX + 25, sY + 73 + iYloc, cTxt, Color(255, 255, 255));
                    }
                    else put_string_sprite_font(sX - 20 + 44, sY + 70 + iYloc, cTxt, 250, 250, 250);
                    format_to_local(cMana, "{}", m_pMagicCfgList[iCPivot + i]->m_sValue2);
                    put_string_sprite_font(sX - 20 + 220, sY + 70 + iYloc, cMana, 250, 250, 250);
                    format_to_local(cMana, "{}", m_pMagicCfgList[iCPivot + i]->m_sValue3);
                    put_string_sprite_font(sX - 20 + 261, sY + 70 + iYloc, cMana, 250, 250, 250);
                }
                else
                {
                    if (m_Misc.bCheckIMEString(cTxt) == false)
                    {
                        put_string(sX + 24, sY + 73 + iYloc, cTxt, Color(8, 0, 66));
                        put_string(sX + 25, sY + 73 + iYloc, cTxt, Color(8, 0, 66));
                    }
                    else put_string_sprite_font(sX - 20 + 44, sY + 70 + iYloc, cTxt, 1, 1, 8);
                    format_to_local(cMana, "{}", m_pMagicCfgList[iCPivot + i]->m_sValue2);
                    put_string_sprite_font(sX - 20 + 220, sY + 70 + iYloc, cMana, 1, 1, 8);
                    format_to_local(cMana, "{}", m_pMagicCfgList[iCPivot + i]->m_sValue3);
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

void CGame::DrawDialogBox_ShutDownMsg(short msX, short msY)
{
    short sX{}, sY{}, szX{};

    sX = m_stDialogBoxInfo[25].sX;
    sY = m_stDialogBoxInfo[25].sY;
    szX = m_stDialogBoxInfo[25].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX, sY, 2);

    switch (m_stDialogBoxInfo[25].cMode)
    {
        case 1:
            memset(G_cTxt, 0, sizeof(G_cTxt));
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
    short sX{}, sY{}, szX{};
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

            if (m_bIsDialogEnabled[21] == false)
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
#if DEF_LANGUAGE == 4
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
#if DEF_LANGUAGE == 4
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
#if DEF_LANGUAGE == 4
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
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, Color(255, 255, 255));//"º¸°ü ÇÑ´Ù"
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, Color(255, 255, 255));//"º¸°ü ÇÑ´Ù"
            }
            else
            {
                put_string(sX + 28, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, Color(4, 0, 50));//"º¸°ü ÇÑ´Ù"
                put_string(sX + 29, sY + 55, DRAW_DIALOGBOX_NPCACTION_QUERY48, Color(4, 0, 50));//"º¸°ü ÇÑ´Ù"
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

            if (m_bIsDialogEnabled[21] == false)
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

            if (m_bIsDialogEnabled[21] == false)
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
    short sX{}, sY{}, szX{};
    int i{}, iNth{};

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

            if (m_iPartyStatus != 0)
            {
                if ((msX > sX + 80) && (msX < sX + 195) && (msY > sY + 120) && (msY < sY + 140))
                    put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY7, 65, 65, 65);

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
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY17);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY18);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY19);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY20);
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
            put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_PARTY40);
            put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_PARTY41);
            put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_PARTY42);
            put_aligned_string(sX, sX + szX, sY + 140, DRAW_DIALOGBOX_PARTY43);
            put_aligned_string(sX, sX + szX, sY + 155, DRAW_DIALOGBOX_PARTY44);
            put_aligned_string(sX, sX + szX, sY + 170, DRAW_DIALOGBOX_PARTY45);

            if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
            else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_JOINPARTY, 0, 2, 0, 0, m_cMCName);//Change -- request full party on join
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
    short sX{}, sY{};
    char cTxt[120]{}, cStr1[64]{}, cStr2[64]{}, cStr3[64]{};

    sX = m_stDialogBoxInfo[17].sX;
    sY = m_stDialogBoxInfo[17].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 5);

    switch (m_stDialogBoxInfo[17].cMode)
    {
        case 1:
            GetItemName(m_pItemList[m_stDialogBoxInfo[17].sView]->m_cName, m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwAttribute, cStr1, cStr2, cStr3, 64);
            if (strlen(m_stDialogBoxInfo[17].cStr) == 0) // v1.4
                format_to_local(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT1, cStr1);
            else format_to_local(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT2, cStr1, m_stDialogBoxInfo[17].cStr);

            if (m_stDialogBoxInfo[17].sV3 < 1000) // v1.4
                put_string(sX + 30, sY + 20, cTxt, Color(55, 25, 25));

            put_string(sX + 30, sY + 35, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT3, Color(55, 25, 25));
            if (iGetTopDialogBoxIndex() != 17)
                put_string(sX + 40, sY + 57, m_cAmountString, Color(255, 255, 255), false, 2);
            format_to_local(cTxt, "__________ (0 ~ {})", m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwCount);
            put_string(sX + 38, sY + 62, cTxt, Color(25, 35, 25));
            break;

        case 20:
            GetItemName(m_pItemList[m_stDialogBoxInfo[17].sView]->m_cName, m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwAttribute, cStr1, cStr2, cStr3, 64);
            if (strlen(m_stDialogBoxInfo[17].cStr) == 0) // v1.4
                format_to_local(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT1, cStr1);
            else format_to_local(cTxt, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT2, cStr1, m_stDialogBoxInfo[17].cStr);

            if (m_stDialogBoxInfo[17].sV3 < 1000) // v1.4
                put_string(sX + 30, sY + 20, cTxt, Color(55, 25, 25));

            put_string(sX + 30, sY + 35, DRAW_DIALOGBOX_QUERY_DROP_ITEM_AMOUNT3, Color(55, 25, 25));
            put_string(sX + 40, sY + 57, m_cAmountString, Color(255, 255, 255), false, 2);
            format_to_local(cTxt, "__________ (0 ~ {})", m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwCount);
            put_string(sX + 38, sY + 62, cTxt, Color(25, 35, 25));
            break;
    }
}


void CGame::DrawDialogBox_Quest(int msX, int msY)
{
    short sX{}, sY{}, szX{};
    char cTxt[120]{}, cTemp[21]{};

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
                case 0:
                    put_aligned_string(sX, sX + szX, sY + 50 + 115 - 30, DRAW_DIALOGBOX_QUEST1, 55, 25, 25);
                    break;

                case 1:

                    if (m_stQuest.bIsQuestCompleted == false)
                        put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST2, 55, 25, 25);
                    else put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST3, 55, 25, 25);

                    memset(cTemp, 0, sizeof(cTemp));
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
                    memset(cTxt, 0, sizeof(cTxt));
                    format_to_local(cTxt, DRAW_DIALOGBOX_QUEST5, cTemp);
                    put_aligned_string(sX, sX + szX, sY + 50 + 45, cTxt, 55, 25, 25);

                    memset(cTemp, 0, sizeof(cTemp));
                    GetNpcName(m_stQuest.sTargetType, cTemp);
                    memset(cTxt, 0, sizeof(cTxt));
                    format_to_local(cTxt, NPC_TALK_HANDLER16, m_stQuest.sTargetCount, cTemp);
                    put_aligned_string(sX, sX + szX, sY + 50 + 60, cTxt, 55, 25, 25);

                    memset(cTxt, 0, sizeof(cTxt));
                    if (memcmp(m_stQuest.cTargetName, "NONE", 4) == 0)
                    {
                        strcpy(cTxt, DRAW_DIALOGBOX_QUEST31);
                        put_aligned_string(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);
                    }
                    else
                    {
                        memset(cTemp, 0, sizeof(cTemp));
                        GetOfficialMapName(m_stQuest.cTargetName, cTemp);
                        format_to_local(cTxt, DRAW_DIALOGBOX_QUEST32, cTemp);
                        put_aligned_string(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);

                        if (m_stQuest.sX != 0)
                        {
                            memset(cTxt, 0, sizeof(cTxt));
                            format_to_local(cTxt, DRAW_DIALOGBOX_QUEST33, m_stQuest.sX, m_stQuest.sY, m_stQuest.sRange);
                            put_aligned_string(sX, sX + szX, sY + 50 + 90, cTxt, 55, 25, 25);
                        }
                    }

                    memset(cTxt, 0, sizeof(cTxt));
                    format_to_local(cTxt, DRAW_DIALOGBOX_QUEST34, m_stQuest.sContribution);
                    put_aligned_string(sX, sX + szX, sY + 50 + 105, cTxt, 55, 25, 25);
                    break;

                case 7:
                    if (m_stQuest.bIsQuestCompleted == false)
                        put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST26, 55, 25, 25);
                    else put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_QUEST27, 55, 25, 25);

                    memset(cTemp, 0, sizeof(cTemp));
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
                    memset(cTxt, 0, sizeof(cTxt));
                    format_to_local(cTxt, DRAW_DIALOGBOX_QUEST29, cTemp);
                    put_aligned_string(sX, sX + szX, sY + 50 + 45, cTxt, 55, 25, 25);

                    put_aligned_string(sX, sX + szX, sY + 50 + 60, DRAW_DIALOGBOX_QUEST30, 55, 25, 25);

                    memset(cTxt, 0, sizeof(cTxt));
                    if (memcmp(m_stQuest.cTargetName, "NONE", 4) == 0)
                    {
                        strcpy(cTxt, DRAW_DIALOGBOX_QUEST31);
                        put_aligned_string(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);
                    }
                    else
                    {
                        memset(cTemp, 0, sizeof(cTemp));
                        GetOfficialMapName(m_stQuest.cTargetName, cTemp);
                        format_to_local(cTxt, DRAW_DIALOGBOX_QUEST32, cTemp);
                        put_aligned_string(sX, sX + szX, sY + 50 + 75, cTxt, 55, 25, 25);

                        if (m_stQuest.sX != 0)
                        {
                            memset(cTxt, 0, sizeof(cTxt));
                            format_to_local(cTxt, DRAW_DIALOGBOX_QUEST33, m_stQuest.sX, m_stQuest.sY, m_stQuest.sRange);
                            put_aligned_string(sX, sX + szX, sY + 50 + 90, cTxt, 55, 25, 25);
                        }
                    }

                    memset(cTxt, 0, sizeof(cTxt));
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
    short sX{}, sY{}, szX{};
    int i{}, iItem{};
    char cTemp[255]{}, cStr1[64]{}, cStr2[64]{}, cStr3[64]{};

    sX = m_stDialogBoxInfo[31].sX;
    sY = m_stDialogBoxInfo[31].sY;
    szX = m_stDialogBoxInfo[31].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 11);

    iItem = 0;
    for (i = 0; i < DEF_MAXSELLLIST; i++)
        if (m_stSellItemList[i].iIndex != -1)
        {
            memset(cTemp, 0, sizeof(cTemp));
            GetItemName(m_pItemList[m_stSellItemList[i].iIndex]->m_cName, m_pItemList[m_stSellItemList[i].iIndex]->m_dwAttribute, cStr1, cStr2, cStr3, 64);
            if (m_stSellItemList[i].iAmount > 1)
            {
                format_to_local(cTemp, DRAW_DIALOGBOX_SELL_LIST1, m_stSellItemList[i].iAmount, cStr1);
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
                        memset(G_cTxt, 0, sizeof(G_cTxt));
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
                        memset(G_cTxt, 0, sizeof(G_cTxt));
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
        put_aligned_string(sX, sX + szX, sY + 55 + 155 + 282 - 117 - 170, DRAW_DIALOGBOX_SELL_LIST9);
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
    short sX{}, sY{};
    int64_t dwTime = m_dwCurTime;
    char cItemID{}, cItemColor{}, cTxt[120]{}, cTemp[120]{}, cStr2[120]{}, cStr3[120]{};

    sX = m_stDialogBoxInfo[23].sX;
    sY = m_stDialogBoxInfo[23].sY;

    switch (m_stDialogBoxInfo[23].cMode)
    {
        case 1:
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 11);
            cItemID = m_stDialogBoxInfo[23].sV1;

            cItemColor = m_pItemList[cItemID]->m_cItemColor; // v1.4
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


            memset(cTemp, 0, sizeof(cTemp));
            memset(cStr2, 0, sizeof(cStr2));
            memset(cStr3, 0, sizeof(cStr3));

            GetItemName(m_pItemList[cItemID]->m_cName, m_pItemList[cItemID]->m_dwAttribute, cTemp, cStr2, cStr3, 120);
            if (m_stDialogBoxInfo[23].sV4 == 1) strcpy(cTxt, cTemp);
            else format_to_local(cTxt, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM1, m_stDialogBoxInfo[23].sV4, cTemp);

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
            //"°¡ Ä¡: {}Gold" 
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
            cItemColor = m_pItemList[cItemID]->m_cItemColor; // v1.4
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

            memset(cTemp, 0, sizeof(cTemp));
            memset(cStr2, 0, sizeof(cStr2));
            memset(cStr3, 0, sizeof(cStr3));

            GetItemName(m_pItemList[cItemID], cTemp, cStr2, cStr3, 120);

            format_to_local(cTxt, "{}", cTemp);

            //		put_aligned_string(sX + 25, sX + 240, sY + 60, cTxt, 45,25,25);
            //		put_aligned_string(sX + 25 +1, sX + 240 +1, sY + 60, cTxt, 45,25,25);
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
    short sX{}, sY{};
    int64_t dwTime = m_dwCurTime;
    int i{}, iTemp{};
    char cTemp[255]{}, cStr2[255]{}, cStr3[255]{};
    int iCost{}, iDiscountCost{}, iDiscountRatio{}, iMsgLoc{};
    double dTmp1{}, dTmp2{}, dTmp3{};
    int iTotalLines{}, iPointerLoc{};
    bool bFlag = true;
    double d1{}, d2{}, d3{};

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
                if (m_pItemForSaleList[i] != 0) iTotalLines++;
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
            else m_stDialogBoxInfo[11].bIsScrollSelected = false;
            if (iGetTopDialogBoxIndex() == 11 && msZ != 0)
            {
                m_stDialogBoxInfo[11].sView = m_stDialogBoxInfo[11].sView - msZ / 60;
                m_stMCursor.sZ = 0;
            }
            if (iTotalLines > 13 && m_stDialogBoxInfo[11].sView > iTotalLines - 13) m_stDialogBoxInfo[11].sView = iTotalLines - 13;
            if (m_stDialogBoxInfo[11].sView < 0 || iTotalLines < 13) m_stDialogBoxInfo[11].sView = 0;
            put_aligned_string(sX + 22, sX + 165, sY + 45, DRAW_DIALOGBOX_SHOP1);
            put_aligned_string(sX + 23, sX + 166, sY + 45, DRAW_DIALOGBOX_SHOP1);
            put_aligned_string(sX + 153, sX + 250, sY + 45, DRAW_DIALOGBOX_SHOP3);//"°¡   °Ý"
            put_aligned_string(sX + 154, sX + 251, sY + 45, DRAW_DIALOGBOX_SHOP3);//"°¡   °Ý"

            for (i = 0; i < 13; i++)
                if (((i + m_stDialogBoxInfo[11].sView) < DEF_MAXMENUITEMS) && (m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView] != 0))
                {
                    memset(cTemp, 0, sizeof(cTemp));
                    GetItemName(m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView], cTemp, cStr2, cStr3, 255);

                    if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 65) && (msY <= sY + i * 18 + 79))
                    {
                        put_aligned_string(sX + 10, sX + 190, sY + i * 18 + 65, cTemp, 255, 255, 255);
                    }
                    else put_aligned_string(sX + 10, sX + 190, sY + i * 18 + 65, cTemp, 4, 0, 50);
                }

            for (i = 0; i < 13; i++)
                if (((i + m_stDialogBoxInfo[11].sView) < DEF_MAXMENUITEMS) && (m_pItemForSaleList[i + m_stDialogBoxInfo[11].sView] != 0))
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

                    memset(cTemp, 0, sizeof(cTemp));
                    format_to_local(cTemp, "%6d", iCost);
                    if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 65) && (msY <= sY + i * 18 + 79))
                        put_aligned_string(sX + 148, sX + 260, sY + i * 18 + 65, cTemp, 255, 255, 255);
                    else	put_aligned_string(sX + 148, sX + 260, sY + i * 18 + 65, cTemp, 4, 0, 50);
                }
            break;

        default:
            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sSprite]->put_sprite_fast(sX + 62 + 30 - 35, sY + 84 + 30 - 10,
                m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_sSpriteFrame, dwTime);

            memset(cTemp, 0, sizeof(cTemp));
            GetItemName(m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1], cTemp, cStr2, cStr3, 255);

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

void CGame::DrawDialogBox_Skill(short msX, short msY, short msZ, char cLB)
{
    short sX{}, sY{};
    int i{}, iTotalLines{}, iPointerLoc{};
    char cTemp[255]{}, cTemp2[255]{};
    double d1{}, d2{}, d3{};

    sX = m_stDialogBoxInfo[15].sX;
    sY = m_stDialogBoxInfo[15].sY;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0); // Normal Dialog
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 1); // Skill Dialog Title Bar

    switch (m_stDialogBoxInfo[15].cMode)
    {
        case 0:
            for (i = 0; i < 17; i++)
                if ((i < DEF_MAXSKILLTYPE) && (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView] != 0))
                {
                    memset(cTemp, 0, sizeof(cTemp));
                    format_to_local(cTemp, "{}", m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_cName);

                    m_Misc.ReplaceString(cTemp, '-', ' ');

                    memset(cTemp2, 0, sizeof(cTemp2));
                    format_to_local(cTemp2, "{}%", m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_iLevel);

                    if ((msX >= sX + 25) && (msX <= sX + 166) && (msY >= sY + 45 + i * 15) && (msY <= sY + 59 + i * 15))
                    {
                        if ((m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_bIsUseable == true) &&
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
                        if ((m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_bIsUseable == true) &&
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
                if (m_pSkillCfgList[i] != 0) iTotalLines++;
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
            else m_stDialogBoxInfo[15].bIsScrollSelected = false;
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
    char cStr1[64], cStr2[64], cStr3[64];
    int64_t dwTime = m_dwCurTime;

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
                put_string_sprite_font(sX + iAdjX + 60, sY + iAdjY + 175, "Try Now!", 16, 16, 30);
            else put_string_sprite_font(sX + iAdjX + 60, sY + iAdjY + 175, "Try Now!", 6, 6, 20);
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
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_CREATEPOTION, 0, 0, 0, 0, 0);
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
                if (m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView] != 0)
                {

                    memset(cTemp, 0, sizeof(cTemp));
                    GetItemName(m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView]->m_cName, 0, cStr1, cStr2, cStr3, 64);
                    format_to_local(cTemp, "{}", cStr1);
                    memset(cTemp2, 0, sizeof(cTemp2));
                    format_to_local(cTemp2, "{}%%", m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView]->m_iMaxSkill);

                    if ((msX >= sX + 30) && (msX <= sX + 180) && (msY >= sY + iAdjY + 55 + iLoc * 15) && (msY <= sY + iAdjY + 69 + iLoc * 15))
                    {
                        put_string(sX + 30, sY + iAdjY + 55 + iLoc * 15, cTemp, Color(255, 255, 255));
                        put_string(sX + 190, sY + iAdjY + 55 + iLoc * 15, cTemp2, Color(255, 255, 255));
                    }
                    else
                    {
                        if (m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView]->m_bBuildEnabled == true)
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
            if ((m_stDialogBoxInfo[26].sView >= 1) && (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView - 1] != 0))
                m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_sprite_fast(sX + iAdjX + 225, sY + iAdjY + 210, 23, dwTime);
            else m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->put_trans_sprite_color(sX + iAdjX + 225, sY + iAdjY + 210, 23, 5, 5, 5, dwTime);

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView + 13] != 0)
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
                    if (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView + 13] != 0)
                        m_stDialogBoxInfo[26].sView++;
                }
            }
            if ((msZ != 0) && (iGetTopDialogBoxIndex() == 26))
            {
                m_stDialogBoxInfo[26].sView = m_stDialogBoxInfo[26].sView - msZ / 60;
                m_stMCursor.sZ = 0;
            }
            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView + 12] == 0)
            {
                while (1)
                {
                    m_stDialogBoxInfo[26].sView--;
                    if (m_stDialogBoxInfo[26].sView < 1) break;
                    if (m_pDispBuildItemList[m_stDialogBoxInfo[26].sView + 12] != 0) break;
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

            memset(cTemp, 0, sizeof(cTemp));
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
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[1] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[2] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName2, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[2] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[3] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName3, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[3] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[4] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName4, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[4] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[5] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName5, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[5] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[6] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName6, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[6] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(150, 150, 150));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bBuildEnabled == true)
            {


                //
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

                //
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

            memset(cTemp, 0, sizeof(cTemp));
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
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[1] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[2] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName2, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[2] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[3] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName3, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[3] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[4] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName4, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[4] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[5] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName5, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[5] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_iElementCount[6] != 0)
            {
                GetItemName(m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cElementName6, 0, cStr1, cStr2, cStr3, 64);
                format_to_local(cTemp, "{}", cStr1);
                if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bElementFlag[6] == true)
                    put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(45, 25, 25));
                else put_string(sX + iAdjX + 44 + 20 + 60, sY + iAdjY + 55 + iLoc * 15 + 5, cTemp, Color(120, 120, 120));
                iLoc++;
            }

            //
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

            //

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
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_BUILDITEM, 0, 0, 0, 0, m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_cName);
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

            memset(cTemp, 0, sizeof(cTemp));
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
    memset(G_cTxt, 0, sizeof(G_cTxt));
    format_to_local(G_cTxt, "{}:{}:{}:{}:{}", SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
    put_string(sX + 23, sY + 204, G_cTxt, Color(45, 25, 25));
    put_string(sX + 24, sY + 204, G_cTxt, Color(45, 25, 25));

    put_string(sX + 23, sY + 46, UPDATE_SCREEN_ON_SELECT_CHARACTER36, Color(45, 25, 25));
    put_string(sX + 24, sY + 46, UPDATE_SCREEN_ON_SELECT_CHARACTER36, Color(45, 25, 25));


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
            if (m_bMusicStat && m_pBGM.getStatus() != sf::SoundSource::Status::Playing)
                StartBGM();
            else
                m_pBGM.setVolume(m_cMusicVolume);
        }
    }
    else m_stDialogBoxInfo[19].bIsScrollSelected = false;

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

void CGame::DrawDialogBox_Commander(int msX, int msY)
{
    short sX, sY, szX, szY, MapSzX, MapSzY;
    int64_t dwTime = G_dwGlobalTime;
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

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 0, false, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 15, false, m_bDialogTrans);

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

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);
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

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);

            if ((msX >= sX + 15) && (msX <= sX + 15 + 280) && (msY >= sY + 60) && (msY <= sY + 60 + 253))
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, msX, msY, 42, false, true);
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

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);
            break;

        case 3:
            if ((m_bCitizen == true) && (m_bAresden == true))
            {
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20, sY + 220, 6, dwTime);
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 50, sY + 220, 5, dwTime);
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 100, sY + 220, 7, dwTime);
                m_pSprite[DEF_SPRID_INTERFACE_ND_CRUSADE]->put_sprite_fast(sX + 20 + 150, sY + 220, 35, dwTime);
            }
            else if ((m_bCitizen == true) && (m_bAresden == false))
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

            if ((m_bCitizen == true) && (m_bAresden == true))
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
            else if ((m_bCitizen == true) && (m_bAresden == false))
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
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, msX, msY, 41, false, true);
            }
            break;
    }
    switch (m_stDialogBoxInfo[36].cMode)
    {
        case 0:
        case 1:
        case 2:
        case 4:
            szX = 0;
            szY = 0;
            MapSzX = 0;
            MapSzY = 0;

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
                for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
                    if (m_stCrusadeStructureInfo[i].cType != 0)
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
                                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 39, false, true);
                                else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 37, false, true);
                                break;

                            case 36:
                            case 37:
                            case 39:
                                if (m_stCrusadeStructureInfo[i].cSide == 1)
                                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 38, false, true);
                                else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 36, false, true);
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
                        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, false, true);
                    }
                    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, false, true);
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

                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 41, false, true);
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
                put_string_sprite_font3(msX + 10, msY - 10, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
            }
            break;
    }
}

void CGame::DrawDialogBox_Constructor(int msX, int msY)
{
    short sX, sY, szX, szY, MapSzX, MapSzY;
    int64_t dwTime = G_dwGlobalTime;
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

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 0, false, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 16, false, m_bDialogTrans);

    switch (m_stDialogBoxInfo[37].cMode)
    {
        case 0:
            if (m_iConstructLocX != -1)
            {
                memset(cMapName, 0, sizeof(cMapName));
                GetOfficialMapName(m_cConstructMapName, cMapName);
                format_to_local(G_cTxt, DRAW_DIALOGBOX_CONSTRUCTOR1, cMapName, m_iConstructLocX, m_iConstructLocY);
                put_aligned_string(sX, sX + szX, sY + 40, G_cTxt);
            }
            else put_aligned_string(sX, sX + szX, sY + 40, DRAW_DIALOGBOX_CONSTRUCTOR2);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);
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

        case 1: // °Ç¼³ ¸Þ´º 
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

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);

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
            szX = 0;
            szY = 0;
            MapSzX = 0;
            MapSzY = 0;

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

                if (szX != 0)
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iTeleportLocX;
                    dV3 = (dV2 * (double)szX) / dV1;
                    tX = (int)dV3;

                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iTeleportLocY;
                    dV3 = (dV2 * (double)szY) / dV1;
                    tY = (int)dV3;

                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, false, true);
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

                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 41, false, true);
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
                put_string_sprite_font3(msX + 10, msY - 10, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
            }
            break;
    }
}

void CGame::DrawDialogBox_Soldier(int msX, int msY)
{
    short sX, sY, szX, szY, MapSzX, MapSzY;
    int64_t dwTime = G_dwGlobalTime;
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

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 0, false, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX, sY, 21, false, m_bDialogTrans);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 17, false, m_bDialogTrans); // Crusade Soldier Menu Text

    switch (m_stDialogBoxInfo[38].cMode)
    {
        case 0:

            if (m_iTeleportLocX != -1)
            {
                memset(cMapName, 0, sizeof(cMapName));
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
            szX = 0;
            szY = 0;
            MapSzX = 0;
            MapSzY = 0;
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

                if (szX != 0)
                {
                    dV1 = (double)MapSzX;
                    dV2 = (double)m_iTeleportLocX;
                    dV3 = (dV2 * (double)szX) / dV1;
                    tX = (int)dV3;

                    dV1 = (double)MapSzY;
                    dV2 = (double)m_iTeleportLocY;
                    dV3 = (dV2 * (double)szY) / dV1;
                    tY = (int)dV3;

                    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_CRUSADE, sX + tX + 15, sY + tY + 60, 42, false, true);
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
                put_string_sprite_font3(msX + 10, msY - 10, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
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

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
}

void CGame::DrawDialogBox_ItemUpgrade(int msX, int msY)
{
    int i, sX, sY, iValue;
    char cItemColor, cStr1[120], cStr2[120], cStr3[120];
    int64_t dwTime = m_dwCurTime;

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
                memset(cStr1, 0, sizeof(cStr1));
                memset(cStr2, 0, sizeof(cStr2));
                memset(cStr3, 0, sizeof(cStr3));
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
                memset(cStr1, 0, sizeof(cStr1));
                memset(cStr2, 0, sizeof(cStr2));
                memset(cStr3, 0, sizeof(cStr3));
                GetItemName(m_pItemList[m_stDialogBoxInfo[34].sV1], cStr1, cStr2, cStr3, 120);
                put_aligned_string(sX + 24, sX + 248, sY + 230 + 20, cStr1);
                put_aligned_string(sX + 24, sX + 248, sY + 245 + 20, cStr2);
                put_aligned_string(sX + 24, sX + 248, sY + 260 + 20, cStr3);
            }

            if (((dwTime - m_stDialogBoxInfo[34].dwV1) / 1000 > 4) && (m_stDialogBoxInfo[34].dwV1 != 0))
            {
                m_stDialogBoxInfo[34].dwV1 = 0;
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_UPGRADEITEM, 0, m_stDialogBoxInfo[34].sV1, 0, 0, 0);
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

                memset(cStr1, 0, sizeof(cStr1));
                memset(cStr2, 0, sizeof(cStr2));
                memset(cStr3, 0, sizeof(cStr3));
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
            if ((m_stDialogBoxInfo[34].sV1 != -1) && (m_pItemList[m_stDialogBoxInfo[34].sV1] == 0))
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

                memset(cStr1, 0, sizeof(cStr1));
                memset(cStr2, 0, sizeof(cStr2));
                memset(cStr3, 0, sizeof(cStr3));
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

                memset(cStr1, 0, sizeof(cStr1));
                memset(cStr2, 0, sizeof(cStr2));
                memset(cStr3, 0, sizeof(cStr3));
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
void CGame::DrawDialogBox_Resurrect(short msX, short msY)
{
    short sX, sY;
    sX = m_stDialogBoxInfo[50].sX;
    sY = m_stDialogBoxInfo[50].sY;
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME1, sX, sY, 2);

    put_string(sX + 50, sY + 20, "Someone requested to resurrect you.", Color(4, 0, 50));
    put_string(sX + 80, sY + 35, "Will you accept?", Color(4, 0, 50));

    if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 19);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 30, sY + 55, 18);

    if ((msX >= sX + 170) && (msX <= sX + 170 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 3);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 170, sY + 55, 2);
}
void CGame::DrawDialogBox_CMDHallMenu(short msX, short msY)
{
    short sX, sY, szX;
    char cTxt[120];
    sX = m_stDialogBoxInfo[51].sX;
    sY = m_stDialogBoxInfo[51].sY;
    szX = m_stDialogBoxInfo[51].sSizeX;
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 2);

    switch (m_stDialogBoxInfo[51].cMode)
    {
        case 0: // initial diag
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
                put_aligned_string(sX, sX + szX, sY + 70, "Teleport to Battle Field", 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 70, "Teleport to Battle Field", 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
                put_aligned_string(sX, sX + szX, sY + 95, "Hire a soldier", 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 95, "Hire a soldier", 4, 0, 50);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
                put_aligned_string(sX, sX + szX, sY + 120, "Taking Flags", 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 120, "Taking Flags", 4, 0, 50);
            // Angels
            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 145) && (msY < sY + 170))
                put_aligned_string(sX, sX + szX, sY + 145, "Receive a Tutelary Angel", 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 145, "Receive a Tutelary Angel", 4, 0, 50);
            break;

        case 1:
            if (m_iTeleportMapCount > 0)
            {
                //put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_CITYHALL_MENU69, 55,25,25); // "Teleporting to dungeon level 2."
                //put_aligned_string(sX, sX + szX, sY + 80, DRAW_DIALOGBOX_CITYHALL_MENU70, 55,25,25); // "5000Gold is required"
                //put_aligned_string(sX, sX + szX, sY + 95, DRAW_DIALOGBOX_CITYHALL_MENU71, 55,25,25); //
                //put_aligned_string(sX, sX + szX, sY + 110, DRAW_DIALOGBOX_CITYHALL_MENU72, 55,25,25);//
                put_string2(sX + 35, sY + 250, DRAW_DIALOGBOX_CITYHALL_MENU72_1, 55, 25, 25);//"Civilians cannot go some area."
                for (int i = 0; i < m_iTeleportMapCount; i++)
                {
                    memset(cTxt, 0, sizeof(cTxt));
                    GetOfficialMapName(m_stTeleportList[i].mapname, cTxt);
                    format_to_local(G_cTxt, DRAW_DIALOGBOX_CITYHALL_MENU77, cTxt, m_stTeleportList[i].iCost);
                    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 130 + i * 15) && (msY <= sY + 144 + i * 15))
                        put_aligned_string(sX, sX + szX, sY + 130 + i * 15, G_cTxt, 255, 255, 255);
                    else put_aligned_string(sX, sX + szX, sY + 130 + i * 15, G_cTxt, 250, 250, 0);
                }
            }
            else if (m_iTeleportMapCount == -1)
            {
                put_aligned_string(sX, sX + szX, sY + 125, DRAW_DIALOGBOX_CITYHALL_MENU73, 55, 25, 25);//"Now it's searching for possible area"
                put_aligned_string(sX, sX + szX, sY + 150, DRAW_DIALOGBOX_CITYHALL_MENU74, 55, 25, 25);//"to teleport."
                put_aligned_string(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_CITYHALL_MENU75, 55, 25, 25);//"Please wait for a moment."
            }
            else
            {
                put_aligned_string(sX, sX + szX, sY + 175, DRAW_DIALOGBOX_CITYHALL_MENU76, 55, 25, 25);//"There is no area that you can teleport."
            }
            break;

        case 2:
            put_aligned_string(sX, sX + szX, sY + 45, "You will hire a soldier by summon points", 255, 255, 255);
            if ((m_iConstructionPoint >= 2000) && (m_bIsCrusadeMode == false))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
                    put_aligned_string(sX, sX + szX, sY + 70, "Sorceress             2000 Point", 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 70, "Sorceress             2000 Point", 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 70, "Sorceress             2000 Point", 65, 65, 65);

            if ((m_iConstructionPoint >= 3000) && (m_bIsCrusadeMode == false))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
                    put_aligned_string(sX, sX + szX, sY + 95, "Ancient Temple Knight 3000 Point", 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 95, "Ancient Temple Knight 3000 Point", 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 95, "Ancient Temple Knight 3000 Point", 65, 65, 65);

            if ((m_iConstructionPoint >= 1500) && (m_bIsCrusadeMode == false))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
                    put_aligned_string(sX, sX + szX, sY + 120, "Elf Master            1500 Point", 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 120, "Elf Master            1500 Point", 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 120, "Elf Master            1500 Point", 65, 65, 65);

            if ((m_iConstructionPoint >= 3000) && (m_bIsCrusadeMode == false))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 145) && (msY < sY + 171))
                    put_aligned_string(sX, sX + szX, sY + 145, "Dark Shadow Knight    3000 Point", 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 145, "Dark Shadow Knight    3000 Point", 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 145, "Dark Shadow Knight    3000 Point", 65, 65, 65);

            if ((m_iConstructionPoint >= 4000) && (m_bIsCrusadeMode == false))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 170) && (msY < sY + 195))
                    put_aligned_string(sX, sX + szX, sY + 170, "Heavy Battle Tank     4000 Point", 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 170, "Heavy Battle Tank     4000 Point", 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 170, "Heavy Battle Tank     4000 Point", 65, 65, 65);

            if ((m_iConstructionPoint >= 3000) && (m_bIsCrusadeMode == false))
            {
                if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 195) && (msY < sY + 220))
                    put_aligned_string(sX, sX + szX, sY + 195, "Barbarian             3000 Point", 255, 255, 255);
                else put_aligned_string(sX, sX + szX, sY + 195, "Barbarian             3000 Point", 4, 0, 50);
            }
            else put_aligned_string(sX, sX + szX, sY + 195, "Barbarian             3000 Point", 65, 65, 65);

            put_aligned_string(sX, sX + szX, sY + 220, "You should join a guild to hire soldiers.", 4, 0, 50);
            format_to_local(G_cTxt, "Summon points : {}", m_iConstructionPoint);
            put_aligned_string(sX, sX + szX, sY + 250, G_cTxt, 4, 0, 50);
            put_aligned_string(sX, sX + szX, sY + 280, "Maximum summon points : 12000 points.", 4, 0, 50);
            put_aligned_string(sX, sX + szX, sY + 300, "Maximum hiring number : 5 ", 4, 0, 50);
            break;

        case 3: // Hire a Flag Diag
            put_aligned_string(sX, sX + szX, sY + 45, "You may acquire Flags with EK points.", 4, 0, 50);
            put_aligned_string(sX, sX + szX, sY + 70, "Price is 10 EK per Flag.", 4, 0, 50);
            if ((msX >= sX + 35) && (msX <= sX + 220) && (msY >= sY + 140) && (msY <= sY + 165))
                put_aligned_string(sX, sX + szX, sY + 140, "Take a Flag", 255, 255, 255);
            else
                put_aligned_string(sX, sX + szX, sY + 140, "Take a Flag", 250, 250, 0);
            break;

        case 4: // Tutelar Angel Diag
            put_aligned_string(sX, sX + szX, sY + 45, "5 magesty points will be deducted", 4, 0, 50);
            put_aligned_string(sX, sX + szX, sY + 80, "upon receiving the Pendant of Tutelary Angel.", 4, 0, 50);
            put_aligned_string(sX, sX + szX, sY + 105, "Would you like to receive the Tutelary Angel?", 4, 0, 50);
            format_to_local(G_cTxt, DRAW_DIALOGBOX_ITEMUPGRADE11, m_iGizonItemUpgradeLeft);// "Item upgrade point : {}"
            put_aligned_string(sX, sX + szX, sY + 140, G_cTxt, 0, 0, 0);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 175) && (msY < sY + 200)
                && (m_iGizonItemUpgradeLeft > 4))
                put_aligned_string(sX, sX + szX, sY + 175, "Tutelary Angel (STR) will be handed out.", 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 175, "Tutelary Angel (STR) will be handed out.", 250, 250, 0);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 200) && (msY < sY + 225)
                && (m_iGizonItemUpgradeLeft > 4))
                put_aligned_string(sX, sX + szX, sY + 200, "Tutelary Angel (DEX) will be handed out.", 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 200, "Tutelary Angel (DEX) will be handed out.", 250, 250, 0);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 225) && (msY < sY + 250)
                && (m_iGizonItemUpgradeLeft > 4))
                put_aligned_string(sX, sX + szX, sY + 225, "Tutelary Angel (INT) will be handed out.", 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 225, "Tutelary Angel (INT) will be handed out.", 250, 250, 0);

            if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 250) && (msY < sY + 275)
                && (m_iGizonItemUpgradeLeft > 4))
                put_aligned_string(sX, sX + szX, sY + 250, "Tutelary Angel (MAG) will be handed out.", 255, 255, 255);
            else put_aligned_string(sX, sX + szX, sY + 250, "Tutelary Angel (MAG) will be handed out.", 250, 250, 0);

            break;
    }
}
void CGame::DrawDialogBox_ChangeStatsMajestic(short msX, short msY)
{
    short sX{}, sY{}, szX{};
    int64_t dwTime = m_dwCurTime;
    char cTxt[120]{};
    int iStats{};
    sX = m_stDialogBoxInfo[42].sX;
    sY = m_stDialogBoxInfo[42].sY;
    szX = m_stDialogBoxInfo[42].sSizeX;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_TEXT, sX, sY, 2);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX + 16, sY + 100, 4);

    put_aligned_string(sX, sX + szX, sY + 50, DRAW_DIALOGBOX_LEVELUP_SETTING14);
    put_aligned_string(sX, sX + szX, sY + 65, DRAW_DIALOGBOX_LEVELUP_SETTING15);

    put_string(sX + 20, sY + 85, DRAW_DIALOGBOX_LEVELUP_SETTING16, Color(0, 0, 0));
    format_to_local(cTxt, "{}", m_iGizonItemUpgradeLeft);
    if (m_iGizonItemUpgradeLeft > 0)
    {
        put_string(sX + 78, sY + 102, cTxt, Color(0, 0, 255));
    }
    else
    {
        put_string(sX + 78, sY + 102, cTxt, Color(0, 0, 0));
    }
    put_string(sX + 24, sY + 125, DRAW_DIALOGBOX_LEVELUP_SETTING4, Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_iStr);
    put_string(sX + 109, sY + 125, cTxt, Color(25, 35, 25));
    iStats = m_iStr + m_cLU_Str;
    format_to_local(cTxt, "{}", iStats);
    if (iStats < m_iStr)
        put_string(sX + 162, sY + 125, cTxt, Color(255, 0, 0));
    else
        put_string(sX + 162, sY + 125, cTxt, Color(25, 35, 25));

    put_string(sX + 24, sY + 144, DRAW_DIALOGBOX_LEVELUP_SETTING5, Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_iVit);
    put_string(sX + 109, sY + 144, cTxt, Color(25, 35, 25));
    iStats = m_iVit + m_cLU_Vit;
    format_to_local(cTxt, "{}", iStats);
    if (iStats < m_iVit)
        put_string(sX + 162, sY + 144, cTxt, Color(255, 0, 0));
    else
        put_string(sX + 162, sY + 144, cTxt, Color(25, 35, 25));

    put_string(sX + 24, sY + 163, DRAW_DIALOGBOX_LEVELUP_SETTING6, Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_iDex);
    put_string(sX + 109, sY + 163, cTxt, Color(25, 35, 25));
    iStats = m_iDex + m_cLU_Dex;
    format_to_local(cTxt, "{}", iStats);
    if (iStats < m_iDex)
        put_string(sX + 162, sY + 163, cTxt, Color(255, 0, 0));
    else
        put_string(sX + 162, sY + 163, cTxt, Color(25, 35, 25));

    put_string(sX + 24, sY + 182, DRAW_DIALOGBOX_LEVELUP_SETTING7, Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_iInt);
    put_string(sX + 109, sY + 182, cTxt, Color(25, 35, 25));
    iStats = m_iInt + m_cLU_Int;
    format_to_local(cTxt, "{}", iStats);
    if (iStats < m_iInt)
        put_string(sX + 162, sY + 182, cTxt, Color(255, 0, 0));
    else
        put_string(sX + 162, sY + 182, cTxt, Color(25, 35, 25));

    put_string(sX + 24, sY + 201, DRAW_DIALOGBOX_LEVELUP_SETTING8, Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_iMag);
    put_string(sX + 109, sY + 201, cTxt, Color(25, 35, 25));
    iStats = m_iMag + m_cLU_Mag;
    format_to_local(cTxt, "{}", iStats);
    if (iStats < m_iMag)
        put_string(sX + 162, sY + 201, cTxt, Color(255, 0, 0));
    else
        put_string(sX + 162, sY + 201, cTxt, Color(25, 35, 25));

    put_string(sX + 24, sY + 220, DRAW_DIALOGBOX_LEVELUP_SETTING9, Color(5, 5, 5));
    format_to_local(cTxt, "{}", m_iCharisma);
    put_string(sX + 109, sY + 220, cTxt, Color(25, 35, 25));
    iStats = m_iCharisma + m_cLU_Char;
    format_to_local(cTxt, "{}", iStats);
    if (iStats < m_iCharisma)
        put_string(sX + 162, sY + 220, cTxt, Color(255, 0, 0));
    else
        put_string(sX + 162, sY + 220, cTxt, Color(25, 35, 25));

    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 17);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_LBTNPOSX, sY + DEF_BTNPOSY, 16);

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + DEF_RBTNPOSX, sY + DEF_BTNPOSY, 0);
}

void CGame::DrawDialogBox_Slates(short msX, short msY, short msZ, char cLB)
{
    int iAdjX, iAdjY;
    short sX, sY;
    int64_t dwTime = m_dwCurTime;

    iAdjX = 5;
    iAdjY = 8;

    switch (m_stDialogBoxInfo[40].cMode)
    {
        case 1:
            sX = m_stDialogBoxInfo[40].sX;
            sY = m_stDialogBoxInfo[40].sY;
            iAdjX = -1;
            iAdjY = -7;

            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_INVENTORY, sX, sY, 4);

            if (m_stDialogBoxInfo[40].sV1 != -1)
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_INVENTORY, sX + 20, sY + 12, 5);
            }
            if (m_stDialogBoxInfo[40].sV2 != -1)
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_INVENTORY, sX + 20, sY + 87, 6);
            }
            if (m_stDialogBoxInfo[40].sV3 != -1)
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_INVENTORY, sX + 85, sY + 32, 7);
            }
            if (m_stDialogBoxInfo[40].sV4 != -1)
            {
                DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_INVENTORY, sX + 70, sY + 100, 8);
            }

            if ((m_stDialogBoxInfo[40].sV1 != -1) && (m_stDialogBoxInfo[40].sV2 != -1) && (m_stDialogBoxInfo[40].sV3 != -1) && (m_stDialogBoxInfo[40].sV4 != -1))
            {
                if ((msX >= sX + 120) && (msX <= sX + 180) && (msY >= sY + 150) && (msY <= sY + 165))
                    put_string_sprite_font(sX + 120, sY + 150, "Casting", 6, 6, 20);
                else put_string_sprite_font(sX + 120, sY + 150, "Casting", 0, 0, 7);
            }

            break;

        case 2:
            PlaySound('E', 16, 0);
            if (m_stDialogBoxInfo[40].cStr[0] != 0)
            {
                sX = m_stDialogBoxInfo[40].sX + iAdjX + (m_stDialogBoxInfo[40].cStr[0] - (rand() % (m_stDialogBoxInfo[40].cStr[0] * 2)));
                sY = m_stDialogBoxInfo[40].sY + iAdjY + (m_stDialogBoxInfo[40].cStr[0] - (rand() % (m_stDialogBoxInfo[40].cStr[0] * 2)));
            }
            else
            {
                sX = m_stDialogBoxInfo[40].sX;
                sY = m_stDialogBoxInfo[40].sY;
            }
            m_pSprite[DEF_SPRID_INTERFACE_ND_INVENTORY]->put_sprite_fast(sX, sY, 4, dwTime);
            m_pSprite[DEF_SPRID_INTERFACE_ND_INVENTORY]->put_sprite_fast(sX + 22, sY + 14, 3, dwTime);
            put_aligned_string(199, 438, 201, "KURURURURURURU!!!", 90, 220, 200);
            put_aligned_string(200, 439, 200, "KURURURURURURU!!!", 220, 140, 160);

            if ((dwTime - m_stDialogBoxInfo[40].dwT1) > 1000)
            {
                m_stDialogBoxInfo[40].dwT1 = dwTime;
                m_stDialogBoxInfo[40].cStr[0]++;
            }
            if (m_stDialogBoxInfo[40].cStr[0] >= 5)
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_CREATESLATE, 0, m_stDialogBoxInfo[40].sV1, m_stDialogBoxInfo[40].sV2, m_stDialogBoxInfo[40].sV3, 0, m_stDialogBoxInfo[40].sV4);
                DisableDialogBox(40);
            }
            break;
        default:break;
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

            if (m_bCitizen == true)
            {
                if (m_bAresden)
                {

                    if ((m_iGuildRank == 0) || (m_iGuildRank == 1))
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
                else if (m_bAresden == false)
                {
                    if ((m_iGuildRank == 0) || (m_iGuildRank == 1))
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
void CGame::DrawDialogBox_Map()
{
    short sX{}, sY{};
    int64_t dwTime = m_dwCurTime;
    double dV1{}, dV2{}, dV3{};
    int tX{}, tY{}, szX{}, szY{}, dX{}, dY{};

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

            //m_pSprite[DEF_SPRID_INTERFACE_MAPS1]->put_sprite_fast(sX +tX, sY +tY, 4, dwTime);
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, sX + tX, sY + tY, 43);
            format_to_local(G_cTxt, "{},{}", m_sPlayerX, m_sPlayerY);
            put_string_sprite_font3(sX + 10 + tX - 5, sY + 10 + tY - 6, G_cTxt, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
            break;
    }
}

void CGame::DrawChatMsgBox(short sX, short sY, int iChatIndex, bool bIsPreDC)
{
    char cMsgA[22]{}, cMsgB[22]{}, cMsgC[22]{};
    int iLines{}, i{}, iSize{}, iSize2{}, iLoc{}, iFontSize{};
    int64_t dwTime{};
    Color rgb{};
    bool bIsTrans{};
    std::string & str = m_pChatMsgList[iChatIndex]->message;

    memset(cMsgA, 0, sizeof(cMsgA));
    memset(cMsgB, 0, sizeof(cMsgB));
    memset(cMsgC, 0, sizeof(cMsgC));

    dwTime = m_pChatMsgList[iChatIndex]->m_dwTime;
    iLines = 0;

    rgb = Color(255, 255, 255);
    switch (m_pChatMsgList[iChatIndex]->m_cType)
    {
        case 1:
            rgb = Color(255, 255, 255);
            break;
        case 20:
            rgb = Color(255, 255, 20);
            if ((m_dwCurTime - dwTime) < 650) return;
            dwTime += 650;
            break;
        case 41:
            rgb = Color(255, 80, 80);
            break;

        case 42:
            rgb = Color(255, 80, 80);
            if ((m_dwCurTime - dwTime) < 650) return;
            dwTime += 650;
            break;
    }

    if (str.length() < 21)
    {
        memcpy(cMsgA, str.c_str(), 20);
        iLines = 1;
    }
    else if (str.length() < 41)
    {
        memcpy(cMsgA, str.c_str(), 20);
        memcpy(cMsgB, str.c_str() + 20, 20);
        iLines = 2;
    }
    else
    {
        memcpy(cMsgA, str.c_str(), 20);
        memcpy(cMsgB, str.c_str() + 20, 20);
        memcpy(cMsgC, str.c_str() + 40, 20);
        iLines = 3;
    }

    iSize = 0;
    for (i = 0; i < 20; i++)
    {
        if (cMsgA[i] != 0)
        {
            if ((unsigned char)cMsgA[i] >= 128)
            {
                iSize += 5;
                i++;
            }
            else iSize += 4;
        }
    }

    iLoc = m_dwCurTime - dwTime;
    switch (m_pChatMsgList[iChatIndex]->m_cType)
    {
        case 21:
        case 22:
        case 23:
            if (iLoc > 80) iLoc = 10;
            else iLoc = iLoc >> 3;
            break;
        default:
            if (iLoc > 352) iLoc = 9;
            else if (iLoc > 320) iLoc = 10;
            else iLoc = iLoc >> 5;
            break;
    }

    if (m_cDetailLevel == 0)
        bIsTrans = false;
    else bIsTrans = true;

    switch (m_pChatMsgList[iChatIndex]->m_cType)
    {
        case 41:
        case 42:
            iSize2 = 0;
            for (i = 0; i < str.length(); i++)
                if (str[i] != 0)
                    if ((unsigned char)str[i] >= 128)
                    {
                        iSize2 += 5;
                        i++;
                    }
                    else iSize2 += 4;
            if (m_Misc.bCheckIMEString(str.c_str()) == false)
            {
                put_string3(sX - iSize2, sY - 65 - iLoc, str, Color(180, 30, 30));
            }
            else
            {
                put_overhead_string(sX - iSize2, sY - 65 - iLoc, str, overhead_magic_color, 4, false, 16);
            }
            break;

        case 21:
        case 22:
        case 23:
            switch (m_pChatMsgList[iChatIndex]->m_cType)
            {
                case 21:
                    iFontSize = 12; break;
                case 22:
                    iFontSize = 16; break;
                case 23:
                    iFontSize = 20; break;
            }
            switch (iLines)
            {
                case 1:
                    put_overhead_string(sX - iSize, sY - 65 - iLoc, cMsgA, overhead_damage_color, 2, bIsTrans, iFontSize);
                    break;
                case 2:
                    put_overhead_string(sX - iSize, sY - 81 - iLoc, cMsgA, overhead_damage_color, 2, bIsTrans, iFontSize);
                    put_overhead_string(sX - iSize, sY - 65 - iLoc, cMsgB, overhead_damage_color, 2, bIsTrans, iFontSize);
                    break;
                case 3:
                    put_overhead_string(sX - iSize, sY - 97 - iLoc, cMsgA, overhead_damage_color, 2, bIsTrans, iFontSize);
                    put_overhead_string(sX - iSize, sY - 81 - iLoc, cMsgB, overhead_damage_color, 2, bIsTrans, iFontSize);
                    put_overhead_string(sX - iSize, sY - 65 - iLoc, cMsgC, overhead_damage_color, 2, bIsTrans, iFontSize);
                    break;
            }
            break;

        case 20:
        default:
            int len = (int)str.length();

            sf::Text chat_text;

            chat_text.setPosition(0.f, 0.f);
            chat_text.setFont(*arya_font);
            chat_text.setCharacterSize(12);
            chat_text.setString(str);

            sf::FloatRect fr = chat_text.getGlobalBounds();

            switch (int(fr.width / 160))
            {
                case 0:
                default:
                    put_overhead_string(sX - 80 + 1, sY - 65 - iLoc, str, Color(0, 0, 0), 2, bIsTrans, 12);
                    put_overhead_string(sX - 80, sY - 65 - iLoc, str, Color(0, 0, 0), 2, bIsTrans, 12);
                    put_overhead_string(sX - 80, sY - 65 - iLoc, str, rgb, 2, bIsTrans, 12);
                    break;

                case 1:
                    put_overhead_string(sX - 80 + 1, sY - 83 - iLoc, str, Color(0, 0, 0), 2, bIsTrans, 12);
                    put_overhead_string(sX - 80, sY - 83 - iLoc + 1, str, Color(0, 0, 0), 2, bIsTrans, 12);
                    put_overhead_string(sX - 80, sY - 83 - iLoc, str, rgb, 2, bIsTrans, 12);
                    break;

                case 2:
                    put_overhead_string(sX - 80 + 1, sY - 101 - iLoc, str, Color(0, 0, 0), 2, bIsTrans, 12);
                    put_overhead_string(sX - 80, sY - 101 - iLoc + 1, str, Color(0, 0, 0), 2, bIsTrans, 12);
                    put_overhead_string(sX - 80, sY - 101 - iLoc, str, rgb, 2, bIsTrans, 12);
                    break;

                case 3:
                    put_overhead_string(sX - 80 + 1, sY - 119 - iLoc, str, Color(0, 0, 0), 2, bIsTrans, 12);
                    put_overhead_string(sX - 80, sY - 119 - iLoc + 1, str, Color(0, 0, 0), 2, bIsTrans, 12);
                    put_overhead_string(sX - 80, sY - 119 - iLoc, str, rgb, 2, bIsTrans, 12);
                    break;
            }
            break;
    }
}
