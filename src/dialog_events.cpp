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


void CGame::DlbBoxDoubleClick_Character(short msX, short msY)
{
    char cEquipPosStatus[DEF_MAXITEMEQUIPPOS], cItemID = -1;
    short sX, sY, sSprH, sFrame;
    int i;
    if (m_bIsDialogEnabled[17] == TRUE) return;
    sX = m_stDialogBoxInfo[1].sX;
    sY = m_stDialogBoxInfo[1].sY;

    for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
        cEquipPosStatus[i] = -1;

    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if ((m_pItemList[i] != NULL) && (m_bIsItemEquipped[i] == TRUE))	cEquipPosStatus[m_pItemList[i]->m_cEquipPos] = i;
    }
    
    if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
    {
        if (cEquipPosStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 41, sY + 137, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_BACK];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_PANTS];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_ARMS];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_BOOTS];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_BODY];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_FULLBODY];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 90, sY + 170, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_LHAND];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_RHAND];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_TWOHAND];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 35, sY + 120, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_NECK];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 32, sY + 193, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_RFINGER];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 72, sY + 135, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_HEAD];
        }
    }
    else if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
    {
        if (cEquipPosStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 45, sY + 143, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_BACK];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_BOOTS];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_PANTS];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_ARMS];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_BOOTS];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_BODY];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_FULLBODY];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 84, sY + 175, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_LHAND];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_RHAND];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_TWOHAND];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 35, sY + 120, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_NECK];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 32, sY + 193, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_RFINGER];
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 72, sY + 139, sFrame, msX, msY))
                cItemID = cEquipPosStatus[DEF_EQUIPPOS_HEAD];
        }
    }

    if (cItemID == -1 || m_pItemList[cItemID] == NULL) return;
    if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EAT) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW) || (m_pItemList[cItemID]->m_dwCount > 1)) return;
    
    
    if ((m_bIsDialogEnabled[11] == TRUE) && (m_bIsDialogEnabled[23] == FALSE) && (m_stDialogBoxInfo[39].sV3 == 24))
        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_REPAIRITEM, NULL, cItemID, m_stDialogBoxInfo[39].sV3, NULL, m_pItemList[cItemID]->m_cName, m_stDialogBoxInfo[39].sV4);
    else
    {
        if (m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] == TRUE)
        {
            char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
            GetItemName(m_pItemList[m_stMCursor.sSelectedObjectID], cStr1, cStr2, cStr3, 64);
            ZeroMemory(G_cTxt, sizeof(G_cTxt));
            format_to_local(G_cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
            AddEventList(G_cTxt, 10);

            PlaySound('E', 29, 0);

            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_RELEASEITEM, NULL, m_stMCursor.sSelectedObjectID, NULL, NULL, NULL);
            
            m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] = FALSE;
            m_sItemEquipmentStatus[m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos] = -1;
            m_stMCursor.cSelectedObjectType = NULL;
            m_stMCursor.sSelectedObjectID = NULL;
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
    if (sX > 640 - 128 - 20) sX = 640 - 128;
    if (sY > 427 - 128 - 20) sY = 427 - 128;
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
    if ((m_bRunningMode == TRUE) && (m_iSP > 0))
        m_cCommand = DEF_OBJECTRUN;
    else m_cCommand = DEF_OBJECTMOVE;
    m_sCommX = shX;
    m_sCommY = shY;
    GetPlayerTurn();
}

void CGame::DlbBoxDoubleClick_Inventory(short msX, short msY)
{
    int i;
    char  cItemID, cTxt[120]{};
    short sX, sY, x1, x2, y1, y2;
    char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};



    
    if (m_iHP <= 0) return;
    
    if (m_bItemUsingStatus == TRUE)
    {
        AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY1, 10);
        return;
    }

    sX = m_stDialogBoxInfo[2].sX;
    sY = m_stDialogBoxInfo[2].sY;

    

#if DEF_LANGUAGE == 3
    int iConsumeNum; 
    if (m_bIsDialogEnabled[26] == TRUE)
    {
        
        for (i = 0; i < DEF_MAXITEMS; i++)
        {
            if (m_cItemOrder[DEF_MAXITEMS - 1 - i] == -1) continue;
            cItemID = m_cItemOrder[DEF_MAXITEMS - 1 - i];
            if (m_pItemList[cItemID] == NULL) continue;

            m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + 32 + m_pItemList[cItemID]->m_sX,
                sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame);
            
            x1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
            y1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
            x2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
            y2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

            if ((m_bIsItemDisabled[cItemID] == FALSE) && (m_bIsItemEquipped[cItemID] == FALSE) && (msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
            {

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
                    if ((m_pItemList[cItemID]->m_cItemType != DEF_ITEMTYPE_EAT) &&
                        (m_pItemList[cItemID]->m_cItemType != DEF_ITEMTYPE_CONSUME) &&
                        (m_pItemList[cItemID]->m_cItemType != DEF_ITEMTYPE_NONE)) return;

                    
                    if (m_stDialogBoxInfo[26].sV1 == -1)
                    {
                        m_stDialogBoxInfo[26].sV1 = cItemID;
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV2 == -1)
                    {
                        m_stDialogBoxInfo[26].sV2 = cItemID;
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV3 == -1)
                    {
                        m_stDialogBoxInfo[26].sV3 = cItemID;
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV4 == -1)
                    {
                        m_stDialogBoxInfo[26].sV4 = cItemID;
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV5 == -1)
                    {
                        m_stDialogBoxInfo[26].sV5 = cItemID;
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV6 == -1)
                    {
                        m_stDialogBoxInfo[26].sV6 = cItemID;
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
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
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV2 == -1)
                    {
                        m_stDialogBoxInfo[26].sV2 = cItemID;
                        
                        m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV3 == -1)
                    {
                        m_stDialogBoxInfo[26].sV3 = cItemID;
                        
                        m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV4 == -1)
                    {
                        m_stDialogBoxInfo[26].sV4 = cItemID;
                        
                        m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV5 == -1)
                    {
                        m_stDialogBoxInfo[26].sV5 = cItemID;
                        
                        m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    else if (m_stDialogBoxInfo[26].sV6 == -1)
                    {
                        m_stDialogBoxInfo[26].sV6 = cItemID;
                        
                        m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                        if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                        {
                            
                        }
                        else m_bIsItemDisabled[cItemID] = TRUE;
                        return;
                    }
                    
                    AddEventList(BITEMDROP_SKILLDIALOG4, 10);
                    break;
                default:
                    break;
                } // Close switch block
            } 
        } // Close for loop
    } 
#endif
    

    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if (m_cItemOrder[DEF_MAXITEMS - 1 - i] == -1) continue;
        cItemID = m_cItemOrder[DEF_MAXITEMS - 1 - i];
        if (m_pItemList[cItemID] == NULL) continue;

        m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + 32 + m_pItemList[cItemID]->m_sX, sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame);
        
        x1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
        y1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
        x2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
        y2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

        if ((m_bIsItemDisabled[cItemID] == FALSE) && (m_bIsItemEquipped[cItemID] == FALSE) && (msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
        {
            
            
            _SetItemOrder(0, cItemID);
            GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3, 64);

            if (m_bIsDialogEnabled[11] && (m_bIsDialogEnabled[23] == FALSE) && (m_bIsDialogEnabled[23] == FALSE) && (m_stDialogBoxInfo[39].sV3 == 24))
            {
                if (m_pItemList[cItemID]->m_cEquipPos != DEF_EQUIPPOS_NONE)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_REPAIRITEM, NULL, cItemID, m_stDialogBoxInfo[39].sV3, NULL, m_pItemList[cItemID]->m_cName, m_stDialogBoxInfo[39].sV4);
                    return;
                }
            }

            
#ifdef DEF_FEEDBACKCARD
            DisableDialogBox(5);
            if (!strcmp(m_pItemList[cItemID]->m_cName, "Realfeedbackcard1"))
                m_iFeedBackCardIndex = 1;
            if (!strcmp(m_pItemList[cItemID]->m_cName, "Realfeedbackcard2"))
                m_iFeedBackCardIndex = 2;
            if (!strcmp(m_pItemList[cItemID]->m_cName, "Realfeedbackcard3"))
                m_iFeedBackCardIndex = 3;
            if (!strcmp(m_pItemList[cItemID]->m_cName, "Realfeedbackcard4"))
                m_iFeedBackCardIndex = 4;
            if (!strcmp(m_pItemList[cItemID]->m_cName, "Realfeedbackcard5"))
                m_iFeedBackCardIndex = 5;
            if (!strcmp(m_pItemList[cItemID]->m_cName, "Realfeedbackcard6"))
                m_iFeedBackCardIndex = 6;
            if (!strcmp(m_pItemList[cItemID]->m_cName, "Realfeedbackcard7"))
                m_iFeedBackCardIndex = 7;

            if (m_iFeedBackCardIndex > 0)
            {
                m_stDialogBoxInfo[5].sX = 150;
                m_stDialogBoxInfo[5].sY = 100;
                EnableDialogBox(5, NULL, NULL, NULL);
            }
            else
                m_iFeedBackCardIndex = -1;
            //	return;
