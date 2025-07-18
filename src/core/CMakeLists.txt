# Crashpad 路径
set(CRASHPAD_DIR ${PROJECT_SOURCE_DIR}/3rdparty/crashpad/crashpad)

# 添加 crashpad_client
add_library(crashpad_client STATIC IMPORTED)
set_target_properties(crashpad_client PROPERTIES
    IMPORTED_LOCATION ${CRASHPAD_DIR}/out/Default/obj/client/libclient.a
    INTERFACE_LINK_LIBRARIES "crashpad_util"
)
add_library(crashpad_common STATIC IMPORTED)
set_target_properties(crashpad_common PROPERTIES
    IMPORTED_LOCATION ${CRASHPAD_DIR}/out/Default/obj/client/libcommon.a
    INTERFACE_LINK_LIBRARIES "crashpad_util"
)

# 添加 crashpad_util
add_library(crashpad_util STATIC IMPORTED)
set_target_properties(crashpad_util PROPERTIES
    IMPORTED_LOCATION ${CRASHPAD_DIR}/out/Default/obj/util/libutil.a
    INTERFACE_LINK_LIBRARIES "mini_chromium_base"

)

# 添加 mini_chromium_base
add_library(mini_chromium_base STATIC IMPORTED)
set_target_properties(mini_chromium_base PROPERTIES
    IMPORTED_LOCATION ${CRASHPAD_DIR}/out/Default/obj/third_party/mini_chromium/mini_chromium/base/libbase.a

)

# 所有 crashpad 相关头文件
set(CRASHPAD_INCLUDE_DIRS
    ${CRASHPAD_DIR}
    ${CRASHPAD_DIR}/third_party/mini_chromium/mini_chromium
)

# 给所有 crashpad 库设置头文件路径（INTERFACE）
target_include_directories(crashpad_client INTERFACE ${CRASHPAD_INCLUDE_DIRS})
target_include_directories(crashpad_util INTERFACE ${CRASHPAD_INCLUDE_DIRS})
target_include_directories(mini_chromium_base INTERFACE ${CRASHPAD_INCLUDE_DIRS})


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
    crashpad_client
    crashpad_common
    crashpad_util
    mini_chromium_base
)

# 暴露头文件目录
target_include_directories(Core PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)


