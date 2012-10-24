#ifndef ONENCODEDSYNCPDATA_INCLUDE
#define ONENCODEDSYNCPDATA_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


///  Callback including encoded data of any SyncP packets that SYNC needs to send back to the mobile device.

class OnEncodedSyncPData : public ALRPCNotification
{
public:

  OnEncodedSyncPData(const OnEncodedSyncPData& c);
  OnEncodedSyncPData(void);
  
  virtual ~OnEncodedSyncPData(void);

  OnEncodedSyncPData& operator =(const OnEncodedSyncPData&);

  bool checkIntegrity(void);

  const std::vector<std::string>* get_data(void) const;

  void reset_data(void);
  bool set_data(const std::vector<std::string>& data_);

private:

  friend class OnEncodedSyncPDataMarshaller;

  std::vector<std::string>* data;	//!<   [%s..%s] (10000)
};

#endif
