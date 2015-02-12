#include "cantest.h"

CANTest::CANTest(QObject *parent) :
    QObject(parent)
{
}

void CANTest::myFirstTest()
{
    QVERIFY2( 1 == 0, "First test not passed, congratulations!");
}