#endif



        
            if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_PERM) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EAT))
            {


                
                if (bCheckItemOperationEnabled(cItemID) == FALSE) return;

                
                if ((unixtime() - m_dwDamagedTime) < 10000)
                {
                    
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

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, NULL, cItemID, NULL, NULL, NULL);

                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
                    (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EAT))
                {
                    
                    m_bIsItemDisabled[cItemID] = TRUE;
                    m_bItemUsingStatus = TRUE;
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_SKILL)
            {
                
                if (_bIsItemOnHand() == TRUE)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);
                    return;
                }

                if (m_bSkillUsingStatus == TRUE)
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
                    m_bIsGetPointingMode = TRUE;
                    m_iPointCommandType = cItemID; 
                    format_to_local(cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY7, cStr1);
                    AddEventList(cTxt, 10);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST)
            {
                
                if (_bIsItemOnHand() == TRUE)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);
                    return;
                }

                if (m_bSkillUsingStatus == TRUE)
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
                    m_bIsGetPointingMode = TRUE;
                    m_iPointCommandType = cItemID; 
                    format_to_local(cTxt, BDLBBOX_DOUBLE_CLICK_INVENTORY8, cStr1);
                    AddEventList(cTxt, 10);
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX)
            {
                
                if (_bIsItemOnHand() == TRUE)
                {
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);
                    return;
                }

                if (m_bSkillUsingStatus == TRUE)
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
                        
                        if (m_cSkillMastery[12] == 0)
                        {
                            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY9, 10);
                        }
                        else
                        {
                            EnableDialogBox(26, 1, NULL, NULL, NULL);
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
                            EnableDialogBox(26, 3, NULL, NULL, NULL);
                            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY12, 10);
                        }
                        break;
                    }
                }
            }

            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EQUIP)
            {
                
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = (short)cItemID;
                bItemDrop_Character();
                m_stMCursor.cSelectedObjectType = NULL;
                m_stMCursor.sSelectedObjectID = NULL;
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
        m_cDetailLevel = 0;
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
            if (m_bSoundStat == TRUE)
            {
                m_pESound[38].stop();
                m_bSoundStat = FALSE;
                AddEventList(NOTIFY_MSG_SOUND_OFF, 10);
            }
            else
            {
                m_bSoundStat = TRUE;
                AddEventList(NOTIFY_MSG_SOUND_ON, 10);
            }
        }
    }

    if ((msX >= sX + 116) && (msX <= sX + 202) && (msY >= sY + 81) && (msY <= sY + 100))
    {
        if (m_bSoundFlag)
        {
            if (m_bMusicStat == TRUE)
            {
                // Music Off
                m_bMusicStat = FALSE;
                AddEventList(NOTIFY_MSG_MUSIC_OFF, 10);
                if (m_bSoundFlag) m_pBGM.stop();
            }
            else
            {
                // Music On
                if (m_bSoundFlag)
                {
                    m_bMusicStat = TRUE;
                    AddEventList(NOTIFY_MSG_MUSIC_ON, 10);
                    StartBGM();
                }
            }
        }
    }

    
    if ((msX >= sX + 23) && (msX <= sX + 108) && (msY >= sY + 108) && (msY <= sY + 119))
    {
        if (m_bWhisper == TRUE)
        {
            m_bWhisper = FALSE;
            AddEventList(BCHECK_LOCAL_CHAT_COMMAND7, 10);
        }
        else
        {
            m_bWhisper = TRUE;
            AddEventList(BCHECK_LOCAL_CHAT_COMMAND6, 10);
        }
    }

    if ((msX >= sX + 123) && (msX <= sX + 203) && (msY >= sY + 108) && (msY <= sY + 119))
    {
        if (m_bShout == TRUE)
        {
            m_bShout = FALSE;
            AddEventList(BCHECK_LOCAL_CHAT_COMMAND9, 10);
        }
        else
        {
            m_bShout = TRUE;
            AddEventList(BCHECK_LOCAL_CHAT_COMMAND8, 10);
        }
    }
    

    //Transparency Change
    if ((msX >= sX + 28) && (msX <= sX + 235) && (msY >= sY + 156) && (msY <= sY + 171)) m_bDialogTrans = !m_bDialogTrans;

    //Guide Map Toggle
    if ((msX >= sX + 28) && (msX <= sX + 235) && (msY >= sY + 178) && (msY <= sY + 193))
    {
        if (m_bIsDialogEnabled[9]) DisableDialogBox(9);
        else EnableDialogBox(9, 0, 0, 0, NULL);
    }

    if (m_bForceDisconn) return; 
    if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + 225) && (msY <= sY + 225 + DEF_BTNSZY))
    {
        if (m_cLogOutCount == -1)
#ifdef _DEBUG
            m_cLogOutCount = 1;
#else
            m_cLogOutCount = 11;
#endif
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
            
            m_cRestartCount = 5;
            m_dwRestartCountTime = unixtime();
            DisableDialogBox(19);
            format_to_local(G_cTxt, DLGBOX_CLICK_SYSMENU1, (int16_t)m_cRestartCount);
            AddEventList(G_cTxt, 10);
            PlaySound('E', 14, 5);
        }
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
                if ((m_pBankList[m_stDialogBoxInfo[14].sView + i] != NULL) && ((m_stDialogBoxInfo[14].sView + i) < DEF_MAXBANKITEMS))
                {
                    
                    if (_iGetTotalItemNum() >= 50)
                    {
                        AddEventList(DLGBOX_CLICK_BANK1, 10);
                        return;
                    }
                    bSendCommand(MSGID_REQUEST_RETRIEVEITEM, NULL, NULL, (m_stDialogBoxInfo[14].sView + i), NULL, NULL, NULL);
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
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETFISHTHISTIME, NULL, NULL, NULL, NULL, NULL);
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
        if ((m_cMagicMastery[iCPivot + i] != NULL) && (m_pMagicCfgList[iCPivot + i] != NULL))
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

   
    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + 285) && (msY <= sY + 285 + DEF_BTNSZY))
    {
        
        if (m_cSkillMastery[12] == 0) AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY16, 10);
        else
        {
            
            for (i = 0; i < DEF_MAXITEMS; i++)
                if ((m_pItemList[i] != NULL) && (m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX) &&
                    (m_pItemList[i]->m_sSpriteFrame == 55))
                {
                    
                    EnableDialogBox(26, 1, NULL, NULL, NULL);
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

    
    if (m_bIsDialogEnabled[27] == TRUE)
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
            
            EnableDialogBox(m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV2, NULL, NULL);
            DisableDialogBox(20);
        }

        if ((m_bIsDialogEnabled[21] == FALSE) && (msX > sX + 125) && (msX < sX + 180) && (msY > sY + 55) && (msY < sY + 70))
        {
            
            switch (m_stDialogBoxInfo[20].sV1)
            {
            case 7:
                // Guild
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 1, NULL, NULL, NULL);
                AddEventList(TALKING_TO_GUILDHALL_OFFICER, 10);
                break;

            case 11:
               
                switch (m_stDialogBoxInfo[20].sV2)
                {
                case 1:
                    
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 2, NULL, NULL, NULL);
                    AddEventList(TALKING_TO_SHOP_KEEPER, 10);
                    break;
                case 2:
                    
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 3, NULL, NULL, NULL);
                    AddEventList(TALKING_TO_BLACKSMITH_KEEPER, 10);
                    break;
                }
                break;

            case 13:
                
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 4, NULL, NULL, NULL);
                AddEventList(TALKING_TO_CITYHALL_OFFICER, 10);
                break;

            case 14:
                
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 5, NULL, NULL, NULL);
                AddEventList(TALKING_TO_WAREHOUSE_KEEPER, 10);
                break;

            case 16:
                
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 6, NULL, NULL, NULL);
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
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV3, m_stDialogBoxInfo[20].sV5, m_stDialogBoxInfo[20].sV6, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4);
            else AddEventList(DLGBOX_CLICK_NPCACTION_QUERY7, 10);
            DisableDialogBox(20);
        }
        else
            if ((msX > sX + 155) && (msX < sX + 210) && (msY > sY + 55) && (msY < sY + 70))
            {
                
                absX = abs(m_stDialogBoxInfo[20].sV5 - m_sPlayerX);
                absY = abs(m_stDialogBoxInfo[20].sV6 - m_sPlayerY);

                if ((absX <= 4) && (absY <= 4))
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_EXCHANGEITEMTOCHAR, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV3, m_stDialogBoxInfo[20].sV5, m_stDialogBoxInfo[20].sV6, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4);
                else AddEventList(DLGBOX_CLICK_NPCACTION_QUERY8, 10);
                DisableDialogBox(20);
            }
        break;

    case 2:
        
        if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
        {
            
            
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_SELLITEM, NULL, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV2, m_stDialogBoxInfo[20].sV3, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4);
            DisableDialogBox(20);
        }
        else
            if ((msX > sX + 125) && (msX < sX + 180) && (msY > sY + 55) && (msY < sY + 70))
            {
                
                
                
                if (m_stDialogBoxInfo[20].sV3 == 1)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_REPAIRITEM, NULL, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV2, NULL, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4);
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
                else bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV3, m_stDialogBoxInfo[20].sV5, m_stDialogBoxInfo[20].sV6, m_pItemList[m_stDialogBoxInfo[20].sV1]->m_cName, m_stDialogBoxInfo[20].sV4);
            }
            else AddEventList(DLGBOX_CLICK_NPCACTION_QUERY7, 10);

            DisableDialogBox(20);
        }
        break;

    case 4: 
        if ((m_bIsDialogEnabled[21] == FALSE) && (msX > sX + 125) && (msX < sX + 180) && (msY > sY + 55) && (msY < sY + 70))
        {
            
            switch (m_stDialogBoxInfo[20].sV3)
            {
            case 21:
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 21, NULL, NULL, NULL);
                AddEventList(TALKING_TO_GUARD, 10);
                break;

            case 32:
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 32, NULL, NULL, NULL);
                AddEventList(TALKING_TO_UNICORN, 10);
                break;

                
            case 67:
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 67, NULL, NULL, NULL);
                AddEventList(TALKING_TO_MCGAFFIN, 10);
                break;
            case 68:
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 68, NULL, NULL, NULL);
                AddEventList(TALKING_TO_PERRY, 10);
                break;
            case 69:
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 69, NULL, NULL, NULL);
                AddEventList(TALKING_TO_DEVLIN, 10);
                break;
            }
        }

        
        DisableDialogBox(20);
        break;


    case 5:
        if ((msX > sX + 25) && (msX < sX + 100) && (msY > sY + 55) && (msY < sY + 70))
        {
            
            EnableDialogBox(m_stDialogBoxInfo[20].sV1, m_stDialogBoxInfo[20].sV2, NULL, NULL);
            DisableDialogBox(20);
        }

        if ((msX > sX + 25 + 75) && (msX < sX + 80 + 75) && (msY > sY + 55) && (msY < sY + 70))
        {
            
            EnableDialogBox(31, NULL, NULL, NULL);
            DisableDialogBox(20);
        }

        if ((m_bIsDialogEnabled[21] == FALSE) && (msX > sX + 155) && (msX < sX + 210) && (msY > sY + 55) && (msY < sY + 70))
        {
            
            switch (m_stDialogBoxInfo[20].sV1)
            {
            case 7:
                // Guild
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 1, NULL, NULL, NULL);
                AddEventList(TALKING_TO_GUILDHALL_OFFICER, 10);
                break;

            case 11:
               
                switch (m_stDialogBoxInfo[20].sV2)
                {
                case 1:
                    
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 2, NULL, NULL, NULL);
                    AddEventList(TALKING_TO_SHOP_KEEPER, 10);
                    break;
                case 2:
                    
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 3, NULL, NULL, NULL);
                    AddEventList(TALKING_TO_BLACKSMITH_KEEPER, 10);
                    break;
                }
                break;

            case 13:
                
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 4, NULL, NULL, NULL);
                AddEventList(TALKING_TO_CITYHALL_OFFICER, 10);
                break;

            case 14:
                
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 5, NULL, NULL, NULL);
                AddEventList(TALKING_TO_WAREHOUSE_KEEPER, 10);
                break;

            case 16:
                
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TALKTONPC, NULL, 6, NULL, NULL, NULL);
                AddEventList(TALKING_TO_MAGICIAN, 10);
                break;
            }

            
            DisableDialogBox(20);
        }
        break;
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
                if (m_pItemForSaleList[m_stDialogBoxInfo[11].sView + i] != NULL)
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
                ZeroMemory(cTemp, sizeof(cTemp));
                strcpy(cTemp, m_pItemForSaleList[m_stDialogBoxInfo[11].cMode - 1]->m_cName);
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_PURCHASEITEM, NULL, m_stDialogBoxInfo[11].sV3, NULL, NULL, cTemp);
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
            if ((i < DEF_MAXSKILLTYPE) && (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView] != NULL))
            {

                if ((msX >= sX + 44) && (msX <= sX + 135 + 44) && (msY >= sY + 45 + i * 15) && (msY <= sY + 59 + i * 15))
                {
                    if ((m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_bIsUseable == TRUE) &&
                        (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_iLevel != 0))
                    {
                        
                        if (m_bSkillUsingStatus == TRUE)
                        {
                            AddEventList(DLGBOX_CLICK_SKILL1, 10);
                            
                            return;
                        }

                        if ((m_bCommandAvailable == FALSE) || (m_iHP <= 0))
                        {
                            AddEventList(DLGBOX_CLICK_SKILL2, 10);
                            
                            return;
                        }

                        if (m_bIsGetPointingMode == TRUE)
                        {
                            return;
                        }

                        
                        switch (m_pSkillCfgList[i + m_stDialogBoxInfo[15].sView]->m_cUseMethod)
                        {
                        case 0:
                            
                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USESKILL, NULL, (i + m_stDialogBoxInfo[15].sView), NULL, NULL, NULL);
                            m_bSkillUsingStatus = TRUE;
                            DisableDialogBox(15);
                            PlaySound('E', 14, 5);
                            break;
                        }
                    }
                }
                else if ((msX >= sX + 215) && (msX <= sX + 240) && (msY >= sY + 45 + i * 15) && (msY <= sY + 59 + i * 15))
                {

                    if (m_stDialogBoxInfo[15].bFlag == FALSE)
                    {
                        
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_SETDOWNSKILLINDEX, NULL, i + m_stDialogBoxInfo[15].sView, NULL, NULL, NULL);
                        PlaySound('E', 14, 5);
                        m_stDialogBoxInfo[15].bFlag = TRUE;
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
            if (m_pDispBuildItemList[i + m_stDialogBoxInfo[26].sView] != NULL)
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
        if (m_pDispBuildItemList[m_stDialogBoxInfo[26].cStr[0]]->m_bBuildEnabled == TRUE)
        {
            
            if ((msX >= sX + iAdjX + 32) && (msX <= sX + iAdjX + 95) && (msY >= sY + iAdjY + 353) && (msY <= sY + iAdjY + 372))
            {
                // Back
                m_stDialogBoxInfo[26].cMode = 3;
                PlaySound('E', 14, 5);

                if ((m_stDialogBoxInfo[26].sV1 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV1] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV1] = FALSE;
                if ((m_stDialogBoxInfo[26].sV2 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV2] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV2] = FALSE;
                if ((m_stDialogBoxInfo[26].sV3 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV3] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV3] = FALSE;
                if ((m_stDialogBoxInfo[26].sV4 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV4] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV4] = FALSE;
                if ((m_stDialogBoxInfo[26].sV5 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV5] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV5] = FALSE;
                if ((m_stDialogBoxInfo[26].sV6 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV6] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV6] = FALSE;

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

                if ((m_stDialogBoxInfo[26].sV1 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV1] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV1] = FALSE;
                if ((m_stDialogBoxInfo[26].sV2 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV2] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV2] = FALSE;
                if ((m_stDialogBoxInfo[26].sV3 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV3] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV3] = FALSE;
                if ((m_stDialogBoxInfo[26].sV4 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV4] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV4] = FALSE;
                if ((m_stDialogBoxInfo[26].sV5 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV5] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV5] = FALSE;
                if ((m_stDialogBoxInfo[26].sV6 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV6] != NULL))
                    m_bIsItemDisabled[m_stDialogBoxInfo[26].sV6] = FALSE;

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

            if ((m_stDialogBoxInfo[26].sV1 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV1] != NULL))
                m_bIsItemDisabled[m_stDialogBoxInfo[26].sV1] = FALSE;
            if ((m_stDialogBoxInfo[26].sV2 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV2] != NULL))
                m_bIsItemDisabled[m_stDialogBoxInfo[26].sV2] = FALSE;
            if ((m_stDialogBoxInfo[26].sV3 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV3] != NULL))
                m_bIsItemDisabled[m_stDialogBoxInfo[26].sV3] = FALSE;
            if ((m_stDialogBoxInfo[26].sV4 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV4] != NULL))
                m_bIsItemDisabled[m_stDialogBoxInfo[26].sV4] = FALSE;
            if ((m_stDialogBoxInfo[26].sV5 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV5] != NULL))
                m_bIsItemDisabled[m_stDialogBoxInfo[26].sV5] = FALSE;
            if ((m_stDialogBoxInfo[26].sV6 != -1) && (m_pItemList[m_stDialogBoxInfo[26].sV6] != NULL))
                m_bIsItemDisabled[m_stDialogBoxInfo[26].sV6] = FALSE;

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

            bSendCommand(MSGID_REQUEST_CREATENEWGUILD, DEF_MSGTYPE_CONFIRM, NULL, NULL, NULL, NULL, NULL);
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
            
            ZeroMemory(cTemp, sizeof(cTemp));
