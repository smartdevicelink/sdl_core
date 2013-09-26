#include "hmi_plugin.h"
#include "masked_container.h"
#include "attributed_mouse_event.h"

#include <QtQml>

void HmiHWButtons::registerTypes(const char *uri)
{
    // @uri com.ford.hmi_framework
    qmlRegisterType<MaskedContainer>(uri, 1, 0, "MaskedContainer");
    qmlRegisterType<AttributedMouseEvent>();
}


