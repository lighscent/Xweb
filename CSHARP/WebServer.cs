using System;
using System.IO;
using System.Net;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace WebServerCSharp
{
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

    public class WebServer
    {
        private const int PORT = 8080;
        private HttpListener httpListener;
        private bool isRunning;

        public WebServer()
        {
            httpListener = new HttpListener();
            httpListener.Prefixes.Add($"http://localhost:{PORT}/");
            isRunning = false;
        }

        public async Task StartAsync()
        {
            try
            {
                httpListener.Start();
                isRunning = true;
                Console.WriteLine($"Web server started on port {PORT}");
                
                while (isRunning)
                {
                    var context = await httpListener.GetContextAsync();
                    _ = Task.Run(() => HandleRequestAsync(context));
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Server error: {ex.Message}");
            }
        }

        public void Stop()
        {
            isRunning = false;
            httpListener?.Stop();
            httpListener?.Close();
            Console.WriteLine("Server stopped");
        }

        private async Task HandleRequestAsync(HttpListenerContext context)
        {
            try
            {
                var request = context.Request;
                var response = context.Response;

                string responseContent;
                string contentType;

                if (request.Url.AbsolutePath == "/api")
                {
                    responseContent = CreateApiResponse();
                    contentType = "application/json";
                }
                else
                {
                    responseContent = CreateHtmlResponse();
                    contentType = "text/html";
                }

                response.ContentType = contentType;
                response.ContentLength64 = Encoding.UTF8.GetByteCount(responseContent);
                response.Headers.Add("Connection", "close");

                using (var output = response.OutputStream)
                {
                    var buffer = Encoding.UTF8.GetBytes(responseContent);
                    await output.WriteAsync(buffer, 0, buffer.Length);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Request handling error: {ex.Message}");
            }
        }

        private string CreateHtmlResponse()
        {
            var platform = Environment.OSVersion.Platform.ToString();
            var osVersion = Environment.OSVersion.VersionString;

            return $@"<html><body>
                <h1>Hello, World!</h1>
                <p>Server info:</p>
                <ul>
                    <li>Port: {PORT}</li>
                    <li>Platform: {platform}</li>
                    <li>OS: {osVersion}</li>
                    <li>API endpoint: <a href='/api'>/api</a></li>
                </ul>
                <h2>Browser Info</h2>
                <p id='browser'></p>
                <script>
                    document.getElementById('browser').innerHTML = 
                        'User-Agent: ' + navigator.userAgent + '<br>' + 
                        'Platform: ' + navigator.platform + '<br>' + 
                        'Language: ' + navigator.language + '<br>' + 
                        'Cookies enabled: ' + navigator.cookieEnabled;
                </script>
            </body></html>";
        }

        private string CreateApiResponse()
        {
            var now = DateTime.Now;
            var platform = Environment.OSVersion.Platform switch
            {
                PlatformID.Win32NT => "win32",
                PlatformID.Unix => "unix",
                PlatformID.MacOSX => "darwin",
                _ => "unknown"
            };

            var apiResponse = new ApiResponse
            {
                server_info = new ServerInfo
                {
                    port = PORT,
                    platform = platform,
                    os = Environment.OSVersion.VersionString,
                    datetime = now.ToString("yyyy-MM-dd HH:mm:ss"),
                    timestamp = ((DateTimeOffset)now).ToUnixTimeSeconds(),
                    status = "running",
                    language = "csharp"
                },
                message = "Server API endpoint"
            };

            var options = new JsonSerializerOptions
            {
                WriteIndented = false,
                PropertyNamingPolicy = JsonNamingPolicy.CamelCase
            };

            return JsonSerializer.Serialize(apiResponse, options);
        }
    }

    class Program
    {
        private static WebServer server;

        static async Task Main(string[] args)
        {
            Console.CancelKeyPress += (sender, e) =>
            {
                e.Cancel = true;
                server?.Stop();
            };

            try
            {
                server = new WebServer();
                await server.StartAsync();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to start server: {ex.Message}");
            }
        }
    }
}