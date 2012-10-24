#ifndef SUBSCRIBEBUTTON_H
#define SUBSCRIBEBUTTON_H value

#include "JSONHandler/MobileRPCRequest.h"
#include <vector>
#include <string>

class SubscribeButton : public MobileRPCRequest
{
public:
  SubscribeButton( unsigned int protocolVersion ):MobileRPCRequest( protocolVersion, "SubscribeButton" ){}
  ~SubscribeButton() {}

  void setButtonName ( std::string buttonName ) { mButtonName = buttonName; }

  std::string getButtonName() const { return mButtonName; }

  /* data */
private:
  std::string mButtonName;

};

#endif // SUBSCRIBEBUTTON_H
