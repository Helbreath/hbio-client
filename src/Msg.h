//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <windows.h>
#include <cstring>
#include <winbase.h>
#include <string_view>

class CMsg  
{
public:
	CMsg(char cType, const char * pMsg, DWORD dwTime);
	CMsg(char cType, std::string_view msg, DWORD dwTime);
	CMsg(char cType, std::string & msg, DWORD dwTime);
	~CMsg() = default;

	char m_cType;
	short m_sX, m_sY;
	DWORD m_dwTime;

    std::string message;

	int m_iObjectID;
};
