//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include <cstdio>
#include <cstdlib>
#include <process.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <sys/timeb.h>
#include "Game.h"
#include "global_def.h"
#include <condition_variable>
#include <filesystem>

#include <intrin.h>
#include <fmt/format.h>
#include <ixwebsocket/IXWebSocket.h>

#include "sprite.h"
#include "sprite_id.h"

char G_cSpriteAlphaDegree;
CGame * game;

bool isrunning = true;
bool is_ui_running = true;
bool restart_ui = false;

std::condition_variable cv;
std::condition_variable cv2;

int64_t unixtime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

int32_t unixseconds()
{
    return (int32_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

bool isvm()
{
    unsigned int cpuInfo[4] = {};
    __cpuid((int *)cpuInfo, 1);
    return ((cpuInfo[2] >> 31) & 1) == 1;
}

constexpr uint64_t BUFSIZE = 2048;
#define UNLIMITED_CLIENTS
#define CLI_PARAMS

int main(int argc, char * argv[])
{
    srand((unsigned)time(nullptr));

    std::filesystem::current_path("../");

    game = new CGame();

#if !defined(_DEBUG) && !defined(UNLIMITED_CLIENTS)
    game->account_name = received_login.substr(0, pos);
    game->password = received_login.substr(pos + 1);
    game->autologin = true;
#endif

    game->_renderer = "OpenGL";

    game->inside_vm = isvm();


    if (argc >= 2)
    {
        for (int i = 0; i < argc; ++i)
        {
#if defined(_DEBUG) || defined(CLI_PARAMS)
            //debug only stuff
            if (memcmp(argv[i], "-res=", 5) == 0)
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=x", &ctx);
                game->screenwidth = atoi(strtok_s(nullptr, "=x", &ctx));
                game->screenheight = atoi(strtok_s(nullptr, "=x", &ctx));
                game->autoresolution = true;
            }
            else if (memcmp(argv[i], "-vres=", 6) == 0)
            {
                char * ctx;
                char * c = strtok_s(argv[i], "=x", &ctx);
                game->screenwidth_v = atoi(strtok_s(nullptr, "=x", &ctx));
                game->screenheight_v = atoi(strtok_s(nullptr, "=x", &ctx));
                game->autovresolution = true;
            }
#endif
        }
    }

#if defined(_DEBUG) || defined(HBX_CONFIG)
    std::ifstream config_file("config.json");
    json cfg;
    config_file >> cfg;

    auto read_property = [](json & cfg, const std::string & key, auto default_) -> auto
        {
            if (!cfg[key].is_null())
                return cfg[key].get<decltype(default_)>();
            return default_;
        };

    game->account_name = read_property(cfg, "username", std::string(""));
    game->password = read_property(cfg, "password", std::string(""));

    if (!game->account_name.empty() && !game->password.empty())
    {
        game->autologin = true;
    }

    game->set_resolution(read_property(cfg, "xresolution", 800), read_property(cfg, "yresolution", 600));
    game->set_virtual_resolution(read_property(cfg, "vxresolution", 800), read_property(cfg, "vyresolution", 600));
    game->dev_mode.wss = read_property(cfg, "wss", true);
    game->login_address = read_property(cfg, "login_address", std::string(""));
    game->login_port = read_property(cfg, "login_port", 80);
    game->core_usage = read_property(cfg, "core_usage", 0);

    game->core_count = std::thread::hardware_concurrency();

    if (game->core_usage == 0 || game->core_usage > game->core_count) game->core_usage = game->core_count;

#endif

    if (!game->create_renderer())
    {
        return 0;
    }

    if (game->bInit() == false)
    {
        return false;
    }

    game->window.setMouseCursorGrabbed(game->clipmousegame);
    game->window.setMouseCursorVisible(false);

    // Load interface sprites first to show sprites during loading
    game->m_pSprite[DEF_SPRID_MOUSECURSOR] = sprite::create_sprite("interface", 0, false);
    game->m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS] = sprite::create_sprite("interface", 1, false);
    game->m_pSprite[DEF_SPRID_INTERFACE_ND_LOGIN] = sprite::create_sprite("LoginDialog", 0, false);
    game->m_pSprite[DEF_SPRID_INTERFACE_ND_MAINMENU] = sprite::create_sprite("New-Dialog", 1, false);
    game->m_pSprite[DEF_SPRID_INTERFACE_ND_QUIT] = sprite::create_sprite("New-Dialog", 2, false);

    sf::Event evt;
    sf::RenderWindow & window = game->window;

    while (window.isOpen() && isrunning)
    {
        //timers first
        game->OnTimer();
        game->fps_counter.update();

        while (window.pollEvent(evt))
        {

            if (evt.type == sf::Event::Closed)
            {
                isrunning = false;
                window.close();
                break;
            }

            game->on_input_event(evt);
        }

        {
            std::unique_lock<std::mutex> l(game->screenupdate);
            window.clear(sf::Color::Black);

            game->UpdateScreen();

            window.display();
        }
    }
    isrunning = false;

    game->Quit();
    if (window.isOpen())
        window.close();

    game->ws->stop();

    delete game;

#if !defined(_DEBUG) && !defined(UNLIMITED_CLIENTS)
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
#endif

    return 0;
}
