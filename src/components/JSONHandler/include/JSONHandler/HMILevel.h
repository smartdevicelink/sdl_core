#ifndef HMILEVEL_STRUCT
#define HMILEVEL_STRUCT value

#include <string>

struct HMILevel
{
    enum HMILevelEnum { FULL, LIMITED, BACKGROUND, NONE };

    static HMILevelEnum valueFromString( const std::string & input ) 
    {
      if ( input.compare("FULL") == 0 )
      {
        return FULL;
      }
      if ( input.compare("LIMITED") == 0 )
      {
        return LIMITED;
      }
      if ( input.compare("BACKGROUND") == 0 )
      {
        return BACKGROUND;
      }
      if ( input.compare("NONE") == 0 )
      {
        return NONE;
      }
    }

    std::string toString() const { 
      if (mInternalHMILevel == FULL)
      {
        return "FULL";
      }
      if (mInternalHMILevel == LIMITED)
      {
        return "LIMITED";
      }
      if (mInternalHMILevel == BACKGROUND)
      {
        return "BACKGROUND";
      }
      if (mInternalHMILevel == NONE)
      {
        return "NONE";
      }
    }

    void setHMILevel( std::string & _level ) { mInternalHMILevel = valueFromString(_level); }

private:
    HMILevelEnum mInternalHMILevel;
};

#endif  // HMILEVEL_STRUCT
