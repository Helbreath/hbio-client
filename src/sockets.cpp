//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "game.h"
#include <iostream>
#include <fmt/format.h>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

// todo: make part of core class
extern int64_t G_iBytesOut, G_iBytesIn;

std::shared_ptr<CGame::msg_queue_entry> CGame::get_login_msg_queue()
{
    std::shared_ptr<CGame::msg_queue_entry> msg = loginpipe.front();
    loginpipe.pop_front();
    return msg;
}

void CGame::on_message(const ix::WebSocketMessagePtr & msg)
{
    G_iBytesIn += msg->str.length();
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
        std::cout << "ws close" << msg->closeInfo.reason.c_str() << "\n";
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
#if defined(_DEVMODE)
    if (dev_mode.wss)
        ws->setUrl(fmt::format("wss://{}:{}", SERVER_IP, SERVER_PORT));
    else
        ws->setUrl(fmt::format("ws://{}:{}", SERVER_IP, SERVER_PORT));
#else
    ws->setUrl(fmt::format("wss://{}:{}", SERVER_IP, SERVER_PORT));
#endif
    //ws->setUrl(fmt::format("wss://{}:8443", SERVER_IP));
    ws->setUrl(fmt::format("ws://{}:{}", SERVER_IP, SERVER_PORT));
    ws->disableAutomaticReconnection();

    ws->start();
}

int32_t CGame::write(const char * data, const std::size_t size)
{
    if (is_connected())
    {
        std::cout << "Sent " << size << " cbytes of data\n";
        auto d = ix::IXWebSocketSendData{ data, size };
        int32_t send_size = ws->sendBinary(d).payloadSize;
        G_iBytesOut += send_size;
        if (is_connected()) return send_size;
    }
    return -129; // DEF_XSOCKEVENT_SOCKETERROR
}

int32_t CGame::write(stream_write & sw)
{
    if (is_connected())
    {
        std::cout << "Sent " << sw.position << " bytes of data\n";
        auto d = ix::IXWebSocketSendData{ sw.data, sw.position };
        int32_t send_size = ws->sendBinary(d).payloadSize;
        G_iBytesOut += send_size;
        return send_size;
    }
    return -129; // DEF_XSOCKEVENT_SOCKETERROR
}

int32_t CGame::write(nlohmann::json & obj)
{
    if (is_connected())
    {
        int32_t send_size = ws->send(obj.dump(-1, 0x32, false, nlohmann::detail::error_handler_t::ignore)).payloadSize;
        G_iBytesOut += send_size;
        return send_size;
    }
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
            ChangeGameMode(DEF_GAMEMODE_ONCONNECTIONLOST);
            break;
        case DEF_GAMEMODE_ONMAINMENU:
            break;
        default:
            ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
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

        ws->close(code, reason);
    }
    catch (std::system_error &)
    {
        std::cout << "close() mutex lock timeout?\n";
    }
}
