#ifndef ENCODEDSYNCPDATA_REQUEST_INCLUDE
#define ENCODEDSYNCPDATA_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

  std::vector<std::string>* data;	//!<   [%s..%s] (10000)
};

#endif
