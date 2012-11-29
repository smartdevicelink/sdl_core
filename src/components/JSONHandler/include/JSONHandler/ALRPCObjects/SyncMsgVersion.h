#ifndef NSAPPLINKRPC_SYNCMSGVERSION_INCLUDE
#define NSAPPLINKRPC_SYNCMSGVERSION_INCLUDE




/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/


namespace NsAppLinkRPC
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
