# 为测试创建单独的可执行文件
add_executable(CoreTests
    tst_Core.cpp
)

# 链接需要测试的库和QtTest
target_link_libraries(CoreTests PRIVATE
    Core
    Qt6::Test
)

# 将测试添加到 CTest
include(CTest)
add_test(NAME CoreTests COMMAND CoreTests)
