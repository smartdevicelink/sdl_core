#ifndef CANFSTEST_H
#define CANFSTEST_H

#include <QObject>
#include <QtTest>
#include <QFile>
#include <QTextStream>
#include "FileSystem.h"

class canFSTest : public QObject
{
    Q_OBJECT
public:
    explicit canFSTest(QObject *parent = 0);

private Q_SLOTS:
    void writeToFSTest();

};

#endif // CANFSTEST_H
