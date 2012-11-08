#include "AppMgr/HMIHandler.h"

namespace NsAppManager
{

const HMIHandler& HMIHandler::getInstance()
{
    static HMIHandler instance;
    return instance;
}

HMIHandler::HMIHandler()
{
}

HMIHandler::HMIHandler(const HMIHandler &)
{
}

}
