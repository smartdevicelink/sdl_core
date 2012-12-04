#ifndef NSAPPLINKRPCV2_UPDATETURNLIST_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_UPDATETURNLIST_REQUEST_INCLUDE

#include <vector>

#include "SoftButton.h"
#include "Turn.h"
#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class UpdateTurnList_request : public ALRPC2Message
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
