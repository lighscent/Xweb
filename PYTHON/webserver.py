#!/usr/bin/env python3
"""
Simple HTTP web server implementation in Python
Demonstrates Python web programming with built-in libraries
"""

import json
import platform
import socket
import threading
from datetime import datetime
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse


class WebServerHandler(BaseHTTPRequestHandler):
    """HTTP request handler for the web server"""
    
    def log_message(self, format, *args):
        """Override to customize logging"""
        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] {format % args}")
    
    def do_GET(self):
        """Handle GET requests"""
        try:
            parsed_path = urlparse(self.path)
            
            if parsed_path.path == '/api':
                self._handle_api_request()
            else:
                self._handle_html_request()
                
        except Exception as e:
            self._handle_error(500, f"Internal server error: {str(e)}")
    
    def _handle_html_request(self):
        """Serve HTML page"""
        html_content = self._create_html_response()
        
        self.send_response(200)
        self.send_header('Content-Type', 'text/html; charset=utf-8')
        self.send_header('Content-Length', str(len(html_content.encode('utf-8'))))
        self.send_header('Connection', 'close')
        self.end_headers()
        
        self.wfile.write(html_content.encode('utf-8'))
    
    def _handle_api_request(self):
        """Serve JSON API response"""
        api_response = self._create_api_response()
        json_content = json.dumps(api_response, indent=2)
        
        self.send_response(200)
        self.send_header('Content-Type', 'application/json; charset=utf-8')
        self.send_header('Content-Length', str(len(json_content.encode('utf-8'))))
        self.send_header('Connection', 'close')
        self.end_headers()
        
        self.wfile.write(json_content.encode('utf-8'))
    
    def _handle_error(self, status_code, message):
        """Handle error responses"""
        self.send_response(status_code)
        self.send_header('Content-Type', 'text/plain')
        self.end_headers()
        self.wfile.write(message.encode('utf-8'))
    
    def _create_html_response(self):
        """Create HTML response content"""
        python_version = f"{platform.python_version()}"
        system_info = f"{platform.system()} {platform.release()}"
        
        html_template = """<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Python Web Server</title>
    <style>
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 40px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            color: #333;
        }}
        .container {{
            max-width: 900px;
            margin: 0 auto;
            background-color: white;
            padding: 40px;
            border-radius: 15px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
        }}
        h1 {{
            color: #2c3e50;
            text-align: center;
            margin-bottom: 10px;
            font-size: 2.5em;
            font-weight: 300;
        }}
        .language-badge {{
            display: inline-block;
            background: linear-gradient(45deg, #3776ab, #ffd43b);
            color: white;
            padding: 8px 16px;
            border-radius: 25px;
            font-size: 0.9em;
            font-weight: bold;
            margin-left: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.2);
        }}
        h2 {{
            color: #34495e;
            border-bottom: 3px solid #3498db;
            padding-bottom: 10px;
            margin-top: 40px;
        }}
        .info-grid {{
            display: grid;
            grid-template-columns: auto 1fr;
            gap: 15px 25px;
            margin: 25px 0;
            background: linear-gradient(135deg, #f8f9fa, #e9ecef);
            padding: 25px;
            border-radius: 10px;
            border-left: 5px solid #3498db;
        }}
        .info-label {{
            font-weight: bold;
            color: #2c3e50;
        }}
        .info-value {{
            color: #34495e;
        }}
        a {{
            color: #3498db;
            text-decoration: none;
            font-weight: 500;
            transition: all 0.3s ease;
        }}
        a:hover {{
            color: #2980b9;
            text-decoration: underline;
        }}
        #browser {{
            background: linear-gradient(135deg, #e8f4f8, #d1ecf1);
            padding: 20px;
            border-radius: 10px;
            margin-top: 15px;
            border-left: 5px solid #17a2b8;
            font-family: 'Courier New', monospace;
            font-size: 0.9em;
        }}
        .footer {{
            text-align: center;
            margin-top: 40px;
            padding-top: 20px;
            border-top: 1px solid #dee2e6;
            color: #6c757d;
            font-size: 0.9em;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>Hello, World! <span class="language-badge">Python</span></h1>
        
        <h2>Server Information</h2>
        <div class="info-grid">
            <span class="info-label">Port:</span>
            <span class="info-value">{port}</span>
            <span class="info-label">Platform:</span>
            <span class="info-value">{platform}</span>
            <span class="info-label">System:</span>
            <span class="info-value">{system}</span>
            <span class="info-label">Python Version:</span>
            <span class="info-value">{python_version}</span>
            <span class="info-label">API Endpoint:</span>
            <span class="info-value"><a href='/api'>/api</a></span>
        </div>
        
        <h2>Browser Information</h2>
        <div id='browser'>
            <em>JavaScript required to display browser information</em>
        </div>
        
        <div class="footer">
            <p>Multi-Language Web Server Collection | Python Implementation</p>
        </div>
    </div>
    
    <script>
        const browserInfo = document.getElementById('browser');
        const info = [
            '<strong>User-Agent:</strong> ' + navigator.userAgent,
            '<strong>Platform:</strong> ' + navigator.platform,
            '<strong>Language:</strong> ' + navigator.language,
            '<strong>Languages:</strong> ' + navigator.languages.join(', '),
            '<strong>Cookies enabled:</strong> ' + navigator.cookieEnabled,
            '<strong>Screen resolution:</strong> ' + screen.width + 'x' + screen.height,
            '<strong>Color depth:</strong> ' + screen.colorDepth + ' bits',
            '<strong>Timezone:</strong> ' + Intl.DateTimeFormat().resolvedOptions().timeZone,
            '<strong>Online status:</strong> ' + (navigator.onLine ? 'Online' : 'Offline'),
            '<strong>Hardware concurrency:</strong> ' + (navigator.hardwareConcurrency || 'Unknown') + ' cores'
        ];
        browserInfo.innerHTML = info.join('<br>');
    </script>
</body>
</html>"""
        
        return html_template.format(
            port=PORT,
            platform=platform.platform(),
            system=system_info,
            python_version=python_version
        )
    
    def _create_api_response(self):
        """Create API JSON response"""
        now = datetime.now()
        
        # Determine platform identifier
        system = platform.system().lower()
        platform_map = {
            'windows': 'win32',
            'linux': 'linux',
            'darwin': 'darwin',
            'freebsd': 'freebsd'
        }
        platform_id = platform_map.get(system, system)
        
        return {
            "server_info": {
                "port": PORT,
                "platform": platform_id,
                "os": f"{platform.system()} {platform.release()}",
                "python_version": platform.python_version(),
                "datetime": now.strftime("%Y-%m-%d %H:%M:%S"),
                "timestamp": int(now.timestamp()),
                "status": "running",
                "language": "python"
            },
            "message": "Server API endpoint"
        }


