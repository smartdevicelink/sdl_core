#include "JSONHandler/Show.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

Show::Show()
:RPC2Request( RPC2Marshaller::METHOD_SHOW_REQUEST )
{
    mAlignment = 0;
}

Show::~Show()
{
    if (mAlignment)
        delete mAlignment;
    mAlignment = 0;
}

void Show::setMainField1(const std::string & str)
{
    mMainField1 = str;
}
        
void Show::setMainField2(const std::string & str)
{
    mMainField2 = str;
}
        
void Show::setTextAlignment(const TextAlignment& s)
{
    delete mAlignment;
    mAlignment = new TextAlignment(s);
}
        
void Show::setStatusBar(const std::string& s)
{
    mStatusBar = s;
}
        
void Show::setMediaClock(const std::string& s)
{
    mMediaClock = s;
}
        
void Show::setMediaTrack(const std::string& str)
{
    mMediaTrack = str;
}

std::string Show::getMainField1() const
{
    return mMainField1;
}
        
std::string Show::getMainField2() const
{
    return mMainField2;
}
        
const TextAlignment * Show::getTextAlignment() const
{
    return mAlignment;
}
        
std::string Show::getStatusBar() const
{
    return mStatusBar;
}
        
std::string Show::getMediaClock() const
{
    return mMediaClock;
}
        
std::string Show::getMediaTrack() const
{
    return mMediaTrack;
}