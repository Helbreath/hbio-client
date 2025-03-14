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
#include "char_info.h"
#include "mouse_interface.h"
#include "action_id.h"
#include "Item.h"
#include "map_data.h"
#include "lan_eng.h"
#include "msg.h"

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
extern uint16_t  wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int iFocusStatus;
extern int iFocusApprColor;

extern int64_t focus_owner_time;
extern int64_t focus_frame_time;

void CGame::_Draw_CharacterBody(short sX, short sY, short sType)
{
    int64_t dwTime = m_dwCurTime;
    int  iR, iG, iB;

    if (sType <= 3)
    {
        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 0]->put_sprite_fast(sX, sY, sType - 1, dwTime);
        _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 18]->put_sprite_color(sX, sY, (_tmp_sAppr1 & 0x0F00) >> 8, iR, iG, iB, dwTime);

        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 19]->put_sprite_fast(sX, sY, (_tmp_sAppr1 & 0x000F), dwTime);
    }
    else
    {
        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 40]->put_sprite_fast(sX, sY, sType - 4, dwTime);

        _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 18 + 40]->put_sprite_color(sX, sY, (_tmp_sAppr1 & 0x0F00) >> 8, iR, iG, iB, dwTime);

        m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 19 + 40]->put_sprite_fast(sX, sY, (_tmp_sAppr1 & 0x000F), dwTime);
    }
}

bool CGame::_bDraw_OnCreateNewCharacter(char * pName, short msX, short msY, int iPoint)
{
    bool bFlag = true;
    int64_t dwTime = m_dwCurTime;
    int i = 0;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_NEWCHAR, 0, 0, 0, true);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, 0, 0, 69, true);
    put_aligned_string(64, 282, 90, _BDRAW_ON_CREATE_NEW_CHARACTER1, 5, 5, 5);
    put_aligned_string(57, 191, 110, DEF_MSG_CHARACTERNAME, 5, 5, 5);//"Character Name"
    if (m_cCurFocus != 1) put_string(197, 112, pName, Color(25, 35, 25));
    put_aligned_string(64, 282, 140, _BDRAW_ON_CREATE_NEW_CHARACTER2, 5, 5, 5);
    put_string(100, 160, DEF_MSG_GENDER, Color(5, 5, 5));//"Gender"
    put_string(100, 175, DEF_MSG_SKINCOLOR, Color(5, 5, 5));//"Skin Color"
    put_string(100, 190, DEF_MSG_HAIRSTYLE, Color(5, 5, 5));//"Hair Style"
    put_string(100, 205, DEF_MSG_HAIRCOLOR, Color(5, 5, 5));//"Hair Color"
    put_string(100, 220, DEF_MSG_UNDERWEARCOLOR, Color(5, 5, 5));//"Underwear Color"
    put_aligned_string(64, 282, 245, _BDRAW_ON_CREATE_NEW_CHARACTER3, 5, 5, 5);
    format_to_local(G_cTxt, _BDRAW_ON_CREATE_NEW_CHARACTER4, iPoint);
    put_aligned_string(64, 282, 260, G_cTxt, 15, 10, 10);
    put_string(100, 275, DEF_MSG_STRENGTH, Color(5, 5, 5));//"Strength"
    put_string(100, 292, DEF_MSG_VITALITY, Color(5, 5, 5));//"Vitality"
    put_string(100, 309, DEF_MSG_DEXTERITY, Color(5, 5, 5));//"Dexterity"
    put_string(100, 326, DEF_MSG_INTELLIGENCE, Color(5, 5, 5));//"Intelligence"
    put_string(100, 343, DEF_MSG_MAGIC, Color(5, 5, 5));//"Magic"
    put_string(100, 360, DEF_MSG_CHARISMA, Color(5, 5, 5));//"Charisma"

    format_to_local(G_cTxt, "{}", m_ccStr);
    put_string(204, 277 + 16 * i++, G_cTxt, Color(25, 35, 25));
    format_to_local(G_cTxt, "{}", m_ccVit);
    put_string(204, 277 + 16 * i++, G_cTxt, Color(25, 35, 25));
    format_to_local(G_cTxt, "{}", m_ccDex);
    put_string(204, 277 + 16 * i++, G_cTxt, Color(25, 35, 25));
    format_to_local(G_cTxt, "{}", m_ccInt);
    put_string(204, 277 + 16 * i++, G_cTxt, Color(25, 35, 25));
    format_to_local(G_cTxt, "{}", m_ccMag);
    put_string(204, 277 + 16 * i++, G_cTxt, Color(25, 35, 25));
    format_to_local(G_cTxt, "{}", m_ccChr);
    put_string(204, 277 + 16 * i++, G_cTxt, Color(25, 35, 25));
    put_aligned_string(64, 295, 380, _BDRAW_ON_CREATE_NEW_CHARACTER5, 5, 5, 5);

    if (strlen(pName) <= 0) bFlag = false;
    if (iPoint > 0) bFlag = false;
    if (m_Misc.bCheckValidName(pName) == false) bFlag = false;
    //if (_bCheckBadWords(pName) == true) bFlag = false;

    if ((bFlag == true) && (m_cCurFocus == 2)) m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(384, 445, 25, dwTime);
    else m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(384, 445, 24, dwTime);
    if (m_cCurFocus == 3)
        m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(500, 445, 17, dwTime);
    else m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(500, 445, 16, dwTime);
    if (m_cCurFocus == 4)
        m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(60, 445, 68, dwTime);
    else m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(60, 445, 67, dwTime);
    if (m_cCurFocus == 5)
        m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(145, 445, 66, dwTime);
    else m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(145, 445, 65, dwTime);
    if (m_cCurFocus == 6)
        m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(230, 445, 64, dwTime);
    else m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(230, 445, 63, dwTime);

    ShowReceivedString();

    switch (m_cGender)
    {
        case 1:	_tmp_sOwnerType = 1; break;
        case 2:	_tmp_sOwnerType = 4; break;
    }
    _tmp_sOwnerType += m_cSkinCol - 1;
    _tmp_cDir = m_cMenuDir;
    _tmp_sAppr1 = 0;
    _tmp_sAppr1 = _tmp_sAppr1 | (m_cUnderCol);
    _tmp_sAppr1 = _tmp_sAppr1 | (m_cHairStyle << 8);
    _tmp_sAppr1 = _tmp_sAppr1 | (m_cHairCol << 4);
    _tmp_sAppr2 = 0;
    _tmp_sAppr3 = 0;
    _tmp_sAppr4 = 0;
    memset(_tmp_cName, 0, sizeof(_tmp_cName));
    memcpy(_tmp_cName, m_cPlayerName, 10);
    _tmp_cAction = DEF_OBJECTMOVE;
    _tmp_cFrame = m_cMenuFrame;

    _Draw_CharacterBody(507, 267, _tmp_sOwnerType);

    DrawObject_OnMove_ForMenu(0, 0, 500, 174, false, dwTime, msX, msY);

    i = 0;

    put_string(445, 192, DEF_MSG_HITPOINT, Color(5, 5, 5));
    format_to_local(G_cTxt, "{}", m_ccVit * 3 + 2 + m_ccStr / 2);
    put_string(550, 192 + 16 * i++, G_cTxt, Color(25, 35, 25));

    put_string(445, 208, DEF_MSG_MANAPOINT, Color(5, 5, 5));
    format_to_local(G_cTxt, "{}", m_ccMag * 2 + 2 + m_ccInt / 2);
    put_string(550, 192 + 16 * i++, G_cTxt, Color(25, 35, 25));

    put_string(445, 224, DEF_MSG_STAMINAPOINT, Color(5, 5, 5));
    format_to_local(G_cTxt, "{}", m_ccStr * 2 + 2);
    put_string(550, 192 + 16 * i++, G_cTxt, Color(25, 35, 25));

    return bFlag;
}


