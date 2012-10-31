#include "JSONHandler/Alert.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

Alert::Alert()
:RPC2Request( RPC2Marshaller::METHOD_ALERT_REQUEST )
{
    mAlertText1 = 0;
    mAlertText2 = 0;
    mDuration = 0;
    mPlayTone = 0;
}

Alert::~Alert()
{
    delete mAlertText1;
    delete mAlertText2;
    delete mDuration;
    delete mPlayTone;
}

const std::string * Alert::getAlertText1() const
{
    return mAlertText1;
}
        
const std::string * Alert::getAlertText2() const
{
    return mAlertText2;
}
        
int * Alert::getDuration() const
{
    return mDuration;
}
        

bool * Alert::getPlayTone() const
{
    return mPlayTone;
}

void Alert::setAlertText1( const std::string & str )
{
    delete mAlertText1;
    mAlertText1 = new std::string(str);
}

void Alert::setAlertText2( const std::string & str )
{
    delete mAlertText2;
    mAlertText2 = new std::string(str);
}
        
void Alert::setDuration( int duration )
{
    delete mDuration;
    mDuration = new int(duration);
}
        
void Alert::setPlayTone( bool playTone )
{
    delete mPlayTone;
    mPlayTone = new bool(playTone);
}