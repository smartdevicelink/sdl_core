#ifndef NSAPPLINKRPCV2_ONENCODEDSYNCPDATA_INCLUDE
#define NSAPPLINKRPCV2_ONENCODEDSYNCPDATA_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Callback including encoded data of any SyncP packets that SYNC needs to send back to the mobile device.

  class OnEncodedSyncPData : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    OnEncodedSyncPData(const OnEncodedSyncPData& c);
    OnEncodedSyncPData(void);
    
    virtual ~OnEncodedSyncPData(void);
  
    OnEncodedSyncPData& operator =(const OnEncodedSyncPData&);
  
    bool checkIntegrity(void);

    const std::vector<std::string>& get_data(void) const;
    const std::string* get_URL(void) const;
    const int* get_Timeout(void) const;

    bool set_data(const std::vector<std::string>& data_);
    void reset_URL(void);
    bool set_URL(const std::string& URL_);
    void reset_Timeout(void);
    bool set_Timeout(int Timeout_);

  private:
  
    friend class OnEncodedSyncPDataMarshaller;


///  Contains base64 encoded string of SyncP packets.
      std::vector<std::string> data;	//!<   [%s..%s] (10000)

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