#ifdef DEF_ENGLISHITEM
            strcpy(cTemp, "GuildAdmissionTicket");
#else
            strcpy(cTemp, "±æµå°¡ÀÔ½ÅÃ»¼­");
#endif
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_PURCHASEITEM, NULL, 1, NULL, NULL, cTemp);
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
            
            ZeroMemory(cTemp, sizeof(cTemp));
#ifdef DEF_ENGLISHITEM
            strcpy(cTemp, "GuildSecessionTicket");
#else
            strcpy(cTemp, "±æµåÅ»Åð½ÅÃ»¼­");
#endif
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_PURCHASEITEM, NULL, 1, NULL, NULL, cTemp);
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
            
            bSendCommand(MSGID_REQUEST_DISBANDGUILD, DEF_MSGTYPE_CONFIRM, NULL, NULL, NULL, NULL, NULL);
            m_stDialogBoxInfo[7].cMode = 6;
            PlaySound('E', 14, 5);
        }
        if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            m_stDialogBoxInfo[7].cMode = 0;
            PlaySound('E', 14, 5);
        }
        break;
        
        // BOOL bSendCommand(DWORD dwMsgID, WORD wCommand, char cDir, int iV1, int iV2, int iV3, char * pString, int iV4 = NULL);

    case 13:
        
        if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 168) && (msY < sY + iAdjY + 185))
        {
            
            bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, NULL, NULL, 1, NULL, NULL, NULL);
            m_stDialogBoxInfo[7].cMode = 18;
            m_iFightzoneNumberTemp = 1;
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 168) && (msY < sY + iAdjY + 185))
        {
            
            bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, NULL, NULL, 2, NULL, NULL, NULL);
            m_stDialogBoxInfo[7].cMode = 18;
            m_iFightzoneNumberTemp = 2;
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 188) && (msY < sY + iAdjY + 205))
        {
            
            bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, NULL, NULL, 3, NULL, NULL, NULL);
            m_stDialogBoxInfo[7].cMode = 18;
            m_iFightzoneNumberTemp = 3;
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 188) && (msY < sY + iAdjY + 205))
        {
            
            bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, NULL, NULL, 4, NULL, NULL, NULL);
            m_stDialogBoxInfo[7].cMode = 18;
            m_iFightzoneNumberTemp = 4;
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 208) && (msY < sY + iAdjY + 225))
        {
            
            bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, NULL, NULL, 5, NULL, NULL, NULL);
            m_stDialogBoxInfo[7].cMode = 18;
            m_iFightzoneNumberTemp = 5;
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 208) && (msY < sY + iAdjY + 225))
        {
            
            bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, NULL, NULL, 6, NULL, NULL, NULL);
            m_stDialogBoxInfo[7].cMode = 18;
            m_iFightzoneNumberTemp = 6;
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + iAdjX + 65) && (msX < sX + iAdjX + 137) && (msY > sY + iAdjY + 228) && (msY < sY + iAdjY + 245))
        {
            
            bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, NULL, NULL, 7, NULL, NULL, NULL);
            m_stDialogBoxInfo[7].cMode = 18;
            m_iFightzoneNumberTemp = 7;
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + iAdjX + 150) && (msX < sX + iAdjX + 222) && (msY > sY + iAdjY + 228) && (msY < sY + iAdjY + 245))
        {
            
            bSendCommand(MSGID_REQUEST_FIGHTZONE_RESERVE, NULL, NULL, 8, NULL, NULL, NULL);
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

    ZeroMemory(cName, sizeof(cName));
    ZeroMemory(cName20, sizeof(cName20));
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
            
            if (m_stGuildOpList[0].cOpMode == NULL) DisableDialogBox(8);
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
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_JOINGUILDAPPROVE, NULL, NULL, NULL, NULL, cName20);
            break;

        case 2:
            
            strcpy(cName20, m_stGuildOpList[0].cName);
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_DISMISSGUILDAPPROVE, NULL, NULL, NULL, NULL, cName20);
            break;
        }

        _ShiftGuildOperationList();
        
        if (m_stGuildOpList[0].cOpMode == NULL) DisableDialogBox(8);
    }

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        
        PlaySound('E', 14, 5);

        switch (m_stGuildOpList[0].cOpMode)
        {
        case 1:
            
            strcpy(cName20, m_stGuildOpList[0].cName);
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_JOINGUILDREJECT, NULL, NULL, NULL, NULL, cName20);
            break;

        case 2:
            
            strcpy(cName20, m_stGuildOpList[0].cName);
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_DISMISSGUILDREJECT, NULL, NULL, NULL, NULL, cName20);
            break;
        }

        _ShiftGuildOperationList();
        
        if (m_stGuildOpList[0].cOpMode == NULL) DisableDialogBox(8);
    }
}

void CGame::DlgBoxClick_IconPannel(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[30].sX;
    sY = m_stDialogBoxInfo[30].sY;

    if ((msX > 322) && (msX < 355) && (434 < msY) && (475 > msY))
    {
        
        if (m_bIsCrusadeMode == FALSE) return;
        switch (m_iCrusadeDuty)
        {
        case 1: // Fighter
            EnableDialogBox(38, NULL, NULL, NULL);
            break;

        case 2: // Constructor
            EnableDialogBox(37, NULL, NULL, NULL);
            break;

        case 3: // Commander
            EnableDialogBox(36, NULL, NULL, NULL);
            break;

        default: break;
        }
        PlaySound('E', 14, 5);
    }

    if ((362 < msX) && (404 > msX) && (434 < msY) && (475 > msY))
    {
        // Combat Mode Toggle
        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TOGGLECOMBATMODE, NULL, NULL, NULL, NULL, NULL);
        PlaySound('E', 14, 5);
    }

    // Character
    if ((413 <= msX) && (446 >= msX) && (434 < msY) && (475 > msY))
    {
        if (m_bIsDialogEnabled[1] == TRUE)
            DisableDialogBox(1);
        else EnableDialogBox(1, NULL, NULL, NULL);
        PlaySound('E', 14, 5);
    }

    // Inventory
    if ((453 <= msX) && (486 >= msX) && (434 < msY) && (475 > msY))
    {
        if (m_bIsDialogEnabled[2] == TRUE)
            DisableDialogBox(2);
        else EnableDialogBox(2, NULL, NULL, NULL);
        PlaySound('E', 14, 5);
    }

    // Magic
    if ((490 <= msX) && (522 >= msX) && (434 < msY) && (475 > msY))
    {
        if (m_bIsDialogEnabled[3] == TRUE)
            DisableDialogBox(3);
        else EnableDialogBox(3, NULL, NULL, NULL);
        PlaySound('E', 14, 5);
    }

    // Skill
    if ((526 <= msX) && (552 >= msX) && (434 < msY) && (475 > msY))
    {
        if (m_bIsDialogEnabled[15] == TRUE)
            DisableDialogBox(15);
        else EnableDialogBox(15, NULL, NULL, NULL);
        PlaySound('E', 14, 5);
    }

    // Chat
    if ((556 <= msX) && (587 >= msX) && (434 < msY) && (475 > msY))
    {
        if (m_bIsDialogEnabled[10] == TRUE)
            DisableDialogBox(10);
        else EnableDialogBox(10, NULL, NULL, NULL);
        PlaySound('E', 14, 5);
    }

    // System Menu
    if ((589 <= msX) && (621 >= msX) && (434 < msY) && (475 > msY))
    {
        if (m_bIsDialogEnabled[19] == TRUE)
            DisableDialogBox(19);
        else EnableDialogBox(19, NULL, NULL, NULL);
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
                m_bIsGetPointingMode = TRUE;
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
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_JOINPARTY, NULL, 2, NULL, NULL, m_cMCName);
                m_stDialogBoxInfo[32].cMode = 4;
                PlaySound('E', 14, 5);
            }
        }

        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY)) DisableDialogBox(32);
        break;

    case 1:
        if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY, NULL, 1, NULL, NULL, m_stDialogBoxInfo[32].cStr);
            DisableDialogBox(32);
            PlaySound('E', 14, 5);
        }

        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY, NULL, 0, NULL, NULL, m_stDialogBoxInfo[32].cStr);
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
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_ACCEPTJOINPARTY, NULL, 2, NULL, NULL, m_stDialogBoxInfo[32].cStr);
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
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_JOINPARTY, NULL, NULL, NULL, NULL, m_cMCName);
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
        if (m_bCitizen == FALSE)
        {
            
            DisableDialogBox(33);
            PlaySound('E', 14, 5);
        }
        else if (m_bAresden == TRUE)
        {
            
            if (m_iGuildRank == 0)
            {
                if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                {
                    
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, NULL, 3, NULL, NULL, NULL);
                    DisableDialogBox(33);
                    PlaySound('E', 14, 5);
                }
            }
            else
            {
                if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                {
                    
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, NULL, 1, NULL, NULL, NULL);
                    DisableDialogBox(33);
                }

                
                if (m_iGuildRank != -1)
                {
                    if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 175) && (msY < sY + 190))
                    {
                        
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, NULL, 2, NULL, NULL, NULL);
                        DisableDialogBox(33);
                        PlaySound('E', 14, 5);
                    }
                }
            }
        }
        else if (m_bAresden == FALSE)
        {
            
            if (m_iGuildRank == 0)
            {
                
                if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                {
                    
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, NULL, 3, NULL, NULL, NULL);
                    DisableDialogBox(33);
                    PlaySound('E', 14, 5);
                }
            }
            else
            {
                if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 150) && (msY < sY + 165))
                {
                    
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, NULL, 1, NULL, NULL, NULL);
                    DisableDialogBox(33);
                    PlaySound('E', 14, 5);
                }

                
                if (m_iGuildRank != -1)
                {
                    if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 175) && (msY < sY + 190))
                    {
                        
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_SELECTCRUSADEDUTY, NULL, 2, NULL, NULL, NULL);
                        DisableDialogBox(33);
                        PlaySound('E', 14, 5);
                    }
                }
            }
        }

        if ((msX > sX + 210) && (msX < sX + 260) && (msY >= sY + 296) && (msY <= sY + 316))
        {
            
            DisableDialogBox(18);
            EnableDialogBox(18, 813, NULL, NULL);
            PlaySound('E', 14, 5);
        }
        break;

    case 2:
        if ((msX > sX + 24) && (msX < sX + 246) && (msY > sY + 160) && (msY < sY + 175))
        {
            switch (m_iCrusadeDuty)
            {
            case 1: EnableDialogBox(18, 803, NULL, NULL); break;
            case 2: EnableDialogBox(18, 805, NULL, NULL); break;
            case 3: EnableDialogBox(18, 808, NULL, NULL); break;
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

    if (m_bIsCrusadeMode == FALSE) return;

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
            EnableDialogBox(18, 808, NULL, NULL);
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

            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SETGUILDTELEPORTLOC, NULL, tX, tY, NULL, "middleland");
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
            EnableDialogBox(18, 809, NULL, NULL);
            PlaySound('E', 14, 5);
        }
        break;

    case 2:
        
        if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
        {
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GUILDTELEPORT, NULL, NULL, NULL, NULL, NULL);
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
            EnableDialogBox(18, 810, NULL, NULL);
            PlaySound('E', 14, 5);
        }
        break;

    case 3:
        if (m_bAresden == TRUE)
        {
            
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                
                if (m_iConstructionPoint >= 3000)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 47, 1, m_stDialogBoxInfo[36].sV1, NULL);
                    PlaySound('E', 14, 5);
                    DisableDialogBox(36);
                }
            }
            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                
                if (m_iConstructionPoint >= 2000)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 46, 1, m_stDialogBoxInfo[36].sV1, NULL);
                    PlaySound('E', 14, 5);
                    DisableDialogBox(36);
                }
            }
            if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                
                if (m_iConstructionPoint >= 1000)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 43, 1, m_stDialogBoxInfo[36].sV1, NULL);
                    PlaySound('E', 14, 5);
                    DisableDialogBox(36);
                }
            }
            if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                
                if (m_iConstructionPoint >= 1500)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 51, 1, m_stDialogBoxInfo[36].sV1, NULL);
                    PlaySound('E', 14, 5);
                    DisableDialogBox(36);
                }
            }
        }
        else if (m_bAresden == FALSE)
        {
            
            if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                
                if (m_iConstructionPoint >= 3000)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 45, 1, m_stDialogBoxInfo[36].sV1, NULL);
                    PlaySound('E', 14, 5);
                    DisableDialogBox(36);
                }
            }
            if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                
                if (m_iConstructionPoint >= 2000)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 44, 1, m_stDialogBoxInfo[36].sV1, NULL);
                    PlaySound('E', 14, 5);
                    DisableDialogBox(36);
                }
            }
            if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                
                if (m_iConstructionPoint >= 1000)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 43, 1, m_stDialogBoxInfo[36].sV1, NULL);
                    PlaySound('E', 14, 5);
                    DisableDialogBox(36);
                }
            }
            if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
            {
                
                if (m_iConstructionPoint >= 1500)
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 51, 1, m_stDialogBoxInfo[36].sV1, NULL);
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
            EnableDialogBox(18, 811, NULL, NULL);
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

            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SETGUILDCONSTRUCTLOC, NULL, tX, tY, NULL, "middleland");
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
            EnableDialogBox(18, 812, NULL, NULL);
            PlaySound('E', 14, 5);
        }
        break;
    }
}

