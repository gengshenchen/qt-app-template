#pragma once
#include <vector>

#include <QWidget>

namespace Ui {
class LogViewer;
}

class LogViewer : public QWidget {
    Q_OBJECT
public:
    explicit    LogViewer(QWidget* parent = nullptr);
    ~LogViewer();
public slots:
    void onNewMessage(const QString& message);
    void newFunction(int value, const std::string& name);

private:
    Ui::LogViewer* ui;
};
