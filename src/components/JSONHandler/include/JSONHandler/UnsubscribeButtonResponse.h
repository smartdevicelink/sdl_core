#ifndef UNSUBSCRIBEBUTTONRESPONSE_H
#define UNSUBSCRIBEBUTTONRESPONSE_H

#include "JSONHandler/MobileRPCResponse.h"
#include "JSONHandler/Result.h"
#include <string>

class UnsubscribeButtonResponse : public MobileRPCResponse
{
public:
  UnsubscribeButtonResponse( unsigned int protocolVersion ):MobileRPCResponse( protocolVersion, "UnsubscribeButton" ){}
  ~UnsubscribeButtonResponse() {}

  void setSuccess( bool success ) { mSuccess = success; }
  bool getSuccess() const { return mSuccess; }
  void setResultCode( std::string result ) { mResultCode.setResult( result ); }
  std::string getResultString() const { return mResultCode.toString(); }

  /* data */
private:
  bool mSuccess;
  Result mResultCode;

};

#endif // UNSUBSCRIBEBUTTONRESPONSE_H
