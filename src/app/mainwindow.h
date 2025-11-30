#pragma once
#include <QMainWindow>
#define SHORT_NAME 42
#define LONGER_NAME 0x007f
#define EVEN_LONGER_NAME (2)
#define foo(x) (x * x)
#define bar(y, z) (y + z)

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class LogViewer;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void testJJJJ();

private:
    void setupDocks();
    void setupMenus();
    void embedQmlView();
    Ui::MainWindow* ui;
    LogViewer* m_logViewer = nullptr;
};
