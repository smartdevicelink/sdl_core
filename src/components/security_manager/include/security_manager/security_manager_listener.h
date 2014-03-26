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
  virtual void OnHandshakeDone(bool success)=0;
  virtual void OnHandshakeFailed()=0;
};

}



#endif /* SECURITY_MANAGER_LISTENER_H_ */
