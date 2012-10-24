#ifndef ONENCODEDSYNCPDATA_INCLUDE
#define ONENCODEDSYNCPDATA_INCLUDE

#include <vector>
#include <string>

#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
  const std::string* get_URL(void) const;
  const int* get_Timeout(void) const;

  void reset_data(void);
  bool set_data(const std::vector<std::string>& data_);
  void reset_URL(void);
  bool set_URL(const std::string& URL_);
  void reset_Timeout(void);
  bool set_Timeout(int Timeout_);

private:

  friend class OnEncodedSyncPDataMarshaller;

  std::vector<std::string>* data;	//!<   [%s..%s] (10000)
  std::string* URL;	//!< (1000)
  int* Timeout;	//!<  (-2000000000,2000000000)
};

#endif
