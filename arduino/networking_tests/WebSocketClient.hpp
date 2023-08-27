#pragma once

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <iostream>

using namespace std;

class WebSocketClient {
public:
    WebSocketClient();

    bool Connect(const std::string& uri);
    void Disconnect();

    void Send(const std::string& message);

    void Start();
    void Stop();

private:
    void OnMessage(websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg);

    websocketpp::client<websocketpp::config::asio_client> client_;
    websocketpp::connection_hdl connection_;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_;
    bool is_running_;
};

