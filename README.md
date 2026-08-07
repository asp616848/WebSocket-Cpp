# WebSocket-Cpp

A small C++17 WebSocket **client** with an interactive CLI, built on top of Boost.Beast/Boost.Asio for the network/TLS layer. It connects to a secure (`wss://`) WebSocket endpoint and lets you send text messages, binary messages, and whole files, printing whatever the server echoes back.

## Overview

The project is split into a handful of focused static libraries (via a GN build):

- **`WebSocket`** — the low-level client: opens a TLS (`wss://`) connection with Boost.Beast, and sends/receives text or binary frames.
- **`WebSocketClient`** — a thin wrapper around `WebSocket` that parses CLI commands (`text`, `binary`, `file`) and dispatches to the right send/receive path.
- **`CLI`** — reads a line of user input and prints server responses.
- **`FileHandler`** — reads a file from disk into a `std::vector<uint8_t>` so it can be sent as a binary WebSocket message.
- **`StringUtils`** — converts between `std::string` and `std::vector<uint8_t>` for binary payloads.

The example entry point (`src/main.cpp`) connects to the public echo server `wss://echo.websocket.events` and drives an interactive send/receive loop.

## Features

- Secure WebSocket client over TLS (`wss://`), using Boost.Beast's `websocket::stream<ssl::stream<tcp::socket>>`.
- Text message send/receive.
- Binary message send/receive (arbitrary byte payloads).
- Send an entire file as a binary WebSocket message (`FileHandler` + `file <path>` command).
- Simple interactive CLI loop with `text`, `binary`, `file`, and `exit` commands.
- Unit tests (Google Test) covering connect/close/error paths against a live echo server.
- Debug/Release build configurations (`-O0 -g` vs `-O3 -DNDEBUG`) via GN.

Note: this repository implements a WebSocket **client** only — there is no bundled WebSocket server implementation.

## Tech stack

- **Language:** C++17
- **Networking/WebSocket:** [Boost.Beast](https://www.boost.org/doc/libs/release/libs/beast/) + Boost.Asio
- **TLS:** OpenSSL (`libssl`, `libcrypto`)
- **Testing:** Google Test (`gtest`, `gtest_main`)
- **Build system:** [GN](https://gn.googlesource.com/gn/) + [Ninja](https://ninja-build.org/) (`BUILD.gn`, `.gn`); can also be compiled directly with `g++`/`clang++`

### Dependencies

Install before building:
- Boost (system + Beast/Asio headers)
- OpenSSL (dev headers/libs)
- GN and Ninja (for the GN-based build)
- Google Test (only needed to build `websocket_test`)
- pthreads (used by the test binary)

On Debian/Ubuntu:
```bash
sudo apt-get install libboost-system-dev libssl-dev libgtest-dev ninja-build
# GN itself typically needs to be built/installed separately, see https://gn.googlesource.com/gn
```

## Build

### Option 1: Direct compilation (quickest)

From the `src/` directory:
```bash
g++ -std=c++17 main.cpp WebSocket.cpp WebSocketConnect.cpp WebSocketClient.cpp CLI.cpp FileHandler.cpp StringUtils.cpp \
    -I../include -o websocket_client -lboost_system -lpthread -lssl -lcrypto
```
(or substitute `clang++` for `g++`).

### Option 2: GN + Ninja

From the repository root:
```bash
gn gen out/release --args="is_debug=false"
ninja -C out/release
```
or for a debug build:
```bash
gn gen out/debug --args="is_debug=true"
ninja -C out/debug
```
This produces the `websocket_client` executable and a `websocket_test` binary (`group("all")` in `BUILD.gn` builds both).

## Usage

Run the client:
```bash
./websocket_client
```

By default it connects to `wss://echo.websocket.events`. Once connected, use the interactive commands:
```
text <message>    - send a text message
binary <message>  - send the message as a binary payload
file <filename>   - read a file from disk and send it as binary
exit              - close the connection and quit
```

To point the client at a different WebSocket endpoint, change the URL passed to `wsClient.connect(...)` in `src/main.cpp`.

## Running tests

```bash
ninja -C out/debug websocket_test
./out/debug/websocket_test
```
The test suite (`tests/WebSocketTest.cpp`) exercises connection establishment, invalid URLs, unreachable hosts, secure connection setup, clean termination, and multiple concurrent connections — all against the live `wss://echo.websocket.events` endpoint, so tests require network access.

## Project structure

```
.
├── BUILD.gn                # GN build targets (websocket, websocket_client_lib, cli, file_handler, string_utils, tests)
├── .gn                      # GN root config
├── include/                 # Public headers (WebSocket, WebSocketClient, CLI, FileHandler, StringUtils)
├── src/                      # Implementations + main.cpp entry point
├── tests/WebSocketTest.cpp   # Google Test suite
└── build/                    # GN toolchain/build configuration
```
