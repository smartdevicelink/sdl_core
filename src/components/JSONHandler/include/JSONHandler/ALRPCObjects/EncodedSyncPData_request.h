#ifndef ENCODEDSYNCPDATA_REQUEST_INCLUDE
#define ENCODEDSYNCPDATA_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