void CGame::DlgBoxClick_Constructor(int msX, int msY)
{
    short sX, sY;

    if (m_bIsCrusadeMode == FALSE) return;

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
            EnableDialogBox(18, 805, NULL, NULL);
            PlaySound('E', 14, 5);
        }
        break;

    case 1:
        
        if ((msX >= sX + 20) && (msX <= sX + 20 + 46) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 38, 1, m_stDialogBoxInfo[36].sV1, NULL);
            PlaySound('E', 14, 5);
            DisableDialogBox(37);
        }
        if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 50 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 39, 1, m_stDialogBoxInfo[36].sV1, NULL);
            PlaySound('E', 14, 5);
            DisableDialogBox(37);
        }
        if ((msX >= sX + 20 + 100) && (msX <= sX + 20 + 100 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 36, 1, m_stDialogBoxInfo[36].sV1, NULL);
            PlaySound('E', 14, 5);
            DisableDialogBox(37);
        }
        if ((msX >= sX + 20 + 150) && (msX <= sX + 20 + 150 + 45) && (msY >= sY + 220) && (msY <= sY + 220 + 50))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SUMMONWARUNIT, NULL, 37, 1, m_stDialogBoxInfo[36].sV1, NULL);
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
            EnableDialogBox(18, 806, NULL, NULL);
            PlaySound('E', 14, 5);
        }
        break;

    case 2:
        
        if ((msX >= sX + 20 + 50) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
        {
            // Teleport Confirm
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GUILDTELEPORT, NULL, NULL, NULL, NULL, NULL);
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
            EnableDialogBox(18, 807, NULL, NULL);
            PlaySound('E', 14, 5);
        }
        break;
    }
}

void CGame::DlgBoxClick_Soldier(int msX, int msY)
{
    short sX, sY;

    if (m_bIsCrusadeMode == FALSE) return;

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
            EnableDialogBox(18, 803, NULL, NULL);
            PlaySound('E', 14, 5);
        }
        break;

    case 1:
        
        if ((msX >= sX + 20) && (msX <= sX + 20 + 46 + 50) && (msY >= sY + 322) && (msY <= sY + 322 + 52))
        {
            // Teleport Confirm
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GUILDTELEPORT, NULL, NULL, NULL, NULL, NULL);
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
            EnableDialogBox(18, 804, NULL, NULL);
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
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_QUESTACCEPTED, NULL, NULL, NULL, NULL, NULL);
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
            m_stDialogBoxInfo[34].dwV1 = unixtime();
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
                if (m_pItemList[i] != NULL)
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
            m_stDialogBoxInfo[34].dwV1 = unixtime();
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
            if (m_pItemList[m_stSellItemList[i].iIndex] != NULL)
            {
                
                m_bIsItemDisabled[m_stSellItemList[i].iIndex] = FALSE;
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
        bSendCommand(MSGID_REQUEST_SELLITEMLIST, NULL, NULL, NULL, NULL, NULL, NULL);
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

#ifdef _DEBUG
void CGame::DlgBoxClick_LevelUpSettings(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[12].sX;
    sY = m_stDialogBoxInfo[12].sY;

    

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 177 - 50) && (msY < sY + 187 - 50))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Str++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 197 - 50) && (msY < sY + 206 - 50))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Vit++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 216 - 50) && (msY < sY + 224 - 50))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Dex++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 235 - 50) && (msY < sY + 242 - 50))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Int++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 255 - 50) && (msY < sY + 262 - 50))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Mag++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 228 - 50) && (msX < sX + 237 - 50) && (msY > sY + 274 - 50) && (msY < sY + 280 - 50))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Char++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 177 - 50) && (msY < sY + 187 - 50))
    {
        if (m_cLU_Str > 0)
        {
            m_cLU_Str--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 197 - 50) && (msY < sY + 206 - 50))
    {
        if (m_cLU_Vit > 0)
        {
            m_cLU_Vit--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 216 - 50) && (msY < sY + 224 - 50))
    {
        if (m_cLU_Dex > 0)
        {
            m_cLU_Dex--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 235 - 50) && (msY < sY + 242 - 50))
    {
        if (m_cLU_Int > 0)
        {
            m_cLU_Int--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 255 - 50) && (msY < sY + 262 - 50))
    {
        if (m_cLU_Mag > 0)
        {
            m_cLU_Mag--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 245 - 50) && (msX < sX + 253 - 50) && (msY > sY + 274 - 50) && (msY < sY + 280 - 50))
    {
        if (m_cLU_Char > 0)
        {
            m_cLU_Char--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        if (m_iLU_Point == 0)
        {
            bSendCommand(MSGID_LEVELUPSETTINGS, NULL, NULL, NULL, NULL, NULL, NULL);
            DisableDialogBox(12);
        }
        PlaySound('E', 14, 5);
    }
}

#else

void CGame::DlgBoxClick_LevelUpSettings(short msX, short msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[12].sX;
    sY = m_stDialogBoxInfo[12].sY;

    if ((msX > sX + 184) && (msX < sX + 197) && (msY > sY + 110) && (msY < sY + 122))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Str++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 184) && (msX < sX + 197) && (msY > sY + 125) && (msY < sY + 137))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Vit++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 184) && (msX < sX + 197) && (msY > sY + 140) && (msY < sY + 152))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Dex++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 184) && (msX < sX + 197) && (msY > sY + 155) && (msY < sY + 168))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Int++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 184) && (msX < sX + 197) && (msY > sY + 170) && (msY < sY + 183))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Mag++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 184) && (msX < sX + 197) && (msY > sY + 185) && (msY < sY + 198))
    {
        if (m_iLU_Point > 0)
        {
            m_iLU_Point--;
            m_cLU_Char++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 199) && (msX < sX + 212) && (msY > sY + 110) && (msY < sY + 122))
    {
        if (m_cLU_Str > 0)
        {
            m_cLU_Str--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 199) && (msX < sX + 212) && (msY > sY + 125) && (msY < sY + 138))
    {
        if (m_cLU_Vit > 0)
        {
            m_cLU_Vit--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 199) && (msX < sX + 212) && (msY > sY + 140) && (msY < sY + 152))
    {
        if (m_cLU_Dex > 0)
        {
            m_cLU_Dex--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 199) && (msX < sX + 212) && (msY > sY + 155) && (msY < sY + 168))
    {
        if (m_cLU_Int > 0)
        {
            m_cLU_Int--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 199) && (msX < sX + 212) && (msY > sY + 170) && (msY < sY + 183))
    {
        if (m_cLU_Mag > 0)
        {
            m_cLU_Mag--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX > sX + 199) && (msX < sX + 212) && (msY > sY + 185) && (msY < sY + 198))
    {
        if (m_cLU_Char > 0)
        {
            m_cLU_Char--;
            m_iLU_Point++;
        }
        PlaySound('E', 14, 5);
    }

    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        if (m_iLU_Point == 0)
        {
            bSendCommand(MSGID_LEVELUPSETTINGS, NULL, NULL, NULL, NULL, NULL, NULL);
            DisableDialogBox(12);
        }
        PlaySound('E', 14, 5);
    }
}
#endif

