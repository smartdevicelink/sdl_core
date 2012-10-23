#include "JSONHandler/Alert.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

Alert::Alert()
:RPC2Request( RPC2Marshaller::METHOD_ALERT_REQUEST )
{}

Alert::~Alert()
{}

std::string Alert::getAlertText1() const
{
    return mAlertText1;
}
        
std::string Alert::getAlertText2() const
{
    return mAlertText2;
}
        
int Alert::getDuration() const
{
    return mDuration;
}
        

bool Alert::getPlayTone() const
{
    return mPlayTone;
}

void Alert::setAlertText1( const std::string & str )
{
    mAlertText1 = str;
}

void Alert::setAlertText2( const std::string & str )
{
    mAlertText2 = str;
}
        
void Alert::setDuration( int duration )
{
    mDuration = duration;
}
        
void Alert::setPlayTone( bool playTone )
{
    mPlayTone = playTone;
}