//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "sprite.h"
#include "game.h"
#include <sodium/crypto_secretstream_xchacha20poly1305.h>
#include <fmt/format.h>

extern char G_cSpriteAlphaDegree;
extern bool isrunning;

extern CGame * game;

#ifndef WIN32
typedef struct tagBITMAPINFOHEADER
{
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER, * LPBITMAPINFOHEADER, * PBITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
} BITMAPINFO, * LPBITMAPINFO, * PBITMAPINFO;
#endif

sprite::sprite(std::ifstream & hPakFile, std::string & cPakFileName, short sNthFile, bool bAlphaEffect)
{
    brush = 0;
    m_bIsSurfaceEmpty = true;

    m_cAlphaDegree = 1;
    m_bOnCriticalSection = false;
    m_iTotalFrame = 0;

    sprite_ = nullptr;

    m_cPakFileName = cPakFileName;
    m_bAlphaEffect = bAlphaEffect;
    wPageid = sNthFile;
}

sprite::~sprite()
{
    if (brush != nullptr)
        delete[] brush;
    if (sprite_)
        delete[] sprite_;
}

bool sprite::make_sprite_surface_()
{
    if (/*(m_stBrush == 0) || */ (!m_bIsSurfaceEmpty) || !isrunning)
    {
        return false;
    }

    unsigned char * m_lpDib;
    std::stringstream ss;
    ss << "sprites\\" << m_cPakFileName << ".pak";
    std::ifstream szfile(ss.str().c_str(), std::ios::in | std::ios::binary);
    if (!szfile.is_open())
    {
        return false;
    }

    // replaces object constructor
    if (!sprite_)
    {
        uint64_t iASDstart = 0;
        szfile.seekg(24 + wPageid * 8, std::ios::beg);
        szfile.read((char *)&iASDstart, 4);
        szfile.seekg(iASDstart + 100, std::ios::beg);
        szfile.read((char *)&m_iTotalFrame, 4);

        m_dwBitmapFileStartLoc = iASDstart + (108 + (12 * m_iTotalFrame));
        brush = new stBrush[m_iTotalFrame];
        szfile.read((char *)brush, 12 * m_iTotalFrame);

        sprite_ = new sf::Sprite[m_iTotalFrame];
    }
    //////////////////////////////////////////////////////////////////////////

    szfile.seekg(m_dwBitmapFileStartLoc, std::ios::beg);

    uint32_t filesize = 0;
    uint32_t encryption = 0;
    unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES] = {};
    szfile.read((char *)&filesize, 4);
    m_lpDib = (unsigned char *)new unsigned char[filesize];
    szfile.read((char *)&encryption, 4);
    if (encryption == 0x3f8ebc3d)
        szfile.read((char *)header, 24);
    szfile.read((char *)m_lpDib, filesize);

    //check for invalid frames
    if (filesize > 100'000'000)
        __debugbreak();

    int a = (int)szfile.gcount();
    szfile.close();
    if (encryption == 0x3f8ebc3d)
    {
        if (!game->has_key)
        {
            //error, has not received decryption key
            MessageBoxA(0, fmt::format("Cannot open {}", m_cPakFileName).c_str(), "Error", MB_OK);
            delete[] m_lpDib;
            isrunning = false;
            return false;
        }
        unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES] = { 0xf4, 0xb7, 0xdd, 0x28, 0xc5, 0xaf, 0xff, 0x3f, 0xdf, 0xd5, 0xa2, 0x9d, 0x68, 0x58, 0xde, 0x91, 0xd7, 0x4c, 0x4c, 0xf5, 0xbe, 0x74, 0xc3, 0xa7, 0xc9, 0xf3, 0x5, 0x81, 0x43, 0xee, 0x6f, 0x71 };
        unsigned char * decrypted = new unsigned char[(int)(filesize * 1.5)];
        unsigned long long decrypted_len;

        if (crypto_aead_xchacha20poly1305_ietf_decrypt(decrypted, &decrypted_len, nullptr, m_lpDib, filesize, nullptr, 0, header, /*game->*/key) != -1)
        {
            delete[] m_lpDib;
            m_lpDib = new unsigned char[decrypted_len];
            memcpy(m_lpDib, decrypted, decrypted_len);
            delete[] decrypted;
        }
        else
        {
            // error decrypting
            delete[] decrypted;
        }
    }
    else if (encryption == 0xa8b73afe)
    {
        //do nothing
    }

    sf::MemoryInputStream is;
    is.open((char *)m_lpDib, filesize);

    _image.loadFromStream(is);

    if (!_localimage.loadFromImage(_image))
    {
        std::cout << "Failed to load " << m_cPakFileName << "\n";
        delete[] m_lpDib;
        return false;
    }
    delete[] m_lpDib;

    for (int i = 0; i < m_iTotalFrame; ++i)
    {
        sprite_[i].setTexture(_localimage);
        sprite_[i].setTextureRect(sf::IntRect(brush[i].sx, brush[i].sy, brush[i].szx, brush[i].szy));
    }

    m_bIsSurfaceEmpty = false;

    //CreateShadow();

    return true;
}

