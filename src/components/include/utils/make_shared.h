/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_MAKE_SHARED_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_MAKE_SHARED_H_

#include <new>
/**
 * @brief The functions set below allows to create shared pointer in a safe way.
 * There are up to 5 parameters could be passed as the constructor parameters.
 *
 * @example
 * struct A {
 *   A(int){}
 *   A(int, double) {}
 *   A(int, double, std::string) {}
 * }
 * SharedPtr<A> shared1(MakeShared<A>(5);
 * SharedPtr<A> shared2(MakeShared<A>(5, 5.5);
 * SharedPtr<A> shared3(MakeShared<A>(5, 5.5, std::string("MyStr"));
 *
 * The profit in using MakeShared instead of simple allocation with operator new
 *is evident.
 * Firstly it allows us to centralize allocation place, secondly it allows us to
 *use
 * safe operator new overloading (no throwable one).
 */
namespace utils {
template <typename T>
class SharedPtr;

namespace {
template <typename T>
SharedPtr<T> Initialize(T* object) {
  return object == NULL ? SharedPtr<T>() : SharedPtr<T>(object);
}
}

template <typename T>
SharedPtr<T> MakeShared() {
  T* t = new (std::nothrow) T;
  return Initialize(t);
}

template <typename T, typename Arg1>
SharedPtr<T> MakeShared(Arg1& arg1) {
  T* t = new (std::nothrow) T(arg1);
  return Initialize(t);
}

template <typename T, typename Arg1, typename Arg2>
SharedPtr<T> MakeShared(Arg1& arg1, Arg2& arg2) {
  T* t = new (std::nothrow) T(arg1, arg2);
  return Initialize(t);
}

template <typename T, typename Arg1, typename Arg2, typename Arg3>
SharedPtr<T> MakeShared(Arg1& arg1, Arg2& arg2, Arg3& arg3) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3);
  return Initialize(t);
}

template <typename T,
          typename Arg1,
          typename Arg2,
          typename Arg3,
          typename Arg4>
SharedPtr<T> MakeShared(Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3, arg4);
  return Initialize(t);
}

template <typename T,
          typename Arg1,
          typename Arg2,
          typename Arg3,
          typename Arg4,
          typename Arg5>
SharedPtr<T> MakeShared(
    Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3, arg4, arg5);
  return Initialize(t);
}

template <typename T,
          typename Arg1,
          typename Arg2,
          typename Arg3,
          typename Arg4,
          typename Arg5,
          typename Arg6>
SharedPtr<T> MakeShared(
    Arg1& arg1, Arg2& arg2, Arg3& arg3, Arg4& arg4, Arg5& arg5, Arg6& arg6) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3, arg4, arg5, arg6);
  return Initialize(t);
}

template <typename T,
          typename Arg1,
          typename Arg2,
          typename Arg3,
          typename Arg4,
          typename Arg5,
          typename Arg6,
          typename Arg7>
SharedPtr<T> MakeShared(Arg1& arg1,
                        Arg2& arg2,
                        Arg3& arg3,
                        Arg4& arg4,
                        Arg5& arg5,
                        Arg6& arg6,
                        Arg7& arg7) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
  return Initialize(t);
}

template <typename T, typename Arg1>
SharedPtr<T> MakeShared(const Arg1& arg1) {
  T* t = new (std::nothrow) T(arg1);
  return Initialize(t);
}

template <typename T, typename Arg1, typename Arg2>
SharedPtr<T> MakeShared(const Arg1& arg1, const Arg2& arg2) {
  T* t = new (std::nothrow) T(arg1, arg2);
  return Initialize(t);
}

template <typename T, typename Arg1, typename Arg2, typename Arg3>
SharedPtr<T> MakeShared(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3);
  return Initialize(t);
}

template <typename T,
          typename Arg1,
          typename Arg2,
          typename Arg3,
          typename Arg4>
SharedPtr<T> MakeShared(const Arg1& arg1,
                        const Arg2& arg2,
                        const Arg3& arg3,
                        const Arg4& arg4) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3, arg4);
  return Initialize(t);
}

template <typename T,
          typename Arg1,
          typename Arg2,
          typename Arg3,
          typename Arg4,
          typename Arg5>
SharedPtr<T> MakeShared(const Arg1& arg1,
                        const Arg2& arg2,
                        const Arg3& arg3,
                        const Arg4& arg4,
                        const Arg5& arg5) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3, arg4, arg5);
  return Initialize(t);
}

template <typename T,
          typename Arg1,
          typename Arg2,
          typename Arg3,
          typename Arg4,
          typename Arg5,
          typename Arg6>
SharedPtr<T> MakeShared(const Arg1& arg1,
                        const Arg2& arg2,
                        const Arg3& arg3,
                        const Arg4& arg4,
                        const Arg5& arg5,
                        const Arg6& arg6) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3, arg4, arg5, arg6);
  return Initialize(t);
}

template <typename T,
          typename Arg1,
          typename Arg2,
          typename Arg3,
          typename Arg4,
          typename Arg5,
          typename Arg6,
          typename Arg7>
SharedPtr<T> MakeShared(const Arg1& arg1,
                        const Arg2& arg2,
                        const Arg3& arg3,
                        const Arg4& arg4,
                        const Arg5& arg5,
                        const Arg6& arg6,
                        const Arg7& arg7) {
  T* t = new (std::nothrow) T(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
  return Initialize(t);
}

}  // namespace utils;
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_MAKE_SHARED_H_
