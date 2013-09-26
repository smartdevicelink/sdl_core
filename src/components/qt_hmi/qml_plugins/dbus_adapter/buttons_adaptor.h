#ifndef BUTTONSADAPTOR_H
#define BUTTONSADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QDBusContext>

#include "button_capabilities.h"
#include "preset_bank_capabilities.h"

QT_BEGIN_NAMESPACE
    class QQuickItem;
QT_END_NAMESPACE

class ButtonsAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.ford.sdl.hmi.Buttons")
    Q_CLASSINFO("D-Bus Introspection",
"<interface name=\"com.ford.sdl.hmi.Buttons\">\n"
"  <method name=\"GetCapabilities\">\n"
//"    <annotation name=\"org.qtproject.QtDBus.QtTypeName.Out0\" value=\"QList&lt;ButtonCapabilities&gt;\"/>\n"
//"    <annotation name=\"org.qtproject.QtDBus.QtTypeName.Out1\" value=\"OptionalArgument&lt;PresetBankCapabilities&gt;\"/>\n"
"    <arg direction=\"out\" type=\"a(ibbb)\" name=\"capabilities\" />\n"
"    <arg direction=\"out\" type=\"(b(b))\" name=\"presetBankCapabilities\" />\n"
"  </method>\n"
"  <signal name=\"Notify\">\n"
"    <arg name=\"what\" type=\"s\"/>\n"
"  </signal>\n"
"</interface>"
)
public:
    explicit ButtonsAdaptor(QObject *parent = 0);
    void setButtonsApi(QQuickItem*);
signals:
    void Notify(QString what);
public slots:
    QList<ButtonCapabilities> GetCapabilities(OptionalArgument<PresetBankCapabilities> &);
    void Hello();

private:
    QQuickItem *buttonsApi;
};

#endif // BUTTONSADAPTOR_H
