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

#ifndef RPC_BASE_GTEST_SUPPORT_H_
#define RPC_BASE_GTEST_SUPPORT_H_

#include <gtest/gtest.h>

#include "rpc_base/validation_report.h"

// A predicate-formatter for asserting that intergen generated
// object is valid
template<typename T>
::testing::AssertionResult AssertRpcObjValid(const char* obj_expr,
                                               const T& obj) {
  if (obj.is_valid())
    return ::testing::AssertionSuccess();

  rpc::ValidationReport report(obj_expr);
  obj.ReportErrors(&report);

  return ::testing::AssertionFailure()
      << obj_expr << " failed validation. Violations are:\n"
      << rpc::PrettyFormat(report);
}

#define ASSERT_RPCTYPE_VALID(object) \
  ASSERT_PRED_FORMAT1(AssertRpcObjValid, object)

#define EXPECT_RPCTYPE_VALID(object) \
  EXPECT_PRED_FORMAT1(AssertRpcObjValid, object)

#endif /* RPC_BASE_GTEST_SUPPORT_H_ */
