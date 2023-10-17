//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "ItemName.h"

CItemName::CItemName()
{
	ZeroMemory(m_cOriginName, sizeof(m_cOriginName));
	ZeroMemory(m_cName, sizeof(m_cName));
}

CItemName::~CItemName()
{

}


