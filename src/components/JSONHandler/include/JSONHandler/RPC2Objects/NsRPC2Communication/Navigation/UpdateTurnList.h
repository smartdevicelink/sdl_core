#ifndef NSRPC2COMMUNICATION_NAVIGATION_UPDATETURNLIST_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_UPDATETURNLIST_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V2/Turn.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SoftButton.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Navigation
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
      const std::vector< NsAppLinkRPCV2::Turn>& get_turnList(void);

      const std::vector< NsAppLinkRPCV2::SoftButton>& get_softButtons(void);

      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_turnList(const std::vector< NsAppLinkRPCV2::Turn>& turnList);

/// 0 <= size <= 1
      bool set_softButtons(const std::vector< NsAppLinkRPCV2::SoftButton>& softButtons);

      bool set_appId(int appId);


    private:

      friend class UpdateTurnListMarshaller;

      std::vector< NsAppLinkRPCV2::Turn> turnList;
      std::vector< NsAppLinkRPCV2::SoftButton> softButtons;
      int appId;

    };
  }
}

#endif
