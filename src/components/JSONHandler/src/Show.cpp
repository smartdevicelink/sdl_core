#include "JSONHandler/Show.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

Show::Show()
:RPC2Request( RPC2Marshaller::METHOD_SHOW_REQUEST )
{
    mAlignment = 0;
    mMainField1 = 0;
    mMainField2 = 0;
    mStatusBar = 0;
    mMediaClock = 0;
    mMediaTrack = 0;
}

Show::~Show()
{
    if (mAlignment)
        delete mAlignment;
    mAlignment = 0;
    delete mMainField1;
    delete mMainField2;
    delete mStatusBar;
    delete mMediaTrack;
    delete mMediaClock;
}

void Show::setMainField1(const std::string & str)
{
    delete mMainField1;
    mMainField1 = new std::string(str);
}
        
void Show::setMainField2(const std::string & str)
{
    delete mMainField2;
    mMainField2 = new std::string(str);
}
        
void Show::setTextAlignment(const TextAlignment& s)
{
    delete mAlignment;
    mAlignment = new TextAlignment(s);
}
        
void Show::setStatusBar(const std::string& s)
{
    delete mStatusBar;
    mStatusBar = new std::string(s);
}
        
void Show::setMediaClock(const std::string& s)
{
    delete mMediaClock;
    mMediaClock = new std::string(s);
}
        
void Show::setMediaTrack(const std::string& str)
{
    delete mMediaTrack;
    mMediaTrack = new std::string(str);
}

const std::string* Show::getMainField1() const
{
    return mMainField1;
}
        
const std::string * Show::getMainField2() const
{
    return mMainField2;
}
        
const TextAlignment * Show::getTextAlignment() const
{
    return mAlignment;
}
        
const std::string* Show::getStatusBar() const
{
    return mStatusBar;
}
        
const std::string* Show::getMediaClock() const
{
    return mMediaClock;
}
        
const std::string* Show::getMediaTrack() const
{
    return mMediaTrack;
}