#ifndef HMI_PLUGIN_H
#define HMI_PLUGIN_H

#include <QQmlExtensionPlugin>

class HmiPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
    
public:
    void registerTypes(const char *uri);
};

#endif // HMI_PLUGIN_H

