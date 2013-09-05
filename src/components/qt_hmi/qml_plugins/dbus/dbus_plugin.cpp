#include "dbus_plugin.h"
#include "api.h"

#include <qqml.h>

void DbusPlugin::registerTypes(const char *uri)
{
    // @uri sdl.core.api
    qmlRegisterType<Api>(uri, 1, 0, "Api");
}


