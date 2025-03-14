//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <cstdint>

class CTile
{
public:
    void Clear();

    CTile() = default;
    ~CTile() = default;

    int64_t m_dwOwnerTime{};
    int64_t m_dwEffectTime{};
    int64_t m_dwDeadOwnerTime{};
    int64_t m_dwDynamicObjectTime{};

    int m_iChatMsg{};
    int m_cItemColor{};
    int m_iEffectType{};
    int m_iDeadApprColor{};
    int m_iEffectFrame{}, m_iEffectTotalFrame{};
    int m_iApprColor{};
    int m_iDeadChatMsg{};

    uint16_t m_wDeadObjectID{};
    uint16_t m_wObjectID{};

    short m_sOwnerType{};
    short m_sAppr1{};
    short m_sAppr2{};
    short m_sAppr3{};
    short m_sAppr4{};
    int m_iStatus{};

    short m_sDeadOwnerType{};
    short m_sDeadAppr1{};
    short m_sDeadAppr2{};
    short m_sDeadAppr3{};
    short m_sDeadAppr4{};

    int m_iDeadStatus{};
    short m_sV1{};
    short m_sV2{};
    short m_sV3{};
    short m_sDynamicObjectType{};

    short m_sItemSprite{};
    short m_sItemSpriteFrame{};

    char  m_cDeadOwnerFrame = -1;
    char  m_cOwnerAction{};
    char  m_cOwnerFrame{};
    uint16_t max_frames{};
    int64_t frame_time{};
    int64_t start_time{};
    char  m_cDir{};
    char  m_cDeadDir{};

    char  m_cDynamicObjectFrame{};
    char  m_cDynamicObjectData1{}, m_cDynamicObjectData2{}, m_cDynamicObjectData3{}, m_cDynamicObjectData4{};
    char  m_cOwnerName[12]{};
    char  m_cDeadOwnerName[12]{};
};
