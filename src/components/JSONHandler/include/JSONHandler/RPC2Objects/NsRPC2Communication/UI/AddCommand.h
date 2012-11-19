#ifndef NSRPC2COMMUNICATION_UI_ADDCOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_UI_ADDCOMMAND_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/MenuParams.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class AddCommand : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      AddCommand(const AddCommand& c);
      AddCommand(void);
    
      AddCommand& operator =(const AddCommand&);
    
      virtual ~AddCommand(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_cmdId(void);

      const NsAppLinkRPC::MenuParams& get_menuParams(void);


// setters
/// cmdId <= 2000000000
      bool set_cmdId(unsigned int cmdId);

      bool set_menuParams(const NsAppLinkRPC::MenuParams& menuParams);


    private:

      friend class AddCommandMarshaller;

      unsigned int cmdId;
      NsAppLinkRPC::MenuParams menuParams;

    };
  }
}

#endif