class WebServer:
    """Main web server class"""
    
    def __init__(self, port=8080, host='localhost'):
        self.port = port
        self.host = host
        self.server = None
        self.server_thread = None
        self._running = False
    
    def start(self):
        """Start the web server"""
        try:
            self.server = HTTPServer((self.host, self.port), WebServerHandler)
            self._running = True
            
            print(f"Web server started on http://{self.host}:{self.port}")
            print(f"Main page: http://{self.host}:{self.port}/")
            print(f"API endpoint: http://{self.host}:{self.port}/api")
            print("Press Ctrl+C to stop the server")
            
            # Start server in a separate thread for better control
            self.server_thread = threading.Thread(target=self._serve_forever)
            self.server_thread.daemon = True
            self.server_thread.start()
            
            return True
            
        except OSError as e:
            if e.errno == 98 or "Address already in use" in str(e):
                print(f"Error: Port {self.port} is already in use")
            else:
                print(f"Error starting server: {e}")
            return False
        except Exception as e:
            print(f"Unexpected error: {e}")
            return False
    
    def _serve_forever(self):
        """Internal method to serve requests"""
        try:
            self.server.serve_forever()
        except KeyboardInterrupt:
            pass
    
    def stop(self):
        """Stop the web server gracefully"""
        if self.server:
            print("\nShutting down server...")
            self._running = False
            self.server.shutdown()
            self.server.server_close()
            if self.server_thread:
                self.server_thread.join(timeout=5)
            print("Server stopped")
    
    def is_running(self):
        """Check if server is running"""
        return self._running and self.server is not None


# Global configuration
PORT = 8080
HOST = 'localhost'


def main():
    """Main entry point"""
    import signal
    import sys
    
    # Create server instance
    server = WebServer(PORT, HOST)
    
    # Set up signal handlers for graceful shutdown
    def signal_handler(signum, frame):
        print(f"\nReceived signal {signum}")
        server.stop()
        sys.exit(0)
    
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)
    
    # Start the server
    if not server.start():
        sys.exit(1)
    
    try:
        # Keep the main thread alive
        while server.is_running():
            threading.Event().wait(1)
    except KeyboardInterrupt:
        pass
    finally:
        server.stop()


if __name__ == "__main__":
    main()