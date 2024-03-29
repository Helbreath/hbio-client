//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <algorithm>
#include <iostream>
#include <fmt/format.h>
#include <SFML/Window/ContextSettings.hpp>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include "SpriteID.h"
#include "sprite.h"

std::shared_ptr<CGame::msg_queue_entry> CGame::get_login_msg_queue()
{
    std::shared_ptr<CGame::msg_queue_entry> msg = loginpipe.front();
    loginpipe.pop_front();
    return msg;
}

void CGame::on_message(const ix::WebSocketMessagePtr & msg)
{
    if (msg->type == ix::WebSocketMessageType::Message)
    {
        auto message = std::make_shared<CGame::msg_queue_entry>();
        message->data = new char[msg->str.length()];
        memcpy(message->data, msg->str.c_str(), msg->str.length());
        message->size = msg->str.length();
        std::lock_guard<std::mutex> l(socket_mut);
        put_msg_queue(message, loginpipe);
    }
    else if (msg->type == ix::WebSocketMessageType::Open)
    {
        std::lock_guard<std::recursive_mutex> l(connection_mut);
        loggedin = false;
        newconnection = true;
    }
    else if (msg->type == ix::WebSocketMessageType::Error)
    {
        std::lock_guard<std::recursive_mutex> l(connection_mut);
        disconnecting = true;
        std::cout << "ws error\n" << msg->errorInfo.reason.c_str() << '\n';
        connection_loss_gamemode();
        socket_mode(0);
        loggedin = false;
    }
    else if (msg->type == ix::WebSocketMessageType::Close)
    {
        std::lock_guard<std::recursive_mutex> l(connection_mut);
        disconnecting = true;
        std::cout << "ws close\n";
        connection_loss_gamemode();
        socket_mode(0);
        loggedin = false;
    }
}

bool CGame::is_connected() const
{
    return ws && ws->getReadyState() == ix::ReadyState::Open;
}

bool CGame::is_closed() const
{
    return !ws || ws->getReadyState() == ix::ReadyState::Closed;
}

bool CGame::is_connecting() const
{
    return ws && ws->getReadyState() == ix::ReadyState::Connecting;
}

bool CGame::is_closing() const
{
    return ws && ws->getReadyState() == ix::ReadyState::Closing;
}

void CGame::put_msg_queue(msg_queue & q, char * data, std::size_t size)
{
    //poco_information(*logger, "PutMsgQueue()");
    std::shared_ptr<msg_queue_entry> msg = std::make_shared<msg_queue_entry>();

    msg->data = new char[size];
    memcpy(msg->data, data, size);
    msg->size = size;

    q.push_back(msg);
}

void CGame::put_msg_queue(std::shared_ptr<CGame::msg_queue_entry> msg, msg_queue & q)
{
    q.push_back(msg);
}

std::shared_ptr<CGame::msg_queue_entry> CGame::get_msg_queue()
{
    std::shared_ptr<CGame::msg_queue_entry> msg = socketpipe.front();
    socketpipe.pop_front();
    return msg;
}

void CGame::perform_connect()
{
    std::lock_guard<std::recursive_mutex> l(connection_mut);
    if (ws)
    {
        ws->close();
        ws.reset();
    }

    ws = std::make_unique<ix::WebSocket>();

    ws->setOnMessageCallback(std::bind(&CGame::on_message, this, std::placeholders::_1));
    //ws->setUrl(fmt::format("wss://{}:8443", SERVER_IP));
    ws->setUrl(std::format("ws://{}:80", SERVER_IP));
    ws->disableAutomaticReconnection();

    ws->start();
}

int32_t CGame::write(const char * data, const std::size_t size)
{
    //std::cout << "Sent " << size << " cbytes of data\n";
    auto d = ix::IXWebSocketSendData{ data, size };
    if (is_connected()) return (int32_t)ws->sendBinary(d).payloadSize;
    return -129; // DEF_XSOCKEVENT_SOCKETERROR
}

