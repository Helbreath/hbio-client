//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <fmt/format.h>
#include "sprite.h"
#include "sprite_id.h"
#include "CharInfo.h"
#include "MouseInterface.h"
#include "action_id.h"
#include "Item.h"
#include "MapData.h"
#include "lan_eng.h"

extern int64_t G_iBytesOut, G_iBytesIn;//Change 0x0107
extern int64_t G_iBytesOutLast, G_iBytesInLast;

extern char G_cSpriteAlphaDegree;

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];


extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;//, _tmp_sStatus;
extern int _tmp_iStatus;
extern char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
extern uint16_t  _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern uint16_t  wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int iFocusStatus;
extern int   iFocusApprColor;

void CGame::UpdateScreen()
{
    G_dwGlobalTime = unixtime();
    visible.clear();

    switch (m_cGameMode)
    {
#ifdef DEF_MAKE_ACCOUNT
        case DEF_GAMEMODE_ONAGREEMENT:
            UpdateScreen_OnAgreement();
            break;

        case DEF_GAMEMODE_ONCREATENEWACCOUNT:
            UpdateScreen_OnCreateNewAccount();
            break;
#endif

        case DEF_GAMEMODE_ONVERSIONNOTMATCH:
            UpdateScreen_OnVersionNotMatch();
            break;

        case DEF_GAMEMODE_ONCONNECTING:
            UpdateScreen_OnConnecting();
            break;

        case DEF_GAMEMODE_ONMAINMENU:
            m_bIllusionMVT = false;
            m_iIlusionOwnerH = 0;
            m_cIlusionOwnerType = 0;
            UpdateScreen_OnMainMenu();
            break;

        case DEF_GAMEMODE_ONLOADING:
            UpdateScreen_OnLoading(true);
            break;

        case DEF_GAMEMODE_ONMAINGAME:
            UpdateScreen_OnGame();
            break;

        case DEF_GAMEMODE_ONWAITINGINITDATA:
            UpdateScreen_OnWaitInitData();
            break;

        case DEF_GAMEMODE_ONCONNECTIONLOST:
            UpdateScreen_OnConnectionLost();
            break;

        case DEF_GAMEMODE_ONMSG:
            UpdateScreen_OnMsg();
            break;

        case DEF_GAMEMODE_ONLOGIN:
            UpdateScreen_OnLogin();
            break;

        case DEF_GAMEMODE_ONQUIT:
            UpdateScreen_OnQuit();
            break;

        case DEF_GAMEMODE_ONQUERYFORCELOGIN:
            UpdateScreen_OnQueryForceLogin();
            break;

        case DEF_GAMEMODE_ONSELECTCHARACTER:
            UpdateScreen_OnSelectCharacter();
            break;

        case DEF_GAMEMODE_ONCREATENEWCHARACTER:
            UpdateScreen_OnCreateNewCharacter();
            break;

        case DEF_GAMEMODE_ONWAITINGRESPONSE:
            UpdateScreen_OnWaitingResponse();
            break;

        case DEF_GAMEMODE_ONQUERYDELETECHARACTER:
            UpdateScreen_OnQueryDeleteCharacter();
            break;

        case DEF_GAMEMODE_ONLOGRESMSG:
            UpdateScreen_OnLogResMsg();
            break;
    }

    render_target(DS_VISIBLE);

    // Things rendered over the UI/game are here

    float diffx = static_cast<float>(screenwidth) / screenwidth_v;
    float diffy = static_cast<float>(screenheight) / screenheight_v;
    int mx = int(m_stMCursor.sX * diffx);
    int my = int(m_stMCursor.sY * diffy);

    if (m_bSoundFlag && m_bMusicStat && (m_pBGM.Stopped || m_pBGM.getBuffer() == nullptr) && get_game_mode() == DEF_GAMEMODE_ONMAINGAME)
    {
        StartBGM();
    }

    //Change 0x0107
    put_font_string_size(default_font, 5, 5,
        fmt::format("{} - {}({},{}) Players:({}) Ping({:4}) BW:({:4}bps in|{:4}bps out) FPS:({:3})",
            m_cWorldServerName, m_cMapMessage, m_sPlayerX, m_sPlayerY, m_iTotalUsers, m_iPing,
            (int)G_iBytesInLast, (int)G_iBytesOutLast, fps_counter.getFPS()
        ), Color::White, 12, 0.5, Color::Black);

    visible.display();

    sf::Sprite sprite = sf::Sprite(visible.getTexture());
    sprite.setPosition(0.f, 0.f);
    sprite.setScale(static_cast<float>(screenwidth) / screenwidth_v, static_cast<float>(screenheight) / screenheight_v);

    window.clear();

    window.draw(sprite);

    m_stMCursor.sZ = 0;

    sf::Text TESTTEXT;

    TESTTEXT.setFont(*arya_font);

    int drawcursorx = mx;
    int drawcursory = my - 40;

    if (mx > get_width() - 110)
        drawcursorx = mx - 120;
    if (my < 55)
        drawcursory = my + 50;

    _text.setFont(*arya_font);
    _text.setOutlineThickness(0);
    _text.setOutlineColor(sf::Color::Black);
    _text.setFillColor(sf::Color::White);
    _text.setCharacterSize(14);


    //window.draw(_text);

    _text.setPosition(5.f, 40.f);
    _text.setString(fmt::format("x: {} - y: {}", testx, testy));
    window.draw(_text);

    _text.setPosition(5.f, 80.f);
    _text.setString(fmt::format("x: {} - y: {}", this->m_sPlayerX, this->m_sPlayerY));
    window.draw(_text);
    render_mouse(mx, my);
}


void CGame::UpdateScreen_OnMainMenu()
{
    uint16_t msX{}, msY{}, msZ{};
    char cLB{}, cRB{}, cMIresult{};
    int  iMIbuttonNum{};
    static CMouseInterface * pMI;
    uint32_t dwTime = G_dwGlobalTime;

    m_iItemDropCnt = 0;
    m_bItemDrop = false;

    if (m_cGameModeCount == 0)
    {
        m_stMCursor.sCursorFrame = 0;
        if (m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING] != 0)
        {
            delete m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING];
            m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING] = 0;
        }
        EndInputString();

        pMI = new CMouseInterface;

        pMI->AddRect(384, 177, 548, 198);
        pMI->AddRect(384, 215, 548, 236);
        pMI->AddRect(384, 254, 548, 275);
        //		pMI->AddRect(112,150,247,191);
        //		pMI->AddRect(73,192,281,230);//Change Old Coords
        //		pMI->AddRect(120,231,239,262);
        m_stMCursor.sX = short(screenwidth / 2);
        m_stMCursor.sY = short(screenheight / 2);

        m_cCurFocus = 1;
        m_cMaxFocus = 3;

        m_bEnterPressed = false;
        m_cArrowPressed = 0;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_MAINMENU, -1, -1, 0, true);

    if ((msX >= 384) && (msY >= 177) && (msX <= 548) && (msY <= 198)) m_cCurFocus = 1;
    if ((msX >= 384) && (msY >= 215) && (msX <= 548) && (msY <= 236)) m_cCurFocus = 2;
    if ((msX >= 384) && (msY >= 254) && (msX <= 548) && (msY <= 275)) m_cCurFocus = 3;

    switch (m_cCurFocus)
    {
        case 1:
            m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU]->put_sprite_fast(384, 177, 1, dwTime);
            break;
        case 2:
            m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU]->put_sprite_fast(384, 215, 2, dwTime);
            break;

        case 3:
            m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU]->put_sprite_fast(384, 254, 3, dwTime);
            break;

            /*case 1:
                m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU]->put_sprite_fast(114 , 159, 1, dwTime);
                break;
            case 2:
                m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU]->put_sprite_fast(82, 197, 2, dwTime);
                break;

            case 3:
                m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU]->put_sprite_fast(114, 236, 3, dwTime);
                break;*/
    }

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 1:
                m_cCurFocus--;
                if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
                break;
            case 3:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (m_bEnterPressed == true)
    {
        PlaySound('E', 14, 5);
        m_bEnterPressed = false;
        switch (m_cCurFocus)
        {
            case 1:
                delete pMI;
                ChangeGameMode(DEF_GAMEMODE_ONSELECTSERVER);
                return;
            case 2:
                return;
            case 3:
                delete pMI;
                ChangeGameMode(DEF_GAMEMODE_ONQUIT);
                return;
        }
    }

    DrawVersion();
    update_mouse_state(msX, msY, msZ, cLB, cRB);

    iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
    if (cMIresult == DEF_MIRESULT_CLICK)
    {
        PlaySound('E', 14, 5);
        m_cCurFocus = iMIbuttonNum;
        switch (iMIbuttonNum)
        {
            case 1:
                ChangeGameMode(DEF_GAMEMODE_ONSELECTSERVER);
                delete pMI;
                break;

            case 3:
                ChangeGameMode(DEF_GAMEMODE_ONQUIT);
                delete pMI;
                break;
        }
    }

    //	if ((msX >= 112) && (msY >= 150) && (msX <= 247) && (msY <= 191)) m_cCurFocus = 1;
    //	if ((msX >= 73) && (msY >= 192) && (msX <= 281) && (msY <= 230)) m_cCurFocus = 2;
    //	if ((msX >= 120) && (msY >= 231) && (msX <= 239) && (msY <= 262)) m_cCurFocus = 3;

    //	if (m_cGameModeCount < 6) draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
    //	if (m_cGameModeCount < 2) draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
}

struct load_data
{
    uint32_t id;
    uint32_t sprite_type;
    std::string name;
    uint32_t num;
    bool alpha;
    std::string label;
};

#define SPRITETYPE_SPRITE 1
#define SPRITETYPE_TILE 2
#define SPRITETYPE_EFFECT 3

std::queue<load_data> data_list;
std::size_t data_max = 0;

