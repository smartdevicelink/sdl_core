/**
 * \file CSender.hpp
 * \brief CSender class header.
 * \author AKara
 */

#ifndef CSENDER_H
#define CSENDER_H

#if _MSC_VER >= 1400 // VC++ 8.0
typedef int ssize_t;
#endif

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker 
{
   /**
   * \class CSender
   * \brief CSender interface class implementation.
   */
   class CSender
   {
   public:
      /**
      * \brief Constructor.
      */
      CSender()
      {
      }

      /**
      * \brief Destructor.
      */
      ~CSender()
      {
      }

      /**
      * \brief Send data.
      * \param fd file descriptor of the client TCP socket
      * \param data data to send
      * \return number of bytes sent or -1 if error
      */
      virtual ssize_t Send(int fd, const std::string& data)=0;
   };
} /* namespace NsMessageBroker */

#endif // CSENDER_H


