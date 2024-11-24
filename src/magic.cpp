//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Magic.h"
#include <memory>

CMagic::CMagic()
{
    memset(m_cName, 0, sizeof(m_cName));
    m_sValue1 = 0;
    m_sValue2 = 0;
    m_sValue3 = 0;
    m_bIsVisible = true;
}

CMagic::~CMagic()
{
}
