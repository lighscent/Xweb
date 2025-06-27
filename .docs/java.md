# Java Web Server Implementation

## Overview

HTTP server implementation using Java's built-in `com.sun.net.httpserver.HttpServer` with modern Java features. This implementation demonstrates Java best practices with object-oriented design, exception handling, and concurrent request processing.

## Prerequisites

### All Platforms
- **Java Development Kit (JDK) 11** or later
- No external dependencies (uses standard library only)

### Installation Check
```bash
# Check Java version
java --version
javac --version

# Ensure JAVA_HOME is set
echo $JAVA_HOME    # Linux/macOS
echo %JAVA_HOME%   # Windows
```

## Quick Start

### Compile
```bash
# Navigate to JAVA directory
cd JAVA

# Compile the Java source
javac WebServer.java

# Alternative with explicit classpath
javac -cp . WebServer.java
```

### Run
```bash
# Run the compiled class
java WebServer

# Alternative with explicit classpath
java -cp . WebServer

# Run with custom JVM options
java -Xmx256m -server WebServer
```

### Test
- Main page: `http://localhost:8080/`
- API endpoint: `http://localhost:8080/api`

## Features

- HTTP/1.1 server on port 8080
- Object-oriented design with handler classes
- Built-in thread pool for concurrent requests
- Comprehensive error handling and logging
- Cross-platform compatibility (Write Once, Run Anywhere)
- Enhanced HTML page with modern styling and detailed system info
- JSON API endpoint with extensive Java runtime information
- Real-time browser information display
- Graceful shutdown handling with shutdown hooks
- Text blocks for readable HTML templates (Java 15+)

## Code Structure

### Main Server Class
```java
public class WebServer {
    private static final int PORT = 8080;
    private HttpServer server;
    
    public WebServer() throws IOException;  // Constructor
    public void start();                    // Start server
    public void stop();                     // Stop server gracefully
    private void setupRoutes();             // Configure request handlers
}
```

### Request Handlers
```java
static class RootHandler implements HttpHandler {
    public void handle(HttpExchange exchange);      // Handle root requests
    private String createHtmlResponse();            // Generate HTML content
}

static class ApiHandler implements HttpHandler {
    public void handle(HttpExchange exchange);      // Handle API requests
    private String createApiResponse();             // Generate JSON content
}
```

### HTTP Request Handling
```java
@Override
public void handle(HttpExchange exchange) throws IOException {
    if ("GET".equals(exchange.getRequestMethod())) {
        String response = createResponse();
        
        exchange.getResponseHeaders().set("Content-Type", "application/json");
        exchange.sendResponseHeaders(200, response.getBytes("UTF-8").length);
        
        try (OutputStream os = exchange.getResponseBody()) {
            os.write(response.getBytes("UTF-8"));
        }
    }
}
```

### System Information Gathering
```java
String javaVersion = System.getProperty("java.version");
String osName = System.getProperty("os.name");
String osVersion = System.getProperty("os.version");
String osArch = System.getProperty("os.arch");
String javaVendor = System.getProperty("java.vendor");
```

### Platform Detection
```java
private String getPlatformId() {
    String osName = System.getProperty("os.name").toLowerCase();
    if (osName.contains("win")) return "win32";
    else if (osName.contains("mac")) return "darwin";
    else if (osName.contains("linux")) return "linux";
    else return "unknown";
}
```

### API Response Format
```json
{
  "server_info": {
    "port": 8080,
    "platform": "win32",
    "os": "Windows 10",
    "architecture": "amd64",
    "java_version": "17.0.2",
    "java_vendor": "Eclipse Adoptium",
    "datetime": "2025-06-27 14:30:00",
    "timestamp": 1719500200,
    "status": "running",
    "language": "java"
  },
  "message": "Server API endpoint"
}
```

## Common Issues

### Java Not Found
```bash
# Install OpenJDK
# Windows
winget install Microsoft.OpenJDK.17

# macOS (Homebrew)
brew install openjdk@17

# Linux (Ubuntu/Debian)
sudo apt update
sudo apt install openjdk-17-jdk

# Linux (CentOS/RHEL)
sudo yum install java-17-openjdk-devel
```

### Compilation Errors
```bash
# Check Java version compatibility
javac -version

# Compile with specific Java version
javac --release 11 WebServer.java

# Check classpath issues
javac -cp . -verbose WebServer.java
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
# For ports < 1024, use sudo
sudo java WebServer

# Check firewall settings
# Windows
netsh advfirewall firewall add rule name="Java Web Server" dir=in action=allow protocol=TCP localport=8080
```

### Runtime Errors
- **BindException**: Port already in use or insufficient privileges
- **SecurityException**: Security manager blocking network operations
- **OutOfMemoryError**: Increase heap size with `-Xmx` flag

## Debug Mode

### Compilation with Debug Info
```bash
# Compile with debug information
javac -g WebServer.java

# Run with debug output
java -Djava.util.logging.level=ALL WebServer

# Enable HTTP server logging
java -Dcom.sun.net.httpserver.HttpServer.debug=true WebServer
```

### JVM Debugging
```bash
# Enable remote debugging
java -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005 WebServer

# Memory debugging
java -XX:+PrintGC -XX:+PrintGCDetails WebServer

# Enable assertions
java -ea WebServer
```

## Advanced Features

### Thread Pool Configuration
```java
// Custom thread pool
server.setExecutor(Executors.newFixedThreadPool(20));

// Cached thread pool for variable load
server.setExecutor(Executors.newCachedThreadPool());

// Custom thread factory
ThreadFactory factory = r -> {
    Thread t = new Thread(r, "WebServer-" + threadCounter.incrementAndGet());
    t.setDaemon(true);
    return t;
};
server.setExecutor(Executors.newFixedThreadPool(10, factory));
```

