#ifndef NSAPPLINKRPCV2_PUTFILE_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_PUTFILE_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
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

/**
     Response is sent, when the file data was copied (success case). Or when an error occured.
     Not supported on First generation SYNC vehicles. 
*/

  class PutFile_response : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    PutFile_response(const PutFile_response& c);
    PutFile_response(void);
    
    virtual ~PutFile_response(void);
  
    PutFile_response& operator =(const PutFile_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    unsigned int get_spaceAvailable(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    bool set_spaceAvailable(unsigned int spaceAvailable_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class PutFile_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;

///  Provides the total local space available on SYNC for the registered app.
      unsigned int spaceAvailable;	//!<  (0,2000000000)

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)
  };

}

#endif
