# C Web Server Implementation

## Overview

Simple HTTP server implementation in C with cross-platform support for Windows and Linux/Unix systems.

## Prerequisites

### Windows
- **MinGW-w64** or **Visual Studio**
- Windows Sockets 2 (included with Windows)

### Linux/Unix
- **GCC** compiler
- Standard C library

## Quick Start

### Build
```bash
# Windows
gcc webserver.c -o webserver.exe -lws2_32

# Linux/Unix
gcc webserver.c -o webserver
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
- Cross-platform socket programming
- Static HTML page with server info
- JSON API endpoint
- Real-time browser information display

## Code Structure

### Platform Detection
```c
#ifdef _WIN32
    // Windows: Winsock2
    #include <winsock2.h>
#else
    // Unix: POSIX sockets
    #include <sys/socket.h>
#endif
```

### Request Routing
```c
if (strncmp(buffer, "GET /api", 8) == 0) {
    // Serve JSON response
} else {
    // Serve HTML page
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
    "status": "running"
  },
  "message": "Server API endpoint"
}
```

## Common Issues

### Port in Use
```bash
# Windows
netstat -ano | findstr :8080

# Linux
sudo lsof -i :8080
```

### Build Errors
- **Windows**: Ensure MinGW is installed and `-lws2_32` flag is used
- **Linux**: Install `gcc` and development tools

### Permission Denied
- Run as administrator (Windows) or with `sudo` (Linux) if needed
- Check firewall settings

## Debug Build
```bash
# Windows
gcc -g -Wall webserver.c -o webserver.exe -lws2_32

# Linux
gcc -g -Wall webserver.c -o webserver
```

## Limitations

- Single-threaded (one request at a time)
- Fixed 1024-byte buffer for requests
- No input validation or security features
- Basic error handling

## References

- [Winsock2 Documentation](https://docs.microsoft.com/en-us/windows/win32/winsock/)
- [POSIX Socket Programming](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [HTTP/1.1 Specification](https://tools.ietf.org/html/rfc2616)

---

*Last updated: June 27, 2025*