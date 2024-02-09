//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Msg.h"

CMsg::CMsg(char cType, const char * pMsg, uint64_t dwTime)
{
	m_cType = cType;
    message = pMsg;
	m_dwTime = dwTime;
	m_iObjectID = -1;
}

CMsg::CMsg(char cType, std::string_view msg, uint64_t dwTime)
{
    m_cType = cType;
    message = msg;
    m_dwTime = dwTime;
    m_iObjectID = -1;
}

CMsg::CMsg(char cType, std::string & msg, uint64_t dwTime)
{
    m_cType = cType;
    message = msg;
    m_dwTime = dwTime;
    m_iObjectID = -1;
}
