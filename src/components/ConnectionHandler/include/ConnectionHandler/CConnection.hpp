/**
 * \file CConnection.hpp
 * \brief Connection class.
 * Stores connetion information
 * \author AKara
 */

#ifndef CONNECTION_H 
#define CONNECTION_H 

#include "Logger.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{
    /**
    * \class CConnection
    * \brief Connection class
    */
    class CConnection
    {
    public:
        /**
         * \brief Class constructor
         */
        CConnection();

        /**
         * \brief Destructor
         */
        ~CConnection();


    private:
        /**
          *\brief For logging.
        */
        static log4cplus::Logger mLogger;
    };
}/* namespace NsConnectionHandler */

#endif /* CONNECTION_H */