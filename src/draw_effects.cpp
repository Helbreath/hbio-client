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
#include "effect.h"

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

void CGame::DrawEffects()
{
    int i, dX, dY, iDvalue, tX, tY, rX, rY, rX2, rY2, rX3, rY3, rX4, rY4, rX5, rY5, iErr;
    char  cTempFrame;
    int64_t dwTime = m_dwCurTime;

    for (i = 0; i < DEF_MAXEFFECTS; i++)
        if ((m_pEffectList[i] != 0) && (m_pEffectList[i]->m_cFrame >= 0))
        {

            switch (m_pEffectList[i]->m_sType)
            {
                case 1:
                    if (m_pEffectList[i]->m_cFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[8]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 2:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 2;
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[7]->put_sprite_fast(dX, dY, cTempFrame, dwTime);
                    break;

                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[8]->put_trans_sprite_no_color_key(dX, dY, 1, dwTime);
                    break;

                case 4:
                    /* 1.5
                    if (m_pEffectList[i]->m_cFrame < 9) break;
                    cTempFrame = m_pEffectList[i]->m_cFrame - 9;
                    dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
                    dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
                    m_pEffectSpr[1]->put_sprite_fast(dX, dY-40, cTempFrame, dwTime);
                    */
                    break;

                case 5:
                    // Fire Explosion
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    iDvalue = (cTempFrame - 8) * (-5);

                    if (cTempFrame < 7)
                        m_pEffectSpr[3]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    else m_pEffectSpr[3]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);
                    break;

                case 6:	 // Energy Bolt 
                case 10: // Lightning Arrow
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    iDvalue = (cTempFrame - 7) * (-6);

                    if (cTempFrame < 6)
                        m_pEffectSpr[6]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    else m_pEffectSpr[6]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 7:
                    // Magic Missile Explosion
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    iDvalue = (cTempFrame - 4) * (-3);

                    if (cTempFrame < 4)
                        m_pEffectSpr[6]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    else m_pEffectSpr[6]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 8:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    cTempFrame = 4 - cTempFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 9:
                    cTempFrame = (rand() % 5);
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 11:
                    cTempFrame = (rand() % 5) + 5;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite2(dX, dY, cTempFrame, dwTime);
                    break;

                case 12:
                    cTempFrame = (rand() % 6) + 10;
                    if (cTempFrame < 0) break;

                    iDvalue = (m_pEffectList[i]->m_cFrame - 4) * (-3);

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if (cTempFrame < 4)
                        m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    else m_pEffectSpr[11]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    //m_pEffectSpr[11]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                    break;

                case 13:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if (cTempFrame < 13)
                    {
                        m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, 25 + (cTempFrame / 5), dwTime);
                    }
                    else
                    {

                        m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, (8 + cTempFrame), dwTime);
                    }
                    break;

                case 14:
                    if (m_pEffectList[i]->m_cFrame < 0) break;

                    dX = m_pEffectList[i]->m_mX - m_sViewPointX;
                    dY = m_pEffectList[i]->m_mY - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite50_no_color_key(dX, dY, (28 + m_pEffectList[i]->m_cFrame), dwTime);
                    break;

                case 15:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = m_pEffectList[i]->m_mX - m_sViewPointX;
                    dY = m_pEffectList[i]->m_mY - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite50_no_color_key(dX, dY, (33 + cTempFrame), dwTime);
                    break;

                case 16:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY, 0, dwTime);
                    break;

                case 17:
                    //test
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    cTempFrame = 39 + (rand() % 3) * 3 + (rand() % 3);
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);

                    dX = (m_pEffectList[i]->m_mX2) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY2) - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite50_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 18:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[18]->put_trans_sprite70_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 30:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[14]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 31:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[15]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 32:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame + 20;
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 33:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    iDvalue = 0;
                    m_pEffectSpr[19]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 34: //Change \/ 35/36
                    break;

                case 35:
                    if (m_pEffectList[i]->m_cFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    m_pEffectSpr[6]->put_trans_sprite_no_color_key(dX - 30, dY - 18, cTempFrame, dwTime);
                    break;

                case 36:
                    if (m_pEffectList[i]->m_cFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    m_pEffectSpr[97]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 40:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[20]->put_trans_sprite50_no_color_key(dX, dY, cTempFrame, dwTime); // 20
                    break;

                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:
                    dX = (m_pEffectList[i]->m_sX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_sY) - m_sViewPointY;
                    m_pEffectSpr[21]->put_fade_sprite(dX, dY, 48, dwTime);

                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if ((8 * (m_pEffectList[i]->m_sType - 41) + cTempFrame) < (8 * (m_pEffectList[i]->m_sType - 41) + 7))
                    {
                        iDvalue = -8 * (6 - cTempFrame);
                        m_pEffectSpr[21]->put_trans_sprite_color(dX, dY, 8 * (m_pEffectList[i]->m_sType - 41) + cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    }
                    else
                    {
                        if ((cTempFrame - 5) >= 8) cTempFrame = ((cTempFrame - 5) - 8) + 5;
                        m_pEffectSpr[21]->put_sprite_fast(dX, dY, 8 * (m_pEffectList[i]->m_sType - 41) + (cTempFrame - 5), dwTime);
                    }
                    break;
                case 47:
                case 48:
                case 49:

                    dX = (m_pEffectList[i]->m_sX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_sY) - m_sViewPointY;
                    m_pEffectSpr[m_pEffectList[i]->m_sType - 1]->put_reverse_trans_sprite(dX, dY, 0, dwTime);

                    cTempFrame = m_pEffectList[i]->m_cFrame;

                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    //put_string(dX, dY, "*", Color(255,255,255));
                    if (cTempFrame < 7)
                    {
                        iDvalue = -8 * (6 - cTempFrame);
                        m_pEffectSpr[m_pEffectList[i]->m_sType - 1]->put_trans_sprite_color(dX, dY, cTempFrame + 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    else
                    {
                        if (cTempFrame >= 8) cTempFrame = cTempFrame % 8;
                        m_pEffectSpr[m_pEffectList[i]->m_sType - 1]->put_sprite_fast(dX, dY, cTempFrame + 1, dwTime);
                    }
                    break;

                case 50:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if (cTempFrame <= 6)
                    {
                        iDvalue = 0;
                        m_pEffectSpr[22]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);	// RGB2			
                    }
                    else
                    {
                        iDvalue = -5 * (cTempFrame - 6);
                        m_pEffectSpr[22]->put_trans_sprite_color(dX, dY, 6, iDvalue, iDvalue, iDvalue, dwTime); // RGB2				
                    }
                    break;

                case 51:
                    cTempFrame = m_pEffectList[i]->m_cFrame + 11; //15
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[28]->put_trans_sprite25(dX, dY, cTempFrame, dwTime); //20
                    break;

                case 52:
                    break;
                    /*
                    case 52: // Protection Ring
                        cTempFrame = m_pEffectList[i]->m_cFrame;
                        dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
                        dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
                        m_pEffectSpr[24]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                        break;
                    */

                case 53: // Hold Twist
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    if (cTempFrame < 0) cTempFrame = 0;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[25]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime); //25
                    break;

                case 54:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) cTempFrame = 0;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[28]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                    break;

                case 55:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) cTempFrame = 0;
                    dX = (m_pEffectList[i]->m_mX);
                    dY = (m_pEffectList[i]->m_mY);
                    m_pEffectSpr[28]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 56:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) cTempFrame = 0;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[29]->put_trans_sprite50_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 181:
                case 60:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    if (cTempFrame > 4)
                    {
                        cTempFrame = cTempFrame / 4;
                    }

                    if (cTempFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                        m_pEffectSpr[31]->put_sprite_fast(dX, dY, 15 + cTempFrame, dwTime);
                        m_pEffectSpr[31]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                    }
                    break;

                case 61:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[32]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 62:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    if (cTempFrame > 0)
                    {
                        cTempFrame = cTempFrame - 1;
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                        m_pEffectSpr[31]->put_reverse_trans_sprite(dX, dY, 20 + cTempFrame, dwTime, cTempFrame / 3);
                    }
                    break;

                case 63:

                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[33]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 64:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[34]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 65:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    cTempFrame = cTempFrame / 6;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[31]->put_reverse_trans_sprite(dX, dY, 20 + cTempFrame, dwTime, cTempFrame >> 2);
                    break;

                case 66:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[39]->put_reverse_trans_sprite(dX, dY, cTempFrame, dwTime);
                    m_pEffectSpr[39]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 67:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    switch (rand() % 3)
                    {
                        case 0: m_pEffectSpr[0]->put_trans_sprite25_no_color_key(dX, dY + 20, 1, dwTime); break;
                        case 1: m_pEffectSpr[0]->put_trans_sprite50_no_color_key(dX, dY + 20, 1, dwTime); break;
                        case 2: m_pEffectSpr[0]->put_trans_sprite70_no_color_key(dX, dY + 20, 1, dwTime); break;
                    }

                    m_pEffectSpr[35]->put_trans_sprite70_no_color_key(dX, dY, cTempFrame / 3, dwTime);
                    break;

                case 68:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    if (cTempFrame <= 11)
                    {
                        m_pEffectSpr[40]->put_sprite_fast(dX, dY, cTempFrame, dwTime);
                        m_pEffectSpr[41]->put_trans_sprite50_no_color_key(dX, dY, cTempFrame, dwTime);
                        m_pEffectSpr[44]->put_reverse_trans_sprite(dX - 2, dY - 3, cTempFrame, dwTime);
                        m_pEffectSpr[44]->put_trans_sprite_no_color_key(dX - 4, dY - 3, cTempFrame, dwTime);
                    }
                    else
                    {
                        switch (cTempFrame)
                        {
                            case 12:
                            case 13:
                            case 14: m_pEffectSpr[40]->put_sprite_fast(dX, dY, 11, dwTime); break;
                            case 15: m_pEffectSpr[40]->put_trans_sprite70_no_color_key(dX, dY, 11, dwTime); break;
                            case 16: m_pEffectSpr[40]->put_trans_sprite50_no_color_key(dX, dY, 11, dwTime); break;
                            case 17: m_pEffectSpr[40]->put_trans_sprite25_no_color_key(dX, dY, 11, dwTime); break;
                        }
                    }
                    break;

                case 69:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[42]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 70:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[43]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;
                case 72:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if (cTempFrame <= 8)
                    {
                        iDvalue = 0;
                        m_pEffectSpr[51]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    else
                    {
                        iDvalue = -1 * (cTempFrame - 8);
                        m_pEffectSpr[51]->put_trans_sprite_color(dX, dY, 8, iDvalue, iDvalue, iDvalue, dwTime);	// RGB2			
                    }
                    break;

                case 73: // absent v220 et v351
                case 74: // absent v220 et v351
                case 75: // absent v220 et v351
                case 76: // absent v220 et v351
                case 77: // absent v220 et v351
                    break;

                case 80://Change
                case 196: // Earth-Shock-Wave
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[91]->put_sprite_fast(dX, dY, cTempFrame, dwTime);
                    m_pEffectSpr[92]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                    break;

                case 81: //Storm Bringer
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    m_pEffectSpr[100]->put_trans_sprite_no_color_key(dX + 70, dY + 70, cTempFrame, dwTime);
                    break;

                case 82: // Gate (apocalypse)
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    // todo - check if 320, 480 is correct
                    m_pEffectSpr[101]->put_trans_sprite_no_color_key(320, 480, cTempFrame, dwTime);
                    break;

                    //		case 90: //Change Earth Shock Wave
                    //			cTempFrame = m_pEffectList[i]->m_cFrame;
                    //			if (cTempFrame < 0) break;
                    //			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
                    //			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
                    //
                    //			m_pEffectSpr[91]->put_sprite_fast(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, cTempFrame, dwTime);
                    //			m_pEffectSpr[92]->put_trans_sprite70(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, cTempFrame, dwTime);
                    ////			m_pEffectSpr[91]->put_sprite_fast(dX, dY, cTempFrame, dwTime);
                    ////			m_pEffectSpr[92]->put_trans_sprite70(dX, dY, cTempFrame, dwTime);
                    //			break;

                case 100: // Magic Missile
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY, 0, dwTime);
                    break;

                case 110: // Energy-Bolt
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY, 2 + (rand() % 4), dwTime);
                    break;
                case 102: // Create Food

                case 124: // Protection from N.M
                case 125: // Hold-Person
                case 126: // Possession
                case 167: // Scan
                case 127: // Poison
                case 133: // Protect-From-Magic
                case 134: // Detect-Invisibility
                case 135: // Paralyze
                case 136: // Cure
                case 142: // Confuse Language
                case 152:
                case 153: // Mass-Poison
                case 162: // Confusion
                case 171: // Mass-Confusion
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);

                    if (cTempFrame < 5)
                        m_pEffectSpr[4]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[4]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 176: //Change Cancellation   
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[90]->put_trans_sprite_no_color_key(dX + 50, dY + 85, cTempFrame, dwTime);
                    break;

                case 177: // Illusion-Movement 
                case 180: // Illusion
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-3);

                    if (cTempFrame < 9)	m_pEffectSpr[60]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[60]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 182: //Mass-Magic-Missile 
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[98]->put_trans_sprite(dX, dY, cTempFrame, dwTime, 0);
                    break;

                case 183: // Inhibition-Casting
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    iDvalue = (cTempFrame - 5) * (-3);
                    if (cTempFrame < 9) m_pEffectSpr[94]->put_trans_sprite_no_color_key(dX, dY + 40, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[94]->put_trans_sprite_color(dX, dY + 40, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime);
                    break;

                case 244: // Mass MagicMissile
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = m_pEffectList[i]->m_mX - m_sViewPointX;
                    dY = m_pEffectList[i]->m_mY - m_sViewPointY;
                    m_pEffectSpr[96]->put_trans_sprite(dX, dY, m_pEffectList[i]->m_cFrame, dwTime, 0);
                    break;

                case 190: // Mass-Illusion
                case 195: // Mass-Illusion-Movement
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-3);

                    if (cTempFrame < 9) m_pEffectSpr[61]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[61]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                    //case 192: // Mage Hero set effect
                case 242:
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[87]->put_trans_sprite_no_color_key(dX + 50, dY + 57, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                    //case 193: // War Hero set effect	
                case 243:
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[88]->put_trans_sprite_no_color_key(dX + 65, dY + 80, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 194: // Resurrection
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[99]->put_trans_sprite(dX, dY, m_pEffectList[i]->m_cFrame, dwTime, 0);
                    break;

                case 200: // shotstar fall on ground
                    dX = m_pEffectList[i]->m_mX;
                    dY = m_pEffectList[i]->m_mY;
                    m_pEffectSpr[133]->put_trans_sprite_no_color_key(dX, dY, (rand() % 15), dwTime);
                    break;

                case 201: // shotstar fall on ground
                    dX = m_pEffectList[i]->m_mX;
                    dY = m_pEffectList[i]->m_mY;
                    m_pEffectSpr[134]->put_trans_sprite_no_color_key(dX, dY, (rand() % 15), dwTime);
                    break;

                case 202: // shotstar fall on ground
                    dX = m_pEffectList[i]->m_mX;
                    dY = m_pEffectList[i]->m_mY;
                    m_pEffectSpr[135]->put_trans_sprite_no_color_key(dX, dY, (rand() % 15), dwTime);
                    break;

                case 203: // explosion feu apoc
                    dX = m_pEffectList[i]->m_mX;
                    dY = m_pEffectList[i]->m_mY;
                    m_pEffectSpr[136]->put_trans_sprite_no_color_key(dX, dY, (rand() % 18), dwTime);
                    break;

                case 204: // Faille oblique
                    dX = m_pEffectList[i]->m_mX;
                    dY = m_pEffectList[i]->m_mY;
                    m_pEffectSpr[137]->put_trans_sprite_no_color_key(dX, dY, (rand() % 12), dwTime);
                    break;

                case 205: // Faille horizontale
                    dX = m_pEffectList[i]->m_mX;
                    dY = m_pEffectList[i]->m_mY;
                    m_pEffectSpr[138]->put_trans_sprite_no_color_key(dX, dY, (rand() % 12), dwTime);
                    break;

                case 206: // steams
                    dX = m_pEffectList[i]->m_mX;
                    dY = m_pEffectList[i]->m_mY;
                    m_pEffectSpr[139]->put_trans_sprite_no_color_key(dX, dY, (rand() % 20), dwTime);
                    break;

                case 250: // Gate (round one)
                    dX = m_pEffectList[i]->m_mX - m_sViewPointX;
                    dY = m_pEffectList[i]->m_mY - m_sViewPointY;
                    m_pEffectSpr[103]->put_trans_sprite_no_color_key(dX, dY, (rand() % 3), dwTime);
                    break;

                case 252: // burst (lisgt salmon color)
                    dX = m_pEffectList[i]->m_mX - m_sViewPointX;
                    dY = m_pEffectList[i]->m_mY - m_sViewPointY;
                    m_pEffectSpr[104]->put_trans_sprite_no_color_key(dX, dY, (rand() % 3), dwTime);
                    break;


                case 113: // Defense Shield
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);

                    if (cTempFrame < 6)
                        m_pEffectSpr[62]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[62]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 144: // Great-Defense-Shield
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);

                    if (cTempFrame < 9)
                        m_pEffectSpr[63]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[63]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;
                case 111:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);
                    m_pEffectSpr[49]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 101: // Heal
                case 178: // Super Heal
                case 121:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);
                    m_pEffectSpr[50]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 123: // Stamina-Recovery
                case 128:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);
                    m_pEffectSpr[56]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 132: // Invisibility
                case 112: // Recall
                case 131: // Summon-Creature
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);
                    m_pEffectSpr[52]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;


                case 165: // Absolute-Magic-Protect
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[53]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;


                case 166: // Armor-Break
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[55]->put_reverse_trans_sprite(dX, dY + 35, m_pEffectList[i]->m_cFrame, dwTime);
                    m_pEffectSpr[54]->put_trans_sprite50(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 120: // Fire Ball
                case 130: // Fire Strike
                case 161: // Mass-Fire-Strike
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[5]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 137: // Lightning Arrow

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    tX = (m_pEffectList[i]->m_mX2) - m_sViewPointX;
                    tY = (m_pEffectList[i]->m_mY2) - m_sViewPointY;

                    iErr = 0;
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX, &rY, &iErr, 15);
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX2, &rY2, &iErr, 30);
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX3, &rY3, &iErr, 45);
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX4, &rY4, &iErr, 60);
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX5, &rY5, &iErr, 75);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite25_no_color_key(rX5, rY5, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite25_no_color_key(rX4, rY4, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite50_no_color_key(rX3, rY3, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite50_no_color_key(rX2, rY2, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite70_no_color_key(rX, rY, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                    break;

                case 143: // Lightning
                    _DrawThunderEffect(m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY - 800,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX, m_pEffectList[i]->m_rY, 1);

                    _DrawThunderEffect(m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY - 800,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX + 4, m_pEffectList[i]->m_rY + 2, 2);

                    _DrawThunderEffect(m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY - 800,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX - 2, m_pEffectList[i]->m_rY - 2, 2);
                    break;

                case 151: // Lightning Bolt
                    _DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX, m_pEffectList[i]->m_rY, 1);

                    _DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX + 2, m_pEffectList[i]->m_rY - 2, 2);

                    _DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX - 2, m_pEffectList[i]->m_rY - 2, 2);
                    break;
            }
        }
}



