# Xweb - Multi-Language Web Server Collection

## Overview

Simple web server implementations in various programming languages. Each implementation provides the same basic HTTP server functionality to demonstrate language-specific approaches.

## Implementations

| Language | Status | Documentation |
|----------|--------|---------------|
| **C** | ✅ Complete | [📖 C Documentation](.docs/c.md) |
| **C++** | ✅ Complete | [📖 C++ Documentation](.docs/cpp.md) |
| **C#** | ✅ Complete | [📖 C# Documentation](.docs/csharp.md) |
| **Java** | ✅ Complete | [📖 Java Documentation](.docs/java.md) |
| **JavaScript (Node.js)** | ✅ Complete | [📖 JavaScript Documentation](.docs/js.md) |
| **Python** | ✅ Complete | [📖 Python Documentation](.docs/python.md) |

## Features

- 🌐 HTTP/1.1 server on port 8080
- 📄 HTML page with server information
- 🔗 JSON API endpoint (`/api`)
- 🌍 Browser information display
- 🖥️ Cross-platform compatibility

## Testing

- **Main page**: `http://localhost:8080/`
- **API endpoint**: `http://localhost:8080/api`

## API Response Format

```json
{
  "server_info": {
    "port": 8080,
    "platform": "platform_name",
    "os": "operating_system",
    "datetime": "2025-06-27 12:00:00",
    "timestamp": 1719500200,
    "status": "running",
    "language": "implementation_language"
  },
  "message": "Server API endpoint"
}
```

## Contributing

To add a new language implementation:

1. Create a subdirectory for the language
2. Implement the standard features (HTTP server, HTML page, JSON API)
3. Write documentation in `.docs/[language].md`
4. Update this README

## License

MIT License - see [LICENSE](LICENSE) file.

---

*Last updated: June 27, 2025*