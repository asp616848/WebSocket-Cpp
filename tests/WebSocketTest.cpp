#include <gtest/gtest.h>
#include "WebSocket.h"

TEST(WebSocketTest, ConnectToServer) {
    WebSocket ws;
    ASSERT_TRUE(ws.connect("wss://echo.websocket.events"));
}

TEST(WebSocketTest, SendMessage) {
    WebSocket ws;
    ws.connect("wss://echo.websocket.events");
    ASSERT_TRUE(ws.sendMessage("Hello, World!"));
}
