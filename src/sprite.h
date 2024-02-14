//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

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
    bool _pMakeSpriteSurface();
    sf::Texture _localimage;
    sf::Image _image;
    //sf::Texture * _localshadow;
    //sf::Texture ** subtextures;

    sprite(std::ifstream & hPakFile, std::string & cPakFileName, short sNthFile, bool bAlphaEffect = true);
    ~sprite();
    static sprite * CreateSprite(std::string cPakFileName, short sNthFile, bool bAlphaEffect = true);
    void DrawSubSprite(int sX, int sY, int sFrame, uint64_t dwTime = 0, Color color = Color(255, 255, 255));
    void DrawSpriteNCK(int sX, int sY, int sFrame, uint64_t dwTime = 0, Color color = Color(255, 255, 255));
    void DrawRGBNCK(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255));
    void DrawSprite(int sX, int sY, int sFrame, uint64_t dwTime = 0, Color color = Color(255, 255, 255));
    void DrawScaledSprite(int sX, int sY, int sFrame, int sWidth, int sHeight, uint64_t dwTime = 0, Color color = Color(255, 255, 255));
    void DrawColor(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255));
    void draw_to(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255), int draw_mode = DS_VISIBLE);
    void DrawWidth(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime, Color color = Color(255, 255, 255));
    void DrawShadow(int sX, int sY, int sFrame, uint64_t dwTime, Color color = Color(255, 255, 255));
    void CreateShadow();

    void PutSpriteColor(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime);

    void PutSpriteFast(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutShiftSpriteFast(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime);
    void PutShiftTransSprite2(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime);
    void PutSpriteFastFrontBuffer(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutSpriteFastWidth(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime);
    void PutSpriteFastNoColorKey(int sX, int sY, int sFrame, uint64_t dwTime);

    void PutTransSprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth = 30);
    void PutTransSprite2(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth = 0);
    void PutTransSpriteRGB_NoColorKey(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime);
    void PutTransSpriteColor(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime);
    void PutTransSprite70(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite50(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite25(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite70_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite50_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutTransSprite25_NoColorKey(int sX, int sY, int sFrame, uint64_t dwTime);

    void PutTransSpriteColor(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime);
    void PutSpriteColor(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime);

    void PutShadowSpriteClip(int sX, int sY, int sFrame, uint64_t dwTime);
    void PutShadowSprite(int sX, int sY, int sFrame, uint64_t dwTime);

    void PutRevTransSprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth = 0);

    void PutFadeSprite(short sX, short sY, short sFrame, uint64_t dwTime);
    void PutFadeSpriteDst(uint16_t * pDstAddr, short sPitch, short sX, short sY, short sFrame, uint64_t dwTime);

    bool _bCheckCollison(int sX, int sY, short sFrame, int msX, int msY);
    void _GetSpriteRect(int sX, int sY, int sFrame);
    bool _iOpenSprite() { return _pMakeSpriteSurface(); }
    void _iCloseSprite() { /*OutputDebugStringW(("Unloaded image: " + m_cPakFileName + "\n").c_str()); if (_localimage) _localimage->drop(); m_bIsSurfaceEmpty = TRUE;*/ }

    RECT m_rcBound;
    uint64_t m_dwRefTime = 0;
    bool m_bIsSurfaceEmpty;
    bool m_bOnCriticalSection;
    bool m_bAlphaEffect;
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
