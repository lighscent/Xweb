# JavaScript (Node.js) Web Server Implementation

## Overview

HTTP server implementation using Node.js with built-in modules for cross-platform compatibility. This implementation demonstrates modern JavaScript server programming with clean, readable code.

## Prerequisites

### All Platforms
- **Node.js** (version 12.0 or higher)
- **npm** (included with Node.js)

### Installation
```bash
# Check if Node.js is installed
node --version
npm --version

# If not installed, download from: https://nodejs.org/
```

## Quick Start

### Run
```bash
# Navigate to JS directory
cd JS

# Run the server
node webserver.js
```

### Test
- Main page: `http://localhost:8080/`
- API endpoint: `http://localhost:8080/api`

## Features

- HTTP/1.1 server on port 8080
- Cross-platform compatibility (Windows, Linux, macOS)
- Static HTML page with server info
- JSON API endpoint
- Real-time browser information display
- Error handling and graceful shutdown

## Code Structure

### Module Imports
```javascript
const http = require('http');
const url = require('url');
const os = require('os');
```

### Request Routing
```javascript
const parsedUrl = url.parse(req.url, true);
const pathname = parsedUrl.pathname;

if (pathname === '/api') {
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
    "os": "Windows_NT",
    "datetime": "2025-06-27 14:30:00",
    "timestamp": 1719500200,
    "status": "running",
    "language": "javascript"
  },
  "message": "Server API endpoint"
}
```

## Common Issues

### Port in Use
```bash
# Windows
netstat -ano | findstr :8080

# Linux/macOS
sudo lsof -i :8080

# Kill process using port
# Windows
taskkill /PID <PID> /F

# Linux/macOS
kill -9 <PID>
```

### Node.js Not Found
```bash
# Install Node.js from official website
# Or using package managers:

# Windows (Chocolatey)
choco install nodejs

# macOS (Homebrew)
brew install node

# Linux (Ubuntu/Debian)
sudo apt update
sudo apt install nodejs npm
```

### Permission Issues
- On Linux/macOS, ports below 1024 require sudo
- Use ports 8080+ for development to avoid permission issues

## Debug Mode
```bash
# Run with debug output
NODE_DEBUG=http node webserver.js

# Run with inspector for debugging
node --inspect webserver.js
```

## Limitations

- Single-threaded event loop (but non-blocking I/O)
- Runtime dependency on Node.js
- No type safety (dynamic typing)
- Memory usage higher than compiled languages

## References

- [Node.js HTTP Module Documentation](https://nodejs.org/api/http.html)
- [Node.js OS Module Documentation](https://nodejs.org/api/os.html)
- [Node.js URL Module Documentation](https://nodejs.org/api/url.html)

---

*Last updated: June 27, 2025*