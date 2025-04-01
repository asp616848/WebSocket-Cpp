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

// Passing tests

TEST_F(WebSocketTest, ConnectToServer) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
    ws.close();
}

TEST_F(WebSocketTest, ConnectWithInvalidURL) {
    WebSocket ws;
    ASSERT_FALSE(ws.connect("invalid-url"));
}

TEST_F(WebSocketTest, ConnectionErrorHandling) {
    WebSocket ws;
    // Try to connect to a non-existent server
    ASSERT_FALSE(ws.connect("wss://non-existent-server.example.com"));
}
