#include "JSONHandler/RPC2Notification.h"

using namespace RPC2Communication;

RPC2Notification::RPC2Notification( )
:RPC2Command(  )
{}

RPC2Notification::RPC2Notification( int method )
:RPC2Command( method )
{}

RPC2Notification::~RPC2Notification() 
{}