void CGame::DlgBoxClick_CityhallMenu(short msX, short msY)
{
    short sX, sY;

    
    sX = m_stDialogBoxInfo[13].sX;
    sY = m_stDialogBoxInfo[13].sY;

    switch (m_stDialogBoxInfo[13].cMode)
    {
    case 0:
        
        if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
        {
            if (m_bCitizen == TRUE) return;
            
            m_stDialogBoxInfo[13].cMode = 1;
            PlaySound('E', 14, 5);
        }
        if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
        {
            if (m_iRewardGold <= 0) return;
            
            m_stDialogBoxInfo[13].cMode = 5;
            PlaySound('E', 14, 5);
        }
        if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
        {
            if (m_iEnemyKillCount < 300) return;
            
            m_stDialogBoxInfo[13].cMode = 7;
            PlaySound('E', 14, 5);
        }
        if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 145) && (msY < sY + 170))
        {
            if (m_stQuest.sQuestType == NULL) return;
            
            m_stDialogBoxInfo[13].cMode = 8;
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 170) && (msY < sY + 195))
        {
            if (m_bIsCrusadeMode) return;
            if (m_iPKCount != 0) return;
            if (m_bCitizen == FALSE) return;
            if ((m_iLevel > 100) && (m_bHunter == FALSE)) return;

            
            m_stDialogBoxInfo[13].cMode = 9;
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 195) && (msY < sY + 220))
        {
            
            m_stDialogBoxInfo[13].cMode = 10;
            m_iTeleportMapCount = -1;
            bSendCommand(MSGID_REQUEST_TELEPORT_LIST, NULL, NULL, NULL, NULL, NULL, NULL);
            PlaySound('E', 14, 5);
        }

        if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 195) && (msY < sY + 220))
        {
            if (m_bIsCrusadeMode == FALSE) return;
            
            EnableDialogBox(33, 1, NULL, NULL);
            PlaySound('E', 14, 5);
        }
        break;

    case 1:
        
        if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            bSendCommand(MSGID_REQUEST_CIVILRIGHT, DEF_MSGTYPE_CONFIRM, NULL, NULL, NULL, NULL, NULL);
            m_stDialogBoxInfo[13].cMode = 2;
            PlaySound('E', 14, 5);
        }
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            m_stDialogBoxInfo[13].cMode = 0;
            PlaySound('E', 14, 5);
        }
        break;

    case 3:
        
    case 4:
        
        
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            m_stDialogBoxInfo[13].cMode = 0;
            PlaySound('E', 14, 5);
        }
        break;

    case 5:
        
        if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETREWARDMONEY, NULL, NULL, NULL, NULL, NULL);
            m_stDialogBoxInfo[13].cMode = 0;
            PlaySound('E', 14, 5);

        }
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            m_stDialogBoxInfo[13].cMode = 0;
            PlaySound('E', 14, 5);
        }
        break;

    case 7:
        
        if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_GETHEROMANTLE, NULL, NULL, NULL, NULL, NULL);
            m_stDialogBoxInfo[13].cMode = 0;
            PlaySound('E', 14, 5);

        }
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            m_stDialogBoxInfo[13].cMode = 0;
            PlaySound('E', 14, 5);
        }
        break;

    case 8:
        
        if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_CANCELQUEST, NULL, NULL, NULL, NULL, NULL);
            m_stDialogBoxInfo[13].cMode = 0;
            PlaySound('E', 14, 5);
        }
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            m_stDialogBoxInfo[13].cMode = 0;
            PlaySound('E', 14, 5);
        }
        break;

    case 9:
        
        if ((msX >= sX + DEF_LBTNPOSX) && (msX <= sX + DEF_LBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_HUNTMODE, NULL, NULL, NULL, NULL, NULL);
            m_stDialogBoxInfo[13].cMode = 0;
            PlaySound('E', 14, 5);
        }
        if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
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
                    //TEST CODE
                    //format_to_local( G_cTxt, "i : {}, Index : {}, MapName : {}", i, m_stTeleportList[i].iIndex, m_stTeleportList[i].mapname );
                    //AddEventList( G_cTxt, 10 );
                    bSendCommand(MSGID_REQUEST_CHARGED_TELEPORT, NULL, NULL, m_stTeleportList[i].iIndex, NULL, NULL, NULL);
                    DisableDialogBox(13);
                    return;
                }
            }
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
        if (m_iGizonItemUpgradeLeft == NULL) m_iGizonItemUpgradeLeft = 0;
        
        EnableDialogBox(34, 5, NULL, NULL);
        PlaySound('E', 14, 5);
    }

    if ((msX >= sX + 140) && (msX <= sX + 212) && (msY >= sY + 172) && (msY <= sY + 184))
    {
        
        if (m_cSkillMastery[13] == 0)
        {
            AddEventList(DLGBOXCLICK_INVENTORY1, 10);
            AddEventList(DLGBOXCLICK_INVENTORY2, 10);
        }
        else if (m_bSkillUsingStatus == TRUE)
        {
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY5, 10);
            return;
        }
        else if (_bIsItemOnHand() == TRUE)
        {
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY4, 10);
            return;
        }
        else
        {
            
           
            for (i = 0; i < DEF_MAXITEMS; i++)
                if ((m_pItemList[i] != NULL) && (m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX) &&
                    (m_pItemList[i]->m_sSpriteFrame == 113) && (m_pItemList[i]->m_wCurLifeSpan > 0))
                {
                    
                    EnableDialogBox(26, 3, NULL, NULL, NULL);
                    AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY12, 10);
                    PlaySound('E', 14, 5);
                    return;
                }
            AddEventList(BDLBBOX_DOUBLE_CLICK_INVENTORY14, 10);
           
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
        EnableDialogBox(28, 1, NULL, NULL);
        DisableDialogBox(1);
        PlaySound('E', 14, 5);
    }
    else if ((msX >= sX + 98) && (msX <= sX + 98 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
    {
        EnableDialogBox(32, NULL, NULL, NULL);
        DisableDialogBox(1);
        PlaySound('E', 14, 5);
    }
    else if ((msX >= sX + 180) && (msX <= sX + 180 + DEF_BTNSZX) && (msY >= sY + 340) && (msY <= sY + 340 + DEF_BTNSZY))
    {
        EnableDialogBox(12, NULL, NULL, NULL);
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
        if ((m_pMagicCfgList[iCPivot + i] != NULL) && (m_pMagicCfgList[iCPivot + i]->m_bIsVisible))
        {
            if ((msX >= sX + iAdjX + 44) && (msX <= sX + iAdjX + 135 + 44) && (msY >= sY + iAdjY + 70 + iYloc + 35) && (msY <= sY + iAdjY + 70 + 14 + iYloc + 35))
            {
                if (m_cMagicMastery[iCPivot + i] == 0)
                {
                    
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_STUDYMAGIC, NULL, NULL, NULL, NULL, m_pMagicCfgList[iCPivot + i]->m_cName);
                    
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
            NULL,
            m_stDialogBoxInfo[4].sView,
            1,
            NULL,
            m_pItemList[m_stDialogBoxInfo[4].sView]->m_cName);
        //m_stDialogBoxInfo[40].sView ;
        DisableDialogBox(4);
    }

    else if ((msX >= sX + 170) && (msX <= sX + 170 + DEF_BTNSZX) && (msY >= sY + 55) && (msY <= sY + 55 + DEF_BTNSZY))
    {

        for (int i = 0; i < DEF_MAXSELLLIST; i++)
            m_bIsItemDisabled[i] = FALSE;

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
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_SELLITEMCONFIRM, NULL, m_stDialogBoxInfo[23].sV1, m_stDialogBoxInfo[23].sV4, m_stDialogBoxInfo[23].sV3, m_pItemList[m_stDialogBoxInfo[23].sV1]->m_cName); //v1.2
            m_stDialogBoxInfo[23].cMode = 3;
        }
        if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            m_bIsItemDisabled[m_stDialogBoxInfo[23].sV1] = FALSE;
            DisableDialogBox(23);
        }
        break;

    case 2:
        
        if ((msX >= sX + 30) && (msX <= sX + 30 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_REPAIRITEMCONFIRM, NULL, m_stDialogBoxInfo[23].sV1, NULL, NULL, m_pItemList[m_stDialogBoxInfo[23].sV1]->m_cName);
            m_stDialogBoxInfo[23].cMode = 4;
        }
        if ((msX >= sX + 154) && (msX <= sX + 154 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
        {
            
            m_bIsItemDisabled[m_stDialogBoxInfo[23].sV1] = FALSE;
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
            // Exchange
            if ((m_stDialogBoxInfo[27].sV1 != -1) && (m_stDialogBoxInfo[27].sV5 != -1))
            {

                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_CONFIRMEXCHANGEITEM, NULL, m_stDialogBoxInfo[27].sView, // ItemID
                    m_stDialogBoxInfo[27].sV3, // Amount
                    NULL, NULL);

                PlaySound('E', 14, 5);
                m_stDialogBoxInfo[27].cMode = 2;
            }
            return;
        }

        if ((msX >= sX + 450) && (msX <= sX + 450 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY))
        {
            
            DisableDialogBox(27);
            DisableDialogBox(22);
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_CANCELEXCHANGEITEM, NULL, NULL, NULL, NULL, NULL);

            PlaySound('E', 14, 5);
            return;
        }
        break;

    case 2:
        if ((msX >= sX + 450) && (msX <= sX + 450 + DEF_BTNSZX) && (msY >= sY + 310) && (msY <= sY + 310 + DEF_BTNSZY))
        {
            
            DisableDialogBox(27);
            DisableDialogBox(22);
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_CANCELEXCHANGEITEM, NULL, NULL, NULL, NULL, NULL);

            PlaySound('E', 14, 5);
            return;
        }
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

void CGame::DlgBoxClick_Help(int msX, int msY)
{
    short sX, sY;

    sX = m_stDialogBoxInfo[35].sX;
    sY = m_stDialogBoxInfo[35].sY;


    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 0) && (msY < sY + 50 + 15 * 1))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 900, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 1) && (msY < sY + 50 + 15 * 2))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 1000, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 2) && (msY < sY + 50 + 15 * 3))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 901, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 3) && (msY < sY + 50 + 15 * 4))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 902, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 4) && (msY < sY + 50 + 15 * 5))
    {
        
        DisableDialogBox(18);
        EnableDialogBox(18, 903, NULL, NULL); 
        m_bIsF1HelpWindowEnabled = TRUE;
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 5) && (msY < sY + 50 + 15 * 6))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 904, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 6) && (msY < sY + 50 + 15 * 7))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 905, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 7) && (msY < sY + 50 + 15 * 8))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 906, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 8) && (msY < sY + 50 + 15 * 9))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 907, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 9) && (msY < sY + 50 + 15 * 10))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 908, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 10) && (msY < sY + 50 + 15 * 11))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 909, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 11) && (msY < sY + 50 + 15 * 12))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 910, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 12) && (msY < sY + 50 + 15 * 13))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 911, NULL, NULL); // FAQ
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 13) && (msY < sY + 50 + 15 * 14))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 912, NULL, NULL); 
    }
#if DEF_LANGUAGE == 3
    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 14) && (msY < sY + 50 + 15 * 15))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 913, NULL, NULL); 
    }

    if ((msX >= sX + 25) && (msX <= sX + 248) && (msY >= sY + 50 + 15 * 15) && (msY < sY + 50 + 15 * 16))
    {
        DisableDialogBox(18);
        EnableDialogBox(18, 914, NULL, NULL); 
    }
#endif
    if ((msX >= sX + DEF_RBTNPOSX) && (msX <= sX + DEF_RBTNPOSX + DEF_BTNSZX) && (msY > sY + DEF_BTNPOSY) && (msY < sY + DEF_BTNPOSY + DEF_BTNSZY))
    {
        PlaySound('E', 14, 5);
        DisableDialogBox(35);
    }
}