sprite * sprite::create_sprite(std::string cPakFileName, short sNthFile, bool bAlphaEffect)
{
    std::ifstream szfile(fmt::format("sprites\\{}.pak", cPakFileName), std::ios::in | std::ios::binary);

    if (!szfile.is_open())
    {
        //error
        //MessageBoxW(0, ("Error loading pak: " + str).c_str(), "ERROR", MB_OK);
    }

    return new sprite(szfile, cPakFileName, sNthFile, bAlphaEffect);
}

void sprite::draw_shadow(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    // 	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;

    // 	test newSprite;
    // 	newSprite.setTexture(*sprite[sFrame].getTexture());

    // 	newSprite[sFrame].setColor(Color(0,0,0,127));
    // 	newSprite[sFrame].setPosition(sX + m_stBrush[sFrame].pvx, sY + m_stBrush[sFrame].pvy);
    // 	newSprite[sFrame].trythis();
    //     game->draw(newSprite[sFrame]);

    // 	if (m_bIsSurfaceEmpty) if (_iOpenSprite() == false) return;
    // 	game->driver->draw2DImage(_localshadow, core::position2d<s32>(sX+m_stBrush[sFrame].pvx,sY+m_stBrush[sFrame].pvy),
    // 		core::rect<s32>(m_stBrush[sFrame].sx,m_stBrush[sFrame].sy,m_stBrush[sFrame].sx+m_stBrush[sFrame].szx,m_stBrush[sFrame].sy+m_stBrush[sFrame].szy), 0,
    // 		color, true);
}

void sprite::draw_sub_sprite(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    if (m_bIsSurfaceEmpty && open_sprite_() == false)
        return;
    sprite_[sFrame].setColor(color);
    sprite_[sFrame].setPosition(float(sX + brush[sFrame].pvx), float(sY + brush[sFrame].pvy));
    game->draw(sprite_[sFrame]);
}

void sprite::draw_sprite_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    draw_rgb_no_color_key(sX, sY, sFrame, dwTime, color);
}

void sprite::draw_rgb_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    if (m_bIsSurfaceEmpty && open_sprite_() == false)
        return;
    if ((uint32_t)sFrame >= m_iTotalFrame)
    {
        std::cout << "Sprite out_of_bounds! - " << sFrame << " - " << m_cPakFileName << '\n';
        return;
    }
    sprite_[sFrame].setColor(color);
    sprite_[sFrame].setPosition(float(sX + brush[sFrame].pvx), float(sY + brush[sFrame].pvy));
    game->draw(sprite_[sFrame]);

    short dX, dY, sx, sy, szx, szy, pvx, pvy;

    sx = brush[sFrame].sx;
    sy = brush[sFrame].sy;
    szx = brush[sFrame].szx;
    szy = brush[sFrame].szy;
    pvx = brush[sFrame].pvx;
    pvy = brush[sFrame].pvy;

    dX = sX + pvx;
    dY = sY + pvy;

    m_rcBound.left = dX;
    m_rcBound.top = dY;
    m_rcBound.right = dX + szx;
    m_rcBound.bottom = dY + szy;
}

