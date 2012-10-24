#ifndef READDID_REQUEST_INCLUDE
#define READDID_REQUEST_INCLUDE

#include <vector>

#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Non periodic vehicle data read request.

class ReadDID_request : public ALRPCRequest
{
public:

  ReadDID_request(const ReadDID_request& c);
  ReadDID_request(void);
  
  virtual ~ReadDID_request(void);

  ReadDID_request& operator =(const ReadDID_request&);

  bool checkIntegrity(void);

  const unsigned int* get_ecuName(void) const;
  const std::vector<unsigned int>* get_didLocation(void) const;
  const bool* get_encrypted(void) const;

  void reset_ecuName(void);
  bool set_ecuName(unsigned int ecuName_);
  void reset_didLocation(void);
  bool set_didLocation(const std::vector<unsigned int>& didLocation_);
  void reset_encrypted(void);
  bool set_encrypted(bool encrypted_);

private:

  friend class ReadDID_requestMarshaller;

  unsigned int* ecuName;	//!<  (0,65535)
  std::vector<unsigned int>* didLocation;	//!<   [%s..%s]  (0,65535)
  bool* encrypted;
};

#endif
