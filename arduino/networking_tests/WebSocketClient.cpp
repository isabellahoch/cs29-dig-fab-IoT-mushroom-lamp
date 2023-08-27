#include "WebSocketClient.hpp"

WebSocketClient::WebSocketClient() : is_running_(false) {
    client_.init_asio();
    client_.set_message_handler(bind(&WebSocketClient::OnMessage, this, placeholders::_1, placeholders::_2));
}

bool WebSocketClient::Connect(const std::string& uri) {
    try {
        websocketpp::lib::error_code ec;
        websocketpp::client<websocketpp::config::asio_client>::connection_ptr con = client_.get_connection(uri, ec);
        if (ec) {
            cerr << "Error: " << ec.message() << endl;
            return false;
        }

        client_.connect(con);
        connection_ = con->get_handle();
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    }
    
    return true;
}

void WebSocketClient::Disconnect() {
    client_.close(connection_, websocketpp::close::status::going_away, "User initiated disconnect");
}

void WebSocketClient::Send(const std::string& message) {
    client_.send(connection_, message, websocketpp::frame::opcode::text);
}

void WebSocketClient::Start() {
    if (!is_running_) {
        thread_ = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client_::run, &client_);
        is_running_ = true;
    }
}

void WebSocketClient::Stop() {
    if (is_running_) {
        client_.stop();
        thread_->join();
        is_running_ = false;
    }
}

void WebSocketClient::OnMessage(websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg) {
    cout << "Received message: " << msg->get_payload() << endl;
}
