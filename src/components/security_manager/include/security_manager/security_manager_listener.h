/*
 * security_manager_listener.h
 *
 *  Created on: 23 марта 2014 г.
 *      Author: arv
 */

#ifndef SECURITY_MANAGER_LISTENER_H_
#define SECURITY_MANAGER_LISTENER_H_

namespace security_manager {

class SecurityManagerListener {
 public:
  /**
   * \brief Notification about protection result
   * \param connection_key Unique key used by other components as session identifier
   * \param succecc result of connection protection
   * \return \c true on success notification or \c false otherwise
   */
  virtual bool OnHandshakeDone(const uint32_t &connection_key,
                               bool success)=0;
  virtual bool OnHandshakeFailed(const uint32_t &connection_key)=0;
};

}



#endif /* SECURITY_MANAGER_LISTENER_H_ */
