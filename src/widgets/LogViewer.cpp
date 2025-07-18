#include "LogViewer.h"
#include "ui_LogViewer.h"
#include "core/Logger.h"

LogViewer::LogViewer(QWidget *parent) : QWidget(parent), ui(new Ui::LogViewer) {
    ui->setupUi(this);
    connect(&Logger::instance(), &Logger::newMessage, this, &LogViewer::onNewMessage);
}

LogViewer::~LogViewer() {
    delete ui;
}

void LogViewer::onNewMessage(const QString &message) {
    ui->plainTextEdit->appendPlainText(message);
}
