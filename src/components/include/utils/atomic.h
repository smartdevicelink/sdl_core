/*
 * Copyright (c) 2013-2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_ATOMIC_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_ATOMIC_H_

#if defined(__QNXNTO__)
#include <atomic.h>
#elif defined(OS_WINDOWS)
#include "utils/winhdr.h"
#endif

#if defined(__QNXNTO__)
#define atomic_post_inc(ptr) atomic_add_value((ptr), 1)
#elif defined(__GNUG__)
#define atomic_post_inc(ptr) __sync_fetch_and_add((ptr), 1)
#elif defined(OS_WINDOWS)
#define atomic_post_inc(ptr) (InterlockedIncrement((ptr)) - 1)
#else
#warning "atomic_post_inc() implementation is not atomic"
#define atomic_post_inc(ptr) (*(ptr))++
#endif

#if defined(__QNXNTO__)
#define atomic_post_dec(ptr) atomic_sub_value((ptr), 1)
#elif defined(__GNUG__)
#define atomic_post_dec(ptr) __sync_fetch_and_sub((ptr), 1)
#elif defined(OS_WINDOWS)
#define atomic_post_dec(ptr) (InterlockedDecrement((ptr)) + 1)
#else
#warning "atomic_post_dec() implementation is not atomic"
#define atomic_post_dec(ptr) (*(ptr))--
#endif

// pointer assignment is believed to be atomic
#define atomic_pointer_assign(dst, src) (dst) = (src)

#if defined(__QNXNTO__)
#define atomic_post_set(dst) atomic_set_value(dst, 1)
#elif defined(__GNUG__)
#define atomic_post_set(dst) __sync_val_compare_and_swap((dst), 0, 1)
#elif defined(OS_WINDOWS)
#define atomic_post_set(dst) InterlockedCompareExchange((dst), 1, 0)
#else
#warning "atomic_post_set() implementation is not atomic"
#define atomic_post_set(dst) (dst) = (dst) == 0 ? 1 : (dst)
#endif

#if defined(__QNXNTO__)
#define atomic_post_clr(dst) atomic_clr_value(dst, 1)
#elif defined(__GNUG__)
#define atomic_post_clr(dst) __sync_val_compare_and_swap((dst), 1, 0)
#elif defined(OS_WINDOWS)
#define atomic_post_clr(dst) InterlockedCompareExchange((dst), 0, 1)
#else
#warning "atomic_post_clr() implementation is not atomic"
#define atomic_post_clr(dst) (dst) = (dst) == 1 ? 0 : (dst)
#endif

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_ATOMIC_H_
