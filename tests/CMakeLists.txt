# 開啟測試功能
enable_testing()

# --------------------------------------------------------------------
#  Google Test Suite for the 'core' library
# --------------------------------------------------------------------

add_executable(core_tests
    # 為了清晰，我們把所有core庫的gtest用例都放在一個文件裡
    tst_core_managers_gtest.cc
)

# 2. 為測試目標鏈接所有必要的庫
target_link_libraries(core_tests PRIVATE
    # 要測試的目標庫
    Core
    # Core庫的依賴，測試程序也需要
    spdlog::spdlog
    SimpleIni::SimpleIni
    Taskflow
    nlohmann_json::nlohmann_json
    httplib::httplib
    Shared
    # Google Test 框架
    gtest
    gtest_main
)

# 3. 使用gtest_discover_tests自動發現並註冊所有測試用例到CTest
include(GoogleTest)
gtest_discover_tests(core_tests)

# --------------------------------------------------------------------
#  Qt Test Suite for the 'widgets' library (未來預留)
# --------------------------------------------------------------------
 if(TARGET WidgetsUi)
     add_executable(widgets_ui_qttests
         tst_widgets.cpp
     )
     target_link_libraries(widgets_ui_qttests PRIVATE
         WidgetsUi
         Qt6::Test
     )
     add_test(NAME WidgetsUiQtTests COMMAND widgets_ui_qttests)
 endif()

target_include_directories(core_tests PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/3rdparty
)
