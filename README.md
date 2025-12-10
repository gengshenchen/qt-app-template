# MyQtAppTemplate

This is a cross-platform Qt application template based on CMake that supports hybrid development with QWidget and QML.

## Architecture

This project adopts a modular design concept, separating code with different responsibilities into independent libraries:

- `src/core`: Core business logic (non-GUI)
- `src/shared`: Data structures and constants shared across modules
- `src/widgets`: Reusable custom QWidget controls
- `src/qml`: Reusable QML components and their C++ backends
- `src/app`: Main program entry point, responsible for assembling all modules
- `tests`: Unit tests

## Web Frontend Integration

This template integrates [QCefView](https://github.com/CefView/QCefView), which is a Qt-based QWidget that encapsulates CEF (Chromium Embedded Framework). This allows you to build your application's UI using modern web technologies (HTML, CSS, JavaScript) while seamlessly integrating with the Qt backend.

Key features:
- **Hybrid Development**: Combine the power of Qt for native functionalities with the flexibility of web technologies for the UI.
- **Decoupled UI**: Keep your UI code (web frontend) separate from the business logic (C++ backend).
- **Modern UI/UX**: Leverage popular web frameworks like React, Vue, or Angular to create modern and responsive user interfaces.

## How to Build

You need to install:
- CMake (>= 3.16)
- C++ Compiler (GCC, Clang, or MSVC)
- Qt (>= 6.2)

### Using the `build.sh` Script (Recommended)

The `build.sh` script provides a convenient way to configure, build, and update translations for the project. It handles various build configurations and automatically detects the number of CPU cores for parallel compilation.

**Usage:**

```bash
./build.sh [clean] [Debug|Release|RelWithDebInfo|MinSizeRel] [/path/to/qt]
```

Arguments can be provided in any order:

-   `clean`: (Optional) Performs a clean build (removes previous build artifacts before recompiling).
-   `Debug|Release|RelWithDebInfo|MinSizeRel`: (Optional) Specifies the CMake build type. If omitted, defaults to `Debug`.
-   `/path/to/qt`: (Optional) Specifies the path to your Qt SDK installation (e.g., `/home/user/Qt/6.2.4/gcc_64`). If omitted, the script tries to use the `QT_PREFIX_PATH` environment variable or a default path configured within the script.

**Examples:**

-   Build in Debug mode using the default Qt path:
    ```bash
    ./build.sh
    ```
-   Build in Release mode with a specific Qt path:
    ```bash
    ./build.sh Release /opt/Qt/6.5.0/gcc_64
    ```
-   Perform a clean build in RelWithDebInfo mode:
    ```bash
    ./build.sh clean RelWithDebInfo
    ```

### Manual Build Steps

If you prefer to build manually, follow these steps:

1.  **Clone the repository**
    ```bash
    git clone <your-repo-url>
    ```

2.  **Configure the project**
    ```bash
    rm -rf out && cmake -S . -B out -DCMAKE_PREFIX_PATH=<path-to-your-qt-sdk>
    ```
    For example:
    ```bash
    rm -rf out && cmake -S . -B out -DCMAKE_PREFIX_PATH=/home/user/Qt/6.2.4/gcc_64
    ```

3.  **Update translations** (Optional)
    This command will scan the source code for translatable strings and generate/update the `.ts` translation files.
    ```bash
    cmake --build out --target update_translations -j16
    ```

4.  **Build the application**
    ```bash
    cmake --build out -j16
    ```