void CGame::create_load_list()
{
    uint32_t i = 0;

    auto make_sprite = [&](std::string FileName, uint32_t iStart, uint16_t sCount, bool bAlphaEffect, std::string label)
        {
            for (uint32_t i = 0; i < sCount; i++)
                data_list.push({ i + iStart, SPRITETYPE_SPRITE, FileName, i, bAlphaEffect, label });
        };

    auto make_tile_sprite = [&](std::string FileName, uint32_t iStart, uint16_t sCount, bool bAlphaEffect, std::string label)
        {
            for (uint32_t i = 0; i < sCount; i++)
                data_list.push({ i + iStart, SPRITETYPE_TILE, FileName, i, bAlphaEffect, label });
        };

    auto make_effect_sprite = [&](std::string FileName, uint32_t iStart, uint16_t sCount, bool bAlphaEffect, std::string label)
        {
            for (uint32_t i = 0; i < sCount; i++)
                data_list.push({ i + iStart, SPRITETYPE_EFFECT, FileName, i, bAlphaEffect, label });
        };

    make_sprite("bars", 54000, 1, false, "Loading interface");

    data_list.push({ DEF_SPRID_INTERFACE_NEWMAPS1, SPRITETYPE_SPRITE, "Newmaps", 0, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_NEWMAPS2, SPRITETYPE_SPRITE, "Newmaps", 1, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_NEWMAPS3, SPRITETYPE_SPRITE, "Newmaps", 2, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_NEWMAPS4, SPRITETYPE_SPRITE, "Newmaps", 3, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_NEWMAPS5, SPRITETYPE_SPRITE, "Newmaps", 4, false, "Loading interface" });

    data_list.push({ DEF_SPRID_INTERFACE_ND_LOGIN, SPRITETYPE_SPRITE, "LoginDialog", 0, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_NEWACCOUNT, SPRITETYPE_SPRITE, "LoginDialog", 1, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_AGREEMENT, SPRITETYPE_SPRITE, "LoginDialog", 2, false, "Loading interface" });

    data_list.push({ DEF_SPRID_INTERFACE_ND_MAINMENU, SPRITETYPE_SPRITE, "New-Dialog", 1, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_QUIT, SPRITETYPE_SPRITE, "New-Dialog", 2, false, "Loading interface" });

    data_list.push({ DEF_SPRID_INTERFACE_ND_GAME1, SPRITETYPE_SPRITE, "GameDialog", 0, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_GAME2, SPRITETYPE_SPRITE, "GameDialog", 1, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_GAME3, SPRITETYPE_SPRITE, "GameDialog", 2, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_GAME4, SPRITETYPE_SPRITE, "GameDialog", 3, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_CRUSADE, SPRITETYPE_SPRITE, "GameDialog", 4, false, "Loading interface" });
    //data_list.push({ DEF_SPRID_INTERFACE_GUIDEMAP, SPRITETYPE_SPRITE, "GameDialog", 5, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_ICONPANNEL, SPRITETYPE_SPRITE, "GameDialog", 6, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_INVENTORY, SPRITETYPE_SPRITE, "GameDialog", 7, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_SELECTCHAR, SPRITETYPE_SPRITE, "GameDialog", 8, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_NEWCHAR, SPRITETYPE_SPRITE, "GameDialog", 9, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_NEWEXCHANGE, SPRITETYPE_SPRITE, "GameDialog", 10, false, "Loading interface" });

    data_list.push({ DEF_SPRID_INTERFACE_ND_TEXT, SPRITETYPE_SPRITE, "DialogText", 0, false, "Loading interface" });
    data_list.push({ DEF_SPRID_INTERFACE_ND_BUTTON, SPRITETYPE_SPRITE, "DialogText", 1, false, "Loading interface" });


    make_sprite("Telescope", DEF_SPRID_INTERFACE_GUIDEMAP, 32, false, "Loading interface");
    make_sprite("Telescope2", DEF_SPRID_INTERFACE_GUIDEMAP2, 4, false, "Loading interface");
    make_sprite("monster", DEF_SPRID_INTERFACE_MONSTER, 1, false, "Loading interface");

    make_tile_sprite("maptiles1", 0, 32, true, "Loading map data");
    data_list.push({ 51, SPRITETYPE_TILE, "structures1", 1, true, "Loading map data" });
    data_list.push({ 55, SPRITETYPE_TILE, "structures1", 5, true, "Loading map data" });
    make_tile_sprite("Sinside1", 70, 27, false, "Loading map data");
    make_tile_sprite("Trees1", 100, 46, true, "Loading map data");
    make_tile_sprite("TreeShadows", 150, 46, true, "Loading map data");
    make_tile_sprite("objects1", 200, 10, true, "Loading map data");
    make_tile_sprite("objects2", 211, 5, true, "Loading map data");
    make_tile_sprite("objects3", 216, 4, true, "Loading map data");
    make_tile_sprite("objects4", 220, 2, true, "Loading map data");

    make_tile_sprite("Tile223-225", 223, 3, true, "Loading map data");
    make_tile_sprite("Tile226-229", 226, 4, true, "Loading map data");
    make_tile_sprite("objects5", 230, 9, true, "Loading map data");
    make_tile_sprite("objects6", 238, 4, true, "Loading map data");
    make_tile_sprite("objects7", 242, 7, true, "Loading map data");
    make_tile_sprite("maptiles2", 300, 15, true, "Loading map data");
    make_tile_sprite("maptiles4", 320, 10, true, "Loading map data");
    make_tile_sprite("maptiles5", 330, 19, true, "Loading map data");
    make_tile_sprite("maptiles6", 349, 4, true, "Loading map data");
    make_tile_sprite("maptiles353-361", 353, 9, true, "Loading map data");
    make_tile_sprite("Tile363-366", 363, 4, true, "Loading map data");
    make_tile_sprite("Tile367-367", 367, 1, true, "Loading map data");
    make_tile_sprite("Tile370-381", 370, 12, true, "Loading map data"); // Tile370~381
    make_tile_sprite("Tile382-387", 382, 6, true, "Loading map data");
    make_tile_sprite("Tile388-402", 388, 15, true, "Loading map data");


    make_tile_sprite("Tile403-405", 403, 3, true, "Loading map data");
    make_tile_sprite("Tile406-421", 406, 16, true, "Loading map data");
    make_tile_sprite("Tile422-429", 422, 8, true, "Loading map data");
    make_tile_sprite("Tile430-443", 430, 14, true, "Loading map data");
    make_tile_sprite("Tile444-444", 444, 1, true, "Loading map data");
    make_tile_sprite("Tile445-461", 445, 17, true, "Loading map data");

    make_tile_sprite("Tile462-473", 462, 12, true, "Loading map data");
    make_tile_sprite("Tile474-478", 474, 5, true, "Loading map data");
    make_tile_sprite("Tile479-488", 479, 10, true, "Loading map data");
    make_tile_sprite("Tile489-522", 489, 34, true, "Loading map data"); // Drunken City
    make_tile_sprite("Tile523-530", 523, 8, true, "Loading map data");  // Rampart
    make_tile_sprite("Tile531-540", 531, 10, true, "Loading map data"); // GodH + Pont
    make_tile_sprite("Tile541-545", 541, 5, true, "Loading map data");  // GodH

    make_sprite("item-pack", DEF_SPRID_ITEMPACK_PIVOTPOINT + 1, 17, false, "Loading items");
    data_list.push({ DEF_SPRID_ITEMPACK_PIVOTPOINT + 20, SPRITETYPE_SPRITE, "item-pack", 17, false, "Loading items" });
    data_list.push({ DEF_SPRID_ITEMPACK_PIVOTPOINT + 21, SPRITETYPE_SPRITE, "item-pack", 18, false, "Loading items" });
    make_sprite("item-ground", DEF_SPRID_ITEMGROUND_PIVOTPOINT + 1, 17, false, "Loading items");
    data_list.push({ DEF_SPRID_ITEMGROUND_PIVOTPOINT + 20, SPRITETYPE_SPRITE, "item-ground", 17, false, "Loading items" });
    data_list.push({ DEF_SPRID_ITEMGROUND_PIVOTPOINT + 21, SPRITETYPE_SPRITE, "item-ground", 18, false, "Loading items" });
    make_sprite("item-dynamic", DEF_SPRID_ITEMDYNAMIC_PIVOTPOINT, 3, false, "Loading items");

    // MALE
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 0, SPRITETYPE_SPRITE, "item-equipM", 0, false, "Loading items" });   // body
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 1, SPRITETYPE_SPRITE, "item-equipM", 1, false, "Loading items" });   // 1-swords
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 2, SPRITETYPE_SPRITE, "item-equipM", 2, false, "Loading items" });   // 2-bows
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 3, SPRITETYPE_SPRITE, "item-equipM", 3, false, "Loading items" });   // 3-shields
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 4, SPRITETYPE_SPRITE, "item-equipM", 4, false, "Loading items" });   // 4-tunics
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 5, SPRITETYPE_SPRITE, "item-equipM", 5, false, "Loading items" });   // 5-shoes
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 7, SPRITETYPE_SPRITE, "item-equipM", 6, false, "Loading items" });   // 6-berk
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 8, SPRITETYPE_SPRITE, "item-equipM", 7, false, "Loading items" });   // 7-hoses
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 9, SPRITETYPE_SPRITE, "item-equipM", 8, false, "Loading items" });   // 8-bodyarmor
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 15, SPRITETYPE_SPRITE, "item-equipM", 11, false, "Loading items" }); // Axe hammer
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 17, SPRITETYPE_SPRITE, "item-equipM", 12, false, "Loading items" }); // Wands
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 18, SPRITETYPE_SPRITE, "item-equipM", 9, false, "Loading items" });  // hair
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 19, SPRITETYPE_SPRITE, "item-equipM", 10, false, "Loading items" }); // undies
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 20, SPRITETYPE_SPRITE, "item-equipM", 13, false, "Loading items" }); // capes
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 21, SPRITETYPE_SPRITE, "item-equipM", 14, false, "Loading items" }); // helm

    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 16, SPRITETYPE_SPRITE, "item-pack", 15, false, "Loading items" }); // Necks, Angels, Pendants

    // FEMALE
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 40, SPRITETYPE_SPRITE, "item-equipW", 0, false, "Loading items" });  // body
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 41, SPRITETYPE_SPRITE, "item-equipW", 1, false, "Loading items" });  // 1-swords
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 42, SPRITETYPE_SPRITE, "item-equipW", 2, false, "Loading items" });  // 2-bows
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 43, SPRITETYPE_SPRITE, "item-equipW", 3, false, "Loading items" });  // 3-shields
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 45, SPRITETYPE_SPRITE, "item-equipW", 4, false, "Loading items" });  // 4-tunics
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 50, SPRITETYPE_SPRITE, "item-equipW", 5, false, "Loading items" });  // 5-shoes
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 51, SPRITETYPE_SPRITE, "item-equipW", 6, false, "Loading items" });  // 6-berk
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 52, SPRITETYPE_SPRITE, "item-equipW", 7, false, "Loading items" });  // 7-hoses
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 53, SPRITETYPE_SPRITE, "item-equipW", 8, false, "Loading items" });  // 8-bodyarmor
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 55, SPRITETYPE_SPRITE, "item-equipW", 11, false, "Loading items" }); // Axe hammer
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 57, SPRITETYPE_SPRITE, "item-equipW", 12, false, "Loading items" }); // Wands
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 58, SPRITETYPE_SPRITE, "item-equipW", 9, false, "Loading items" });  // hair
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 59, SPRITETYPE_SPRITE, "item-equipW", 10, false, "Loading items" }); // undies
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 60, SPRITETYPE_SPRITE, "item-equipW", 13, false, "Loading items" }); // capes
    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 61, SPRITETYPE_SPRITE, "item-equipW", 14, false, "Loading items" }); // helm

    data_list.push({ DEF_SPRID_ITEMEQUIP_PIVOTPOINT + 56, SPRITETYPE_SPRITE, "item-pack", 15, false, "Loading items" }); // Necks, Angels, Pendants

    make_sprite("Bm", 500 + 15 * 8 * 0, 96, true, "Loading characters"); // Black Man (Type: 1)
    make_sprite("Wm", 500 + 15 * 8 * 1, 96, true, "Loading characters"); // White Man (Type: 2)
    make_sprite("Ym", 500 + 15 * 8 * 2, 96, true, "Loading characters"); // Yellow Man (Type: 3)
    make_sprite("Bw", 500 + 15 * 8 * 3, 96, true, "Loading characters"); // Black Woman (Type: 4)
    make_sprite("Ww", 500 + 15 * 8 * 4, 96, true, "Loading characters"); // White Woman (Type: 5)
    make_sprite("Yw", 500 + 15 * 8 * 5, 96, true, "Loading characters"); // Yellow Woman (Type: 6)

    //     make_sprite("TutelarAngel1", DEF_SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 0, 48, false, "Loading angels"); //(STR)
    //     make_sprite("TutelarAngel2", DEF_SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 1, 48, false, "Loading angels"); //(DEX)
    //     make_sprite("TutelarAngel3", DEF_SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 2, 48, false, "Loading angels"); //(INT)
    //     make_sprite("TutelarAngel4", DEF_SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 3, 48, false, "Loading angels"); //(MAG)

    make_sprite("slm", DEF_SPRID_NPC + 7 * 8 * 0, 40, true, "Loading npcs");    // Slime (Type: 10)
    make_sprite("ske", DEF_SPRID_NPC + 7 * 8 * 1, 40, true, "Loading npcs");    // Skeleton (Type: 11)
    make_sprite("Gol", DEF_SPRID_NPC + 7 * 8 * 2, 40, true, "Loading npcs");    // Stone-Golem (Type: 12)
    make_sprite("Cyc", DEF_SPRID_NPC + 7 * 8 * 3, 40, true, "Loading npcs");    // Cyclops (Type: 13)
    make_sprite("Orc", DEF_SPRID_NPC + 7 * 8 * 4, 40, true, "Loading npcs");    // Orc (Type: 14)
    make_sprite("Shopkpr", DEF_SPRID_NPC + 7 * 8 * 5, 8, true, "Loading npcs"); // ShopKeeper-Woman (Type: 15)
    make_sprite("Ant", DEF_SPRID_NPC + 7 * 8 * 6, 40, true, "Loading npcs");    //  Giant-Ant (Type: 16)
    make_sprite("Scp", DEF_SPRID_NPC + 7 * 8 * 7, 40, true, "Loading npcs");    //  Scorpion (Type: 17)
    make_sprite("Zom", DEF_SPRID_NPC + 7 * 8 * 8, 40, true, "Loading npcs");    //  Zombie (Type: 18)
    make_sprite("Gandlf", DEF_SPRID_NPC + 7 * 8 * 9, 8, true, "Loading npcs");
    make_sprite("Howard", DEF_SPRID_NPC + 7 * 8 * 10, 8, true, "Loading npcs");

    make_sprite("Guard", DEF_SPRID_NPC + 7 * 8 * 11, 40, true, "Loading npcs");  // Guard (Type: 21)
    make_sprite("Amp", DEF_SPRID_NPC + 7 * 8 * 12, 40, true, "Loading npcs");    // Amphis (Type: 22)
    make_sprite("Cla", DEF_SPRID_NPC + 7 * 8 * 13, 40, true, "Loading npcs");    // Clay-Golem (Type: 23)
    make_sprite("tom", DEF_SPRID_NPC + 7 * 8 * 14, 8, true, "Loading npcs");     // Tom (Type: 24)
    make_sprite("William", DEF_SPRID_NPC + 7 * 8 * 15, 8, true, "Loading npcs"); // William (Type: 25)
    make_sprite("Kennedy", DEF_SPRID_NPC + 7 * 8 * 16, 8, true, "Loading npcs");        // Kennedy (Type: 26)
    make_sprite("Helb", DEF_SPRID_NPC + 7 * 8 * 17, 40, true, "Loading npcs");          // Hellbound (Type: 27)
    make_sprite("Troll", DEF_SPRID_NPC + 7 * 8 * 18, 40, true, "Loading npcs");         // Troll (Type: 28)
    make_sprite("Orge", DEF_SPRID_NPC + 7 * 8 * 19, 40, true, "Loading npcs");          // Orge (Type: 29)
    make_sprite("Liche", DEF_SPRID_NPC + 7 * 8 * 20, 40, true, "Loading npcs");         // Liche (Type: 30)
    make_sprite("Demon", DEF_SPRID_NPC + 7 * 8 * 21, 40, true, "Loading npcs");         // Demon (Type: 31)
    make_sprite("Unicorn", DEF_SPRID_NPC + 7 * 8 * 22, 40, true, "Loading npcs");     // Unicorn (Type: 32)
    make_sprite("WereWolf", DEF_SPRID_NPC + 7 * 8 * 23, 40, true, "Loading npcs");    // WereWolf (Type: 33)
    make_sprite("Dummy", DEF_SPRID_NPC + 7 * 8 * 24, 40, true, "Loading npcs");       // Dummy (Type: 34)

    for (i = 0; i < 40; i++)
        data_list.push({ DEF_SPRID_NPC + i + 7 * 8 * 25, SPRITETYPE_SPRITE, "Effect5", 0, false, "Loading npcs" }); // Energy-Ball (Type: 35) 

    make_sprite("GT-Arrow", DEF_SPRID_NPC + 7 * 8 * 26, 40, true, "Loading npcs");        // Arrow-GuardTower (Type: 36)
    make_sprite("GT-Cannon", DEF_SPRID_NPC + 7 * 8 * 27, 40, true, "Loading npcs");       // Cannon-GuardTower (Type: 37)
    make_sprite("ManaCollector", DEF_SPRID_NPC + 7 * 8 * 28, 40, true, "Loading npcs");   // Mana Collector (Type: 38)
    make_sprite("Detector", DEF_SPRID_NPC + 7 * 8 * 29, 40, true, "Loading npcs");        // Detector (Type: 39)
    make_sprite("ESG", DEF_SPRID_NPC + 7 * 8 * 30, 40, true, "Loading npcs");             // ESG (Type: 40)
    make_sprite("GMG", DEF_SPRID_NPC + 7 * 8 * 31, 40, true, "Loading npcs");             // GMG (Type: 41)
    make_sprite("ManaStone", DEF_SPRID_NPC + 7 * 8 * 32, 40, true, "Loading npcs");       // ManaStone (Type: 42)
    make_sprite("LWB", DEF_SPRID_NPC + 7 * 8 * 33, 40, true, "Loading npcs");             // Light War Beetle (Type: 43)
    make_sprite("GHK", DEF_SPRID_NPC + 7 * 8 * 34, 40, true, "Loading npcs");             // God's Hand Knight (Type: 44)
    make_sprite("GHKABS", DEF_SPRID_NPC + 7 * 8 * 35, 40, true, "Loading npcs");          // God's Hand Knight with Armored Battle Steed (Type: 45)
    make_sprite("TK", DEF_SPRID_NPC + 7 * 8 * 36, 40, true, "Loading npcs");              // Temple Knight (Type: 46)
    make_sprite("BG", DEF_SPRID_NPC + 7 * 8 * 37, 40, true, "Loading npcs");              // Battle Golem (Type: 47)
    make_sprite("Stalker", DEF_SPRID_NPC + 7 * 8 * 38, 40, true, "Loading npcs");         // Stalker (Type: 48)
    make_sprite("Hellclaw", DEF_SPRID_NPC + 7 * 8 * 39, 40, true, "Loading npcs");        // Hellclaw (Type: 49)
    make_sprite("Tigerworm", DEF_SPRID_NPC + 7 * 8 * 40, 40, true, "Loading npcs");       // Tigerworm (Type: 50)
    make_sprite("Catapult", DEF_SPRID_NPC + 7 * 8 * 41, 40, true, "Loading npcs");        // Catapult (Type: 51)
    make_sprite("Gagoyle", DEF_SPRID_NPC + 7 * 8 * 42, 40, true, "Loading npcs");         // Gargoyle (Type: 52)
    make_sprite("EnragedGagoyle", DEF_SPRID_NPC + 7 * 8 * 115, 40, true, "Loading npcs"); // Enraged Gagoyle (Type: 125)
    make_sprite("Beholder", DEF_SPRID_NPC + 7 * 8 * 43, 40, true, "Loading npcs");        // Beholder (Type: 53)
    make_sprite("DarkElf", DEF_SPRID_NPC + 7 * 8 * 44, 40, true, "Loading npcs");         // Dark-Elf (Type: 54)
    make_sprite("Bunny", DEF_SPRID_NPC + 7 * 8 * 45, 40, true, "Loading npcs");           // Bunny (Type: 55)
    make_sprite("Cat", DEF_SPRID_NPC + 7 * 8 * 46, 40, true, "Loading npcs");             // Cat (Type: 56)
    make_sprite("GiantFrog", DEF_SPRID_NPC + 7 * 8 * 47, 40, true, "Loading npcs");       // GiantFrog (Type: 57)
    make_sprite("MTGiant", DEF_SPRID_NPC + 7 * 8 * 48, 40, true, "Loading npcs");         // Mountain Giant (Type: 58)
    make_sprite("Ettin", DEF_SPRID_NPC + 7 * 8 * 49, 40, true, "Loading npcs");           // Ettin (Type: 59)
    make_sprite("CanPlant", DEF_SPRID_NPC + 7 * 8 * 50, 40, true, "Loading npcs");        // Cannibal Plant (Type: 60)
    make_sprite("Rudolph", DEF_SPRID_NPC + 7 * 8 * 51, 40, true, "Loading npcs");         // Rudolph (Type: 61)
    make_sprite("DireBoar", DEF_SPRID_NPC + 7 * 8 * 52, 40, true, "Loading npcs");        // Boar (Type: 62)
    make_sprite("frost", DEF_SPRID_NPC + 7 * 8 * 53, 40, true, "Loading npcs");           // Frost (Type: 63)
    make_sprite("Crop", DEF_SPRID_NPC + 7 * 8 * 54, 40, true, "Loading npcs");            // Crop(Type: 64)
    make_sprite("IceGolem", DEF_SPRID_NPC + 7 * 8 * 55, 40, true, "Loading npcs");        // IceGolem (Type: 65)
    make_sprite("Wyvern", DEF_SPRID_NPC + 7 * 8 * 56, 24, true, "Loading npcs");          // Wyvern (Type: 66)
    make_sprite("McGaffin", DEF_SPRID_NPC + 7 * 8 * 57, 16, true, "Loading npcs");        // McGaffin (Type: 67)
    make_sprite("Perry", DEF_SPRID_NPC + 7 * 8 * 58, 16, true, "Loading npcs");           // Perry (Type: 68)
    make_sprite("Devlin", DEF_SPRID_NPC + 7 * 8 * 59, 16, true, "Loading npcs");          // Devlin (Type: 69)

    make_sprite("Barlog", DEF_SPRID_NPC + 7 * 8 * 60, 40, true, "Loading npcs");          // Barlog (Type: 70)
    make_sprite("Centaurus", DEF_SPRID_NPC + 7 * 8 * 61, 40, true, "Loading npcs");       // Centaurus (Type: 71)
    make_sprite("ClawTurtle", DEF_SPRID_NPC + 7 * 8 * 62, 40, true, "Loading npcs");      // Claw-Turtle (Type: 72)
    make_sprite("FireWyvern", DEF_SPRID_NPC + 7 * 8 * 63, 24, true, "Loading npcs");      // Fire-Wyvern (Type: 73)
    make_sprite("GiantCrayfish", DEF_SPRID_NPC + 7 * 8 * 64, 40, true, "Loading npcs");   // Giant-Crayfish (Type: 74)
    make_sprite("GiantLizard", DEF_SPRID_NPC + 7 * 8 * 65, 40, true, "Loading npcs");     // Giant-Lizard (Type: 75)
    make_sprite("GiantPlant", DEF_SPRID_NPC + 7 * 8 * 66, 40, true, "Loading npcs");       // Giant-Plant (Type: 76)
    make_sprite("MasterMageOrc", DEF_SPRID_NPC + 7 * 8 * 67, 40, true, "Loading npcs");    // MasterMage-Orc (Type: 77)
    make_sprite("Minotaurs", DEF_SPRID_NPC + 7 * 8 * 68, 40, true, "Loading npcs");        // Minotaurs (Type: 78)
    make_sprite("Nizie", DEF_SPRID_NPC + 7 * 8 * 69, 40, true, "Loading npcs");            // Nizie (Type: 79)
    make_sprite("Tentocle", DEF_SPRID_NPC + 7 * 8 * 70, 40, true, "Loading npcs");         // Tentocle (Type: 80)
    make_sprite("yspro", DEF_SPRID_NPC + 7 * 8 * 71, 32, true, "Loading npcs");            // Abaddon (Type: 81)
    make_sprite("Sorceress", DEF_SPRID_NPC + 7 * 8 * 72, 40, true, "Loading npcs");        // Sorceress (Type: 82)
    make_sprite("TPKnight", DEF_SPRID_NPC + 7 * 8 * 73, 40, true, "Loading npcs");         // TPKnight (Type: 83)
    make_sprite("ElfMaster", DEF_SPRID_NPC + 7 * 8 * 74, 40, true, "Loading npcs");        // ElfMaster (Type: 84)
    make_sprite("DarkKnight", DEF_SPRID_NPC + 7 * 8 * 75, 40, true, "Loading npcs");       // DarkKnight (Type: 85)
    make_sprite("HBTank", DEF_SPRID_NPC + 7 * 8 * 76, 32, true, "Loading npcs");           // HeavyBattleTank (Type: 86)
    make_sprite("CBTurret", DEF_SPRID_NPC + 7 * 8 * 77, 32, true, "Loading npcs");         // CBTurret (Type: 87)
    make_sprite("Babarian", DEF_SPRID_NPC + 7 * 8 * 78, 40, true, "Loading npcs");         // Babarian (Type: 88)
    make_sprite("ACannon", DEF_SPRID_NPC + 7 * 8 * 79, 32, true, "Loading npcs");          // ACannon (Type: 89)
    make_sprite("Gail", DEF_SPRID_NPC + 7 * 8 * 80, 8, true, "Loading npcs");  // Gail (Type: 90)
    make_sprite("Gate", DEF_SPRID_NPC + 7 * 8 * 81, 24, true, "Loading npcs"); // Heldenian Gate (Type: 91)

    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_M + i + 15 * 0, SPRITETYPE_SPRITE, "Mpt", i + 12 * 0, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_M + i + 15 * 1, SPRITETYPE_SPRITE, "Mpt", i + 12 * 1, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_M + i + 15 * 2, SPRITETYPE_SPRITE, "Mpt", i + 12 * 2, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_M + i + 15 * 3, SPRITETYPE_SPRITE, "Mpt", i + 12 * 3, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_M + i + 15 * 4, SPRITETYPE_SPRITE, "Mpt", i + 12 * 4, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_M + i + 15 * 5, SPRITETYPE_SPRITE, "Mpt", i + 12 * 5, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_M + i + 15 * 6, SPRITETYPE_SPRITE, "Mpt", i + 12 * 6, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_M + i + 15 * 7, SPRITETYPE_SPRITE, "Mpt", i + 12 * 7, false, "Loading items" });

    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_M + i + 15 * 0, SPRITETYPE_SPRITE, "Mhr", i + 12 * 0, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_M + i + 15 * 1, SPRITETYPE_SPRITE, "Mhr", i + 12 * 1, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_M + i + 15 * 2, SPRITETYPE_SPRITE, "Mhr", i + 12 * 2, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_M + i + 15 * 3, SPRITETYPE_SPRITE, "Mhr", i + 12 * 3, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_M + i + 15 * 4, SPRITETYPE_SPRITE, "Mhr", i + 12 * 4, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_M + i + 15 * 5, SPRITETYPE_SPRITE, "Mhr", i + 12 * 5, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_M + i + 15 * 6, SPRITETYPE_SPRITE, "Mhr", i + 12 * 6, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_M + i + 15 * 7, SPRITETYPE_SPRITE, "Mhr", i + 12 * 7, false, "Loading items" });

    make_sprite("MLArmor", DEF_SPRID_ARMOR_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MCMail", DEF_SPRID_ARMOR_M + 15 * 2, 12, true, "Loading items");
    make_sprite("MSMail", DEF_SPRID_ARMOR_M + 15 * 3, 12, true, "Loading items");
    make_sprite("MPMail", DEF_SPRID_ARMOR_M + 15 * 4, 12, true, "Loading items");
    make_sprite("Mtunic", DEF_SPRID_ARMOR_M + 15 * 5, 12, true, "Loading items");
    make_sprite("MRobe1", DEF_SPRID_ARMOR_M + 15 * 6, 12, true, "Loading items");
    make_sprite("MSanta", DEF_SPRID_ARMOR_M + 15 * 7, 12, true, "Loading items");
    make_sprite("MHPMail1", DEF_SPRID_ARMOR_M + 15 * 8, 12, true, "Loading items");       //hero
    make_sprite("MHPMail2", DEF_SPRID_ARMOR_M + 15 * 9, 12, true, "Loading items");       //hero
    make_sprite("MHRobe1", DEF_SPRID_ARMOR_M + 15 * 10, 12, true, "Loading items");       //hero
    make_sprite("MHRobe2", DEF_SPRID_ARMOR_M + 15 * 11, 12, true, "Loading items");       //hero

    make_sprite("MShirt", DEF_SPRID_HAUBERK_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MHauberk", DEF_SPRID_HAUBERK_M + 15 * 2, 12, true, "Loading items");
    make_sprite("MHHauberk1", DEF_SPRID_HAUBERK_M + 15 * 3, 12, true, "Loading items");
    make_sprite("MHHauberk2", DEF_SPRID_HAUBERK_M + 15 * 4, 12, true, "Loading items");

    make_sprite("MTrouser", DEF_SPRID_LEGS_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MHTrouser", DEF_SPRID_LEGS_M + 15 * 2, 12, true, "Loading items");
    make_sprite("MCHoses", DEF_SPRID_LEGS_M + 15 * 3, 12, true, "Loading items");
    make_sprite("MLeggings", DEF_SPRID_LEGS_M + 15 * 4, 12, true, "Loading items");
    make_sprite("MHLeggings1", DEF_SPRID_LEGS_M + 15 * 5, 12, true, "Loading items");   // hero
    make_sprite("MHLeggings2", DEF_SPRID_LEGS_M + 15 * 6, 12, true, "Loading items");   // hero
    make_sprite("MShoes", DEF_SPRID_BOOT_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MLBoots", DEF_SPRID_BOOT_M + 15 * 2, 12, true, "Loading items");

    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 1, SPRITETYPE_SPRITE, "Msw", i + 56 * 0, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 2, SPRITETYPE_SPRITE, "Msw", i + 56 * 1, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 3, SPRITETYPE_SPRITE, "Msw", i + 56 * 2, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 4, SPRITETYPE_SPRITE, "Msw", i + 56 * 3, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 5, SPRITETYPE_SPRITE, "Msw", i + 56 * 4, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 6, SPRITETYPE_SPRITE, "Msw", i + 56 * 5, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 7, SPRITETYPE_SPRITE, "Msw", i + 56 * 6, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 8, SPRITETYPE_SPRITE, "Msw", i + 56 * 7, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 9, SPRITETYPE_SPRITE, "Msw", i + 56 * 8, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 10, SPRITETYPE_SPRITE, "Msw", i + 56 * 9, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 11, SPRITETYPE_SPRITE, "Msw", i + 56 * 10, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 12, SPRITETYPE_SPRITE, "Msw", i + 56 * 11, false, "Loading items" });

    make_sprite("Mswx", DEF_SPRID_WEAPON_M + 64 * 5, 56, true, "Loading items");
    make_sprite("Msw2", DEF_SPRID_WEAPON_M + 64 * 13, 56, true, "Loading items");
    make_sprite("Msw3", DEF_SPRID_WEAPON_M + 64 * 14, 56, true, "Loading items");
    make_sprite("MStormBringer", DEF_SPRID_WEAPON_M + 64 * 15, 56, true, "Loading items");
    make_sprite("MDarkExec", DEF_SPRID_WEAPON_M + 64 * 16, 56, true, "Loading items");
    make_sprite("MKlonessBlade", DEF_SPRID_WEAPON_M + 64 * 17, 56, true, "Loading items");
    make_sprite("MKlonessAstock", DEF_SPRID_WEAPON_M + 64 * 18, 56, true, "Loading items");
    make_sprite("MDebastator", DEF_SPRID_WEAPON_M + 64 * 19, 56, true, "Loading items");
    make_sprite("MAxe1", DEF_SPRID_WEAPON_M + 64 * 20, 56, true, "Loading items"); // Axe
    make_sprite("MAxe2", DEF_SPRID_WEAPON_M + 64 * 21, 56, true, "Loading items");
    make_sprite("MAxe3", DEF_SPRID_WEAPON_M + 64 * 22, 56, true, "Loading items");
    make_sprite("MAxe4", DEF_SPRID_WEAPON_M + 64 * 23, 56, true, "Loading items");
    make_sprite("MAxe5", DEF_SPRID_WEAPON_M + 64 * 24, 56, true, "Loading items");
    make_sprite("MPickAxe1", DEF_SPRID_WEAPON_M + 64 * 25, 56, true, "Loading items");
    make_sprite("MAxe6", DEF_SPRID_WEAPON_M + 64 * 26, 56, true, "Loading items");
    make_sprite("Mhoe", DEF_SPRID_WEAPON_M + 64 * 27, 56, true, "Loading items");
    make_sprite("MKlonessAxe", DEF_SPRID_WEAPON_M + 64 * 28, 56, true, "Loading items");
    make_sprite("MLightBlade", DEF_SPRID_WEAPON_M + 64 * 29, 56, true, "Loading items");
    make_sprite("MHammer", DEF_SPRID_WEAPON_M + 64 * 30, 56, true, "Loading items");
    make_sprite("MBHammer", DEF_SPRID_WEAPON_M + 64 * 31, 56, true, "Loading items");
    make_sprite("MBabHammer", DEF_SPRID_WEAPON_M + 64 * 32, 56, true, "Loading items");
    make_sprite("MBShadowSword", DEF_SPRID_WEAPON_M + 64 * 33, 56, true, "Loading items");
    make_sprite("MBerserkWand", DEF_SPRID_WEAPON_M + 64 * 34, 56, true, "Loading items");
    make_sprite("Mstaff1", DEF_SPRID_WEAPON_M + 64 * 35, 56, true, "Loading items"); // Staff
    make_sprite("Mstaff2", DEF_SPRID_WEAPON_M + 64 * 36, 56, true, "Loading items");
    make_sprite("MStaff3", DEF_SPRID_WEAPON_M + 64 * 37, 56, true, "Loading items");
    make_sprite("MReMagicWand", DEF_SPRID_WEAPON_M + 64 * 38, 56, true, "Loading items");
    make_sprite("MKlonessWand", DEF_SPRID_WEAPON_M + 64 * 39, 56, true, "Loading items");
    //     make_sprite("Staff4M", DEF_SPRID_WEAPON_M + 64 * 40, 56, true, "Loading items");

    // Bows 40 41 below
    //make_sprite("Mbo", DEF_SPRID_WEAPON_M + 64 * 41, 56, true, "Loading items");
    make_sprite("MDirectBow", DEF_SPRID_WEAPON_M + 64 * 43, 56, true, "Loading items");
    make_sprite("MFireBow", DEF_SPRID_WEAPON_M + 64 * 44, 56, true, "Loading items");

    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_M + i + 64 * 42, SPRITETYPE_SPRITE, "Mbo", i + 56 * 1, false, "Loading items" });

    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_M + i + 8 * 1, SPRITETYPE_SPRITE, "Msh", i + 7 * 0, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_M + i + 8 * 2, SPRITETYPE_SPRITE, "Msh", i + 7 * 1, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_M + i + 8 * 3, SPRITETYPE_SPRITE, "Msh", i + 7 * 2, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_M + i + 8 * 4, SPRITETYPE_SPRITE, "Msh", i + 7 * 3, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_M + i + 8 * 5, SPRITETYPE_SPRITE, "Msh", i + 7 * 4, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_M + i + 8 * 6, SPRITETYPE_SPRITE, "Msh", i + 7 * 5, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_M + i + 8 * 7, SPRITETYPE_SPRITE, "Msh", i + 7 * 6, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_M + i + 8 * 8, SPRITETYPE_SPRITE, "Msh", i + 7 * 7, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_M + i + 8 * 9, SPRITETYPE_SPRITE, "Msh", i + 7 * 8, false, "Loading items" });

    make_sprite("Mmantle01", DEF_SPRID_CAPE_M + 15 * 1, 12, true, "Loading items");
    make_sprite("Mmantle02", DEF_SPRID_CAPE_M + 15 * 2, 12, true, "Loading items");
    make_sprite("Mmantle03", DEF_SPRID_CAPE_M + 15 * 3, 12, true, "Loading items");
    make_sprite("Mmantle04", DEF_SPRID_CAPE_M + 15 * 4, 12, true, "Loading items");
    make_sprite("Mmantle05", DEF_SPRID_CAPE_M + 15 * 5, 12, true, "Loading items");
    make_sprite("Mmantle06", DEF_SPRID_CAPE_M + 15 * 6, 12, true, "Loading items");

    make_sprite("MHelm1", DEF_SPRID_HEAD_M + 15 * 1, 12, true, "Loading items");
    make_sprite("MHelm2", DEF_SPRID_HEAD_M + 15 * 2, 12, true, "Loading items");
    make_sprite("MHelm3", DEF_SPRID_HEAD_M + 15 * 3, 12, true, "Loading items");
    make_sprite("MHelm4", DEF_SPRID_HEAD_M + 15 * 4, 12, true, "Loading items");
    make_sprite("NMHelm1", DEF_SPRID_HEAD_M + 15 * 5, 12, true, "Loading items");
    make_sprite("NMHelm2", DEF_SPRID_HEAD_M + 15 * 6, 12, true, "Loading items");
    make_sprite("NMHelm3", DEF_SPRID_HEAD_M + 15 * 7, 12, true, "Loading items");
    make_sprite("NMHelm4", DEF_SPRID_HEAD_M + 15 * 8, 12, true, "Loading items");

    make_sprite("MHHelm1", DEF_SPRID_HEAD_M + 15 * 9, 12, true, "Loading items");
    make_sprite("MHHelm2", DEF_SPRID_HEAD_M + 15 * 10, 12, true, "Loading items");
    make_sprite("MHCap1", DEF_SPRID_HEAD_M + 15 * 11, 12, true, "Loading items");
    make_sprite("MHCap2", DEF_SPRID_HEAD_M + 15 * 12, 12, true, "Loading items");

    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_W + i + 15 * 0, SPRITETYPE_SPRITE, "Wpt", i + 12 * 0, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_W + i + 15 * 1, SPRITETYPE_SPRITE, "Wpt", i + 12 * 1, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_W + i + 15 * 2, SPRITETYPE_SPRITE, "Wpt", i + 12 * 2, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_W + i + 15 * 3, SPRITETYPE_SPRITE, "Wpt", i + 12 * 3, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_W + i + 15 * 4, SPRITETYPE_SPRITE, "Wpt", i + 12 * 4, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_W + i + 15 * 5, SPRITETYPE_SPRITE, "Wpt", i + 12 * 5, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_W + i + 15 * 6, SPRITETYPE_SPRITE, "Wpt", i + 12 * 6, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_UNDERWEAR_W + i + 15 * 7, SPRITETYPE_SPRITE, "Wpt", i + 12 * 7, false, "Loading items" });

    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_W + i + 15 * 0, SPRITETYPE_SPRITE, "Whr", i + 12 * 0, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_W + i + 15 * 1, SPRITETYPE_SPRITE, "Whr", i + 12 * 1, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_W + i + 15 * 2, SPRITETYPE_SPRITE, "Whr", i + 12 * 2, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_W + i + 15 * 3, SPRITETYPE_SPRITE, "Whr", i + 12 * 3, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_W + i + 15 * 4, SPRITETYPE_SPRITE, "Whr", i + 12 * 4, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_W + i + 15 * 5, SPRITETYPE_SPRITE, "Whr", i + 12 * 5, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_W + i + 15 * 6, SPRITETYPE_SPRITE, "Whr", i + 12 * 6, false, "Loading items" });
    for (i = 0; i < 12; i++)
        data_list.push({ DEF_SPRID_HAIR_W + i + 15 * 7, SPRITETYPE_SPRITE, "Whr", i + 12 * 7, false, "Loading items" });

    make_sprite("WBodice1", DEF_SPRID_ARMOR_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WBodice2", DEF_SPRID_ARMOR_W + 15 * 2, 12, true, "Loading items");
    make_sprite("WLArmor", DEF_SPRID_ARMOR_W + 15 * 3, 12, true, "Loading items");
    make_sprite("WCMail", DEF_SPRID_ARMOR_W + 15 * 4, 12, true, "Loading items");
    make_sprite("WSMail", DEF_SPRID_ARMOR_W + 15 * 5, 12, true, "Loading items");
    make_sprite("WPMail", DEF_SPRID_ARMOR_W + 15 * 6, 12, true, "Loading items");
    make_sprite("WRobe1", DEF_SPRID_ARMOR_W + 15 * 7, 12, true, "Loading items");
    make_sprite("WSanta", DEF_SPRID_ARMOR_W + 15 * 8, 12, true, "Loading items");
    make_sprite("WHPMail1", DEF_SPRID_ARMOR_W + 15 * 9, 12, true, "Loading items");       //hero
    make_sprite("WHPMail2", DEF_SPRID_ARMOR_W + 15 * 10, 12, true, "Loading items");      //hero
    make_sprite("WHRobe1", DEF_SPRID_ARMOR_W + 15 * 11, 12, true, "Loading items");       // hero
    make_sprite("WHRobe2", DEF_SPRID_ARMOR_W + 15 * 12, 12, true, "Loading items");       // hero

    make_sprite("WChemiss", DEF_SPRID_HAUBERK_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WShirt", DEF_SPRID_HAUBERK_W + 15 * 2, 12, true, "Loading items");
    make_sprite("WHauberk", DEF_SPRID_HAUBERK_W + 15 * 3, 12, true, "Loading items");
    make_sprite("WHHauberk1", DEF_SPRID_HAUBERK_W + 15 * 4, 12, true, "Loading items");
    make_sprite("WHHauberk2", DEF_SPRID_HAUBERK_W + 15 * 5, 12, true, "Loading items");

    make_sprite("WSkirt", DEF_SPRID_LEGS_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WTrouser", DEF_SPRID_LEGS_W + 15 * 2, 12, true, "Loading items");
    make_sprite("WHTrouser", DEF_SPRID_LEGS_W + 15 * 3, 12, true, "Loading items");
    make_sprite("WCHoses", DEF_SPRID_LEGS_W + 15 * 4, 12, true, "Loading items");
    make_sprite("WLeggings", DEF_SPRID_LEGS_W + 15 * 5, 12, true, "Loading items");

    make_sprite("WHLeggings1", DEF_SPRID_LEGS_W + 15 * 6, 12, true, "Loading items");
    make_sprite("WHLeggings2", DEF_SPRID_LEGS_W + 15 * 7, 12, true, "Loading items");

    make_sprite("WShoes", DEF_SPRID_BOOT_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WLBoots", DEF_SPRID_BOOT_W + 15 * 2, 12, true, "Loading items");

    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 1, SPRITETYPE_SPRITE, "Wsw", i + 56 * 0, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 2, SPRITETYPE_SPRITE, "Wsw", i + 56 * 1, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 3, SPRITETYPE_SPRITE, "Wsw", i + 56 * 2, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 4, SPRITETYPE_SPRITE, "Wsw", i + 56 * 3, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 5, SPRITETYPE_SPRITE, "Wsw", i + 56 * 4, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 6, SPRITETYPE_SPRITE, "Wsw", i + 56 * 5, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 7, SPRITETYPE_SPRITE, "Wsw", i + 56 * 6, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 8, SPRITETYPE_SPRITE, "Wsw", i + 56 * 7, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 9, SPRITETYPE_SPRITE, "Wsw", i + 56 * 8, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 10, SPRITETYPE_SPRITE, "Wsw", i + 56 * 9, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 11, SPRITETYPE_SPRITE, "Wsw", i + 56 * 10, false, "Loading items" });
    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 12, SPRITETYPE_SPRITE, "Wsw", i + 56 * 11, false, "Loading items" });

    make_sprite("Wswx", DEF_SPRID_WEAPON_W + 64 * 5, 56, true, "Loading items");
    make_sprite("Wsw2", DEF_SPRID_WEAPON_W + 64 * 13, 56, true, "Loading items");
    make_sprite("Wsw3", DEF_SPRID_WEAPON_W + 64 * 14, 56, true, "Loading items");
    make_sprite("WStormBringer", DEF_SPRID_WEAPON_W + 64 * 15, 56, true, "Loading items");
    make_sprite("WDarkExec", DEF_SPRID_WEAPON_W + 64 * 16, 56, true, "Loading items");
    make_sprite("WKlonessBlade", DEF_SPRID_WEAPON_W + 64 * 17, 56, true, "Loading items");
    make_sprite("WKlonessAstock", DEF_SPRID_WEAPON_W + 64 * 18, 56, true, "Loading items");
    make_sprite("WDebastator", DEF_SPRID_WEAPON_W + 64 * 19, 56, true, "Loading items");

    make_sprite("Wmantle01", DEF_SPRID_CAPE_W + 15 * 1, 12, true, "Loading items");
    make_sprite("Wmantle02", DEF_SPRID_CAPE_W + 15 * 2, 12, true, "Loading items");
    make_sprite("Wmantle03", DEF_SPRID_CAPE_W + 15 * 3, 12, true, "Loading items");
    make_sprite("Wmantle04", DEF_SPRID_CAPE_W + 15 * 4, 12, true, "Loading items");
    make_sprite("Wmantle05", DEF_SPRID_CAPE_W + 15 * 5, 12, true, "Loading items");
    make_sprite("Wmantle06", DEF_SPRID_CAPE_W + 15 * 6, 12, true, "Loading items");

    make_sprite("WHelm1", DEF_SPRID_HEAD_W + 15 * 1, 12, true, "Loading items");
    make_sprite("WHelm4", DEF_SPRID_HEAD_W + 15 * 4, 12, true, "Loading items");
    make_sprite("NWHelm1", DEF_SPRID_HEAD_W + 15 * 5, 12, true, "Loading items");
    make_sprite("NWHelm2", DEF_SPRID_HEAD_W + 15 * 6, 12, true, "Loading items");
    make_sprite("NWHelm3", DEF_SPRID_HEAD_W + 15 * 7, 12, true, "Loading items");
    make_sprite("NWHelm4", DEF_SPRID_HEAD_W + 15 * 8, 12, true, "Loading items");
    make_sprite("WHHelm1", DEF_SPRID_HEAD_W + 15 * 9, 12, true, "Loading items");
    make_sprite("WHHelm2", DEF_SPRID_HEAD_W + 15 * 10, 12, true, "Loading items");
    make_sprite("WHCap1", DEF_SPRID_HEAD_W + 15 * 11, 12, true, "Loading items");
    make_sprite("WHCap2", DEF_SPRID_HEAD_W + 15 * 12, 12, true, "Loading items");

    make_sprite("WAxe1", DEF_SPRID_WEAPON_W + 64 * 20, 56, true, "Loading items"); // Axe
    make_sprite("WAxe2", DEF_SPRID_WEAPON_W + 64 * 21, 56, true, "Loading items");
    make_sprite("WAxe3", DEF_SPRID_WEAPON_W + 64 * 22, 56, true, "Loading items");
    make_sprite("WAxe4", DEF_SPRID_WEAPON_W + 64 * 23, 56, true, "Loading items");
    make_sprite("WAxe5", DEF_SPRID_WEAPON_W + 64 * 24, 56, true, "Loading items");
    make_sprite("WpickAxe1", DEF_SPRID_WEAPON_W + 64 * 25, 56, true, "Loading items");
    make_sprite("WAxe6", DEF_SPRID_WEAPON_W + 64 * 26, 56, true, "Loading items");
    make_sprite("Whoe", DEF_SPRID_WEAPON_W + 64 * 27, 56, true, "Loading items");
    make_sprite("WKlonessAxe", DEF_SPRID_WEAPON_W + 64 * 28, 56, true, "Loading items");
    make_sprite("WLightBlade", DEF_SPRID_WEAPON_W + 64 * 29, 56, true, "Loading items");
    make_sprite("WHammer", DEF_SPRID_WEAPON_W + 64 * 30, 56, true, "Loading items");
    make_sprite("WBHammer", DEF_SPRID_WEAPON_W + 64 * 31, 56, true, "Loading items");
    make_sprite("WBabHammer", DEF_SPRID_WEAPON_W + 64 * 32, 56, true, "Loading items");
    make_sprite("WBShadowSword", DEF_SPRID_WEAPON_W + 64 * 33, 56, true, "Loading items");
    make_sprite("WBerserkWand", DEF_SPRID_WEAPON_W + 64 * 34, 56, true, "Loading items");
    make_sprite("Wstaff1", DEF_SPRID_WEAPON_W + 64 * 35, 56, true, "Loading items");
    make_sprite("Wstaff2", DEF_SPRID_WEAPON_W + 64 * 36, 56, true, "Loading items");
    make_sprite("WStaff3", DEF_SPRID_WEAPON_W + 64 * 37, 56, true, "Loading items");

    make_sprite("WKlonessWand", DEF_SPRID_WEAPON_W + 64 * 39, 56, true, "Loading items");
    make_sprite("WReMagicWand", DEF_SPRID_WEAPON_W + 64 * 38, 56, true, "Loading items");
    //     make_sprite("Staff4W", DEF_SPRID_WEAPON_W + 64 * 40, 56, true, "Loading items");

    // bows 40 41 below
    make_sprite("WDirectBow", DEF_SPRID_WEAPON_W + 64 * 43, 56, true, "Loading items");
    make_sprite("WFireBow", DEF_SPRID_WEAPON_W + 64 * 44, 56, true, "Loading items");
    //make_sprite("Wbo", DEF_SPRID_WEAPON_W + 64 * 41, 56, true, "Loading items");        // Bow

    for (i = 0; i < 56; i++)
        data_list.push({ DEF_SPRID_WEAPON_W + i + 64 * 42, SPRITETYPE_SPRITE, "Wbo", i + 56 * 1, false, "Loading items" });

    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_W + i + 8 * 1, SPRITETYPE_SPRITE, "Wsh", i + 7 * 0, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_W + i + 8 * 2, SPRITETYPE_SPRITE, "Wsh", i + 7 * 1, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_W + i + 8 * 3, SPRITETYPE_SPRITE, "Wsh", i + 7 * 2, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_W + i + 8 * 4, SPRITETYPE_SPRITE, "Wsh", i + 7 * 3, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_W + i + 8 * 5, SPRITETYPE_SPRITE, "Wsh", i + 7 * 4, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_W + i + 8 * 6, SPRITETYPE_SPRITE, "Wsh", i + 7 * 5, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_W + i + 8 * 7, SPRITETYPE_SPRITE, "Wsh", i + 7 * 6, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_W + i + 8 * 8, SPRITETYPE_SPRITE, "Wsh", i + 7 * 7, false, "Loading items" });
    for (i = 0; i < 7; i++)
        data_list.push({ DEF_SPRID_SHIELD_W + i + 8 * 9, SPRITETYPE_SPRITE, "Wsh", i + 7 * 8, false, "Loading items" });

    make_effect_sprite("effect", 0, 10, false, "Loading effects");
    make_effect_sprite("effect2", 10, 3, false, "Loading effects");
    make_effect_sprite("effect3", 13, 6, false, "Loading effects");
    make_effect_sprite("effect4", 19, 5, false, "Loading effects");
    for (i = 0; i <= 6; i++)
        data_list.push({ i + 24, SPRITETYPE_EFFECT, "effect5", i + 1, false, "Loading effects" });
    make_effect_sprite("CruEffect1", 31, 9, false, "Loading effects");
    make_effect_sprite("effect6", 40, 5, false, "Loading effects");
    make_effect_sprite("effect7", 45, 12, false, "Loading effects");
    make_effect_sprite("effect8", 57, 9, false, "Loading effects");
    make_effect_sprite("effect9", 66, 21, false, "Loading effects");

    make_effect_sprite("effect10", 87, 2, false, "Loading effects");  // Effects Hero items
    make_effect_sprite("effect11", 89, 14, false, "Loading effects"); // Cancel, stormBlade, resu, GateHeldenian....etc
    make_effect_sprite("effect11s", 104, 1, false, "Loading effects");
    make_effect_sprite("yseffect2", 140, 8, false, "Loading effects"); // Abaddon's death
    make_effect_sprite("effect12", 148, 4, false, "Loading effects");  // Slates auras
    make_effect_sprite("yseffect3", 152, 16, false, "Loading effects");
    make_effect_sprite("yseffect4", 133, 7, false, "Loading effects"); // Abaddon's map thunder.
    //make_effect_sprite("effects", 168, 1, false, "Loading effects");   // minimap ping

        //progressLabel = "Loading sounds";
    std::string path = {};
    for (i = 1; i <= 8; i++)
    {
        path = fmt::format("data\\sounds\\C{}.wav", i);
        CSoundBuffer[i].loadFromFile(path);
        m_pCSound[i].setBuffer(CSoundBuffer[i]);
    }
    for (i = 1; i <= 50; i++)
    {
        path = fmt::format("data\\sounds\\M{}.wav", i);
        MSoundBuffer[i].loadFromFile(path);
        m_pMSound[i].setBuffer(MSoundBuffer[i]);
    }
    for (i = 1; i <= 15; i++)
    {
        path = fmt::format("data\\sounds\\E{}.wav", i);
        ESoundBuffer[i].loadFromFile(path);
        m_pESound[i].setBuffer(ESoundBuffer[i]);
    }
    for (i = 9; i <= 16; i++)
    {
        path = fmt::format("data\\sounds\\C{}.wav", i);
        CSoundBuffer[i].loadFromFile(path);
        m_pCSound[i].setBuffer(CSoundBuffer[i]);
    }
    for (i = 51; i <= 100; i++)
    {
        path = fmt::format("data\\sounds\\M{}.wav", i);
        MSoundBuffer[i].loadFromFile(path);
        m_pMSound[i].setBuffer(MSoundBuffer[i]);
    }
    for (i = 16; i <= 30; i++)
    {
        path = fmt::format("data\\sounds\\E{}.wav", i);
        ESoundBuffer[i].loadFromFile(path);
        m_pESound[i].setBuffer(ESoundBuffer[i]);
    }
    for (i = 17; i <= 24; i++)
    {
        path = fmt::format("data\\sounds\\C{}.wav", i);
        CSoundBuffer[i].loadFromFile(path);
        m_pCSound[i].setBuffer(CSoundBuffer[i]);
    }
    for (i = 101; i <= 156; i++)
    {
        path = fmt::format("data\\sounds\\M{}.wav", i);
        MSoundBuffer[i].loadFromFile(path);
        m_pMSound[i].setBuffer(MSoundBuffer[i]);
    }
    for (i = 31; i <= 54; i++)
    {
        path = fmt::format("data\\sounds\\E{}.wav", i);
        ESoundBuffer[i].loadFromFile(path);
        m_pESound[i].setBuffer(ESoundBuffer[i]);
    }

    data_max = data_list.size();
}

