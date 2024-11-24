//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <cstdint>
#include "action_id.h"
#include <mutex>

#define MAPDATASIZEX 950
#define MAPDATASIZEY 950

class CTile;
class CTileSpr;
class CGame;

class CMapData
{
public:
    CMapData(CGame * pGame);
    ~CMapData();
    void Init();
    void OpenMapDataFile(char * cFn);
    void GetOwnerStatusByObjectID(uint16_t wObjectID, char * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pStatus, int * pColor, char * pName) const;
    void ClearDeadChatMsg(short sX, short sY);
    void ClearChatMsg(short sX, short sY);
    void _bDecodeMapInfo(char * pHeader);
    bool bSetChatMsgOwner(uint16_t wObjectID, short sX, short sY, int iIndex);
    bool bSetDeadOwner(uint16_t wObjectID, short sX, short sY, short sType, char cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, int sStatus, char * pName);
    bool bGetDeadOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, char * pFrame, char * pName, short * pItemSprite, short * pItemSpriteFrame, int * pCharIndex);
    bool bGetOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, int * pStatus, char * pName, char * pAction, char * pFrame, int * pChatIndex, short * pV1, short * pV2);
    bool bSetOwner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, int sStatus, char * pName, short sAction, short sV1, short sV2, short sV3, int iPreLoc = 0, int iFrame = 0);
    bool bGetOwner(short sX, short sY, char * pName, short * pOwnerType, int * pOwnerStatus, uint16_t * pObjectID) const;
    bool bSetDynamicObject(short sX, short sY, uint16_t wID, short sType, bool bIsEvent);
    bool bIsTeleportLoc(short x, short y) const;
    bool bGetIsLocateable(short x, short y) const;
    bool bSetItem(short sX, short sY, short sItemSpr, short sItemSprFrame, char cItemColor, bool bDropEffect = true);
    void object_frame_counter(short sViewPointX, short sViewPointY, bool self_only = false);

    CTile ** m_pData;
    CTile ** m_pTmpData;
    CTileSpr ** m_tile;
    CGame * m_pGame;

    struct
    {
        short m_sMaxFrame;
        int64_t m_sFrameTime;
    } m_stFrame[DEF_TOTALCHARACTERS][DEF_TOTALACTION]{};
    int64_t m_dwFrameTime{};
    int64_t m_dwDOframeTime{};
    int64_t m_dwFrameCheckTime{};

    // one server managing all maps will generate lots of npcs - this needs to be adjusted
    // todo: use std::set or something - for now use something higher than uint16_t
    int m_iObjectIDcacheLocX[70000]{};
    int m_iObjectIDcacheLocY[70000]{};
    int64_t m_dwFrameAdjustTime{};
    short m_sMapSizeX{}, m_sMapSizeY{};
    short m_sRectX{}, m_sRectY{};
    mutable std::mutex map_mut;
};
