//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <windows.h>

class CItemName  
{
public:
	CItemName();
	~CItemName();

	char m_cOriginName[21];
	char m_cName[34];
};
