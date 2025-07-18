#pragma once
#include <QObject>
#include <QTimer>
#include <qqmlintegration.h>

class DashboardBackend : public QObject {
    Q_OBJECT
    Q_PROPERTY(int cpuUsage READ cpuUsage NOTIFY cpuUsageChanged)
    Q_PROPERTY(int memUsage READ memUsage NOTIFY memUsageChanged)

    QML_ELEMENT
public:
    explicit DashboardBackend(QObject *parent = nullptr);
    int cpuUsage() const;
    int memUsage() const;

public slots:
    Q_INVOKABLE void resetValues();

signals:
    void cpuUsageChanged();
    void memUsageChanged();

private:
    void updateData();
    QTimer m_timer;
    int m_cpuUsage = 0;
    int m_memUsage = 0;
};
