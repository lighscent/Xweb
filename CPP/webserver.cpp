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
        html << "<html><body>"
             << "<h1>Hello, World!</h1>"
             << "<p>Server info:</p>"
             << "<ul>"
             << "<li>Port: " << PORT << "</li>"
#ifdef _WIN32
             << "<li>Platform: Windows</li>"
#else
             << "<li>Platform: Linux/Unix</li>"
#endif
             << "<li>API endpoint: <a href='/api'>/api</a></li>"
             << "</ul>"
             << "<h2>Browser Info</h2>"
             << "<p id='browser'></p>"
             << "<script>"
             << "document.getElementById('browser').innerHTML = "
             << "'User-Agent: ' + navigator.userAgent + '<br>' + "
             << "'Platform: ' + navigator.platform + '<br>' + "
             << "'Language: ' + navigator.language + '<br>' + "
             << "'Cookies enabled: ' + navigator.cookieEnabled;"
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