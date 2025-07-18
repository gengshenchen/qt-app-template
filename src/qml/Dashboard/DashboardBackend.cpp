#include "DashboardBackend.h"
#include "core/Logger.h"
#include <QRandomGenerator>
DashboardBackend::DashboardBackend(QObject *parent) : QObject(parent) {
    connect(&m_timer, &QTimer::timeout, this, &DashboardBackend::updateData);
    m_timer.start(1000);
    Logger::instance().log("Dashboard Backend Initialized.");
}

int DashboardBackend::cpuUsage() const { return m_cpuUsage; }
int DashboardBackend::memUsage() const { return m_memUsage; }

void DashboardBackend::resetValues() {
    Logger::instance().log("QML requested to reset dashboard values.");
    m_cpuUsage = 0;
    m_memUsage = 0;
    emit cpuUsageChanged();
    emit memUsageChanged();
    volatile int* ptr = nullptr;
    *ptr = 42;
}

void DashboardBackend::updateData() {
    m_cpuUsage = (m_cpuUsage + QRandomGenerator::global()->bounded(5)) % 101;
    m_memUsage = (m_memUsage + QRandomGenerator::global()->bounded(3)) % 101;

    emit cpuUsageChanged();
    emit memUsageChanged();
}
