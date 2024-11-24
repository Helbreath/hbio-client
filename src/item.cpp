//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Item.h"
#include <memory>

CItem::CItem()
{
    memset(m_cName, 0, sizeof(m_cName));
    m_sSprite = 0;
    m_sSpriteFrame = 0;
    m_dwAttribute = 0;
    m_cItemColor = 0;
    m_cItemType = 0;
}

CItem::~CItem()
{

}