void CGame::UpdateScreen_OnLoading(bool bActive)
{
    draw_version = true;
    if (data_list.empty())
    {
        ChangeGameMode(DEF_GAMEMODE_ONCONNECTING);
        return;
    }

    UpdateScreen_OnLoading_Progress();


    std::string progress_label;

    int perform = 300; // (game->fps.getFPS() + 20) / 20;

    while (--perform)
    {
        if (!data_list.empty())
        {
            load_data item = data_list.front();
            data_list.pop();
            progress_label = item.label;
            if (item.sprite_type == SPRITETYPE_SPRITE)
            {
                m_pSprite[item.id] = sprite::create_sprite(item.name, item.num, item.alpha);
            }
            else if (item.sprite_type == SPRITETYPE_TILE)
            {
                m_pTileSpr[item.id] = sprite::create_sprite(item.name, item.num, item.alpha);
            }
            else if (item.sprite_type == SPRITETYPE_EFFECT)
            {
                m_pEffectSpr[item.id] = sprite::create_sprite(item.name, item.num, item.alpha);
            }
        }
    }

    double percent = ((double(data_max) - data_list.size()) / double(data_max)) * 118;

    m_cLoading = (int8_t)percent;

    return;
}

void CGame::UpdateScreen_OnLoading_Progress()
{
    m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING]->open_sprite_();
    int16_t bgx = screenwidth_v / 2 - m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING]->brush[0].szx / 2;
    int16_t bgy = screenheight_v / 2 - m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING]->brush[0].szy / 2;
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_LOADING, bgx, bgy, 0, true);
    int16_t fgx = screenwidth_v / 2 - m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING]->brush[1].szx / 2;
    int16_t fgy = screenheight_v / 2 - m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING]->brush[1].szy / 2;
    m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING]->put_sprite_fast_width(fgx + 426 / 2, fgy + 424 / 2, 1, m_cLoading, G_dwGlobalTime);
    DrawVersion();
}