BOOL CGame::bDlgBoxPress_Character(short msX, short msY)
{
    int i;
    short sX, sY, sSprH, sFrame;
    char cEquipPosStatus[DEF_MAXITEMEQUIPPOS];

    if (m_bIsDialogEnabled[17] == TRUE) return FALSE;

    sX = m_stDialogBoxInfo[1].sX;
    sY = m_stDialogBoxInfo[1].sY;
    for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) cEquipPosStatus[i] = -1;
    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if ((m_pItemList[i] != NULL) && (m_bIsItemEquipped[i] == TRUE))	cEquipPosStatus[m_pItemList[i]->m_cEquipPos] = i;
    }

    if ((m_sPlayerType >= 1) && (m_sPlayerType <= 3))
    {
        if (cEquipPosStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 72, sY + 135, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 32, sY + 193, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_RFINGER];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 35, sY + 120, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 57, sY + 186, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 90, sY + 170, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_FULLBODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BOOTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BOOTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH]->_bCheckCollison(sX + 41, sY + 137, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BACK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
    }
    else if ((m_sPlayerType >= 4) && (m_sPlayerType <= 6))
    {
        if (cEquipPosStatus[DEF_EQUIPPOS_HEAD] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_HEAD]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 72, sY + 139, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_RFINGER] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RFINGER]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 32, sY + 193, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_RFINGER];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_NECK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_NECK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 35, sY + 120, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_TWOHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_TWOHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_RHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_RHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 60, sY + 191, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_LHAND] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_LHAND]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 84, sY + 175, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_FULLBODY] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_FULLBODY]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_FULLBODY];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if ((cEquipPosStatus[DEF_EQUIPPOS_BOOTS] != -1))
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BOOTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BOOTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_ARMS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_ARMS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_PANTS] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_PANTS]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 171, sY + 290, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
        if (cEquipPosStatus[DEF_EQUIPPOS_BACK] != -1)
        {
            sSprH = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSprite;
            sFrame = m_pItemList[cEquipPosStatus[DEF_EQUIPPOS_BACK]]->m_sSpriteFrame;
            if (m_pSprite[DEF_SPRID_ITEMEQUIP_PIVOTPOINT + sSprH + 40]->_bCheckCollison(sX + 45, sY + 143, sFrame, msX, msY))
            {
                m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                m_stMCursor.sSelectedObjectID = m_sItemEquipmentStatus[DEF_EQUIPPOS_BACK];
                m_stMCursor.sDistX = 0;
                m_stMCursor.sDistY = 0;
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL CGame::bDlgBoxPress_SkillDlg(short msX, short msY)
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
        ZeroMemory(sArray, sizeof(sArray));
        sArray[1] = m_stDialogBoxInfo[26].sV1;
        sArray[2] = m_stDialogBoxInfo[26].sV2;
        sArray[3] = m_stDialogBoxInfo[26].sV3;
        sArray[4] = m_stDialogBoxInfo[26].sV4;
        sArray[5] = m_stDialogBoxInfo[26].sV5;
        sArray[6] = m_stDialogBoxInfo[26].sV6;

        for (i = 1; i <= 6; i++)
            if ((sArray[i] != -1) && (m_pItemList[sArray[i]] != NULL))
            {
                cItemID = (char)sArray[i];

                switch (i)
                {
                case 1: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 2: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 1, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 3: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 2, sY + iAdjY + 55, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 4: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 5: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 1, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 6: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 2, sY + iAdjY + 100, m_pItemList[cItemID]->m_sSpriteFrame); break;
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

                    
                    m_bIsItemDisabled[cItemID] = FALSE;

                    m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                    m_stMCursor.sSelectedObjectID = cItemID;
                    m_stMCursor.sDistX = msX + iAdjX - x1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                    m_stMCursor.sDistY = msY + iAdjY - y1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;
                    return TRUE;
                }
            }
        break;

    case 4:
        ZeroMemory(sArray, sizeof(sArray));
        sArray[1] = m_stDialogBoxInfo[26].sV1;
        sArray[2] = m_stDialogBoxInfo[26].sV2;
        sArray[3] = m_stDialogBoxInfo[26].sV3;
        sArray[4] = m_stDialogBoxInfo[26].sV4;
        sArray[5] = m_stDialogBoxInfo[26].sV5;
        sArray[6] = m_stDialogBoxInfo[26].sV6;

        for (i = 1; i <= 6; i++)
            if ((sArray[i] != -1) && (m_pItemList[sArray[i]] != NULL))
            {
                cItemID = (char)sArray[i];

                switch (i)
                {
                case 1: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 55 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 2: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 55 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 3: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 55 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 4: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 30 + 13, sY + iAdjY + 100 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 5: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 1 + 30 + 13, sY + iAdjY + 100 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
                case 6: m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + iAdjX + 55 + 45 * 2 + 30 + 13, sY + iAdjY + 100 + 180, m_pItemList[cItemID]->m_sSpriteFrame); break;
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

                    
                    m_bIsItemDisabled[cItemID] = FALSE;

                    m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                    m_stMCursor.sSelectedObjectID = cItemID;
                    m_stMCursor.sDistX = msX + iAdjX - x1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                    m_stMCursor.sDistY = msY + iAdjY - y1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;

                    m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                    return TRUE;
                }
            }
        break;
    }

    return FALSE;
}

BOOL CGame::bDlgBoxPress_Inventory(short msX, short msY)
{
    int i;
    char  cItemID;
    short sX, sY, x1, x2, y1, y2;

#ifdef _DEBUG
    AddEventList("Press Inventory", 10);
#endif

    if (m_bIsDialogEnabled[2] == FALSE) return FALSE;
    if (m_bIsDialogEnabled[17] == TRUE) return FALSE;
    if (m_bIsDialogEnabled[4] == TRUE) return FALSE;

    sX = m_stDialogBoxInfo[2].sX;
    sY = m_stDialogBoxInfo[2].sY;

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_cItemOrder[DEF_MAXITEMS - 1 - i] != -1)
        {
            cItemID = m_cItemOrder[DEF_MAXITEMS - 1 - i];

            if (m_pItemList[cItemID] != NULL)
            {

                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_GetSpriteRect(sX + 32 + m_pItemList[cItemID]->m_sX,
                    sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame);
                
                x1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.left;
                y1 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.top;
                x2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.right;
                y2 = (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_rcBound.bottom;

                if ((m_bIsItemDisabled[cItemID] == FALSE) && (m_bIsItemEquipped[cItemID] == FALSE) && (msX > x1) && (msX < x2) && (msY > y1) && (msY < y2))
                {

                    if (m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->_bCheckCollison(sX + 32 + m_pItemList[cItemID]->m_sX, sY + 44 + m_pItemList[cItemID]->m_sY, m_pItemList[cItemID]->m_sSpriteFrame, msX, msY) == TRUE)
                    {
                        
                        
                        _SetItemOrder(0, cItemID);

                        if ((m_bIsGetPointingMode == TRUE) && (m_iPointCommandType < 100) && (m_iPointCommandType >= 0) &&
                            (m_pItemList[m_iPointCommandType] != NULL) &&
                            (m_pItemList[m_iPointCommandType]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST) &&
                            (m_iPointCommandType != cItemID))
                        {

                            PointCommandHandler(NULL, NULL, cItemID);
                            //m_bCommandAvailable  = FALSE;
                            m_bIsGetPointingMode = FALSE;
                        }
                        else
                        {
                            
                            m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_ITEM;
                            m_stMCursor.sSelectedObjectID = cItemID;
                            m_stMCursor.sDistX = msX - x1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotX;
                            m_stMCursor.sDistY = msY - y1 + (short)m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[cItemID]->m_sSprite]->m_sPivotY;
                        }
                        return TRUE;
                    }
                }
            }
        }

    return FALSE;
}

BOOL CGame::_bCheckDlgBoxClick(short msX, short msY)
{
    int i;
    char         cDlgID;

    m_stMCursor.sZ = 0;
    for (i = 0; i < 41; i++)
        if (m_cDialogBoxOrder[40 - i] != NULL)
        {
            cDlgID = m_cDialogBoxOrder[40 - i];
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
                }

                return TRUE;
            }
        }

    return FALSE;
}

BOOL CGame::_bCheckDlgBoxDoubleClick(short msX, short msY)
{
    int i;
    char cDlgID;
    if (m_iHP <= 0) return FALSE;

    for (i = 0; i < 41; i++)
        if (m_cDialogBoxOrder[40 - i] != NULL)
        {
            cDlgID = m_cDialogBoxOrder[40 - i];
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
                return TRUE;
            }
        }
    return FALSE;
}

