import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.concurrent.Executors;

public class WebServer {
    private static final int PORT = 8080;
    private static final String HOST = "localhost";

    private HttpServer server;

    public WebServer() throws IOException {
        server = HttpServer.create(new InetSocketAddress(HOST, PORT), 0);
        setupRoutes();
        server.setExecutor(Executors.newFixedThreadPool(10));
    }

    private void setupRoutes() {
        server.createContext("/", new RootHandler());
        server.createContext("/api", new ApiHandler());
    }

    public void start() {
        server.start();
        System.out.println("Web server started on http://" + HOST + ":" + PORT);
        System.out.println("Main page: http://" + HOST + ":" + PORT + "/");
        System.out.println("API endpoint: http://" + HOST + ":" + PORT + "/api");
        System.out.println("Press Ctrl+C to stop the server");
    }

    public void stop() {
        if (server != null) {
            System.out.println("\nShutting down server...");
            server.stop(0);
            System.out.println("Server stopped");
        }
    }

    static class RootHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            try {
                if ("GET".equals(exchange.getRequestMethod())) {
                    String response = createHtmlResponse();

                    exchange.getResponseHeaders().set("Content-Type", "text/html; charset=UTF-8");
                    exchange.getResponseHeaders().set("Connection", "close");
                    exchange.sendResponseHeaders(200, response.getBytes("UTF-8").length);

                    try (OutputStream os = exchange.getResponseBody()) {
                        os.write(response.getBytes("UTF-8"));
                    }
                } else {
                    exchange.sendResponseHeaders(405, 0); // Method Not Allowed
                }
            } catch (Exception e) {
                e.printStackTrace();
                exchange.sendResponseHeaders(500, 0);
            }
        }

        private String createHtmlResponse() {
            String javaVersion = System.getProperty("java.version");
            String osName = System.getProperty("os.name");
            String osVersion = System.getProperty("os.version");
            String osArch = System.getProperty("os.arch");

            return String.format(
                    """
                                <!DOCTYPE html>
                                <html lang="en">
                                <head>
                                    <meta charset="UTF-8">
                                    <meta name="viewport" content="width=device-width, initial-scale=1.0">
                                    <title>Java Web Server</title>
                                    <style>
                                        body {
                                            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
                                            margin: 0;
                                            padding: 40px;
                                            background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%);
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
                                            background: linear-gradient(45deg, #f89820, #ed8b00);
                                            color: white;
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
                                        <h1>Hello, World! <span class="language-badge">Java</span></h1>

                                        <h2>Server Information</h2>
                                        <div class="info-grid">
                                            <span class="info-label">Port:</span>
                                            <span class="info-value">%d</span>
                                            <span class="info-label">Platform:</span>
                                            <span class="info-value">%s</span>
                                            <span class="info-label">OS:</span>
                                            <span class="info-value">%s %s (%s)</span>
                                            <span class="info-label">Java Version:</span>
                                            <span class="info-value">%s</span>
                                            <span class="info-label">API Endpoint:</span>
                                            <span class="info-value"><a href='/api'>/api</a></span>
                                        </div>

                                        <h2>Browser Information</h2>
                                        <div id='browser'>
                                            <em>JavaScript required to display browser information</em>
                                        </div>

                                        <div class="footer">
                                            <p>Multi-Language Web Server Collection | Java Implementation</p>
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
                                </html>
                            """,
                    PORT,
                    getPlatformName(),
                    osName,
                    osVersion,
                    osArch,
                    javaVersion);
        }

        private String getPlatformName() {
            String osName = System.getProperty("os.name").toLowerCase();
            if (osName.contains("win")) {
                return "Windows";
            } else if (osName.contains("mac")) {
                return "macOS";
            } else if (osName.contains("linux")) {
                return "Linux";
            } else if (osName.contains("unix")) {
                return "Unix";
            } else {
                return "Unknown";
            }
        }
    }

    static class ApiHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            if ("GET".equals(exchange.getRequestMethod())) {
                String response = createApiResponse();

                exchange.getResponseHeaders().set("Content-Type", "application/json; charset=UTF-8");
                exchange.getResponseHeaders().set("Connection", "close");
                exchange.sendResponseHeaders(200, response.getBytes("UTF-8").length);

                try (OutputStream os = exchange.getResponseBody()) {
                    os.write(response.getBytes("UTF-8"));
                }
            } else {
                exchange.sendResponseHeaders(405, 0); // Method Not Allowed
            }
        }

        private String createApiResponse() {
            LocalDateTime now = LocalDateTime.now();
            String datetime = now.format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"));
            long timestamp = Instant.now().getEpochSecond();

            String osName = System.getProperty("os.name");
            String osVersion = System.getProperty("os.version");
            String osArch = System.getProperty("os.arch");
            String javaVersion = System.getProperty("java.version");
            String javaVendor = System.getProperty("java.vendor");

            String platform = getPlatformId();

            // Using StringBuilder for better performance with JSON construction
            StringBuilder json = new StringBuilder();
            json.append("{\n");
            json.append("  \"server_info\": {\n");
            json.append("    \"port\": ").append(PORT).append(",\n");
            json.append("    \"platform\": \"").append(platform).append("\",\n");
            json.append("    \"os\": \"").append(osName).append(" ").append(osVersion).append("\",\n");
            json.append("    \"architecture\": \"").append(osArch).append("\",\n");
            json.append("    \"java_version\": \"").append(javaVersion).append("\",\n");
            json.append("    \"java_vendor\": \"").append(escapeJson(javaVendor)).append("\",\n");
            json.append("    \"datetime\": \"").append(datetime).append("\",\n");
            json.append("    \"timestamp\": ").append(timestamp).append(",\n");
            json.append("    \"status\": \"running\",\n");
            json.append("    \"language\": \"java\"\n");
            json.append("  },\n");
            json.append("  \"message\": \"Server API endpoint\"\n");
            json.append("}");

            return json.toString();
        }

        private String getPlatformId() {
            String osName = System.getProperty("os.name").toLowerCase();
            if (osName.contains("win")) {
                return "win32";
            } else if (osName.contains("mac")) {
                return "darwin";
            } else if (osName.contains("linux")) {
                return "linux";
            } else if (osName.contains("unix")) {
                return "unix";
            } else {
                return "unknown";
            }
        }

        private String escapeJson(String value) {
            if (value == null)
                return "";
            return value.replace("\\", "\\\\")
                    .replace("\"", "\\\"")
                    .replace("\n", "\\n")
                    .replace("\r", "\\r")
                    .replace("\t", "\\t");
        }
    }

    public static void main(String[] args) {
        try {
            WebServer webServer = new WebServer();

            // Add shutdown hook for graceful shutdown
            Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                webServer.stop();
            }));

            webServer.start();

            // Keep the main thread alive
            Thread.currentThread().join();

        } catch (IOException e) {
            System.err.println("Failed to start server: " + e.getMessage());
            e.printStackTrace();
        } catch (InterruptedException e) {
            System.err.println("Server interrupted: " + e.getMessage());
            Thread.currentThread().interrupt();
        }
    }
}