#ifndef NSAPPLINKRPCV2_DELETEFILE_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_DELETEFILE_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

/**
     Response is sent, when the file data was deleted (success case). Or when an error occured.
     Not supported on First generation SYNC vehicles. 
*/

  class DeleteFile_response : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    DeleteFile_response(const DeleteFile_response& c);
    DeleteFile_response(void);
    
    virtual ~DeleteFile_response(void);
  
    DeleteFile_response& operator =(const DeleteFile_response&);
  
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
  
    friend class DeleteFile_responseMarshaller;


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
