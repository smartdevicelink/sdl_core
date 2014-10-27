/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_PT_EXCHANGE_IMPL_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_PT_EXCHANGE_IMPL_H_

#include "application_manager/policies/pt_exchange_handler.h"
#include "utils/lock.h"
#include "utils/threads/thread.h"

namespace policy {

class PolicyHandler;

class PTExchangeHandlerImpl : public PTExchangeHandler {
 public:
  PTExchangeHandlerImpl(PolicyHandler* handler);
  virtual ~PTExchangeHandlerImpl();
  virtual void Start();
  virtual void Stop();

 protected:
  PolicyHandler* policy_handler_;
  threads::Thread* retry_sequence_;
  sync_primitives::Lock retry_sequence_lock_;

  friend class RetrySequence;
};

}  // namespace policy

#endif  //  SRC_COMPONENTS_POLICY_INCLUDE_POLICY_PT_EXCHANGE_IMPL_H_
