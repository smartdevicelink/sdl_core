#ifndef DELETECOMMAND_REQUEST_INCLUDE
#define DELETECOMMAND_REQUEST_INCLUDE


#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