bool CGame::DrawObject_OnAttack(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iWeapon{}, iAdd{}, iShieldIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern
    //if(_tmp_sOwnerType == 81) bInv = true; //Change Abaddon invis

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
                if (iWeapon == 0) iAdd = 6;
                if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
                if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 4;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 5;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 5;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 5;

                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
                if (iWeapon == 0) iAdd = 6;
                if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
                if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 4;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 5;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 5;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 5;

                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;


        default:
            if (_tmp_sAppr2 != 0)
            {
                iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                _tmp_cFrame = _tmp_sAppr2 - 1;
            }
            else if (_tmp_sOwnerType == 66) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else if (_tmp_sOwnerType == 73) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
            //if (_tmp_sOwnerType >= 70) { iBodyIndex = DEF_NPC + (2*8); }//ChangeSprite
            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }

    if (m_bIsCrusadeMode) DrawObjectFOE(sX, sY, _tmp_cFrame);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {

        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                {
                    m_pSprite[iWeaponIndex]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
                }
                else
                {
                    m_pSprite[iWeaponIndex]->put_sprite_color(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }


                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }

            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // FireWyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                    }
                    break;
            }
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (_tmp_sOwnerType == 81) //Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            }
            else if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }
        else
        {
            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // Fire Wyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                    }
                    break;
            }
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (_tmp_sOwnerType == 81) //Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            }
            else if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->put_sprite_color(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);//Change DK weapon glare

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }


                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }
        }

        // Berserk 
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Poison
//		if ((_tmp_iStatus & 0x80) != 0) 
//			m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }

    //Abaddon Effects
    if (_tmp_sOwnerType == 81)
    {
        int randFrame = _tmp_cFrame % 12;
        m_pEffectSpr[154]->put_trans_sprite70(sX - 50, sY - 50, randFrame, dwTime);
        m_pEffectSpr[155]->put_trans_sprite70(sX - 20, sY - 80, randFrame, dwTime);
        m_pEffectSpr[156]->put_trans_sprite70(sX + 70, sY - 50, randFrame, dwTime);
        m_pEffectSpr[157]->put_trans_sprite70(sX - 30, sY, randFrame, dwTime);
        m_pEffectSpr[158]->put_trans_sprite70(sX - 60, sY + 90, randFrame, dwTime);
        m_pEffectSpr[159]->put_trans_sprite70(sX + 65, sY + 85, randFrame, dwTime);
        switch (_tmp_cDir)
        {
            case 1:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 108, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 50, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 2:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 70, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 3:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 105, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 90, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 4:
                m_pEffectSpr[153]->put_trans_sprite70(sX - 35, sY + 100, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 80, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 5:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 65, sY - 5, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 6:
                m_pEffectSpr[153]->put_trans_sprite70(sX + 45, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 31, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 7:
                m_pEffectSpr[153]->put_trans_sprite70(sX + 40, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 30, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 8:
                m_pEffectSpr[153]->put_trans_sprite70(sX + 20, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 20, sY + 16, _tmp_iEffectFrame % 15, dwTime);
                break;
        }
    }

    DisplayHPBar(_tmp_wObjectID, sX, sY, dwTime, _tmp_sOwnerType);

    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnAttackMove(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iWeapon{}, iAdd{}, iShieldIndex{}, iMantleIndex{}, dx{}, dy{}, dsx{}, dsy{};
    int cFrameMoveDots{};
    bool bInv = false, bDashDraw = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};
    bool is_player = false;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern
    if (_tmp_sOwnerType == 81) bInv = true; //Abaddon invis

    is_player = memcmp(m_cPlayerName, _tmp_cName, 10) == 0;

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);

    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (is_player) bInv = true;
        else
#ifndef DEF_HACKCLIENT
            if (_iGetFOE(_tmp_iStatus) == 1)
#endif
                bInv = true;
#ifndef DEF_HACKCLIENT
            else return false;//Invis hack?
#endif
    }

    switch (_tmp_cFrame)
    {
        case 4:  _tmp_cFrame = 4; break;
        case 5:  _tmp_cFrame = 4; break;
        case 6:  _tmp_cFrame = 4; break;
        case 7:  _tmp_cFrame = 4; break;
        case 8:  _tmp_cFrame = 4; break;
        case 9:  _tmp_cFrame = 4; break;
        case 10: _tmp_cFrame = 5; break;
        case 11: _tmp_cFrame = 6; break;
        case 12: _tmp_cFrame = 7; break;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
                if (iWeapon == 0) iAdd = 6;
                if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
                if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 4;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 5;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 5;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 5;

                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iWeapon = ((_tmp_sAppr2 & 0x0FF0) >> 4);
                if (iWeapon == 0) iAdd = 6;
                if ((iWeapon >= 1) && (iWeapon <= 39)) iAdd = 6;
                if ((iWeapon >= 40) && (iWeapon <= 59)) iAdd = 7;

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 4;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 5;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 5;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 5;

                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;


        default:
            iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }

    dx = 0;
    dy = 0;
//     if ((_tmp_cFrame >= 1) && (_tmp_cFrame <= 3))
//     {
// 
//         switch (_tmp_cFrame)
//         {
//             case 1: cFrameMoveDots = 26; break;
//             case 2: cFrameMoveDots = 16; break;
//             case 3: cFrameMoveDots = 0;  break;
//         }
// 
//         switch (_tmp_cDir)
//         {
//             case 1: dy = cFrameMoveDots; break;
//             case 2: dy = cFrameMoveDots; dx = -cFrameMoveDots; break;
//             case 3: dx = -cFrameMoveDots; break;
//             case 4: dx = -cFrameMoveDots; dy = -cFrameMoveDots; break;
//             case 5: dy = -cFrameMoveDots; break;
//             case 6: dy = -cFrameMoveDots; dx = cFrameMoveDots; break;
//             case 7: dx = cFrameMoveDots; break;
//             case 8: dx = cFrameMoveDots; dy = cFrameMoveDots; break;
//         }
// 
//         switch (_tmp_cFrame)
//         {
//             case 1: dy++;    break;
//             case 2: dy += 2; break;
//             case 3: dy++;    break;
//         }
//         switch (_tmp_cFrame)
//         {
//             case 2: bDashDraw = true; cFrameMoveDots = 26; break;
//             case 3: bDashDraw = true; cFrameMoveDots = 16; break;
//         }
// 
//         dsx = 0;
//         dsy = 0;
//         switch (_tmp_cDir)
//         {
//             case 1: dsy = cFrameMoveDots; break;
//             case 2: dsy = cFrameMoveDots; dsx = -cFrameMoveDots; break;
//             case 3: dsx = -cFrameMoveDots; break;
//             case 4: dsx = -cFrameMoveDots; dsy = -cFrameMoveDots; break;
//             case 5: dsy = -cFrameMoveDots; break;
//             case 6: dsy = -cFrameMoveDots; dsx = cFrameMoveDots; break;
//             case 7: dsx = cFrameMoveDots; break;
//             case 8: dsx = cFrameMoveDots; dsy = cFrameMoveDots; break;
//         }
//     }
//     else if (_tmp_cFrame > 3)
//     {
//         dx = 0;
//         dy = 0;
//     }
//     else
//     {
//         switch (_tmp_cDir)
//         {
//             case 1: dy = 32; break;
//             case 2: dy = 32; dx = -32; break;
//             case 3: dx = -32; break;
//             case 4: dx = -32; dy = -32; break;
//             case 5: dy = -32; break;
//             case 6: dy = -32; dx = 32; break;
//             case 7: dx = 32; break;
//             case 8: dx = 32; dy = 32; break;
//         }
//     }

    //////////////////////////////////////////////////////////////////////////
    // Replace old positioning code with interpolation

    int64_t time_elapsed = dwTime - _tmp_owner_time;
    float cycle_progress = (float)time_elapsed / (float)_tmp_frame_time;
    cycle_progress = cycle_progress > 1.0f ? 1.0f : cycle_progress;
    float interpolated_frame = _tmp_cFrame + cycle_progress;

    switch (_tmp_cDir)
    {
        case 1: dy = int(28 - (interpolated_frame * 4)); break;
        case 2: dy = int(28 - (interpolated_frame * 4)); dx = int((interpolated_frame * 4) - 28); break;
        case 3: dx = int((interpolated_frame * 4) - 28); break;
        case 4: dx = int((interpolated_frame * 4) - 28); dy = int((interpolated_frame * 4) - 28); break;
        case 5: dy = int((interpolated_frame * 4) - 28); break;
        case 6: dy = int((interpolated_frame * 4) - 28); dx = int(28 - (interpolated_frame * 4)); break;
        case 7: dx = int(28 - (interpolated_frame * 4)); break;
        case 8: dx = int(28 - (interpolated_frame * 4)); dy = int(28 - (interpolated_frame * 4)); break;
    }

    if (is_player)
    {
        camera_frame_details.time_elapsed = time_elapsed;
        camera_frame_details.cycle_progress = cycle_progress;
        camera_frame_details.interpolated_frame = interpolated_frame;
        camera_frame_details.offset_x = dx;
        camera_frame_details.offset_y = dy;
        camera_frame_details.direction = _tmp_cDir;
    }

    //////////////////////////////////////////////////////////////////////////

    if (m_bIsCrusadeMode) DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }

                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }
            /*
            if ((_tmp_sOwnerType != 10) && (_tmp_sOwnerType != 35)) {
                if (m_cDetailLevel != 0) {
                    if (sX < 50)
                         m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                }
            }
            */
            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // Fire Wyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    }
                    break;
            }

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            //
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }
        else
        {
            /*
            if ((_tmp_sOwnerType != 10) && (_tmp_sOwnerType != 35)) {
                if (m_cDetailLevel != 0) {
                    if (sX < 50)
                         m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                }
            }
            */
            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // Fire Wyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    }
                    break;
            }

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }


                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }
        }

        // Berserk 
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
        // Poison
