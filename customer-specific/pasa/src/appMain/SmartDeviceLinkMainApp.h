/*
 * SmartDeviceLinkMainApp.hpp
 *
 *  Created on: Jun 25, 2013
 *      Author: adderleym
 */

#ifndef SMARTDEVICELINKMAINAPP_HPP_
#define SMARTDEVICELINKMAINAPP_HPP_

#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <mqueue.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>  // cpplint: Streams are highly discouraged.
#include <fstream>   // cpplint: Streams are highly discouraged.

#include <applink_types.h>

#define MAX_QUEUE_NAME_SIZE     24
#define MAX_QUEUE_MSG_SIZE 		4095
#define MSGQ_MAX_MESSAGES		128
#define MSGQ_MESSAGE_SIZE 		MAX_QUEUE_MSG_SIZE

/**
 * \namespace NsSmartDeviceLink
 * \brief SmartDeviceLink main Application related functions.
 */
namespace NsSmartDeviceLink {

}  // namespace NsSmartDeviceLink


#endif /* SMARTDEVICELINKMAINAPP_HPP_ */
