#ifndef LISTFILES_RESPONSE_INCLUDE
#define LISTFILES_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "Result.h"
#include "../../../JSONHandler/ALRPCResponse.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


/**
     Returns the current list of resident filenames for the registered app along with the current space available
     Not supported on First generation SYNC vehicles. 
*/

class ListFiles_response : public ALRPCResponse
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

  bool success;
  Result resultCode;
  std::vector<std::string>* filenames;	//!<   [%s..%s] (500)
  unsigned int spaceAvailable;	//!<  (0,2000000000)
  std::string* info;	//!< (1000)
};

#endif