void CGame::UpdateScreen_OnMsg()
{
    uint16_t msX{}, msY{}, msZ{};
    bool LB{}, RB{}, MB{};
    uint32_t dwTime = G_dwGlobalTime;

    update_mouse_state(msX, msY, msZ, LB, RB, MB);

    put_string(10, 10, m_cMsg, Color(255, 155, 155), false, 1);

    DrawVersion();

    if ((G_dwGlobalTime - m_dwTime) > 1500)
    {
        ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
    }
}

void CGame::UpdateScreen_OnConnecting()
{
    short sX, sY;
    uint32_t dwTime = unixtime();
    static CMouseInterface * pMI;
    static uint32_t dwMTime, dwCTime;

    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
        dwCTime = dwMTime = unixtime();

        if (is_closed())
        {
            perform_connect();
        }
        else if (is_connected())
        {
            ConnectionEstablishHandler(DEF_SERVERTYPE_LOG);
            return;
        }
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {

        if ((dwTime - m_dwTime) > 1000)
        {
            ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
            if (!is_closed())
                close(ix::WebSocketCloseConstants::kNormalClosureCode, ix::WebSocketCloseConstants::kNormalClosureMessage);
        }

        m_bEscPressed = false;
        return;
    }

    if ((dwTime - dwMTime) > 150) dwMTime = dwTime;

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    m_bIsHideLocalCursor = true;
    switch (m_cMsg[0])
    {
        case '0':
            _Draw_UpdateScreen_OnCreateNewAccount();
            break;

        case '1':
            sX = 146;
            sY = 114;
            _Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
            break;

        case '2':
            _bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
            break;

        case '3':
            UpdateScreen_OnSelectCharacter(0, 0, 0, 0);
            break;

        case '4':
            // Change Password
            UpdateScreen_OnSelectCharacter(0, 0, 0, 0, true);
            break;
    }
    m_bIsHideLocalCursor = false;

    draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, 162, 125, 2);
    format_to_local(G_cTxt, "Connecting to Server... %3dSec", (dwTime - m_dwTime) / 1000);
    put_string_sprite_font(172 + 35, 190, G_cTxt, 7, 0, 0);

    if ((dwTime - m_dwTime) > 7000)
    {
        put_aligned_string(180, 463, 195 + 30, UPDATE_SCREEN_ON_CONNECTING1);
        put_aligned_string(180, 463, 195 + 45, UPDATE_SCREEN_ON_CONNECTING2);
    }
    else put_aligned_string(180, 463, 195 + 30, UPDATE_SCREEN_ON_CONNECTING3);

    DrawVersion();
    m_stMCursor.sCursorFrame = 8;
}

void CGame::UpdateScreen_OnWaitInitData()
{
    uint16_t msX{}, msY{}, msZ{};
    uint32_t dwTime = unixtime();

    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        if ((dwTime - m_dwTime) > 7000)
        {
            ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
            close(ix::WebSocketCloseConstants::kNormalClosureCode, ix::WebSocketCloseConstants::kNormalClosureMessage);
        }
        m_bEscPressed = false;
        return;
    }

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, 162, 125, 2);

    format_to_local(G_cTxt, "Waiting for response... {}sec", (dwTime - m_dwTime) / 1000);
    put_string_sprite_font(172 + 44, 190, G_cTxt, 7, 0, 0);
    if ((dwTime - m_dwTime) > 7000)
    {
        put_aligned_string(174, 467, 190 + 30, UPDATE_SCREEN_ON_WAIT_INIT_DATA1);
        put_aligned_string(174, 467, 190 + 45, UPDATE_SCREEN_ON_WAIT_INIT_DATA2);
    }
    else put_aligned_string(174, 467, 195 + 30, UPDATE_SCREEN_ON_WAIT_INIT_DATA3);

    DrawVersion();

    m_stMCursor.sCursorFrame = 8;
}

void CGame::UpdateScreen_OnConnectionLost()
{
    static uint32_t dwTime;

    if (m_cGameModeCount == 0)
    {
        dwTime = unixtime();
        if (m_bSoundFlag) m_pESound[38].stop();
        if ((m_bSoundFlag) && (m_bMusicStat == true))
        {
            m_pBGM.stop();
        }
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, 162, 125, 2);
    put_string_sprite_font(172 + 54, 180, "Connection Lost!", 7, 0, 0);
    put_string(172 + 50, 180 + 30, UPDATE_SCREEN_ON_CONNECTION_LOST, Color(0, 0, 0));

    DrawVersion();
    m_stMCursor.sCursorFrame = 0;

    if ((unixtime() - m_dwTime) > 5000)
    {
        ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
    }
}

void CGame::UpdateScreen_OnCreateNewCharacter()
{
    static CMouseInterface * pMI;
    //short sX, sY;
    int i = 0;

    int iMIbuttonNum;
    static int iPoint;
    uint16_t msX{}, msY{}, msZ{};
    bool LB{}, RB{}, MB{};
    char cMIresult;
    static char cName[12];
    static char cPrevFocus;
    bool bFlag;
    static uint32_t dwMTime;
    uint32_t dwTime = unixtime();

    update_mouse_state(msX, msY, msZ, LB, RB, MB);

    if (m_cGameModeCount == 0)
    {
        pMI = new CMouseInterface;
        pMI->AddRect(65 + 4, 65 + 45, 275 + 4, 82 + 45);

        pMI->AddRect(232 + 4, 111 + 45, 274 + 4 - 21, 124 + 45);
        pMI->AddRect(255 + 4, 111 + 45, 289 + 4 - 13, 124 + 45);

        pMI->AddRect(232 + 4, 126 + 45, 274 + 4 - 21, 139 + 45);
        pMI->AddRect(255 + 4, 126 + 45, 289 + 4 - 13, 139 + 45);

        pMI->AddRect(232 + 4, 141 + 45, 274 + 4 - 21, 154 + 45);
        pMI->AddRect(255 + 4, 141 + 45, 289 + 4 - 13, 154 + 45);

        pMI->AddRect(232 + 4, 156 + 45, 274 + 4 - 21, 169 + 45);
        pMI->AddRect(255 + 4, 156 + 45, 289 + 4 - 13, 169 + 45);

        pMI->AddRect(232 + 4, 171 + 45, 274 + 4 - 21, 184 + 45);
        pMI->AddRect(255 + 4, 171 + 45, 289 + 4 - 13, 184 + 45);


        pMI->AddRect(232 + 4, 231 + 45, 253 + 4, 244 + 45);
        pMI->AddRect(255 + 4, 231 + 45, 276 + 4, 244 + 45);

        pMI->AddRect(232 + 4, 246 + 45, 253 + 4, 259 + 45);
        pMI->AddRect(255 + 4, 246 + 45, 276 + 4, 259 + 45);

        pMI->AddRect(232 + 4, 261 + 45, 253 + 4, 274 + 45);
        pMI->AddRect(255 + 4, 261 + 45, 276 + 4, 274 + 45);

        pMI->AddRect(232 + 4, 276 + 45, 253 + 4, 289 + 45);
        pMI->AddRect(255 + 4, 276 + 45, 276 + 4, 289 + 45);

        pMI->AddRect(232 + 4, 291 + 45, 253 + 4, 304 + 45);
        pMI->AddRect(255 + 4, 291 + 45, 276 + 4, 304 + 45);

        pMI->AddRect(232 + 4, 306 + 45, 253 + 4, 319 + 45);
        pMI->AddRect(255 + 4, 306 + 45, 276 + 4, 319 + 45);

        pMI->AddRect(384, 445, 384 + 72, 445 + 15);
        pMI->AddRect(500, 445, 500 + 72, 445 + 15);

        pMI->AddRect(60, 445, 60 + 72, 445 + 15);
        pMI->AddRect(145, 445, 145 + 72, 445 + 15);
        pMI->AddRect(230, 445, 230 + 72, 445 + 15);

        iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
        iPoint = 70 - iPoint;

        cPrevFocus = 1;
        m_cCurFocus = 1;
        m_cMaxFocus = 6;
        m_bEnterPressed = false;
        m_cArrowPressed = 0;

        dwMTime = unixtime();

        StartInputString(193 + 4, 65 + 45, 11, cName);
        ClearInputString();
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 1:
                m_cCurFocus--;
                if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
                break;

            case 3:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (cPrevFocus != m_cCurFocus)
    {
        EndInputString();
        switch (m_cCurFocus)
        {
            case 1:
                StartInputString(193 + 4, 65 + 45, 11, cName);
                break;
        }
        cPrevFocus = m_cCurFocus;
    }

    if (m_bEscPressed == true)
    {
        ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
        delete pMI;
        m_bEscPressed = false;
        return;
    }

    bFlag = _bDraw_OnCreateNewCharacter(cName, msX, msY, iPoint);

    if ((dwTime - dwMTime) > 100)
    {
        m_cMenuFrame++;
        dwMTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    DrawVersion();
    m_stMCursor.sCursorFrame = 0;
    iMIbuttonNum = pMI->iGetStatus(msX, msY, LB, &cMIresult);
    if (cMIresult == DEF_MIRESULT_CLICK)
    {

        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            case 1:
                m_cCurFocus = 1;
                break;
            case 2:
                m_cGender--;
                if (m_cGender < 1) m_cGender = 2;
                break;
            case 3:
                m_cGender++;
                if (m_cGender > 2) m_cGender = 1;
                break;
            case 4:
                m_cSkinCol--;
                if (m_cSkinCol < 1) m_cSkinCol = 3;
                break;
            case 5:
                m_cSkinCol++;
                if (m_cSkinCol > 3) m_cSkinCol = 1;
                break;
            case 6:
                m_cHairStyle--;
                if (m_cHairStyle < 0) m_cHairStyle = 7;
                break;
            case 7:
                m_cHairStyle++;
                if (m_cHairStyle > 7) m_cHairStyle = 0;
                break;
            case 8:
                m_cHairCol--;
                if (m_cHairCol < 0) m_cHairCol = 15;
                break;
            case 9:
                m_cHairCol++;
                if (m_cHairCol > 15) m_cHairCol = 0;
                break;
            case 10:
                m_cUnderCol--;
                if (m_cUnderCol < 0) m_cUnderCol = 7;
                break;
            case 11:
                m_cUnderCol++;
                if (m_cUnderCol > 7) m_cUnderCol = 0;
                break;
            case 12:
                if (iPoint > 0)
                {
                    if (m_ccStr < 14)
                    {
                        m_ccStr++;
                        iPoint--;
                    }
                }
                break;
            case 13:
                if (m_ccStr > 10)
                {
                    m_ccStr--;
                    iPoint++;
                }
                break;
            case 14:
                if (iPoint > 0)
                {
                    if (m_ccVit < 14)
                    {
                        m_ccVit++;
                        iPoint--;
                    }
                }
                break;
            case 15:
                if (m_ccVit > 10)
                {
                    m_ccVit--;
                    iPoint++;
                }
                break;
            case 16:
                if (iPoint > 0)
                {
                    if (m_ccDex < 14)
                    {
                        m_ccDex++;
                        iPoint--;
                    }
                }
                break;
            case 17:
                if (m_ccDex > 10)
                {
                    m_ccDex--;
                    iPoint++;
                }
                break;
            case 18:
                if (iPoint > 0)
                {
                    if (m_ccInt < 14)
                    {
                        m_ccInt++;
                        iPoint--;
                    }
                }
                break;
            case 19:
                if (m_ccInt > 10)
                {
                    m_ccInt--;
                    iPoint++;
                }
                break;
            case 20:
                if (iPoint > 0)
                {
                    if (m_ccMag < 14)
                    {
                        m_ccMag++;
                        iPoint--;
                    }
                }
                break;
            case 21:
                if (m_ccMag > 10)
                {
                    m_ccMag--;
                    iPoint++;
                }
                break;
            case 22:
                if (iPoint > 0)
                {
                    if (m_ccChr < 14)
                    {
                        m_ccChr++;
                        iPoint--;
                    }
                }
                break;
            case 23:
                if (m_ccChr > 10)
                {
                    m_ccChr--;
                    iPoint++;
                }
                break;

            case 24:
                if (m_cCurFocus != 2)
                {
                    m_cCurFocus = 2;
                    return;
                }
                if (bFlag == false) return;
                //if (m_Misc.bCheckValidName(m_cPlayerName) == false) break;
                if (m_Misc.bCheckValidName(cName) == false) break;

                memset(m_cPlayerName, 0, sizeof(m_cPlayerName));
                strcpy(m_cPlayerName, cName);

                // todo: we don't connect anymore
                ChangeGameMode(DEF_GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_CREATENEWCHARACTER;
                memset(m_cMsg, 0, sizeof(m_cMsg));
                strcpy(m_cMsg, "22");
                delete pMI;
                return;

            case 25:
                if (m_cCurFocus != 3)
                {
                    m_cCurFocus = 3;
                    return;
                }
                ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
                delete pMI;
                return;

            case 26:
                if (m_cCurFocus != 4)
                {
                    m_cCurFocus = 4;
                    return;
                }

                m_ccMag = 10;
                m_ccInt = 10;
                m_ccChr = 10;
                m_ccStr = 14;
                m_ccVit = 12;
                m_ccDex = 14;
                iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
                iPoint = 70 - iPoint;
                break;

            case 27:
                if (m_cCurFocus != 5)
                {
                    m_cCurFocus = 5;
                    return;
                }

                m_ccMag = 14;
                m_ccInt = 14;
                m_ccChr = 10;
                m_ccStr = 10;
                m_ccVit = 12;
                m_ccDex = 10;
                iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
                iPoint = 70 - iPoint;
                break;

            case 28:
                if (m_cCurFocus != 6)
                {
                    m_cCurFocus = 6;
                    return;
                }

                m_ccMag = 10;
                m_ccInt = 10;
                m_ccChr = 14;
                m_ccStr = 14;
                m_ccVit = 10;
                m_ccDex = 12;
                iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
                iPoint = 70 - iPoint;
                break;
        }
    }

    if ((msX >= 65 + 4 - 127) && (msX <= 275 + 4) && (msY >= 65 + 45) && (msY <= 82 + 45))
    {
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER1);
    }
    else if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 111 + 45) && (msY <= 124 + 45))
    {
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER2);
    }
    else if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 126 + 45) && (msY <= 139 + 45))
    {
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER3);
    }
    else if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 141 + 45) && (msY <= 154 + 45))
    {
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER4);
    }
    else if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 156 + 45) && (msY <= 169 + 45))
    {
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER5);
    }
    else if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 171 + 45) && (msY <= 184 + 45))
    {
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER6);
    }
    else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 231 + 45) && (msY <= 244 + 45))
    {
        // Str
        i = 0;
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER7);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER8);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER9);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER10);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER11);
    }
    else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 246 + 45) && (msY <= 259 + 45))
    {
        // Vit
        i = 0;
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER12);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER13);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER14);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER15);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER16);
    }
    else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 261 + 45) && (msY <= 274 + 45))
    {
        // Dex
        i = 0;
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER17);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER18);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER19);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER20);
    }
    else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 276 + 45) && (msY <= 289 + 45))
    {
        // Int
        i = 0;
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER21);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER22);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER23);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER24);
    }
    else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 291 + 45) && (msY <= 304 + 45))
    {
        // Mag
        i = 0;
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER25);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER26);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER27);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER28);
    }
    else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 306 + 45) && (msY <= 319 + 45))
    {
        // Charisma
        i = 0;
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER29);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER30);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER31);
        put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER32);
    }
    else if ((msX >= 384) && (msX <= 384 + 72) && (msY >= 445) && (msY <= 445 + 15))
    {
        m_cCurFocus = 2;
        if (strlen(cName) <= 0)
        {
            i = 0;
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER33);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER34);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER35);
        }
        else if (iPoint > 0)
        {
            i = 0;
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER36);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER37);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER38);
        }
        else if (m_Misc.bCheckValidName(cName) == false)
        {
            i = 0;
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER39);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER40);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER41);
        }
//         else if (_bCheckBadWords(cName) == true)
//         {
//             i = 0;
//             put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER42);
//             put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER43);
//         }
        else
        {
            i = 0;
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER44);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER45);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER46);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER47);
            put_aligned_string(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER48);
        }
    }
    else if ((msX >= 500) && (msX <= 500 + 72) && (msY >= 445) && (msY <= 445 + 15))
    {
        m_cCurFocus = 3;
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER49);
    }

    if ((msX >= 60) && (msX <= 60 + 72) && (msY >= 445) && (msY <= 445 + 15))
    {
        m_cCurFocus = 4;
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER50);
    }

    if ((msX >= 145) && (msX <= 145 + 72) && (msY >= 445) && (msY <= 445 + 15))
    {
        m_cCurFocus = 5;
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER51);
    }

    if ((msX >= 230) && (msX <= 230 + 72) && (msY >= 445) && (msY <= 445 + 15))
    {
        m_cCurFocus = 6;
        put_aligned_string(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER52);
    }

    //	if (m_cGameModeCount < 6) draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
    //	if (m_cGameModeCount < 2) draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
}

void CGame::UpdateScreen_OnLogin()
{
    uint16_t msX{}, msY{}, msZ{}, sX{}, sY{};
    bool LB{}, RB{}, MB{};
    char cMIresult;
    int  iMIbuttonNum;
    static CMouseInterface * pMI;
    static char cPassword[12], cPrevFocus;
    static char cName[12];

    sX = 146;
    sY = 114;
    if (m_cGameModeCount == 0)
    {
        EndInputString();

        pMI = new CMouseInterface;
        pMI->AddRect(222, 271, 360, 288);
        pMI->AddRect(222, 328, 360, 345);
        pMI->AddRect(209, 352, 271, 371);
        pMI->AddRect(321, 352, 369, 370);
        cPrevFocus = 1;
        m_cCurFocus = 1;
        m_cMaxFocus = 4;
        m_bEnterPressed = false;
        m_cArrowPressed = 0;

        memset(cName, 0, sizeof(cName));
        memset(cPassword, 0, sizeof(cPassword));
        if (strlen(m_cAutoName) > 0)
            memcpy(cName, m_cAutoName, 11);
        if (strlen(m_cAutoPass) > 0)
            memcpy(cPassword, m_cAutoPass, 11);
        ClearInputString();
        StartInputString(258, 273, 11, cName);
        m_bIsCheckingGateway = false;
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 1:
                m_cCurFocus--;
                if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
                break;

            case 2:
                if (m_cCurFocus == 3) m_cCurFocus = 4;
                else if (m_cCurFocus == 4) m_cCurFocus = 3;
                break;

            case 3:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;

            case 4:
                if (m_cCurFocus == 3) m_cCurFocus = 4;
                else if (m_cCurFocus == 4) m_cCurFocus = 3;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (m_bEnterPressed == true)
    {
        m_bEnterPressed = false;
        PlaySound('E', 14, 5);

        switch (m_cCurFocus)
        {
            case 1:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;
            case 2:
            case 3:
                if ((strlen(cName) == 0) || (strlen(cPassword) == 0)) break;

                if (m_bIsCheckingGateway == false)
                {
                    memset(m_cAccountName, 0, sizeof(m_cAccountName));
                    memset(m_cAccountPassword, 0, sizeof(m_cAccountPassword));
                    strcpy(m_cAccountName, cName);
                    strcpy(m_cAccountPassword, cPassword);

                    ChangeGameMode(DEF_GAMEMODE_ONCONNECTING);
                    m_dwConnectMode = MSGID_REQUEST_LOGIN;
                    memset(m_cMsg, 0, sizeof(m_cMsg));
                    strcpy(m_cMsg, "11");
                    ConnectionEstablishHandler(DEF_SERVERTYPE_LOG);
                    delete pMI;
                }
                return;

            case 4:
                ChangeGameMode(DEF_GAMEMODE_ONSELECTSERVER); // ONMAINMENU
                delete pMI;
                return;
        }
    }

    if (m_bEscPressed == true)
    {
        EndInputString();
        ChangeGameMode(DEF_GAMEMODE_ONSELECTSERVER);
        delete pMI;
        m_bEscPressed = false;
        return;
    }

    if (cPrevFocus != m_cCurFocus)
    {
        EndInputString();
        switch (m_cCurFocus)
        {
            case 1:
                StartInputString(258, 273, 11, cName);
                break;
            case 2:
                StartInputString(258, 330, 11, cPassword, true);
                break;
            case 3:
            case 4:
                break;
        }
        cPrevFocus = m_cCurFocus;
    }
    update_mouse_state(msX, msY, msZ, LB, RB, MB);
    iMIbuttonNum = pMI->iGetStatus(msX, msY, LB, &cMIresult);
    if (cMIresult == DEF_MIRESULT_CLICK)
    {

        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            case 1:
                m_cCurFocus = 1;
                break;

            case 2:
                m_cCurFocus = 2;
                break;

            case 3:
                if ((strlen(cName) == 0) || (strlen(cPassword) == 0)) break;
                if (m_bIsCheckingGateway == false)
                {
                    EndInputString();
                    memset(m_cAccountName, 0, sizeof(m_cAccountName));
                    memset(m_cAccountPassword, 0, sizeof(m_cAccountPassword));
                    strcpy(m_cAccountName, cName);
                    strcpy(m_cAccountPassword, cPassword);
                    ChangeGameMode(DEF_GAMEMODE_ONCONNECTING);
                    m_dwConnectMode = MSGID_REQUEST_LOGIN;
                    memset(m_cMsg, 0, sizeof(m_cMsg));
                    strcpy(m_cMsg, "11");
                    ConnectionEstablishHandler(DEF_SERVERTYPE_LOG);
                    delete pMI;

                }
                return;

            case 4:
                ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                delete pMI;
                return;
        }
    }

    if ((msX >= 209) && (msX <= 271) && (msY >= 352) && (msY <= 371)) m_cCurFocus = 3;
    if ((msX >= 321) && (msX <= 369) && (msY >= 352) && (msY <= 370)) m_cCurFocus = 4;

    _Draw_OnLogin(cName, cPassword, msX, msY, m_cGameModeCount);
    if (m_bIsCheckingGateway == true)
    {
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, 162, 125, 2);
        put_aligned_string(175, 458, 160 + 30, UPDATE_SCREEN_ON_LOGIN1);
        put_aligned_string(175, 458, 160 + 45, UPDATE_SCREEN_ON_LOGIN2);
    }
}