//		if ((_tmp_iStatus & 0x80) != 0) 
//			m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);

        if (bDashDraw == true)
        {
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            if (iWeaponIndex != -1) m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            if (iShieldIndex != -1) m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dsx, sY + dsy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
        }
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }

    _tmp_dx = dx;
    _tmp_dy = dy;
    DisplayHPBar(_tmp_wObjectID, sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnMagic(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iR{}, iG{}, iB{}, iHelmIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern
    if (_tmp_sOwnerType == 81) bInv = true; //Change Abaddon invis

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }
    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0)
            bInv = true;
        else
        {
            if (_tmp_iChatIndex != 0)
            {
                if (m_pChatMsgList[_tmp_iChatIndex] != 0)
                {
                    m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
                    m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
                }
                else
                {
                    m_pMapData->ClearChatMsg(indexX, indexY);
                }
            }

            return false;
        }
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (8 * 8);
            iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 8;
            iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 8;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 8;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 8;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 8;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 8;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 8;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 8;
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (8 * 8);

            iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 8;
            iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 8;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 8;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 8;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 8;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 8;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 8;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 8;
            break;
    }
    /*
    switch (_tmp_cFrame) {
    case 15:
        _tmp_cFrame = 14;
        break;
    case 16:
        _tmp_cFrame = 14;
        break;
    case 17:
        _tmp_cFrame = 15;
        break;
    case 18:
        _tmp_cFrame = 15;
        break;
    case 19:
        _tmp_cFrame = 15;
        break;
    case 20:
        _tmp_cFrame = 15;
        break;
    case 21:
        _tmp_cFrame = 15;
        break;
    case 22:
        _tmp_cFrame = 15;
        break;
    case 23:
        _tmp_cFrame = 14;
        break;
    }
    */

    if (m_bIsCrusadeMode) DrawObjectFOE(sX, sY, _tmp_cFrame);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        /*
        if (m_cDetailLevel != 0) {
            if (sX < 50)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
        }
        */
        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);

        switch (_tmp_sOwnerType)
        {
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
                break;

            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
        }

        if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
        else
        {
            if ((_tmp_iStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
        }

        m_rcBodyRect = {
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
        };

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }

        if (iMantleIndex != -1)
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        // Berserk 
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Poison
//		if ((_tmp_iStatus & 0x80) != 0) 
//			m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            //m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    DisplayHPBar(_tmp_wObjectID, sX, sY, dwTime, _tmp_sOwnerType);


    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}