int32_t CGame::write(stream_write & sw)
{
    //std::cout << "Sent " << sw.position << " bytes of data\n";
    auto d = ix::IXWebSocketSendData{ sw.data, sw.position };
    if (is_connected()) return (int32_t)ws->sendBinary(d).payloadSize;
    return -129; // DEF_XSOCKEVENT_SOCKETERROR
}

int32_t CGame::write(nlohmann::json & obj)
{
    return (int32_t)ws->send(obj.dump(-1, 0x32, false, nlohmann::detail::error_handler_t::ignore)).payloadSize;
    //return -129; // DEF_XSOCKEVENT_SOCKETERROR
}

void CGame::connection_loss_gamemode()
{
    switch (m_cGameMode)
    {
        case DEF_GAMEMODE_ONCONNECTING:
        case DEF_GAMEMODE_ONMAINGAME:
        case DEF_GAMEMODE_ONLOGIN:
        case DEF_GAMEMODE_ONSELECTCHARACTER:
        case DEF_GAMEMODE_ONLOGRESMSG:
            // play mode. connection stop here would typically indicate a disconnect
            change_game_mode(DEF_GAMEMODE_ONCONNECTIONLOST);
            break;
        case DEF_GAMEMODE_ONMAINMENU:
            break;
        default:
            change_game_mode(DEF_GAMEMODE_ONMAINMENU);
            break;
    }
}

void CGame::handle_stop()
{
    close(1000, "handle_stop()");
}

void CGame::close(uint32_t code, const std::string & reason)
{
    if (!ws) return;
    try
    {
        std::lock_guard<std::recursive_mutex> l(connection_mut);

        ws->close(code, "close()");
    }
    catch (std::system_error &)
    {
        std::cout << "close() mutex lock timeout?\n";
    }
}

