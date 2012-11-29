#ifndef NSAPPLINKRPC_UPDATETURNLIST_REQUEST_INCLUDE
#define NSAPPLINKRPC_UPDATETURNLIST_REQUEST_INCLUDE

#include <vector>

#include "SoftButton.h"
#include "Turn.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class UpdateTurnList_request : public ALRPCRequest
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
