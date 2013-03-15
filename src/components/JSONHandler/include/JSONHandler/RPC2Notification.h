/**
* \file RPC2Notification.hpp
* \brief RPC2Notification class header.
* Copyright (c) 2013 Ford Motor Company
*/


#ifndef RPCS2_NOTIFICATION_CLASS
#define RPCS2_NOTIFICATION_CLASS

#include "RPC2Command.h"

/**
  *\namespace NsRPC2Communication
  *\brief Namespace for communication with HMI.
*/
namespace NsRPC2Communication
{
    /**
     * \class RPC2Notification
     * \brief Base class for notifications generated from Json RPC2
    */
    class RPC2Notification : public RPC2Command
    {
    public:
        /**
         * \brief Default Constructor
        */
        RPC2Notification( );

        /**
         * \brief Constructor
         * \param method Method Id.
        */
        RPC2Notification( int method );

        /**
         * \brief Destructor
        */
        ~RPC2Notification();
    };
}

#endif  //  RPCS2_NOTIFICATION_CLASS

