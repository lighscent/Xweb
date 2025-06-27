# Xweb - Multi-Language Web Server Collection

## Overview

This repository contains implementations of a simple web server in various programming languages. The goal is to demonstrate how different languages handle HTTP server functionality while maintaining consistent features across all implementations.

## Documentation

Detailed documentation for each language implementation can be found in the `docs/` folder:

- 📖 [C Documentation](docs/c.md) - Complete implementation guide


## Documentation Structure

Each language documentation includes:
- 📋 Prerequisites and dependencies
- 🔧 Build and compilation instructions
- 🚀 Running the server
- 🧪 Testing and debugging
- 📝 Code explanation and architecture
- 🐛 Troubleshooting common issues
- 📚 Language-specific best practices

## API Response Format

All implementations should return JSON in this format for `/api`:

```json
{
  "server_info": {
    "port": 8080,
    "platform": "platform_name",
    "os": "operating_system",
    "datetime": "2024-01-01 12:00:00",
    "timestamp": 1704110400,
    "status": "running",
    "language": "implementation_language"
  },
  "message": "Server API endpoint"
}
```

## Testing

Access the server at:
- Main page: `http://localhost:8080/`
- API endpoint: `http://localhost:8080/api`

## Contributing

When adding a new language implementation:

1. Create a subdirectory for the language
2. Implement all standard features
3. Create documentation in `docs/[language].md`
4. Update this README with the new language entry
5. Ensure cross-platform compatibility where possible
6. Follow the established API response format

## License

This project is open source and available under the MIT License.