bool CGame::DrawObject_OnGetItem(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iR{}, iG{}, iB{}, iHelmIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else
#ifndef DEF_HACKCLIENT
            if (_iGetFOE(_tmp_iStatus) == 1)
#endif
                bInv = true;
#ifndef DEF_HACKCLIENT
            else return false;//Invis hack?
#endif
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (9 * 8);
            iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 9;
            iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 9;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 9;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 9;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 9;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 9;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 9;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 9;
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (9 * 8);

            iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 9;
            iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 9;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 9;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 9;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 9;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 9;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 9;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 9;
            break;

        default:
            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }

    if (m_bIsCrusadeMode) DrawObjectFOE(sX, sY, _tmp_cFrame);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        /*
        if (m_cDetailLevel != 0) {
            if (sX < 50)
                 m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
        }
        */
        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);

        switch (_tmp_sOwnerType)
        {
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
                break;

            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
        }

        if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite2(sX, sY, _tmp_cFrame, dwTime);
        else
        {
            if ((_tmp_iStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
        }

        m_rcBodyRect = {
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
        };

        if (iUndiesIndex != -1)
        {
            if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
        }

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }
        }

        if (iPantsIndex != -1)
        {
            if (bInv) m_pSprite[iPantsIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }
        }

        if (iArmArmorIndex != -1)
        {
            if (bInv) m_pSprite[iArmArmorIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }
        }

        if (iBodyArmorIndex != -1)
        {
            if (bInv) m_pSprite[iBodyArmorIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }
        }

        if (iHelmIndex != -1)
        {
            if (bInv) m_pSprite[iHelmIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }
        }

        if (iMantleIndex != -1)
        {
            if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }

        // Berserk 
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Poison
//		if ((_tmp_iStatus & 0x80) != 0) 
//			m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    DisplayHPBar(_tmp_wObjectID, sX, sY, dwTime, _tmp_sOwnerType);


    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnDamage(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iWeaponIndex{}, iShieldIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iAdd{}, iDrawMode{}, iMantleIndex{};
    char cFrame{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern
    if (_tmp_sOwnerType == 81) bInv = true; //Abaddon invis

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);

    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else
#ifndef DEF_HACKCLIENT
            if (_iGetFOE(_tmp_iStatus) == 1)
#endif
                bInv = true;
#ifndef DEF_HACKCLIENT
            else return false;//Invis hack?
#endif
    }

    cFrame = _tmp_cFrame;

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if (cFrame < 4)
            {
                if ((_tmp_sAppr2 & 0xF000) != 0) iAdd = 1;
                else iAdd = 0;

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * iAdd + (_tmp_cDir - 1);
                }
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;

                iDrawMode = 0;
            }
            else
            {
                cFrame -= 4;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 10;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 10;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 10;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 10;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 5;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 10;

                iDrawMode = 1;
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            if (cFrame < 4)
            {
                if ((_tmp_sAppr2 & 0xF000) != 0) iAdd = 1;
                else iAdd = 0;

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * iAdd + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;

                iDrawMode = 0;
            }
            else
            {
                cFrame -= 4;

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 10;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 10;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 10;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 10;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 5;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
                }
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 10;

                iDrawMode = 1;
            }
            break;

        default:
            if (cFrame < 4)
            {
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 67) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 68) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 69) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 73) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 81) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 86) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 87) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 89) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 91) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            }
            else
            {
                cFrame -= 4;
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 67) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 68) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 69) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 73) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else if (_tmp_sOwnerType == 81) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 86) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 87) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 89) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 91) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
                else iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);

            }

            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;

            iDrawMode = 0;
            break;
    }

    if (m_bIsCrusadeMode) DrawObjectFOE(sX, sY, cFrame);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {

        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);

        if (iDrawMode == 1)
        {

            if (_cDrawingOrder[_tmp_cDir] == 1)
            {
                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                    DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                    switch (iWeaponGlare)
                    {
                        case 0: break;
                        case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                /*
                if ((_tmp_sOwnerType != 10) && (_tmp_sOwnerType != 35)) {
                    if (m_cDetailLevel != 0) {
                        if (sX < 50)
                             m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
                    }
                }
                */
                switch (_tmp_sOwnerType)
                {
                    case 10: // Slime
                    case 35: // Energy Sphere
                    case 50: // TW
                    case 51: // CP
                    case 60: // Plant
                    case 65: // IceGolem
                    case 66: // Wyvern
                    case 73: // Fire Wyvern
                    case 81: // Abaddon
                    case 91: // Gate
                        break;

                    default:
                        if (m_cDetailLevel != 0)
                        {
                            if (sX < 50)
                                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
                        }
                        break;
                }
                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

                if (_tmp_sOwnerType == 81) //Abaddon
                {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
                }
                else if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_iStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
                }

                m_rcBodyRect = {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
                };

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                    switch (iShieldGlare)
                    {
                        case 0: break;
                            //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13, sY - 34, 0, dwTime);
                        case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
            else
            {
                /*
                if ((_tmp_sOwnerType != 10) && (_tmp_sOwnerType != 35)) {
                    if (m_cDetailLevel == 0) {
                        if (sX < 50)
                             m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
                    }
                }
                */
                switch (_tmp_sOwnerType)
                {
                    case 10: // Slime
                    case 35: // Energy Sphere
                    case 50: // TW
                    case 51: // CP
                    case 60: // Plant
                    case 65: // IceGolem
                    case 66: // Wyvern
                    case 73: // Fire Wyvern
                    case 81: // Abaddon
                    case 91: // Gate
                        break;

                    default:
                        if (m_cDetailLevel != 0)
                        {
                            if (sX < 50)
                                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
                        }
                        break;
                }
                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

                if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_iStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
                }

                m_rcBodyRect = {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
                };

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                    switch (iShieldGlare)
                    {
                        case 0: break;
                            //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13, sY - 34, 0, dwTime);
                        case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                    DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                    switch (iWeaponGlare)
                    {
                        case 0: break;
                        case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }
            }

            // Berserk 
            if ((_tmp_iStatus & 0x20) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, cFrame, 0, -5, -5, dwTime);
            // Poison
    //		if ((_tmp_iStatus & 0x80) != 0) 
    //			m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
        }
        else
        {

            if (_cDrawingOrder[_tmp_cDir] == 1)
            {
                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                    DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);//Change DK weapon glare

                    //V1.432 Weapon Glare
                    switch (iWeaponGlare)
                    {
                        case 0: break;
                        case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                /*
                if ((_tmp_sOwnerType != 10) && (_tmp_sOwnerType != 35)) {
                    if (m_cDetailLevel != 0) {
                        if (sX < 50)
                             m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
                    }
                }
                */
                switch (_tmp_sOwnerType)
                {
                    case 10:
                    case 35:
                    case 50:
                    case 51:
                    case 60:
                    case 65:
                    case 66:
                        break;

                    default:
                        if (m_cDetailLevel != 0)
                        {
                            if (sX < 50)
                                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
                        }
                        break;
                }

                if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_iStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
                }

                m_rcBodyRect = {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
                };

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                    switch (iShieldGlare)
                    {
                        case 0: break;
                            //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13, sY - 34, 0, dwTime);
                        case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
            else
            {
                /*
                if ((_tmp_sOwnerType != 10) && (_tmp_sOwnerType != 35)) {
                    if (m_cDetailLevel != 0) {
                        if (sX < 50)
                             m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
                    }
                }
                */
                switch (_tmp_sOwnerType)
                {
                    case 10: // Slime
                    case 35: // Energy Sphere
                    case 50: // TW
                    case 51: // CP
                    case 60: // Plant
                    case 65: // IceGolem
                    case 66: // Wyvern
                    case 73: // Fire Wyvern
                    case 81: // Abaddon
                    case 91: // Gate
                        break;

                    default:
                        if (m_cDetailLevel != 0)
                        {
                            if (sX < 50)
                                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
                        }
                        break;
                }

                if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_iStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
                }

                m_rcBodyRect = {
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
                };

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                    switch (iShieldGlare)
                    {
                        case 0: break;
                            //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13, sY - 34, 0, dwTime);
                        case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                    DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                    switch (iWeaponGlare)
                    {
                        case 0: break;
                        case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                        case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }
            }

            // Berserk 
            if ((_tmp_iStatus & 0x20) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, cFrame, 0, -5, -5, dwTime);
            // Poison
    //		if ((_tmp_iStatus & 0x80) != 0) 
    //			m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
        }
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }

    //Abaddon effects
    if (_tmp_sOwnerType == 81)
    {
        int randFrame = _tmp_cFrame % 12;
        m_pEffectSpr[154]->put_trans_sprite70(sX - 50, sY - 50, randFrame, dwTime);
        m_pEffectSpr[155]->put_trans_sprite70(sX - 20, sY - 80, randFrame, dwTime);
        m_pEffectSpr[156]->put_trans_sprite70(sX + 70, sY - 50, randFrame, dwTime);
        m_pEffectSpr[157]->put_trans_sprite70(sX - 30, sY, randFrame, dwTime);
        m_pEffectSpr[158]->put_trans_sprite70(sX - 60, sY + 90, randFrame, dwTime);
        m_pEffectSpr[159]->put_trans_sprite70(sX + 65, sY + 85, randFrame, dwTime);
        switch (_tmp_cDir)
        {
            case 1:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 108, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 50, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 2:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 70, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 3:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 105, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 90, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 4:
                m_pEffectSpr[153]->put_trans_sprite70(sX - 35, sY + 100, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 80, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 5:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 65, sY - 5, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 6:
                m_pEffectSpr[153]->put_trans_sprite70(sX + 45, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 31, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 7:
                m_pEffectSpr[153]->put_trans_sprite70(sX + 40, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 30, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 8:
                m_pEffectSpr[153]->put_trans_sprite70(sX + 20, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 20, sY + 16, _tmp_iEffectFrame % 15, dwTime);
                break;
        }
    }
    DisplayHPBar(_tmp_wObjectID, sX, sY, dwTime, _tmp_sOwnerType);

    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnDying(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iR{}, iG{}, iB{}, iHelmIndex{}, iMantleIndex{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};
    char cFrame{};

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    cFrame = _tmp_cFrame;
    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if (cFrame < 6)
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 0;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 0;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 0;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 0;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 0;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 0;
                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 0;
            }
            else
            {
                cFrame -= 6;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 11;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 11;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 11;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 11;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;
                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 11;
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            if (cFrame < 6)
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 0;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 0;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 0;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 0;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 0;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 0;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 0;
            }
            else
            {
                cFrame -= 6;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 11;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 11;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 11;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 11;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 11;
            }
            break;

        default:
            if (cFrame < 4)
            {
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 73) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 86) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 87) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 89) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                iUndiesIndex = -1;
                iHairIndex = -1;
                iArmArmorIndex = -1;
                iBodyArmorIndex = -1;
                iPantsIndex = -1;
                iBootsIndex = -1;
                iMantleIndex = -1;
                iHelmIndex = -1;

                switch (_tmp_sOwnerType)
                {
                    case 36: // AGT
                    case 37: // CGT
                    case 38: // MS
                    case 39: // DT
                    case 40: // ESG
                    case 41: // GMG
                    case 42: // ManaStone	

                        if (_tmp_sAppr2 == 0) cFrame = 0;
                        break;

                    case 51: cFrame = 0; break;
                }
            }
            else
            {
                switch (_tmp_sOwnerType)
                {
                    case 51: cFrame = 0; break;

                    default: cFrame -= 4; break;
                }

                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 73) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else if (_tmp_sOwnerType == 81) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 86) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 87) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 89) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                else if (_tmp_sOwnerType == 91) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                iUndiesIndex = -1;
                iHairIndex = -1;
                iArmArmorIndex = -1;
                iBodyArmorIndex = -1;
                iPantsIndex = -1;
                iBootsIndex = -1;
                iMantleIndex = -1;
                iHelmIndex = -1;
            }
            break;
    }

    if (m_bIsCrusadeMode) DrawObjectFOE(sX, sY, cFrame);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {

        /*
        if (_tmp_sOwnerType != 10) {
            if (m_cDetailLevel != 0) {
                if (sX < 50)
                     m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
            }
        }
        */
        switch (_tmp_sOwnerType)
        {
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
                break;

            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, cFrame, dwTime);
                }
                break;
        }

        //if( _tmp_sOwnerType == 66 ) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime); 
        //if( _tmp_sOwnerType == 73 ) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime); 
        if (_tmp_sOwnerType == 81)
        {
            m_pEffectSpr[152]->put_trans_sprite70(sX - 80, sY - 15, _tmp_iEffectFrame % 27, dwTime); // Explosion Abaddon
            m_pEffectSpr[152]->put_trans_sprite70(sX, sY - 15, _tmp_iEffectFrame % 27, dwTime);
            m_pEffectSpr[152]->put_trans_sprite70(sX - 40, sY, _tmp_iEffectFrame % 27, dwTime);
            m_pEffectSpr[163]->put_trans_sprite70(sX - 90, sY - 80, _tmp_iEffectFrame % 12, dwTime); // Ames qui s'envolent
            m_pEffectSpr[160]->put_trans_sprite70(sX - 60, sY - 50, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[161]->put_trans_sprite70(sX - 30, sY - 20, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[162]->put_trans_sprite70(sX, sY - 100, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[163]->put_trans_sprite70(sX + 30, sY - 30, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[162]->put_trans_sprite70(sX + 60, sY - 90, _tmp_iEffectFrame % 12, dwTime);
            m_pEffectSpr[163]->put_trans_sprite70(sX + 90, sY - 50, _tmp_iEffectFrame % 12, dwTime);
            switch (_tmp_cDir)
            {
                case 1: m_pEffectSpr[140]->put_trans_sprite70(sX, sY, cFrame, dwTime); break; // Abbadon dying
                case 2: m_pEffectSpr[141]->put_trans_sprite70(sX, sY, cFrame, dwTime); break; // fixed sprit IDs
                case 3: m_pEffectSpr[142]->put_trans_sprite70(sX, sY, cFrame, dwTime); break;
                case 4: m_pEffectSpr[143]->put_trans_sprite70(sX, sY, cFrame, dwTime); break;
                case 5: m_pEffectSpr[144]->put_trans_sprite70(sX, sY, cFrame, dwTime); break;
                case 6: m_pEffectSpr[145]->put_trans_sprite70(sX, sY, cFrame, dwTime); break;
                case 7: m_pEffectSpr[146]->put_trans_sprite70(sX, sY, cFrame, dwTime); break;
                case 8: m_pEffectSpr[147]->put_trans_sprite70(sX, sY, cFrame, dwTime); break;
            }
        }
        else if (_tmp_sOwnerType == 66) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
        //else if( _tmp_sOwnerType == 73 ) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime); 
        else
        {
            if ((_tmp_iStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
        }

        m_rcBodyRect = {
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
        };

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }

        if (iMantleIndex != -1)
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        // Berserk 
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, cFrame, 0, -5, -5, dwTime);
        // Poison
//		if ((_tmp_iStatus & 0x80) != 0) 
//			m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    DisplayHPBar(_tmp_wObjectID, sX, sY, dwTime, _tmp_sOwnerType);



    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnDead(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iR{}, iG{}, iB{}, iFrame{}, iMantleIndex{}, iHelmIndex{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};

    if (_tmp_sOwnerType == 66) return false;
    //if( _tmp_sOwnerType == 73 ) return false;

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iFrame = 7;
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
            iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 11;
            iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 11;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 11;
            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 11;
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 11;
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iFrame = 7;
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
            iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 11;
            iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 11;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 11;
            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 11;
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 11;
            break;


        default:
            switch (_tmp_sOwnerType)
            {
                case 28: // Troll
                case 29: // Ogre
                case 30: // Liche
                case 31: // DD
                case 63: // Frost
                    iFrame = 5;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 32: // Uni
                case 33: // WW
                case 43: // LWB
                case 44: // GHK
                case 45: // GHKABS
                case 46: // TK
                case 47: // BG
                case 48: // SK
                case 49: // HC
                case 50: // TW
                case 53: // BB
                case 54: // DE
                case 55: // Rabbit
                case 56: // Cat
                case 57: // Frog	
                case 58: // MG
                case 59: // Ettin
                case 60: // Plant
                case 61: // Rudolph
                case 62: // Direboar
                case 64: // Crops
                case 65: // IceGolem
                case 70: // Barlog
                case 71: // Centaur
                case 72: // ClawTurtle
                case 74: // GiantCrayfish
                case 75: // Giant Lizard
                case 76: // Giant Tree
                case 77: // Master Orc
                case 78: // Minotaur
                case 79: // Nizie
                case 80: // Tentocle
                case 82: // Sorceress
                case 83: // ATK
                case 84: // MasterElf
                case 85: // DSK
                case 88: // Barbarian
                    iFrame = 7;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 86: // HBT
                case 87: // CT
                case 89: // AGC
                    iFrame = 7;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
                    break;

                case 66: // Wyvern
                    iFrame = 15;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                    break;

                case 73: // FireWyvern
                    iFrame = 7;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                    //bTrans = true; // Prevents showing ugly corpse
                    break;

                case 81: // Abaddon	
                    iFrame = 0;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                    //bTrans = true; // Prevents showing ugly corpse
                    break;

                case 51: // CP
                    iFrame = 0;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 52: // GG
                    iFrame = 11;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 91: // Gate
                    iFrame = 5;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                    break;

                default:
                    iFrame = 3;
                    iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;
            }

            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }

    if (bTrans == false)
    {
        if (_tmp_cFrame == -1)
        {
            _tmp_cFrame = 7;

            if ((_tmp_iStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, iFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, iFrame, dwTime);

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if (iMantleIndex != -1)
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }
        else
        {
            if ((_tmp_iStatus & 0x20) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, iFrame, -2 * _tmp_cFrame + 5, -2 * _tmp_cFrame - 5, -2 * _tmp_cFrame - 5, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, iFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, dwTime);
        }
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearDeadChatMsg(indexX, indexY);
        }
    }

    //Abaddon's Death Animation (lightning)
    if (_tmp_sOwnerType == 81)
    {
        Abaddon_corpse(sX, sY);
    }
    //	else if (_tmp_sOwnerType == 73)	
    //	{
            //m_pEffectSpr[35]->put_trans_sprite70(sX+120, sY+120, rand(), dwTime);
    //		m_pEffectSpr[35]->put_trans_sprite70(sX+20, sY-15,  rand()%10, dwTime);
    //	}
    DisplayHPBar(_tmp_wObjectID, sX, sY, dwTime, _tmp_sOwnerType);

    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnMove(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int dx{}, dy{};
    int iBodyIndex{}, iHairIndex{}, iUndiesIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iShieldIndex{}, iAdd{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};
    bool is_player = false;

    is_player = memcmp(m_cPlayerName, _tmp_cName, 10) == 0;

    //if(_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern
    if (_tmp_sOwnerType == 35) bInv = true; //Energy-Ball,Wyvern
    //if(_tmp_sOwnerType == 73) bInv = true; //Energy-Ball,Wyvern
    if (_tmp_sOwnerType == 66)
    {
        bInv = true; //Energy-Ball,Wyvern
    }

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);

    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (is_player) bInv = true;
        else
#ifndef DEF_HACKCLIENT
            if (_iGetFOE(_tmp_iStatus) == 1)
#endif
                bInv = true;
#ifndef DEF_HACKCLIENT
            else return false;//Invis hack?
#endif
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iAdd = 3;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 3;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 2;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 2;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iAdd = 3;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 3;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 2;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 2;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
            }
            break;

        default:
            if (_tmp_sOwnerType == 66) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
            else if (_tmp_sOwnerType == 73) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
            else iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);

            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }

    dx = 0;
    dy = 0;

    int64_t time_elapsed = dwTime - _tmp_owner_time;
    float cycle_progress = (float)time_elapsed / (float)_tmp_frame_time;
    cycle_progress = cycle_progress > 1.0f ? 1.0f : cycle_progress;
    float interpolated_frame = _tmp_cFrame + cycle_progress;

    switch (_tmp_cDir)
    {
        case 1: dy = int(28 - (interpolated_frame * 4)); break;
        case 2: dy = int(28 - (interpolated_frame * 4)); dx = int((interpolated_frame * 4) - 28); break;
        case 3: dx = int((interpolated_frame * 4) - 28); break;
        case 4: dx = int((interpolated_frame * 4) - 28); dy = int((interpolated_frame * 4) - 28); break;
        case 5: dy = int((interpolated_frame * 4) - 28); break;
        case 6: dy = int((interpolated_frame * 4) - 28); dx = int(28 - (interpolated_frame * 4)); break;
        case 7: dx = int(28 - (interpolated_frame * 4)); break;
        case 8: dx = int(28 - (interpolated_frame * 4)); dy = int(28 - (interpolated_frame * 4)); break;
    }

    if (is_player)
    {
        camera_frame_details.time_elapsed = time_elapsed;
        camera_frame_details.cycle_progress = cycle_progress;
        camera_frame_details.interpolated_frame = interpolated_frame;
        camera_frame_details.offset_x = dx;
        camera_frame_details.offset_y = dy;
        camera_frame_details.direction = _tmp_cDir;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:

        case 28: // Troll.
        case 29: // Orge.
        case 30: // Liche
        case 31: // DD
        case 32: // Uni
        case 33: // ww

        case 43: // LWB
        case 44: // GHK
        case 45: // GHKABS
        case 46: // TK
        case 47: // BG
        case 48: // SK
        case 49: // HC
        case 50: // TW

        case 52: // GG
        case 53: // BB
        case 54: // DE
        case 55: // Rabbit
        case 56: // Cat
        case 57: // Frog
        case 58: // MG
        case 59: // Ettin
        case 60: // Plant
        case 61: // Rudolph
        case 62: // DireBoar
        case 63: // Frost

        case 65: // Ice-Golem
        case 66: // Wyvern

        case 70: // Balrog
        case 71: // Centaur
        case 72: // ClawTurtle
        case 73: // FireWyvern
        case 74: // GiantCrayfish
        case 75: // Gi Lizard
        case 76: // Gi Tree
        case 77: // Master Orc
        case 78: // Minotaur
        case 79: // Nizie
        case 80: // Tentocle
        case 81: // Abaddon
        case 82: // Sorceress
        case 83: // ATK
        case 84: // MasterElf
        case 85: // DSK
        case 86: // HBT
        case 87: // CT
        case 88: // Barbarian
        case 89: // AGC
        case 90: // Gail
            break;

        default:
            _tmp_cFrame = _tmp_cFrame / 2;
            break;
    }

    if (m_bIsCrusadeMode) DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);

    //	if ((_tmp_sAppr1 == m_sAppr1_IE) && (_tmp_sAppr2 == m_sAppr2_IE) && (m_pMapData[m_sPlayerX][m_sPlayerY].m_sItemSprite != 0))
    //		bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTGETITEM, m_cPlayerDir, 0, 0, 0, 0);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (_tmp_sOwnerType == 65) // IceGolem
    {
        //m_pEffectSpr[77]->put_trans_sprite70(sX+dx, sY+dy, _tmp_cFrame, dwTime);
        switch (rand() % 3)
        {
            case 0:
                m_pEffectSpr[76]->put_trans_sprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                break;
            case 1:
                m_pEffectSpr[77]->put_trans_sprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                break;
            case 2:
                m_pEffectSpr[78]->put_trans_sprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                break;
        }
    }

    if (bTrans == false)
    {
        CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->put_trans_sprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // Fire Wyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    }
                    break;
            }
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX + dx, sY + dy, 1, dwTime);

            if (_tmp_sOwnerType == 81) //Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            else if (bInv == true)
                //m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
        }
        else
        {
            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // Fire Wyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    }
                    break;
            }
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX + dx, sY + dy, 1, dwTime);

            if (_tmp_sOwnerType == 81) //Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            else if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            //
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->put_trans_sprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        // Berserk 
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
        // Poison
    //	if ((_tmp_iStatus & 0x80) != 0) 
    //		m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
