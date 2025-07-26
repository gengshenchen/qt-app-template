# 定义一个名为Core的静态库(STATIC)或动态库(SHARED)
add_library(Core STATIC
    Logger.cpp
    log/log.cpp
    crash/crashpad_handler.cc
)

# Core库需要链接Shared库来获取常量定义，并链接Qt6::Core来使用QObject等
target_link_libraries(Core PUBLIC
    spdlog::spdlog
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
)