void sprite::draw_sprite(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    draw_sprite_colored(sX, sY, sFrame, dwTime, color);
}

void sprite::draw_sprite_colored(int sX, int sY, int sFrame, uint64_t dwTime, Color color)
{
    if (this == nullptr)
    {
        // todo: find a better solution - like showing what doesn't exist
        //std::cout << "Sprite does not exist!\n";
        return;
    }
    if (m_bIsSurfaceEmpty && open_sprite_() == false)
        return;
    if (sFrame >= m_iTotalFrame)
    {
        //std::cout << "Sprite out_of_bounds! - " << sFrame << " - " << m_cPakFileName << '\n';
        return;
    }
    sprite_[sFrame].setColor(color);
    sprite_[sFrame].setPosition(float(sX + brush[sFrame].pvx), float(sY + brush[sFrame].pvy));
    game->draw(sprite_[sFrame]);

    short dX, dY, sx, sy, szx, szy, pvx, pvy;

    sx = brush[sFrame].sx;
    sy = brush[sFrame].sy;
    szx = brush[sFrame].szx;
    szy = brush[sFrame].szy;
    pvx = brush[sFrame].pvx;
    pvy = brush[sFrame].pvy;

    dX = sX + pvx;
    dY = sY + pvy;

    m_rcBound.left = dX;
    m_rcBound.top = dY;
    m_rcBound.right = dX + szx;
    m_rcBound.bottom = dY + szy;
}

void sprite::draw_to(int sX, int sY, int sFrame, uint64_t dwTime, Color color, int draw_mode)
{
    if (m_bIsSurfaceEmpty && open_sprite_() == false)
        return;
    if ((uint32_t)sFrame >= m_iTotalFrame || sFrame < 0)
    {
        std::cout << "Sprite out_of_bounds! - " << sFrame << " - " << m_cPakFileName << '\n';
        return;
    }
    sprite_[sFrame].setColor(color);
    sprite_[sFrame].setPosition(float(sX + brush[sFrame].pvx), float(sY + brush[sFrame].pvy));
    game->draw_to(sprite_[sFrame], draw_mode);
}

void sprite::draw_scaled_sprite(int sX, int sY, int sFrame, int sWidth, int sHeight, uint64_t dwTime, Color color)
{
    if (m_bIsSurfaceEmpty && open_sprite_() == false)
        return;
    sf::FloatRect f = sprite_[sFrame].getLocalBounds();
    sprite_[sFrame].setScale((sWidth / f.width) * 100, (sHeight / f.height) * 100);
    sprite_[sFrame].setColor(color);
    sprite_[sFrame].setPosition(float(sX + brush[sFrame].pvx), float(sY + brush[sFrame].pvy));
    game->draw(sprite_[sFrame]);
    sprite_[sFrame].setScale(1.0, 1.0);
}

void sprite::draw_sprite_width(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime, Color color)
{
    if (m_bIsSurfaceEmpty && open_sprite_() == false)
        return;
    sprite_[sFrame].setTextureRect(sf::IntRect(brush[sFrame].sx, brush[sFrame].sy, sWidth, brush[sFrame].szy));
    sprite_[sFrame].setColor(color);
    sprite_[sFrame].setPosition(float(sX + brush[sFrame].pvx), float(sY + brush[sFrame].pvy));
    game->draw(sprite_[sFrame]);
    sprite_[sFrame].setTextureRect(sf::IntRect(brush[sFrame].sx, brush[sFrame].sy, brush[sFrame].szx, brush[sFrame].szy));
}

