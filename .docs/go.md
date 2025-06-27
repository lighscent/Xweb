# Go Web Server Implementation

## Overview

HTTP server implementation using Go's built-in `net/http` package with modern Go idioms and patterns. This implementation demonstrates Go best practices with goroutines, channels, interfaces, and comprehensive error handling.

## Prerequisites

### All Platforms
- **Go 1.19** or later
- No external dependencies (uses standard library only)

### Installation Check
```bash
# Check Go version
go version

# Ensure GOPATH and GOROOT are set (optional with Go modules)
go env GOPATH
go env GOROOT
```

## Quick Start

### Initialize Module
```bash
# Navigate to GO directory
cd GO

# Initialize Go module (if not already done)
go mod init webserver

# Verify dependencies
go mod tidy
```

### Build
```bash
# Build the executable
go build -o webserver webserver.go

# Build for specific platform
GOOS=windows GOARCH=amd64 go build -o webserver.exe webserver.go
GOOS=linux GOARCH=amd64 go build -o webserver webserver.go
GOOS=darwin GOARCH=amd64 go build -o webserver webserver.go
```

### Run
```bash
# Run directly
go run webserver.go

# Run built executable
./webserver        # Linux/macOS
webserver.exe      # Windows

# Run with race detection (development)
go run -race webserver.go
```

### Test
- Main page: `http://localhost:8080/`
- API endpoint: `http://localhost:8080/api`

## Features

- HTTP/1.1 server on port 8080
- Idiomatic Go code with proper error handling
- Concurrent request handling with goroutines
- Structured logging and graceful shutdown
- Cross-platform compilation support
- Enhanced HTML page with Go-specific styling
- JSON API endpoint with comprehensive system information
- Real-time browser information display
- Signal handling for graceful shutdown (SIGINT, SIGTERM)
- Template-based HTML generation
- Comprehensive platform detection

## Code Structure

### Main Server Struct
```go
type WebServer struct {
    server *http.Server
    mux    *http.ServeMux
}

func NewWebServer() *WebServer {
    // Constructor pattern
}

func (ws *WebServer) Start() error {
    // Start HTTP server
}

func (ws *WebServer) Stop() error {
    // Graceful shutdown
}
```

### Data Models
```go
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

type APIResponse struct {
    ServerInfo ServerInfo `json:"server_info"`
    Message    string     `json:"message"`
}
```

### HTTP Request Handling
```go
func (ws *WebServer) handleAPI(w http.ResponseWriter, r *http.Request) {
    if r.Method != http.MethodGet {
        http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
        return
    }

    apiResponse := ws.createAPIResponse()
    jsonData, err := json.MarshalIndent(apiResponse, "", "  ")
    if err != nil {
        http.Error(w, "Internal server error", http.StatusInternalServerError)
        return
    }

    w.Header().Set("Content-Type", "application/json; charset=utf-8")
    w.WriteHeader(http.StatusOK)
    w.Write(jsonData)
}
```

### Template-based HTML Generation
```go
const htmlTemplate = `<!DOCTYPE html>
<html>
<head>
    <title>Go Web Server</title>
</head>
<body>
    <h1>Port: {{.Port}}</h1>
    <p>Go Version: {{.GoVersion}}</p>
</body>
</html>`

tmpl, err := template.New("html").Parse(htmlTemplate)
tmpl.Execute(&buf, data)
```

### Signal Handling for Graceful Shutdown
```go
func setupSignalHandling(server *WebServer) {
    sigChan := make(chan os.Signal, 1)
    signal.Notify(sigChan, syscall.SIGINT, syscall.SIGTERM)

    go func() {
        sig := <-sigChan
        log.Printf("Received signal: %v", sig)
        server.Stop()
        os.Exit(0)
    }()
}
```

### Runtime Information Gathering
```go
// Platform detection
func (ws *WebServer) getPlatformID() string {
    switch runtime.GOOS {
    case "windows": return "win32"
    case "darwin":  return "darwin"
    case "linux":   return "linux"
    default:        return runtime.GOOS
    }
}

// System information
goVersion := runtime.Version()        // Go version
architecture := runtime.GOARCH       // CPU architecture
platform := runtime.GOOS            // Operating system
```

### API Response Format
```json
{
  "server_info": {
    "port": 8080,
    "platform": "linux",
    "os": "Ubuntu 22.04.1 LTS",
    "go_version": "go1.21.0",
    "architecture": "amd64",
    "datetime": "2025-06-27 14:30:00",
    "timestamp": 1719500200,
    "status": "running",
    "language": "go"
  },
  "message": "Server API endpoint"
}
```

## Common Issues

### Go Not Installed
```bash
# Install Go from official website: https://golang.org/dl/

# Or using package managers:
# Windows (Chocolatey)
choco install golang

# macOS (Homebrew)
brew install go

# Linux (Ubuntu/Debian)
sudo apt update
sudo apt install golang-go

# Linux (CentOS/RHEL)
sudo yum install golang
```

### Module Issues
```bash
# Initialize module
go mod init webserver

# Clean module cache
go clean -modcache

# Update dependencies
go get -u
go mod tidy
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

### Build Issues
```bash
# Clean build cache
go clean -cache