void CGame::UpdateScreen_OnQuit()
{
    uint16_t msX{}, msY{}, msZ{};
    bool LB{}, RB{}, MB{};
    char cMIresult;
    int iMIbuttonNum;

    static CMouseInterface * pMI;

    update_mouse_state(msX, msY, msZ, LB, RB, MB);

    uint32_t dwTime = unixtime();

    if (m_cGameModeCount == 0)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        pMI = new CMouseInterface;
        pMI->AddRect(0, 0, get_virtual_width(), get_virtual_height());
        m_bEnterPressed = false;
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 120) m_cGameModeCount = 120;

    if (m_bEscPressed == true || m_bEnterPressed == true)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        delete pMI;
        ChangeGameMode(DEF_GAMEMODE_NULL);
        // todo: windows
        SendMessage(m_hWnd, WM_DESTROY, 0, 0);
        return;
    }

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_QUIT, 0, 0, 0, true);
    if (m_cGameModeCount > 20) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_QUIT, 255, 123, 1, true);
    else if ((m_cGameModeCount >= 15) && (m_cGameModeCount <= 20)) m_pSprite[DEF_SPRID_INTERFACE_ND_QUIT]->put_trans_sprite25(255, 123, 1, true);

    DrawVersion();

    if (m_cGameModeCount == 100)
    {
        ChangeGameMode(DEF_GAMEMODE_NULL);
        delete pMI;
        SendMessage(m_hWnd, WM_DESTROY, 0, 0);
        return;
    }
    m_stMCursor.sCursorFrame = 0;

    iMIbuttonNum = pMI->iGetStatus(msX, msY, LB, &cMIresult);
    if ((cMIresult == DEF_MIRESULT_CLICK) && (iMIbuttonNum == 1))
    {
        ChangeGameMode(DEF_GAMEMODE_NULL);
        // todo: windows
        SendMessage(m_hWnd, WM_DESTROY, 0, 0);
        delete pMI;
        return;
    }

    //	if (m_cGameModeCount < 6) draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
    //	if (m_cGameModeCount < 2) draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
}

void CGame::UpdateScreen_OnQueryForceLogin()
{
    uint16_t msX{}, msY{}, msZ{};
    bool LB{}, RB{}, MB{};
    char cMIresult;
    int iMIbuttonNum;

    static CMouseInterface * pMI;
    static uint32_t dwCTime;
    uint32_t dwTime = unixtime();

    update_mouse_state(msX, msY, msZ, LB, RB, MB);

    if (m_cGameModeCount == 0)
    {
        pMI = new CMouseInterface;
        pMI->AddRect(200, 244, 200 + DEF_BTNSZX, 244 + DEF_BTNSZY);
        pMI->AddRect(370, 244, 370 + DEF_BTNSZX, 244 + DEF_BTNSZY);
        m_bEnterPressed = false;
        m_bEscPressed = false;
        m_cArrowPressed = 0;

        dwCTime = unixtime();

        PlaySound('E', 25, 0);
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
        delete pMI;
        m_bEscPressed = false;
        return;
    }

    UpdateScreen_OnSelectCharacter(modx, mody, 0, 0);
    if ((m_cGameModeCount >= 0) && (m_cGameModeCount < 6))
    {
        draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
    }
    else if (m_cGameModeCount >= 6)
    {
        draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
        draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
    }

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, 162, 130, 2);

    put_string_sprite_font(172 + 86, 160, "Character in Use", 7, 0, 0);
    put_aligned_string(178, 453, 195, UPDATE_SCREEN_ON_QUERY_FORCE_LOGIN1);
    put_aligned_string(178, 453, 215, UPDATE_SCREEN_ON_QUERY_FORCE_LOGIN2);

    if ((msX >= 200) && (msX <= 200 + DEF_BTNSZX) && (msY >= 244) && (msY <= 244 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, 200, 244, 19);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, 200, 244, 18);

    if ((msX >= 370) && (msX <= 370 + DEF_BTNSZX) && (msY >= 244) && (msY <= 244 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, 370, 244, 3);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, 370, 244, 2);

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    iMIbuttonNum = pMI->iGetStatus(msX, msY, LB, &cMIresult);
    if (cMIresult == DEF_MIRESULT_CLICK)
    {
        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            case 1:
                ChangeGameMode(DEF_GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                m_wEnterGameType = DEF_ENTERGAMEMSGTYPE_NOENTER_FORCEDISCONN;
                memset(m_cMsg, 0, sizeof(m_cMsg));
                strcpy(m_cMsg, "33");
                ConnectionEstablishHandler(DEF_SERVERTYPE_LOG);
                delete pMI;
                return;

            case 2:
                ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
                delete pMI;
                break;
        }
    }

    DrawVersion();
    m_stMCursor.sCursorFrame = 0;
}

void CGame::UpdateScreen_OnSelectCharacter(short sX, short sY, short msX, short msY, bool bIgnoreFocus)
{
    int i;
    int iYear, iMonth, iDay, iHour, iMinute;
    __int64 iTemp1, iTemp2;
    char cTotalChar = 0;
    uint32_t dwTime = unixtime();

    //sY = 10;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_SELECTCHAR, sX, sY, 0);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX, sY, 50);

    iTemp1 = 0;
    iTemp2 = 0;
    iYear = iMonth = iDay = iHour = iMinute = 0;

    for (i = 0; i < 4; i++)
    {

        if ((m_cCurFocus - 1 == i) && (bIgnoreFocus == false))
            m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(sX + 110 + i * 109 - 7, sY + 63 - 9, 62, dwTime);
        else m_pSprite[DEF_SPRID_INTERFACE_ND_BUTTON]->put_sprite_fast(sX + 110 + i * 109 - 7, sY + 63 - 9, 61, dwTime);

        if (m_pCharList[i] != 0)
        {

            cTotalChar++;

            switch (m_pCharList[i]->m_sSex)
            {
                case 1:	_tmp_sOwnerType = 1; break;
                case 2:	_tmp_sOwnerType = 4; break;
            }
            _tmp_sOwnerType += m_pCharList[i]->m_sSkinCol - 1;
            _tmp_cDir = m_cMenuDir;
            _tmp_sAppr1 = m_pCharList[i]->m_sAppr1;
            _tmp_sAppr2 = m_pCharList[i]->m_sAppr2;
            _tmp_sAppr3 = m_pCharList[i]->m_sAppr3;
            _tmp_sAppr4 = m_pCharList[i]->m_sAppr4;
            _tmp_iApprColor = m_pCharList[i]->m_iApprColor;

            memset(_tmp_cName, 0, sizeof(_tmp_cName));
            memcpy(_tmp_cName, m_pCharList[i]->m_cName, 10);
            _tmp_cAction = DEF_OBJECTMOVE;
            _tmp_cFrame = m_cMenuFrame;

            if (m_pCharList[i]->m_sSex != 0)
            {

                if (m_Misc.bCheckValidString(m_pCharList[i]->m_cName) == true)
                {

                    m_pEffectSpr[0]->put_trans_sprite(sX + 157 + i * 109, sY + 138, 1, dwTime);
                    DrawObject_OnMove_ForMenu(0, 0, sX + 157 + i * 109, sY + 138, false, dwTime, 0, 0);

                    put_string(sX + 112 + i * 109, sY + 179 - 9, m_pCharList[i]->m_cName, Color::White, false, 1);//25,35,25);
                    int	_sLevel = m_pCharList[i]->m_sLevel;
                    format_to_local(G_cTxt, "{}", _sLevel);
                    put_string(sX + 138 + i * 109, sY + 196 - 10, G_cTxt, Color::White, false, 1); //25,35,25);

                    format_to_local(G_cTxt, "{}", m_pCharList[i]->m_iExp);
                    put_string(sX + 138 + i * 109, sY + 211 - 10, G_cTxt, Color::White, false, 1); //25,35,25);
                }
                else
                {

                }
                iTemp2 = m_pCharList[i]->m_iYear * 1000000 + m_pCharList[i]->m_iMonth * 60000 + m_pCharList[i]->m_iDay * 1700 + m_pCharList[i]->m_iHour * 70 + m_pCharList[i]->m_iMinute;
                if (iTemp1 < iTemp2)
                {
                    iYear = m_pCharList[i]->m_iYear;
                    iMonth = m_pCharList[i]->m_iMonth;
                    iDay = m_pCharList[i]->m_iDay;
                    iHour = m_pCharList[i]->m_iHour;
                    iMinute = m_pCharList[i]->m_iMinute;
                    iTemp1 = iTemp2;
                }
            }
            else
            {
            }
        }
        else
        {
        }
    }

    i = 0;

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 51);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 52);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 53);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 54);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 55);
    if ((msX > sX + 360) && (msY >= sY + 283) && (msX < sX + 545) && (msY <= sY + 315))
    {
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 56);
        put_aligned_string(sX + 98, sX + 357, sY + 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER1);
        put_aligned_string(sX + 98, sX + 357, sY + 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER2);
        put_aligned_string(sX + 98, sX + 357, sY + 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER3);
        put_aligned_string(sX + 98, sX + 357, sY + 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER4);
    }
    else if ((msX > sX + 360) && (msY >= sY + 316) && (msX < sX + 545) && (msY <= sY + 345))
    {
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 57);
        put_aligned_string(sX + 98, sX + 357, sY + 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER5);
    }
    else if ((msX > sX + 360) && (msY >= sY + 346) && (msX < sX + 545) && (msY <= sY + 375))
    {
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 58);
        put_aligned_string(sX + 98, sX + 357, sY + 275 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER6);
        put_aligned_string(sX + 98, sX + 357, sY + 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER7);
        put_aligned_string(sX + 98, sX + 357, sY + 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER8);
        put_aligned_string(sX + 98, sX + 357, sY + 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER9);
        put_aligned_string(sX + 98, sX + 357, sY + 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER10);
        put_aligned_string(sX + 98, sX + 357, sY + 350 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER11);

    }
    else if ((msX > sX + 360) && (msY >= sY + 376) && (msX < sX + 545) && (msY <= sY + 405))
    {
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 59);
        put_aligned_string(sX + 98, sX + 357, sY + 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER12);
    }
    else if ((msX > sX + 360) && (msY >= sY + 406) && (msX < sX + 545) && (msY <= sY + 435))
    {
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, sX + 0, sY + 0, 60);
        put_aligned_string(sX + 98, sX + 357, sY + 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER13);
    }
    else
    {
        //		if ((msX > 360) && (msY >= 406) && (msX < 545) & (msY <= 435)) {
        //			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, 0,0,60);

        //		} else {

        if (cTotalChar == 0)
        {
            put_aligned_string(sX + 98, sX + 357, sY + 275 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER14);
            put_aligned_string(sX + 98, sX + 357, sY + 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER15);
            put_aligned_string(sX + 98, sX + 357, sY + 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER16);
            put_aligned_string(sX + 98, sX + 357, sY + 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER17);
            put_aligned_string(sX + 98, sX + 357, sY + 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER18);
        }
        else if (cTotalChar < 4)
        {
            put_aligned_string(sX + 98, sX + 357, sY + 275 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER19);
            put_aligned_string(sX + 98, sX + 357, sY + 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER20);
            put_aligned_string(sX + 98, sX + 357, sY + 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER21);
            put_aligned_string(sX + 98, sX + 357, sY + 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER22);
            put_aligned_string(sX + 98, sX + 357, sY + 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER23);
            put_aligned_string(sX + 98, sX + 357, sY + 350 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER24);
        }
        if (cTotalChar == 4)
        {
            put_aligned_string(sX + 98, sX + 357, sY + 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER25);
            put_aligned_string(sX + 98, sX + 357, sY + 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER26);
            put_aligned_string(sX + 98, sX + 357, sY + 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER27);
            put_aligned_string(sX + 98, sX + 357, sY + 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER28);
        }
    }


    int iTempMon, iTempDay, iTempHour, iTempMin;
    iTempMon = iTempDay = iTempHour = iTempMin = 0;

    if (m_iAccntYear != 0)
    {
        iTempMin = (m_iTimeLeftSecAccount / 60);
        format_to_local(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER37, m_iAccntYear, m_iAccntMonth, m_iAccntDay, iTempMin);
    }
    else
    {
        if (m_iTimeLeftSecAccount > 0)
        {
            iTempDay = (m_iTimeLeftSecAccount / (60 * 60 * 24));
            iTempHour = (m_iTimeLeftSecAccount / (60 * 60)) % 24;
            iTempMin = (m_iTimeLeftSecAccount / 60) % 60;
            format_to_local(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER38, iTempDay, iTempHour, iTempMin);
        }
        else strcpy(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER39);

    }
    put_aligned_string(sX + 98, sX + 357, sY + 385 + 10, G_cTxt);

    if (m_iIpYear != 0)
    {
        iTempHour = (m_iTimeLeftSecIP / (60 * 60));
        iTempMin = (m_iTimeLeftSecIP / 60) % 60;
        format_to_local(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER40, m_iIpYear, m_iIpMonth, m_iIpDay, iTempHour, iTempMin);
    }
    else
    {
        if (m_iTimeLeftSecIP > 0)
        {
            iTempDay = (m_iTimeLeftSecIP / (60 * 60 * 24));
            iTempHour = (m_iTimeLeftSecIP / (60 * 60)) % 24;
            iTempMin = (m_iTimeLeftSecIP / 60) % 60;
            format_to_local(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER41, iTempDay, iTempHour, iTempMin);

        }
        else strcpy(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER42);

    }

    put_aligned_string(sX + 98, sX + 357, sY + 400 + 10, G_cTxt);

    if (iYear != 0)
    {
        format_to_local(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER43, iYear, iMonth, iDay, iHour, iMinute);
        put_aligned_string(sX + 98, sX + 357, sY + 415 + 10, G_cTxt);
    }

    put_aligned_string(sX + 129, sX + 321, sY + 456, MSG_WORLDNAME1);
}

void CGame::UpdateScreen_OnWaitingResponse()
{
    short sX, sY;
    uint16_t msX{}, msY{}, msZ{};
    bool LB{}, RB{}, MB{};

    uint32_t dwTime = unixtime();
    //static CMouseInterface * pMI;
    static uint32_t dwCTime;

    update_mouse_state(msX, msY, msZ, LB, RB, MB);

    if (m_cGameModeCount == 0)
    {
        m_bEnterPressed = false;
        m_bEscPressed = false;
        dwCTime = unixtime();
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {

        if ((dwTime - m_dwTime) > 7000)
        {
            ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
            close(ix::WebSocketCloseConstants::kNormalClosureCode, ix::WebSocketCloseConstants::kNormalClosureMessage);
        }

        m_bEscPressed = false;
        return;
    }

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    m_bIsHideLocalCursor = true;
    switch (m_cMsg[0])
    {
        case '0':
            _Draw_UpdateScreen_OnCreateNewAccount();
            break;

        case '1':
            sX = 146;
            sY = 114;
            _Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
            break;

        case '2':
            _bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
            break;

        case '3':
            UpdateScreen_OnSelectCharacter(modx, mody, 0, 0);
            break;

        case '4':
            // Change Password
            UpdateScreen_OnSelectCharacter(modx, mody, 0, 0, true);
            break;
    }
    m_bIsHideLocalCursor = false;

    draw_shadow_box(modx, mody, get_virtual_width() - 1, get_virtual_height() - 1);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, modx + 162, mody + 125, 2);
    put_string_sprite_font(modx + 172 + 44 - 17, mody + 190, "Connected. Waiting for response...", 7, 0, 0);

    if ((dwTime - m_dwTime) > 7000)
    {
        put_aligned_string(modx + 180, modx + 463, mody + 195 + 30, UPDATE_SCREEN_ON_WATING_RESPONSE1);

        put_aligned_string(modx + 180, modx + 463, mody + 195 + 45, UPDATE_SCREEN_ON_WATING_RESPONSE2);

    }
    else put_aligned_string(modx + 180, modx + 463, mody + 195 + 30, UPDATE_SCREEN_ON_WATING_RESPONSE3);

    DrawVersion();
    m_stMCursor.sCursorFrame = 8;
}

void CGame::UpdateScreen_OnQueryDeleteCharacter()
{
    uint16_t msX{}, msY{}, msZ{};
    bool LB{}, RB{}, MB{};
    char cMIresult;
    int  iMIbuttonNum;
    static CMouseInterface * pMI;
    static uint32_t dwCTime;
    uint32_t dwTime = unixtime();

    update_mouse_state(msX, msY, msZ, LB, RB, MB);

    if (m_cGameModeCount == 0)
    {
        pMI = new CMouseInterface;
        pMI->AddRect(200, 244, 200 + DEF_BTNSZX, 244 + DEF_BTNSZY);
        pMI->AddRect(370, 244, 370 + DEF_BTNSZX, 244 + DEF_BTNSZY);
        //m_cCurFocus = 1;
        //m_cMaxFocus = 2;
        m_bEnterPressed = false;
        m_cArrowPressed = 0;

        dwCTime = unixtime();

        PlaySound('E', 25, 0);
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true)
    {
        ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
        delete pMI;
        m_bEscPressed = false;
        return;
    }

    UpdateScreen_OnSelectCharacter(modx, mody, 500, 70);
    if ((m_cGameModeCount >= 0) && (m_cGameModeCount < 6))
    {
        draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
    }
    else if (m_cGameModeCount >= 6)
    {
        draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
        draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
    }

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, modx + 162, mody + 125, 2);

    put_string_sprite_font(modx + 172 + 86, mody + 160, "Delete Character", 7, 0, 0);
    put_string(modx + 215, mody + 195, UPDATE_SCREEN_ON_QUERY_DELETE_CHARACTER1, Color(5, 5, 5));
    //"Character Name"
    put_string(modx + 335, mody + 199, "__________", Color(5, 5, 5));
    put_string(modx + 335, mody + 195, m_pCharList[m_wEnterGameType - 1]->m_cName, Color(25, 35, 25));
    put_aligned_string(modx + 178, modx + 453, mody + 220, UPDATE_SCREEN_ON_QUERY_DELETE_CHARACTER2);


    // v2.05
    if ((msX >= modx + 200) && (msX <= modx + 200 + DEF_BTNSZX) && (msY >= mody + 244) && (msY <= mody + 244 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, modx + 200, mody + 244, 19);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, modx + 200, mody + 244, 18);

    if ((msX >= modx + 370) && (msX <= modx + 370 + DEF_BTNSZX) && (msY >= mody + 244) && (msY <= mody + 244 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, modx + 370, mody + 244, 3);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, modx + 370, mody + 244, 2);

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    iMIbuttonNum = pMI->iGetStatus(msX, msY, LB, &cMIresult);
    if (cMIresult == DEF_MIRESULT_CLICK)
    {

        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            case 1:

                ChangeGameMode(DEF_GAMEMODE_ONCONNECTING);
                m_dwConnectMode = MSGID_REQUEST_DELETECHARACTER;
                memset(m_cMsg, 0, sizeof(m_cMsg));
                strcpy(m_cMsg, "33");
                delete pMI;
                ConnectionEstablishHandler(DEF_SERVERTYPE_LOG);
                return;

            case 2:
                ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
                delete pMI;
                break;
        }
    }

    DrawVersion();
    m_stMCursor.sCursorFrame = 0;
}

