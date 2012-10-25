#ifndef PERFORMINTERACTION_RESPONSE_INCLUDE
#define PERFORMINTERACTION_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "TriggerSource.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


class PerformInteraction_response : public ALRPCResponse
{
public:

  PerformInteraction_response(const PerformInteraction_response& c);
  PerformInteraction_response(void);
  
  virtual ~PerformInteraction_response(void);

  PerformInteraction_response& operator =(const PerformInteraction_response&);

  bool checkIntegrity(void);

  bool get_success(void) const;
  const Result& get_resultCode(void) const;
  const std::string* get_info(void) const;
  const unsigned int* get_choiceID(void) const;
  const TriggerSource* get_triggerSource(void) const;

  bool set_success(bool success_);
  bool set_resultCode(const Result& resultCode_);
  void reset_info(void);
  bool set_info(const std::string& info_);
  void reset_choiceID(void);
  bool set_choiceID(unsigned int choiceID_);
  void reset_triggerSource(void);
  bool set_triggerSource(const TriggerSource& triggerSource_);

private:

  friend class PerformInteraction_responseMarshaller;

  bool success;
  Result resultCode;
  std::string* info;	//!< (1000)
  unsigned int* choiceID;	//!<  (0,2000000000)
  TriggerSource* triggerSource;
};

#endif
