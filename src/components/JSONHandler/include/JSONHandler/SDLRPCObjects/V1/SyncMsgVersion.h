//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_SYNCMSGVERSION_INCLUDE
#define NSSMARTDEVICELINKKRPC_SYNCMSGVERSION_INCLUDE




/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/


namespace NsSmartDeviceLinkRPC
{

///  Specifies the version number of the SYNC V4 protocol, that is supported by the mobile application

  class SyncMsgVersion
  {
  public:
  
    SyncMsgVersion(const SyncMsgVersion& c);
    SyncMsgVersion(void);
  
    bool checkIntegrity(void);
  // getters

    unsigned int get_majorVersion(void) const;
    unsigned int get_minorVersion(void) const;

// setters

    bool set_majorVersion(unsigned int majorVersion_);
    bool set_minorVersion(unsigned int minorVersion_);

  private:

    friend class SyncMsgVersionMarshaller;


///  The major version indicates versions that is not-compatible to previous versions.
      unsigned int majorVersion;	//!<  (1,1)

///  The minor version indicates a change to a previous version that should still allow to be run on an older version (with limited functionality)
      unsigned int minorVersion;	//!<  (0,1000)
  };

}

#endif