int CGame::_iCheckDlgBoxFocus(short msX, short msY, char cButtonSide)
{
    int i;
    char         cDlgID;
    short        sX, sY;
    DWORD		  dwTime = m_dwCurTime;

    if (cButtonSide == 1)
    {
        
        for (i = 0; i < 41; i++)
            if (m_cDialogBoxOrder[40 - i] != NULL)
            {
                cDlgID = m_cDialogBoxOrder[40 - i];
                if ((m_stDialogBoxInfo[cDlgID].sX <= msX) && ((m_stDialogBoxInfo[cDlgID].sX + m_stDialogBoxInfo[cDlgID].sSizeX) >= msX) &&
                    (m_stDialogBoxInfo[cDlgID].sY <= msY) && ((m_stDialogBoxInfo[cDlgID].sY + m_stDialogBoxInfo[cDlgID].sSizeY) >= msY))
                {
                    
                    EnableDialogBox(cDlgID, NULL, NULL, NULL);

                    m_stMCursor.sPrevX = msX;
                    m_stMCursor.sPrevY = msY;
                    m_stMCursor.sDistX = msX - m_stDialogBoxInfo[cDlgID].sX;
                    m_stMCursor.sDistY = msY - m_stDialogBoxInfo[cDlgID].sY;

                    
                    switch (cDlgID)
                    {
                    case 1:
                        
                        
                        if (bDlgBoxPress_Character(msX, msY) == FALSE)
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
                        
                        

                        
                        if (bDlgBoxPress_Inventory(msX, msY) == FALSE)
                        {
                            
                            m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                            m_stMCursor.sSelectedObjectID = cDlgID;
                        }
                        break;

                    case 10:
                        sX = m_stDialogBoxInfo[10].sX;
                        sY = m_stDialogBoxInfo[10].sY;
                        
                        if ((msX >= sX + 340) && (msX <= sX + 360) && (msY >= sY + 22) && (msY <= sY + 138))
                        {
                            m_stDialogBoxInfo[10].bIsScrollSelected = TRUE;
                            return -1;
                        }

                        if (m_stDialogBoxInfo[10].bIsScrollSelected == FALSE)
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
                            m_stDialogBoxInfo[11].bIsScrollSelected = TRUE;
                            return -1;
                        }

                        if ((m_stDialogBoxInfo[11].bIsScrollSelected == FALSE))
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
                            m_stDialogBoxInfo[14].bIsScrollSelected = TRUE;
                            return -1;
                        }

                        if (m_stDialogBoxInfo[14].bIsScrollSelected == FALSE)
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
                            m_stDialogBoxInfo[15].bIsScrollSelected = TRUE;
                            return -1;
                        }

                        if (m_stDialogBoxInfo[15].bIsScrollSelected == FALSE)
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
                            m_stDialogBoxInfo[18].bIsScrollSelected = TRUE;
                            return -1;
                        }

                        if (m_stDialogBoxInfo[18].bIsScrollSelected == FALSE)
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
                            m_stDialogBoxInfo[19].bIsScrollSelected = TRUE;
                            return -1;
                        }
                        if ((msX >= sX + 126) && (msX <= sX + 238) && (msY >= sY + 139) && (msY <= sY + 155))
                        {
                            m_stDialogBoxInfo[19].bIsScrollSelected = TRUE;
                            return -1;
                        }

                        if (m_stDialogBoxInfo[19].bIsScrollSelected == FALSE)
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
                            m_stDialogBoxInfo[21].bIsScrollSelected = TRUE;
                            return -1;
                        }

                        if (m_stDialogBoxInfo[21].bIsScrollSelected == FALSE)
                        {
                            m_stMCursor.cSelectedObjectType = DEF_SELECTEDOBJTYPE_DLGBOX;
                            m_stMCursor.sSelectedObjectID = cDlgID;
                        }
                        else return -1;
                        break;

                    case 26:
                        
                        if (bDlgBoxPress_SkillDlg(msX, msY) == FALSE)
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
            
            for (i = 0; i < 41; i++)
                if (m_cDialogBoxOrder[40 - i] != NULL)
                {
                    cDlgID = m_cDialogBoxOrder[40 - i];
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

BOOL   CGame::DrawObject_OnAttack(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iWeapon, iAdd, iShieldIndex, iMantleIndex;
    BOOL bInv = FALSE;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = TRUE; //Energy-Ball,Wyvern
   
    
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == FALSE)
    {

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            
            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                {
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
                }
                else
                {
                    m_pSprite[iWeaponIndex]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }

                
                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
            }

            
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (bInv == TRUE)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
            }

            
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (bInv == TRUE)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }

                
                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }
        }

        // Berserk 
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL   CGame::DrawObject_OnAttackMove(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iWeapon, iAdd, iShieldIndex, iMantleIndex, dx, dy, dsx, dsy;
    int cFrameMoveDots;
    BOOL bInv = FALSE, bDashDraw = FALSE;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;


    
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = TRUE; //Energy-Ball,Wyvern

   
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
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = TRUE;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = TRUE;
        else return FALSE;
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
        case 2: bDashDraw = TRUE; cFrameMoveDots = 26; break;
        case 3: bDashDraw = TRUE; cFrameMoveDots = 16; break;
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == FALSE)
    {

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            
            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }

                
                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
            }

            if (bInv == TRUE)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
            }

            if (bInv == TRUE)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }

                
                if (_tmp_cFrame == 3) m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame - 1, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            }
        }

        // Berserk 
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, -5, 0, 5, dwTime);

        if (bDashDraw == TRUE)
        {
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            if (iWeaponIndex != -1) m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dsx, sY + dsy, _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
            if (iShieldIndex != -1) m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dsx, sY + dsy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[10] - (m_wR[0] / 3), m_wG[10] - (m_wG[0] / 3), m_wB[10] - (m_wB[0] / 3), dwTime);
        }
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL   CGame::DrawObject_OnMagic(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iHelmIndex, iMantleIndex;
    BOOL bInv = FALSE;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = TRUE; //Energy-Ball,Wyvern

   
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
            bInv = TRUE;
        else
        {
           
            if (_tmp_iChatIndex != NULL)
            {
                if (m_pChatMsgList[_tmp_iChatIndex] != NULL)
                {
                    
                    m_pChatMsgList[_tmp_iChatIndex]->m_sX = sX;
                    m_pChatMsgList[_tmp_iChatIndex]->m_sY = sY;
                }
                else
                {
                    
                    m_pMapData->ClearChatMsg(indexX, indexY);
                }
            }
           

            return FALSE;
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == FALSE)
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
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
            }
            break;
        }

        if (bInv == TRUE)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
        else
        {
            if ((_tmp_sStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
        }
        
        SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }

        if (iMantleIndex != -1)
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 16 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        // Berserk 
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL   CGame::DrawObject_OnGetItem(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iHelmIndex, iMantleIndex;
    BOOL bInv = FALSE;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = TRUE; //Energy-Ball,Wyvern

   
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
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = TRUE;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = TRUE;
        else return FALSE;
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == FALSE)
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
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
            }
            break;
        }

        if (bInv == TRUE)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX, sY, _tmp_cFrame, dwTime);
        else
        {
            if ((_tmp_sStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
        }
        
        SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

        if (iUndiesIndex != -1)
        {
            if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
        }

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }
        }

        if (iPantsIndex != -1)
        {
            if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }
        }

        if (iArmArmorIndex != -1)
        {
            if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }
        }

        if (iBodyArmorIndex != -1)
        {
            if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }
        }

        if (iHelmIndex != -1)
        {
            if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }
        }

        if (iMantleIndex != -1)
        {
            if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
            else
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }

        // Berserk 
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL   CGame::DrawObject_OnDamage(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iWeaponIndex, iShieldIndex, iHelmIndex, iR, iG, iB;
    int iAdd, iDrawMode, iMantleIndex;
    char cFrame;
    BOOL bInv = FALSE;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = TRUE; //Energy-Ball,Wyvern

   
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
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = TRUE;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = TRUE;
        else return FALSE;
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == FALSE)
    {
        if (iDrawMode == 1)
        {

            if (_cDrawingOrder[_tmp_cDir] == 1)
            {
                
                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                   
                    switch (iWeaponGlare)
                    {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
                    }
                    break;
                }

                
                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

                if (bInv == TRUE)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_sStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
                }
                
                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

               
                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

               
                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                   
                    switch (iShieldGlare)
                    {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
                    }
                    break;
                }

                
                if (_tmp_sOwnerType == 35)
                    m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

                if (bInv == TRUE)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_sStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
                }
                
                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

               
                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

               
                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                   
                    switch (iShieldGlare)
                    {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                   
                    switch (iWeaponGlare)
                    {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }
            }

            // Berserk 
            if ((_tmp_sStatus & 0x20) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, cFrame, 0, -5, -5, dwTime);
            // Protection From Magic
            if ((_tmp_sStatus & 0x80) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, cFrame, -5, 0, 5, dwTime);
        }
        else
        {

            if (_cDrawingOrder[_tmp_cDir] == 1)
            {
                
                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                   
                    switch (iWeaponGlare)
                    {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
                    }
                    break;
                }

                if (bInv == TRUE)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_sStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
                }
                
                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

               
                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

               
                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                   
                    switch (iShieldGlare)
                    {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
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
                            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
                    }
                    break;
                }

                if (bInv == TRUE)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
                else
                {
                    if ((_tmp_sStatus & 0x40) != 0)
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
                }
                
                SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

                if ((iHairIndex != -1) && (iHelmIndex == -1))
                {
                    _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                    m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 1))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iPantsIndex != -1)
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }

                if (iArmArmorIndex != -1)
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }

                if ((iBootsIndex != -1) && (iSkirtDraw == 0))
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }

                if (iBodyArmorIndex != -1)
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }

                if (iHelmIndex != -1)
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }

               
                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iShieldIndex != -1)
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

                   
                    switch (iShieldGlare)
                    {
                    case 0: break;
                        //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                        
                    case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                    case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }

                if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }

                if (iWeaponIndex != -1)
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX, sY, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

                   
                    switch (iWeaponGlare)
                    {
                    case 0: break;
                    case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                    case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                    }
                }
            }

            // Berserk 
            if ((_tmp_sStatus & 0x20) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, cFrame, 0, -5, -5, dwTime);
            // Protection From Magic
            if ((_tmp_sStatus & 0x80) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, cFrame, -5, 0, 5, dwTime);
        }
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL CGame::DrawObject_OnDying(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iHelmIndex, iMantleIndex;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;
    char cFrame;

   
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == FALSE)
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
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, cFrame, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, cFrame, dwTime);
            }
            break;
        }

        if (_tmp_sOwnerType == 66) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, cFrame, dwTime);
        else
        {
            if ((_tmp_sStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, cFrame, dwTime);
        }
        
        SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }

        if (iMantleIndex != -1)
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        // Berserk 
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL   CGame::DrawObject_OnDead(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iR, iG, iB, iFrame, iMantleIndex, iHelmIndex;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    if (_tmp_sOwnerType == 66) return FALSE;
   
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

    if (bTrans == FALSE)
    {
        if (_tmp_cFrame == -1)
        {
            _tmp_cFrame = 7;

            if ((_tmp_sStatus & 0x40) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, iFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, iFrame, dwTime);
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

            if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

            if (iMantleIndex != -1)
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }
        }
        else
        {
            if ((_tmp_sStatus & 0x20) != 0)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, iFrame, -2 * _tmp_cFrame + 5, -2 * _tmp_cFrame - 5, -2 * _tmp_cFrame - 5, dwTime);
            else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, iFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, -2 * _tmp_cFrame, dwTime);
        }
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL   CGame::DrawObject_OnMove(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int dx, dy;
    int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iShieldIndex, iAdd, iMantleIndex;
    BOOL bInv = FALSE;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    
    //if(_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = TRUE; //Energy-Ball,Wyvern
    if (_tmp_sOwnerType == 35) bInv = TRUE; //Energy-Ball,Wyvern
    if (_tmp_sOwnerType == 66)
    {
        bInv = TRUE; //Energy-Ball,Wyvern
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
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = TRUE;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = TRUE;
        else return FALSE;
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (_tmp_sOwnerType == 65)
    {
        m_pEffectSpr[77]->PutTransSprite70(sX + dx, sY + dy, _tmp_cFrame, dwTime);
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

    if (bTrans == FALSE)
    {

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            
            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
            }

            
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX + dx, sY + dy, 1, dwTime);

            if (bInv == TRUE)
                //m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }
           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
            }

            
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX + dx, sY + dy, 1, dwTime);

            if (bInv == TRUE)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        // Berserk 
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL CGame::DrawObject_OnDamageMove(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int cFrame, cDir;
    int dx, dy;
    int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iShieldIndex, iMantleIndex;
    BOOL bInv = FALSE;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = TRUE; //Energy-Ball,Wyvern

   
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
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = TRUE;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = TRUE;
        else return FALSE;
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX + dy, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == FALSE)
    {

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            
            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, cFrame, dwTime);
                }
                break;
            }

            
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (bInv == TRUE)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, cFrame, dwTime);
                }
                break;
            }

            
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (bInv == TRUE)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX + dx, sY + dy, cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);
           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iPantsIndex != -1)
            {
                if (iPantsColor == 0)
                    m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
            }

            if (iArmArmorIndex != -1)
            {
                if (iArmColor == 0)
                    m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (iBootsColor == 0)
                    m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
            }

            if (iBodyArmorIndex != -1)
            {
                if (iArmorColor == 0)
                    m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
            }

            if (iHelmIndex != -1)
            {
                if (iHelmColor == 0)
                    m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iHelmIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iShieldIndex != -1)
            {
                if (iShieldColor == 0)
                    m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (iMantleColor == 0)
                    m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, dwTime);
                else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 4 + cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
            }

            if (iWeaponIndex != -1)
            {
                if (iWeaponColor == 0)
                    m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, cFrame, dwTime);
                else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        // Berserk 
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX + dx, sY + dy, cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX + dx, sY + dy, cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL CGame::DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    short dx, dy;
    int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iShieldIndex, iAdd, iMantleIndex;
    BOOL bInv = FALSE;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
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
                m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
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
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            break;
        }

        if (bInv == TRUE)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);

       
        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iUndiesIndex != -1)
        {
            if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
        }

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }
       
        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iShieldIndex != -1)
        {
            if (iShieldColor == 0)
                m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
        }

        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
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
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            break;
        }

        if (bInv == TRUE)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
        else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);

       
        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

        if ((iHairIndex != -1) && (iHelmIndex == -1))
        {
            _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
            m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 1))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iPantsIndex != -1)
        {
            if (iPantsColor == 0)
                m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
        }

        if (iArmArmorIndex != -1)
        {
            if (iArmColor == 0)
                m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
        }

        if ((iBootsIndex != -1) && (iSkirtDraw == 0))
        {
            if (iBootsColor == 0)
                m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
        }

        if (iBodyArmorIndex != -1)
        {
            if (iArmorColor == 0)
                m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
        }

        if (iHelmIndex != -1)
        {
            if (iHelmColor == 0)
                m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
        }

       
        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iShieldIndex != -1)
        {
            if (iShieldColor == 0)
                m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
        }

        if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
        {
            if (iMantleColor == 0)
                m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
        }

        if (iWeaponIndex != -1)
        {
            if (iWeaponColor == 0)
                m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
        }
    }

    if (_tmp_iChatIndex != NULL)
    {
        if (m_pChatMsgList[_tmp_iChatIndex] != NULL)
        {

            DrawChatMsgBox(sX + dx, sY + dy, _tmp_iChatIndex, FALSE);

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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

BOOL   CGame::DrawObject_OnStop(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int iBodyIndex, iUndiesIndex, iHairIndex, iBodyArmorIndex, iArmArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
    int iWeaponIndex, iShieldIndex, iMantleIndex;
    BOOL bInv = FALSE;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = TRUE; //Energy-Ball,Wyvern

   
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
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = TRUE;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = TRUE;
        else return FALSE;
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
        m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX, sY, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == FALSE)
    {

        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            
            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX, sY, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
            }

            
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (bInv) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

           
            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX, sY, _tmp_cFrame, dwTime);
                }
                break;
            }

            
            if (_tmp_sOwnerType == 35)
                m_pEffectSpr[0]->PutTransSprite(sX, sY, 1, dwTime);

            if (bInv) m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX, sY, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

           
            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13, sY - 34, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX, sY, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX, sY, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX, sY, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        if (_tmp_sOwnerType == 64)
        {
            switch (_tmp_cFrame)
            {
            case 0:
                m_pEffectSpr[84]->PutTransSprite(sX + 52, sY + 54, (dwTime % 3000) / 120, dwTime);
                break;
            case 1:
                m_pEffectSpr[83]->PutTransSprite(sX + 53, sY + 59, (dwTime % 3000) / 120, dwTime);
                break;
            case 2:
                m_pEffectSpr[82]->PutTransSprite(sX + 53, sY + 65, (dwTime % 3000) / 120, dwTime);
                break;
            }
        }
        // Berserk 
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, _tmp_cFrame, 0, -5, -5, dwTime);
        // Protection From Magic
        if ((_tmp_sStatus & 0x80) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX, sY, _tmp_cFrame, -5, 0, 5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX, sY, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX, sY, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;


    return FALSE;
}

