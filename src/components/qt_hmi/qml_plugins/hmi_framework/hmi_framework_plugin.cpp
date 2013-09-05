#include "hmi_framework_plugin.h"
#include "hmiframework.h"

#include <qqml.h>

void Ford_FrameworkPlugin::registerTypes(const char *uri)
{
    // @uri com.ford
    qmlRegisterType<hmiframework>(uri, 1, 0, "hmiframework");
}


