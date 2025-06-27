# Python Web Server Implementation

## Overview

HTTP server implementation using Python's built-in `http.server` module with modern Python features. This implementation demonstrates Python web programming best practices with clean, readable code and comprehensive error handling.

## Prerequisites

### All Platforms
- **Python 3.7+** (recommended 3.9+)
- No external dependencies (uses standard library only)

### Installation Check
```bash
# Check Python version
python --version
# or
python3 --version

# Ensure pip is available (for optional dependencies)
pip --version
```

## Quick Start

### Run
```bash
# Navigate to PYTHON directory
cd PYTHON

# Run the server
python webserver.py

# Alternative for systems with both Python 2 and 3
python3 webserver.py

# Run with custom configuration
python webserver.py --port 8080 --host localhost
```

### Test
- Main page: `http://localhost:8080/`
- API endpoint: `http://localhost:8080/api`

## Features

- HTTP/1.1 server on port 8080
- Object-oriented design with clean separation
- Built-in threading for concurrent requests
- Comprehensive error handling and logging
- Cross-platform compatibility
- Enhanced HTML page with modern styling
- JSON API endpoint with detailed system info
- Real-time browser information display
- Graceful shutdown handling
- Signal handling (SIGINT, SIGTERM)

## Code Structure

### Class Design
```python
class WebServerHandler(BaseHTTPRequestHandler):
    def do_GET(self):              # Handle GET requests
    def _handle_html_request(self): # Serve HTML content
    def _handle_api_request(self):  # Serve JSON API
    def _handle_error(self):       # Error handling

class WebServer:
    def start(self):               # Start server
    def stop(self):                # Stop server gracefully
    def is_running(self):          # Check server status
```

### Request Routing
```python
def do_GET(self):
    parsed_path = urlparse(self.path)
    
    if parsed_path.path == '/api':
        self._handle_api_request()
    else:
        self._handle_html_request()
```

### Error Handling
```python
try:
    # Server operations
except OSError as e:
    if "Address already in use" in str(e):
        print(f"Port {self.port} is already in use")
except Exception as e:
    print(f"Unexpected error: {e}")
```

### Signal Handling
```python
import signal

def signal_handler(signum, frame):
    server.stop()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)
signal.signal(signal.SIGTERM, signal_handler)
```

### API Response Format
```json
{
  "server_info": {
    "port": 8080,
    "platform": "win32",
    "os": "Windows 10",
    "python_version": "3.11.0",
    "datetime": "2025-06-27 14:30:00",
    "timestamp": 1719500200,
    "status": "running",
    "language": "python"
  },
  "message": "Server API endpoint"
}
```

## Common Issues

### Python Not Found
```bash
# Windows - Install from python.org
# Or use Microsoft Store version

# macOS - Using Homebrew
brew install python

# Linux (Ubuntu/Debian)
sudo apt update
sudo apt install python3 python3-pip

# Linux (CentOS/RHEL)
sudo yum install python3 python3-pip
```

### Port in Use
```bash
# Find process using port 8080
# Windows
netstat -ano | findstr :8080

# Linux/macOS
sudo lsof -i :8080

# Kill process
# Windows
taskkill /PID <PID> /F

# Linux/macOS
kill -9 <PID>
```

### Permission Issues
```bash
# Linux/macOS - Use higher port numbers (8080+) to avoid sudo
# Or use sudo for ports < 1024
sudo python3 webserver.py
```

### Module Import Errors
```bash
# Ensure Python 3.7+ is being used
python3 --version

# Check if modules are available
python3 -c "import http.server, json, platform, threading"
```

## Development Mode

### Debug Logging
```python
# Add debug logging
import logging
logging.basicConfig(level=logging.DEBUG)

# Enhanced error output
import traceback
traceback.print_exc()
```

### Custom Configuration
```python
# Modify server configuration
PORT = 8080
HOST = '0.0.0.0'  # Listen on all interfaces

# Custom handler
class CustomHandler(WebServerHandler):
    def log_message(self, format, *args):
        # Custom logging implementation
        pass
```

## Advanced Features

### Threading Model
```python
# Built-in threading for concurrent requests
self.server_thread = threading.Thread(target=self._serve_forever)
self.server_thread.daemon = True
self.server_thread.start()
```

