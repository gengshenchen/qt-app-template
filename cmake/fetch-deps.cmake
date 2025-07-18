#下载github上的库，out/_deps  ，FetchContent_MakeAvailable(spdlog) 类似addsubdirectory 的动作，希望项目是完全自包含,所以不使用这种方式

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gengshenchen/spdlog.git   
    GIT_TAG        v1.15.3 
)

FetchContent_MakeAvailable(spdlog)
