#ifndef NSRPC2COMMUNICATION_UI_ADDCOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_UI_ADDCOMMAND_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V1/MenuParams.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Image.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
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

      const NsAppLinkRPCV2::Image* get_cmdIcon(void);
      int get_appId(void);


// setters
/// cmdId <= 2000000000
      bool set_cmdId(unsigned int cmdId);

      bool set_menuParams(const NsAppLinkRPC::MenuParams& menuParams);

      bool set_cmdIcon(const NsAppLinkRPCV2::Image& cmdIcon);

      void reset_cmdIcon(void);

      bool set_appId(int appId);


    private:

      friend class AddCommandMarshaller;

      unsigned int cmdId;
      NsAppLinkRPC::MenuParams menuParams;
      NsAppLinkRPCV2::Image* cmdIcon;
      int appId;

    };
  }
}

#endif
