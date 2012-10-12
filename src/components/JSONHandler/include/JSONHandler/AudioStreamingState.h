#ifndef AUDIOSTREAMINGSTATE_STRUCT
#define AUDIOSTREAMINGSTATE_STRUCT value

#include <string>

struct AudioStreamingState
{
    enum AudioStreamingStateEnum { AUDIBLE, NOT_AUDIBLE };

    static AudioStreamingStateEnum valueFromString( const std::string & input ) 
    {
      if ( input.compare("AUDIBLE") == 0 )
      {
        return AUDIBLE;
      }
      if ( input.compare("NOT_AUDIBLE") == 0 )
      {
        return NOT_AUDIBLE;
      }
    }

    std::string toString() const { 
      if (mInternalAudioStreamingState == AUDIBLE)
      {
        return "AUDIBLE";
      }
      if (mInternalAudioStreamingState == NOT_AUDIBLE)
      {
        return "NOT_AUDIBLE";
      }
    }

    void setAudioStreamingState( std::string & _level ) { mInternalAudioStreamingState = valueFromString(_level); }

private:
    AudioStreamingStateEnum mInternalAudioStreamingState;
};

#endif  // AUDIOSTREAMINGSTATE_STRUCT
