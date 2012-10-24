#ifndef ADDCOMMAND_REQUEST_INCLUDE
#define ADDCOMMAND_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Image.h"
#include "MenuParams.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
  const Image* get_cmdIcon(void) const;

  bool set_cmdID(unsigned int cmdID_);
  void reset_menuParams(void);
  bool set_menuParams(const MenuParams& menuParams_);
  void reset_vrCommands(void);
  bool set_vrCommands(const std::vector<std::string>& vrCommands_);
  void reset_cmdIcon(void);
  bool set_cmdIcon(const Image& cmdIcon_);

private:

  friend class AddCommand_requestMarshaller;

  unsigned int cmdID;	//!<  (0,2000000000)
  MenuParams* menuParams;
  std::vector<std::string>* vrCommands;	//!<   [%s..%s] (99)
  Image* cmdIcon;
};

#endif
