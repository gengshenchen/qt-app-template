#pragma once
#include <QObject>
#include <QString>

class Logger : public QObject {
    Q_OBJECT
public:
    static Logger& instance();
    void log(const QString& message);
    void test();
signals:
    void newMessage(const QString& message);

private:
    Logger() = default;
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;
};
