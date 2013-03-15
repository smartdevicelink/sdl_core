//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_ADDCOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_UI_ADDCOMMAND_INCLUDE

#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/MenuParams.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Image.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
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

      const NsSmartDeviceLinkRPCV2::MenuParams& get_menuParams(void);

      const NsSmartDeviceLinkRPCV2::Image* get_cmdIcon(void);
      int get_appId(void);


// setters
/// cmdId <= 2000000000
      bool set_cmdId(unsigned int cmdId);

      bool set_menuParams(const NsSmartDeviceLinkRPCV2::MenuParams& menuParams);

      bool set_cmdIcon(const NsSmartDeviceLinkRPCV2::Image& cmdIcon);

      void reset_cmdIcon(void);

      bool set_appId(int appId);


    private:

      friend class AddCommandMarshaller;

      unsigned int cmdId;
      NsSmartDeviceLinkRPCV2::MenuParams menuParams;
      NsSmartDeviceLinkRPCV2::Image* cmdIcon;
      int appId;

    };
  }
}

#endif