//         _text.setFont(*arya_font);
//         _text.setOutlineThickness(0);
//         _text.setOutlineColor(sf::Color::Black);
//         _text.setFillColor(sf::Color::White);
//         _text.setCharacterSize(14);
//         _text.setPosition(sX + dx + 100, sY + dy);
//         _text.setString(fmt::format(
//             "index: {}, {}\n"
//             "s: {}, {}\n"
//             "d: {}, {}\n",
//             indexX, indexY, sX, sY, dx, dy
//         ));
//         draw(_text);
//         return true;
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }

    _tmp_dx = dx;
    _tmp_dy = dy;
    // Abaddon effects
    if (_tmp_sOwnerType == 81)
    {
        int randFrame = _tmp_iEffectFrame % 12;
        m_pEffectSpr[154]->put_trans_sprite70(sX - 50, sY - 50, randFrame, dwTime);
        m_pEffectSpr[155]->put_trans_sprite70(sX - 20, sY - 80, randFrame, dwTime);
        m_pEffectSpr[156]->put_trans_sprite70(sX + 70, sY - 50, randFrame, dwTime);
        m_pEffectSpr[157]->put_trans_sprite70(sX - 30, sY, randFrame, dwTime);
        m_pEffectSpr[158]->put_trans_sprite70(sX - 60, sY + 90, randFrame, dwTime);
        m_pEffectSpr[159]->put_trans_sprite70(sX + 65, sY + 85, randFrame, dwTime);
        switch (_tmp_cDir)
        {
            case 1:
                m_pEffectSpr[153]->put_trans_sprite70(sX + dx, sY + dy + 108, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX + dx - 50, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 2:
                m_pEffectSpr[153]->put_trans_sprite70(sX + dx, sY + dy + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX + dx - 70, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 3:
                m_pEffectSpr[153]->put_trans_sprite70(sX + dx, sY + dy + 105, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX + dx - 90, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 4:
                m_pEffectSpr[153]->put_trans_sprite70(sX + dx - 35, sY + dy + 100, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX + dx - 80, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 5:
                m_pEffectSpr[153]->put_trans_sprite70(sX + dx, sY + dy + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX + dx - 65, sY + dy - 5, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 6:
                m_pEffectSpr[153]->put_trans_sprite70(sX + dx + 45, sY + dy + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX + dx - 31, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 7:
                m_pEffectSpr[153]->put_trans_sprite70(sX + dx + 40, sY + dy + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX + dx - 30, sY + dy + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 8:
                m_pEffectSpr[153]->put_trans_sprite70(sX + dx + 20, sY + dy + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX + dx - 20, sY + dy + 16, _tmp_iEffectFrame % 15, dwTime);
                break;
        }
    }

    DisplayHPBar(_tmp_wObjectID, sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

//      _text.setFont(*arya_font);
//      _text.setOutlineThickness(0);
//      _text.setOutlineColor(sf::Color::Black);
//      _text.setFillColor(sf::Color::White);
//      _text.setCharacterSize(14);
//      _text.setPosition(sX+dx, sY+dy + 100);
//      _text.setString(fmt::format(
//          "index: {}, {}\n"
//          "s: {}, {}\n"
//          "d: {}, {}\n",
//          indexX, indexY, sX, sY, dx, dy
//      ));
//      draw(_text);
// 
//     char cTxt[64], cTxt2[128];
//     memset(cTxt, 0, sizeof(cTxt));
//     memset(cTxt2, 0, sizeof(cTxt2));
// 
//     GetNpcName(_tmp_sOwnerType, cTxt);
//     if ((_tmp_iStatus & 0x20) != 0) strcat(cTxt, DRAW_OBJECT_NAME50);//" Berserk" 
//     if ((_tmp_iStatus & 0x40) != 0) strcat(cTxt, DRAW_OBJECT_NAME51);//" Frozen"
// 
//     put_string2(sX + dx, sY + dy + 40, cTxt, 255, 255, 255);
// 
//     if (m_bIsObserverMode == true) put_string2(sX + dx, sY + dy + 14, cTxt, 50, 50, 255);
//     else if (m_bIsConfusion || (m_iIlusionOwnerH != 0))
//     {
//         memset(cTxt, 0, sizeof(cTxt));
//         strcpy(cTxt, DRAW_OBJECT_NAME87);
//         put_string2(sX + dx, sY + dy + 14, cTxt, 150, 150, 150); // v2.171
//     }
//     else
//     {
//         switch (_iGetFOE(_tmp_iStatus))
//         {
//             case -2:
//                 put_string2(sX + dx, sY + dy + 14 + 40, DRAW_OBJECT_NAME90, 255, 0, 0);
//                 break;
//             case -1:
//                 put_string2(sX + dx, sY + dy + 14 + 40, DRAW_OBJECT_NAME90, 255, 0, 0);
//                 break;
//             case 0:
//                 put_string2(sX + dx, sY + dy + 14 + 40, DRAW_OBJECT_NAME88, 50, 50, 255);
//                 break;
//             case 1:
//                 put_string2(sX + dx, sY + dy + 14 + 40, DRAW_OBJECT_NAME89, 30, 255, 30);
//                 break;
//         }
//     }
// 
//     switch ((_tmp_iStatus & 0x0F00) >> 8)
//     {
//         case 0: break;
//         case 1: strcpy(cTxt2, DRAW_OBJECT_NAME52); break;//"Clairvoyant"
//         case 2: strcpy(cTxt2, DRAW_OBJECT_NAME53); break;//"Destruction of Magic Protection"
//         case 3: strcpy(cTxt2, DRAW_OBJECT_NAME54); break;//"Anti-Physical Damage"
//         case 4: strcpy(cTxt2, DRAW_OBJECT_NAME55); break;//"Anti-Magic Damage"
//         case 5: strcpy(cTxt2, DRAW_OBJECT_NAME56); break;//"Poisonous"
//         case 6: strcpy(cTxt2, DRAW_OBJECT_NAME57); break;//"Critical Poisonous" 
//         case 7: strcpy(cTxt2, DRAW_OBJECT_NAME58); break;//"Explosive"  
//         case 8: strcpy(cTxt2, DRAW_OBJECT_NAME59); break;//"Critical Explosive"
//     }
//     if (m_Misc.bCheckIMEString(cTxt2)) put_string_sprite_font3(sX + dx, sY + dy + 28 + 40, cTxt2, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
//     else put_string2(sX + dx, sY + dy + 28 + 40, cTxt2, 240, 240, 70);



    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnDamageMove(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int cFrame{}, cDir{};
    int dx{}, dy{};
    int iBodyIndex{}, iHairIndex{}, iUndiesIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iShieldIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};
    bool is_player = memcmp(m_cPlayerName, _tmp_cName, 10) == 0;

    if (_tmp_sOwnerType == 67 || _tmp_sOwnerType == 68 || _tmp_sOwnerType == 69 || _tmp_sOwnerType == 81) return false;
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);

    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (is_player) bInv = true;
        else
#ifndef DEF_HACKCLIENT
            if (_iGetFOE(_tmp_iStatus) == 1)
#endif
                bInv = true;
#ifndef DEF_HACKCLIENT
            else return false;//Invis hack?
#endif
    }

    cDir = _tmp_cDir;
    switch (_tmp_cDir)
    {
        case 1: _tmp_cDir = 5; break;
        case 2: _tmp_cDir = 6; break;
        case 3: _tmp_cDir = 7; break;
        case 4: _tmp_cDir = 8; break;
        case 5: _tmp_cDir = 1; break;
        case 6: _tmp_cDir = 2; break;
        case 7: _tmp_cDir = 3; break;
        case 8: _tmp_cDir = 4; break;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
            iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 10;
            iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 10;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 10;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 10;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;

            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 5;

            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
            }

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 10;
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);

            iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 10;
            iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 10;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 10;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 10;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;

            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 5;

            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
            }

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 10;
            break;

        default:
            if (_tmp_sOwnerType == 66) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else if (_tmp_sOwnerType == 73) iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);

            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }

    dx = 0;
    dy = 0;

    int64_t time_elapsed = dwTime - _tmp_owner_time;
    float cycle_progress = (float)time_elapsed / (float)_tmp_frame_time;
    cycle_progress = cycle_progress > 1.0f ? 1.0f : cycle_progress;
    float interpolated_frame = _tmp_cFrame + cycle_progress;

    switch (_tmp_cDir)
    {
        case 1: dy = int(28 - (interpolated_frame * 4)); break;
        case 2: dy = int(28 - (interpolated_frame * 4)); dx = int((interpolated_frame * 4) - 28); break;
        case 3: dx = int((interpolated_frame * 4) - 28); break;
        case 4: dx = int((interpolated_frame * 4) - 28); dy = int((interpolated_frame * 4) - 28); break;
        case 5: dy = int((interpolated_frame * 4) - 28); break;
        case 6: dy = int((interpolated_frame * 4) - 28); dx = int(28 - (interpolated_frame * 4)); break;
        case 7: dx = int(28 - (interpolated_frame * 4)); break;
        case 8: dx = int(28 - (interpolated_frame * 4)); dy = int(28 - (interpolated_frame * 4)); break;
    }

    if (is_player)
    {
        camera_frame_details.time_elapsed = time_elapsed;
        camera_frame_details.cycle_progress = cycle_progress;
        camera_frame_details.interpolated_frame = interpolated_frame;
        camera_frame_details.offset_x = dx;
        camera_frame_details.offset_y = dy;
        camera_frame_details.direction = _tmp_cDir;
    }

    cFrame = _tmp_cFrame;

    if (m_bIsCrusadeMode) DrawObjectFOE(sX + dx, sY + dy, cFrame);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX + dy, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {

        CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            /*
            if ((_tmp_sOwnerType != 10) && (_tmp_sOwnerType != 35)) {
                if (m_cDetailLevel != 0) {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX+dx, sY+dy, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX+dx, sY+dy, cFrame, dwTime);
                }
            }
            */
            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // Fire Wyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, cFrame, dwTime);
                    }
                    break;
            }
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                else m_pSprite[iHelmIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            //
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }
        else
        {
            /*
            if ((_tmp_sOwnerType != 10) && (_tmp_sOwnerType != 35)) {
                if (m_cDetailLevel == 0) {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX+dx, sY+dy, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX+dx, sY+dy, cFrame, dwTime);
                }
            }
            */
            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // Fire Wyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, cFrame, dwTime);
                    }
                    break;
            }
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iHelmIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        // Berserk 
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, cFrame, 0, -5, -5, dwTime);
        // Poison
    //	if ((_tmp_iStatus & 0x80) != 0) 
    //		m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }

    _tmp_dx = dx;
    _tmp_dy = dy;

    DisplayHPBar(_tmp_wObjectID, sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    short dx{}, dy{};
    int iBodyIndex{}, iHairIndex{}, iUndiesIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iShieldIndex{}, iAdd{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};

    iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
    iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
    iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
    iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
    iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
    iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
    iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
    iHelmColor = (_tmp_iApprColor & 0x0000000F);

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iAdd = 3;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else
                    {
                        iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                    }
                }
                else
                {
                    iBodyArmorIndex = -1;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else
                {
                    iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 3;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else
                    {
                        iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;
                    }
                }
                else
                {
                    iBodyArmorIndex = -1;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else
                {
                    iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 2;
                }

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else
                {
                    iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;
                }

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else
                {
                    iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
                }

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 2;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
            }
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iAdd = 3;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else
                    {
                        iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                    }
                }
                else
                {
                    iBodyArmorIndex = -1;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else
                {
                    iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 3;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else
                    {
                        iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;
                    }
                }
                else
                {
                    iBodyArmorIndex = -1;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else
                {
                    iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 2;
                }

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 2;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
            }
            break;

        default:
            iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);

            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iHelmIndex = -1;
            break;
    }

    dx = 0;
    dy = 0;

    if (_cDrawingOrder[_tmp_cDir] == 1)
    {
        if (iWeaponIndex != -1)
        {
            if (iWeaponColor == 0)
                m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
        }

        switch (_tmp_sOwnerType)
        {
            /*case 10:
            case 35:
            case 50:
            case 51:*/
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
                break;

            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
        }

        if (_tmp_sOwnerType == 81) //Abaddon
        {
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
        }
        else if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);

        //
        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iUndiesIndex != -1)
        {
            if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
        }

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }

        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iShieldIndex != -1)
        {
            if (iShieldColor == 0)
                m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
        }

        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }
    }
    else
    {
        switch (_tmp_sOwnerType)
        {
            case 10: // Slime
            case 35: // Energy Sphere
            case 50: // TW
            case 51: // CP
            case 60: // Plant
            case 65: // IceGolem
            case 66: // Wyvern
            case 73: // Fire Wyvern
            case 81: // Abaddon
            case 91: // Gate
                break;

            default:
                if (m_cDetailLevel != 0)
                {
                    if (sX < 50)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
        }

        if (_tmp_sOwnerType == 81) //Abaddon
        {
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
        }
        else if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);

        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }

        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iShieldIndex != -1)
        {
            if (iShieldColor == 0)
                m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
        }

        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iWeaponIndex != -1)
        {
            if (iWeaponColor == 0)
                m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
        }
    }

    if (_tmp_iChatIndex != 0)
    {
        if (m_pChatMsgList[_tmp_iChatIndex] != 0)
        {

            DrawChatMsgBox(sX + dx, sY + dy, _tmp_iChatIndex, false);

        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }

    _tmp_dx = dx;
    _tmp_dy = dy;
    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnStop(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iBodyArmorIndex{}, iArmArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iShieldIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66 || _tmp_sOwnerType == 81) bInv = true; //Energy-Ball, Wyvern

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

#ifdef DEF_COLOR
    iWeaponColor = G_iColor;
    iShieldColor = G_iColor;
    iArmorColor = G_iColor;
    iMantleColor = G_iColor;
    iArmColor = G_iColor;
    iPantsColor = G_iColor;
    iBootsColor = G_iColor;
    iHelmColor = G_iColor;
#endif

    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);

    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else
