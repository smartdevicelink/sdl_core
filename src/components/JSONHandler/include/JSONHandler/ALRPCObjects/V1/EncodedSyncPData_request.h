#ifndef NSAPPLINKRPC_ENCODEDSYNCPDATA_REQUEST_INCLUDE
#define NSAPPLINKRPC_ENCODEDSYNCPDATA_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Allows encoded data in the form of SyncP packets to be sent to the SYNC module

  class EncodedSyncPData_request : public ALRPCRequest
  {
  public:
  
    EncodedSyncPData_request(const EncodedSyncPData_request& c);
    EncodedSyncPData_request(void);
    
    virtual ~EncodedSyncPData_request(void);
  
    EncodedSyncPData_request& operator =(const EncodedSyncPData_request&);
  
    bool checkIntegrity(void);

    const std::vector<std::string>* get_data(void) const;

    void reset_data(void);
    bool set_data(const std::vector<std::string>& data_);

  private:
  
    friend class EncodedSyncPData_requestMarshaller;


///  Contains base64 encoded string of SyncP packets.
      std::vector<std::string>* data;	//!<   [%s..%s] (10000)
  };

}

#endif
