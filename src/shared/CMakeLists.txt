# INTERFACE库是一种特殊的库，它本身不产生编译产物（.a或.so文件）
# 它只用于封装“用法要求”（usage requirements），比如头文件目录和编译定义
# 非常适合用于仅包含头文件的共享库
add_library(Shared INTERFACE)

# 将当前目录作为公共的头文件搜索路径暴露给所有链接到Shared库的目标
target_include_directories(Shared INTERFACE
    ${PROJECT_SOURCE_DIR}/src
)
