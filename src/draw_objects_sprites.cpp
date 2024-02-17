//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <algorithm>
#include <iostream>
#include <fmt/format.h>
#include "sprite.h"
#include "Msg.h"
#include "MapData.h"

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
extern uint16_t  _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern uint16_t  wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int sFocusStatus;
extern int   iFocusApprColor;

bool   CGame::DrawObject_OnAttack(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iWeapon{}, iAdd{}, iShieldIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;


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
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 4;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 5;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 5;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 5;

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

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 4;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 5;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 5;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 5;

                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;


        default:
            if (_tmp_sAppr2 != 0)
            {
                iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                _tmp_cFrame = _tmp_sAppr2 - 1;
            }
            else if (_tmp_sOwnerType == 66) iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                    }
                    break;
            }


            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                    }
                    break;
            }


            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool   CGame::DrawObject_OnAttackMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iWeapon{}, iAdd{}, iShieldIndex{}, iMantleIndex{}, dx{}, dy{}, dsx{}, dsy{};
    int cFrameMoveDots{};
    bool bInv = false, bDashDraw = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;



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

    if ((_tmp_sStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = true;
        else return false;
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
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 4;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 5;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 5;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 5;

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

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 4 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 4;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (5 * 8);

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 5;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 5;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 5;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 5;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 5;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 5;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 5;

                iWeaponIndex = -1;
                iShieldIndex = -1;
            }
            break;


        default:
            iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
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
    if ((_tmp_cFrame >= 1) && (_tmp_cFrame <= 3))
    {

        switch (_tmp_cFrame)
        {
            case 1: cFrameMoveDots = 26; break;
            case 2: cFrameMoveDots = 16; break;
            case 3: cFrameMoveDots = 0;  break;
        }

        switch (_tmp_cDir)
        {
            case 1: dy = cFrameMoveDots; break;
            case 2: dy = cFrameMoveDots; dx = -cFrameMoveDots; break;
            case 3: dx = -cFrameMoveDots; break;
            case 4: dx = -cFrameMoveDots; dy = -cFrameMoveDots; break;
            case 5: dy = -cFrameMoveDots; break;
            case 6: dy = -cFrameMoveDots; dx = cFrameMoveDots; break;
            case 7: dx = cFrameMoveDots; break;
            case 8: dx = cFrameMoveDots; dy = cFrameMoveDots; break;
        }

        switch (_tmp_cFrame)
        {
            case 1: dy++;    break;
            case 2: dy += 2; break;
            case 3: dy++;    break;
        }


        switch (_tmp_cFrame)
        {
            case 2: bDashDraw = true; cFrameMoveDots = 26; break;
            case 3: bDashDraw = true; cFrameMoveDots = 16; break;
        }

        dsx = 0;
        dsy = 0;
        switch (_tmp_cDir)
        {
            case 1: dsy = cFrameMoveDots; break;
            case 2: dsy = cFrameMoveDots; dsx = -cFrameMoveDots; break;
            case 3: dsx = -cFrameMoveDots; break;
            case 4: dsx = -cFrameMoveDots; dsy = -cFrameMoveDots; break;
            case 5: dsy = -cFrameMoveDots; break;
            case 6: dsy = -cFrameMoveDots; dsx = cFrameMoveDots; break;
            case 7: dsx = cFrameMoveDots; break;
            case 8: dsx = cFrameMoveDots; dsy = cFrameMoveDots; break;
        }
    }
    else if (_tmp_cFrame > 3)
    {
        dx = 0;
        dy = 0;
    }
    else
    {
        switch (_tmp_cDir)
        {
            case 1: dy = 32; break;
            case 2: dy = 32; dx = -32; break;
            case 3: dx = -32; break;
            case 4: dx = -32; dy = -32; break;
            case 5: dy = -32; break;
            case 6: dy = -32; dx = 32; break;
            case 7: dx = 32; break;
            case 8: dx = 32; dy = 32; break;
        }
    }

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

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);


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
                         m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX+dx, sY+dy, _tmp_cFrame, dwTime);
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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    }
                    break;
            }

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
                         m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX+dx, sY+dy, _tmp_cFrame, dwTime);
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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    }
                    break;
            }

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->put_sprite_color(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, -5, 0, 5, dwTime);

        if (bDashDraw == true)
        {
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            if (iWeaponIndex != -1) m_pSprite[iWeaponIndex]->put_trans_sprite_color(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            if (iShieldIndex != -1) m_pSprite[iShieldIndex]->put_trans_sprite_color(sX + dsx, sY + dsy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
        }
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool   CGame::DrawObject_OnMagic(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iR{}, iG{}, iB{}, iHelmIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;


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


    if ((_tmp_sStatus & 0x10) != 0)
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
            iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 8;
            iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 8;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 8;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 8;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 8;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 8;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 8;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 8;
            break;

        case 4:
        case 5:
        case 6:

            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (8 * 8);

            iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 8;
            iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 8;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 8;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 8;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 8;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 8;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 8;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 8;
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
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
        }

        if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
        else
        {
            if ((_tmp_sStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
        }

        SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool   CGame::DrawObject_OnGetItem(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iR{}, iG{}, iB{}, iHelmIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;


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

    if ((_tmp_sStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = true;
        else return false;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (9 * 8);
            iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 9;
            iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 9;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 9;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 9;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 9;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 9;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 9;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 9;
            break;

        case 4:
        case 5:
        case 6:

            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (9 * 8);

            iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 9;
            iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 9;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 9;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 9;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 9;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 9;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 9;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 9;
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
                 m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
        }

        if (bInv == true)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite2(sX, sY, _tmp_cFrame, dwTime);
        else
        {
            if ((_tmp_sStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
        }

        SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

        if (iUndiesIndex != -1)
        {
            if (bInv) m_pSprite[iUndiesIndex]->put_trans_sprite2(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
        }

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool   CGame::DrawObject_OnDamage(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iWeaponIndex{}, iShieldIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iAdd{}, iDrawMode{}, iMantleIndex{};
    char cFrame{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;


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

    if ((_tmp_sStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = true;
        else return false;
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
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * iAdd + (_tmp_cDir - 1);
                }
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;

                iDrawMode = 0;
            }
            else
            {
                cFrame -= 4;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 10;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 10;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 10;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 10;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 5;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 10;

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
                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * iAdd + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;

                iDrawMode = 0;
            }
            else
            {
                cFrame -= 4;

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 10;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 10;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 10;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 10;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 5;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
                }
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 10;

                iDrawMode = 1;
            }
            break;

        default:
            if (cFrame < 4)
            {
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            }
            else
            {
                cFrame -= 4;
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
                else iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);
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
        if (iDrawMode == 1)
        {

            if (_cDrawingOrder[_tmp_cDir] == 1)
            {

                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->put_sprite_fast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->put_sprite_color(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);


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
                             m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
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


                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

                if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_sStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
                }

                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                            //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
                             m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
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


                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

                if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_sStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
                }

                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                            //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
            if ((_tmp_sStatus & 0x20) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, cFrame, 0, -5, -5, dwTime);
            // Protection From Magic
            if ((_tmp_sStatus & 0x80) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, cFrame, -5, 0, 5, dwTime);
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
                             m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
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
                                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                        }
                        break;
                }

                if (bInv == true)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_sStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
                }

                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                            //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
                             m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
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
                    if ((_tmp_sStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
                }

                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->put_sprite_color(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                            //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
            if ((_tmp_sStatus & 0x20) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, cFrame, 0, -5, -5, dwTime);
            // Protection From Magic
            if ((_tmp_sStatus & 0x80) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, cFrame, -5, 0, 5, dwTime);
        }
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnDying(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iR{}, iG{}, iB{}, iHelmIndex{}, iMantleIndex{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;
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
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 0;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 0;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 0;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 0;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 0;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 0;
                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 0;
            }
            else
            {
                cFrame -= 6;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 11;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 11;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 11;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 11;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;
                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 11;
            }
            break;

        case 4:
        case 5:
        case 6:

            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            if (cFrame < 6)
            {
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 0;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 0;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 0;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 0;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 0;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 0;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 0;
            }
            else
            {
                cFrame -= 6;
                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 11;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 11;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 11;
                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 11;
                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 11;
            }
            break;

        default:
            if (cFrame < 4)
            {
                if (_tmp_sAppr2 != 0)
                {
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
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
                    case 36:
                    case 37:
                    case 39:
                    case 38:
                    case 40:
                    case 41:
                    case 42:

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
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    cFrame = _tmp_sAppr2 - 1;
                }
                else if (_tmp_sOwnerType == 66) iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                else iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
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
                     m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
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

        if (_tmp_sOwnerType == 66) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, cFrame, dwTime);
        else
        {
            if ((_tmp_sStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, cFrame, dwTime);
        }

        SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool   CGame::DrawObject_OnDead(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iR{}, iG{}, iB{}, iFrame{}, iMantleIndex{}, iHelmIndex{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 66) return false;

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
            iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 11;
            iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 11;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 11;
            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 11;
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 11;
            break;

        case 4:
        case 5:
        case 6:

            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iFrame = 7;
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (11 * 8);
            iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 11;
            iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 11;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 11;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 11;
            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 11;
            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 11;
            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 11;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 11;
            break;


        default:
            switch (_tmp_sOwnerType)
            {
                case 28:
                case 29:
                case 30:
                case 31:
                    iFrame = 5;
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

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
                case 64:
                case 65:
                    iFrame = 7;
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 63:
                    iFrame = 5;
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 66:
                    iFrame = 15;
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (2 * 8);
                    break;

                case 51:
                    iFrame = 0;
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                case 52:
                    iFrame = 11;
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                    break;

                default:
                    iFrame = 3;
                    iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
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

            if ((_tmp_sStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, iFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, iFrame, dwTime);

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->put_sprite_fast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
            if ((_tmp_sStatus & 0x20) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, iFrame, -2 * _tmp_cFrame + 5, -2 * _tmp_cFrame - 5, -2 * _tmp_cFrame - 5, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, iFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, dwTime);
        }
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool   CGame::DrawObject_OnMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int dx{}, dy{};
    int iBodyIndex{}, iHairIndex{}, iUndiesIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iShieldIndex{}, iAdd{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;


    //if(_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = true; //Energy-Ball,Wyvern
    if (_tmp_sOwnerType == 35) bInv = true; //Energy-Ball,Wyvern
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

    if ((_tmp_sStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = true;
        else return false;
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
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 3;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 2;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 2;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
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

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 3;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 2;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 2;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
            }
            break;

        default:
            if (_tmp_sOwnerType == 66) iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
            else iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
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

    switch (_tmp_cDir)
    {
        case 1: dy = 28 - (_tmp_cFrame << 2); break;
        case 2: dy = 28 - (_tmp_cFrame << 2); dx = (_tmp_cFrame << 2) - 28; break;
        case 3: dx = (_tmp_cFrame << 2) - 28; break;
        case 4: dx = (_tmp_cFrame << 2) - 28; dy = (_tmp_cFrame << 2) - 28; break;
        case 5: dy = (_tmp_cFrame << 2) - 28; break;
        case 6: dy = (_tmp_cFrame << 2) - 28; dx = 28 - (_tmp_cFrame << 2); break;
        case 7: dx = 28 - (_tmp_cFrame << 2); break;
        case 8: dx = 28 - (_tmp_cFrame << 2); dy = 28 - (_tmp_cFrame << 2); break;
    }


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
            _tmp_cFrame = _tmp_cFrame / 2;
            break;
    }

    if (m_bIsCrusadeMode) DrawObjectFOE(sX + dx, sY + dy, _tmp_cFrame);


    if (_tmp_iEffectType != 0)
    {

        switch (_tmp_iEffectType)
        {
            case 1: m_pEffectSpr[26]->put_trans_sprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
            case 2: m_pEffectSpr[27]->put_trans_sprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (_tmp_sOwnerType == 65)
    {
        m_pEffectSpr[77]->put_trans_sprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime);
        /*
        switch( rand()%3 )
        {
        case 0:
            m_pEffectSpr[76]->PutTransSprite70(sX+dx, sY+dy, _tmp_cFrame, dwTime);
            break;
        case 1:
            m_pEffectSpr[77]->PutTransSprite70(sX+dx, sY+dy, _tmp_cFrame, dwTime);
            break;
        case 2:
            m_pEffectSpr[78]->PutTransSprite70(sX+dx, sY+dy, _tmp_cFrame, dwTime);
            break;
        }
        */
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
                case 53:
                case 60:
                case 65:
                case 66:
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

            if (bInv == true)
                //m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


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
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
                case 10:
                case 35:
                case 50:
                case 51:
                case 53:
                case 60:
                case 65:
                case 66:
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

            if (bInv == true)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


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
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnDamageMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int cFrame{}, cDir{};
    int dx{}, dy{};
    int iBodyIndex{}, iHairIndex{}, iUndiesIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iShieldIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;


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

    if ((_tmp_sStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = true;
        else return false;
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
            iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 10;
            iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 10;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 10;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 10;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;

            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 5;

            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
            }

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 10;
            break;

        case 4:
        case 5:
        case 6:

            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (10 * 8);

            iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 10;
            iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 10;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 10;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 10;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 10;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 10;

            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 5;

            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 5 + (_tmp_cDir - 1);
            }

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 10;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 10;
            break;

        default:
            if (_tmp_sOwnerType == 66) iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (3 * 8);

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

    switch (_tmp_cDir)
    {
        case 1: dy = 28 - (_tmp_cFrame << 2); break;
        case 2: dy = 28 - (_tmp_cFrame << 2); dx = (_tmp_cFrame << 2) - 28; break;
        case 3: dx = (_tmp_cFrame << 2) - 28; break;
        case 4: dx = (_tmp_cFrame << 2) - 28; dy = (_tmp_cFrame << 2) - 28; break;
        case 5: dy = (_tmp_cFrame << 2) - 28; break;
        case 6: dy = (_tmp_cFrame << 2) - 28; dx = 28 - (_tmp_cFrame << 2); break;
        case 7: dx = 28 - (_tmp_cFrame << 2); break;
        case 8: dx = 28 - (_tmp_cFrame << 2); dy = 28 - (_tmp_cFrame << 2); break;
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

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->put_sprite_fast(sX + dx, sY + dy, cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->put_sprite_color(sX + dx, sY + dy, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);


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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX+dx, sY+dy, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX+dx, sY+dy, cFrame, dwTime);
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
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


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
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX+dx, sY+dy, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX+dx, sY+dy, cFrame, dwTime);
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
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

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
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool CGame::DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    short dx{}, dy{};
    int iBodyIndex{}, iHairIndex{}, iUndiesIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iShieldIndex{}, iAdd{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;


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
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else
                    {
                        iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;

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
                    iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                }

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 3;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else
                    {
                        iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;

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
                    iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 2;

                }

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else
                {
                    iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;

                }

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else
                {
                    iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;

                }

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 2;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
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

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else
                    {
                        iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;

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
                    iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;

                }

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 3;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 2;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else
                    {
                        iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;

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
                    iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 2;

                }

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 2;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
            }
            break;

        default:
            iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
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
            case 10:
            case 35:
            case 50:
            case 51:
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
        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);


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
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
        }

        if (bInv == true)
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
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

bool   CGame::DrawObject_OnStop(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int iBodyIndex{}, iUndiesIndex{}, iHairIndex{}, iBodyArmorIndex{}, iArmArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iHelmIndex{}, iR{}, iG{}, iB{};
    int iWeaponIndex{}, iShieldIndex{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;


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

    if ((_tmp_sStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = true;
        else return false;
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
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 1;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 1;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 1;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 1;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 1;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 1;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 1 + (_tmp_cDir - 1);
                }
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 1;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 1;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 1;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);
                iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 0 + (_tmp_cDir - 1);
                }
                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 0;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 0;
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

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 1;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 1;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 1;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 1;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 1;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 1;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 1 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 1;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 1;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 1;
            }
            else
            {

                iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (0 * 8);

                iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15;
                iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15;

                if ((_tmp_sAppr4 & 0x80) == 0)
                {
                    if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                        iBodyArmorIndex = -1;
                    else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15;
                }

                if ((_tmp_sAppr3 & 0x000F) == 0)
                    iArmArmorIndex = -1;
                else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15;

                if ((_tmp_sAppr3 & 0x0F00) == 0)
                    iPantsIndex = -1;
                else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15;

                if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                    iBootsIndex = -1;
                else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15;

                if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                    iWeaponIndex = -1;
                else
                {
                    iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 0 + (_tmp_cDir - 1);
                }

                if ((_tmp_sAppr2 & 0x000F) == 0)
                    iShieldIndex = -1;
                else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 0;

                if ((_tmp_sAppr4 & 0x0F00) == 0)
                    iMantleIndex = -1;
                else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15;

                if ((_tmp_sAppr3 & 0x00F0) == 0)
                    iHelmIndex = -1;
                else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 0;
            }
            break;

        default:
            if (_tmp_sAppr2 != 0)
            {
                iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (4 * 8);
                _tmp_cFrame = (_tmp_sAppr2 & 0x00FF) - 1;
            }
            else if (_tmp_sOwnerType == 66) iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
            else iBodyIndex = 1220 + (_tmp_sOwnerType - 10) * 8 * 7 + (0 * 8);
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
        case 15:
        case 19:
        case 20:
        case 24:
        case 25:
        case 26:
            m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);
            break;
    }

#ifdef _DEBUG
    //m_pEffectSpr[74]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime );
    //m_pEffectSpr[79]->PutTransSprite70_NoColorKey(sX, sY+32, _tmp_cFrame, dwTime );
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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                    }
                    break;
            }


            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (bInv) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


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
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite_clip(sX, sY, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_shadow_sprite(sX, sY, _tmp_cFrame, dwTime);
                    }
                    break;
            }


            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->put_trans_sprite(sX, sY, 1, dwTime);

            if (bInv) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX, sY, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


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
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX, sY, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
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

    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;


    return false;
}

bool   CGame::DrawObject_OnRun(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY)
{
    int dx{}, dy{};
    int iBodyIndex{}, iHairIndex{}, iUndiesIndex{}, iArmArmorIndex{}, iBodyArmorIndex{}, iPantsIndex{}, iBootsIndex{}, iWeaponIndex{}, iShieldIndex{}, iHelmIndex{}, iR{}, iG{}, iB{}, iMantleIndex{};
    bool bInv = false;
    int iWeaponGlare{}, iShieldGlare{};
    int iWeaponColor{}, iShieldColor{}, iArmorColor{}, iMantleColor{}, iArmColor{}, iPantsColor{}, iBootsColor{}, iHelmColor{};
    int iSkirtDraw = 0;


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

    if ((_tmp_sStatus & 0x10) != 0)
    {
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = true;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = true;
        else return false;
    }

    switch (_tmp_sOwnerType)
    {
        case 1:
        case 2:
        case 3:
            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (4 * 8);
            iUndiesIndex = 4580 + (_tmp_sAppr1 & 0x000F) * 15 + 4;
            iHairIndex = 4820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 4;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 5060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 4;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 5300 + (_tmp_sAppr3 & 0x000F) * 15 + 4;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 5540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 4;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 5780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 4;

            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = 6020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 6 + (_tmp_cDir - 1);
            }

            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = 9100 + (_tmp_sAppr2 & 0x000F) * 8 + 6;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 9230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 4;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 9300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 4;
            break;

        case 4:
        case 5:
        case 6:

            if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;

            iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (4 * 8);

            iUndiesIndex = 14580 + (_tmp_sAppr1 & 0x000F) * 15 + 4;
            iHairIndex = 14820 + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 4;

            if ((_tmp_sAppr4 & 0x80) == 0)
            {
                if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
                    iBodyArmorIndex = -1;
                else iBodyArmorIndex = 15060 + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 4;
            }

            if ((_tmp_sAppr3 & 0x000F) == 0)
                iArmArmorIndex = -1;
            else iArmArmorIndex = 15300 + (_tmp_sAppr3 & 0x000F) * 15 + 4;

            if ((_tmp_sAppr3 & 0x0F00) == 0)
                iPantsIndex = -1;
            else iPantsIndex = 15540 + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 4;

            if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
                iBootsIndex = -1;
            else iBootsIndex = 15780 + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 4;

            if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
                iWeaponIndex = -1;
            else
            {
                iWeaponIndex = 16020 + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 6 + (_tmp_cDir - 1);
            }

            if ((_tmp_sAppr2 & 0x000F) == 0)
                iShieldIndex = -1;
            else iShieldIndex = 19100 + (_tmp_sAppr2 & 0x000F) * 8 + 6;

            if ((_tmp_sAppr4 & 0x0F00) == 0)
                iMantleIndex = -1;
            else iMantleIndex = 19230 + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 4;

            if ((_tmp_sAppr3 & 0x00F0) == 0)
                iHelmIndex = -1;
            else iHelmIndex = 19300 + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 4;
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

    switch (_tmp_cDir)
    {
        case 1: dy = 28 - (_tmp_cFrame << 2); break;
        case 2: dy = 28 - (_tmp_cFrame << 2); dx = (_tmp_cFrame << 2) - 28; break;
        case 3: dx = (_tmp_cFrame << 2) - 28; break;
        case 4: dx = (_tmp_cFrame << 2) - 28; dy = (_tmp_cFrame << 2) - 28; break;
        case 5: dy = (_tmp_cFrame << 2) - 28; break;
        case 6: dy = (_tmp_cFrame << 2) - 28; dx = 28 - (_tmp_cFrame << 2); break;
        case 7: dx = 28 - (_tmp_cFrame << 2); break;
        case 8: dx = 28 - (_tmp_cFrame << 2); dy = 28 - (_tmp_cFrame << 2); break;
    }

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
                //m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


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
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_color(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_sprite_fast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }

            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);


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
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
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
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare

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
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->put_trans_sprite_color(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_sStatus);
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


    if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return true;

    return false;
}

void CGame::DrawObjectFOE(int ix, int iy, int iFrame)
{
    if (_iGetFOE(_tmp_sStatus) < 0)
    {
        if (iFrame <= 4) m_pEffectSpr[38]->put_trans_sprite(ix, iy, iFrame, G_dwGlobalTime);
    }
}

void CGame::DrawObjectName(short sX, short sY, char * pName, int sStatus)
{
    char cTxt[64], cTxt2[64];
    short sR, sG, sB;
    int i, iGuildIndex, iFOE, iAddY = 0;
    bool bPK, bCitizen, bAresden, bHunter;

    sY += 14;

    iFOE = _iGetFOE(sStatus);
    if (iFOE < 0)
    {
        sR = 255; sG = 0; sB = 0;
    }
    else if (iFOE == 0)
    {
        sR = 50; sG = 50; sB = 255;
    }
    else
    {
        sR = 30; sG = 200; sB = 30;
    }

    memset(cTxt, 0, sizeof(cTxt));
    memset(cTxt2, 0, sizeof(cTxt2));

    if (m_iIlusionOwnerH == 0)
    {
        if (m_bIsCrusadeMode == false) format_to_local(cTxt, "{}", pName);
        else
        {
            if (_tmp_wObjectID >= 10000) strcpy(cTxt, NPC_NAME_MERCENARY);
            else
            {
                if (iFOE == -1) format_to_local(cTxt, "{}", _tmp_wObjectID);
                else strcpy(cTxt, pName);
            }
        }
        if (m_iPartyStatus != 0)
        {
            for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
            {
                if (strcmp(m_stPartyMemberNameList[i].cName, pName) == 0)
                {
                    strcat(cTxt, BGET_NPC_NAME23);
                    break;
                }
            }
        }
    }
    else strcpy(cTxt, "?????");

    if ((sStatus & 0x20) != 0) strcat(cTxt, DRAW_OBJECT_NAME50);//" Berserk" 
    if ((sStatus & 0x40) != 0) strcat(cTxt, DRAW_OBJECT_NAME51);//" Frozen"

    put_under_entity_string(sX, sY, cTxt, Color(255, 255, 255));

    //PutString2(sX, sY, cTxt, 255, 255, 255);
    memset(cTxt, 0, sizeof(cTxt));

    if (memcmp(m_cPlayerName, pName, 10) == 0)
    {
        if (m_iGuildRank == 0)
        {
            format_to_local(G_cTxt, DEF_MSG_GUILDMASTER, m_cGuildName);
            put_under_entity_string(sX, sY + 14, G_cTxt, Color(180, 180, 180));
            //PutString2(sX, sY + 14, G_cTxt, 180, 180, 180);
            iAddY = 14;
        }
        if (m_iGuildRank > 0)
        {
            format_to_local(G_cTxt, DEF_MSG_GUILDSMAN, m_cGuildName);
            put_under_entity_string(sX, sY + 14, G_cTxt, Color(180, 180, 180));
            //PutString2(sX, sY + 14, G_cTxt, 180, 180, 180);
            iAddY = 14;
        }
        if (m_iPKCount != 0)
        {
            bPK = true;
            sR = 255; sG = 0; sB = 0;
        }
        else
        {
            bPK = false;
            sR = 30; sG = 200; sB = 30;
        }
        bCitizen = m_bCitizen;
        bAresden = m_bAresden;
        bHunter = m_bHunter;
    }
    else
    {
        if (sStatus & 0x8000) bPK = true;
        else bPK = false;
        if (sStatus & 0x4000) bCitizen = true;
        else bCitizen = false;
        if (sStatus & 0x2000) bAresden = true;
        else bAresden = false;
        if (sStatus & 0x1000) bHunter = true;
        else bHunter = false;
        if (m_bIsCrusadeMode == false || iFOE >= 0)
        {
            if (FindGuildName(pName, &iGuildIndex) == true)
            {
                if (m_stGuildName[iGuildIndex].cGuildName[0] != 0)
                {
                    if (strcmp(m_stGuildName[iGuildIndex].cGuildName, "NONE") != 0)
                    {
                        if (m_stGuildName[iGuildIndex].iGuildRank == 0)
                        {
                            format_to_local(G_cTxt, DEF_MSG_GUILDMASTER, m_stGuildName[iGuildIndex].cGuildName);
                            put_under_entity_string(sX, sY + 14, G_cTxt, Color(180, 180, 180));
                            //PutString2(sX, sY + 14, G_cTxt, 180, 180, 180);
                            m_stGuildName[iGuildIndex].dwRefTime = m_dwCurTime;
                            iAddY = 14;
                        }
                        else if (m_stGuildName[iGuildIndex].iGuildRank > 0)
                        {
                            format_to_local(G_cTxt, DEF_MSG_GUILDSMAN, m_stGuildName[iGuildIndex].cGuildName);
                            put_under_entity_string(sX, sY + 14, G_cTxt, Color(180, 180, 180));
                            //PutString2(sX, sY + 14, G_cTxt, 180, 180, 180);
                            m_stGuildName[iGuildIndex].dwRefTime = m_dwCurTime;
                            iAddY = 14;
                        }
                    }
                    else
                    {
                        m_stGuildName[iGuildIndex].dwRefTime = 0;
                    }
                }
            }
            else bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQGUILDNAME, 0, _tmp_wObjectID, iGuildIndex, 0, 0);
        }
    }

    if (bCitizen == false)	strcpy(cTxt, DRAW_OBJECT_NAME60);
    else
    {
        if (bAresden)
        {
            //#if DEF_LANGUAGE > 2		// Korea 2.19
            if (bHunter == true) strcpy(cTxt, DEF_MSG_ARECIVIL);
            else strcpy(cTxt, DEF_MSG_ARESOLDIER);
            //#else

            //#endif
        }
        else
        {
            //#if DEF_LANGUAGE > 2		// Korea 2.19
            if (bHunter == true) strcpy(cTxt, DEF_MSG_ELVCIVIL);
            else strcpy(cTxt, DEF_MSG_ELVSOLDIER);
            //#else

            //#endif
        }
    }
    if (bPK == true)
    {
        if (bCitizen == false) strcpy(cTxt, DEF_MSG_PK);
        else
        {
            if (bAresden) strcpy(cTxt, DEF_MSG_AREPK);
            else strcpy(cTxt, DEF_MSG_ELVPK);
        }
    }
    //PutString2(sX, sY + 14 + iAddY, cTxt, sR, sG, sB);
    put_under_entity_string(sX, sY + 14 + iAddY, cTxt, Color(sR, sG, sB));
}

void CGame::DrawNpcName(short sX, short sY, short sOwnerType, int sStatus)
{
    char cTxt[32], cTxt2[64];
    memset(cTxt, 0, sizeof(cTxt));
    memset(cTxt2, 0, sizeof(cTxt2));

    GetNpcName(sOwnerType, cTxt);
    if ((sStatus & 0x20) != 0) strcat(cTxt, DRAW_OBJECT_NAME50);//" Berserk" 
    if ((sStatus & 0x40) != 0) strcat(cTxt, DRAW_OBJECT_NAME51);//" Frozen"

    sY += 14;

    put_under_entity_string(sX, sY, cTxt, Color(255, 255, 255));

    //PutString2(sX, sY, cTxt, 255, 255, 255);

    if (m_bIsObserverMode == true) put_string2(sX, sY + 14, cTxt, 50, 50, 255);
    else if (m_bIsConfusion || (m_iIlusionOwnerH != 0))
    {
        memset(cTxt, 0, sizeof(cTxt));
        strcpy(cTxt, DRAW_OBJECT_NAME87);
        put_under_entity_string(sX, sY + 14, cTxt, Color(150, 150, 150));
        //PutString2(sX, sY + 14, cTxt, 150, 150, 150);
    }
    else
    {
        switch (_iGetFOE(sStatus))
        {
            case -2:
                put_under_entity_string(sX, sY + 14, DRAW_OBJECT_NAME90, Color(255, 0, 0));
                //PutString2(sX, sY + 14, DRAW_OBJECT_NAME90, 255, 0, 0);
                break;
            case -1:
                put_under_entity_string(sX, sY + 14, DRAW_OBJECT_NAME90, Color(255, 0, 0));
                //PutString2(sX, sY + 14, DRAW_OBJECT_NAME90, 255, 0, 0);
                break;
            case 0:
                put_under_entity_string(sX, sY + 14, DRAW_OBJECT_NAME88, Color(50, 50, 255));
                //PutString2(sX, sY + 14, DRAW_OBJECT_NAME88, 50, 50, 255);
                break;
            case 1:
                put_under_entity_string(sX, sY + 14, DRAW_OBJECT_NAME89, Color(30, 255, 30));
                //PutString2(sX, sY + 14, DRAW_OBJECT_NAME89, 30, 255, 30);
                break;
        }
    }

    switch ((sStatus & 0x0F00) >> 8)
    {
        case 0: break;
        case 1: strcpy(cTxt2, DRAW_OBJECT_NAME52); break;//"Clairvoyant"
        case 2: strcpy(cTxt2, DRAW_OBJECT_NAME53); break;//"Destruction of Magic Protection"
        case 3: strcpy(cTxt2, DRAW_OBJECT_NAME54); break;//"Anti-Physical Damage"
        case 4: strcpy(cTxt2, DRAW_OBJECT_NAME55); break;//"Anti-Magic Damage"
        case 5: strcpy(cTxt2, DRAW_OBJECT_NAME56); break;//"Poisonous"
        case 6: strcpy(cTxt2, DRAW_OBJECT_NAME57); break;//"Critical Poisonous" 
        case 7: strcpy(cTxt2, DRAW_OBJECT_NAME58); break;//"Explosive"  
        case 8: strcpy(cTxt2, DRAW_OBJECT_NAME59); break;//"Critical Explosive"
    }
    //     if (m_Misc.bCheckIMEString(cTxt2)) PutString_SprFont3(sX, sY + 28, cTxt2, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
    //     else PutString2(sX, sY + 28, cTxt2, 240, 240, 70);
    put_under_entity_string(sX, sY + 28, cTxt2, Color(240, 240, 70));
}
