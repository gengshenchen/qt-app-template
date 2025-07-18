# File: README.md
# MyQtAppTemplate

这是一个基于CMake的、支持QWidget和QML混合开发的、跨平台的Qt应用程序模板。

## 架构

本项目采用模块化的设计理念，将不同职责的代码分离到独立的库中：

- `src/core`: 核心业务逻辑 (非GUI)
- `src/shared`: 跨模块共享的数据结构和常量
- `src/gui-widgets`: 可复用的自定义QWidget控件
- `src/gui-qml`: 可复用的QML组件及其C++后端
- `src/app`: 主程序入口，负责组装所有模块
- `tests`: 单元测试

## 如何构建

你需要安装:
- CMake (>= 3.16)
- C++ 编译器 (GCC, Clang, or MSVC)
- Qt6 (>= 6.2)

### 构建步骤

1. **克隆仓库**
   ```bash
   git clone <your-repo-url>

   rm -rf out && cmake -S . -B out -DCMAKE_PREFIX_PATH=/home/karl/Qt6/6.9.1/gcc_64 -DUSE_STATIC_QML_MODULES=OFF

2. **更新翻译，提取生成 ts file**
   ```bash
   cmake --build out --target update_translations -j16

3. **build all**
   ```bash
   cmake --build out  -j16
