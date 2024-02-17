//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <windows.h>
#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <cstdint>
#include "defines.h"
#include "GlobalDef.h"

using sf::Color;

using stBrush = struct stBrushtag
{
    short sx;
    short sy;
    short szx;
    short szy;
    short pvx;
    short pvy;
};

class sprite
{
public:
    bool make_sprite_surface_();
    sf::Texture _localimage;
    sf::Image _image;
    //sf::Texture * _localshadow;
    //sf::Texture ** subtextures;

    sprite(std::ifstream & hPakFile, std::string & cPakFileName, short sNthFile, bool bAlphaEffect = true);
    ~sprite();
    static sprite * create_sprite(std::string cPakFileName, short sNthFile, bool bAlphaEffect = true);
    void draw_sub_sprite(int sX, int sY, int sFrame, uint64_t dwTime = 0, Color color = Color(255, 255, 255));
    void draw_sprite_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime = 0, Color color = Color(255, 255, 255));
    void draw_rgb_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255));
    void draw_sprite(int sX, int sY, int sFrame, uint64_t dwTime = 0, Color color = Color(255, 255, 255));
    void draw_scaled_sprite(int sX, int sY, int sFrame, int sWidth, int sHeight, uint64_t dwTime = 0, Color color = Color(255, 255, 255));
    void draw_sprite_colored(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255));
    void draw_to(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255), int draw_mode = DS_VISIBLE);
    void draw_sprite_width(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime, Color color = Color(255, 255, 255));
    void draw_shadow(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255));
    void create_shadow_sprite();

    void put_sprite_color(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime);

    void put_sprite_fast(int sX, int sY, int sFrame, uint64_t dwTime);
    void put_shift_sprite_fast(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime);
    void put_shift_trans_sprite2(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime);
    void put_sprite_fast_front_buffer(int sX, int sY, int sFrame, uint64_t dwTime);
    void put_sprite_fast_width(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime);
    void put_sprite_fast_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime);

    void put_trans_sprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth = 30);
    void put_trans_sprite2(int sX, int sY, int sFrame, uint64_t dwTime);
    void put_trans_sprite_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth = 0);
    void put_trans_sprite_rgb_no_color_key(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime);
    void put_trans_sprite_color(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime);
    void put_trans_sprite70(int sX, int sY, int sFrame, uint64_t dwTime);
    void put_trans_sprite50(int sX, int sY, int sFrame, uint64_t dwTime);
    void put_trans_sprite25(int sX, int sY, int sFrame, uint64_t dwTime);
    void put_trans_sprite70_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime);
    void put_trans_sprite50_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime);
    void put_trans_sprite25_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime);

    void put_trans_sprite_color(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime);
    void put_sprite_color(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime);

    void put_shadow_sprite_clip(int sX, int sY, int sFrame, uint64_t dwTime);
    void put_shadow_sprite(int sX, int sY, int sFrame, uint64_t dwTime);

    void put_reverse_trans_sprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth = 0);

    void put_fade_sprite(short sX, short sY, short sFrame, uint64_t dwTime);
    void put_fade_sprite_destination(uint16_t * pDstAddr, short sPitch, short sX, short sY, short sFrame, uint64_t dwTime);

    bool check_collison(int sX, int sY, short sFrame, int msX, int msY);
    void get_sprite_rect(int sX, int sY, int sFrame);
    bool open_sprite_() { return make_sprite_surface_(); }
    void close_sprite_() { }

    RECT m_rcBound{};
    uint64_t m_dwRefTime = 0;
    bool m_bIsSurfaceEmpty{};
    bool m_bOnCriticalSection{};
    bool m_bAlphaEffect{};
    short m_sPivotX = 0, m_sPivotY = 0;
    uint16_t * m_pSurfaceAddr = nullptr;
    uint64_t m_dwBitmapFileStartLoc = 0;
    short m_sPitch = 0;
    uint64_t m_iTotalFrame = 0;
    char m_cAlphaDegree = 0;
    uint16_t m_wBitmapSizeX = 0, m_wBitmapSizeY = 0;
    uint16_t m_wColorKey = 0;
    std::string m_cPakFileName;
    stBrush * brush = nullptr;
    uint64_t wPageid = 0;

    sf::Sprite * sprite_ = nullptr;
};
