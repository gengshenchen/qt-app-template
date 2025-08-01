add_library(WidgetsUi
    LogViewer.cpp
    # LogViewer.ui 会被 CMAKE_AUTOUIC 自动处理
)
target_link_libraries(WidgetsUi PUBLIC
    Core
    Qt6::Widgets
)
target_include_directories(WidgetsUi PUBLIC
    #${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src

)