# Build with verbose output
go build -v webserver.go

# Check for race conditions
go build -race webserver.go
```

### Cross-compilation Issues
```bash
# List supported platforms
go tool dist list

# Build for Windows from Linux
GOOS=windows GOARCH=amd64 go build webserver.go

# Build for Linux from Windows
set GOOS=linux
set GOARCH=amd64
go build webserver.go
```

## Debug Mode

### Development Build
```bash
# Build with debug symbols
go build -gcflags="all=-N -l" webserver.go

# Run with race detection
go run -race webserver.go

# Run with memory profiling
go run webserver.go -memprofile=mem.prof

# Run with CPU profiling
go run webserver.go -cpuprofile=cpu.prof
```

### Debugging Tools
```bash
# Use Delve debugger
go install github.com/go-delve/delve/cmd/dlv@latest
dlv debug webserver.go

# Memory analysis
go tool pprof mem.prof

# CPU analysis
go tool pprof cpu.prof
```

## Advanced Features

### Concurrent Request Handling
```go
// Go's net/http automatically handles concurrent requests with goroutines
server := &http.Server{
    Addr:         ":8080",
    Handler:      mux,
    ReadTimeout:  10 * time.Second,
    WriteTimeout: 10 * time.Second,
    IdleTimeout:  120 * time.Second,
}
```

### Context-based Request Handling
```go
func (ws *WebServer) handleWithContext(w http.ResponseWriter, r *http.Request) {
    ctx := r.Context()
    
    select {
    case <-ctx.Done():
        http.Error(w, "Request cancelled", http.StatusRequestTimeout)
        return
    default:
        // Handle request normally
    }
}
```

### Middleware Support
```go
func loggingMiddleware(next http.Handler) http.Handler {
    return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
        start := time.Now()
        next.ServeHTTP(w, r)
        log.Printf("%s %s %v", r.Method, r.URL.Path, time.Since(start))
    })
}

// Apply middleware
mux.Handle("/", loggingMiddleware(http.HandlerFunc(ws.handleRoot)))
```

### Custom Error Handling
```go
type ErrorResponse struct {
    Error   string `json:"error"`
    Code    int    `json:"code"`
    Message string `json:"message"`
}

func (ws *WebServer) sendErrorJSON(w http.ResponseWriter, code int, message string) {
    w.Header().Set("Content-Type", "application/json")
    w.WriteHeader(code)
    
    errorResp := ErrorResponse{
        Error:   http.StatusText(code),
        Code:    code,
        Message: message,
    }
    
    json.NewEncoder(w).Encode(errorResp)
}
```

### Environment Configuration
```go
import "os"

func getConfig() (string, int) {
    host := os.Getenv("SERVER_HOST")
    if host == "" {
        host = "localhost"
    }
    
    portStr := os.Getenv("SERVER_PORT")
    port := 8080
    if portStr != "" {
        if p, err := strconv.Atoi(portStr); err == nil {
            port = p
        }
    }
    
    return host, port
}
```

## Performance Considerations

- Go's goroutines provide excellent concurrent request handling
- Built-in HTTP server is production-ready
- Garbage collector optimized for low latency
- Efficient memory usage with value types
- Fast compilation and startup times
- Consider using `sync.Pool` for object reuse in high-traffic scenarios

## Testing

### Unit Testing
```go
// webserver_test.go
package main

import (
    "encoding/json"
    "net/http"
    "net/http/httptest"
    "testing"
)

func TestAPIEndpoint(t *testing.T) {
    server := NewWebServer()
    
    req, err := http.NewRequest("GET", "/api", nil)
    if err != nil {
        t.Fatal(err)
    }
    
    rr := httptest.NewRecorder()
    server.mux.ServeHTTP(rr, req)
    
    if status := rr.Code; status != http.StatusOK {
        t.Errorf("handler returned wrong status code: got %v want %v",
            status, http.StatusOK)
    }
    
    var response APIResponse
    if err := json.Unmarshal(rr.Body.Bytes(), &response); err != nil {
        t.Errorf("could not parse response: %v", err)
    }
    
    if response.ServerInfo.Language != "go" {
        t.Errorf("expected language 'go', got '%s'", response.ServerInfo.Language)
    }
}

func TestHTMLEndpoint(t *testing.T) {
    server := NewWebServer()
    
    req, err := http.NewRequest("GET", "/", nil)
    if err != nil {
        t.Fatal(err)
    }
    
    rr := httptest.NewRecorder()
    server.mux.ServeHTTP(rr, req)
    
    if status := rr.Code; status != http.StatusOK {
        t.Errorf("handler returned wrong status code: got %v want %v",
            status, http.StatusOK)
    }
    
    if !strings.Contains(rr.Body.String(), "Hello, World!") {
        t.Errorf("response should contain 'Hello, World!'")
    }
}
```

### Benchmark Testing
```go
func BenchmarkAPIEndpoint(b *testing.B) {
    server := NewWebServer()
    
    req, _ := http.NewRequest("GET", "/api", nil)
    
    b.ResetTimer()
    for i := 0; i < b.N; i++ {
        rr := httptest.NewRecorder()
        server.mux.ServeHTTP(rr, req)
    }
}
```

### Load Testing
```bash
# Run tests
go test -v

