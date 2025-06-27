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
    
    char html_body[1800];
    int html_len = snprintf(
        html_body, sizeof(html_body),
        "<html><body>"
        "<h1>Hello, World!</h1>"
        "<p>Server info:</p>"
        "<ul>"
        "<li>Port: %d</li>"
#ifdef _WIN32
        "<li>Platform: Windows</li>"
#else
        "<li>Platform: Linux/Unix</li>"
#endif
        "<li>API endpoint: <a href='/api'>/api</a></li>"
        "</ul>"
        "<h2>Browser Info</h2>"
        "<p id='browser'></p>"
        "<script>"
        "document.getElementById('browser').innerHTML = "
        "'User-Agent: ' + navigator.userAgent + '<br>' + "
        "'Platform: ' + navigator.platform + '<br>' + "
        "'Language: ' + navigator.language + '<br>' + "
        "'Cookies enabled: ' + navigator.cookieEnabled;"
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