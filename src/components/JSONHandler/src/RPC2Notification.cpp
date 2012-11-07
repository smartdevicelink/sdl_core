
#include "JSONHandler/RPC2Notification.h"

using namespace RPC2Communication;

RPC2Notification::RPC2Notification(void) : RPC2Command(NOTIFICATION)
{
}

RPC2Notification::RPC2Notification(int method) : RPC2Command(NOTIFICATION, method)
{
}


RPC2Notification::~RPC2Notification()
{
}