void CGame::UpdateScreen_OnLogResMsg()
{
    uint16_t msX{}, msY{}, msZ{}, sX{}, sY{};
    bool LB{}, RB{}, MB{};
    uint32_t dwTime = unixtime();
    static uint32_t dwCTime;
    static CMouseInterface * pMI;
    int   iMIbuttonNum;
    char  cMIresult;

    update_mouse_state(msX, msY, msZ, LB, RB, MB);

    if (m_cGameModeCount == 0)
    {
        pMI = new CMouseInterface();
        pMI->AddRect(modx + 370, mody + 240, modx + 370 + DEF_BTNSZX, mody + 240 + DEF_BTNSZY);

        m_bEnterPressed = false;
        m_bEscPressed = false;
        m_cArrowPressed = 0;
        dwCTime = unixtime();

        if (m_bSoundFlag) m_pESound[38].stop();
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_bEscPressed == true || m_bEnterPressed)
    {
        switch (m_cMsg[0])
        {
            case '0':
                ChangeGameMode(DEF_GAMEMODE_ONCREATENEWACCOUNT);
                break;
            case '1':
                ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                break;
            case '2':
                ChangeGameMode(DEF_GAMEMODE_ONCREATENEWCHARACTER);
                break;
            case '3':
                ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
                break;
            case '4':
                ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
                break;
            case '5':
                ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                break;
            case '6':
                switch (m_cMsg[1])
                {
                    case 'B':
                        ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                        break;
                    case 'C': /*ChangeGameMode(DEF_GAMEMODE_ONCHANGEPASSWORD);*/ break;
                    case 'M': /*ChangeGameMode(DEF_GAMEMODE_ONCHANGEPASSWORD);*/ break;
                }
                break;
            case '7':
            case '8':
                ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                break;
        }

        delete pMI;
        m_bEscPressed = false;
        return;
    }

    switch (m_cMsg[0])
    {
        case '0':
        case '5':
            _Draw_UpdateScreen_OnCreateNewAccount();
            break;

        case '1':
        case '7':
            sX = 146;
            sY = 114;

            _Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
            break;

        case '2':
        case '4':
            _bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
            break;

        case '3':
            sX = 0;
            sY = 0;
            UpdateScreen_OnSelectCharacter(modx, mody, 0, 0);
            break;

        case '6':
            sX = 146;
            sY = 114;

            UpdateScreen_OnSelectCharacter(modx, mody, 0, 0, true);
            break;
        case '8':
            DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_MAINMENU, modx + -1, mody + -1, 0, true);
            break;
    }

    draw_shadow_box(modx, mody, get_virtual_width() - 1, get_virtual_height() - 1);

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, modx + 162, mody + 125, 2);

    if ((msX >= modx + 370) && (msX <= modx + 370 + DEF_BTNSZX) && (msY >= mody + 244) && (msY <= mody + 244 + DEF_BTNSZY))
        DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, modx + 370, mody + 244, 1);
    else DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_BUTTON, modx + 370, mody + 244, 0);

    switch (m_cMsg[1])
    {
        case '1':
            put_string_sprite_font(modx + 172 + 70, mody + 165, "Password is not correct!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG5);
            break;

        case '2':
            put_string_sprite_font(modx + 172 + 70, mody + 165, "Not existing account!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG6);
            put_aligned_string(modx + 198, modx + 453, mody + 215, UPDATE_SCREEN_ON_LOG_MSG7);
            break;

        case '3':
            put_string_sprite_font(modx + 172 + 10 + 34, mody + 165, "Can not connect to game server!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG8);
            put_aligned_string(modx + 198, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG9);
            put_aligned_string(modx + 198, modx + 453, mody + 225, UPDATE_SCREEN_ON_LOG_MSG10);
            break;

        case '4':
            put_string_sprite_font(modx + 172 + 58, mody + 165, "New account created.", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG11);
            put_aligned_string(modx + 198, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG12);
            break;

        case '5':
            put_string_sprite_font(modx + 172 + 58, mody + 165, "Can not create new account!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG13);
            break;

        case '6':
            put_string_sprite_font(modx + 172 + 36, mody + 165, "Can not create new account!", 7, 0, 0);
            put_string_sprite_font(modx + 172 + 24, mody + 180, "Already existing account name.", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 205, UPDATE_SCREEN_ON_LOG_MSG14);
            put_aligned_string(modx + 198, modx + 453, mody + 220, UPDATE_SCREEN_ON_LOG_MSG15);
            break;

        case '7':
            put_string_sprite_font(modx + 172 + 58, mody + 165, "New character created.", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG16);
            break;

        case '8':
            put_string_sprite_font(modx + 172 + 58, mody + 165, "Can not create new character!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG17);
            break;

        case '9':
            put_string_sprite_font(modx + 172 + 36, mody + 165, "Can not create new character!", 7, 0, 0);
            put_string_sprite_font(modx + 172 + 24, mody + 180, "Already existing character name.", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 205, UPDATE_SCREEN_ON_LOG_MSG18);
            put_aligned_string(modx + 198, modx + 453, mody + 220, UPDATE_SCREEN_ON_LOG_MSG19);
            break;

        case 'A':
            put_string_sprite_font(modx + 172 + 36 + 45, mody + 165, "Character deleted.", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG20);

            break;
        case 'B':
            put_string_sprite_font(modx + 172 + 36 + 45, mody + 165, "Password changed.", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG21);
            break;
        case 'C':
            put_string_sprite_font(modx + 172 + 36, mody + 165, "Can not change password!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG22);
            break;

        case 'D':
            put_string_sprite_font(modx + 172 + 10 + 34, mody + 165, "Can not connect to game server!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG23);
            put_aligned_string(modx + 198, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG24);
            break;

        case 'E':
            put_string_sprite_font(modx + 172 + 10 + 34, mody + 165, "Can not connect to game server!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG25);
            put_aligned_string(modx + 198, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG26);
            put_aligned_string(modx + 198, modx + 453, mody + 225, UPDATE_SCREEN_ON_LOG_MSG27);
            break;

        case 'F':
            put_string_sprite_font(modx + 172 + 10 + 34, mody + 165, "Can not connect to game server!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG28);
            put_aligned_string(modx + 198, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG29);
            break;

        case 'G':
            put_string_sprite_font(modx + 172 + 10 + 34, mody + 165, "Can not connect to game server!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG30);
            put_aligned_string(modx + 198, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG31);
            break;

        case 'H':
            put_string_sprite_font(modx + 172 + 68, mody + 165, "Connection Rejected!", 7, 0, 0);
            if (m_iBlockYear == 0)
            {
                put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG32);
                put_aligned_string(modx + 198, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG33);
            }
            else
            {
                put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG34);
                format_to_local(G_cTxt, UPDATE_SCREEN_ON_LOG_MSG35, m_iBlockYear, m_iBlockMonth, m_iBlockDay);
                put_aligned_string(modx + 198, modx + 453, mody + 210, G_cTxt);
            }

            break;

        case 'I':
            put_string_sprite_font(modx + 172 + 68, mody + 165, "Not Enough Point!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 210, "ÂI¼Æ¨Ï¥Î´Á­­¤wµ²§ô, ½Ð¦ÜGD2S.gamania.com©µªø¨Ï¥Î´Á­­");

            break;

        case 'J':
            put_string_sprite_font(modx + 172 + 68, mody + 165, "World Server Full", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 210, "Please ! Try Other World Server");
            break;

        case 'M':
            put_string_sprite_font(modx + 172 + 68, mody + 165, "Your password expired", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 210, "Please! Change password");
            break;


        case 'U':
            put_string_sprite_font(modx + 172 + 68, mody + 165, "Keycode input Success!", 7, 0, 0);
            put_aligned_string(modx + 198, modx + 453, mody + 210, "Keycode Registration successed.");

            break;

        case 'X':
            put_aligned_string(modx + 198, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG38);
            put_aligned_string(modx + 198, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG39);
            break;

        case 'Y':
            put_aligned_string(modx + 178, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG40);
            put_aligned_string(modx + 178, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG41);
            break;

        case 'Z':
            put_aligned_string(modx + 178, modx + 453, mody + 195, UPDATE_SCREEN_ON_LOG_MSG42);
            put_aligned_string(modx + 178, modx + 453, mody + 210, UPDATE_SCREEN_ON_LOG_MSG41);
            break;
    }

    iMIbuttonNum = pMI->iGetStatus(msX, msY, LB, &cMIresult);
    if (cMIresult == DEF_MIRESULT_CLICK)
    {

        switch (iMIbuttonNum)
        {
            case 1:
                switch (m_cMsg[0])
                {
                    case '0':
                        ChangeGameMode(DEF_GAMEMODE_ONCREATENEWACCOUNT);
                        break;
                    case '1':
                        ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                        break;
                    case '2':
                        ChangeGameMode(DEF_GAMEMODE_ONCREATENEWCHARACTER);
                        break;
                    case '3':
                        ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
                        break;
                    case '4':
                        ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
                        break;
                    case '5':
                        ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                        break;
                    case '6':
                        switch (m_cMsg[1])
                        {
                            case 'B':
                                ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                                break;
                            case 'C': /*ChangeGameMode(DEF_GAMEMODE_ONCHANGEPASSWORD);*/ break;
                            case 'M': /*ChangeGameMode(DEF_GAMEMODE_ONCHANGEPASSWORD);*/ break;
                        }
                        break;
                    case '7':
                    case '8':
                        ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                        break;
                }
                delete pMI;
                return;
        }
    }

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    DrawVersion();

    m_stMCursor.sCursorFrame = 0;
}

void CGame::UpdateScreen_OnVersionNotMatch()
{
    uint16_t msX{}, msY{}, msZ{};
    bool LB{}, RB{}, MB{};

    update_mouse_state(msX, msY, msZ, LB, RB, MB);

    char cMIresult;
    int  iMIbuttonNum;

    static CMouseInterface * pMI;
    uint32_t dwTime = unixtime();

    if (m_cGameModeCount == 0)
    {
        pMI = new CMouseInterface;
        pMI->AddRect(modx + 0, mody + 0, get_virtual_width(), get_virtual_height());
        m_bEnterPressed = false;
    }
    m_cGameModeCount++;
    if (m_cGameModeCount > 120) m_cGameModeCount = 120;

    if (m_bEscPressed == true || m_bEnterPressed == true)
    {
        m_bEscPressed = false;
        m_bEnterPressed = false;
        delete pMI;
        ChangeGameMode(DEF_GAMEMODE_NULL);
        // todo: windows
        SendMessage(m_hWnd, WM_DESTROY, 0, 0);
        return;
    }

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_QUIT, 0, 0, 0, true);
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME4, 162, 125, 2);

    put_aligned_string(modx + 168, modx + 474, 160, UPDATE_SCREEN_ON_VERSION_NO_MATCH1);
    put_aligned_string(modx + 168, modx + 474, 180, UPDATE_SCREEN_ON_VERSION_NO_MATCH2);
    put_aligned_string(modx + 168, modx + 474, 200, UPDATE_SCREEN_ON_VERSION_NO_MATCH3);

    put_aligned_string(modx + 168, modx + 474, 250, "https://helbreathx.net");

    DrawVersion();
    m_stMCursor.sCursorFrame = 0;

    iMIbuttonNum = pMI->iGetStatus(msX, msY, LB, &cMIresult);
    if ((cMIresult == DEF_MIRESULT_CLICK) && (iMIbuttonNum == 1))
    {
        ChangeGameMode(DEF_GAMEMODE_NULL);
        delete pMI;
        // todo: windows
        SendMessage(m_hWnd, WM_DESTROY, 0, 0);
        return;
    }
}
void CGame::UpdateScreen_OnGame()
{
    uint16_t sVal{}, sDivX{}, sModX{}, sDivY{}, sModY{}, sPivotX{}, sPivotY{}, sVPXsave{}, sVPYsave{};
    //static int  iUpdateRet = -1;
    uint16_t msX{}, msY{}, msZ{}, absX{}, absY{}, tX{}, tY{};
    bool LB{}, RB{}, MB{};
    char cItemColor{};
    int i{}, iAmount{};
    uint32_t dwTime = unixtime();
    static uint32_t dwPrevChatTime = 0;
    static int imX = 0, imY = 0;
    char cTmp[100]{}, cTmp2[100]{};

    update_mouse_state(msX, msY, msZ, LB, RB, MB);

    if (m_cGameModeCount == 0)
    {
        m_dwFPStime = m_dwCheckConnTime = m_dwCheckSprTime = m_dwCheckChatTime = m_dwCheckWhoTime = m_dwCheckPingTime = m_dwCheckPkTime = m_dwPartyListTime = m_dwBWTime = dwTime;
#ifdef DEF_ADMINCLIENT
        m_dwCheckRefreshTime = dwTime;
#endif
        if (m_bMusicStat) StartBGM();
        if (m_iLevel < 40) AddEventList(UPDATE_SCREEN_ONGAME12, 10);
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 20) m_cGameModeCount = 20;

    m_dwCurTime = unixtime();

    // todo: ??
    // Magic types for coords setup
    if ((m_bIsGetPointingMode == true) && (m_iPointCommandType > 100 && m_iPointCommandType < 200))
    {
        if (m_iPointCommandType == 173)
        {
            m_coordlist[0].x = ((m_sViewPointX + m_sMX + 16) / 32) - 2;
            m_coordlist[0].y = ((m_sViewPointY + m_sMY + 16) / 32);
            m_coordlist[1].x = ((m_sViewPointX + m_sMX + 16) / 32) - 1;
            m_coordlist[1].y = ((m_sViewPointY + m_sMY + 16) / 32);
            m_coordlist[2].x = ((m_sViewPointX + m_sMX + 16) / 32);
            m_coordlist[2].y = ((m_sViewPointY + m_sMY + 16) / 32);
            m_coordlist[3].x = ((m_sViewPointX + m_sMX + 16) / 32) + 1;
            m_coordlist[3].y = ((m_sViewPointY + m_sMY + 16) / 32);
            m_coordlist[4].x = ((m_sViewPointX + m_sMX + 16) / 32) + 2;
            m_coordlist[4].y = ((m_sViewPointY + m_sMY + 16) / 32);

            m_coordlist[5].x = ((m_sViewPointX + m_sMX + 16) / 32) - 2;
            m_coordlist[5].y = ((m_sViewPointY + m_sMY + 16) / 32) - 2;
            m_coordlist[6].x = ((m_sViewPointX + m_sMX + 16) / 32) - 1;
            m_coordlist[6].y = ((m_sViewPointY + m_sMY + 16) / 32) - 2;
            m_coordlist[7].x = ((m_sViewPointX + m_sMX + 16) / 32);
            m_coordlist[7].y = ((m_sViewPointY + m_sMY + 16) / 32) - 2;
            m_coordlist[8].x = ((m_sViewPointX + m_sMX + 16) / 32) + 1;
            m_coordlist[8].y = ((m_sViewPointY + m_sMY + 16) / 32) - 2;
            m_coordlist[9].x = ((m_sViewPointX + m_sMX + 16) / 32) + 2;
            m_coordlist[9].y = ((m_sViewPointY + m_sMY + 16) / 32) - 2;

            m_coordlist[10].x = ((m_sViewPointX + m_sMX + 16) / 32) - 2;
            m_coordlist[10].y = ((m_sViewPointY + m_sMY + 16) / 32) - 1;
            m_coordlist[11].x = ((m_sViewPointX + m_sMX + 16) / 32) - 1;
            m_coordlist[11].y = ((m_sViewPointY + m_sMY + 16) / 32) - 1;
            m_coordlist[12].x = ((m_sViewPointX + m_sMX + 16) / 32);
            m_coordlist[12].y = ((m_sViewPointY + m_sMY + 16) / 32) - 1;
            m_coordlist[13].x = ((m_sViewPointX + m_sMX + 16) / 32) + 1;
            m_coordlist[13].y = ((m_sViewPointY + m_sMY + 16) / 32) - 1;
            m_coordlist[14].x = ((m_sViewPointX + m_sMX + 16) / 32) + 2;
            m_coordlist[14].y = ((m_sViewPointY + m_sMY + 16) / 32) - 1;
        }
    }

    if (m_bEnterPressed == true)
    {
        m_bEnterPressed = false;

        if ((m_bIsDialogEnabled[7] == true) && (m_stDialogBoxInfo[7].cMode == 1) && (iGetTopDialogBoxIndex() == 7))
        {
            EndInputString();

            m_bEnterPressed = false;
            if (strlen(m_cGuildName) == 0) return;
            if (strcmp(m_cGuildName, "NONE") != 0)
            {
                bSendCommand(MSGID_REQUEST_CREATENEWGUILD, DEF_MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
                m_stDialogBoxInfo[7].cMode = 2;
            }
        }
        else if ((m_bIsDialogEnabled[71] == true) && (m_stDialogBoxInfo[71].sV1 == 1) && (iGetTopDialogBoxIndex() == 71))
        {
            EndInputString();
            m_bEnterPressed = false;
            m_stDialogBoxInfo[71].sV1 = 0;
            return;
        }
        else if ((m_bIsDialogEnabled[17] == true) && (m_stDialogBoxInfo[17].cMode == 1) && (iGetTopDialogBoxIndex() == 17))
        {
            // Drop Item Query 

            EndInputString();
            if (m_bSkillUsingStatus == true)
            {
                AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                return;
            }

            if ((m_bIsDialogEnabled[20] == true) &&
                ((m_stDialogBoxInfo[20].cMode == 1) || (m_stDialogBoxInfo[20].cMode == 2)))
            {
                AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                return;
            }

            if ((m_bIsDialogEnabled[4] == true) || (m_bIsDialogEnabled[23] == true) || (m_bIsDialogEnabled[26] == true))
            {
                AddEventList(UPDATE_SCREEN_ONGAME1, 10);
                return;
            }

            if (strlen(m_cAmountString) == 0) return;
            iAmount = atoi(m_cAmountString);
            if ((int)(m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwCount) < iAmount)
            {
                iAmount = m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwCount;
            }

            if (iAmount != 0)
            {
                if ((int)(m_pItemList[m_stDialogBoxInfo[17].sView]->m_dwCount) >= iAmount)
                {
                    if (m_stDialogBoxInfo[17].sV1 != 0)
                    {
                        absX = abs(m_stDialogBoxInfo[17].sV1 - m_sPlayerX);
                        absY = abs(m_stDialogBoxInfo[17].sV2 - m_sPlayerY);

                        if ((absX == 0) && (absY == 0))
                            AddEventList(UPDATE_SCREEN_ONGAME5, 10);
                        else if ((absX <= 8) && (absY <= 8))
                        {

                            switch (m_stDialogBoxInfo[17].sV3)
                            {
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                case 5:
                                case 6:
                                    EnableDialogBox(20, 1, m_stDialogBoxInfo[17].sView, m_stDialogBoxInfo[17].sV3);
                                    m_stDialogBoxInfo[20].sV3 = iAmount;
                                    m_stDialogBoxInfo[20].sV4 = m_wCommObjectID;	// v1.4
                                    m_stDialogBoxInfo[20].sV5 = m_stDialogBoxInfo[17].sV1;
                                    m_stDialogBoxInfo[20].sV6 = m_stDialogBoxInfo[17].sV2;

                                    tX = msX - 117;
                                    tY = msY - 50;
                                    if (tX < 0) tX = 0;
                                    if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                    if (tY < 0) tY = 0;
                                    if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                    m_stDialogBoxInfo[20].sX = tX;
                                    m_stDialogBoxInfo[20].sY = tY;

                                    memset(m_stDialogBoxInfo[20].cStr, 0, sizeof(m_stDialogBoxInfo[20].cStr));
                                    strcpy(m_stDialogBoxInfo[20].cStr, m_stDialogBoxInfo[17].cStr);
                                    break;

                                case 20:
                                    EnableDialogBox(20, 3, m_stDialogBoxInfo[17].sView, m_stDialogBoxInfo[17].sV3);
                                    m_stDialogBoxInfo[20].sV3 = iAmount;
                                    m_stDialogBoxInfo[20].sV4 = m_wCommObjectID;	// v1.4
                                    m_stDialogBoxInfo[20].sV5 = m_stDialogBoxInfo[17].sV1;
                                    m_stDialogBoxInfo[20].sV6 = m_stDialogBoxInfo[17].sV2;

                                    tX = msX - 117;
                                    tY = msY - 50;
                                    if (tX < 0) tX = 0;
                                    if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                    if (tY < 0) tY = 0;
                                    if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;

                                    m_stDialogBoxInfo[20].sX = tX;
                                    m_stDialogBoxInfo[20].sY = tY;

                                    memset(m_stDialogBoxInfo[20].cStr, 0, sizeof(m_stDialogBoxInfo[20].cStr));
                                    GetNpcName(m_stDialogBoxInfo[17].sV3, m_stDialogBoxInfo[20].cStr);
                                    break;

                                case 15:
                                case 24:
                                    EnableDialogBox(20, 2, m_stDialogBoxInfo[17].sView, m_stDialogBoxInfo[17].sV3);
                                    m_stDialogBoxInfo[20].sV3 = iAmount;
                                    m_stDialogBoxInfo[20].sV4 = m_wCommObjectID;

                                    tX = msX - 117;
                                    tY = msY - 50;
                                    if (tX < 0) tX = 0;
                                    if ((tX + 235) > get_virtual_width() - 1) tX = get_virtual_width() - 1 - 235;
                                    if (tY < 0) tY = 0;
                                    if ((tY + 100) > get_virtual_height() - 1) tY = get_virtual_height() - 1 - 100;
                                    m_stDialogBoxInfo[20].sX = tX;
                                    m_stDialogBoxInfo[20].sY = tY;

                                    memset(m_stDialogBoxInfo[20].cStr, 0, sizeof(m_stDialogBoxInfo[20].cStr));
                                    GetNpcName(m_stDialogBoxInfo[17].sV3, m_stDialogBoxInfo[20].cStr);
                                    break;

                                case 1000:
                                    /*m_stDialogBoxInfo[27].sV1 = m_pItemList[m_stDialogBoxInfo[17].sV4]->m_sSprite;
                                    m_stDialogBoxInfo[27].sV2 = m_pItemList[m_stDialogBoxInfo[17].sV4]->m_sSpriteFrame;
                                    m_stDialogBoxInfo[27].sV3 = iAmount;
                                    m_stDialogBoxInfo[27].sV4 = m_pItemList[m_stDialogBoxInfo[17].sV4]->m_cItemColor;*/

                                    if (m_stDialogBoxExchangeInfo[0].sV1 == -1)			m_stDialogBoxExchangeInfo[0].sItemID = m_stDialogBoxInfo[17].sV4;
                                    else if (m_stDialogBoxExchangeInfo[1].sV1 == -1)	m_stDialogBoxExchangeInfo[1].sItemID = m_stDialogBoxInfo[17].sV4;
                                    else if (m_stDialogBoxExchangeInfo[2].sV1 == -1)	m_stDialogBoxExchangeInfo[2].sItemID = m_stDialogBoxInfo[17].sV4;
                                    else if (m_stDialogBoxExchangeInfo[3].sV1 == -1)	m_stDialogBoxExchangeInfo[3].sItemID = m_stDialogBoxInfo[17].sV4;
                                    else return; // Impossible case, tested at function beginning
                                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SETEXCHANGEITEM, 0, m_stDialogBoxInfo[17].sV4, iAmount, 0, 0);
                                    break;

                                case 1001:
                                    for (i = 0; i < DEF_MAXSELLLIST; i++)
                                        if (m_stSellItemList[i].iIndex == -1)
                                        {
                                            m_stSellItemList[i].iIndex = m_stDialogBoxInfo[17].sV4;
                                            m_stSellItemList[i].iAmount = iAmount;
                                            m_bIsItemDisabled[m_stDialogBoxInfo[17].sV4] = true;
                                            break;
                                        }
                                    if (i == DEF_MAXSELLLIST) AddEventList(UPDATE_SCREEN_ONGAME6, 10);
                                    break;

                                case 1002:
                                    if (_iGetBankItemCount() >= (DEF_MAXBANKITEMS - 1)) AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
                                    else bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, m_stDialogBoxInfo[39].sV1, iAmount, m_stDialogBoxInfo[39].sV5, m_stDialogBoxInfo[39].sV6, m_pItemList[m_stDialogBoxInfo[39].sV1]->m_cName, m_stDialogBoxInfo[39].sV4); //v1.4
                                    break;

                                default:
                                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, (char)(m_stDialogBoxInfo[17].sView), iAmount, m_stDialogBoxInfo[17].sV1, m_stDialogBoxInfo[17].sV2, m_pItemList[m_stDialogBoxInfo[17].sView]->m_cName);
                                    break;
                            }
                            m_bIsItemDisabled[m_stDialogBoxInfo[17].sView] = true;
                        }
                        else AddEventList(UPDATE_SCREEN_ONGAME7, 10);
                    }
                    else
                    {
                        if (iAmount <= 0)
                        {
                            AddEventList(UPDATE_SCREEN_ONGAME8, 10);
                        }
                        else
                        {
                            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_ITEMDROP, 0, m_stDialogBoxInfo[17].sView, iAmount, 0, m_pItemList[m_stDialogBoxInfo[17].sView]->m_cName);
                            m_bIsItemDisabled[m_stDialogBoxInfo[17].sView] = true;
                        }
                    }
                }
                else
                {
                    AddEventList(UPDATE_SCREEN_ONGAME9, 10);
                }
            }
            else
            {
            }
            DisableDialogBox(17);
        }
        else
        {
            if (!m_bInputStatus)
            {
                switch (m_cBackupChatMsg[0])
                {
                    case '!':
                    case '@':
                    case '#':
                    case '$':
                    case '^':
                        memset(m_cChatMsg, 0, sizeof(m_cChatMsg));
                        m_cChatMsg[0] = m_cBackupChatMsg[0];
                        StartInputString(10, get_virtual_height() - 66, sizeof(m_cChatMsg), m_cChatMsg);
                        break;
                    default:
                        StartInputString(10, get_virtual_height() - 66, sizeof(m_cChatMsg), m_cChatMsg);
                        ClearInputString();
                        break;
                }
            }
            else
            {
                EndInputString();
                memset(G_cTxt, 0, sizeof(G_cTxt));
                ReceiveString((char *)G_cTxt);
                memset(m_cBackupChatMsg, 0, sizeof(m_cBackupChatMsg));
                strcpy(m_cBackupChatMsg, G_cTxt);
#ifdef DEF_HACKCLIENT
                if ((m_dwCurTime - dwPrevChatTime) < 0)
                {
#else
                if ((m_dwCurTime - dwPrevChatTime) < 700)
                {
#endif
                }
                else
                {
                    dwPrevChatTime = m_dwCurTime;
                    if (strlen(G_cTxt) > 0)
                    {
                        if ((G_cTxt[0] == '!') || (G_cTxt[0] == '~'))
                        {
                            if (m_Misc.bCheckIMEString(G_cTxt) == false) return;
                        }
                        bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, G_cTxt);
                    }
                }
                }
            }
        }
    sVPXsave = m_sViewPointX;
    sVPYsave = m_sViewPointY;
#ifdef DEF_HACKCLIENT
    if (m_stConfigList.bCameraShake == true)
    {
#endif
        if ((m_iCameraShakingDegree > 0) && (iUpdateRet != 0))
        {
            m_sViewPointX += m_iCameraShakingDegree - (rand() % m_iCameraShakingDegree * 2);
            m_sViewPointY += m_iCameraShakingDegree - (rand() % m_iCameraShakingDegree * 2);

            m_iCameraShakingDegree--;
            if (m_iCameraShakingDegree <= 0) m_iCameraShakingDegree = 0;
        }
#ifdef DEF_HACKCLIENT
    }
    else
    {
        m_iCameraShakingDegree = 0;
    }
#endif

    float delta_time = player_motion_clock.restart().asSeconds();

    // todo: ???
    uint16_t left = m_sViewPointX / 32;
    uint16_t top = m_sViewPointY / 32;
    sPivotX = 0;//m_sPlayerX - 10; // 12
    sPivotY = 0;//m_sPlayerY - 7; // 9

    sVal = m_sViewPointX;
    sDivX = sVal / 32;
    sModX = sVal % 32;

    sVal = m_sViewPointY;
    sDivY = sVal / 32;
    sModY = sVal % 32;

    float bgdx = (float(get_width()) / 2) - ((float(get_virtual_width()) * (zoom / 100)) / 2) - sModX + pan_x;
    float bgdy = (float(get_height()) / 2) - ((float(get_virtual_height()) * (zoom / 100)) / 2) - sModY + pan_y;

    uint8_t oldTarget = render_target();

    static int16_t mouse_x = ((sDivX) * 32 + sModX + m_stMCursor.sX - 17) / 32 + 1;
    static int16_t mouse_y = ((sDivY) * 32 + sModY + m_stMCursor.sY - 17) / 32 + 1;

    {
        iUpdateRet = m_pMapData->object_frame_counter(m_cPlayerName, m_sViewPointX, m_sViewPointY, true);
        if (iUpdateRet == 0)
            iUpdateRet = -1;
        //if ((bEffectFrameCounter() == true) && (iUpdateRet == 0)) iUpdateRet = -1;
        if (iUpdateRet == 2)
        {
            m_bCommandAvailable = true;
            m_dwCommandTime = 0;
        }
    }

    if (
        update_background
        || update_objects
        || mouse_x != ((sDivX) * 32 + sModX + m_stMCursor.sX - 17) / 32 + 1
        || mouse_y != ((sDivY) * 32 + sModY + m_stMCursor.sY - 17) / 32 + 1
        )
    {
        mouse_x = ((sDivX) * 32 + sModX + m_stMCursor.sX - 17) / 32 + 1;
        mouse_y = ((sDivY) * 32 + sModY + m_stMCursor.sY - 17) / 32 + 1;
        render_target(DS_BG, true, Color(0, 0, 0, 0));
        DrawBackground(sDivX, sModX, sDivY, sModY);

        render_target(DS_DYNBG, true, Color(0, 0, 0, 0));
        DrawEffectLights();
        DrawObjects(sPivotX, sPivotY, sDivX, sDivY, sModX, sModY, msX, msY);
        DrawEffects();
        update_objects = false;
        update_background = false;

        bg.display();
        bg_sprite = sf::Sprite(bg.getTexture());

        dynamic_bg.display();
        dynbg_sprite = sf::Sprite(dynamic_bg.getTexture());
    }

    if ((m_iGatePositX >= m_sViewPointX / 32) && (m_iGatePositX <= m_sViewPointX / 32 + 20)
        && (m_iGatePositY >= m_sViewPointY / 32) && (m_iGatePositY <= m_sViewPointY / 32 + 15))
    {
        m_pEffectSpr[101]->put_trans_sprite(m_iGatePositX * 32 - m_sViewPointX - 96, m_iGatePositY * 32 - m_sViewPointY - 69, (dwTime / 100) % 30, dwTime);
    }

    //if (m_cMapIndex == 26)	//Add Apocalypse map effect (fires in inferniaA)
    //{
    //	m_pEffectSpr[89]->put_trans_sprite(1296 - m_sViewPointX, 1283 - m_sViewPointY, _tmp_iEffectFrame %12, 0);
    //	m_pEffectSpr[89]->put_trans_sprite(1520 - m_sViewPointX, 1123 - m_sViewPointY, _tmp_iEffectFrame %12, 0);
    //	m_pEffectSpr[89]->put_trans_sprite(1488 - m_sViewPointX, 3971 - m_sViewPointY, _tmp_iEffectFrame %12, 0);
    //	m_pEffectSpr[93]->put_trans_sprite(2574 - m_sViewPointX, 3677 - m_sViewPointY, _tmp_iEffectFrame %12, 0);
    //	m_pEffectSpr[93]->put_trans_sprite(3018 - m_sViewPointX, 3973 - m_sViewPointY, _tmp_iEffectFrame %12, 0);
    //}
    //else if (m_cMapIndex == 27)	//Add Apocalypse map effect (fires in inferniaB)
    //{
    //	m_pEffectSpr[89]->put_trans_sprite(1293 - m_sViewPointX, 3657 - m_sViewPointY, _tmp_iEffectFrame %12, 0);
    //	m_pEffectSpr[89]->put_trans_sprite(944  - m_sViewPointX, 3881 - m_sViewPointY, _tmp_iEffectFrame %12, 0);
    //	m_pEffectSpr[89]->put_trans_sprite(1325 - m_sViewPointX, 4137 - m_sViewPointY, _tmp_iEffectFrame %12, 0);
    //	m_pEffectSpr[89]->put_trans_sprite(1648 - m_sViewPointX, 3913 - m_sViewPointY, _tmp_iEffectFrame %12, 0);
    //}

    bg_sprite.setScale(zoom / 100, zoom / 100);
    bg_sprite.setPosition(-sModX, -sModY);
    dynbg_sprite.setScale(zoom / 100, zoom / 100);
    render_target(oldTarget);
    DrawWhetherEffects();
    visible.draw(bg_sprite);
    visible.draw(dynbg_sprite);

    //if (update_dialogs)
    if (iUpdateRet != 0)
    {
        render_target(DS_DIALOGS, true, Color(0, 0, 0, 0));
        DrawChatMsgs(-100, 0, get_width(), get_height());
        weather_object_frame_counter();
        DrawDialogBoxs(msX, msY, msZ, LB);

        if (m_bInputStatus)
        {
            if (((m_bIsDialogEnabled[7] == true) && (m_stDialogBoxInfo[7].cMode == 1)) ||
                ((m_bIsDialogEnabled[17] == true) && (m_stDialogBoxInfo[17].cMode == 1)))
            {
            }
            else draw_shadow_box(0, get_virtual_height() - 63, get_virtual_width() - 1, get_virtual_height() - 51);
            ShowReceivedString();
        }

        ShowEventList(m_dwCurTime);

        dialog.display();

        render_target(oldTarget);
    }

    dialog_sprite = sf::Sprite(dialog.getTexture());
    dialog_sprite.setScale(zoom / 100, zoom / 100);
    visible.draw(dialog_sprite);

    if ((iUpdateRet != 0) && (m_stMCursor.cSelectedObjectType == DEF_SELECTEDOBJTYPE_ITEM) &&
        (m_pItemList[m_stMCursor.sSelectedObjectID] != 0))
    {
        cItemColor = m_pItemList[m_stMCursor.sSelectedObjectID]->m_cItemColor;

        if (cItemColor != 0)
        {
            if ((m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos == DEF_EQUIPPOS_LHAND) ||
                (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos == DEF_EQUIPPOS_RHAND) ||
                (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
            {
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->put_sprite_color(msX - m_stMCursor.sDistX, msY - m_stMCursor.sDistY,
                    m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame,
                    m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0],
                    dwTime);
            }
            else
            {
                m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->put_sprite_color(msX - m_stMCursor.sDistX, msY - m_stMCursor.sDistY,
                    m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame,
                    m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0],
                    dwTime);
            }
        }
        else m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite]->put_sprite_fast(msX - m_stMCursor.sDistX, msY - m_stMCursor.sDistY,
            m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame, dwTime);

        char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
        GetItemName(m_pItemList[m_stMCursor.sSelectedObjectID], cStr1, cStr2, cStr3, 64);

        std::vector<chat_msg> strings;

        Color stockcolor = Color(180, 180, 180);

        if (strlen(cStr1) != 0)
        {
            if (m_bIsSpecial)
                strings.push_back({ cStr1, Color(0, 255, 50), 14 });
            else
                strings.push_back({ cStr1, Color(255, 255, 255), 14 });
        }

        if (strlen(cStr2) != 0)
        {
            strings.push_back({ cStr2, stockcolor, 14 });
        }

        if (strlen(cStr3) != 0)
        {
            strings.push_back({ cStr3, stockcolor, 14 });
        }

        if ((m_pItemList[m_stMCursor.sSelectedObjectID]->m_sLevelLimit != 0) && ((m_pItemList[m_stMCursor.sSelectedObjectID]->m_dwAttribute & 0x00000001) == 0))
        {
            format_to_local(G_cTxt, "{}: {}", DRAW_DIALOGBOX_SHOP24, m_pItemList[m_stMCursor.sSelectedObjectID]->m_sLevelLimit);
            strings.push_back({ G_cTxt, stockcolor, 14 });
        }

        if ((m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos != DEF_EQUIPPOS_NONE) && (m_pItemList[m_stMCursor.sSelectedObjectID]->m_wWeight >= 1100))
        {
            int _wWeight = 0;
            if (m_pItemList[m_stMCursor.sSelectedObjectID]->m_wWeight % 100) _wWeight = 1;
            format_to_local(G_cTxt, DRAW_DIALOGBOX_SHOP15, m_pItemList[m_stMCursor.sSelectedObjectID]->m_wWeight / 100 + _wWeight);

            strings.push_back({ G_cTxt, stockcolor, 14 });
        }

        if (m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos != DEF_EQUIPPOS_NONE)
        {
            format_to_local(G_cTxt, UPDATE_SCREEN_ONGAME10, m_pItemList[m_stMCursor.sSelectedObjectID]->m_wCurLifeSpan);
            strings.push_back({ G_cTxt, stockcolor, 14 });
        }

        int iLoc = 0;
        for (int iTmp = 0; iTmp < DEF_MAXITEMS; iTmp++)
        {
            if (m_pItemList[iTmp] != 0)
            {
                if (strcmp(m_pItemList[iTmp]->m_cName, m_pItemList[m_stMCursor.sSelectedObjectID]->m_cName) == 0) iLoc++;
            }
        }
        if (iLoc > 1)
        {
            format_to_local(G_cTxt, DEF_MSG_TOTAL_NUMBER, iLoc);
            strings.push_back({ G_cTxt, stockcolor, 14 });
        }

        render_item_details_box(msX, msY + 20, strings, Color(0, 0, 0, 230));
    }

    DrawTopMsg();

#ifdef DEF_SHOWCURSORPOS
    if ((!m_bIsObserverMode) && (m_stConfigList.bMouseTileDebug))
    {
        format_to_local(G_cTxt, "M({},{}) T({},{})", msX, msY, (m_sViewPointX + msX) / 32, (m_sViewPointY + msY) / 32);
        put_string(msX, msY + 30, G_cTxt, Color(255, 255, 255));
    }
#endif

    //	if (m_cGameModeCount <= 15) {
    //		for (i = 0; i < (14 - (m_cGameModeCount-1)); i++) _DrawBlackRect(i);

    //	}

        // LogOut process
    if (m_cLogOutCount > 0)
    {
        if ((dwTime - m_dwLogOutCountTime) > 1000)
        {
            m_cLogOutCount--;
            m_dwLogOutCountTime = dwTime;
            format_to_local(G_cTxt, UPDATE_SCREEN_ONGAME13, m_cLogOutCount);
            AddEventList(G_cTxt, 10);
        }
    }
    if (m_cLogOutCount == 0)
    {
        close(ix::WebSocketCloseConstants::kNormalClosureCode, ix::WebSocketCloseConstants::kNormalClosureMessage);
        m_bEscPressed = false;

        PlaySound('E', 14, 5);
        if (m_bSoundFlag) m_pESound[38].stop();
        if ((m_bSoundFlag) && (m_bMusicStat == true)) m_pBGM.stop();

        ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
        return;
    }

    // Restart Process
    if (m_cRestartCount > 0)
    {
        if ((dwTime - m_dwRestartCountTime) > 1000)
        {
            m_cRestartCount--;
            m_dwRestartCountTime = dwTime;
            format_to_local(G_cTxt, UPDATE_SCREEN_ONGAME14, m_cRestartCount);
            AddEventList(G_cTxt, 10);
        }
    }
    if (m_cRestartCount == 0)
    {
        m_cRestartCount = -1;
//         for (int i = 0; i < 50; i++)//Change bag fix?
//         {
//             if (m_pItemList[i] != 0)
//                 bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, i, m_pItemList[i]->m_sX, m_pItemList[i]->m_sY, 0, 0);
//         }
        bSendCommand(MSGID_REQUEST_RESTART, 0, 0, 0, 0, 0, 0);
        return;
    }

    //Change Level up button
    if (m_iLU_Point > 0) put_string_sprite_font(get_virtual_width() - 80, get_virtual_height() - 90, "Level Up", 250, 250, 0);

    if (m_cGameModeCount < 6) draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);
    if (m_cGameModeCount < 2) draw_shadow_box(0, 0, get_virtual_width() - 1, get_virtual_height() - 1);

    if (iUpdateRet != 0)
    {
        int a = 100;
        char cBW1[5];
        char cBW2[5];
        memset(cBW1, 0, 5);
        memset(cBW2, 0, 5);

#ifdef DEF_HACKCLIENT
        if (m_stConfigList.bMouseTileDebug)
        {
            format_to_local(G_cTxt, "m_sMCX({}) m_sMCY({}) sX({}) sY({})", m_sMCX, m_sMCY, m_ssX, m_ssY);
            put_string(10, a, G_cTxt, Color(255, 255, 255));
            a += 12;
        }
#endif
        //				format_to_local(G_cTxt, "m_bIsGetPointingMode({})", IsPoint);
        //				put_string(10, 122, G_cTxt, Color(255,255,255));
        //				format_to_local(G_cTxt, "m_bCommandAvailable({})", IsCommand);
        //				put_string(10, 133, G_cTxt, Color(255,255,255));
        /*				if (m_iLastAmp < unixtime())
                        {
                            format_to_local(G_cTxt, "Amp runs out in: 0");
                        } else
                            format_to_local(G_cTxt, "Amp runs out in: {}", abs((long)(unixtime() - m_iLastAmp)));
                        put_string(10, a, G_cTxt, Color(255,255,255));
                        a += 12;
                        if (m_iLastBerserk  < unixtime())
                        {
                            format_to_local(G_cTxt, "Zerk runs out in: 0");
                        } else
                            format_to_local(G_cTxt, "Zerk runs out in: {}", abs((long)(unixtime() - m_iLastBerserk)));
                        put_string(10, a, G_cTxt, Color(255,255,255));
                        a += 12;*/
                        //			}
                        //		}
        if (m_bIsHeldenianMode)
        {
            if (memcmp(m_cMapName, "BtField", 7) == 0)
            {
                format_to_local(G_cTxt, "Aresden death toll : {}", m_iAresdenDeathToll);
                put_string(10, a, G_cTxt, Color(255, 255, 255));
                a += 12;
                format_to_local(G_cTxt, "Elvine death toll : {}", m_iElvineDeathToll);
                put_string(10, a, G_cTxt, Color(255, 255, 255));
                a += 12;
                format_to_local(G_cTxt, "Aresden's left building number : {}", m_iAresdenLeftBuildings);
                put_string(10, a, G_cTxt, Color(255, 255, 255));
                a += 12;
                format_to_local(G_cTxt, "Elvine's left building number : {}", m_iElvineLeftBuildings);
                put_string(10, a, G_cTxt, Color(255, 255, 255));
                a += 12;
            }
        }

        //0x010A
        put_string_sprite_font4(225, 400, m_cGMList, 0, 255, 0);
        if ((m_iTotalPartyMember > 1) && (m_stPartyConfig.bEnabled))
        {//Change Party list 0x0106
            put_string_sprite_font4(5, 17, "Party:", 230, 230, 40);
            int icount = 0;
            for (int i = 0; i < m_iTotalPartyMember; i++)
            {
                if (memcmp(m_stPartyMemberNameList[i].cName, m_cPlayerName, 12) != 0)
                {
                    if (m_stPartyConfig.cType == 0)
                    {
                        format_to_local(G_cTxt, "{} - {}", m_stPartyMemberNameList[i].cName, m_stPartyMemberNameList[i].iLevel);
                        put_string_sprite_font4(5, 30 + (icount * 39) + 0, G_cTxt, 230, 100, 0);
                        if (m_stPartyConfig.bShowPercent)
                            format_to_local(G_cTxt, "HP: {}/{} - {}%%", m_stPartyMemberNameList[i].iHP, m_stPartyMemberNameList[i].iMaxHP, int(((float)m_stPartyMemberNameList[i].iHP / (float)m_stPartyMemberNameList[i].iMaxHP) * 100));
                        else
                            format_to_local(G_cTxt, "HP: {}/{}", m_stPartyMemberNameList[i].iHP, m_stPartyMemberNameList[i].iMaxHP);
                        put_string_sprite_font4(5, 30 + (icount * 39) + 13, G_cTxt, 255, 50, 50);
                        if (m_stPartyConfig.bShowPercent)
                            format_to_local(G_cTxt, "MP: {}/{} - {}%%", m_stPartyMemberNameList[i].iMP, m_stPartyMemberNameList[i].iMaxMP, int(((float)m_stPartyMemberNameList[i].iMP / (float)m_stPartyMemberNameList[i].iMaxMP) * 100));
                        else
                            format_to_local(G_cTxt, "MP: {}/{}", m_stPartyMemberNameList[i].iMP, m_stPartyMemberNameList[i].iMaxMP);
                        put_string_sprite_font4(5, 30 + (icount * 39) + 26, G_cTxt, 30, 160, 160);
                    }
                    else if (m_stPartyConfig.cType == 1)
                    {
                        format_to_local(G_cTxt, "{} - {}", m_stPartyMemberNameList[i].cName, m_stPartyMemberNameList[i].iLevel);
                        put_string_sprite_font4(5, 30 + (icount * 39) + 0, G_cTxt, 230, 100, 0);
                        if (m_stPartyConfig.bShowPercent)
                            format_to_local(G_cTxt, "HP: {}/{} {}%% - MP: {}/{} {}%%", m_stPartyMemberNameList[i].iHP, m_stPartyMemberNameList[i].iMaxHP, int(((float)m_stPartyMemberNameList[i].iHP / (float)m_stPartyMemberNameList[i].iMaxHP) * 100), m_stPartyMemberNameList[i].iMP, m_stPartyMemberNameList[i].iMaxMP, int(((float)m_stPartyMemberNameList[i].iMP / (float)m_stPartyMemberNameList[i].iMaxMP) * 100));
                        else
                            format_to_local(G_cTxt, "HP: {}/{} - MP: {}/{}", m_stPartyMemberNameList[i].iHP, m_stPartyMemberNameList[i].iMaxHP, m_stPartyMemberNameList[i].iMP, m_stPartyMemberNameList[i].iMaxMP);
                        put_string_sprite_font4(5, 30 + (icount * 39) + 13, G_cTxt, 255, 50, 50);
                    }
                    icount++;
                }
            }
        }
    }

    if ((effect_frame_counter() == true) && (iUpdateRet == 0)) iUpdateRet = -1;

    if (iUpdateRet == 2)
    {
        m_bCommandAvailable = true;
        m_dwCommandTime = 0;
    }

    CommandProcessor(msX, msY, ((sDivX + sPivotX) * 32 + sModX + msX - 17) / 32 + 1, ((sDivY + sPivotY) * 32 + sModY + msY - 17) / 32 + 1, LB, RB);
    m_sViewPointX = sVPXsave;
    m_sViewPointY = sVPYsave;
    if (iUpdateRet > 0) CalcViewPoint();

    if (m_bIsObserverMode)
    {
        if ((dwTime - m_dwObserverCamTime) > 25)
        {
            m_dwObserverCamTime = dwTime;
            CalcViewPoint();
            iUpdateRet = -1;
        }
    }

    if (iUpdateRet > 0)
    {
        if (m_bDrawFlagDir == false)
        {
            m_iDrawFlag++;
            if (m_iDrawFlag >= 25)
            {
                m_iDrawFlag = 25;
                m_bDrawFlagDir = true;
            }
        }
        else
        {
            m_iDrawFlag--;
            if (m_iDrawFlag < 0)
            {
                m_iDrawFlag = 0;
                m_bDrawFlagDir = false;
            }
        }
    }

    /*
    0000:0046443A     loc_46443A:                             ; CODE XREF: UpdateScreen_OnGame+1F27j
    0000:0046443A                                             ; UpdateScreen_OnGame+1F2Fj ...
    0000:0046443A 11C                 mov     edx, 246h
    0000:0046443F
    0000:0046443F     loc_46443F:                             ; CODE XREF: UpdateScreen_OnGame+1FA0j
    0000:0046443F 11C                 mov     eax, edx
    0000:00464441 11C                 mov     ecx, 10h
    0000:00464446
    0000:00464446     loc_464446:                             ; CODE XREF: UpdateScreen_OnGame+1F95j
    0000:00464446 11C                 mov     edi, [ebp+6EA9Ch]
    0000:0046444C 11C                 add     eax, 2          ; Add
    0000:0046444F 11C                 dec     ecx             ; Decrement by 1
    0000:00464450 11C                 mov     [eax+edi-2], si
    0000:00464455 11C                 jnz     short loc_464446 ; Jump if Not Zero (ZF=0)
    0000:00464455
    0000:00464457 11C                 add     edx, 46h        ; Add
    0000:0046445A 11C                 cmp     edx, 84Ah       ; Compare Two Operands
    0000:00464460 11C                 jl      short loc_46443F ; Jump if Less (SF!=OF)
    0000:00464460
    0000:00464462 11C                 mov     ecx, 13Dh
    0000:00464467 11C                 mov     [esp+11Ch+var_F4], 9988h
    0000:0046446F 11C                 mov     ebx, 42h
    0000:00464474
    0000:00464474     loc_464474:                             ; CODE XREF: UpdateScreen_OnGame+21E8j
    0000:00464474 11C                 mov     edi, [esp+11Ch+var_F4]
    0000:00464478 11C                 mov     eax, 9
    0000:0046447D
    0000:0046447D     loc_46447D:                             ; CODE XREF: UpdateScreen_OnGame+21CDj
    0000:0046447D 11C                 mov     edx, [ebp+6EA9Ch]
    0000:00464483 11C                 mov     si, [edi+edx+0B2Ch]
    0000:0046448B 11C                 cmp     si, bx          ; Compare Two Operands
    0000:0046448E 11C                 jnz     loc_464528      ; Jump if Not Zero (ZF=0)
    0000:0046448E
    0000:00464494 11C                 cmp     word ptr [edi+edx+0B2Ah], 0 ; Compare Two Operands
    0000:0046449D 11C                 jz      loc_464528      ; Jump if Zero (ZF=1)
    0000:0046449D
    0000:004644A3 11C                 lea     esi, [ecx+eax]  ; Load Effective Address
    0000:004644A6 11C                 mov     [edx+esi*2-46h], bx
    0000:004644AB 11C                 mov     esi, [ebp+6EA9Ch]
    0000:004644B1 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:004644B4 11C                 mov     [esi+edx*2], bx
    0000:004644B8 11C                 mov     esi, [ebp+6EA9Ch]
    0000:004644BE 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:004644C1 11C                 mov     [esi+edx*2-44h], bx
    0000:004644C6 11C                 mov     esi, [ebp+6EA9Ch]
    0000:004644CC 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:004644CF 11C                 mov     [esi+edx*2+2], bx
    0000:004644D4 11C                 mov     esi, [ebp+6EA9Ch]
    0000:004644DA 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:004644DD 11C                 mov     [esi+edx*2-8Ch], bx
    0000:004644E5 11C                 mov     esi, [ebp+6EA9Ch]
    0000:004644EB 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:004644EE 11C                 mov     [esi+edx*2-48h], bx
    0000:004644F3 11C                 mov     esi, [ebp+6EA9Ch]
    0000:004644F9 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:004644FC 11C                 mov     [esi+edx*2-8Eh], bx
    0000:00464504 11C                 mov     esi, [ebp+6EA9Ch]
    0000:0046450A 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:0046450D 11C                 mov     [esi+edx*2-2], bx
    0000:00464512 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464518 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:0046451B 11C                 mov     [esi+edx*2-8Ah], bx
    0000:00464523 11C                 jmp     loc_464683      ; Jump
    0000:00464523
    0000:00464528     ; ---------------------------------------------------------------------------
    0000:00464528
    0000:00464528     loc_464528:                             ; CODE XREF: UpdateScreen_OnGame+1FCEj
    0000:00464528                                             ; UpdateScreen_OnGame+1FDDj
    0000:00464528 11C                 cmp     si, 49h         ; Compare Two Operands
    0000:0046452C 11C                 jnz     loc_4645D8      ; Jump if Not Zero (ZF=0)
    0000:0046452C
    0000:00464532 11C                 cmp     word ptr [edi+edx+0B2Ah], 0 ; Compare Two Operands
    0000:0046453B 11C                 jz      loc_4645D8      ; Jump if Zero (ZF=1)
    0000:0046453B
    0000:00464541 11C                 lea     esi, [ecx+eax]  ; Load Effective Address
    0000:00464544 11C                 mov     word ptr [edx+esi*2-46h], 49h
    0000:0046454B 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464551 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464554 11C                 mov     word ptr [esi+edx*2], 49h
    0000:0046455A 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464560 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464563 11C                 mov     word ptr [esi+edx*2-44h], 49h
    0000:0046456A 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464570 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464573 11C                 mov     word ptr [esi+edx*2+2], 49h
    0000:0046457A 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464580 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464583 11C                 mov     word ptr [esi+edx*2-8Ch], 49h
    0000:0046458D 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464593 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464596 11C                 mov     word ptr [esi+edx*2-48h], 49h
    0000:0046459D 11C                 mov     esi, [ebp+6EA9Ch]
    0000:004645A3 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:004645A6 11C                 mov     word ptr [esi+edx*2-8Eh], 49h
    0000:004645B0 11C                 mov     esi, [ebp+6EA9Ch]
    0000:004645B6 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:004645B9 11C                 mov     word ptr [esi+edx*2-2], 49h
    0000:004645C0 11C                 mov     esi, [ebp+6EA9Ch]
    0000:004645C6 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:004645C9 11C                 mov     word ptr [esi+edx*2-8Ah], 49h
    0000:004645D3 11C                 jmp     loc_464683      ; Jump
    0000:004645D3
    0000:004645D8     ; ---------------------------------------------------------------------------
    0000:004645D8
    0000:004645D8     loc_4645D8:                             ; CODE XREF: UpdateScreen_OnGame+206Cj
    0000:004645D8                                             ; UpdateScreen_OnGame+207Bj
    0000:004645D8 11C                 cmp     si, 51h         ; Compare Two Operands
    0000:004645DC 11C                 jnz     loc_464683      ; Jump if Not Zero (ZF=0)
    0000:004645DC
    0000:004645E2 11C                 cmp     word ptr [edi+edx+0B2Ah], 0 ; Compare Two Operands
    0000:004645EB 11C                 jz      loc_464683      ; Jump if Zero (ZF=1)
    0000:004645EB
    0000:004645F1 11C                 lea     esi, [ecx+eax]  ; Load Effective Address
    0000:004645F4 11C                 mov     word ptr [edx+esi*2-46h], 51h
    0000:004645FB 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464601 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464604 11C                 mov     word ptr [esi+edx*2], 51h
    0000:0046460A 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464610 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464613 11C                 mov     word ptr [esi+edx*2-44h], 51h
    0000:0046461A 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464620 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464623 11C                 mov     word ptr [esi+edx*2+2], 51h
    0000:0046462A 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464630 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464633 11C                 mov     word ptr [esi+edx*2-8Ch], 51h
    0000:0046463D 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464643 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464646 11C                 mov     word ptr [esi+edx*2-48h], 51h
    0000:0046464D 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464653 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464656 11C                 mov     word ptr [esi+edx*2-8Eh], 51h
    0000:00464660 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464666 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464669 11C                 mov     word ptr [esi+edx*2-2], 51h
    0000:00464670 11C                 mov     esi, [ebp+6EA9Ch]
    0000:00464676 11C                 lea     edx, [ecx+eax]  ; Load Effective Address
    0000:00464679 11C                 mov     word ptr [esi+edx*2-8Ah], 51h
    0000:00464683
    0000:00464683     loc_464683:                             ; CODE XREF: UpdateScreen_OnGame+2063j
    0000:00464683                                             ; UpdateScreen_OnGame+2113j ...
    0000:00464683 11C                 inc     eax             ; Increment by 1
    0000:00464684 11C                 add     edi, 88h        ; Add
    0000:0046468A 11C                 cmp     eax, 19h        ; Compare Two Operands
    0000:0046468D 11C                 jl      loc_46447D      ; Jump if Less (SF!=OF)
    0000:0046468D
    0000:00464693 11C                 mov     eax, [esp+11Ch+var_F4]
    0000:00464697 11C                 add     ecx, 23h        ; Add
    0000:0046469A 11C                 add     eax, 1298h      ; Add
    0000:0046469F 11C                 cmp     eax, 23298h     ; Compare Two Operands
    0000:004646A4 11C                 mov     [esp+11Ch+var_F4], eax
    0000:004646A8 11C                 jl      loc_464474      ; Jump if Less (SF!=OF)
    0000:004646A8
    0000:004646AE
    0000:004646AE     loc_4646AE:                             ; CODE XREF: UpdateScreen_OnGame+1F4j
    0000:004646AE                                             ; UpdateScreen_OnGame+371j ...
    0000:004646AE 11C                 pop     edi
    0000:004646AF 118                 pop     esi
    0000:004646B0 114                 pop     ebp
    0000:004646B1 110                 pop     ebx
    0000:004646B2 10C                 add     esp, 10Ch       ; Add
    0000:004646B8 000                 retn                    ; Return Near from Procedure

    */

    // Auto Pot Chugger
#ifdef DEF_HACKCLIENT
    if ((dwTime - m_dwPotCheckTime) > 200)
    {
        m_dwPotCheckTime = unixtime();
        if (m_iHP <= 0) return;
        int iMaxPoint = m_iVit * 3 + m_iLevel * 2 + m_iStr / 2;
        int i;
        if (m_iHP < (iMaxPoint * 0.75))
        {
            for (i = 0; i < DEF_MAXITEMS; i++)
            {
                if ((m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
                    (m_pItemList[i]->m_sSprite == 6) && ((m_pItemList[i]->m_sSpriteFrame == 1) || (m_pItemList[i]->m_sSpriteFrame == 2)))
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);
                    m_bIsItemDisabled[i] = true;
                    return;
                }
            }
        }

        iMaxPoint = m_iMag * 2 + m_iLevel * 2 + m_iInt / 2;
        if (m_iMP < (iMaxPoint / 2))
        {
            for (i = 0; i < DEF_MAXITEMS; i++)
            {
                if ((m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
                    (m_pItemList[i]->m_sSprite == 6) && ((m_pItemList[i]->m_sSpriteFrame == 3) || (m_pItemList[i]->m_sSpriteFrame == 4)))
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);
                    m_bIsItemDisabled[i] = true;
                    return;
                }
            }
        }
        iMaxPoint = m_iStr * 2 + m_iLevel * 2;
        if (m_iSP < (iMaxPoint / 2))
        {
            for (i = 0; i < DEF_MAXITEMS; i++)
            {
                if ((m_pItemList[i] != 0) && (m_bIsItemDisabled[i] != true) &&
                    (m_pItemList[i]->m_sSprite == 6) && ((m_pItemList[i]->m_sSpriteFrame == 5) || (m_pItemList[i]->m_sSpriteFrame == 6)))
                {
                    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, i, 0, 0, 0);
                    m_bIsItemDisabled[i] = true;
                    return;
                }
            }
        }
    }
