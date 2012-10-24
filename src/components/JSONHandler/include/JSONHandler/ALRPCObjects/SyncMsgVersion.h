#ifndef SYNCMSGVERSION_INCLUDE
#define SYNCMSGVERSION_INCLUDE




/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



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

  unsigned int majorVersion;	//!<  (1,10)
  unsigned int minorVersion;	//!<  (0,1000)
};

#endif
