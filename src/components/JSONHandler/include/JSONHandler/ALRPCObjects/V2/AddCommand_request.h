#ifndef NSAPPLINKRPCV2_ADDCOMMAND_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_ADDCOMMAND_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Image.h"
#include "MenuParams.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

/**
     Adds a command to the in application menu.
     Either menuParams or vrCommands must be provided.
*/

  class AddCommand_request : public NsAppLinkRPC::ALRPCMessage
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


///  unique ID of the command to add.
      unsigned int cmdID;	//!<  (0,2000000000)

///  Optional sub value containing menu parameters
      MenuParams* menuParams;

/**
     An array of strings to be used as VR synonyms for this command.
     If this array is provided, it may not be empty.
*/
      std::vector<std::string>* vrCommands;	//!<   [%s..%s] (99)

/**
     Image struct determining whether static or dynamic icon.
     If omitted on supported displays, no (or the default if applicable) icon shall be displayed.
*/
      Image* cmdIcon;
  };

}

#endif
