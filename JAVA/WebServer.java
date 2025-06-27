import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.Instant;
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
        }

        private String createHtmlResponse() {
            String javaVersion = System.getProperty("java.version");
            String osName = System.getProperty("os.name");
            String osVersion = System.getProperty("os.version");
            String osArch = System.getProperty("os.arch");

            return String.format("""
                    <!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1.0">
                        <title>Java Web Server</title>
                        <style>
                            body {
                                font-family: Arial, sans-serif;
                                margin: 40px;
                                background-color: #f9f9f9;
                            }
                            .container {
                                max-width: 800px;
                                margin: 0 auto;
                                background-color: white;
                                padding: 20px;
                                border-radius: 8px;
                                box-shadow: 0 2px 4px rgba(0,0,0,0.1);
                            }
                            ul { list-style-type: disc; margin-left: 20px; }
                            a { color: #0066cc; text-decoration: none; }
                            a:hover { text-decoration: underline; }
                            #browser {
                                background-color: #f5f5f5;
                                padding: 15px;
                                border-radius: 5px;
                                margin-top: 10px;
                            }
                            .info-grid {
                                display: grid;
                                grid-template-columns: auto 1fr;
                                gap: 5px 15px;
                                margin: 10px 0;
                            }
                            .info-label { font-weight: bold; }
                        </style>
                    </head>
                    <body>
                        <div class="container">
                            <h1>Hello, World!</h1>
                            <h2>Server Information</h2>
                            <div class="info-grid">
                                <span class="info-label">Port:</span>
                                <span>%d</span>
                                <span class="info-label">Platform:</span>
                                <span>%s</span>
                                <span class="info-label">OS:</span>
                                <span>%s %s (%s)</span>
                                <span class="info-label">Java Version:</span>
                                <span>%s</span>
                                <span class="info-label">API Endpoint:</span>
                                <span><a href='/api'>/api</a></span>
                            </div>

                            <h2>Browser Information</h2>
                            <div id='browser'>JavaScript required to display browser information</div>

                            <script>
                                const browserInfo = document.getElementById('browser');
                                const info = [
                                    '<strong>User-Agent:</strong> ' + navigator.userAgent,
                                    '<strong>Platform:</strong> ' + navigator.platform,
                                    '<strong>Language:</strong> ' + navigator.language,
                                    '<strong>Cookies enabled:</strong> ' + navigator.cookieEnabled,
                                    '<strong>Screen resolution:</strong> ' + screen.width + 'x' + screen.height,
                                    '<strong>Timezone:</strong> ' + Intl.DateTimeFormat().resolvedOptions().timeZone,
                                    '<strong>Online status:</strong> ' + (navigator.onLine ? 'Online' : 'Offline')
                                ];
                                browserInfo.innerHTML = info.join('<br>');
                            </script>
                        </div>
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