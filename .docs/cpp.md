# C++ Web Server Implementation

## Overview

Object-oriented HTTP server implementation in C++ using modern C++ features and cross-platform socket programming. This implementation demonstrates C++ best practices with RAII, exceptions, and STL containers.

## Prerequisites

### Windows
- **MinGW-w64** or **Visual Studio** (C++11 or later)
- Windows Sockets 2 (included with Windows)

### Linux/Unix
- **GCC** or **Clang** (C++11 or later)
- Standard C++ library

## Quick Start

### Build
```bash
# Windows (MinGW)
g++ -std=c++11 webserver.cpp -o webserver.exe -lws2_32

# Windows (Visual Studio)
cl /EHsc webserver.cpp ws2_32.lib

# Linux/Unix
g++ -std=c++11 webserver.cpp -o webserver
```

### Run
```bash
# Windows
./webserver.exe

# Linux/Unix
./webserver
```

### Test
- Main page: `http://localhost:8080/`
- API endpoint: `http://localhost:8080/api`

## Features

- HTTP/1.1 server on port 8080
- Object-oriented design with RAII
- Cross-platform socket programming
- Static HTML page with server info
- JSON API endpoint
- Real-time browser information display
- Exception handling and resource management

## Code Structure

### Class Design
```cpp
class WebServer {
private:
    int server_fd;
    struct sockaddr_in server_addr;
    
public:
    WebServer();           // Constructor with initialization
    ~WebServer();          // Destructor with cleanup
    bool start();          // Start server
    void run();            // Main server loop
};
```

### Platform Detection
```cpp
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
#endif
```

### Modern C++ Features
```cpp
// String streams for response building
std::ostringstream response;
response << "HTTP/1.1 200 OK\r\n";

// Chrono for time handling
auto now = std::chrono::system_clock::now();
auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
    now.time_since_epoch()).count();

// RAII for resource management
~WebServer() {
    stop();  // Automatic cleanup
}
```

### API Response Format
```json
{
  "server_info": {
    "port": 8080,
    "platform": "win32",
    "os": "Windows",
    "datetime": "2025-06-27 14:30:00",
    "timestamp": 1719500200,
    "status": "running",
    "language": "cpp"
  },
  "message": "Server API endpoint"
}
```

## Common Issues

### Compilation Errors
```bash
# Missing C++11 support
g++ -std=c++11 webserver.cpp -o webserver

# Windows linking error
g++ webserver.cpp -o webserver.exe -lws2_32

# Missing headers (Linux)
sudo apt install build-essential
```

### Port in Use
```bash
# Windows
netstat -ano | findstr :8080

# Linux
sudo lsof -i :8080
```

### Runtime Errors
- **Socket creation failed**: Check system permissions
- **Bind failed**: Port may be in use or insufficient privileges
- **WSAStartup failed**: Windows socket initialization error

## Debug Build
```bash
# Windows
g++ -std=c++11 -g -Wall -Wextra webserver.cpp -o webserver.exe -lws2_32

# Linux
g++ -std=c++11 -g -Wall -Wextra webserver.cpp -o webserver
```

## Advanced Features

### Exception Safety
```cpp
try {
    WebServer server;
    server.start();
    server.run();
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

### Resource Management
- RAII ensures proper socket cleanup
- Automatic WSA cleanup on Windows
- Exception-safe design

### Performance Considerations
- Single-threaded (consider std::thread for multi-threading)
- Stack-allocated buffers for efficiency
- Move semantics for string operations

## Limitations

- Single-threaded (one request at a time)
- Fixed buffer size (1024 bytes)
- Basic error handling
- No input validation or security features

## Extensions

### Multi-threading Example
```cpp
#include <thread>
#include <vector>

void handleClientAsync(int client_fd) {
    std::thread([client_fd]() {
        // Handle client in separate thread
        handleClient(client_fd);
        close(client_fd);
    }).detach();
}
```

## References

- [C++ Socket Programming](https://en.cppreference.com/w/cpp)
- [Winsock2 Documentation](https://docs.microsoft.com/en-us/windows/win32/winsock/)
- [POSIX Socket Programming](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [Modern C++ Guidelines](https://isocpp.github.io/CppCoreGuidelines/)

---

*Last updated: June 27, 2025*