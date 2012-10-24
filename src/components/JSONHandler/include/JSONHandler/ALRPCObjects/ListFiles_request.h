#ifndef LISTFILES_REQUEST_INCLUDE
#define LISTFILES_REQUEST_INCLUDE


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
     Requests the current list of resident filenames for the registered app
     Not supported on first generation SYNC vehicles. 
*/

class ListFiles_request : public ALRPCRequest
{
public:

  ListFiles_request(const ListFiles_request& c);
  ListFiles_request(void);
  
  virtual ~ListFiles_request(void);

  bool checkIntegrity(void);



private:

  friend class ListFiles_requestMarshaller;

};

#endif
