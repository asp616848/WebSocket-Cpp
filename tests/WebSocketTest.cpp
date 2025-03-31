#include <gtest/gtest.h>
#include "../include/WebSocket.h"
#define BOOST_BEAST_ALLOW_SSL_STREAM
#include <boost/beast/websocket/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <chrono>
#include <thread>

class WebSocketTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup for all tests
    }

    void TearDown() override {
        // Common cleanup for all tests
    }
};

TEST_F(WebSocketTest, ConnectToServer) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    ws.close();
}

TEST_F(WebSocketTest, ConnectWithInvalidURL) {
    WebSocket ws;
    ASSERT_FALSE(ws.connect("invalid-url"));
}

TEST_F(WebSocketTest, SendAndReceiveMessage) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    
    const std::string testMessage = "Hello, World!";
    ASSERT_TRUE(ws.sendMessage(testMessage));
    
    std::string response = ws.receiveMessage();
    ASSERT_FALSE(response.empty());
    ASSERT_EQ(response, testMessage);
    
    ws.close();
}

TEST_F(WebSocketTest, SendMultipleMessages) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    
    const std::vector<std::string> messages = {
        "First message",
        "Second message",
        "Third message with special chars: !@#$%^&*()"
    };
    
    for (const auto& msg : messages) {
        ASSERT_TRUE(ws.sendMessage(msg));
        std::string response = ws.receiveMessage();
        ASSERT_FALSE(response.empty());
        ASSERT_EQ(response, msg);
    }
    
    ws.close();
}

TEST_F(WebSocketTest, CloseAndReconnect) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    ws.close();
    
    // Wait a moment before reconnecting
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    ASSERT_TRUE(ws.sendMessage("After reconnection"));
    std::string response = ws.receiveMessage();
    ASSERT_EQ(response, "After reconnection");
    
    ws.close();
}

TEST_F(WebSocketTest, LargeMessage) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    
    // Create a large message (100KB)
    std::string largeMessage(100 * 1024, 'A');
    ASSERT_TRUE(ws.sendMessage(largeMessage));
    
    std::string response = ws.receiveMessage();
    ASSERT_EQ(response.size(), largeMessage.size());
    ASSERT_EQ(response, largeMessage);
    
    ws.close();
}

// This test checks if the WebSocket implementation handles connection errors properly
TEST_F(WebSocketTest, ConnectionErrorHandling) {
    WebSocket ws;
    // Try to connect to a non-existent server
    ASSERT_FALSE(ws.connect("wss://non-existent-server.example.com"));
}