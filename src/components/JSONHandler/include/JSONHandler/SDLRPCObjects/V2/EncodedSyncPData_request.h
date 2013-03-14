#ifndef NSAPPLINKRPCV2_ENCODEDSYNCPDATA_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_ENCODEDSYNCPDATA_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{

///  Allows encoded data in the form of SyncP packets to be sent to the SYNC module

  class EncodedSyncPData_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    EncodedSyncPData_request(const EncodedSyncPData_request& c);
    EncodedSyncPData_request(void);
    
    virtual ~EncodedSyncPData_request(void);
  
    bool checkIntegrity(void);

    const std::vector<std::string>& get_data(void) const;

    bool set_data(const std::vector<std::string>& data_);

  private:
  
    friend class EncodedSyncPData_requestMarshaller;


///  Contains base64 encoded string of SyncP packets.
      std::vector<std::string> data;	//!<   [%s..%s] (10000)
  };

}

#endif
