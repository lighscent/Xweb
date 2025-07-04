package main

import (
	"encoding/json"
	"fmt"
	"html/template"
	"log"
	"net/http"
	"os"
	"os/signal"
	"runtime"
	"strconv"
	"strings"
	"syscall"
	"time"
)

const (
    PORT = 8080
    HOST = "localhost"
)

// ServerInfo represents the server information for the API response
type ServerInfo struct {
    Port         int    `json:"port"`
    Platform     string `json:"platform"`
    OS           string `json:"os"`
    GoVersion    string `json:"go_version"`
    Architecture string `json:"architecture"`
    DateTime     string `json:"datetime"`
    Timestamp    int64  `json:"timestamp"`
    Status       string `json:"status"`
    Language     string `json:"language"`
}

// APIResponse represents the complete API response structure
type APIResponse struct {
    ServerInfo ServerInfo `json:"server_info"`
    Message    string     `json:"message"`
}

// WebServer handles HTTP requests and server management
type WebServer struct {
    server *http.Server
    mux    *http.ServeMux
}

// NewWebServer creates a new web server instance
func NewWebServer() *WebServer {
    mux := http.NewServeMux()
    server := &http.Server{
        Addr:         fmt.Sprintf("%s:%d", HOST, PORT),
        Handler:      mux,
        ReadTimeout:  10 * time.Second,
        WriteTimeout: 10 * time.Second,
        IdleTimeout:  120 * time.Second,
    }

    ws := &WebServer{
        server: server,
        mux:    mux,
    }

    ws.setupRoutes()
    return ws
}

// setupRoutes configures the HTTP routes
func (ws *WebServer) setupRoutes() {
    ws.mux.HandleFunc("/", ws.handleRoot)
    ws.mux.HandleFunc("/api", ws.handleAPI)
}

// Start begins listening for HTTP requests
func (ws *WebServer) Start() error {
    fmt.Printf("Web server started on http://%s:%d\n", HOST, PORT)
    fmt.Printf("Main page: http://%s:%d/\n", HOST, PORT)
    fmt.Printf("API endpoint: http://%s:%d/api\n", HOST, PORT)
    fmt.Println("Press Ctrl+C to stop the server")

    return ws.server.ListenAndServe()
}

// Stop gracefully shuts down the server
func (ws *WebServer) Stop() error {
    fmt.Println("\nShutting down server...")
    return ws.server.Close()
}

// handleRoot serves the HTML page
func (ws *WebServer) handleRoot(w http.ResponseWriter, r *http.Request) {
    if r.Method != http.MethodGet {
        http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
        return
    }

    htmlContent := ws.createHTMLResponse()

    w.Header().Set("Content-Type", "text/html; charset=utf-8")
    w.Header().Set("Connection", "close")
    w.Header().Set("Content-Length", strconv.Itoa(len(htmlContent)))
    w.WriteHeader(http.StatusOK)

    if _, err := w.Write([]byte(htmlContent)); err != nil {
        log.Printf("Error writing HTML response: %v", err)
    }
}

// handleAPI serves the JSON API response
func (ws *WebServer) handleAPI(w http.ResponseWriter, r *http.Request) {
    if r.Method != http.MethodGet {
        http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
        return
    }

    apiResponse := ws.createAPIResponse()

    jsonData, err := json.MarshalIndent(apiResponse, "", "  ")
    if err != nil {
        http.Error(w, "Internal server error", http.StatusInternalServerError)
        log.Printf("Error marshaling JSON: %v", err)
        return
    }

    w.Header().Set("Content-Type", "application/json; charset=utf-8")
    w.Header().Set("Connection", "close")
    w.Header().Set("Content-Length", strconv.Itoa(len(jsonData)))
    w.WriteHeader(http.StatusOK)

    if _, err := w.Write(jsonData); err != nil {
        log.Printf("Error writing JSON response: %v", err)
    }
}

