add_library(WidgetsUI
    LogViewer.cpp
    # LogViewer.ui 会被 CMAKE_AUTOUIC 自动处理
)
target_link_libraries(WidgetsUI PUBLIC
    Core
    Qt6::Widgets
)
target_include_directories(WidgetsUI PUBLIC
    #${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src

)
