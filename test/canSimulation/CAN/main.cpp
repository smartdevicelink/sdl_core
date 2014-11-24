#include <QApplication>
#include <QQmlApplicationEngine>
#include "server.h"
#include "FileSystem.h"
#include "loger.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    // Step 1: get access to the root object

    QObject *rootObject = engine.rootObjects().first();

    Server myServer(rootObject);
    FileSystem fileSystem;
    Loger::rootView=rootObject;

    QMetaObject::invokeMethod(rootObject, "setCurrentPath",
            Q_ARG(QVariant,  QDir::currentPath()));

    // Step 2: connect qml signal to C++ slot
    QObject::connect(rootObject,SIGNAL(viewClicked(QString)), &myServer, SLOT(write(QString)));
    QObject::connect(rootObject,SIGNAL(createConnection(QString, int)), &myServer, SLOT(createConection(QString, int)));
    QObject::connect(rootObject,SIGNAL(sendMessageTCP(QString)), &myServer, SLOT(write(QString)));
    QObject::connect(rootObject,SIGNAL(saveLog(QString, QString)), &fileSystem, SLOT(write(QString, QString)));


    return app.exec();
}