#ifndef DEF_HACKCLIENT
            if (_iGetFOE(_tmp_iStatus) == 1)
#endif
                bInv = true;
#ifndef DEF_HACKCLIENT
            else return false;//Invis hack?
#endif
    }

    //Single-direction monsters
    switch (_tmp_sOwnerType)
    {
        case 91: // Gate 
            if (_tmp_cDir <= 3) _tmp_cDir = 3;
            else  _tmp_cDir = 5;
            break;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            _tmp_cFrame = _tmp_cFrame / 2;

            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (1 * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 1;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 1;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 1;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 1;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 1;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 1;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    // something is broken with stop animation of LightAxe 8 * (1). 8 * (3) (move animation) can be a temp fix
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 1 + (_tmp_cDir - 1);
                }
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 1;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 1;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 1;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
                iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 0 + (_tmp_cDir - 1);
                }
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 0;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 0;
            }
            break;

        case 4:
        case 5:
        case 6:
            _tmp_cFrame = _tmp_cFrame / 2;
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            if ((_tmp_sAppr2 & 0xF000) != 0)
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (1 * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 1;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 1;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 1;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 1;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 1;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 1;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 1 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 1;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 1;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 1;
            }
            else
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);

                iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 0 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 0;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 0;
            }
            break;

        default:
            if (_tmp_sAppr2 != 0)
            {
                iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                _tmp_cFrame = (_tmp_sAppr2 & 0x00FF) - 1;
            }
            /*
            else if (_tmp_sOwnerType == 66) iBodyIndex = DEF_NPC +  (_tmp_sOwnerType - 10 )*8*7 + (0 * 8);
            else if (_tmp_sOwnerType == 73) iBodyIndex = DEF_NPC +  (_tmp_sOwnerType - 10 )*8*7 + (0 * 8);
            else if (_tmp_sOwnerType == 81) iBodyIndex =  DEF_SPRID_MOB  +  (_tmp_sOwnerType - 10 )*8*7 + (0 * 8);
            */
            else iBodyIndex = DEF_SPRID_NPC + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);

            iUndiesIndex = -1;
            iHairIndex = -1;
            iBodyArmorIndex = -1;
            iArmArmorIndex = -1;
            iBootsIndex = -1;
            iPantsIndex = -1;
            iWeaponIndex = -1;
            iShieldIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }

    if (m_bIsCrusadeMode) DrawObjectFOE(sX, sY, _tmp_cFrame);
    switch (_tmp_sOwnerType)
    {
        case 15: // ShopKeeper
        case 19: // Gandalf
        case 20: // Howard
        case 24: // Tom
        case 25: // William
        case 26: // Kenedy
        case 51: // CP
        case 86: // HBT
        case 90: // Gail
            m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);
            break;
    }

