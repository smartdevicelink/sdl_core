#include "buttonsadaptor.h"

ButtonsAdaptor::ButtonsAdaptor(QObject *parent) :
    QDBusAbstractAdaptor(parent)
{

}

QList<ButtonCapabilities> ButtonsAdaptor::GetCapabilities(OptionalArgument<PresetBankCapabilities> &pbc)
{
    QList<ButtonCapabilities> rc;
    ButtonCapabilities bc;
    bc.name = 10;
    bc.shortPressAvailable = true;
    bc.longPressAvailable = false;
    bc.upDownAvailable = true;
    rc.append(bc);

    PresetBankCapabilities p;
    p.onScreenPresetsAvailable = true;
    pbc = OptionalArgument<PresetBankCapabilities>(p);

    qDebug() << "ButtonsAdaptor::GetCapabilities";

    return rc;
}


void ButtonsAdaptor::Hello()
{
    qDebug() << "ButtonsAdaptor::Hello";
}
