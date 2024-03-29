//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#define DEF_MAGICTYPE_DAMAGE_SPOT		1
#define DEF_MAGICTYPE_HPUP_SPOT			2
#define DEF_MAGICTYPE_DAMAGE_AREA		3
#define DEF_MAGICTYPE_SPDOWN_SPOT		4
#define DEF_MAGICTYPE_SPDOWN_AREA		5
#define DEF_MAGICTYPE_SPUP_SPOT			6
#define DEF_MAGICTYPE_SPUP_AREA			7
#define DEF_MAGICTYPE_TELEPORT			8
#define DEF_MAGICTYPE_SUMMON			9 
#define DEF_MAGICTYPE_CREATE			10
#define DEF_MAGICTYPE_PROTECT			11
#define DEF_MAGICTYPE_HOLDOBJECT		12
#define DEF_MAGICTYPE_INVISIBILITY		13
#define DEF_MAGICTYPE_CREATE_DYNAMIC	14
#define DEF_MAGICTYPE_POSSESSION		15 
#define DEF_MAGICTYPE_CONFUSE			16 
#define DEF_MAGICTYPE_POISON			17 
#define DEF_MAGICTYPE_BERSERK			18
#define DEF_MAGICTYPE_POLYMORPH			20
#define DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT	21
#define DEF_MAGICTYPE_TREMOR				22 
#define DEF_MAGICTYPE_ICE					23 

class CMagic
{
public:
    CMagic();
    ~CMagic();
    char m_cName[31];
    int  m_sValue1, m_sValue2, m_sValue3;
    bool m_bIsVisible;
};
