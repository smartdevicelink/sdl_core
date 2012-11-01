#include "JSONHandler/AddSubMenuResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

AddSubMenuResponse::AddSubMenuResponse() 
: RPC2Response( RPC2Marshaller::METHOD_ADDSUBMENU_RESPONSE ) 
{}
        
AddSubMenuResponse::~AddSubMenuResponse() 
{}