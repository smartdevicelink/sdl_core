#include "../../include/JSONHandler/ALRPCObjects/SyncMsgVersion.h"
#include "SyncMsgVersionMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



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