// createHTMLResponse generates the HTML page content
func (ws *WebServer) createHTMLResponse() string {
    const htmlTemplate = `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Go Web Server</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 40px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            color: #333;
        }
        .container {
            max-width: 900px;
            margin: 0 auto;
            background-color: white;
            padding: 40px;
            border-radius: 15px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
        }
        h1 {
            color: #2c3e50;
            text-align: center;
            margin-bottom: 10px;
            font-size: 2.5em;
            font-weight: 300;
        }
        .language-badge {
            display: inline-block;
            background: linear-gradient(45deg, #f7df1e, #f0db4f);
            color: black;
            padding: 8px 16px;
            border-radius: 25px;
            font-size: 0.9em;
            font-weight: bold;
            margin-left: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.2);
        }
        h2 {
            color: #34495e;
            border-bottom: 3px solid #3498db;
            padding-bottom: 10px;
            margin-top: 40px;
        }
        .info-grid {
            display: grid;
            grid-template-columns: auto 1fr;
            gap: 15px 25px;
            margin: 25px 0;
            background: linear-gradient(135deg, #f8f9fa, #e9ecef);
            padding: 25px;
            border-radius: 10px;
            border-left: 5px solid #3498db;
        }
        .info-label {
            font-weight: bold;
            color: #2c3e50;
        }
        .info-value {
            color: #34495e;
        }
        a {
            color: #3498db;
            text-decoration: none;
            font-weight: 500;
            transition: all 0.3s ease;
        }
        a:hover {
            color: #2980b9;
            text-decoration: underline;
        }
        #browser {
            background: linear-gradient(135deg, #e8f4f8, #d1ecf1);
            padding: 20px;
            border-radius: 10px;
            margin-top: 15px;
            border-left: 5px solid #17a2b8;
            font-family: 'Courier New', monospace;
            font-size: 0.9em;
        }
        .footer {
            text-align: center;
            margin-top: 40px;
            padding-top: 20px;
            border-top: 1px solid #dee2e6;
            color: #6c757d;
            font-size: 0.9em;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Hello, World! <span class="language-badge">Go</span></h1>
        
        <h2>Server Information</h2>
        <div class="info-grid">
            <span class="info-label">Port:</span>
            <span class="info-value">{{.Port}}</span>
            <span class="info-label">Platform:</span>
            <span class="info-value">{{.Platform}}</span>
            <span class="info-label">Operating System:</span>
            <span class="info-value">{{.OS}}</span>
            <span class="info-label">Go Version:</span>
            <span class="info-value">{{.GoVersion}}</span>
            <span class="info-label">Architecture:</span>
            <span class="info-value">{{.Architecture}}</span>
            <span class="info-label">API Endpoint:</span>
            <span class="info-value"><a href='/api'>/api</a></span>
        </div>
        
        <h2>Browser Information</h2>
        <div id='browser'>
            <em>JavaScript required to display browser information</em>
        </div>

        <div class="footer">
            <p>Multi-Language Web Server Collection | JavaScript Implementation</p>
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
    </div>
</body>
</html>`

    tmpl, err := template.New("html").Parse(htmlTemplate)
    if err != nil {
        log.Printf("Error parsing HTML template: %v", err)
        return "<html><body><h1>Template Error</h1></body></html>"
    }

    data := struct {
        Port         int
        Platform     string
        OS           string
        GoVersion    string
        Architecture string
    }{
        Port:         PORT,
        Platform:     ws.getPlatformName(),
        OS:           ws.getOSName(),
        GoVersion:    runtime.Version(),
        Architecture: runtime.GOARCH,
    }

    var buf strings.Builder
    if err := tmpl.Execute(&buf, data); err != nil {
        log.Printf("Error executing HTML template: %v", err)
        return "<html><body><h1>Template Execution Error</h1></body></html>"
    }

    return buf.String()
}

// createAPIResponse generates the JSON API response
func (ws *WebServer) createAPIResponse() APIResponse {
    now := time.Now()

    return APIResponse{
        ServerInfo: ServerInfo{
            Port:         PORT,
            Platform:     ws.getPlatformID(),
            OS:           ws.getOSName(),
            GoVersion:    runtime.Version(),
            Architecture: runtime.GOARCH,
            DateTime:     now.Format("2006-01-02 15:04:05"),
            Timestamp:    now.Unix(),
            Status:       "running",
            Language:     "go",
        },
        Message: "Server API endpoint",
    }
}

// getPlatformName returns a human-readable platform name
func (ws *WebServer) getPlatformName() string {
    switch runtime.GOOS {
    case "windows":
        return "Windows"
    case "darwin":
        return "macOS"
    case "linux":
        return "Linux"
    case "freebsd":
        return "FreeBSD"
    case "openbsd":
        return "OpenBSD"
    case "netbsd":
        return "NetBSD"
    default:
        return strings.Title(runtime.GOOS)
    }
}

// getPlatformID returns a platform identifier for API responses
func (ws *WebServer) getPlatformID() string {
    switch runtime.GOOS {
    case "windows":
        return "win32"
    case "darwin":
        return "darwin"
    case "linux":
        return "linux"
    case "freebsd":
        return "freebsd"
    default:
        return runtime.GOOS
    }
}

// getOSName returns detailed OS information
func (ws *WebServer) getOSName() string {
    goos := runtime.GOOS
    switch goos {
    case "windows":
        return "Windows"
    case "darwin":
        return "macOS"
    case "linux":
        // Try to get more specific Linux distribution info
        if distro := ws.getLinuxDistribution(); distro != "" {
            return distro
        }
        return "Linux"
    default:
        return strings.Title(goos)
    }
}

// getLinuxDistribution attempts to detect the Linux distribution
func (ws *WebServer) getLinuxDistribution() string {
    // Try to read /etc/os-release
    if data, err := os.ReadFile("/etc/os-release"); err == nil {
        content := string(data)
        lines := strings.Split(content, "\n")
        for _, line := range lines {
            if strings.HasPrefix(line, "PRETTY_NAME=") {
                // Remove PRETTY_NAME= and quotes
                name := strings.TrimPrefix(line, "PRETTY_NAME=")
                name = strings.Trim(name, `"`)
                return name
            }
        }
    }
    return ""
}

// setupSignalHandling configures graceful shutdown on interrupt signals
func setupSignalHandling(server *WebServer) {
    sigChan := make(chan os.Signal, 1)
    signal.Notify(sigChan, syscall.SIGINT, syscall.SIGTERM)

    go func() {
        sig := <-sigChan
        log.Printf("Received signal: %v", sig)
        if err := server.Stop(); err != nil {
            log.Printf("Error stopping server: %v", err)
        }
        os.Exit(0)
    }()
}

func main() {
    // Create and configure the web server
    server := NewWebServer()

    // Setup graceful shutdown
    setupSignalHandling(server)

    // Start the server
    if err := server.Start(); err != nil && err != http.ErrServerClosed {
        log.Printf("Server error: %v", err)
        os.Exit(1)
    }
}