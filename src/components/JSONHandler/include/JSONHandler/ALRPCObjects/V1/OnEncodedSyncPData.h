#ifndef NSAPPLINKRPC_ONENCODEDSYNCPDATA_INCLUDE
#define NSAPPLINKRPC_ONENCODEDSYNCPDATA_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

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


///  Contains base64 encoded string of SyncP packets.
      std::vector<std::string>* data;	//!<   [%s..%s] (10000)
  };

}

#endif
