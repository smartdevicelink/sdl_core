#include "preset_bank_capabilities.h"

QQuickPresetBankCapabilities::QQuickPresetBankCapabilities(const PresetBankCapabilities &value, QQuickItem *parent)
    : QQuickItem(parent),
      value(value)
{
}

QQuickPresetBankCapabilities::QQuickPresetBankCapabilities(QQuickItem *parent)
    : QQuickItem(parent)
{
}

bool QQuickPresetBankCapabilities::onScreenPresetsAvailable() const
{
    return value.onScreenPresetsAvailable;
}

QDBusArgument& operator << (QDBusArgument& arg, const PresetBankCapabilities& pbc)
{
    arg.beginStructure();
    arg << pbc.onScreenPresetsAvailable;
    arg.endStructure();
    return arg;
}

const QDBusArgument& operator >> (const QDBusArgument& arg, PresetBankCapabilities& pbc)
{
    arg.beginStructure();
    arg >> pbc.onScreenPresetsAvailable;
    arg.endStructure();
    return arg;
}
