//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once
#include <windows.h>

class CStrTok  
{
public:
	CStrTok(char * pData, char * pSeps);
	~CStrTok();
	char * pGet();
	bool _bIsSeperator(char cData, char cNextData);

	char * m_pData, * m_pSeps, m_cToken[256];
	int    m_iDataLength, m_iCurLoc;
};