void CGame::DrawEffectLights()
{
    int i, dX, dY, iDvalue;
    int64_t dwTime = m_dwCurTime;
    char  cTempFrame;
    for (i = 0; i < DEF_MAXEFFECTS; i++)
        if (m_pEffectList[i] != 0)
        {
            switch (m_pEffectList[i]->m_sType)
            {
                case 1:
                    break;

                case 2:
                    break;

                case 4:
                    break;

                case 5:
                    // Fire Explosion
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 7) * (-1);

                        if (m_pEffectList[i]->m_cFrame < 6)
                            m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY + 30, 1, dwTime);
                        else m_pEffectSpr[0]->put_trans_sprite_color(dX, dY + 30, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    break;

                case 7:
                    // Magic Missile Explosion
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 2) * (-1);

                        if (m_pEffectList[i]->m_cFrame < 2)
                            m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY + 30, 1, dwTime);
                        else m_pEffectSpr[0]->put_trans_sprite_color(dX, dY + 30, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    break;

                case 69:
                case 70:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[0]->put_trans_sprite25(dX, dY + 30, 1, dwTime);
                    break;

                case 16:
                case 61:
                case 66:
                case 100:
                case 110:
                case 120:
                case 130:
                case 137:
                case 165:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    iDvalue = -5;
                    m_pEffectSpr[0]->put_trans_sprite_color(dX, dY + 30, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    break;

                case 6:	 // Energy Bolt
                case 10: // Lightning Arrow
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 9) * (-1);

                        if (m_pEffectList[i]->m_cFrame < 8)
                            m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY + 30, 1, dwTime);
                        else m_pEffectSpr[0]->put_trans_sprite_color(dX, dY + 30, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    break;

                case 33:
                    /*
                    if (m_pEffectList[i]->m_cFrame >= 0) {
                        dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
                        dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 9)*(-1);

                        if (m_pEffectList[i]->m_cFrame < 8)
                             m_pEffectSpr[0]->put_trans_sprite(dX, dY, 1, dwTime);
                        else m_pEffectSpr[0]->put_trans_sprite_color(dX, dY, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    */
                    break;

                case 40:
                case 56:
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 7) * (-1);

                        if (m_pEffectList[i]->m_cFrame < 6)
                            m_pEffectSpr[0]->put_trans_sprite(dX, dY, 1, dwTime);
                        else m_pEffectSpr[0]->put_trans_sprite_color(dX, dY, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    break;

                case 52: // Protection Ring
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                        m_pEffectSpr[24]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    }
                    break;

                case 57:
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                        m_pEffectSpr[30]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    }
                    break;

                case 73:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[74]->put_trans_sprite(dX, dY - 34, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 74:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[75]->put_trans_sprite(dX, dY + 35, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 75:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[76]->put_trans_sprite25(dX + m_pEffectList[i]->m_dX * m_pEffectList[i]->m_cFrame, dY + m_pEffectList[i]->m_dY * m_pEffectList[i]->m_cFrame, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 76:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[77]->put_trans_sprite25(dX + m_pEffectList[i]->m_dX * m_pEffectList[i]->m_cFrame, dY + m_pEffectList[i]->m_dY * m_pEffectList[i]->m_cFrame, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 77:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[78]->put_trans_sprite25(dX + m_pEffectList[i]->m_dX * m_pEffectList[i]->m_cFrame, dY + m_pEffectList[i]->m_dY * m_pEffectList[i]->m_cFrame, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 150: // Berserk : Circle 6 magic 
                case 184: // Ultra Berserk
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[58]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 180:
                case 190:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[59]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    //if( cTempFrame < 9 ) m_pEffectSpr[59]->put_trans_sprite_no_color_key( dX, dY, cTempFrame, dwTime );
                    //else m_pEffectSpr[59]->put_sprite_fast( dX, dY, cTempFrame, dwTime );
                    break;

                case 177: // Illusion mvt
                case 195: // Mass Illusion mvt
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[102]->put_trans_sprite_no_color_key(dX, dY + 30, cTempFrame, dwTime);
                    break;

                case 183: // Inhibition casting
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[95]->put_trans_sprite_no_color_key(dX, dY + 40, cTempFrame, dwTime);
                    break;
            }
        }
}
