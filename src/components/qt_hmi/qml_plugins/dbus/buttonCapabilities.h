#ifndef BUTTON_CAPABILITIES_H
#define BUTTON_CAPABILITIES_H

#include <QQuickItem>

#include "optionalArgument.h"
#include <QDBusArgument>

struct ButtonCapabilities
{
    int name;
    bool shortPressAvailable;
    bool longPressAvailable;
    bool upDownAvailable;
};

class QQuickButtonCapabilities : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int name READ name)
    Q_PROPERTY(bool shortPressAvailable READ shortPressAvailable)
    Q_PROPERTY(bool longPressAvailable READ longPressAvailable)
    Q_PROPERTY(bool upDownAvailable READ upDownAvailable)
public:
    QQuickButtonCapabilities(QQuickItem *parent = NULL);
    QQuickButtonCapabilities(const ButtonCapabilities&, QQuickItem *parent = NULL);

    int name() const;
    bool shortPressAvailable() const;
    bool longPressAvailable() const;
    bool upDownAvailable() const;
private:
    ButtonCapabilities value;
};

QDBusArgument& operator << (QDBusArgument&, const ButtonCapabilities&);
const QDBusArgument& operator >> (const QDBusArgument&, ButtonCapabilities&);

Q_DECLARE_METATYPE(ButtonCapabilities)
Q_DECLARE_METATYPE(OptionalArgument<ButtonCapabilities>)

Q_DECLARE_METATYPE(QList<ButtonCapabilities>)
Q_DECLARE_METATYPE(OptionalArgument<QList<ButtonCapabilities> >)

QML_DECLARE_TYPE(QQuickButtonCapabilities)

#endif // BUTTON_CAPABILITIES_H
