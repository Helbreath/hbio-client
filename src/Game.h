//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <memory>
#include <mutex>
#include <windows.h>
#include <windef.h>
#include "GlobalDef.h"
#include "defines.h"
#include "Misc.h"
#include "fps.h"

class CCharInfo;
class CMapData;
class CItem;
class CBuildItem;
class CEffect;
class CItemName;
class sprite;
class CMsg;
class CSkill;
class CMagic;

#define DEF_BTNSZX				74
#define DEF_BTNSZY				20
#define DEF_LBTNPOSX			30
#define DEF_RBTNPOSX			154
#define DEF_BTNPOSY				292

#define DEF_INDEX4_MSGID		0
#define DEF_INDEX2_MSGTYPE		4

#define DEF_SOCKETBLOCKLIMIT	300

#define DEF_MAXSPRITES			20000
#define DEF_MAXTILES			500
#define DEF_MAXEFFECTSPR		100
#define DEF_MAXSOUNDEFFECTS		110
#define DEF_MAXCHATMSGS			500
#define DEF_MAXWHISPERMSG		5
#define DEF_MAXCHATSCROLLMSGS	80
#define DEF_MAXEFFECTS			300
#define DEF_CHATTIMEOUT_A		4000
#define DEF_CHATTIMEOUT_B		500
#define DEF_CHATTIMEOUT_C		2000
#define DEF_MAXMENUITEMS		140
#define DEF_TEXTDLGMAXLINES		300

#define DEF_MAXMAGICTYPE		100
#define DEF_MAXSKILLTYPE		60

#define DEF_MAXWHETHEROBJECTS	600
#define DEF_MAXBUILDITEMS		100

#define DEF_MAXGAMEMSGS			300
#define DEF_MAXITEMNAMES		1000

#define DEF_MAXGUILDNAMES		100

#define DEF_MAXSELLLIST			12

#define WM_USER_GAMESOCKETEVENT	WM_USER + 2000
#define WM_USER_LOGSOCKETEVENT	WM_USER + 2001

#define DEF_GAMEMODE_NULL					-2
#define DEF_GAMEMODE_ONQUIT					-1
#define DEF_GAMEMODE_ONMAINMENU				0
#define DEF_GAMEMODE_ONCONNECTING			1
#define DEF_GAMEMODE_ONLOADING				2
#define DEF_GAMEMODE_ONWAITINGINITDATA		3
#define DEF_GAMEMODE_ONMAINGAME				4
#define DEF_GAMEMODE_ONCONNECTIONLOST		5
#define DEF_GAMEMODE_ONMSG					6
#define DEF_GAMEMODE_ONCREATENEWACCOUNT		7
#define DEF_GAMEMODE_ONLOGIN				8
#define DEF_GAMEMODE_ONQUERYFORCELOGIN		9
#define DEF_GAMEMODE_ONSELECTCHARACTER		10
#define DEF_GAMEMODE_ONCREATENEWCHARACTER	11
#define DEF_GAMEMODE_ONWAITINGRESPONSE		12
#define DEF_GAMEMODE_ONQUERYDELETECHARACTER 13
#define DEF_GAMEMODE_ONLOGRESMSG			14
#define DEF_GAMEMODE_ONCHANGEPASSWORD		15
#define DEF_GAMEMODE_ONVERSIONNOTMATCH		17
#define DEF_GAMEMODE_ONINTRODUCTION			18
#define DEF_GAMEMODE_ONAGREEMENT			19
#define DEF_GAMEMODE_ONSELECTSERVER			20
#define DEF_GAMEMODE_ONINPUTKEYCODE			21

#define DEF_SERVERTYPE_GAME			1
#define DEF_SERVERTYPE_LOG			2

#define DEF_CURSORSTATUS_NULL		0
#define DEF_CURSORSTATUS_PRESSED	1
#define DEF_CURSORSTATUS_SELECTED	2
#define DEF_CURSORSTATUS_DRAGGING	3

#define DEF_SELECTEDOBJTYPE_DLGBOX	1
#define DEF_SELECTEDOBJTYPE_ITEM	2

#define DEF_DOUBLECLICKTIME			300
#define DEF_MAXPARTYMEMBERS			8
#define DEF_MAXCRUSADESTRUCTURES	300


#define DEF_XSOCKEVENT_SOCKETMISMATCH			-121
#define DEF_XSOCKEVENT_CONNECTIONESTABLISH		-122
#define DEF_XSOCKEVENT_RETRYINGCONNECTION		-123
#define DEF_XSOCKEVENT_ONREAD					-124
#define DEF_XSOCKEVENT_READCOMPLETE				-125
#define DEF_XSOCKEVENT_UNKNOWN					-126
#define DEF_XSOCKEVENT_SOCKETCLOSED				-127
#define DEF_XSOCKEVENT_BLOCK					-128
#define DEF_XSOCKEVENT_SOCKETERROR				-129
#define DEF_XSOCKEVENT_CRITICALERROR			-130
#define DEF_XSOCKEVENT_NOTINITIALIZED			-131
#define DEF_XSOCKEVENT_MSGSIZETOOLARGE			-132
#define DEF_XSOCKEVENT_CONFIRMCODENOTMATCH		-133
#define DEF_XSOCKEVENT_QUENEFULL                -134
#define DEF_XSOCKEVENT_UNSENTDATASENDBLOCK		-135
#define DEF_XSOCKEVENT_UNSENTDATASENDCOMPLETE	-136

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <ixwebsocket/IXWebSocketCloseConstants.h>
#include "streams.h"
#include "json.hpp"

using sf::Keyboard;
using sf::Color;
using namespace std::chrono_literals;

namespace ix
{
class WebSocket;
struct WebSocketMessage;
using WebSocketMessagePtr = std::unique_ptr<WebSocketMessage>;
};

extern void * G_hWnd;

class connection;
using connection_ptr = std::shared_ptr<connection>;
using nlohmann::json;

using std::make_shared;
using std::make_unique;
using sf::RenderStates;
using sf::RenderWindow;
using sf::Sprite;
using sf::Color;
using sf::Drawable;

#define FONT_BUILTIN 0
#define FONT_TREBMS6PX 1
#define FONT_TREBMS8PX 2
#define FONT_TREBMS10PX 3
#define FONT_TREBMS12PX 4
#define FONT_TREBMS14PX 5
#define FONT_TREBMS16PX 6

class CMapData;

class CGame
{
public:
    fps fps_counter;
    int send_message(char * cData, std::size_t dwSize, char cKey = 0);
    std::unique_ptr<std::thread> mapdata_object_counter;

    int16_t antialiasing = 16;

    std::string account_name;
    std::string password;

    int xtest = 0;
    int ytest = 0;
    std::string response;
    float zoom = 100;

    // admin client/test stuff
    int32_t pan_x = 0;
    int32_t pan_y = 0;
    bool highlight_tile = true;

    bool m_showGrid = false;
    bool m_altPressed = false;
    unsigned char key[32];
    bool has_key = false;
    bool update_background = false;
    bool update_objects = false;
    bool update_dialogs = false;
    bool update_effects = false;

    int viewdstxvar = 0;
    int viewdstyvar = 0;
    int viewdstxcharvar = 0;
    int viewdstycharvar = 0;

