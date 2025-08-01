#include "Logger.h"

#include <QDateTime>
#include <QDebug>

Logger& Logger::instance() {
    static Logger loggerInstance;
    return loggerInstance;
}

void Logger::log(const QString& message) {
    QString timedMessage =
        QString("[%1] %2")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
            .arg(message);
    qDebug().noquote() << timedMessage;
    emit newMessage(timedMessage);
}
 
