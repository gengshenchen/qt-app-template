#include <chrono>
#include <iostream>
#include <thread>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QTranslator>
#include <qatomic.h>

#include "core/Logger.h"
#include "core/config/config_manager.h"
#include "core/crash/crashpad_handler.h"
#include "core/log/log.h"
#include "core/network/network_manager.h"
#include "core/platform/path_manager.h"
#include "core/task/task_manager.h"
#include "mainwindow.h"
#include "shared/Constants.h"
#include "version.h"

#ifdef USE_STATIC_QML_MODULES
#include <QQmlEngineExtensionPlugin>
Q_IMPORT_QML_PLUGIN(app_uiPlugin)  // URI app.ui
#endif

void listResources(const QString& path) {
    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "目录不存在:" << path;
        return;
    }
    QStringList entries = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& entry : entries) {
        QString fullPath = path + "/" + entry;
        if (QDir(fullPath).exists()) {
            // 是目录，递归进去
            listResources(fullPath);
        } else {
            // 是文件
            qDebug() << "资源文件:" << fullPath;
        }
    }
}

void benchmark(const std::string& mode_desc, int N) {
    using namespace std::chrono;
    std::cout << "Starting test: " << mode_desc << std::endl;
    int x{8};
    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        LOGINFO("Log message number {}", i);
        LOGTRACE("Log message number {}", i);
    }
    auto end = high_resolution_clock::now();
    auto dur_ms = duration_cast<milliseconds>(end - start).count();
    std::cout << "Finished " << N << " logs in " << dur_ms << "ms" << std::endl;
}

void setup_crashpad() {
    // 1. 定义路径和参数
    std::string handler_path =
        "/home/karl/work/qt-app-template/3rdparty/crashpad/bin/"
        "crashpad_handler";  // 在Linux/macOS
    // std::string handler_path = "crashpad_handler.exe"; // 在Windows
    std::string db_path = "/home/karl/work/qt-app-template/out/crashes";
    //    std::string upload_url =
    //    "https://submit.backtrace.io/cgs/e461fe30fdad99925439a62c599edf7d66b7e007d1379d2359a7e2380af62130/minidump";

    std::string upload_url =
        "https://submit.backtrace.io/cgs/"
        "dbeda80fb3f6b7ce2e48659e62206d795be35210a80e993460a9261a4ba0c4ff/"
        "minidump";

    // 2. 定义自定义元数据
    std::map<std::string, std::string> annotations = {{"format", "minidump"},
                                                      {"product", "qt-app-template"},
                                                      {"version", "1.2.6"},
                                                      {"user_id", "user-12345"}};
    // 3. 定义附件
    std::vector<std::string> attachments = {
        "/home/karl/work/qt-app-template/out/logs/"
        "qt-app-template.rotating.log"};

    // 4. 初始化
    bool initialized = qt_app_template::core::CrashpadHandler::instance().initialize(
        handler_path, db_path, upload_url, annotations, attachments);

    if (initialized) {
        std::cout << "Crashpad initialized successfully." << std::endl;
    } else {
        std::cerr << "Failed to initialize Crashpad." << std::endl;
    }
}
void crash_now() {
    volatile int* ptr = nullptr;
    *ptr = 42;
}

int main(int argc, char* argv[]) {
#ifdef NDEBUG
    std::cout << "This is a RELEASE build." << std::endl;
#else
    std::cout << "This is a DEBUG build." << std::endl;
#endif
    QApplication a(argc, argv);
    a.setOrganizationName(Constants::ORG_NAME);
    a.setApplicationName(Constants::APP_NAME);
    // PathManager是第一個被實例化的，因為其他管理器依賴它
    auto& paths = qt_app_template::core::PathManager::instance();
    qt_app_template::core::ConfigManager::instance().load(paths.config_dir().string());

    setup_crashpad();

    const int log_count = 1132;
    // clang-format off
    qt_app_template::core::Log::instance().init({
        .use_async = true,
        .log_dir = paths.log_dir(),
        .log_name = Constants::APP_NAME.toStdString(),
    });
    // clang-format on

    qt_app_template::core::Log::instance().set_level(spdlog::level::trace);

    // benchmark("vim-commentarySync + Rotating", log_count);

    qt_app_template::core::ConfigManager::instance().load(
        (paths.config_dir() / "settings.ini").string());

    // 使用版本信息
    LOGINFO("Starting {} version {}",
            Constants::APP_NAME.toStdString(),
            qt_app_template::version::kVersionString.data());
    LOGINFO("exe path: {}", paths.executable_path().string());
    LOGINFO("data path: {}", paths.data_dir().string());
    LOGINFO("executable_dir : {}", paths.executable_dir().string());
    LOGINFO("cache_dir : {}", paths.cache_dir().string());
    LOGINFO("log_dir : {}", paths.log_dir().string());
    LOGINFO("machine_config_dir : {}", paths.machine_config_dir().string());
    LOGINFO("resources_dir : {}", paths.resources_dir().string());
    // 2. 使用任務管理器執行異步任務
    std::stringstream ss;
    ss << std::this_thread::get_id();
    LOGINFO("主線程ID: {}", ss.str());
    qt_app_template::core::TaskManager::instance().enqueue([]() {
        LOGINFO("這是一個來自後台線程的問候！線程ID: {}",
                std::hash<std::thread::id>{}(std::this_thread::get_id()));
    });

    // 3. 使用網絡管理器獲取數據
    qt_app_template::core::NetworkManager::instance().get(
        "https://jsonplaceholder.typicode.com", "/todos/1", [](bool success, const json& data) {
            if (success) {
                LOGINFO("網絡請求成功，獲取標題: {}", data["title"].get<std::string>());
                // 【關鍵】如果要在回調中更新UI，必須使用Qt的機制切回主線程
                // QMetaObject::invokeMethod(...);
            } else {
                LOGERROR("網絡請求失敗: {}", data["error"].get<std::string>());
            }
        });
    QTranslator translator;
    // const QLocale currentLocale = QLocale();
    const QLocale currentLocale("zh_CN");
    // 2. 定义翻译文件的基础名 (与项目名一致)
    const QString baseName = QCoreApplication::applicationName();

    // 3. 定义翻译文件在资源系统中的搜索路径
    const QString path = ":/i18n/translations/";
    if (translator.load(currentLocale, baseName, "_", path)) {
        QCoreApplication::installTranslator(&translator);
        qDebug() << "Successfully loaded translation for" << currentLocale.name();
    } else {
        qDebug() << "Could not load translation for" << currentLocale.name();
    }

    Logger::instance().log("Application starting...");
    MainWindow w;
    w.show();
    Logger::instance().log("Main window shown.");
    int result = a.exec();

    qt_app_template::core::Log::instance().deinit();
    return result;
}

