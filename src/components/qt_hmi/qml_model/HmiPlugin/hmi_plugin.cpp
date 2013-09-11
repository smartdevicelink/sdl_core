#include "hmi_plugin.h"
#include "masked_container.h"
#include "attributedmouseevent.h"

#include <QtQml>

void HmiPlugin::registerTypes(const char *uri)
{
    // @uri com.ford.hmiplugin
    qmlRegisterType<MaskedContainer>(uri, 1, 0, "MaskedContainer");
    qmlRegisterType<AttributedMouseEvent>();
}


