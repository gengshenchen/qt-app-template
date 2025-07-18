#include "shared/Constants.h"
#include "core/Logger.h"
#include "core/log/log.h"
#include "core/crash/crashpad_handler.h"
#include "mainwindow.h"
#include "spdlog/common.h"
#include <iostream>
#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QDebug>

#ifdef USE_STATIC_QML_MODULES
#include <QQmlEngineExtensionPlugin>
Q_IMPORT_QML_PLUGIN(app_uiPlugin) // URI app.ui
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
    std::string handler_path = "/home/karl/work/qt-app-template/3rdparty/crashpad/bin/crashpad_handler"; // 在Linux/macOS
    // std::string handler_path = "crashpad_handler.exe"; // 在Windows
    std::string db_path = "/home/karl/work/qt-app-template/out/crashes";
//    std::string upload_url =  "https://submit.backtrace.io/cgs/e461fe30fdad99925439a62c599edf7d66b7e007d1379d2359a7e2380af62130/minidump";
        
   std::string upload_url =        "https://submit.backtrace.io/cgs/dbeda80fb3f6b7ce2e48659e62206d795be35210a80e993460a9261a4ba0c4ff/minidump";

    // 2. 定义自定义元数据
    std::map<std::string, std::string> annotations = {
        {"format", "minidump"},
        {"product", "qt-app-template"},
        {"version", "1.2.6"},
        {"user_id", "user-12345"}
    };
    // 3. 定义附件
    std::vector<std::string> attachments = {"/home/karl/work/qt-app-template/out/logs/qt-app-template.rotating.log"};

    // 4. 初始化
    bool initialized = core::CrashpadHandler::instance().initialize(
        handler_path,
        db_path,
        upload_url,
        annotations,
        attachments
    );

    if (initialized) {
        std::cout << "Crashpad initialized successfully." << std::endl;
    } else {
        std::cerr << "Failed to initialize Crashpad." << std::endl;
    }}
void crash_now() {
    volatile int* ptr = nullptr;
    *ptr = 42;
}

int main(int argc, char *argv[]) {
#ifdef NDEBUG
    std::cout << "This is a RELEASE build." << std::endl;
#else
    std::cout << "This is a DEBUG build." << std::endl;
#endif

    QApplication a(argc, argv);
    a.setOrganizationName(Constants::ORG_NAME);
    a.setApplicationName(Constants::APP_NAME);

    setup_crashpad();

    //std::filesystem::path exe_dir = std::filesystem::current_path();
    QString exe_dir = QCoreApplication::applicationDirPath();  // 可执行文件所在路径
    std::string log_dir = (std::filesystem::path(exe_dir.toStdString()) / "../logs").string();
    const int log_count = 1132;
    core::Log::instance().init(
            {.use_async = true,
            .log_dir = log_dir,
            .log_name = Constants::APP_NAME.toStdString()
            });
    core::Log::instance().set_level(spdlog::level::trace);
    benchmark("Sync + Rotating", log_count);

    QTranslator translator;
    // const QLocale currentLocale = QLocale();
    const QLocale currentLocale("zh_CN");  
    // 2. 定义翻译文件的基础名 (与项目名一致)
    const QString baseName = QCoreApplication::applicationName();

    // 3. 定义翻译文件在资源系统中的搜索路径
    const QString path = ":/i18n/translations/";
    if (translator.load(currentLocale, baseName, "_" , path ))   {
        QCoreApplication::installTranslator(&translator);
        qDebug() << "Successfully loaded translation for" << currentLocale.name();
    } else {
        qDebug() << "Could not load translation for" << currentLocale.name() ;  
    }

    Logger::instance().log("Application starting...");
    MainWindow w;
    w.show();
    Logger::instance().log("Main window shown.");
    int result = a.exec();

    core::Log::instance().deinit();

    return result;
}