#endif
}

void CGame::_Draw_OnLogin(char * pAccount, char * pPassword, int msX, int msY, int iFrame)
{
    bool bFlag = true;
    uint32_t dwTime = unixtime();

    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_LOGIN, modx, mody, 0, true);
    DrawVersion();

    //if ((iFrame >= 15) && (iFrame <= 20)) m_pSprite[DEF_SPRID_INTERFACE_ND_LOGIN]->put_trans_sprite25(39,121, 1, true);
    //else if (iFrame > 20) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_LOGIN, 39,121, 1, true);
    /*if ((iFrame >= 15) && (iFrame <= 20)) m_pSprite[DEF_SPRID_INTERFACE_ND_LOGIN]->put_trans_sprite25(191,188, 1, true);
    else if (iFrame > 20) */DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_LOGIN, modx + 191, mody + 188, 1, true);

    if (m_cCurFocus != 1)
    {
        if (m_Misc.bCheckValidName(pAccount) != false)
            put_string2(modx + 258, mody + 273, pAccount, 200, 200, 200);
        else put_string2(modx + 258, mody + 273, pAccount, 200, 100, 100);
    }
    if ((m_Misc.bCheckValidName(pAccount) == false) || (strlen(pAccount) == 0)) bFlag = false;

    if (m_cCurFocus != 2)
    {
        if ((m_Misc.bCheckValidString(pPassword) != false))
            put_string(modx + 258, mody + 330, pPassword, Color(200, 200, 200), true, 1);
        else put_string(modx + 258, mody + 330, pPassword, Color(200, 100, 100), true, 1);
    }
    if ((m_Misc.bCheckValidString(pPassword) == false) || (strlen(pPassword) == 0)) bFlag = false;

    if (m_cCurFocus == 1)
        ShowReceivedString();
    else
        if (m_cCurFocus == 2)
            ShowReceivedString(true);

    if (bFlag == true)
    {
        if (m_cCurFocus == 3) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_LOGIN, 209, 352, 3, true);
    }
    if (m_cCurFocus == 4) DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_LOGIN, 321, 352, 4, true);
}

