#ifndef PROTOCOL_OBSERVER_HPP
#define PROTOCOL_OBSERVER_HPP

#include "utils/misc/Types.hpp"

class IProtocolObserver
{
public:
   /**
    * Callback informs client about session start if the session started.
    */
   virtual void sessionStartedCallback(const UInt8 sessionID) = 0;

   /**
    * Callback informs client about session end if the session ended.
    */
   virtual void sessionEndedCallback(const UInt8 sessionID) = 0;

   /**
    * Callback for data receiving. Informs client about readable data and buffer size.
    * Client should implement this callback
    * @param sessionID session with incoming data number
    * @param dataSize data size available for read
    */
   virtual void dataReceivedCallback(const UInt8 sessionID, const UInt32 messageID, const UInt32 dataSize) = 0;

protected:

   virtual ~IProtocolObserver() { }
};

#endif
