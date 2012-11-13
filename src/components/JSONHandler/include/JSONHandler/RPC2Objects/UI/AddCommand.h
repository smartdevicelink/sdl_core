#ifndef RPC2COMMUNICATION_UI_ADDCOMMAND_INCLUDE
#define RPC2COMMUNICATION_UI_ADDCOMMAND_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/MenuParams.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace UI
  {

    class AddCommand : public ::RPC2Communication::RPC2Request
    {
    public:
    
      AddCommand(const AddCommand& c);
      AddCommand(void);
    
      AddCommand& operator =(const AddCommand&);
    
      virtual ~AddCommand(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_cmdId(void);

      const ::AppLinkRPC::MenuParams& get_menuParams(void);


// setters
/// cmdId <= 2000000000
      bool set_cmdId(unsigned int cmdId);

      bool set_menuParams(const ::AppLinkRPC::MenuParams& menuParams);


    private:

      friend class AddCommandMarshaller;

      unsigned int cmdId;
      ::AppLinkRPC::MenuParams menuParams;

    };
  }
}

#endif
