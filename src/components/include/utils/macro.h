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
#ifndef SRC_COMPONENTS_INCLUDE_UTILS_MACRO_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_MACRO_H_

#ifdef DEBUG
#include <assert.h>
#else  // RELEASE
#include <stdio.h>
#endif
#include "utils/logger.h"

#if defined(OS_POSIX)
#include <signal.h>
#elif defined(QT_PORT)
#include <QCoreApplication>
#endif

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
#define SDL_ASSERT(condition) \
  SDL_FLUSH_LOGGER();         \
  do {                        \
    assert(condition);        \
  } while (false)
#else  // RELEASE
#define SDL_ASSERT(condition)                                    \
  fprintf(stderr,                                                \
          "Failed condition \"" #condition "\" [%s:%d][%s]\n\n", \
          __FILE__,                                              \
          __LINE__,                                              \
          __FUNCTION__)
#endif

#define DCHECK(condition)                                                     \
  if (!(condition)) {                                                         \
    SDL_CREATE_LOGGER("Utils");                                               \
    SDL_FATAL("DCHECK failed with \"" << #condition << "\" [" << __FUNCTION__ \
                                      << "][" << __FILE__ << ':' << __LINE__  \
                                      << ']');                                \
    SDL_ASSERT((condition));                                                  \
  }

/*
 * Will cauch assert on debug version,
 * Will return return_value in release build
 */
#define DCHECK_OR_RETURN(condition, return_value)                             \
  if (!(condition)) {                                                         \
    SDL_CREATE_LOGGER("Utils");                                               \
    SDL_FATAL("DCHECK failed with \"" << #condition << "\" [" << __FUNCTION__ \
                                      << "][" << __FILE__ << ':' << __LINE__  \
                                      << ']');                                \
    SDL_ASSERT((condition));                                                  \
    return (return_value);                                                    \
  }
/*
 * Will cauch assert on debug version,
 * Will return return_value in release build
 */
#define DCHECK_OR_RETURN_VOID(condition)                                      \
  if (!(condition)) {                                                         \
    SDL_CREATE_LOGGER("Utils");                                               \
    SDL_FATAL("DCHECK failed with \"" << #condition << "\" [" << __FUNCTION__ \
                                      << "][" << __FILE__ << ':' << __LINE__  \
                                      << ']');                                \
    SDL_ASSERT((condition));                                                  \
    return;                                                                   \
  }

#define NOTREACHED() DCHECK(!"Unreachable code")

#if __cplusplus >= 201103L
#define SDL_CPP11
#endif
// Allows to perform static check that virtual function from base class is
// actually being overriden if compiler support is available
#ifdef SDL_CPP11
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
#define GETARRAYSIZE(arr) sizeof(arr) / sizeof(*arr)

#if defined(OS_POSIX)
#define SDL_EXPORT extern "C"
#elif defined(OS_WINDOWS)
#define SDL_EXPORT extern "C" __declspec(dllexport)
#endif

#ifdef BUILD_TESTS
#define FRIEND_TEST(test_case_name, test_name) \
  friend class test_case_name##_##test_name##_Test
#endif

#if defined(OS_POSIX)
// Disable some system signals receiving in thread
// by blocking those signals
// (system signals processes only in the main thread)
// Mustn't block all signals!
// See "Advanced Programming in the UNIX Environment, 3rd Edition"
// (http://poincare.matf.bg.ac.rs/~ivana//courses/ps/sistemi_knjige/pomocno/apue.pdf,
// "12.8. Threads and Signals".
#define PLATFORM_INIT(argc, argv)  \
  sigset_t signal_set;             \
  sigemptyset(&signal_set);        \
  sigaddset(&signal_set, SIGINT);  \
  sigaddset(&signal_set, SIGTERM); \
  pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
#elif defined(QT_PORT)
// setupapi.dll should be loaded explicitly to avoid
// Windows 7 known issue with this library unloading.
// See: https://bugreports.qt.io/browse/QTBUG-20067
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#define PLATFORM_INIT(argc, argv)                    \
  qt_ntfs_permission_lookup++;                       \
  QLibrary("setupapi.dll").load();                   \
  QCoreApplication application(argc, argv);          \
  QThreadPool* pool = QThreadPool::globalInstance(); \
  pool->setMaxThreadCount(100)
#else
#define PLATFORM_INIT(argc, argv)
#endif

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_MACRO_H_
