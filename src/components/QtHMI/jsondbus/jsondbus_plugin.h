#ifndef JSONDBUS_PLUGIN_H
#define JSONDBUS_PLUGIN_H

#include <QQmlExtensionPlugin>

class JsondbusPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
    
public:
    void registerTypes(const char *uri);
};

#endif // JSONDBUS_PLUGIN_H

