#ifndef FORDSDLCORE_PLUGIN_H
#define FORDSDLCORE_PLUGIN_H

#include <QQmlExtensionPlugin>

class FordSdlCorePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
    
public:
    void registerTypes(const char *uri);
};

#endif // FORDSDLCORE_PLUGIN_H

