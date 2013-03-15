//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SyncMsgVersion.h"
#include "SyncMsgVersionMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

SyncMsgVersion::SyncMsgVersion(const SyncMsgVersion& c)
{
  *this=c;
}


bool SyncMsgVersion::checkIntegrity(void)
{
  return SyncMsgVersionMarshaller::checkIntegrity(*this);
}


SyncMsgVersion::SyncMsgVersion(void)
{
}



bool SyncMsgVersion::set_majorVersion(unsigned int majorVersion_)
{
  if(majorVersion_>10)  return false;
  if(majorVersion_<1)  return false;
  majorVersion=majorVersion_;
  return true;
}

bool SyncMsgVersion::set_minorVersion(unsigned int minorVersion_)
{
  if(minorVersion_>1000)  return false;
  minorVersion=minorVersion_;
  return true;
}




unsigned int SyncMsgVersion::get_majorVersion(void) const
{
  return majorVersion;
}


unsigned int SyncMsgVersion::get_minorVersion(void) const
{
  return minorVersion;
}


