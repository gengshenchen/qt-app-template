#include "js_bridge.h"
#include "core/log/log.h"
#include <QMessageBox>

JsBridge::JsBridge(QObject* parent) : QObject(parent) {}

void JsBridge::test(const QString& message) {
    LOGINFO("Message from JS: {}", message.toStdString());
    QMessageBox::information(nullptr, "Message from JS", message);
}
