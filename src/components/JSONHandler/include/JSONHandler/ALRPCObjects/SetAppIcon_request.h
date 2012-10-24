#ifndef SETAPPICON_REQUEST_INCLUDE
#define SETAPPICON_REQUEST_INCLUDE

#include <string>

#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


/**
     Used to set existing local file on SYNC as the app's icon
     Not supported on first generation SYNC vehicles. 
*/

class SetAppIcon_request : public ALRPCRequest
{
public:

  SetAppIcon_request(const SetAppIcon_request& c);
  SetAppIcon_request(void);
  
  virtual ~SetAppIcon_request(void);

  SetAppIcon_request& operator =(const SetAppIcon_request&);

  bool checkIntegrity(void);

  const std::string* get_syncFileName(void) const;

  void reset_syncFileName(void);
  bool set_syncFileName(const std::string& syncFileName_);

private:

  friend class SetAppIcon_requestMarshaller;

  std::string* syncFileName;	//!< (500)
};

#endif
