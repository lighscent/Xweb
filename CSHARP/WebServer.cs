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
        public string? platform { get; set; }
        public string? os { get; set; }
        public string? datetime { get; set; }
        public long timestamp { get; set; }
        public string? status { get; set; }
        public string? language { get; set; }
    }

    public class ApiResponse
    {
        public ServerInfo? server_info { get; set; }
        public string? message { get; set; }
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

            return $@"<!DOCTYPE html>
<html lang=""en"">
<head>
    <meta charset=""UTF-8"">
    <meta name=""viewport"" content=""width=device-width, initial-scale=1.0"">
    <title>C# Web Server</title>
    <style>
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 40px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            color: #333;
        }}
        .container {{
            max-width: 900px;
            margin: 0 auto;
            background-color: white;
            padding: 40px;
            border-radius: 15px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
        }}
        h1 {{
            color: #2c3e50;
            text-align: center;
            margin-bottom: 10px;
            font-size: 2.5em;
            font-weight: 300;
        }}
        .language-badge {{
            display: inline-block;
            background: linear-gradient(45deg, #239120, #68217a);
            color: white;
            padding: 8px 16px;
            border-radius: 25px;
            font-size: 0.9em;
            font-weight: bold;
            margin-left: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.2);
        }}
        h2 {{
            color: #34495e;
            border-bottom: 3px solid #3498db;
            padding-bottom: 10px;
            margin-top: 40px;
        }}
        .info-grid {{
            display: grid;
            grid-template-columns: auto 1fr;
            gap: 15px 25px;
            margin: 25px 0;
            background: linear-gradient(135deg, #f8f9fa, #e9ecef);
            padding: 25px;
            border-radius: 10px;
            border-left: 5px solid #3498db;
        }}
        .info-label {{
            font-weight: bold;
            color: #2c3e50;
        }}
        .info-value {{
            color: #34495e;
        }}
        a {{
            color: #3498db;
            text-decoration: none;
            font-weight: 500;
            transition: all 0.3s ease;
        }}
        a:hover {{
            color: #2980b9;
            text-decoration: underline;
        }}
        #browser {{
            background: linear-gradient(135deg, #e8f4f8, #d1ecf1);
            padding: 20px;
            border-radius: 10px;
            margin-top: 15px;
            border-left: 5px solid #17a2b8;
            font-family: 'Courier New', monospace;
            font-size: 0.9em;
        }}
        .footer {{
            text-align: center;
            margin-top: 40px;
            padding-top: 20px;
            border-top: 1px solid #dee2e6;
            color: #6c757d;
            font-size: 0.9em;
        }}
    </style>
</head>
<body>
    <div class=""container"">
        <h1>Hello, World! <span class=""language-badge"">C#</span></h1>
        
        <h2>Server Information</h2>
        <div class=""info-grid"">
            <span class=""info-label"">Port:</span>
            <span class=""info-value"">{PORT}</span>
            <span class=""info-label"">Platform:</span>
            <span class=""info-value"">{platform}</span>
            <span class=""info-label"">OS:</span>
            <span class=""info-value"">{osVersion}</span>
            <span class=""info-label"">API Endpoint:</span>
            <span class=""info-value""><a href='/api'>/api</a></span>
        </div>
        
        <h2>Browser Information</h2>
        <div id='browser'>
            <em>JavaScript required to display browser information</em>
        </div>
        
        <div class=""footer"">
            <p>Multi-Language Web Server Collection | C# Implementation</p>
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
</html>";
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
        private static WebServer? server;

        static async Task Main(string[] args)
        {
            Console.CancelKeyPress += (sender, e) =>
            {
                e.Cancel = true;
                if (server != null)
                    server.Stop();
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