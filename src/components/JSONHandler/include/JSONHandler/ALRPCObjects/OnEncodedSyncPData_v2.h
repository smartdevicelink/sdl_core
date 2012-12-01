#ifndef NSAPPLINKRPC_ONENCODEDSYNCPDATA_V2_INCLUDE
#define NSAPPLINKRPC_ONENCODEDSYNCPDATA_V2_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/ALRPCNotification.h"


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

///  Callback including encoded data of any SyncP packets that SYNC needs to send back to the mobile device.

  class OnEncodedSyncPData_v2 : public ALRPCNotification
  {
  public:
  
    OnEncodedSyncPData_v2(const OnEncodedSyncPData_v2& c);
    OnEncodedSyncPData_v2(void);
    
    virtual ~OnEncodedSyncPData_v2(void);
  
    OnEncodedSyncPData_v2& operator =(const OnEncodedSyncPData_v2&);
  
    bool checkIntegrity(void);

    const std::vector<std::string>* get_data(void) const;
    const std::string* get_URL(void) const;
    const int* get_Timeout(void) const;

    void reset_data(void);
    bool set_data(const std::vector<std::string>& data_);
    void reset_URL(void);
    bool set_URL(const std::string& URL_);
    void reset_Timeout(void);
    bool set_Timeout(int Timeout_);

  private:
  
    friend class OnEncodedSyncPData_v2Marshaller;


///  Contains base64 encoded string of SyncP packets.
      std::vector<std::string>* data;	//!<   [%s..%s] (10000)

/**
     If blank, the SyncP data shall be forwarded to the app.
     If not blank, the SyncP data shall be forwarded to the provided URL.
*/
      std::string* URL;	//!< (1000)

/**
     If blank, the SyncP data shall be forwarded to the app.
     If not blank, the SyncP data shall be forwarded with the provided timeout in seconds.
*/
      int* Timeout;	//!<  (-2000000000,2000000000)
  };

}

#endif
