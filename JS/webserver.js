const http = require('http');
const url = require('url');
const os = require('os');

const PORT = 8080;

// Create HTML response
function createHtmlResponse() {
    const htmlBody = `<html><body>
        <h1>Hello, World!</h1>
        <p>Server info:</p>
        <ul>
            <li>Port: ${PORT}</li>
            <li>Platform: ${os.platform()}</li>
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
    </body></html>`;

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