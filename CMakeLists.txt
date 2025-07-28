cmake_minimum_required(VERSION 3.19)

project(qt-app-template VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables.") # in windows , it is also for dll.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib CACHE PATH "Single Directory for all shared Libraries")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib CACHE PATH "Single Directory for all static archive")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Quick
    QuickWidgets  # QWidget中嵌入QML的关键
    Qml
    Test          # 用于单元测试
    LinguistTools
)

# 是否启用静态 QML 模块打包（默认为 OFF）
option(USE_STATIC_QML_MODULES "Build QML modules as static" OFF)

qt_standard_project_setup(
    I18N_SOURCE_LANGUAGE en  #default language 
    I18N_TRANSLATED_LANGUAGES  zh de fr
    REQUIRES 6.5)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

# 按照“从底层到上层”的依赖顺序添加
add_subdirectory(3rdparty/spdlog)
add_subdirectory(3rdparty/crashpad)
# 設置EXCLUDE_FROM_ALL可以防止在默認構建時編譯這些庫的測試等無關目標，加快編譯速度
add_subdirectory(3rdparty/simpleini EXCLUDE_FROM_ALL)
add_subdirectory(3rdparty/json EXCLUDE_FROM_ALL)
add_subdirectory(3rdparty/cpp-httplib EXCLUDE_FROM_ALL)
add_subdirectory(3rdparty/taskflow EXCLUDE_FROM_ALL)

add_subdirectory(src/shared)
add_subdirectory(src/core)
add_subdirectory(src/widgets)
add_subdirectory(src/qml)
add_subdirectory(src/app)
add_subdirectory(tests)

