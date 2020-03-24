#include <QDebug>
#include <QObject>
#include <QTest>

#include "qutepart.h"


class Test: public QObject {
    Q_OBJECT

private slots:
    void Append() {
        Qutepart::Qutepart qpart(nullptr, "asdf\nfd");
        qpart.lines().append("xyz");
        QCOMPARE(qpart.toPlainText(), QString("asdf\nfd\nxyz"));
    }
};

QTEST_MAIN(Test)
#include "test_lines.moc"
