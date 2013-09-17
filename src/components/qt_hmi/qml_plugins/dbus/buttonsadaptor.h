#ifndef BUTTONSADAPTOR_H
#define BUTTONSADAPTOR_H

#include <QDBusAbstractAdaptor>

#include "buttonCapabilities.h"
#include "presetBankCapabilities.h"

class ButtonsAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "sdl.core.api.Buttons")
    Q_CLASSINFO("D-Bus Introspection", ""
"<interface name=\"sdl.core.api.Buttons\">"
"  <method name=\"Hello\"/>"
"  <method name=\"GetCapabilities\">"
"    <annotation name=\"org.qtproject.QtDBus.QtTypeName.Out0\" value=\"QList&ltButtonCapabilities;&gt;\">"
"    <annotation name=\"org.qtproject.QtDBus.QtTypeName.Out1\" value=\"OptionalArgument&lt;PresetBankCapabilities&gt;\">"
"    <arg direction=\"out\" type=\"a(ibbb)\" name=\"capabilities\">"
"    <arg direction=\"out\" type=\"(b(b))\" name=\"presetBankCapabilities\">"
"  <method>"
"</interface>"
)
public:
    explicit ButtonsAdaptor(QObject *parent = 0);
    
signals:
    
public slots:
    QList<ButtonCapabilities> GetCapabilities(OptionalArgument<PresetBankCapabilities> &);
    void Hello();
};

#endif // BUTTONSADAPTOR_H
