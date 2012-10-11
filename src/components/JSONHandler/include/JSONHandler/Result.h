#ifndef RESULT_STRUCT
#define RESULT_STRUCT value

#include <string>

struct Result
{
    enum ResultEnum { SUCCESS, INVALID_DATA, UNSUPPORTED_REQUEST, OUT_OF_MEMORY, 
      TOO_MANY_PENDING_REQUESTS, INVALID_ID, DUPLICATE_NAME, 
      TOO_MANY_APPLICATIONS, APPLICATION_REGISTERED_ALREADY,
      UNSUPPORTED_VERSION, WRONG_LANGUAGE, APPLICATION_NOT_REGISTERED,
      IN_USE, VEHICLE_DATA_NOT_ALLOWED, VEHICLE_DATA_NOT_AVAILABLE,
      REJECTED, ABORTED, IGNORED, UNSUPPORTED_BUTTON, FILE_NOT_FOUND, GENERIC_ERROR };

    static ResultEnum valueFromString( const std::string & input ) 
    {
      if ( input.compare("SUCCESS") == 0 )
      {
        return SUCCESS;
      }
      if ( input.compare("INVALID_DATA") == 0 )
      {
        return INVALID_DATA;
      }
      if ( input.compare("UNSUPPORTED_REQUEST") == 0 )
      {
        return UNSUPPORTED_REQUEST;
      }
      if ( input.compare("OUT_OF_MEMORY") == 0 )
      {
        return OUT_OF_MEMORY;
      }
      if ( input.compare("TOO_MANY_PENDING_REQUESTS") == 0 )
      {
        return TOO_MANY_PENDING_REQUESTS;
      }
      if ( input.compare("INVALID_ID") == 0 )
      {
        return INVALID_ID;
      }
      if ( input.compare("DUPLICATE_NAME") == 0 )
      {
        return DUPLICATE_NAME;
      }
      if ( input.compare("TOO_MANY_APPLICATIONS") == 0 )
      {
        return TOO_MANY_APPLICATIONS;
      }
      if ( input.compare("APPLICATION_REGISTERED_ALREADY") == 0 )
      {
        return APPLICATION_REGISTERED_ALREADY;
      }
      if ( input.compare("UNSUPPORTED_VERSION") == 0 )
      {
        return UNSUPPORTED_VERSION;
      }
      if ( input.compare("WRONG_LANGUAGE") == 0 )
      {
        return WRONG_LANGUAGE;
      }
      if ( input.compare("APPLICATION_NOT_REGISTERED") == 0 )
      {
        return APPLICATION_NOT_REGISTERED;
      }
      if ( input.compare("IN_USE") == 0 )
      {
        return IN_USE;
      }
      if ( input.compare("VEHICLE_DATA_NOT_ALLOWED") == 0 )
      {
        return VEHICLE_DATA_NOT_ALLOWED;
      }
      if ( input.compare("VEHICLE_DATA_NOT_AVAILABLE") == 0 )
      {
        return VEHICLE_DATA_NOT_AVAILABLE;
      }
      if ( input.compare("APPLICATION_NOT_REGISTERED") == 0 )
      {
        return APPLICATION_NOT_REGISTERED;
      }
      if ( input.compare("REJECTED") == 0 )
      {
        return REJECTED;
      }
      if ( input.compare("ABORTED") == 0 )
      {
        return ABORTED;
      }
      if ( input.compare("IGNORED") == 0 )
      {
        return IGNORED;
      }
      if ( input.compare("UNSUPPORTED_BUTTON") == 0 )
      {
        return UNSUPPORTED_BUTTON;
      }
      if ( input.compare("FILE_NOT_FOUND") == 0 )
      {
        return FILE_NOT_FOUND;
      }
      if ( input.compare("GENERIC_ERROR") == 0 )
      {
        return GENERIC_ERROR;
      }
    }

    std::string toString() const { return mInternalResult; }
    void setResult( std::string & result ) { mInternalResult = result; }

private:
    std::string mInternalResult;
};

#endif