### Enhanced Error Handling
```java
@Override
public void handle(HttpExchange exchange) throws IOException {
    try {
        // Handle request
    } catch (Exception e) {
        logger.log(Level.SEVERE, "Request handling error", e);
        sendErrorResponse(exchange, 500, "Internal Server Error");
    } finally {
        exchange.close();
    }
}
```

### HTTPS Support
```java
import javax.net.ssl.SSLContext;
import com.sun.net.httpserver.HttpsServer;

HttpsServer httpsServer = HttpsServer.create(new InetSocketAddress(8443), 0);
SSLContext sslContext = SSLContext.getInstance("TLS");
// Configure SSL context with certificates
httpsServer.setHttpsConfigurator(new HttpsConfigurator(sslContext));
```

### Request Logging
```java
import java.util.logging.Logger;
import java.util.logging.FileHandler;

private static final Logger logger = Logger.getLogger(WebServer.class.getName());

static {
    try {
        FileHandler fileHandler = new FileHandler("webserver.log", true);
        logger.addHandler(fileHandler);
    } catch (IOException e) {
        System.err.println("Failed to initialize logging: " + e.getMessage());
    }
}
```

### JSON Response Builder
```java
public class JsonBuilder {
    private StringBuilder json = new StringBuilder();
    
    public JsonBuilder startObject() { json.append("{"); return this; }
    public JsonBuilder endObject() { json.append("}"); return this; }
    public JsonBuilder addField(String key, Object value) {
        json.append("\"").append(key).append("\":\"").append(value).append("\",");
        return this;
    }
    public String build() { return json.toString().replaceAll(",$", ""); }
}
```

## Performance Considerations

- HttpServer handles multiple concurrent connections efficiently
- Thread pool prevents resource exhaustion
- Memory usage optimized with proper resource management
- Suitable for development and light production use
- Consider application servers (Tomcat, Jetty) for production

## Testing

### Unit Testing with JUnit
```java
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.AfterEach;

class WebServerTest {
    private WebServer server;
    
    @BeforeEach
    void setUp() throws IOException {
        server = new WebServer();
        server.start();
    }
    
    @AfterEach
    void tearDown() {
        server.stop();
    }
    
    @Test
    void testApiEndpoint() throws IOException {
        // Test API endpoint response
        URL url = new URL("http://localhost:8080/api");
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        assertEquals(200, conn.getResponseCode());
        assertEquals("application/json; charset=UTF-8", 
                     conn.getHeaderField("Content-Type"));
    }
}
```

### Manual Testing
```bash
# Test with curl
curl http://localhost:8080/
curl http://localhost:8080/api

# Test with wget
wget -q -O - http://localhost:8080/api

# Load testing with Apache Bench
ab -n 1000 -c 10 http://localhost:8080/
```

## Limitations

- Single JVM process (consider clustering for scalability)
- Basic HTTP/1.1 support (no HTTP/2)
- No built-in request body parsing for POST/PUT
- Limited security features (add authentication/authorization as needed)
- Memory overhead of JVM

## Extensions

### Adding POST Support
```java
static class PostHandler implements HttpHandler {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
        if ("POST".equals(exchange.getRequestMethod())) {
            InputStream requestBody = exchange.getRequestBody();
            String body = new String(requestBody.readAllBytes(), StandardCharsets.UTF_8);
            // Process POST data
        }
    }
}
```

### Configuration File Support
```java
import java.util.Properties;

Properties config = new Properties();
try (InputStream input = new FileInputStream("server.properties")) {
    config.load(input);
    int port = Integer.parseInt(config.getProperty("server.port", "8080"));
}
```

### Database Integration
```java
// Add database dependencies to classpath
import java.sql.Connection;
import java.sql.DriverManager;

Connection conn = DriverManager.getConnection(
    "jdbc:h2:mem:testdb", "sa", ""
);
```

## Production Deployment

### Creating Executable JAR
```bash
# Create manifest file
echo "Main-Class: WebServer" > manifest.txt

# Create JAR with manifest
jar cfm webserver.jar manifest.txt *.class

# Run JAR
java -jar webserver.jar
```

### JVM Tuning for Production
```bash
# Production JVM settings
java -server \
     -Xms512m -Xmx1024m \
     -XX:+UseG1GC \
     -XX:MaxGCPauseMillis=100 \
     -XX:+UseStringDeduplication \
     -jar webserver.jar
```

### Docker Deployment
```dockerfile
FROM openjdk:17-jre-slim

WORKDIR /app
COPY WebServer.class .

EXPOSE 8080
CMD ["java", "WebServer"]
```

### Systemd Service
```ini
# /etc/systemd/system/java-webserver.service
[Unit]
Description=Java Web Server
After=network.target

[Service]
Type=simple
User=webserver
WorkingDirectory=/opt/webserver
ExecStart=/usr/bin/java -server -Xmx512m WebServer
Restart=on-failure
RestartSec=10

[Install]
WantedBy=multi-user.target
```

## References

- [Java SE Documentation](https://docs.oracle.com/en/java/javase/17/)
- [HttpServer API Documentation](https://docs.oracle.com/en/java/javase/17/docs/api/jdk.httpserver/com/sun/net/httpserver/HttpServer.html)
- [Java Concurrency in Practice](https://jcip.net/)
- [Effective Java by Joshua Bloch](https://www.oreilly.com/library/view/effective-java/9780134686097/)

---

*Last updated: June 27, 2025*