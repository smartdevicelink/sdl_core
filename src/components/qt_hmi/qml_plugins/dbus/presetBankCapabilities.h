#ifndef PRESET_BANK_CAPABILITIES_H
#define PRESET_BANK_CAPABILITIES_H

#include <QQuickItem>

#include "optionalArgument.h"

struct PresetBankCapabilities
{
    bool onScreenPresetsAvailable;
};

class QQuickPresetBankCapabilities : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool onScreenPresetsAvailable READ onScreenPresetsAvailable)
public:
    QQuickPresetBankCapabilities(const PresetBankCapabilities&, QQuickItem *parent = NULL);

    bool onScreenPresetsAvailable() const;
private:
    PresetBankCapabilities value;
};

QDBusArgument& operator << (QDBusArgument&, const PresetBankCapabilities&);
const QDBusArgument& operator >> (const QDBusArgument&, PresetBankCapabilities&);

Q_DECLARE_METATYPE(PresetBankCapabilities)
Q_DECLARE_METATYPE(OptionalArgument<PresetBankCapabilities>)

Q_DECLARE_METATYPE(QList<PresetBankCapabilities>)
Q_DECLARE_METATYPE(OptionalArgument<QList<PresetBankCapabilities> >)

QML_DECLARE_TYPE(QQuickPresetBankCapabilities)

#endif // PRESET_BANK_CAPABILITIES_H
