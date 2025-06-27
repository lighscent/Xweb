#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#define PORT 8080

int main() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    int server_fd, client_fd;
    struct sockaddr_in server, client;
    char buffer[1024];
    
    char html_body[4000];
    int html_len = snprintf(
        html_body, sizeof(html_body),
        "<!DOCTYPE html>"
        "<html lang=\"en\">"
        "<head>"
        "<meta charset=\"UTF-8\">"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "<title>C Web Server</title>"
        "<style>"
        "body {"
        "font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"
        "margin: 0; padding: 40px;"
        "background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%);"
        "min-height: 100vh; color: #333;"
        "}"
        ".container {"
        "max-width: 900px; margin: 0 auto; background-color: white;"
        "padding: 40px; border-radius: 15px;"
        "box-shadow: 0 10px 30px rgba(0,0,0,0.2);"
        "}"
        "h1 {"
        "color: #2c3e50; text-align: center; margin-bottom: 10px;"
        "font-size: 2.5em; font-weight: 300;"
        "}"
        ".language-badge {"
        "display: inline-block;"
        "background: linear-gradient(45deg, #a8b9cc, #00599c);"
        "color: white; padding: 8px 16px; border-radius: 25px;"
        "font-size: 0.9em; font-weight: bold; margin-left: 10px;"
        "box-shadow: 0 2px 10px rgba(0,0,0,0.2);"
        "}"
        "h2 {"
        "color: #34495e; border-bottom: 3px solid #3498db;"
        "padding-bottom: 10px; margin-top: 40px;"
        "}"
        ".info-grid {"
        "display: grid; grid-template-columns: auto 1fr;"
        "gap: 15px 25px; margin: 25px 0;"
        "background: linear-gradient(135deg, #f8f9fa, #e9ecef);"
        "padding: 25px; border-radius: 10px;"
        "border-left: 5px solid #3498db;"
        "}"
        ".info-label { font-weight: bold; color: #2c3e50; }"
        ".info-value { color: #34495e; }"
        "a {"
        "color: #3498db; text-decoration: none; font-weight: 500;"
        "transition: all 0.3s ease;"
        "}"
        "a:hover { color: #2980b9; text-decoration: underline; }"
        "#browser {"
        "background: linear-gradient(135deg, #e8f4f8, #d1ecf1);"
        "padding: 20px; border-radius: 10px; margin-top: 15px;"
        "border-left: 5px solid #17a2b8;"
        "font-family: 'Courier New', monospace; font-size: 0.9em;"
        "}"
        ".footer {"
        "text-align: center; margin-top: 40px; padding-top: 20px;"
        "border-top: 1px solid #dee2e6; color: #6c757d; font-size: 0.9em;"
        "}"
        "</style>"
        "</head>"
        "<body>"
        "<div class=\"container\">"
        "<h1>Hello, World! <span class=\"language-badge\">C</span></h1>"
        "<h2>Server Information</h2>"
        "<div class=\"info-grid\">"
        "<span class=\"info-label\">Port:</span>"
        "<span class=\"info-value\">%d</span>"
        "<span class=\"info-label\">Platform:</span>"
#ifdef _WIN32
        "<span class=\"info-value\">Windows</span>"
#else
        "<span class=\"info-value\">Linux/Unix</span>"
#endif
        "<span class=\"info-label\">API Endpoint:</span>"
        "<span class=\"info-value\"><a href='/api'>/api</a></span>"
        "</div>"
        "<h2>Browser Information</h2>"
        "<div id='browser'><em>JavaScript required to display browser information</em></div>"
        "<div class=\"footer\">"
        "<p>Multi-Language Web Server Collection | C Implementation</p>"
        "</div>"
        "</div>"
        "<script>"
        "const browserInfo = document.getElementById('browser');"
        "const info = ["
        "'<strong>User-Agent:</strong> ' + navigator.userAgent,"
        "'<strong>Platform:</strong> ' + navigator.platform,"
        "'<strong>Language:</strong> ' + navigator.language,"
        "'<strong>Languages:</strong> ' + navigator.languages.join(', '),"
        "'<strong>Cookies enabled:</strong> ' + navigator.cookieEnabled,"
        "'<strong>Screen resolution:</strong> ' + screen.width + 'x' + screen.height,"
        "'<strong>Color depth:</strong> ' + screen.colorDepth + ' bits',"
        "'<strong>Timezone:</strong> ' + Intl.DateTimeFormat().resolvedOptions().timeZone,"
        "'<strong>Online status:</strong> ' + (navigator.onLine ? 'Online' : 'Offline'),"
        "'<strong>Hardware concurrency:</strong> ' + (navigator.hardwareConcurrency || 'Unknown') + ' cores'"
        "];"
        "browserInfo.innerHTML = info.join('<br>');"
        "</script>"
        "</body></html>",
        PORT
    );
    
    char final_response[2048];
    snprintf(
        final_response, sizeof(final_response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        html_len,
        html_body
    );

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("bind failed");
        return 1;
    }

    listen(server_fd, 3);

    printf("Web server started on port %d\n", PORT);

    while (1) {
#ifdef _WIN32
        int addrlen = sizeof(client);
#else
        socklen_t addrlen = sizeof(client);
#endif
        client_fd = accept(server_fd, (struct sockaddr*)&client, &addrlen);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }
        int recv_len = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (recv_len > 0) {
            buffer[recv_len] = '\0';
            if (strncmp(buffer, "GET /api", 8) == 0) {
                time_t rawtime;
                struct tm * timeinfo;
                char time_str[80];
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
                
                char response_api[512];
                int api_json_len = snprintf(
                    response_api + 100, 400,
                    "{"
                    "\"server_info\":{"
                    "\"port\":%d,"
                    "\"platform\":\"%s\","
#ifdef _WIN32
                    "\"os\":\"Windows\","
#else
                    "\"os\":\"Linux/Unix\","
#endif
                    "\"datetime\":\"%s\","
                    "\"timestamp\":%ld,"
                    "\"status\":\"running\""
                    "},"
                    "\"message\":\"Server API endpoint\""
                    "}",
                    PORT,
#ifdef _WIN32
                    "win32",
#else
                    "unix",
#endif
                    time_str,
                    (long)rawtime
                );
                
                snprintf(
                    response_api, sizeof(response_api),
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: %d\r\n"
                    "\r\n"
                    "%s",
                    api_json_len,
                    response_api + 100
                );
                
                send(client_fd, response_api, strlen(response_api), 0);
            } else {
                send(client_fd, final_response, strlen(final_response), 0);
            }
        }
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
    }

#ifdef _WIN32
    closesocket(server_fd);
    WSACleanup();
#else
    close(server_fd);
#endif
    return 0;
}