void CGame::_Draw_UpdateScreen_OnCreateNewAccount()
{
    DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_NEWACCOUNT, modx, mody, 0, true);
    put_string2(modx + 329, mody + 110, m_cAccountName, 200, 200, 200);
    put_string(modx + 329, mody + 125, m_cAccountPassword, Color(200, 200, 200), true, 1);
    put_string(modx + 329, mody + 140, m_cAccountPassword, Color(200, 200, 200), true, 1);
    put_string2(modx + 300, mody + 202, m_cAccountCountry, 200, 200, 200);
    put_string2(modx + 300, mody + 218, m_cAccountSSN, 200, 200, 200);
    put_string2(modx + 194, mody + 257, m_cEmailAddr, 200, 200, 200);
}
void CGame::UpdateScreen_OnSelectCharacter()
{
    uint16_t sX{}, sY{}, msX{}, msY{}, msZ{};
    char  cLB{}, cRB{}, cTotalChar{};
    char  cMIresult{};
    static CMouseInterface * pMI;
    uint32_t dwTime{};
    static uint32_t dwCTime;

    int iMIbuttonNum;

    dwTime = unixtime();
    sX = 0;
    sY = 0;
    cTotalChar = 0;

    if (m_cGameModeCount == 0)
    {
        G_cSpriteAlphaDegree = 1;

        InitGameSettings();

        pMI = new CMouseInterface;
        pMI->AddRect(100 + modx, 50 + mody, 210 + modx, 250 + mody);
        pMI->AddRect(211 + modx, 50 + mody, 321 + modx, 250 + mody);
        pMI->AddRect(322 + modx, 50 + mody, 431 + modx, 250 + mody);
        pMI->AddRect(432 + modx, 50 + mody, 542 + modx, 250 + mody);

        pMI->AddRect(360 + modx, 283 + mody, 545 + modx, 315 + mody);
        pMI->AddRect(360 + modx, 316 + mody, 545 + modx, 345 + mody);
        pMI->AddRect(360 + modx, 346 + mody, 545 + modx, 375 + mody);
        pMI->AddRect(360 + modx, 376 + mody, 545 + modx, 405 + mody);
        pMI->AddRect(360 + modx, 406 + mody, 545 + modx, 435 + mody);

        m_cMaxFocus = 4;
        if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
        if (m_cCurFocus < 1)		   m_cCurFocus = 1;

        m_cArrowPressed = 0;
        m_bEnterPressed = false;

        dwCTime = unixtime();
    }

    m_cGameModeCount++;
    if (m_cGameModeCount > 100) m_cGameModeCount = 100;

    if (m_cArrowPressed != 0)
    {
        switch (m_cArrowPressed)
        {
            case 2:
                m_cCurFocus++;
                if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                break;
            case 4:
                m_cCurFocus--;
                if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
                break;
        }
        m_cArrowPressed = 0;
    }

    if (m_bEscPressed == true)
    {
        ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
        delete pMI;
        m_bEscPressed = false;
        return;
    }

    if (m_bEnterPressed == true)
    {
        m_bEnterPressed = false;
        PlaySound('E', 14, 5);

        if (m_pCharList[m_cCurFocus - 1] != 0)
        {
            if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
            {
                memset(m_cPlayerName, 0, sizeof(m_cPlayerName));
                strcpy(m_cPlayerName, m_pCharList[m_cCurFocus - 1]->m_cName);

                m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;

                if (m_Misc.bCheckValidString(m_cPlayerName) == true)
                {
                    m_pSprite[DEF_SPRID_INTERFACE_ND_LOGIN]->close_sprite_();
                    m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU]->close_sprite_();

                    ChangeGameMode(DEF_GAMEMODE_ONCONNECTING);
                    m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                    m_wEnterGameType = DEF_ENTERGAMEMSGTYPE_NEW;
                    memset(m_cMsg, 0, sizeof(m_cMsg));
                    strcpy(m_cMsg, "33");

                    memset(m_cMapName, 0, sizeof(m_cMapName));
                    memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName, 10);

                    ConnectionEstablishHandler(DEF_SERVERTYPE_LOG);
                    delete pMI;
                    return;
                }
            }
            else
            {
            }
        }
        else
        {
            _InitOnCreateNewCharacter();
            ChangeGameMode(DEF_GAMEMODE_ONCREATENEWCHARACTER);
            delete pMI;
            return;
        }
    }

    update_mouse_state(msX, msY, msZ, cLB, cRB);
    UpdateScreen_OnSelectCharacter(modx + sX, mody + sY, msX, msY);

    if ((dwTime - dwCTime) > 100)
    {
        m_cMenuFrame++;
        dwCTime = dwTime;
    }
    if (m_cMenuFrame >= 8)
    {
        m_cMenuDirCnt++;
        if (m_cMenuDirCnt > 8)
        {
            m_cMenuDir++;
            m_cMenuDirCnt = 1;
        }
        m_cMenuFrame = 0;
    }
    if (m_cMenuDir > 8) m_cMenuDir = 1;

    DrawVersion();
    m_pSprite[DEF_SPRID_MOUSECURSOR]->put_sprite_fast(msX, msY, 0, dwTime);

    iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
    if (cMIresult == DEF_MIRESULT_CLICK)
    {
        PlaySound('E', 14, 5);

        switch (iMIbuttonNum)
        {
            case 1:
            case 2:
            case 3:
            case 4:
                if (m_cCurFocus != iMIbuttonNum)
                    m_cCurFocus = iMIbuttonNum;
                else
                {
                    if (m_pCharList[m_cCurFocus - 1] != 0)
                    {
                        if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
                        {
                            memset(m_cPlayerName, 0, sizeof(m_cPlayerName));
                            strcpy(m_cPlayerName, m_pCharList[m_cCurFocus - 1]->m_cName);
                            m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;

                            if (m_Misc.bCheckValidString(m_cPlayerName) == true)
                            {
                                m_pSprite[DEF_SPRID_INTERFACE_ND_LOGIN]->close_sprite_();
                                m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU]->close_sprite_();

                                ChangeGameMode(DEF_GAMEMODE_ONCONNECTING);
                                m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                                m_wEnterGameType = DEF_ENTERGAMEMSGTYPE_NEW;
                                memset(m_cMsg, 0, sizeof(m_cMsg));
                                strcpy(m_cMsg, "33");

                                memset(m_cMapName, 0, sizeof(m_cMapName));
                                memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName, 10);

                                ConnectionEstablishHandler(DEF_SERVERTYPE_LOG);
                                delete pMI;
                                return;
                            }
                        }
                        else
                        {
                        }
                    }
                    else
                    {
                        _InitOnCreateNewCharacter();
                        ChangeGameMode(DEF_GAMEMODE_ONCREATENEWCHARACTER);
                        delete pMI;
                        return;
                    }
                }
                break;

            case 5:
                if (m_pCharList[m_cCurFocus - 1] != 0)
                {
                    if (m_pCharList[m_cCurFocus - 1]->m_sSex != 0)
                    {
                        memset(m_cPlayerName, 0, sizeof(m_cPlayerName));
                        strcpy(m_cPlayerName, m_pCharList[m_cCurFocus - 1]->m_cName);
                        m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;

                        if (m_Misc.bCheckValidString(m_cPlayerName) == true)
                        {
                            m_pSprite[DEF_SPRID_INTERFACE_ND_LOGIN]->close_sprite_();
                            m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU]->close_sprite_();

                            ChangeGameMode(DEF_GAMEMODE_ONCONNECTING);
                            m_dwConnectMode = MSGID_REQUEST_ENTERGAME;
                            m_wEnterGameType = DEF_ENTERGAMEMSGTYPE_NEW;
                            memset(m_cMsg, 0, sizeof(m_cMsg));
                            strcpy(m_cMsg, "33");

                            memset(m_cMapName, 0, sizeof(m_cMapName));
                            memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName, 10);

                            ConnectionEstablishHandler(DEF_SERVERTYPE_LOG);
                            delete pMI;
                            return;
                        }
                    }
                    else
                    {
                    }
                }
                break;

            case 6:
                if (m_iTotalChar < 4)
                {
                    //
                    _InitOnCreateNewCharacter();
                    ChangeGameMode(DEF_GAMEMODE_ONCREATENEWCHARACTER);
                    delete pMI;
                    return;
                }
                break;

            case 7:
                if ((m_pCharList[m_cCurFocus - 1] != 0)/* && (m_pCharList[m_cCurFocus - 1]->m_sLevel < 50)*/)
                {
                    //
                    ChangeGameMode(DEF_GAMEMODE_ONQUERYDELETECHARACTER);
                    m_wEnterGameType = m_cCurFocus;
                    delete pMI;
                    return;
                }
                break;

            case 8:
                return;

            case 9:

                ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
                delete pMI;
                return;
        }
    }

    if (m_cGameModeCount < 6) draw_shadow_box(modx, mody, get_virtual_width() - 1, get_virtual_height() - 1);
    if (m_cGameModeCount < 2) draw_shadow_box(modx, mody, get_virtual_width() - 1, get_virtual_height() - 1);
}
