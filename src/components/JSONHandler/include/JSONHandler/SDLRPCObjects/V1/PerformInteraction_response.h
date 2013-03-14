#ifndef NSAPPLINKRPC_PERFORMINTERACTION_RESPONSE_INCLUDE
#define NSAPPLINKRPC_PERFORMINTERACTION_RESPONSE_INCLUDE

#include "TriggerSource.h"
#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    robok0der
*/

namespace NsSmartDeviceLinkRPC
{
    class PerformInteraction_response : public SDLRPCResponse
    {
    public:
        PerformInteraction_response(const PerformInteraction_response& c);
        PerformInteraction_response(void);

        virtual ~PerformInteraction_response(void);

        PerformInteraction_response& operator =(const PerformInteraction_response&);

        bool checkIntegrity(void);

        const unsigned int* get_choiceID(void) const;
        const TriggerSource* get_triggerSource(void) const;

        void reset_choiceID(void);
        bool set_choiceID(unsigned int choiceID_);
        void reset_triggerSource(void);
        bool set_triggerSource(const TriggerSource& triggerSource_);

    private:
        friend class PerformInteraction_responseMarshaller;

        ///  ID of the choice that was selected in response to PerformInteraction.
        unsigned int* choiceID;   //!<  (0,2000000000)

        ///  See TriggerSource
        TriggerSource* triggerSource;
    };
}

#endif
