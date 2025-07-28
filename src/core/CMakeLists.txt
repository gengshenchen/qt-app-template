# 定义一个名为Core的静态库(STATIC)或动态库(SHARED)
add_library(Core STATIC
    Logger.cpp
    log/log.cpp
    crash/crashpad_handler.cc
    config/config_manager.cc
    task/task_manager.cc
    network/network_manager.cc
)

# Core库需要链接Shared库来获取常量定义，并链接Qt6::Core来使用QObject等
target_link_libraries(Core PUBLIC
    spdlog::spdlog
    SimpleIni::SimpleIni
    Taskflow
    nlohmann_json::nlohmann_json
    httplib::httplib
    Shared
    Qt6::Core
)
# crashpad 相关库（PRIVATE 依赖）
target_link_libraries(Core PRIVATE
    crashpad::crashpad
)

# 暴露头文件目录
target_include_directories(Core PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/3rdparty
)