### Enhanced HTML Response
```python
# Modern HTML with CSS styling
html_template = """<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        #browser { background-color: #f5f5f5; padding: 10px; }
    </style>
</head>
```

### JSON Serialization
```python
# Pretty-printed JSON with proper encoding
json_content = json.dumps(api_response, indent=2, ensure_ascii=False)
self.wfile.write(json_content.encode('utf-8'))
```

### Platform Detection
```python
system = platform.system().lower()
platform_map = {
    'windows': 'win32',
    'linux': 'linux',
    'darwin': 'darwin'
}
platform_id = platform_map.get(system, system)
```

## Performance Considerations

- Single-threaded server with threading for request handling
- Memory efficient with built-in modules
- Fast startup time
- Suitable for development and light production use
- Consider `gunicorn` or `uvicorn` for production

## Testing

### Unit Testing
```python
# Create test_webserver.py
import unittest
import requests
import threading
import time

class TestWebServer(unittest.TestCase):
    def setUp(self):
        self.server = WebServer(port=8081)
        self.server.start()
        time.sleep(0.1)  # Wait for server to start
    
    def tearDown(self):
        self.server.stop()
    
    def test_html_response(self):
        response = requests.get('http://localhost:8081/')
        self.assertEqual(response.status_code, 200)
        self.assertIn('Hello, World!', response.text)
    
    def test_api_response(self):
        response = requests.get('http://localhost:8081/api')
        self.assertEqual(response.status_code, 200)
        data = response.json()
        self.assertIn('server_info', data)
```

### Manual Testing
```bash
# Test with curl
curl http://localhost:8080/
curl http://localhost:8080/api

# Test with Python requests
python3 -c "import requests; print(requests.get('http://localhost:8080/api').json())"
```

## Limitations

- Single-process architecture
- Basic HTTP/1.1 support only
- No built-in HTTPS support
- Limited concurrent connections
- No request body parsing for POST/PUT
- Basic security features

## Extensions

### HTTPS Support
```python
import ssl

# Create SSL context
context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
context.load_cert_chain('server.crt', 'server.key')

# Wrap server socket
httpd.socket = context.wrap_socket(httpd.socket, server_side=True)
```

### Request Body Handling
```python
def do_POST(self):
    content_length = int(self.headers['Content-Length'])
    post_data = self.rfile.read(content_length)
    # Process POST data
```

### Configuration File Support
```python
import configparser

config = configparser.ConfigParser()
config.read('server.ini')
PORT = config.getint('server', 'port', fallback=8080)
```

### Logging Enhancement
```python
import logging
from logging.handlers import RotatingFileHandler

# Setup rotating file handler
handler = RotatingFileHandler('server.log', maxBytes=1024*1024, backupCount=5)
logging.basicConfig(handlers=[handler], level=logging.INFO)
```

## Production Deployment

### Using Gunicorn
```bash
# Install gunicorn
pip install gunicorn

# Create WSGI app wrapper
# wsgi_app.py
from webserver import WebServerHandler
# ... WSGI wrapper code

# Run with gunicorn
gunicorn --bind 0.0.0.0:8080 --workers 4 wsgi_app:application
```

### Docker Deployment
```dockerfile
FROM python:3.11-slim

WORKDIR /app
COPY webserver.py .
COPY requirements.txt .

RUN pip install --no-cache-dir -r requirements.txt

EXPOSE 8080
CMD ["python", "webserver.py"]
```

### Systemd Service
```ini
# /etc/systemd/system/python-webserver.service
[Unit]
Description=Python Web Server
After=network.target

[Service]
Type=simple
User=www-data
WorkingDirectory=/opt/webserver
ExecStart=/usr/bin/python3 webserver.py
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

## References

- [Python http.server Documentation](https://docs.python.org/3/library/http.server.html)
- [Python threading Module](https://docs.python.org/3/library/threading.html)
- [Python json Module](https://docs.python.org/3/library/json.html)
- [Python platform Module](https://docs.python.org/3/library/platform.html)
- [Python signal Module](https://docs.python.org/3/library/signal.html)

---

*Last updated: June 27, 2025*