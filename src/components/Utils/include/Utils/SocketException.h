/**
 * \file SocketException.h
 * \brief Definition of the SocketException class
 */

#ifndef SocketException_class
#define SocketException_class

#include <string>

namespace log4cplus
{
    class Logger;
}

/**
 * \brief SocketException class acts as a socket exception type
 */
class SocketException
{
public:
    /**
     * \brief Class constructor
     * \param s exception string
     */
    SocketException ( std::string s );

    /**
     * \brief Default class destructor
     */
    ~SocketException ();

    /**
     * \brief description of exception
     * \return description
     */
    const std::string& description();

private:

    std::string m_s;
    static log4cplus::Logger mLogger;
};

#endif
