#include <QApplication>
#include <QQmlApplicationEngine>
#include "canlib.h"
#include "loger.h"

QObject* rootObject = NULL;

static void incomingMessageFromTCP(const QString &qMessage){

    QMetaObject::invokeMethod(rootObject, "incomingTCP",
            Q_ARG(QVariant,  qMessage));

}

static void incomingMessageFromWS(const QString &qMessage){

    QMetaObject::invokeMethod(rootObject, "incomingWS",
            Q_ARG(QVariant,  qMessage));

}

static void incomingLog(const QString &qMessage, int color){

   Loger::loger(qMessage, color);
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));


    // Step 1: get access to the root object
    rootObject = engine.rootObjects().first();

    Loger::rootView = rootObject;

    // Path to log.txt is set co directory where CAN is executed
    QMetaObject::invokeMethod(rootObject, "setCurrentPath",
            Q_ARG(QVariant,  QDir::currentPath()));

    CANlib CAN;

    CAN.init();

    // Step 2: connect qml signal to C++ slot
    QObject::connect(rootObject,SIGNAL(createConnectionTCP(QString, int)), &CAN, SLOT(createConectionTCP(const QString&, int)));
    QObject::connect(rootObject,SIGNAL(sendMessageTCP(QString)), &CAN, SLOT(writeToTCP(const QString&)));

    QObject::connect(rootObject,SIGNAL(saveLogToFile(QString, QString)), &CAN, SLOT(writeToFS(const QString&, const QString&)));

    QObject::connect(rootObject,SIGNAL(createConnectionWS(QString, int)), &CAN, SLOT(createConectionWS(const QString&, int)));
    QObject::connect(rootObject,SIGNAL(sendMessageWS(QString)), &CAN, SLOT(writeToWS(const QString&)));

    QObject::connect(&CAN, &CANlib::incomingMesageTCP, &incomingMessageFromTCP);

    QObject::connect(&CAN, &CANlib::incomingMesageWS, &incomingMessageFromWS);

    QObject::connect(&CAN, &CANlib::loger, &incomingLog);

    return app.exec();
}
