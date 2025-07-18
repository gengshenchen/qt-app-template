add_executable(${PROJECT_NAME} 
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${PROJECT_SOURCE_DIR}/resources/resources.qrc
)

if(USE_STATIC_QML_MODULES)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_STATIC_QML_MODULES)
endif()

# 链接我们创建的所有模块和必要的Qt模块
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickWidgets
  
    Core
    WidgetsUI
    QmlUI
    # 静态库需要 链接插件目标
    $<$<BOOL:${USE_STATIC_QML_MODULES}>:QmlUIplugin> 
    #$<IF:$<BOOL:${USE_STATIC_QML_MODULES}>,QmlUIplugin,"">        # 插件库本身
)

qt_import_qml_plugins(${PROJECT_NAME})# 静态插件引入（官方 Qt 插件）

# --- 国际化(i18n)配置 ---
qt_add_translations(${PROJECT_NAME}
    TS_OUTPUT_DIRECTORY ../../resources/i18n
    QM_OUTPUT_DIRECTORY translations
)

