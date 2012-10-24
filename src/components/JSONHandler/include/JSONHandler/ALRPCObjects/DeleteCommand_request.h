#ifndef DELETECOMMAND_REQUEST_INCLUDE
#define DELETECOMMAND_REQUEST_INCLUDE


#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Deletes all commands from the in-application menu with the specified command id.

class DeleteCommand_request : public ALRPCRequest
{
public:

  DeleteCommand_request(const DeleteCommand_request& c);
  DeleteCommand_request(void);
  
  virtual ~DeleteCommand_request(void);

  bool checkIntegrity(void);

  unsigned int get_cmdID(void) const;

  bool set_cmdID(unsigned int cmdID_);

private:

  friend class DeleteCommand_requestMarshaller;

  unsigned int cmdID;	//!<  (0,2000000000)
};

#endif
