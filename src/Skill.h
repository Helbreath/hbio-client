//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

class CSkill
{
public:
    CSkill();
    ~CSkill();

    char m_cName[21];

    int  m_iLevel;
    bool m_bIsUseable;
    char m_cUseMethod;
};
