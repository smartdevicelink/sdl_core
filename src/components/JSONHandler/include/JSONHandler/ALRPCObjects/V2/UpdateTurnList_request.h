#ifndef NSAPPLINKRPCV2_UPDATETURNLIST_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_UPDATETURNLIST_REQUEST_INCLUDE

#include <vector>

#include "SoftButton.h"
#include "Turn.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class UpdateTurnList_request : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    UpdateTurnList_request(const UpdateTurnList_request& c);
    UpdateTurnList_request(void);
    
    virtual ~UpdateTurnList_request(void);
  
    bool checkIntegrity(void);

    const std::vector<Turn>& get_turnList(void) const;
    const std::vector<SoftButton>& get_softButtons(void) const;

    bool set_turnList(const std::vector<Turn>& turnList_);
    bool set_softButtons(const std::vector<SoftButton>& softButtons_);

  private:
  
    friend class UpdateTurnList_requestMarshaller;

      std::vector<Turn> turnList;	//!<   [%s..%s] 

///  If omitted on supported displays, app-defined SoftButton will be left blank.
      std::vector<SoftButton> softButtons;	//!<   [%s..%s] 
  };

}

#endif
