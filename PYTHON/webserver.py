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
        body {{ font-family: Arial, sans-serif; margin: 40px; }}
        ul {{ list-style-type: disc; margin-left: 20px; }}
        a {{ color: #0066cc; text-decoration: none; }}
        a:hover {{ text-decoration: underline; }}
        #browser {{ background-color: #f5f5f5; padding: 10px; border-radius: 5px; }}
    </style>
</head>
<body>
    <h1>Hello, World!</h1>
    <p>Server info:</p>
    <ul>
        <li>Port: {port}</li>
        <li>Platform: {platform}</li>
        <li>System: {system}</li>
        <li>Python Version: {python_version}</li>
        <li>API endpoint: <a href='/api'>/api</a></li>
    </ul>
    <h2>Browser Info</h2>
    <div id='browser'>JavaScript required to display browser information</div>
    <script>
        document.getElementById('browser').innerHTML = 
            '<strong>User-Agent:</strong> ' + navigator.userAgent + '<br>' + 
            '<strong>Platform:</strong> ' + navigator.platform + '<br>' + 
            '<strong>Language:</strong> ' + navigator.language + '<br>' + 
            '<strong>Cookies enabled:</strong> ' + navigator.cookieEnabled + '<br>' +
            '<strong>Screen resolution:</strong> ' + screen.width + 'x' + screen.height;
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