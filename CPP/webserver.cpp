#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cstring>
#include <ctime>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

class WebServer {
private:
    int server_fd;
    struct sockaddr_in server_addr;
    
#ifdef _WIN32
    WSADATA wsa_data;
#endif

    std::string getCurrentDateTime() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    
    long getCurrentTimestamp() const {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(
            now.time_since_epoch()).count();
    }
    
    std::string createHtmlResponse() const {
        std::ostringstream html;
        html << "<!DOCTYPE html>"
             << "<html lang=\"en\">"
             << "<head>"
             << "<meta charset=\"UTF-8\">"
             << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
             << "<title>C++ Web Server</title>"
             << "<style>"
             << "body {"
             << "font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"
             << "margin: 0; padding: 40px;"
             << "background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
             << "min-height: 100vh; color: #333;"
             << "}"
             << ".container {"
             << "max-width: 900px; margin: 0 auto; background-color: white;"
             << "padding: 40px; border-radius: 15px;"
             << "box-shadow: 0 10px 30px rgba(0,0,0,0.2);"
             << "}"
             << "h1 {"
             << "color: #2c3e50; text-align: center; margin-bottom: 10px;"
             << "font-size: 2.5em; font-weight: 300;"
             << "}"
             << ".language-badge {"
             << "display: inline-block;"
             << "background: linear-gradient(45deg, #00599c, #004482);"
             << "color: white; padding: 8px 16px; border-radius: 25px;"
             << "font-size: 0.9em; font-weight: bold; margin-left: 10px;"
             << "box-shadow: 0 2px 10px rgba(0,0,0,0.2);"
             << "}"
             << "h2 {"
             << "color: #34495e; border-bottom: 3px solid #3498db;"
             << "padding-bottom: 10px; margin-top: 40px;"
             << "}"
             << ".info-grid {"
             << "display: grid; grid-template-columns: auto 1fr;"
             << "gap: 15px 25px; margin: 25px 0;"
             << "background: linear-gradient(135deg, #f8f9fa, #e9ecef);"
             << "padding: 25px; border-radius: 10px;"
             << "border-left: 5px solid #3498db;"
             << "}"
             << ".info-label { font-weight: bold; color: #2c3e50; }"
             << ".info-value { color: #34495e; }"
             << "a {"
             << "color: #3498db; text-decoration: none; font-weight: 500;"
             << "transition: all 0.3s ease;"
             << "}"
             << "a:hover { color: #2980b9; text-decoration: underline; }"
             << "#browser {"
             << "background: linear-gradient(135deg, #e8f4f8, #d1ecf1);"
             << "padding: 20px; border-radius: 10px; margin-top: 15px;"
             << "border-left: 5px solid #17a2b8;"
             << "font-family: 'Courier New', monospace; font-size: 0.9em;"
             << "}"
             << ".footer {"
             << "text-align: center; margin-top: 40px; padding-top: 20px;"
             << "border-top: 1px solid #dee2e6; color: #6c757d; font-size: 0.9em;"
             << "}"
             << "</style>"
             << "</head>"
             << "<body>"
             << "<div class=\"container\">"
             << "<h1>Hello, World! <span class=\"language-badge\">C++</span></h1>"
             << "<h2>Server Information</h2>"
             << "<div class=\"info-grid\">"
             << "<span class=\"info-label\">Port:</span>"
             << "<span class=\"info-value\">" << PORT << "</span>"
             << "<span class=\"info-label\">Platform:</span>"
#ifdef _WIN32
             << "<span class=\"info-value\">Windows</span>"
#else
             << "<span class=\"info-value\">Linux/Unix</span>"
#endif
             << "<span class=\"info-label\">API Endpoint:</span>"
             << "<span class=\"info-value\"><a href='/api'>/api</a></span>"
             << "</div>"
             << "<h2>Browser Information</h2>"
             << "<div id='browser'><em>JavaScript required to display browser information</em></div>"
             << "<div class=\"footer\">"
             << "<p>Multi-Language Web Server Collection | C++ Implementation</p>"
             << "</div>"
             << "</div>"
             << "<script>"
             << "const browserInfo = document.getElementById('browser');"
             << "const info = ["
             << "'<strong>User-Agent:</strong> ' + navigator.userAgent,"
             << "'<strong>Platform:</strong> ' + navigator.platform,"
             << "'<strong>Language:</strong> ' + navigator.language,"
             << "'<strong>Languages:</strong> ' + navigator.languages.join(', '),"
             << "'<strong>Cookies enabled:</strong> ' + navigator.cookieEnabled,"
             << "'<strong>Screen resolution:</strong> ' + screen.width + 'x' + screen.height,"
             << "'<strong>Color depth:</strong> ' + screen.colorDepth + ' bits',"
             << "'<strong>Timezone:</strong> ' + Intl.DateTimeFormat().resolvedOptions().timeZone,"
             << "'<strong>Online status:</strong> ' + (navigator.onLine ? 'Online' : 'Offline'),"
             << "'<strong>Hardware concurrency:</strong> ' + (navigator.hardwareConcurrency || 'Unknown') + ' cores'"
             << "];"
             << "browserInfo.innerHTML = info.join('<br>');"
             << "</script>"
             << "</body></html>";
        
        std::string body = html.str();
        std::ostringstream response;
        response << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: text/html\r\n"
                 << "Connection: close\r\n"
                 << "Content-Length: " << body.length() << "\r\n"
                 << "\r\n"
                 << body;
        
        return response.str();
    }
    
