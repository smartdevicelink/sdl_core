#include <QtGui/QGuiApplication>
#include <QQmlContext>
#include "qtquick2applicationviewer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;

    viewer.addImportPath(QStringLiteral("plugins"));

    viewer.setMainQmlFile(QStringLiteral("qml/Bananas/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