    void on_input_event(sf::Event event);
    bool calcoldviewport = true;

    bool autologin = false;
    bool autoresolution = false;
    bool autovresolution = false;
    bool newconnection = false;

    std::vector<sf::Rect<int16_t>> dialogs;

    struct msg_queue_entry
    {
        msg_queue_entry()
        {
            data = nullptr;
            size = 0;
        }
        ~msg_queue_entry() { delete[] data; }
        char * data;
        std::size_t size;
    };
    using msg_queue = std::list<std::shared_ptr<msg_queue_entry>>;
    msg_queue socketpipe;
    void put_msg_queue(msg_queue & q, char * data, std::size_t size);
    void put_msg_queue(std::shared_ptr<msg_queue_entry>, msg_queue & q);
    std::shared_ptr<msg_queue_entry> get_msg_queue();

    msg_queue loginpipe;
    std::shared_ptr<msg_queue_entry> get_login_msg_queue();

    std::unique_ptr<ix::WebSocket> ws;

    void on_message(const ix::WebSocketMessagePtr & msg);
    bool is_connected() const;
    bool is_closed() const;
    bool is_connecting() const;
    bool is_closing() const;

    void perform_connect();
    void connection_loss_gamemode();

    int32_t write(const char * data, const std::size_t size);
    int32_t write(stream_write & sw);
    int32_t write(nlohmann::json & obj);

    void handle_stop();
    void close(uint32_t code = ix::WebSocketCloseConstants::kNormalClosureCode, const std::string & reason = ix::WebSocketCloseConstants::kNormalClosureMessage);

    std::pair<int16_t, int16_t> get_distance_from_player(int16_t x, int16_t y, bool abs_ = false)
    {
        int16_t distanceX = x - m_sPlayerX;
        int16_t distanceY = y - m_sPlayerY;
        if (abs_)
            return std::make_pair(std::abs(distanceX), std::abs(distanceY));
        return std::make_pair(distanceX, distanceY);
    }

    bool loggedin;
    bool disconnecting = false;

    std::mutex screen_mtx;
    std::mutex screenupdate;
    std::mutex socket_mut;
    std::mutex thread_mut;
    std::recursive_mutex connection_mut;

    sf::RenderTexture visible;
    sf::RenderTexture bg;
    sf::RenderTexture dynamic_bg;
    sf::RenderTexture charselect;
    sf::RenderTexture dialog;
    sf::RenderTexture item_box;

    sf::Sprite bg_sprite;
    sf::Sprite dynbg_sprite;
    sf::Sprite dialog_sprite;
    sf::Sprite item_box_sprite;

    // todo: cleanup
    uint16_t modx{};
    uint16_t mody{};

    uint8_t drawState = 0;

    void draw(const Drawable & drawable, const RenderStates & states = RenderStates::Default)
    {
        if (drawState == DS_VISIBLE)
        {
            //render to visible
            visible.draw(drawable, states);
        }
        else if (drawState == DS_BG)
        {
            //render to bg
            bg.draw(drawable, states);
        }
        else if (drawState == DS_DYNBG)
        {
            //render to dynamic bg
            dynamic_bg.draw(drawable, states);
        }
        else if (drawState == DS_CS)
        {
            //render to charselect
            charselect.draw(drawable, states);
        }
        else if (drawState == DS_WIN)
        {
            //render directly to window
            window.draw(drawable, states);
        }
        else if (drawState == DS_DIALOGS)
        {
            //render to dialog
            dialog.draw(drawable, states);
        }
    }

    void draw_to(const Drawable & drawable, int draw_mode, const RenderStates & states = RenderStates::Default)
    {
        if (draw_mode == DS_VISIBLE)
        {
            //render to visible
            visible.draw(drawable, states);
        }
        else if (draw_mode == DS_BG)
        {
            //render to bg
            bg.draw(drawable, states);
        }
        else if (drawState == DS_DYNBG)
        {
            //render to dynamic bg
            dynamic_bg.draw(drawable, states);
        }
        else if (draw_mode == DS_CS)
        {
            //render to charselect
            charselect.draw(drawable, states);
        }
        else if (draw_mode == DS_WIN)
        {
            //render directly to window
            window.draw(drawable, states);
        }
        else if (drawState == DS_DIALOGS)
        {
            //render to dialog
            dialog.draw(drawable, states);
        }
    }

    uint8_t render_target()
    {
        return drawState;
    }

    void render_mouse(uint16_t mx, uint16_t my);
    bool scale_mouse_rendering = false;

    void render_target(uint8_t s, bool clearbuffer = false, Color color = Color(0, 0, 0))
    {
        drawState = s;
        if (s == DS_VISIBLE)
        {
            if (clearbuffer)
                visible.clear(color);
        }
        else if (s == DS_BG)
        {
            if (clearbuffer)
                bg.clear(color);
        }
        else if (s == DS_DYNBG)
        {
            if (clearbuffer)
                dynamic_bg.clear(color);
        }
        else if (s == DS_CS)
        {
            if (clearbuffer)
                charselect.clear(color);
        }
        else if (s == DS_DIALOGS)
        {
            if (clearbuffer)
                dialog.clear(color);
        }
    };

    void socket_mode(bool _socketmode)
    {
        socketmode_ = _socketmode;
    }
    bool socket_mode()
    {
        return socketmode_;
    }
    bool socketmode_;

    int16_t lastchar;
    bool capslock;
    bool fullscreen;
    char oldmode;
    bool fullscreenswap = false;
    bool vsync = false;
    uint64_t foregroundfpstarget = 60;
    uint64_t foregroundfps = 0;
    uint64_t foregroundframetime = 1000 / foregroundfpstarget;
    uint64_t backgroundfpstarget = 45;
    uint64_t backgroundfps = 0;
    uint64_t backgroundframetime = 1000 / backgroundfpstarget;
    uint64_t time1;
    uint64_t time2;

    bool wasinactive = false;

    std::string _renderer;

    uint16_t charselectx = 0, charselecty = 0;

    sf::RenderWindow window;

    char winName[256];

    bool m_bInputStatus = false;
    int m_iInputX, m_iInputY, m_iInputX2;
    char * m_pInputBuffer;
    uint32_t m_inputMaxLen;

    //     void ShowReceivedString(bool bIsHide = false);
    void start_input_string(int left, int top, uint32_t len, char * pBuffer, bool bIsHide = false, int right = 0);
    //     void EndInputString();
    //     void ReceiveString(char * pString);
    //     void ClearInputString();

    bool captured = false;
    bool inside_vm = false;
    bool hide_ui = false;
    bool take_screen = false;
    bool rendering_character = false;

    std::mutex gamemode_m;

    int32_t testx = 0;
    int32_t testy = 0;
    int32_t testx2 = 0;
    int32_t testy2 = 0;
    int32_t iUpdateRet = -1;

    bool isloaded = false;
    uint32_t frame_limit = 60;
    uint32_t frame_limit_bg = 30;

    uint64_t ping = 0;

    void load_settings();
    void save_settings();

    bool create_renderer();
    void create_surfaces();
    void create_fonts();
    void create_window();
    sf::WindowHandle handle;