[[deprecated]] void sprite::put_sprite_fast(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
}

[[deprecated]] void sprite::put_sprite_fast_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
}

[[deprecated]] void sprite::put_sprite_fast_front_buffer(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
}

[[deprecated]] void sprite::put_sprite_fast_width(int sX, int sY, int sFrame, int sWidth, uint64_t dwTime)
{
    draw_sprite_width(sX, sY, sFrame, sWidth, dwTime, Color(255, 255, 255));
}

[[deprecated]] void sprite::put_shadow_sprite(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
}

[[deprecated]] void sprite::put_shadow_sprite_clip(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
}

[[deprecated]] void sprite::put_trans_sprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, alphaDepth));
}

[[deprecated]] void sprite::put_trans_sprite_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, alphaDepth));
}

[[deprecated]] void sprite::put_trans_sprite70(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 180));
}

[[deprecated]] void sprite::put_trans_sprite70_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 180));
}

[[deprecated]] void sprite::put_trans_sprite50(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 125));
}

[[deprecated]] void sprite::put_trans_sprite50_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 125));
}

[[deprecated]] void sprite::put_trans_sprite25(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 64));
}

[[deprecated]] void sprite::put_trans_sprite25_no_color_key(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255, 64));
}

[[deprecated]] void sprite::put_trans_sprite2(int sX, int sY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(180, 255, 255));
}

[[deprecated]] void sprite::put_shift_trans_sprite2(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
}

[[deprecated]] void sprite::put_fade_sprite(short sX, short sY, short sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
}

[[deprecated]] void sprite::put_fade_sprite_destination(uint16_t * pDstAddr, short sPitch, short sX, short sY, short sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
}

[[deprecated]] void sprite::put_sprite_color(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(color));
}

[[deprecated]] void sprite::put_sprite_color(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(sRed, sGreen, sBlue, 255));
}

[[deprecated]] void sprite::put_trans_sprite_color(int sX, int sY, int sFrame, uint32_t color, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(color));
}

[[deprecated]] void sprite::put_trans_sprite_color(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(sRed, sGreen, sBlue, 180));
}

[[deprecated]] void sprite::put_trans_sprite_rgb_no_color_key(int sX, int sY, int sFrame, int sRed, int sGreen, int sBlue, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(sRed, sGreen, sBlue, 180));
}

void sprite::get_sprite_rect(int sX, int sY, int sFrame)
{
    short dX, dY, sx, sy, szx, szy, pvx, pvy;
    if (this == nullptr || brush == nullptr)
        return;
    if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0))
        return;

    sx = brush[sFrame].sx;
    sy = brush[sFrame].sy;
    szx = brush[sFrame].szx;
    szy = brush[sFrame].szy;
    pvx = brush[sFrame].pvx;
    pvy = brush[sFrame].pvy;

    dX = sX + pvx;
    dY = sY + pvy;

    m_rcBound.top = -1;
    m_rcBound.bottom = -1;
    m_rcBound.left = -1;
    m_rcBound.right = -1;

    if (dX < 0)
    {
        sx = sx + (0 - dX);
        szx = szx - (0 - dX);
        if (szx < 0)
        {
            m_rcBound.top = -1;
            return;
        }
        dX = (short)0;
    }
    else if (dX + szx > game->get_width())
    {
        szx = szx - ((dX + szx) - (short)game->get_width());
        if (szx < 0)
        {
            m_rcBound.top = -1;
            return;
        }
    }

    if (dY < 0)
    {
        sy = sy + (0 - dY);
        szy = szy - (0 - dY);
        if (szy < 0)
        {
            m_rcBound.top = -1;
            return;
        }
        dY = (short)0;
    }
    else if (dY + szy > game->get_height())
    {
        szy = szy - ((dY + szy) - (short)game->get_height());
        if (szy < 0)
        {
            m_rcBound.top = -1;
            return;
        }
    }

    SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);
    m_sPivotX = pvx;
    m_sPivotY = pvy;
}

