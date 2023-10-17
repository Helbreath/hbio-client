//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Skill.h"

CSkill::CSkill()
{
	ZeroMemory(m_cName, sizeof(m_cName));

	m_iLevel = 0;
	m_bIsUseable = FALSE;
	m_cUseMethod = NULL;
}

CSkill::~CSkill()
{

}
