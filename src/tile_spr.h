//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

class CTileSpr
{
public:
    CTileSpr();
    ~CTileSpr() = default;
    short m_sTileSprite;
    short m_sTileSpriteFrame;
    short m_sObjectSprite;
    short m_sObjectSpriteFrame;
    bool m_bIsMoveAllowed;
    bool m_bIsTeleport;
};
