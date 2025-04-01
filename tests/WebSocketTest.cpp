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

// 1. Connection establishment test
TEST_F(WebSocketTest, ConnectToServer) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    ws.close();
}

// 2. Connection rejection test
TEST_F(WebSocketTest, ConnectWithInvalidURL) {
    WebSocket ws;
    ASSERT_FALSE(ws.connect("invalid-url"));
}

// 3. Connection error handling test
TEST_F(WebSocketTest, ConnectionErrorHandling) {
    WebSocket ws;
    ASSERT_FALSE(ws.connect("wss://non-existent-server.example.com"));
}

// 4. Secure connection test
TEST_F(WebSocketTest, SecureConnectionTest) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    ws.close();
}

// 5. Connection termination test
TEST_F(WebSocketTest, ConnectionTermination) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    ws.close();
    // Attempt to send a message after closing should fail
    ASSERT_FALSE(ws.sendMessage("Test message"));
}

// 6. Multiple connection test
TEST_F(WebSocketTest, MultipleConnections) {
    WebSocket ws1, ws2;
    ASSERT_TRUE(ws1.connect("wss://echo.websocket.events"));
    ASSERT_TRUE(ws2.connect("wss://echo.websocket.events"));
    ws1.close();
    ws2.close();
}

// 7. Connection timeout test
TEST_F(WebSocketTest, ConnectionTimeout) {
    WebSocket ws;
    // Assuming your WebSocket class has a setTimeout method
    // ws.setTimeout(std::chrono::milliseconds(100));
    ASSERT_FALSE(ws.connect("wss://example.com:12345")); // Non-responsive endpoint
}
