#ifndef FORD_FRAMEWORK_PLUGIN_H
#define FORD_FRAMEWORK_PLUGIN_H

#include <QQmlExtensionPlugin>

class Ford_FrameworkPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
    
public:
    void registerTypes(const char *uri);
};

#endif // FORD_FRAMEWORK_PLUGIN_H

