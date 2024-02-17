//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Skill.h"
#include <memory>

CSkill::CSkill()
{
    memset(m_cName, 0, sizeof(m_cName));

    m_iLevel = 0;
    m_bIsUseable = false;
    m_cUseMethod = 0;
}

CSkill::~CSkill()
{

}
