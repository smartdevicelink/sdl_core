#ifndef NSAPPLINKRPC_ADDCOMMAND_REQUEST_INCLUDE
#define NSAPPLINKRPC_ADDCOMMAND_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "MenuParams.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
{

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


///  unique ID of the command to add.
      unsigned int cmdID;	//!<  (0,2000000000)

///  Optional sub value containing menu parameters
      MenuParams* menuParams;

/**
     An array of strings to be used as VR sysnonyms for this command.
     If this array is provided, it may not be empty.
*/
      std::vector<std::string>* vrCommands;	//!<   [%s..%s] (99)
  };

}

#endif
