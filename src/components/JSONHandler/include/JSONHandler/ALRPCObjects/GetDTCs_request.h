#ifndef GETDTCS_REQUEST_INCLUDE
#define GETDTCS_REQUEST_INCLUDE


#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Vehicle module diagnostic trouble code request.

class GetDTCs_request : public ALRPCRequest
{
public:

  GetDTCs_request(const GetDTCs_request& c);
  GetDTCs_request(void);
  
  virtual ~GetDTCs_request(void);

  GetDTCs_request& operator =(const GetDTCs_request&);

  bool checkIntegrity(void);

  const unsigned int* get_ecuName(void) const;
  const bool* get_encrypted(void) const;

  void reset_ecuName(void);
  bool set_ecuName(unsigned int ecuName_);
  void reset_encrypted(void);
  bool set_encrypted(bool encrypted_);

private:

  friend class GetDTCs_requestMarshaller;

  unsigned int* ecuName;	//!<  (0,65535)
  bool* encrypted;
};

#endif
