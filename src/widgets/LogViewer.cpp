#include "LogViewer.h"

#include "core/Logger.h"
#include "ui_LogViewer.h"

LogViewer::LogViewer(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::LogViewer) {
    ui->setupUi(this);
    connect(&Logger::instance(), &Logger::newMessage, this, &LogViewer::onNewMessage);
}

LogViewer::~LogViewer() {
    delete ui;
}

void LogViewer::onNewMessage(const QString& message) {
    ui->plainTextEdit->appendPlainText(message);
}

void LogViewer::newFunction(int value, const std::string& name) {
    (void)value;
    (void)name;
}