bool sprite::check_collison(int sX, int sY, short sFrame, int msX, int msY)
{
    short dX, dY, sx, sy, szx, szy, pvx, pvy;
    int  ix{}, iy{};
    uint16_t * pSrc{};
    int  tX, tY;

    if (this == 0) return false;
    if (brush == 0) return false;
    if ((m_iTotalFrame - 1 < sFrame) || (sFrame < 0)) return false;
    if (m_bIsSurfaceEmpty == true) return false;
    if (msX < 0 + 3) return false;
    if (msX > game->get_width() - 3) return false;
    if (msY < 0 + 3) return false;
    if (msY > game->get_height() - 3) return false;

    sx = brush[sFrame].sx;
    sy = brush[sFrame].sy;
    szx = brush[sFrame].szx;
    szy = brush[sFrame].szy;
    pvx = brush[sFrame].pvx;
    pvy = brush[sFrame].pvy;

    dX = sX + pvx;
    dY = sY + pvy;

    if (msX < dX) return false;
    if (msX > dX + szx) return false;
    if (msY < dY) return false;
    if (msY > dY + szy) return false;

    if (dX < 0 + 3)
    {
        sx = sx + (0 + 3 - dX);
        szx = szx - (0 + 3 - dX);
        if (szx < 0)
        {
            m_rcBound.top = -1;
            return false;
        }
        dX = (short)0 + 3;
    }
    else if (dX + szx > game->get_width() - 3)
    {
        szx = szx - ((dX + szx) - (short)game->get_width() - 3);
        if (szx < 0)
        {
            m_rcBound.top = -1;
            return false;
        }
    }

    if (dY < 0 + 3)
    {
        sy = sy + (0 + 3 - dY);
        szy = szy - (0 + 3 - dY);
        if (szy < 0)
        {
            m_rcBound.top = -1;
            return false;
        }
        dY = (short)0 + 3;
    }
    else if (dY + szy > game->get_height() - 3)
    {
        szy = szy - ((dY + szy) - (short)game->get_height() - 3);
        if (szy < 0)
        {
            m_rcBound.top = -1;
            return false;
        }
    }

    SetRect(&m_rcBound, dX, dY, dX + szx, dY + szy);

    tX = dX;
    tY = dY;

    if (msX - dX < 0 || msY - dY < 0 || msX - dX > szx || msY - dY > szy)
        return false;

    auto image_size = _image.getSize();
    int tarX = 0;
    int tarY = 0;
    Color pixel;

    if (msX - dX - 10 > image_size.x) tarX = image_size.x - 1;
    else if (msX - dX - 10 < 0) tarX = 0;
    else tarX = msX - dX - 10;
    if (msY - dY - 10 > image_size.y) tarY = image_size.y - 1;
    else if (msY - dY - 10 < 0) tarY = 0;
    else tarY = msY - dY - 10;
    Color pixel4 = _image.getPixel(tarX, tarY);

    if (pixel != sf::Color::Transparent)
        return true;
    return false;
}

void sprite::put_shift_sprite_fast(int sX, int sY, int shX, int shY, int sFrame, uint64_t dwTime)
{
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
    return;
}

void sprite::put_reverse_trans_sprite(int sX, int sY, int sFrame, uint64_t dwTime, int alphaDepth)
{
    if (this == nullptr || (m_bIsSurfaceEmpty && open_sprite_() == false) || sFrame >= m_iTotalFrame)
        return;
    sf::Vector2f scale = sprite_[sFrame].getScale();
    sprite_[sFrame].setScale({ scale.x * -1.0f, scale.y });
    draw_sprite(sX, sY, sFrame, dwTime, Color(255, 255, 255));
    sprite_[sFrame].setScale(scale);
    return;
}
