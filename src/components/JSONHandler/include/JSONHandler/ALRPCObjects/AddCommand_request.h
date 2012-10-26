#ifndef ADDCOMMAND_REQUEST_INCLUDE
#define ADDCOMMAND_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "MenuParams.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


/**
     Adds a command to the in application menu.
     Either menuParams or vrCommands must be provided.
*/

class AddCommand_request : public ALRPCRequest
{
public:

  AddCommand_request(const AddCommand_request& c);
  AddCommand_request(void);
  
  virtual ~AddCommand_request(void);

  AddCommand_request& operator =(const AddCommand_request&);

  bool checkIntegrity(void);

  unsigned int get_cmdID(void) const;
  const MenuParams* get_menuParams(void) const;
  const std::vector<std::string>* get_vrCommands(void) const;

  bool set_cmdID(unsigned int cmdID_);
  void reset_menuParams(void);
  bool set_menuParams(const MenuParams& menuParams_);
  void reset_vrCommands(void);
  bool set_vrCommands(const std::vector<std::string>& vrCommands_);

private:

  friend class AddCommand_requestMarshaller;

  unsigned int cmdID;	//!<  (0,2000000000)
  MenuParams* menuParams;
  std::vector<std::string>* vrCommands;	//!<   [%s..%s] (99)
};

#endif