    void put_font_string_size(std::string fontname, uint16_t iX, uint16_t iY, std::string text, Color color, int size)
    {
        put_font_string_size(&_font.at(fontname), iX, iY, text, color, size);
    }
    void put_font_string_size(sf::Font * fontname, uint16_t iX, uint16_t iY, std::string text, Color color, int size);
    void put_font_string(std::string fontname, uint16_t iX, uint16_t iY, std::string pString, Color color = Color(255, 255, 255))
    {
        put_font_string(&_font.at(fontname), iX, iY, pString, color);
    }
    void put_font_string(sf::Font * fontname, uint16_t iX, uint16_t iY, std::string pString, Color color = Color(255, 255, 255));
    void put_overhead_string(uint16_t x, uint16_t y, std::string text, Color color = Color(255, 255, 255), int multiplier = 1, bool transparency = false, int size = 12);
    void put_under_entity_string(uint16_t x, uint16_t y, std::string text, Color color = Color(255, 255, 255), int size = 14);
    void put_chat_string(uint16_t iX, uint16_t iY, std::string pString, Color color = Color(255, 255, 255));
    void put_chat_window_string(uint16_t iX, uint16_t iY, std::string pString, Color color = Color(255, 255, 255));
    struct chat_msg
    {
        chat_msg(std::string _msg, Color _color = Color::White, int _size = 12) : msg(_msg), color(_color), size(_size) {}
        std::string msg;
        Color color;
        int size;
    };
    void render_item_details_box(uint16_t iX, uint16_t iY, std::vector<chat_msg> strings, Color background_color = Color(0, 0, 0, 200));
    void put_string(uint16_t iX, uint16_t iY, std::string pString, Color color = Color(255, 255, 255), bool bHide = false, char cBGtype = 2);
    void put_string2(uint16_t iX, uint16_t iY, std::string pString, int r = 255, int g = 255, int b = 255)
    { //TODO: remove
        put_string(iX, iY, pString, Color(r, g, b, 255));
    }
    void put_string3(uint16_t iX, uint16_t iY, std::string pString, Color color = Color(255, 255, 255))
    { //TODO: remove
        put_string(iX, iY, pString, color);
    }
    void put_aligned_string(uint16_t iX1, uint16_t iX2, uint16_t iY, std::string pString, bool highlight, int font_size = 12)
    {
        put_aligned_string(iX1, iX2, iY, pString, Color(highlight ? 255 : 4, highlight ? 255 : 0, highlight ? 255 : 50), font_size);
    }
    void put_aligned_string(uint16_t iX1, uint16_t iX2, uint16_t iY, std::string pString, bool wordWrap, Color color, int font_size = 12)
    {
        // todo - figure this out?
        put_aligned_string(iX1, iX2, iY, pString, color, font_size);
    }
    void put_aligned_string(uint16_t iX1, uint16_t iX2, uint16_t iY, std::string text, Color color = Color(255, 255, 255), int font_size = 12);
    void put_aligned_string(uint16_t iX1, uint16_t iX2, uint16_t iY, std::string text, int r, int g, int b, int a = 255, int font_size = 12)
    {
        put_aligned_string(iX1, iX2, iY, text, Color(r, g, b, a), font_size);
    }
    __inline sf::Font & get_font(std::string font = "default")
    {
        auto it = _font.find(font);
        if (it != _font.end())
            return it->second;

        it = _font.find("default");
        if (it != _font.end())
            return it->second;

        throw std::out_of_range("Font not found");
    }
    void put_string_sprite_font(uint16_t iX, uint16_t iY, std::string_view pStr, short sR, short sG, short sB);
    void put_string_sprite_font2(uint16_t iX, uint16_t iY, std::string_view pStr, short sR, short sG, short sB);
    void put_string_sprite_font3(uint16_t iX, uint16_t iY, std::string_view pStr, short sR, short sG, short sB, bool bTrans = false, int iType = 0);
    void put_string_sprite_number(uint16_t iX, uint16_t iY, char * pStr, short sR, short sG, short sB);

    void create_load_list();

    std::string get_game_mode_str();
    std::string get_game_mode(int _gamemode);
    int16_t get_game_mode(std::string _gamemode);
    int16_t get_game_mode() { return m_cGameMode; };

    std::map<std::string, sf::Font> _font;
    sf::Text _text;
    sf::Text chat_window_text;
    sf::Text overhead_text;
    sf::Text under_text;
    sf::Font * default_font;
    sf::Font * arya_font;
    sf::Font * test_font;

    std::string workingdirectory;

    std::shared_ptr<CCharInfo> selectedchar = nullptr;
    bool clipmousegame = false;
    bool clipmousewindow;
    bool isactive;
    uint16_t screenwidth;
    uint16_t screenheight;
    uint16_t screenwidth_windowed;
    uint16_t screenheight_windowed;
    uint16_t screenwidth_v;
    uint16_t screenheight_v;
    void set_resolution(uint16_t width, uint16_t height)
    {
        screenwidth = width;
        screenheight = height;
        if (!fullscreen)
        {
            screenwidth_windowed = width;
            screenheight_windowed = height;
        }
    }
    void set_virtual_resolution(uint16_t width, uint16_t height)
    {
        screenwidth_v = width;
        screenheight_v = height;
    }

    void init_dialogs();

    __forceinline uint16_t get_width() { return screenwidth; }
    __forceinline uint16_t get_height() { return screenheight; }

    __forceinline uint16_t get_windowed_width() { return screenwidth_windowed; }
    __forceinline uint16_t get_windowed_height() { return screenheight_windowed; }

    __forceinline uint16_t get_virtual_width() { return screenwidth_v; }
    __forceinline uint16_t get_virtual_height() { return screenheight_v; }

    void send_screen_settings_to_server();

    uint64_t mtime;

    void make_sprite(char * FileName, int iStart, short sCount, bool bAlphaEffect);
    void make_tile_sprite(char * FileName, short sStart, short sCount, bool bAlphaEffect);
    void make_effect_sprite(char * FileName, short sStart, short sCount, bool bAlphaEffect);

    sf::Text version_text;
    bool draw_version = false;

    std::map<int, std::string> game_mode_map = {
        { DEF_GAMEMODE_NULL, "0" },
        { DEF_GAMEMODE_ONQUIT, "quit" },
        { DEF_GAMEMODE_ONMAINMENU, "main-menu" },
        { DEF_GAMEMODE_ONCONNECTING, "connecting" },
        { DEF_GAMEMODE_ONLOADING, "loading" },
        { DEF_GAMEMODE_ONWAITINGINITDATA, "waiting-init-data" },
        { DEF_GAMEMODE_ONMAINGAME, "main-game" },
        { DEF_GAMEMODE_ONCONNECTIONLOST, "connection-lost" },
        { DEF_GAMEMODE_ONMSG, "msg" },
        { DEF_GAMEMODE_ONCREATENEWACCOUNT, "create-new-account" },
        { DEF_GAMEMODE_ONLOGIN, "login" },
        { DEF_GAMEMODE_ONQUERYFORCELOGIN, "query-force-login" },
        { DEF_GAMEMODE_ONSELECTCHARACTER, "select-character" },
        { DEF_GAMEMODE_ONCREATENEWCHARACTER, "create-new-character" },
        { DEF_GAMEMODE_ONWAITINGRESPONSE, "waiting-response" },
        { DEF_GAMEMODE_ONQUERYDELETECHARACTER, "query-delete-character" },
        { DEF_GAMEMODE_ONLOGRESMSG, "log-res-msg" },
        { DEF_GAMEMODE_ONVERSIONNOTMATCH, "version-not-match" },
        { DEF_GAMEMODE_ONINTRODUCTION, "introduction" },
        { DEF_GAMEMODE_ONAGREEMENT, "agreement" },
        { DEF_GAMEMODE_ONSELECTSERVER, "select-server" },
    };

