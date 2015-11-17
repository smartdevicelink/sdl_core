<<<<<<< HEAD:test/components/transport_manager/src/raw_message_matcher.cc
/*
 * \file matchers.cc
 * \brief customers matchers for gmock
 *
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "transport_manager/raw_message_matcher.h"

namespace test {
namespace components {
namespace transport_manager {

RawMessageMatcher::RawMessageMatcher(RawMessagePtr ptr)
      : ptr_(ptr) {}

bool RawMessageMatcher::MatchAndExplain(const RawMessagePtr msg,
                                             MatchResultListener* listener) const {
  if (msg->data_size() != ptr_->data_size()) {
    return ::std::equal(msg->data(), msg->data() + msg->data_size(), ptr_->data());
  } else
    return false;
}

void RawMessageMatcher::DescribeTo(::std::ostream* os) const {
  *os << "data_ is " ;
  ::std::ostream_iterator<unsigned char> out(*os);
  ::std::copy(ptr_->data(), ptr_->data() + ptr_->data_size(), out);
}

void RawMessageMatcher::DescribeNegationTo(::std::ostream* os) const {
  *os << "data_ is not " ;
  ::std::ostream_iterator<unsigned char> out(*os);
  ::std::copy(ptr_->data(), ptr_->data() + ptr_->data_size(), out);
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test
=======
/*
 * Copyright (c) 2014, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "transport_manager/pasa_bt/bluetooth_PASA_transport_adapter.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iomanip>
#include <set>

#include "transport_manager/pasa_bt/bluetooth_PASA_listener.h"
#include "transport_manager/pasa_bt/bluetooth_PASA_connection_factory.h"
#include "transport_manager/pasa_bt/bluetooth_PASA_device.h"
#include "resumption/last_state.h"
#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

BluetoothPASATransportAdapter::BluetoothPASATransportAdapter()
      : TransportAdapterImpl(0, new BluetoothPASAConnectionFactory(this),
                             new BluetoothPASAListener(this)) {
}

DeviceType BluetoothPASATransportAdapter::GetDeviceType() const {
  return PASA_BLUETOOTH;
}

}  // namespace transport_adapter
}  // namespace transport_manager

>>>>>>> 35db1ec... APPLINK-12266. Implement ability to handle transport type.:src/components/transport_manager/src/pasa_bt/bluetooth_PASA_transport_adapter.cc
