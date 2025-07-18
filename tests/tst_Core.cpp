#include <QTest>
#include <QSignalSpy>
#include "core/Logger.h" // from core

class CoreTest : public QObject {
    Q_OBJECT
private slots:
    void testLoggerSingleton();
    void testLoggerSignal();
};

void CoreTest::testLoggerSingleton() {
    Logger& logger1 = Logger::instance();
    Logger& logger2 = Logger::instance();
    QCOMPARE(&logger1, &logger2);
}

void CoreTest::testLoggerSignal() {
    Logger& logger = Logger::instance();
    QSignalSpy spy(&logger, &Logger::newMessage);
    QVERIFY(spy.isValid());

    logger.log("Test Message");
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toString().contains("Test Message"));
}

QTEST_MAIN(CoreTest)
#include "tst_Core.moc"
