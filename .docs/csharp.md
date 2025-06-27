# C# Web Server Implementation

## Overview

Modern C# HTTP server implementation using `HttpListener` and async/await patterns. This implementation demonstrates C# best practices with async programming, JSON serialization, and proper resource management.

## Prerequisites

### All Platforms
- **.NET 6.0 SDK** or later
- C# 10 language features support

### Installation
```bash
# Check if .NET is installed
dotnet --version

# If not installed, download from: https://dotnet.microsoft.com/download
```

## Quick Start

### Build
```bash
# Navigate to CSHARP directory
cd CSHARP

# Build the project
dotnet build

# Or build in release mode
dotnet build -c Release
```

### Run
```bash
# Run directly
dotnet run

# Or run the built executable
dotnet bin/Debug/net8.0/WebServer.dll

# Run in release mode
dotnet run -c Release
```

### Test
- Main page: `http://localhost:8080/`
- API endpoint: `http://localhost:8080/api`

## Features

- HTTP/1.1 server on port 8080
- Async/await programming model
- Built-in JSON serialization with System.Text.Json
- Cross-platform compatibility (.NET 8+)
- Static HTML page with server info
- JSON API endpoint with typed responses
- Real-time browser information display
- Graceful shutdown handling

## Code Structure

### Class Design
```csharp
public class WebServer
{
    private HttpListener httpListener;
    private bool isRunning;
    
    public async Task StartAsync();        // Start server asynchronously
    public void Stop();                    // Stop server gracefully
    private Task HandleRequestAsync();     // Handle individual requests
}
```

### Data Models
```csharp
public class ServerInfo
{
    public int port { get; set; }
    public string platform { get; set; }
    public string os { get; set; }
    public string datetime { get; set; }
    public long timestamp { get; set; }
    public string status { get; set; }
    public string language { get; set; }
}

public class ApiResponse
{
    public ServerInfo server_info { get; set; }
    public string message { get; set; }
}
```

### Async Request Handling
```csharp
private async Task HandleRequestAsync(HttpListenerContext context)
{
    var request = context.Request;
    var response = context.Response;
    
    // Process request asynchronously
    string content = request.Url.AbsolutePath == "/api" 
        ? CreateApiResponse() 
        : CreateHtmlResponse();
    
    // Write response asynchronously
    using var output = response.OutputStream;
    var buffer = Encoding.UTF8.GetBytes(content);
    await output.WriteAsync(buffer, 0, buffer.Length);
}
```

### JSON Serialization
```csharp
var options = new JsonSerializerOptions
{
    WriteIndented = false,
    PropertyNamingPolicy = JsonNamingPolicy.CamelCase
};

return JsonSerializer.Serialize(apiResponse, options);
```

### Platform Detection
```csharp
var platform = Environment.OSVersion.Platform switch
{
    PlatformID.Win32NT => "win32",
    PlatformID.Unix => "unix",
    PlatformID.MacOSX => "darwin",
    _ => "unknown"
};
```

### API Response Format
```json
{
  "serverInfo": {
    "port": 8080,
    "platform": "win32",
    "os": "Microsoft Windows NT 10.0.22000.0",
    "datetime": "2025-06-27 14:30:00",
    "timestamp": 1719500200,
    "status": "running",
    "language": "csharp"
  },
  "message": "Server API endpoint"
}
```

## Common Issues

### .NET SDK Not Found
```bash
# Install .NET SDK
# Windows
winget install Microsoft.DotNet.SDK.8

# macOS
brew install dotnet

# Linux (Ubuntu)
sudo apt-get update
sudo apt-get install -y dotnet-sdk-8.0
```

### Port in Use
```bash
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
- On Linux/macOS, HttpListener may require elevated privileges
- Consider using Kestrel for production scenarios
- Use `sudo` if binding fails on Unix systems

### Compilation Errors
```bash
# Restore NuGet packages
dotnet restore

# Clean and rebuild
dotnet clean
dotnet build

# Check target framework
dotnet --list-sdks
```

## Debug Build
```bash
# Build with debug symbols
dotnet build -c Debug

# Run with debug output
dotnet run --verbosity diagnostic

# Attach debugger
dotnet run --debug
```

## Advanced Features

### Async Programming
```csharp
// Concurrent request handling
while (isRunning)
{
    var context = await httpListener.GetContextAsync();
    _ = Task.Run(() => HandleRequestAsync(context));
}
```

### Graceful Shutdown
```csharp
Console.CancelKeyPress += (sender, e) =>
{
    e.Cancel = true;
    server?.Stop();
};
```

### Error Handling
```csharp
try
{
    // Server operations
}
catch (HttpListenerException ex)
{
    Console.WriteLine($"HTTP error: {ex.Message}");
}
catch (Exception ex)
{
    Console.WriteLine($"General error: {ex.Message}");
}
```

## Performance Considerations

- HttpListener handles multiple concurrent connections
- Async/await prevents thread blocking
- JSON serialization is optimized with System.Text.Json
- Memory management handled by garbage collector
- Consider connection limits for production use

## Limitations

- Requires .NET runtime
- HttpListener has platform-specific behaviors
- Basic security (no HTTPS, authentication, etc.)
- Single-process architecture
- Limited HTTP feature support

## Extensions

### HTTPS Support
```csharp
// Configure HTTPS
httpListener.Prefixes.Add("https://localhost:8443/");

// Certificate configuration required
```

### Dependency Injection
```csharp
// Use Microsoft.Extensions.DependencyInjection
services.AddSingleton<WebServer>();
services.AddLogging();
```

### Configuration
```csharp
// Use Microsoft.Extensions.Configuration
var config = new ConfigurationBuilder()
    .AddJsonFile("appsettings.json")
    .Build();
```

## Production Deployment

### Self-Contained Deployment
```bash
# Publish self-contained
dotnet publish -c Release -r win-x64 --self-contained

# Publish framework-dependent
dotnet publish -c Release
```

### Docker Support
```dockerfile
FROM mcr.microsoft.com/dotnet/runtime:8.0
COPY bin/Release/net8.0/publish/ App/
WORKDIR /App
ENTRYPOINT ["dotnet", "WebServer.dll"]
```

## References

- [.NET 6 Documentation](https://docs.microsoft.com/en-us/dotnet/core/)
- [HttpListener Class](https://docs.microsoft.com/en-us/dotnet/api/system.net.httplistener)
- [System.Text.Json](https://docs.microsoft.com/en-us/dotnet/api/system.text.json)
- [Async Programming](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/async/)

---

*Last updated: June 27, 2025*