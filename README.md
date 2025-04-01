# WebSocket Cpp
 


### Folder Structure: 
![Folder Structure](projImages/image.png)
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
├── BUILD.gn            # GN build root
├── .gn            # GN build configuration
└── README.md           # Documentation


##

in src run : 
```
g++ main.cpp WebSocketClient.cpp FileHandler.cpp StringUtils.cpp WebSocket.cpp WebSocketConnect.cpp CLI.cpp -o websocket_client -lboost_system -lpthread -lssl -lcrypto
```
or
```
clang++ main.cpp WebSocketClient.cpp FileHandler.cpp StringUtils.cpp WebSocket.cpp WebSocketConnect.cpp CLI.cpp -o websocket_client -Iinclude -I. -lboost_system -lpthread -lssl -lcrypto
```
For test case:
```
/src$ clang++ ../tests/WebSocketTest.cpp WebSocket.cpp WebSocketConnect.cpp -o websocket_test -Iinclude -I. -lboost_system -lpthread -lssl -lcrypto -lgtest -lgtest_main
/src$ ./websocket_test 
```
##


Build:

```
proj/src$ gn gen out/release --args="is_debug=false"
Done. Made 5 targets from 3 files in 3ms

proj/src$ ninja -C out/release
ninja: Entering directory `out/release'
[9/9] STAMP obj/all.stamp
```

```
proj/src$ gn gen out/debug --args="is_debug=true"
Done. Made 5 targets from 3 files in 3ms

proj/src$ ninja -C out/debug
ninja: Entering directory `out/debug'
[9/9] STAMP obj/all.stamp
```

