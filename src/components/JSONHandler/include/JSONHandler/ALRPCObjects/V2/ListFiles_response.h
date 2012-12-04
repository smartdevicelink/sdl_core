#ifndef NSAPPLINKRPCV2_LISTFILES_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_LISTFILES_RESPONSE_INCLUDE

#include <vector>
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
     Returns the current list of resident filenames for the registered app along with the current space available
     Not supported on First generation SYNC vehicles. 
*/

  class ListFiles_response : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    ListFiles_response(const ListFiles_response& c);
    ListFiles_response(void);
    
    virtual ~ListFiles_response(void);
  
    ListFiles_response& operator =(const ListFiles_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::vector<std::string>* get_filenames(void) const;
    unsigned int get_spaceAvailable(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_filenames(void);
    bool set_filenames(const std::vector<std::string>& filenames_);
    bool set_spaceAvailable(unsigned int spaceAvailable_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class ListFiles_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;

/**
     An array of all filenames resident on SYNC for the given registered app.
     If omitted, then no files currently reside on the system.
*/
      std::vector<std::string>* filenames;	//!<   [%s..%s] (500)

///  Provides the total local space available on SYNC for the registered app.
      unsigned int spaceAvailable;	//!<  (0,2000000000)

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)
  };

}

#endif
