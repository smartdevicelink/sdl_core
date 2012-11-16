#ifndef ONDRIVERDISTRACTION_H
#define ONDRIVERDISTRACTION_H

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/DriverDistractionState.h"

namespace RPC2Communication
{
  namespace UI
  {
    class OnDriverDistraction : public RPC2Communication::RPC2Notification
    {
    public:
        OnDriverDistraction();
        virtual ~OnDriverDistraction();
        OnDriverDistraction(const OnDriverDistraction& c);

        OnDriverDistraction& operator =(const OnDriverDistraction&);

        bool checkIntegrity(void);

  // getters
        const ::AppLinkRPC::DriverDistractionState& get_state(void);


  // setters
        bool set_state(const ::AppLinkRPC::DriverDistractionState& state);

    private:
        friend class OnDriverDistractionMarshaller;

        ::AppLinkRPC::DriverDistractionState state;
    };
  }
}

#endif // ONDRIVERDISTRACTION_H
