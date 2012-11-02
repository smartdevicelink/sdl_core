#include "JSONHandler/DeleteSubMenuResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

DeleteSubMenuResponse::DeleteSubMenuResponse() 
: RPC2Response( RPC2Marshaller :: METHOD_DELETESUBMENU_RESPONSE ) 
{}
        
DeleteSubMenuResponse::~DeleteSubMenuResponse() 
{}