# Run benchmarks
go test -bench=.

# Test with race detection
go test -race

# Generate coverage report
go test -cover
go test -coverprofile=coverage.out
go tool cover -html=coverage.out
```

## Limitations

- Single binary deployment (pro and con)
- Requires Go runtime for development
- Larger binary size compared to some compiled languages
- No built-in ORM or web framework features
- Basic HTTP features only (no HTTP/2 push, WebSockets, etc.)

## Extensions

### Adding HTTP/2 Support
```go
import "golang.org/x/net/http2"

server := &http.Server{
    Addr:    ":8080",
    Handler: mux,
}

// Enable HTTP/2
http2.ConfigureServer(server, &http2.Server{})
```

### Adding HTTPS Support
```go
func (ws *WebServer) StartTLS(certFile, keyFile string) error {
    return ws.server.ListenAndServeTLS(certFile, keyFile)
}

// Generate self-signed certificate for testing
// go run $GOROOT/src/crypto/tls/generate_cert.go --host=localhost
```

### Adding WebSocket Support
```go
// Install gorilla/websocket
// go get github.com/gorilla/websocket

import "github.com/gorilla/websocket"

var upgrader = websocket.Upgrader{
    CheckOrigin: func(r *http.Request) bool {
        return true // Allow all origins in development
    },
}

func (ws *WebServer) handleWebSocket(w http.ResponseWriter, r *http.Request) {
    conn, err := upgrader.Upgrade(w, r, nil)
    if err != nil {
        log.Print("upgrade failed: ", err)
        return
    }
    defer conn.Close()
    
    // Handle WebSocket connection
}
```

### Configuration with Viper
```go
// go get github.com/spf13/viper

import "github.com/spf13/viper"

func loadConfig() {
    viper.SetDefault("server.port", 8080)
    viper.SetDefault("server.host", "localhost")
    
    viper.SetConfigName("config")
    viper.SetConfigType("yaml")
    viper.AddConfigPath(".")
    
    if err := viper.ReadInConfig(); err != nil {
        log.Printf("Error reading config file: %v", err)
    }
}
```

## Production Deployment

### Building for Production
```bash
# Build optimized binary
go build -ldflags="-w -s" -o webserver webserver.go

# Build with version information
VERSION=$(git describe --tags --always)
go build -ldflags="-w -s -X main.version=${VERSION}" webserver.go

# Cross-compile for different platforms
GOOS=linux GOARCH=amd64 go build -o webserver-linux webserver.go
GOOS=windows GOARCH=amd64 go build -o webserver.exe webserver.go
GOOS=darwin GOARCH=amd64 go build -o webserver-darwin webserver.go
```

### Docker Deployment
```dockerfile
# Multi-stage build
FROM golang:1.21-alpine AS builder

WORKDIR /app
COPY go.mod go.sum ./
RUN go mod download

COPY . .
RUN CGO_ENABLED=0 GOOS=linux go build -ldflags="-w -s" -o webserver webserver.go

# Final stage
FROM alpine:latest
RUN apk --no-cache add ca-certificates
WORKDIR /root/

COPY --from=builder /app/webserver .

EXPOSE 8080
CMD ["./webserver"]
```

### Systemd Service
```ini
# /etc/systemd/system/go-webserver.service
[Unit]
Description=Go Web Server
After=network.target

[Service]
Type=simple
User=webserver
Group=webserver
WorkingDirectory=/opt/webserver
ExecStart=/opt/webserver/webserver
Restart=on-failure
RestartSec=10

# Environment variables
Environment=SERVER_PORT=8080
Environment=SERVER_HOST=0.0.0.0

# Security settings
NoNewPrivileges=true
PrivateTmp=true
ProtectSystem=strict
ProtectHome=true
ReadWritePaths=/opt/webserver

[Install]
WantedBy=multi-user.target
```

### Kubernetes Deployment
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: go-webserver
spec:
  replicas: 3
  selector:
    matchLabels:
      app: go-webserver
  template:
    metadata:
      labels:
        app: go-webserver
    spec:
      containers:
      - name: webserver
        image: go-webserver:latest
        ports:
        - containerPort: 8080
        env:
        - name: SERVER_PORT
          value: "8080"
        - name: SERVER_HOST
          value: "0.0.0.0"
        resources:
          limits:
            memory: "128Mi"
            cpu: "100m"
---
apiVersion: v1
kind: Service
metadata:
  name: go-webserver-service
spec:
  selector:
    app: go-webserver
  ports:
  - port: 80
    targetPort: 8080
  type: LoadBalancer
```

## References

- [Go Official Documentation](https://golang.org/doc/)
- [net/http Package](https://pkg.go.dev/net/http)
- [Go by Example](https://gobyexample.com/)
- [Effective Go](https://golang.org/doc/effective_go.html)
- [Go Code Review Comments](https://github.com/golang/go/wiki/CodeReviewComments)

---

*Last updated: June 27, 2025*
