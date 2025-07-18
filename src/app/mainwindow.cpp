#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/LogViewer.h" // from gui-widgets
#include "core/Logger.h"
#include "core/log/log.h"

#include <QDirIterator>
#include <QDockWidget>
#include <QQuickWidget>
#include <QQmlContext>
#include <QDir>
#include <QQuickView>
#include <QVBoxLayout>
#include <QQmlEngine>
MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
        ui->setupUi(this);
        setWindowTitle(tr("My App"));
        setWindowIcon(QIcon(":/icons/app-icon.svg"));
setFixedWidth(800); // 设置窗口宽度为800像素
        setFixedHeight(600); // 设置窗口高度为60素
        setupMenus();
        LOGINFO("hello world");
       // setupDocks();
#if 0        
        // ---  ---
        qDebug() << "=========================================================";
        qDebug() << "Listing all available application resources at runtime...";
        QDirIterator it(":", QDirIterator::Subdirectories);
        bool foundAny = false;
        while (it.hasNext()) {
            foundAny = true;
            // 打印找到的每一个资源的路径
            qDebug() << "  Found resource:" << it.next();
        }
        if (!foundAny) {
            qDebug() << "  !!! CRITICAL: No resources found inside the application. !!!";
            qDebug() << "  This confirms the resource file was not linked correctly.";
        }
        qDebug() << "=========================================================";
#endif // --- 调试代码结束 ---

        embedQmlView();

        Logger::instance().log("Main Window constructed and configured.");
    }

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupMenus() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::setupDocks() {
    // 创建 Log Viewer Dock
    auto* logDock = new QDockWidget("Log Viewer", this);
    m_logViewer = new LogViewer(logDock);
    logDock->setWidget(m_logViewer);
    addDockWidget(Qt::BottomDockWidgetArea, logDock);
    Logger::instance().log("Log viewer dock created.");
}

void MainWindow::embedQmlView() {
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *quickWidget = new QQuickWidget(this);
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

#ifdef USE_STATIC_QML_MODULES
    
    qDebug() << "use static qml module";
    quickWidget->engine()->addImportPath("qrc:/qt/qml");
    quickWidget->setSource(QUrl("qrc:/qt/qml/app/ui/Dashboard/Dashboard.qml")); 

#else

    qDebug() << "use shared qml module";
    QString strQmlRootPath =  QCoreApplication::applicationDirPath() + "/../qml";
    quickWidget->engine()->addImportPath(strQmlRootPath);

    QString qmlFilePath = (strQmlRootPath) + "/app/ui/Dashboard/Dashboard.qml";
    quickWidget->setSource(QUrl::fromLocalFile(qmlFilePath));

    qDebug() << "QCoreApplication::applicationDirPath:" << QCoreApplication::applicationDirPath();
    qDebug() << "strQmlRootPath:" << strQmlRootPath;
#endif

#if 1
    qDebug() << "QML import paths:";
    for (const auto& path :  quickWidget->engine()->importPathList()) {
        qDebug() << path;
    }
#endif

    layout->addWidget(quickWidget);
    setCentralWidget(central);
}
