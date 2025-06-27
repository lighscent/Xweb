const http = require('http');
const url = require('url');
const os = require('os');

const PORT = 8080;

// Create HTML response
function createHtmlResponse() {
    const htmlBody = `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>JavaScript Web Server</title>
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
        <h1>Hello, World! <span class="language-badge">JavaScript</span></h1>
        
        <h2>Server Information</h2>
        <div class="info-grid">
            <span class="info-label">Port:</span>
            <span class="info-value">${PORT}</span>
            <span class="info-label">Platform:</span>
            <span class="info-value">${os.platform()}</span>
            <span class="info-label">OS:</span>
            <span class="info-value">${os.type()}</span>
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
</html>`;

    return htmlBody;
}

// Create API JSON response
function createApiResponse() {
    const now = new Date();
    const datetime = now.toISOString().replace('T', ' ').substring(0, 19);
    const timestamp = Math.floor(now.getTime() / 1000);

    return {
        server_info: {
            port: PORT,
            platform: os.platform(),
            os: os.type(),
            datetime: datetime,
            timestamp: timestamp,
            status: "running",
            language: "javascript"
        },
        message: "Server API endpoint"
    };
}

// Create HTTP server
const server = http.createServer((req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const pathname = parsedUrl.pathname;

    if (pathname === '/api') {
        // API endpoint
        res.writeHead(200, {
            'Content-Type': 'application/json',
            'Connection': 'close'
        });
        res.end(JSON.stringify(createApiResponse()));
    } else {
        // HTML page
        const htmlContent = createHtmlResponse();
        res.writeHead(200, {
            'Content-Type': 'text/html',
            'Connection': 'close',
            'Content-Length': Buffer.byteLength(htmlContent)
        });
        res.end(htmlContent);
    }
});

// Start server
server.listen(PORT, () => {
    console.log(`Web server started on port ${PORT}`);
});

// Handle server errors
server.on('error', (err) => {
    if (err.code === 'EADDRINUSE') {
        console.error(`Port ${PORT} is already in use`);
    } else {
        console.error('Server error:', err);
    }
});