void CGame::on_input_event(sf::Event event)
{
    //TODO: fix

    if (m_bInputStatus && event.type == sf::Event::TextEntered)
    {
        //rethink this
//         if (
//             event.key.code != Keyboard::Backspace
//             && event.key.code != Keyboard::Enter
//             && event.key.code != Keyboard::Escape
//             && event.key.code != Keyboard::Tab
// //             && event.key.code != Keyboard::LShift
// //             && event.key.code != Keyboard::RShift
// //             && event.key.code != Keyboard::LSystem
// //             && event.key.code != Keyboard::RSystem
// //             && event.key.code != Keyboard::Menu
// //             && event.key.code != Keyboard::LControl
// //             && event.key.code != Keyboard::RControl
// //             && event.key.code != Keyboard::LAlt
// //             && event.key.code != Keyboard::RAlt
//             )
        if (event.text.unicode == 8)
        {

        }
        else if (
            event.text.unicode != 9
            && event.text.unicode != 4
            && event.text.unicode != 13
            && event.text.unicode != 27
            )
        {
            std::string s{ (char)(((int32_t)event.text.unicode) & 0xff) };

            uint32_t len = (uint32_t)strlen(m_pInputBuffer);
            if (len >= m_inputMaxLen - 1) return;
            m_pInputBuffer[len] = s[0];
            m_pInputBuffer[len + 1] = 0;
            return;
        }
    }

    switch (event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch (event.key.code)
            {
                case Keyboard::Backspace:
                    if (m_pInputBuffer)
                        if (int len = (int)strlen(m_pInputBuffer))
                            m_pInputBuffer[len - 1] = 0;
                    break;
                case Keyboard::Add:
                    //                                      iv1, iv2, iv3,          iv4
                    //bSendCommand(message_id::TEST_MSG, 1, 0,  1,   0,   0, nullptr,   0);
                    //window.setFramerateLimit(++frame_limit);
                    zoom *= 1.05;
                    //                     m_sViewPointX *= 1.05;
                    //                     m_sViewPointY *= 1.05;
                    break;
                case Keyboard::Subtract:
                    //                            submsg    iv1, iv2, iv3,          iv4
                    //bSendCommand(message_id::TEST_MSG, 1, 0, -1,   0,   0, nullptr,   0);
                    //window.setFramerateLimit(--frame_limit);
                    zoom /= 1.05;
                    //                     m_sViewPointX /= 1.05;
                    //                     m_sViewPointY /= 1.05;
                    break;
                case Keyboard::Left:
                    //                                      iv1, iv2, iv3,          iv4
                    //bSendCommand(message_id::TEST_MSG, 1, 0,  0,   1,   0, nullptr,   0);
    //                 if (event.key.shift)
    //                 {
    //                     if (event.key.control)
    //                         m_sViewPointX--;
    //                     else
    //                         m_sViewDstX--;
    //                 }
    //                 else
    //                 {
    //                     m_sPlayerX--;
    //                     m_sViewDstX = m_sViewPointX = (m_sPlayerX - ((get_virtual_width() / 32) / 2)) * 32;
    //                     m_sViewDstY = m_sViewPointY = (m_sPlayerY - ((get_virtual_height() / 32) / 2)) * 32;
    //                 }
                    if (event.key.shift)
                        testx2--;
                    else
                        testx--;
                    m_cArrowPressed = 1;
                    break;
                case Keyboard::Right:
                    //                            submsg    iv1, iv2, iv3,          iv4
                    //bSendCommand(message_id::TEST_MSG, 1, 0,  0,  -1,   0, nullptr,   0);
    //                 if (event.key.shift)
    //                 {
    //                     if (event.key.control)
    //                         m_sViewPointX++;
    //                     else
    //                         m_sViewDstX++;
    //                 }
    //                 else
    //                 {
    //                     m_sPlayerX++;
    //                     m_sViewDstX = m_sViewPointX = (m_sPlayerX - ((get_virtual_width() / 32) / 2)) * 32;
    //                     m_sViewDstY = m_sViewPointY = (m_sPlayerY - ((get_virtual_height() / 32) / 2)) * 32;
    //                 }
                    if (event.key.shift)
                        testx2++;
                    else
                        testx++;
                    m_cArrowPressed = 3;
                    break;
                case Keyboard::Up:
                    //                 if (event.key.shift)
                    //                 {
                    //                     if (event.key.control)
                    //                         m_sViewPointY--;
                    //                     else
                    //                         m_sViewDstY--;
                    //                 }
                    //                 else
                    //                 {
                    //                     m_sPlayerY--;
                    //                     m_sViewDstX = m_sViewPointX = (m_sPlayerX - ((get_virtual_width() / 32) / 2)) * 32;
                    //                     m_sViewDstY = m_sViewPointY = (m_sPlayerY - ((get_virtual_height() / 32) / 2)) * 32;
                    //                 }
                    if (event.key.shift)
                        testy2--;
                    else
                        testy--;
                    m_cArrowPressed = 2;
                    break;
                case Keyboard::Down:
                    //                 if (event.key.shift)
                    //                 {
                    //                     if (event.key.control)
                    //                         m_sViewPointY++;
                    //                     else
                    //                         m_sViewDstY++;
                    //                 }
                    //                 else
                    //                 {
                    //                     m_sPlayerY++;
                    //                     m_sViewDstX = m_sViewPointX = (m_sPlayerX - ((get_virtual_width() / 32) / 2)) * 32;
                    //                     m_sViewDstY = m_sViewPointY = (m_sPlayerY - ((get_virtual_height() / 32) / 2)) * 32;
                    //                 }
                    if (event.key.shift)
                        testy2++;
                    else
                        testy++;
                    m_cArrowPressed = 4;
                    break;

                case Keyboard::F5:
                    //RequestFullObjectData(ot_player, m_sPlayerObjectID);
                    if (!is_connected()) perform_connect();
                    break;
                case Keyboard::F7:
                    change_game_mode(DEF_GAMEMODE_ONCONNECTING);
                    break;
                case Keyboard::F8:
                    m_showGrid = !m_showGrid;
                    break;
                case Keyboard::Escape:
                    clipmousegame = !clipmousegame;
                    window.setMouseCursorGrabbed(clipmousegame);
                    m_bEscPressed = true;
                    captured = true;
                    break;
                case Keyboard::LShift:
                    m_bShiftPressed = true;
                    break;
                case Keyboard::LControl:
                    m_bCtrlPressed = true;
                    break;
                case Keyboard::LAlt:
                    m_altPressed = true;
                    break;
                case Keyboard::Tab:
                    if (m_bShiftPressed)
                    {
                        m_cCurFocus--;
                        if (m_cCurFocus < 1) m_cCurFocus = m_cMaxFocus;
                    }
                    else
                    {
                        m_cCurFocus++;
                        if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
                    }
                    if (m_cGameMode == DEF_GAMEMODE_ONMAINGAME)
                    {
                        //bSendCommand(message_id::COMMAND_COMMON, DEF_COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);
                    }
                    break;
                case Keyboard::Return:
                    if (event.key.alt)
                    {
                        // todo: recreate surfaces to resize to recenter area
                        fullscreen = !fullscreen;
                        create_renderer();
                        captured = true;
                        window.setMouseCursorGrabbed(clipmousegame);
                        window.setMouseCursorVisible(false);
                    }
                    else
                    {
                        m_bEnterPressed = true;
                    }
                    break;
                case Keyboard::F12:
                    CreateScreenShot();
                    break;

                case Keyboard::F11:
                    scale_mouse_rendering = !scale_mouse_rendering;
                    if (scale_mouse_rendering)
                    {
                        for (int i = 0; i < m_pSprite[DEF_SPRID_MOUSECURSOR]->m_iTotalFrame; ++i)
                            m_pSprite[DEF_SPRID_MOUSECURSOR]->sprite_[i].setScale(static_cast<float>(screenwidth) / screenwidth_v, static_cast<float>(screenheight) / screenheight_v);
                    }
                    else
                        for (int i = 0; i < m_pSprite[DEF_SPRID_MOUSECURSOR]->m_iTotalFrame; ++i)
                            m_pSprite[DEF_SPRID_MOUSECURSOR]->sprite_[i].setScale(1, 1);

                    break;

                case Keyboard::F6:
                    m_sViewDstX = (m_sPlayerX * 32) - ((get_virtual_width() / 32) / 2) - testx; // 288 | 9
                    m_sViewDstY = (m_sPlayerY * 32) - ((get_virtual_height() / 32) / 2) - testy; // 224 | 7
                    //                 calcoldviewport = !calcoldviewport;
                    //                 if (!calcoldviewport)
                    //                 {
                    //                     AddEventList("Switched to new viewport code.");
                    //                 }
                    //                 else
                    //                 {
                    //                     AddEventList("Switched to old viewport code.");
                    //                 }
                    break;
            }
            break;
        }
        case sf::Event::KeyReleased:
        {
            if (event.key.code == Keyboard::Backspace)
            {
            }
            if (event.key.code == Keyboard::Tab)
            {
            }

            switch (event.key.code)
            {
                case Keyboard::Escape:
                    break;
                case Keyboard::LShift:
                    m_bShiftPressed = false;
                    break;
                case Keyboard::LControl:
                    m_bCtrlPressed = false;
                    break;
                case Keyboard::LAlt:
                    m_altPressed = false;
                    break;
                case Keyboard::Tab:
                    break;
                case Keyboard::Return:
                    break;
                case Keyboard::F12:
                    break;
                case Keyboard::F5:
                    break;
            }
            break;
        }
        case sf::Event::Resized:
            break;
        case sf::Event::LostFocus:
            window.setFramerateLimit(frame_limit_bg); //set to var
            break;
        case sf::Event::GainedFocus:
            if (m_cGameMode != DEF_GAMEMODE_ONLOADING)
                window.setFramerateLimit(frame_limit);
            else
                window.setFramerateLimit(0);
            break;
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                if (event.mouseWheelScroll.delta > 0.f)
                    m_stMCursor.sZ++;
                else if (event.mouseWheelScroll.delta < 0.f)
                    m_stMCursor.sZ--;
            }
            else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
            {
                if (event.mouseWheelScroll.delta > 0.f)
                    m_stMCursor.sZ2++;
                else if (event.mouseWheelScroll.delta < 0.f)
                    m_stMCursor.sZ2--;
            }
            else
            {
            }
            break;
        case sf::Event::MouseButtonPressed:
        {
            if (wasinactive)
            {
                wasinactive = false;
            }

            //             for (auto & rect : dialogs)
            //             {
            //                 if (rect.contains(m_stMCursor.sX, m_stMCursor.sY))
            //                 {
            //                     break;
            //                 }
            //             }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                m_stMCursor.RB = true;
            }
            else if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_stMCursor.LB = true;
            }
            else if (event.mouseButton.button == sf::Mouse::Middle)
            {
                m_stMCursor.MB = true;
            }
            break;
        }
        case sf::Event::MouseButtonReleased:
        {
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                m_stMCursor.RB = false;
            }
            else if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_stMCursor.LB = false;
            }
            else if (event.mouseButton.button == sf::Mouse::Middle)
            {
                m_stMCursor.MB = false;
            }
            break;
        }
        case sf::Event::MouseMoved:
        {
            float diffx = static_cast<float>(screenwidth_v) / screenwidth;
            float diffy = static_cast<float>(screenheight_v) / screenheight;
            uint16_t x = uint16_t(event.mouseMove.x * diffx);
            uint16_t y = uint16_t(event.mouseMove.y * diffy);

            // if admin client
            if (m_stMCursor.LB && m_bCtrlPressed)
            {
                // pan event
                pan_x += event.mouseMove.x - m_stMCursor.sX;
                pan_y += event.mouseMove.y - m_stMCursor.sY;
            }

            m_stMCursor.sX = x;
            m_stMCursor.sY = y;

            //std::cout << fmt::format("{:#4}, {:#4} || {:#4}, {:#4}\n", event.mouseMove.x, event.mouseMove.y, x, y);
            break;
        }
    }
}

