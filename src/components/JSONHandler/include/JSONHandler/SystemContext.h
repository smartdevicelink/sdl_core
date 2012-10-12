#ifndef SYSTEMCONTEXT_STRUCT
#define SYSTEMCONTEXT_STRUCT value

#include <string>

struct SystemContext
{
    enum SystemContextEnum { MAIN, VRSESSION, MENU };

    static SystemContextEnum valueFromString( const std::string & input ) 
    {
      if ( input.compare("MAIN") == 0 )
      {
        return MAIN;
      }
      if ( input.compare("VRSESSION") == 0 )
      {
        return VRSESSION;
      }
      if ( input.compare("MENU") == 0 )
      {
        return MENU;
      }
    }

    std::string toString() const { 
      if (mInternalSystemContext == MAIN)
      {
        return "MAIN";
      }
      if (mInternalSystemContext == VRSESSION)
      {
        return "VRSESSION";
      }
      if (mInternalSystemContext == MENU)
      {
        return "MENU";
      }
    }

    void setSystemContext( std::string & _level ) { mInternalSystemContext = valueFromString(_level); }

private:
    SystemContextEnum mInternalSystemContext;
};

#endif  // SYSTEMCONTEXT_STRUCT
