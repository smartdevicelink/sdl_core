#include <QtTest/QTest>
#include "cantcptest.h"
#include "canwstest.h"
#include "canfstest.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    canTCPTest test1;
    canWSTest test2;
    canFSTest test3;

    QTest::qExec(&test1, argc, argv);
    QTest::qExec(&test2, argc, argv);
    QTest::qExec(&test3, argc, argv);

    return 0;
}
