// In WebSocket.h (add these declarations)
#include <memory>  // For std::unique_ptr
#include <string>  // For std::string
#include <vector>  // For std::vector
#include <boost/asio/ssl.hpp>


class WebSocket {
    public:
        WebSocket();
        ~WebSocket();
        
        bool connect(const std::string& url);
        bool sendMessage(const std::string& message);
        bool sendBinaryMessage(const std::vector<uint8_t>& data);  // New method
        std::string receiveMessage();
        std::vector<uint8_t> receiveBinaryMessage();  // New method
        void close();
        
        void setBinaryMode(bool binary);  // New method to set message type
        
    private:
        class Impl;
        std::unique_ptr<Impl> impl;
    };
    