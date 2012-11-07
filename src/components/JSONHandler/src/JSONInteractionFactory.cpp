#include "JSONHandler/Alert.h"
#include "JSONHandler/ALRPCObjects/Alert_request.h"
#include "JSONHandler/AlertResponse.h"
#include "JSONHandler/ALRPCObjects/Alert_response.h"
#include "JSONHandler/JSONInteractionFactory.h"


RPC2Communication::Alert * ALAlertToRPC2Alert::ALAlertToRPC2Alert( 
        const Alert_request& alAlert )
{
    RPC2Communication::Alert * result = new RPC2Communication::Alert;
    if ( alAlert.get_alertText1() )
    {
        result->setAlertText1( *alAlert.get_alertText1() );
    }

    if ( alAlert.get_alertText2() )
    {
        result->setAlertText2( *alAlert.get_alertText2() );
    }

    if ( alAlert.get_duration() )
    {
        result->setDuration( *alAlert.get_duration() );
    }

    if ( alAlert.get_playTone() )
    {
        result->setPlayTone( *alAlert.get_playTone() );
    }

    return result;
}

Alert_response RPC2AlertResponseToALResponse( 
        const RPC2Communication::AlertResponse& rpc2AlertResponse )
{
    
}

