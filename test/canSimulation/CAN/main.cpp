#include <QApplication>
#include <QQmlApplicationEngine>
#include "server.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));   

    // Step 1: get access to the root object

    QObject *rootObject = engine.rootObjects().first();

    Server myServer(rootObject);

    // Step 2: connect qml signal to C++ slot
    qDebug() << QObject::connect(rootObject,SIGNAL(viewClicked(QString)), &myServer, SLOT(write(QString)));
    qDebug() << QObject::connect(rootObject,SIGNAL(createConnection(QString, int)), &myServer, SLOT(createConection(QString, int)));

    //qDebug() << QObject::connect(&myServer, SIGNAL(), rootObject, SLOT(incoming(QString)));

    return app.exec();
}
