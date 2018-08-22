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
#ifndef SRC_COMPONENTS_INCLUDE_UTILS_MACRO_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_MACRO_H_

#ifdef DEBUG
#include <assert.h>
#else  // RELEASE
#include <stdio.h>
#endif
#include "logger.h"

// A macro to set some action for variable to avoid "unused variable" warning
#define UNUSED(x) (void) x;
// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// A macro to allow utils::Singleton call derivative constructor and destructor
#define FRIEND_BASE_SINGLETON_CLASS(TypeName) \
  friend class utils::Singleton<TypeName>

#define FRIEND_BASE_SINGLETON_CLASS_WITH_DELETER(TypeName, TypeDeleter) \
  friend class utils::Singleton<TypeName, TypeDeleter>

// A macro to allow utils::deleters::Deleter::~Deleter() call class destructor
#define FRIEND_DELETER_DESTRUCTOR(TypeName) \
  friend utils::deleters::Deleter<TypeName>::~Deleter()

#ifdef DEBUG
#define ASSERT(condition) \
  FLUSH_LOGGER();         \
  do {                    \
    DEINIT_LOGGER();      \
    assert(condition);    \
  } while (false)
#else  // RELEASE
#define ASSERT(condition)                                        \
  fprintf(stderr,                                                \
          "Failed condition \"" #condition "\" [%s:%d][%s]\n\n", \
          __FILE__,                                              \
          __LINE__,                                              \
          __FUNCTION__)
#endif

#define DCHECK(condition)                                                     \
  if (!(condition)) {                                                         \
    CREATE_LOGGERPTR_LOCAL(logger_, "Utils");                                 \
    LOG4CXX_FATAL(logger_,                                                    \
                  "DCHECK failed with \"" << #condition << "\" ["             \
                                          << __FUNCTION__ << "][" << __FILE__ \
                                          << ':' << __LINE__ << ']');         \
    ASSERT((condition));                                                      \
  }

/*
 * Will cauch assert on debug version,
 * Will return return_value in release build
 */
#define DCHECK_OR_RETURN(condition, return_value)                             \
  if (!(condition)) {                                                         \
    CREATE_LOGGERPTR_LOCAL(logger_, "Utils");                                 \
    LOG4CXX_FATAL(logger_,                                                    \
                  "DCHECK failed with \"" << #condition << "\" ["             \
                                          << __FUNCTION__ << "][" << __FILE__ \
                                          << ':' << __LINE__ << ']');         \
    ASSERT((condition));                                                      \
    return (return_value);                                                    \
  }
/*
 * Will cauch assert on debug version,
 * Will return return_value in release build
 */
#define DCHECK_OR_RETURN_VOID(condition)                                      \
  if (!(condition)) {                                                         \
    CREATE_LOGGERPTR_LOCAL(logger_, "Utils");                                 \
    LOG4CXX_FATAL(logger_,                                                    \
                  "DCHECK failed with \"" << #condition << "\" ["             \
                                          << __FUNCTION__ << "][" << __FILE__ \
                                          << ':' << __LINE__ << ']');         \
    ASSERT((condition));                                                      \
    return;                                                                   \
  }

#define EXPORT_FUNCTION(TypeName) extern "C" TypeName* Create();

#define EXPORT_FUNCTION_IMPL(TypeName) \
  extern "C" TypeName* Create() {      \
    return new TypeName();             \
  }

#define NOTREACHED() DCHECK(!"Unreachable code")

// Allows to perform static check that virtual function from base class is
// actually being overriden if compiler support is available
#if __cplusplus >= 201103L
#define OVERRIDE override
#define FINAL final
#else
#define OVERRIDE
#define FINAL
#endif

/*
* @brief Calculate size of na array
* @param arr  array, which size need to calculate
*/
#define ARRAYSIZE(arr) sizeof(arr) / sizeof(*arr)

#ifdef BUILD_TESTS
#define FRIEND_TEST(test_case_name, test_name) \
  friend class test_case_name##_##test_name##_Test
#else  // BUILD_TESTS
#define FRIEND_TEST(test_case_name, test_name)
#endif  // BUILD_TESTS

/*
* @brief deprecate a method declaration, a warning will be thrown by your
*        compiler if a method with this macro is used
*/
#if __cplusplus > 201103L
#define DEPRECATED [[deprecated]]
#else
#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#define DEPRECATED_CLASS __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif
#endif

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_MACRO_H_
