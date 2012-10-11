#ifndef REGISTERAPPINTERFACERESPONSE_CLASS
#define REGISTERAPPINTERFACERESPONSE_CLASS value

#include "JSONHandler/MobileRPCResponse.h"
#include "JSONHandler/Result.h"
#include <string>

class RegisterAppInterfaceResponse : public MobileRPCResponse
{
public:
  RegisterAppInterfaceResponse( unsigned int protocolVersion ):MobileRPCResponse( protocolVersion, "RegisterAppInterface" ){}
  ~RegisterAppInterfaceResponse() {}

  void setSuccess( bool success ) { mSuccess = success; }
  bool getSuccess() const { return mSuccess; }
  void setResultCode( std::string result ) { mResultCode.setResult( result ); }
  std::string getResultString() const { return mResultCode.toString(); }

  /* data */
private:
  bool mSuccess;
  Result mResultCode;
  
};

#endif
