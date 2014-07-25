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

#include "security_manager/security_query_matcher.h"

::testing::AssertionResult QueryHeader_EQ(
    const char* m_expr, const char* n_expr,
    const ::security_manager::SecurityQuery::QueryHeader& q1,
    const ::security_manager::SecurityQuery::QueryHeader& q2 ) {
  ::testing::AssertionResult fail_result = ::testing::AssertionFailure();
  fail_result  << "(\"" << m_expr << " and \"" << n_expr << "\") are not equal "
               << " : different ";
  if (q1.json_size  != q2.json_size)
    return fail_result << "json_size_1=" << q1.json_size
                       << ", json_size_2=" << q2.json_size;
  if (q1.query_id   != q2.query_id)
    return fail_result << "query_id_1=" << q1.query_id
                       << ", query_id_2=" << q2.query_id;
  if (q1.query_type != q2.query_type)
    return fail_result << "query_type_1=" << q1.query_type
                       << ", query_type_2=" << q2.query_type;
  if (q1.seq_number != q2.seq_number)
    return fail_result  << "seq_number_1=" << q1.seq_number
                        << ", seq_number_2=" << q2.seq_number;
  return ::testing::AssertionSuccess();
}
