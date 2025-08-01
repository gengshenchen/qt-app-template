#include <QtTest/QtTest>  // 引入 Qt Test 主要头文件

#include <QSignalSpy>
// #include <QTest>

#include "core/Logger.h"  // from core

class WidgetsTest : public QObject {
    Q_OBJECT
  private slots:
    void testLoggerSingleton();
    void testLoggerSignal();
};

void WidgetsTest::testLoggerSingleton() {
    Logger& logger1 = Logger::instance();
    Logger& logger2 = Logger::instance();
    QCOMPARE(&logger1, &logger2);
}

void WidgetsTest::testLoggerSignal() {
    Logger& logger = Logger::instance();
    QSignalSpy spy(&logger, &Logger::newMessage);
    QVERIFY(spy.isValid());

    logger.log("Test Message");
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString().contains("Test Message"));
}

QTEST_MAIN(WidgetsTest)
#include "tst_widgets.moc"

