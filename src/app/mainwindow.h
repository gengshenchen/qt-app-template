#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LogViewer;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupDocks();
    void setupMenus();
    void embedQmlView();

    Ui::MainWindow *ui;
    LogViewer* m_logViewer = nullptr;
};
