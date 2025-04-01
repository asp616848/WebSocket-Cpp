# WebSocket Cpp
 
WEBSOCKET-CPP/
├── Build/                # Build files
│   ├── config            # config point
│   │    └── BUILDCONFIG.gn   
│   └── toolchain         # toolchain
│        └── BUILD.gn      
│          # Command-line interface implementation
├── src/                # Source files
│   ├── main.cpp        # Entry point
│   ├── WebSocket.cpp   # WebSocket client implementation
│   └── CLI.cpp         # Command-line interface implementation
├── include/            # Header files
│   ├── WebSocket.h
│   └── CLI.h
├── tests/              # Unit tests
│   └── WebSocketTest.cpp
├── BUILD.gn            # GN build configuration
├── .gn            # GN build configuration
└── README.md           # Documentation


in src run : g++ main.cpp WebSocket.cpp CLI.cpp -o websocket_client -lboost_system -lpthread -lssl -lcrypto