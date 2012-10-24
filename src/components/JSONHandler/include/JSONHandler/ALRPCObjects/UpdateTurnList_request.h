#ifndef UPDATETURNLIST_REQUEST_INCLUDE
#define UPDATETURNLIST_REQUEST_INCLUDE

#include <vector>

#include "SoftButton.h"
#include "Turn.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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
  std::vector<SoftButton> softButtons;	//!<   [%s..%s] 
};

#endif