    std::string createApiResponse() const {
        std::ostringstream json;
        json << "{"
             << "\"server_info\":{"
             << "\"port\":" << PORT << ","
#ifdef _WIN32
             << "\"platform\":\"win32\","
             << "\"os\":\"Windows\","
#else
             << "\"platform\":\"unix\","
             << "\"os\":\"Linux/Unix\","
#endif
             << "\"datetime\":\"" << getCurrentDateTime() << "\","
             << "\"timestamp\":" << getCurrentTimestamp() << ","
             << "\"status\":\"running\","
             << "\"language\":\"cpp\""
             << "},"
             << "\"message\":\"Server API endpoint\""
             << "}";
        
        std::string body = json.str();
        std::ostringstream response;
        response << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Connection: close\r\n"
                 << "Content-Length: " << body.length() << "\r\n"
                 << "\r\n"
                 << body;
        
        return response.str();
    }

public:
    WebServer() : server_fd(-1) {
#ifdef _WIN32
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
#endif
    }
    
    ~WebServer() {
        stop();
#ifdef _WIN32
        WSACleanup();
#endif
    }
    
    bool start() {
        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            std::cerr << "Socket creation failed" << std::endl;
            return false;
        }
        
        // Set socket options
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, 
                      reinterpret_cast<char*>(&opt), sizeof(opt)) < 0) {
            std::cerr << "Setsockopt failed" << std::endl;
            return false;
        }
        
        // Configure server address
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);
        
        // Bind socket
        if (bind(server_fd, reinterpret_cast<struct sockaddr*>(&server_addr), 
                sizeof(server_addr)) < 0) {
            std::cerr << "Bind failed" << std::endl;
            return false;
        }
        
        // Listen for connections
        if (listen(server_fd, 3) < 0) {
            std::cerr << "Listen failed" << std::endl;
            return false;
        }
        
        std::cout << "Web server started on port " << PORT << std::endl;
        return true;
    }
    
    void run() {
        while (true) {
            struct sockaddr_in client_addr;
#ifdef _WIN32
            int client_len = sizeof(client_addr);
#else
            socklen_t client_len = sizeof(client_addr);
#endif
            
            int client_fd = accept(server_fd, 
                                 reinterpret_cast<struct sockaddr*>(&client_addr), 
                                 &client_len);
            
            if (client_fd < 0) {
                std::cerr << "Accept failed" << std::endl;
                continue;
            }
            
            handleClient(client_fd);
            
#ifdef _WIN32
            closesocket(client_fd);
#else
            close(client_fd);
#endif
        }
    }
    
private:
    void handleClient(int client_fd) {
        char buffer[BUFFER_SIZE];
        int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received <= 0) {
            return;
        }
        
        buffer[bytes_received] = '\0';
        std::string request(buffer);
        
        std::string response;
        if (request.find("GET /api") == 0) {
            response = createApiResponse();
        } else {
            response = createHtmlResponse();
        }
        
        send(client_fd, response.c_str(), response.length(), 0);
    }
    
    void stop() {
        if (server_fd >= 0) {
#ifdef _WIN32
            closesocket(server_fd);
#else
            close(server_fd);
#endif
            server_fd = -1;
        }
    }
};

int main() {
    try {
        WebServer server;
        
        if (!server.start()) {
            std::cerr << "Failed to start server" << std::endl;
            return 1;
        }
        
        server.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}