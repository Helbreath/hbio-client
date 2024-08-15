//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <cstdint>
#include <cstring>
#include <string_view>

class CMsg
{
public:
    CMsg(char cType, const char * pMsg, uint64_t dwTime);
    CMsg(char cType, std::string_view msg, uint64_t dwTime);
    CMsg(char cType, std::string & msg, uint64_t dwTime);
    ~CMsg() = default;

    char m_cType;
    short m_sX, m_sY;
    uint64_t m_dwTime;

    std::string message;

    int m_iObjectID;

    int m_iGM = 0;
};
