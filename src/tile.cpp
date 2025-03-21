//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "tile.h"
#include <memory>

void CTile::Clear()
{
    m_wObjectID = 0;
    m_wDeadObjectID = 0;

    m_sOwnerType = 0;
    memset(m_cOwnerName, 0, sizeof(m_cOwnerName));

    m_sDeadOwnerType = 0;
    memset(m_cDeadOwnerName, 0, sizeof(m_cDeadOwnerName));

    m_cDeadOwnerFrame = -1;
    m_dwDeadOwnerTime = 0;

    m_cOwnerAction = 0;
    m_cDir = 0;
    m_cOwnerFrame = 0;

    m_sItemSprite = 0;
    m_sItemSpriteFrame = 0;
    m_cItemColor = 0;

    m_sDynamicObjectType = 0;
    m_cDynamicObjectFrame = 0;

    m_iChatMsg = 0;
    m_iDeadChatMsg = 0;

    m_iStatus = 0;
    m_iDeadStatus = 0;

    m_sV1 = 0;
    m_sV2 = 0;
    m_sV3 = 0;

    m_sAppr1 = 0;
    m_sAppr2 = 0;
    m_sAppr3 = 0;
    m_sAppr4 = 0;
    m_iApprColor = 0;

    m_iEffectType = 0;
    m_iEffectFrame = 0;
    m_iEffectTotalFrame = 0;
    m_dwEffectTime = 0;

    m_dwOwnerTime = 0;
}
