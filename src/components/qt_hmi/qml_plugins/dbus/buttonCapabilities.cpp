#include "buttonCapabilities.h"

QQuickButtonCapabilities::QQuickButtonCapabilities(const ButtonCapabilities &value, QQuickItem *parent)
    : QQuickItem(parent),
      value(value)
{
}

int QQuickButtonCapabilities::name() const
{
    return value.name;
}

bool QQuickButtonCapabilities::shortPressAvailable() const
{
    return value.shortPressAvailable;
}

bool QQuickButtonCapabilities::longPressAvailable() const
{
    return value.longPressAvailable;
}

bool QQuickButtonCapabilities::upDownAvailable() const
{
    return value.upDownAvailable;
}

QDBusArgument& operator << (QDBusArgument& arg, const ButtonCapabilities& bc)
{
    arg.beginStructure();
    arg << bc.name << bc.shortPressAvailable << bc.longPressAvailable << bc.upDownAvailable;
    arg.endStructure();
    return arg;
}

const QDBusArgument& operator >> (const QDBusArgument& arg, ButtonCapabilities &bc)
{
    arg.beginStructure();
    arg >> bc.name >> bc.shortPressAvailable >> bc.longPressAvailable >> bc.upDownAvailable;
    arg.endStructure();
    return arg;
}
