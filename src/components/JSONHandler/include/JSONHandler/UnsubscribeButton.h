#ifndef UNSUBSCRIBEBUTTON_H
#define UNSUBSCRIBEBUTTON_H

#include "JSONHandler/MobileRPCRequest.h"
#include <vector>
#include <string>

class UnsubscribeButton : public MobileRPCRequest
{
public:
  UnsubscribeButton( unsigned int protocolVersion ):MobileRPCRequest( protocolVersion, "UnsubscribeButton" ){}
  ~UnsubscribeButton() {}

  void setButtonName ( std::string buttonName ) { mButtonName = buttonName; }

  std::string getButtonName() const { return mButtonName; }

  /* data */
private:
  std::string mButtonName;

};

#endif // UNSUBSCRIBEBUTTON_H
