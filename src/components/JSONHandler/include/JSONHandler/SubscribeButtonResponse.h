#ifndef SUBSCRIBEBUTTONRESPONSE_H
#define SUBSCRIBEBUTTONRESPONSE_H

#include "JSONHandler/MobileRPCResponse.h"
#include "JSONHandler/Result.h"
#include <string>

class SubscribeButtonResponse : public MobileRPCResponse
{
public:
  SubscribeButtonResponse( unsigned int protocolVersion ):MobileRPCResponse( protocolVersion, "SubscribeButton" ){}
  ~SubscribeButtonResponse() {}

  void setSuccess( bool success ) { mSuccess = success; }
  bool getSuccess() const { return mSuccess; }
  void setResultCode( std::string result ) { mResultCode.setResult( result ); }
  std::string getResultString() const { return mResultCode.toString(); }

  /* data */
private:
  bool mSuccess;
  Result mResultCode;

};

#endif // SUBSCRIBEBUTTONRESPONSE_H
