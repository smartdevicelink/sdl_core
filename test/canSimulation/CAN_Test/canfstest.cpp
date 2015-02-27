#include "canfstest.h"
#include <QDir>

canFSTest::canFSTest(QObject *parent) :
    QObject(parent)
{
}

void canFSTest::writeToFSTest()
{
    QString path = QDir::currentPath() + "/canFSTest.log";
    QString testData = "Hello World!";
    QString dataFromFile;

    FileSystem fileSystem;

    QFile file(path);

    fileSystem.write(path, testData);

    QTest::qWait(1000);

    QVERIFY2(file.open(QFile::ReadOnly), "File can not be open");

    QTextStream in(&file);

    while (!in.atEnd()) {
    dataFromFile.append(in.readLine());
    }

    QCOMPARE(dataFromFile, testData);

}
