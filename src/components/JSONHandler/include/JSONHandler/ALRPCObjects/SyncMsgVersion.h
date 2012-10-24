#ifndef SYNCMSGVERSION_INCLUDE
#define SYNCMSGVERSION_INCLUDE




/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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

  unsigned int majorVersion;	//!<  (1,1)
  unsigned int minorVersion;	//!<  (0,1000)
};

#endif
