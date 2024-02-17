//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <cstdint>
#include "ActionID.h"

// #define MAPDATASIZEX	40
// #define MAPDATASIZEY	35
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
    void GetOwnerStatusByObjectID(uint16_t wObjectID, char * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pStatus, int * pColor, char * pName);
    void ClearDeadChatMsg(short sX, short sY);
    void ClearChatMsg(short sX, short sY);
    void ShiftMapData(char cDir);
    void _bDecodeMapInfo(char * pHeader);
    bool __fastcall bSetChatMsgOwner(uint16_t wObjectID, short sX, short sY, int iIndex);
    bool __fastcall bSetDeadOwner(uint16_t wObjectID, short sX, short sY, short sType, char cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, int sStatus, char * pName);
    bool __fastcall bGetDeadOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, char * pFrame, char * pName, short * pItemSprite, short * pItemSpriteFrame, int * pCharIndex) const;
    bool __fastcall get_owner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, int * pStatus, char * pName, char * pAction, char * pFrame, int * pChatIndex, short * pV1, short * pV2) const;
    bool set_owner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, int sStatus, char * pName, short sAction, short sV1, short sV2, short sV3, int iPreLoc = 0, int iFrame = 0);
    bool __fastcall get_owner(short sX, short sY, char * pName, short * pOwnerType, int * pOwnerStatus, uint16_t * pObjectID);
    bool bSetDynamicObject(short sX, short sY, uint16_t wID, short sType, bool bIsEvent);
    bool is_teleport(short x, short y) const;
    bool is_locateable(short x, short y) const;
    bool bSetItem(short sX, short sY, short sItemSpr, short sItemSprFrame, char cItemColor, bool bDropEffect = true);
    int  object_frame_counter(char * cPlayerName, short sViewPointX, short sViewPointY, bool self_only = false);

    CTile ** m_pData;
    CTile ** m_pTmpData;
    CTileSpr ** m_tile;
    CGame * m_pGame;

    struct
    {
        short m_sMaxFrame;
        short m_sFrameTime;
    } m_stFrame[DEF_TOTALCHARACTERS][DEF_TOTALACTION]{};
    uint32_t m_dwFrameTime{};
    uint32_t m_dwDOframeTime{};
    uint32_t m_dwFrameCheckTime{};
    int m_iObjectIDcacheLocX[30000]{};
    int m_iObjectIDcacheLocY[30000]{};
    uint32_t m_dwFrameAdjustTime{};
    short m_sMapSizeX{}, m_sMapSizeY{};
    short m_sRectX{}, m_sRectY{};
    short m_sPivotX{}, m_sPivotY{};
};