void CGame::create_surfaces()
{
    visible.create(screenwidth_v, screenheight_v);
    bg.create(screenwidth_v + 300, screenheight_v + 300);
    dynamic_bg.create(screenwidth_v + 300, screenheight_v + 300);
    dialog.create(screenwidth_v + 300, screenheight_v + 300);
    item_box.create(screenwidth_v + 300, screenheight_v + 300);
    //bg.create(32768, 32768);
    //charselect.create(256, 256);
}

void CGame::create_fonts()
{
    sf::Font s;
    s.loadFromFile(workingdirectory + "fonts/Arya-Regular.ttf");
    _font.insert(std::pair<std::string, sf::Font>("arya", s));

    s.loadFromFile(workingdirectory + "fonts/OpenSans-Regular.ttf");
    _font.insert(std::pair<std::string, sf::Font>("default", s));

    s.loadFromFile(workingdirectory + "fonts/PoetsenOne-Regular.ttf");
    _font.insert(std::pair<std::string, sf::Font>("test", s));

    _text.setFont(_font.at("arya"));
    arya_font = &_font.at("arya");
    default_font = &_font.at("default");
    test_font = &_font.at("test");

    chat_window_text.setFont(*arya_font);
    chat_window_text.setCharacterSize(12);

    overhead_text.setFont(*test_font);
    overhead_text.setCharacterSize(12);
    overhead_text.setOutlineColor(sf::Color::Black);
    overhead_text.setOutlineThickness(2);

    under_text.setFont(*arya_font);
    under_text.setCharacterSize(12);
    under_text.setOutlineColor(sf::Color::Black);
    under_text.setOutlineThickness(1);
}

