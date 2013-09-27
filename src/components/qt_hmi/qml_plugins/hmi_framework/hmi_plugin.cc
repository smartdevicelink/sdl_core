#include "hmi_plugin.h"
#include "masked_container.h"
#include "attributedmouseevent.h"

#include <QtQml>

void HmiFramework::registerTypes(const char *uri)
{
    // @uri com.ford.hmi_framework
    qmlRegisterType<MaskedContainer>(uri, 1, 0, "MaskedContainer");
    qmlRegisterType<AttributedMouseEvent>();
}


