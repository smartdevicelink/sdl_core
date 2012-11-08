#include "OnReadyMarshaller.h"
#include "JSONHandler/RPC2Objects/UI/Marshaller.h"

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;

OnReady::OnReady()
:RPC2Notification(Marshaller::METHOD_ONREADY)
{}
        
OnReady::~OnReady()
{}