void CGame::create_window()
{
    if (window.isOpen()) window.close();

    format_to_local(winName, "Helbreath Xtreme {}.{}.{} Renderer: {}", DEF_UPPERVERSION, DEF_LOWERVERSION, DEF_PATCHVERSION, _renderer);
    sf::ContextSettings context;
    context.antialiasingLevel = antialiasing;
    context.majorVersion = 3;
    context.minorVersion = 3;

    if (fullscreen)
    {
        screenwidth_windowed = screenwidth;
        screenheight_windowed = screenheight;
        auto mode = sf::VideoMode::getDesktopMode();
        screenwidth = mode.width;
        screenheight = mode.height;
        window.create(sf::VideoMode(screenwidth, screenheight), winName, (fullscreen ? sf::Style::Fullscreen : (sf::Style::Close)), context);
    }
    else
    {
        screenwidth = screenwidth_windowed;
        screenheight = screenheight_windowed;
        window.create(sf::VideoMode(screenwidth, screenheight), winName, (fullscreen ? sf::Style::Fullscreen : (sf::Style::Close)), context);
    }

    handle = window.getSystemHandle();

    window.setVerticalSyncEnabled(vsync);

    frame_limit = 0;
}

bool CGame::create_renderer()
{
    create_window();
    create_surfaces();
    create_fonts();
    create_load_list();

    modx = get_virtual_width() / 2 - 640 / 2;
    mody = get_virtual_height() / 2 - 480 / 2;

    return true;
}