    sf::Sound m_pCSound[MAX_CHARACTER_SOUNDS];
    sf::Sound m_pMSound[MAX_MONSTER_SOUNDS];
    sf::Sound m_pESound[MAX_EFFECT_SOUNDS];
    sf::SoundBuffer CSoundBuffer[MAX_CHARACTER_SOUNDS];
    sf::SoundBuffer MSoundBuffer[MAX_MONSTER_SOUNDS];
    sf::SoundBuffer ESoundBuffer[MAX_EFFECT_SOUNDS];
    sf::SoundBuffer bgmbuffer;
    sf::Sound m_pBGM;
    bool bgm_override = false;
    std::string bgm_override_name;
    std::string bgm_current;

    void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, int iR, int iG, int iB, int iA = 255);
    void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color = Color(255, 255, 255));
    void draw_shadow_box(uint16_t sX, uint16_t sY, uint16_t dX, uint16_t dY, Color color = Color(0, 0, 0, 64));
    void put_pixel(uint16_t sX, uint16_t sY, Color color = Color(255, 255, 255));
    void put_pixel(uint16_t sX, uint16_t sY, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255)
    {
        put_pixel(sX, sY, Color(r, g, b, a));
    }

    void update_mouse_state(uint16_t & x, uint16_t & y, uint16_t & z, char & left_button, char & right_button);

    void change_display_mode();
















    //////////////////////////////////////////////////////////////////////////

    int m_iTeleportMapCount;
    void ResponseTeleportList(char * pData);
    void ResponseChargedTeleport(char * pData);


    void ItemEquipHandler(char cItemID);
    void ReleaseEquipHandler(char cEquipPos);

    int  iGetManaCost(int iMagicNo);
    void UseMagic(int iMagicNo);
    bool _bCheckMoveable(short sx, short sy);
    bool FindGuildName(char * pName, int * ipIndex);
    void bItemDrop_SkillDialog();
    void bItemDrop_IconPannel(short msX, short msY);
    void bItemDrop_Character();
    void bItemDrop_Inventory(short msX, short msY);
    void bItemDrop_ItemUpgrade();
    void bItemDrop_SellList(short msX, short msY);
    void bItemDrop_ExchangeDialog(short msX, short msY);
    void bItemDrop_Bank(short msX, short msY);
    void bItemDrop_ExternalScreen(char cItemID, short msX, short msY);
    void CreateScreenShot();
    void CrusadeWarResult(int iWinnerSide);
    void CrusadeContributionResult(int iWarContribution);
    void CannotConstruct(int iCode);
    void DrawTopMsg();
    void SetTopMsg(std::string pString, unsigned char iLastSec);
    void DrawObjectFOE(int ix, int iy, int iFrame);
    void GrandMagicResult(char * pMapName, int iV1, int iV2, int iV3, int iV4, int iHP1, int iHP2, int iHP3, int iHP4);
    void MeteorStrikeComing(int iCode);
    void _Draw_OnLogin(char * pAccount, char * pPassword, int msX, int msY, int iFrame = 60000);
    void DrawNewDialogBox(char cType, int sX, int sY, int iFrame, bool bIsNoColorKey = false, bool bIsTrans = false);
    void AddMapStatusInfo(char * pData, bool bIsLastData);
    void _RequestMapStatus(char * pMapName, int iMode);
    int  GetCharKind(char * str, int index);
    void ReceiveString(char * pString);
    void EndInputString();
    void ClearInputString();
    void ShowReceivedString(bool bIsHide = false);
    bool bReadItemNameConfigFile();
    void DrawDialogBoxs(short msX, short msY, short msZ, char cLB);
    void DrawDialogBox_Character(short msX, short msY);//1
    void DrawDialogBox_Inventory(int msX, int msY);//2
    void DrawDialogBox_Magic(short msX, short msY, short msZ);//3
    void DrawDialogBox_GuildMenu(short msX, short msY);//7
    void DrawDialogBox_GuildOperation(short msX, short msY);//8
    void DrawDialogBox_GuideMap(short msX, short msY, char cLB);//9
    void DrawDialogBox_Chat(short msX, short msY, short msZ, char cLB);//10
    void DrawDialogBox_Shop(short msX, short msY, short msZ, char cLB);//11
    void DrawDialogBox_LevelUpSetting(short msX, short msY);//12
    void DrawDialogBox_CityHallMenu(short msX, short msY);//13
    void DrawDialogBox_Bank(short msX, short msY, short msZ, char cLB);//14
    void DrawDialogBox_Skill(short msX, short msY, short msZ, char cLB);//15
    void DrawDialogBox_MagicShop(short msX, short msY, short msZ);//16
    void DrawDialogBox_QueryDropItemAmount();//17
    void DrawDialogBox_Text(short msX, short msY, short msZ, char cLB);//18
    void DrawDialogBox_SysMenu(short msX, short msY, char cLB);//19
    void DrawDialogBox_NpcActionQuery(short msX, short msY);//20
    void DrawDialogBox_NpcTalk(short msX, short msY, char cLB);//21
    void DrawDialogBox_Map();//22
    void DrawDialogBox_SellorRepairItem(short msX, short msY);//23
    void DrawDialogBox_Fishing(short msX, short msY);//24
    void DrawDialogBox_ShutDownMsg(short msX, short msY);//25
    void DrawDialogBox_SkillDlg(short msX, short msY, short msZ, char cLB);//26
    void DrawDialogBox_Exchange(short msX, short msY);//27
    void DrawDialogBox_Quest(int msX, int msY);//28
    void DrawDialogBox_GaugePannel();//29
    void DrawDialogBox_IconPannel(short msX, short msY);//30
    void DrawDialogBox_SellList(short msX, short msY);//31
    void DrawDialogBox_Party(short msX, short msY);//32
    void DrawDialogBox_CrusadeJob(short msX, short msY);//33
    void DrawDialogBox_ItemUpgrade(int msX, int msY);//34
    void DrawDialogBox_Help(int msX, int msY);//35
    void DrawDialogBox_Commander(int msX, int msY);//36
    void DrawDialogBox_Constructor(int msX, int msY);//37
    void DrawDialogBox_Soldier(int msX, int msY);//38
    void DrawDialogBox_ItemDrop(short msX, short msY);//4
    void DrawDialogBox_WarningMsg(short msX, short msY);//6
    void DrawDialogBox_15AgeMsg(short msX, short msY);//5
    void DrawDialogBox_FeedBackCard(short msX, short msY);//40
    void DisplayGold(int iGold);//5

    bool _bCheckDlgBoxClick(short msX, short msY);
    void DlgBoxClick_WarningMsg(short msX, short msY);
    void DlgBoxClick_15AgeMsg(short msX, short msY);
    void DlgBoxClick_ItemDrop(short msX, short msY);
    void DlgBoxClick_Character(short msX, short msY);
    void DlgBoxClick_Inventory(short msX, short msY);
    void DlgBoxClick_Magic(short msX, short msY);
    void DlgBoxClick_GuildMenu(short msX, short msY);
    void DlgBoxClick_GuildOp(short msX, short msY);
    void DlgBoxClick_Shop(short msX, short msY);
    void DlgBoxClick_LevelUpSettings(short msX, short msY);
    void DlgBoxClick_CityhallMenu(short msX, short msY);
    void DlgBoxClick_Bank(short msX, short msY);
    void DlgBoxClick_Skill(short msX, short msY);
    void DlgBoxClick_MagicShop(short msX, short msY);
    void DlgBoxClick_FeedBackCard(short msX, short msY);
    void DlgBoxClick_Text(short msX, short msY);
    void DlgBoxClick_SysMenu(short msX, short msY);
    void DlgBoxClick_NpcActionQuery(short msX, short msY);
    void DlgBoxClick_NpcTalk(int msX, int msY);
    void DlgBoxClick_ItemSellorRepair(short msX, short msY);
    void DlgBoxClick_Fish(short msX, short msY);
    void DlgBoxClick_ShutDownMsg(short msX, short msY);
    void DlgBoxClick_SkillDlg(short msX, short msY);
    void DlgBoxClick_Exchange(short msX, short msY);
    void DlgBoxClick_Quest(int msX, int msY);
    void DlgBoxClick_SellList(short msX, short msY);
    void DlgBoxClick_IconPannel(short msX, short msY);
    void DlgBoxClick_Party(short msX, short msY);
    void DlgBoxClick_CrusadeJob(short msX, short msY);
    void DlgBoxClick_ItemUpgrade(int msX, int msY);
    void DlgBoxClick_Help(int msX, int msY);
    void DlgBoxClick_Commander(int msX, int msY);
    void DlgBoxClick_Constructor(int msX, int msY);
    void DlgBoxClick_Soldier(int msX, int msY);
    void NotifyMsgHandler(char * pData);
    void NotifyMsg_GlobalAttackMode(char * pData);
    void NotifyMsg_QuestReward(char * pData);
    void NotifyMsg_QuestContents(char * pData);
    void NotifyMsg_ItemColorChange(char * pData);
    void NotifyMsg_DropItemFin_CountChanged(char * pData);
    void NotifyMsg_CannotGiveItem(char * pData);
    void NotifyMsg_GiveItemFin_CountChanged(char * pData);
    void NotifyMsg_SetExchangeItem(char * pData);
    void NotifyMsg_OpenExchageWindow(char * pData);
    void NotifyMsg_DownSkillIndexSet(char * pData);
    void NotifyMsg_AdminInfo(char * pData);
    void NotifyMsg_WhetherChange(char * pData);
    void NotifyMsg_FishChance(char * pData);
    void NotifyMsg_EventFishMode(char * pData);
    void NotifyMsg_NoticeMsg(char * pData);
    void NotifyMsg_RatingPlayer(char * pData);
    void NotifyMsg_CannotRating(char * pData);
    void NotifyMsg_PlayerShutUp(char * pData);
    void NotifyMsg_TimeChange(char * pData);
    void NotifyMsg_Hunger(char * pData);
    void NotifyMsg_PlayerProfile(char * pData);
    void NotifyMsg_WhisperMode(bool bActive, char * pData);
    void NotifyMsg_PlayerStatus(bool bOnGame, char * pData);
    void NotifyMsg_Charisma(char * pData);
    void NotifyMsg_ItemRepaired(char * pData);
    void NotifyMsg_RepairItemPrice(char * pData);
    void NotifyMsg_CannotRepairItem(char * pData);
    void NotifyMsg_CannotSellItem(char * pData);
    void NotifyMsg_SellItemPrice(char * pData);
    void NotifyMsg_ShowMap(char * pData);
    void NotifyMsg_SkillUsingEnd(char * pData);
    void NotifyMsg_TotalUsers(char * pData);
    void NotifyMsg_MagicEffectOff(char * pData);
    void NotifyMsg_MagicEffectOn(char * pData);
    void NotifyMsg_SetItemCount(char * pData);
    void NotifyMsg_ItemDepleted_EraseItem(char * pData);
    void NotifyMsg_Skill(char * pData);
    void NotifyMsg_DropItemFin_EraseItem(char * pData);
    void NotifyMsg_GiveItemFin_EraseItem(char * pData);
    void NotifyMsg_EnemyKillReward(char * pData);
    void NotifyMsg_PKcaptured(char * pData);
    void NotifyMsg_PKpenalty(char * pData);
    void NotifyMsg_ItemToBank(char * pData);
    void NotifyMsg_ItemLifeSpanEnd(char * pData);
    void NotifyMsg_ItemReleased(char * pData);
    void NotifyMsg_LevelUp(char * pData);
    void NotifyMsg_MP(char * pData);
    void NotifyMsg_SP(char * pData);
    void NotifyMsg_SkillTrainSuccess(char * pData);
    void NotifyMsg_MagicStudyFail(char * pData);
    void NotifyMsg_MagicStudySuccess(char * pData);
    void NotifyMsg_DismissGuildsMan(char * pData);
    void NotifyMsg_NewGuildsMan(char * pData);
    void NotifyMsg_CannotJoinMoreGuildsMan(char * pData);
    void NotifyMsg_GuildDisbanded(char * pData);
    void NotifyMsg_Exp(char * pData);
    void NotifyMsg_Killed(char * pData);
    void NotifyMsg_HP(char * pData);
    void NotifyMsg_ItemPurchased(char * pData);
    void NotifyMsg_DismissGuildReject(char * pData);
    void NotifyMsg_DismissGuildApprove(char * pData);
    void NotifyMsg_JoinGuildReject(char * pData);
    void NotifyMsg_JoinGuildApprove(char * pData);
    void NotifyMsg_QueryDismissGuildPermission(char * pData);
    void NotifyMsg_QueryJoinGuildPermission(char * pData);
    void NotifyMsg_ItemObtained(char * pData);
    void NotifyMsg_ForceDisconn(char * pData);
    void NotifyMsg_BanGuildMan(char * pData);

    void ResponsePanningHandler(char * pData);
    //void StartInputString(int sX, int sY, unsigned char iLen, char * pBuffer, bool bIsHide = false);
    void _SetIlusionEffect(int iOwnerH);
    int _iGetFOE(int sStatus);
    void NoticementHandler(char * pData);
    void GetItemName(char * cItemName, uint32_t dwAttribute, char * pStr1, char * pStr2, char * pStr3, std::size_t len);
    void GetItemName(class CItem * pItem, char * pStr1, char * pStr2, char * pStr3, std::size_t len);
    void _InitOnCreateNewCharacter();
    void _LoadGameMsgTextContents();
    bool _bCheckCurrentBuildItemStatus();
    bool _bCheckBuildItemStatus();
    bool _bDecodeBuildItemContents();
    int  _iCheckLUS();
    void GetNpcName(short sType, char * pName);
    void UseShortCut(int num);
    void UpdateScreen();
    void UpdateScreen_OnMainMenu();
    void UpdateScreen_OnGame();
    void UpdateScreen_OnConnecting();
    void UpdateScreen_OnWaitInitData();
    void UpdateScreen_OnLoading();
    void UpdateScreen_OnConnectionLost();
    void UpdateScreen_OnLogin();
    void UpdateScreen_OnMsg();
    void UpdateScreen_OnQuit();
    void UpdateScreen_OnQueryForceLogin();
    void UpdateScreen_OnQueryDeleteCharacter();
    void UpdateScreen_OnWaitingResponse();
    void UpdateScreen_OnCreateNewCharacter();
    void UpdateScreen_OnSelectCharacter();
    void UpdateScreen_OnLogResMsg();
    void UpdateScreen_OnSelectCharacter(short sX, short sY, short msX, short msY, bool bIgnoreFocus = false);
    void UpdateScreen_OnLoading_Progress();
    void UpdateScreen_OnVersionNotMatch();
    void NpcTalkHandler(char * pData);
    int  _iGetWeaponSkillType();
    void SetCameraShakingEffect(short sDist, int iMul = 0);
    bool bDlgBoxPress_SkillDlg(short msX, short msY);
    bool bDlgBoxPress_Inventory(short msX, short msY);
    bool bDlgBoxPress_Character(short msX, short msY);
    void ClearSkillUsingStatus();
    bool bCheckItemOperationEnabled(char cItemID);
    void _DrawThunderEffect(int sX, int sY, int dX, int dY, int rX, int rY, char cType);
    void DrawLine2(int x0, int y0, int x1, int y1, int iR, int iG, int iB);
    void SetWhetherStatus(bool bStart, char cType);
    void weather_object_frame_counter();
    void draw_weather_effects();
    bool bCheckExID(char * pName);
    bool bCheckLocalChatCommand(char * pMsg);
    char GetOfficialMapName(char * pMapName, char * pName);
    int iGetLevelExp(int iLevel);
    int _iCalcTotalWeight();
    void DrawVersion();
    bool _bIsItemOnHand();
    void DynamicObjectHandler(char * pData);
    bool _bCheckItemByType(char cType);
    void _DrawBlackRect(int iSize);
    void DrawNpcName(short sX, short sY, short sOwnerType, int sStatus);
    void DrawObjectName(short sX, short sY, char * pName, int sStatus);
    void PlaySound(char cType, int iNum, int iDist, long lPan = 0);
    void _RemoveChatMsgListByObjectID(int iObjectID);
    void _LoadTextDlgContents(int cType);
    int  _iLoadTextDlgContents2(int iType);
    void DrawChatMsgs(short sX, short sY, short dX, short dY);
    void RequestFullObjectData(uint16_t wObjectID);
    bool bInitSkillCfgList();
    bool bCheckImportantFile();
    void DlbBoxDoubleClick_Inventory(short msX, short msY);
    void DlbBoxDoubleClick_Character(short msX, short msY);
    void DlbBoxDoubleClick_GuideMap(short msX, short msY);
    bool _bCheckDlgBoxDoubleClick(short msX, short msY);
    void EraseItem(char cItemID);
    void RetrieveItemHandler(char * pData);
    void CivilRightAdmissionHandler(char * pData);
    void _Draw_CharacterBody(short sX, short sY, short sType);
    void ClearContents_OnSelectCharacter();
    void ClearContents_OnCreateNewAccount();
    void _Draw_UpdateScreen_OnCreateNewAccount();
    bool _bDraw_OnCreateNewCharacter(char * pName, short msX, short msY, int iPoint);
    bool _bGetIsStringIsNumber(char * pStr);
    bool bInitMagicCfgList();
    bool __bDecodeContentsAndBuildItemForSaleList(char * pBuffer);
    void _LoadShopMenuContents(char cType);
    void PutChatScrollList(const char * pMsg, char cType);
    void RequestTeleportAndWaitData();
    void DrawEffectLights();
    void PointCommandHandler(int indexX, int indexY, char cItemID = -1);
    void DrawEffects();
    void bAddNewEffect(short sType, int sX, int sY, int dX, int dY, char cStartFrame, int iV1 = 1);
    void AddEventList(const char * pTxt, char cColor = 0, bool bDupAllow = true);
    void ShowEventList(uint32_t dwTime);
    void SetItemCount(char * pItemName, uint32_t dwCount);
    void _ShiftGuildOperationList();
    void _PutGuildOperationList(char * pName, char cOpMode);
    void DisbandGuildResponseHandler(char * pData);
    void InitPlayerCharacteristics(char * pData);
    void CreateNewGuildResponseHandler(char * pData);
    void _GetHairColorColor(int iColorType, int * pR, int * pG, int * pB);
    void InitGameSettings();
    void CommonEventHandler(char * pData);
    bool _bCheckDraggingItemRelease(short msX, short msY);
    void _SetItemOrder(char cWhere, char cItemID);
    int iGetTopDialogBoxIndex();
    void DisableDialogBox(int iBoxID);
    void EnableDialogBox(int iBoxID, int cType, int sV1, int sV2, char * pString = 0);
    void InitItemList(char * pData);
    int _iCheckDlgBoxFocus(short msX, short msY, char cButtonSide);
    void GetPlayerTurn();
    bool __fastcall DrawObject_OnDead(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnDying(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnMagic(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnAttack(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnAttackMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnStop(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnDamageMove(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnRun(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnDamage(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool __fastcall DrawObject_OnGetItem(int indexX, int indexY, int sX, int sY, bool bTrans, uint32_t dwTime, int msX, int msY);
    bool effect_frame_counter();
    void ClearGuildNameList();
    void DrawBackground(short sDivX, short sModX, short sDivY, short sModY);
    void DrawChatMsgBox(short sX, short sY, int iChatIndex, bool bIsPreDC);
    void ReleaseTimeoverChatMsg();
    void ChatMsgHandler(char * pData);
    void ReleaseUnusedSprites();
    bool bReadLoginConfigFile(char * cFn);
    void change_game_mode(char cMode);
    // 	void PutString(int iX, int iY, char * pString, COLORREF color);
    // 	void PutString(int iX, int iY, char * pString, COLORREF color, bool bHide, char cBGtype, bool bIsPreDC = false);
    // 	void PutString2(int iX, int iY, char * pString, short sR, short sG, short sB);
    // 	void PutAlignedString(int iX1, int iX2, int iY, char * pString, short sR = 0, short sG = 0, short sB = 0);
    // 	void PutString_SprFont(int iX, int iY, char * pStr, short sR, short sG, short sB);
    // 	void PutString_SprFont2(int iX, int iY, char * pStr, short sR, short sG, short sB);
    // 	void PutString_SprFont3(int iX, int iY, char * pStr, short sR, short sG, short sB, bool bTrans = false, int iType = 0);
    // 	void PutString_SprNum(int iX, int iY, char * pStr, short sR, short sG, short sB);
    void LogResponseHandler(char * pData, uint64_t size);
    void OnTimer();
    void LogEventHandler(char * pData);
    void _ReadMapData(short sPivotX, short sPivotY, char * pData);
    void MotionEventHandler(char * pData);
    void InitDataResponseHandler(char * pData);
    void ConnectionEstablishHandler(char cWhere);
    void MotionResponseHandler(char * pData);
    bool GameRecvMsgHandler(char * pData, uint64_t size);
    void DrawObjects(short sPivotX, short sPivotY, short sDivX, short sDivY, short sModX, short sModY, short msX, short msY);
    bool bSendCommand(uint32_t dwMsgID, uint16_t wCommand, char cDir, int iV1, int iV2, int iV3, char * pString, int iV4 = 0);
    char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, bool bMoveCheck = false);
    void RestoreSprites();
    void CommandProcessor(short msX, short msY, short indexX, short indexY, char cLB, char cRB);
    void CalcViewPoint();
    void Quit();
    bool bInit();
    void ReserveFightzoneResponseHandler(char * pData);
    int _iGetAttackType();
    bool __bDecodeBuildItemContents(char * pBuffer);
    int _iGetBankItemCount();
    int _iGetTotalItemNum();
    void StartBGM();

    bool _ItemDropHistory(char * ItemName);

    CGame();
    ~CGame();

    struct
    {
        short sX;
        short sY;
        short sZ;
        short sZ2;
        bool LB;
        bool RB;
        bool MB;
        short sCursorFrame;
        char  cPrevStatus;
        char  cSelectedObjectType;
        short sSelectedObjectID;
        short sPrevX, sPrevY, sDistX, sDistY;
        uint32_t dwSelectClickTime;
        short sClickX, sClickY;
    } m_stMCursor;

    struct
    {
        int   sV1, sV2, sV3, sV4, sV5, sV6, sV7, sV8, sV9, sV10, sV11, sV12, sV13, sV14;
        uint32_t dwV1, dwV2, dwT1;
        bool  bFlag;
        short sX, sY;
        short sSizeX, sSizeY;
        short sView;
        char  cStr[32], cStr2[32], cStr3[32], cStr4[32];
        char  cMode;
        bool  bIsScrollSelected;
    } m_stDialogBoxInfo[41];

    struct
    {
        int iIndex;
        int iAmount;
    } m_stSellItemList[DEF_MAXSELLLIST];

    struct
    {
        char cName[22];
        char cOpMode;
    } m_stGuildOpList[100];

    struct
    {
        uint32_t dwTime;
        char  cColor;
        char  cTxt[96];
    } m_stEventHistory[6];

    struct
    {
        uint32_t dwTime;
        char  cColor;
        char  cTxt[96];
    } m_stEventHistory2[6];

    struct
    {
        short sX, sY;
        char cStep;
    } m_stWhetherObject[DEF_MAXWHETHEROBJECTS];

    struct
    {
        bool bIsQuestCompleted;
        short sWho, sQuestType, sContribution, sTargetType, sTargetCount, sX, sY, sRange;
        char cTargetName[22];
    } m_stQuest;

    struct
    {
        char cStatus;
        char cName[12];
    } m_stPartyMember[DEF_MAXPARTYMEMBERS];

    struct
    {
        short sX, sY;
        char cType;
        char cSide;
    } m_stCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];

    struct
    {
        char cName[12];
    } m_stPartyMemberNameList[DEF_MAXPARTYMEMBERS + 1];

    struct
    {
        uint32_t dwRefTime;
        int iGuildRank;
        char cCharName[12];
        char cGuildName[24];
    } m_stGuildName[DEF_MAXGUILDNAMES];

    struct
    {
        int iIndex;
        char mapname[12];
        int iX;
        int iY;
        int iCost;
    } m_stTeleportList[20];

    sprite * m_pSprite[DEF_MAXSPRITES];
    sprite * m_pTileSpr[DEF_MAXTILES];
    sprite * m_pEffectSpr[DEF_MAXEFFECTSPR];
    CMisc m_Misc;
    CMapData * m_pMapData;
    CMsg * m_pChatMsgList[DEF_MAXCHATMSGS];
    CMsg * m_pChatScrollList[DEF_MAXCHATSCROLLMSGS];
    CMsg * m_pWhisperMsg[DEF_MAXWHISPERMSG];
    CEffect * m_pEffectList[DEF_MAXEFFECTS];
    CItem * m_pItemList[DEF_MAXITEMS];
    CItem * m_pBankList[DEF_MAXBANKITEMS];
    CMagic * m_pMagicCfgList[DEF_MAXMAGICTYPE];
    CSkill * m_pSkillCfgList[DEF_MAXSKILLTYPE];
    CMsg * m_pMsgTextList[DEF_TEXTDLGMAXLINES];
    CMsg * m_pMsgTextList2[DEF_TEXTDLGMAXLINES];
    CMsg * m_pAgreeMsgTextList[DEF_TEXTDLGMAXLINES];
    CMsg * m_pExID;
    CBuildItem * m_pBuildItemList[DEF_MAXBUILDITEMS];
    CBuildItem * m_pDispBuildItemList[DEF_MAXBUILDITEMS];
    CItem * m_pItemForSaleList[DEF_MAXMENUITEMS];
    CCharInfo * m_pCharList[4];
    CMsg * m_pGameMsgList[DEF_MAXGAMEMSGS];
    CItemName * m_pItemNameList[DEF_MAXITEMNAMES];

    uint32_t G_dwGlobalTime;
    uint32_t m_dwCommandTime;
    uint32_t m_dwConnectMode;
    uint32_t m_dwTime;
    uint32_t m_dwCurTime;
    uint32_t m_dwCheckConnTime, m_dwCheckSprTime, m_dwCheckChatTime;
    uint32_t m_dwDialogCloseTime;
    uint32_t m_dwLogOutCountTime;
    uint32_t m_dwRestartCountTime;
    uint32_t m_dwWOFtime;
    uint32_t m_dwObserverCamTime;
    uint32_t m_dwDamagedTime;
    uint32_t m_dwSpecialAbilitySettingTime;
    uint32_t m_dwCommanderCommandRequestedTime;
    uint32_t m_dwTopMsgTime;
    uint32_t m_dwEnvEffectTime;

    //v2.2
    uint32_t m_dwMonsterEventTime;
    short m_sMonsterID;
    short m_sEventX, m_sEventY;



    bool m_bHunter;
    bool m_bAresden;
    bool m_bCitizen;


    bool m_bZoomMap;
    bool m_bIsProgramActive = true;
    bool m_bCommandAvailable;
    bool m_bSoundFlag;
    bool m_bSoundStat, m_bMusicStat; // On/Off
    bool m_bIsItemEquipped[DEF_MAXITEMS];
    bool m_bIsItemDisabled[DEF_MAXITEMS];
    bool m_bIsGetPointingMode;
    bool m_bEnterPressed, m_bEscPressed, m_bCtrlPressed, m_bRunningMode, m_bShiftPressed;
    bool m_bIsDialogEnabled[41];
    bool m_bDialogTrans;
    bool m_bIsCombatMode;
    bool m_bIsSafeAttackMode;
    bool m_bSkillUsingStatus;
    bool m_bItemUsingStatus;
    bool m_bIsWhetherEffect;
    bool m_bSuperAttackMode;
    bool m_bIsObserverMode, m_bIsObserverCommanded;
    bool m_bIsPoisoned;
    bool m_bIsFirstConn;
    bool m_bIsConfusion;
    bool m_bIsRedrawPDBGS;
    bool m_bDrawFlagDir;
    bool m_bIsCrusadeMode;
    bool m_bIsSpecialAbilityEnabled;
    bool m_bToggleScreen;
    bool m_bIsSpecial;

    bool m_bIsF1HelpWindowEnabled;
    bool m_bIsTeleportRequested;
    bool m_bIsPrevMoveBlocked;
    bool m_bIsHideLocalCursor;
    bool m_bIsCheckingGateway;
    bool m_bForceDisconn;
    bool m_bForceAttack;
    bool m_bParalyze;

    short m_sFrameCount;
    short m_sFPS;
    uint32_t m_dwFPStime;
    bool  m_bShowFPS;

    int m_iFightzoneNumber;
    int m_iFightzoneNumberTemp;
    int m_iPlayerApprColor;
    int m_iHP;			// Hit Point
    int m_iMP;			// Mana Point
    int m_iSP;			// Stamina Point
    int m_iAC;						// Armor Class
    int m_iTHAC0;					// To Hit Armour Class 0

    int m_iLevel, m_iStr, m_iInt, m_iVit, m_iDex, m_iMag, m_iCharisma, m_iExp, m_iContribution;
    int m_iEnemyKillCount;
    int m_iPKCount;
    int m_iRewardGold;
    int m_iGuildRank, m_iTotalGuildsMan;
    int m_iPointCommandType;

    int m_iTotalChar;
    //	int m_iAccountStatus;
    short m_sMagicShortCut;
    int m_iLU_Point;
    int m_iCameraShakingDegree;
    int m_iSuperAttackLeft;
    int m_iAccntYear, m_iAccntMonth, m_iAccntDay;
    int m_iIpYear, m_iIpMonth, m_iIpDay;
    int m_iDownSkillIndex;

    int m_iIlusionOwnerH;
    int m_iApprColor_IE;
    //int m_iInputX, m_iInputY;
    int m_iPDBGSdivX, m_iPDBGSdivY;
    short m_sRecentShortCut;
    short m_sShortCut[5];
    int	m_iSpecialAbilityTimeLeftSec;
    int m_iDrawFlag;
    int m_iSpecialAbilityType;
    int m_iTimeLeftSecAccount, m_iTimeLeftSecIP;
    int m_iCrusadeDuty;
    int m_iLogServerPort;


    int m_iPrevMoveX, m_iPrevMoveY;
    int m_iBlockYear, m_iBlockMonth, m_iBlockDay;
    unsigned char m_iTopMsgLastSec;
    int m_iConstructionPoint;
    int m_iWarContribution;
    int m_iConstructLocX, m_iConstructLocY;
    int m_iNetLagCount;
    int m_iTeleportLocX, m_iTeleportLocY;
    int m_iTotalPartyMember;
    int m_iPartyStatus;
    int m_iGizonItemUpgradeLeft;
    int m_iFeedBackCardIndex;

    short m_sItemEquipmentStatus[DEF_MAXITEMEQUIPPOS];
    short m_sPlayerX, m_sPlayerY;
    short m_sPlayerObjectID;
    short m_sPlayerType;
    short m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4;
    int m_sPlayerStatus;
    short m_sMCX, m_sMCY;
    short m_sCommX, m_sCommY;
    int   m_iCastingMagicType;
    short m_sDamageMove, m_sDamageMoveAmount;
    short m_sAppr1_IE, m_sAppr2_IE, m_sAppr3_IE, m_sAppr4_IE;
    int m_sStatus_IE;
    short m_sViewDstX, m_sViewDstY;
    short m_sViewPointX, m_sViewPointY;
    short m_sVDL_X, m_sVDL_Y;

    uint16_t m_wCommObjectID;
    uint16_t m_wEnterGameType;
    uint16_t m_wR[16], m_wG[16], m_wB[16];
    uint16_t m_wWR[16], m_wWG[16], m_wWB[16];

    unsigned char m_cInputMaxLen;
    char m_cEdit[4];
    char G_cTxt[128];
    char m_cGameModeCount;
    char m_cBGMmapName[12];
    char m_cItemOrder[DEF_MAXITEMS];
    char m_cAmountString[12];
    char m_cLogOutCount;
    char m_cRestartCount;
    char m_cGameMode;
    char m_cWhisperIndex;
    char m_cAccountName[60];
    char m_cAccountPassword[60];
    char m_cAccountAge[12];
    char m_cNewPassword[12];
    char m_cNewPassConfirm[12];
    char m_cAccountCountry[18];
    char m_cAccountSSN[32];
    char m_cEmailAddr[52];
    char m_cAccountQuiz[46];
    char m_cAccountAnswer[22];
    char m_cPlayerName[12];
    char m_cPlayerDir;
    char m_cMsg[200];
    char m_cLocation[12];
    char m_cCurLocation[12];
    char m_cGuildName[22];
    char m_cMCName[12];
    char m_cMapName[12];
    char m_cMapMessage[32];
    char m_cMapIndex;
    char m_cPlayerTurn;
    char m_cCommand;
    char m_cCurFocus, m_cMaxFocus;
    char m_cEnterCheck, m_cTabCheck, m_cLeftArrowCheck;
    char m_cArrowPressed;
    char m_cLogServerAddr[16];
    char m_cChatMsg[64];
    char m_cBackupChatMsg[64];
    char m_cGender, m_cSkinCol, m_cHairStyle, m_cHairCol, m_cUnderCol;
    short m_ccStr, m_ccVit, m_ccDex, m_ccInt, m_ccMag, m_ccChr;
    short m_cLU_Str, m_cLU_Vit, m_cLU_Dex, m_cLU_Int, m_cLU_Mag, m_cLU_Char;
    char m_cDialogBoxOrder[42];
    char m_cMagicMastery[DEF_MAXMAGICTYPE];
    unsigned char m_cSkillMastery[DEF_MAXSKILLTYPE];
    char m_cWorldServerName[32];
    char m_cDetailLevel;
    char m_cMenuDir, m_cMenuDirCnt, m_cMenuFrame;
    char m_cSoundVolume, m_cMusicVolume;
    char m_cWhetherEffectType;
    char m_cWhetherStatus;
    char m_cIlusionOwnerType;
    char m_cName_IE[12];
    char m_sViewDX, m_sViewDY;
    char m_cCommandCount;
    char m_cLoading;
    char m_cDiscount;

    char m_cStatusMapName[12];
    char m_cTopMsg[64];
    char m_cTeleportMapName[12];
    char m_cConstructMapName[12];
    char m_cGameServerName[22]; // v2.04 Gateway


    char m_cItemDrop[25][25];

    RECT m_rcPlayerRect, m_rcBodyRect;
    HWND m_hWnd;

    HANDLE m_hPakFile;


    bool m_bWhisper;
    bool m_bShout;


    bool m_bItemDrop;
    int  m_iItemDropCnt;
};