#ifdef _DEBUG
    //m_pEffectSpr[74]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime );
    //m_pEffectSpr[79]->put_trans_sprite70_no_color_key(sX, sY+32, _tmp_cFrame, dwTime );
#endif
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        CheckActiveAura(sX, sY, dwTime, _tmp_sOwnerType);

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->put_trans_sprite25(sX, sY, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // Fire Wyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                    }
                    break;
            }
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (_tmp_sOwnerType == 81) //Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);

            }
            else if (bInv) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
        }
        else
        {
            switch (_tmp_sOwnerType)
            {
                case 10: // Slime
                case 35: // Energy Sphere
                case 50: // TW
                case 51: // CP
                case 60: // Plant
                case 65: // IceGolem
                case 66: // Wyvern
                case 73: // Fire Wyvern
                case 81: // Abaddon
                case 91: // Gate
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                    }
                    break;
            }
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (_tmp_sOwnerType == 81) //Abaddon
            {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            }
            else if (bInv) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
            }
            //if (!m_pSprite[iBodyIndex + (_tmp_cDir - 1)])
            //	return false;

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->put_trans_sprite25(sX, sY, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        if (_tmp_sOwnerType == 64)
        {
            switch (_tmp_cFrame)
            {
                case 0:
                    m_pEffectSpr[84]->put_trans_sprite(sX + 52, sY + 54, (dwTime % 3000) / 120, dwTime);
                    break;
                case 1:
                    m_pEffectSpr[83]->put_trans_sprite(sX + 53, sY + 59, (dwTime % 3000) / 120, dwTime);
                    break;
                case 2:
                    m_pEffectSpr[82]->put_trans_sprite(sX + 53, sY + 65, (dwTime % 3000) / 120, dwTime);
                    break;
            }
        }
        // Berserk 
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Poison
    //	if ((_tmp_iStatus & 0x80) != 0) 
    //		m_pEffectSpr[81]->put_trans_sprite70(sX+115, sY+85, _tmp_iEffectFrame%21, dwTime);
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }
    // : Abaddon effects
    if (_tmp_sOwnerType == 81)
    {
        int randFrame = _tmp_cFrame % 12;
        m_pEffectSpr[154]->put_trans_sprite70(sX - 50, sY - 50, randFrame, dwTime);
        m_pEffectSpr[155]->put_trans_sprite70(sX - 20, sY - 80, randFrame, dwTime);
        m_pEffectSpr[156]->put_trans_sprite70(sX + 70, sY - 50, randFrame, dwTime);
        m_pEffectSpr[157]->put_trans_sprite70(sX - 30, sY, randFrame, dwTime);
        m_pEffectSpr[158]->put_trans_sprite70(sX - 60, sY + 90, randFrame, dwTime);
        m_pEffectSpr[159]->put_trans_sprite70(sX + 65, sY + 85, randFrame, dwTime);
        switch (_tmp_cDir)
        {
            case 1:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 108, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 50, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 2:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 70, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 3:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 105, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 90, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 4:
                m_pEffectSpr[153]->put_trans_sprite70(sX - 35, sY + 100, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 80, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 5:
                m_pEffectSpr[153]->put_trans_sprite70(sX, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 65, sY - 5, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 6:
                m_pEffectSpr[153]->put_trans_sprite70(sX + 45, sY + 95, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 31, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 7:
                m_pEffectSpr[153]->put_trans_sprite70(sX + 40, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 30, sY + 10, _tmp_iEffectFrame % 15, dwTime);
                break;
            case 8:
                m_pEffectSpr[153]->put_trans_sprite70(sX + 20, sY + 110, _tmp_iEffectFrame % 28, dwTime);
                m_pEffectSpr[164]->put_trans_sprite70(sX - 20, sY + 16, _tmp_iEffectFrame % 15, dwTime);
                break;
        }
    }

    DisplayHPBar(_tmp_wObjectID, sX, sY, dwTime, _tmp_sOwnerType);

    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnRun(int indexX, int indexY, int sX, int sY, bool bTrans, int64_t dwTime, int msX, int msY)
{
    int dx{}, dy{};
    int iBodyIndex{}, iHairIndex{}, iUndiesIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iWeaponIndex{}, iShieldIndex{}, iHelmIndex{}, iR{}, iG{}, iB{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw{};
    bool is_player = memcmp(m_cPlayerName, _tmp_cName, 10) == 0;

    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern
    if (_tmp_sOwnerType == 81) bInv = true; //Change Abaddon invis

    if (m_cDetailLevel == 0)
    {
        iWeaponColor = 0;
        iShieldColor = 0;
        iArmorColor = 0;
        iMantleColor = 0;
        iArmColor = 0;
        iPantsColor = 0;
        iBootsColor = 0;
        iHelmColor = 0;
    }
    else
    {
        iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
        iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
        iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
        iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
        iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
        iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
        iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
        iHelmColor = (_tmp_iApprColor & 0x0000000F);
    }

    iWeaponGlare = (_tmp_sAppr4 & 0x000C) >> 2;
    iShieldGlare = (_tmp_sAppr4 & 0x0003);

    if ((_tmp_iStatus & 0x10) != 0)
    {
        if (is_player) bInv = true;
        else
#ifndef DEF_HACKCLIENT
            if (_iGetFOE(_tmp_iStatus) == 1)
#endif
                bInv = true;
#ifndef DEF_HACKCLIENT
            else return false;//Invis hack?
#endif
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (4 * 8);
            iUndiesIndex = DEF_SPRID_UNDERWEAR_M + (_tmp_sAppr1 & 0x000F) * 15 + 4;
            iHairIndex = DEF_SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 4;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 4;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 4;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 4;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 4;

            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = DEF_SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 6 + (_tmp_cDir - 1);
            }

            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = DEF_SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 6;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 4;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 4;
            break;

        case 4:
        case 5:
        case 6:
            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (4 * 8);

            iUndiesIndex = DEF_SPRID_UNDERWEAR_W + (_tmp_sAppr1 & 0x000F) * 15 + 4;
            iHairIndex = DEF_SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 4;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = DEF_SPRID_ARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 4;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = DEF_SPRID_HAUBERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 4;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = DEF_SPRID_LEGS_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 4;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = DEF_SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 4;

            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = DEF_SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 6 + (_tmp_cDir - 1);
            }

            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = DEF_SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 6;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = DEF_SPRID_CAPE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 4;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = DEF_SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 4;
            break;

        default:
            iUndiesIndex = -1;
            iHairIndex = -1;
            iArmArmorIndex = -1;
            iBodyArmorIndex = -1;
            iPantsIndex = -1;
            iBootsIndex = -1;
            iMantleIndex = -1;
            iHelmIndex = -1;
            break;
    }

    dx = 0;
    dy = 0;

    int64_t time_elapsed = dwTime - _tmp_owner_time;
    float cycle_progress = (float)time_elapsed / (float)_tmp_frame_time;
    cycle_progress = cycle_progress > 1.0f ? 1.0f : cycle_progress;
    float interpolated_frame = _tmp_cFrame + cycle_progress;

    switch (_tmp_cDir)
    {
        case 1: dy = int(28 - (interpolated_frame * 4)); break;
        case 2: dy = int(28 - (interpolated_frame * 4)); dx = int((interpolated_frame * 4) - 28); break;
        case 3: dx = int((interpolated_frame * 4) - 28); break;
        case 4: dx = int((interpolated_frame * 4) - 28); dy = int((interpolated_frame * 4) - 28); break;
        case 5: dy = int((interpolated_frame * 4) - 28); break;
        case 6: dy = int((interpolated_frame * 4) - 28); dx = int(28 - (interpolated_frame * 4)); break;
        case 7: dx = int(28 - (interpolated_frame * 4)); break;
        case 8: dx = int(28 - (interpolated_frame * 4)); dy = int(28 - (interpolated_frame * 4)); break;
    }

    if (is_player)
    {
        camera_frame_details.time_elapsed = time_elapsed;
        camera_frame_details.cycle_progress = cycle_progress;
        camera_frame_details.interpolated_frame = interpolated_frame;
        camera_frame_details.offset_x = dx;
        camera_frame_details.offset_y = dy;
        camera_frame_details.direction = _tmp_cDir;
    }

    if (m_bIsCrusadeMode) DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);

    CheckActiveAura(sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
    if (_tmp_iEffectType != 0)
    {
        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == false)
    {
        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->put_trans_sprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }

            }

            switch (_tmp_sOwnerType)
            {
                case 10:
                case 35:
                case 50:
                case 51:
                case 60:
                case 65:
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    }
                    break;
            }

            if (bInv == true)
                //m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }
        }
        else
        {
            switch (_tmp_sOwnerType)
            {
                case 10:
                case 35:
                case 50:
                case 51:
                case 60:
                case 65:
                    break;

                default:
                    if (m_cDetailLevel != 0)
                    {
                        if (sX < 50)
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    }
                    break;
            }

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite2(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_iStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            m_rcBodyRect = {
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom
            };

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

                switch (iShieldGlare)
                {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 1: m_pEffectSpr[45]->put_trans_sprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->put_trans_sprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->put_trans_sprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

                DKGlare(iWeaponColor, iWeaponIndex, &iWeaponGlare);

                switch (iWeaponGlare)
                {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        // Berserk 
        if ((_tmp_iStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
    }
//     else if (strlen(_tmp_cName) > 0)
//     {
//         if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
//         else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
//     }

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX))
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_iStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_iStatus);
    }

    if (_tmp_iChatIndex != 0)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != 0) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
        {
            m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX + dx;
            m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY + dy;
        }
        else
        {
            m_pMapData->ClearChatMsg(indexX, indexY);
        }
    }

    _tmp_dx = dx;
    _tmp_dy = dy;


    DisplayHPBar(_tmp_wObjectID, sX + dx, sY + dy, dwTime, _tmp_sOwnerType);
    //return m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->check_collison(sX, sY, _tmp_cFrame, msX, msY);
    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}