int CGame::send_message(char * cData, std::size_t dwSize, char cKey)
{
    return write(cData, dwSize);
}

std::string CGame::get_game_mode_str()
{
    for (auto & mode : game_mode_map)
    {
        if (mode.first == m_cGameMode)
            return mode.second;
    }
    return "unknown";
}

std::string CGame::get_game_mode(int _gamemode)
{
    for (auto & mode : game_mode_map)
    {
        if (mode.first == _gamemode)
            return mode.second;
    }
    return "unknown";
}

int16_t CGame::get_game_mode(std::string _gamemode)
{
    for (auto & mode : game_mode_map)
    {
        if (mode.second == _gamemode)
            return mode.first;
    }
    return DEF_GAMEMODE_NULL;
}

void CGame::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Color color)
{ //TODO: replace all instances
    draw_line(x0, y0, x1, y1, color.r, color.g, color.b, color.a);
}

void CGame::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, int iR, int iG, int iB, int iA)
{
    if ((x0 == x1) && (y0 == y1))
        return;

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x0, y0), Color(iR, iG, iB, iA)),
        sf::Vertex(sf::Vector2f(x1, y1), Color(iR, iG, iB, iA))
    };
    visible.draw(line, 2, sf::Lines);
}

void CGame::update_mouse_state(uint16_t & x, uint16_t & y, uint16_t & z, char & left_button, char & right_button)
{
    x = m_stMCursor.sX;
    y = m_stMCursor.sY;
    z = m_stMCursor.sZ;
    left_button = m_stMCursor.LB;
    right_button = m_stMCursor.RB;
}

void CGame::draw_shadow_box(uint16_t sX, uint16_t sY, uint16_t dX, uint16_t dY, Color color)
{
    if ((sX == dX) && (sY == dY))
        return;

    sf::RectangleShape rectangle(sf::Vector2f(float(dX - sX), float(dY - sY)));
    rectangle.setPosition((float)sX, (float)sY);
    rectangle.setFillColor(color);
    visible.draw(rectangle);
}

void CGame::put_pixel(uint16_t sX, uint16_t sY, Color color)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f((float)sX, (float)sY), color)
    };
    visible.draw(line, 1, sf::Points);
}

void CGame::change_display_mode()
{
    fullscreen = !fullscreen;
    create_renderer();
}

void CGame::render_mouse(uint16_t mx, uint16_t my)
{
    if (m_bIsObserverMode == true)
    {
        draw_line(mx - 5, my, mx + 5, my, Color(255, 0, 0));
        draw_line(mx, my, mx, my + 5, Color(255, 0, 0));

        draw_line(mx - 5, my - 1, mx + 5, my - 1, Color(255, 0, 0, 127));
        draw_line(mx - 1, my - 5, mx - 1, my + 5, Color(255, 0, 0, 127));

        draw_line(mx - 5, my + 1, mx + 5, my + 1, Color(255, 0, 0, 127));
        draw_line(mx + 1, my - 5, mx + 1, my + 5, Color(255, 0, 0, 127));
    }
    else
    {
        m_pSprite[DEF_SPRID_MOUSECURSOR]->draw_to(mx, my, m_stMCursor.sCursorFrame, unixtime(), Color(255, 255, 255), DS_WIN);
    }
}