BOOL   CGame::DrawObject_OnRun(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
    int dx, dy;
    int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iWeaponIndex, iShieldIndex, iHelmIndex, iR, iG, iB, iMantleIndex;
    BOOL bInv = FALSE;
    int iWeaponGlare, iShieldGlare;
    int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
    int iSkirtDraw = 0;

    
    if (_tmp_sOwnerType == 35 || _tmp_sOwnerType == 66) bInv = TRUE; //Energy-Ball,Wyvern

   
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
        if (memcmp(m_cPlayerName, _tmp_cName, 10) == 0) bInv = TRUE;
        else if (_iGetFOE(_tmp_sStatus) == 1) bInv = TRUE;
        else return FALSE;
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
        case 1: m_pEffectSpr[26]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Attack Effect
        case 2: m_pEffectSpr[27]->PutTransSprite(sX + dx, sY + dy, _tmp_iEffectFrame, dwTime); break; // Special Ability: Protect Effect
        }
    }

    if (bTrans == FALSE)
    {
        if (_cDrawingOrder[_tmp_cDir] == 1)
        {
            
            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
            }

            if (bInv == TRUE)
                //m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX+dx, sY+dy, _tmp_cFrame, dwTime);
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
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
                        m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                }
                break;
            }

            if (bInv == TRUE)
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite2(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            else
            {
                if ((_tmp_sStatus & 0x40) != 0)
                    m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wR[10] - m_wR[0] / 2, m_wG[10] - m_wG[0] / 2, m_wB[10] - m_wB[0] / 2, dwTime);
                else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
            }
            
            SetRect(&m_rcBodyRect, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top,
                m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right, m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom);

           
            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 0))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iUndiesIndex != -1)
            {
                if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
            }

            if ((iHairIndex != -1) && (iHelmIndex == -1))
            {
                _GetHairColorColor(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
                m_pSprite[iHairIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 1))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iPantsIndex != -1)
            {
                if (bInv) m_pSprite[iPantsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iPantsColor == 0)
                        m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iPantsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
                }
            }

            if (iArmArmorIndex != -1)
            {
                if (bInv) m_pSprite[iArmArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmColor == 0)
                        m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iArmArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
                }
            }

            if ((iBootsIndex != -1) && (iSkirtDraw == 0))
            {
                if (bInv) m_pSprite[iBootsIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iBootsColor == 0)
                        m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBootsIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
                }
            }

            if (iBodyArmorIndex != -1)
            {
                if (bInv) m_pSprite[iBodyArmorIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iArmorColor == 0)
                        m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iBodyArmorIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
                }
            }

            if (iHelmIndex != -1)
            {
                if (bInv) m_pSprite[iHelmIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iHelmColor == 0)
                        m_pSprite[iHelmIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iHelmIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
                }
            }

            //	
            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 2))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iShieldIndex != -1)
            {
                if (bInv) m_pSprite[iShieldIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iShieldColor == 0)
                        m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iShieldIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
                }

               
                switch (iShieldGlare)
                {
                case 0: break;
                    //case 1: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX, sY,  (_tmp_cDir-1) * 8 + _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                    
                case 1: m_pEffectSpr[45]->PutTransSprite(sX - 13 + dx, sY - 34 + dy, 0, dwTime);
                case 2: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iShieldIndex]->PutTransSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }

            if ((iMantleIndex != -1) && (_cMantleDrawingOrderOnRun[_tmp_cDir] == 1))
            {
                if (bInv) m_pSprite[iMantleIndex]->PutTransSprite25(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                else
                {
                    if (iMantleColor == 0)
                        m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
                    else m_pSprite[iMantleIndex]->PutSpriteColor(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
                }
            }

            if (iWeaponIndex != -1)
            {
                if (bInv) m_pSprite[iWeaponIndex]->PutTransSprite25(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                else
                {
                    if (iWeaponColor == 0)
                        m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
                    else m_pSprite[iWeaponIndex]->PutSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
                }

               
                switch (iWeaponGlare)
                {
                case 0: break;
                case 1: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, m_iDrawFlag, 0, 0, dwTime); break; // Red Glare
                case 2: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, m_iDrawFlag, 0, dwTime); break; // Green Glare
                case 3: m_pSprite[iWeaponIndex]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, 0, m_iDrawFlag, dwTime); break; // Blue Glare
                }
            }
        }

        // Berserk 
        if ((_tmp_sStatus & 0x20) != 0)
            m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSpriteColor(sX + dx, sY + dy, _tmp_cFrame, 0, -5, -5, dwTime);
    }
    else if (strlen(_tmp_cName) > 0)
    {
        if ((_tmp_sOwnerType >= 1) && (_tmp_sOwnerType <= 6)) DrawObjectName(sX + dx, sY + dy, _tmp_cName, _tmp_sStatus);
        else DrawNpcName(sX + dx, sY + dy, _tmp_sOwnerType, _tmp_sStatus);
    }

    if (_tmp_iChatIndex != NULL)
    {
        if ((m_pChatMsgList[_tmp_iChatIndex] != NULL) && (m_pChatMsgList[_tmp_iChatIndex]->m_iObjectID == _tmp_wObjectID))
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
        (m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;

    return FALSE;
}

void CGame::DrawObjectFOE(int ix, int iy, int iFrame)
{
    if (_iGetFOE(_tmp_sStatus) < 0)
    {
        if (iFrame <= 4) m_pEffectSpr[38]->PutTransSprite(ix, iy, iFrame, G_dwGlobalTime);
    }
}

void CGame::DrawObjectName(short sX, short sY, char * pName, int sStatus)
{
    char cTxt[64], cTxt2[64];
    short sR, sG, sB;
    int i, iGuildIndex, iFOE, iAddY = 0; 
    BOOL bPK, bCitizen, bAresden, bHunter;

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

    ZeroMemory(cTxt, sizeof(cTxt));
    ZeroMemory(cTxt2, sizeof(cTxt2));

    if (m_iIlusionOwnerH == NULL)
    {
        if (m_bIsCrusadeMode == FALSE) format_to_local(cTxt, "{}", pName);
        else
        {
            if (_tmp_wObjectID >= 10000) strcpy(cTxt, NPC_NAME_MERCENARY); 
            else
            {
                if (iFOE == -1) format_to_local(cTxt, "{}", _tmp_wObjectID);
                else strcpy(cTxt, pName);
            }
        }
        if (m_iPartyStatus != NULL)
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

    PutUnderEntityString(sX, sY, cTxt, Color(255, 255, 255));

    //PutString2(sX, sY, cTxt, 255, 255, 255);
    ZeroMemory(cTxt, sizeof(cTxt));

    if (memcmp(m_cPlayerName, pName, 10) == 0)
    {
        if (m_iGuildRank == 0)
        {
            format_to_local(G_cTxt, DEF_MSG_GUILDMASTER, m_cGuildName);
            PutUnderEntityString(sX, sY + 14, G_cTxt, Color(180, 180, 180));
            //PutString2(sX, sY + 14, G_cTxt, 180, 180, 180);
            iAddY = 14;
        }
        if (m_iGuildRank > 0)
        {
            format_to_local(G_cTxt, DEF_MSG_GUILDSMAN, m_cGuildName);
            PutUnderEntityString(sX, sY + 14, G_cTxt, Color(180, 180, 180));
            //PutString2(sX, sY + 14, G_cTxt, 180, 180, 180);
            iAddY = 14;
        }
        if (m_iPKCount != 0)
        {
            bPK = TRUE;
            sR = 255; sG = 0; sB = 0;
        }
        else
        {
            bPK = FALSE;
            sR = 30; sG = 200; sB = 30;
        }
        bCitizen = m_bCitizen;
        bAresden = m_bAresden;
        bHunter = m_bHunter;
    }
    else
    {
        if (sStatus & 0x8000) bPK = TRUE;
        else bPK = FALSE;
        if (sStatus & 0x4000) bCitizen = TRUE;
        else bCitizen = FALSE;
        if (sStatus & 0x2000) bAresden = TRUE;
        else bAresden = FALSE;
        if (sStatus & 0x1000) bHunter = TRUE;
        else bHunter = FALSE;
        if (m_bIsCrusadeMode == FALSE || iFOE >= 0)
        {
            if (FindGuildName(pName, &iGuildIndex) == TRUE)
            {
                if (m_stGuildName[iGuildIndex].cGuildName[0] != NULL)
                {	
                    if (strcmp(m_stGuildName[iGuildIndex].cGuildName, "NONE") != 0)
                    {
                        if (m_stGuildName[iGuildIndex].iGuildRank == 0)
                        {
                            format_to_local(G_cTxt, DEF_MSG_GUILDMASTER, m_stGuildName[iGuildIndex].cGuildName);
                            PutUnderEntityString(sX, sY + 14, G_cTxt, Color(180, 180, 180));
                            //PutString2(sX, sY + 14, G_cTxt, 180, 180, 180);
                            m_stGuildName[iGuildIndex].dwRefTime = m_dwCurTime;
                            iAddY = 14;
                        }
                        else if (m_stGuildName[iGuildIndex].iGuildRank > 0)
                        {
                            format_to_local(G_cTxt, DEF_MSG_GUILDSMAN, m_stGuildName[iGuildIndex].cGuildName);
                            PutUnderEntityString(sX, sY + 14, G_cTxt, Color(180, 180, 180));
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
            else bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQGUILDNAME, NULL, _tmp_wObjectID, iGuildIndex, NULL, NULL);
        }
    }

    if (bCitizen == FALSE)	strcpy(cTxt, DRAW_OBJECT_NAME60);
    else
    {
        if (bAresden)
        {
            //#if DEF_LANGUAGE > 2		// Korea 2.19
            if (bHunter == TRUE) strcpy(cTxt, DEF_MSG_ARECIVIL);
            else strcpy(cTxt, DEF_MSG_ARESOLDIER);
            //#else
            
            //#endif
        }
        else
        {
            //#if DEF_LANGUAGE > 2		// Korea 2.19
            if (bHunter == TRUE) strcpy(cTxt, DEF_MSG_ELVCIVIL);
            else strcpy(cTxt, DEF_MSG_ELVSOLDIER);
            //#else
            
            //#endif
        }
    }
    if (bPK == TRUE)
    {
        if (bCitizen == FALSE) strcpy(cTxt, DEF_MSG_PK);
        else
        {
            if (bAresden) strcpy(cTxt, DEF_MSG_AREPK);
            else strcpy(cTxt, DEF_MSG_ELVPK);
        }
    }
    //PutString2(sX, sY + 14 + iAddY, cTxt, sR, sG, sB);
    PutUnderEntityString(sX, sY + 14 + iAddY, cTxt, Color(sR, sG, sB));
}

void CGame::DrawNpcName(short sX, short sY, short sOwnerType, int sStatus)
{
    char cTxt[32], cTxt2[64];
    ZeroMemory(cTxt, sizeof(cTxt));
    ZeroMemory(cTxt2, sizeof(cTxt2));

    GetNpcName(sOwnerType, cTxt);
    if ((sStatus & 0x20) != 0) strcat(cTxt, DRAW_OBJECT_NAME50);//" Berserk" 
    if ((sStatus & 0x40) != 0) strcat(cTxt, DRAW_OBJECT_NAME51);//" Frozen"

    sY += 14;

    PutUnderEntityString(sX, sY, cTxt, Color(255, 255, 255));

    //PutString2(sX, sY, cTxt, 255, 255, 255);

    if (m_bIsObserverMode == TRUE) PutString2(sX, sY + 14, cTxt, 50, 50, 255);
    else if (m_bIsConfusion || (m_iIlusionOwnerH != NULL))
    {
        ZeroMemory(cTxt, sizeof(cTxt));
        strcpy(cTxt, DRAW_OBJECT_NAME87);
        PutUnderEntityString(sX, sY + 14, cTxt, Color(150, 150, 150));
        //PutString2(sX, sY + 14, cTxt, 150, 150, 150);
    }
    else
    {
        switch (_iGetFOE(sStatus))
        {
            case -2:
                PutUnderEntityString(sX, sY + 14, DRAW_OBJECT_NAME90, Color(255, 0, 0));
                //PutString2(sX, sY + 14, DRAW_OBJECT_NAME90, 255, 0, 0);
                break;
            case -1:
                PutUnderEntityString(sX, sY + 14, DRAW_OBJECT_NAME90, Color(255, 0, 0));
                //PutString2(sX, sY + 14, DRAW_OBJECT_NAME90, 255, 0, 0);
                break;
            case 0:
                PutUnderEntityString(sX, sY + 14, DRAW_OBJECT_NAME88, Color(50, 50, 255));
                //PutString2(sX, sY + 14, DRAW_OBJECT_NAME88, 50, 50, 255);
                break;
            case 1:
                PutUnderEntityString(sX, sY + 14, DRAW_OBJECT_NAME89, Color(30, 255, 30));
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
//     if (m_Misc.bCheckIMEString(cTxt2)) PutString_SprFont3(sX, sY + 28, cTxt2, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, FALSE, 2);
//     else PutString2(sX, sY + 28, cTxt2, 240, 240, 70);
    PutUnderEntityString(sX, sY + 28, cTxt2, Color(240, 240, 70));
}
