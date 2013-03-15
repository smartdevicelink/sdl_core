//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_UPDATETURNLIST_INCLUDE
#define NSRPC2COMMUNICATION_UI_UPDATETURNLIST_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/Turn.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/SoftButton.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class UpdateTurnList : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      UpdateTurnList(const UpdateTurnList& c);
      UpdateTurnList(void);
    
      UpdateTurnList& operator =(const UpdateTurnList&);
    
      virtual ~UpdateTurnList(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsSmartDeviceLinkRPCV2::Turn>& get_turnList(void);

      const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& get_softButtons(void);

      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_turnList(const std::vector< NsSmartDeviceLinkRPCV2::Turn>& turnList);

/// 0 <= size <= 1
      bool set_softButtons(const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& softButtons);

      bool set_appId(int appId);


    private:

      friend class UpdateTurnListMarshaller;

      std::vector< NsSmartDeviceLinkRPCV2::Turn> turnList;
      std::vector< NsSmartDeviceLinkRPCV2::SoftButton> softButtons;
      int appId;

    };
  }
}

#endif
