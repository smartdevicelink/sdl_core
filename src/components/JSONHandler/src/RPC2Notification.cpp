/**
* \file RPC2Notification.cpp
* \brief RPC2Notification class source file.
* \author PVyshnevska
*/


#include "JSONHandler/RPC2Notification.h"

using namespace RPC2Communication;

RPC2Notification::RPC2Notification(void) :
RPC2Command(NOTIFICATION)
{
}

RPC2Notification::RPC2Notification(int method) :
RPC2Command(NOTIFICATION, method)
{
}


RPC2Notification::~RPC2Notification()
{
}



